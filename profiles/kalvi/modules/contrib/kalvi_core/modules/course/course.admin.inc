<?php

/**
 * @file
 * Course editing UI.
 *
 * We make very little use of the EntityAPI interface for this - preferring instead to use
 * views. That offers more flexibility to change a UI that will, more often than not,
 * be end-user facing.
 */

/**
 * UI controller.
 */
class CourseUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults. Main reason for doing this is that
   * parent class hook_menu() is optimized for entity type administration.
   */
  public function hook_menu() {

    $items = array();
    $id_count = count(explode('/', $this->path));
    $wildcard = isset($this->entityInfo['admin ui']['menu wildcard']) ? $this->entityInfo['admin ui']['menu wildcard'] : '%' . $this->entityType;

    $items[$this->path] = array(
      'title' => 'Courses',
      'description' => 'Add edit and update courses.',
      'page callback' => 'system_admin_menu_block_page',
      'access arguments' => array('access administration pages'),
      'file path' => drupal_get_path('module', 'system'),
      'file' => 'system.admin.inc',
    );

    // Change the overview menu type for the list of courses.
    $items[$this->path]['type'] = MENU_LOCAL_TASK;

    // Change the add page menu to multiple types of entities
    $items[$this->path . '/add'] = array(
      'title' => 'Add a course',
      'description' => 'Add a new course',
      'page callback'  => 'course_add_page',
      'access callback'  => 'course_access',
      'access arguments' => array('edit'),
      'type' => MENU_NORMAL_ITEM,
      'weight' => 20,
      'file' => 'course.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])

    );

    // Add menu items to add each different type of entity.
    foreach (course_get_types() as $type) {
      $items[$this->path . '/add/' . $type->type] = array(
        'title' => 'Add ' . $type->label,
        'page callback' => 'course_form_wrapper',
        'page arguments' => array(course_create(array('type' => $type->type))),
        'access callback' => 'course_access',
        'access arguments' => array('edit', 'edit ' . $type->type),
        'file' => 'course.admin.inc',
        'file path' => drupal_get_path('module', $this->entityInfo['module'])
      );
    }


    // Menu item for viewing courses

    $items['course/' . $wildcard] = array(
      'title callback' => 'course_page_title',
      'title arguments' => array(1),
      'page callback' => 'course_page_view',
      'page arguments' => array(1),
      'access callback' => 'course_access',
      'access arguments' => array('view', 1),
      'type' => MENU_CALLBACK,
    );

    $items['course/' . $wildcard . '/edit'] = array(
      'title' => 'Edit',
      'page callback' => 'course_form_wrapper',
      'page arguments' => array(1),
      'access callback' => 'course_access',
      'access arguments' => array('edit', 1),
      'weight' => 0,
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'file' => 'course.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );

    $items['course/' . $wildcard . '/delete'] = array(
      'title' => 'Delete',
      'page callback' => 'course_delete_form_wrapper',
      'page arguments' => array(1),
      'access callback' => 'course_access',
      'access arguments' => array('edit', 1),
      'type' => MENU_LOCAL_TASK,
      'context' => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
      'weight' => 1,
      'file' => 'course.admin.inc',
      'file path' => drupal_get_path('module', $this->entityInfo['module'])
    );
    return $items;
  }


  /**
   * Create the markup for the add Course Entities page within the class
   * so it can easily be extended/overriden.
   */
  public function addPage() {
    $item = menu_get_item();
    $content = system_admin_menu_block($item);

    if (count($content) == 1) {
      $item = array_shift($content);
      drupal_goto($item['href']);
    }

    return theme('course_add_list', array('content' => $content));
  }

}


/**
 * Form callback wrapper: create or edit a course.
 *
 * @param $course
 *   The course object being edited by this form.
 *
 * @see course_edit_form()
 */
function course_form_wrapper($course) {
  // Add the breadcrumb for the form's location.
  course_set_breadcrumb();
  return drupal_get_form('course_edit_form', $course);
}


/**
 * Form callback wrapper: delete a course.
 *
 * @param $course
 *   The course object being edited by this form.
 *
 * @see course_edit_form()
 */
function course_delete_form_wrapper($course) {
  // Add the breadcrumb for the form's location.
  //course_set_breadcrumb();
  return drupal_get_form('course_delete_form', $course);
}


/**
 * Form callback: create or edit a course.
 *
 * @param $course
 *   The course object to edit or for a create form an empty course object
 *     with only a course type defined.
 */
function course_edit_form($form, &$form_state, $course) {
  // Add the default field elements.
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Course Name'),
    '#default_value' => isset($course->name) ? $course->name : '',
    '#maxlength' => 255,
    '#required' => TRUE,
    '#weight' => -100,
  );

  // Add the field related form elements.
  $form_state['course'] = $course;
  field_attach_form('course', $course, $form, $form_state);


  // module_load_include('inc', 'kalvi_session', 'session.admin');
  // $form['session'] = drupal_get_form('session_edit_form',session_create(array('type' => 'session')), $course);

  $sessions = _course_sessions_list($course->course_id);

  $form['sessions']['#tree'] = TRUE;

  foreach ($sessions as $item) {
    $form['sessions'][$item['session_id']] = array(

      'name' => array(
        '#markup' => check_plain($item['name']),
      ),

      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $item['weight'],
        '#delta' => 10,
        '#title_display' => 'invisible',
      ),
    );
  }

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
    '#value' => t('Save course'),
    '#submit' => $submit + array('course_edit_form_submit'),
  );

  if (!empty($course->name)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete course'),
      '#suffix' => l(t('Cancel'), 'admin/kalvi/courses'),
      '#submit' => $submit + array('course_form_submit_delete'),
      '#weight' => 45,
    );
  }

  // We append the validate handler to #validate in case a form callback_wrapper
  // is used to add validate handlers earlier.
  $form['#validate'][] = 'course_edit_form_validate';
  return $form;
}

