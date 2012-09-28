<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_grade', 'data_access');
module_load_include('inc', 'ntlp_school', 'cron_grades');
module_load_include('inc', 'ntlp_courses_activity', 'activity_graded_mgmt');
module_load_include('inc', 'ntlp_reports');

function grades_gsc_block_contents() {
    include "grades.template.inc";
    global $user, $base_url;

    drupal_add_js(drupal_get_path('module', 'ntlp_blocks') . '/grades.contents.js');
    $user_id = $user->uid;

    if ($courseid == 0) {
        //Getting Query String From URL
        $courseid = 0;
        $url = $_GET['q'];
        $args = explode('/', $url);
        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i]))
                $courseid = $args[$i];
        }
    }

    set_item_url_handler('Grade');

    $view_activities = t('You do not have the required permissions.');
    $student_box = '';
    $templinks_box = '';
    $outcome_box = '';

    if (isset($user->roles[NTLP_ROLEID_PARENT])) {  // if parent come here show their student record
        $user_role = NTLP_ROLEID_PARENT;
        $user_id = $_SESSION['PARENT_STUDENT_UID'];

        if (!check_user_exist_in_course($user_id, $courseid, true)) {
            drupal_goto("ntlp/courses/my");
        }
    } else {
        $user_role = check_user_permission($courseid);
    }

    $course_Obj = get_course_detail($courseid);
    $school_year = date('Y');       // Cannot use school year from School selector, as we are using current year in the get-term function
    $school_nid = $course_Obj->school_nid;
    if ($r = get_current_school_term($school_nid)) {
        $rec_sch = db_fetch_object($r);
        $term_tid = $rec_sch->school_term_tid;
        $school_year = $rec_sch->school_year;
    }

    $term_tid = get_current_selected_course_term($courseid);

    if (isset($user_role)) {
        if ($user_role == NTLP_ROLEID_NETWORKMANAGER || $user_role == NTLP_ROLEID_NETWORKSTAFF
                || $user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_SITEGUEST) {

            //Show Outcome summary for all students

            $outcome_box = _get_course_outcome_grade($course_Obj);

            $tab_student = (isset($_GET['tab'])) ? ($_GET['tab'] == 'students') : true;

            if ($tab_student) {

                //Run cron, but first clear the dw table
                dw_setall_student_course_grade_toupdate($courseid, $term_tid, $school_nid);
                cron_course_student_grades_dw($courseid, $term_tid);

                $view_activities = render_course_student_grades($courseid, $term_tid, $filters = array());
            }
            else
                $view_activities = get_gradebook_activities_view($courseid, $term_tid);
        } else {
            $gradebook_filter = '';
            $outcome_box = showStudentGrades($courseid, $user_id, 'G', $view_activities, $outcome_box, $student_box, $templinks_box);
        }
    }

    $output = $outcome_box;
    $output = str_replace('<!-- @FILTER_PLACEHOLDER -->', $gradebook_filter, $output);
    $output = str_replace('@GRADES_VIEW', $view_activities, $output);
    $output = str_replace('@OUTCOME_BOX', $outcome_box, $output);
    $output = str_replace('@STUDENT_BOX', '', $output);
    $output = str_replace('@BACK_LINK', url("ntlp/courses/grade/$courseid"), $output);
    $output = str_replace('@TAB_STUDENTS', '<a href="' . url('ntlp/courses/grade/') . $courseid . '&tab=students" id="grades_tab_students" ' . (($tab_student) ? 'class="GradesTabActive"' : '') . '>Students</a>', $output);
    $output = str_replace('@TAB_SEPARATOR', '|', $output);
    $output = str_replace('@TAB_ACTIVITIES', '<a href="' . url('ntlp/courses/grade/') . $courseid . '&tab=activities" id="grades_tab_activities" ' . (($tab_student) ? '' : 'class="GradesTabActive"') . '>Activities</a>', $output);
    if ($user_role == NTLP_ROLEID_NETWORKMANAGER || $user_role == NTLP_ROLEID_NETWORKSTAFF
            || $user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_SITEGUEST) {
        $output = str_replace('@DOWNLOAD_COURSE_PDF', '<a href="' . url('ntlp/report/course_progress/' . $school_nid . '/' . $school_year . '/' . $term_tid . '/' . $courseid) .
                        '" target="_blank" class="SubmitBtnVeryLong">Course Progress Reports for All Students</a>', $output);
    } else {
        $output = str_replace('@DOWNLOAD_COURSE_PDF', '', $output);
    }
    return $output;
}

function grades_gasc_block_contents($display) {
    include "grades.template.inc";
    global $user, $base_url, $base_path;

    if ($courseid == 0) {
        //Getting Query String From URL
        $courseid = 0;
        $studentid = 0;
        $url = $_GET['q'];
        $args = explode('/', $url);
        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i]) && empty($courseid))
                $courseid = $args[$i];
            elseif (is_numeric($args[$i]) && empty($studentid))
                $studentid = $args[$i];
        }
//        if ($args[0] == 'node')
//            $courseid = $args[1];
    }
//echo 'before set '.$studentid;
    $view_activities = t('You do not have the required permissions.');
    $student_box = '';
    $templinks_box = '';
    $outcome_box = '';

//    if (!user_access('grade submission')) {
//        $studentid = $user_id;
//    }
//echo 'after set '.$studentid;
//    $output = $outcome_box;
//    $output = str_replace('@GRADES_VIEW', $view_activities, $output);
//    $output = str_replace('@OUTCOME_BOX', $outcome_box, $output);
//    $output = str_replace('@BACK_LINK', url("ntlp/courses/grade/$courseid"), $output);

    if (isset($user->roles[NTLP_ROLEID_STUDENT])) {
        if ($user->uid != $studentid) {
            set_item_url_handler('Gradebook');
            drupal_set_message("You are not authorized to access this student account information", "error");
            drupal_goto('ntlp/goback');
        }
    }

    return showStudentGrades($courseid, $studentid, $display, $view_activities, $outcome_box, $student_box, $templinks_box);
}

