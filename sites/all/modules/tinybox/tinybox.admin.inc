<?php
/**
 * @file
 * Settings page callback file for the tinybox module.
 */

function tinybox_settings() {
  $tinybox_lib_path = variable_get('tinybox_lib_path', 'sites/all/libraries/tinybox');
  
  if (!file_exists($tinybox_lib_path . '/tinybox.js')) {
    drupal_set_message(check_plain('Can not find: ' . $tinybox_lib_path . '/tinybox.js. You must upload TinyBox Libariy to: ' . $tinybox_lib_path), 'error');
  }

  $form['general'] = array(
    '#type' => 'fieldset',
    '#weight' => -20,
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['general']['tinybox_lib_path'] = array(
    '#type' => 'textfield',
    '#title' => t('TinyBox Library Path'),
    '#default_value' => variable_get('tinybox_lib_path', 'sites/all/libraries/tinybox'),
    '#description' => t('You must download TinyBox library from: <a href="http://www.scriptiny.com/2011/03/javascript-modal-windows/" target="_blank">TinyBox Library download</a>.'),
  );

  $form['tinybox_splash'] = array(
    '#type' => 'fieldset',
    '#weight' => -10,
    '#title' => t('TinyBox Splash'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  $form['tinybox_splash']['tinybox_splash_source_type'] = array(
    '#type' => 'select',
    '#title' => t('Splash Source Type'),
    '#options' => array(
      'nid' => 'NID',
      'content_type' => 'Content Type',
      'views' => 'Views',
      ),
    '#default_value' => variable_get('tinybox_splash_source_type', 'nid')
  );
  
  $form['tinybox_splash']['tinybox_splash_source_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Splash Source Value'),
    '#default_value' => variable_get('tinybox_splash_source_value', ''),
    '#description' => t('Leave blank to disable Splash. Enter NID, Content Type or Views name.'),
  );

  $form['tinybox_splash']['tinybox_splash_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Splash Width'),
    '#default_value' => variable_get('tinybox_splash_width', ''),
    '#description' => t('Leave blank for auto width.'),
  );
  
  $form['tinybox_splash']['tinybox_splash_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Splash Height'),
    '#default_value' => variable_get('tinybox_splash_height', ''),
    '#description' => t('Leave blank for auto height.'),
  );
  
  $form['tinybox_splash']['tinybox_splash_autohide'] = array(
    '#type' => 'textfield',
    '#title' => t('Splash Autohide'),
    '#default_value' => variable_get('tinybox_splash_autohide', 0),
    '#description' => t('Set 0 to disable. Autohide Splash Screen after certain seconds.'),
  );
  
  $form['tinybox_splash']['tinybox_splash_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Splash Delay'),
    '#default_value' => variable_get('tinybox_splash_delay', 0),
    '#description' => t('Set 0 to disable. Delay in second before next display. If you set 3600 then Splash Screen will be show again after 3600 seconds.'),
  );
  
  $form['tinybox_splash']['tinybox_splash_css'] = array(
    '#type' => 'textfield',
    '#title' => t('Tinybox custom CSS'),
    '#default_value' => variable_get('tinybox_splash_css', ''),
    '#description' => t('Leave blank for default.'),
  );
  
  return system_settings_form($form);
}

