<?php
/**
 * @file
 * Implementation of DrupalTermMigration for Drupal 4 sources.
 */

/**
 * Handling specific to a Drupal 4 source for taxonomy terms.
 */
abstract class Drupal4TermMigration extends DrupalTermMigration {
  public function __construct(array $arguments) {
    $this->sourceFields['parent'] = t('Term: Parent term');
    parent::__construct($arguments);

    // Drupal 4 had no format for terms.
    $this->addFieldMapping('format');
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

    // Add the (potentially multiple) parents
    $result = Database::getConnection('default', $this->sourceConnection)
              ->select('term_hierarchy', 'th')
              ->fields('th', array('parent'))
              ->condition('tid', $row->tid)
              ->execute();
    foreach ($result as $parent_row) {
      $row->parent[] = $parent_row->parent;
    }
  }
}
