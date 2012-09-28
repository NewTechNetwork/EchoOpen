<?php
/**
 * Sort handler to sort nodes by Flag:likethispost
 */
class ntlp_handler_sort_liked extends views_handler_sort {
  function query() {
    $this->ensure_my_table();
    $join = new views_join();
    $join->construct('flag_counts', $this->table_alias, 'nid', 'content_id');

    $this->user_table = $this->query->ensure_table('node_flag_counts', $this->relationship, $join);
    $this->query->add_where($this->options['group'], "$this->user_table.fid = 2 OR $this->user_table.fid IS NULL");      //ID of LIKETHISPOST
    $this->query->add_where($this->options['group'], "$this->user_table.content_type = 'node' OR $this->user_table.content_type IS NULL");

    // Add the field.
    $this->query->add_orderby($this->user_table, "count", $this->options['order']);
  }
}

