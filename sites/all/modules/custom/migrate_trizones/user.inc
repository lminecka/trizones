<?php
/**
 * @file
 * Implementation of DrupalUserMigration for Drupal 4 sources.
 */

/**
 * Handling specific to a Drupal 4 source for users.
 */
class TrizonesUserMigration extends DrupalUser4Migration {
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);
    
    // Indicate that our incoming MD4 passwords should be rehashed for Drupal 7.
    $this->destination = new DrupalDestinationUser(array('md4_passwords' => TRUE));

    // Per-user comment settings dropped in Drupal 7.
    $this->addUnmigratedSources(array('mode', 'threshold', 'sort'));

    $this->addFieldMapping(NULL, 'timezone')
         ->description('If present, assigned to D7 timezone');
    $this->addFieldMapping('field_first_name:language', NULL);
    $this->addFieldMapping('field_last_name:language', NULL);
    $this->addFieldMapping('field_address', NULL);
		$this->addFieldMapping('field_address:name', NULL);
		$this->addFieldMapping('field_address:street', NULL);
		$this->addFieldMapping('field_address:additional', NULL);
		$this->addFieldMapping('field_address:city', NULL);
		$this->addFieldMapping('field_address:province', NULL);
		$this->addFieldMapping('field_address:postal_code', NULL);
		$this->addFieldMapping('field_address:latitude', NULL);
		$this->addFieldMapping('field_address:longitude', NULL);
		$this->addFieldMapping('field_address:country', NULL);
		$this->addFieldMapping('field_address:source', NULL);
		$this->addFieldMapping('field_address:is_primary', NULL);
    $this->addFieldMapping('field_publish_address', 'profile_publish_address');
  	$this->addFieldMapping(NULL, 'profile_publish_phone');
  	$this->addFieldMapping('field_usat_number', 'profile_usat');
  	$this->addFieldMapping('field_chip_number', 'profile_chip_number');
  	$this->addFieldMapping('field_weakest_sport', 'profile_weakest_sport');
  	$this->addFieldMapping('field_strongest_sport', 'profile_strongest_sport');
  	$this->addFieldMapping('field_swim_distance', 'profile_swim_distance');
  	$this->addFieldMapping('field_bike_distance', 'profile_bike_distance');
  	$this->addFieldMapping('field_bike_speed', 'profile_bike_speed');
  	$this->addFieldMapping('field_run_distance', 'profile_run_distance');
  	$this->addFieldMapping('field_run_speed', 'profile_run_speed');
  	$this->addFieldMapping('field_first_name', 'profile_first_name');
  	$this->addFieldMapping('field_last_name', 'profile_last_name');
  	$this->addFieldMapping('field_birthdate', 'profile_birthdate');
  	$this->addFieldMapping('field_decide', 'profile_decide');
  	$this->addFieldMapping('field_decide:language', NULL);
  	$this->addFieldMapping('field_goals', 'profile_goals');
  	$this->addFieldMapping('field_goals:language', NULL);
  	$this->addFieldMapping('field_referer', 'profile_referer');
  	$this->addFieldMapping('field_referer_other', 'profile_referer_other');
  	$this->addFieldMapping('field_referer_other:language', NULL);
  	$this->addFieldMapping('field_training_suggestions', 'profile_training_suggestions');
  	$this->addFieldMapping('field_training_suggestions:language', NULL);
  	$this->addFieldMapping('field_usat_expiration_date', 'profile_usat_expire');
  	$this->addFieldMapping('field_shirt_size', 'profile_shirt_size');
  	$this->addFieldMapping('field_team', 'profile_team');
  	
  }
  
  /**
   * Implementation of Migration::prepareRow().
   *
   * @param $row
   */
  public function prepareRow($row) {
    if (parent::prepareRow($row) === FALSE) {
      return FALSE;
    }
  	watchdog('migrate_lm_a', print_r($row, true));
    $bday_array = unserialize($row->profile_birthdate);
    $year  = $bday_array['year'];
		$month = ($bday_array['month'] < 9) ?  '0'.$bday_array['month'] : $bday_array['month'];
		$day   = ($bday_array['day'] < 9) ?  '0'.$bday_array['day'] : $bday_array['day'];
		$row->profile_birthdate = "$year-$month-$day";
		watchdog('migrate_lm_b', print_r($row, true));
  }
}