function showStudentGrades($courseid, $studentid, $display, $view_activities, $outcome_box, $student_box, $templinks_box) {
    include "grades.template.inc";
    global $base_path;

    //Show Outcome summary for the selected students
    $user_role = check_user_permission($courseid);

    set_item_url_handler('Student Gradebook');

    $rec_user = get_user($studentid);
    if (!($rec_user === false)) {

        $term_tid = 0;
        $course_Obj = get_course_detail($courseid);
        $school_nid = $course_Obj->school_nid;
        $term_tid = get_current_selected_course_term();
        $term_rec = get_school_term_info($term_tid);
        $school_year = $term_rec->school_year;
        
        $rs = get_outcome_student($courseid, $school_year, $term_tid, $studentid);
        $outcome_lines = '';
//        $total_grade = 0;
        $count = 0;
        while ($rec = db_fetch_object($rs)) {
            if (!isset($rec->grade)) {
                $gv = "*";
            } else {
                if ($rec->grade > 100) {
                    $gv = 100;
                } else {
                    $gv = $rec->grade;
                }
                //Show actual grade % even if goes above 100%
                $ag = FormatNumber($rec->grade, 0, ',');
            }
            $outcome_lines .= $HTMLoutcome_line;
            $outcome_lines = str_replace('@OUTCOME_NAME', $rec->name, $outcome_lines);
            $outcome_lines = str_replace('@OUTCOME_GRADE_PERCENT', $gv == '*' ? 0 : $gv, $outcome_lines);
            $outcome_lines = str_replace('@OUTCOME_GRADE', $gv == '*' ? $gv : $ag . '%', $outcome_lines);

            $activities_count = get_outcome_activities_count($term_tid, $courseid, $rec->tid, $studentid);
            $activities_count .= ( $activities_count == 1) ? ' activity' : ' activities';
            $outcome_lines = str_replace('@OUTCOME_ACTIVITY_COUNT', '(' . $activities_count . ')', $outcome_lines);
//            $outcome_lines .= $HTMLoutcome_line;
            $count++;
//            $total_grade += $gv;
        }
        $student_box = $HTMLstudent_box;
        $student_box = str_replace('@STUDENT_NAME', l($rec_user->first_name . ' ' . $rec_user->last_name, 'ntlp/user/profile/' . $rec_user->uid, array('attributes' => array('style' => 'color:#009900'))), $student_box);

        $student_box = str_replace('@USER_PICTURE_PATH', is_user_image_exist($rec_user->picture), $student_box);

        $student_grade = '';
        $grade_output = '';

        if (!is_not_graded_course($courseid)) {
            if (_user_activity_submission_by_course_nid($term_tid, $courseid, $rec_user->uid)) {
                //            $rec = get_course($courseid);
                //            if($r = get_current_school_term($rec->school_nid)) {
                //                $row = db_fetch_object($r);
                //                    $student_grade .= _get_course_grade(
                //                            _get_student_course_percentage( $rec_user->uid, $courseid, $row->school_term_tid),
                //                        $courseid);
                //            } else {
                //                $student_grade .= 'Term In-active!';
                //            }
                $stu_percent = _get_student_course_percentage($rec_user->uid, $courseid, $term_tid);
                $student_grade = _get_course_grade($stu_percent, $courseid);
                $grade_output = '<div align="center" class="grade">';
                $grade_output .= $student_grade;
                $grade_output .= '</div>';
            }
        }

        $outcome_box = $HTMLcontainer;

        if ($user_role == NTLP_ROLEID_NETWORKMANAGER || $user_role == NTLP_ROLEID_NETWORKSTAFF
                || $user_role == NTLP_ROLEID_TEACHER && is_user_enrolled($courseid, $studentid)) {

            $edit = (isset($_GET['tab'])) ? ($_GET['tab'] == 'grade') : false;

            $students_tab = '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="' . url('ntlp/courses/grade/') . $courseid . '/' . $studentid . '&tab=view" id="grades_tab_viewall" ' . ((!$edit) ? 'class="GradesTabActive"' : '' ) . ' > View All</a>';
            $tab_separator = ' | ';
            $activities_tab = '<a href="' . url('ntlp/courses/grade/') . $courseid . '/' . $studentid . '&tab=grade" id="grades_tab_gradeall" ' . (($edit) ? 'class="GradesTabActive"' : '' ) . ' >Grade All</a>';

            $outcome_box = str_replace('@TAB_STUDENTS', '<table><tr><td width="40%"; style="color:#3660a6;font-size:16px;">Activities</td><td width="60%";>' . $students_tab . $tab_separator . $activities_tab . '</td></tr></table>', $outcome_box);

            $tab_separator = '';
            $activities_tab = '';
        }

        $outcome_box = str_replace('@OUTCOME_LINES', $outcome_lines, $outcome_box);
        $outcome_box = str_replace('@HEADING', $grade_output == '' ? 'Not Graded' : 'Current Weighted Grade', $outcome_box);
        $outcome_box = str_replace('@COURSE_GRADE_DATA', $grade_output, $outcome_box);
        $outcome_box = str_replace('@STUDENT_BOX', $student_box, $outcome_box);
        $outcome_box = str_replace('@TAB_STUDENTS', '<h2 style="color:#3660a6;font-size:16px;">Activities</h2>' . $students_tab, $outcome_box);
        $outcome_box = str_replace('@TAB_SEPARATOR', '' . $tab_separator, $outcome_box);
        $outcome_box = str_replace('@TAB_ACTIVITIES', '' . $activities_tab, $outcome_box);

        if ($user_role == NTLP_ROLEID_NETWORKMANAGER || $user_role == NTLP_ROLEID_NETWORKSTAFF
                || $user_role == NTLP_ROLEID_TEACHER) {

            $output = $HTMLreport_buttons;
            $output = str_replace('@STUDENT_COURSE_REPORT', l('Course Progress Report', 'ntlp/report/student_progress/' . $school_nid . '/' . $school_year . '/' . $term_tid . '/' . $courseid . '/' . $rec_user->uid, array('attributes' => array('target' => '_blank'))), $output);
            $output = str_replace('@STUDENT_COURSE_DETAILED_REPORT', l('Detailed Report', 'ntlp/report/student_detailed_progress/' . $school_nid . '/' . $school_year . '/' . $term_tid . '/' . $courseid . '/' . $rec_user->uid, array('attributes' => array('target' => '_blank'))), $output);

            $outcome_box = str_replace('@REPORT_STUDENT_PDF', $output, $outcome_box);
//            $outcome_box = str_replace('@REPORT_STUDENT_PDF', '<a style="margin:0 5px 0 0" href="' . url('ntlp/report/student_progress/' . $school_nid . '/' . $school_year . '/' . $term_tid . '/' . $courseid . '/' . $rec_user->uid) .
//                            '" class="SubmitAssignmentBtn" target="_blank">Course Progress Report</a>', $outcome_box);
        } else {
            $outcome_box = str_replace('@REPORT_STUDENT_PDF', '', $outcome_box);
        }
        $outcome_box = str_replace('@DOWNLOAD_COURSE_PDF', '', $outcome_box);

        //Refresh DW table
        $view_activities = render_course_student_graded_activities_table($user_role, $courseid, $studentid, $term_tid, $filters, $asblock);

//        if ($user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_DIRECTORS
//                || $user_role == NTLP_ROLEID_NETWORKMANAGER || $user_role == NTLP_ROLEID_SITEADMIN
//                || $user_role == NTLP_ROLEID_SCHOOLLEADER) {
//            $view_activities = drupal_get_form('render_course_student_graded_activities_form', $user_role, $courseid, $studentid, $term_tid, $filters, $asblock);
//        } else {
//            $view_activities = drupal_get_form('render_course_student_graded_activities_form', $user_role, $courseid, $studentid, $term_tid, $filters, $asblock, false);
//        }
        $view_activities = str_replace('@GRADES_VIEW', $view_activities, $outcome_box);
    } else {
        $view_activities = t('Student not found!.');
    }

    return $view_activities;
}

