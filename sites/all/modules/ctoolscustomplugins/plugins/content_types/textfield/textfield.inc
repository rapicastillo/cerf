<?php
// $Id$

/**
 * @file
 * Textfield
 *
 * This is a single line text field.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'key' => 'textfield',
  'single' => TRUE,
  'title' => t('New single line text'),
  'icon' => 'icon_textfield.png',
  'description' => t('Create a single line text field.'),
  'top level' => TRUE,
  'category' => t('Custom'),
  'defaults' => array(
    'admin_title' => '',
    'textfield' => '',
    'textfield_css' => '',
  ),
  'render callback' => 'ctoolscustomplugins_textfield_content_type_render',
  'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
  'all contexts' => TRUE,
);

/**
 * Output function for the image. 
 * It goes through theme_panel_image()
 *
 * @see theme_panel_textfield
 */
function ctoolscustomplugins_textfield_content_type_render($subtype, $conf, $args, $contexts) {
  static $delta = 0;

  $block = new stdClass();
  $block->subtype = ++$delta;
  $block->title   = '';
  $block->content = '';
  $block->content .= theme_panel_textfield($conf['textfield'], $conf['textfield_css']);
  
  return $block;
}

/**
 * Callback to provide administrative info.
 * We render the text field in the administrative pane.
 *
 * @see theme_panel_textfield
 */
function ctoolscustomplugins_textfield_content_type_admin_info($subtype, $conf) {
  $block = new stdClass();
  $block->title = '';
  $block->content = '';
  $block->content .= theme_panel_textfield($conf['textfield'], $conf['textfield_css']);
  
  return $block;
}

/**
 * Callback to provide the administrative title
 */
function ctoolscustomplugins_textfield_content_type_admin_title($subtype, $conf) {
  $output = t('Text');
  
  if(empty($conf['admin_title']) && !empty($conf['textfield'])) {
    // If the 'textfield' is not empty then use the textfield as an administrative title
     $title = $conf['textfield'];
  }
  else {
    // If we have an admin title, use it.
    $title = $conf['admin_title'];
  }
  
  if ($title) {
    $output = t('Text: @title', array('@title' => $title));
  }
  
  return $output;
}

/**
 * Add/edit form for the pane content
 */
function ctoolscustomplugins_textfield_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];
  $form['admin_title'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($conf['admin_title']) ? $conf['admin_title'] : '',
    '#title' => t('Administrative title'),
    '#description' => t('This title will be used administratively to identify this pane. If blank, the regular title will be used.'),
  );
  
  $form['textfield'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['textfield'],
    '#title' => t('Text'),
  );
  $form['textfield_css'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['textfield_css'],
    '#title' => t('CSS Class'),
    '#description' => t("CSS class to apply to this link. This may be blank.")
  );

  return $form;
}

/**
 * The submit form stores the data in $conf
 * Ctools will do the rest
 */
function ctoolscustomplugins_textfield_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }  
}
