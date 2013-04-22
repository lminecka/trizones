<?php

/**
 * We override the default SQL source class just so we can clean up subfield
 * names for the UI.
 */
class MigrateDrupal4SourceSQL extends MigrateSourceSQL {

  public function __construct(SelectQueryInterface $query, array $fields = array(),
        SelectQueryInterface $count_query = NULL, array $options = array()) {
    parent::__construct($query, $fields, $count_query, $options);
  }
  
  /**
   * Return a count of available source records, from the cache if appropriate.
   * Returns -1 if the source is not countable.
   *
   * @param boolean $refresh
   */
  public function count($refresh = FALSE) {
 
    if ($this->skipCount) {
      return -1;
    }

    if (!isset($this->cacheKey)) {
      $this->cacheKey = md5((string)$this);
    }

    // If a refresh is requested, or we're not caching counts, ask the derived
    // class to get the count from the source.
    if ($refresh || !$this->cacheCounts) {
      $count = $this->computeCount();
      cache_set($this->cacheKey, $count, 'cache');
    }
    else {
      // Caching is in play, first try to retrieve a cached count.
      $cache_object = cache_get($this->cacheKey, 'cache');
      if (is_object($cache_object)) {
        // Success
        $count = $cache_object->data;
      }
      else {
        // No cached count, ask the derived class to count 'em up, and cache
        // the result
        $count = $this->computeCount();
        cache_set($this->cacheKey, $count, 'cache');
      }
    }
    return $count;
  }
}