function FormatNumber($number, $decimals = 0, $thousand_separator = '&nbsp;', $decimal_point = '.') {
    $tmp1 = round((float) $number, $decimals);

    while (($tmp2 = preg_replace('/(\d+)(\d\d\d)/', '\1 \2', $tmp1)) != $tmp1)
        $tmp1 = $tmp2;

    return strtr($tmp1, array(' ' => $thousand_separator, '.' => $decimal_point));
}

function get_gradescale($grade) {
    $scale = "F";
    if ($grade >= 90) {
        $scale = "A+";
    } elseif ($grade >= 80) {
        $scale = "A";
    } elseif ($grade >= 70) {
        $scale = "B+";
    } elseif ($grade >= 60) {
        $scale = "B";
    } elseif ($grade >= 50) {
        $scale = "C+";
    } elseif ($grade >= 40) {
        $scale = "C";
    } elseif ($grade >= 30) {
        $scale = "D";
    } elseif ($grade >= 20) {
        $scale = "E";
    }
    return $scale;
}

function _get_course_outcome_grade($course_Obj) {
    include "grades.template.inc";

    $term_tid = get_current_selected_course_term();
    $term_rec = get_school_term_info($term_tid);
    $school_year = $term_rec->school_year;

    $rs = get_outcome_student($course_Obj->nid, $school_year, $term_tid);

    $enrolled_students_arr = get_enrolled_students_in_course($course_Obj->nid);
    $outcome_lines = '';
//    $total_grade = 0;
    $count = 0;

    if ($recent_act = get_recent_activities_graded($course_Obj->nid, $term_tid)) {
//        watchdog('course_grade', 'recent update ' . print_r($recent_act, true));
        $recent_act_graded_date = get_tz_course($course_Obj->nid, 'm/d/Y', ($recent_act[0]->date_graded));
        $recent_act = '<tr>
                           <td colspan="2" style="border:none"></td>
                       </tr>
                       <tr>
                           <td style="border:none"><b style="color:#3570a8">Grades Last Updated</b></td>
                       </tr>
                       <tr>
                           <td style="border:none" colspan="2">
                              <b style="color:#F27B1F;font-size:15px">' . $recent_act_graded_date . '</b>
                           </td>
                       </tr>';
    } else {
        $recent_act = '';
    }

    $stuCount = (int) _get_students_having_grade_percent($course_Obj->nid, $term_tid, 70, 'lt', true);

    $grade_output = '';
    $grade_output .= '<table style="border:none" align="center"><tr><td style="border:none"><b style="color:#3570a8">Enrolled Students</b></td><td style="border:none"></td></tr><tr><td style="border:none" colspan="2"><b style="color:#F27B1F;font-size:15px">' . sizeof($enrolled_students_arr) . '</b></td></tr>';
    $grade_output .= '<tr><td colspan="2" style="border:none"></td></tr><tr><td style="border:none"><b style="color:#3570a8">Students Below  70%<b></td><td style="border:none"></td></tr><tr><td style="border:none" colspan="2"><b style="color:#F27B1F;font-size:15px">' . $stuCount . '</b></td></tr>';
//    $grade_output .= '<tr><td colspan="2" style="border:none"></td></tr><tr><td style="border:none"><b style="color:#3570a8">Grades Last Updated</b></td></tr><tr><td style="border:none" colspan="2"><b style="color:#F27B1F;font-size:15px">' . $recent_act . '</b></td></tr></table>';
    $grade_output .= $recent_act;
    $grade_output .= '</table>';

    while ($rec = db_fetch_object($rs)) {
        if (!isset($rec->grade)) {
            $gv = "*";
        } else {
            if ($rec->grade > 100) {
                $gv = 100;
            } else {
                $gv = $rec->grade;
            }
            //Show actual grade % even if goes above 100%
            $ag = FormatNumber($rec->grade, 0, ',');
        }
        $outcome_line = $HTMLoutcome_line;
        $outcome_line = str_replace('@OUTCOME_NAME', $rec->name, $outcome_line);
        $outcome_line = str_replace('@HEADING', 'Class Stats', $outcome_line);
        $outcome_line = str_replace('@COURSE_GRADE_DATA', $grade_output, $outcome_line);
        $outcome_line = str_replace('@OUTCOME_GRADE_PERCENT', $gv == '*' ? 0 : $gv, $outcome_line);
        $outcome_line = str_replace('@OUTCOME_GRADE', $gv == '*' ? $gv : $ag . '%', $outcome_line);

        $activities_count = get_outcome_activities_count($term_tid, $course_Obj->nid, $rec->tid);

        $activities_count .= ( $activities_count == 1) ? ' activity' : ' activities';
        $outcome_line = str_replace('@OUTCOME_ACTIVITY_COUNT', '(' . $activities_count . ')', $outcome_line);

        $outcome_lines .= $outcome_line;
        $count++;
//        $total_grade += $gv;
    }
    if ($count > 0) {
        $outcome_box = $HTMLcontainer;
        $outcome_box = str_replace('@OUTCOME_LINES', $outcome_lines, $outcome_box);
        $outcome_box = str_replace('@HEADING', 'Class Stats', $outcome_box);
        $outcome_box = str_replace('@COURSE_GRADE_DATA', $grade_output, $outcome_box);

        return $outcome_box;
    } else {
        return $outcome_line;
//        drupal_set_message("Fatal error or course is not configured properly");
    }
}

function get_gradebook_student_view($courseid) {
    $view = views_get_view('ntlp_grades_stu');

    $display_id = 'default';
    $cols = get_course_outcomes($courseid);

    while ($col = db_fetch_object($cols)) {

        $outcome_name = '';
        $str_arr = explode(" ", $col->name);

        $str_length = sizeof($str_arr);

        for ($j = 0; $j < $str_length; $j++) {

            if (strlen($str_arr[$j]) >= 8) {
                $value = substr($str_arr[$j], 0, 8);
                $strval = _get_consonant($value);
                $outcome_name .= $strval . ' ';
            } else {
                $outcome_name .= $str_arr[$j] . ' ';
            }
        }


        $id = $view->add_item($display_id, 'field', 'ntlp_course_activity', 'outcome_tid',
                        array('outcome_tid' => $col->tid,
                            'outcome_name' => $outcome_name . '  ' . floor($col->weight_percent) . '%',
                            'calc_type' => 'GAA',
                ));
    }


    $view_activities = $view->preview($display_id, array($courseid));
    echo $view_activities;
}

