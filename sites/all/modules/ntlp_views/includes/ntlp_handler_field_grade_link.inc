<?php
module_load_include('inc', 'ntlp_school', 'data_access');

class ntlp_handler_field_grade_link extends views_handler_field {
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
//        if ($us->is_graded) {                 //As per Chris 03/10/2010 Show link even if its graded
//            $output = "Graded";               //And rename the link as Score instead of Grade
//        } else {

        //Teachers want to score students on their offline submissions
        //therefore score link is required even if student have not submitted
//            if ($us->is_complete) {
                $link = url("ntlp/courses/activity/submission/grade/edit/$us->course_nid/$us->nid");
                $output = "<a class='jslink' id='grade_$auid' href='$link'>Score</a>";
                break;
//            }
//        }
    }
    return $output;
  }
}
/**
 * The following version uses activity_submission table, but this creating a problem of OUTER JOINS
 */
//class ntlp_handler_field_grade_link extends views_handler_field {
//  function construct() {
//    parent::construct();
//    $this->additional_fields['is_graded'] = array('table' => 'ntlp_activity_submission', 'field' => 'is_graded');
//    $this->additional_fields['is_complete'] = array('table' => 'ntlp_activity_submission', 'field' => 'is_complete');
//    $this->additional_fields['user_uid'] = array('table' => 'ntlp_activity_submission', 'field' => 'user_uid');
//    $this->additional_fields['nid'] = array('table' => 'ntlp_activity_submission', 'field' => 'nid');
//  }
//
//  function query() {
//    $this->ensure_my_table();
//    $this->add_additional_fields();
//  }
//
// function render($values) {
//    $nid = $values->{$this->aliases['nid']};
//    $uid = $values->{$this->aliases['user_uid']};
//    $iscomplete = $values->{$this->aliases['is_complete']};
//    $output = '';
//    if ($values->{$this->aliases['is_graded']}) {
//        $output = "Graded";
//    } else {
//        if ($iscomplete) {
//            $link = url("ntlp/activity/submission/grade/".$nid);
//            $output = "<a id='grade_$uid' href='$link'>Grade</a>";
//        }
//    }
//    return $output;
//  }
//}

?>