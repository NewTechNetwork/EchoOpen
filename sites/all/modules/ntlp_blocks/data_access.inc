<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


/*
 * Data query functions
 */

/*
 * Returns total courses count.
 * @param $school_id (int)
 * @param [$filter (array)]
 * @return int
 */

function getCoursesCount($school_id, $filter = array()) {
    $sqlPart = '';
    $sqlJoin = '';
    //below line has to be fixed when gradebook completes
    $sqlPart .= isset($filter['GRADEBOOKS']) ? ' AND {ntlp_course}.graded = ' . ((int) $filter['GRADEBOOKS']) : ' ';
    if (isset($filter['SCHOOLTERM'])) {
        $sqlPart .= ( isset($filter['SCHOOLTERM']) and !empty($filter['SCHOOLTERM'])) ? ' AND ct.school_term_tid = ' . ((int) $filter['SCHOOLTERM']) : ' ';
    }

    if (isset($filter['PROJECTS'])) {
        $sqlPart .= ' AND {ntlp_course}.is_project = ' . ((int) $filter['PROJECTS']);
        $sqlJoin = ' INNER JOIN {ntlp_course_term} ct ON ct.course_nid = {ntlp_course}.course_nid ';
    } else {
        $sqlJoin = ' INNER JOIN {ntlp_course_term} ct ON ct.course_nid = {ntlp_course}.nid ';
    }

    $sqlPart .= isset($filter['ACTIVE']) ? " AND {ntlp_course}.availability = '" . $filter['ACTIVE'] . "'" : " AND {ntlp_course}.availability = 'S'";

    $sql = "SELECT
                    ".(isset($filter['PROJECTS']) ? " COUNT(DISTINCT {ntlp_course}.course_nid) AS total " : " count(*) AS total ")."
            FROM
                    {ntlp_course}
            $sqlJoin
            WHERE
                    {ntlp_course}.school_nid = %d
            {$sqlPart}
            ";
    watchdog('rec_act_c', $sql . ' school id ' . $school_id . ' filter ' . print_r($filter, true));
    $result = db_fetch_object(db_query($sql, $school_id));
    return $result ? $result->total : 0;
}

/*
 * Returns the total count of the courses
 * @param $school_id (int)
 * @param [$school_term_id (int)]
 * @return int
 */

function getCoursesAgendaCount($school_id, $school_term_id = '') {
    $sqlPart = '';
    $sqlJoin = '';

    if (!empty($school_term_id)) {
        $sqlJoin = ' INNER JOIN {ntlp_course_term} ct ON ct.course_nid = {ntlp_course}.nid ';
        $sqlPart = ' AND ct.school_term_tid = ' . ((int) $school_term_id);
    }


    $sql = "SELECT
                    count(*) AS total
            FROM
                    {ntlp_course_agenda}
                    INNER JOIN
                    {ntlp_course}
                    ON
                    {ntlp_course}.nid = {ntlp_course_agenda}.course_nid

            $sqlJoin
            WHERE
                    school_nid = %d
                    AND {ntlp_course}.availability = 'S'
                    AND YEAR(NOW()) = {ntlp_course_agenda}.year
                    AND MONTH(NOW()) = {ntlp_course_agenda}.month
                    AND DAY(NOW()) = {ntlp_course_agenda}.day
            {$sqlPart}
            ";

    watchdog('rec_Agenda_c', $sql . ' school_term_id ' . $school_term_id);
    $result = db_fetch_object(db_query($sql, $school_id));
    return $result ? $result->total : 0;
}

/*
 * This function is used to fetch online activity of a user
 *
 * @param $user_role_id (int)
 * @param [$school_id (int)]
 * @param [$opts array( 'INTERVAL' => 24, 'INTERVAL_PERIOD' => 'HOUR' )]
 *
 * @return object
 */

