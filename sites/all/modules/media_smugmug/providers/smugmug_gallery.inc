<?php
// $Id: smugmug_gallery.inc,v 1.1 2010/04/05 03:42:35 winston Exp $

/**
 * @file
 *   This include processes smugmug.com galleries (photosets) for use by emfield/emvideo.module.
 */

// Include Common Smugmug Functions and Constants
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
function emvideo_smugmug_gallery_info() {
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS Attachment'), t('No'), ''),
    array(t('Thumbnails'), t('Yes'), t('')),
    array(t('Custom player colors'), t('Yes'), t("You may customize the player's skin by choosing your own colors, and/or border in that setting field set.")),
  );
  return array(
    'provider' => 'smugmug_gallery',
    'name' => t('Smugmug gallery'),
    'url' => EMFIELD_SMUGMUG_MAIN_URL,
    'settings_description' => t('These settings specifically affect slideshows displayed from <a href="@smugmug_video" target="_blank">Smugmug</a>. You can learn more about its <a href="@smugmug_video_api" target="_blank">API</a> here.', array('@smugmug_video' => EMFIELD_SMUGMUG_MAIN_URL, '@smugmug_video_api' => EMFIELD_SMUGMUG_API_INFO)),
    'supported_features' => $features,
  );
}

/**
 * hook emvideo_PROVIDER_settings
 * this should return a subform to be added to the emvideo_settings() admin settings page.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['smugmug_gallery'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emvideo_smugmug_gallery_settings() {
  $form['smugmug_gallery']['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('Smugmug API'),
    '#description' => t('You will first need to apply for an API Key from the <a href="@smugmug_video" target="_blank">Smugmug API Keys page</a>.', array('@smugmug_video' => EMFIELD_SMUGMUG_API_APPLICATION_URL)),
    '#collapsible' => TRUE,
    '#collapsed' => (variable_get('emfield_smugmug_api_key', '') != ''),
  );
  if (function_exists('emimage_smugmug_image_settings') || function_exists('emvideo_smugmug_video_settings')) {
    $form['smugmug_gallery']['api']['notice'] = array(
      '#type' => 'item',
      '#value' => t('Please enter your Smugmug API Key in the fieldset for Smugmug, in the Embedded Image Field field set above or the Embedded Video Field field set.'),
    );
  }
  else {
    $form['smugmug_gallery']['api']['emfield_smugmug_api_key'] = array(
      '#type' => 'textfield',
      '#title' => t('Smugmug API Key'),
      '#default_value' => variable_get('emfield_smugmug_api_key', ''),
      '#description' => t('Please enter your Smugmug Developer Key here.'),
    );
    $form['smugmug_gallery']['api']['emfield_smugmug_api_secret'] = array(
      '#type' => 'textfield',
      '#title' => t('Smugmug API Secret'),
      '#default_value' => variable_get('emfield_smugmug_api_secret', ''),
      '#description' => t('If you have a secret for the Smugmug API, enter it here.'),
    );
  }
  $form['player_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Embedded video player options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
   $form['player_options']['preview'] = array(
    '#type' => 'fieldset',
    '#title' => t('Preview display'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['player_options']['preview']['emvideo_smugmug_gallery_preview_showspeed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show speed'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_preview_showspeed', 1),
    '#description' => t('Allow users to control speed of the slide show in preview display.'),
  );
  $form['player_options']['preview']['emvideo_smugmug_gallery_preview_speeddefault'] = array(
    '#type' => 'select',
    '#title' => t('Speed default'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_preview_speeddefault', 'med'),
    '#options' => array(
      'slow' => t('Slow'),
      'med' => t('Medium'),
      'fast' => t('Fast'),
    ),
    '#description' => t('Default slideshow speed.'),
  );
  $form['player_options']['preview']['emvideo_smugmug_gallery_preview_showthumbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show thumbnails'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_preview_showthumbs', 0),
    '#description' => t('Shows thumbnails across the bottom of the slideshow.'),
  );
  $form['player_options']['preview']['emvideo_smugmug_gallery_preview_showbuttons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show buttons'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_preview_showbuttons', 1),
    '#description' => t('Whether to show any play/pause/next buttons.'),
  );
  $form['player_options']['preview']['emvideo_smugmug_gallery_preview_clickto'] = array(
    '#type' => 'select',
    '#title' => t('Clicking on image goes'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_preview_clickto', '1'),
    '#options' => array(
      '0' => t('Nowhere'),
      '1' => t('Smugmug gallery (at that picture)'),
      '2' => t('Smugmug gallery (at start)'),
    ),
    '#description' => t('Where should the user go if they click an image in the slideshow.'),
  );
  $form['player_options']['preview']['emvideo_smugmug_gallery_preview_newwindow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Open in new window'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_preview_newwindow', 1),
    '#description' => t('If "clicking on image goes" is set open in new window?'),
  );
  $form['player_options']['preview']['emvideo_smugmug_gallery_preview_showlogo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show smugmug logo'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_preview_showlogo', 1),
    '#description' => t('Show the small smugmug logo in the player.'),
  );
  $form['player_options']['preview']['emvideo_smugmug_gallery_preview_captions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show picture captions'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_preview_captions', 1),
    '#description' => t('Show picture captions.'),
  );
  $form['player_options']['preview']['emvideo_smugmug_gallery_preview_crossfadespeed'] = array(
    '#type' => 'textfield',
    '#title' => t('Cross fade speed'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_preview_crossfadespeed', 350),
    '#description' => t('Cross fade speed between photos (in milliseconds).'),
  );
  $form['player_options']['preview']['emvideo_smugmug_gallery_preview_borderthickness'] = array(
    '#type' => 'textfield',
    '#title' => t('Border thickness'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_preview_borderthickness', ''),
    '#description' => t('Thickness of border around images. Must also set Border color for this to have effect.'),
  );
  $form['player_options']['preview']['emvideo_smugmug_gallery_preview_bordercolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Border color'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_preview_bordercolor', ''),
    '#description' => t('Color of border around images. Must also set Border thickness for this to have effect.'),
  );
  $form['player_options']['preview']['emvideo_smugmug_gallery_preview_bgcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_preview_bgcolor', ''),
    '#description' => t('Background color. Blank means transparent (default)'),
  );
  $form['player_options']['preview']['emvideo_smugmug_gallery_preview_imgalign'] = array(
    '#type' => 'checkbox',
    '#title' => t('Image align top'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_preview_imgalign', 0),
    '#description' => t('Align the displayed image to the top of the slideshow area.'),
  );
  $form['player_options']['video'] = array(
    '#type' => 'fieldset',
    '#title' => t('Video display'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['player_options']['video']['emvideo_smugmug_gallery_video_showspeed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show speed'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_video_showspeed', 1),
    '#description' => t('Allow users to control speed of the slide show in video display.'),
  );
  $form['player_options']['video']['emvideo_smugmug_gallery_video_speeddefault'] = array(
    '#type' => 'select',
    '#title' => t('Speed default'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_video_speeddefault', 'med'),
    '#options' => array(
      'slow' => t('Slow'),
      'med' => t('Medium'),
      'fast' => t('Fast'),
    ),
    '#description' => t('Default slideshow speed.'),
  );
  $form['player_options']['video']['emvideo_smugmug_gallery_video_showthumbs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show thumbnails'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_video_showthumbs', 0),
    '#description' => t('Shows thumbnails across the bottom of the slideshow.'),
  );
  $form['player_options']['video']['emvideo_smugmug_gallery_video_showbuttons'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show buttons'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_video_showbuttons', 1),
    '#description' => t('Whether to show any play/pause/next buttons.'),
  );
  $form['player_options']['video']['emvideo_smugmug_gallery_video_clickto'] = array(
    '#type' => 'select',
    '#title' => t('Clicking on image goes'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_video_clickto', '1'),
    '#options' => array(
      '0' => t('Nowhere'),
      '1' => t('Smugmug gallery (at that picture)'),
      '2' => t('Smugmug gallery (at start)'),
    ),
    '#description' => t('Where should the user go if they click an image in the slideshow.'),
  );
  $form['player_options']['video']['emvideo_smugmug_gallery_video_newwindow'] = array(
    '#type' => 'checkbox',
    '#title' => t('Open in new window'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_video_newwindow', 1),
    '#description' => t('If "clicking on image goes" is set open in new window?'),
  );
  $form['player_options']['video']['emvideo_smugmug_gallery_video_showlogo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show smugmug logo'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_video_showlogo', 1),
    '#description' => t('Show the small smugmug logo in the player.'),
  );
  $form['player_options']['video']['emvideo_smugmug_gallery_video_captions'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show picture captions'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_video_captions', 1),
    '#description' => t('Show picture captions.'),
  );
  $form['player_options']['video']['emvideo_smugmug_gallery_video_crossfadespeed'] = array(
    '#type' => 'textfield',
    '#title' => t('Cross fade speed'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_video_crossfadespeed', 350),
    '#description' => t('Cross fade speed between photos (in milliseconds).'),
  );
  $form['player_options']['video']['emvideo_smugmug_gallery_video_borderthickness'] = array(
    '#type' => 'textfield',
    '#title' => t('Border thickness'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_video_borderthickness', ''),
    '#description' => t('Thickness of border around images. Must also set Border color for this to have effect.'),
  );
  $form['player_options']['video']['emvideo_smugmug_gallery_video_bordercolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Border color'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_video_bordercolor', ''),
    '#description' => t('Color of border around images. Must also set Border thickness for this to have effect.'),
  );
  $form['player_options']['video']['emvideo_smugmug_gallery_video_bgcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_video_bgcolor', ''),
    '#description' => t('Background color. Blank means transparent (default)'),
  );
  $form['player_options']['video']['emvideo_smugmug_gallery_video_imgalign'] = array(
    '#type' => 'checkbox',
    '#title' => t('Image align top'),
    '#default_value' => variable_get('emvideo_smugmug_gallery_video_imgalign', 0),
    '#description' => t('Align the displayed image to the top of the slideshow area.'),
  );
 
  //Use color picker module for colors if available
  if (module_exists('colorpicker')) {
    $form['player_options']['preview']['emvideo_smugmug_gallery_preview_bordercolor']['#type'] = 'colorpicker_textfield';
    $form['player_options']['preview']['emvideo_smugmug_gallery_preview_bordercolorpicker']['#colorpicker'] = 'previewbordercolorpicker';
    $form['player_options']['preview']['previewcolorpicker'] = array(
      '#type' => 'colorpicker',
      '#title' => t('Background color picker'),
      '#description' => t('Click in this textfield to start picking your color'),
    );
    $form['player_options']['preview']['emvideo_smugmug_gallery_preview_bgcolor']['#type'] = 'colorpicker_textfield';
    $form['player_options']['preview']['emvideo_smugmug_gallery_preview_bgcolorpicker']['#colorpicker'] = 'previewbgcolorpicker';
    $form['player_options']['preview']['previewbgcolorpicker'] = array(
      '#type' => 'colorpicker',
      '#title' => t('Background color picker'),
      '#description' => t('Click in this textfield to start picking your color'),
    );
    $form['player_options']['video']['emvideo_smugmug_gallery_video_bordercolor']['#type'] = 'colorpicker_textfield';
    $form['player_options']['video']['emvideo_smugmug_gallery_video_bordercolorpicker']['#colorpicker'] = 'videobordercolorpicker';
    $form['player_options']['video']['videobordercolorpicker'] = array(
      '#type' => 'colorpicker',
      '#title' => t('Background color picker'),
      '#description' => t('Click in this textfield to start picking your color'),
    );
    $form['player_options']['video']['emvideo_smugmug_gallery_video_bgcolor']['#type'] = 'colorpicker_textfield';
    $form['player_options']['video']['emvideo_smugmug_gallery_video_bgcolorpicker']['#colorpicker'] = 'videobgcolorpicker';
    $form['player_options']['video']['videobgcolorpicker'] = array(
      '#type' => 'colorpicker',
      '#title' => t('Background color picker'),
      '#description' => t('Click in this textfield to start picking your color'),
    );
  }
  else {
    $form['player_options']['preview']['emvideo_smugmug_gallery_preview_bordercolor']['#description'] .= '<br /> '. t('The <a href="@colorpicker">Colorpicker module</a>, if active, gives an easy way to select this color.', array('@colorpicker' => 'http://drupal.org/project/colorpicker'));
    $form['player_options']['video']['emvideo_smugmug_gallery_video_bordercolor']['#description'] .= '<br /> '. t('The <a href="@colorpicker">Colorpicker module</a>, if active, gives an easy way to select this color.', array('@colorpicker' => 'http://drupal.org/project/colorpicker'));
     $form['player_options']['preview']['emvideo_smugmug_gallery_preview_bgcolor']['#description'] .= '<br /> '. t('The <a href="@colorpicker">Colorpicker module</a>, if active, gives an easy way to select this color.', array('@colorpicker' => 'http://drupal.org/project/colorpicker'));
    $form['player_options']['video']['emvideo_smugmug_gallery_video_bgcolor']['#description'] .= '<br /> '. t('The <a href="@colorpicker">Colorpicker module</a>, if active, gives an easy way to select this color.', array('@colorpicker' => 'http://drupal.org/project/colorpicker'));
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
function emvideo_smugmug_gallery_extract($embed = '') {
  // Smugmug items (pictures or video) have an image id and an image key
  // Why does smugmug have an ID and a KEY for each item?
  // see http://blogs.smugmug.com/don/2008/02/08/big-privacy-changes-at-smugmug/
  // Long and short is the ids were too easy to guess which became a privacy issue

  // Smugmug provides a variety of links for a single video as well as multiple embed codes
  // for video.  We'll use three regex to deal with the possible links

  // Regex documentation
  // '@smugmug\.com/*/[^#]*#(\w+)@i' deals with...
  // http://www.smugmug.com/gallery/7930192_CWUod#555555663_g3Qcc
  // http://nickname.smugmug.com/gallery/7930192_CWUod#555555663_g3Qcc
  // http://nickname.smugmug.com/gallery/7929871_CUVKf#555555663_g3Qcc-A-LB
  // http://nickname.smugmug.com/category/nicename/7930192_CWUod#555555663_g3Qcc
  // http://nickname.smugmug.com/category/subcategory/nicename/7930192_CWUod#555555663_g3Qcc
  // '@smugmug\.com/gallery/[^/]*/[\d]/[#]?(\w+)@i' deals with...
  // http://nickname.smugmug.com/gallery/7930158_uJtW6/1/555555663_g3Qcc
  // http://nickname.smugmug.com/gallery/7930158_uJtW6/1/#555555663_g3Qcc-A-LB
  // http://nickname.smugmug.com/gallery/8154250_5mfCp/1/555555663_g3Qcc/Medium
  // http://nickname.smugmug.com/category/nicename/7930192_CWUod/1/555555663_g3Qcc (etc)
  // '@smugmug\.com/photos/(\w+)@i' deals with...
  // http://nickname.smugmug.com/photos/555555663_g3Qcc-L.jpg (and other variants of same from "get link")

  // In all of the above it is 555555663_g3Qcc we are trying to extract

  // Album ids only regex documentation
  // http://nickname.smugmug.com/hack/feed.mg?Type=gallery&Data=5555555_XCA3S&format=rss200

  // We'll first do our own test on a url that the default emfield extract won't handle
  // This is because the album id and key are separated in this URL so no way to return them
  // as one match in the regex (or at least I don't know a way)
  // Specifically we're trying to match this link which is a slideshow link:
  // http://username.smugmug.com/photos/swfpopup.mg?AlbumID=5555555&AlbumKey=g3Qcc
  $regex = '@smugmug.com.*ID=(\w{7}).*Key=(\w{5})@';
  $matches = NULL;
  if (preg_match($regex, $embed, $matches)) {
    return $matches[1] . '_' . $matches[2];
  }

  // For most links, we return an array of regex strings so the calling function will actually do the extracting
  return array(
  // First three extract image ids where possible (we can get the gallery from that later)
    '@smugmug\.com/*/[^#]*#(\w+)@i',
    '@smugmug\.com/*/[^#]*/[\d]/[#]?(\w+)@i',
    '@smugmug\.com/photos/(\w+)@i',
  // Otherwise we'll try to get the album ids (from which we may be able to extract a featured image)
  // RSS, ATOM, GOOGLE, and BOXEE links
    '@smugmug.*=(\w{7}_\w{5})@i',
  );
}

