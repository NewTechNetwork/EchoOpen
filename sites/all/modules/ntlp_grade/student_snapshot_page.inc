<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');

function student_snapshot_frm($student_id) {
    global $user;

//    $mainmenu = menu_get_item("ntlp/grades");
//    menu_set_item(NULL, $mainmenu);

    set_item_url_handler('Snapshot Page');

    if (isset($user->roles[NTLP_ROLEID_PARENT])) {
        $student_id = $_SESSION['PARENT_STUDENT_UID'];
    } else if (isset($user->roles[NTLP_ROLEID_STUDENT])) {

        if ($user->uid != $student_id) {

            drupal_set_message("You are not authorized to access this student account information", "error");
            drupal_goto('ntlp/goback');
        }
    }/* else if(isset($user->roles[NTLP_ROLEID_TEACHER])){ //teacher should access student of the same school only

      if(get_this_school() != get_user_school($student_id) ){
      drupal_set_message("You are not authorized to access other school student account information", "error");
      drupal_goto('ntlp/goback');
      }
      }
     */
    return drupal_get_form('student_snapshot_form', $student_id);
}

function get_current_term() {

    $school_terms = get_ntlp_school_term(5);

//    $today = date("n-j-Y");
    $current_term = "";
    $today = date("n-j");

    foreach ($school_terms as $term_Obj) {
        $start_date = $term_Obj->month . '-' . $term_Obj->day;
        $close_date = $term_Obj->to_month . '-' . $term_Obj->to_day;

        if ($today >= $start_date && $today <= $close_date) {
            $current_term = $term_Obj->tid;
            break;
        }
    }

    return taxonomy_get_term($current_term);
}

