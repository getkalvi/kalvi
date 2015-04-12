<?php

/**
 * @file
 * Course type editing UI.
 */

/**
 * UI controller.
 */
class CourseTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
		$items[$this->path]['description'] = 'Manage Course entity types, including adding
		and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Generates the course type editing form.
 */
function course_type_form($form, &$form_state, $course_type, $op = 'edit') {

  if ($op == 'clone') {
    $course_type->label .= ' (cloned)';
    $course_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $course_type->label,
    '#description' => t('The human-readable name of this course type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($course_type->type) ? $course_type->type : '',
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'course_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this course type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save course type'),
    '#weight' => 40,
  );
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function course_type_form_submit(&$form, &$form_state) {
  $course_type = entity_ui_form_submit_build_entity($form, $form_state);
  $course_type->save();
  $form_state['redirect'] = 'admin/kalvi/course_types';
}

/**
 * Form API submit callback for the delete button.
 */
function course_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/kalvi/course_types/manage/' . $form_state['course_type']->type . '/delete';
}
