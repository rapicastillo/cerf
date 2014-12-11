<?php
// $Id: relcontext_item.inc,v 1.1 2009/02/11 16:53:54 rfay Exp $


/**
 * @file
 * Content type that displays the relcontext context type
 *
 * This example is for use with the relcontext relationship to show
 * how we can get a relationship-context into a data type
*
 * There is some information about the content_types plugin at
 * http://doxy.samboyer.org/panels2/ctools_api_plugins_content_types.html
 *
 */

/**
 * Hook to supply a list of content types.
 * The hook is:
 * <modulename>_<content_type>_ctools_content_types()
 */
function ctools_plugin_example_relcontext_content_type_ctools_content_types() {
    // Key must be the name of the content type
  $items['relcontext_content_type'] = array(
    // Unknown where this is used
    'title' => t('Ctools Example Relcontext Content Type'),
    // 'admin title' => 'ctools_plugin_example_relcontext_content_type_admin_title',
    'admin info' => 'ctools_plugin_example_relcontext_content_type_admin_info',
    // Constructor?
    'content_types' => 'relcontext_content_type',
    'single' => TRUE,
    'render callback' => 'relcontext_content_type_render',
    // goes in the directory with the content type. Here, in panels/content_types
    'icon' => 'icon_example.png',
    'description' => t('Relcontext content type - works with relcontext context.'),
    'required context' => new ctools_context_required(t('Relcontext'), 'relcontext'),
    'category' => array(t('ReliefWeb API'), -9),
    'edit form' => 'relcontext_edit_form',

  );
  return $items;
}


//function ctools_plugin_example_relcontext_content_type_admin_title($subtype, $conf) {
//  $output = t('Custom');
//  if (!empty($conf['title'])) {
//    $output .= " (" . filter_xss_admin($conf['title']) . ")";
//  }
//  return $output;
//}


/**
 * Run-time rendering of the body of the blcok
 *
 * @param $subtype
 * @param $conf Configuration as done at admin time
 * @param $args
 * @param $context  Context - in this case we don't have any
 *
 * @return unknown_type  An object with at least title and content members
 */
function relcontext_content_type_render($subtype, $conf, $args, $context) {
  $data = $context->data;
  $block = new stdClass();

  // Don't forget to check this data if it's untrusted
  // The title actually used in rendering
  $block->title = "Relcontext content type";
  $block->content = <<<END
    This is a block of data created by the Relcontent content type.
    Data in the block may be assembled from static text (like this) or from the
    content type settings form (\$conf) for the content type, or from the context
    that is passed in. <br/>
    In our case, the configuration form (\$conf) has just one field, 'config_item_1;
    and it's configured with:
END;
  if (!empty($conf)) {
    $block->content .= '<div style="border: 1px solid red;">' . var_export($conf['config_item_1'], TRUE) . '</div>';
  }
  if (!empty($context)) {
    $block->content .= '<br/>The args ($args) were <div style="border: 1px solid yellow;" >' .
      var_export($args, TRUE). '</div>';
  }
  $block->content .= '<br/>And the relcontext context  ($context->data->description) ,
    (which was created from a
    simplecontext context) was <div style="border: 1px solid green;" >' .
    print_r($context->data->description,TRUE) . '</div>';

  return $block;

}


/**
 * 'Edit' callback for the content type
 * This example just returns a form
 *
 */
function relcontext_edit_form(&$form, &$form_state) {
  $conf = $form_state['conf'];

  $form['config_item_1'] = array(
    '#type' => 'textfield',
    '#title' => t('Config Item 1 (relcontext)'),
    '#size' => 50,
    '#description' => t('Setting for relcontext.'),
    '#default_value' => $conf['config_item_1'],
    '#prefix' => '<div class="clear-block no-float">',
    '#suffix' => '</div>',
  );

  return $form;
}

function relcontext_edit_form_submit(&$form, &$form_state) {
  foreach (array_keys($form) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}
