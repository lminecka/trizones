<?php
/**
 * @file
 * Implementation of DrupalUserMigration for Drupal 4 sources.
 */

/**
 * Handling specific to a Drupal 4 source for users.
 */
class DrupalUser4Migration extends DrupalUserMigration {
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
    $this->addFieldMapping('field_publish_address', 'profile_publish_address');
  	$this->addFieldMapping(NULL, 'profile_publish_phone');
  	$this->addFieldMapping('field_usat', 'profile_usat');
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
  	$this->addFieldMapping('field_goals', 'profile_goals');
  	$this->addFieldMapping('field_referer', 'profile_referer');
  	$this->addFieldMapping('field_referer_other', 'profile_referer_other');
  	$this->addFieldMapping('field_training_suggestions', 'profile_training_suggestions');
  	$this->addFieldMapping('field_usat_expire', 'profile_usat_expire');
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

    /**
     * Note that the date module adds a timezone_name column to the users table,
     * so if present we can use that directly. Otherwise, we do as the D6->D7
     * upgrade does and just clear it - let users reset their timezones in the
     * D7 site.
     */
    if (!empty($row->timezone_name)) {
      $row->timezone = $row->timezone_name;
    }
    else {
      $row->timezone = NULL;
    }
  }
}
