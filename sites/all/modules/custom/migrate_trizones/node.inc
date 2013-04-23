<?php

/**
 * Common mappings for the Drupal 4 node migrations.
 */
abstract class TrizonesNodeMigration extends DrupalNode4Migration {
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addFieldMapping('field_legacy_nid', 'nid')
         ->description('We have a common field to save the D4 nid');
  }
}

/**
 * Image-specific mappings and handling.
 */
class TrizonesImageMigration extends TrizonesNodeMigration {
  public function __construct(array $arguments) {
    // Add any other data we're pulling into the source row, before the parent
    // constructor.
    $this->sourceFields['copyright'] = 'Media copyright notice';
    $this->sourceFields['filename'] = 'Path of image within mounted filesystem';

    parent::__construct($arguments);

    $this->addFieldMapping('field_credit', 'copyright');
    $arguments = array('source_path' => '/mnt/source_images');
    $this->addFieldMapping('field_main_image', 'filename')
         ->arguments($arguments);
  }

  /**
   * Implementation of Migration::prepareRow().
   *
   * Here we pull in additional data from a non-core, non-CCK table.
   *
   * @param $row
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $image_row = Database::getConnection('default', $this->sourceConnection)
                 ->select('custom_image_table', 'i')
                 ->fields('i', array('copyright', 'filename'))
                 ->condition('vid', $row->vid)
                 ->execute()
                 ->fetchObject();
    if ($image_row) {
      $row->copyright = $image_row->copyright;
      $row->filename = $image_row->filename;
    }
    else {
      $this->saveMessage(t('No custom_image_table data for image node with vid !vid',
                         array('!vid' => $row->vid)));
    }
  }
}

class TrizonesForumMigration extends TrizonesNodeMigration {
  public function __construct(array $arguments) {
    // Add any other data we're pulling into the source row, before the parent
    // constructor.

    
		$this->sourceFields['create_term'] = '';
    $this->sourceFields['ignore_case'] = '';
    $this->sourceFields['language'] = '';
    $this->sourceFields['source_type'] = '';
    $this->sourceFields['taxonomy_forums'] = '';
    
		$this->addFieldMapping('taxonomy_forums', 'taxonomy_forums');
		$this->addFieldMapping('taxonomy_forums:create_term', 'create_term');
		$this->addFieldMapping('taxonomy_forums:ignore_case', 'ignore_case');
		$this->addFieldMapping('taxonomy_forums:source_type', 'source_type');
		$this->addFieldMapping('language', 'language');
		$this->addFieldMapping('tnid', NULL);
		$this->addFieldMapping('translate', NULL);
 			
    parent::__construct($arguments);

  }
  
  /**
   * Implementation of Migration::prepareRow().
   *
   * Here we pull in additional data from a non-core, non-CCK table.
   *
   * @param $row
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $forum_row = Database::getConnection('default', $this->sourceConnection)
                 ->select('forum', 'f')
                 ->fields('f', array('tid'))
                 ->condition('vid', $row->vid)
                 ->execute()
                 ->fetchObject();
    if ($forum_row) {
      $row->taxonomy_forums = $forum_row->tid;
      $row->create_term = false;
  		$row->ignore_case = true;
  		$row->source_type = 'tid';
      $row->language = 'en';
    }
    else {
      $this->saveMessage(t('No forum data for forum node with vid !vid',
                         array('!vid' => $row->vid)));
    }
  }
}
