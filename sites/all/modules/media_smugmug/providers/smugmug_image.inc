<?php
// $Id: smugmug_image.inc,v 1.3 2010/04/05 03:42:35 winston Exp $

/**
 * @file
 * This include processes smugmug.com image files for use by emfield.module.
 */

// Include Common Smugmug Functions and Contants
module_load_include('inc', 'media_smugmug_image', 'smugmug_common');

/**
 * hook emimage_PROVIDER_info
 * this returns information relevant to a specific 3rd party video provider
 * @return
 *   an array of strings requested by various admin and other forms
 *   'name' => the translated name of the provider
 *   'url' => the url to the main page for the provider
 *   'settings_description' => a description of the provider that will be posted in the admin settings form
 *   'supported_features' => an array of rows describing the state of certain supported features by the provider.
 *      These will be rendered in a table, with the columns being 'Feature', 'Supported', 'Notes'.
 */
function emimage_smugmug_image_info() {
  $name = t('Smugmug');
  $features = array(
    array(t('Import photosets'), t('No'), ''),
  );
  return array(
    'provider' => 'smugmug_image',
    'name' => t('Smugmug'),
    'url' => EMFIELD_SMUGMUG_MAIN_URL,
    'settings_description' => t('These settings specifically affect images displayed from <a href="@smugmug_image" target="_blank">Smugmug</a>.', array('@smugmug_image' => EMFIELD_SMUGMUG_MAIN_URL)),
    'supported_features' => $features,
    'import_sets_word' => t('photosets'),
  );
}

/**
 * hook emimage_PROVIDER_settings
 * this should return a subform to be added to the emimage_settings() admin settings page.
 * note that a form field will already be provided, at $form['PROVIDER'] (such as $form['smugmug_image'])
 * so if you want specific provider settings within that field, you can add the elements to that form field.
 */
function emimage_smugmug_image_settings() {
  $form['smugmug_image']['api'] = array(
    '#type' => 'fieldset',
    '#title' => t('Smugmug API'),
    '#description' => t('You will first need to apply for an API Key from the <a href="@smugmug_image" target="_blank">Smugmug API Keys page</a>.', array('@smugmug_image' => EMFIELD_SMUGMUG_API_APPLICATION_URL)),
    '#collapsible' => TRUE,
    '#collapsed' => (variable_get('emfield_smugmug_api_key', '') != ''),
  );
  $form['smugmug_image']['api']['emfield_smugmug_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Smugmug API Key'),
    '#default_value' => variable_get('emfield_smugmug_api_key', ''),
    '#description' => t('Please enter your Smugmug Developer Key here.'),
  );
  $form['smugmug_image']['api']['emfield_smugmug_api_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Smugmug API Secret'),
    '#default_value' => variable_get('emfield_smugmug_api_secret', ''),
    '#description' => t('If you have a secret for the Smugmug API, enter it here.'),
  );
  return $form;
}

function emimage_smugmug_image_data($field, $item) {
  //Note: We capture a lot of details to allow for improved theming beyond cck formatters
  if ($item) {
    //Unlike other providers smugmug has both and id and key for each item
    $idandkey = preg_split("/_/", $item['value']);
    $imageid = $idandkey[0];
    $imagekey = $idandkey[1];
    $sid = _emfield_smugmug_getsessionid();
    $xml = emfield_smugmug_request_xml('smugmug.images.getInfo', array('SessionID' => $sid , 'ImageID' => $imageid, 'ImageKey' => $imagekey));
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
    $data['custombaseurl'] = preg_replace('/-S.*/', '', $data['smallurl']);
    $data['md5sum'] = $xml['Image']['MD5Sum'];
    $data['position'] = $xml['Image']['Position'];
    $data['serial'] = $xml['Image']['Serial'];
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
     * array_walk($keywords, '_emimage_smugmug_image_trimkeywords');
     * $data['tags'][] = $keywords;
     * foreach ($keywords as $tag) {
     *   $data['tags'][] = trim($tag, '"');
     * }
     */
    $data['keywords'] = $xml['Image']['Keywords'];
    return $data;
  }
}

