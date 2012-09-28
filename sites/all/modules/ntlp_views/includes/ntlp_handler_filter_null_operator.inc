<?php
class ntlp_handler_filter_null_operator extends views_handler_filter {
  // exposed filter options
  var $no_single = TRUE;
  // Don't display empty space where the operator would be.
  var $no_operator = TRUE;

  /**
   * Provide simple equality operator
   */
  function operator_options() {
    return array(
      'IS NULL' => t('Is NULL'),
      'IS NOT NULL' => t('Is not NULL'),
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field " . $this->operator);
  }
}

