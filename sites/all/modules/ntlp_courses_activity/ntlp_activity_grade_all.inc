<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


require_once "data_access.inc";
require_once "grade_cache.inc";
module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_school', 'data_access');

//Clear All Grades
function ntlp_grade_all_clear_scores($activity_nid, $user_ids) {
    global $user;

    $activity_nid = check_plain(arg(4));
    $user_ids = json_decode(check_plain(arg(5)));
    if (count($user_ids) <= 0) {
        print 'No students found';
        return;
    }

    $users = implode(",", $user_ids);

    $act_rec = get_activity_records($activity_nid, true);

    for ($i=0; $i<sizeof($user_ids); $i++) 
    {
        clearcache_score_single_student_for_activity($user->uid, $activity_nid, $user_ids[$i]);
    }

    //These delete statements take too much time
//    db_query("DELETE FROM {ntlp_gb_activity_grade}
//        WHERE grade_id IN (SELECT id FROM {ntlp_gb_grade} WHERE activity_nid = %d AND user_uid IN (%s))", 
//        $activity_nid, $users);
//
//    //remove previous excused entries if exists of same outcome and submission
//    db_query("DELETE FROM {ntlp_gb_activity_excuse}
//        WHERE grade_id IN (SELECT id FROM {ntlp_gb_grade} WHERE activity_nid = %d AND user_uid IN (%s))", 
//        $activity_nid, $users);

    db_query("DELETE FROM {ntlp_gb_grade} WHERE activity_nid = %d AND user_uid IN (%s)", $activity_nid, $users);

    //No need to save, when the page is rendered again, it will save it
//    //Save data from cache as well
//    ntlp_courses_activity_grade_all_table_save($activity_nid, $user->uid);

    //TRIGGER_POINT: UPDATE STUDENT RECORD FOR ALL COURSE TERMS
    dw_set_allstudents_course_grade_toupdate($act_rec->course_nid, $act_rec->school_term_tid);

//     ntlpsubmissionactivity_submit("grade", $activity_nid, -1, $user->uid);

    $last_saved_date = db_fetch_object(get_activity_students_last_graded_date($activity_nid))->date_graded;
    $last_saved_date = (isset($last_saved_date) ? ('Last saved ' . get_tz(get_user_school($user->uid), DATE_FORMAT_LAST_SAVED, $last_saved_date)) : "");

    print($last_saved_date);
}