function _course_sessions_list($course_id) {
  $query = relation_query('course', $course_id);
  $course_sessions = $query->propertyCondition('relation_type', 'course_session')
                     ->fieldOrderBy('field_session_weight', 'value')
                     ->execute();
  $relations = relation_load_multiple(array_keys($course_sessions));

  $sessions = array();
  foreach($relations as $course_session) {
    $endpoints = field_get_items('relation', $course_session, 'endpoints');
    $weight = field_get_items('relation', $course_session, 'field_session_weight');
    $session = session_load($endpoints[1]['entity_id']);
    $sessions[] = array(
      'session_id' => $session->session_id,
      'weight' => $weight[0]['value'],
      'name' => $session->name
    );
  }

  return $sessions;
}

function _update_session_weight($course_id, $session_id, $weight) {
  $query = relation_query('session', $session_id);
  $course_session = $query->propertyCondition('relation_type', 'course_session')
                     ->related('course', $course_id)
                     ->execute();
  $rel = reset($course_session);
  $wrapped_course_session = entity_metadata_wrapper("relation", relation_load($rel->rid));
  $wrapped_course_session->field_session_weight->set($weight);
  $wrapped_course_session->save();
}

function theme_course_edit_form($variables) {
  $form = $variables['form'];

  // Initialize the variable which will store our table rows.
  $rows = array();

  foreach (element_children($form['sessions']) as $id) {

    $form['sessions'][$id]['weight']['#attributes']['class'] = array('session-weight');

    $rows[] = array(
      'data' => array(
        drupal_render($form['sessions'][$id]['name']),
        drupal_render($form['sessions'][$id]['weight']),
      ),
      'class' => array('draggable'),
    );
  }

  $header = array(t('Name'), t('Weight'));

  $table_id = 'sessions-list';

  $output = theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => $table_id),
  ));

  $output .= drupal_render_children($form);

  drupal_add_tabledrag($table_id, 'order', 'sibling', 'session-weight');

  return $output;
}

/**
 * Form API validate callback for the course form
 */
function course_edit_form_validate(&$form, &$form_state) {
  $course = $form_state['course'];

  // Notify field widgets to validate their data.
  field_attach_form_validate('course', $course, $form, $form_state);
}


/**
 * Form API submit callback for the course form.
 *
 * @todo remove hard-coded link
 */
function course_edit_form_submit(&$form, &$form_state) {

  $course = entity_ui_controller('course')->entityFormSubmitBuildEntity($form, $form_state);
  // Save the course and go back to the list of courses

  // Add in created and changed times.
  if ($course->is_new = isset($course->is_new) ? $course->is_new : 0){
    $course->created = time();
  }

  $course->changed = time();

  $course->save();

  if(isset($form_state['values']['sessions'])) {
    foreach ($form_state['values']['sessions'] as $id => $item) {
      _update_session_weight($course->course_id, $id, $item['weight']);
    }
  }

  $form_state['redirect'] = 'admin/kalvi/courses';
}

/**
 * Form API submit callback for the delete button.
 *
 * @todo Remove hard-coded path
 */
function course_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/kalvi/courses/course/' . $form_state['course']->course_id . '/delete';
}


/**
 * Form callback: confirmation form for deleting a course.
 *
 * @param $course
 *   The course to delete
 *
 * @see confirm_form()
 */
function course_delete_form($form, &$form_state, $course) {
  $form_state['course'] = $course;

  $form['#submit'][] = 'course_delete_form_submit';

  $form = confirm_form($form,
    t('Are you sure you want to delete course %name?', array('%name' => $course->name)),
    'admin/kalvi/courses/course',
    '<p>' . t('This action cannot be undone.') . '</p>',
    t('Delete'),
    t('Cancel'),
    'confirm'
  );

  return $form;
}

/**
 * Submit callback for course_delete_form
 */
function course_delete_form_submit($form, &$form_state) {
  $course = $form_state['course'];

  course_delete($course);

  drupal_set_message(t('The course %name has been deleted.', array('%name' => $course->name)));
  watchdog('course', 'Deleted course %name.', array('%name' => $course->name));

  $form_state['redirect'] = 'admin/kalvi/courses';
}



/**
 * Page to add Course Entities.
 *
 * @todo Pass this through a proper theme function
 */
function course_add_page() {
  $controller = entity_ui_controller('course');
  return $controller->addPage();
}


/**
 * Displays the list of available course types for course creation.
 *
 * @ingroup themeable
 */
function theme_course_add_list($variables) {
  $content = $variables['content'];
  $output = '';
  if ($content) {
    $output = '<dl class="course-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer course types')) {
      $output = '<p>' . t('Course Entities cannot be added because you have not created any course types yet. Go to the <a href="@create-course-type">course type creation page</a> to add a new course type.', array('@create-course-type' => url('admin/structure/course_types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No course types have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}





/**
 * Sets the breadcrumb for administrative course pages.
 */
function course_set_breadcrumb() {
  $breadcrumb = array(
    l(t('Home'), '<front>'),
    l(t('Administration'), 'admin'),
    l(t('Kalvi'), 'admin/kalvi'),
    l(t('Courses'), 'admin/kalvi/courses'),
  );

  drupal_set_breadcrumb($breadcrumb);
}
