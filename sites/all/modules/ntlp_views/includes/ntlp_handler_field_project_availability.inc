<?php

class ntlp_handler_field_project_availability extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['availability'] = array('table' => 'ntlp_course', 'field' => 'availability');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

 function render($values) {
    $availability = $values->{$this->aliases['availability']};
    $output = 'Hide';
    if (isset($availability) && $availability == 'S') {
        $output = 'Show';
    }
    return $output;
  }
}

?>