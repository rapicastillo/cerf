<?php
// $Id: smugmug_video.inc,v 1.5 2010/04/05 04:47:41 winston Exp $

/**
 * @file
 * This include processes smugmug.com video files for use by emfield.module.
 */

// Include Common Smugmug Functions and Contants
module_load_include('inc', 'media_smugmug_image', 'smugmug_common');

/**
 * hook emvideo_PROVIDER_info
 * this returns information relevant to a specific 3rd party video provider
 * @return
 *   an array of strings requested by various admin and other forms
 *   'name' => the translated name of the provider
 *   'url' => the url to the main page for the provider
 *   'settings_description' => a description of the provider that will be posted in the admin settings form
 *   'supported_features' => an array of rows describing the state of certain supported features by the provider.
 *      These will be rendered in a table, with the columns being 'Feature', 'Supported', 'Notes'.
 */
function emvideo_smugmug_video_info() {
  $features = array(
    array(t('Full screen mode'), t('Yes'), t('You may customize the player to enable or disable full screen playback by user. By default user is allowed to switch to full screen playback.')),
  );
  return array(
    'provider' => 'smugmug_video',
    'name' => t('Smugmug'),
    'url' => EMFIELD_SMUGMUG_MAIN_URL,
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@smugmug_video" target="_blank">Smugmug</a>. You can learn more about its <a href="@smugmug_video_api" target="_blank">API</a> here.', array('@smugmug_video' => EMFIELD_SMUGMUG_MAIN_URL, '@smugmug_video_api' => EMFIELD_SMUGMUG_API_INFO)),
    'supported_features' => $features,
  );
}

/**
 * hook emvideo_PROVIDER_settings
 * this should return a subform to be added to the emvideo_settings() admin settings page.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['smugmug_video'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emvideo_smugmug_video_settings() {
  $form['smugmug_video']['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('Smugmug API'),
    '#description' => t('You will first need to apply for an API Key from the <a href="@smugmug_video" target="_blank">Smugmug API Keys page</a>.', array('@smugmug_video' => EMFIELD_SMUGMUG_API_APPLICATION_URL)),
    '#collapsible' => TRUE,
    '#collapsed' => (variable_get('emfield_smugmug_api_key', '') != ''),
  );
  if (function_exists('emimage_smugmug_image_settings')) {
    $form['smugmug_video']['api']['notice'] = array(
      '#type' => 'item',
      '#value' => t('Please enter your Smugmug API Key in the fieldset for Smugmug, in the Embedded Image Field field set above.'),
    );
  }
  else {
    $form['smugmug_video']['api']['emfield_smugmug_api_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Smugmug API Key'),
      '#default_value' => variable_get('emfield_smugmug_api_key', ''),
      '#description' => t('Please enter your Smugmug Developer Key here.'),
    );
    $form['smugmug_video']['api']['emfield_smugmug_api_secret'] = array(
      '#type' => 'textfield',
      '#title' => t('Smugmug API Secret'),
      '#default_value' => variable_get('emfield_smugmug_api_secret', ''),
      '#description' => t('If you have a secret for the Smugmug API, enter it here.'),
    );

  }

  return $form;
}

/**
 * hook emvideo_PROVIDER_extract
 * this is called to extract the video code from a pasted URL or embed code.
 * @param $embed
 *   an optional string with the pasted URL or embed code
 * @return
 *   either an array of regex expressions to be tested, or a string with the video code to be used
 *   if the hook tests the code itself, it should return either the string of the video code (if matched), or an empty array.
 *   otherwise, the calling function will handle testing the embed code against each regex string in the returned array.
 */
