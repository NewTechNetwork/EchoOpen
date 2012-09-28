<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_grade', 'data_access');
module_load_include('inc', 'ntlp_school', 'basic_grade_report');
module_load_include('inc', 'ntlp_school', 'student_course_grade_report');
module_load_include('inc', 'ntlp_school', 'student_course_grade_detail_report');

// Generates a report for a school for a selected term
// Export Data in PDF format
function term_wise_export_in_PDF($school_nid, $term_tid, $school_year, $print_page) {
    verify_database_health();
    //http://localhost/echopb/?q=ntlp/reports/3709&school_term_tid=30&school_name=TV Test&todays_date=5 23, 2011&school_term_name=1st Term
    if ($print_page == 1) {
        $school_rec = get_school_record($school_nid);
        $school_name = $school_rec->title;
        if ($school_rec->grade_rounding == 1) {
            $grade_format = "#0." . str_repeat("0", $school_rec->grade_scale) . " %";
        } else {
            $grade_format = "#0 %";
        }
        $today = date('M d, Y', time());
        $term = get_school_term_name($term_tid);
        $query = array('school_term_tid' => $term_tid,
            'school_name' => $school_name,
            'grade_format' => $grade_format,
            'todays_date' => $today,
            'school_term_name' => $term->name,
            'school_year' => $school_year);

        return drupal_goto("ntlp/reports/190397284/PDF", $query);
    } else {
        return grade_report_export($school_nid, $school_year, $term_tid, $print_page);
    }
}

// Generates a report for a course for a selected term
// Export Data in PDF format
function course_wise_export_in_PDF($school_nid, $school_year, $selected_term_id, $course_id) {
    verify_database_health();

    $school_rec = get_school_record($school_nid);
    $school_name = $school_rec->title;
    if ($school_rec->grade_rounding == 1) {
        $grade_format = "#0." . str_repeat("0", $school_rec->grade_scale) . " %";
    } else {
        $grade_format = "#0 %";
    }
    $today = date('M d, Y', time());
    $term = get_school_term_name($selected_term_id);
    $query = array('school_term_tid' => $selected_term_id,
        'school_name' => $school_name,
        'grade_format' => $grade_format,
        'todays_date' => $today,
        'school_term_name' => $term->name,
        'school_year' => $school_year,
        'course_nid' => $course_id);

    return drupal_goto("ntlp/reports/190397282/PDF", $query);
//    return course_grade_report_export($school_nid, $school_year, $selected_term_id, $course_id);
}

// Generates a report for a course for a selected term
// Export Data in PDF format
function student_wise_export_in_PDF($school_nid, $school_year, $selected_term_id, $course_id) {
    verify_database_health();
    
    $school_rec = get_school_record($school_nid);
    $school_name = $school_rec->title;
    if ($school_rec->grade_rounding == 1) {
        $grade_format = "#0." . str_repeat("0", $school_rec->grade_scale) . " %";
    } else {
        $grade_format = "#0 %";
    }
    $today = date('M d, Y', time());
    $term = get_school_term_name($selected_term_id);
    $query = array('school_term_tid' => $selected_term_id,
        'school_name' => $school_name,
        'grade_format' => $grade_format,
        'todays_date' => $today,
        'school_term_name' => $term->name,
        'school_year' => $school_year,
        'course_nid' => $course_id,
        'student_uid' => arg(7));

    return drupal_goto("ntlp/reports/190397285/PDF", $query);
//    return course_grade_report_export($school_nid, $school_year, $selected_term_id, $course_id, arg(7));
}

function student_course_detail_export_in_PDF($school_nid, $school_year, $selected_term_id, $course_id) {
    verify_database_health();

    $school_rec = get_school_record($school_nid);
    $school_name = $school_rec->title;
    if ($school_rec->grade_rounding == 1) {
        $grade_format = "#0." . str_repeat("0", $school_rec->grade_scale) . " %";
    } else {
        $grade_format = "#0 %";
    }
    $today = date('M d, Y', time());
    $term = get_school_term_name($selected_term_id);
    $query = array('school_term_tid' => $selected_term_id,
        'school_name' => $school_name,
        'grade_format' => $grade_format,
        'todays_date' => $today,
        'school_term_name' => $term->name,
        'school_year' => $school_year,
        'course_nid' => $course_id,
        'student_uid' => arg(7));

    return drupal_goto("ntlp/reports/190397283/PDF", $query);
//    return course_grade_detail_report_export($school_nid, $school_year, $selected_term_id, $course_id, arg(7));
}

