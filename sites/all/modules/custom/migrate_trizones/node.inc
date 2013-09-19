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
   * In the Drupal 4 install we are migrating from; they were using an early form of CCK called flexinodes
   * The coach type was flexinode-4
   * We need to collect the data in the flexinodes table, mapped by it's fields & felxinode type.
   *
   *
   * @param $row
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }

    $query = Database::getConnection('default', $this->sourceConnection)
				->select('node', 'n');
		$query->join('node_revisions', 'r', 'n.nid = r.nid');
		$query->join('term_node', 'tn', 'n.nid = tn.nid');
		$query->join('flexinode_data', 'fd', 'n.nid = fd.nid');
		$query->join('flexinode_field', 'ff', 'fd.field_id = ff.field_id');
		$query->fields('fd', array('textual_data', 'serialized_data'));
		$query->fields('ff', array('label'));
		$query->fields('tn', array('tid'));
		$query->conditions('n.type', 'flexinode-4');
		$query->conditions('ff.ctype_id', '4');
    $query->condition('r.vid', $row->vid);
    $coach_rows = $query->execute()->fetchAll();
     
      
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
			$label = str_replace(' ', '', $coach_row->label);
			
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
    
    $this->sourceFields9['Date'] = '';
 		$this->sourceFields['Distance'] = '';
 		$this->sourceFields['Distance_language'] = '';
    $this->sourceFields['language'] = '';
    $this->sourceFields['Location'] = '';
    // additional fields for location fields
		$this->sourceFields['Location_name'] = '';
		$this->sourceFields['Location_street'] = '';
		$this->sourceFields['Location_additional'] = '';
		$this->sourceFields['Location_city'] = '';
		$this->sourceFields['Location_province'] = '';
		$this->sourceFields['Location_postal_code'] = '';
		$this->sourceFields['Location_latitude'] = '';
		$this->sourceFields['Location_longitude'] = '';
		$this->sourceFields['Location_country'] = '';
		$this->sourceFields['Location_source'] = '';
		$this->sourceFields['Location_is_primary'] = '';
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

		$this->addFieldMapping('body', 'RaceDescription ');
		$this->addFieldMapping('field_datetime', 'Date');
		$this->addFieldMapping('field_race_location', 'Location');
		$this->addFieldMapping('field_race_location:name', 'Location_name');
		$this->addFieldMapping('field_race_location:street', 'Location_street');
		$this->addFieldMapping('field_race_location:additional', 'Location_additional');
		$this->addFieldMapping('field_race_location:city', 'Location_city');
		$this->addFieldMapping('field_race_location:province', 'Location_province');
		$this->addFieldMapping('field_race_location:postal_code', 'Location_postal_code');
		$this->addFieldMapping('field_race_location:latitude', 'Location_latitude');
		$this->addFieldMapping('field_race_location:longitude', 'Location_longitude');
		$this->addFieldMapping('field_race_location:country', 'Location_country');
		$this->addFieldMapping('field_race_location:source', 'Location_source');
		$this->addFieldMapping('field_race_location:is_primary', 'Location_is_primary');
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
		$this->addFieldMapping('field_race_type', 'race_type')
			->sourceMigration('TZRaceTypeTerms')
      ->arguments(array('source_type' => 'tid'));
		$this->addFieldMapping('field_race_type:create_term', 'race_type_create_term');
		$this->addFieldMapping('field_race_type:ignore_case', 'race_type_ignore_case');
		$this->addFieldMapping('field_race_type:source_type', 'race_type_source_type');
		$this->addFieldMapping('field_text', "Distance");
		$this->addFieldMapping('field_text:language', 'Distance_language');
		$this->addFieldMapping('field_url', 'Registration');
		$this->addFieldMapping('language', 'language');
		$this->addFieldMapping('tnid', NULL);
		$this->addFieldMapping('translate', NULL);
 			
    parent::__construct($arguments);

  }
  
    /**
   * Query for basic node fields from Drupal 4.
   *
   * @return QueryConditionInterface
   */
  protected function query() {
    $query = Database::getConnection('default', $this->sourceConnection)
             ->select('node', 'n')
             ->fields('n', array('nid', 'vid', 'title',
                 'uid', 'status', 'created', 'changed', 'comment', 'promote',
                 'moderate', 'sticky'))
             ->condition('type', $this->sourceType)
             ->orderBy('changed');
    $query->innerJoin('node_revisions', 'nr', 'n.vid=nr.vid');
    $query->leftJoin('term_node', 'tn', 'n.nid=tn.nid');
    $query->fields('nr', array('body', 'teaser', 'format'));
    $query->fields('tn', array('tid'), array('race_type'));
    
    return $query;
  }
	  /**
   * Implementation of Migration::prepareRow().
   *
   * In the Drupal 4 install we are migrating from; they were using an early form of CCK called flexinodes
   * The race type was flexinode-2
   * We need to collect the data in the flexinodes table, mapped by it's fields & felxinode type.
   *
   * @param $row
   */
  public function prepareRow($row) {
  	watchdog('migrate_triones', 'race: prepareRow');
    $query = Database::getConnection('default', $this->sourceConnection)
    	->select('node', 'n');
		$query->join('node_revisions', 'r', 'n.nid = r.nid');
				$query->join('term_node', 'tn', 'n.nid = tn.nid');
				$query->join('flexinode_data', 'fd', 'n.nid = fd.nid');
				$query->join('flexinode_field', 'ff', 'fd.field_id = ff.field_id');
 				$query->fields('fd', array('textual_data', 'serialized_data'));
 				$query->fields('ff', array('label'));
 				$query->fields('tn', array('tid'));
 				$query->condition('n.type', 'flexinode-2');
 				$query->condition('ff.ctype_id', '2');
        $query->condition('r.vid', $row->vid);
      	$race_rows = $query->execute()->fetchAll();
       
     
    if (!$race_rows) {
    	$this->saveMessage(t('No Race data for node with vid !vid',
                         array('!vid' => $row->vid)));
      watchdog('hotdog', t('No Race data for node with vid !vid',
                         array('!vid' => $row->vid)));
      return;
    }
    // here is where will will loop through all the flexinode fields
		for($i=0; $i< count($race_rows); $i++) {
			$race_row = $race_rows[$i];
			$label = str_replace(' ', '', $race_row->label);
			
			$row->$label = $row->textual_value;
			
			// the regular node data will be present for every iteration of the flexinode data
			// we're just setting it the first time.
			if ($i == 0) {
			//	die('tid: '.$race_row->tid);
			//	$row->race_type = $race_row->tid;
				$row->race_type_create_term = false;
				$row->race_type_ignore_case = true;
				$row->race_type_source_type = 'tid';
				
				$row->Distance_language = 'en';
				$row->language = 'en';
			}
		
			// for the Logo field we will need to save the old file path along with all the new file settings
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
			} else if ($label = 'Location') {
				
			// most of the locations were simply stored as "city, state" so this regex matches for that
			// and saves those values in the associated location fields.  if it doesn't match that we are just
			// going to put whatever it is into the location name
				if (preg_match('/([A-z ]+), *([A-z ]+)/', $race_row->textual_data, $matches)) {
					$row->Location_city = $matches[1];
					$row->Location_province = $matches[2];
				} else {
					$row->Location_name = $race_row->textual_data;
				}
			} else if ($label = 'Race Description') {
				die('desc');
				$row->body = $row->textual_value;
			} else {
				$row->$label = $row->textual_data;
				
			//	watchdog('migrate', $eval);
			//	eval($eval);
			}
		}
	}
}

