<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'constants');

class ntlp_handler_field_submission_date extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['user_uid'] = array('table' => 'ntlp_activity_user', 'field' => 'user_uid');
    $this->additional_fields['activity_nid'] = array('table' => 'ntlp_activity_user', 'field' => 'activity_nid');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {

    $anid = $values->{$this->aliases['activity_nid']};
    $auid = $values->{$this->aliases['user_uid']};

    $output = '';
    $subs = get_user_submission($anid, $auid);

    while ($us = db_fetch_object($subs)) {
                $school_nid = get_course_school_nid($us->course_nid);

        if (isset($us->date_submitted)) {

            $output = get_tz($school_nid, NTLP_DATE_TZ_TIME, $us->date_submitted);
//            $output = date('m/d/Y - g:ia', strtotime($us->date_submitted));
        } elseif (isset($us->date_graded)) {
            $output = get_tz($school_nid, NTLP_DATE_TZ_TIME, $us->date_graded);
            //$output .= date('m/d/Y - g:ia', strtotime($us->date_graded));
            $output .= ' (Offline)';
        }
        break;
    }
    return $output;
  }

}

?>