<?php

/**
 * This field handler aggregates operations that can be done on a session
 * under a single field providing a more flexible way to present them in a view
 */
class session_handler_session_operations_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['session_id'] = 'session_id';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {

    $links = menu_contextual_links('session', 'session', array($this->get_value($values, 'session_id')));

    if (!empty($links)) {
      return theme('links', array('links' => $links, 'attributes' => array('class' => array('links', 'inline', 'operations'))));
    }
  }
}