function get_gradebook_activities_view($course_nid, $term_tid) {

    $user_role = check_user_permission($course_nid);

    //Get Sort order
    $sort_order = strtolower(isset($_GET['order']) ? $_GET['order'] : 'due_date');
    $sort_dir = isset($_GET['sort']) ? $_GET['sort'] : 'desc';
    $sort_dir = $sort_dir == 'none' ? 'desc' : $sort_dir;
    $sort_field = $sort_order;

    if ($sort_order == "due date")
        $sort_field = 'due_date';
    if ($sort_order == "activity")
        $sort_field = 'node_title';
    if ($sort_order == "type")
        $sort_field = 'node_type';
    if ($sort_order == "view")
        $sort_field = 'available_from';
    if ($sort_order == "submitted")
        $sort_field = 'submissions_percent';
    if ($sort_order == "grades published")
        $sort_field = 'published_date';

    $result_set = get_course_activities_detail($term_tid, $course_nid, $user_role, $sort_field . " $sort_dir");

    $view_activities = ntlp_render_activity_table($term_tid, $result_set, $user_role, true);

    return '<div id="gradebook_activities">' . $view_activities . '</div>';
}

//-----------------------------------------------
// Gradebook table view using Drupal table
// Teachers view of all student's grades
//
//function render_course_student_grades($course_nid, $term_tid, $filters=array()) {
//
//    global $base_path;
//    $cols = get_course_outcomes($course_nid);
//    $otcols = array();
//    $outcomes = array();
//
//    while ($col = db_fetch_object($cols)) {
//        $otcols[] = array(
//            'data' => $col->name . '  ' . floor($col->weight_percent) . '%',
//            'field' => 'outcome_'.$col->tid,
//            'width' => '40');
//        $outcomes[$col->tid] = $col->name . '  ' . floor($col->weight_percent) . '%';
//    }
//    $head = array(
//            array('data' => t('Period'), 'field' => 'period', 'width' => '30'),
//            array('data' => t('Name'), 'field' => 'name', 'sort' => 'asc','width' => '100'),
//            array('data' => t('Grade'), 'width' => '30'),
//            array('data' => t('%'), 'width' => '20'),
//    );
////            array('data' => t('Grade'), 'field' => 'grade_letter', 'width' => '30'),
////            array('data' => t('%'), 'field' => 'grade_percent', 'width' => '20'),
//    $head = array_merge($head, $otcols);
//
//    //Get Sort order
//    $sort_order = isset($_GET['order']) ? $_GET['order'] : 'name';
//    $sort_dir = isset($_GET['sort']) ? $_GET['sort'] : 'asc';
//    $sort_field = $sort_order;
//    reset($outcomes);
//    if ($sort_order != "Period" || $sort_order != "Name" || $sort_order != "Grade" || $sort_order != "%") {
//        while (list($key, $val) = each($outcomes)) {
//            if ($sort_order == $val) {
//                $sort_field = "outcome_$key";
//            }
//        }
//    }
//    //echo "<br>$sort_order --- $sort_dir --- $sort_field";
//    //Get data
//    $datars = getview_grades_stu($course_nid, $outcomes, $sort_field.' '.$sort_dir);
//    while($data = db_fetch_array($datars)) {
//        $percent = _get_student_course_percentage( $data['uid'], $course_nid );
//        $grade_letter = _get_course_grade($percent, $course_nid, 'L');
//        $grade_percent = format_course_grade_percent($percent, $course_nid);
//
//        $datarow = array(
//                array('data' => $data['period']),
//                array('data' => l($data['name'], 'ntlp/courses/grade/'.$course_nid.'/'.$data['uid'], array('attributes' => array('class' => 'greenLink')))),
//                array('data' => $grade_letter,
//                    'style' => "text-align: center ! important; background: none repeat scroll 0% 0% rgb(159, 204, 233) ! important;"),
//                array('data' => $grade_percent,
//                    'style' => "text-align: center ! important; background: none repeat scroll 0% 0% rgb(159, 204, 233) ! important;"),
//            );
//        $otcols = array();
//        reset($outcomes);
//        while (list($key, $val) = each($outcomes)) {
//            $otcols[] = array('data' => number_format($data['outcome_'.$key], 0, '.', '').'%',
//                'style' => 'text-align: center');
//        }
//        $datarow = array_merge($datarow, $otcols);
//        $rows[] = $datarow;
//    }
//
//    if(!$rows) {
//        $rows[] = array( array( 'data' => '<strong>No students found.</strong>', 'colspan' => '4', 'align' => 'center') );
//    }
//
//    $attributes = array('class' => 'ntlp_table','width' => '100%','cellpadding' => '8', 'cellspacing' => '0', 'style' => 'margin-bottom:50px !important;
//              border-top:1px solid #9ecbe8; border-bottom:1px solid #9ecbe8; border-left:1px solid #9ecbe8 !important; border-right:1px solid #9ecbe8 !important;');
//    $output = theme('table', $head, $rows,$attributes);
////    $output .= theme('pager', NULL, $limit, 0);
//    return '<div id="school_grade_students">'.$output.'</div>';
//}


