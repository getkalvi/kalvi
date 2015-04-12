<?php

/**
 * @file
 * Provides session entity related classes.
 */


/**
 * The class used for session entities
 */
class Session extends Entity {

  public function __construct($values = array()) {
    parent::__construct($values, 'session');
  }

  protected function defaultLabel() {
    return $this->name;
  }

  protected function defaultUri() {
    return array('path' => 'session/' . $this->session_id);
  }

  public function url() {
    return 'session/' . $this->session_id;
  }

}


/**
 * The class used for session type entities
 */
class SessionType extends Entity {

  public $type;
  public $label;
  public $class;

  public function __construct($values = array()) {
    parent::__construct($values, 'session_type');
  }

}


/**
 * The Controller for Session entities
 */
class SessionController extends EntityAPIController {
  public function __construct($entityType) {
    parent::__construct($entityType);
  }

  /**
   * Overrides EntityAPIController::query().
   */
  public function query($ids, $conditions, $revision_id = FALSE) {
    $query = $this->buildQuery($ids, $conditions, $revision_id); $result = $query->execute();
    $result->setFetchMode(PDO::FETCH_ASSOC);
    // Build the resulting objects ourselves, since the standard PDO ways of
    // doing that are completely useless.
    $objects = array();
    foreach ($result as $row) {
      $row['is_new'] = FALSE;
      $objects[] = $this->create($row);
    }
    return $objects;
  }

  /**
   * Create a session - we first set up the values that are specific
   * to our session schema but then also go through the EntityAPIController
   * function.
   *
   * @param $type
   *   The machine-readable type of the session.
   *
   * @return
   *   A session object with all default fields initialized.
   */
  public function create(array $values = array()) {
    if (!isset($values[$this->entityInfo['entity keys']['bundle']])) {
      throw new Exception(t('No bundle provided to MyEntityAPIController::create().'));
    }
    $bundle = $values[$this->entityInfo['entity keys']['bundle']];
    global $user;
    // Add values that are specific to our Session
    $values += array(
      'session_id' => NULL,
      'is_new' => TRUE,
      'name' => '',
      'created' => '',
      'changed' => '',
      'author' => $user->uid,
    );
    $default_class = isset($this->entityInfo['entity class']) ? $this->entityInfo['entity class'] : NULL;
    if(isset($this->entityInfo['bundles'][$bundle]['entity class'])) {
      $class = $this->entityInfo['bundles'][$bundle]['entity class'];
    }
    else {
      $class = $default_class;
    }

    if (!class_exists($class)) {
      $class = "Entity";
    }
    return new $class($values, $this->entityType);
  }

  /**
   * Overriding the buldContent function to add entity specific fields
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $content = parent::buildContent($entity, $view_mode, $langcode, $content);
    $content['author'] = array('#markup' => t('Created by: !author', array('!author' => user_load($entity->author)->name)));
    return $content;
  }

}


/**
 * The Controller for Session entities
 */
class SessionTypeController extends EntityAPIControllerExportable {
  public function __construct($entityType) {
    parent::__construct($entityType);
  }

   /**
   * Create a session type - we first set up the values that are specific
   * to our session type schema but then also go through the EntityAPIController
   * function.
   *
   * @param $type
   *   The machine-readable type of the session.
   *
   * @return
   *   A session type object with all default fields initialized.
   */
  public function create(array $values = array()) {
    // Add values that are specific to our Session
    $values += array(
      'id' => '',
      'is_new' => TRUE,
    );
    $session_type = parent::create($values);
    return $session_type;
  }

  /**
   * Overridden to set default bundle.
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    if (!isset($entity->class)) {
      $entity->class = 'Session';
    }
    return parent::save($entity, $transaction);
  }

}
