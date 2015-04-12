<?php

/**
 * @file
 * Providing extra functionality for the Course UI via views.
 */


/**
 * Implements hook_views_data()
 */
function kalvi_course_views_data_alter(&$data) {
  $data['kalvi_course']['link_course'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the course.'),
      'handler' => 'course_handler_link_field',
    ),
  );
  $data['kalvi_course']['edit_course'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the course.'),
      'handler' => 'course_handler_edit_link_field',
    ),
  );
  $data['kalvi_course']['delete_course'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the course.'),
      'handler' => 'course_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows courses/course/%course_id/op
  $data['kalvi_course']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this course.'),
      'handler' => 'course_handler_course_operations_field',
    ),
  );
}
