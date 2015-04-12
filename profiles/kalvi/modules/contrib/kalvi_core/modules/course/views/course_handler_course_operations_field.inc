<?php

/**
 * This field handler aggregates operations that can be done on a course
 * under a single field providing a more flexible way to present them in a view
 */
class course_handler_course_operations_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['course_id'] = 'course_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {

    $links = menu_contextual_links('course', 'course', array($this->get_value($values, 'course_id')));

    if (!empty($links)) {
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
