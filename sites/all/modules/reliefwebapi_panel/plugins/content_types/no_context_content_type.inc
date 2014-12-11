<?php
// $Id: no_context_content_type.inc,v 1.2 2009/02/11 16:53:54 rfay Exp $


/**
 * @file
 * "No context" sample content type. It operates with no context at all. It would
 * be basically the same as a 'custom content' block, but it's not even that
 * sophisticated.
 *
 */

/**
 * Hook to supply a list of content types.
 * The hook is:
 * <modulename>_<content_type>_ctools_content_types()
 */
 
function reliefweb_panel_no_context_content_type_ctools_content_types() {
    // Key must be the name of the content type
  $items['no_context_content_type'] = array(
    'title' => t('ReliefWeb API Interface'),
    'description' => t('No Context Content Type - requires and uses no context.'),
    'admin info' => 'reliefweb_panel_no_context_content_type_admin_info',
    // 'single' => TRUE means has no subtypes
    'single' => TRUE,
    // Constructor
    'content_types' => array('no_context_content_type'),
    // Name of a function which will render the block
    'render callback' => 'no_context_content_type_render',
    'render admininfo' => 'no_context_conent_type_admininfo_render',
    // The default context
    'defaults' => array(),

    // This explicitly declares the config form. Without this line, the func would be
    // reliefweb_panel_no_context_content_type_edit_form
    'edit form' => 'no_context_content_type_edit_form',

    // goes in the directory with the content type. Here, in panels/content_types
    'icon' => 'icon_example.png',
    // 'required context' => new ctools_context_required(t('Junk'), 'junk'),
    'category' => array(t('ReliefWeb API'), -9),
    
  );
  return $items;
}


function reliefweb_panel_no_context_content_type_admin_info($subtype, $conf, $context = NULL) {
	
	  $output = no_context_conent_type_admininfo_render($subtype, $conf, null, $context);
	  
	  if (empty($output) || !is_object($output)) {
	    $output = new stdClass();
	    $output->title = t("No Info");
	    $output->content = t("Currently no Info Available");
	  } 
	  return $output;
}

