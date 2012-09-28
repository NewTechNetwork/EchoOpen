<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


/**
 * We do not try to run db_rewrite_sql() on this subquery so the count is potentially more than the user can see.
 */
class ntlp_handler_field_user_course_grades extends views_handler_field {
//  function query() {
////    $type = isset($this->options['calc_type']) ? $this->options['calc_type'] : 'CSUB';
////    $this->options['label'] = 'CALC_TYPE:'.$type;
//    //If there is an argument defined for OG, then use it as a Course Nid
//    $coursenid = 0;
//    foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
////        $this->options['label'] = $handler->ui_name();
//        if ($handler->ui_name() == "Organic groups: Group node")
//            $coursenid = $handler->get_value();
//    }
//
//    $type = 'ASGT';
//    switch ($type) {
//        case 'ASGT':        //All Student Grades - Teacher's view
//            $table = $this->query->ensure_table('ntlp_user');
//            $sql = "SELECT (sum(g.points)/sum(x.max_points))*100
//                    FROM {ntlp_gb_activity_grade} g, {ntlp_activity_submission} s, {ntlp_gb_activity_outcome} x,
//                        {ntlp_course_activity} a
//                    WHERE g.submission_nid = s.nid
//                    AND g.tid = x.tid
//                    AND s.activity_nid = x.activity_nid
//                    AND s.activity_nid = a.nid
//                    AND a.course_nid = $coursenid
//                    AND s.user_uid = $table.uid";
//            $this->query->add_field('', "($sql)", 'stu_grades_percent');
//            $this->field_alias = 'stu_grades_percent';
//            break;
//    }
//  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['calc_type'] = array(
        '#type' => 'radios',
        '#title' => 'Grade output as',
        '#description' => t('Select grade output'),
        '#options' => array(
            'L' => 'Grade Letter',
            'P' => 'Grade Percentage',
            ),
        '#default_value' => isset($this->options['calc_type']) ? $this->options['calc_type'] : 'P',
        '#required' => TRUE,
    );

  }

  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = array('table' => 'users', 'field' => 'uid');
  }
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $uid = $values->{$this->aliases['uid']};

    $type = isset($this->options['calc_type']) ? $this->options['calc_type'] : 'P';
    
    //If there is an argument defined for OG, then use it as a Course Nid
    $course_nid = 0;
    foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
        // If argument is a node handler
        if (strcmp(strtolower($handler->ui_name()), "node") > 0)
            $course_nid = $handler->get_value();
    }
    $term_tid = get_current_selected_course_term();
    $percent = _get_student_course_percentage( $uid, $course_nid, $term_tid );

    switch ($type) {
        case 'L':
            $output = _get_course_grade($percent, $course_nid, 'L');
            break;
        case 'P':
            $output = format_course_grade_percent($percent, $course_nid);
            break;
    }
    return $output;
  }
}