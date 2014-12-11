<?php
// $Id: smugmug_common.inc,v 1.3 2010/04/05 03:42:35 winston Exp $

/**
 * @file
 * This include contains common constants and functions to support all smugmug emfield providers
 */

define('EMFIELD_SMUGMUG_MAIN_URL', 'http://www.smugmug.com/');
define('EMFIELD_SMUGMUG_API_INFO', 'http://wiki.smugmug.net/display/SmugMug/API');
define('EMFIELD_SMUGMUG_API_APPLICATION_URL', 'http://www.smugmug.com/hack/apikeys');
define('EMFIELD_SMUGMUG_REST_ENDPOINT', 'http://api.smugmug.com/services/api/php/1.2.2/');

/**
 * this is a wrapper for emfield_request_xml that checks for api key
 */
function emfield_smugmug_request_xml($method, $args = array(), $cached = FALSE) {
  // display an error if we don't have an api key yet
  emfield_smugmug_error_check();

  $args['method'] = $method;
  $xml = module_invoke('emfield', 'request_xml', 'smugmug', EMFIELD_SMUGMUG_REST_ENDPOINT, $args, $cached, FALSE, FALSE, TRUE);
  return $xml;
}

/**
 *  This will log an error if we don't have a key yet. In addition, if the user is an admin, we'll display an error.
 */
function emfield_smugmug_error_check() {
  static $checked;
  if (!$checked && (variable_get('emfield_smugmug_api_key', '') == '')) {
    global $user;
    $error = t('You do not yet have a Smugmug API key set. You will need to <a href="@apply" target="_blank">apply for a Smugmug API key</a> and enter your key at the <a href="@settings">settings administration page</a> before Smugmug items may be displayed.', array('@apply' => EMFIELD_SMUGMUG_API_APPLICATION_URL, '@settings' => url('admin/content/emfield')));
    if (user_access('administer site configuration')) {
      drupal_set_message($error, 'error');
    }
    watchdog('Embedded Media Field', '!error', array('!error' => $error));
  }
  $checked = TRUE;
}

/**
 *  Helper function to retrieve a smugmug session id.
 *  Returns a session id to be used by further method requests.
 */
function _emfield_smugmug_getsessionid() {
  static $sid;
  if ($sid) {
    return $sid;
  }

  //First retrieve our cached sessionid and see if it is too old (>5 hours)
  //Calculate cacheid (cid)
  $cargs['method'] = 'smugmug.login.anonymously';
  $cargs['APIKey'] = trim(variable_get('emfield_smugmug_api_key', ''));
  ksort($cargs);
  $arghash = $provider .':';
  foreach ($cargs as $k => $v) {
    $arghash .= $k . $v;
  }
  $arghash = 'smugmug'. $arghash  .':';
  //Fetch session id from cache if it exists
  $cache = cache_get($arghash, 'cache');
  if ($cache) {
    $ccreated = $cache->created;
    $time_since_sessionid = time() - $ccreated;
    if ($time_since_sessionid > 18000) {
      //it is too old so invalidate smugmug: cache items
      cache_clear_all('smugmug', 'cache', TRUE);
    }
    else {
      //it is not too old so store in static $sid and return
      $sid = $cache->data['Login']['Session']['id'];
      return $sid;
    }
  }

  //No cached sessionid so we'll need to get a new one from smugmug after all
  //Note: APIKey only needed to get a session id.  All other methods use session id.
  $args['APIKey'] = trim(variable_get('emfield_smugmug_api_key', ''));
  $loginresponse = emfield_smugmug_request_xml('smugmug.login.anonymously', $args, TRUE);
  $sid = $loginresponse['Login']['Session']['id'];
  return $sid;
}

/**
 * Helper function for hook emfield_PROVIDER_embedded_link($code)
 * returns a link to view the content at the provider's site
 *  @param $code
 *    the string containing the content
 *  @return
 *    a string containing the URL to view the item at the original provider's site
 */
function _emfield_smugmug_embedded_link($code, $data = array()) {
  //Link directly to the item
  //Note: In smugmug all items have to be in an album so this is always a "item in album" link.
  if ($data['albumurl']) {
    return $data['albumurl'];
  }
  if ($code) {
    $idandkey = preg_split("/_/", $code);
    $imageid = $idandkey[0];
    $imagekey = $idandkey[1];
    $sid = _emfield_smugmug_getsessionid();
    $smugmugurls = emfield_smugmug_request_xml('smugmug.images.getInfo', array('SessionID' => $sid , 'ImageID' => $imageid, 'ImageKey' => $imagekey));
    return $smugmugurls['Image']['Album']['URL'];
  }
}
