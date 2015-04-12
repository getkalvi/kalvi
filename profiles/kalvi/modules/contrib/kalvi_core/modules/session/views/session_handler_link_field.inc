<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying links to entities
 * as fields.
 */

class session_handler_link_field extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['session_id'] = 'session_id';
    $this->additional_fields['type'] = 'type';
    $this->additional_fields['name'] = 'name';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['use_session_title'] = array('default' => isset($this->definition['use_session_title default']) ? $this->definition['use_session_title default'] : FALSE, 'bool' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['use_session_title'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use Session Name as Link Text'),
      '#default_value' => TRUE,
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $text = ($this->options['use_session_title']) ? $values->{$this->aliases['name']} : t('view');
    $session_id = $values->{$this->aliases['session_id']};

    return l($text, 'session/' . $session_id);
  }
}
