<?php
class ntlp_handler_filter_random_projects extends views_handler_filter {
  // exposed filter options
  var $no_single = TRUE;
  // Don't display empty space where the operator would be.
  var $no_operator = TRUE;

  /**
   * Provide simple equality operator
   */
  function operator_options() {
    return array(
      'NONE' => t('None'),
    );
  }

  function query() {
    $rs = db_query('SELECT nid FROM {ntlp_library_project} WHERE is_exemplary=1');
    $allids = array();
    $index = 0;
    while ($row = db_fetch_array($rs)) {
        $allids[$index] = $row[$this->real_field];
        $index++;
    }
//    watchdog('random', print_r($allids, true));
//    $tolog='<div>Random numbers<br>';
    $uniqids = array();
    while (count($uniqids) < 5) {
        $random_number = mt_rand(0, $index-1);
//        $tolog .= "<br>$random_number = ".$allids[$random_number];
        if (!in_array($allids[$random_number], $uniqids)) {
            $uniqids[] = $allids[$random_number];
        }
    }
//    $tolog .= '</div>';
//    watchdog('random', $tolog);

    $values = implode(",", $uniqids);

    $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field in (" . $values . ")");
  }
}

