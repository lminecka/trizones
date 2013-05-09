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

class TrizonesCoachMigration extends TrizonesNodeMigration {
  public function __construct(array $arguments) {
    
		
 		$this->sourceFields['Bio'] = '';
 		$this->sourceFields['Email'] = '';
 		$this->sourceFields['JobTitle'] = '';
 		$this->sourceFields['JobTitle_language'] = '';
    $this->sourceFields['language'] = '';
    $this->sourceFields['Photograph'] = '';
    // additional fields for file fields
    $this->sourceFields['Photograph_file_class'] = '';
    $this->sourceFields['Photograph_language'] = '';
    $this->sourceFields['Photograph_destination_dir'] = '';
    $this->sourceFields['Photograph_destination_file'] = '';
    $this->sourceFields['Photograph_file_replace'] = '';
    $this->sourceFields['Photograph_preserve_files'] = '';
    $this->sourceFields['Photograph_source_dir'] = '';
    $this->sourceFields['Photograph_description'] = '';
    $this->sourceFields['Photograph_display'] = '';
    
    $this->sourceFields['position'] = '';
    // additional fields for term reference fields
    $this->sourceFields['position_create_term'] = '';
    $this->sourceFields['position_ignore_case'] = '';
    $this->sourceFields['position_source_type'] = '';

		$this->addFieldMapping('body', 'Bio');
		$this->addFieldMapping('field_email', 'Email');
		$this->addFieldMapping('field_job_title', 'JobTitle');
		$this->addFieldMapping('field_job_title:language', 'JobTitle_language');
		$this->addFieldMapping('field_photograph', 'Photograph');
		$this->addFieldMapping('field_photograph:file_class', 'Photograph_file_class');
		$this->addFieldMapping('field_photograph:language', 'Photograph_language');
    $this->addFieldMapping('field_photograph:destination_dir', 'Photograph_destination_dir');
    $this->addFieldMapping('field_photograph:destination_file', 'Photograph_destination_file');
    $this->addFieldMapping('field_photograph:file_replace', 'Photograph_file_replace');
    $this->addFieldMapping('field_photograph:preserve_files', 'Photograph_preserve_files');
    $this->addFieldMapping('field_photograph:source_dir', 'Photograph_source_dir');
    $this->addFieldMapping('field_photograph:description', 'Photograph_description');
    $this->addFieldMapping('field_photograph:display', 'Photograph_display');
		$this->addFieldMapping('field_position', 'position');
		$this->addFieldMapping('field_position:create_term', 'position_create_term');
		$this->addFieldMapping('field_position:ignore_case', 'position_ignore_case');
		$this->addFieldMapping('field_position:source_type', 'position_source_type');
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

    $coach_rows = Database::getConnection('default', $this->sourceConnection)
				->select('node', 'n')
				->join('node_revisions', 'r', 'n.nid = r.nid')
				->join('term_node', 'tn', 'n.nid = tn.nid')
				->join('flexinode_data', 'fd', 'n.nid = fd.nid')
				->join('flexinode_field', 'ff', 'fd.field_id = ff.field_id')
 				->fields('fd', array('textual_data', 'serialized_data'))
 				->fields('ff', array('label'))
 				->fields('tn', array('tid'))
 				->condition('n.type', 'flexinode-4')
 				->condition('ff.ctype_id', '4')
        ->condition('r.vid', $row->vid)
                 ->execute()
                 ->fetchAll();
     
      
    if (!$coach_rows) {
    	$this->saveMessage(t('No coach data for node with vid !vid',
                         array('!vid' => $row->vid)));
      return;
    }
		for($i=0; $i< count($coach_rows); $i++) {
			$coach_row = $coach_rows[$i];
			if ($i == 0) {
				$row->position = $coach_row->tid;
				$row->position_create_term = false;
				$row->position_ignore_case = true;
				$row->position_source_type = 'tid';
				
				$row->JobTitle_language = 'en';
				$row->language = 'en';
			}
			$label = str_replace(' ', '', $coach_row['label']);
			
			if ($label == 'Photograph') {
				$values = unserialize($coach_row->serialized_data);
				$source_dir = '/Users/lhb/Sites/trizones/trizones_d4/'.$values->filepath;
				$row->Photograph_file_class = 'MigrateFileUri';
				$row->Photograph_language = 'en';
				$row->Photograph_destination_dir = 'coaches';
				$row->Photograph_destination_file = '';
				$row->Photograph_file_replace = FILE_EXISTS_RENAME;
				$row->Photograph_preserve_files = false;
				$row->Photograph_source_dir = $source_dir;
				$row->Photograph_description = '';
				$row->Photograph_display = '';
			} else {
			//	$eval = '$row->'.$label.' = $row->textual_value;';
				$row->$label = $row->textual_value;
				watchdog('migrate', $label);
			}
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
   * Implementation of Migration::prepareRow()
   * Here we pull in additional data from the forum table
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

class TrizonesRaceMigration extends TrizonesNodeMigration {
  public function __construct(array $arguments) {
    
 		$this->sourceFields['Distance'] = '';
 		$this->sourceFields['Distance_language'] = '';
    $this->sourceFields['language'] = '';
    $this->sourceFields['Location'] = '';
    $this->sourceFields['Logo'] = '';
    // additional fields for file fields
    $this->sourceFields['Logo_file_class'] = '';
    $this->sourceFields['Logo_language'] = '';
    $this->sourceFields['Logo_destination_dir'] = '';
    $this->sourceFields['Logo_destination_file'] = '';
    $this->sourceFields['Logo_file_replace'] = '';
    $this->sourceFields['Logo_preserve_files'] = '';
    $this->sourceFields['Logo_source_dir'] = '';
    $this->sourceFields['Logo_description'] = '';
    $this->sourceFields['Logo_display'] = '';
    $this->sourceFields['RaceDescription'] = '';
    $this->sourceFields['Registration'] = '';
    $this->sourceFields['race_type'] = '';
    // additional fields for term reference fields
    $this->sourceFields['race_type_create_term'] = '';
    $this->sourceFields['race_type_ignore_case'] = '';
    $this->sourceFields['race_type_source_type'] = '';

		$this->addFieldMapping('body', 'RaceDescription');
		$this->addFieldMapping('field_distance', 'Distance');
		$this->addFieldMapping('field_distance:language', 'Distance_language');
		$this->addFieldMapping('field_location', 'Location');
		$this->addFieldMapping('field_race_logo', 'Logo');
		$this->addFieldMapping('field_race_logo:file_class', 'Logo_file_class');
		$this->addFieldMapping('field_race_logo:language', 'Logo_language');
    $this->addFieldMapping('field_race_logo:destination_dir', 'Logo_destination_dir');
    $this->addFieldMapping('field_race_logo:destination_file', 'Logo_destination_file');
    $this->addFieldMapping('field_race_logo:file_replace', 'Logo_file_replace');
    $this->addFieldMapping('field_race_logo:preserve_files', 'Logo_preserve_files');
    $this->addFieldMapping('field_race_logo:source_dir', 'Logo_source_dir');
    $this->addFieldMapping('field_race_logo:description', 'Logo_description');
    $this->addFieldMapping('field_race_logo:display', 'Logo_display');
		$this->addFieldMapping('field_race_type', 'race_type');
		$this->addFieldMapping('field_race_type:create_term', 'race_type_create_term');
		$this->addFieldMapping('field_race_type:ignore_case', 'race_type_ignore_case');
		$this->addFieldMapping('field_race_type:source_type', 'race_type_source_type');
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

    $race_rows = Database::getConnection('default', $this->sourceConnection)
				->select('node', 'n')
				->join('node_revisions', 'r', 'n.nid = r.nid')
				->join('term_node', 'tn', 'n.nid = tn.nid')
				->join('flexinode_data', 'fd', 'n.nid = fd.nid')
				->join('flexinode_field', 'ff', 'fd.field_id = ff.field_id')
 				->fields('fd', array('textual_data', 'serialized_data'))
 				->fields('ff', array('label'))
 				->fields('tn', array('tid'))
 				->condition('n.type', 'flexinode-2')
 				->condition('ff.ctype_id', '2')
        ->condition('r.vid', $row->vid)
                 ->execute()
                 ->fetchAll();
     
      
    if (!$race_rows) {
    	$this->saveMessage(t('No Race data for node with vid !vid',
                         array('!vid' => $row->vid)));
      return;
    }
		for($i=0; $i< count($race_rows); $i++) {
			$race_row = $race_rows[$i];
			if ($i == 0) {
				$row->race_type = $race_row->tid;
				$row->race_type_create_term = false;
				$row->race_type_ignore_case = true;
				$row->race_type_source_type = 'tid';
				
				$row->Distance_language = 'en';
				$row->language = 'en';
			}
			$label = str_replace(' ', '', $race_row['label']);
			
			if ($label == 'Logo') {
				$values = unserialize($race_row->serialized_data);
				$source_dir = '/Users/lhb/Sites/trizones/trizones_d4/'.$values->filepath;
				$row->Logo_file_class = 'MigrateFileUri';
				$row->Logo_language = 'en';
				$row->Logo_destination_dir = 'races/logos';
				$row->Logo_destination_file = '';
				$row->Logo_file_replace = FILE_EXISTS_RENAME;
				$row->Logo_preserve_files = false;
				$row->Logo_source_dir = $source_dir;
				$row->Logo_description = '';
				$row->Logo_display = '';
			} else {
			//	$eval = '$row->'.$label.' = $row->textual_value;';
				$row->$label = $row->textual_value;
				watchdog('migrate', $label);
			}
		}
	}
}
