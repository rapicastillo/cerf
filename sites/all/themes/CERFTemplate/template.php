<?php


/*RCSC - Checks if the referer is WHD if so, show splash*/
//$referer = $_SERVER['HTTP_REFERER'];
$referer = $_GET['ref'];
if (preg_match("@worldhumanitarianday@", $referer))
{
   drupal_add_css( drupal_get_path('theme', 'CERFTemplate') .'/css/show-splash.css', 'theme', 'screen');
}
else
{
   drupal_add_css( drupal_get_path('theme', 'CERFTemplate') .'/css/hide-splash.css', 'theme', 'screen');
}



/**
 * @file
 * Contains theme override functions and preprocess functions for the CERFTemplate theme.
 */

/**
 * Implements theme_preprocess_page().
 * We are overwriting the default meta character type tag with HTML5 version.
 */
function CERFTemplate_preprocess_page(&$vars, $hook) {
  $vars['head'] = str_replace(
    '<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />',
    '<meta charset="utf-8" />',
    $vars['head']
  );
}









function CERFTemplate_preprocess_search_theme_form(&$vars, $hook) {
  // html5 compatible edits
  $vars['search_form'] = str_replace('type="text"', 'type="search"', $vars['search_form']);
  // Remove the "Search this site" label from the form.
  $vars['form']['search_theme_form']['#title'] = t('');
  
  // Set a default value for text inside the search box field.
  $vars['form']['search_theme_form']['#value'] = t('Search');
  
  // Add a custom class and placeholder text to the search box.
  $vars['form']['search_theme_form']['#attributes'] = array('class' => 'NormalTextBox txtSearch', 'onblur' => "if (this.value == '') {this.value = '".$vars['form']['search_theme_form']['#value']."';} ;", 'onfocus' => "if (this.value == '".$vars['form']['search_theme_form']['#value']."') {this.value = '';} ;" );

  
  // Change the text on the submit button
  //$vars['form']['submit']['#value'] = t('Go');

  // Rebuild the rendered version (search form only, rest remains unchanged)
  unset($vars['form']['search_theme_form']['#printed']);
  $vars['search']['search_theme_form'] = drupal_render($vars['form']['search_theme_form']);

  $vars['form']['submit']['#type'] = 'image_button';
  $vars['form']['submit']['#src'] = path_to_theme() . '/images/search.png';
    
  // Rebuild the rendered version (submit button, rest remains unchanged)
  unset($vars['form']['submit']['#printed']);
  $vars['search']['submit'] = drupal_render($vars['form']['submit']);

  // Collect all form elements to make it easier to print the whole form.
  $vars['search_form'] = implode($vars['search']);
}


function CERFTemplate_menu_item_link($link) {
  if (empty($link['localized_options'])) {
    $link['localized_options'] = array();
  }
  if ($link['type'] && $link['href'] == 'node/1') {
    return $link['title'];
  }
  else {
    return l($link['title'], $link['href'], $link['localized_options']);
  }
}