function getRecentUserActivity($user_role_id, $school_id = '', $opts = array()) {
//    global $user;
//    $school_id = get_user_school($user->uid);

    $defaultOpts = array('INTERVAL' => 24, 'INTERVAL_PERIOD' => 'HOUR');
    $opts = array_merge($defaultOpts, $opts);
//    $sql = "SELECT
//                    count( DISTINCT {activity}.uid ) logged_in_count,
//                    (
//                            SELECT
//                                    COUNT(*)
//                            FROM
//                                    {ntlp_user}
//                                    INNER JOIN
//                                    {users_roles}
//                                    ON
//                                    {ntlp_user}.uid = {users_roles}.uid
//                            WHERE
//                                    {users_roles}.rid = %d
//                                    AND
//                                    {ntlp_user}.school_nid = %d
//
//                    ) AS total_count,
//                    GROUP_CONCAT( DISTINCT {activity}.uid ) AS user_ids
//            FROM
//                    {activity}
//                    INNER JOIN
//                    {users_roles}
//                    ON
//                    {activity}.uid = {users_roles}.uid
//
//                    INNER JOIN
//                    {ntlp_user}
//                    ON
//                    {ntlp_user}.uid = {activity}.uid
//            WHERE
//                    {activity}.created >= UNIX_TIMESTAMP( DATE_SUB( CURRENT_DATE, INTERVAL {$opts['INTERVAL']} {$opts['INTERVAL_PERIOD']} ) )
//                    AND
//                    {users_roles}.rid = %d
//                    AND
//                    {ntlp_user}.school_nid = %d";
    
    $sql = "SELECT COUNT(*) as logged_in_count, (SELECT
                                    COUNT(*) FROM {ntlp_user}
                                    INNER JOIN {users_roles} ON {ntlp_user}.uid = {users_roles}.uid
                                    WHERE {users_roles}.rid = %d AND {ntlp_user}.school_nid = %d
                    ) AS total_count FROM dpl_users u 
    INNER JOIN dpl_users_roles ur ON ur.uid = u.uid
    INNER JOIN dpl_ntlp_user nu ON nu.uid = u.uid
    WHERE u.login >=  UNIX_TIMESTAMP( DATE_SUB( CURRENT_DATE, INTERVAL {$opts['INTERVAL']} {$opts['INTERVAL_PERIOD']} ) )
    AND ur.rid = %d AND nu.school_nid = %d";

    return db_fetch_object(db_query($sql, $user_role_id, $school_id, $user_role_id, $school_id));
}

/*
 * This function is used to fetch active gradebooks
 *
 * @param [$school_id (int)]
 * @param [$opts array( 'MIN_GRADED'=> 20, 'INTERVAL' => 14, 'INTERVAL_PERIOD' => 'DAY' )]
 *
 * @return int
 */

function getActiveGradebook($school_id = '', $opts = array()) {
//    $defaultOpts = array('MIN_GRADED' => 20, 'INTERVAL' => 14, 'INTERVAL_PERIOD' => 'DAY');
//    $opts = array_merge($defaultOpts, $opts);
//    $sql = "SELECT COUNT(*) AS total FROM (
//                SELECT {ntlp_course}.nid, count(*) AS total FROM {ntlp_activity_submission}
//
//                INNER JOIN
//                {ntlp_course_activity}
//                ON
//                {ntlp_course_activity}.nid = {ntlp_activity_submission}.activity_nid
//
//                INNER JOIN
//                {ntlp_course}
//                ON
//                {ntlp_course}.nid = {ntlp_course_activity}.course_nid
//
//                WHERE DATE({ntlp_activity_submission}.date_graded) >= DATE_SUB( CURRENT_DATE, INTERVAL {$opts['INTERVAL']} {$opts['INTERVAL_PERIOD']} )
//                AND
//                {ntlp_course}.school_nid = %d
//
//                GROUP BY
//                    {ntlp_course}.nid
//                HAVING
//                    total >= {$opts['MIN_GRADED']}
//            ) graded_courses
//            ";
    
    if(isset($opts['SCHOOLTERM'])){
        $sqlPart .= ( isset($opts['SCHOOLTERM']) and !empty($opts['SCHOOLTERM'])) ? ' AND ct.school_term_tid = ' . ((int) $opts['SCHOOLTERM']) : ' ';
    }
    
    $sql = "SELECT COUNT(DISTINCT c.nid) AS total 
        FROM {ntlp_course_activity} ca 
	INNER JOIN {ntlp_course} c ON c.nid = ca.course_nid
	INNER JOIN {ntlp_course_term} ct ON ct.course_nid = ca.course_nid
      WHERE DATE( ca.due_date) >= NOW() ".$sqlPart."
          AND c.school_nid = %d";
    
    watchdog('rec_gb_c', $sql . ' school_nid  ' . $school_id . ' opts ' . print_r($opts, true));
    if ($result = db_query($sql, $school_id)) {
        $row = db_fetch_object($result);
        return $row->total;
    }
    return 0;
}

/*
 * Get the kudos
 * @param $school_id (int)
 * @param [$school_term_id (int)]
 * @param [array( 'OFFSET' => 0, 'LIMIT' => 3 )]
 *
 * @return Array (objects)
 */

function getKudos($school_id, $condition) {
//    $recs = array();

    if ($condition == 'Week') {
        $CONDITION = ' AND k.on_date  BETWEEN DATE_ADD(NOW(),INTERVAL 1-DAYOFWEEK(NOW()) DAY) 
            AND DATE_ADD(NOW(),INTERVAL 7-DAYOFWEEK(NOW()) DAY) ';
    } else if ($condition == 'Month') {
        $CONDITION = ' AND DATE_FORMAT(NOW(),"%Y%m")  = DATE_FORMAT(k.on_date,"%Y%m")';
    } else {
        $CONDITION = ' AND ds.current_school_year = DATE_FORMAT(k.on_date,"%Y")';
    }
    $sql = "SELECT  COUNT(DISTINCT k.nid) as kudos_count FROM {ntlp_kudos} k
        INNER JOIN {ntlp_kudos_category} kc ON   kc.nid = k.nid
        INNER JOIN {ntlp_kudos_school_category} ks ON   ks.category_tid = kc.category_tid
        INNER JOIN {ntlp_school} ds ON ds.nid = ks.school_nid  
        WHERE ks.school_nid = %d $CONDITION ";

    $result = db_query($sql, $school_id);
    if ($result->num_rows > 0) {
        $recs = db_fetch_object($result);
    } else {
        $recs = 0;
    }
    return $recs;
}

