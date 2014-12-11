<?php
// $Id: simplecontext_content_type.inc,v 1.2 2009/02/11 16:53:54 rfay Exp $


/**
 * @file
 * Sample ctools content type that takes advantage of context
 *
 * This example uses the context it gets (simplecontext) to demo how a
 * ctools content type can access and use context. Note that the simplecontext
 * can be either configured manually into a panel or it can be retrieved via
 * an argument.
 *
 */

/**
 * Hook to supply a list of content types.
 * The hook is:
 * <modulename>_<content_type>_ctools_content_types()
 */
function reliefweb_panel_simplecontext_content_type_ctools_content_types() {
    // Key must be the name of the content type
  $items['simplecontext_content_type'] = array(
    'title' => t('Simplecontext Content Type'),
    // Constructor?
    'content_types' => 'simplecontext_content_type',
    // 'single' means not to be subtyped
    'single' => TRUE,
    // Name of a function which will render the block
    'render callback' => 'simplecontext_content_type_render',

    // goes in the directory with the content type. Here, in panels/content_types
    'icon' => 'icon_example.png',
    'description' => t('Simplecontext content type - works with a simplecontext context.'),
    'required context' => new ctools_context_required(t('Simplecontext'), 'simplecontext'),
    'edit form' => 'simplecontext_content_type_edit_form',
    'admin title' => 'reliefweb_panel_simplecontext_content_type_admin_title',
    'admin info' => 'reliefweb_panel_simplecontext_content_type_admin_info',
    'category' => array(t('ReliefWeb API'), -9),

  );
  return $items;
}

function reliefweb_panel_simplecontext_content_type_admin_title($subtype, $conf, $context = NULL) {
  $output = t('Simplecontext');
  if ($conf['override_title'] && !empty($conf['override_title_text'])) {
    $output = filter_xss_admin($conf['override_title_text']);
  }
  return $output;
}



/**
 * Callback to provide administrative info. In this case we'll render the
 * content as long as it's not PHP, which is too risky to render here.
 */
function reliefweb_panel_simplecontext_content_type_admin_info($subtype, $conf, $context = NULL) {
  $block = new stdClass();
  $block->title = check_plain($conf['title']);
  if (!empty($context) && !empty($context->data->description)) {
    $block->content = check_plain($context->data->description);
  }
  return $block;

}

/**
 * Run-time rendering of the body of the block (content type)
 *
 * @param $subtype
 * @param $conf Configuration as done at admin time
 * @param $args
 * @param $context  Context - in this case we don't have any
 *
 * @return unknown_type  An object with at least title and content members
 */
function simplecontext_content_type_render($subtype, $conf, $args, $context) {
  $data = $context->data;
  $block = new stdClass();

  // Don't forget to check this data if it's untrusted
  // The title actually used in rendering
  $block->title = "Simplecontext Content Type";
  $block->content = <<<END
    This is a block of data created by the Simplecontext content type.
    Data in the block may be assembled from static text (like this) or from the
    content type settings form (\$conf) for the content type, or from the context
    that is passed in. <br/>
    In our case, the configuration form (\$conf) has just one field, 'config_item_1;
    and it's configured with:
END;
  if (!empty($conf)) {
    $block->content .= '<div style="border: 1px solid red;">' . print_r($conf['config_item_1'], TRUE) . '</div>';
  }
  if (!empty($context)) {
    $block->content .= '<br/>The args ($args) were <div style="border: 1px solid yellow;" >' .
      var_export($args, TRUE). '</div>';
  }
  $block->content .= '<br/>And the simplecontext context ($context->data->description) was <div style="border: 1px solid green;" >' .
    print_r($context->data->description,TRUE) . '</div>';

  return $block;
}


/**
 * 'Edit' callback for the content type
 * This example just returns a form
 *
 */
function simplecontext_content_type_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];

  $form['config_item_1'] = array(
    '#type' => 'textfield',
    '#title' => t('Config Item 1 for simplecontext content type'),
    '#size' => 50,
    '#description' => t('The stuff for item 1.'),
    '#default_value' => $conf['config_item_1'],
    '#prefix' => '<div class="clear-block no-float">',
    '#suffix' => '</div>',
  );

  return $form;
}

function simplecontext_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}