function render_course_student_grades($course_nid, $term_tid=0, $filters=array(), $asblock=false) {
    global $base_path;

    include "grades.template.inc";
//    print '<br>Selected Term in Grades: '.print_r($term_tid, true);
    //Redirection was implemented to make the sort links work when block is rendered from Ajax
//    $asblock = $_GET['sort'] == 'none' ? 1 : 0;
//    if ($asblock == 0 && strpos($_GET['q'], 'gradebook/student_view')) {
//        drupal_goto("ntlp/courses/grade/$course_nid", "sort=" . $_GET['sort'] . "&order=" . $_GET['order']);
//    }
//    print $_GET['q'] . ", " . $_GET['sort'] . ", " . $asblock;
//    $asblock = strpos($_GET['q'], 'gradebook/student_view');
//    $asblock = $asblock && !strpos($_GET['q'], 'sort=');
//    $asblock = $asblock && !strpos($_SERVER['HTTP_REFERER'], 'courses/grade');
//    print $asblock;
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
    $options = array(
        'Status' => array(
            'Enrolled' => array(
                'type' => 'checkbox',
                'group' => 'enroll_status',
                'value' => 1,
                'name' => 'enrolled',
                'default' => true,
            ),
            'Dropped' => array(
                'type' => 'checkbox',
                'group' => 'enroll_status',
                'value' => 2,
                'name' => 'dropped',
                'default' => false,
            ),
        ),
        'Course Grades' => array(
            'All' => array(
                'type' => 'checkbox',
                'group' => 'courses_grade',
                'value' => 1,
                'name' => 'all',
                'default' => true,
            ),
            '100-90%' => array(
                'type' => 'checkbox',
                'group' => 'courses_grade',
                'value' => 100,
                'name' => 0,
                'default' => false,
            ),
            '89-80%' => array(
                'type' => 'checkbox',
                'group' => 'courses_grade',
                'value' => 89,
                'name' => 1,
                'default' => false,
            ),
            '79-70%' => array(
                'type' => 'checkbox',
                'group' => 'courses_grade',
                'value' => 79,
                'name' => 2,
                'default' => false,
            ),
            '69-60%' => array(
                'type' => 'checkbox',
                'group' => 'courses_grade',
                'value' => 69,
                'name' => 3,
                'default' => false,
            ),
            '59-0%' => array(
                'type' => 'checkbox',
                'group' => 'courses_grade',
                'value' => 59,
                'name' => 4,
                'default' => false,
            ),
        ),
    );

    $cols = get_course_outcomes($course_nid);
    $otcols = array();
    $outcomes = array();
    $ocno = 1;
    while ($col = db_fetch_object($cols)) {

//        watchdog('columname', $col->name);
        $outcome_name = '';
        $str_arr = explode(" ", $col->name);

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

        $col_width_offset = 0;

        if (strpos($_SERVER['HTTP_USER_AGENT'], 'Safari')) {
            // Chrome user agent string contains both 'Chrome' and 'Safari'
            $col_width_offset = 20;
        }

        $otcols[] = array(
            'data' => $outcome_name . '  <span style="font-weight: normal !important;font-size:11px;">' . floor($col->weight_percent) . '%</span>',
            'field' => 'outcome_value_' . $ocno,
            'title' => $col->name,
            'width' => 54 + $col_width_offset,
            'onmouseover' => 'set_header_title()');
        $outcomes[$col->tid] = $otcols[sizeof($otcols) - 1]['data'];
        $ocno++;
    }
    $head = array(
        array('data' => t('Per.'), 'field' => 'period', 'width' => 20 + $col_width_offset),
        array('data' => t('Name'), 'field' => 'fullname', 'sort' => 'asc', 'width' => 150 + $col_width_offset),
        array('data' => t('Grade'), 'field' => 'grade_letter', 'width' => 30 + $col_width_offset),
        array('data' => t('%'), 'field' => 'grade_percent', 'width' => 30 + $col_width_offset),
    );
//            array('data' => t('Grade'), 'field' => 'grade_letter', 'width' => '30'),
//            array('data' => t('%'), 'field' => 'grade_percent', 'width' => '20'),

    //Add an additional column for spacing
    $size_of_outcome = count($otcols);
    if (count($otcols) < 7) {
        $otcols[] = array('data' => '');
    }
    $head = array_merge($head, $otcols);

    //Get Sort order
    $sort_order = strtolower(isset($_GET['order']) ? $_GET['order'] : 'fullname');
    $sort_dir = isset($_GET['sort']) ? $_GET['sort'] : 'asc';
    $sort_dir = $sort_dir == 'none' ? 'asc' : $sort_dir;
    $sort_field = $sort_order;
    reset($outcomes);
    $ocno = 1;
    if ($sort_order != "per." || $sort_order != "name" || $sort_order != "grade" || $sort_order != "%") {
        while (list($key, $val) = each($outcomes)) {
            if ($sort_order == strtolower($val)) {
                $sort_field = "outcome_value_$ocno";
            }
            $ocno++;
        }
    }
    if ($sort_order == "per.")
        $sort_field = 'period';
    if ($sort_order == "name")
        $sort_field = 'fullname';
    if ($sort_order == "grade")
        $sort_field = 'grade_letter';
    if ($sort_order == "%")
        $sort_field = 'grade_percent';

//    echo "<br>$sort_order --- $sort_dir --- $sort_field";
    //Get data
    $datars = dwgetview_grades_stu($course_nid, $term_tid, $ocno - 1, $sort_field . ' ' . $sort_dir, $filters);

    while ($data = db_fetch_array($datars)) {
        $percent = $data['grade_percent'];
        $grade_letter = $data['grade_letter'];
        //if ($grade_letter =="INC")
        //$grade_percent = "*";
        //else
        $grade_percent = format_course_grade_percent($percent, $course_nid, $grade_letter);

        $datarow = array(
            array('data' => $data['period'] == 0 ? '*' : $data['period']),
            array('data' => l($data['fullname'], 'ntlp/courses/grade/' . $course_nid . '/' . $data['uid'], array('attributes' => array('class' => 'greenLink')))),
            array('data' => $grade_letter,
                'style' => "text-align: center ! important; background: none repeat scroll 0% 0% rgb(159, 204, 233) ! important;"),
            array('data' => $grade_percent,
                'style' => "text-align: center ! important; background: none repeat scroll 0% 0% rgb(159, 204, 233) ! important;"),
        );
        $otcols = array();
        reset($outcomes);
        $ocno = 1;
        while (list($key, $val) = each($outcomes)) {
            if (!isset($data['outcome_value_' . $ocno])) {
                $gv = "*";
            } else {
                $gv = number_format($data['outcome_value_' . $ocno], 0, '.', '') . '%';
            }
            $otcols[] = array('data' => $gv, 'style' => 'text-align: center');
            $ocno++;
        }

        //Add an additional column for spacing
        if (count($otcols) < 7) {
            $otcols[] = array('data' => '');
        }
        $datarow = array_merge($datarow, $otcols);

        $rows[] = $datarow;
    }

    if (!$rows) {
        $rows[] = array(array('data' => '<strong>No students found.</strong>', 'colspan' => '4', 'align' => 'center'));
    }

//    $output = $HTML_GRADEBOOK_FILTER;
    ///////////////////////////////////////////////
    ////////////////////////////////////////////////////////////
    $attributes = array('class' => 'ntlp_table', 'cellpadding' => '8', 'cellspacing' => '0', 'style' => 'margin-bottom:50px !important;
            border-top:1px solid #9ecbe8; border-bottom:1px solid #9ecbe8; border-left:1px solid #9ecbe8 !important; border-right:1px solid #9ecbe8 !important;',
            'width' => $size_of_outcome < 7 ? "100%" : (230 + (22*4) +($size_of_outcome*(56 + 20 + 2))) );
    $output .= '<div style="position:relative;">'.theme('table', $head, $rows, $attributes).'</div>';
//    $output .= theme('pager', NULL, $limit, 0);
//    $output = str_replace('ntlp/courses/grade/', 'ntlp/gradebook/grade/filter/', $output);
//    $output = str_replace('ntlp/gradebook/student_view/', 'ntlp/gradebook/grade/filter/', $output);


    if (!isset($_GET['sort']) && !isset($_GET['filter']) && $_GET['filter'] != 'none') {
        $opts = render_custom_filter(
                        $options,
                        array('start' => $HTML_grade_school_filter_cellStart, 'end' => $HTML_grade_school_filter_cellEnd),
                        array('template' => $HTML_grade_school_filter_heading, 'placeholder' => '@HEADING'),
                        $HTML_grade_school_filter_row_end,
                        url('ntlp/gradebook/grade/filter/'.$course_nid) . '&filter=none',
                        'school_grade_students'
        );

        $foutput = str_replace('@OPTIONS', $opts, $HTML_GRADEBOOK_FILTER);
    }
    if (($_GET['filter'] == 'none') || isset($_GET['sort']) || isset($_GET['block']))
        if (isset($_GET['block']))
            return $foutput . '<div id="school_grade_students">' . $output . '</div>';
        else
            print $output;
    else
        return $foutput . '<div id="school_grade_students">' . $output . '</div>';
}

