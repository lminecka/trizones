<?php
/**
 * @file
 * Implementation of DrupalRoleMigration for Drupal 4 sources.
 */

/**
 * Handling specific to a Drupal 4 source for roles.
 */
class DrupalRole4Migration extends DrupalRoleMigration {
  /**
   * @param array $arguments
   */
  public function __construct(array $arguments) {
    parent::__construct($arguments);

    $this->addUnmigratedDestinations(array('weight'));
  }
}