function no_context_conent_type_admininfo_render($subtype, $conf, $args, $context) {
	$output =  new stdClass();
	$output->title = "ReliefWeb API : " . $conf['override_title_text'];
	$query_text = $conf['query_text'];
	$primary_country = is_array($conf['primary_country']) ? implode("|", array_filter(array_map("get_active_keys", $conf['primary_country']))) : $conf['primary_country'];
		$primary_country = trim($primary_country) == "" ? "None Selected" : $primary_country;

	$country = implode("|", array_filter(array_map("get_active_keys", $conf['country'])));
		$country = trim($country) == "" ? "None Selected" : $country;

  	$theme = implode("|", array_filter(array_map("get_active_keys", $conf['theme'])));
		$theme = trim($theme) == "" ? "None Selected" : $theme;
		
  	$organisation = implode("|", array_filter(array_map("get_active_keys", $conf['organisation'])));
		$organisation = trim($organisation) == "" ? "None Selected" : $organisation;
		
  	$content_format = implode("|", array_filter(array_map("get_active_keys", $conf['content_format'])));
		$content_format = trim($content_format) == "" ? "None Selected" : $content_format;
		
	$show_blurb_on_list = (t( !empty($conf['show_blurb_on_list'])) && (int) $conf['show_blurb_on_list'] ) ? "YES" : 'NO';
  	$show_thumb_on_list = (t( !empty($conf['show_thumb_on_list'])) && (int) $conf['show_thumb_on_list'] ) ? "YES" : 'NO';
  	$post_num_on_list = (t( !empty($conf['post_num_on_list'])) && (int) $conf['post_num_on_list'] ) ? $conf['post_num_on_list'] : '1';
  
  	$show_blurb_on_single = (t( !empty($conf['show_blurb_on_single'])) && (int) $conf['show_blurb_on_single'] ) ? "YES" : 'NO';
  	$show_thumb_on_single = (t( !empty($conf['show_thumb_on_single'])) && (int) $conf['show_thumb_on_single'] ) ? "YES" : 'NO';
  	$post_num_on_single = (t( !empty($conf['post_num_on_single'])) && (int) $conf['post_num_on_single'] ) ? $conf['post_num_on_single'] : '5';
  	$redirect_to_reliefweb = (t( !empty($conf['redirect_to_reliefweb'])) && (int) $conf['redirect_to_reliefweb'] ) ? "YES" : 'NO';
	
	
	$output->content = <<<END
	<div>
		<h5>Filters</h5>
		<div><strong>Query String: </strong> $query_text </div>
		<div><strong>Primary Country</strong>: $primary_country</div>
		<div><strong>Country</strong>: $country</div>
		<div><strong>Theme</strong>: $theme</div>
		<div><strong>Source</strong>: $organisation</div>
		<div><strong>Content Format</strong>: $content_format</div>
		
		<table>
			<tr><td>&nbsp;</td><td>Summary Page</td><td>Filter List Page</td></tr>
			<tr><td>View Blurb</td><td>$show_blurb_on_list</td><td>$show_blurb_on_single</td></tr>
			<tr><td>View Thumbnail</td><td>$show_thumb_on_list</td><td>$show_thumb_on_single</td></tr>
			<tr><td>Number of Post</td><td>$post_num_on_list</td><td>$post_num_on_single</td></tr>
			<tr><td>Redirect to RW</td><td colspan='2'>$redirect_to_reliefweb</td></tr>
		</table> 
	</div>
END;
	//$output->content = print_r($conf, true);
	return $output ;
}
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
function no_context_content_type_render($subtype, $conf, $args, $context) {
  $block = new stdClass();

  $asset_path = drupal_get_path('module', 'reliefweb_panel');
  
  drupal_add_js($asset_path . '/js/plugin/jquery.dotdotdot-1.5.9.min.js');
  drupal_add_css($asset_path . '/css/list.css');  
  drupal_add_js($asset_path . '/js/list.js');
 
  $ctools_help = theme('advanced_help_topic', 'ctools', 'plugins', 'title');
  $reliefweb_panel_help = theme('advanced_help_topic','reliefweb_panel','Chaos-Tools--Ctools--Plugin-Examples','title');

  // The title actually used in rendering
  $block->title = check_plain("No-context Content Type");
  
  /*
  $block->content = <<<END
  <div>Welcome to the Ctools Plugin Example demonstration panel.

  This block is a content type which requires no context at all. It's like a custom block,
  but not even that sophisticated.

  For more information on the example plugins, please see the advanced help for

  {$ctools_help} and {$reliefweb_panel_help}
  </div>
END;20.`
   * 
   * 
   * 
   * <input type="hidden" name='organisation' value='<?= t( isset($item['organisation'] ) ? $item['organisation'] : '') ?>'/>
				<input type="hidden" name='theme' value='<?= t( isset($item['theme'] ) ? $item['theme'] : '') ?>'/>
				<input type="hidden" name='disaster_type' value='<?= t( isset($item['disaster_type'] ) ? $item['disaster_type'] : '') ?>'/>
				<input type="hidden" name='vulnerable_groups' value='<?= t( isset($item['vulnerable_groups'] ) ? $item['vulnerable_groups'] : '') ?>'/>
				<input type="hidden" name='content_format' value='<?= t( isset($item['content_format'] ) ? $item['content_format'] : '') ?>'/>
				<input type="hidden" name='language' value='<?= t( isset($item['language'] ) ? $item['language'] : '') ?>'/>
				<input type="hidden" name='published_date' value='<?= t( isset($item['published_date'] ) ? $item['published_date'] : '') ?>'/>
				<input type="hidden" name='offset' value='<?= t( isset($item['published_date'] ) ? $item['published_date'] : '') ?>'/>
				
				<input type="hidden" name='summ_show_blurb' value='<?= t( isset($item['show_blurb_on_list'] ) ? $item['show_blurb_on_list'] : '') ?>' />
				<input type="hidden" name='summ_show_thumb' value='<?= t( isset($item['show_thumb_on_list'] ) ? $item['show_thumb_on_list'] : '') ?>' />
				<input type="hidden" name='summ_show_count' value='<?= t( isset($item['post_num_on_list'] ) ? $item['post_num_on_list'] : '') ?>' />
   */
  $rid = $conf['rid'];
  $query_text = isset($conf['query_text']) ? $conf['query_text'] : '';
  $primary_country = implode("|", array_filter(array_map("get_active_keys", $conf['primary_country'])));
  $country = implode("|", array_filter(array_map("get_active_keys", $conf['country'])));
  $theme = implode("|", array_filter(array_map("get_active_keys", $conf['theme'])));
  $organisation = implode("|", array_filter(array_map("get_active_keys", $conf['organisation'])));
  $content_format = implode("|", array_filter(array_map("get_active_keys", $conf['content_format'])));
  $language =  implode("|", array_filter(array_map("get_active_keys", $conf['language'])));
  

  $show_blurb_on_list = (t( !empty($conf['show_blurb_on_list'])) && (int) $conf['show_blurb_on_list'] ) ? $conf['show_blurb_on_list'] : '';
  $show_thumb_on_list = (t( !empty($conf['show_thumb_on_list'])) && (int) $conf['show_thumb_on_list'] ) ? $conf['show_thumb_on_list'] : '';
  $post_num_on_list = (t( !empty($conf['post_num_on_list'])) && (int) $conf['post_num_on_list'] ) ? $conf['post_num_on_list'] : '';
  
  $show_blurb_on_single = (t( !empty($conf['show_blurb_on_single'])) && (int) $conf['show_blurb_on_single'] ) ? $conf['show_blurb_on_single'] : '';
  $show_thumb_on_single = (t( !empty($conf['show_thumb_on_single'])) && (int) $conf['show_thumb_on_single'] ) ? $conf['show_thumb_on_single'] : '';
  $post_num_on_single = (t( !empty($conf['post_num_on_single'])) && (int) $conf['post_num_on_single'] ) ? $conf['post_num_on_single'] : '';
  $redirect_to_reliefweb = isset($conf['redirect_to_reliefweb']) && ((int) $conf['redirect_to_reliefweb']) ? $conf['redirect_to_reliefweb'] : '';
  $redirect_link = isset($conf['redirect_link']) ? $conf['redirect_link'] : '';
  $redirect_text = isset($conf['redirect_text']) ? $conf['redirect_text'] : 'Next';
  $title_size = isset($conf['title_size']) ? 'title_size_' . $conf['title_size'] : 'title_size_12px';
  
  $show_pagination = isset($conf['show_pagination']) ? (int) $conf['show_pagination'] : 0;
  $list_type = isset($conf['list_type']) ? $conf['list_type'] : 'LIST';
  $title_text_header = isset($conf['title_text_header']) ? $conf['title_text_header'] : NULL;
  $add_border_per_line = isset($conf['add_border_per_line']) ? $conf['add_border_per_line'] : 0;

  $is_frontpage = drupal_is_front_page() ? "yes" : "no";

  $site_base_url = base_path();
  $add_bullets_per_line = isset($conf['add_bullets_per_line']) ? $conf['add_bullets_per_line'] : 0;
  $title_word_limit = isset($conf['title_word_limit']) ? $conf['title_word_limit'] : 0;
  $blurb_word_limit = isset($conf['blurb_word_limit']) ? $conf['blurb_word_limit'] : 0;
  $add_date_per_line = isset($conf['add_date_to_item']) ? $conf['add_date_to_item'] : 0;
  $block->content = <<<END

<div class="reliefwebapi-block-items-filter-list is_frontpage_{$is_frontpage} {$title_size} list_count_{$post_num_on_list} list_type_{$list_type} add_bullets_{$add_bullets_per_line}">
		<div class="rwapi-item-square">
			<form id="rwapi-filters-x" class="rwapi-filter-form" action='/reliefweb/list'>
				<input type="hidden" name='query_text' value='{$query_text}' />
                                <input type="hidden" name='title_text_header' value='{$title_text_header}'/>
                                <input type="hidden" name='add_border_per_line' value='{$add_border_per_line}'/>
                                <input type="hidden" name='add_bullets_per_line' value='{$add_bullets_per_line}'/>
                                <input type="hidden" name='title_word_limit' value='{$title_word_limit}'/>
                                <input type="hidden" name='blurb_word_limit' value='{$blurb_word_limit}'/>
				<input type="hidden" name='add_date_per_line' value='{$add_date_per_line}' />
				<input type="hidden" name='base_url' value='{$site_base_url}' />

				<input type="hidden" name='primary_country' value='{$primary_country}'/>
				<input type="hidden" name='country' value='{$country}'/>
				<input type="hidden" name='theme' value='{$theme}' />
				<input type="hidden" name="organisation" value='{$organisation}' />
				<input type="hidden" name="content_format" value='{$content_format}' />
				<input type="hidden" name="language" value='{$language}' />

				<input type="hidden" name='summ_show_blurb' value='{$show_blurb_on_list}' />
				<input type="hidden" name='summ_show_thumb' value='{$show_thumb_on_list}' />
				<input type="hidden" name='summ_show_count' value='{$post_num_on_list}' />
				
				<input type="hidden" name='sing_show_blurb' value='{$show_blurb_on_single}' />
				<input type="hidden" name='sing_show_thumb' value='{$show_thumb_on_single}' />
				<input type="hidden" name='sing_show_count' value='{$post_num_on_single}' />
				<input type="hidden" name='redirect-to-reliefweb' value='{$redirect_to_reliefweb}' />
				<input type="hidden" name="rid" value="{$rid}" />
				<input type="hidden" name="offset" value="0" />
  			</form>
			<ul class="rwapi-contents">
				<li class='loader-container' style="width: 100%; list-style: none; list-style-type: none;"><img class='loader-image' src='/images/loader.gif'/></li>
			</ul>
			
END;

	if ($show_pagination) {
		$block->content .= '<div class="rwapi-filter-pagination"></div>';
	}
	
	
	#Alignment condition for redirect header 
		$footer_content .= '<div class="rwapi-more-link ' . ($post_num_on_list==1 && $list_type == "SQUARE" ? "rwapi-more-list-single-sq" : "") . ' rwapi-align-right">';
		
		
	if ($redirect_to_reliefweb) {
		$redirect_link = build_rw_redirect_link(array_filter(array_map("get_active_keys", $conf['primary_country'])),
						array_filter(array_map("get_active_keys", $conf['country'])),
						array_filter(array_map("get_active_keys", $conf['theme'])),
						array_filter(array_map("get_active_keys", $conf['organisation'])),
						array_filter(array_map("get_active_keys", $conf['content_format'])),
						array_filter(array_map("get_active_keys", $conf['language']))
				);
						
							
		$footer_content .= <<<END
				<a href="{$redirect_link}" class='_reliefwebapi-submit-form'>{$redirect_text}</a>
				<div style="clear: both;"></div>
			</div>
END;
	} else {
		$footer_content .= <<<END
				<a href="{$redirect_link}" class='reliefwebapi-submit-form'>{$redirect_text}</a>
				<div style="clear: both;"></div>
			</div>
END;
				
	}	
	$block->content .= <<<END
	</div> 

	{$footer_content}
</div>  
END;
  
  /*
  if (!empty($conf)) {
    $block->content .= '<div>The only information that can be displayed in this block comes from the code and its settings form: </div>';
    $block->content .= '<div style="border: 1px solid red;">' . var_export($conf, TRUE). '</div>';
  } */

  return $block;

}