function emvideo_smugmug_video_extract($embed = '') {
  // Smugmug items (pictures or video) have an image id and an image key
  // Why does smugmug have an ID and a KEY for each item?
  // see http://blogs.smugmug.com/don/2008/02/08/big-privacy-changes-at-smugmug/
  // Long and short is the ids were too easy to guess which became a privacy issue

  // Smugmug provides a variety of links for a single video as well as multiple embed codes
  // for video.  We'll use three regex to deal with the possible links

  // Regex documentation
  // '@smugmug\.com/gallery/[^#]*#(\w+)@i' deals with...
  // http://www.smugmug.com/gallery/7930192_CWUod#555555663_g3Qcc
  // http://nickname.smugmug.com/gallery/7930192_CWUod#555555663_g3Qcc
  // http://nickname.smugmug.com/gallery/7929871_CUVKf#555555663_g3Qcc-A-LB
  // '@smugmug\.com/gallery/[^/]*/[\d]/[#]?(\w+)@i' deals with...
  // http://nickname.smugmug.com/gallery/7930158_uJtW6/1/555555663_g3Qcc
  // http://nickname.smugmug.com/gallery/7930158_uJtW6/1/#555555663_g3Qcc-A-LB
  // http://nickname.smugmug.com/gallery/8154250_5mfCp/1/555555663_g3Qcc/Medium
  // '@smugmug\.com/photos/(\w+)@i' deals with...
  // http://nickname.smugmug.com/photos/555555663_g3Qcc-L.jpg (and other variants of same from "get link" page)

  // Unfortunately it looks like embed codes won't work - no way to extract imageid and imagekey
  // I don't see this changing in near future, but will support extracting id later if embed
  // codes change

  // In all of the above it is 555555663_g3Qcc we are trying to extract

  // We return an array of regex strings so the calling function will actually do the extracting
  return array(
    '@smugmug\.com/*/[^#]*#(\w+)@i',
    '@smugmug\.com/*/[^#]*/[\d]/[#]?(\w+)@i',
    '@smugmug\.com/photos/(\w+)@i',
  );
}

function emvideo_smugmug_video_data($field, $item) {
  //Note: We capture a lot of details to allow for improved theming beyond cck formatters
  if ($item) {
    //Unlike other providers smugmug has both and id and key for each item
    $idandkey = preg_split("/_/", $item['value']);
    $imageid = $idandkey[0];
    $imagekey = $idandkey[1];
    $sid = _emfield_smugmug_getsessionid();
    $xml = emfield_smugmug_request_xml('smugmug.images.getInfo', array('SessionID' => $sid , 'ImageID' => $imageid, 'ImageKey' => $imagekey));
    //Create some versioning
    $data['emvideo_smugmug_video_data_version'] = 1;
    //Note: Caption is much less common on smugmug vs. flickr
    //Also, filename can only be retrieved by the owner
    //So we use the smugmug id+key as a default
    $data['title'] = $xml['Image']['Caption'];
    if (!$data['title']) {
      $data['title'] = $item['value'];
    }
    $data['caption'] = $xml['Image']['Caption'];
    $data['albumurl'] = $xml['Image']['Album']['URL'];
    $data['height'] = $xml['Image']['Height'];
    $data['width'] = $xml['Image']['Width'];
    $data['size'] = $xml['Image']['Size'];
    $data['lastupdated'] = $xml['Image']['LastUpdated'];
    $data['tinyurl'] = $xml['Image']['TinyURL'];
    $data['thumburl'] = $xml['Image']['ThumbURL'];
    $data['smallurl'] = $xml['Image']['SmallURL'];
    $data['mediumurl'] = $xml['Image']['MediumURL'];
    $data['largeurl'] = $xml['Image']['LargeURL'];
    $data['xlargeurl'] = $xml['Image']['XLargeURL'];
    $data['x2largeurl'] = $xml['Image']['X2LargeURL'];
    $data['x3largeurl'] = $xml['Image']['X3LargeURL'];
    $data['originalurl'] = $xml['Image']['OriginalURL'];
    //Video
    $data['duration'] = $xml['Image']['Duration'];
    $data['md5sum'] = $xml['Image']['MD5Sum'];
    $data['position'] = $xml['Image']['Position'];
    $data['serial'] = $xml['Image']['Serial'];
    $data['video320url'] = $xml['Image']['Video320URL'];
    $data['video640url'] = $xml['Image']['Video640URL'];
    $data['video960url'] = $xml['Image']['Video960URL'];
    $data['video1280url'] = $xml['Image']['Video1280URL'];
    $data['video1920url'] = $xml['Image']['Video1920URL'];

    /* PLANNED ENHANCEMENT: process keywords
     * Right now we just stick the raw keyword (tag) string into the data.
     * Smugmug DOES NOT give you an array of keywords, but a simple string
     * which would be OK if they didn't allow so many options (they allow
     * three delimiters with option to quote).  They also quietly eliminate
     * certain characters and anything that starts with a digit, but STILL
     * return the whole keyword string mess from the API.  Untill their API returns
     * keywords in a decent way (array or cleaned up string) this will be a problem
     * See http://www.dgrin.com/showthread.php?t=72264
     * The below might have worked if we got a simple csv string.
     * $keywords = preg_split("/[,;]/", $xml['Image']['Keywords']);
     * array_walk($keywords, '_emvideo_smugmug_video_trimkeywords');
     * $data['tags'][] = $keywords;
     * foreach ($keywords as $tag) {
     *   $data['tags'][] = trim($tag, '"');
     * }
     */
    $data['keywords'] = $xml['Image']['Keywords'];
    return $data;
  }
}

