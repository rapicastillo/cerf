<?php
// $Id$
/**
 * @file
 * File
 *
 * This content type lets you add a file to a panel pane.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'key' => 'file',
  'single' => TRUE,
  'title' => t('New file'),
  'icon' => 'icon_file.png',
  'description' => t('Create a file.'),
  'top level' => TRUE,
  'category' => t('Custom'),
  'render callback' => 'ctoolscustomplugins_file_content_type_render',
  'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
  'all contexts' => TRUE,
  'defaults' => array(
    'admin_title' => '',
    'title' => '',
    'file_upload' => '',
    'file_title' => '',
    'file_description' => '',
    'file_css' => '',
  ),
);

/**
 * Output function for the 'file'
 * It goes through the theme_panel_file() function
 *
 * @see theme_panel_file
 */
function ctoolscustomplugins_file_content_type_render($subtype, $conf, $args, $contexts) {
  static $delta = 0;

  $block = new stdClass();
  $block->subtype = ++$delta;
  $block->title = filter_xss_admin($conf['title']);
  $block->content = '';
  $block->content .= theme_panel_file(filter_xss_admin($conf['file_title']), $conf['file'], $conf['file_css'], $conf['file_description']);
  
  return $block;
}

/**
 * Callback to provide administrative info.
 * We render the file in the administrative pane.
 * 
 * @see theme_panel_file
 */
function ctoolscustomplugins_file_content_type_admin_info($subtype, $conf) {
  $block = new stdClass();
  $block->title = filter_xss_admin($conf['title']);
  $block->content = '';
  $block->content .= theme_panel_file(filter_xss_admin($conf['file_title']), $conf['file'], $conf['file_css'], $conf['file_description']);
  
  return $block;
}

/**
 * Callback to provide the administrative title
 */
function ctoolscustomplugins_file_content_type_admin_title($subtype, $conf) {
  $output = t('File');
  
  if(empty($conf['admin_title'])) {
    if(!empty($conf['title'])) {
      // If the 'title' is not empty then use the title as an administrative title
      $title = $conf['title'];
    }
    else {
      // If there is no 'admin title' and no 'title' then use the 'file title'
      $title = $conf['file_title'];
    }
  }
  else {
    // If we have an admin title, use it.
    $title = $conf['admin_title'];
  }
  
  if ($title) {
    $output = t('File: @title', array('@title' => $title));
  }
  
  return $output;
}

/**
 * Add/edit form for the pane content
 */
function ctoolscustomplugins_file_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];
  $form['admin_title'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($conf['admin_title']) ? $conf['admin_title'] : '',
    '#title' => t('Administrative title'),
    '#description' => t('This title will be used administratively to identify this pane. If blank, the regular title will be used. If the title is also blank, the File title will be used.'),
  );
  
  $form['title'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['title'],
    '#title' => t('Title'),
    '#description' => t("This will be used as the Pane title."),
  );
  
  $form['#attributes']['enctype'] = 'multipart/form-data';
  
  $form['file'] = array(
    '#prefix' => '<div class="file-field-wrapper">',
    '#suffix' => '</div>',
  );
  $form['file']['preview'] = array(
    '#type' => 'item',
    '#after_build' => array('ctoolscustomplugins_file_preview'),
  );
  $form['file']['file_upload'] = array(
    '#title' => t('Upload file'),
    '#type'  => 'file',
  );
  $form['file']['file_title'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['file_title'],
    '#title' => t('File title'),
    '#description' => t("This will be the title that replaces the filename. If empty, the filename will be used"),
  );
  $form['file']['file_description'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['file_description'],
    '#title' => t("File description"),
    '#description' => t("The file description"),
  );
  $form['file']['file_css'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['file_css'],
    '#title' => t("CSS Class"),
    '#description' => t("CSS class to apply to this file. This may be blank."),
  );
  
  $form['#validate'] = array('ctoolscustomplugins_file_content_type_edit_form_submit_validate');

  return $form;
}

/**
 * Validate the submitted data before closing the popup
 */
function ctoolscustomplugins_file_content_type_edit_form_submit_validate(&$form, &$form_state) {
  $ctoolscustomplugins_upload_extensions_default = variable_get('ctoolscustomplugins_upload_extensions_default', 'jpg jpeg gif png txt doc xls pdf ppt pps odt ods odp');
  $ctoolscustomplugins_upload_uploadsize_default = variable_get('ctoolscustomplugins_upload_uploadsize_default', 5) * 1024 * 1024;
  
  $validators = array(
    'file_validate_size' => array($ctoolscustomplugins_upload_uploadsize_default),
    'file_validate_extensions' => array($ctoolscustomplugins_upload_extensions_default),
  );
  
  if (user_access('upload files in panels') && ($file = file_save_upload('file_upload', $validators, file_directory_path(), FILE_EXISTS_RENAME))) {
    $file->description = $file->filename;
    $file->weight = 0;
    $file->new = TRUE;
    $form_state['conf']['file'] = (array)$file;
    file_set_status($file, FILE_STATUS_PERMANENT);
  }  
}

/**
 * The submit form stores the data in $conf
 * Ctools will do the rest
 */
function ctoolscustomplugins_file_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

/**
 * If we already have a file, display it using theme_panel_file_preview()
 *
 * @see theme_panel_file_preview
 *
 * @return
 * The file preview if it exists
 */
function ctoolscustomplugins_file_preview($form, &$form_state) {
  if ($form_state['conf']['file']) {
    $file = (object)($form_state['conf']['file']);
    $form['#title'] = t("File details");
    $form['#value'] = theme_panel_file_preview($file);
    $form['#description'] = t("If you want to remove this file just upload a new one");
  }
  return $form;
}