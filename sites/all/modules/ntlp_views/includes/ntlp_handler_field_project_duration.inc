<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


class ntlp_handler_field_project_duration extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['from_date'] = array('table' => 'ntlp_course', 'field' => 'from_date');
    $this->additional_fields['to_date'] = array('table' => 'ntlp_course', 'field' => 'to_date');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

 function render($values) {

    $from = $values->{$this->aliases['from_date']};
    $to = $values->{$this->aliases['to_date']};
    $output = 'N/A';

    if (isset($from) && isset($to)) {
        //$tfrom = strtotime($from);
        //$tto = strtotime($to);
        //$output = date('m/d/Y', $tfrom).' - '.date('m/d/Y', $tto);
        $school_nid = get_project_school_nid($values->nid);
        $output = get_tz($school_nid,'m/d/Y', $from).' - '.get_tz($school_nid,'m/d/Y', $to);

    }
    return $output;
  }
}

?>