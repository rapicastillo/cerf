<?php
// $Id$
/**
 * @file
 * Image
 *
 * This content type lets you add a link.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'key' => 'link',
  'single' => TRUE,
  'title' => t('New link'),
  'icon' => 'icon_link.png',
  'description' => t('Create a link.'),
  'top level' => TRUE,
  'category' => t('Custom'),
  'defaults' => array(
    'admin_title' => '',
    'title' => '',
    'link_title' => '',
    'link_uri' => '',
    'link_target' => '',
    'link_css' => '',
    'link_rel' => '',
  ),
  'render callback' => 'ctoolscustomplugins_link_content_type_render',
  'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
  'all contexts' => TRUE,
);

/**
 * Output function for the link. 
 * It goes through theme_panel_link()
 *
 * @see theme_panel_link
 */
function ctoolscustomplugins_link_content_type_render($subtype, $conf, $args, $contexts) {
  static $delta = 0;

  $block = new stdClass();
  $block->subtype = ++$delta;
  $block->title   = filter_xss_admin($conf['title']);
  $block->content = '';
  $block->content .= theme_panel_link($conf['link_uri'], $conf['link_title'], $conf['link_target'], $conf['link_css'], $conf['link_rel']);

  return $block;
}

/**
 * Callback to provide administrative info.
 * We render the link in the administrative pane.
 * 
 * @see theme_panel_link
 */
function ctoolscustomplugins_link_content_type_admin_info($subtype, $conf) {
  $block = new stdClass();
  $block->title = filter_xss_admin($conf['title']);  
  $block->content .= theme_panel_link($conf['link_uri'], $conf['link_title'], $conf['link_target'], $conf['link_css'], $conf['link_rel']);
  
  return $block;
}

/**
 * Callback to provide the administrative title
 */
function ctoolscustomplugins_link_content_type_admin_title($subtype, $conf) {
  $output = t('Link');
  
  if(empty($conf['admin_title'])) {
    if(!empty($conf['title'])) {
      // If the 'title' is not empty then use the title as an administrative title
      $title = $conf['title'];
    }
    else {
      // If there is no 'admin title' and no 'title' then use the 'link title'
      $title = $conf['link_title'];
    }
  }
  else {
    // If we have an admin title, use it.
    $title = $conf['admin_title'];
  }
  
  if ($title) {
    $output = t('Link: @title', array('@title' => $title));
  }
  
  return $output;
}

/**
 * Returns an edit form for the custom type.
 */
function ctoolscustomplugins_link_content_type_edit_form(&$form, &$form_state) {
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
    '#description' => t("This title is the pane title, do not confuse with the link title"),
  );
  
  $form['link'] = array(
    '#type' => 'fieldset',
    '#title' => t('Link'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['link']['link_title'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['link_title'],
    '#title' => t('Title'),
    '#description' => t("Enter a title for this link, if blank, the URI will be used as the title."),
  );
  $form['link']['link_uri'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['link_uri'],
    '#title' => t('URI'),
    '#description' => t('Enter a URI. For external links, use "http://", example: http://www.example.com. For internal links just use the internal path, example: node/82'),
    '#required' => TRUE
  );
  $form['link']['link_target'] = array(
    '#type' => 'checkbox',
    '#title' => t("Open in a new window"),
    '#default_value' => $conf['link_target'],
  );
  $form['link']['link_css'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['link_css'],
    '#title' => t('CSS Class'),
    '#description' => t("CSS class to apply to this link. This may be blank."),
  );
  $form['link']['link_rel'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['link_rel'],
    '#title' => t('Rel'),
    '#description' => t("The rel attribute specifies the relationship between the current document and the linked document."),
  );
  
  return $form;
}

/**
 * The submit form stores the data in $conf.
 * Ctools will do the rest
 */
function ctoolscustomplugins_link_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }  
}