class TrizonesResourceMigration extends TrizonesNodeMigration {
  public function __construct(array $arguments) {
    
 		$this->sourceFields['Description'] = '';
    $this->sourceFields['language'] = '';
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
    $this->sourceFields['Perks'] = '';
 		$this->sourceFields['Perks_language'] = '';
    $this->sourceFields['ResourceDescription'] = '';
    $this->sourceFields['Registration'] = '';
    $this->sourceFields['Resource_type'] = '';
    // additional fields for term reference fields
    $this->sourceFields['Resource_type_create_term'] = '';
    $this->sourceFields['Resource_type_ignore_case'] = '';
    $this->sourceFields['Resource_type_source_type'] = '';
    $this->sourceFields['Website'] = '';

		$this->addFieldMapping('body', 'Description');
		$this->addFieldMapping('field_text', 'Perks');
		$this->addFieldMapping('field_text:language', 'Perks_language');
		$this->addFieldMapping('field_location', 'Location');
		$this->addFieldMapping('field_resource_logo', 'Logo');
		$this->addFieldMapping('field_resource_logo:file_class', 'Logo_file_class');
		$this->addFieldMapping('field_resource_logo:language', 'Logo_language');
    $this->addFieldMapping('field_resource_logo:destination_dir', 'Logo_destination_dir');
    $this->addFieldMapping('field_resource_logo:destination_file', 'Logo_destination_file');
    $this->addFieldMapping('field_resource_logo:file_replace', 'Logo_file_replace');
    $this->addFieldMapping('field_resource_logo:preserve_files', 'Logo_preserve_files');
    $this->addFieldMapping('field_resource_logo:source_dir', 'Logo_source_dir');
    $this->addFieldMapping('field_resource_logo:description', 'Logo_description');
    $this->addFieldMapping('field_resource_logo:display', 'Logo_display');
		$this->addFieldMapping('field_resource_type', 'resource_type');
		$this->addFieldMapping('field_resource_type:create_term', 'Resource_type_create_term');
		$this->addFieldMapping('field_resource_type:ignore_case', 'Resource_type_ignore_case');
		$this->addFieldMapping('field_resource_type:source_type', 'Resource_type_source_type');
		$this->addFieldMapping('field_url', 'Website');
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

    $resource_rows = Database::getConnection('default', $this->sourceConnection)
				->select('node', 'n');
		$query->join('node_revisions', 'r', 'n.nid = r.nid');
		$query->join('term_node', 'tn', 'n.nid = tn.nid');
		$query->join('flexinode_data', 'fd', 'n.nid = fd.nid');
		$query->join('flexinode_field', 'ff', 'fd.field_id = ff.field_id');
		$query->fields('fd', array('textual_data', 'serialized_data'));
		$query->fields('ff', array('label'));
		$query->fields('tn', array('tid'));
		$query->conditions('n.type', 'flexinode-3');
		$query->conditions('ff.ctype_id', '3')
        ->condition('r.vid', $row->vid)
                 ->execute()
                 ->fetchAll();
     
      
    if (!$resource_rows) {
    	$this->saveMessage(t('No Resource data for node with vid !vid',
                         array('!vid' => $row->vid)));
      return;
    }
		for($i=0; $i< count($resource_rows); $i++) {
			$resource_row = $resource_rows[$i];
			if ($i == 0) {
				$row->resource_type = $resource_row->tid;
				$row->resource_type_create_term = false;
				$row->resource_type_ignore_case = true;
				$row->resource_type_source_type = 'tid';
				
				$row->Perks_language = 'en';
				$row->language = 'en';
			}
			$label = str_replace(' ', '', $resource_row['label']);
			
			if ($label == 'Logo') {
				$values = unserialize($resource_row->serialized_data);
				$source_dir = '/Users/lhb/Sites/trizones/trizones_d4/'.$values->filepath;
				$row->Logo_file_class = 'MigrateFileUri';
				$row->Logo_language = 'en';
				$row->Logo_destination_dir = 'resources/logos';
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
