<?php
// $Id: role.inc,v 1.5 2009/04/17 19:16:22 merlinofchaos Exp $

/**
 * @file
 * Plugin to provide access control based upon role membership.
 * This is directly from the ctools module, but serves as a good
 * example of an access plugin
 */

/**
 * Implementation of specially named hook_ctools_access().
 */
function reliefweb_panel_example_role_ctools_access() {
  $args['example_role'] = array(
    'title' => t("Ctools Example: role"),
    'description' => t('Control access by role.'),
    'callback' => 'reliefweb_panel_example_role_ctools_access_check',
    'default' => array('rids' => array()),
    'settings form' => 'reliefweb_panel_example_role_ctools_access_settings',
    'summary' => 'reliefweb_panel_example_role_ctools_acesss_summary',
    'required context' => new ctools_context_required(t('User'), 'user'),
  );

  return $args;
}

/**
 * Settings form for the 'by role' access plugin
 */
function reliefweb_panel_example_role_ctools_access_settings(&$form, &$form_state, $conf) {
  $form['settings']['rids'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Role'),
    '#default_value' => $conf['rids'],
    '#options' => ctools_get_roles(),
    '#description' => t('Only the checked roles will be granted access.'),
  );
}

/**
 * Compress the roles allowed to the minimum.
 */
function reliefweb_panel_example_role_ctools_access_settings_submit(&$form, &$form_state) {
  $form_state['values']['settings']['rids'] = array_keys(array_filter($form_state['values']['settings']['rids']));
}

/**
 * Check for access.
 */
function reliefweb_panel_example_role_ctools_access_check($conf, $context) {
  // As far as I know there should always be a context at this point, but this
  // is safe.
  if (empty($context) || empty($context->data) || !isset($context->data->roles)) {
    return FALSE;
  }

  $roles = array_keys($context->data->roles);
  $roles[] = $context->data->uid ? DRUPAL_AUTHENTICATED_RID : DRUPAL_ANONYMOUS_RID;
  return (bool) array_intersect($conf['rids'], $roles);
}

/**
 * Provide a summary description based upon the checked roles.
 */
function reliefweb_panel_example_role_ctools_acesss_summary($conf, $context) {
  if (!isset($conf['rids'])) {
    $conf['rids'] = array();
  }
  $roles = ctools_get_roles();

  $names = array();
  foreach (array_filter($conf['rids']) as $rid) {
    $names[] = check_plain($roles[$rid]);
  }

  if (empty($names)) {
    return t('@identifier can have any role', array('@identifier' => $context->identifier));
  }

  return format_plural(count($names), '@identifier must have role "@roles"', '@identifier can be one of "@roles"', array('@roles' => implode(', ', $names), '@identifier' => $context->identifier));
}

