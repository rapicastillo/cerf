<?php
// $Id$

/**
 * @file
 * Emvideo
 *
 * This is an embedded video from Youtube, Blip or Vimeo. It uses Video Filter
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'key' => 'emvideo',
  'single' => TRUE,
  'title' => t('New embedded video'),
  'icon' => 'icon_emvideo.png',
  'description' => t('Create an embedded video.'),
  'top level' => TRUE,
  'category' => t('Custom'),
  'defaults' => array(
    'admin_title' => '',
    'emvideo_url' => '',
    'emvideo_width' => '',
    'emvideo_height' => '',
    'emvideo_caption' => '',
    'emvideo_css' => '',
  ),
  'render callback' => 'ctoolscustomplugins_emvideo_content_type_render',
  'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
  'all contexts' => TRUE,
);

/**
 * Output function for the embedded video. 
 * It goes through theme_panel_emvideo()
 *
 * @see theme_panel_emvideo
 */
function ctoolscustomplugins_emvideo_content_type_render($subtype, $conf, $args, $contexts) {
  static $delta = 0;

  $block = new stdClass();
  $block->subtype = ++$delta;
  $block->title   = '';
  $block->content = '';
  $block->content .= theme_panel_emvideo(check_plain($conf['emvideo_url']), check_plain($conf['emvideo_width']), check_plain($conf['emvideo_height']), check_plain($conf['emvideo_caption']), check_plain($conf['emvideo_css']));
  
  return $block;
}

/**
 * Callback to provide administrative info.
 * We render the emvideo in the administrative pane.
 *
 * @see theme_panel_emvideo
 */
function ctoolscustomplugins_emvideo_content_type_admin_info($subtype, $conf) {
  $block = new stdClass();
  $block->title = '';
  $block->content = '';
  $block->content .= theme_panel_emvideo(check_plain($conf['emvideo_url']), check_plain($conf['emvideo_width']), check_plain($conf['emvideo_height']), check_plain($conf['emvideo_caption']), check_plain($conf['emvideo_css']));  

  return $block;
}

/**
 * Callback to provide the administrative title
 */
function ctoolscustomplugins_emvideo_content_type_admin_title($subtype, $conf) {
  $output = t('Video');
  
  if(empty($conf['admin_title']) && !empty($conf['title'])) {
    // If the 'title' is not empty then use the title as an administrative title
     $title = $conf['title'];
  }
  else {
    // If we have an admin title, use it.
    $title = $conf['admin_title'];
  }
  
  if ($title) {
    $output = t('Video: @title', array('@title' => $title));
  }
  
  return $output;
}

/**
 * Add/edit form for the pane content
 */
function ctoolscustomplugins_emvideo_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];
  $form['admin_title'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($conf['admin_title']) ? $conf['admin_title'] : '',
    '#title' => t('Administrative title'),
    '#description' => t('This title will be used administratively to identify this pane. If blank, the regular title will be used.'),
  );
  
  $form['title'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['title'],
    '#title' => t('Title'),
    '#description' => t("This will be used as the Pane title"),
  );
  
  $form['emvideo'] = array(
    '#type' => 'fieldset',
    '#title' => t("Embedded video"),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['emvideo']['emvideo_url'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['emvideo_url'],
    '#title' => t('Video URL'),
    '#description' => t("Enter a URL of the video. Only YouTube, Blip.tv and Vimeo are supported."),
  );
  $form['emvideo']['emvideo_width'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['emvideo_width'],
    '#title' => t('Video Width'),
    '#description' => t("Enter a Width for the video. Recommended value is 425. If blank, the recommended value will be used."),
  );
  $form['emvideo']['emvideo_height'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['emvideo_height'],
    '#title' => t('Video Height'),
    '#description' => t("Enter a Height for the video. Recommended value is 350. If blank, the recommended value will be used."),
  );
  $form['emvideo']['emvideo_caption'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['emvideo_caption'],
    '#title' => t('Video Caption'),
  );
  $form['emvideo']['emvideo_css'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['emvideo_css'],
    '#title' => t('CSS Class'),
    '#description' => t("CSS class to apply to this link. This may be blank.")
  );

  return $form;
}

/**
 * The submit form stores the data in $conf
 * Ctools will do the rest
 */
function ctoolscustomplugins_emvideo_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }  
}
