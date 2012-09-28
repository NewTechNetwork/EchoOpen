<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


class ntlp_handler_field_activity_status extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['available_from'] = array('table' => 'ntlp_course_activity', 'field' => 'available_from');
    $this->additional_fields['available_to'] = array('table' => 'ntlp_course_activity', 'field' => 'available_to');
    $this->additional_fields['availability_mode'] = array('table' => 'ntlp_course_activity', 'field' => 'availability_mode');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

 function render($values) {
    $from = $values->{$this->aliases['available_from']};
    $to = $values->{$this->aliases['available_to']};
    $mode = $values->{$this->aliases['availability_mode']};
    $output = 'Hide';
    if (isset($from) && isset($to)) {
        $tfrom = strtotime($from);
        $tto = strtotime($to);
        $ttime = time();
        if ($mode == 'S') {
            if ($tfrom <= $ttime && $ttime <= $tto)
                $output = 'Show';
        } elseif ($mode == 'M') {
            if ($from != '0000-00-00 00:00:00')
                $output = 'Show';
        }
    }
    return $output;
  }
}

?>