<?php
/**
 * Default panels pages for Ctools Plugin Example
 *
 * To pick up this file, your module needs to implement
 *
 *
 * Note the naming of the file: <modulename>.pages_default.inc
 * With this naming, no additional code needs to be provided. Ctools will just find the file.
 * The name of the hook is <modulename>_default_page_manager_pages()
 *
 * Although this example only provides one page, the returned array could
 * have several pages.
 *
 * @return none
 */

function reliefwebpanel_default_page_manager_pages() {
  $page = new stdClass;
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'reliefwebpanel';
  $page->task = 'page';
  $page->admin_title = 'ReliefWeb API Panel';
  $page->admin_description = 'This panel allows the user to add ReliefWeb windows onto a panel.';
  $page->path = 'reliefwebpanel/%sc';
  $page->access = array(
  'logic' => 'and',
  );
  $page->menu = array(
  'type' => 'normal',
  'title' => 'ReliefWeb_1 Plugin Example',
  'name' => 'navigation',
  'weight' => '0',
  'parent' => array(
    'type' => 'none',
    'title' => '',
    'name' => 'navigation',
    'weight' => '0',
  ),
  );
  $page->arguments = array(
  'sc' => array(
    'id' => 2,
    'identifier' => 'simplecontext-arg',
    'name' => 'simplecontext_arg',
    'settings' => array(),
  ),
  );
  $page->conf = array();
  $page->default_handlers = array();
  $handler = new stdClass;
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_ctools_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'ctools_plugin_example';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
  'title' => 'Panel',
  'no_blocks' => FALSE,
  'css_id' => '',
  'css' => '',
  'contexts' => array(
    '0' => array(
      'name' => 'simplecontext',
      'id' => 1,
      'identifier' => 'Configured Simplecontext (not from argument)',
      'keyword' => 'configured_simplecontext',
      'context_settings' => array(
        'sample_simplecontext_setting' => 'default simplecontext setting',
  ),
  ),
  ),
  'relationships' => array(
    '0' => array(
      'context' => 'argument_simplecontext_arg_2',
      'name' => 'relcontext_from_simplecontext',
      'id' => 1,
      'identifier' => 'Relcontext from Simplecontext (from relationship)',
      'keyword' => 'relcontext',
  ),
  ),
  'access' => array(
    'logic' => 'and',
  ),
  );
  $display = new panels_display;
  $display->layout = 'threecol_33_34_33_stacked';
  $display->layout_settings = array();
  $display->panel_settings = array(
  'style' => 'rounded_corners',
  'style_settings' => array(
    'default' => array(
      'corner_location' => 'pane',
  ),
  ),
  );
  $display->cache = array();
  $display->title = 'Ctools Plugin Example Panel';
  $display->hide_title = FALSE;
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass;
  $pane->pid = 'new-1';
  $pane->panel = 'left';
  $pane->type = 'no_context_content_type';
  $pane->subtype = 'no_context_content_type';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'item1' => 'contents of config item 1',
    'item2' => 'contents of config item 2',
    'override_title' => 0,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $display->content['new-1'] = $pane;
  $display->panels['left'][0] = 'new-1';
  $pane = new stdClass;
  $pane->pid = 'new-2';
  $pane->panel = 'left';
  $pane->type = 'custom';
  $pane->subtype = 'custom';
  $pane->shown = TRUE;
  $pane->access = array(
    'plugins' => array(
      '0' => array(
        'name' => 'arg_length',
        'settings' => array(
          'greater_than' => '1',
          'arg_length' => '4',
  ),
        'context' => 'argument_simplecontext_arg_2',
  ),
  ),
  );
  $pane->configuration = array(
    'title' => 'Long Arg Visibility Block',
    'body' => 'This block will be here when the argument is longer than configured arg length. It uses the \'arg_length\' access plugin to test against the length of the argument used for Simplecontext.',
    'format' => '1',
    'substitute' => 1,
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $display->content['new-2'] = $pane;
  $display->panels['left'][1] = 'new-2';
  $pane = new stdClass;
  $pane->pid = 'new-3';
  $pane->panel = 'left';
  $pane->type = 'custom';
  $pane->subtype = 'custom';
  $pane->shown = TRUE;
  $pane->access = array(
    'plugins' => array(
      '0' => array(
        'name' => 'arg_length',
        'settings' => array(
          'greater_than' => '0',
          'arg_length' => '4',
  ),
        'context' => 'argument_simplecontext_arg_2',
  ),
  ),
  );
  $pane->configuration = array(
    'title' => 'Short Arg Visibility',
    'body' => 'This block appears when the simplecontext argument is <i>less than</i> the configured length.',
    'format' => '1',
    'substitute' => 1,
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $display->content['new-3'] = $pane;
  $display->panels['left'][2] = 'new-3';
  $pane = new stdClass;
  $pane->pid = 'new-4';
  $pane->panel = 'middle';
  $pane->type = 'simplecontext_content_type';
  $pane->subtype = 'simplecontext_content_type';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'buttons' => NULL,
    '#validate' => NULL,
    '#submit' => NULL,
    '#action' => NULL,
    'context' => 'argument_simplecontext_arg_2',
    'aligner_start' => NULL,
    'override_title' => 1,
    'override_title_text' => 'Simplecontext (with an arg)',
    'aligner_stop' => NULL,
    'override_title_markup' => NULL,
    'config_item_1' => 'Config item 1 contents',
    '#build_id' => NULL,
    '#type' => NULL,
    '#programmed' => NULL,
    'form_build_id' => 'form-19c4ae6cb54fad8f096da46e95694e5a',
    '#token' => NULL,
    'form_token' => '17141d3531eaa7b609da78afa6f3b560',
    'form_id' => 'simplecontext_content_type_edit_form',
    '#id' => NULL,
    '#description' => NULL,
    '#attributes' => NULL,
    '#required' => NULL,
    '#tree' => NULL,
    '#parents' => NULL,
    '#method' => NULL,
    '#post' => NULL,
    '#processed' => NULL,
    '#defaults_loaded' => NULL,
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $display->content['new-4'] = $pane;
  $display->panels['middle'][0] = 'new-4';
  $pane = new stdClass;
  $pane->pid = 'new-5';
  $pane->panel = 'middle';
  $pane->type = 'simplecontext_content_type';
  $pane->subtype = 'simplecontext_content_type';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'buttons' => NULL,
    '#validate' => NULL,
    '#submit' => NULL,
    '#action' => NULL,
    'context' => 'context_simplecontext_1',
    'aligner_start' => NULL,
    'override_title' => 1,
    'override_title_text' => 'Configured simplecontext content type (not from arg)',
    'aligner_stop' => NULL,
    'override_title_markup' => NULL,
    'config_item_1' => '(configuration for simplecontext)',
    '#build_id' => NULL,
    '#type' => NULL,
    '#programmed' => NULL,
    'form_build_id' => 'form-d016200490abd015dc5b8a7e366d76ea',
    '#token' => NULL,
    'form_token' => '17141d3531eaa7b609da78afa6f3b560',
    'form_id' => 'simplecontext_content_type_edit_form',
    '#id' => NULL,
    '#description' => NULL,
    '#attributes' => NULL,
    '#required' => NULL,
    '#tree' => NULL,
    '#parents' => NULL,
    '#method' => NULL,
    '#post' => NULL,
    '#processed' => NULL,
    '#defaults_loaded' => NULL,
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $display->content['new-5'] = $pane;
  $display->panels['middle'][1] = 'new-5';
  $pane = new stdClass;
  $pane->pid = 'new-6';
  $pane->panel = 'right';
  $pane->type = 'relcontext_content_type';
  $pane->subtype = 'relcontext_content_type';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'buttons' => NULL,
    '#validate' => NULL,
    '#submit' => NULL,
    '#action' => NULL,
    'context' => 'relationship_relcontext_from_simplecontext_1',
    'aligner_start' => NULL,
    'override_title' => 0,
    'override_title_text' => '',
    'aligner_stop' => NULL,
    'override_title_markup' => NULL,
    'config_item_1' => 'some stuff in this one',
    '#build_id' => NULL,
    '#type' => NULL,
    '#programmed' => NULL,
    'form_build_id' => 'form-8485f84511bd06e51b4a48e998448054',
    '#token' => NULL,
    'form_token' => '1c3356396374d51d7d2531a10fd25310',
    'form_id' => 'relcontext_edit_form',
    '#id' => NULL,
    '#description' => NULL,
    '#attributes' => NULL,
    '#required' => NULL,
    '#tree' => NULL,
    '#parents' => NULL,
    '#method' => NULL,
    '#post' => NULL,
    '#processed' => NULL,
    '#defaults_loaded' => NULL,
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $display->content['new-6'] = $pane;
  $display->panels['right'][0] = 'new-6';
  $pane = new stdClass;
  $pane->pid = 'new-7';
  $pane->panel = 'top';
  $pane->type = 'custom';
  $pane->subtype = 'custom';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'title' => 'Demonstrating Ctools Plugins',
    'body' => 'The Ctools Plugin Example module (and this panel page) are just here to demonstrate how to build Ctools plugins.

        ',
    'format' => '2',
    'substitute' => 1,
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $display->content['new-7'] = $pane;
  $display->panels['top'][0] = 'new-7';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;

  $pages['ctools_plugin_example_demo_page'] = $page;
  /* ---------------------------- */

  $page = new stdClass;
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'ctools_plugin_example_base';
  $page->task = 'page';
  $page->admin_title = 'Ctools Plugin Example Base Page';
  $page->admin_description = 'This panel is for when people hit /ctools_plugin_example without an argument. We can use it to tell people to move on.';
  $page->path = 'ctools_plugin_example';
  $page->access = array();
  $page->menu = array();
  $page->arguments = array();
  $page->conf = array();
  $page->default_handlers = array();
  $handler = new stdClass;
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_reliefwebpanel_base_panel_context';
  $handler->task = 'page';
  #$handler->subtask = 'ctools_plugin_example_base';
  $handler->subtask = 'reliefwebpanel_base';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
  'title' => 'Panel',
  'no_blocks' => FALSE,
  'css_id' => '',
  'css' => '',
  'contexts' => array(),
  'relationships' => array(),
  );
  $display = new panels_display;
  $display->layout = 'onecol';
  $display->layout_settings = array();
  $display->panel_settings = array();
  $display->cache = array();
  $display->title = '';
  $display->hide_title = FALSE;
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass;
  $pane->pid = 'new-1';
  $pane->panel = 'middle';
  $pane->type = 'custom';
  $pane->subtype = 'custom';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'title' => 'Use this page with an argument',
    'body' => 'This demo page works if you use an argument, like <a href="ctools_plugin_example/xxxxx">ctools_plugin_example/xxxxx</a>.',
    'format' => '1',
    'substitute' => NULL,
  );
  $pane->cache = array();
  $pane->style = array();
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $display->content['new-1'] = $pane;
  $display->panels['middle'][0] = 'new-1';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['base_page'] = $page;




  return $pages;
}