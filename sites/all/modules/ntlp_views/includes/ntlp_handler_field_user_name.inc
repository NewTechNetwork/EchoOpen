<?php

class ntlp_handler_field_user_name extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['first_name'] = array('table' => 'ntlp_user', 'field' => 'first_name');
    $this->additional_fields['last_name'] = array('table' => 'ntlp_user', 'field' => 'last_name');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function click_sort($order) {
    $tableu = $this->query->ensure_table('ntlp_user');
    $this->query->add_orderby($tableu, 'last_name', $order);
    $this->query->add_orderby($tableu, 'first_name', $order);
  }

 function render($values) {
    $fname = $values->{$this->aliases['first_name']};
    $lname = $values->{$this->aliases['last_name']};
    $fullname = '';
    if (isset($fname)) {
        $fullname .= $fname;
    }
    if (isset($lname)) {
        $fullname = $lname.', '.$fullname;
    }

    return $fullname;
  }
}

?>