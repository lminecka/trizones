<?php
/**
 * @file
 * Base class for migrating nodes into Drupal.
 */

/**
 * Base class for all node migrations - handles commonalities across all
 * supported source Drupal versions.
 *
 * In addition to the arguments supported by DrupalMigration, the following
 * must be passed in the $arguments array:
 *
 * source_type - Drupal 6 content type machine name.
 * destination_type - Drupal 7 content type machine name (bundle).
 *
 * The following optional arguments may be passed:
 *
 * user_migration - Machine name of a user migration, used to establish
 *   dependencies and a sourceMigration for the uid mapping.
 * default_uid - Drupal 7 (destination) uid of the user account to use as
 *   the default.
 * default_language - Default language for the node and node body. Defaults
 *   to LANGUAGE_NONE.
 */
abstract class DrupalNodeMigration extends DrupalMigration {
  /**
   * The source and destination content types (bundles) we're dealing with.
   */
  protected $destinationType;

  /**
   * Default language to apply to the node and it's body field.
   *
   * @var string
   */
  protected $defaultLanguage = LANGUAGE_NONE;

  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    $this->destinationType = $arguments['destination_type'];
    if (!empty($arguments['user_migration'])) {
      $user_migration = $arguments['user_migration'];
      $this->dependencies[] = $user_migration;
    }
    if (!empty($arguments['default_language'])) {
      $this->defaultLanguage = $arguments['default_language'];
    }
    parent::__construct($arguments);
    $this->sourceFields += $this->version->getSourceFields('node', $this->sourceType);
    if ($this->moduleExists('path')) {
      $this->sourceFields['path'] = array('label' => t('Node: Path alias'));
    }

    $this->destination = new MigrateDestinationNode($this->destinationType);

    $this->map = new MigrateSQLMap($this->machineName,
      array(
        'nid' => array('type' => 'int',
                       'unsigned' => TRUE,
                       'not null' => TRUE,
                       'description' => 'Source node ID',
                       'alias' => 'n',
                      ),
      ),
      MigrateDestinationNode::getKeySchema()
    );

    $this->highwaterField = array(
      'name' => 'changed',
      'alias' => 'n',
      'type' => 'int',
    );

    // Setup common mappings
    $this->addSimpleMappings(array('title', 'body', 'status', 'created', 'changed',
      'comment', 'promote', 'sticky'));

    $this->addFieldMapping('body:language')
         ->defaultValue($this->defaultLanguage);

    $this->addUnmigratedSources(array('vid'));
    $this->addUnmigratedDestinations(array('is_new', 'revision',
      'revision_uid', 'log'));

    if (!empty($arguments['default_uid'])) {
      $default_uid = $arguments['default_uid'];
    }
    else {
      $default_uid = 1;
    }
    if (isset($user_migration)) {
      $this->addFieldMapping('uid', 'uid')
           ->sourceMigration($user_migration)
           ->defaultValue($default_uid);
    }
    else {
      $this->addFieldMapping('uid')
           ->defaultValue($default_uid);
    }

    if ($this->moduleExists('path')) {
      $this->addFieldMapping('path', 'path')
           ->description('Handled in prepareRow');
    }

    if (module_exists('pathauto')) {
      $this->addFieldMapping('pathauto')
           ->description('By default, disable in favor of migrated paths')
           ->defaultValue(0);
    }

    if (module_exists('statistics')) {
      if ($this->moduleExists('statistics')) {
        $this->addSimpleMappings(array('totalcount', 'daycount', 'timestamp'));
      }
      else {
        $this->addUnmigratedDestinations(array('totalcount', 'daycount', 'timestamp'));
      }
    }
  }

  /**
   * Called after the query data is fetched - we'll use this to populate the
   * source row with the CCK fields.
   */
  public function prepareRow($row) {
  	watchdog('migrate_d2d', 'prepareRow');
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    // Add the path to the source row, if relevant
    if ($this->moduleExists('path')) {
      $path = $this->version->getPath('node/' . $row->nid);
      if ($path) {
        $row->path = $path;
      }
    }

    $this->version->getSourceValues($row, $row->nid);
  }

  /**
   * Implementation of Migration::createStub().
   *
   * @param $migration
   * @return array|bool
   */
  protected function createStub($migration) {
    migrate_instrument_start('DrupalNodeMigration::createStub');
    $node = new stdClass;
    $node->title = t('Stub');
    $node->body = t('Stub body');
    $node->type = $this->destination->getBundle();
    $node->uid = 1;
    node_save($node);
    migrate_instrument_stop('DrupalNodeMigration::createStub');
    if (isset($node->nid)) {
      return array($node->nid);
    }
    else {
      return FALSE;
    }
  }
}
