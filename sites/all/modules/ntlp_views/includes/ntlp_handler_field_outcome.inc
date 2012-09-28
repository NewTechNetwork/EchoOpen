<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


/**
 * We do not try to run db_rewrite_sql() on this subquery so the count is potentially more than the user can see.
 */
class ntlp_handler_field_outcome extends views_handler_field {

    function options(&$options) {
        parent::options($options);
        $options['outcome_tid'] = '';
    }

    function options_form(&$form, &$form_state) {
        parent::options_form($form, $form_state);

        //The following field is not required as activities/students can be filtered using arguments or filters
//    $form['course_nid'] = array(
//      '#type' => 'textfield',
//      '#title' => t('NTLP Course Node ID'),
//      '#default_value' => isset($this->options['course_nid']) ? $this->options['course_nid'] : '',
//      '#description' => t('Enter the NTLP Course Node ID for which we want to filter your data.'),
//      '#required' => TRUE,
//    );

        $form['outcome_tid'] = array(
            '#type' => 'textfield',
            '#title' => t('Outcome (Taxonomy term) ID'),
            '#default_value' => isset($this->options['outcome_tid']) ? $this->options['outcome_tid'] : '',
            '#description' => t('The Outcome (Taxonomy term) ID of the data you want in this field. Blank this field to get aggregate of all outcomes.'),
            '#required' => TRUE,
        );

        $form['outcome_name'] = array(
            '#type' => 'textfield',
            '#title' => t('Outcome Name'),
            '#default_value' => isset($this->options['outcome_name']) ? $this->options['outcome_name'] : '',
            '#description' => t('Leave empty for default.'),
            '#required' => FALSE,
        );

        $form['calc_type'] = array(
            '#type' => 'radios',
            '#title' => 'Calculation Type',
            '#description' => t('Select the source and calculation type for the value you want to be displayed in this field.'),
            '#options' => array(
                'PGX' => 'Activity: Points earned and max points for each activity of a student.',
                'AMP' => 'Activity: Max Points',
                'SAS' => 'Submission: Sum of all scores of all students for an activity',
                'SAA' => 'Submission: Sum of all scores for all activities of a student',
                'GAS' => 'Submission: Grade of all scores of all students for an activity',
                'GAA' => 'Submission: Grade of all scores for all activities of a student',
                'GSA' => 'Submission: Grade of all scores for an activity of a student',
            ),
            '#default_value' => isset($this->options['calc_type']) ? $this->options['calc_type'] : '',
            '#required' => TRUE,
        );
    }

    function query() {
        $otid = isset($this->options['outcome_tid']) ? $this->options['outcome_tid'] : 0;
        $otname = isset($this->options['outcome_name']) ? $this->options['outcome_name'] : "Outcome $otid";
        $type = isset($this->options['calc_type']) ? $this->options['calc_type'] : 0;
        $this->options['label'] = $otname;

        //If there is an argument defined for OG, then use it as a Course Nid
        $coursenid = 0;
        foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
            if ($handler->ui_name() == "Organic groups: Group node")
                $coursenid = $handler->get_value();
        }
        if (empty($coursenid))
            $coursenid = 0;      //To protect queries in returning error

