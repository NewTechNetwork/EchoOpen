<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


//Aggregate of grades earned in each outcome in all activities for a student
//$student_uid == NULL, then show aggregated grades of all students
function get_outcome_student($course_nid, $school_year, $term_tid = NULL, $student_uid = NULL, $school_wide_only=false) {
    $course_nid = (int) $course_nid;
    if ($school_wide_only)
        $oc_join = 'INNER';
    else
        $oc_join = 'LEFT';

    $sql = '/* SLAVE */SELECT outcome.tid, outcome.name,
            (SELECT (sum(g.points)/sum(x.max_points))*100
                FROM {ntlp_gb_activity_grade} g, {ntlp_gb_grade} gb,
                    {ntlp_gb_activity_outcome} x, {ntlp_course_activity} a,
                    {ntlp_activity_user} au, {ntlp_course_user} cu
                WHERE g.grade_id = gb.id
                    AND a.is_published = 1
                    AND gb.activity_nid = x.activity_nid
                    AND gb.activity_nid = a.nid
                    AND au.activity_nid = a.nid
                    AND au.user_uid = gb.user_uid
                    AND cu.course_nid = a.course_nid
                    AND cu.user_uid = gb.user_uid
                    ' . ($term_tid ? 'AND a.school_term_tid = %d ' : '');
    $sql .= $student_uid == NULL ? " AND cu.role_id = %d " : "
                    AND (cu.role_id = %d OR cu.role_id IS NULL)
                    AND gb.user_uid = %d ";
    $sql .= '       AND a.course_nid = %d
                    AND g.tid = outcome.tid AND x.tid = outcome.tid) grade
            FROM {ntlp_gb_course_outcome} course
                INNER JOIN {term_data} outcome ON outcome.tid = course.tid
                %s JOIN {ntlp_gb_outcome} gb_outcome ON gb_outcome.tid = outcome.tid AND gb_outcome.school_year = %d
            WHERE course.course_nid = %d
            ORDER BY gb_outcome.sort_order, outcome.name';

//    watchdog('sql', $sql);
    if ($student_uid == NULL) {
        if ($term_tid)
            $rs = db_query($sql, intval($term_tid), NTLP_ROLEID_STUDENT, intval($course_nid), $oc_join, intval($school_year), intval($course_nid));
        else
            $rs = db_query($sql, NTLP_ROLEID_STUDENT, $course_nid, $oc_join, intval($school_year), $course_nid);
    } else {
        if ($term_tid)
            $rs = db_query($sql, intval($term_tid), NTLP_ROLEID_STUDENT, intval($student_uid), $course_nid, $oc_join, intval($school_year), $course_nid);
        else
            $rs = db_query($sql, NTLP_ROLEID_STUDENT, intval($student_uid), $course_nid, $oc_join, intval($school_year), $course_nid);
    }

    return $rs;
}

//Aggregate of grades earned in each outcome in all activities for a student in ALL courses in current school year/term
function get_outcome_student_schoolwide($school_nid, $school_year, $term_tid, $student_uid) {

    $sql = '/* SLAVE */SELECT outcome.tid, outcome.name,
            IFNULL(FORMAT((SELECT (sum(g.points)/sum(x.max_points))*100
                FROM {ntlp_gb_activity_grade} g, {ntlp_gb_grade} gb,
                    {ntlp_gb_activity_outcome} x, {ntlp_course_activity} a,
                    {ntlp_activity_user} au, {ntlp_course_user} cu
                WHERE g.grade_id = gb.id
                    AND a.is_published = 1
                    AND gb.activity_nid = x.activity_nid
                    AND gb.activity_nid = a.nid
                    AND au.activity_nid = a.nid
                    AND au.user_uid = gb.user_uid
                    AND cu.course_nid = a.course_nid
                    AND cu.user_uid = gb.user_uid
                    AND a.school_term_tid = %d
                    AND cu.role_id = %d
                    AND gb.user_uid = %d
                    AND g.tid = outcome.tid AND x.tid = outcome.tid), 1), "*") grade
            FROM {term_data} outcome
            INNER JOIN {ntlp_gb_outcome} gb_outcome ON gb_outcome.tid = outcome.tid AND gb_outcome.school_year = %d
            WHERE gb_outcome.school_nid = %d
            ORDER BY gb_outcome.sort_order, outcome.name';

    $rs = db_query($sql, intval($term_tid), NTLP_ROLEID_STUDENT, intval($student_uid), intval($school_year), $school_nid);

    return $rs;
}