//Auto-Populate All Grades
function ntlp_grade_all_set_scores($activity_nid, $user_ids, $scores, $outcome_tid) {
    global $user;

    $activity_nid = check_plain(arg(4));
    $user_ids = json_decode(check_plain(arg(5)), true);
    $scores = json_decode(check_plain(arg(6)), true);
    $outcome_tid = check_plain(arg(7));

    if (count($user_ids) <= 0) {
        print 'No students found';
        return;
    }
    $userlist = implode(",", $user_ids);

    $act_rec = get_activity_records($activity_nid, true);

    for ($i=0; $i<sizeof($user_ids); $i++) 
    {
        clearcache_score_single_student_for_activity($user->uid, $activity_nid, $user_ids[$i]);
    }

    for ($i=0; $i<sizeof($user_ids); $i++) {
        if (intval($user_ids[$i]) > 0) {
            $userinput = $scores[$i];

            $grade_id = save_activity_user_grade($activity_nid, $user_ids[$i], null, $user->uid, null, null);
            
            db_query("DELETE FROM {ntlp_gb_activity_excuse} WHERE grade_id = %d and tid = %d", $grade_id, $outcome_tid);

            $result = db_query("SELECT * FROM {ntlp_gb_activity_grade}
                    WHERE grade_id = %d and tid = %d", $grade_id, $outcome_tid);

            if ($result->num_rows > 0) {

                if (isset($userinput) && strlen($userinput) > 0) {
                    db_query("UPDATE {ntlp_gb_activity_grade} SET points=%f
                    WHERE grade_id = %d and tid = %d", $userinput, $grade_id, $outcome_tid);
                } else {
                db_query("DELETE FROM {ntlp_gb_activity_grade} WHERE grade_id = %d and tid = %d", $grade_id, $outcome_tid);
                }
            } else {
                if (isset($userinput) && strlen($userinput) > 0) {
                db_query("INSERT INTO {ntlp_gb_activity_grade} (tid, grade_id, points)
                            VALUES (%d, %d, %f)", $outcome_tid, $grade_id, $userinput);
                }
            }

            if ($act_rec->is_published)
                ntlpsubmissionactivity_submit("grade", $activity_nid, $user_ids[$i], $user->uid);
        }
    }


    //No need to save, when the page is rendered again, it will save it
//    //Save data from cache as well
//    ntlp_courses_activity_grade_all_table_save($activity_nid, $user->uid);

    //TRIGGER_POINT: UPDATE STUDENT RECORD FOR COURSE TERM
    dw_set_allstudents_course_grade_toupdate($act_rec->course_nid, $act_rec->school_term_tid);

    $last_saved_date = db_fetch_object(get_activity_students_last_graded_date($activity_nid))->date_graded;
    $last_saved_date = (isset($last_saved_date) ? ('Last saved ' . get_tz(get_user_school($user->uid), DATE_FORMAT_LAST_SAVED, $last_saved_date)) : "");

    print($last_saved_date);
}

/* Mark 100% For All the Students  START */
function mark_all_100($activity_nid) {
    global $user;

    if ($act_rec = get_activity_records($activity_nid, true)) {

        $rs_students = get_activity_users_lite($activity_nid);
        $activityoutcome = get_activity_outcome($activity_nid);

        while ($selected_students = db_fetch_object($rs_students))
        {
            clearcache_score_single_student_for_activity($user->uid, $activity_nid, $selected_students->uid);

            $grade_id = save_activity_user_grade($activity_nid, $selected_students->uid, null, $user->uid, 1, null, true);
            auto_grade_maximum_to_user_submission($grade_id, $activityoutcome, false);

        	if ($act_rec->is_published) {
                ntlpsubmissionactivity_submit("grade", $activity_nid, $selected_students->uid, $user->uid);
            }
        }
        //No need to save, when the page is rendered again, it will save it
//        //Save data from cache as well
//        ntlp_courses_activity_grade_all_table_save($activity_nid, $user->uid);
        
        //TRIGGER_POINT: UPDATE ALL STUDENT RECORD FOR CURRENT COURSE TERM
        dw_set_allstudents_course_grade_toupdate($act_rec->course_nid, $act_rec->school_term_tid);

    } else {
        drupal_set_message('Activity not found');
    }
}
/* Mark 100% For All the Students  END */


//Called from Grade-All score textbox when user tabs-out after changing the score
function ntlp_grade_all_ajax_frm($activity_nid, $activity_user, $outcome_tid) {
    global $user;
//    activity_nid+"/"+activity_user+"/"+submission_nid+"/"+outcome_tid+"/"+user_input;

    $activity_nid = check_plain(arg(4));
    $activity_user = check_plain(arg(5));
    $outcome_tid = check_plain(arg(6));
    $userinput = check_plain(arg(7));

    save_score_in_cache($user->uid, $activity_nid, $activity_user, $outcome_tid, $userinput, 'score');
//    print "Saved in Cache:".$activity_nid."/".$activity_user."/".$outcome_tid."/".$userinput;
    print "OK";
}

function ntlp_grade_all_ajax_chcbox_frm($activity_nid, $activity_user, $checked, $late_or_incomplete) {
    global $user;

    save_score_in_cache($user->uid, $activity_nid, $activity_user, 0, $checked, $late_or_incomplete);
//    print "Saved in Cache:".$activity_nid."/".$activity_user."/".$checked."/".$late_or_incomplete;
    print "OK";
}


function ntlp_grade_all_frm() {
    return drupal_get_form('ntlp_courses_activity_grade_all_form');
}

define('SUBMISSION_STATUS_SUBMITTED', 2);
define('SUBMISSION_STATUS_RESUBMITTED', 3);
define('SUBMISSION_STATUS_NOTYETGRADED', 4);
define('SUBMISSION_STATUS_LATESUBMITTED', 5);
define('SUBMISSION_STATUS_INCOMPLETE', 6);

function get_submission_status_filter($activity_obj) {

    $options = array(
        'Status' => array(
            'All' => array(
                'type' => 'checkbox',
                'group' => 'submission_status',
                'value' => 1,
                'name' => 'all',
                'default' => true,
            )
        )
    );

    if ($activity_obj->is_graded) {
        $options['Status'] = $options['Status'] + array('Not Yet Graded' => array(
                'type' => 'checkbox',
                'group' => 'submission_status',
                'value' => 4,
                'name' => 'Not Yet Graded',
                'default' => false,
                ));
    }
    if ($activity_obj->type == "lo_journal" || $activity_obj->submission_type == 'D') {
        $options['Status'] = $options['Status'] + array('Submitted' => array(
                'type' => 'checkbox',
                'group' => 'submission_status',
                'value' => 2,
                'name' => 'Submitted',
                'default' => false,
            ),
            'Re-submitted' => array(
                'type' => 'checkbox',
                'group' => 'submission_status',
                'value' => 3,
                'name' => 'Re-submitted',
                'default' => false,
                ));
    }

    $options['Status'] = $options['Status'] + array('Submitted Late' => array(
            'type' => 'checkbox',
            'group' => 'submission_status',
            'value' => 5,
            'name' => 'Submitted Late &nbsp',
            'default' => false,
            'class' => 'late',
        ),
        'Incomplete' => array(
            'type' => 'checkbox',
            'group' => 'submission_status',
            'value' => 6,
            'name' => 'Incomplete &nbsp',
            'default' => false,
            'class' => 'incomplete',
            ));

    return $options;
}

function ntlp_courses_activity_grade_all_form($form_state = null, $course_id = 0, $activity_id = 0) {
    global $base_path;
    include("activity_view_template.inc");

    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/ntlp_courses_activity.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/rubric_activity.js');
    drupal_add_js('misc/tableheader.js');

    $form['grade_all'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => $HTMLform_grading0ption_conf_dlg,
    );

    $last_saved_date = db_fetch_object(get_activity_students_last_graded_date($activity_id))->date_graded;
    $last_saved_date = (isset($last_saved_date) ? ('Last saved ' . get_tz_course($course_id, DATE_FORMAT_LAST_SAVED, $last_saved_date)) : "");

    $suffix = '</td><td align="left"><div name="last_saved_date" style="padding: 5px; ' . (($last_saved_date != "") ? ' display:block; ' : ' display:none; ') . ' background-color: #ddd; border: 1px solid #ddd; float:left; font-size: 12px; color: rgb(0, 0, 0);" >@SAVE_DATE</div></td>';
    $suffix = str_replace('@SAVE_DATE', $last_saved_date, $suffix);

    $activity_obj = get_activity_records($activity_id, true);

    $user_role = check_user_permission($course_id);
    
    if ($user_role != NTLP_ROLEID_SITEGUEST) {

        $form['grade_all']['submitbtn'] = array(
            '#type' => 'item',
            '#value' => '<img class="SaveBtnDisabled" src="' . $base_path . 'themes/Boldr/Images/SaveDisabled.png" style="display:none;" /><a class ="SaveBtn" onclick="on_save_grade_all('.$activity_id.')" onmousedown="this.className=\'SaveBtnClick\'" onmouseup="this.className=\'SaveBtn\'">Save</a>',
            '#prefix' => '<table width="100%" cellpadding="5" cellspacing="0"><tr><td style="width:51px">',
            '#suffix' => $suffix,
        );

        if ($activity_obj->is_graded && ($activity_obj->type != "lo_rubric")) {

            $form['grade_all']['markall'] = array(
                '#type' => 'item',
                '#value' => '<a class ="white_big_btn" onclick="onclick_grade_all(' . $activity_id . '); return false;" >Mark All 100% </a>',
                '#prefix' => '<td style="width:100px;">',
                '#suffix' => '</td></tr></table>',
            );
        } else if ($activity_obj->type == "lo_rubric") {
            $form['grade_all']['clear_all'] = array(
                '#type' => 'item',
                '#value' => '<a class ="white_big_btn" onclick="clear_all_grades('.$activity_id.');" >Clear All Grades </a>',
                '#prefix' => '<td style="width:100px;">',
                '#suffix' => '</td>',
            );
            $form['grade_all']['auto_populate'] = array(
                '#type' => 'item',
                '#value' => '<a class ="white_big_btn" onclick="auto_populate_all_grades('.$activity_id.');" >Auto-Populate All Grades </a>',
                '#prefix' => '<td style="width:100px;">',
                '#suffix' => '</td></tr></table>',
            );
        } else {
            $form['grade_all']['table_html_fix'] = array(
                '#type' => 'hidden',
                '#suffix' => '</tr></table>',
            );
        }
    }

    // check late sumbmission

    if ($activity_obj->grading_method == 'C') {
        $activity_grading_method = 1; // setting a value 1 if this activity is auto grading.. true
    } else {
        $activity_grading_method = 0;
    }


    /* setting a flag for if due date passed and auto grading activity */
    $form['grade_all']['activity_grading_method'] = array(
        '#type' => 'hidden',
        '#value' => $activity_grading_method,
        '#id' => 'activity_grading_method',
    );

    $filter = get_submission_status_filter($activity_obj);
//        if (!isset($_GET['sort']) && !isset($_GET['filter']) && $_GET['filter'] != 'none') {
    if (true) {

        watchdog("sort order", $_GET['order'] . " " . $_GET['sort']);

        $sort_order = strtolower(isset($_GET['order']) ? $_GET['order'] : 'name');
        $sort_dir = isset($_GET['sort']) ? $_GET['sort'] : 'asc';

        $opts = render_custom_filter(
                        $filter,
                        array('start' => $HTML_student_activity_filter_cellStart, 'end' => $HTML_student_activity_filter_cellEnd),
                        array('template' => $HTML_student_activity_filter_heading, 'placeholder' => '@HEADING'),
                        $HTML_student_activity_filter_row_end,
                        url('ntlp/courses/activity/gradeall_table/' . $course_id . '/' . $activity_id, array("query" => "sort=$sort_dir" . "&order=$sort_order")),
                        'DataEntryTableCSS'
        );

        $foutput = str_replace('@OPTIONS', $opts, $HTML_student_activity_filter);
    }

    $form['grade_all']['filter'] = array(
        '#type' => 'item',
        '#value' => $foutput,
    );

    $form['grade_all']['main_table'] = array(
        '#type' => 'item',
        '#value' => ntlp_courses_activity_grade_all_table($activity_obj->school_term_tid, $course_id, $activity_id, array(), $user_role),
        '#prefix' => '<div id="DataEntryTableCSS" style="width:821px;">',
        '#suffix' => '</div>',
    );


    return $form;
}

//Renders the Students Grade-All table for an Activity
function ntlp_courses_activity_grade_all_table($school_term_tid, $course_id = 0, $activity_id = 0, $filter=array(), $user_role) {
    global $base_path;
    global $user;

    //Save the scores from cache to database
    if ($activity_id > 0) {
        ntlp_courses_activity_grade_all_table_save($activity_id, $user->uid);
    }
    
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/ntlp_courses_activity.js');
    drupal_add_js('misc/tableheader.js');

    drupal_add_js('
                function set_header_title(){
                $(".ntlp_table th").each(function(k, v) {

                $(".ntlp_table th a").each(function(key, val) {
                        if( k == key){

                         $(this).attr("title", v.title);
                        }
                 });
             });
             }
                ', 'inline');

    $activityoutcome = get_activity_outcome($activity_id);
    $size_of_outcome = sizeof($activityoutcome);

    $col_width_offset = 0;

    if (strpos($_SERVER['HTTP_USER_AGENT'], 'Safari')) {
        // Chrome user agent string contains both 'Chrome' and 'Safari'
        $col_width_offset = 20;
    }

    $rubric_activity = get_rubric_activity($activity_id);
    $school_nid = get_course_school_nid($course_id);

    if (true || $size_of_outcome > 0) {

        $head = array(
            array('data' => t('Per.'), 'field' => 'period', 'sort' => 'asc', 'width' => 20 + $col_width_offset),
            array('data' => t('Student Work'), 'field' => 'name', 'width' => 200 + $col_width_offset),
            array('data' => t('Late'), 'field' => 'is_late', 'width' => 30 + $col_width_offset),
            array('data' => t('Inc'), 'field' => 'is_complete', 'width' => 30 + $col_width_offset),
        );

        $ocno = 1;
        $outcomes = array();
        $otcols = array();

        if ($size_of_outcome > 0) {
            foreach ($activityoutcome as $outcome) {

                $outcome_name = '';
                $str_arr = explode(" ", $outcome->name);

                $str_length = sizeof($str_arr);

                for ($j = 0; $j < $str_length; $j++) {

                    if (strlen($str_arr[$j]) >= 8) {
                        $value = substr($str_arr[$j], 0, 8);
                        $strval = _get_consonant($value);
                        $outcome_name .= $strval . '<br>';
                    } else {
                        $outcome_name .= $str_arr[$j] . '<br>';
                    }
                }

                $otcols[] = array(
                    'data' => $outcome_name . '  <span style="font-weight: normal !important;font-size:11px;">(' . $outcome->max_points . ' pts)</span>',
                    'field' => 'outcome_' . $ocno,
                    'title' => $outcome->name,
                    'width' => 56 + $col_width_offset,
                    'style' => 'text-align: center; ',
                    'onmouseover' => 'set_header_title()');
                $outcomes[$outcome->tid] = $otcols[sizeof($otcols) - 1]['data'];
                $ocno++;
            }

            $head = array_merge($head, $otcols);
        }

        if ($rubric_activity != NULL) {

            $feedback_cols = array(
                array(
                    'data' => 'Ave Pts (#) <br> Received',
                    'style' => 'text-align: center; ',
                    'width' => 80 + $col_width_offset
                ),
                array(
                    'data' => 'Ave Pts (#) <br> Submitted',
                    'style' => 'text-align: center; ',
                    'width' => 80 + $col_width_offset
                ),
            );

            $head = array_merge($head, $feedback_cols);
        }

        if ($size_of_outcome < 7) {
            $head = array_merge($head, array('data' => ''));
        }

        //Get Sort order
        $sort_order = strtolower(isset($_REQUEST['order']) ? $_REQUEST['order'] : 'per.');
        $sort_dir = isset($_REQUEST['sort']) ? $_REQUEST['sort'] : 'asc';
        $sort_dir = $sort_dir == 'none' ? 'asc' : $sort_dir;
        $sort_field = $sort_order;
        reset($outcomes);
        $ocno = 1;
        if ($sort_order != "per." || $sort_order != "student work" || $sort_order != "late" || $sort_order != "inc") {
            if (sizeof($outcomes) > 0) {
                while (list($key, $val) = each($outcomes)) {
                    if ($sort_order == strtolower($val)) {
                        $sort_field = "(outcome_graded_$key + 0)";
                    }
                    $ocno++;
                }
            }
        }
        if ($sort_order == "per.")
            $sort_field = 'period';
        if ($sort_order == "student work")
            $sort_field = 'name';
        if ($sort_order == "inc")
            $sort_field = 'is_complete';
        if ($sort_order == "late")
            $sort_field = 'is_late';

        if ($sort_order == "per.")
            $order_by = $sort_field . ' ' . $sort_dir . ', name asc ';
        else
            $order_by = $sort_field . ' ' . $sort_dir;



        $selected_students = _get_activity_selected_students($school_term_tid, $activity_id, $order_by, $outcomes, $filter);

        $n = 0;
        $rows = 1;
        $total_rows = sizeof($selected_students);

        $edit_mode = ($user_role != NTLP_ROLEID_SITEGUEST);

        if (!empty($selected_students) && $total_rows > 0) {

            $load_script = 'var user_ids = new Array();';
            $load_script .= 'var feedback_points = new Array();';

            $total_points = get_rubric_activity_total_points($activity_id)->total_points;
            $load_script .= 'var max_rubric_score = ' . $total_points . ';';

            foreach ($selected_students as $obj) {

                $load_script .= 'user_ids.push("' . $obj->selected_user . '");';

                $gradedoutcome = array();
                if (isset($obj->grade_id) && $obj->grade_id != null) {
                    $gradedoutcome = load_graded_outcome_points($obj->grade_id);
                    }

                $period = empty($obj->period) ? '*' : $obj->period;
                $version_number = intval($obj->version_num);
                $submission_date = (isset($obj->date_submitted) && isset($obj->version_num) && $version_number > 0 ? get_tz($school_nid, DATE_FORMAT_SUBMISSION, $obj->date_submitted) : '' );
                $version = ($version_number > 1) ? ( ' (v' . $version_number . ')' ) : '';
                $notes = (isset($obj->note_to_teacher) && !empty($obj->note_to_teacher) ? ('<img title="' . strip_tags($obj->note_to_teacher) . '" align="right" src="' . $base_path . 'themes/Boldr/Images/common/comments.png" >') : '' );

                //Check if the score is in cache
                $late_cached = false;
                $inc_cached = false;
                unset($cache_score);
                $score_in_cache = "";
                $type_in_cache = "";
                $key = "GRADEALL_SCORE:$user->uid:$activity_id:$obj->selected_user";
                $cache_score_obj = cache_get($key, 'ntlp_cache');
                if ($cache_score_obj) {
                    $cache_score = $cache_score_obj->data;
                    if (is_object($cache_score)) {
                        if (!$cache_score->saved) {
//                            print $key; print_r($cache_score); print "<br>";
                            if (isset($cache_score->is_late)) {
                                $is_late = intval($cache_score->is_late);
                                $late_cached = true;
                            }
                            $score_in_cache = $cache_score->value;
                            if ($cache_score->type == 'incomplete') {
                                $score_in_cache = $cache_score->value;
                                $is_incomplete = $score_in_cache['incomplete'];
                                $inc_cached = true;
                            }
                        }
                    }
                }

                $late = "";
                $inc = "";
//                print_r($obj);
                if ($late_cached) {
                    $obj->is_late = $is_late;
                    $late = "[L=>{$is_late}]";
                }
                if ($inc_cached) {
                    $obj->is_complete = ($is_incomplete ? 0 : 1);
                    $obj->date_submitted = strftime(DATE_FORMAT, time());
                    $obj->is_graded = 1;
                    $inc = "[I=>{$is_incomplete}]";
                }

                $late .= ($edit_mode)
                    ? '<input onclick="submission_late_tid(' . $obj->selected_user . ', ' . $activity_id . ')" type="checkbox" name=outcome_late[' . $obj->selected_user . ']' . ((isset($obj->is_late) && $obj->is_late == 1) ? ' checked' : "") . ' >'
                    : ((isset($obj->is_late) && $obj->is_late == 1) ? '<div align=center>x</div>' : "");

                $inc .= ($edit_mode)
                    ? '<input onclick="submission_inc_tid(' . $obj->selected_user . ', ' . $activity_id . ')" type="checkbox" name=outcome[' . $obj->selected_user . '] ' . (($obj->is_complete == 0 && $obj->is_graded == 1) ? ' checked' : "") . ' >'
                    : (($obj->is_complete == 0 && $obj->is_graded == 1) ? '<div align=center>x</div>' : "");

                //Set appropriate CSS class for the data row
                $activity_class = "";
                if ($obj->is_complete == 0 && $obj->is_graded == 1)
                    $activity_class = "incomplete";
                elseif ($obj->is_late == 1)
                    $activity_class = "late";

                //Prepare data row
                $datarow = array(
                    array('data' => $period,
                        'class' => $activity_class,
                        'name' => "user[$obj->selected_user]"),
                    array('data' => l($obj->name, 'ntlp/courses/activity/grade/' . $course_id . '/' . $activity_id . '/' . $obj->selected_user)
                        . '<br>' . $submission_date . $version . $notes,
                        'class' => $activity_class,
                        'name' => "user[$obj->selected_user]"),
                    array('data' => $late,
                        'class' => $activity_class,
                        'name' => "user[$obj->selected_user]"),
                    array('data' => $inc,
                        'class' => $activity_class,
                        'name' => "user[$obj->selected_user]"),
                );

                $otcols = array();
                for ($cols = 0; $cols < $size_of_outcome; $cols++) {
                    //Check if the score is in cache
                    $cached = false;
                    $score_in_cache = "";
                    $type_in_cache = "";
                    if (is_object($cache_score)) {
                        if (!$cache_score->saved) {
                            if ($cache_score->type == 'score') {
//                                echo '<br>Outcome Id:'.$activityoutcome[$cols]->tid;
//                                print_r($cache_score);
                                if (isset($cache_score->value[$activityoutcome[$cols]->tid])) {
                                    $score_in_cache = $cache_score->value[$activityoutcome[$cols]->tid];
//                                    echo '<br>Score:'.$score_in_cache.'/'.rtrim(trim($score_in_cache, '0'), '.');
                                    if (!empty($score_in_cache)) {
                                        $cached = true;
                                    }
                                }
                            }
                        }
                    }

                    if ($rubric_activity != NULL) {
                        if ($rubric_activity->outcome_tid == $activityoutcome[$cols]->tid) {
                            $load_script .= 'var primary_outcome_id = ' . $rubric_activity->outcome_tid . ';';
                            $load_script .= 'var outcome_max_points = ' . $activityoutcome[$cols]->max_points . ';';
                        }
                    }

                    if ($gradedoutcome[$activityoutcome[$cols]->tid] <> "EX") {
                        $gradedoutcome[$activityoutcome[$cols]->tid] = clean_score($gradedoutcome[$activityoutcome[$cols]->tid]);
                    }

                    $outcome_txtbox = "";
                    if ($cached) {
                        if ($score_in_cache <> "EX") {
                            $score_in_cache = clean_score($score_in_cache);
                        }
                        $outcome_txtbox = "[S=>$score_in_cache]";
                        if (sizeof($gradedoutcome) > 0) {
                            $gradedoutcome[$activityoutcome[$cols]->tid] = $score_in_cache;
                        }
                    }
                    if ($edit_mode) {
                        $outcome_txtbox .= "<input style='width:30px;' maxlength='6' onfocus=save_previous_score(". $obj->selected_user . "," . $activityoutcome[$cols]->tid . "); onblur='" . 'submission_grade_tid(' . $activity_id . ',' . $obj->selected_user . ',' . $activityoutcome[$cols]->max_points . ',' . $activityoutcome[$cols]->tid . ',this.value)' . "' tabindex=$rows id=$obj->selected_user-" . $activityoutcome[$cols]->tid . " type='text' size='3' name=disabled_outcome[$obj->selected_user] ";
                        $outcome_txtbox .= ($obj->is_complete == 0 && $obj->is_graded == 1) ? ' disabled' : '';
                        $outcome_txtbox .= " value='" . ((sizeof($gradedoutcome) > 0) ? $gradedoutcome[$activityoutcome[$cols]->tid] : "") . "'";
                        $outcome_txtbox .= " >";
                        $outcome_txtbox .= '<input type="hidden" value="' . $activityoutcome[$cols]->max_points . '" id="' . $obj->selected_user . '-' . $activityoutcome[$cols]->tid . '" name="auto_graded_outcome[' . $obj->selected_user . ']">';
                    }else
                        $outcome_txtbox = ((sizeof($gradedoutcome) > 0) ? $gradedoutcome[$activityoutcome[$cols]->tid] : "");

                    $otcols[] = array(
                        'data' => $outcome_txtbox,
                        'style' => 'text-align: center; ',
                        'class' => $activity_class,
                        'name' => "user[$obj->selected_user]",
                    );
                }
                $rows++;

                $datarow = array_merge($datarow, $otcols);

                if ($rubric_activity != NULL) {
                    $feedback_submitd = get_user_submitted_feedback_avg_score($obj->selected_user, $activity_id);
                    $feedback_rcvd = get_rubric_submission_avg_score($obj->selected_user, $activity_id);

                    $feedback_datacols = array(
                        array('data' => (isset($feedback_rcvd->avg_score) ? round($feedback_rcvd->avg_score) : 0)
                            . " ($feedback_rcvd->score_count)",
                            'class' => $activity_class,
                            'style' => 'text-align: center; ',
                            'name' => "user[$obj->selected_user]"),
                        array('data' => (isset($feedback_submitd->avg_score) ? round($feedback_submitd->avg_score) : 0 )
                            . " ($feedback_submitd->score_count)",
                            'class' => $activity_class,
                            'style' => 'text-align: center; ',
                            'name' => "user[$obj->selected_user]"),
                    );

                    $load_script .= 'feedback_points.push(' . (isset($feedback_rcvd->avg_score) ? round($feedback_rcvd->avg_score) : 0) . ' );';

                    $datarow = array_merge($datarow, $feedback_datacols);
                }

                if ($size_of_outcome < 7) {
                    $datarow = array_merge($datarow, array(array('data' => '')));
                }
                $data_rows[] = $datarow;
            }
        } else {
            $data_rows[] = array(array('data' => '<strong>No students found.</strong>', 'colspan' => sizeof($outcomes) + 3, 'align' => 'center'));
        }

        $attributes = array('class' => 'ntlp_table', 'cellpadding' => '8', 'cellspacing' => '0', 'style' => ' margin-bottom:50px !important;', 
            'width' => $size_of_outcome < 7 ? "100%" : (280 + (22*4) +($size_of_outcome*(56 + 20 + 2))) + ($rubric_activity != NULL ? (80+20+2) * 2 : 0));

        if ($rubric_activity != NULL) {
            drupal_add_js($load_script, 'inline');
        }
        return '<div style="position:relative;">'.theme_table($head, $data_rows, $attributes).'</div>';
    } else {
        drupal_set_message("Outcomes are not defined for this activity");
    }

    return "";
}

//Saves the Students Grade-All scores for an Activity from cache to database
function ntlp_courses_activity_grade_all_table_save($activity_id = 0, $teacher_uid = 0) {
    global $user;

    if (!isset($teacher_uid) || empty($teacher_uid)) {
        $teacher_uid = $user->uid;
    }

    $act_rec = get_activity_records($activity_id, true);        //Get activity record
    $outcome_array = get_activity_outcome($activity_id);        //Get activity outcomes
    $rs_students = get_activity_users_lite($activity_id);

    $affected_users = array();
    while ($selected_students = db_fetch_object($rs_students)) 
    {
        if (save_grades_from_cache($teacher_uid, $activity_id, $selected_students->uid, $outcome_array, $act_rec->grading_method)) {
            $affected_users[] = $selected_students->uid;
        }
    }

    if (count($affected_users) > 0) {
        foreach($affected_users as $student_uid) {
            if ($act_rec->is_published) {
                //TRIGGER_POINT: UPDATE STUDENT RECORD FOR CURRENT COURSE TERMS
                dw_set_student_course_grade_toupdate($student_uid, $act_rec->course_nid, $act_rec->school_term_tid);
            
                ntlpsubmissionactivity_submit("grade", $activity_id, $student_uid, $teacher_uid);
            }
        }
    }
//    $last_saved_date = db_fetch_object(get_activity_students_last_graded_date($activity_id))->date_graded;
//    $last_saved_date = (isset($last_saved_date) ? ('Last saved ' . get_tz(get_user_school($teacher_uid), DATE_FORMAT_LAST_SAVED, $last_saved_date)) : "");
//
//    print($last_saved_date);
}

//Saves the Students Grade-All scores for ALL Activities of a course-term from cache to database
function ntlp_student_courses_grade_all_table_save($student_uid, $course_nid, $term_tid, $teacher_uid = 0) {
    global $user;

    if (!isset($teacher_uid) || empty($teacher_uid)) {
        $teacher_uid = $user->uid;
    }

    $result = get_course_outcomes($course_nid);        //Get course outcomes
    $outcome_array = array();
    while ($obj = db_fetch_object($result)) {
        $outcome_array[] = $obj;
    }

    $ars = get_course_user_activities($course_nid, $term_tid, $student_uid);

    while ($arec = db_fetch_object($ars))  
    {
        if (save_grades_from_cache($teacher_uid, $arec->nid, $student_uid, $outcome_array, $arec->grading_method))
        {
            if ($arec->is_published) {
                $affected_activities[] = $arec->nid;
            }
        }
    }

    if (count($affected_activities) > 0) {
        //TRIGGER_POINT: UPDATE STUDENT RECORD FOR CURRENT COURSE TERMS
        dw_set_student_course_grade_toupdate($student_uid, $course_nid, $term_tid);
        
        foreach($affected_activities as $activity_id) {
            ntlpsubmissionactivity_submit("grade", $activity_id, $student_uid, $teacher_uid);
        }
    }
    
//    $last_saved_date = db_fetch_object(get_activity_students_last_graded_date($activity_id))->date_graded;
//    $last_saved_date = (isset($last_saved_date) ? ('Last saved ' . get_tz(get_user_school($teacher_uid), DATE_FORMAT_LAST_SAVED, $last_saved_date)) : "");
//
//    print($last_saved_date);
}

function course_activity_gradeall_submit($form, $form_state) {
    drupal_goto('ntlp/courses/activity/view/' . arg(4) . '/' . arg(5), 'tab=work');
//    drupal_goto('ntlp/goback');
}

function _get_activity_selected_students($school_term_tid, $activity_nid, $order_by=NULL, $outcome_rows=null, $filters=array()) {

    $query = "SELECT ";

    if (isset($outcome_rows)) {
        while (list($key, $val) = each($outcome_rows)) {
            $query .= "(CASE WHEN (SELECT COUNT(*) FROM dpl_ntlp_gb_grade g, dpl_ntlp_gb_activity_outcome x, dpl_ntlp_gb_activity_excuse ex WHERE ex.grade_id = g.id AND ex.tid = @OUTCOME_ID AND g.activity_nid = x.activity_nid AND g.activity_nid = nca.nid AND g.user_uid = nau.user_uid) THEN 'EX' ELSE (SELECT TRIM(TRAILING '.' FROM TRIM(TRAILING '0' FROM p.points)) FROM dpl_ntlp_gb_activity_grade p, dpl_ntlp_gb_grade g, dpl_ntlp_gb_activity_outcome x WHERE p.grade_id = g.id AND p.tid = x.tid AND g.activity_nid = x.activity_nid AND g.activity_nid = nca.nid AND g.user_uid = nau.user_uid AND p.tid = @OUTCOME_ID AND x.tid = @OUTCOME_ID) END) AS outcome_graded_@OUTCOME_ID, ";

            $query = str_replace('@OUTCOME_ID', $key, $query);
        }
    }

    $filter_condition = '';
    if ($filters && !empty($filters)) {

        //using having because of condition on aliases -- AA
        $filter_condition .= " GROUP BY u.uid HAVING ";
        $first_filter = true;

        foreach ($filters as $status_filter) {

            $filter_condition .= ( !$first_filter) ? " OR " : "";
            $first_filter = false;

            switch ($status_filter) {

                //If “Submitted” is selected, the Student list will include Students who have submitted
                //work for the Activity only once.
                case (SUBMISSION_STATUS_SUBMITTED) :
                    $filter_condition .= '(nas.version_num > "0" AND nas.version_num < "2")';
                    break;

                //If “Re-submitted” is selected, the Student list will include Students who have
                //submitted work for the Activity more than once.
                case (SUBMISSION_STATUS_RESUBMITTED) :
                    $filter_condition .= 'nas.version_num >= "2"';
                    break;

                //If “Not Yet Graded” is selected, the Student list will include Students who have not been
                //graded for at least one of the Outcomes yet.
                case (SUBMISSION_STATUS_NOTYETGRADED) :
                    $filter_condition .= '(ng.id IS NULL ';

                    reset($outcome_rows);
                    while (list($key, $val) = each($outcome_rows)) {
                        $filter_condition .= " OR outcome_graded_$key IS NULL ";
                    }
                    $filter_condition .= ")";

                    break;

                //If “Submitted Late” is selected, the Student list will include Students
                //who have submitted work for the Activity (one or more times) and the work
                //is marked as Late. Those rows are colored yellow to match the Filter selection.
                case (SUBMISSION_STATUS_LATESUBMITTED) :
                    $filter_condition .= '(ng.is_late = 1)';
                    break;

                //If “Incomplete” is selected, the Student list will include Students whose
                //work is marked as Incomplete (whether or not it was submitted). Those rows are
                //colored red to match the Filter selection.
                case (SUBMISSION_STATUS_INCOMPLETE) :
                    $filter_condition .= '(ng.is_complete = 0)';
                    break;
            }
        }
    }

    $query .= " ncu.period, nau.*, CONCAT(u.last_name, ', ', u.first_name) AS name, u.uid AS selected_user,
            nas.nid, nas.date_opened, nas.date_submitted, nas.is_final, nas.note_to_teacher, nas.version_num, 
            ng.id grade_id, ng.is_complete, ng.is_late, (CASE WHEN ng.id IS NULL THEN 0 ELSE 1 END) is_graded, ng.date_graded, ng.teachers_notes
        FROM {ntlp_activity_user} nau
          INNER JOIN {ntlp_user} u ON nau.user_uid = u.uid
          INNER JOIN {ntlp_course_activity} nca ON nca.nid = nau.activity_nid
          INNER JOIN {ntlp_course_user} ncu ON ncu.course_nid = nca.course_nid AND ncu.user_uid = u.uid  AND ncu.role_id IS NOT NULL
          LEFT JOIN {ntlp_activity_submission} nas ON nau.activity_nid = nas.activity_nid AND nas.user_uid = u.uid AND nas.is_final = 1
          LEFT JOIN {ntlp_gb_grade} ng ON nau.activity_nid = ng.activity_nid AND nau.user_uid = ng.user_uid
        WHERE nau.activity_nid = %d " .
            $filter_condition . " " .
            (isset($order_by) ? "ORDER BY $order_by" : "ORDER BY ncu.period, u.last_name, u.first_name");

//    watchdog("get_activity_students", $query);
    $result = db_query($query, $activity_nid);

    if ($result->num_rows > 0) {
        while ($note = db_fetch_object($result)) {
            $activity_submitted[] = $note;
        }

        return $activity_submitted;
    } else {
        return false;
    }
}

function ntlp_activity_stu_submissions_filter($courseid, $activityid) {

    $submission_status = array();

    if (isset($_REQUEST['submission_status']) and !isset($_REQUEST['submission_status']['all'])) {
        foreach ($_REQUEST['submission_status'] as $status) {

            $submission_status[] = $status;
        }
    }

//    $_GET['order'] = isset($_POST['order']) ? $_POST['order'] : $_GET['order'];
//    $_GET['sort'] = isset($_POST['sort']) ? $_POST['sort'] : $_GET['sort'];
//
//    watchdog("post sort order", $_POST['sort'] . " " . $_POST['order']);
//
//watchdog("filter array", serialize($submission_status));
    $act_rec = get_activity_records($activityid, true);        //Get activity record
    $table = ntlp_courses_activity_grade_all_table($act_rec->school_term_tid, $courseid, $activityid, $submission_status, check_user_permission($courseid));
    print $table;
}
