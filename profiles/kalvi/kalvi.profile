<?php
/**
 * @file
 * Enables modules and site configuration for a kalvi site installation.
 */

/**
 * Set Kalvi as default install profile.
 *
 * Must use system as the hook module because our callback is not active yet
 */
function system_form_install_select_profile_form_alter(&$form, $form_state) {
  foreach($form['profile'] as $key => $element) {
    $form['profile'][$key]['#value'] = 'kalvi';
  }
}

/**
 * Implements hook_form_FORM_ID_alter() for install_configure_form().
 *
 * Allows the profile to alter the site configuration form.
 */
function kalvi_form_install_configure_form_alter(&$form, $form_state) {
  // Pre-populate some fields.
  $form['site_information']['site_name']['#default_value'] = 'Kalvi';
  $form['site_information']['site_mail']['#default_value'] = 'admin@'. $_SERVER['HTTP_HOST'];
  $form['admin_account']['account']['name']['#default_value'] = 'admin';
  $form['admin_account']['account']['mail']['#default_value'] = 'admin@'. $_SERVER['HTTP_HOST'];
}

/**
 * Change the final task to our task
 */
function kalvi_install_tasks_alter(&$tasks, $install_state) {
  require_once(drupal_get_path('module', 'panopoly_core') . '/panopoly_core.profile.inc');
  $tasks['install_load_profile']['function'] = 'panopoly_core_install_load_profile';
}