/**
 *  hook emvideo_PROVIDER_rss
 *
 *  This attaches a file to an RSS feed.
 */
function emvideo_smugmug_video_rss($item, $teaser = NULL) {
  if ($item['value']) {
    if ($item['data']['emvideo_smugmug_video_data_version'] >= 1) {
      $data = $item['data'];
    }
    else {
      $data = emvideo_smugmug_video_data(NULL, $item);
    }

    $file = array();
    if ($data['size'] > 0) {
      $file['filepath'] = $data['video640url'];
      $file['filesize'] = $data['size'];
      $file['filemime'] = 'application/x-shockwave-flash';
    }
    $file['thumbnail']['filepath'] = $data['mediumurl'];

    return $file;
  }
}

/**
 * hook emvideo_PROVIDER_embedded_link($code)
 * returns a link to view the content at the provider's site
 *  @param $code
 *    the string containing the content to watch
 *  @return
 *    a string containing the URL to view the video at the original provider's site
 */
function emvideo_smugmug_video_embedded_link($code, $data = array()) {
  // Use a common procedure for both image and video provider
  return _emfield_smugmug_embedded_link($code, $data = array());
}

/**
 * Theme function for embedded smugmug video.
 */
function theme_emvideo_smugmug_video_embedded_video($type, $url, $width, $height, $autoplay = FALSE, $field = NULL, $item = NULL) {
  // NOTE: Smugmug has a flashplayer embed.  They just don't provide any way to provide it with a URL, or any other way to
  // fetch it via the API.  With this limitation we must rely on other available players for now.
  // http://api.smugmug.com/services/oembed/?url=http%3A%2F%2Fblackknights.smugmug.com%2FMarching-Band%2FVideo-2010%2F10084118_DLjY6%23692056563_Vo9kM&maxwidth=800
  //$xml = module_invoke('emfield', 'request_xml', 'smugmug', EMFIELD_SMUGMUG_REST_ENDPOINT, $args, $cached, FALSE, FALSE, TRUE);
  if ($url) {
      if (module_exists('flowplayer')) { 
        $size = _emvideo_smugmug_video_guess_size($width, $item);
        $url = $item['data'][$size];
        $config = array(
          'clip' => array(
          'autoPlay' => $autoplay,
          'url' => $url,
          'autoBuffering' => '"true"',
          ),
        );
        $attributes = array(
          'style' => "width:{$width}px;height:{$height}px;",
        );
		return theme('flowplayer', $config, 'emvideo-smugmug-video-embedded-video', $attributes);
	  }
    }
    //No flowplayer, so we'll try to get it from smugmug OEMBED provider
    //We won't support autoplay with this option
    $xml = module_invoke('emfield', 'request_xml', 'smugmug', 'http://api.smugmug.com/services/oembed/', array('url' => $item['embed'], 'maxwidth' => $width), TRUE);
    $html = $xml['OEMBED']['HTML'][0];
    if ($html) {
      return $xml['OEMBED']['HTML'][0];
	}
    //So we really failed at this point, so we'll try just returning whatever 
    //the browser has
    $autostart = $autoplay ? 'true' : 'false';
    return '<embed src="'. $url .'" width="'. $width .'" height="'. $height .'" autoplay="'. $autostart .'" autobuffering="true" controller="true" type="video/quicktime" scale="tofit" pluginspage="http://www.apple.com/quicktime/download/"> </embed>';
}

