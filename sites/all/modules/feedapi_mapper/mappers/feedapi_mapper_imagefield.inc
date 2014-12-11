<?php
// $Id$

/**
 * @file
 * Implements a FeedAPI mapper on behalf of the ImageField module.
 * @link http://drupal.org/project/imagefield
 */

/**
 * Implementation of hook_feedapi_mapper().
 */
function imagefield_feedapi_mapper($op, $node, $feed_element = array(), $field_name = '', $sub_field = '') {
  switch ($op) {
  case 'describe':
    return t('Maps image URLs to CCK ImageFields. The image will be downloaded and saved as a local file. You can choose whether to keep the original file name or use a hash value of the URL. The latter is better for images that are returned by scripts (like getimage.php?foo=), otherwise they would all be saved as "getimage.php".');

  case 'list':
    $sub_fields = array(
      'original' => t('Original file name'),
      'md5' => t('Hashed URL'),
    );
    
    $info = content_types($node->type);
    $fields = array();
    if (@count($info['fields'])) {
      foreach ($info['fields'] as $field_name => $field) {
        //drupal_set_message('<pre>'.htmlspecialchars(print_r($field, TRUE), ENT_QUOTES).'</pre>');
        if ($field['type'] == 'filefield' && $field['widget']['type'] == 'imagefield_widget') {
          $fields[$field_name] = $sub_fields;
        }
      }
    }
    //drupal_set_message('<pre>'.htmlspecialchars(print_r(array($fields), TRUE), ENT_QUOTES).'</pre>');
    if (count($fields)) {
      return $fields;
    }
    return FALSE;

  case 'map':
    //drupal_set_message('<pre>'.htmlspecialchars(print_r(array($node), TRUE), ENT_QUOTES).'</pre>');
    // Here is where the actual mapping happens.
    if (isset($node->{$field_name})) {
      $items = $node->{$field_name};
    }
    else {
      $items = array();
    }
    $field = content_fields($field_name);
    foreach ((array)$feed_element as $element) {
      //drupal_set_message('<pre>'.htmlspecialchars(print_r("element: $element", TRUE), ENT_QUOTES).'</pre>');
      $url = NULL;
      if (is_array($element)) {
        // is this an options->enclosure-media->type array
        if (valid_url($element['link'])) {
          $url = $element['link'];
        }
        elseif (valid_url($element['player'])) {
          $url = $element['player'];
        }
      }
      elseif (is_string($element) && valid_url($element)) {
        // straight link usually from options->original_url or options->guid
        $url = $element;
      }
      //drupal_set_message('<pre>'.htmlspecialchars(print_r("url: $url", TRUE), ENT_QUOTES).'</pre>');
      if ($url 
        && $file = _feedapi_mapper_imagefield_file_insert($node, $url, $sub_field, $field)) {
        // Always add as last, Image Field takes care of deleting the
        // first image for non-multiple fields. Has this been confirmed??
        $items[] = $file;
        // update the file status in the database
        field_file_save($node, $file);
        if (!$field['multiple']) {
          break;
        }
      }
    }
    $node->$field_name = $items;
    return $node;
  }
}

