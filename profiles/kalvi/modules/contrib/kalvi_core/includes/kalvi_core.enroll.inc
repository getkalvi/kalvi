<?php

/**
 * Returns a form with all students who are not enrolled to this course
 */
function course_enroll_form($form, &$form_state, $nid) {
    if (!empty($nid)) {
        $form['students'] = array(
            '#type' => 'checkboxes',
            '#title' => t('Students'),
            '#attributes' => array('class' => array('students')),
            // '#default_value' => !empty($syl['value']) ? $syl['value'] : '',
            '#options' => get_unenrolled_students($nid),
        );
        $form['course_id'] = array(
            '#type' => 'hidden',
            '#default_value' => $nid,
        );
        $form['submit'] = array(
            '#type' => 'submit',
            '#value' => t('Enroll'),
        );

        $form['#submit'][] = 'course_enroll_form_submit';
    }

    return $form;
}

/**
 * Submit handler for 'course_enroll_form
 * @param type $form
 * @param type $form_state
 */
function course_enroll_form_submit($form, &$form_state) {
    $users_to_enroll = $form_state['values']['students'];
    $nid = $form_state['values']['course_id'];
    enroll_users_to_course($users_to_enroll, $nid);
}

/**
 *
 * @param type $course_id
 * @param type $role
 * @return all student ids who are not enrolled to given course
 */
function get_unenrolled_students($course_id, $role='student') {
    $students = array();
    $endpoint_users = array();
    //get users enrolled in this course
    $endpoints = get_enrolled_students($course_id);
    $endpoint_users = array();
    if (!empty($endpoints['users'])) {
        $endpoint_users = $endpoints['users'];
    }
    $role = user_role_load_by_name($role);
    $result = db_select('users_roles', 'ur')
            ->fields('ur', array('uid'))
            ->condition('rid', $role->rid);
    if (!empty($endpoint_users)) {
        $result = $result->condition('ur.uid', $endpoint_users, 'NOT IN');
    }
    $result->join('users', 'u', 'u.uid = ur.uid');

    $result = $result->fields('u', array('name'))->execute();

    foreach ($result as $record) {
        $students[$record->uid] = $record->name;
    }

    return $students;
}


/**
 *
 * @param type $course_id
 * @return array all students ids enrolled to given course
 */
function get_enrolled_students($course_id) {
  $rel = kalvi_get_related_entities('course', $course_id, 'enrollment');
  if (empty($rel['enrollment'])) {
    return array();
  }
  foreach($rel['enrollment'] as $relation) {
    //get the id from [0] position as 'enrollment' is directional
    $endpoint_users['users'][] = $relation->endpoints[LANGUAGE_NONE][0]['entity_id'];
  }
  return $endpoint_users;
}

/**
 * Extension of relation_get_related_entity
 * Returns all entities that are related to the given entity
 *
 * @param type $entity_type
 * @param type $entity_id
 * @param type $relation_type
 * @param type $r_index
 * @return An array of all relations if exists, false if not
 */
function kalvi_get_related_entities($entity_type, $entity_id, $relation_type = NULL, $r_index = NULL) {
    $query = relation_query($entity_type, $entity_id, $r_index);
    if ($relation_type) {
        $query->entityCondition('bundle', $relation_type);
    }
    $results = $query->execute();
    $result = reset($results);
    if (empty($result)) {
        return FALSE;
    }
    // Loop through each relation
    foreach ($results as $relation) {
        $rids[] = $relation->rid;
    }
    // Load all related entities endpoint references
    $raw_relations = relation_load_multiple($rids);

    // Reformat relations output.
    foreach ($raw_relations as $relation) {
        if (!isset($relations[$relation->relation_type])) {
            $relations[$relation->relation_type] = array();
        }
        $relations[$relation->relation_type][] = $relation;
    }

    return $relations;
}

/**
 * Takes a list of users and course id
 * Enrolls all users in list to the given course
 *
 * @param type $users (list of user ids)
 * @param type $course_id
 */
function enroll_users_to_course($users, $course_id) {
  foreach ($users as $user_id) {
    enroll_user_to_course($course_id, $user_id);
  }
}

/**
 * Takes user_id and course id
 * Enrolls given user to the given course
 *
 * @param type $course_id
 * @param type $user_id
 * @return type 'relation' object
 */
function enroll_user_to_course($course_id, $user_id) {
    $user_course = is_user_enrolled($course_id, $user_id);
    if(!$user_course) {
      //TODO: check if user meets conditions
      $endpoints = array(
        array('entity_type' => 'user', 'entity_id' => $user_id),
        array('entity_type' => 'course', 'entity_id' => $course_id),
    );
      //create new relation
    $enroll = relation_create('enrollment', $endpoints);
    return relation_save($enroll);
    }
    return $user_course;
}

function unenroll_user_from_course($course_id, $user_id, $check=TRUE) {
  $enrolled = is_user_enrolled($course_id, $user_id);
  //TODO: utilize $check
  if($enrolled) {
    $rel = reset($enrolled);
    relation_delete($rel->rid);
  }
}

/**
 * Checks if a given user is enrolled to the given course
 *
 * @param type $course_id
 * @param type $user_id
 * @return type 'relation' object if user enrolled, 'false' if not
 */
function is_user_enrolled($course_id, $user_id) {
    $rel = 'enrollment';
    $endpoints = array(
        array('entity_type' => 'user', 'entity_id' => $user_id),
        array('entity_type' => 'course', 'entity_id' => $course_id),
    );
    $user_course = relation_relation_exists($endpoints, $rel);
    return $user_course;
}

/**
 * Adds a session to given course
 * @param type $course_id
 * @param type $session_id
 * @return type relation obj bet. course and session
 */
function add_session_to_course($course_id, $session_id) {
  $rel = 'course_session';
  $endpoints = array(
    array('entity_type' => 'course', 'entity_id' => $course_id),
    array('entity_type' => 'session', 'entity_id' => $session_id),
  );
  $course_session = relation_relation_exists($endpoints, $rel);
  if(!$course_session) {
    $new_rel = relation_create($rel, $endpoints);
    return relation_save($new_rel);
  }
  return $course_session;
}

function remove_session_from_course($course_id, $session_id) {
  $course_session = course_has_session($course_id, $session_id);
  if($course_session) {
    $rel = reset($course_session);
    relation_delete($rel->rid);
  }
}

function course_has_session($course_id, $session_id) {
  $rel = 'course_session';
  $endpoints = array(
    array('entity_type' => 'course', 'entity_id' => $course_id),
    array('entity_type' => 'session', 'entity_id' => $session_id),
  );
  $course_session = relation_relation_exists($endpoints, $rel);
  return $course_session;
}
