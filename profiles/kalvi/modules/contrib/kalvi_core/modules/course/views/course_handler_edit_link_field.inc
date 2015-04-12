<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields
 */


class course_handler_edit_link_field extends course_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};

    //Creating a dummy course to check access against
    $dummy_course = (object) array('type' => $type);
    if (!course_access('edit', $dummy_course)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $course_id = $values->{$this->aliases['course_id']};

    return l($text, 'course/' . $course_id . '/edit');
  }
}