function render_course_student_graded_activities_table($user_role, $course_nid, $student_id, $term_tid, $filters=array(), $asblock=false, $render_form = true) {
    global $base_path;
    $edit = (isset($_GET['tab'])) ? 
        ($_GET['tab'] == 'grade'
        && ($user_role == NTLP_ROLEID_NETWORKMANAGER || $user_role == NTLP_ROLEID_NETWORKSTAFF || $user_role == NTLP_ROLEID_TEACHER)
        && is_user_enrolled($course_nid, $student_id)) : false;

    include "grades.template.inc";

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

    drupal_add_js(drupal_get_path('module', 'ntlp_blocks') . '/grades.contents.js');

    $cols = get_course_outcomes($course_nid);
    $otcols = array();
    $outcomes = array();
    $ocno = 1;
    while ($col = db_fetch_object($cols)) {

//        watchdog('columname', $col->name);
        $outcome_name = '';
        $str_arr = explode(" ", $col->name);

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

        $col_width_offset = 0;

        if (strpos($_SERVER['HTTP_USER_AGENT'], 'Safari')) {
            // Chrome user agent string contains both 'Chrome' and 'Safari'
            $col_width_offset = 20;
        }

        $otcols[] = array(
            'data' => $outcome_name . '  <span style="font-weight: normal !important;font-size:11px;">' . floor($col->weight_percent) . '%</span>',
            'field' => 'outcome_' . $ocno,
            'title' => $col->name,
            'width' => 56 + $col_width_offset,
            'onmouseover' => 'set_header_title()');
        $outcomes[$col->tid] = $otcols[sizeof($otcols) - 1]['data'];
        $ocno++;
    }

    $head = array(
        array('data' => t('Student Work and Assessment'), 'field' => 'activity_title', 'sort' => 'asc', 'width' => 200 + $col_width_offset)
    );

    global $user;

    if ((!isset($user->roles[NTLP_ROLEID_PARENT]) && $user_role == NTLP_ROLEID_SITEGUEST) || $user_role == NTLP_ROLEID_NETWORKMANAGER || $user_role == NTLP_ROLEID_NETWORKSTAFF
                || $user_role == NTLP_ROLEID_TEACHER) {

        $head[] = array('data' => t('Pub.'), 'field' => 'is_published', 'width' => 30 + $col_width_offset);
    }

    $head[] = array('data' => t('Late'), 'field' => 'is_late', 'width' => 30 + $col_width_offset);
    $head[] = array('data' => t('Inc'), 'field' => 'is_complete', 'width' => 30 + $col_width_offset);
    
    $size_of_outcome = count($otcols);
    if (count($otcols) < 7) {
        $otcols[] = array('data' => '');
    }
    $head = array_merge($head, $otcols);

    //Get Sort order
    $sort_order = strtolower(isset($_GET['order']) ? $_GET['order'] : 'ntlp_course_activity.due_date DESC, activity_title');
    $sort_dir = isset($_GET['sort']) ? $_GET['sort'] : 'asc';
    $sort_dir = $sort_dir == 'none' ? 'asc' : $sort_dir;
    $sort_field = $sort_order;
    reset($outcomes);
    $ocno = 1;
    if ($sort_order != "student work and assessment" || $sort_order != "late" || $sort_order != "inc" || $sort_order != "pub.") {
        while (list($key, $val) = each($outcomes)) {
            if ($sort_order == strtolower($val)) {
                $sort_field = "(outcome_graded_$key + 0)";
            }
            $ocno++;
        }
    }
    if ($sort_order == "student work and assessment")
        $sort_field = 'activity_title';
    if ($sort_order == "inc")
        $sort_field = 'is_complete';
    if ($sort_order == "late")
        $sort_field = 'is_late';
    if ($sort_order == "pub.")
        $sort_field = 'is_published';

    //Save data from cache as well
    ntlp_student_courses_grade_all_table_save($student_id, $course_nid, $term_tid, $user->uid);

    //Get data
    $datars = get_student_course_activity_outcomes($course_nid, $student_id, $term_tid, NULL, $sort_field . ' ' . $sort_dir, NULL, true);

    while ($data = db_fetch_array($datars)) {
        // check late sumbmission
        $new_array[] = $data;

        if ($data['grading_method'] == 'C') {
            $activity_grading_method = 1; // setting a value 1 if this activity is auto grading.. true
        } else {
            $activity_grading_method = 0;
        }

        $activity_ids[] = $data['activity_id'];
        $version_number = intval($data['version_num']);
        $version = ($version_number > 1) ? ( ' (v' . $version_number . ')' ) : '';

        $inc = ($edit) ? '<input onclick="submission_inc_tid(' . $student_id . ', ' . $data['activity_id'] . ');" type="checkbox" id=inc_activity_id_' . $data['activity_id'] . ' name=inc[' . $data['activity_id'] . '] ' . (($data['is_complete'] == 0 && $data['is_graded'] == 1) ? 'checked="checked"' : "") . ' >' : '';
        $inc .= (!$edit && $data['is_complete'] == 0 && $data['is_graded'] == 1) ? '<div style="text-align:center;">x</div>' : '';

        $inc .= ($edit) ? '<input type="hidden" id="activity_grading_method" value="' . $activity_grading_method . '" name="activites_nid[' . $data['activity_id'] . ']">' : '';

        $late = ($edit) ? '<input onclick="submission_late_tid(' . $student_id . ', ' . $data['activity_id'] . ');" type="checkbox" id=late_activity_id_' . $data['activity_id'] . ' name=late[' . $data['activity_id'] . '] ' . (($data['is_late']) ? 'checked="checked"' : "") . ' >' : '';
        $late .= (!$edit && $data['is_late'] == 1) ? '<div style="text-align:center;">x</div>' : '';

        $is_published = ( $data['is_published'] == 1) ? '<div style="text-align:center;">x</div>' : '';

        $activity_class = "";

        if ($data['not_gradeable'] == 0)
            $activity_class = " not_graded";
        elseif ($data['is_complete'] == 0 && $data['is_graded'] == 1)
            $activity_class = " incomplete";
        elseif ($data['is_late'] == 1)
            $activity_class = " late";

        if ($user_role == NTLP_ROLEID_NETWORKMANAGER || $user_role == NTLP_ROLEID_NETWORKSTAFF
                || $user_role == NTLP_ROLEID_TEACHER) {

            $notes = (isset($data['note_to_teacher']) && !empty($data['note_to_teacher']) ? ('<img title="' . strip_tags($data['note_to_teacher']) . '" align="right" src="' . $base_path . 'themes/Boldr/Images/common/comments.png" >') : '' );

            $activity_url = 'ntlp/courses/activity/grade/' . $course_nid . '/' . $data['activity_id'] . '/' . $student_id;
        } else {
            if ($data['is_published'] == 1) {
            $notes = (isset($data['teachers_notes']) && !empty($data['teachers_notes']) ? ('<img title="' . strip_tags($data['teachers_notes']) . '" align="right" src="' . $base_path . 'themes/Boldr/Images/common/comments.png" >') : '' );
            } else {
                $notes = '';
                $inc = '';
                $late = '';
                $activity_class = '';
            }

            $activity_url = 'ntlp/courses/activity/view/' . $course_nid . '/' . $data['activity_id'];
            $actiivty_url_query = 'tab=work';
        }

        $submission_date = isset($data['date_submitted']) && $version_number > 0 ? get_tz_course($course_nid, DATE_FORMAT_SUBMISSION, $data['date_submitted']) : '';

        $datarow = array(
            array('data' => l($data['activity_title'], $activity_url, array('attributes' => array('class' => 'greenLink'), 'query' => $actiivty_url_query))
                . $notes . '<br>' . $submission_date . $version,
                'class' => $activity_class,
                'name' => 'activity['.$data['activity_id'].']')
        );

        if ((!isset($user->roles[NTLP_ROLEID_PARENT]) && $user_role == NTLP_ROLEID_SITEGUEST) || $user_role == NTLP_ROLEID_NETWORKMANAGER || $user_role == NTLP_ROLEID_NETWORKSTAFF
                || $user_role == NTLP_ROLEID_TEACHER) {

            $datarow[] = array('data' => $is_published,
                'class' => $activity_class,
                'name' => 'activity['.$data['activity_id'].']');
        }

        $datarow[] = array('data' => $late,
                'class' => $activity_class,
                'name' => 'activity['.$data['activity_id'].']');

        $datarow[] = array('data' => $inc,
                'class' => $activity_class,
                'name' => 'activity['.$data['activity_id'].']');

        $otcols = array();
        reset($outcomes);
        $ocno = 1;

        if (!$edit && $data['not_gradeable'] != 1)
            $otcols[] = array('data' => 'This is not a graded activity', 'style' => 'text-align: center; ', 'colspan' => sizeof($outcomes), 'class' => $activity_class);
        else {
            while (list($key, $val) = each($outcomes)) {

                $gv = '';

                $score = "";
                $max_score = "";
                $cell_html = "";

                $score .= '<input type="hidden" value="' . $data['outcome_maxscore_' . $key] . '" id="' . $key . '" name="activity_outcome[' . $data['activity_id'] . ']">';
                if (isset($data['outcome_graded_' . $key]) || isset($data['outcome_maxscore_' . $key])) {

                    $disabled = ($edit && $data['is_complete'] == 0 && $data['is_graded'] == 1) ? ' disabled="disabled" ' : '';

                    if ($edit) {

                        $cell_html = "<span style='width:100%; ' align=left><span style='width:33px; ' align=right>@SCORE</span><span style='width:33px; ' align=left>&nbsp;/&nbsp;@MAX_SCORE</span></span>";

                        $score .= '<input ' . $disabled . 'style="text-align:center; font-weight: bold;" type="text" size="3" maxlength="6" onblur="submit_grade('. $data['activity_id']. ', '. $student_id  . ', ' . $data['outcome_maxscore_' . $key] . ', ' . $key . ',this.value);" id=score_outcome_' . $data['activity_id'] . ' onfocus="save_previous_score('. $data['activity_id'] . ', ' . $key . ');" name="outcome_scores[' . $data['activity_id'] . '][' . $key . ']" value="';
                        $score .= isset($data['outcome_graded_' . $key]) ? $data['outcome_graded_' . $key] : '';
                        $score .= '" />';
                    } else {

                        $td_class = empty($activity_class) ? "" : "class=$activity_class";
                        $cell_html = "<table align=center style='border-style:hidden !important;'><tr><td $td_class  style='width:33%; border-style:hidden !important;' align=right>@SCORE</td><td $td_class style='width:33%; border-style:hidden !important;' align=center> / </td><td $td_class style='width:33%; border-style:hidden !important;' align=left>@MAX_SCORE</td></td></tr></table>";

                        if (isset($data['outcome_maxscore_' . $key]) && strtoupper($data['outcome_graded_' . $key]) == 'EX')
                            $cell_html = '<span align=center><span align=center><b>@SCORE</b></span>';

                        $score .= '<b>';
                        $score .= isset($data['outcome_graded_' . $key]) ? $data['outcome_graded_' . $key] : (($data['is_complete'] == 0 && $data['is_graded'] == 1) ? '0' : '');
                        $score .= '</b>';
                    }
                }

                $cell_html = str_replace("@SCORE", $score, $cell_html);

                if (isset($data['outcome_maxscore_' . $key]))
                    $max_score = $data['outcome_maxscore_' . $key];

                $cell_html = str_replace("@MAX_SCORE", $max_score, $cell_html);
                $cell_html = str_replace("@MAX_SCORE", "", $cell_html); //for removing extra template texts

                $otcols[] = array('data' => $cell_html,
                                'style' => 'text-align: center; ',
                                'class' => $activity_class,
                                'name' => 'activity['.$data['activity_id'].']');
                $ocno++;
            }
        }
        if (count($otcols) < 7 &&  !(!$edit && $data['not_gradeable'] != 1)) {
            $otcols[] = array('data' => "");
        }
        $datarow = array_merge($datarow, $otcols);

        $rows[] = $datarow;
    }

    if (!$rows) {
        $rows[] = array(array('data' => '<strong>No activities found.</strong>', 'colspan' => sizeof($outcomes) + 3, 'align' => 'center'));
    }

    ///////////////////////////////////////////////
    ////////////////////////////////////////////////////////////
    $attributes = array('class' => 'ntlp_table', 'cellpadding' => '8', 'cellspacing' => '0', 'style' => 'margin-bottom:50px !important;
            border-top:1px solid #9ecbe8; border-bottom:1px solid #9ecbe8; border-left:1px solid #9ecbe8 !important; border-right:1px solid #9ecbe8 !important;',
            'width' => $size_of_outcome < 7 ? "100%" : (230 + (22*4) +($size_of_outcome*(56 + 20 + 2))) );

    $output .= '<div style="position:relative;">'.theme_table($head, $rows, $attributes).'</div>';

    if ($edit && $render_form) {
        return drupal_get_form('render_course_student_graded_activities_form',
                '<div id="school_stugrade_activities">' . $output . '</div>',
                $outcomes, $activity_ids, $course_nid, $student_id, $term_tid);
    }

    return '<div id="school_stugrade_activities">' . $output . '</div>';
}

