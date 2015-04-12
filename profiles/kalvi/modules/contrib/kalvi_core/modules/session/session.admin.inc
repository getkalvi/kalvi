<?php

/**
 * @file
 * Session editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */

/**
 * UI controller.
 */
class SessionUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {

    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'Sessions',
      'description' => 'Add edit and update sessions.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );

    // Change the overview menu type for the list of sessions.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;

    // Add menu items to add each different type of entity.
    foreach (session_get_types() as $type) {
      $items['course/%course/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'session_form_wrapper',
        'page arguments' => array(session_create(array('type' => $type->type)), 1),
        'access callback' => 'session_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'session.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }

    // Loading and editing session entities
    $items['session/' . $wildcard] = array(
      'title callback' => 'session_page_title',
      'title arguments' => array(1),
      'page callback' => 'session_page_view',
      'page arguments' => array(1),
      'access callback' => 'session_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );

    $items['session/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'page callback' => 'session_form_wrapper',
      'page arguments' => array(1),
      'access callback' => 'session_access',
      'access arguments' => array('edit', 1),
      'weight' => 0,
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'session.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );

    $items['session/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'session_delete_form_wrapper',
      'page arguments' => array(1),
      'access callback' => 'session_access',
      'access arguments' => array('edit', 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'weight' => 1,
      'file' => 'session.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    return $items;
  }


  /**
   * Create the markup for the add Session Entities page within the class
   * so it can easily be extended/overriden.
   */
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }

    return theme('session_add_list', array('content' => $content));
  }

}


/**
 * Form callback wrapper: create or edit a session.
 *
 * @param $session
 *   The session object being edited by this form.
 *
 * @see session_edit_form()
 */
function session_form_wrapper($session, $course = NULL) {
  // Add the breadcrumb for the form's location.
  session_set_breadcrumb();
  return drupal_get_form('session_edit_form', $session, $course);
}


/**
 * Form callback wrapper: delete a session.
 *
 * @param $session
 *   The session object being edited by this form.
 *
 * @see session_edit_form()
 */
function session_delete_form_wrapper($session) {
  // Add the breadcrumb for the form's location.
  //session_set_breadcrumb();
  return drupal_get_form('session_delete_form', $session);
}


/**
 * Form callback: create or edit a session.
 *
 * @param $session
 *   The session object to edit or for a create form an empty session object
 *     with only a session type defined.
 */
function session_edit_form($form, &$form_state, $session, $course) {
  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Session Name'),
    '#default_value' => isset($session->name) ? $session->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -5,
  );
  if($course) {
    $form['course_id'] = array(
      '#type' => 'hidden',
      '#value' => $course->course_id,
    );
  }

  // Add the field related form elements.
  $form_state['session'] = $session;
  field_attach_form('session', $session, $form, $form_state);

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );

  // We add the form's #submit array to this button along with the actual submit
  // handler to preserve any submit handlers added by a form callback_wrapper.
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save session'),
    '#submit' => $submit + array('session_edit_form_submit'),
  );

  if (!empty($session->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete session'),
      '#suffix' => l(t('Cancel'), 'admin/kalvi/sessions'),
      '#submit' => $submit + array('session_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'session_edit_form_validate';
  return $form;
}


/**
 * Form API validate callback for the session form
 */
function session_edit_form_validate(&$form, &$form_state) {
  $session = $form_state['session'];

  // Notify field widgets to validate their data.
  field_attach_form_validate('session', $session, $form, $form_state);
}


/**
 * Form API submit callback for the session form.
 *
 * @todo remove hard-coded link
 */
function session_edit_form_submit(&$form, &$form_state) {
  global $user;
  $session = entity_ui_controller('session')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the session and go back to the list of sessions

  // Add in created and changed times.
  if ($session->is_new = isset($session->is_new) ? $session->is_new : 0) {
    $session->created = time();
  }

  $session->changed = time();

  $session->author = $user->uid;
  $session->save();
  if($form_state['values']['course_id']) {
    module_load_include('inc', 'kalvi_core', 'includes/kalvi_core.enroll');
    add_session_to_course($form_state['values']['course_id'], $session->session_id);
  }
  $form_state['redirect'] = 'admin/kalvi/sessions';
}

/**
 * Form API submit callback for the delete button.
 *
 * @todo Remove hard-coded path
 */
function session_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'session/' . $form_state['session']->session_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a session.
 *
 * @param $session
 *   The session to delete
 *
 * @see confirm_form()
 */
function session_delete_form($form, &$form_state, $session) {
  $form_state['session'] = $session;

  $form['#submit'][] = 'session_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete session %name?', array('%name' => $session->name)),
    'admin/kalvi/sessions/session',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for session_delete_form
 */
function session_delete_form_submit($form, &$form_state) {
  $session = $form_state['session'];

  session_delete($session);

  drupal_set_message(t('The session %name has been deleted.', array('%name' => $session->name)));
  watchdog('session', 'Deleted session %name.', array('%name' => $session->name));

  $form_state['redirect'] = 'admin/kalvi/sessions';
}



/**
 * Page to add Session Entities.
 *
 * @todo Pass this through a proper theme function
 */
function session_add_page() {
  $controller = entity_ui_controller('session');
  return $controller->addPage();
}


/**
 * Displays the list of available session types for session creation.
 *
 * @ingroup themeable
 */
function theme_session_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="session-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer session types')) {
      $output = '<p>' . t('Session Entities cannot be added because you have not created any session types yet. Go to the <a href="@create-session-type">session type creation page</a> to add a new session type.', array('@create-session-type' => url('admin/structure/session_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No session types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}





/**
 * Sets the breadcrumb for administrative session pages.
 */
function session_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Kalvi'), 'admin/kalvi'),
    l(t('Sessions'), 'admin/kalvi/sessions'),
  );

  drupal_set_breadcrumb($breadcrumb);
}