/*
 * Get Behavours
 * @param $school_id (int)
 * @param [$school_term_id (int)]
 * @param [array( 'OFFSET' => 0, 'LIMIT' => 3 )]
 *
 * @return Array (objects)
 */

function getBehaviour($school_id, $condition) {

    if ($condition == 'Week') {
        $CONDITION = ' AND d.on_date  BETWEEN DATE_ADD(NOW(),INTERVAL 1-DAYOFWEEK(NOW()) DAY) 
            AND DATE_ADD(NOW(),INTERVAL 7-DAYOFWEEK(NOW()) DAY) ';
    } else if ($condition == 'Month') {
        $CONDITION = ' AND DATE_FORMAT(NOW(),"%Y%m")  = DATE_FORMAT(d.on_date,"%Y%m")';
    } else {
        $CONDITION = ' AND s.current_school_year = DATE_FORMAT(d.on_date,"%Y")';
    }

    $sql = "SELECT  COUNT(DISTINCT d.nid) as report_count FROM {ntlp_discpline} d
        INNER JOIN {ntlp_discpline_category} dc ON   dc.nid = d.nid
        INNER JOIN {ntlp_discpline_school_category} ds ON   ds.category_tid = dc.category_tid
        INNER JOIN {ntlp_school} s ON s.nid = ds.school_nid  
        WHERE ds.school_nid = %d";

    $result = db_query($sql, $school_id);
    if ($result->num_rows > 0) {
        $recs = db_fetch_object($result);
    } else {
        $recs = 0;
    }
    return $recs;
}

/*
 * Get school wide courses
 * @param $school_id (int)
 * @return Array (object)
 */

function getSchoolWideCourses($school_id) {
    $recs = array();
    $sql = "SELECT DISTINCT
                    ct.school_term_tid,
                    n.course_short_title,
                    n.period,
                    td.name 
            FROM
                    {ntlp_course} n
                    INNER JOIN {ntlp_course_term} ct ON ct.course_nid = n.nid
                    INNER JOIN {term_data} td ON td.tid = ct.school_term_tid
            WHERE
                    n.school_nid = %d
                    AND n.is_project = 0
                    AND n.school_year = %d GROUP BY ct.school_term_tid
                    ";
    watchdog('schoolwidecourses', $sql . ' school _nid ' . $school_id . ' year ' . get_school_year($school_id));
    $result = db_query($sql, $school_id, get_school_year($school_id));

    while ($rec = db_fetch_object($result)) {
        $recs[] = $rec;
    }

    return $recs;
}

function getSchools() {
    $sql = "SELECT n.nid, n.title
            FROM {node} n
            INNER JOIN {ntlp_school} s ON s.nid=n.nid
            ORDER BY n.title ASC";
    $result = db_query($sql);
    while ($rec = db_fetch_object($result)) {
        $recs[] = $rec;
    }

    return $recs;
}