function render_course_student_graded_activities_form($form_state, $output, $outcomes, $activity_ids, $course_nid, $student_id, $term_tid) {

    $last_saved_date = db_fetch_object(get_course_activity_last_graded_date($term_tid, $course_nid, $student_id))->date_graded;
    $last_saved_date = (isset($last_saved_date) ? ('Last saved ' . get_tz(get_user_school($user->uid), DATE_FORMAT_LAST_SAVED, $last_saved_date)) : "");

    $suffix = '</td><td><div name="last_saved_date" style="padding: 5px;" ' . (($last_saved_date != "") ? ' display:block; ' : ' display:none; ') . ' class="ProjectResources-YellowTopLeftCorner published_grade_box">@SAVE_DATE</div></td></tr></table>';
    $suffix = str_replace('@SAVE_DATE', $last_saved_date, $suffix);

    $form['save'] = array(
        '#type' => 'item',
        '#value' => '<img class="SaveBtnDisabled" src="' . $base_path . 'themes/Boldr/Images/SaveDisabled.png" style="display:none;" /><a class="SaveBtn" onclick=on_save_student_grade_all('.$student_id.','.$course_nid.','.$term_tid.') onmousedown="this.className=\'SaveBtnClick\'" onmouseup="this.className=\'SaveBtn\'">Save</a>',
        '#prefix' => '<table><tr><td>',
        '#suffix' => $suffix,
//        '#submit' => array('form_activities_grade_all_submit'),
//        '#validate' => array('form_activities_grade_all_validate')

    );

    $form['form_activities_grade_all'] = array(
        '#type' => 'item',
        '#value' => $output,
    );
    $form['form_activities_grade_all']['activity_ids'] = array(
        '#type' => 'hidden',
        '#value' => serialize($activity_ids),
    );
    $form['form_activities_grade_all']['outcomes'] = array(
        '#type' => 'hidden',
        '#value' => serialize($outcomes),
    );
    $form['form_activities_grade_all']['course_id'] = array(
        '#type' => 'hidden',
        '#value' => $course_nid,
    );
    $form['form_activities_grade_all']['school_term_id'] = array(
        '#type' => 'hidden',
        '#value' => $term_tid,
    );
    $form['form_activities_grade_all']['student_id'] = array(
        '#type' => 'hidden',
        '#value' => $student_id,
    );
    return $form;
}

