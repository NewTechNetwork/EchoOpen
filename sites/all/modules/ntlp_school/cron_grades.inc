<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');

/**
 * Implementation of hook_cron().
 */
// Get all courses and generate their enrollments and grades in DW table
function ntlp_school_cron($school_nid = 0, $quick = false) {
    if (!$quick) {
        //Run enrollment first
        ntlp_enrollment_cron($school_nid);
    }

    $where_clause = '';
    if ($school_nid > 0) {
        $where_clause = ' AND nc.school_nid = '.$school_nid;
    }
    //Get active courses
    $crs = db_query("SELECT nc.nid as course_nid, nst.school_term_tid, nc.school_nid
            FROM dpl_ntlp_school_term nst
            INNER JOIN dpl_ntlp_course_term nct ON nct.school_term_tid = nst.school_term_tid
            INNER JOIN dpl_ntlp_course nc ON nc.nid = nct.course_nid
            INNER JOIN dpl_term_data td ON td.tid = nct.school_term_tid
            WHERE nc.is_project <> 1
            %s", $where_clause);

//    echo '<br>Total Courses:'.$crs->num_rows;
    $i = 0;
    while ($crec = db_fetch_object($crs)) {
        //Insert student records
//        if ($school_nid > 0) {
//            dw_setall_student_course_grade_toupdate($crec->course_nid, $crec->school_term_tid, $crec->school_nid);
//        } else {
            dw_setall_student_course_grade_toupdate($crec->course_nid, $crec->school_term_tid, $crec->school_nid, 0, false);
//        }
        $i += cron_course_student_grades_dw($crec->course_nid, $crec->school_term_tid);
//        echo '<br>Course Processed:'.$i;
//        $i++;
        if ($i > 50000) break;
    }
    watchdog('cron', 'Grade calculation completed for '.$i.' records.');
//    echo 'Grade calculation completed for '.$i.' records.';
}

// Get all courses and generate their pending enrollments and grades in DW table
function ntlp_enrollment_cron($school_nid = 0) {
    $where_clause = '';
    if ($school_nid > 0) {
        $where_clause = ' AND nc.school_nid = '.$school_nid;
    }
    //Get active courses
    //5-12-2011: Include dropped users
    $crs = db_query("SELECT nc.nid as course_nid, nst.school_term_tid, nc.school_nid,
            (select count(*) from dpl_ntlp_course_user cu1, dpl_users u1 where cu1.user_uid=u1.uid and (cu1.role_id = 5 OR cu1.role_id IS NULL) and nc.nid = cu1.course_nid) as course_users,
            (select count(*) from dpl_ntlp_dw_course_grade cg2, dpl_users u2 where cg2.uid=u2.uid and nc.nid = cg2.course_nid and nst.school_term_tid = cg2.school_term_tid) as dw_users
            FROM dpl_ntlp_school_term nst
            INNER JOIN dpl_ntlp_course_term nct ON nct.school_term_tid = nst.school_term_tid
            INNER JOIN dpl_ntlp_course nc ON nc.nid = nct.course_nid
            INNER JOIN dpl_term_data td ON td.tid = nct.school_term_tid
            WHERE nc.is_project <> 1
            %s
            having course_users <> dw_users", $where_clause);

//    echo '<br>Total Courses:'.$crs->num_rows;
    $i = 0;
    while ($crec = db_fetch_object($crs)) {
        //Insert student records
//        if ($school_nid > 0) {
//            dw_setall_student_course_grade_toupdate($crec->course_nid, $crec->school_term_tid, $crec->school_nid);
//        } else {
            dw_setall_student_course_grade_toupdate($crec->course_nid, $crec->school_term_tid, $crec->school_nid, 1);
//        }
        $i += cron_course_student_grades_dw($crec->course_nid, $crec->school_term_tid);
//        echo '<br>Course Processed:'.$i;
//        $i++;
        if ($i > 50000) break;
    }
    watchdog('cron', 'Enrollment+Grade calculation completed for '.$i.' records.');
//    echo '<br>Enrollment+Grade calculation completed for '.$i.' records.';
}

//---------------------------------------------------------------------------------
function cron_course_student_grades_dw($course_nid, $school_term_tid) {
    global $base_path;

    //Get all records which are invalid and requires updation.
//    watchdog('grade_cron', "load toupdates for:$course_nid, $school_term_tid");
    $dwrs = dw_get_course_grade_toupdate_foracourse($course_nid, $school_term_tid);  //dw_get_course_grade_toupdate();
    $rec_count = db_affected_rows($dwrs);
    while($dw = db_fetch_array($dwrs)) {
        //Get course outcomes, so that the data query function get values for these outcomes
        $cols = get_course_outcomes($dw['course_nid']);
        $outcomes = array();
        $oc_weight = array();
        while ($col = db_fetch_object($cols)) {
            $outcomes[$col->tid] = $col->tid;
            $oc_weight[$col->tid] = $col->weight_percent;
        }
        //Call data query function, which calculates outcome wise grades
        $datars = getall_course_term_grades($dw['uid'], $dw['course_nid'], $dw['school_term_tid'], $outcomes);
        $sql_oc = '';
        if ($data = db_fetch_array($datars)) {
            $grade = _get_student_grade_by_grad_year($data['grad_year']);
            $percent = _get_student_course_percentage( $dw['uid'], $dw['course_nid'], $dw['school_term_tid'] );


            if ($percent!="*"){//JR change it

            $grade_letter = _get_course_grade($percent, $dw['course_nid'], 'L');
            $grade_percent = format_course_grade_percent($percent, $dw['course_nid'],$grade_letter);
            }
            else
            {
                $grade_letter ="INC";
                $grade_percent="*";
            }

            reset($outcomes);
            $ocno = 1;
            while (list($key, $val) = each($outcomes)) {
                $sql_oc .= ", outcome_tid_$ocno = ".$key;
                $sql_oc .= ", outcome_weight_$ocno = ".$oc_weight[$key];
                if (isset($data['outcome_'.$key])) {
                    $sql_oc .= ", outcome_value_$ocno = ".$data['outcome_'.$key];
                } else {
                    $sql_oc .= ", outcome_value_$ocno = null";
                }
                $ocno ++;
            }
            $sql = "UPDATE {ntlp_dw_course_grade}
                SET is_updated = 1, fullname = '%s', period = %d, grad_year = %d, grade_letter = '%s', grade_percent = %f
                $sql_oc
                WHERE uid = %d AND course_nid = %d AND school_term_tid = %d";

            db_query($sql, $data['fullname'], $data['period'], $grade, $grade_letter, $grade_percent,
                    $dw['uid'], $dw['course_nid'], $dw['school_term_tid']);
        }
    }
    return $rec_count;
}


function getall_course_term_grades($student_uid, $course_nid, $school_term_tid, $outcomes) {

    //THE SQL BELOW NEEDS TO BE IN ONE LINE, AS str_replace FUNCTION INSERTS \n CHARS
//    $sql_oc = "(SELECT (sum(g.points)/sum(x.max_points))*100 FROM dpl_ntlp_gb_activity_grade g, dpl_ntlp_activity_submission s, dpl_ntlp_gb_activity_outcome x, dpl_ntlp_course_activity a WHERE g.submission_nid = s.nid AND g.tid = x.tid AND s.activity_nid = x.activity_nid AND s.activity_nid = a.nid AND s.is_graded <> 0 AND s.is_complete <> 0 AND a.course_nid = @COURSE AND s.user_uid = users.uid AND g.tid = @TERM_TID AND x.tid = @TERM_TID) AS @FIELDNAME";
    $sql_oc = "(SELECT (sum(g.points)/sum(x.max_points))*100 FROM dpl_ntlp_gb_activity_grade g, dpl_ntlp_gb_grade s, dpl_ntlp_gb_activity_outcome x, dpl_ntlp_course_activity a, dpl_ntlp_activity_user au WHERE g.grade_id = s.id AND g.tid = x.tid AND s.activity_nid = x.activity_nid AND s.activity_nid = a.nid AND a.is_published = 1 AND a.course_nid = @COURSE AND s.user_uid = users.uid AND au.activity_nid = a.nid AND au.user_uid = users.uid AND g.tid = @OC_TID AND x.tid = @OC_TID AND a.school_term_tid = @TERM_TID) AS @FIELDNAME";
    $sql_oc = str_replace('@COURSE', $course_nid, $sql_oc);
//    $sql_oc = str_replace('@ROLE', NTLP_ROLEID_STUDENT, $sql_oc);
    $ocfields = "";
    reset($outcomes);
    while (list($key, $val) = each($outcomes)) {
        $field = $sql_oc;
        $field = str_replace('@FIELDNAME', 'outcome_'.$key, $field);
        $field = str_replace('@OC_TID', $key, $field);
        $field = str_replace('@OC_TID', $key, $field);
        $field = str_replace('@TERM_TID', $school_term_tid, $field);
        $ocfields .= ", ".$field;
    }

    //5-12-2011: Include dropped users
    $sql = "/* SLAVE */SELECT  users.uid AS uid, ntlp_course_user.period AS period,
           ntlp_user.uid AS ntlp_user_uid, ntlp_user.grad_year,
           concat(ntlp_user.last_name, ', ', ntlp_user.first_name) AS fullname
           %s
         FROM dpl_users users
         LEFT JOIN dpl_ntlp_user ntlp_user ON users.uid = ntlp_user.uid
         INNER JOIN dpl_ntlp_course_user ntlp_course_user
            ON users.uid = ntlp_course_user.user_uid AND ntlp_course_user.course_nid = %d
         WHERE ntlp_user.uid = %d AND (ntlp_course_user.role_id = %d OR ntlp_course_user.role_id IS NULL)";
//    drupal_set_message("$student_uid, $course_nid, $school_term_tid, $outcomes");
    //drupal_set_message($sql);
    $rs = db_query($sql, $ocfields, $course_nid, $student_uid, NTLP_ROLEID_STUDENT);
    return $rs;
}


//-----------------------------------------------------------------------------
//Functions for TRIGGER POINTS
//-----------------------------------------------------------------------------
function dw_get_course_grade_toupdate() {
    $rs = db_query("SELECT *
        FROM {ntlp_dw_course_grade}
        WHERE is_updated = 0");
    return $rs;
}
function dw_get_course_grade_toupdate_foracourse($course_nid, $school_term_tid) {
    $rs = db_query("SELECT *
        FROM {ntlp_dw_course_grade}
        WHERE is_updated = 0 AND course_nid = %d AND school_term_tid = %d",
        $course_nid, $school_term_tid);
    return $rs;
}

//Deletes all records of all terms in a course.
function dw_set_course_grade_todelete($course_nid) {
    $rs = db_query("DELETE FROM {ntlp_dw_course_grade}
        WHERE course_nid = %d", $course_nid);
}

//Deletes and reinserts student record of all terms in a course.
function dw_set_student_course_grade_todelete($student_uid, $course_nid) {
    $rs = db_query("DELETE FROM {ntlp_dw_course_grade}
        WHERE course_nid = %d AND uid = %d",
            $course_nid, $student_uid);
}
//Multiple students
function dw_set_students_course_grade_todelete($student_uids, $course_nid) {
    $rs = db_query("DELETE FROM {ntlp_dw_course_grade}
        WHERE course_nid = %d AND uid IN (%s)",
            $course_nid, $student_uids);
}

//Deletes and reinserts student record of all terms in a course.
function dw_set_student_course_grade_toinsert($student_uid, $course_nid) {
    dw_set_student_course_grade_todelete($student_uid, $course_nid);

    $school_nid = get_course_school_nid($course_nid);
    $ctrs = get_course_terms($course_nid);
    while( $ct = db_fetch_object($ctrs) ) {
        $school_term_tid = $ct->tid;
        $rs = db_query("INSERT INTO {ntlp_dw_course_grade}
                (SELECT cu.user_uid as uid, concat(u.last_name, ', ', u.first_name) AS fullname, cu.course_nid, 1, cu.period, 0, %d, %d, 0,
                    null, null,
                    null, null, null,  null, null, null,  null, null, null,  null, null, null,  null, null, null,
                    null, null, null,  null, null, null,  null, null, null,  null, null, null,  null, null, null,
                    null, null, null,  null, null, null,  null, null, null,  null, null, null,  null, null, null,
                    null, null, null,  null, null, null,  null, null, null,  null, null, null,  null, null, null
                    FROM {ntlp_course_user} cu, {ntlp_user} u 
                    WHERE cu.user_uid = u.uid AND cu.course_nid = %d AND cu.user_uid = %d)",
                $school_term_tid, $school_nid, $course_nid, $student_uid);
    }
}

//Deletes and reinserts student record of all terms in a course.
function dw_set_student_course_grade_todrop($student_uid, $course_nid) {
    $rs = db_query("UPDATE {ntlp_dw_course_grade} SET is_enrolled = 0
        WHERE course_nid = %d AND uid = %d",
            $course_nid, $student_uid);
}

function dw_set_student_course_grade_toupdate($student_uid, $course_nid, $school_term_tid = 0) {
    if ($school_term_tid == 0) {
    $rs = db_query("UPDATE {ntlp_dw_course_grade} SET is_updated = 0
            WHERE uid = %d AND course_nid = %d",
            $student_uid, $course_nid);
    } else {
        $rs = db_query("UPDATE {ntlp_dw_course_grade} SET is_updated = 0
        WHERE uid = %d AND course_nid = %d AND school_term_tid = %d",
            $student_uid, $course_nid, $school_term_tid);
    }
    return $rs;
}
function dw_set_allstudents_course_grade_toupdate($course_nid, $school_term_tid) {
    $rs = db_query("UPDATE {ntlp_dw_course_grade} SET is_updated = 0
        WHERE course_nid = %d AND school_term_tid = %d",
            $course_nid, $school_term_tid);
    return $rs;
}
function dw_set_allterms_course_grade_toupdate($course_nid) {
    $rs = db_query("UPDATE {ntlp_dw_course_grade} SET is_updated = 0
        WHERE course_nid = %d", $course_nid);
    return $rs;
}
function dw_set_students_all_course_grade_toupdate($student_uid, $multiple = 0) {
    if ($multiple == 0) {
        $rs = db_query("UPDATE {ntlp_dw_course_grade} SET is_updated = 0 WHERE uid = %d", $student_uid);
    } else {
        $rs = db_query("UPDATE {ntlp_dw_course_grade} SET is_updated = 0 WHERE uid IN (".implode(',', $student_uid).')');
    }
    return $rs;
}

function dw_setall_student_course_grade_toupdate($course_nid, $school_term_tid, $school_nid, $force_reset = 0, $update=true) {
    $rs = db_query("SELECT uid FROM {ntlp_dw_course_grade}
        WHERE course_nid = %d AND school_term_tid = %d",
        $course_nid, $school_term_tid);

    if ($rs->num_rows == 0 || $force_reset == 1) {
//        watchdog('grade_cron', "reinsert for:$course_nid, $school_term_tid, $school_nid");
        $rs = db_query("DELETE FROM {ntlp_dw_course_grade}
            WHERE course_nid = %d AND school_term_tid = %d",
                $course_nid, $school_term_tid);

        $rs = db_query("SELECT count(*) FROM {ntlp_dw_course_grade}
            WHERE course_nid = %d AND school_term_tid = %d",
            $course_nid, $school_term_tid);

        $rs = db_query("INSERT INTO {ntlp_dw_course_grade}
                (SELECT cu.user_uid as uid, concat(u.last_name, ', ', u.first_name) AS fullname, cu.course_nid, 1, cu.period, 0, %d, %d, 0,
                    null, null,
                    null, null, null,  null, null, null,  null, null, null,  null, null, null,  null, null, null,
                    null, null, null,  null, null, null,  null, null, null,  null, null, null,  null, null, null,
                    null, null, null,  null, null, null,  null, null, null,  null, null, null,  null, null, null,
                    null, null, null,  null, null, null,  null, null, null,  null, null, null,  null, null, null
                    FROM {ntlp_course_user} cu, {ntlp_user} u 
                    WHERE cu.user_uid = u.uid AND cu.course_nid = %d AND cu.role_id = %d)",
                $school_term_tid, $school_nid, $course_nid, NTLP_ROLEID_STUDENT);

        //Add Dropped students
        $rs = db_query("INSERT INTO {ntlp_dw_course_grade}
                (SELECT cu.user_uid as uid, concat(u.last_name, ', ', u.first_name) AS fullname, cu.course_nid, 0, cu.period, 0, %d, %d, 0,
                    null, null,
                    null, null, null,  null, null, null,  null, null, null,  null, null, null,  null, null, null,
                    null, null, null,  null, null, null,  null, null, null,  null, null, null,  null, null, null,
                    null, null, null,  null, null, null,  null, null, null,  null, null, null,  null, null, null,
                    null, null, null,  null, null, null,  null, null, null,  null, null, null,  null, null, null
                    FROM {ntlp_course_user} cu, {ntlp_user} u
                    WHERE cu.user_uid = u.uid AND cu.course_nid = %d AND cu.role_id IS NULL)",
                $school_term_tid, $school_nid, $course_nid);
        return $rs;
    } else {
        if ($update) {
//            watchdog('grade_cron', "reset flag for:$course_nid, $school_term_tid, $school_nid");
            //Reset the flag for current course and term
            dw_set_allstudents_course_grade_toupdate($course_nid, $school_term_tid);
        }
    }
}

function dwgetview_grades_stu($course_nid, $term_tid, $outcome_count, $orderby = NULL, $filters = array()) {

    //THE SQL BELOW NEEDS TO BE IN ONE LINE, AS str_replace FUNCTION INSERTS \n CHARS
    $ocno = 1;
    $ocfields = "";
    for ($ocno = 1; $ocno <= $outcome_count; $ocno++) {
        $ocfields .= ", outcome_value_$ocno";
    }

    $is_enrolled = 1;

    if ($filters) {
        $is_enrolled = $filters['ENROLLED'];
        
        $percent_filters = $filters['GRADE_YEARS']['percent'];
        $PERCENTAGE_FILTER = '';
        if (!empty($percent_filters)) {

            $flag = true;
            $PERCENTAGE_FILTER = " AND (";
            foreach ($percent_filters as $percent_filter) {

                $user_percent = explode('-', $percent_filter);
                $max_percent = $user_percent[0];
                $min_percent = $user_percent[1];

                if ($max_percent == 100) $max_percent = 10000;
                if ($flag) {
                    $PERCENTAGE_FILTER .= " dw.grade_percent >= " . $min_percent . " AND dw.grade_percent < " . ($max_percent+1);
                    $flag = false;
                } else {
                    $PERCENTAGE_FILTER .= " OR dw.grade_percent >= " . $min_percent . " AND dw.grade_percent < " . ($max_percent+1);
                }
            }
            $PERCENTAGE_FILTER .= ") ";
        }
    }

    //Default filter
    $ENROLL_FILTER = " AND dw.is_enrolled = ";
    if ($is_enrolled == 1) {
        $ENROLL_FILTER .= "1 ";
    } else {
        $ENROLL_FILTER .= "0 ";
    }

    $sql = "SELECT dw.uid AS uid, dw.period, dw.grad_year, dw.course_nid, dw.school_term_tid, dw.school_nid,
           dw.fullname, dw.grade_letter, dw.grade_percent
           %s
         FROM {ntlp_dw_course_grade} dw
         WHERE dw.course_nid = %d AND dw.school_term_tid = %d" . $PERCENTAGE_FILTER . $ENROLL_FILTER;
    if (empty($orderby)) {
        $sql .= " ORDER BY fullname ASC";
    } else {
        $sql .= " ORDER BY $orderby, fullname ASC";
    }
    watchdog("dwgetview_grades_stu", $sql);
    $rs = db_query($sql, $ocfields, $course_nid, $term_tid );
    return $rs;
}