function get_tea_activities_tograde($course_nid) {
//This query ignores records with NULL availability from/to dates
//In activity pages we have to make sure that hidden activities should be marked NULL
    $rs = db_query("SELECT count(*) num_activities
            FROM {ntlp_activity_submission} s
            INNER JOIN {ntlp_course_activity} a ON a.nid = s.activity_nid
            WHERE a.course_nid = %d
            AND a.is_graded = 1 AND s.is_graded = 0 AND s.is_complete = 1 AND s.resubmit_count = 1
            ", $course_nid);
//            AND s.nid NOT IN (SELECT submission_nid FROM {ntlp_gb_activity_grade} g)
    if ($rs) {
        $rec = db_fetch_object($rs);
        return $rec;
    }
    return false;
}

function getStudentPendingActivities($school_id, $course_id, $user_uid) {
    $rs = db_query("SELECT count(*) num_activities
            FROM {ntlp_course_activity} a
            INNER JOIN {ntlp_activity_user} au ON au.activity_nid = a.nid AND au.user_uid = %d
            WHERE a.course_nid = %d
            AND a.available_from != '0000-00-00 00:00:00' AND a.due_date > NOW()", $user_uid, $course_id);
    if ($rs) {
        $rec = db_fetch_object($rs);
        return $rec;
    }
    return false;
//
//    $sql = "SELECT (
//                SELECT
//                        COUNT(*)
//                FROM
//                        {ntlp_course_activity}
//                        INNER JOIN
//                        {ntlp_course}
//                        ON {ntlp_course_activity}.course_nid = {ntlp_course}.nid
//
//                WHERE
//                        {ntlp_course_activity}.due_date >= DATE_SUB( CURRENT_DATE, INTERVAL 7 DAY )
//                        AND
//                        {ntlp_course}.nid = %d
//                )-(
//                        SELECT
//                                COUNT(*)
//                        FROM
//                                {ntlp_course_activity}
//                                INNER JOIN
//                                {ntlp_activity_submission}
//                                ON {ntlp_activity_submission}.activity_nid = {ntlp_course_activity}.nid
//
//                                INNER JOIN
//                                {ntlp_course}
//                                ON {ntlp_course_activity}.course_nid = {ntlp_course}.nid
//                        WHERE
//                                {ntlp_course_activity}.due_date >= DATE_SUB( CURRENT_DATE, INTERVAL 7 DAY )
//                                AND
//                                {ntlp_activity_submission}.user_uid = %d
//                                AND
//                                {ntlp_course}.nid = %d
//              ) AS pending_activities";
//
//    if( $result = db_query( $sql, $course_id, $user_id, $course_id ) ) {
//        $row = db_fetch_object( $result );
//        return $row->pending_activities;
//    }
//    return 0;
}

function get_resources_new($course_nid, $user_id) {
//unix_timestamp(date_sub(now(),
    $rs = db_query("SELECT access from {users} where uid = %d", $user_id);
    $rec = db_fetch_object($rs);
//This query ignores records with NULL availability from/to dates
//In activity pages we have to make sure that hidden activities should be marked NULL
    $rs = db_query("SELECT count(*) num_resources
            FROM {ntlp_course_resource} r INNER JOIN {node} n
            WHERE n.nid = r.nid
            AND r.course_nid = %d
            AND n.changed > %d", $course_nid, $rec->access);
    if ($rs) {
        $rec = db_fetch_object($rs);
        return $rec;
    }
    return false;
}

function _get_recent_groups_activity($user_id, $joined = true) {
    //$school_nid = get_this_school();
//    $sql = "SELECT n.nid, n.title, count(*) new_count
//            FROM
//                {ntlp_group} ng
//                INNER JOIN {og_uid} og ON og.nid = ng.nid
//                INNER JOIN {node} n ON n.nid = ng.nid
//                INNER JOIN {ntlp_course_resource} gr ON ng.nid = gr.course_nid
//            WHERE (og.uid = $user_id)
//            AND og.is_active = 1
//            GROUP BY n.nid
//            ORDER BY n.changed DESC
//            LIMIT 0,3";

    $sql = "SELECT nn.nid, nn.title     , COUNT(gr.course_nid ) new_count
FROM {ntlp_group} ng , {ntlp_course_resource} gr, {node} n  , {node} nn  , {og_uid} og
WHERE gr.course_nid  = ng.nid AND n.nid = gr.nid AND nn.nid = gr.course_nid AND og.nid = ng.nid AND nn.nid = ng.nid
AND (og.uid = $user_id) AND og.is_active = 1
GROUP BY gr.course_nid ORDER BY MAX(n.changed)  DESC LIMIT 0,3";



//                WHERE (ng.showin_network_directory = 1) OR
//                  (ng.showin_school_directory = 1 AND ng.school_nid = $school_nid) OR
//                  (og.uid = $user_id)
//            HAVING og.uid ". ($joined ? '=' : '<>') ." %d
    $rows = array();
    if ($result = db_query($sql, $user_id)) {
        while ($row = db_fetch_object($result)) {
            $rows[] = $row;
        }
    }

    return $rows;
}

function _get_courses_for_year($school_nid, $school_year) {

    $result = db_query("SELECT nc.*,n.title,nct.school_term_tid, nst.term_abbrv, (nst.date_started <= NOW() AND nst.date_closed >= NOW()) is_term_active 
                FROM dpl_ntlp_school_term nst
                INNER JOIN dpl_ntlp_course_term nct ON nct.school_term_tid = nst.school_term_tid
                INNER JOIN dpl_node n ON n.nid = nct.course_nid
                INNER JOIN dpl_ntlp_course nc ON nc.nid = n.nid
                WHERE nc.is_project <> 1 AND nst.school_year = %d 
                AND nst.school_nid = %d ORDER BY n.title ASC", $school_year, $school_nid);
    $rows = array();

    if ($result->num_rows > 0) {
        while ($row = db_fetch_object($result)) {
            $rows[] = $row;
        }
    }
    return $rows;
}

function get_courses_for_user($user_uid, $school_year, $tab_name, $sort_option) {
    global $user;

    $query = " SELECT @SELECT_RECORDS c.*, n.title 
        FROM dpl_ntlp_course c
        INNER JOIN dpl_node n ON n.nid = c.nid
        @INNER_JOIN
        WHERE c.is_project <> 1 @AND_CONDITION GROUP BY c.nid ORDER BY n.title";

    if ($tab_name == 'teaching') {
        $user_role = NTLP_ROLEID_TEACHER;
    } else if ($tab_name == 'training') {

        if (isset($user->roles[NTLP_ROLEID_PARENT])) {

            if (isset($_SESSION['PARENT_STUDENT_UID'])) {
                $user_uid = $_SESSION['PARENT_STUDENT_UID'];
            }
        } else {
            $user_uid = $user->uid;
        }
        $user_role = NTLP_ROLEID_STUDENT;
    }

    if ($sort_option == 'ct') {

        $query = str_replace('@SELECT_RECORDS', ' c.period,', $query);
        $query = str_replace('@INNER_JOIN', ' INNER JOIN dpl_ntlp_course_user cu ON cu.course_nid = c.nid
                      INNER JOIN dpl_ntlp_course_term ct ON ct.course_nid = c.nid 
                      INNER JOIN dpl_ntlp_school_term st ON st.school_term_tid = ct.school_term_tid ', $query);
        $query = str_replace('@ORDER_BY', '', $query);
        $query = str_replace('@AND_CONDITION', "
                    AND st.date_started <= now() AND st.date_closed >= now()
                    AND cu.user_uid = $user_uid AND cu.role_id = " . $user_role . "
                AND c.school_year = " . get_school_year(get_user_school($user->uid)), $query);

        $rows = array();
        if ($result = db_query($query)) {

            if ($result->num_rows > 0) {
                while ($row = db_fetch_object($result)) {
                    $courses[] = $row;
                }
            }
        }

        $totCourses = count($courses);
        for ($i = 0; $i < $totCourses; $i++) {
            $courses[$i]->total_students = _get_courses_enrollment($courses[$i]->nid);
        }

        for ($j = 0; $j < $totCourses; $j++) {
            $result = get_course_resources($courses[$j]->nid);
            $courses[$j]->total_share_resources = $result->num_rows;
        }

        return $courses;
    } else if ($sort_option == 'ay') {

        $query = str_replace('@SELECT_RECORDS', ' c.period, (SELECT COUNT(*) FROM {ntlp_course_term} nct WHERE c.nid = nct.course_nid) AS course_term_count,
   (SELECT COUNT(*) FROM {ntlp_school_term} nst WHERE c.school_nid = nst.school_nid AND c.school_year = nst.school_year) AS school_term_count, ', $query);
        $query = str_replace('@INNER_JOIN', ' 
                INNER JOIN dpl_ntlp_course_user cu ON cu.course_nid = c.nid
                INNER JOIN dpl_ntlp_course_term ct ON ct.course_nid = c.nid 
                INNER JOIN dpl_ntlp_school_term st ON st.school_term_tid = ct.school_term_tid ', $query);
        $query = str_replace('@ORDER_BY', '', $query);
        $query = str_replace('@AND_CONDITION', "
                    AND cu.user_uid = $user_uid AND cu.role_id = " . $user_role . " 
                    AND c.school_year = " . $school_year, $query);

        $rows = array();
        if ($result = db_query($query)) {

            if ($result->num_rows > 0) {
                while ($row = db_fetch_object($result)) {
                    $courses[] = $row;
                }
            }
        }
        return $courses;
    } else {

        $query = str_replace('@SELECT_RECORDS', ' c.period, (SELECT COUNT(*) FROM {ntlp_course_term} nct WHERE c.nid = nct.course_nid) AS course_term_count,
   (SELECT COUNT(*) FROM {ntlp_school_term} nst WHERE c.school_nid = nst.school_nid AND c.school_year = nst.school_year) AS school_term_count, ', $query);
        $query = str_replace('@INNER_JOIN', ' 
                INNER JOIN dpl_ntlp_course_user cu ON cu.course_nid = c.nid
                INNER JOIN dpl_ntlp_course_term ct ON ct.course_nid = c.nid 
                INNER JOIN dpl_ntlp_school_term st ON st.school_term_tid = ct.school_term_tid ', $query);
        $query = str_replace('@ORDER_BY', '', $query);
        $query = str_replace('@AND_CONDITION', "
                    AND cu.user_uid = $user_uid  AND cu.role_id = " . $user_role . " 
                    AND c.school_year = " . $school_year, $query);

        $rows = array();
        if ($result = db_query($query)) {

            if ($result->num_rows > 0) {
                while ($row = db_fetch_object($result)) {
                    $courses[] = $row;
                }
            }
        }
        return $courses;
    }
}

function _get_teacher_courses($teacher_id, $head, $enrolled = false, $school_year = 0, $school_term_tid = 0) {
    module_load_include('inc', 'ntlp_school', 'constants');
    $select = '';
    $school_year = (int) $school_year;

    $where = ' AND ncu.user_uid = %d AND ncu.role_id = ';
    $where .= $enrolled ? NTLP_ROLEID_STUDENT : NTLP_ROLEID_TEACHER;
    $where .= $school_year ? ' AND nst.school_year = ' . $school_year : '';

    //this line added by saifuddin to include a filter for school term tid
    $where .= ( $school_term_tid > 0) ? ' AND nst.school_term_tid = ' . $school_term_tid : '';

    $order_by = (isset($_GET['sort']) || isset($_GET['order'])) ? tablesort_sql($head) : ' ORDER BY nc.period ASC, n.title ASC ';
    if ($enrolled) {
        $select = ', ncu.period';
    }

    $sql = "SELECT nc.*,n.title,nct.school_term_tid, td.name semester_name, nst.term_abbrv, (nst.date_started <= now() and nst.date_closed >= now()) is_term_active $select
                FROM {ntlp_school_term} nst
                INNER JOIN {ntlp_course_term} nct ON nct.school_term_tid = nst.school_term_tid
                INNER JOIN {node} n ON n.nid = nct.course_nid
                INNER JOIN {ntlp_course} nc ON nc.nid = n.nid
                INNER JOIN {term_data} td ON td.tid = nct.school_term_tid
                INNER JOIN {ntlp_course_user} ncu ON ncu.course_nid = nc.nid
                WHERE nc.is_project <> 1
            $where
            " . $order_by;
    $rows = array();
    if ($result = db_query($sql, $teacher_id)) {

        while ($row = db_fetch_object($result)) {
            $rows[] = $row;
        }
    }

    return $rows;
}

function get_teacher_courses_in_year($teacher_id, $school_year = 0) {
    global $user;

    $select = '';
    $school_year = (int) $school_year;

//    if (isset($user->roles[NTLP_ROLEID_NTNMANAGER]) || isset($user->roles[NTLP_ROLEID_NTNSTAFF])) {
//        $COURSE_USER_INNER_JOIN = "";
//        $AND_CONDITION = "";
//    } else
//    if (isset($user->roles[NTLP_ROLEID_SCHOOLLEADER]) || isset($user->roles[NTLP_ROLEID_SITEADMIN])) {
//        $AND_CONDITION = " AND c.school_nid = " . get_this_school();
//    } else {
    $COURSE_USER_INNER_JOIN = " INNER JOIN {ntlp_course_user} cu ON cu.course_nid = c.nid AND cu.user_uid = $user->uid ";
    $AND_CONDITION = " AND cu.user_uid = $teacher_id AND cu.role_id = " . NTLP_ROLEID_TEACHER;
//    }

    $sql = "SELECT n.title, n.nid 
                FROM {ntlp_course} c
                INNER JOIN {node} n ON n.nid = c.nid                
             $COURSE_USER_INNER_JOIN
                WHERE c.is_project <> 1 AND c.school_year = %d
            $AND_CONDITION ORDER BY n.title asc";

    $rows = array();
    if ($result = db_query($sql, $school_year)) {

        while ($row = db_fetch_object($result)) {
            $rows[] = $row;
        }
    }

    return $rows;
}

function _get_all_school_courses($school_id, $head, $school_year, $school_term_tid) {
    global $user;

    if (isset($user->roles[NTLP_ROLEID_TEACHER]) || isset($user->roles[NTLP_ROLEID_SCHOOLLEADER])
            || isset($user->roles[NTLP_ROLEID_SITEADMIN])) {

        if ($school_id != get_user_school($user->uid)) {
            $INNER_JOIN = ' LEFT JOIN {ntlp_course_user} cu ON cu.course_nid = nc.nid AND cu.user_uid = ' . $user->uid;
            $AND_CONDITION = '(nc.is_project <> 1 AND nc.school_nid = ' . $school_id . '  AND nst.school_year = ' . $school_year . ' 
                AND nct.school_term_tid = ' . intval($school_term_tid) . ' AND cu.role_id IS NOT NULL)
                OR (nc.is_project <> 1 AND nc.school_nid = ' . $school_id . '  AND nst.school_year = ' . $school_year . ' 
                AND nct.school_term_tid = ' . intval($school_term_tid) . ' AND nc.allow_self_enroll = 1 AND cu.user_uid IS NULL
                AND cu.role_id IS NULL)';
        } else {
            $AND_CONDITION = 'nc.is_project <> 1 AND nc.school_nid = ' . $school_id . ' 
            AND nst.school_year = ' . $school_year . ' AND nct.school_term_tid = ' . intval($school_term_tid);
            $INNER_JOIN = '';
        }
    } else {
        $AND_CONDITION = 'nc.is_project <> 1 AND nc.school_nid = ' . $school_id . ' 
            AND nst.school_year = ' . $school_year . ' AND nct.school_term_tid = ' . intval($school_term_tid);
        $INNER_JOIN = '';
    }



    $sql = "SELECT nc.*, n.title, nct.school_term_tid, td.name semester_name, nst.term_abbrv, (nst.date_started <= now() and nst.date_closed >= now()) is_term_active
                FROM {ntlp_school_term} nst
                INNER JOIN {ntlp_course_term} nct ON nct.school_term_tid = nst.school_term_tid
                INNER JOIN {node} n ON n.nid = nct.course_nid
                INNER JOIN {ntlp_course} nc ON nc.nid = n.nid
                INNER JOIN {term_data} td ON td.tid = nct.school_term_tid
                $INNER_JOIN
                WHERE  $AND_CONDITION
                    ORDER BY n.title ASC";

    $rows = array();

    if ($result = db_query($sql)) {
        while ($row = db_fetch_object($result)) {
            $rows[] = $row;
        }
    }

    return $rows;
}

function _get_parent_children($parent_id) {
    $sql = "SELECT nu.*, u.picture
            FROM {ntlp_user} nu
                INNER JOIN {users} u ON u.uid = nu.uid
            INNER JOIN {ntlp_user_parent} up ON up.student_uid = u.uid
            WHERE up.parent_uid = %d
            ORDER BY up.student_uid ASC";

    $rows = array();
    if ($result = db_query($sql, $parent_id)) {
        while ($row = db_fetch_object($result)) {
            $rows[] = $row;
        }
    }

    return $rows;
}

//function is_course_exist($course_id) {
//
//    $flag = false;
//    $result =  db_query("SELECT * FROM {ntlp_course} WHERE nid = %d", $group_id);
//    if($result->num_rows> 0) {
//        $flag = true;
//    }else {
//        $flag = false;
//    }
//    return $flag;
//}

function get_school_terms($school_nid, $school_year) {
    $rs = db_query("select td.tid,td.name, st.date_started, st.date_closed
            from {ntlp_school_term} st inner join {term_data} td on st.school_term_tid = td.tid
            where st.school_nid=%d and st.school_year=%d", $school_nid, $school_year);
    return $rs;
}

function get_course_terms($course_nid) {
    $rs = db_query("SELECT td.tid, td.name, st.date_started, st.date_closed
            FROM {ntlp_course_term} ct
            LEFT JOIN {term_data} td on ct.school_term_tid = td.tid
            LEFT JOIN {ntlp_school_term} st ON st.school_term_tid = ct.school_term_tid
            where ct.course_nid=%d", $course_nid);
    return $rs;
}

function _get_course_forum_topics($course_nid, $limit = 10, $header = array()) {
    $output = array();

    $sql = "SELECT tn.tid,n.nid, n.title, n.uid, n.sticky, n.created, n.comment,
        l.comment_count, l.last_comment_timestamp, l.last_comment_uid
        FROM dpl_term_node tn
        inner join dpl_node n on tn.nid=n.nid
        inner join dpl_node_comment_statistics l on l.nid=n.nid
        where tn.tid IN(select tn2.tid from dpl_term_node tn2 where tn2.nid=$course_nid )
        and type='echo_forum' and status='1' " . tablesort_sql($header, 'n.sticky DESC, ');

    $result = pager_query($sql, $limit);
    if ($result->num_rows > 0) {
        while ($topic = db_fetch_object($result)) {
//            $user_Detail_Obj = get_user_school_info($topic->uid);
            $output[] = $topic;
        }
    }

    return $output;
}

function _get_vocab_vid($string) {

    $rs = db_query("SELECT vid FROM {vocabulary} where name = '%s'", $string);
    return db_fetch_object($rs);
}

/* Fetch Course Primary Teacher */

function get_course_primary_teacher($course_nid) {

    $rs = db_query("select CONCAT(u.first_name, ' ',u.last_name) as primary_teacher_1, u.uid as primary_teacher_1_uid, 
        CONCAT(u2.first_name, ' ',u2.last_name) as primary_teacher_2, u2.uid as primary_teacher_2_uid
        FROM {ntlp_course} c
        INNER JOIN {ntlp_user} u ON u.uid = c.teacher1_uid
        LEFT JOIN {ntlp_user} u2 ON u2.uid = c.teacher2_uid
        where c.nid = %d ", $course_nid);

    return $rs;
}

//
///* Fetch Document LIbrary Resources for library landing page*/
//function get_doc_library_resources($school_nid = 0, $string) {
//
//    $rs =db_query("SELECT lc.category_tid,
//                      tn.nid AS node_id, n.title
//                    FROM dpl_ntlp_module nm
//                      INNER JOIN dpl_ntlp_library_category lc
//                        ON lc.module_nid = nm.nid
//                        INNER JOIN dpl_term_node tn ON tn.tid = lc.category_tid
//                        INNER JOIN dpl_ntlp_library_resource lr  ON lr.nid = tn.nid
//                        INNER JOIN dpl_node n   ON n.nid = lr.nid
//                    WHERE school_nid = %d
//                        AND nm.type = '%s'
//                    ORDER BY lr.nid limit 0,3",$school_nid, $string);
//
//    return $rs;
//}
//
//function get_library_projects($school_nid = 0, $string) {
//
//    $rs =db_query("SELECT * FROM dpl_ntlp_module nm
//                    INNER JOIN dpl_ntlp_library_project lr ON lr.module_nid = nm.nid
//                    INNER JOIN dpl_node n ON n.nid = lr.nid
//                    WHERE school_nid = %d AND nm.type ='%s' ORDER BY lr.nid DESC LIMIT 0, 3",$school_nid, $string);
//
//    return $rs;
//}
//

function get_new_comment_count($topic_nid) {

    $result = db_query("SELECT COUNT(c.cid)FROM dpl_node n
        INNER JOIN dpl_comments c ON n.nid = c.nid
        WHERE n.nid = %d AND FROM_UNIXTIME(c.timestamp) > (NOW() - INTERVAL 1 HOUR)", $topic_nid);

    if ($result->num_rows > 0) {

        return db_result($result);
    } else {
        return false;
    }
}

function is_course_exist_in_year($course_nid, $school_year) {
    $flag = false;
    $result = db_query("SELECT * FROM {ntlp_course} 
        WHERE school_year = %d AND nid = %d ", $school_year, $course_nid);

    if ($result->num_rows > 0) {
        $flag = true;
    }
    watchdog('is_course_exist', $flag . ' ' . $course_nid . ' ' . $school_year . ' rslt ' . print_r($result, true));
    return $flag;
}

function get_course_resources($course_nid) {

    $result = db_query("SELECT cr.* FROM {ntlp_course_resource} cr 
            WHERE course_nid = %d           
            AND cr.nid NOT IN (SELECT pr.nid FROM {ntlp_project_resource} pr )", $course_nid);

    return $result;
}

function get_forum_topic_by_nid($course_nid) {

    $output = array();

    $result = db_query("SELECT tn.tid,n.nid, n.title, n.uid, n.sticky, n.created, n.changed, n.comment,
        l.comment_count, l.last_comment_timestamp, l.last_comment_uid
        FROM dpl_term_node tn
        INNER JOIN dpl_node n ON tn.nid=n.nid
        INNER JOIN dpl_node_comment_statistics l ON l.nid=n.nid
        WHERE tn.tid IN(SELECT tn2.tid FROM dpl_term_node tn2 WHERE tn2.nid=%d )
        AND TYPE='echo_forum' AND STATUS='1' ", $course_nid);

    if ($result->num_rows > 0) {
        while ($topic = db_fetch_object($result)) {
//            $user_Detail_Obj = get_user_school_info($topic->uid);
            $output[] = $topic;
        }
    }
    return $output;
}

function get_all_open_courses($school_id, $head, $school_year, $school_term_tid) {
    global $user;

    $sql = "SELECT nc.*, n.title 
                FROM {ntlp_school_term} nst
                INNER JOIN {ntlp_course_term} nct ON nct.school_term_tid = nst.school_term_tid
                INNER JOIN {node} n ON n.nid = nct.course_nid
                INNER JOIN {ntlp_course} nc ON nc.nid = n.nid
                WHERE nc.is_project <> 1
                    AND nc.school_nid = %d
                    AND nst.school_year = %d
		    AND nct.school_term_tid = %d
                    AND nc.allow_self_enroll = 1 
                    AND NOT EXISTS (SELECT user_uid FROM {ntlp_course_user} cu WHERE cu.course_nid = nc.nid AND user_uid = $user->uid ) 
                    ORDER BY n.title ASC";

    $rows = array();

    if ($result = db_query($sql, $school_id, $school_year, $school_term_tid)) {
        while ($row = db_fetch_object($result)) {
            $rows[] = $row;
        }
    }

    return $rows;
}

function get_parent_child_selector($parent_uid) {

    drupal_add_js("
                    function change_parent_student(obj){
                        $.post('?q=ntlp/change/parent/user/'+obj.value, function(){
                            window.location.reload();
                        });
                    }
                ", 'inline');

//    $children_selector= 'No children Found';

    $children = _get_parent_children($parent_uid);

    if (count($children) > 1) {
        $children_selector = '<select id="parent_student_selector" onchange="change_parent_student(this);" >';
        foreach ($children as $child) {
            $children_selector .= '<option value="' . $child->uid . '">' . $child->first_name . ' ' . $child->last_name . '</option>';
        }
        $children_selector .= '</select>';
    } else {
        $children_selector = $children[0]->first_name . ' ' . $children[0]->last_name;
    }
    return $children_selector;
}

function get_all_user_courses_teachers($user_uid) {
    
    $rs = db_query("SELECT DISTINCT cu.user_uid uid FROM {ntlp_course_user} cu
            WHERE role_id=6 AND cu.course_nid IN
                (SELECT course_nid FROM dpl_ntlp_course_user WHERE user_uid IN ({$user_uid}) AND role_id=5)");
    return $rs;
}