/**
 * 'Edit form' callback for the content type
 * This example just returns a form; validation and submission are standard drupal
 * Note that if we had not provided an entry for this in hook_content_types,
 * this could have had the default name
 * reliefweb_panel_no_context_content_type_edit_form
 *
 */
function no_context_content_type_edit_form(&$form, &$form_state) {
  	$conf = $form_state['conf'];
 	$FOLDER = drupal_get_path('module', 'reliefweb_panel');
	require_once($FOLDER . '/filters/countries.inc');
	require_once($FOLDER . '/filters/content_format.inc');
	require_once($FOLDER . '/filters/organisation.inc');
	require_once($FOLDER . '/filters/theme.inc');
	require_once($FOLDER . '/filters/language.inc');

	drupal_add_css($FOLDER . "/css/edit.css");
    drupal_add_js($FOLDER . "/js/edit.js");

  $form['query_text'] = array(
         '#type' => 'textfield',
         '#title' => 'Query String',
         '#default_value' => !empty($conf['query_text']) ? $conf['query_text'] : ''
  );

   $form['primary_country'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose Primary Country'),
 #   '#description' => t('The setting for item 1.'),
    '#default_value' =>  count($conf['primary_country']) ? array_map("get_active_keys", is_array($conf['primary_country']) ? $conf['primary_country']: array($conf['primary_country']) ) : array(),
    '#options' => $reliefweb_country,
    '#prefix' => '<div class="clear-block no-float theme-checkbox rw-parent-div">
    				<h2>Primary Country</h2>
    				<div class="filter-input-area"><input type="text" class="text-filter" name="text-filter" placeholder="Filter"/></div>
    				<div class="rw-chosen-item"></div><div style="clear: both"></div>
    			',
    '#suffix' => '</div>',
  );
   $form['country'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose Countries'),
 #   '#size' => 50,
 #   '#description' => t('The setting for item 1.'),
    '#default_value' => count($conf['country']) ? array_map("get_active_keys", $conf['country']) : array(),
    '#options' => $reliefweb_country,
    '#prefix' => '<div class="clear-block no-float country-checkbox rw-parent-div">
    				<h2>Country</h2>
    				<div class="filter-input-area"><input type="text" class="text-filter" name="text-filter" placeholder="Filter"/></div>
    				<div class="rw-chosen-item"></div><div style="clear: both"></div>',
    '#suffix' => '</div><div style="clear: both"></div>',
  );
  
  $form['theme'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose Themes'),
 #   '#size' => 50,
 #   '#description' => t('The setting for item 1.'),
    '#default_value' => count($conf['theme']) ? array_map("get_active_keys", $conf['theme']) : array(),
    '#options' => $reliefweb_theme,
    '#prefix' => '<div class="clear-block no-float theme-checkbox rw-parent-div">
    				<h2>Theme</h2>
    				<div class="filter-input-area"><input type="text" class="text-filter" name="text-filter" placeholder="Filter"/></div>
    				<div class="rw-chosen-item"></div><div style="clear: both"></div>',
    '#suffix' => '</div>',
  );
 
  $form['language'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose Languages'),
 #   '#size' => 50,
 #   '#description' => t('The setting for item 1.'),
    '#default_value' => count($conf['language']) ? array_map("get_active_keys", $conf['language']) : array(),
    '#options' => $reliefweb_language,
    '#prefix' => '<div class="clear-block no-float theme-checkbox rw-parent-div">
                                <h2>Language</h2>
                                <div class="filter-input-area"><input type="text" class="text-filter" name="text-filter" placeholder="Filter"/></div>
                                <div class="rw-chosen-item"></div><div style="clear: both"></div>',
    '#suffix' => '</div><div style="clear: both"></div>',
  );


  $form['organisation'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose Info Source'),
 #   '#size' => 50,
 #   '#description' => t('The setting for item 1.'),
    '#default_value' => count($conf['organisation']) ? array_map("get_active_keys", $conf['organisation']) : array(),
    '#options' => $reliefweb_organisation,
    '#prefix' => '<div class="clear-block no-float source-checkbox rw-parent-div">
    				<h2>Source</h2>
    				<div class="filter-input-area"><input type="text" class="text-filter" name="text-filter" placeholder="Filter"/></div>
    				<div class="rw-chosen-item"></div><div style="clear: both"></div>',
    '#suffix' => '</div>',
  );
  
  $form['content_format'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Choose Format'),
 #   '#size' => 50,
 #   '#description' => t('The setting for item 1.'),
 
    '#default_value' => !empty($conf['content_format']) ? array_map("get_active_keys", $conf['content_format']) : array(),
    '#options' => $reliefweb_content_format,
    '#prefix' => '<div class="clear-block no-float content-format-checkbox rw-parent-div">
    				<h2>Content Format</h2>
    				<div class="filter-input-area"><input type="text" class="text-filter" name="text-filter" placeholder="Filter"/></div>
    				<div class="rw-chosen-item"></div><div style="clear: both"></div>',
    '#suffix' => '</div><div style="clear: both"></div>

		<script type="text/javascript">$jq(function($) { $(document).trigger("reliefweb-load-chosen-items"); });</script>',
  );
  
  $form['show_blurb_on_list'] = array(
   '#type' => 'radios',
    '#title' => t('Show Blurb on Summary View'),
 #   '#size' => 50,
 #   '#description' => t('The setting for item 1.'),
    '#default_value' => !empty($conf['show_blurb_on_list']) ? $conf['show_blurb_on_list'] : '0',
    '#options' => array(
    	'1' => t('Yes'),
    	'0' => t('No'),
    ),
    '#prefix' => '<div class="clear-block no-float show-blurb-radio rw-yes-no">',
    '#suffix' => '</div>',
  );
  
  $form['show_thumb_on_list'] = array(
   '#type' => 'radios',
    '#title' => t('Show Thumbnail on Summary View'),
 #   '#size' => 50,
 #   '#description' => t('The setting for item 1.'),
    '#default_value' => !empty($conf['show_thumb_on_list']) ? $conf['show_thumb_on_list'] : '0',
    '#options' => array(
    	'1' => t('Yes'),
    	'0' => t('No'),
    ),
    '#prefix' => '<div class="clear-block no-float show-thumb-radio rw-yes-no">',
    '#suffix' => '</div>',
  );
  
  $form['post_num_on_list'] = array(
   '#type' => 'select',
    '#title' => t('Number of Items in Summary View'),
 #   '#size' => 50,
 #   '#description' => t('The setting for item 1.'),
    '#default_value' => !empty($conf['post_num_on_list']) ? $conf['post_num_on_list'] : '1',
    '#options' => array(
    	'1' => 1,
    	'2' => 2,
    	'3' => 3,
    	'4' => 4,
    	'5' => 5,
	'6' => 6,
	'7' => 7,
	'8' => 8,
	'9' => 9,
    	'10' => 10,
    	'15' => 15,
    	'20' => 20,
    	'25' => 25,
    	'30' => 30,
    ),
    '#prefix' => '<div class="clear-block no-float post-num-on-summary rw-yes-no">',
    '#suffix' => '</div>',
  );
  
  $form['list_type'] = array(
 	'#type' => 'radios',
 	'#title' => t('Choose if it should be list or square type'), 
 	'#default_value' => !empty($conf['list_type']) ? $conf['list_type'] : 'SQUARE',
 	'#options' => array(
 		'LIST' => t("List View"),
 	 	'SQUARE' => t("Square View") 
	)
  );
  
   $form['show_pagination'] = array(
   '#type' => 'radios',
    '#title' => t('Show Pagination?'),
 #   '#size' => 50,
 #   '#description' => t('The setting for item 1.'),
    '#default_value' => !empty($conf['show_pagination']) ? $conf['show_pagination'] : '0',
    '#options' => array(
    	'1' => t('Yes'),
    	'0' => t('No'),
    ),
    '#prefix' => '<div class="clear-block no-float show-thumb-radio rw-yes-no">',
    '#suffix' => '</div>',
  );
   
    $form['show_blurb_on_single'] = array(
   '#type' => 'radios',
    '#title' => t('Show Blurb in Filter View'),
 #   '#size' => 50,
 #   '#description' => t('The setting for item 1.'),
    '#default_value' => !empty($conf['show_blurb_on_single']) ? $conf['show_blurb_on_single'] : '0',
    '#options' => array(
    	'1' => t('Yes'),
    	'0' => t('No'),
    ),
    '#prefix' => '<div class="clear-block no-float show-blurb-on-single rw-yes-no">',
    '#suffix' => '</div>',
  );
  
  
    $form['show_thumb_on_single'] = array(
   '#type' => 'radios',
    '#title' => t('Show Thumbnail in Filter View'),
 #   '#size' => 50,
 #   '#description' => t('The setting for item 1.'),
    '#default_value' => !empty($conf['show_thumb_on_single']) ? $conf['show_thumb_on_single'] : '0',
    '#options' => array(
    	'1' => t('Yes'),
    	'0' => t('No'),
    ),
    '#prefix' => '<div class="clear-block no-float show-thumb-on-single rw-yes-no">',
    '#suffix' => '</div>',
  );
  
  
    $form['post_num_on_single'] = array(
   '#type' => 'select',
    '#title' => t('Number of Posts in Filter View'),
 #   '#size' => 50,
 #   '#description' => t('The setting for item 1.'),
    '#default_value' => !empty($conf['post_num_on_single']) ? $conf['post_num_on_single'] : '5',
    '#options' => array(
    	'5' => 5,
    	'10' => 10,
    	'15' => 15,
    	'20' => 20,
    	'25' => 25,
    	'30' => 30,
    ),
    '#prefix' => '<div class="clear-block no-float post-num-on-single rw-yes-no">',
    '#suffix' => '</div>',
  );
  
  $form['title_size'] = array(
  	'#type' => 'select', 
  	'#title' => t('Size for the title'),
  	'#default_value' => !empty($conf['title_size']) ? $conf['title_size'] : '12px', 
  	'#options' => array(
		'10px' => '10px',
    	'12px' => '12px',
    	'18px' => '18px',
    	'24px' => '24px',
    	'32px' => '32px',
    	'48px' => '48px'
	), 
	#'#prefix' => '<div class="clear-block no-float title_size">',
    #'#suffix' => '</div>',
  );
  
  $form['title_text_header'] = array ("#type" => 'textfield',
        '#title' => 'Header for the title link',
        '#default_value' => $conf['title_text_header'],
	'#placeholder' => "Enter title text here"
  );

 /* $form['redirect_text_header'] = array("#type" => 'textfield',
  	'#title' => 'Header for the link', 
  	'#default_value' => !empty($conf['redirect_text_header']) ? $conf['redirect_text_header'] : ''
  );*/

  $form['redirect_text'] = array('#type' => 'textfield', 
  	'#title' => 'Text to use for the redirection link',
	'#default_value' => !empty($conf['redirect_text']) ? $conf['redirect_text'] : 'More >>');
  
  $form['redirect_to_reliefweb'] = array(
	  '#type' => 'radios',
	    '#title' => t('Redirect to ReliefWeb page'),
	 #   '#size' => 50,
	 #   '#description' => t('The setting for item 1.'),
	    '#default_value' => !empty($conf['redirect_to_reliefweb']) ? $conf['redirect_to_reliefweb'] : 0,
	    '#options' => array(
	    	'1' => t('Yes'),
	    	'0' => t('No'),
	    ),
	    '#prefix' => '<div class="clear-block no-float redirect_to_reliefweb-radio rw-yes-no">',
	    '#suffix' => '</div>',
  );
  $form['redirect_link'] = array(
        '#type' => 'textfield',
        '#title' => t('URL Where to redirect link'),
        '#default_value' => !empty($conf['redirect_link']) ? $conf['redirect_link'] : ''
  );

  $form['add_bullets_per_line'] =array(
  	 '#type' => 'radios',
            '#title' => t('Add Bullets Per Line'),
         #   '#size' => 50,
         #   '#description' => t('The setting for item 1.'),
            '#default_value' => !empty($conf['add_bullets_per_line']) ? $conf['add_bullets_per_line'] : 0,
            '#options' => array(
                '1' => t('Yes'),
                '0' => t('No'),
            ),
            '#prefix' => '<div class="clear-block no-float redirect_to_reliefweb-radio rw-yes-no">',
            '#suffix' => '</div>',
  );

  $form['title_word_limit'] = array(
         '#type' => 'textfield',
         '#title' => 'Character limit per title',
         '#default_value' => !empty($conf['title_word_limit']) ? $conf['title_word_limit'] : 100
  );

  $form['blurb_word_limit'] = array(
         '#type' => 'textfield',
         '#title' => 'Character limit per blurb',
         '#default_value' => !empty($conf['blurb_word_limit']) ? $conf['blurb_word_limit'] : 400
  );


  $form['add_border_per_line'] = array(
          '#type' => 'radios',
            '#title' => t('Add Border Per Line'),
         #   '#size' => 50,
         #   '#description' => t('The setting for item 1.'),
            '#default_value' => !empty($conf['add_border_per_line']) ? $conf['add_border_per_line'] : 0,
            '#options' => array(
                '1' => t('Yes'),
                '0' => t('No'),
            ),
            '#prefix' => '<div class="clear-block no-float redirect_to_reliefweb-radio rw-yes-no">',
            '#suffix' => '</div>',
  );

  $form['add_date_to_item'] = array(
          '#type' => 'radios',
            '#title' => t('Add Date per Line'),
         #   '#size' => 50,
         #   '#description' => t('The setting for item 1.'),
            '#default_value' => !empty($conf['add_date_to_item']) ? $conf['add_date_to_item'] : 0,
            '#options' => array(
                '1' => t('Yes'),
                '0' => t('No'),
            ),
            '#prefix' => '<div class="clear-block no-float redirect_to_reliefweb-radio rw-yes-no">',
            '#suffix' => '</div>',
  );

  $form['rid'] = array(
  	'#type' => 'hidden',
  	'#default_value' => !empty($conf['rid']) ? $conf['rid'] : '0' 
  );
  

  return $form;
}

