<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying edit links
 * as fields
 */


class session_handler_edit_link_field extends session_handler_link_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = 'type';
  }


  function render($values) {
    $type = $values->{$this->aliases['type']};

    //Creating a dummy session to check access against
    $dummy_session = (object) array('type' => $type);
    if (!session_access('edit', $dummy_session)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $session_id = $values->{$this->aliases['session_id']};

    return l($text, 'session/' . $session_id . '/edit');
  }
}