function _feedapi_mapper_imagefield_file_insert($node, $url, $filename_option, $field) {
  global $user;
  static $blacklist;
  static $users = array(); // user_load() cache

  if (!isset($blacklist)) {
    // The blacklist is an associative array consisting of an MD5 fingerprint
    // and keyed by file size.
    $blacklist = variable_get('imagefield_mapper_blacklist', array());
  }

  // Download the image.
  $result = drupal_http_request(html_entity_decode($url));
  //drupal_set_message('<pre>'.htmlspecialchars(print_r($result, TRUE), ENT_QUOTES).'</pre>');
  if ($result->code != 200) {
    return FALSE;
  }

  // Check for blacklisted image.
  // is there a case where Content-Length might be incorrect?
  //$filesize = strlen($result->data);
  $filesize = $result->headers['Content-Length'];
  if (isset($blacklist[$filesize]) && md5($result->data) == $blacklist[$filesize]) {
    return FALSE;
  }

  // Build the filename.
  $mime_type = $result->headers['Content-Type'];
  $filename = _feedapi_mapper_imagefield_get_filename($filename_option, $url, $mime_type);
  if (!$filename) {
    return FALSE;
  }
  
  // Write to temporary file. If a file with the specified name already exists, an alternative will be used.
  $tmpfile = file_create_filename($filename, file_directory_temp());
  $fp = fopen($tmpfile, 'w');
  if (!fp) {
    return FALSE;
  }
  $file = FALSE;
  if (fwrite($fp, $result->data)
    && fclose($fp)) {
    // detokenize & transliterate path by filefield_widget
    // doesn't work with http://drupal.org/project/filefield_paths
    $feedapi_node_user = $user;
    //drupal_set_message('<pre>'.htmlspecialchars(print_r($node->feedapi_node->feed_nids, TRUE), ENT_QUOTES).'</pre>');
    if ($node->feedapi_node->feed_nids) {
      // use feedapi_node's user to replace tokens
      $feed_nids = array_keys($node->feedapi_node->feed_nids);
      $feed_node = node_load($feed_nids[0]); // use the first one
      $feedapi_node_user = ($users[$feed_node->uid] ? $users[$feed_node->uid] : user_load($feed_node->uid));
      $users[$feed_node->uid] = $feedapi_node_user; // cache
    }
    $file_path = filefield_widget_file_path($field, $feedapi_node_user);
    
    // make sure path exists and writable
    if (file_check_directory($file_path, FILE_CREATE_DIRECTORY)
      || (mkdir($file_path, 0775, true) // recursive 
        && file_check_directory($file_path))) { // & check again
      // save file
      //drupal_set_message('<pre>'.htmlspecialchars(print_r(array('field_file_save_file: ', $filename, $tmpfile, $file_path), TRUE), ENT_QUOTES).'</pre>');
      $file = field_file_save_file($tmpfile, array(), $file_path);
      if ($file) {
        watchdog('feedapi_mapper', 'Saved feed image as %name from %url', array('%name' => $file['filepath'], '%url' => $url), WATCHDOG_INFO);
      }
    }
  }
   
  // delete the temporary file
  @unlink($tmpfile);
  
  return $file;
}

/**
 * Get local filename from image URL.
 *
 * @param $filename_option
 *   The selected filename generation option.
 * @param $url
 *   The feed URL.
 * @param &$mime_type
 *   The MIME type of the downloaded image.
 * @return
 *   The local filename or FALSE if the file was no image.
 */
function _feedapi_mapper_imagefield_get_filename($filename_option, $url, &$mime_type) {
  // Figure out the proper file extension from the MIME type.
  static $types = array(
    'image/gif' => 'gif',
    'image/jpeg' => 'jpg',
    'image/png' => 'png',
    'image/psd' => 'psd',
    'image/bmp' => 'bmp',
    'image/tiff' => 'tif',
    'image/jp2' => 'jp2',
    'image/iff' => 'iff',
    'image/vnd.wap.wbmp' => 'wbmp',
    'image/xbm' => 'xbm',
    'image/vnd.microsoft.icon' => 'ico',
  );
  if (!isset($types[$mime_type])) {
    return FALSE;
  }

  $ext = $types[$mime_type];

  // Create filename.
  switch ($filename_option) {
  case 'original':
    $components = parse_url($url);
    $filename = basename(urldecode($components['path']));
    if (!empty($filename)) {
      if (module_exists('transliteration')) {
        $filename = transliteration_get($filename);
      }
      if (strrchr($filename, '.') != '.'.$ext) {
        $filename .= '.'.$ext;
      }
      $munged = file_munge_filename($filename, implode(' ', array_values($types)));
      //drupal_set_message('<pre>'.htmlspecialchars(print_r(array($filename, $munged), TRUE), ENT_QUOTES).'</pre>');
      return $munged;
    }
    // falls through; can this occur?

  case 'md5':
    return file_munge_filename(md5($url) .'.'. $ext, implode(' ', array_values($types)));
  }
}
