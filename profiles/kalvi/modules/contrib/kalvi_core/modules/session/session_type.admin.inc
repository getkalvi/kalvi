<?php

/**
 * @file
 * Session type editing UI.
 */

/**
 * UI controller.
 */
class SessionTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
		$items[$this->path]['description'] = 'Manage Session entity types, including adding
		and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Generates the session type editing form.
 */
function session_type_form($form, &$form_state, $session_type, $op = 'edit') {

  if ($op == 'clone') {
    $session_type->label .= ' (cloned)';
    $session_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $session_type->label,
    '#description' => t('The human-readable name of this session type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($session_type->type) ? $session_type->type : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'session_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this session type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save session type'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function session_type_form_submit(&$form, &$form_state) {
  $session_type = entity_ui_form_submit_build_entity($form, $form_state);
  $session_type->save();
  $form_state['redirect'] = 'admin/kalvi/session_types';
}

/**
 * Form API submit callback for the delete button.
 */
function session_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/kalvi/session_types/manage/' . $form_state['session_type']->type . '/delete';
}
