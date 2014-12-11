<?php
// $Id$
/**
 * @file
 * Image
 *
 * This content type lets you add an image that can also be a link.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'key' => 'image',
  'single' => TRUE,
  'title' => t('New image'),
  'icon' => 'icon_image.png',
  'description' => t('Create an image.'),
  'top level' => TRUE,
  'category' => t('Custom'),
  'render callback' => 'ctoolscustomplugins_image_content_type_render',
  'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
  'all contexts' => TRUE,
  'defaults' => array(
    'admin_title' => '',
    'title' => '',
    'image_upload' => '',
    'image_description' => '',
    'image_caption' => '',
    'image_css' => '',
    'image_preset' => '',
    'link_uri' => '',
    'link_rel' => '',
    'link_css' => '',
    'link_target' => '',
   ),
);

/**
 * Output function for the image. 
 * It goes through theme_panel_image()
 *
 * @see theme_panel_image
 */
function ctoolscustomplugins_image_content_type_render($subtype, $conf, $args, $contexts) {
  static $delta = 0;

  $block = new stdClass();
  $block->subtype = ++$delta;
  $block->title   = filter_xss_admin($conf['title']);
  $block->content = '';
  $block->content .= theme_panel_image($conf['image'], $conf['image_description'], $conf['image_caption'], $conf['image_css'], $conf['image_preset'], $conf['link_uri'], $conf['link_target'], $conf['link_css'], $conf['link_rel']);
  
  return $block;
}

/**
 * Callback to provide administrative info.
 * We render the image in the administrative pane.
 *
 * @see theme_panel_image
 */
function ctoolscustomplugins_image_content_type_admin_info($subtype, $conf) {
  $block = new stdClass();
  $block->title   = filter_xss_admin($conf['title']);
  $block->content = '';
  $block->content .= theme_panel_image($conf['image'], $conf['image_description'], $conf['image_caption'], $conf['image_css'], $conf['image_preset'], $conf['link_uri'], $conf['link_target'], $conf['link_css'], $conf['link_rel']);
  
  return $block;
}

/**
 * Callback to provide the administrative title
 */
function ctoolscustomplugins_image_content_type_admin_title($subtype, $conf) {
  $output = t('Image');
  
  if(empty($conf['admin_title']) && !empty($conf['title'])) {
    // If the 'title' is not empty then use the title as an administrative title
     $title = $conf['title'];
  }
  else {
    // If we have an admin title, use it.
    $title = $conf['admin_title'];
  }
  
  if ($title) {
    $output = t('Image: @title', array('@title' => $title));
  }
  
  return $output;
}

/**
 * Add/edit form for the pane content
 */
function ctoolscustomplugins_image_content_type_edit_form(&$form, &$form_state) {
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
  
  $form['#attributes']['enctype'] = 'multipart/form-data';
  
  $form['image'] = array(
    '#prefix' => '<div class="image-field-wrapper">',
    '#suffix' => '</div>',
  );
  $form['image']['preview'] = array(
    '#type' => 'item',
    '#after_build' => array('ctoolscustomplugins_image_preview'),
  );
  $form['image']['image_upload'] = array(
    '#title' => t('Upload image'),
    '#type'  => 'file',
    '#description' => t('Only files with the following extensions may be uploaded: %extensions. ', array('%extensions' => 'png jpeg jpg gif')),
  );
    
  if(module_exists('imagecache')) {
    $presets = array();
    $presets[] = '';
    foreach (imagecache_presets() as $key => $preset) {
      $presets[$key] = check_plain($preset['presetname']);
    }
  
    $form['image']['image_preset'] = array(
      '#type' => 'select',
      '#title' => t("Imagecache Preset"),
      '#description' => t('Select an imagecache preset. If no preset is selected, the original image will be used.'),
      '#default_value' => $conf['image_preset'],
      '#options' => $presets,
    );
  }
  
  $form['image']['image_description'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['image_description'],
    '#title' => t("Image description"),
    '#description' => t("The image description will be used as the image alternate text and as the image title"),
  );
  $form['image']['image_caption'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['image_caption'],
    '#title' => t("Image caption"),
    '#description' => t("This will be used as an image caption"),
  );
  $form['image']['image_css'] = array(
    '#type' => 'textfield',
    '#default_value' => $conf['image_css'],
    '#title' => t("CSS Class"),
    '#description' => t("CSS class to apply to this image. This may be blank."),
  );
  
  // If the image is a link
  $form['link'] = array(
    '#type' => 'fieldset',
    '#title' => t("Link"),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['link']['link_uri'] = array(
    '#type' => 'textfield',
    '#title' => t("Link URI"),
    '#default_value' => $conf['link_uri'],
    '#description' => t('If you want to make this image a link, please enter a URI. For external links, use "http://", example: http://www.example.com. For internal links just use the internal path, example: node/82'),
  );
  $form['link']['link_target'] = array(
    '#type' => 'checkbox',
    '#title' => t("Open in a new window"),
    '#default_value' => $conf['link_target'],
  );
  $form['link']['link_css'] = array(
    '#type' => 'textfield',
    '#title' => t("CSS Class"),
    '#default_value' => $conf['link_css'],
    '#description' => t("CSS class to apply to this link. This may be blank.")
  );
  $form['link']['link_rel'] = array(
    '#type' => 'textfield',
    '#title' => t("Rel"),
    '#default_value' => $conf['link_rel'],
    '#description' => t("The rel attribute specifies the relationship between the current document and the linked document.")
  );
  
  $form['#validate'] = array('ctoolscustomplugins_image_content_type_edit_form_submit_validate');
  
  return $form;
}

/**
 * Vaildate function 
 */
function ctoolscustomplugins_image_content_type_edit_form_submit_validate(&$form, &$form_state) {
  $validators = array(
    'file_validate_is_image' => array(),
  );
  if (user_access('upload images in panels') && ($image = file_save_upload('image_upload', $validators, file_directory_path(), FILE_EXISTS_RENAME))) {
    $image->description = $image->filename;
    $image->weight = 0;
    $image->new = TRUE;
    $form_state['conf']['image'] = (array)$image;
    file_set_status($image, FILE_STATUS_PERMANENT);
  }
}

/**
 * The submit form stores the data in $conf
 * Ctools will do the rest
 */
function ctoolscustomplugins_image_content_type_edit_form_submit(&$form, &$form_state) { 
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    if (isset($form_state['values'][$key])) {
      $form_state['conf'][$key] = $form_state['values'][$key];
    }
  }
}

/**
 * Callback to display an image if we already have one
 *
 * @see theme_panel_image_preview
 *
 * @return
 * An image
 */
function ctoolscustomplugins_image_preview($form, &$form_state) {
  if ($form_state['conf']['image']) {
    $image = (object)($form_state['conf']['image']);
    $form['#title'] = t("Preview");
    $form['#value'] = theme_panel_image_preview($image);
    $form['#description'] = t("If you want to remove this image just upload a new one");
  }
  return $form;
}