function emimage_smugmug_image_extract($embed = '') {
  // Smugmug items (pictures or video) have an image id and an image key
  // Why does smugmug have an ID and a KEY for each photo?
  // see http://blogs.smugmug.com/don/2008/02/08/big-privacy-changes-at-smugmug/
  // Long and short is the ids were too easy to guess which became a privacy issue

  // Smugmug provides a variety of links for a single photo.  These may be different
  // album styles, links from the "get link" page, etc.
  // We'll use three regex to deal with the possible links

  // Regex documentation
  // '@smugmug\.com/*/[^#]*#(\w+)@i' deals with...
  // http://www.smugmug.com/gallery/7930192_CWUod#555555663_g3Qcc
  // http://nickname.smugmug.com/gallery/7930192_CWUod#555555663_g3Qcc
  // http://nickname.smugmug.com/gallery/7929871_CUVKf#555555663_g3Qcc-L-LB
  // http://nickname.smugmug.com/gallery/7929871_CUVKf#555555663_g3Qcc-X2
  // http://nickname.smugmug.com/category/galleryname/7929871_CUVKf#555555663_g3Qcc
  // '@smugmug\.com/*/[^/]*/[\d]/[#]?(\w+)@i' deals with...
  // http://nickname.smugmug.com/gallery/7930158_uJtW6/1/555555663_g3Qcc
  // http://nickname.smugmug.com/gallery/7930158_uJtW6/1/#555555663_g3Qcc-A-LB
  // http://nickname.smugmug.com/category/galleryname/7929871_CUVKf/1/555555663_g3Qcc
  // '@smugmug\.com/[^#]*/(\w+_\w+)@i' deals with...
  // http://nickname.smugmug.com/photos/555555663_g3Qcc-L.jpg (and other variants from "get link" page)

  // In all of the above it is 555555663_g3Qcc we are trying to extract

  return array(
    '@smugmug\.com/*/[^#]*#(\w+)@i',
    '@smugmug\.com/*/[^#]*/[\d]/[#]?(\w+)@i',
    '@smugmug\.com/[^#]*/(\w+_\w+)@i',
  );
}

/**
 * hook emimage_PROVIDER_embedded_link($code)
 * returns a link to view the content at the provider's site
 *  @param $code
 *    the string containing the content to watch
 *  @return
 *    a string containing the URL to view the video at the original provider's site
 */
function emimage_smugmug_image_embedded_link($code, $data = array()) {
  // Use a common procedure for both image and video provider
  return _emfield_smugmug_embedded_link($code, $data = array());
}

/**
 *  Implements emimage_PROVIDER_image_url.
 *
 *  @param $code
 *    The provider code of the image.
 *  @param $width
 *  @param $height
 *    The dimensions of the photo to display.
 *  @return
 *    The url directly to the image to display.
 */
function emimage_smugmug_image_image_url($code, $width, $height, $formatter = NULL, $field = NULL, $item = NULL, $node = NULL) {
  if ($code) {
    if ($item['data']) {
      $size = _emimage_smugmug_image_guess_size($width, $height, $item);
      $url = $item['data'][$size];
    }
  }
  return $url;
}

/**
 *  Implements emimage_PROVIDER_image_title.
 *
 *  @param $code
 *    The provider code of the image.
 *  @param $data
 *    Any stored data for the image, which may already have the title.
 *  @return
 *    The title as the 3rd party provider knows it, if accessible to us. otherwise, ''.
 */
function emimage_smugmug_image_image_title($code, $data) {
  if ($data['title']) {
    return $data['title'];
  }

  if ($code) {
    $idandkey = preg_split("/_/", $code);
    $imageid = $idandkey[0];
    $imagekey = $idandkey[1];
    $sid = _emfield_smugmug_getsessionid();
    $xml = emfield_smugmug_request_xml('smugmug.images.getInfo', array('SessionID' => $sid , 'ImageID' => $imageid, 'ImageKey' => $imagekey));
    return $xml['Image']['Caption'] ? $xml['Image']['Caption'] : $code;
  }
}

/**
 *  Helper function for emimage_smugmug_image_image_url.
 *  This will return the appropriate array key for the image size we wish.
 */
function _emimage_smugmug_image_guess_size($width, $height, $item) {
  // OK, so the below looks complex.  Problem I'm addressing is simpler algorithms tended to
  // pull pictures that needed to be upsized in the display when only one dimension was provided
  // The below code would (assuming only width is provided) calculate the height based on the original image dimensions
  // That height would be used to ensure that we don't get too small an image because smugmug has a "height max" that
  // we didn't check!
  // Here is a real example:
  // Smugmug definitions:
  // mediumurl = up to 600 width x up to 450 height
  // so if we provide only a width of 500, but the picture is portrait mode, then only comparing widths we would select mediumurl
  // however smugmug would cut the height off at 450 and return an image whose width is < 450 (portrait) which would then be
  // upsized to 500 in the display (ugly)!
  // We instead look at both the width given and the calculated height based on the original image to ensure we select a smugmug
  // size large enough not to be upsized.

  // If height not provided calculate one based on the original image aspect ratio
  if (!($height>0)) {
    $height = (int)ceil(($width * $item['data']['height']) / $item['data']['width']);
  }
  // If width not provided calculate one based on the original image aspect ratio
  if (!($width>0)) {
    $width = (int)ceil(($height * $item['data']['width']) /$item['data']['height']);
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
