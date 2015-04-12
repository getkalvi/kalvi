<?php

/*
* Implementation of hook_rules_condition_info().
*/
function kalvi_core_rules_condition_info() {

}

/**
 * Implementation of hook_rules_event_info().
 * @ingroup rules
 */
function kalvi_core_rules_event_info() {
  return array(
      'kalvi_course_started' => array(
          'group' => 'kalvi',
          'label' => t('After Course has started'),
          'module' => 'kalvi_core',
          'variables' => array(
              'course' => array(
                  'type' => 'user',
                  'label' => t('Course has started')
              ),
          ),
      ),
      'kalvi_user_enrollment' => array(
          'group' => 'kalvi',
          'label' => t('After user enrolls to a Course'),
          'variables' => array(
              'user' => array(
                  'type' => 'user',
                  'label' => t('User'),
              ),
              'course' => array(
                  'type' => 'course',
                  'label' => t('Course'),
              ),
          ),
          'module' => 'kalvi_core',
      ),
      'kalvi_user_disenrollment' => array(
          'group' => 'kalvi',
          'label' => t('After user dis-enrolls from a Course'),
          'variables' => array(
              'user' => array(
                  'type' => 'user',
                  'label' => t('User'),
              ),
              'course' => array(
                  'type' => 'course',
                  'label' => t('Course'),
              ),
          ),
          'module' => 'kalvi_core',
      ),
      'kalvi_course_completed_percent' => array(
          'group' => 'kalvi',
          'label' => t('User completed x% of the course'),
          'variables' => array(
              'user' => array(
                  'type' => 'user',
                  'label' => t('User'),
              ),
              'course' => array(
                  'type' => 'course',
                  'label' => t('Course'),
              ),
          ),
          'module' => 'kalvi_core',
      ),
       'kalvi_user_completes_session' => array(
          'group' => 'kalvi',
          'label' => t('User completes a session'),
          'module' => 'kalvi_core',
          'variables' => array(
              'user' => array(
                  'type' => 'user',
                  'label' => t('User'),
              ),
              'session' => array(
                  'type' => 'session',
                  'label' => t('Session'),
              ),
          ),
      ),
  );
}

function kalvi_course_started($course) {
  $date = date_create();
  $current_time = date_timestamp_get($date);
  $course_end = $current_time;
  $fields_info = field_info_instances('course', $course->type);
  foreach ($fields_info as $field_name => $value) {
    $field_info = field_info_field($field_name);
    $type = $field_info['type'];
    if ($type == 'field_duration') {
      $course_end = $course->$field_name;
    }
  }
  return ($current_time > $course) ? TRUE : FALSE;
}

function kalvi_user_enrollment($user, $course) {
  return !is_user_enrolled($course->cid, $user->uid) ? 0 : 1;
}
