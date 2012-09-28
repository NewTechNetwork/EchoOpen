<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


class ntlp_handler_field_submission_status extends views_handler_field {
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

    $output = 'Not opened';
    $subs = get_user_submission($anid, $auid, true);
    while ($us = db_fetch_object($subs)) {
        if (!$us->is_graded) {
            if (!isset($us->date_opened)) {
                $output = 'Not opened';
            } else {
                if (!isset($us->date_submitted) || !$us->is_complete) {
                    $output = 'Incomplete';
                } else {
                    $output = 'Submitted';
                }
            }
        } else {
            $output = 'Graded';
        }
        break;
    }
    return $output;
  }

}
/**
 * The following version uses activity_submission table, but this creating a problem of OUTER JOINS
 */
//class ntlp_handler_field_submission_status extends views_handler_field {
//  function construct() {
//    parent::construct();
//    $this->additional_fields['date_opened'] = array('table' => 'ntlp_activity_submission', 'field' => 'date_opened');
//    $this->additional_fields['date_submitted'] = array('table' => 'ntlp_activity_submission', 'field' => 'date_submitted');
//    $this->additional_fields['is_complete'] = array('table' => 'ntlp_activity_submission', 'field' => 'is_complete');
//    $this->additional_fields['is_graded'] = array('table' => 'ntlp_activity_submission', 'field' => 'is_graded');
//  }
//
//  function query() {
//    $this->ensure_my_table();
//    $this->add_additional_fields();
//  }
//
// function render($values) {
//    $dopened = $values->{$this->aliases['date_opened']};
//    $dsubmitted = $values->{$this->aliases['date_submitted']};
//    $graded = $values->{$this->aliases['is_graded']};
//    $completed = $values->{$this->aliases['is_complete']};
//    if (!isset($dopened)) {
//        $output = 'Not opened';
//    } else {
//        if (!isset($dsubmitted) || !$completed) {
//            $output = 'Incomplete';
//        } else {
//            if (!$graded) {
//                $output = 'Submitted';
//            } else {
//                $output = 'Graded';
//            }
//        }
//    }
//    return $output;
//  }
//}

?>