function student_snapshot_form($form_state = null, $student_id) {
    global $user;
    global $base_path;

    include ('ntlp_grade.template.inc');

    drupal_add_js(drupal_get_path('module', 'ntlp_grade') . '/ntlp_grade.js');

    $school_id = _get_student_school($student_id);

//    $student_id = 7;
    modalframe_parent_js();

    $student_Obj = get_user_detail_($student_id);

    $form = array();

    $form['courses'] = array(
        '#type' => 'fieldset',
        '#tree' => TRUE,
    );

    $form['courses']['main_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<div id="BoldrProject"><div class="mainProjectHeading">',
        '#suffix' => '</div><div class="clearall"></div></div>',
    );
    if (user_access('view grades')) {
        $form['courses']['main_heading']['bradcrumb'] = array(
            '#type' => 'item',
            '#value' => '<a href="?q=ntlp/grades/students">&laquo; Back to Students</a>',
            '#prefix' => '<div class="BrandCrumb">',
            '#suffix' => '</div>',
        );
    }
    $form['courses']['main_heading']['header'] = array(
        '#type' => 'item',
        '#value' => '<b>' . t("Student's Snapshot") . '</b>',
        '#prefix' => '<div class="Heading">',
        '#suffix' => '</div>',
    );

    $form['courses']['main_table'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => $Students_Snapshot_hdr . '<div class="ProjectDetail-Table"><table cellpadding="4" border="0" width="100%">',
        '#suffix' => '</table></div>' . $Students_Snapshot_ftr,
    );



    $form['courses']['main_table']['row0'] = array(
        '#type' => 'item',
        '#prefix' => '<tr>',
        '#suffix' => '</tr>',
    );

    $picture = user_load($student_id);

    $user_picture = $picture->picture;

    if ($user_picture == 0 || $user_picture == null) {

        $user_image = $base_path . 'themes/Boldr/Images/common/small_person_image_not_found.jpg';
    } else {

        $user_image = $base_path . $picture->picture;
    }

    $form['courses']['main_table']['row0']['student_details'] = array(
        '#type' => 'item',
        '#value' => '<a href="#"><img src =' . $user_image . ' height ="150" width="150" />',
        '#prefix' => '<td style="width:160px;" align="center" rowspan="4">',
        '#suffix' => '</td>',
    );


    $form['courses']['main_table']['row0']['student_name'] = array(
        '#type' => 'item',
        '#value' => t($student_Obj->last_name . ', ' . $student_Obj->first_name),
        '#prefix' => '<th style="width:160px;border-bottom:1px solid #D5E9F5;">Name</th><td style="width:auto">',
        '#suffix' => '</td>',
    );

    $form['courses']['main_table']['row2'] = array(
        '#type' => 'item',
        '#prefix' => '<tr>',
        '#suffix' => '</tr>',
    );
    $form['courses']['main_table']['row2']['student_grade_calc'] = array(
        '#type' => 'item',
        '#value' => ($grade = (int) _get_student_grade_by_grad_year($student_Obj->grad_year)) ? $grade . 'th' : 'Not defined',
        '#prefix' => '<th style="border-bottom:1px solid #D5E9F5;">Grade</th><td style="width:auto">',
        '#suffix' => '</td>',
    );


    $form['courses']['main_table']['row3'] = array(
        '#type' => 'item',
        '#prefix' => '<tr>',
        '#suffix' => '</tr>',
    );


    if ($data = get_student_advisory_teacher($student_Obj->uid)) {
        $advisor = user_load($data->instructor_uid);
    }

    $form['courses']['main_table']['row3']['student_advisor'] = array(
        '#type' => 'item',
        '#value' => (isset($advisor) ? $advisor->firstname . ' ' . $advisor->lastname : 'Not Selected'),
        '#prefix' => '<th>' . t('Advisory Teacher ') . '</th><td style="width:auto">',
        '#suffix' => '</td>',
    );

//    $form['courses']['main_table']['row4'] = array(
//            '#type' => 'item',
//            '#prefix' => '<tr>',
//            '#suffix' => '</tr>',
//    );
    $current_term = get_current_selected_term();
    $school_term_Obj = get_ntlp_school_term($school_id, true, $current_term);

//    watchdog('schoolTerms',print_r( $school_term_Obj, true));

    foreach ($school_term_Obj as $obj) {
        if ($user_courses = get_user_term_courses($student_Obj->uid, $obj->school_term_tid, true, true)) {
            $term_obj = taxonomy_get_term($obj->school_term_tid);

            $form['courses']['main_table2' . $obj->school_term_tid] = array(
                '#type' => 'item',
                '#prefix' => '<div>',
                '#suffix' => '</div>',
            );
            $form['courses']['main_table2' . $obj->school_term_tid]['rows5'] = array(
                '#type' => 'item',
                '#prefix' => $Students_Snapshot_hdr . '<div class="ProjectDetail-Table"><div>',
                '#suffix' => '</div>',
            );

            $form['courses']['main_table2' . $obj->school_term_tid]['rows5_' . $obj->school_term_tid]['term_' . $obj->school_term_tid] = array(
                '#type' => 'item',
                '#value' => $term_obj->name,
                '#prefix' => '<div style="background-color:#9ccfef;display:table;width:100%;"><div style="float:left;padding:5px" class="HeadingAndEditBtn"><div class="Heading"><h3 style="font-size:15px"> ',
                '#suffix' => '</h3></div></div>',
            );

            drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/jquery.tablesorter/jquery.tablesorter.js');
            drupal_add_css(drupal_get_path('module', 'ntlp_school') . '/jquery.tablesorter/themes/blue/styles.css', 'module', 'all', FALSE);

            $form['courses']['main_table2' . $obj->school_term_tid]['rows5_' . $obj->school_term_tid]['link_' . $obj->school_term_tid] = array(
                '#type' => 'item',
                '#value' => '',
                '#prefix' => '<div style="float:right;padding:5px">',
                '#suffix' => '</div></div>',
            );


            // USER COURSES START
            $head = array(
                array('data' => t('Course'), 'field' => 'title', 'sort' => 'asc', 'style' => 'text-align:left'),
//                array('data' => t('Courses Short Name'), 'field' => 'course_short_title', 'sort' => 'asc'),
                array('data' => t('Teacher'), 'field' => 'last_name', 'sort' => 'asc', 'style' => 'text-align:left'),
                array('data' => t('Period'), 'field' => 'period', 'sort' => 'asc', 'style' => 'text-align:left'),
                array('data' => t('Grade'), 'field' => 'course_short_title', 'sort' => 'asc', 'style' => 'text-align:left'),
            );
            $attributes = array('class' => 'ntlp_table', 'width' => '815px', 'cellpadding' => '10');

            //Get Enrolled Courses
            $rows = get_grades_by_course(true, $obj->school_term_tid, $user_courses, $student_id);

            $form['courses']['main_table2' . $obj->school_term_tid]['rows7_' . $obj->school_term_tid]['Gradebycourse'] = array(
                '#type' => 'item',
                '#value' => 'Enrolled Courses',
                '#prefix' => '<div class="HeadingAndEditBtn"><div class="Heading"><br clear="all" /><h3 class="courses_snapshot_innerpanel_heading' . $obj->school_term_tid . '" style="font-size:12px">',
                '#suffix' => '</h3></div></div>',
            );

            $form['courses']['main_table2' . $obj->school_term_tid]['rows9_' . $obj->school_term_tid]['view_' . $obj->school_term_tid] = array(
                '#type' => 'item',
                '#value' => theme_table($head, $rows, $attributes),
                '#prefix' => '<br clear="all" /><div id="course_' . $obj->school_term_tid . '">',
                '#suffix' => '</div>',
            );

            //Get Dropped Courses
            $rows = get_grades_by_course(false, $obj->school_term_tid, $user_courses, $student_id);
            if (count($rows) > 0) {
                $form['courses']['main_table2' . $obj->school_term_tid]['rows7d_' . $obj->school_term_tid]['Gradebycourse'] = array(
                    '#type' => 'item',
                    '#value' => 'Dropped Courses',
                    '#prefix' => '<div class="HeadingAndEditBtn"><div class="Heading"><br clear="all" /><h3 class="d_courses_snapshot_innerpanel_heading' . $obj->school_term_tid . '" style="font-size:12px">',
                    '#suffix' => '</h3></div></div>',
                );

                $form['courses']['main_table2' . $obj->school_term_tid]['rows9d_' . $obj->school_term_tid]['view_' . $obj->school_term_tid] = array(
                    '#type' => 'item',
                    '#value' => theme_table($head, $rows, $attributes),
                    '#prefix' => '<br clear="all" /><div id="d_course_' . $obj->school_term_tid . '">',
                    '#suffix' => '</div>',
                );
            }
            // USER COURSES END

            $form['courses']['main_table2' . $obj->school_term_tid]['rows8_' . $obj->school_term_tid] = array(
                '#type' => 'item',
                '#prefix' => '<div>',
                '#suffix' => '</div>',
            );

            $form['courses']['main_table2' . $obj->school_term_tid]['rows8_' . $obj->school_term_tid]['view_' . $obj->school_term_tid] = array(
                '#type' => 'item',
                '#value' => get_school_wide_learning_outcomes_contents($obj->school_term_tid, $student_Obj->uid, true),
                '#prefix' => '<div id="show_' . $obj->school_term_tid . '">',
                '#suffix' => '</div></div>' . $Students_Snapshot_ftr,
            );
        }
    }

    return $form;
}