function get_semester_courses($user_uid, $head) {
    $rs = db_query("SELECT r.course_nid, n.title, c.course_short_title, c.period, td.name
            FROM dpl_node n
            INNER JOIN {ntlp_course} c ON n.nid = c.nid
            INNER JOIN {ntlp_course_user} r ON r.course_nid = c.nid
	    INNER JOIN {ntlp_course_term} t ON n.nid = t.course_nid
	    INNER JOIN {term_data} td ON t.school_term_tid = td.tid
            WHERE c.is_project = 0 AND r.role_id = 6 AND r.user_uid = %d" . tablesort_sql($head),
                    $user_uid);

    return $rs;
}

function get_school_users($school_id) {

    $rs = db_query("SELECT u.* FROM {ntlp_user} nu
                    INNER JOIN {users} u ON nu.uid =  u.uid
                    WHERE school_nid = %d", $school_id);
    return $rs;
}

//function get_school_term($school_id) {
//
//    $rs = db_query("select td.tid,td.name
//            from {ntlp_school_term} st left join {term_data} td on st.tid = td.tid
//            where st.school_nid=%d",$school_id);
//
//    while ($note = db_fetch_array($rs)) {
//        $school_term[$note['tid']] = $note['name'];
//    }
//    return $school_term;
//}
//function get_school_users($school_nid){
//    $rs = db_query ("SELECT u.first_name,u.last_name FROM {ntlp_user} u
//        INNER JOIN  (ntlp_course_user) cu ON cu.user_uid = u.uid
//        INNER JOIN {ntlp_course} c ON  ")
//
//}

function get_users_for_school($school_nid, $head, $limit, $filters = array()) {
    $GRADE_YEAR_FILTER = '';
//    print_r($filters['GRADE_YEARS']['percent']);

    if (isset($filters['GRADE_YEARS'])) {

        if (!is_array($filters['GRADE_YEARS']['year'])) {
            $filters['GRADE_YEARS']['year'] = array($filters['GRADE_YEARS']['year']);
        }

        if (!empty($filters['GRADE_YEARS']['year'][0])) {
            $filters['GRADE_YEARS']['year'] = array_unique($filters['GRADE_YEARS']['year']);
            $total_years = count($filters['GRADE_YEARS']['year']);

            if ($total_years > 0) {
                for ($i = 0; $i < $total_years; $i++) { // just for security purpose
                    $filters['GRADE_YEARS']['year'][$i] = (int) $filters['GRADE_YEARS']['year'][$i];
                }

                $GRADE_YEAR_FILTER .= ' AND nu.grad_year IN (' . implode(',', $filters['GRADE_YEARS']['year']) . ')';
            }
        }
    }

    $percent_filters = $filters['GRADE_YEARS']['percent'];
     $PERCENTAGE_FILTER = '';
    if (!empty($percent_filters)) {
        
        $flag = true;
        $PERCENTAGE_FILTER = "AND (";
        foreach ($percent_filters as $percent_filter) {

            $user_percent = explode('-', $percent_filter);
            $max_percent = $user_percent[0];
            $min_percent = $user_percent[1];

            if ($flag) {
                $PERCENTAGE_FILTER .= " dw.grade_percent BETWEEN " . $min_percent . " AND " . ($max_percent+1);
                $flag = false;
            } else {
                $PERCENTAGE_FILTER .= " OR dw.grade_percent BETWEEN " . $min_percent . " AND " . ($max_percent+1);
            }
        }
        $PERCENTAGE_FILTER .= ")";
    }
    if (isset($filters['BY_NAME'])) {
        $byName = check_plain($filters['BY_NAME']);
        $GRADE_YEAR_FILTER .= " AND (nu.first_name LIKE '%%$byName%%' OR nu.last_name LIKE '%%$byName%%' OR CONCAT(nu.first_name, ' ', nu.last_name) LIKE '%%$byName%%') ";
    }
//
//    if (isset($filters['BY_SCHOOL_YEAR']) and $filters['BY_SCHOOL_YEAR']) {
//
//        $sql = 'SELECT DISTINCT(u.uid), u.picture, nu.grad_year, u.name, u.mail, nu.first_name, nu.last_name
//                FROM dpl_ntlp_course nc INNER JOIN dpl_ntlp_course_user ncu ON ncu.course_nid = nc.nid INNER JOIN dpl_users u ON u.uid = ncu.user_uid INNER JOIN dpl_ntlp_user nu ON nu.uid = u.uid WHERE school_year = ' . get_this_school_year() . '
//                    ' . $where . '
//                    AND nc.school_nid = ' . $school_nid . '
//                    AND nc.is_project <> 1
//                    AND ncu.role_id = ' . NTLP_ROLEID_STUDENT;
//
//        $count_query = 'SELECT COUNT(DISTINCT(u.uid))
//                        FROM dpl_ntlp_course nc INNER JOIN dpl_ntlp_course_user ncu ON ncu.course_nid = nc.nid INNER JOIN dpl_users u ON u.uid = ncu.user_uid INNER JOIN dpl_ntlp_user nu ON nu.uid = u.uid WHERE school_year = ' . get_this_school_year() . '
//                            ' . $where . '
//                            AND nc.school_nid = ' . $school_nid . '
//                            AND nc.is_project <> 1
//                            AND ncu.role_id = ' . NTLP_ROLEID_STUDENT;
//    } else {
//        $sql = "SELECT r.uid,u.picture, nu.grad_year, u.uid, u.name, u.mail, nu.first_name, nu.last_name
//                FROM {users} u
//                INNER JOIN {ntlp_user} nu ON nu.uid = u.uid
//                INNER JOIN {users_roles} r ON nu.uid = r.uid
//                WHERE  nu.school_nid = " . $school_nid . " $where AND r.rid =" . NTLP_ROLEID_STUDENT;
//        $count_query = NULL;
//    }
    $order_by = (isset($_GET['order']) || isset($_GET['sort'])) ? tablesort_sql($head) : ' ORDER BY nu.last_name, nu.first_name ';
//    echo $sql;


    $sql = "SELECT DISTINCT dw.uid, nu.last_name, nu.first_name
        FROM dpl_ntlp_dw_course_grade dw
        INNER JOIN dpl_ntlp_user nu ON nu.uid = dw.uid 
        WHERE dw.school_nid = ".$school_nid. " " . $PERCENTAGE_FILTER . "
        AND dw.is_enrolled = 1 AND dw.school_term_tid = " . get_current_selected_term() . "".$GRADE_YEAR_FILTER." ORDER BY nu.last_name, nu.first_name";

//    echo $sql;
    $res = pager_query($sql, $limit);
    return $res;
}

function get_user_term_courses($user_uid, $term_tid, $by_school_year = false, $include_dropped = false) {

    $where = '';
    $where .= $by_school_year ? ' AND nst.school_year = ' . intval(get_this_school_year()) : '';

    if ($include_dropped) {
        $enroll_filter = " OR ncu.role_id IS NULL";
    }
//    watchdog('term_courses', ' user uid '.$user_uid.' term tid '. $term_tid.' school years '.get_this_school_year());
    $rs = db_query("SELECT nct.course_nid,nc.nid, n.title, nc.course_short_title, td.name as period_number, nc.teacher1_uid, nc.teacher2_uid, nc.period, ncu.role_id
                FROM dpl_ntlp_school_term nst
                INNER JOIN {ntlp_course_term} nct ON nct.school_term_tid = nst.school_term_tid
                INNER JOIN {node} n ON n.nid = nct.course_nid
                INNER JOIN {ntlp_course} nc ON nc.nid = n.nid
                INNER JOIN {term_data} td ON td.tid = nct.school_term_tid
                INNER JOIN {ntlp_course_user} ncu ON ncu.course_nid = nc.nid
                WHERE nc.is_project <> 1
                AND ncu.user_uid = %d AND (ncu.role_id = %d %s) AND nct.school_term_tid = %d
                $where
              ORDER BY nc.period DESC, n.title ASC ",
            $user_uid, NTLP_ROLEID_STUDENT, $enroll_filter, $term_tid);

    $rows = array();
    while ($row = db_fetch_object($rs)) {
        $rows[] = $row;
    }

//    watchdog('student_snapshot', print_r($rows, true));
    return $rows;
}

function _get_student_school($student_nid) {
    if ($result = db_query("SELECT school_nid FROM {ntlp_user} WHERE uid = '%d' LIMIT 1", $student_nid)) {
        $student = db_fetch_object($result);
        return $student->school_nid;
    }

    return false;
}

function get_enrolled_students_in_course($course_id) {

    $result = db_query("SELECT cu.*, nu.* FROM {ntlp_course_user} cu
        INNER JOIN {ntlp_user} nu ON nu.uid = cu.user_uid
        WHERE cu.course_nid =%d  AND cu.role_id = 5", $course_id);

    while ($data = db_fetch_object($result)) {
        $enrolled_student[] = $data;
    }

    return $enrolled_student;
}

function get_recent_activities_graded($course_id, $term_tid) {
    $sql = "SELECT g.date_graded FROM {ntlp_gb_grade} g
            INNER JOIN {ntlp_course_activity} nca ON nca.nid = g.activity_nid
            WHERE nca.course_nid = %d
            AND nca.school_term_tid = %d
            ORDER by g.date_graded DESC
            LIMIT 0, 1";

    $rows = array();
    if ($result = db_query($sql, $course_id, $term_tid) and $result->num_rows) {
        while ($row = db_fetch_object($result)) {
            $rows[] = $row;
        }
    }

    return $rows;
}

function get_course_detail($course_nid) {

    $result = db_query("SELECT * FROM {ntlp_course} WHERE nid = %d", $course_nid);
    return db_fetch_object($result);
}

function is_not_graded_course($course_nid){
    $is_not_graded = false;
    $rec = db_query("SELECT is_weighted_grades FROM dpl_ntlp_course WHERE nid  = %d", $course_nid);
    if ($result = db_fetch_object($rec)) {
         if (!($result->is_weighted_grades))
            $is_not_graded = true;
    }

    return $is_not_graded ;
}


function _get_course_grade($percent, $course_nid, $output_format = 'F') {

//    if (is_not_graded_course($course_nid))
//    return '';

    $rec = db_query("SELECT c.school_nid, s.grade_rounding, s.grade_scale, c.school_year
            FROM {ntlp_course} c, {ntlp_school} s
            WHERE c.school_nid = s.nid AND c.nid = %d", $course_nid);
    if ($sch_result = db_fetch_object($rec)) {
        $rndpercent = round($percent, $sch_result->grade_rounding);   //Round so that we can catch fraction values for grade scales comparisons
        $get_grades_from_school = true;
        $result = false;
        if ($sch_result->grade_scale == 1) {        //Allow course to override grade scales
            //Get Course Grade Levels
            $source_nid = $course_nid;
            $sql = ' SELECT * FROM {ntlp_course_grade} cg WHERE cg.course_nid = %d and cg.lower_limit <= %d ORDER BY cg.lower_limit DESC ';
            $result = db_query($sql, $source_nid, $rndpercent);
            if ($result) {
                if ($result->num_rows > 0) {
                    //If grades are not defined at Course level, then get from school
                    $get_grades_from_school = false;
                }
            }
        }
        if ($get_grades_from_school) {
            //Get School Grade Levels
            $source_nid = $sch_result->school_nid;
            $sql = ' SELECT * FROM {ntlp_school_grade} sg WHERE sg.school_nid = %d AND sg.school_year = %d AND sg.lower_limit <= %d ORDER BY sg.lower_limit DESC ';
            $result = db_query($sql, $source_nid, $sch_result->school_year, $rndpercent);
        }
        $grade = '__';

        if ($result) {
            if ($result->num_rows > 0) {
                $row = db_fetch_object($result);
                $grade = $row->label;
            }
        }

        if ($output_format == "F") {    //Full

            if (strtoupper($grade)=="INC")
            $gradeFormat = strtoupper($grade) . '<div class="percent">*</div>';
            else
            $gradeFormat = strtoupper($grade) . '<div class="percent">' . round($percent, $sch_result->grade_rounding) . '%</div>';

            return $gradeFormat ;
            
        } elseif ($output_format == "L") {
            return strtoupper($grade);
        }
    }
    return '__';
}

function format_course_grade_percent($percent, $course_nid, $grade_letter="A") {

//    if (is_not_graded_course($course_nid)){
//    return '';
//    }

    if ($grade_letter=="INC")
        return "*";

    $rec = db_query("SELECT c.school_nid, s.grade_rounding, s.grade_scale
            FROM {ntlp_course} c, {ntlp_school} s
            WHERE c.school_nid = s.nid AND c.nid = %d", $course_nid);
    if ($sch_result = db_fetch_object($rec)) {
        return round($percent, $sch_result->grade_rounding) . '%';
    }
    return FormatNumber($percent, 0, ',') . '%';
}

function _get_students_having_grade_percent($course_nid, $school_term_tid, $percent, $op, $count = false) {

    $select = '*';
    if ($count) {
        $select = 'count(*) total';
    }

    if (in_array(strtolower($op), array('lt', 'gt', 'lte', 'gte', 'not'))) {
        switch ($op) {
            case 'lt':
                $op = '<';
                break;
            case 'gt':
                $op = '>';
                break;
            case 'lte':
                $op = '<=';
                break;
            case 'gte':
                $op = '>=';
                break;
            case 'not':
                $op = '<>';
                break;
        }
    } else {
        $op = '=';
    }

//JR 100925 Qry modified .. < x% calculation
    $sql = 'SELECT %s FROM {ntlp_dw_course_grade}
        WHERE grade_percent %s %f AND course_nid = %d AND school_term_tid = %d AND is_enrolled = 1';

    $rows = array();
    if ($result = db_query($sql, $select, $op, $percent, $course_nid, $school_term_tid)) {
        if ($count) {
            $rows = db_fetch_object($result);
            $rows = $rows->total;
        } else {
            while ($row = db_fetch_object($result)) {
                $rows[] = $row;
            }
        }
    }

    return $rows;
}

//-----------------------------------------
// Calculates overall percentage of grades a student have received in a course in a term
// The grades are calculated by taking into account the weightages defined for each outcome
//---
function _get_student_course_percentage($user_id, $course_nid, $term_tid = NULL) {
    $sql = '/* SLAVE */SELECT
                (sum(g.points)/sum(x.max_points))*100 as percent, x.tid, o.weight_percent, sum(g.points) tot_earned, sum(x.max_points) max_points,
                sum(case when x.max_points > 0 then 1 else 0 end) available
            FROM
                {ntlp_gb_activity_grade} g,
                {ntlp_gb_grade} gb,
                {ntlp_activity_user} au,
                {ntlp_course_user} cu,
                {ntlp_gb_activity_outcome} x,
                {ntlp_course_activity} a,
                {ntlp_gb_course_outcome} o
            WHERE
                g.grade_id = gb.id
                AND g.tid = x.tid
                AND gb.activity_nid = x.activity_nid
                AND gb.activity_nid = a.nid
                AND au.activity_nid = a.nid
                AND au.user_uid = cu.user_uid
                AND cu.course_nid = a.course_nid
                AND cu.user_uid = gb.user_uid
                AND (cu.role_id = %d OR cu.role_id IS NULL)
                ' . ($term_tid ? ' AND a.school_term_tid = ' . $term_tid : '');
    $sql .= ' AND x.tid = o.tid
                AND a.is_published = 1
                AND a.course_nid = o.course_nid
                AND o.course_nid = %d
                AND gb.user_uid = %d
             GROUP BY x.tid
          ';

    if ($result = db_query($sql, NTLP_ROLEID_STUDENT, $course_nid, $user_id)) {
        $totalwt = 0;
        $grade = 0;
        while ($row = db_fetch_object($result)) {
            $rows[] = $row;
//            if ($row->percent > 0) {              // Weights of outcomes with Zero percent should be part of calc
            if ($row->available > 0) {
            $totalwt += $row->weight_percent;
        }
        }
        if ($result->num_rows > 0) {
            foreach ($rows as $outcome) {
//                print isset($outcome->percent) ? "NOT NULL" : "NULL";
//                print '<br>'.$user_id.','.$course_nid.','.$term_tid.'<br>'.print_r($rows, true);
//                if ($outcome->percent > 0) {
                $correct_wt = ($outcome->weight_percent / $totalwt) * 100;
                $ocgrade = $outcome->percent * ($correct_wt / 100);
                $grade += $ocgrade;
//                print '<br>'.$outcome->tid.':'.$outcome->weight_percent.','.$outcome->tot_earned.','.$outcome->max_points.','.$totalwt.','.$correct_wt.','.$ocgrade.','.$grade.'***';
//                }
            }
        } else {
//                print '<br>'.$user_id.','.$course_nid.','.$term_tid.'***';
            $grade = "*";
        }
        return $grade;
    }

    return false;
}

function _get_student_grade_by_grad_year($grad_year) {
    $grades = array(12, 11, 10, 9, 8, 7, 6);

//     $index = $grad_year - date('Y');
    $cy = intval(date('Y'));
    $cm = intval(date('n'));
    if ($cm > 6) $cy += 1;

    $index = $grad_year - $cy;
    if (isset($grades[$index])) {
        return $grades[$index];
    }

    return false;
}

function _get_student_grad_year_by_grade($grade) {
    $cy = intval(date('Y'));
    $cm = intval(date('n'));
    if ($cm > 6) $cy += 1;
    $cy -= 1;
    
    $grad_years = array(
        12 => $cy + 1, //        12 => $cy,
        11 => $cy + 2, //        11 => $cy+1,
        10 => $cy + 3, //        10 => $cy+2,
        9 => $cy + 4, //        9  => $cy+3,
        8 => $cy + 5,
        7 => $cy + 6,
        6 => $cy + 7,
        100 => '100-90', //
        89 => '89-80', //
        79 => '79-70',
        69 => '69-60',
        59 => '59-0',
    );

    if (isset($grad_years[$grade])) {
        return $grad_years[$grade];
    }

    return false;
}


function get_user_period($course_id, $user_id) {

    $rs = db_query("SELECT period FROM dpl_ntlp_course_user WHERE course_nid = %d AND user_uid = %d", $course_id, $user_id);

    return db_fetch_object($rs);
}

function check_users_grade($filter, $percent) {

    if ($filter) {
        foreach ($filter as $percent_filter) {
            $percent = round($percent);             //Round so that we can catch fraction values
            $user_percent = explode('-', $percent_filter);
            $max_percent = str_replace('%', '', $user_percent[0]);
            $min_percent = str_replace('%', '', $user_percent[1]);
//            echo "<br>Min:$min_percent Max:$max_percent Course %:$percent";
            if ($percent <= $max_percent && $percent >= $min_percent) {
//                echo "> Match";
                $flag = true;
                break;
            } else {
                $flag = false;
            }
        }
        return $flag;
    }
}

function _get_filtered_courses_for_each_user($user_uid, $school_nid, $semeter_tid, $filters) {
    $PERCENTAGE_FILTER = '';

    $percent_filters = $filters['GRADE_YEARS']['percent'];
    if ($percent_filters) {
        $PERCENTAGE_FILTER = '';
        $flag = true;
        foreach ($percent_filters as $percent_filter) {

            $user_percent = explode('-', $percent_filter);
            $max_percent = $user_percent[0];
            $min_percent = $user_percent[1];

            if ($flag) {
                $PERCENTAGE_FILTER = "HAVING dw.grade_percent BETWEEN " . $min_percent . " AND " . ($max_percent+1);
                $flag = false;
            } else {
                $PERCENTAGE_FILTER .= " OR dw.grade_percent BETWEEN " . $min_percent . " AND " . ($max_percent+1);
            }
        }
    }

    $sql = "SELECT u.picture, nu.grad_year, c.course_short_title, nu.first_name, nu.last_name, dw.uid, dw.course_nid, dw.period,
        dw.school_term_tid, dw.school_nid, dw.grade_letter, dw.grade_percent
        FROM {ntlp_dw_course_grade} dw
        INNER JOIN {ntlp_user} nu ON nu.uid = dw.uid
        INNER JOIN {users} u ON u.uid = dw.uid
        INNER JOIN {ntlp_course} c ON c.nid = dw.course_nid
        WHERE dw.school_nid = " . $school_nid . " AND dw.is_enrolled = 1
        AND dw.uid = " . $user_uid . "
        AND dw.school_term_tid =" . $semeter_tid . " " . $PERCENTAGE_FILTER;

//drupal_set_message($sql.';');
    $result = db_query($sql);
    if ($result->num_rows > 0) {
        while ($record = db_fetch_object($result)) {
            $data[] = $record;
        }
    }else{
        $data = false;
    }

    return $data;
}

function get_outcome_activities_count($term_tid, $course_id, $outcome_id, $student_id=null)
{
    $sql = "SELECT COUNT(ao.ACTIVITY_nid) AS activity_count FROM {ntlp_gb_activity_outcome} ao
                INNER JOIN {ntlp_course_activity} ca ON ao.activity_nid=ca.nid " .
                (isset($student_id) ? " INNER JOIN {ntlp_activity_user} au ON au.activity_nid=ca.nid " : "") .
            "WHERE ca.is_published=1
            AND ao.tid=$outcome_id
            AND ca.course_nid = $course_id
            AND ca.school_term_tid = $term_tid" .
            (isset($student_id) ? " AND au.user_uid=$student_id " : "");

//    watchdog("activity count", "$sql");
    
    $result = db_query($sql);

    if ($result->num_rows > 0) {
        while ($record = db_fetch_object($result)) {
            $count = $record->activity_count;
        }
    }else{
        $count = 0;
    }

    return $count;
}
function is_activity_user_graded($activity_nid, $user_uid) {
    $result = db_query("SELECT id FROM {ntlp_gb_grade} WHERE activity_nid = %d AND user_uid = %d", $activity_nid, $user_uid);

    if ($result->num_rows > 0) {
        $record = db_fetch_object($result);
        return $record->id;
    }
    return false;
}

function create_activity_user_grade($activity_nid, $user_uid, $teacher_comment, $teacher_uid, $is_complete = 1, $is_late = 0) {
    $dateformat = "%Y-%m-%d %H:%M:%S";
    $time = strftime($dateformat, strtotime(date('Y-m-d')));

    if (!isset($teacher_comment)) {
        $teacher_comment = '';
    }
    if (!isset($is_complete)) {
        $is_complete = 1;
    }
    if (!isset($is_late)) {
        $is_late = 0;
    }
    db_query("INSERT INTO {ntlp_gb_grade} (activity_nid, user_uid, date_graded, graded_by_uid,
        is_complete, is_late, teachers_notes)
        VALUES (%d, %d, '%s', %d, %d, %d, '%s')",
        intval($activity_nid), intval($user_uid), strftime($dateformat, time()), intval($teacher_uid), 
        $is_complete, $is_late, $teacher_comment);

    $grade_id = db_last_insert_id('{ntlp_gb_grade}', 'id');

    return $grade_id;
}

function save_activity_user_grade($activity_nid, $user_uid, $teacher_comment, $teacher_uid, $is_complete = 1, $is_late = 0, $force_delete_grades = false) {
    $dateformat = "%Y-%m-%d %H:%M:%S";
    $time = strftime($dateformat, strtotime(date('Y-m-d')));
//    watchdog('save_grade', "($activity_nid, $user_uid, $teacher_comment, $teacher_uid, $is_complete, $is_late)");
    if ($grade_id = is_activity_user_graded($activity_nid, $user_uid)) {
        if (!isset($teacher_comment)) {
            $teacher_comment = 'teachers_notes';
        } else {
            $teacher_comment = "'" . $teacher_comment . "'";
        }
        if (!isset($is_complete)) {
            $is_complete = 'is_complete';
        }
        if (!isset($is_late)) {
            $is_late = 'is_late';
        }
//        echo "[$grade_id, $activity_nid, $user_uid, $teacher_comment, $teacher_uid, $is_complete, $is_late]";
        db_query("UPDATE {ntlp_gb_grade} SET teachers_notes = {$teacher_comment}, graded_by_uid = %d, date_graded = '%s',
            is_complete = %s, is_late = %s WHERE id = %d",
            $teacher_uid, strftime($dateformat, time()), $is_complete, $is_late, $grade_id);
        
        if ($force_delete_grades) {
            db_query("DELETE FROM {ntlp_gb_activity_grade} WHERE grade_id = %d", $grade_id);

            db_query("DELETE FROM {ntlp_gb_activity_excuse} WHERE grade_id = %d", $grade_id);
        }
    } else {
        $grade_id = create_activity_user_grade($activity_nid, $user_uid, $teacher_comment, $teacher_uid, $is_complete, $is_late);
    }
    
    //update block cache block for TEACHERS of the course
    $actrec = get_activity_record($activity_nid);
    if (isset($actrec->course_nid)) {
        ntlp_update_block_cache('ntlp_course', $actrec->course_nid, NTLP_ROLEID_TEACHER);
    }
    
    return $grade_id;
}

function save_activity_user_grade_scores($grade_id, $outcomes, $lock = true) {
    if (isset($outcomes)) {
//        print "($grade_id, "; print_r($outcomes); print ")";
        db_query('DELETE FROM {ntlp_gb_activity_grade} WHERE grade_id = %d', $grade_id);
        db_query('DELETE FROM {ntlp_gb_activity_excuse} WHERE grade_id = %d', $grade_id);
        while (list($key, $val) = each($outcomes)) {
            if (isset($val)) {
                if (strtoupper($val) == "EX") {
                    db_query("INSERT INTO {ntlp_gb_activity_excuse} (tid, grade_id) VALUES (%d, %d)", $key, $grade_id);
                } else {
                    if (strlen($val) > 0) {
                        db_query("INSERT INTO {ntlp_gb_activity_grade} (tid, grade_id, points) VALUES (%d, %d, %f)",
                            $key, $grade_id, $val);
                    }
                }
//                watchdog('grade_ocv', "[$key, $val],".(isset($val) ? 'Set' : 'Blank').(strlen($val)));
            }
        }
    }
}

function auto_grade_maximum_to_user_submission($grade_id, $outcome_array, $lock = true) {

    if ($outcome_array != null) {
        $outcomes = array();
        foreach ($outcome_array as $outcome_obj) {
            $outcomes[$outcome_obj->tid] = $outcome_obj->max_points;
        }
        save_activity_user_grade_scores($grade_id, $outcomes, $lock);
    }
}

//Called when user pressed the "Save" button in Student Grade-All for an Activity page
//OR from an Auto-Save timer
function ntlp_grade_all_save_activity_scores($grade_id, $outcome_tid, $userinput) {
    $activity_outcome_excused = (strtoupper($userinput) == "EX");

    if ($activity_outcome_excused || $userinput == "-") {
        $userinput = "";
    }

    $result = db_query("SELECT grade_id FROM {ntlp_gb_activity_grade}
                WHERE grade_id = %d AND tid = %d", $grade_id, $outcome_tid);

    if ($result->num_rows > 0) {

        if (isset($userinput) && strlen($userinput) > 0) {
            db_query("UPDATE {ntlp_gb_activity_grade} SET points=%f
                WHERE grade_id = %d AND tid = %d", $userinput, $grade_id, $outcome_tid);
        } else {
            db_query("DELETE FROM {ntlp_gb_activity_grade}
                WHERE grade_id = %d AND tid = %d", $grade_id, $outcome_tid);
        }
    } else {
        if (isset($userinput) && strlen($userinput) > 0) {
            db_query("INSERT INTO {ntlp_gb_activity_grade} (tid, grade_id, points)
                        VALUES (%d, %d, %f)", $outcome_tid, $grade_id, $userinput);
        }
    }

    //remove previous entries if exists of same outcome and submission
    db_query("DELETE FROM {ntlp_gb_activity_excuse}
            WHERE tid = %d AND grade_id = %d", $outcome_tid, $grade_id);

    if ($activity_outcome_excused) {
        db_query("INSERT INTO {ntlp_gb_activity_excuse} (tid, grade_id)
         VALUES(%d, %d)", $outcome_tid, $grade_id);
    }

    return true;
}

function load_graded_outcome_points($grade_id) {

    $result = db_query("SELECT tid, points FROM {ntlp_gb_activity_grade} WHERE grade_id = %d", $grade_id);
    
    $gradedoutcome = array();
    while ($row = db_fetch_object($result)) {
        $gradedoutcome[$row->tid] = $row->points;
    }

    //add/overwrite the excused outcomes where applicable
    $result = db_query("SELECT tid FROM {ntlp_gb_activity_excuse} WHERE grade_id = %d", $grade_id);

    while ($row = db_fetch_object($result)) {
        $gradedoutcome[$row->tid] = "EX";
    }


    return $gradedoutcome;
}