/**
 * hook emvideo_PROVIDER_thumbnail
 * Returns the external url for a thumbnail of a specific video.
 *  @param $field
 *    The field of the requesting node.
 *  @param $item
 *    The actual content of the field from the requesting node.
 *  @return
 *    A URL pointing to the thumbnail.
 */
function emvideo_smugmug_video_thumbnail($field, $item, $formatter, $node, $width, $height) {
  if ($item['value']) {
    if ($item['data']['emvideo_smugmug_video_data_version'] >= 1) {
      $data = $item['data'];
    }
    else {
      $data = emvideo_smugmug_video_data(NULL, $item);
    }
    $size = _emvideo_smugmug_thumbnail_guess_size($width, $height);
    $url = $data[$size];
    return $url;
  }
}

/**
 *  hook emvideo_PROVIDER_video
 *  This actually displays the full/normal-sized video we want, usually on the
 *  default page view.
 *  @param $embed
 *    The video code for the video to embed.
 *  @param $width
 *    The width to display the video.
 *  @param $height
 *    The height to display the video.
 *  @param $field
 *    The field info from the requesting node.
 *  @param $item
 *    The actual content from the field.
 *  @return
 *    The html of the embedded video.
 */
function emvideo_smugmug_video_video($code, $width, $height, $field, $item, &$node, $autoplay) {
  $output = theme('emvideo_smugmug_video_embedded_video', $type, $code, $width, $height, $autoplay, $field, $item);
  return $output;
}

/**
 *  hook emvideo_PROVIDER_preview
 *
 *  This actually displays the preview-sized video we want, commonly for the
 *  teaser.
 *  @param $embed
 *    The video code for the video to embed.
 *  @param $width
 *    The width to display the video.
 *  @param $height
 *    The height to display the video.
 *  @param $field
 *    The field info from the requesting node.
 *  @param $item
 *    The actual content from the field.
 *  @return
 *    The html of the embedded video.
 */
function emvideo_smugmug_video_preview($code, $width, $height, $field, $item, &$node, $autoplay) {
  $output = theme('emvideo_smugmug_video_embedded_video', $type, $code, $width, $height, $autoplay, $field, $item);
  return $output;
}

/**
 * Implementation of hook_emfield_subtheme().
 */
function emvideo_smugmug_video_emfield_subtheme() {
  return array(
    'emvideo_smugmug_video_embedded_video' => array(
      'arguments' => array('type' => NULL, 'url' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => FALSE, 'field' => NULL, 'item' => NULL),
      'file' => 'providers/smugmug_video.inc',
      'path' => drupal_get_path('module', 'media_smugmug_video'),
    )
  );
}

/**
 *
 *  Supporting Functions
 *
 */

/**
 *  Helper function for emvideo_smugmug_video_image_url.
 *  This will return the appropriate array key for the image size we wish.
 */
function _emvideo_smugmug_thumbnail_guess_size($width, $height) {
  $max = max($width, $height);
  foreach (array('tinyurl' => 100, 'thumburl' => 150, 'smallurl' => 400, 'mediumurl' => 600, 'largeurl' => 800, 'xlargeurl' => 1024, 'x2largeurl' => 1280, 'x3largeurl' => 1600) as $size => $value) {
    if ($max <= $value) {
      return $size;
    }
  }
  //Higher than max then return original
  return 'OriginalURL';
}

/**
 *  Helper function for emvideo_smugmug_video_image_url.
 *  This will return the appropriate array key for the image size we wish.
 */
function _emvideo_smugmug_video_guess_size($width, $item) {
  // To avoid a problem with the size not being there, we'll check to make sure
  // we aren't asking for something larger than the original size!
  $sizetocheck = ($width > $item['data']['width']) ? $item['data']['width'] : $width;
  foreach (array('video320url' => 320, 'video640url' => 640, 'video960url' => 960, 'video1280url' => 1280, 'video1920url' => 1920) as $size => $value) {
    if ($sizetocheck <= $value) {
      return $size;
    }
  }
  //Higher than max then return common
  return 'video640url';
}