            switch ($type) {
            case 'PGX': //Activity: Points earned and max points for each activity of a student.
                if (empty($coursenid)) {

                    $tablea = $this->query->ensure_table('ntlp_course_activity');
                    $tableu = $this->query->ensure_table('ntlp_activity_user');
//                $sql = "SELECT concat( TRIM(TRAILING '.' FROM TRIM(TRAILING '0' FROM g.points)), ' / ', x.max_points )
//                    FROM {ntlp_gb_activity_grade} g, {ntlp_activity_submission} s, {ntlp_gb_activity_outcome} x
//                    WHERE g.submission_nid = s.nid
//                    AND g.tid = x.tid
//                    AND s.activity_nid = x.activity_nid
//                    AND s.is_graded = 1 AND $tablea.is_published = 1
//                    AND s.activity_nid = $tablea.nid
//                    AND s.user_uid = $tableu.user_uid";

                    // changes by saif
                    $sql = "CASE WHEN (SELECT COUNT(*) FROM dpl_ntlp_gb_activity_grade g,
                    dpl_ntlp_activity_submission s, dpl_ntlp_gb_activity_outcome x, dpl_ntlp_gb_activity_excuse ex
                    WHERE g.submission_nid = s.nid AND s.resubmit_count = 1 AND ex.submission_nid = s.nid AND ex.tid = $otid AND
                    s.activity_nid = x.activity_nid AND s.is_graded = 1 AND $tablea.is_published = 1
                    AND s.activity_nid = $tablea.nid
                    AND s.user_uid = $tableu.user_uid )
                    THEN 'EX'
                    ELSE (SELECT CONCAT( TRIM(TRAILING '.' FROM TRIM(TRAILING '0' FROM g.points)), ' / ', x.max_points ) FROM dpl_ntlp_gb_activity_grade g,
                    dpl_ntlp_activity_submission s, dpl_ntlp_gb_activity_outcome x
                    WHERE g.submission_nid = s.nid AND s.resubmit_count = 1 AND g.tid = x.tid AND
                     s.activity_nid = x.activity_nid AND s.is_graded = 1 AND $tablea.is_published = 1
                    AND s.activity_nid = $tablea.nid
                    AND s.user_uid = $tableu.user_uid";

                    $sql .= empty($otid) ? "" : " AND g.tid = $otid AND x.tid = $otid ) END ";
                    
                } else {

                    $tableu = $this->query->ensure_table('users');
                    $sql = "SELECT concat( TRIM(TRAILING '.' FROM TRIM(TRAILING '0' FROM g.points)), ' / ', x.max_points )
                    FROM {ntlp_gb_activity_grade} g, {ntlp_activity_submission} s, {ntlp_gb_activity_outcome} x,
                        {ntlp_course_activity} a
                    WHERE g.submission_nid = s.nid
                    AND g.tid = x.tid
                    AND s.activity_nid = x.activity_nid
                    AND s.activity_nid = a.nid
                    AND s.is_graded = 1 AND a.is_published = 1
                    AND a.course_nid = $coursenid
                    AND s.user_uid = $tableu.uid";
                    
                    $sql .= empty($otid) ? "" : " AND g.tid = $otid AND x.tid = $otid";
                    $sql .= " ORDER BY g.points DESC LIMIT 0,1";        //So that max points earned are used due to multiple submissions
                }
                
                
                break;
            case 'AMP':
                $table = $this->query->ensure_table('ntlp_course_activity');
                $sql = "SELECT g.max_points FROM {ntlp_gb_activity_outcome} g, {ntlp_course_activity} a
                    WHERE a.nid = g.activity_nid
                    AND g.activity_nid = $table.nid";
                $sql .= empty($otid) ? "" : " AND g.tid = $otid";
                break;
            case 'SAS':
                $tablea = $this->query->ensure_table('ntlp_course_activity');
                $tableu = $this->query->ensure_table('ntlp_activity_user');
                $sql = "SELECT sum(g.points) FROM {ntlp_gb_activity_grade} g, {ntlp_activity_submission} s
                WHERE g.submission_nid = s.nid
                AND s.is_graded = 1 AND $tablea.is_published = 1
                AND s.activity_nid = $tablea.nid
                AND s.user_uid = $tableu.user_uid";
                $sql .= empty($otid) ? "" : " AND g.tid = $otid";
                break;
            case 'SAA': //Submission: Sum of all scores for all activities of a student
                $table = $this->query->ensure_table('ntlp_activity_user');
                $sql = "SELECT sum(g.points)
                FROM {ntlp_gb_activity_grade} g, {ntlp_activity_submission} s, {ntlp_course_activity} a
                WHERE g.submission_nid = s.nid
                AND a.nid = s.activity_nid
                AND s.is_graded = 1 AND a.is_published = 1
                AND s.user_uid = $table.user_uid";
                $sql .= empty($otid) ? "" : " AND g.tid = $otid";
                break;
            case 'GAS': //Submission: Grade of all scores of all students for an activity
                $table = $this->query->ensure_table('ntlp_course_activity');
                $sql = "SELECT (sum(g.points)/sum(x.max_points))*100
                FROM {ntlp_gb_activity_grade} g, {ntlp_activity_submission} s, {ntlp_gb_activity_outcome} x
                WHERE g.submission_nid = s.nid
                AND g.tid = x.tid
                AND s.activity_nid = x.activity_nid
                AND s.is_graded = 1 AND $table.is_published = 1
                AND s.activity_nid = $table.nid";
                $sql .= empty($otid) ? "" : " AND g.tid = $otid AND x.tid = $otid";
                break;
            case 'GAA': //Submission: Grade of all scores for all activities of a student
                if (empty($coursenid)) {
                    $tableu = $this->query->ensure_table('ntlp_activity_user');
                    $sql = "SELECT (sum(g.points)/sum(x.max_points))*100
                    FROM {ntlp_gb_activity_grade} g, {ntlp_activity_submission} s, {ntlp_gb_activity_outcome} x,
                        {ntlp_course_activity} a
                    WHERE g.submission_nid = s.nid
                    AND a.nid = s.activity_nid
                    AND g.tid = x.tid
                    AND s.activity_nid = x.activity_nid
                    AND s.is_graded = 1 AND a.is_published = 1
                    AND s.user_uid = $tableu.user_uid";
                } else {
                    $tableu = $this->query->ensure_table('users');
                    $sql = "SELECT (sum(g.points)/sum(x.max_points))*100
                    FROM {ntlp_gb_activity_grade} g, {ntlp_activity_submission} s, {ntlp_gb_activity_outcome} x,
                        {ntlp_course_activity} a
                    WHERE g.submission_nid = s.nid
                    AND g.tid = x.tid
                    AND s.activity_nid = x.activity_nid
                    AND s.activity_nid = a.nid
                    AND s.is_graded = 1 AND a.is_published = 1
                    AND a.course_nid = $coursenid
                    AND s.user_uid = $tableu.uid";
                }
                $sql .= empty($otid) ? "" : " AND g.tid = $otid AND x.tid = $otid";
                break;
            case 'GSA': //Submission: Grade of all scores for an activity of a student
                $tablea = $this->query->ensure_table('ntlp_course_activity');
                $tableu = $this->query->ensure_table('ntlp_activity_user');
                $sql = "SELECT (sum(g.points)/sum(x.max_points))*100
                FROM {ntlp_gb_activity_grade} g, {ntlp_activity_submission} s, {ntlp_gb_activity_outcome} x
                WHERE g.submission_nid = s.nid
                AND g.tid = x.tid
                AND s.activity_nid = x.activity_nid
                AND s.is_graded = 1 AND $tablea.is_published = 1
                AND s.activity_nid =  $tablea.nid
                AND s.user_uid = $tableu.user_uid";
                $sql .= empty($otid) ? "" : " AND g.tid = $otid AND x.tid = $otid";
                break;
        }

//    $sql = str_replace('%ACTIVITY', $table, $sql);
        $this->query->add_field('', "($sql)", 'outcome_' . $otid);
        $this->field_alias = 'outcome_' . $otid;
    }

    function render($values) {
        $value = $values->{$this->field_alias};
//    if ((int) $value === 0) {
//      return '';
//    }
        $type = isset($this->options['calc_type']) ? $this->options['calc_type'] : 0;
        switch ($type) {
            case 'PGX': //Activity: Points earned and max points for each activity of a student.
            case 'AMP':
                return $value;
                break;
            case 'SAS':
                return number_format($value, 0, '.', '');
                break;
            case 'SAA': //Submission: Sum of all scores for all activities of a student
                return number_format($value, 0, '.', '');
                break;
            case 'GAS': //Submission: Grade of all scores of all students for an activity
                return number_format($value, 0, '.', '') . '%';
                break;
            case 'GAA': //Submission: Grade of all scores for all activities of a student
                return number_format($value, 0, '.', '') . '%';
                break;
            case 'GSA': //Submission: Grade of all scores for an activity of a student
                return number_format($value, 0, '.', '') . '%';
        }
    }

}