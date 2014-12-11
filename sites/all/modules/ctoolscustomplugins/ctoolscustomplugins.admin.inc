<?php
// $Id$

function ctoolscustomplugins_admin_settings() {
  $ctoolscustomplugins_upload_extensions_default = variable_get('ctoolscustomplugins_upload_extensions_default', 'jpg jpeg gif png txt doc xls pdf ppt pps odt ods odp');
  $ctoolscustomplugins_upload_uploadsize_default = variable_get('ctoolscustomplugins_upload_uploadsize_default', 5);

  $form['settings_general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
    '#collapsible' => TRUE,
  );
  $form['settings_general']['ctoolscustomplugins_upload_extensions_default'] = array(
    '#type' => 'textfield',
    '#title' => t('Default permitted file extensions'),
    '#default_value' => $ctoolscustomplugins_upload_extensions_default,
    '#maxlength' => 255,
    '#description' => t('Default extensions that users can upload. Separate extensions with a space and do not include the leading dot.'),
  );
  $form['settings_general']['ctoolscustomplugins_upload_uploadsize_default'] = array(
    '#type' => 'textfield',
    '#title' => t('Default maximum file size per upload'),
    '#default_value' => $ctoolscustomplugins_upload_uploadsize_default,
    '#size' => 5,
    '#maxlength' => 5,
    '#description' => t('The default maximum file size a user can upload. If an image is uploaded and a maximum resolution is set, the size will be checked after the file has been resized.'),
    '#field_suffix' => t('MB'),
  );
  
  $form['#validate'] = array('ctoolscustomplugins_upload_admin_settings_validate');
 
  return system_settings_form($form);
}

function ctoolscustomplugins_upload_admin_settings_validate($form, &$form_state) {
 $default_uploadsize = $form_state['values']['ctoolscustomplugins_upload_uploadsize_default'];

 $exceed_max_msg = t('Your PHP settings limit the maximum file size per upload to %size.', array('%size' => format_size(file_upload_max_size()))) .'<br/>';
 $more_info = t("Depending on your server environment, these settings may be changed in the system-wide php.ini file, a php.ini file in your Drupal root directory, in your Drupal site's settings.php file, or in the .htaccess file in your Drupal root directory.");

 if (!is_numeric($default_uploadsize) || ($default_uploadsize <= 0)) {
    form_set_error('ctoolscustomplugins_upload_uploadsize_default', t('The %role file size limit must be a number and greater than zero.', array('%role' => t('default'))));
 }

 if ($default_uploadsize * 1024 * 1024 > file_upload_max_size()) {
   form_set_error('upload_uploadsize_default', $exceed_max_msg . $more_info);
   $more_info = '';
 }
}