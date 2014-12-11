<?php
// $Id: role.inc,v 1.5 2009/04/17 19:16:22 merlinofchaos Exp $

/**
 * @file
 * Plugin to provide access control/visibility based on length of
 * simplecontext argument (in URL)
 */

/**
 * Implementation of specially named hook_ctools_access().
 */
function reliefweb_panel_arg_length_ctools_access() {
  $args['arg_length'] = array(
    'title' => t("Arg Length"),
    'description' => t('Control access by length of Simplecontext argument.'),
    'callback' => 'reliefweb_panel_arg_length_ctools_access_check',
    'settings form' => 'reliefweb_panel_arg_length_ctools_access_settings',
    'summary' => 'reliefweb_panel_arg_length_ctools_acesss_summary',
    'required context' => new ctools_context_required(t('Simplecontext'), 'simplecontext'),
  );

  return $args;
}

/**
 * Settings form for the 'by role' access plugin
 */
function reliefweb_panel_arg_length_ctools_access_settings(&$form, &$form_state, $conf) {
  $form['settings']['greater_than'] = array(
    '#type' => 'radios',
    '#title' => t('Grant access if simplecontext argument length is'),
    '#options' => array(1 => t('Greater than'), 0 => t('Less than or equal to')),
    '#default_value' => $conf['greater_than'],
  );

  $form['settings']['arg_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Length of Simplecontext argument'),
    '#size' => 3,
    '#default_value' => $conf['arg_length'],
    '#description' => t('Access/Visibility will be granted based on arg length.'),
  );
}


/**
 * Check for access.
 */
function reliefwebapi_arg_length_ctools_access_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  if (empty($context) || empty($context->data)) {
    return FALSE;
  }

  $compare = ($context->arg_length > $conf['arg_length']);
  if ( ($compare && $conf['greater_than']) || (!$compare && !$conf['greater_than'])) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Provide a summary description based upon the checked roles.
 */
function reliefwebapi_arg_length_ctools_acesss_summary($conf, $context) {

  return t('Simpletext argument must be !comp @length characters',
    array('!comp' => $conf['greater_than'] ? 'greater than' : 'less than or equal to',
      '@length' => $conf['arg_length']));
}