function no_context_content_type_edit_form_submit(&$form, &$form_state) {

  $rid = $form_state['values']['rid'];
  
  $query_text = $form_state['values']['query_text'];
  $primary_country = implode('|', array_filter($form_state['values']['primary_country'], 'get_active_keys_from_array'));
  $name = $form['override_title_text'] ? $form['override_title_text']:"Filter";
  $country = implode('|', array_filter($form_state['values']['country'], 'get_active_keys_from_array'));
  $content_format = implode('|', array_filter($form_state['values']['content_format'], 'get_active_keys_from_array'));
  $organisation = implode('|', array_filter($form_state['values']['organisation'], 'get_active_keys_from_array'));
  $theme = implode('|', array_filter($form_state['values']['theme'], 'get_active_keys_from_array'));
  $language = implode('|', array_filter($form_state['values']['theme'], 'get_active_keys_from_array'));

  $show_blurb_on_list = $form_state['values']['show_blurb_on_list'];
  $show_thumb_on_list = $form_state['values']['show_thumb_on_list'];
  $post_num_on_list = $form_state['values']['post_num_on_list'];
  $post_num_on_single = $form_state['values']['post_num_on_single'];
  $show_thumb_on_single = $form_state['values']['show_thumb_on_single'];
  $show_blurb_on_single = $form_state['values']['show_blurb_on_single'];
  $redirect_to_reliefweb = $form_state['values']['redirect_to_reliefweb'];
  
  if ((int) $rid > 0) {
  	$query = "UPDATE {reliefweb} SET 
  				{name}='%s',
  				{primary_country}='%s', 
  				{country}='%s',
  				{content_format}='%s',
  				{theme}='%s',
  				{organisation}='%s',
				{language}='%s',
  				{show_blurb_on_list}='%s',
  				{show_thumb_on_list}='%s',
  				{post_num_on_list}='%s',
  				{post_num_on_single}='%s',
  				{show_thumb_on_single}='%s',
  				{show_blurb_on_single}='%s',
  				{redirect_to_reliefweb}='%s'
  				
  				WHERE {rid}='%d'
  				";
  	db_query($query, 
  		"Filter",
	  	$primary_country, 
	  	$country, 
	  	$content_format,
	  	$theme, 
	  	$organisation, 
		$language,
	  	$show_blurb_on_list, 
	  	$show_thumb_on_list, 
	  	$post_num_on_list, 
	  	$post_num_on_single, 
	  	$show_thumb_on_single, 
	  	$show_blurb_on_single, 
	  	$redirect_to_reliefweb, 
	  	$rid);	
  } else {
  	$insert_data = 
  		array(
  			'primary_country' => $primary_country, 
  			'country' => $country, 
  			'organisation' => $organisation,
			'language' => $language,
  			'content_format' => $content_format, 
  			'show_blurb_on_list' => $show_blurb_on_list, 
  			'show_thumb_on_list' => $show_thumb_on_list, 
  			'post_num_on_list' => $post_num_on_list, 
			'post_num_on_single' => $post_num_on_single, 
			'show_thumb_on_single' => $show_thumb_on_single, 
			'show_blurb_on_single' => $show_blurb_on_single, 
			'redirect_to_reliefweb' => $redirect_to_reliefweb
		);
		$query = "INSERT INTO {reliefweb} ({name}, {primary_country}, {country}, 
					{content_format}, {theme}, {organisation}, {language}, {show_blurb_on_list}, 
					{show_thumb_on_list}, {post_num_on_list}, 
					{post_num_on_single}, {show_thumb_on_single}, 
					{show_blurb_on_single}, {redirect_to_reliefweb})
				  VALUES 
				  	('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')";
							
		db_query($query, "Filter", $primary_country, $country, $content_format, $theme, $organisation, $language,
		$show_blurb_on_list, 
		$show_thumb_on_list, 
		$post_num_on_list, 
	  	$post_num_on_single, 
	  	$show_thumb_on_single, 
	  	$show_blurb_on_single, 
	  	$redirect_to_reliefweb);
		
		
		#error_log(print_r($insert_data, true));
		#drupal_write_record('reliefweb', $insert_data);
	
	$reliefweb_id = 0;
	$result = db_query("SELECT LAST_INSERT_ID() as `last_insert_id`;");
	if ($rid = db_result($result)) {
	}else {
	   $rid = 0;
	}
  }	
  

  $form_state['conf']['rid'] = $rid;

  foreach (array('query_text', 'primary_country', 'country', 'content_format', 'language',
  				'show_blurb_on_list', 'show_thumb_on_list', 'post_num_on_list', 
				'post_num_on_single', 'show_thumb_on_single', 'show_blurb_on_single', 
				'redirect_to_reliefweb', 'organisation', 'theme', 'redirect_text', 'redirect_link', 
				'redirect_text_header', 'list_type', 'show_pagination', 'title_text_header', 'title_size',
				'add_bullets_per_line', 'title_word_limit', 'blurb_word_limit', 'add_border_per_line', 'add_date_to_item') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
    
  }
  
  //Save ID:
  $form_state['conf']['rid'] = $rid;
}

