<?php

/**
 * We override the default SQL source class just so we can clean up subfield
 * names for the UI.
 */
class MigrateDrupal4NodeSourceSQL extends MigrateDrupal4SourceSQL {
  /**
   * Translation from field names assigned when executing the query to our
   * subfield notation.
   *
   * @var array
   *  key: DB-compatible name (e.g., field_buy_link_title).
   *  value: Subfield notation (e.g., field_buy_link:title).
   */
  public $fixFieldNames = array();

  public function __construct(SelectQueryInterface $query, array $fields = array(),
        SelectQueryInterface $count_query = NULL, array $options = array()) {
    $this->fixFieldNames = $options['fix_field_names'];
    parent::__construct($query, $fields, $count_query, $options);
  }

  public function fields() {
    $fields = parent::fields();
    // Remove the default subfield names in favor of the nice-looking ones.
    foreach ($this->fixFieldNames as $clean => $display) {
      if (isset($fields[$clean])) {
        unset($fields[$clean]);
      }
    }
    return $fields;
  }
}