/* Export Data In CSV Format for Data Export Page */
function courses_data_export($selected_course_id, $school_nid, $header_check) {
    verify_database_health();

    if ($header_check == 0) {
        $course_ids = $selected_course_id;
        $term_ids = check_plain(arg(7));
        $sch_rec = get_school_record($school_nid);
        $school_year = $sch_rec->current_school_year;
        if ($sch_rec->grade_rounding == 1) {
            $grade_format = "#0." . str_repeat("0", $sch_rec->grade_scale) . " %";
        } else {
            $grade_format = "#0 %";
        }

        $query = array('school_term_tids' => $term_ids,
            'grade_format' => $grade_format,
            'course_nids' => $course_ids,
            'school_year' => $school_year);

        return drupal_goto("ntlp/reports/190397284/CSV", $query);
    }

    $course_ids = explode(',', $selected_course_id);
    $selected_term_id = explode(',', check_plain(arg(7)));
//    watchdog('expg_info_1', "$school_nid/".check_plain(arg(7))."/$selected_course_id");
    $t = 0;
    $sch_rec = get_school_record($school_nid);
    $school_year = $sch_rec->current_school_year;

    $outcome_name = _get_outcomes_names($school_nid, $school_year);
    $school_oc = explode(',', $outcome_name);
    if($header_check == 1) {
        $csv_output .= "Course ID,Course Name,Instructor ID,Instructor Name,Instructor ID,Instructor Name,Student ID,First Name,Middle Name,Last Name,Letter Grade,Percentage Grade,Term Name,".$outcome_name;
        $csv_output .= "\n";
    }
    for ($t=0; $t < sizeof($selected_term_id); $t++) {
        $term_id = $selected_term_id[$t];
        $term_rec = get_school_term_info($term_tid);
        $school_year = $term_rec->school_year;
        $term_rec = get_school_term_name($term_id);
        $term_name = $term_rec->name;

//        watchdog('expg_info_2', '['.$term_name.']<br>'.print_r($term_rec, true));
        $c = 0;
        for ($c=0; $c < sizeof($course_ids); $c++) {
            $course_id = $course_ids[$c];

        //    $courses = export_csv_data($course_id, $term_id);     //Returns a list of courses and activities, and students
            $courses = get_course_users($course_id, NTLP_ROLEID_STUDENT, "LF");
            $teacher = "";
            unset($teacher_rec);
            unset($teacher_rec2);
            $oc = 0;
            $school = get_school_record($school_nid);
//            watchdog('expg_info', "$school_nid/$term_id/$course_id");

            while($record = db_fetch_array($courses)) {
                //Find teacher info
                if (empty($teacher)) {
                    $teacher_rec = get_user_detail_($record['teacher1_uid']);
                    if ($teacher_rec) {
                        $teacher = $teacher_rec->first_name.' '.$teacher_rec->last_name;
                }
                    if (intval($record['teacher2_uid']) > 0 && $record['teacher1_uid'] != $record['teacher2_uid']) {
                        $teacher_rec2 = get_user_detail_($record['teacher2_uid']);
                        if ($teacher_rec2)
                            $teacher2 = $teacher_rec2->first_name.' '.$teacher_rec2->last_name;
                    }
                }
                //Reset school outcome array
                $outcomes = array();
                for($oc=0; $oc < sizeof($school_oc); $oc++) {
                    if (!empty($school_oc[$oc])) {
                        $outcomes[$school_oc[$oc]] = 0;
                    }
                }
                $outcomesResult = get_outcome_student($course_id, $school_year, $term_id, $record['uid'], true);
                while( $out = db_fetch_object($outcomesResult) ) {
                    if ($out->grade > 100) $gv = 100; else $gv = $out->grade;
                    $gv = round($gv, $school->grade_rounding);
                    if (isset($outcomes[$out->name])) {         //Add only School wide outcomes
                        $outcomes[$out->name] += (float)$gv;
                    }
                }
                $grade_percent = _get_student_course_percentage( $record['uid'], $course_id, $term_id);
                $grade_letter = _get_course_grade($grade_percent, $course_id, "L");
                $grade_percent = format_course_grade_percent($grade_percent, $course_id);
//                watchdog('expg_info', $record['uid']."/$course_id/$term_id");

//                watchdog('expg_info', 'school_term '.print_r($outcomes, true));
                $outcomes = implode(',', $outcomes);
                $csv_output .= $course_id.','.$record['title'].','.$record['teacher1_uid'].','.$teacher.','.$record['teacher2_uid'].','.$teacher2.','.$record['user_code'].','.$record['first_name'].','.$record['middle_name'].','.$record['last_name'].','.$grade_letter.','.$grade_percent.','.$term_name.','.$outcomes;
                $csv_output .= "\n";
            }
        }
    }
    drupal_set_header('Content-Type: text/csv');
    drupal_set_header('Content-disposition: attachment; filename="'. date("Y-m-d").'.csv"');
    drupal_set_header("Cache-Control: no-store");
    drupal_set_header('Pragma: public');
    print $csv_output;
}


function verify_database_health() {
    global $base_path;
    
    // Init cURL
    $url = 'http://ntnetwork.prod.acquia-sites.com'.$base_path.'db_monitor.php?type=json';
    $c = curl_init($url);
    curl_setopt($c, CURLOPT_HEADER, 0);
    curl_setopt($c, CURLOPT_RETURNTRANSFER, 1);      // We need to process the string results so disable direct output
    curl_setopt($c, CURLOPT_FOLLOWLOCATION, 1);      // Follow redirects

    $output = curl_exec($c); // Get it
    $info = curl_getinfo($c);

    if ($output === false || $info['http_code'] != 200) {
        $output = "No cURL data returned for $url [". $info['http_code']. "]";
        if (curl_error($c))
            $output .= "<br>". curl_error($c);
        }
    else {
	$joutput = json_decode($output);
        watchdog('db_monitor_json', 'Connect:' . $joutput->connect);

        if (intval($joutput->connect) != 1) {
            drupal_goto('ntlp/render/error/page/Server busy, please try again later.', array('destination' => $_GET['q']));
        }
    }	    
    curl_close($c); // Close the curl stream

    watchdog('db_monitor', 'URL:'.$url.'<br>'.print_r($info, true).'<br>Output:'.print_r($output, true));
}