/**
 * This function returns the chosen items..
 */
function get_active_keys($arr) {
	$ret = explode(",", $arr);
	if ($ret[0]) {
		return $ret[0];
	}
}


function get_active_keys_from_array($data) {
	return $data != "0"; 
}

function build_rw_redirect_link($primary_country, $country, $theme, $organisation, $content_format, $language) {
        $FOLDER = drupal_get_path('module', 'reliefweb_panel');
        require($FOLDER . '/filters/country_lookup.inc');
        require($FOLDER . '/filters/format_lookup.inc');
        require($FOLDER . '/filters/source_lookup.inc');
        require($FOLDER . '/filters/theme_lookup.inc');
        require($FOLDER . '/filters/language_lookup.inc');
	
	$pc_filter = array();
	$c_filter = array();
	$t_filter = array();
	$s_filter = array();
	$f_filter = array();
	$l_filter = array();

	foreach ($primary_country as $pc) {
		$pc_filter[] = $country_lookup[$pc];
	}

	foreach ($country as $c) {
		$c_filter[] = $country_lookup[$c];
	}

	foreach ($theme as $t) {
		$t_filter[] = $theme_lookup[$t];
	}

	foreach ($organisation as $s) {
		$s_filter[] = $source_lookup[$s];
	}

	foreach ($content_format as $f) {
		$f_filter[] = $format_lookup[$f];
	}

	foreach ($language as $l) {
		$l_filter = $language_lookup[$l];
	}

	$filters = array("entity_name" => "node_report", 
		"filters" => array(
			"field_primary_country" => array_map("in_or_filter", $pc_filter),
			"field_country" => array_map("in_or_filter", $c_filter),
			"field_source" => array_map("in_or_filter", $s_filter), 
			"field_theme" => array_map("in_or_filter", $t_filter), 
			"field_language" => array_map("in_or_filter", $l_filter),
			"field_content_format" => array_map("in_or_filter", $f_filter)
		)
	);

	return "http://reliefweb.int/updates?extended_search=" . urlencode(json_encode($filters));
}

function in_or_filter($data){
	return array("IN|OR|", "" . $data);
}