function emvideo_smugmug_gallery_data($field, $item) {
  //Note: We capture a lot of details to allow for improved theming beyond cck formatters
  if ($item) {
    //Create some versioning
    $data['emvideo_smugmug_gallery_data_version'] = 1;
 
    //Unlike other providers smugmug has both an id and key for each item
    //Get the id/key combo we've been provided by the user
    $idandkey = preg_split("/_/", $item['value']);
    $id = $idandkey[0];
    $key = $idandkey[1];
    //We'll need a session for api calls
    $sid = _emfield_smugmug_getsessionid();
    //We'll first try on the assumption we have an image id/key
    $xml = emfield_smugmug_request_xml('smugmug.images.getInfo', array('SessionID' => $sid , 'ImageID' => $id, 'ImageKey' => $key));
    if ($xml['stat'] == 'fail') {
      //Maybe it's an album so get associated album xml
      $xml = emfield_smugmug_request_xml('smugmug.albums.getInfo', array('SessionID' => $sid , 'AlbumID' => $id, 'AlbumKey' => $key));
      $album['id'] = $xml['Album']['id'];
      $album['key'] = $xml['Album']['Key'];
      $album['xml'] = $xml;
      $xml = emfield_smugmug_request_xml('smugmug.images.get', array('SessionID' => $sid , 'AlbumID' => $id, 'AlbumKey' => $key));
      if ($xml['Album']['ImageCount'] > 0) {
        //Now get first image info as an alternate
        $photoembed['id'] = $xml['Album']['Images'][0]['id'];
        $photoembed['key'] = $xml['Album']['Images'][0]['Key'];
        $xml = emfield_smugmug_request_xml('smugmug.images.getInfo', array('SessionID' => $sid , 'ImageID' => $photoembed['id'], 'ImageKey' => $photoembed['key']));
        $photoembed['xml'] = $xml;
        //And get highlighted photo if available
        if ($album['xml']['Album']['Highlight']) {
          $photohighlight['id'] = $album['xml']['Album']['Highlight']['id'];
          $photohighlight['key'] = $album['xml']['Album']['Highlight']['Key'];
          $xml = emfield_smugmug_request_xml('smugmug.images.getInfo', array('SessionID' => $sid , 'ImageID' => $photohighlight['id'], 'ImageKey' => $photohighlight['key']));
          $photohighlight['xml'] = $xml;
        } 
      }
    }
    else {
      //It's a photo so record xml for later
      $photoembed['id'] = $xml['Image']['id'];
      $photoembed['key'] = $xml['Image']['Key'];
      $photoembed['xml'] = $xml;
      //Now get associated album info
      $album['id'] = $xml['Image']['Album']['id'];
      $album['key'] = $xml['Image']['Album']['Key'];
      $xml = emfield_smugmug_request_xml('smugmug.albums.getInfo', array('SessionID' => $sid , 'AlbumID' => $album['id'], 'AlbumKey' => $album['key']));
      $album['xml'] = $xml;
      //Finally get highlighted photo if any
      if ($album['xml']['Album']['Highlight']) {
        $photohighlight['id'] = $album['xml']['Album']['Highlight']['id'];
        $photohighlight['key'] = $album['xml']['Album']['Highlight']['Key'];
        $xml = emfield_smugmug_request_xml('smugmug.images.getInfo', array('SessionID' => $sid , 'ImageID' => $photohighlight['id'], 'ImageKey' => $photohighlight['key']));
        $photohighlight['xml'] = $xml;
      } 
    }

    //Record Album Data
    $data['album']['id'] = $album['xml']['Album']['id'];
    $data['album']['key'] = $album['xml']['Album']['Key'];
    if ($album['xml']['Album']['Category']) {
      $data['album']['category']['id'] = $album['xml']['Album']['Category']['id'];
      $data['album']['category']['name'] = $album['xml']['Album']['Category']['Name'];
      if ($album['xml']['Album']['SubCategory']) {
        $data['album']['subcategory']['id'] = $album['xml']['Album']['SubCategory']['id'];
        $data['album']['subcategory']['name'] = $album['xml']['Album']['SubCategory']['Name'];
      }
    }
    $data['album']['description'] = $album['xml']['Album']['Description'];
    $data['album']['imagecount'] = $album['xml']['Album']['ImageCount'];
    $data['album']['keywords'] = $album['xml']['Album']['Keywords'];
    $data['album']['lastupdated'] = $album['xml']['Album']['LastUpdated'];
    $data['album']['position'] = $album['xml']['Album']['Position'];
    $data['album']['printable'] = $album['xml']['Album']['Printable'];
    $data['album']['title'] = $album['xml']['Album']['Title'];
    $data['album']['id'] = $album['xml']['Album']['id'];

    //Note: Caption is much less common on smugmug vs. flickr
    //Also, filename can only be retrieved by the owner
    //So we use the smugmug id+key as a default
    //Get the photoembed info (either from the supplied link or the first photo in the set
    $data['photoembed']['id'] = $photoembed['xml']['Image']['Album']['id'];
    $data['photoembed']['key'] = $photoembed['xml']['Image']['Album']['Key'];
    $data['photoembed']['title'] = $photoembed['xml']['Image']['Caption'];
    if (!$data['photoembed']['title']) {
      $data['photoembed']['title'] = $photoembed['id'] . '_' . $photoembed['key'];
    }
    $data['photoembed']['caption'] = $photoembed['xml']['Image']['Caption'];
    $data['photoembed']['albumurl'] = $photoembed['xml']['Image']['Album']['URL'];
    $data['photoembed']['height'] = $photoembed['xml']['Image']['Height'];
    $data['photoembed']['width'] = $photoembed['xml']['Image']['Width'];
    $data['photoembed']['size'] = $photoembed['xml']['Image']['Size'];
    $data['photoembed']['lastupdated'] = $photoembed['xml']['Image']['LastUpdated'];
    $data['photoembed']['tinyurl'] = $photoembed['xml']['Image']['TinyURL'];
    $data['photoembed']['thumburl'] = $photoembed['xml']['Image']['ThumbURL'];
    $data['photoembed']['smallurl'] = $photoembed['xml']['Image']['SmallURL'];
    $data['photoembed']['mediumurl'] = $photoembed['xml']['Image']['MediumURL'];
    $data['photoembed']['largeurl'] = $photoembed['xml']['Image']['LargeURL'];
    $data['photoembed']['xlargeurl'] = $photoembed['xml']['Image']['XLargeURL'];
    $data['photoembed']['x2largeurl'] = $photoembed['xml']['Image']['X2LargeURL'];
    $data['photoembed']['x3largeurl'] = $photoembed['xml']['Image']['X3LargeURL'];
    $data['photoembed']['originalurl'] = $photoembed['xml']['Image']['OriginalURL'];
    //Video
    $data['photoembed']['duration'] = $photoembed['xml']['Image']['Duration'];
    $data['photoembed']['md5sum'] = $photoembed['xml']['Image']['MD5Sum'];
    $data['photoembed']['position'] = $photoembed['xml']['Image']['Position'];
    $data['photoembed']['serial'] = $photoembed['xml']['Image']['Serial'];
    $data['photoembed']['video320url'] = $photoembed['xml']['Image']['Video320URL'];
    $data['photoembed']['video640url'] = $photoembed['xml']['Image']['Video640URL'];
    $data['photoembed']['video960url'] = $photoembed['xml']['Image']['Video960URL'];
    $data['photoembed']['video1280url'] = $photoembed['xml']['Image']['Video1280URL'];
    $data['photoembed']['video1920url'] = $photoembed['xml']['Image']['Video1920URL'];

    //Record highlight photo info if available
    if ($photohighlight) {
      $data['photohighlight']['id'] = $photohighlight['xml']['Image']['Album']['id'];
      $data['photohighlight']['key'] = $photohighlight['xml']['Image']['Album']['Key'];
      $data['photohighlight']['title'] = $photohighlight['xml']['Image']['Caption'];
      if (!$data['photohighlight']['title']) {
        $data['photohighlight']['title'] = $photohighlight['id'] . '_' . $photohighlight['key'];
      }
      $data['photohighlight']['caption'] = $photohighlight['xml']['Image']['Caption'];
      $data['photohighlight']['albumurl'] = $photohighlight['xml']['Image']['Album']['URL'];
      $data['photohighlight']['height'] = $photohighlight['xml']['Image']['Height'];
      $data['photohighlight']['width'] = $photohighlight['xml']['Image']['Width'];
      $data['photohighlight']['size'] = $photohighlight['xml']['Image']['Size'];
      $data['photohighlight']['lastupdated'] = $photohighlight['xml']['Image']['LastUpdated'];
      $data['photohighlight']['tinyurl'] = $photohighlight['xml']['Image']['TinyURL'];
      $data['photohighlight']['thumburl'] = $photohighlight['xml']['Image']['ThumbURL'];
      $data['photohighlight']['smallurl'] = $photohighlight['xml']['Image']['SmallURL'];
      $data['photohighlight']['mediumurl'] = $photohighlight['xml']['Image']['MediumURL'];
      $data['photohighlight']['largeurl'] = $photohighlight['xml']['Image']['LargeURL'];
      $data['photohighlight']['xlargeurl'] = $photohighlight['xml']['Image']['XLargeURL'];
      $data['photohighlight']['x2largeurl'] = $photohighlight['xml']['Image']['X2LargeURL'];
      $data['photohighlight']['x3largeurl'] = $photohighlight['xml']['Image']['X3LargeURL'];
      $data['photohighlight']['originalurl'] = $photohighlight['xml']['Image']['OriginalURL'];
      //Video
      $data['photohighlight']['duration'] = $photohighlight['xml']['Image']['Duration'];
      $data['photohighlight']['md5sum'] = $photohighlight['xml']['Image']['MD5Sum'];
      $data['photohighlight']['position'] = $photohighlight['xml']['Image']['Position'];
      $data['photohighlight']['serial'] = $photohighlight['xml']['Image']['Serial'];
      $data['photohighlight']['video320url'] = $photohighlight['xml']['Image']['Video320URL'];
      $data['photohighlight']['video640url'] = $photohighlight['xml']['Image']['Video640URL'];
      $data['photohighlight']['video960url'] = $photohighlight['xml']['Image']['Video960URL'];
      $data['photohighlight']['video1280url'] = $photohighlight['xml']['Image']['Video1280URL'];
      $data['photohighlight']['video1920url'] = $photohighlight['xml']['Image']['Video1920URL'];
    }

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
    return $data;
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
function emvideo_smugmug_gallery_embedded_link($code, $data = array()) {
  // Use a common procedure for both image and video provider
  return _emfield_smugmug_embedded_link($code, $data = array());
}

/**
 * Theme function for embedded smugmug video.
 */
function theme_emvideo_smugmug_gallery_embedded_video($width, $height, $autoplay = FALSE, $field = NULL, $item = NULL, $options = NULL) {
  // NOTE: We'll default to the fairly well documented smugmug gallery flash player
  // http://wiki.smugmug.net/display/SmugMug/Flash+Slideshow
  $autostart = $autoplay ? 'true' : 'false';
  $showspeed = $options['showspeed'];
  $speeddefault = $options['speeddefault'];
  $showthumbs = $options['showthumbs'];
  $showstartbutton = $options['showstartbutton'];
  $clickurl = $options['clickurl'];
  $clicktoimage = $options['clicktoimage'];
  $crossfadespeed = $options['crossfadespeed'];
  $showlogo = $options['showlogo'];
  $captions = $options['captions'];
  $borderthickness = $options['borderthickness'];
  $bordercolor = $options['bordercolor'];
  $bgcolor = $options['bgcolor'];
  $transparent = $options['transparent'];
  $newwindow = $options['newwindow'];
  $imgalign = $options['imgalign'];
  if ($item) {
    $output .= <<<FLASH
      <object align="middle" height="{$height}" width="{$width}">
        <param name="movie" value="http://www.smugmug.com/ria/ShizamSlides-2007090601.swf">
        <param name="flashvars" value="AlbumID={$item['data']['album']['id']}&AlbumKey={$item['data']['album']['key']}&transparent={$transparent}&autoStart={$autostart}&showSpeed={$showspeed}&setSpeed={$speeddefault}&showThumbs={$showthumbs}&showButtons={$showbuttons}&clickUrl={$clickurl}&clickToImage={$clicktoimage}&newWindow={$newwindow}&crossFadeSpeed={$crossfadespeed}&showLogo={$showlogo}&captions={$captions}&borderThickness={$borderthickness}&borderColor={$bordercolor}&bgColor={$bgcolor}&imgAlign={$imgalign}">
        <param name="wmode" value="transparent">
        <param name="bgcolor" value="000000">
        <param name="allowNetworking" value="all">
        <param name="allowScriptAccess" value="always">
        <embed 
          src="http://www.smugmug.com/ria/ShizamSlides-2007090601.swf"
          flashvars="AlbumID={$item['data']['album']['id']}&AlbumKey={$item['data']['album']['key']}&transparent={$transparent}&autoStart={$autostart}&showSpeed={$showspeed}&setSpeed={$speeddefault}&showThumbs={$showthumbs}&showButtons={$showbuttons}&clickUrl={$clickurl}&clickToImage={$clicktoimage}&newWindow={$newwindow}&crossFadeSpeed={$crossfadespeed}&showLogo={$showlogo}&captions={$captions}&borderThickness={$borderthickness}&borderColor={$bordercolor}&bgColor={$bgcolor}&imgAlign={$imgalign}"
          wmode="transparent"
          type="application/x-shockwave-flash" 
          allowScriptAccess="always" 
          allowNetworking="all" 
          height="{$height}" 
          width="{$width}">
        </embed>
      </object>
FLASH;
  }
  return $output;
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
function emvideo_smugmug_gallery_thumbnail($field, $item, $formatter, $node, $width, $height) {
  if ($item) {  
    // Try first for highlight photo, otherwise embed
    if ($item['data']['photohighlight']) {
      $photo = $item['data']['photohighlight'];
    }
    else {
      $photo = $item['data']['photoembed'];
    }

    $size = _emvideo_smugmug_gallery_guess_size($width, $height, $photo);
    $url = $photo[$size];
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
function emvideo_smugmug_gallery_video($code, $width, $height, $field, $item, &$node, $autoplay) {
  static $options;
  if (!$options) {
    $options = array();
    $options['showspeed'] = variable_get('emvideo_smugmug_gallery_video_showspeed', 1) ? 'true' : 'false';
    $options['speeddefault'] = variable_get('emvideo_smugmug_gallery_video_speeddefault', 'med');
    $options['showthumbs'] = variable_get('emvideo_smugmug_gallery_video_showthumbs', 1) ? 'true' : 'false';
    $options['showbuttons'] = variable_get('emvideo_smugmug_gallery_video_showbuttons', 1) ? 'true' : 'false';
    $options['crossfadespeed'] = variable_get('emvideo_smugmug_gallery_video_crossfadespeed', 350);
    $options['showlogo'] = variable_get('emvideo_smugmug_gallery_video_showlogo', 1) ? 'true' : 'false';
    $options['captions'] = variable_get('emvideo_smugmug_gallery_video_captions', 0) ? 'true' : 'false';
    $options['borderthickness'] = variable_get('emvideo_smugmug_gallery_video_borderthickness', '');
    $options['imgalign'] = variable_get('emvideo_smugmug_gallery_video_imgalign', 0) ? 'top' : '';
    $bordercolor = variable_get('emvideo_smugmug_gallery_video_bordercolor', '');
    if ($bordercolor) {
      if ($bordercolor[0] = '#') {
        $options['bordercolor']= substr($bordercolor, 1);
      }
    }
    else {
      $options['bordercolor'] = '';
    }
    $bgcolor = variable_get('emvideo_smugmug_gallery_video_bgcolor', '');
    if ($bgcolor) {
      if ($bgcolor[0] = '#') {
        $options['bgcolor'] = substr($bgcolor, 1);
        $options['transparent'] = 'false';
      }
    }
    else {
      $options['bgcolor'] = '';
      $options['transparent'] = 'true';
    }
    $clickto  = variable_get('emvideo_smugmug_gallery_video_clickto', 1);
    switch ($clickto) {
      case 0:
        //Go Nowhere on click
        $options['clickurl'] = 'false';
        $options['clicktoimage'] = 'false';
        break;
      case 1:
        //Go to image displayed in gallery
        $options['clickurl'] = 'false';
        $options['clicktoimage'] = 'true';
        break;
      case 2:
        //Go to first image in gallery
        $options['clickurl'] = $item['data']['albumurl'];
        $options['clicktoimage'] = 'false';
        break;
      }
    $options['newwindow'] = variable_get('emvideo_smugmug_gallery_video_newwindow', 1) ? 'true': 'false';
  }
  $output = theme('emvideo_smugmug_gallery_embedded_video', $width, $height, $autoplay, $field, $item, $options);
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
function emvideo_smugmug_gallery_preview($code, $width, $height, $field, $item, &$node, $autoplay) {
  static $options;
  if (!$options) {
    $options = array();
    $options['showspeed'] = variable_get('emvideo_smugmug_gallery_preview_showspeed', 1) ? 'true' : 'false';  
    $options['speeddefault'] = variable_get('emvideo_smugmug_gallery_preview_speeddefault', 'med');
    $options['showthumbs'] = variable_get('emvideo_smugmug_gallery_preview_showthumbs', 0) ? 'true' : 'false';
    $options['showbuttons'] = variable_get('emvideo_smugmug_gallery_preview_showbuttons', 1) ? 'true' : 'false';
    $options['crossfadespeed'] = variable_get('emvideo_smugmug_gallery_preview_crossfadespeed', 350);
    $options['showlogo'] = variable_get('emvideo_smugmug_gallery_preview_showlogo', 1) ? 'true' : 'false';
    $options['captions'] = variable_get('emvideo_smugmug_gallery_preview_captions', 0) ? 'true' : 'false';
    $options['borderthickness'] = variable_get('emvideo_smugmug_gallery_preview_borderthickness', '');
    $options['imgalign'] = variable_get('emvideo_smugmug_gallery_preview_imgalign', 0) ? 'top' : '';
    $bordercolor = variable_get('emvideo_smugmug_gallery_preview_bordercolor', '');
    if ($bordercolor) {
      if ($bordercolor[0] = '#') {
        $options['bordercolor']= substr($bordercolor, 1);
      }
    }
    else {
      $options['bordercolor'] = '';
    }
    $bgcolor = variable_get('empreview_smugmug_gallery_preview_bgcolor', '');
    if ($bgcolor) {
      if ($bgcolor[0] = '#') {
        $options['bgcolor']= substr($bgcolor, 1);
        $options['transparent'] = 'false';
      }
    }
    else {
      $options['bgcolor'] = '';
      $options['transparent'] = 'true';
    }
    $clickto  = variable_get('empreview_smugmug_gallery_preview_clickto', 1);
    switch ($clickto) {
      case 0:
        //Go Nowhere on click
        $options['clickurl'] = 'false';
        $options['clicktoimage'] = 'false';
        break;
      case 1:
        //Go to image displayed in gallery
        $options['clickurl'] = 'false';
        $options['clicktoimage'] = 'true';
        break;
      case 2:
        //Go to first image in gallery
        $options['clickurl'] = $item['data']['albumurl'];
        $options['clicktoimage'] = 'false';
        break;
      }
    $options['newwindow'] = variable_get('emvideo_smugmug_gallery_preview_newwindow', 1) ? 'true': 'false';
  }
  $output = theme('emvideo_smugmug_gallery_embedded_video', $width, $height, $autoplay, $field, $item, $options);
  return $output;
}

/**
 * Implementation of hook_emfield_subtheme().
 */
function emvideo_smugmug_gallery_emfield_subtheme() {
  return array(
    'emvideo_smugmug_gallery_embedded_video' => array(
      'arguments' => array('width' => NULL, 'height' => NULL, 'autoplay' => FALSE, 'field' => NULL, 'item' => NULL, 'options' => NULL),
      'file' => 'providers/smugmug_gallery.inc',
      'path' => drupal_get_path('module', 'media_smugmug_gallery'),
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
function _emvideo_smugmug_gallery_thumbnail_guess_size($width, $height) {
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
function _emvideo_smugmug_gallery_guess_size($width, $height, $photo) {
  // See smugmug_image.inc for details

  // If height not provided calculate one based on the original image aspect ratio
  if (!($height>0)) {
    $height = (int)ceil(($width * $photo['height']) / $photo['width']);
  }
  // If width not provided calculate one based on the original image aspect ratio
  if (!($width>0)) {
    $width = (int)ceil(($height * $photo['width']) /$photo['height']);
  }
  // We'll be testing both width and height so set appropriate arrays
  $arsizes['order'] = array('tinyurl', 'thumburl', 'smallurl', 'mediumurl', 'largeurl', 'xlargeurl', 'x2largeurl', 'originalurl');
  $arsizes['widths'] = array('x3largeurl' => 1600, 'x2largeurl' => 1280, 'xlargeurl' => 1024, 'largeurl' => 800, 'mediumurl' => 600, 'smallurl' => 400, 'thumburl' => 150, 'tinyurl' => 100);
  $arsizes['heights'] = array('x3largeurl' => 1200, 'x2largeurl' => 900, 'xlargeurl' => 768, 'largeurl' => 600, 'mediumurl' => 450, 'smallurl' => 300, 'thumburl' => 150, 'tinyurl' => 100);

  // Find a candidate for width size
  $wcandidate = 'originalurl';
  foreach ($arsizes['widths'] as $wsize => $value) {
    if ($width <= $value) {
      $wcandidate = $wsize;
    }
  }
  // Find a candidate for height size
  $hcandidate = 'originalurl';
  foreach ($arsizes['heights'] as $hsize => $value) {
    if ($height <= $value) {
      $hcandidate = $hsize;
    }
  }

  // Get the indexes
  $hindex = array_search($hcandidate, $arsizes['order']);
  $windex = array_search($wcandidate, $arsizes['order']);
  //Pick the larger index to get an image that is big enough not to be upscaled
  $size = ($hindex > $windex) ? $arsizes['order'][$hindex] : $arsizes['order'][$windex];
  return $size;
}
