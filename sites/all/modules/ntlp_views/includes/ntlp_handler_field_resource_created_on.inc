<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.

module_load_include('inc', 'ntlp_school', 'constants');

class ntlp_handler_field_resource_created_on extends views_handler_field {
  function construct() {
    parent::construct();
    
    $this->additional_fields['created'] = array('table' => 'node', 'field' => 'created');
    $this->additional_fields['nid'] = array('table' => 'node', 'field' => 'nid');
    $this->additional_fields['coursenid'] = array('table' => 'ntlp_course_resource', 'field' => 'course_nid');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

 function render($values) {
    try {
        $nid = $values->{$this->aliases['nid']};
        $created = $values->{$this->aliases['created']};
        $coursenid = $values->{$this->aliases['coursenid']};

        $output = get_tz_course($coursenid, NTLP_DATE_TZ_TIME, date('Y-m-d H:i:s', $created));
            
    }
    catch (Exception $e) {
        $output = $e->getMessage();
    }
    return $output;
  }



}

?>