function get_grades_by_course($enrolled_courses, $school_term_tid, $user_courses, $student_id) {
    global $user;
    $rows = array();
    foreach ($user_courses as $record_course) {
        if ($enrolled_courses && !$record_course->role_id) {
            continue;
        } elseif(!$enrolled_courses && $record_course->role_id) {
            continue;
        }
        $teacher_name = get_user_data($record_course->teacher1_uid);
        $teacher_name2 = '';
        if (intval($record_course->teacher2_uid) > 0 && $record_course->teacher1_uid != $record_course->teacher2_uid)
            $teacher_name2 = get_user_data($record_course->teacher2_uid);
        $period = get_user_period($record_course->course_nid, $student_id);

        if (is_not_graded_course($record_course->course_nid) == true) {
            $course_grade = '';
            $percent = 'Not Graded';
        } else {
            $percent = _get_student_course_percentage($student_id, $record_course->nid, $school_term_tid);
            $course_grade = _get_course_grade($percent, $record_course->nid, 'L');
            $percent = format_course_grade_percent($percent, $record_course->nid, $course_grade);
        }

        if (isset($user->roles[NTLP_ROLEID_STUDENT]) || isset($user->roles[NTLP_ROLEID_PARENT])) {
            $course_title = l($record_course->title, 'ntlp/courses/grade/' . $record_course->course_nid);
        } else {
            $course_title = l($record_course->title, 'ntlp/courses/grade/' . $record_course->course_nid . '/' . $student_id);
        }

        $rows[] = array(
            array('data' =>  $course_title ),
            array('data' => l($teacher_name->last_name . ', ' . $teacher_name->first_name, 'ntlp/user/profile/' . $teacher_name->uid) .
                (!empty($teacher_name2) ? '<br/>' . l($teacher_name2->last_name . ', ' . $teacher_name2->first_name, 'ntlp/user/profile/' . $teacher_name2->uid) : '')),
            array('data' => $period->period),
            array('data' => _user_activity_submission_by_course_nid($school_term_tid, $record_course->nid, $student_id) ?
                        $course_grade . ' (' . $percent . ')' : ''),
        );
    }
    return $rows;
}

function _get_user_school_outcome_pg($student_id, $term_tid) {

    watchdog('school_outcome', $student_id . ' ' . $term_tid);
    $school_outcome = get_school_wide_learning_outcomes_contents($term_tid, $student_id);

    echo $school_outcome;
}

