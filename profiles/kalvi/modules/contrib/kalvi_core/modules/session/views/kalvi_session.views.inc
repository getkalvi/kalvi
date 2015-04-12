<?php

/**
 * @file
 * Providing extra functionality for the Session UI via views.
 */


/**
 * Implements hook_views_data()
 */
function kalvi_session_views_data_alter(&$data) {
  $data['kalvi_session']['link_session'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a link to the session.'),
      'handler' => 'session_handler_link_field',
    ),
  );
  $data['kalvi_session']['edit_session'] = array(
    'field' => array(
      'title' => t('Edit Link'),
      'help' => t('Provide a link to the edit form for the session.'),
      'handler' => 'session_handler_edit_link_field',
    ),
  );
  $data['kalvi_session']['delete_session'] = array(
    'field' => array(
      'title' => t('Delete Link'),
      'help' => t('Provide a link to delete the session.'),
      'handler' => 'session_handler_delete_link_field',
    ),
  );
  // This content of this field are decided based on the menu structure that
  // follows sessions/session/%session_id/op
  $data['kalvi_session']['operations'] = array(
    'field' => array(
      'title' => t('Operations links'),
      'help' => t('Display all operations available for this session.'),
      'handler' => 'session_handler_session_operations_field',
    ),
  );
}
