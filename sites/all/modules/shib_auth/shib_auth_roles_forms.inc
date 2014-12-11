<?php
/**
 * @file
 *  Roles manager forms.
 */

/**
 * Generate the shibboleth rule adding form
 *
 * @param $options contains the data, we want to fill the form with
 * @returns the edit form, with the fields already filled in with the elements of the options array
 */
function shib_auth_edit_form($options) {

  $form['shib_auth_new_id'] = array(
    '#title'          => t('Entry id'),
    '#type'           => 'hidden',
    '#default_value'  => $options[0],
  );

  $form['shib_auth_new_attrib'] = array(
    '#title'          => t('Shibboleth attribute name'),
    '#type'           => 'textfield',
    '#default_value'  => $options[1],
    '#require'        => TRUE,
    '#description'    => t('More properly: <b>$_SERVER</b> field name; enable DEBUG mode to list available fields. <br/>Note that it might differ from your users\' fields.'),
  );

  $form['shib_auth_new_regexp'] = array(
    '#title'          => t('Value (regexp)'),
    '#type'           => 'textfield',
    '#default_value'  => $options[2],
    '#require'        => TRUE,
  );

  $roles = user_roles(TRUE);

  $form['shib_auth_roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#default_value' => count($options[3])>1||(count($options[3])==1 && $options[3] != "")?$options[3]:array(),
    '#options' => $roles,
  );

  $form['sticky_markup'] = array(
    '#value' => '<b>Role type:</b>',
  );

  $form['shib_auth_new_sticky'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sticky'),
    '#default_value' => $options[5],
    '#description'    => t('Set the rule to be sticky if you want to save the role(s) permanently to the user\'s profile.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t($options[4]),
  );

  return $form;
}//function shib_auth_edit_form

/**
 * This function enables the administrator to clone an existing rule, this is useful,
 * when we want to create a rule, which is simiral to another one
 * @param id rule identifier
 */
function _shib_auth_clone_rule($id) {
  $rule = db_query("SELECT * FROM {shib_auth} WHERE id = %d", array($id));
  $db_entry = db_fetch_array($rule);
  $db_entry['id'] = NULL;
  $update = array();
  $ret = drupal_write_record('shib_auth', $db_entry, $update);
  if ($ret = SAVED_NEW) {
    drupal_set_message(t('The rule has been successfully cloned.'));
  }
  else {
    drupal_set_message(t('Unexpected error has been detected.'));
  }
  drupal_goto('admin/user/shib_auth/rules');
}//function _shib_auth_clone_rule

/**
 * This function lets the admin to delete an existing rule
 * @param id rule identifier
 */
function _shib_auth_delete_rule($id) {
  db_query("DELETE FROM {shib_auth} WHERE id = %d", array($id));
  drupal_set_message(t('Rule <strong>#@id</strong> has been deleted.', array('@id' => $id)) , 'warning');
  drupal_goto('admin/user/shib_auth/rules');
}//function _shib_auth_delete_rule

/**
 * This function lists all rules, and let the admin to do certain actions with them
 *
 * @returns
 *  HTML table containing the number of rule, attribute, RegExp, role and the actions which can be done with each role.
 */
function _shib_auth_list_rules() {
  $counter = 0;
  $output = NULL;
  $rows = array();

  //create rows
  $rules = db_query("SELECT * FROM {shib_auth}");
  while ($rule = db_fetch_array($rules)) {
    $roles = unserialize($rule['role']);
    $roles_list = '';

    foreach ($roles as $role) {
      if (
      !empty($role)) {
        $roles_list .= shib_auth_get_rolename($role) . ', ';
      }
    }
    //cut off the last ', ' charaters
    $roles_list = drupal_substr($roles_list, 0, -2);

    $rule['sticky'] == 1 ? $sticky = 'Yes' : $sticky = 'No';
    $rows[] = array(
      $counter++,
      $rule['field'],
      urldecode($rule['regexpression']),
      $roles_list,
      $sticky,
      l(t('Clone'), 'admin/user/shib_auth/clone/' . $rule['id']) .' | '.
      l(t('Edit'), 'admin/user/shib_auth/edit/' . $rule['id']) .' | '.
      l(t('Delete'), 'admin/user/shib_auth/delete/' . $rule['id']),
    );
  }
  //create the rule list in HTML table
  $header = array(t('#'), t('Attribute'), t('RegExp'), t('Roles'), t('Sticky'), t('Actions'));
  $output = theme_table($header, $rows);
  if (empty($rows))
    $output .= t('There is no rule in the database<br/>');
  $output .= l(t('Add new rule'), 'admin/user/shib_auth/new');
  return $output;
} //function _shib_auth_list_rules

/**
 * Validates a new rule
 * @param $form - the identifier of the form, which we have just received
 * @param $form_state - the state of the form, which we have just received, including all of the variables
 */
function shib_auth_new_rule_validate($form, &$form_state) {
  if (empty($form_state['values']['shib_auth_new_attrib']))
    form_set_error('shib_auth_new_attrib', t('This element must not be empty'));
  if (empty($form_state['values']['shib_auth_new_regexp'])) {
    form_set_error('shib_auth_new_regexp', t('This element must not be empty'));
  }
}
/**
 * Creates a new rule by calling universal create/edit form
 */
function shib_auth_new_rule() {
  return shib_auth_edit_form(array(0, '', '', '', 'Add rule', 0));
}
/**
 * Creates a new rule, containing he rule name, the server attrubite, the RegExp, and the role names by calling save rule
 */
function shib_auth_new_rule_submit($form, &$form_state) {
  shib_auth_save_rule($form_state, array());
}//function shib_auth_new_rule
/**
 * Validates rule edit
 * @param $form - the identifier of the form, which we have just received
 * @param $form_state - the state of the form, which we have just received, including all of the variables
 */
function shib_auth_edit_rule_validate($form, &$form_state) {
  if (empty($form_state['values']['shib_auth_new_attrib']))
    form_set_error('shib_auth_new_attrib', t('This element must not be empty'));
  if (empty($form_state['values']['shib_auth_new_regexp'])) {
    form_set_error('shib_auth_new_regexp', t('This element must not be empty'));
  }
}
/**
 * Edits a rule, containing he rule name, the server attrubite, the RegExp, and the role names by calling save rule
 */
function shib_auth_edit_rule_submit($form, &$form_state) {
  shib_auth_save_rule($form_state, "id");
}//function shib_auth_new_rule
/**
 * Saves a new rule into database
 * @param $update - decides if it is a new rule (NULL), or we're just modifing one ('id')
 * @param $form_state - the state of the form, which we have just received, including all of the variables
 */
function shib_auth_save_rule($form_state, $update) {
  $new_id = $form_state['values']['shib_auth_new_id'] == '0' ? NULL : (int) $form_state['values']['shib_auth_new_id'];
  // collect ther roles into an array
  $roles = array();
  if (is_array($form_state['values']['shib_auth_roles'])) {
    foreach ($form_state['values']['shib_auth_roles'] as $role_id) {
      if ($role_id)
        $roles[] = $role_id;
    }
  }
    //save the new element into an array
  $new_element = array(
      'id'            => $new_id,
      'field'         => urlencode($form_state['values']['shib_auth_new_attrib']),
      'regexpression' => urlencode($form_state['values']['shib_auth_new_regexp']),
      'role'          => serialize($roles),
      'sticky'        => urlencode($form_state['values']['shib_auth_new_sticky']),
  );
  //write it in a record
  $ret = drupal_write_record('shib_auth', $new_element, $update);
  // if it wasn't an error
  if (empty($update)) {
    if ($ret = SAVED_NEW) {
      drupal_set_message(t('New rule has been stored.'));
    }
    else {
      drupal_set_message(t('Unexpected error has been detected.'));
    }
  }
  //an existing rule was updated
  else {
    if ($ret = SAVED_UPDATED) {
      drupal_set_message(t('The rule has been modified.'));
    }
    else {
      drupal_set_message(t('Unexpected error has been detected.'));
    }
  }
  //if everything was fine, print the rules with the newly added/modified one
  drupal_goto('admin/user/shib_auth/rules');
}
/**
 * Edits a rule by calling universal create/edit form
 */
function shib_auth_edit_rule($form_state, $id) {
  $form = array();
  // calls the edit form, with the fields of the existing rule
  if (is_int((int)$id)) {
    $rule = db_query("SELECT * FROM {shib_auth} WHERE id = %d", array($id));
    $db_entry = db_fetch_array($rule);
    return shib_auth_edit_form(
      array($db_entry['id'], $db_entry['field'], urldecode($db_entry['regexpression']), unserialize($db_entry['role']), 'Apply', $db_entry['sticky'])
    );
  }

}//function shib_auth_edit_rule