function get_school_wide_learning_outcomes_contents($term_tid, $user_uid, $by_school_year = false) {
    include "grades_template.inc";

    $term_rec = get_school_term_info($term_tid);
    $school_nid = $term_rec->school_nid;
    $school_year = $term_rec->school_year;
    $sw_outcomes = get_outcome_student_schoolwide($school_nid, $school_year, $term_tid, $user_uid);

    $outcome_lines = '';
    $outcome_box = '';
    while ($rec = db_fetch_object($sw_outcomes)) {

        if ($rec->grade != "*")
            $gv = FormatNumber($rec->grade, 0, ',');
        else
            $gv = '*';
        $outcome_line = $HTMLoutcome_line;
        $outcome_line = str_replace('@OUTCOME_NAME', $rec->name, $outcome_line);
        $outcome_line = str_replace('@OUTCOME_GRADE_PERCENT', $gv == '*' ? 0 : ($gv > 100 ? 100 : $gv), $outcome_line);
        $outcome_line = str_replace('@OUTCOME_GRADE', $gv == '*' ? $gv : $gv . '%', $outcome_line);
//        $outcome_line = str_replace('@OUTCOME_GRADE', FormatNumber($gv, 0, ',').'%', $outcome_line);
        $outcome_lines .= $outcome_line;
    }

    $outcome_box .= $HTMLcontainer;
    $outcome_box = str_replace('@OUTCOME_LINES', $outcome_lines, $outcome_box);
    $outcome_box = str_replace('@GRADE_LAST_UPDATED', $recent_act, $outcome_box);

    $output .= $outcome_box;
    $output = str_replace('@GRADES_VIEW', $view_activities, $output);
    $output = str_replace('@OUTCOME_BOX', $outcome_box, $output);
    $output = str_replace('@STUDENT_BOX', '', $output);
    $output = str_replace('@BACK_LINK', url("ntlp/courses/grade/$courseid"), $output);

    return $output;
}

function student_snapshot_is_current_term($term_tid) {
    $school_terms = get_ntlp_school_term(5);
}

function student_snapshot_get_current_term() {


    $school_terms = get_ntlp_school_term(5);

//    $today = date("n-j-Y");
    $current_term = "";
    $today = date("n-j");

    foreach ($school_terms as $term_Obj) {

        $start_date = $term_Obj->month . '-' . $term_Obj->day;
        $close_date = $term_Obj->to_month . '-' . $term_Obj->to_day;

        if ($today >= $start_date && $today <= $close_date) {

            $current_term = $term_Obj->school_term_tid;
            break;
        }
    }

    return taxonomy_get_term($current_term);
}

function get_student_advisory_teacher($student_id) {

    $result = db_query("SELECT instructor_uid FROM {ntlp_user} WHERE uid =%d", $student_id);

    if ($result) {
        return db_fetch_object($result);
    } else {
        return false;
    }
}

function get_semester_courses_list($user_uid, $term_tid) {
    global $user;

    $head = array(
        array('data' => t('Courses Name'), 'field' => 'title', 'sort' => 'asc'),
        array('data' => t('Courses Short Name'), 'field' => 'course_short_title', 'sort' => 'asc'),
        array('data' => t('Grade'), 'field' => 'course_short_title', 'sort' => 'asc'),
    );

    $result = db_query("SELECT r.course_nid, n.title, c.course_short_title, r.period as period_number
        FROM {node} n
        INNER JOIN {ntlp_course} c ON n.nid = c.nid
        INNER JOIN {ntlp_course_term} ct ON ct.course_nid = c.nid
        INNER JOIN {ntlp_course_user} r ON r.course_nid = c.nid
        WHERE c.is_project = 0 AND r.role_id = 5 AND r.user_uid = %d AND ct.school_term_tid = %d" . tablesort_sql($head), $user_uid, $term_tid);

    while ($record = db_fetch_object($result)) {
        $percent = _get_student_course_percentage($user_uid, $record->course_nid, $term_tid);
        $course_grade = _get_course_grade($percent, $record->course_nid, 'L');
        $rows[] = array(
            array('data' => l($record->title, 'ntlp/courses/grade/' . $record->course_nid)),
            array('data' => l($record->course_short_title, 'ntlp/courses/grade/' . $record->course_nid)),
            array('data' => (_user_activity_submission_by_course_nid($term_tid, $record->course_nid, $user_uid) ?
                        $course_grade . ' (' . format_course_grade_percent($percent, $record->course_nid, $course_grade) . ')' : '')),
        );
    }

    $attributes = array('class' => 'ntlp_table', 'width' => '815px', 'cellpadding' => '10');
//    return theme('table', $header, $rows);
    return theme_table($head, $rows, $attributes);
}

?>