//function form_activities_grade_all_submit($form, &$form_state) {
//
//    $course_id = $form_state['clicked_button']['#post']['course_id'];
//    $student_id = $form_state['clicked_button']['#post']['student_id'];
//    $activity_ids = unserialize($form_state['clicked_button']['#post']['activity_ids']);
//
//    $success = true;
//
//    foreach ($activity_ids as $key => $val) {
//
//        $outcomes = unserialize($form_state['clicked_button']['#post']['outcomes']);
//        foreach ($outcomes as $key_oc => $val_oc) {
//
//            $outcome_scores[$key_oc] = $form_state['clicked_button']['#post']['outcome_scores'][$val][$key_oc];
//        }
//
//        //save this activity
//        $submission = db_fetch_array(get_user_submission($val, $student_id, true));
//
//        $late = $form_state['clicked_button']['#post']['late'][$val];
//        $inc = $form_state['clicked_button']['#post']['inc'][$val];
//
//        $success = $success && save_activity_scores($course_id, $val, $outcome_scores, $submission['nid'], null, $late, $inc, false);
//    }
//}
//
//function form_activities_grade_all_validate($form, &$form_state) {
//
//    $activity_ids = unserialize($form_state['clicked_button']['#post']['activity_ids']);
//    $success = true;
//
//    foreach ($activity_ids as $key => $val) {
//
//        $outcomes = unserialize($form_state['clicked_button']['#post']['outcomes']);
//        foreach ($outcomes as $key_oc => $val_oc) {
//
//            $outcome_scores[$key_oc] = $form_state['clicked_button']['#post']['outcome_scores'][$val][$key_oc];
//
//            if ($outcome_scores[$key_oc] < 0) {
//                form_set_error('outcome_scores[' . $val . '][' . $key_oc . ']', "Invalid value for outcome score: " . $outcome_scores[$key_oc]);
//                $success = false;
//            }
//        }
//    }
//
//    return $success;
//}

function ntlp_gradebook_grade_filter() {
//    $school_nid = get_this_school();
    $grad_years = array();

    $url = $_GET['q'];
    $args = explode('/', $url);
    for ($i = 0; $i < count($args); $i++) {
        if (is_numeric($args[$i]))
            $courseid = $args[$i];
    }
    if (isset($_REQUEST['courses_grade']) and !isset($_REQUEST['courses_grade']['all'])) {
        foreach ($_REQUEST['courses_grade'] as $grade) {

            $grad_years['percent'][] = _get_student_grad_year_by_grade((int) $grade);
        }
    }
    $is_enrolled = 1;
    if (isset($_REQUEST['enroll_status']['enrolled'])) {
        $is_enrolled = 1;
    } elseif (isset($_REQUEST['enroll_status']['dropped'])) {
        $is_enrolled = 0;
    }

    $filters = array(
        'GRADE_YEARS' => $grad_years,
        'ENROLLED' => $is_enrolled,
    );

    $table = render_course_student_grades($courseid, get_current_selected_course_term(), $filters, false);
    //print replace_table_header_url('ntlp/gradebook/grade/filter', 'ntlp/courses/grade/' . $courseid, $table);
//    print replace_table_header_url('ntlp/gradebook/grade/filter', 'ntlp/courses/grade/1225', $table);
}

function ntlp_gradebook_stugrade_filter() {

    global $user;

    $url = $_SERVER['HTTP_REFERER'];
    $args = explode('/', $url);
    for ($i = 0; $i < count($args); $i++) {
        if (!isset($courseid) && is_numeric($args[$i]))
            $courseid = $args[$i];
        elseif (isset($courseid) && !isset($studentid)) {
            if (is_numeric($args[$i]))
                $studentid = $args[$i];
            else
                $studentid = strstr($args[$i], "&", true);
        }
    }
    $user_role = check_user_permission($courseid);

    if (isset($user->roles[NTLP_ROLEID_PARENT])) {  // if parent come here show their student record
        $user_role = NTLP_ROLEID_PARENT;
        $studentid = $_SESSION['PARENT_STUDENT_UID'];

        if (!check_user_exist_in_course($user_id, $courseid, true)) {
            drupal_goto("ntlp/courses/my");
        }
    }

    if ($user_role == NTLP_ROLEID_STUDENT)
        $studentid = $user->uid;

    watchdog("stugrade_filter", "role:$user_role studentid:$studentid userid:" . $user->uid);

    $table = render_course_student_graded_activities_table($user_role, $courseid, $studentid, get_current_selected_course_term(), $filters, false, false);
    print $table;
}

function _get_consonant($str) {
    $value = '';
    $flag = true;

    for ($i = strlen($str); $i > 0; $i--) {

        $vowel_regexp = "$[aeiou]$";
        $char = substr($str, ($i - 1), 1);

        if ($flag) {
            if (preg_match($vowel_regexp, $char)) {
                $value = substr($str, 0, ($i - 1));
                $flag = false;
            }
        }
    }
    return $value . '.';
}
