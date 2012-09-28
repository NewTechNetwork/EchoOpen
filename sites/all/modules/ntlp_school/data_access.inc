<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


require_once "constants.inc";

function get_school_name($schoolid) {
    $schools = get_schools($schoolid);
    $result = db_fetch_object($schools);
    $name = $result->school_name;
    return $name;
}

function get_schools($schoolid = NULL) {
    if (isset($schoolid) && is_numeric($schoolid))
        $whereclause = "s.nid = $schoolid";
    else
        $whereclause = "1=1";
    $schools = db_query("SELECT  s.nid, s.address, s.location_tid, s.active,
                t.name location_name, n.title school_name
                FROM {ntlp_school} s
                    INNER JOIN {node} n ON s.nid = n.nid AND n.status = 1
                    LEFT JOIN {term_data} t ON t.tid = s.location_tid
                WHERE %s
                ORDER BY n.title
                ", $whereclause);
    return $schools;
}

function get_user($uid) {
    if (intval($uid) > 0) {
        $squery = db_query("SELECT nu.*, du.name, du.picture FROM {ntlp_user} nu
        INNER JOIN {users} du ON du.uid = nu.uid
        WHERE nu.uid = %d", intval($uid));

        $result = db_fetch_object($squery);
        if (isset($result)) {
            return $result;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

function get_user_school($id) {
    $squery = db_query("SELECT school_nid FROM {ntlp_user} WHERE uid = %d", intval($id));

    $result = db_fetch_object($squery);
    if (isset($result)) {
        $sid = intval($result->school_nid);
        return $sid;
    } else {
//throw new Exception('Unable to find user\'s school');
//ddd course
        return false;
    }
}

function get_user_schoolrec($userid, $lite = false) {
    if ($lite) {
        $schools = db_query("SELECT s.* FROM {ntlp_school} s
                    INNER JOIN {ntlp_user} u ON u.school_nid = s.nid
                    WHERE u.uid = %d
                    ", intval($userid));
    } else {
        $schools = db_query("SELECT s.*,
                    t.name location_name, n.title school_name
                    FROM {ntlp_school} s
                        left JOIN {term_data} t ON s.location_tid = t.tid
                        INNER JOIN {node} n ON s.nid = n.nid
                        INNER JOIN {ntlp_user} u ON u.school_nid = s.nid
                    WHERE u.uid = %d
                    ORDER BY n.title
                    ", intval($userid));
    }
    $result = db_fetch_object($schools);
    if (isset($result)) {
        return $result;
    } else {
        return false;
    }
    return $result;
}

//Returns users in the current school
//Parameters:
//  $string = search text
//  $courseid = The course in which we want to enroll the users
//  $user_role = (Optional) default is 'all'. Enumerated role ids can be used from this file
//  $grade_levels = (Optional) Comma delimited list of grade levels, to allow the user to filter users in current school
//  $schoolid = (Optional) To enroll users from another school. If schoolid is provided, then grade levels would be used for that school
function search_for_enrollment($string, $courseid, $user_role = 'all') {
//IMPORTANT: Get current user's school to get list of this school's students.
//If user wants to enroll user of another school, then another function will be used
//to pull the records.
    global $user;
    $school_nid = get_user_school($user->uid);

    if (strlen($string) > 0) {
        if (strtolower(trim(substr($string, 0, 1))) == 's') {
            $string = "%" . $string;     //to protect the first 's' which will be replaced by %s
//            watchdog('string', $string);
//            echo 'String:'.$string;
        }
    }
    if ($user_role == 'all') {
        $role_clause = "";
    } else {
        $role_clause = " AND r.rid = " . intval($user_role);   //NTLP_ROLEID_STUDENT."
    }
//>> Don't show Dropouts
//>> Don't show users already enrolled in this course
//>> Show users of this (or selected) school only
    $res = db_query("SELECT r.uid, u.uid, u.name, u.mail, nu.first_name, nu.last_name
            FROM {users} u
            INNER JOIN {ntlp_user} nu ON nu.uid = u.uid
            INNER JOIN {users_roles} r ON nu.uid = r.uid
            WHERE (lower(u.name) LIKE '%" . check_plain(strtolower(trim($string))) . "%' OR
                   lower(nu.first_name) LIKE '%" . check_plain(strtolower(trim($string))) . "%' OR
                   lower(nu.last_name) LIKE '%" . check_plain(strtolower(trim($string))) . "%')" .
            $role_clause . "
                AND u.uid NOT IN (SELECT user_uid FROM {ntlp_course_user} WHERE isnull(role_id) AND course_nid = " . intval($courseid) . ")
                AND nu.school_nid = " . intval($school_nid) . "
            ORDER BY nu.first_name, nu.last_name");

    return $res;
}

//Returns users in the selected school having the selected role
//Parameters:
//  $string = search text
//  $school_nid = The school in which the users are registered
//  $user_role = (Optional) default is 'all'. Enumerated role ids can be used from this file
//  $grade_levels = (Optional) Comma delimited list of grade levels, to allow the user to filter users in current school
//  $schoolid = (Optional) To enroll users from another school. If schoolid is provided, then grade levels would be used for that school
function search_users_inschool($string, $school_nid, $user_role = 'all', $filters = array(), $show_staff_only = false) {

    global $user;

    $where = '';
    $school_nid = (int) $school_nid;
    if (strlen($string) > 0) {
        if (strtolower(trim(substr($string, 0, 1))) == 's') {
            $string = "%" . $string;     //to protect the first 's' which will be replaced by %s
//            watchdog('string', $string);
//            echo 'String:'.$string;
        }
    }

    if (isset($filters['IN_GRAD_YEAR'])) {
        $filters['IN_GRAD_YEAR'] = is_array($filters['IN_GRAD_YEAR']) ? $filters['IN_GRAD_YEAR'] : array($filters['IN_GRAD_YEAR']);
        $filters['IN_GRAD_YEAR'] = array_map('intval', $filters['IN_GRAD_YEAR']);
        $gradSql = '  (
                        nu.grad_year IN(' . implode(',', $filters['IN_GRAD_YEAR']) . ')
                        AND
                        nu.school_nid = ' . $school_nid . '
                        AND
                        r.rid = ' . NTLP_ROLEID_STUDENT . '
                    )';
        if (is_array($user_role)) {
            if (in_array(NTLP_ROLEID_TEACHER, $user_role)) {
                $where .= " OR $gradSql";
                unset($user_role[array_search(NTLP_ROLEID_STUDENT, $user_role)]);
            } else {
                $where .= " AND $gradSql";
            }
        } else {
            $where .= " AND $gradSql";
        }
    }


    if ($user_role == 'all' || (count($user_role) <= 0)) {

        if ($show_staff_only)
            $role_clause = " AND r.rid >= " . NTLP_ROLEID_TEACHER;   //NTLP_ROLEID_STUDENT.";
        else
            $role_clause = "";
    } else {
        if ($user_role || (is_array($user_role) and count($user_role))) {
            $user_role = is_array($user_role) ? $user_role : array($user_role);
            $user_role = array_unique($user_role);
            $user_role = '(' . implode(',', $user_role) . ')';
            $role_clause = " AND r.rid IN " . $user_role;   //NTLP_ROLEID_STUDENT."
        } else {
            $role_clause = '';
        }
    }

    if ($filters['action'] == 'get_rubric_users') {
//        $where .= ' AND u.uid IN ( SELECT nau.user_uid FROM dpl_ntlp_activity_user nau WHERE nau.activity_nid = ' . (int) $filters['course_nid'] . ') ';
        $where .= " AND nau.activity_nid = " . $filters['course_nid'];
        $innerJoin .= " INNER JOIN {ntlp_activity_user} nau ON nau.user_uid = u.uid ";
    } else {
        $where .= " AND nu.school_nid = " . intval($school_nid);
    }

    if (isset($filters['RETURN_GRADS_ARRAY'])) {
        $select = 'DISTINCT nu.grad_year';
    } else {
        $select = 'DISTINCT r.uid, u.uid, u.name, u.mail, nu.first_name, nu.last_name, nu.grad_year, rr.rid, rr.name user_type,n.title organization';
    }

    if ($filters['disclude'] && $filters['discludeList']) {
        $filters['discludeList'] = array_map('intval', $filters['discludeList']);
        $where .= ' AND u.uid NOT IN(' . trim(implode(',', $filters['discludeList']), ',') . ') ';
    } else {
        if (count($filters['discludeList']) > 0) {
            $where .= ' OR u.uid IN(' . trim(implode(',', $filters['discludeList']), ',') . ') ';
        }
    }

    if ($filters['block_self_selection'] == 1)
        $where .= " AND u.uid <> {$user->uid} ";

//print_r($filters);
//>> Don't show Dropouts
//>> Don't show users already enrolled in this course
//>> Show users of this (or selected) school only
    $sql = "SELECT $select
            FROM {users} u
            INNER JOIN {ntlp_user} nu ON nu.uid = u.uid
            LEFT JOIN {users_roles} r ON nu.uid = r.uid
            LEFT JOIN {role} rr ON rr.rid = r.rid
            INNER JOIN {node} n ON n.nid = nu.school_nid
            $innerJoin
            WHERE (lower(u.name) LIKE '%" . check_plain(strtolower(trim($string))) . "%' OR
                   lower(nu.first_name) LIKE '%" . check_plain(strtolower(trim($string))) . "%' OR
                   lower(nu.last_name) LIKE '%" . check_plain(strtolower(trim($string))) . "%')" .
            $role_clause . "
                AND nu.is_active = 1
            $where
            ORDER BY nu.last_name, nu.first_name";

//    echo $sql;
    $res = db_query($sql);

    if (isset($filters['RETURN_GRADS_ARRAY'])) {
        $grad_years = array();
        while ($row = db_fetch_object($res)) {
            if ($row->grad_year > 0) {
                $grad_years[] = $row->grad_year;
            }
        }

        return $grad_years;
    } else {
        return $res;
    }
}

//Returns users in the selected course having the selected role
//In this function, the roles are fetched from Course enrollment and not user's actual role
//Parameters:
//  $string = search text
//  $course_nid = The course in which the user is enrolled
//  $user_role = (Optional) default is 'all'. Enumerated role ids can be used from this file
// The school id may not be used in the SQL WHERE clause as it will allow the results to
//  show all users of any school, enrolled in the selected course
function search_users_incourse($string, $course_nid, $user_role = 'all', $filters=array()) {
    $where = '';
    if (isset($filters['IN_GRAD_YEAR'])) {
        $filters['IN_GRAD_YEAR'] = is_array($filters['IN_GRAD_YEAR']) ? $filters['IN_GRAD_YEAR'] : array($filters['IN_GRAD_YEAR']);
        $filters['IN_GRAD_YEAR'] = array_map('intval', $filters['IN_GRAD_YEAR']);
        $where .= ' AND nu.grad_year IN(' . implode(',', $filters['IN_GRAD_YEAR']) . ')';
    }

    if (strlen($string) > 0) {
        if (strtolower(trim(substr($string, 0, 1))) == 's') {
            $string = "%" . $string;     //to protect the first 's' which will be replaced by %s
//            watchdog('string', $string);
//            echo 'String:'.$string;
        }
    }
    if ($user_role == 'all') {
        $role_clause = "";
    } else {
        $user_role = is_array($user_role) ? $user_role : array($user_role);
        $user_role = array_map('intval', $user_role);
        $user_role = array_unique($user_role);
        $user_role = '(' . implode(',', $user_role) . ')';
        $role_clause = " AND cu.role_id IN " . $user_role;
    }
//>> Don't show Dropouts
//>> Don't show users already enrolled in this course
//>> Show users of this (or selected) school only
    $sql = "SELECT u.uid, u.name, u.mail, nu.first_name, nu.last_name
            FROM {users} u
            INNER JOIN {ntlp_user} nu ON nu.uid = u.uid
            INNER JOIN {ntlp_course_user} cu ON cu.user_uid = nu.uid
            WHERE (lower(u.name) LIKE '%" . check_plain(strtolower(trim($string))) . "%' OR
                   lower(nu.first_name) LIKE '%" . check_plain(strtolower(trim($string))) . "%' OR
                   lower(nu.last_name) LIKE '%" . check_plain(strtolower(trim($string))) . "%')" .
            $role_clause . "
                AND cu.course_nid = " . $course_nid . "
            $where
            ORDER BY nu.first_name, nu.last_name";

    $res = db_query($sql);

    return $res;
}

function get_agenda($course_nid, $y, $m, $d) {

    $rs = db_query("SELECT d.nid, d.course_nid, d.year, d.month, d.day,
            r.title, r.body, e.event_start, e.event_end
        from {ntlp_course_agenda} d  INNER JOIN {node_revisions} r
            ON d.nid = r.nid
            INNER JOIN {event} e ON d.nid = e.nid
        WHERE d.year = %d AND d.month = %d AND d.day = %d AND d.status = 1
            AND d.course_nid = %d", intval($y), intval($m), intval($d), intval($course_nid));

    $result = db_fetch_object($rs);
    if (isset($result)) {
        return $result;
    } else {
        return false;
    }
    return $result;
}

//For Calendar
function get_events_ofthemonth($school_nid, $group_nid, $y, $m) {
    global $user;
    $school_nid = (int) $school_nid;
    $group_nid = (int) $group_nid;
    $y = (int) $y;
    $m = (int) $m;
    $where = '';

    if ($group_nid) {
        $where .= " AND d.group_nid = $group_nid";
    }

//    if ($school_nid > 0)
//        $filter_s = ' AND d.school_nid = '.$school_nid;
//    if ($group_nid > 0)
//        $filter_g = ' AND d.group_nid = '.$group_nid;
    if (!empty($visible_to))
        $filter_v = " AND d.visible_to = '" . $visible_to . "'";

    $rs = db_query("SELECT d.nid, d.day, d.status, d.visible_to, e.event_start, e.event_end 
            FROM {ntlp_events} d
            INNER JOIN {event} e ON d.nid = e.nid
            LEFT JOIN dpl_og_uid og ON og.nid = d.group_nid AND og.uid = %d
            WHERE
            (
                (d.group_nid <> 0 AND NOT ISNULL(og.nid))
                OR (d.school_nid = %d AND d.visible_to = 'S')
                OR d.visible_to = 'N'
            )
            $where
            AND ($y >= year(e.event_start) AND $m >= month(e.event_start)) AND ($y <= year(e.event_end) AND $m <= month(e.event_end)) AND d.status = 1" .
            $filter_s . $filter_g . $filter_v, $user->uid, $school_nid);

//    watchdog('events', "For the month: $y/$m. SQL: SELECT d.nid, d.day, d.status, d.visible_to
//            FROM {ntlp_events} d
//            LEFT JOIN dpl_og_uid og ON og.nid = d.group_nid AND og.uid = $user->uid
//            WHERE
//            (
//                (d.group_nid <> 0 AND NOT ISNULL(og.nid))
//                OR (d.school_nid = $school_nid AND d.visible_to = 'S')
//                OR d.visible_to = 'N'
//            )
//            AND d.year = $y AND d.month = $m AND d.status = 1" .
//            $filter_s . $filter_g . $filter_v);

    return $rs;
}

function get_events($school_nid, $group_nid, $y, $m, $d, $event_types = '', $visible_to = '') {
    global $user;
    $school_nid = (int) $school_nid;
    $y = (int) $y;
    $m = (int) $m;
    $d = (int) $d;

    $cur_date = "$y-$m-$d 00:00:00";

    $visible_to = $visible_to ? "'" . check_plain($visible_to) . "'" : '';
    $where = '';

    if ($group_nid) {
        $where .= ' AND d.group_nid = ' . intval($group_nid);
    }

    $event_types = str_replace("'", '', trim($event_types, ','));
    if ($event_types) {
        $event_types = explode(',', $event_types);
        $v_to = array();
        foreach ($event_types as $et) {
            if ($et == 'ntlp_group_event') {
                $v_to[] = "'G'";
            } else if ($et == 'ntlp_school_event') {
                $v_to[] = "'S'";
            } else if ($et == 'ntlp_network_event') {
                $v_to[] = "'N'";
            } else if ($et == 'ntlp_course_event') {
                $v_to[] = "''";
            }
        }
        if ($v_to && !in_array('all', $event_types)) {
            $visible_to = 'AND d.visible_to IN(' . implode(',', $v_to) . ')';
        }
    }

    $rs = db_query("SELECT d.nid, d.school_nid, d.group_nid, d.year, d.month, d.day, d.visible_to,
                        n.type, r.title, r.body, e.event_start, e.event_end, gr.title AS grp_title,
                        u.uid, u.first_name, u.last_name, role.name role_name,
                        s.title school_name
                    FROM {ntlp_events} d
                        INNER JOIN dpl_node n ON d.nid = n.nid
                        INNER JOIN dpl_node_revisions r ON n.vid = r.vid
                        INNER JOIN dpl_event e ON d.nid = e.nid
                        INNER JOIN dpl_node s ON d.school_nid = s.nid
                        LEFT JOIN dpl_node gr ON d.group_nid = gr.nid
                        LEFT JOIN dpl_ntlp_user u ON r.uid = u.uid
                        LEFT JOIN dpl_users_roles ur ON ur.uid = n.uid
                        LEFT JOIN dpl_role role ON role.rid = ur.rid
                        LEFT JOIN dpl_og_uid og ON og.nid = d.group_nid AND og.uid = %d
                   WHERE
                        (
                            (d.group_nid <> 0 AND NOT ISNULL(og.nid))
                            OR (d.school_nid = %d AND d.visible_to = 'S')
                            OR d.visible_to = 'N'
                        )
                        AND DATEDIFF('$cur_date', e.event_start) >= 0 AND DATEDIFF('$cur_date', e.event_end) <= 0 AND d.status = 1
            $where
            $visible_to
                        ORDER BY d.year, d.month, d.day, n.title", $user->uid, $school_nid);

//    watchdog('events', "For the day: $y/$m. SQL: SELECT d.nid, d.day, d.status, d.visible_to
//            FROM {ntlp_events} d
//            LEFT JOIN dpl_og_uid og ON og.nid = d.group_nid AND og.uid = $user->uid
//            WHERE
//            (
//                (d.group_nid <> 0 AND NOT ISNULL(og.nid))
//                OR (d.school_nid = $school_nid AND d.visible_to = 'S')
//                OR d.visible_to = 'N'
//            )
//            AND d.year = $y AND d.month = $m AND d.day = $d AND d.status = 1" .
//            $visible_to);
    return $rs;
}

//function get_events($school_nid, $group_nid, $y, $m, $d, $event_types = '', $visible_to = '') {
//    global $user;
//    $school_nid = (int)$school_nid;
//    $group_nid = (int)$group_nid;
//    $y = (int)$y;
//    $m = (int)$m;
//    $d = (int)$d;
//    $visible_to = $visible_to ? "'".check_plain($visible_to)."'" : '';
//
//    $event_types = str_replace("'", '', trim($event_types, ','));
//    if($event_types) {
//        $event_types = explode(',', $event_types );
//        $v_to = array();
//        foreach($event_types as $et) {
//            if($et == 'ntlp_group_event') {
//                $v_to[] = "'G'";
//            }
//            else if($et == 'ntlp_school_event') {
//                $v_to[] = "'S'";
//            }
//            else if($et == 'ntlp_network_event') {
//                $v_to[] = "'N'";
//            }
//            else if($et == 'ntlp_course_event') {
//                $v_to[] = "''";
//            }
//        }
//        if($v_to) {
//            $visible_to = implode(',', $v_to);
//        }
//        $event_types = "'".implode("','",array_map('check_plain', $event_types))."'";
//    }
//
//    if ($school_nid > 0)
//        $filter_s = ' OR d.school_nid = '.$school_nid;
//    if ($group_nid > 0)
//        $filter_g = ' OR d.group_nid = '.$group_nid;
//    if (!empty($visible_to) && strpos($event_types, "'all'") === false)
//        $filter_v = " AND d.visible_to IN(" . $visible_to . ") ";
//
//    if (strlen($event_types) > 0) {
//        if (strpos($event_types, "'all'") === false)
//            $filter_t = " OR n.type IN ($event_types)";
//    }
//    watchdog('sql', $filter_s . $filter_g . $filter_v . $filter_t);
//
//    $rs = db_query("SELECT d.nid, d.school_nid, d.group_nid, d.year, d.month, d.day, d.visible_to,
//            n.type, r.title, r.body, e.event_start, e.event_end, gr.title as grp_title,
//            u.uid, u.first_name, u.last_name, role.name role_name,
//            s.title school_name
//        from {ntlp_events} d
//            INNER JOIN {node} n ON d.nid = n.nid
//            INNER JOIN {node_revisions} r ON n.vid = r.vid
//            INNER JOIN {event} e ON d.nid = e.nid
//            INNER JOIN {node} s ON d.school_nid = s.nid
//            LEFT JOIN {node} gr ON d.group_nid = gr.nid
//            LEFT JOIN {ntlp_user} u ON r.uid = u.uid
//            LEFT JOIN {users_roles} ur ON ur.uid = n.uid
//            LEFT JOIN {role} role ON role.rid = ur.rid
//        WHERE d.year = %d AND d.month = %d AND d.day = %d AND d.status = 1".
//            " AND (1=2 " . $filter_s . $filter_g . ")" .
//            $filter_v /*. $filter_t */,
//            $y, $m, $d);
//
//    return $rs;
//}

function get_event($event_nid) {

    $rs = db_query("SELECT d.nid, d.school_nid, d.group_nid, d.year, d.month, d.day, d.visible_to,
            n.type, r.title, r.body, e.event_start, e.event_end, gr.title as grp_title,
            u.uid, u.first_name, u.last_name, role.name role_name,
            s.title school_name
        from {ntlp_events} d
            INNER JOIN {node} n ON d.nid = n.nid
            INNER JOIN {node_revisions} r ON n.vid = r.vid
            INNER JOIN {event} e ON d.nid = e.nid
            INNER JOIN {node} s ON d.school_nid = s.nid
            LEFT JOIN {node} gr ON d.group_nid = gr.nid
            LEFT JOIN {ntlp_user} u ON r.uid = u.uid
            LEFT JOIN {users_roles} ur ON ur.uid = n.uid
            LEFT JOIN {role} role ON role.rid = ur.rid
        WHERE d.nid = %d", intval($event_nid));

    $result = db_fetch_object($rs);
    if (isset($result)) {
        return $result;
    } else {
        return false;
    }
    return $result;
}

function get_tz_recent_events() {
    global $user;

    $school_nid = get_user_school($user->uid);

    $datenow = get_tz($school_nid, 'Y-m-d', time());

    $sql = "SELECT   DATE(CONCAT(d.year,'/', d.month,'/', d.day,' ', TIME(NOW()) ))  event_date, d.nid, d.school_nid,
d.group_nid, d.year, d.month, d.day, d.visible_to, n.type, n.title, u.uid, u.first_name, u.last_name, e.event_start, e.event_end
FROM dpl_ntlp_events d
INNER JOIN {event} e ON d.nid = e.nid
INNER JOIN dpl_node n ON d.nid = n.nid
LEFT JOIN dpl_ntlp_user u ON n.uid = u.uid
LEFT JOIN dpl_og_uid og ON og.nid = d.group_nid AND og.uid = $user->uid
WHERE ((d.group_nid <> 0 AND NOT ISNULL(og.nid)) OR (d.school_nid = $school_nid AND d.visible_to = 'S') OR d.visible_to = 'N') AND
date(e.event_end) >= date('$datenow')
ORDER BY d.year, d.month, d.day, n.title LIMIT 0, 3";

    $rows = array();

    if ($result = db_query($sql)) {
        while ($row = db_fetch_object($result)) {
//    drupal_set_message($row);
            $rows[] = $row;
        }
    }

    return $rows;
}

function get_mycourses($uid) {

    $rs = db_query("SELECT n.title, c.*
            FROM {ntlp_course_user} cu
            INNER JOIN {node} n ON n.nid = cu.course_nid
            INNER JOIN {ntlp_course} c ON n.nid = c.nid
            WHERE cu.user_uid = %d
            AND c.is_project =0", intval($uid));

    return $rs;
}

function get_course($course_nid, $user_role = 0) {
    $innerJoin = '';
    $select = '';
    if ($user_role == NTLP_ROLEID_STUDENT) {
        global $user;
        $innerJoin .= ' INNER JOIN {ntlp_course_user} ncu ON ncu.course_nid = c.nid AND ncu.user_uid = ' . $user->uid;
        $select .= ', ncu.period';
    }

//TODO: Replace school_term_tid with relation to school_term table
    $rs = db_query("SELECT n.title, n.changed, nr.body body, c.*, t.name as term_name $select
            FROM {node} n
            INNER JOIN {node_revisions} nr ON nr.nid = n.nid
            INNER JOIN {ntlp_course} c ON n.nid = c.nid
            LEFT JOIN {ntlp_course_term} ct ON ct.course_nid = c.nid
            $innerJoin
            LEFT OUTER JOIN {term_data} t ON t.tid = ct.school_term_tid
            WHERE c.nid = %d
            AND c.is_project =0", $course_nid);

    if ($rs->num_rows > 0) {
        return db_fetch_object($rs);
    } else {
        return false;
    }
//    $result = db_fetch_object($rs);
//    if (isset($result)) {
//        return $result;
//    } else {
//        return false;
//    }
//    return $result;
}

function get_myschool_courses($uid) {
    $rs = db_query("SELECT n.title, c.*
            FROM {ntlp_school} s
            INNER JOIN {ntlp_course} c ON s.nid = c.school_nid
            INNER JOIN {ntlp_user} u ON s.nid = u.school_nid
            INNER JOIN {node} n ON n.nid = c.nid
            WHERE u.uid = %d
            AND c.is_project =0", intval($uid));

    return $rs;
}

function get_course_users($course_nid, $role_in_course = 0, $order="PLF") {
    $course_nid = (int) $course_nid;
    $role_in_course = (int) $role_in_course;

    if ($order == "PLF") {
        $sortorder = "r.period, u.last_name, u.first_name";
    } else {
        $sortorder = "u.last_name, u.first_name";
    }
    if ($role_in_course == 0)
        $role_in_course = NTLP_ROLEID_STUDENT;
    $rs = db_query("SELECT nu.name, r.user_uid uid, u.user_code, u.first_name, u.middle_name, u.last_name, r.period, c.teacher1_uid, c.teacher2_uid, n.title
            FROM {ntlp_course_user} r
            INNER JOIN {node} n ON n.nid = r.course_nid
            INNER JOIN {ntlp_course} c ON n.nid = c.nid
            INNER JOIN {ntlp_user} u ON r.user_uid = u.uid
            INNER JOIN {users} nu ON nu.uid = u.uid
            WHERE c.is_project = 0 AND u.is_active = 1 AND r.role_id = %d AND r.course_nid = %d
            ORDER BY %s", $role_in_course, $course_nid, $sortorder);
    return $rs;
}

function get_course_user_ids($course_nid) {
    $course_nid = (int) $course_nid;

    $rs = db_query("SELECT r.user_uid uid
            FROM {ntlp_course_user} r
            INNER JOIN {node} n ON n.nid = r.course_nid
            INNER JOIN {ntlp_user} u ON r.user_uid = u.uid
            WHERE u.is_active = 1 AND r.role_id IS NOT NULL AND r.course_nid = {$course_nid}");
    return $rs;
}

function is_course_term_used($course_nid, $term_tid) {
    $course_nid = (int) $course_nid;
    $term_tid = (int) $term_tid;

//This query returns whether an activity is used in a course term or not.
    $rs = db_query("SELECT count(*) cnt
            FROM {ntlp_course_activity} a
            WHERE a.course_nid = %d
            AND a.school_term_tid = %d", $course_nid, $term_tid);

    $result = db_fetch_object($rs);
    if (isset($result)) {
        if (intval($result->cnt) > 0)
            return true;
    }
    return false;
}

function is_any_course_term_used($course_nid) {
    $course_nid = (int) $course_nid;

    //This query returns whether an activity is used in any course term or not.
    $rs = db_query("SELECT count(*) cnt
            FROM {ntlp_course_activity} a, {ntlp_course_term} ct
            WHERE a.school_term_tid = ct.school_term_tid AND a.course_nid = %d", $course_nid);

    $result = db_fetch_object($rs);
    if (isset($result)) {
        if (intval($result->cnt) > 0)
            return true;
    }
    return false;
}

function get_stu_activities($uid, $course_nid, $y, $m, $d) {
    $uid = intval($uid);
    $course_nid = intval($course_nid);
    $y = intval($y);
    $m = intval($m);
    $d = intval($d);

//This query ignores records with NULL availability from/to dates
//In activity pages we have to make sure that hidden activities should be marked NULL
    $rs = db_query("SELECT n.title, a.*
            FROM {node} n INNER JOIN {ntlp_course_activity} a ON n.nid = a.nid
                INNER JOIN {ntlp_activity_user} au ON a.nid = au.activity_nid
            WHERE a.course_nid = %d
            AND au.user_uid = %d
            AND to_days(curdate()) >= to_days(a.available_from)
            AND to_days(curdate()) <= to_days(a.available_to)", $course_nid, $uid);

    return $rs;
}

function get_course_outcomes($course_nid) {
    $course_nid = (int) $course_nid;

    $rs = db_query("SELECT o.tid, o.name, weight_percent
        from {ntlp_gb_course_outcome} c
            INNER JOIN {term_data} o ON o.tid = c.tid
        WHERE c.course_nid = %d
        ORDER BY c.sort_order", $course_nid);
    return $rs;
}


function get_selected_course_users($activity_id) {
    $activity_id = (int) $activity_id;

    $result = db_query("SELECT user_uid FROM {ntlp_activity_user} WHERE activity_nid = %d ", $activity_id);

    while ($rs = db_fetch_object($result)) {
        $arr[$rs->user_uid] = $rs->user_uid;
    }

    return $arr;
}

function get_school_years($current_year) {
    $cy = date('Y', time());
   $i = 2010;

    if ($current_year > $cy) {
        $current_year = $cy + 1;
    }

    $years = array();
    for ($i; $i < $cy + 5; $i++) {
        $years[$i] = ($i - 1) . ' - ' . $i;
    }
    return $years;
}

function get_announcement($course_nid, $y, $m, $d) {
    $course_nid = (int) $course_nid;
    $y = (int) $y;
    $m = (int) $m;
    $d = (int) $d;

    $rs = db_query("SELECT d.nid, d.course_nid, d.year, d.month, d.day,
                r.title, r.body
            FROM {ntlp_course_announcement} d
            INNER JOIN {node_revisions} r ON d.nid = r.nid
            WHERE d.status = 1
            AND d.course_nid = %d
            ORDER BY r.timestamp DESC LIMIT 1", $course_nid);

    $result = db_fetch_object($rs);
    if (isset($result)) {
        return $result;
    } else {
        return false;
    }
}

function get_user_courses_forhomepage_orig($user_uid, $role_in_course = 0) {
    $user_uid = (int) $user_uid;
    $role_in_course = (int) $role_in_course;

    $where = '';
    if ($role_in_course == 0)
        $role_in_course = NTLP_ROLEID_STUDENT;

    $rs = db_query("SELECT   n.nid course_nid, nct.school_term_tid, n.title, ncu.period,
            (select st1.date_started from {ntlp_school_term} st1, {ntlp_course_term} ct1 where st1.school_term_tid  = ct1.school_term_tid and ct1.course_nid = n.nid order by st1.date_started limit 0,1) course_term_date_started,
            (select st1.date_closed from {ntlp_school_term} st1, {ntlp_course_term} ct1 where st1.school_term_tid  = ct1.school_term_tid and ct1.course_nid = n.nid order by st1.date_closed desc limit 0,1) course_term_date_closed
        FROM {ntlp_school_term} nst
        INNER JOIN {ntlp_course_term} nct ON nct.school_term_tid = nst.school_term_tid
        INNER JOIN dpl_node n ON n.nid = nct.course_nid
        INNER JOIN {ntlp_course_user} ncu ON ncu.course_nid = n.nid
        WHERE ncu.user_uid = %d AND ncu.role_id = %d AND n.status = 1
        GROUP BY n.nid
        HAVING course_term_date_started <= now() and course_term_date_closed >= now()", $user_uid, $role_in_course);


    return $rs;
}

function get_user_courses_forhomepage($user_uid, $role_in_course = 0) {
    //JR where course terms are  saved?
    $user_uid = (int) $user_uid;
    $role_in_course = (int) $role_in_course;

    $where = '';
    if ($role_in_course == 0)
        $role_in_course = NTLP_ROLEID_STUDENT;

    $now = get_tz(get_this_school(), 'Y-m-d', time());
    $rs = db_query("SELECT  n.nid course_nid, nct.school_term_tid, n.title, ncu.period,
            (select st1.date_started from {ntlp_school_term} st1, {ntlp_course_term} ct1 where st1.school_term_tid  = ct1.school_term_tid and ct1.course_nid = n.nid order by st1.date_started limit 0,1) course_term_date_started,
            (select st1.date_closed from {ntlp_school_term} st1, {ntlp_course_term} ct1 where st1.school_term_tid  = ct1.school_term_tid and ct1.course_nid = n.nid order by st1.date_closed desc limit 0,1) course_term_date_closed
        FROM {ntlp_course} c
        INNER JOIN {node} n ON c.nid = n.nid
        INNER JOIN {ntlp_course_term} nct ON nct.course_nid = n.nid 
        INNER JOIN {ntlp_school_term} nst ON nct.school_term_tid = nst.school_term_tid
        INNER JOIN {ntlp_course_user} ncu ON ncu.course_nid = n.nid
        WHERE ncu.user_uid = %d AND ncu.role_id = %d AND n.status = 1
        GROUP BY n.nid
        HAVING date(course_term_date_started) <= '%s' and date(course_term_date_closed) >= '%s'
        ORDER BY ncu.period", $user_uid, $role_in_course, $now, $now);


    return $rs;
}

function get_user_courses($user_uid, $role_in_course = 0, $current_term = false, $school_year = 0) {
    $user_uid = (int) $user_uid;
    $role_in_course = (int) $role_in_course;

    $where = '';
    if ($current_term) {
        $where = ' AND nst.date_started <= now() and nst.date_closed >= now() ';
//        if($r = get_current_school_term(get_this_school())) {
//            $row = db_fetch_object($r);
//            $where = ' AND nct.school_term_tid = ' . ((int)$row->school_term_tid);
//        }
    }

    if ($school_year != 0) {
        $school_year_filter = 'AND nst.school_year = ' . $school_year . ' ';
    } else {
        $school_year_filter = '';
    }

    if ($role_in_course == 0)
        $role_in_course = NTLP_ROLEID_STUDENT;

    $rs = db_query("SELECT nc.nid course_nid, nct.school_term_tid, n.title, nc.course_short_title, ncu.period, name as semester_name
                FROM `{ntlp_school_term}` nst
                INNER JOIN {ntlp_course_term} nct ON nct.school_term_tid = nst.school_term_tid
                INNER JOIN dpl_node n ON n.nid = nct.course_nid
                INNER JOIN {ntlp_course} nc ON nc.nid = n.nid
                INNER JOIN dpl_term_data td ON td.tid = nct.school_term_tid
                INNER JOIN {ntlp_course_user} ncu ON ncu.course_nid = nc.nid
                WHERE nc.is_project <> 1
                    AND ncu.user_uid = %d
                    AND ncu.role_id = %d
                $where
                $school_year_filter
                GROUP BY nc.nid", $user_uid, $role_in_course);

    return $rs;
}

//Return all the schools in which user is teaching/taking courses
function get_user_course_schools($user_uid, $role_in_course = 0, $lite = false) {
    $user_uid = (int) $user_uid;
    $role_in_course = (int) $role_in_course;

    if ($role_in_course == 0)
        $role_in_course = NTLP_ROLEID_STUDENT;
    
    if ($lite) {
        $rs = db_query("SELECT s.nid
                FROM {ntlp_course} c
                INNER JOIN {ntlp_course_user} r ON r.course_nid = c.nid
                INNER JOIN {ntlp_school} s ON (s.nid = c.school_nid OR s.is_NETWORK = 1)
                WHERE c.is_project = 0 AND r.role_id = %d AND r.user_uid = %d
                GROUP BY s.nid", $role_in_course, $user_uid);
    } else {
        $rs = db_query("SELECT sn.nid, sn.title
                FROM {node} n
                INNER JOIN {ntlp_course} c ON n.nid = c.nid
                INNER JOIN {ntlp_course_user} r ON r.course_nid = c.nid
                INNER JOIN {ntlp_school} s ON (s.nid = c.school_nid OR s.is_NETWORK = 1)
                INNER JOIN {node} sn ON s.nid = sn.nid
                WHERE c.is_project = 0 AND r.role_id = %d AND r.user_uid = %d
                GROUP BY sn.nid
                ORDER BY sn.title", $role_in_course, $user_uid);
    }
    $schools = array();
    while ($rec = db_fetch_object($rs)) {
        $schools[$rec->nid] = ($lite ? $rec->nid : $rec->title);
    }
    return $schools;
}

//Returns whether the user has access to the specified school
//Returns true/false
function user_access_to_school($school_nid) {
    $school_nid = (int) $school_nid;
    global $user;

    if (isset($user->roles[NTLP_ROLEID_TEACHER])
            or isset($user->roles[NTLP_ROLEID_SITEADMIN])
            or isset($user->roles[NTLP_ROLEID_SCHOOLLEADER])) {

        $aschools = get_user_course_schools($user->uid, NTLP_ROLEID_TEACHER, true);

//To make sure that registered school comes up in the school of enrolled classes
        $reg_sch = get_user_schoolrec($user->uid, true);
        if ($reg_sch) {
            if (!array_key_exists($reg_sch->nid, $aschools)) {
                $aschools[$reg_sch->nid] = $reg_sch->nid;
            }
            foreach ($aschools as $key) {
                if ($key == $school_nid) {
                    return true;
                }
            }
        }
    } elseif (user_access('allow access to contents of all schools')) { //If User is Staff and is enrolled in multiple school courses
        $aschools = get_schools();
        while ($rec = db_fetch_object($aschools)) {
            if ($rec->nid == $school_nid) {
                return true;
            }
        }
    } elseif (isset($user->roles[NTLP_ROLEID_STUDENT]) || isset($user->roles[NTLP_ROLEID_PARENT])) { //If User is Student || Parent
        $reg_sch = get_user_schoolrec($user->uid, true);
        if ($reg_sch) {
            if ($reg_sch->nid == $school_nid) {
                return true;
            }
        }
    }

    return false;
}

//Returns current school which user is accessing or which user has selected
//Returns school nid

function get_this_school() {
    global $user;
//    watchdog('get_school_id', 'method get_this_school $_SESSION[NTLP_THIS_SCHOOL] '. $_SESSION['NTLP_THIS_SCHOOL']);
    if (isset($_SESSION['NTLP_THIS_SCHOOL']) && $_SESSION['NTLP_THIS_SCHOOL'] > 0) {
        $school_nid = $_SESSION['NTLP_THIS_SCHOOL'];

        if (!user_access_to_school($school_nid)) {
//            if($school_nid != get_user_school($user->uid)) {
            $_SESSION['NTLP_THIS_SCHOOL'] = 0;
            return false;
//            }
        }
    } else {
//Set user's registered school as the default school
        $school_nid = get_user_school($user->uid);
        $_SESSION['NTLP_THIS_SCHOOL'] = (int) $school_nid;
    }
//echo "SCHOOL:$school_nid:";
    return (int) $school_nid;
}

function get_school_term_info($term_tid) {
    $rs = db_query("SELECT * FROM {ntlp_school_term} WHERE school_term_tid = %d", intval($term_tid));
    $rec = db_fetch_object($rs);
    return $rec;
}

function search_teacher_list($string, $user_role = 'all') {
//IMPORTANT: Get current user's school to get list of this school's students.
//If user wants to enroll user of another school, then another function will be used
//to pull the records.
    global $user;
    $school_nid = get_user_school($user->uid);

    if (strlen($string) > 0) {
        if (strtolower(trim(substr($string, 0, 1))) == 's') {
            $string = "%" . $string;     //to protect the first 's' which will be replaced by %s
//            watchdog('string', $string);
//            echo 'String:'.$string;
        }
    }
    if ($user_role == 'all') {
        $role_clause = "";
    } else {
        $role_clause = " AND r.rid = " . intval($user_role);   //NTLP_ROLEID_STUDENT."
    }
//>> Don't show Dropouts
//>> Don't show users already enrolled in this course
//>> Show users of this (or selected) school only


    $courseid = 1;
    $res = $res = db_query("SELECT r.uid, u.uid, u.name, u.mail, nu.first_name, nu.last_name
            FROM {users} u
            INNER JOIN {ntlp_user} nu ON nu.uid = u.uid
            INNER JOIN {users_roles} r ON nu.uid = r.uid
            WHERE (lower(u.name) LIKE '%" . check_plain(strtolower(trim($string))) . "%' OR
                   lower(nu.first_name) LIKE '%" . check_plain(strtolower(trim($string))) . "%' OR
                   lower(nu.last_name) LIKE '%" . check_plain(strtolower(trim($string))) . "%')" .
            $role_clause . "
                AND nu.school_nid = " . intval($school_nid) . "
            ORDER BY nu.first_name, nu.last_name");

    return $res;
}

function get_user_roles() {

    $rs = db_query("SELECT rid, name
        from {role} r
        WHERE rid > 2");    //to filter out anonymous and authenticated user

    return $rs;
}

function get_filter_user_roles() {

    $rs = db_query("SELECT rid, name
        from {role} r
        WHERE rid > 2");    //to filter out anonymous and authenticated user

    return $rs;
}

function _get_all_school_users($school_nid, $head, $limit, $filter = array()) {
    $school_nid = (int) $school_nid;
    $where = array();

    if (empty($filter['ROLES'])) {
        $filter['ROLES'] = array(NTLP_ROLEID_TEACHER, NTLP_ROLEID_SITEADMIN, NTLP_ROLEID_SITEGUEST, NTLP_ROLEID_SCHOOLLEADER, NTLP_ROLEID_PARENT, NTLP_ROLEID_STUDENT);
    }

    if (is_array($filter) and !empty($filter)) {
        if (isset($filter['ROLES']) && !empty($filter['ROLES'])) {
            $where['ROLES'] = ' r.rid IN ( ' . implode(',', $filter['ROLES']) . ')';
        }

        if (!isset($filter['STATUS'])) {
            $filter['STATUS'] = array(1);       //Filter on Active(Enabled) by default
        }
        if (isset($filter['STATUS']) && !empty($filter['STATUS'])) {
            $where['STATUS'] = " (";
//            $statuses = implode(',', $filter['STATUS']);
            foreach ($filter['STATUS'] as $status) {
                $active = intval($status) != 0 ? 1 : 0;
                $enabled = intval($status) == 1 ? 1 : 0;
                $where['STATUS'] .= ' (n.is_active = ' . $active . ' AND us.status = ' . $enabled . ') OR ';
            }
            if (strlen($where['STATUS']) > 10) {
                $where['STATUS'] = substr($where['STATUS'], 0, strlen($where['STATUS']) - 3) . ')';
            }
        }

        if (isset($filter['GRAD_YEAR']) && !empty($filter['GRAD_YEAR'])) {
            $where['GRAD_YEAR'] = '  n.grad_year IN ( ' . implode(',', $filter['GRAD_YEAR']) . ')';
        }
    }

    if ($where) {
        $where = ' AND (' . implode(' AND ', $where) . ') ';
    } else {
        $where = '';
    }


    $sql = "SELECT  n.uid,n.first_name,n.last_name,n.grad_year,n.is_active,us.login,ro.name as role_name,us.mail,us.status
        FROM {ntlp_user} n
                INNER JOIN  {users_roles} r ON n.uid = r.uid
                INNER JOIN {users} us  ON us.uid = n.uid
                INNER JOIN {role} ro  ON ro.rid = r.rid where n.school_nid = {$school_nid} $where ";

    $order_by = (isset($_GET['order']) || isset($_GET['sort'])) ? tablesort_sql($head) : ' ORDER BY n.last_name, n.first_name';
    $rs = pager_query($sql . $order_by, $limit);

    return $rs;
}

function get_last_login_date($login_date) {

    if (!empty($login_date)) {
        $date = date('n,j,Y');
        $current_date = explode(',', $date);
        $current_month = $current_date[0];
        $current_day = $current_date[1];
        $current_year = $current_date[2];

        $last_login = date('n,j,Y', $login_date);
        $login = explode(',', $last_login);
        $login_month = $login[0];
        $login_day = $login[1];
        $login_year = $login[2];

        if ($current_year != $login_year) {

            $login = date('n/j/Y', $login_date);
        } elseif (($current_year == $login_year && $current_month != $login_month) || ($current_year == $login_year && $current_month == $login_month && $current_day != $login_day)) {

            $login = date('j-M', $login_date);
        } elseif ($current_day == $login_day) {

            $login = date('g:i A', $login_date);
        }
    } else {


        $login = 'New';
    }

    return $login;
}

function _get_courses_enrollment($course_nid) {
    $course_nid = (int) $course_nid;

    if ($result = db_query(" SELECT
                    COUNT(*) as total
                 FROM
                    {ntlp_course_user}
                 WHERE
                    course_nid = %d
                 AND role_id = " . NTLP_ROLEID_STUDENT, $course_nid)) {
        $row = db_fetch_object($result);
        return $row->total;
    }

    return 0;
}

function get_all_users_role($role_ids) {
    $role_ids = check_plain($role_ids);
    $rs = db_query("SELECT * FROM {role} where rid  IN (" . $role_ids . ")");

    while ($data = db_fetch_array($rs)) {

        if ($data['name'] == 'anonymous user' || $data['name'] == 'authenticated user') {
            
        } else {
            $roles [$data['rid']] = $data['name'];
        }
    }
    return $roles;
}

function _get_users_role($role_ids) {

    $role_ids = check_plain($role_ids);
    $rs = db_query("SELECT * FROM {role} where rid  IN (" . $role_ids . ")");

    while ($data = db_fetch_array($rs)) {

        if ($data['name'] == 'anonymous user' || $data['name'] == 'authenticated user') {
            
        } else {
            $user_roles .= '<option value="' . $data['rid'] . '">' . $data['name'] . '</option>';
        }
    }
    return $user_roles;
}

function get_school_details($school_nid) {
    $school_nid = (int) $school_nid;

    $rs = db_query("SELECT n.title,s.location_tid,t.name,f.filepath  FROM {node} n INNER JOIN
                {ntlp_school} s ON n.nid = s.nid
                LEFT JOIN {term_data} t ON t.tid = s.location_tid
                LEFT JOIN {files} f ON f.fid = s.fid WHERE n.nid = %d", $school_nid);

    return db_fetch_object($rs);
}

function get_school_grad_years($current_year) {
    $cy = date('Y', time()) + 8;
    $years = array();

    if ($current_year == null || empty($current_year) || intval($current_year) == 0) {
        $current_year = date('Y', time());
    } else if (intval($current_year) > intval(date('Y', time()))) {
        $current_year = date('Y', time());
    }

    for ($current_year; $current_year < $cy; $current_year++) {
        $years[$current_year] = ($current_year);
    }
    return $years;
}

function get_ntlp_school_term($school_id, $by_school_year = false, $term_tid=-1) {
    $school_id = (int) $school_id;

    $where = '';
    $where .= $by_school_year ? ' AND school_year = ' . intval(get_this_school_year()) : '';
    $where .= $term_tid != -1 ? ' AND tid = ' . $term_tid : '';


    $school_term = array();
//    watchdog('testQuery', "SELECT * FROM {ntlp_school_term} WHERE school_nid = $school_id $where");

    $result = db_query("SELECT *   FROM {ntlp_school_term} st
            INNER JOIN {term_data} td ON td.tid = st.school_term_tid 
            WHERE st.school_nid = %d $where", $school_id);

    while ($note = db_fetch_object($result)) {

        $school_term[] = $note;
    }

    return $school_term;
}

function get_school_term($school_id) {
    $school_id = (int) $school_id;

    $result = db_query("SELECT st.school_term_tid, t.name FROM {ntlp_school_term} st
                        INNER JOIN {term_data} t ON st.school_term_tid = t.tid  WHERE st.school_nid = %d", $school_id);

    while ($note = db_fetch_array($result)) {

        $school_term[$note['school_term_tid']] = $note['name'];
    }

    return $school_term;
}

function get_school_term_data($term_tid) {

    $result = db_query("SELECT st.*, t.name
        FROM {ntlp_school_term} st
        INNER JOIN {term_data} t ON st.school_term_tid = t.tid
        WHERE st.school_term_tid = %d", $term_tid);
    if ($result) {
        return db_fetch_object($result);
    } else {
        return false;
    }
}

function is_term_in_course($course_nid, $school_term_tid) {
    $result = db_query("SELECT 1
            FROM {ntlp_course_term} ct
            WHERE ct.course_nid = %d
            AND ct.school_term_tid = %d", $course_nid, $school_term_tid);
    if ($result->num_rows > 0) {
        return true;
    } else {
        return false;
    }
}

function is_term_in_school($school_nid, $school_term_tid) {
    $sql = "SELECT 1
            FROM {ntlp_school_term} st
            WHERE st.school_nid = $school_nid
            AND st.school_term_tid = $school_term_tid AND st.school_year = " . get_this_school_year();

//    watchdog('term_in_sch', 'is_term_in_school() method '.$sql);
    $result = db_query($sql);
    if ($result->num_rows > 0) {
//        watchdog('term_in_sch', 'is_term_in_school() return true ');
        return true;
    } else {
//        watchdog('term_in_sch', 'is_term_in_school() return  false ');
        return false;
    }
}

function get_current_course_term($course_id) {
    $result = db_query("SELECT ct.school_term_tid, t.name FROM {ntlp_school_term} st
            INNER JOIN {ntlp_course_term} ct ON ct.school_term_tid = st.school_term_tid
            INNER JOIN {term_data} t ON st.school_term_tid = t.tid
            INNER JOIN {ntlp_course} c ON c.nid = ct.course_nid
            INNER JOIN {ntlp_school} s ON st.school_nid = s.nid
            WHERE st.date_started <= now()
            AND ct.course_nid = %d
            ORDER BY st.date_started desc LIMIT 0,1", $course_id);
//                        AND st.date_started <= now() and st.date_closed >= now()",

    return $result;
}

function get_current_school_term($school_id, $school_year = 0) {
    $school_id = (int) $school_id;
    if ($school_year == 0) {
        $school_year = get_this_school_year();
    }
    $result = db_query("SELECT st.*, t.name FROM {ntlp_school_term} st
                    INNER JOIN {term_data} t ON st.school_term_tid = t.tid
		    INNER JOIN {ntlp_school} s ON st.school_nid = s.nid
                    WHERE st.school_nid = %d AND st.school_year = " . $school_year . "
                    AND st.date_started <= now()
                    ORDER BY st.date_started desc LIMIT 0,1", $school_id);

//                        AND st.date_started <= now() and st.date_closed >= now()",

    return $result;
}

function get_myschool_term_courses($school_nid, $term_id) {
    $school_nid = (int) $school_nid;
    $term_id = check_plain($term_id);

    $rs = db_query("SELECT n.title, c.*
            FROM {ntlp_school} s
            INNER JOIN {ntlp_course} c ON s.nid = c.school_nid
            INNER JOIN {node} n ON n.nid = c.nid
            INNER JOIN {ntlp_course_term} ct ON ct.course_nid = c.nid
          Where   c.is_project =0 AND ct.school_term_tid  IN (" . $term_id . ") AND c.school_nid = %d", $school_nid);

    return $rs;
}

function get_myschool_allterms_courses($school_nid) {

    $rs = db_query("SELECT n.title, c.*
            FROM {ntlp_course} c
            INNER JOIN {node} n ON n.nid = c.nid
          Where c.is_project =0 AND c.school_nid = %d", $school_nid);

    return $rs;
}

function export_csv_data($course_id, $term_id) {
    $course_id = (int) $course_id;


    $rs = db_query("SELECT ac.nid, ac.course_nid, ct.school_term_tid, t.name, c.teacher1_uid,
            au.user_uid as student_uid, aui.first_name, aui.middle_name, aui.last_name
        FROM {ntlp_course} c
        INNER JOIN dpl_node n ON n.nid = c.nid
        INNER JOIN {ntlp_course_term} ct  ON ct.course_nid = c.nid
        INNER JOIN dpl_term_data t ON t.tid = ct.school_term_tid
        INNER JOIN dpl_ntlp_course_activity ac ON c.nid = ac.course_nid AND ac.school_term_tid = ct.school_term_tid
        INNER JOIN dpl_ntlp_activity_user au ON au.activity_nid = ac.nid
        INNER JOIN dpl_ntlp_user aui ON au.user_uid = aui.uid
        INNER JOIN {ntlp_course_user} cu ON cu.course_nid = c.nid AND cu.user_uid = au.user_uid
        WHERE c.nid = %d AND ct.school_term_tid = %d
            AND cu.role_id = 5
        ORDER BY n.title, ac.timecreated, ac.nid", $course_id, $term_id);

    return $rs;
}

function _get_outcomes_names($school_nid, $school_year) {

    $rs = db_query("select t.name
        FROM {term_data} t
        INNER JOIN {ntlp_gb_outcome} gb ON gb.tid = t.tid 
        WHERE gb.school_nid = %d AND gb.school_year = %d
        ORDER BY gb.sort_order", $school_nid, $school_year);

    while ($data = db_fetch_array($rs)) {
        $csv_output .= $data['name'] . ',';
    }

    return $csv_output;
}

function _get_vocublary_names($name) {

    $rs = db_query("select t.name,t.tid FROM {vocabulary} v
        INNER JOIN {term_data} t ON t.vid = v.vid WHERE v.name = '%s'", $name);
    while ($data = db_fetch_array($rs)) {

        $vocub[$data['tid']] = $data['name'];
    }

    return $vocub;
}

function get_term_record($school_year, $school_id) {

    $rs = db_query("select s.current_school_year,st.* ,t.name,t.tid from {ntlp_school} s
                 INNER JOIN {ntlp_school_term} st ON st.school_nid = s.nid
                INNER JOIN {term_data} t ON t.tid = st.school_term_tid
                where st.school_year = %d and s.nid = %d", $school_year, $school_id);

    while ($data = db_fetch_array($rs)) {
        $row[] = $data;
    }
    return $row;
}

function get_term_outcome_record($school_id, $school_year) {

    $rs = db_query("SELECT ot.*, t.name, t.description
        FROM {ntlp_gb_outcome} ot
        INNER JOIN {term_data} t ON t.tid = ot.tid
        INNER JOIN {ntlp_school} s ON s.nid = ot.school_nid
        WHERE ot.school_nid = %d AND ot.school_year = %d
        ORDER BY ot.sort_order ASC", $school_id, $school_year);

    while ($data = db_fetch_array($rs)) {
        $row[] = $data;
    }

    return $row;
}

function get_label_outcomes($school_nid, $school_year) {

    $rs = db_query("select * from {ntlp_school_grade} where school_nid = %d 
        AND school_year = %d ", $school_nid, $school_year);

    $row = array();

    while ($data = db_fetch_array($rs)) {


        $row[] = $data;
    }
    return $row;
}

function get_user_school_info($user_id) {
    $result = db_query("SELECT u.first_name, u.last_name, n.title as school_name, ns.nid as school_nid,
            ns.gmail_status, ns.gdocs_status, ns.gsite_status
        FROM {ntlp_user} u
        INNER JOIN {ntlp_school} ns ON u.school_nid = ns.nid
        INNER JOIN {node} n ON ns.nid = n.nid
        WHERE u.uid = %d ", $user_id);

    return db_fetch_object($result);
}

function get_school_student_list($school_nid) {

    $rs = db_query("SELECT u.user_code, u.first_name, u.middle_name, u.last_name, u.grad_year, r.name, dr.status, dr.mail
            FROM {ntlp_user} u
            INNER JOIN {users_roles} ur ON ur.uid = u.uid
            INNER JOIN {role} r ON r.rid = ur.rid
            INNER JOIN {users} dr ON dr.uid = u.uid
            WHERE u.school_nid =  %d", $school_nid);

    return $rs;
}

function get_all_roles() {

    $rs = db_query("SELECT rid, name FROM {role}");
    while ($data = db_fetch_object($rs)) {

        $options .= '<option value = "' . $data->rid . '">' . $data->name . '</option>';
    }
    return $options;
}

function get_school_record($school_id) {

    $rs = db_query("SELECT s.*,n.* FROM {ntlp_school} s
                    INNER JOIN {node} n ON n.nid = s.nid where s.nid = %d ", $school_id);

    return db_fetch_object($rs);
}

function _get_fid_detail($fid) {
    $result = db_query("SELECT * FROM {files} WHERE fid = %d", $fid);
    if ($result) {
        return db_fetch_object($result);
    } else {
        return false;
    }
}

function get_selected_school_permissions($school_nid, $role_id, $perm_name) {

    $perm = ' ' . $school_nid . ' ' . $perm_name;

    $rs = db_query("SELECT pid FROM {permission} WHERE rid = %d AND FIND_IN_SET('%s', perm)", $role_id, $perm);
    if ($rs->num_rows > 0) {
        return 1;
    } else {

        return 0;
    }
}

//get_parent_child(get_this_school(),array('class'=> 'my-dropdown')), $HTMLSchoolYearCombo);

function get_parent_child() {
    static $isFunctionAlreadyCreated = false;
    global $user;
    $onchange = '';

    $return_combo = array('class' => 'my-dropdown');

    $result = db_query("SELECT * FROM dpl_ntlp_user_parent up
        INNER JOIN dpl_ntlp_user nu ON nu.uid = up.student_uid
        WHERE up.parent_uid = %d", $user->uid);

    if ($result->num_rows > 1) {
        $attrs = '';
        if (!isset($return_combo['onchange'])) {
            drupal_add_js("
                    function change_parent_student(obj){
                        $.post('?q=ntlp/change/parent/user/'+obj.value, function(){
                            window.location.reload();
                        });
                    }
                ", 'inline');

            $onchange = ' onchange = "change_parent_student(this);" ';
            $isFunctionAlreadyCreated = true;
        }
    }
    if ($result->num_rows > 1) {
        if (is_array($return_combo)) {
            foreach ($return_combo as $attr => $value) {
                $attrs .= " $attr = \"$value\" ";
            }
            $rows = "<select $attrs $onchange>";
        } else {
            $rows = array();
        }
    }

    if ($result) {
        $this_student_uid = get_current_parent_student();
        while ($row = db_fetch_object($result)) {
            if (!isset($_SESSION['PARENT_STUDENT_UID'])) {
                $_SESSION['PARENT_STUDENT_UID'] = $row->student_uid;
                $this_student_uid = $row->student_uid;
            }
            if ($result->num_rows == 1) {
                $rows = $row->first_name . ' ' . $row->last_name;
            } else if ($result->num_rows > 1) {
                $selected = $this_student_uid == $row->student_uid ? ' selected' : '';
                $rows .= '<option value="' . $row->student_uid . '" ' . $selected . '>' . $row->first_name . ' ' . $row->last_name . '</option>';
            } else {
                $rows[$row->student_uid] = $row->first_name . ' ' . $row->last_name;
//                $rows[$row->school_year]= $row->school_year;
            }
        }
    }

    if ($return_combo) {
        $rows .= '</select>';
    }
    return $rows;
}

function get_school_years_query($id, $isIdStudent = false) {
    global $user;

    if ($isIdStudent) {
        $sql = 'SELECT DISTINCT nc.school_year FROM {ntlp_course_user} ncu INNER JOIN {ntlp_course} nc ON nc.nid = ncu.course_nid WHERE ncu.user_uid = %d AND ncu.role_id = %d AND nc.school_year > 0 ORDER BY nc.school_year ASC';
        $result = db_query($sql, $id, NTLP_ROLEID_STUDENT);
    } else {
        $sql = "SELECT DISTINCT st.school_year
            FROM {ntlp_school_term} st
            INNER JOIN {ntlp_course} c ON st.school_nid = c.school_nid
            INNER JOIN {ntlp_course_user} cu ON cu.course_nid = c.nid AND cu.user_uid = %d
            WHERE st.school_year > 0
            UNION (SELECT cst.school_year FROM {ntlp_school_term} cst WHERE cst.school_nid = %d)";
//        $sql = 'SELECT DISTINCT school_year FROM {ntlp_school_term}
//            WHERE school_year > 0 AND (school_nid IN (
//              SELECT DISTINCT c.school_nid FROM {ntlp_course} c, {ntlp_course_user} cu
//                WHERE cu.course_nid = c.nid AND user_uid = %d)
//              OR school_nid = %d)
//            UNION (
//            SELECT YEAR(NOW()) as school_year)
//            ORDER BY school_year DESC';

        $result = db_query($sql, $user->uid, $id);
    }

    $rows = array();
    while ($data = db_fetch_object($result)) {
        $rows[$data->school_year] = $data->school_year;
    }
    //Add current year, if not exist
    $rows[date('Y', time())] = date('Y', time());
    //Sort descending
    krsort($rows);
    return $rows;
}

function get_school_years_by_id($id, $return_combo = array(), $isIdStudent = false) {
    static $isFunctionAlreadyCreated = false;
    global $user;
    $onchange = '';

    if ($return_combo) {
        $attrs = '';
        if (!isset($return_combo['onchange']) && !$isFunctionAlreadyCalled) {
            drupal_add_js("
                    function change_school_year(obj){
                        $.post('?q=ntlp/change/school/year/'+obj.value, function(){
                            window.location.reload();
                        });
                    }
                ", 'inline');

            $onchange = ' onchange = "change_school_year(this);" ';
            $isFunctionAlreadyCreated = true;
        }

        if (is_array($return_combo)) {
            foreach ($return_combo as $attr => $value) {
                $attrs .= " $attr = \"$value\" ";
            }
        }
        $rows = "<select $attrs $onchange>";
    } else {
        $rows = array();
    }

    $result = get_school_years_query($id, $isIdStudent);
    if ($result) {
        $this_school_year = get_this_school_year();
        foreach ($result as $y) {
            if ($return_combo) {
                $selected = $this_school_year == $y ? ' selected' : '';
                $rows .= '<option value="' . $y . '" ' . $selected . '>' . ($y - 1) . '-' . $y . '</option>';
            } else {
                $rows[$y] = ($y - 1) . '-' . $y;
            }
        }
    }

    if ($return_combo) {
        $rows .= '</select>';
    }

    return $rows;
}

function render_this_error_page($message) {
    global $base_path;
    include "error_page.template.inc";

    $content = $HTML_ERRORPAGE_container;

    $last_state = explode('?q=', $_SERVER['HTTP_REFERER']);
    $content = str_replace('@MESSAGE', $message, $content);
    $content = str_replace('@ROOT', $base_path, $content);
    $content = str_replace('@BACKURL', '<a href="' . url($last_state[1]) . '"> Back to Page</a>', $content);

    return $content;
}

//function check_user_permission($node_nid, $user_id = 0) {
function check_user_permission($node_nid) {
    global $user;
    $flag = false;

    $tmp_node_nid = $node_nid;
    if (is_object($node_nid)) {
        $node = $node_nid;
        $tmp_node_nid = $node->nid;
    }
//    watchdog('node_value', print_r($node, true));
//    watchdog('chk_permission','check_user_permission method start user role '. print_r($user->roles, true).' node id '.$node_nid);
    if ($user->uid == 1) {
        return NTLP_ROLEID_NETWORKMANAGER;
    } else if (isset($user->roles[NTLP_ROLEID_NETWORKSTAFF])) {
        return NTLP_ROLEID_NETWORKSTAFF;
    } else if (isset($user->roles[NTLP_ROLEID_NETWORKMANAGER]) || isset($user->roles[NTLP_ROLEID_SYSTEM_ADMIN])) {
        return NTLP_ROLEID_NETWORKMANAGER;
    }

//    $result = ntlp_get_user_role($user->uid, $tmp_node_nid);
//
//    if ($result == false) {

    if (is_object($node_nid)) {
        $node = $node_nid;
        $node_nid = $node->nid;
    } else {
        $node = node_load($node_nid);
    }

    $user_in_current_school = check_user_school($user);

//    $node = node_load($node_nid);
//    watchdog("chk_perm", 'checking permission for node type ' . $node->type);
    switch ($node->type) {

        case 'ntlp_course':
            $records = is_user_access_this_course($node, $user);
//            watchdog('ntlp_permis', 'checking course permission for user ' . $user->uid . '<br>user role in course ' . $records);
            if ($records != false) {
                //set user permission in db
                ntlp_set_user_access($user->uid, $node->nid, $records);
                return $records;
            } else {
//                watchdog('ntlp_permis', 'user role in system ' . print_r($user->roles, true) . ' user school ' . $user_in_current_school);
                if (isset($user->roles[NTLP_ROLEID_TEACHER]) && $user_in_current_school) { // if is a teacher.. and does not
                    /* Teacher should not be able to get into the courses of other schools from any
                     * place in the application for this current version */
                    if (get_this_school() == get_course_school_nid($node->nid)) {
//                        watchdog('ntlp_permis', 'user is in this school: '.get_this_school().', Course school:'.get_course_school_nid($node->nid));
                        //set user permission in db
                        ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_SITEGUEST);
                        return NTLP_ROLEID_SITEGUEST; // have any role in current course then his is siteguest
                    } else {
                        drupal_goto('ntlp/render/error/page/Sorry, you are not authorized to access courses from another school', array('destination' => $_GET['q']));
                    }
                } else if (isset($user->roles[NTLP_ROLEID_SITEADMIN]) && $user_in_current_school) {
                    /* Site Admin should not be able to get into the courses of other schools from any
                     * place in the application for this current version */
                    if (get_this_school() == get_course_school_nid($node->nid)) {
                        //set user permission in db
                        ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_TEACHER);
                        return NTLP_ROLEID_TEACHER;
                    } else {
                        drupal_goto('ntlp/render/error/page/Sorry, you are not authorized to access courses from another school', array('destination' => $_GET['q']));
                    }
                } else if (isset($user->roles[NTLP_ROLEID_SCHOOLLEADER]) && $user_in_current_school) {
                    /* School Leader should not be able to get into the courses of other schools from any
                     * place in the application for this current version */
                    if (get_this_school() == get_course_school_nid($node->nid)) {
                        //set user permission in db
                        ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_TEACHER);
                        return NTLP_ROLEID_TEACHER;
                    } else {
                        drupal_goto('ntlp/render/error/page/Sorry, you are not authorized to access courses from another school', array('destination' => $_GET['q']));
                    }
                } else if (isset($user->roles[NTLP_ROLEID_PARENT]) && check_parent_kids_course($user, $node)) {
//                    watchdog('ntlp_course_perm', 'checking parent permission for user ' . $user->uid . ' and');
                    //set user permission in db
                    ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_SITEGUEST);
                    return NTLP_ROLEID_SITEGUEST;
                } else {
                    drupal_goto('ntlp/render/error/page/Sorry, you are not authorized to access this Course', array('destination' => $_GET['q']));
                }
            }
            break;

        case 'ntlp_project':
//            watchdog('project_perm', 'checking course project permission for user ' . $user->uid);
            $records = is_user_access_this_project($node, $user);
            if ($records) {
                //set user permission in db
                ntlp_set_user_access($user->uid, $node->nid, $records);
                return $records;
            } else {
                if (isset($user->roles[NTLP_ROLEID_TEACHER]) && $user_in_current_school) {

                    //set user permission in db
                    ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_SITEGUEST);
                    return NTLP_ROLEID_SITEGUEST;
                } else if (isset($user->roles[NTLP_ROLEID_SITEADMIN]) && $user_in_current_school) {
                    //set user permission in db
                    ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_TEACHER);
                    return NTLP_ROLEID_TEACHER;
                } else if (isset($user->roles[NTLP_ROLEID_SCHOOLLEADER]) && $user_in_current_school) {
                    //set user permission in db
                    ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_TEACHER);
                    return NTLP_ROLEID_TEACHER;
                } else if (isset($user->roles[NTLP_ROLEID_PARENT]) && check_parent_kids_project($user, $node)) {
                    //set user permission in db
                    ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_SITEGUEST);
                    return NTLP_ROLEID_SITEGUEST;
                } else {
                    drupal_goto('ntlp/render/error/page/You are Not authorized to see this Project page', array('destination' => $_GET['q']));
                }
            }
            break;

        case 'ntlp_school':

//            watchdog("school_perm", 'School nid ' . $node->nid . ' user uid ' . $user->uid);
            $records = is_user_access_school_contens($node, $user);
            if ($records) {
                //set user permission in db
                ntlp_set_user_access($user->uid, $node->nid, $records);
                return $records;
            } else {

                drupal_goto('ntlp/render/error/page/Contents inaccessible, you are not enrolled in any School please Contact with Site Administrator', array('destination' => $_GET['q']));
            }
            break;

        case 'lo_image':
        case 'lo_document':
        case 'lo_video':
        case 'lo_page':
        case 'lo_link':
        case 'lo_share_idea':
            $perm = check_resource_permission($node, $user);
            if (!$perm) {
                drupal_goto('ntlp/render/error/page/You are not authorized to see this resource', array('destination' => $_GET['q']));
            } else {
                //set user permission in db
                ntlp_set_user_access($user->uid, $node->nid, $perm);

                return $perm;
            }
            break;

        case 'lo_workshop':
            break;

        case 'lo_task':
            break;

        case 'lo_journal':
            break;

        case 'ntlp_group_library_schooldoc':
//            watchdog("check_perm", 'ntlp_group_library_schooldoc' . print_r($node, true));
            $records = is_user_access_school_resource_library($node, $user);
            if ($records) {
                //set user permission in db
                ntlp_set_user_access($user->uid, $node->nid, $records);
                return $records;
            } else if (isset($user->roles[NTLP_ROLEID_SITEADMIN]) || isset($user->roles[NTLP_ROLEID_SCHOOLLEADER])) {
                //set user permission in db
                ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_SITEADMIN);
                return NTLP_ROLEID_SITEADMIN;
            } else if (isset($user->roles[NTLP_ROLEID_PARENT])) {
                //set user permission in db
                ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_STUDENT);
                return NTLP_ROLEID_STUDENT;
            } else {
                drupal_goto('ntlp/render/error/page/You are not authorized to see this School Resource Library', array('destination' => $_GET['q']));
            }
            break;

        case 'ntlp_group_library_ntndoc':

            if (isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])) {
                //set user permission in db
                ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_NETWORKMANAGER);
                return NTLP_ROLEID_NETWORKMANAGER;
            } else if (isset($user->roles[NTLP_ROLEID_NETWORKSTAFF])) {
                //set user permission in db
                ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_NETWORKSTAFF);
                return NTLP_ROLEID_NETWORKSTAFF;
            } else if (isset($user->roles[NTLP_ROLEID_SCHOOLLEADER])) {
                //set user permission in db
                ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_SCHOOLLEADER);
                return NTLP_ROLEID_SCHOOLLEADER;
            } else if (isset($user->roles[NTLP_ROLEID_TEACHER])) {
                //set user permission in db
                ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_TEACHER);
                return NTLP_ROLEID_TEACHER;
            } else if (isset($user->roles[NTLP_ROLEID_SITEADMIN])) {
                //set user permission in db
                ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_SITEADMIN);
                return NTLP_ROLEID_SITEADMIN;
            } else if (isset($user->roles[NTLP_ROLEID_NETWORKGUEST]) || isset($user->roles[NTLP_ROLEID_PARENT])
                    || isset($user->roles[NTLP_ROLEID_STUDENT])) {

                drupal_goto('ntlp/render/error/page/You are not authorized to see this NETWORK Resource Library', array('destination' => $_GET['q']));
            }

            break;

        case 'ntlp_group_library_project':

            if (isset($user->roles[NTLP_ROLEID_TEACHER])) {
                //set user permission in db
                ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_TEACHER);
                return NTLP_ROLEID_TEACHER;
            } else if (isset($user->roles[NTLP_ROLEID_DIRECTORS])) {
                //set user permission in db
                ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_DIRECTORS);
                return NTLP_ROLEID_DIRECTORS;
            } else if (isset($user->roles[NTLP_ROLEID_SITEADMIN])) {
                //set user permission in db
                ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_SITEADMIN);
                return NTLP_ROLEID_SITEADMIN;
            } else if (isset($user->roles[NTLP_ROLEID_SCHOOLLEADER])) {
                //set user permission in db
                ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_SCHOOLLEADER);
                return NTLP_ROLEID_SCHOOLLEADER;
            } else if (isset($user->roles[NTLP_ROLEID_NETWORKSTAFF])) {
                //set user permission in db
                ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_NETWORKSTAFF);
                return NTLP_ROLEID_NETWORKSTAFF;
            } else if (isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])) {
                //set user permission in db
                ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_NETWORKMANAGER);
                return NTLP_ROLEID_NETWORKMANAGER;
            } else {
                drupal_goto('ntlp/render/error/page/You are not authorized to access Project Library', array('destination' => $_GET['q']));
            }
            break;

        case 'ntlp_group':

//            $isGroupAdminRole = is_user_group_admin($node_nid, $user);
//            $groupDetails = get_group_info($node_nid);
//            $userSchool = get_this_school();
// USER IS ADMIN AND EXISTS IN THE SAME SCHOOL
            $record = is_user_group_member($node_nid, $user);
            if ($record === false) {     //Not member
                drupal_goto('ntlp/render/error/page/You are not authorized to access this group.', array('destination' => $_GET['q']));
            } else if ($record == 1) {          //Admin role
                //set user permission in db
                ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_TEACHER);
                return NTLP_ROLEID_TEACHER;
            } else {
                return NTLP_ROLEID_SITEGUEST;
                //set user permission in db
                ntlp_set_user_access($user->uid, $node->nid, NTLP_ROLEID_SITEGUEST);
            }
            break;

    }
}

function check_parent_kids_project($user, $node) {

    $flag = false;

    $result = db_query("SELECT * FROM dpl_ntlp_user_parent up
        INNER JOIN {ntlp_course_user} cu ON cu.user_uid = up.student_uid
        INNER JOIN {ntlp_course} c ON c.course_nid = cu.course_nid
        WHERE up.parent_uid = %d AND c.nid = %d;", $user->uid, $node->nid);

    if ($result->num_rows > 0) {
        $flag = true;
    } else {
        $flag = false;
    }
    return $flag;
}

function check_parent_kids_course($user, $node) {

    $flag = false;

    $result = db_query("SELECT up.student_uid FROM {ntlp_user_parent} up
        INNER JOIN {ntlp_course_user} cu ON cu.user_uid = up.student_uid
        WHERE up.parent_uid = %d AND cu.course_nid = %d", $user->uid, $node->nid);

    if ($result->num_rows > 0) {
        $flag = true;
    } else {
        $flag = false;
    }
    return $flag;
}

function check_user_school($user) {
    $flag = false;

    $school_nid = get_this_school();
//    watchdog('check_school',  'is user exist in school '. $school_nid);
    $result = db_query("SELECT 1 FROM {ntlp_user} nu
        WHERE uid = %d AND school_nid = %d", $user->uid, $school_nid);

    if ($result->num_rows > 0) {
//        $data = db_fetch_object($result);
//        watchdog('check_school', ' current user school '.$data->school_nid);
        $flag = true;
    } else {
        $flag = false;
    }
//    watchdog('check_school', ' check user school '.$flag);
    return $flag;
}

function is_user_access_school_resource_library($node, $user) {
    $flag = false;

    $result = db_query("SELECT * FROM {ntlp_module} m
        INNER JOIN {ntlp_user} u ON u.school_nid = m.school_nid
        INNER JOIN {users_roles} r ON r.uid = u.uid
        WHERE m.nid = %d AND u.uid = %d  ", $node->nid, $user->uid);

    if ($result->num_rows > 0) {
        $rec = db_fetch_object($result);
        $flag = $rec->rid;
    } else {
        $flag = false;
    }

    return $flag;
}

function is_user_access_this_resource($node, $user) {

    $flag = false;

    $course_record = is_resource_exist_in_course($node, $user);
    if ($course_record) {
        $flag = $course_record;

        if (isset($user->roles[NTLP_ROLEID_SITEGUEST]) || isset($user->roles[NTLP_ROLEID_STUDENT]) || isset($user->roles[NTLP_ROLEID_PARENT])) {
// after checking in course.. checking in project
            if (check_resource_exist_in_project($node)) {
                $project_record = is_resource_exist_in_project($node, $user);

                if ($project_record) { // if resource exist in project check hidden for student. and other roles
                    $flag = $project_record;
                } else {
                    $flag = false;
                }
            }
        }
    } else {
        $project_record = is_resource_exist_in_project($node, $user);
        if ($project_record) {
            $flag = $project_record;
        } else {

            $group_record = is_resource_exist_in_group($node, $user);
//            watchdog('chk_res_grp', print_r($group_record, true));
            if ($group_record) {
                $flag = $group_record;
            }
        }
    }// need to check for gropus.. og_uid
//    watchdog('resouce_aces', 'flag value '.$flag);
    return $flag;
}

function check_resource_exist_in_project($node) {
    $flag = false;

    $result = db_query("SELECT * FROM {ntlp_project_resource} pr
        WHERE nid = %d", $node->nid);

    if ($result->num_rows > 0) {
        $flag = true;
    } else {
        $flag = false;
    }

    return $flag;
}

function is_resource_exist_in_course($node, $user) {

    $result = db_query("SELECT * FROM {ntlp_course_resource} cr
        INNER JOIN {ntlp_course_user} cu ON cu.course_nid = cr.course_nid
        WHERE cr.nid = %d AND cu.user_uid = %d", $node->nid, $user->uid);

    if ($result->num_rows > 0) {
        $rec = db_fetch_object($result);
        $flag = $rec->role_id;
    } else {
        $flag = false;
    }
    return $flag;
}

function is_resource_exist_in_group($node, $user) {

    $result = db_query("SELECT cr.course_nid FROM {ntlp_course_resource} cr
                WHERE cr.nid = %d ", $node->nid);
//    watchdog('chk_res_db', print_r($result, true));
    if ($result->num_rows > 0) {
        $rec = db_fetch_object($result);
        $flag = is_user_group_member($rec->course_nid, $user);
    } else {
        $flag = false;
    }

    return $flag;
}

function is_resource_exist_in_project($node, $user) {

    $result = db_query("SELECT pr.availability as resource_availability, cu.role_id,
        cs.availability as section_availability FROM {ntlp_project_resource} pr
        INNER JOIN {ntlp_course_resource} cr ON cr.nid = pr.nid
        INNER JOIN {ntlp_course_user} cu ON cu.course_nid = cr.course_nid
        INNER JOIN {ntlp_course_section} cs ON cs.tid = pr.section_tid
        WHERE pr.nid = %d AND cu.user_uid = %d", $node->nid, $user->uid);

    if ($result->num_rows > 0) {

        while ($rec = db_fetch_object($result)) {

//            watchdog('proj_res_1', print_r($rec, true));
            if ($rec->section_availability == 'H' || $rec->resource_availability == 'H') {

                if ($rec->role_id == NTLP_ROLEID_TEACHER) {
                    $flag = $rec->role_id;
                } else {
                    $flag = false;
                    break;
                }
            } else {

                $flag = $rec->role_id;
            }
        }
    } else {
        $flag = false;
    }
//    watchdog('proj_res_2', 'flag value ' . $flag);
    return $flag;
}

function is_user_access_this_library_project($node, $user) {
    $flag = false;

//    print_r($user);
//    if(isset($user->roles[NTLP_ROLEID_TEACHER])) {
    if (isset($user->roles[NTLP_ROLEID_TEACHER])) {
        $flag = true;
    } else if (isset($user->roles[NTLP_ROLEID_DIRECTORS])) {
        $flag = true;
    } else if (isset($user->roles[NTLP_ROLEID_SITEADMIN])) {
        $flag = true;
    } else if (isset($user->roles[NTLP_ROLEID_SCHOOLLEADER])) {
        $flag = true;
    } else if (isset($user->roles[NTLP_ROLEID_NETWORKSTAFF])) {
        $flag = true;
    } else if (isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])) {
        $flag = true;
    }
    return $flag;
}

function is_user_access_school_contens($node, $user) {
    $flag = false;

    if ($node->nid > 0) {
        $result = db_query("SELECT * FROM {users_roles} r
        INNER JOIN {ntlp_user} u ON u.uid = r.uid
        WHERE r.uid = %d AND u.school_nid = %d", $user->uid, $node->nid);

        if ($result->num_rows > 0) {
            $rec = db_fetch_object($result);
            $flag = $rec->rid;
        } else {
            $flag = false;
        }
    } else {
        $flag = false;
    }


    return $flag;
}

function is_user_access_this_course($node, $user) {
    $flag = false;

    $result = db_query("SELECT cu.role_id FROM {ntlp_course} c
        INNER JOIN {ntlp_course_user} cu ON cu.course_nid = c.nid
        WHERE c.nid = %d AND c.is_project = 0 AND cu.user_uid = %d", $node->nid, $user->uid);

    if ($result->num_rows > 0) {
        $record = db_fetch_object($result);
        $flag = $record->role_id;
    } else {
        $flag = false;
    }
    return $flag;
}

function is_user_access_this_project($node, $user) {
    $flag = false;

//    $school_nid = get_this_school();

    $result = db_query("SELECT c.*, cu.* FROM {ntlp_course} c
        INNER JOIN {ntlp_course_user} cu ON cu.course_nid = c.course_nid
        WHERE c.nid = %d AND c.is_project = 1 AND cu.user_uid = %d", $node->nid, $user->uid);


//    echo 'project permission '.$school_nid .'node nid '.$node->nid .' user id '.$user->uid .'<br>';
//    print_r($result);
    if ($result->num_rows > 0) {
        $record = db_fetch_object($result);
        $flag = $record->role_id;
    } else {
        $flag = false;
    }
    return $flag;
}

function get_user_record($user_id) {

    $rs = db_query("SELECT u.*,nu.*, r.* FROM {ntlp_user} u
          INNER JOIN {users} nu  ON nu.uid = u.uid
          INNER JOIN {users_roles} r ON r.uid = u.uid
          WHERE u.uid  = %d", $user_id);

    return db_fetch_object($rs);
}

function get_user_record_byname($login_name) {

    $rs = db_query("SELECT u.*, nu.*
        FROM {ntlp_user} nu
        INNER JOIN {users} u ON nu.uid = u.uid
        WHERE u.name = '%s'", $login_name);

    return db_fetch_object($rs);
}

function get_users_by_role($role_id) {

    return db_query("SELECT u.uid, u.name
        FROM {users_roles} r
        INNER JOIN {users} u ON r.uid = u.uid
        WHERE r.rid = {$role_id}");
}

function get_user_role($user_id) {

    $rs = db_query("SELECT r.rid, concat(u.first_name,' ' ,u.last_name ) as name FROM {users_roles} r
                    INNER JOIN {ntlp_user} u ON u.uid = r.uid WHERE r.uid = %d", $user_id);
    return db_fetch_object($rs);
}

function get_this_school_year() {
    if (!isset($_SESSION['SCHOOL_YEAR'])) {
        $this_year = get_school_year(get_this_school());
        $_SESSION['SCHOOL_YEAR'] = $this_year;

        if (!isset($this_year)) {
            $this_year = date('Y');
            $_SESSION['SCHOOL_YEAR'] = date('m') >= 6 ? $this_year + 1 : $this_year;
        }
    }

    return $_SESSION['SCHOOL_YEAR'];
}

function get_school_year($school_nid) {
    $rs = db_query("SELECT current_school_year FROM {ntlp_school} where nid = %d ", $school_nid);
    $data = db_fetch_object($rs);
    return $data->current_school_year;
}

function _check_new_user($uid) {

    $rs = db_query("select password_change from {ntlp_user} where  password_change = 1 AND uid = %d", $uid);
    if ($rs->num_rows > 0) {
        return true;
    } else {
        return false;
    }
}

function get_parent_students($uid) {

    return db_query("SELECT student_uid FROM {ntlp_user_parent} WHERE parent_uid = %d", $uid);
}

function get_student_parents($uid) {

    return db_query("SELECT up.parent_uid, u.name FROM {ntlp_user_parent} up
                    INNER JOIN {users} u ON u.uid = up.parent_uid
            WHERE student_uid = {$uid}");
}

function get_user_detail_($user_uid) {
    $rs = db_query("SELECT * FROM {ntlp_user} WHERE uid = %d", $user_uid);

    return db_fetch_object($rs);
}

function disalbed_school_permissions($perm_name, $role_id, $i) {
    ${'perm_notifybyemail_' . NTLP_ROLEID_STUDENT} = array();
    ${'perm_notifybyemail_' . NTLP_ROLEID_PARENT} = array();
    ${'perm_notifybyemail_' . NTLP_ROLEID_TEACHER} = array();
    ${'perm_notifybyemail_' . NTLP_ROLEID_NETWORKSTAFF} = array();
    ${'perm_notifybyemail_' . NTLP_ROLEID_SCHOOLLEADER} = array();
    ${'perm_notifybyemail_' . NTLP_ROLEID_SITEADMIN} = array();

    ${'perm_event_' . NTLP_ROLEID_STUDENT} = array(1, 3);
    ${'perm_event_' . NTLP_ROLEID_PARENT} = array(1, 3);
    ${'perm_event_' . NTLP_ROLEID_TEACHER} = array(2);
    ${'perm_event_' . NTLP_ROLEID_NETWORKSTAFF} = array(2);
    ${'perm_event_' . NTLP_ROLEID_SCHOOLLEADER} = array(2);
    ${'perm_event_' . NTLP_ROLEID_SITEADMIN} = array(1, 2, 3);

    ${'perm_group_' . NTLP_ROLEID_STUDENT} = array(4);
    ${'perm_group_' . NTLP_ROLEID_PARENT} = array(2, 3, 4);
    ${'perm_group_' . NTLP_ROLEID_TEACHER} = array(1, 2, 3);
    ${'perm_group_' . NTLP_ROLEID_NETWORKSTAFF} = array(1, 2, 3);
    ${'perm_group_' . NTLP_ROLEID_SCHOOLLEADER} = array(1, 2, 3);
    ${'perm_group_' . NTLP_ROLEID_SITEADMIN} = array(1, 2, 3);

    ${'perm_people_' . NTLP_ROLEID_STUDENT} = array();
    ${'perm_people_' . NTLP_ROLEID_PARENT} = array(1);
    ${'perm_people_' . NTLP_ROLEID_TEACHER} = array();
    ${'perm_people_' . NTLP_ROLEID_NETWORKSTAFF} = array();
    ${'perm_people_' . NTLP_ROLEID_SCHOOLLEADER} = array();
    ${'perm_people_' . NTLP_ROLEID_SITEADMIN} = array(1);

    $permFor = "perm_{$perm_name}_{$role_id}";

    if (in_array($i, $$permFor)) {
        return True;
    } else {
        return false;
    }
}

//function bulk_user_validation($email, $user_role, $status, $password, $user_code, $username) {
//    if (!preg_match("/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/i", $email)
//            || empty($password) || !is_numeric($user_role) || !preg_match('/[0-9]+/', $user_code)
//            || stristr($username, '@') == TRUE || empty($username) ) {
//
//
//        return 0;
//    }else {
//        if($user_role == NTLP_ROLEID_STUDENT || $user_role == NTLP_ROLEID_TEACHER) {
//            return 1;
//        }else {
//            return  0;
//
//        }
//
//
//    }
//}

function bulk_user_validation_basic($first_name, $last_name, $email, $username, $password, $user_role) {
    $errors = 0;

    $email_error = 1;
    $password_error = 2;
    $role_error = 4;
    $username_error = 8;
    $code_error = 16;

    if (!preg_match("/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/i", $email)) {
//    if (preg_match("/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/i", $email) && $user_role != NTLP_ROLEDID_PARENT) {
        $errors |= $email_error;
    }

    if (!is_numeric($user_role) || ($user_role != NTLP_ROLEID_STUDENT && $user_role != NTLP_ROLEID_TEACHER
            && $user_role != NTLP_ROLEID_PARENT )) {
        $errors |= $role_error;
    }

    if (!empty($password)) {
        if (strlen($password) < MIN_PASSWORD_LENGTH) {
            $errors |= $password_error;
        }
    }

    if ($user_role != NTLP_ROLEID_PARENT) {

        if (stristr($username, '@') == TRUE || empty($username)) {
            $errors |= $username_error;
        }
    }

    return $errors;
}

function bulk_user_validation($email, $user_role, $status, $password, $user_code, $username) {
    $errors = 0;

    $email_error = 1;
    $password_error = 2;
    $role_error = 4;
    $username_error = 8;
    $code_error = 16;

    if (!preg_match("/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/i", $email)) {
//    if (preg_match("/^[_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,3})$/i", $email) && $user_role != NTLP_ROLEDID_PARENT) {
        $errors |= $email_error;
    }

    if (empty($password) || strlen($password) < 8) {
        $errors |= $password_error;
    }

    if (!is_numeric($user_role) || ($user_role != NTLP_ROLEID_STUDENT && $user_role != NTLP_ROLEID_TEACHER
            && $user_role != NTLP_ROLEID_PARENT )) {
        $errors |= $role_error;
    }

    if ($user_role != NTLP_ROLEID_PARENT) {

        if (!preg_match('/[0-9]+/', $user_code)) {
            $errors |= $code_error;
        }

        if (stristr($username, '@') == TRUE || empty($username)) {
            $errors |= $username_error;
        }
    }

    return $errors;
}

function set_item_url_handler($name) {

    $current_url = explode('?q=', $_SERVER['REQUEST_URI']);
    $back_url_session_var = $_SESSION['BACK_URL'];

//    watchdog('set_url',"set_item_url_handler START");
    if (is_array($back_url_session_var)) {

        $element_pos = sizeof($back_url_session_var) - 1;
        $keys = array_keys($back_url_session_var);
        $key = $keys[$element_pos];
        $element = $back_url_session_var[$key];

        if ($current_url[1] != $element->url) {
            $Obj = new stdClass();
            $Obj->url = $current_url[1];
            $Obj->page_name = $name;

//            watchdog("edit_url_session", print_r($Obj, true));

            $back_url_session_var[] = $Obj;
            $_SESSION['BACK_URL'] = $back_url_session_var;
        }
    } else {
//        watchdog("new_url_session", 'current url '.$current_url .' name '. $name);

        $back_url = array();
        $urlObj1 = new stdClass();
        $urlObj1->url = 'ntlp/home';
        $urlObj1->page_name = 'Home';

        $urlObj = new stdClass();
        $urlObj->url = $current_url[1];
        $urlObj->page_name = $name;

        $back_url[] = $urlObj1;
        $back_url[] = $urlObj;

        $_SESSION['BACK_URL'] = $back_url;
    }
//    watchdog("back_url_arry", print_r($_SESSION['BACK_URL'], true));
//    watchdog('set_url',"set_item_url_handler END");
}

function get_back_url_value() {
//    watchdog('get_back_url', 'SESSION URL '.print_r($back_url_array, true));
    $back_url_array = $_SESSION['BACK_URL'];
    $element_pos = sizeof($back_url_array) - 2;
    $keys = array_keys($back_url_array);
    $key = $keys[$element_pos];
    $element = $back_url_array[$key];
    return $element;
}

function get_back_url() {
    $back_url = get_back_url_value();
    return '<div class="BoldrBrandCrumb" style="padding-top:0px;"><a href="' . url('ntlp/goback') . '">« Back to ' . $back_url->page_name . '</a></div>';
}

function goto_back_page() {

//    watchdog('goto_back_page', 'before pop '. print_r($_SESSION['BACK_URL'], true));
    $url_history = $_SESSION['BACK_URL'];

    if (is_array($url_history)) {
        array_pop($url_history);
        $previous_url = array_pop($url_history);
//    watchdog('previous_url: ',$previous_url->url);

        $_SESSION['BACK_URL'] = $url_history;
//    watchdog('goto_back_page', 'after pop '. print_r($_SESSION['BACK_URL'], true));

        $query = NULL;
        $quries = explode('&', $previous_url->url);
        array_shift($quries);
        if ($quries) {
            $previous_url->url = preg_replace('/&.*/', '', $previous_url->url);
            $query = array();
            foreach ($quries as $value) {
                $q = explode('=', $value);
                $query[$q[0]] = $q[1];
            }
        }

        drupal_goto($previous_url->url, $query);
    } else {
        drupal_goto('ntlp/home');
    }
}

function check_gdoc_status_for_school($school_nid) {

    $result = db_query("SELECT gdocs_status FROM {ntlp_school} WHERE nid = %d", $school_nid);

    $data = db_fetch_object($result);

    return $data->gdocs_status;
}

function get_current_parent_student() {
//    if(!isset($_SESSION['PARENT_STUDENT_UID'])) {
//        $this_year = date('Y');
//        $_SESSION['SCHOOL_YEAR'] = date('m') >= 6 ? $this_year+1: $this_year;
//    }

    return $_SESSION['PARENT_STUDENT_UID'];
}

function check_user_exist_in_course($user_id, $course_nid, $include_dropped = false) {
    $flag = false;

    $result = db_query("SELECT cu.role_id FROM {ntlp_course} c
        INNER JOIN {ntlp_course_user} cu ON cu.course_nid = c.nid
        WHERE c.nid = %d AND c.is_project = 0 AND cu.user_uid = %d", $course_nid, $user_id);

    if ($result->num_rows > 0) {
        if ($include_dropped) {
            $flag = true;
        } else {
            $record = db_fetch_object($result);
            $flag = $record->role_id;
            //        watchdog('student_perm', print_r($flag, true));
        }
    } else {
        $flag = false;
    }


    return $flag;
}

function get_parent_child_uid($email) {
//changes by saifuddin Oct 25
//    $rs = db_query("SELECT uid FROM dpl_users WHERE lower(name) = '%s'", strtolower($email));
    $rs = db_query("SELECT uid FROM dpl_users WHERE name = '%s'", strtolower($email));
    if ($rs->num_rows > 0) {
        return db_fetch_object($rs)->uid;
    } else {
        return false;
    }
}

function check_user_role($uid) {

    $rs = db_query("SELECT uid FROM {users_roles} WHERE uid = %d", $uid);
    if ($rs->num_rows > 0) {
        return true;
    } else {
        return false;
    }
}

function check_user_existance($user_id) {

    $rs = db_query("SELECT uid FROM dpl_ntlp_user WHERE uid = %d", $user_id);
    if ($rs->num_rows > 0) {
        return true;
    } else {
        return false;
    }
}

function is_user_adminof_this_event($node, $user, $type) {
    $flag = false;

    switch ($type) {
        case 'ntlp_group_event':
            $group_record = is_user_group_admin($node, $user);
//            watchdog('chk_event_grp', print_r($group_record, true));
            if ($group_record) {
                $flag = true;
            }
            break;
        case 'ntlp_class_event':
//        break;
    }

    return $flag;
}

//------------------------
// Term Selector
function get_course_term_combo($course_nid, $ajax_based = false, $ajax_contents = null) {

    static $isFunctionAlreadyCreated = false;
    global $user;
    $onchange = '';
    $rows = '';


    $result = db_query('SELECT ct.school_term_tid, td.name FROM {ntlp_school_term} st
        INNER JOIN {ntlp_course_term} ct ON ct.school_term_tid = st.school_term_tid
        INNER JOIN {term_data} td ON td.tid = st.school_term_tid
        WHERE ct.course_nid = %d 
            AND st.school_year = %d', $course_nid, get_course_year($course_nid));

    if ($result->num_rows > 1) {
        $return_combo = array('class' => 'my-dropdown');
        $attrs = '';
        if (!isset($return_combo['onchange'])) {

            if ($ajax_based) {
                drupal_add_js("
                    function change_term_value(obj){
                        $.post('?q=ntlp/change/course/term/'+obj.value, function(){
                           " . $ajax_contents . "
                        });
                    }
                ", 'inline');
            } else {
                drupal_add_js("
                    function change_term_value(obj){
                        $.post('?q=ntlp/change/course/term/'+obj.value, function(){
                            window.location.reload();
                        });
                    }
                ", 'inline');
            }


            $onchange = ' onchange = "change_term_value(this);" ';
            $isFunctionAlreadyCreated = true;
        }
    }
    if ($result->num_rows > 1) {
        $rows .= '<span style="width:145px;" class="comboboxBgLeft">';
        if (is_array($return_combo)) {
            foreach ($return_combo as $attr => $value) {
                $attrs .= " $attr = \"$value\" ";
            }
            $rows .= "<select $attrs $onchange>";
        } else {
            $rows = array();
        }
    }

    if ($result) {
        $current_term = get_current_selected_course_term();
//        print 'Session:'.print_r($current_term, true);
        if (!isset($current_term)) {
            if ($r = get_current_course_term($course_nid)) {
                $row = db_fetch_object($r);
                $_SESSION['CURRENT_COURSE_TERM'] = $row->school_term_tid;
                $current_term = $_SESSION['CURRENT_COURSE_TERM'];
//                print 'Db:'.$current_term;
            }
        }
        while ($row = db_fetch_object($result)) {

            if ($result->num_rows == 1) {
                $rows .= '<span style="float: left; line-height: 20px; font-size: 14px; color: #4FAD00;font-weight:bold;"> ' . $row->name . '</span>';
            } else if ($result->num_rows > 1) {
                $selected = $current_term == $row->school_term_tid ? ' selected="selected"' : '';
                $rows .= '<option value="' . $row->school_term_tid . '" ' . $selected . '>' . $row->name . '</option>';
            }
//            else {
//                $rows[$row->school_term_tid] = $row->name;
//
//            }
        }
    }

    if ($return_combo) {
        $rows .= '</select>';
        $rows .= '</span>';
    }
    return $rows;
}

function get_school_term_combo() {

    static $isFunctionAlreadyCreated = false;
    global $user;
    $onchange = '';
    $school_nid = get_this_school();
    $HTML = '';


    $result = db_query('SELECT st.school_term_tid, td.name FROM {ntlp_school_term} st
        INNER JOIN {term_data} td ON td.tid = st.school_term_tid
        WHERE st.school_year = ' . get_this_school_year() . '
            AND st.school_nid = ' . $school_nid);

    if ($result->num_rows > 1) {

        $return_combo = array('class' => 'my-dropdown');
        $attrs = '';
        if (!isset($return_combo['onchange'])) {
            drupal_add_js("
                    function change_term_value(obj){
                        $.post('?q=ntlp/change/school/term/'+obj.value, function(){
                            window.location.reload();
                        });
                    }
                ", 'inline');

            $onchange = ' onchange = "change_term_value(this);" ';
            $isFunctionAlreadyCreated = true;
        }
    }
    if ($result->num_rows > 1) {
        $HTML .= '<span style="width:145px;" class="comboboxBgLeft">';
        if (is_array($return_combo)) {
            foreach ($return_combo as $attr => $value) {
                $attrs .= " $attr = \"$value\" ";
            }
            $HTML .= "<select $attrs $onchange>";
        } else {
            $rows = array();
        }
    }

    if ($result) {
        $current_term = get_current_selected_term();
        watchdog("current_term1", 'get_school_term_combo method ' . $current_term);
//        print 'Session:'.$current_term;
        if (!isset($current_term)) {
            $_SESSION['CURRENT_SCHOOL_TERM'] = get_current_school_term($school_nid);
            $current_term = $_SESSION['CURRENT_SCHOOL_TERM'];
//            print 'Db:'.$current_term;
        }

        while ($row = db_fetch_object($result)) {

            if ($result->num_rows == 1) {
                $_SESSION['CURRENT_SCHOOL_TERM'] = $row->school_term_tid;
//                $rows = '<a class="SubmitAssignmentBtn"> '.$row->name.'</a>';
                $HTML .= '<span style="float: left; line-height: 20px; font-size: 14px; color: #4FAD00;font-weight:bold;"> ' . $row->name . '</span>';
            } else if ($result->num_rows > 1) {
                $selected = $current_term == $row->school_term_tid ? ' selected' : '';
                $HTML .= '<option value="' . $row->school_term_tid . '" ' . $selected . '>' . $row->name . '</option>';
            }
//            else {
//                $rows[$row->school_term_tid] = $row->name;
//
//            }
//            watchdog("current_term2", 'get_school_term_combo method ' . $_SESSION['CURRENT_SCHOOL_TERM']);
        }
    }

    if ($return_combo) {
        $HTML .= '</select>';
        $HTML .= '</span>';
    }
    return $HTML;
}

function get_current_selected_term() {
    if (!isset($_SESSION['CURRENT_SCHOOL_TERM']) || intval($_SESSION['CURRENT_SCHOOL_TERM']) == 0) {
        $school_nid = get_this_school();
        $term_tid = 0;
        if ($r = get_current_school_term($school_nid)) {
            $row = db_fetch_object($r);
            $term_tid = $row->school_term_tid;
        }
        $_SESSION['CURRENT_SCHOOL_TERM'] = $term_tid;
    } else {
        //Make sure that the term in session exists in the current course
        //As the user maybe switching courses
        $selected_term = intval($_SESSION['CURRENT_SCHOOL_TERM']);
        $school_nid = get_this_school();
        if (!is_term_in_school($school_nid, $selected_term)) {
            $term_tid = 0;
            if ($r = get_current_school_term($school_nid)) {
                $row = db_fetch_object($r);
                $term_tid = $row->school_term_tid;
            }
//                print '<br>Term not in course, changing it to:'.$term_tid;
            change_school_term($term_tid);
        }
    }
    return $_SESSION['CURRENT_SCHOOL_TERM'];
}

function get_current_selected_course_term($course_nid = 0) {
    if (!isset($_SESSION['CURRENT_COURSE_TERM'])) {
        $url = $_GET['q'];
        $args = explode('/', $url);
        $course_nid = 0;
        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i]) && empty($course_nid))
                $course_nid = $args[$i];
        }
        $term_tid = 0;
        if ($r = get_current_course_term($course_nid)) {
            $row = db_fetch_object($r);
            $term_tid = $row->school_term_tid;
        }

        if ($term_tid == 0 || empty($term_tid)) {
            $term_tid = get_course_first_term($course_nid);
        }
        $_SESSION['CURRENT_COURSE_TERM'] = $term_tid;
    } else {
        if ($course_nid > 0) {
//Make sure that the term in session exists in the current course
//As the user maybe switching courses
            $selected_term = $_SESSION['CURRENT_COURSE_TERM'];
            if (!is_term_in_course($course_nid, $selected_term)) {
                $term_tid = 0;
                if ($r = get_current_course_term($course_nid)) {
                    $row = db_fetch_object($r);
                    $term_tid = $row->school_term_tid;
                }

                if ($term_tid == 0 || empty($term_tid)) {
                    $term_tid = get_course_first_term($course_nid);
                }
//                print '<br>Term not in course, changing it to:'.$term_tid;
                change_course_term($term_tid);
            }
        }
    }
    return $_SESSION['CURRENT_COURSE_TERM'];
}

function get_school_year_term($school_id, $school_year, $return_res = false) {
    $school_id = (int) $school_id;

    if ($school_year == 0 || empty($school_year)) {
        $school_year = get_school_year($school_nid);
    }

    $result = db_query("SELECT st.school_term_tid, t.name FROM {ntlp_school_term} st
                        INNER JOIN {term_data} t ON st.school_term_tid = t.tid 
                        WHERE st.school_nid = %d and st.school_year = %d ORDER BY st.date_started ", $school_id, $school_year);
//
//    watchdog('schoool admin', 'query check  '.print_r('SELECT st.school_term_tid, t.name FROM {ntlp_school_term} st
//                        INNER JOIN dpl_term_data t ON st.school_term_tid = t.tid  WHERE st.school_nid = '.$school_id.' and st.school_year = '.$school_year, true));
    while ($note = db_fetch_array($result)) {

        if ($return_res) {
            $school_term[] = $note;
        } else {
            $school_term[$note['school_term_tid']] = $note['name'];
        }
    }

    return $school_term;
}

function is_user_image_exist($path) {
    global $base_path;
    if (is_null($path) || empty($path)) {
        $image_path = $base_path . 'themes/Boldr/Images/common/small_person_image_not_found.jpg';
    } else {
        if (file_exists($_SERVER['DOCUMENT_ROOT'] . $base_path . $path)) {
            $image_path = $base_path . $path;
        } else {

            $image_path = $base_path . 'themes/Boldr/Images/common/small_person_image_not_found.jpg';
        }
    }
    return $image_path;
}

function get_network_school() {
    $flag = false;

    $result = db_query("SELECT nid FROM {ntlp_school} WHERE is_NETWORK = 1");

    if ($result->num_rows > 0) {
        $data = db_fetch_object($result);
        $flag = $data->nid;
    } else {
        $flag = false;
    }

    return $flag;
}

function ntlp_get_user_role($uid, $nid) {
    $flag = false;
    $result = db_query('SELECT user_role FROM {ntlp_user_access} WHERE nid = %d AND uid = %d ', $nid, $uid);

    if ($result->num_rows > 0) {
        $data = db_fetch_object($result);
        $flag = $data->user_role;
    } else {
        $flag = false;
    }
    return $flag;
}

function ntlp_set_user_access($user_uid, $nid, $rid) {
    return;
//    watchdog("set_access", "User Access saved for Node:$nid for User:$user_uid as Role:$rid");
    db_query('DELETE FROM {ntlp_user_access} WHERE nid = %d AND uid = %d ', $nid, $user_uid);

    db_query("INSERT INTO {ntlp_user_access} (uid, nid, user_role, expire_on) VALUES (
        %d, %d, %d, ADDTIME(NOW(), '0 1:0:0'))", $user_uid, $nid, $rid);
}

function ntlp_get_node_type($nid) {

    $data = db_fetch_object(db_query("SELECT type FROM dpl_node WHERE nid = %d ", $nid));

    return $data->type;
}

function get_timezones() {
    $query = db_query("SELECT * FROM {event_timezones} t");
    if ($query->num_rows > 0) {
        return $query;
    } else {
        return false;
    }
}

function is_user_parent_kid($parent_uid, $student_uid) {
    $flag = false;
    $result = db_query("SELECT student_uid FROM {ntlp_user_parent} 
        WHERE parent_uid = %d AND student_uid = %d", $parent_uid, $student_uid);

    if ($result->num_rows > 0) {
        $flag = true;
    }
    return $flag;
}

function is_school_image_exists($filename) {
    global $base_path;
    $file_path = $_SERVER['DOCUMENT_ROOT'] . $base_path . $filename;

    if (is_null($filename) || empty($filename)) {
        $file_path = $base_path . 'themes/Boldr/Images/default/school-default.png';
    } else {
        if (file_exists($file_path)) {
            $file_path = $base_path . $filename;
        } else {
            $file_path = $base_path . "themes/Boldr/Images/default/school-default.png";
        }
    }
    return $file_path;
}

function get_school_years_for_staff($school_nid, $return_combo = array()) {
    static $isFunctionAlreadyCreated = false;
    global $user;
    $onchange = '';

    if ($return_combo) {
        $attrs = '';
        if (!isset($return_combo['onchange']) && !$isFunctionAlreadyCalled) {
            drupal_add_js("
                    function change_school_year(obj){
                        $.post('?q=ntlp/change/school/year/'+obj.value, function(){
                            window.location.reload();
                        });
                    }
                ", 'inline');

            $onchange = ' onchange = "change_school_year(this);" ';
            $isFunctionAlreadyCreated = true;
        }

        if (is_array($return_combo)) {
            foreach ($return_combo as $attr => $value) {
                $attrs .= " $attr = \"$value\" ";
            }
        }
        $rows = "<select $attrs $onchange>";
    } else {
        $rows = array();
    }

    $result = get_school_years_for_staff_query($school_nid);
    if ($result) {
        $this_school_year = get_this_school_year();
        foreach ($result as $y) {
            if ($return_combo) {
                $selected = $this_school_year == $y ? ' selected' : '';
                $rows .= '<option value="' . $y . '" ' . $selected . '>' . ($y - 1) . '-' . $y . '</option>';
            } else {
                $rows[$y] = ($y - 1) . '-' . $y;
            }
        }
    }

    if ($return_combo) {
        $rows .= '</select>';
    }

    return $rows;
}

function get_school_years_for_staff_query($school_nid) {
    global $user;

    if (isset($user->roles[NTLP_ROLEID_NETWORKMANAGER]) || isset($user->roles[NTLP_ROLEID_NETWORKSTAFF])
            || isset($user->roles[NTLP_ROLEID_SCHOOLLEADER]) || isset($user->roles[NTLP_ROLEID_SITEADMIN])) {
        $COURSE_USER_INNER_JOIN = "";
        $AND_CONDITION = "";
    } else {
        $COURSE_USER_INNER_JOIN = " INNER JOIN {ntlp_course_user} cu ON cu.course_nid = c.nid AND cu.user_uid = $user->uid ";
        $AND_CONDITION = " AND cu.role_id = " . NTLP_ROLEID_TEACHER;
    }

    $sql = "SELECT DISTINCT c.school_year
            FROM {ntlp_course} c
            $COURSE_USER_INNER_JOIN
            WHERE c.school_year > 0 AND c.school_nid = %d $AND_CONDITION";

    $result = db_query($sql, $school_nid);

    $rows = array();
    while ($data = db_fetch_object($result)) {
        $rows[$data->school_year] = $data->school_year;
    }
    //Add current year, if not exist
    $rows[date('Y', time())] = date('Y', time());
    //Sort descending
    krsort($rows);
    return $rows;
}

function get_max_school_year($school_nid) {

    $result = db_result(db_query("SELECT MAX(school_year) as max_year FROM {ntlp_school_term} WHERE school_nid = %d", $school_nid));
    return $result;
}

function get_school_acives_year($school_nid) {

    $result = db_query("SELECT DISTINCT st.school_year FROM {ntlp_school_term} st
        INNER JOIN dpl_ntlp_school s ON s.nid = st.school_nid
        WHERE st.school_nid = %d AND st.school_year > 0 AND st.school_year <= s.current_school_year 
        ORDER BY school_year DESC", $school_nid);

    while ($data = db_fetch_array($result)) {
        $records[$data['school_year']] = intval($data['school_year']) - 1 . ' - ' . $data['school_year'];
    }

    if ($records) {
        return $records;
    } else {
        $records[date('Y')] = date('Y');
    }
}

function _get_all_school_years($school_nid) {

    $records = array();

    $result = db_query("SELECT DISTINCT school_year FROM {ntlp_school_term}
        WHERE school_nid = %d AND school_year > 0 ORDER BY school_year DESC", $school_nid);

    if ($result->num_rows > 0) {

        while ($data = db_fetch_array($result)) {
            $records[$data['school_year']] = intval($data['school_year']) - 1 . ' - ' . $data['school_year'];
        }

    }

    return $records;
}

function get_this_course_year($course_nid) {
    $data = db_result(db_query("SELECT school_year FROM {ntlp_course} WHERE nid = %d", $course_nid));
    return $data;
}

function is_this_school_year_exist($school_nid, $school_year) {
    $flag = false;
    $result = db_query("SELECT school_year FROM {ntlp_school_term}
        WHERE school_nid = %d AND school_year = %d 
        ORDER BY school_year DESC", $school_nid, $school_year);

    if ($result->num_rows > 0) {
        $flag = true;
    }
    return $flag;
}

function get_all_unused_school_outcome($school_nid, $school_year) {
    $record = array('0' => 'Please Select');

    $result = db_query("SELECT DISTINCT ot.tid, t.name 
        FROM {ntlp_gb_outcome} ot 
        INNER JOIN {term_data} t  ON t.tid = ot.tid
        WHERE ot.school_nid = %d 
        AND ot.tid NOT IN
            (SELECT got.tid FROM {ntlp_gb_outcome} got WHERE got.school_nid = %d AND got.school_year = %d)
        ORDER BY ot.sort_order", $school_nid, $school_nid, $school_year);

    if ($result->num_rows > 0) {

        while ($data = db_fetch_object($result)) {
            $record[$data->tid] = $data->name;
        }
    }
    $record['new'] = 'Create New Outcome';
    return $record;
}

function get_selected_outcome_record($outcome_tid) {

    $result = db_query("SELECT td.name, td.description, ot.short_name, ot.weight_percent 
        FROM {ntlp_gb_outcome} ot
        INNER JOIN {term_data} td ON td.tid = ot.tid
        WHERE ot.tid = %d", $outcome_tid);

    if ($result->num_rows > 0) {
        $data = db_fetch_object($result);
    } else {
        $data = false;
    }
    return $data;
}

function get_max_outcome_order($school_nid, $school_year) {
    $result = db_result(db_query("SELECT max(sort_order) FROM {ntlp_gb_outcome} 
        WHERE school_nid = %d AND school_year = %d", $school_nid, $school_year));
    return $result;
}

function is_outcome_used_in_courses_for_year($outcome_tid, $school_nid, $school_year) {
    $flag = false;

    $result = db_query("SELECT co.* FROM {ntlp_gb_course_outcome} co 
        INNER JOIN {ntlp_course} c ON c.nid = co.course_nid 
        WHERE c.school_year = %d AND co.tid = %d AND c.school_nid = %d", $school_year, $outcome_tid, $school_nid);

    if ($result->num_rows > 0) {
        $flag = true;
    }
    return $flag;
}

function is_term_used_in_courses_for_year($term_tid, $school_nid, $school_year) {
    $flag = false;

    $result = db_query("SELECT ct.*
        FROM {ntlp_course_term} ct 
        INNER JOIN dpl_ntlp_course c ON c.nid = ct.course_nid 
        WHERE c.school_year = %d AND ct.school_term_tid = %d AND c.school_nid = %d", $school_year, $term_tid, $school_nid);

    if ($result->num_rows > 0) {
        $flag = true;
    }
    return $flag;
}

function check_school_term_changes($term_tid, $school_nid, $school_year, $date_started, $date_closed) {
    $flag = false;
    $result = db_query("SELECT * FROM {ntlp_school_term} st
    WHERE st.school_term_tid = %d AND st.date_started = '%s' AND st.date_closed = '%s' 
    AND school_year = %d AND school_nid =%d ", $term_tid, $date_started, $date_closed, $school_year, $school_nid);

    if ($result->num_rows > 0) {
        $flag = true;
    }

    return $flag;
}

function get_school_years_for_teacher($user_uid, $user_role) {
    $result = db_query("SELECT DISTINCT st.school_year 
                    FROM {ntlp_course_user cu 
                    INNER JOIN {ntlp_course} c ON c.nid = cu.course_nid
                    INNER JOIN {ntlp_course_term} ct ON ct.course_nid = cu.course_nid
                    INNER JOIN {ntlp_school_term} st ON st.school_term_tid = ct.school_term_tid
                    WHERE cu.user_uid = %d AND cu.role_id = %d 
                    AND st.school_year > 0 ORDER BY st.school_year DESC", $user_uid, $user_role);


    return $result;
}

function get_school_years_for_student($user_uid) {

    $result = db_query("SELECT DISTINCT st.school_year 
                    FROM {ntlp_course_user cu 
                    INNER JOIN {ntlp_course} c ON c.nid = cu.course_nid 
                    INNER JOIN {ntlp_school} s ON s.nid = c.school_nid 
                    INNER JOIN {ntlp_course_term} ct ON ct.course_nid = cu.course_nid
                    INNER JOIN {ntlp_school_term} st ON st.school_term_tid = ct.school_term_tid
                    WHERE cu.user_uid = %d AND cu.role_id = " . NTLP_ROLEID_STUDENT . "
                    AND st.school_year > 0 AND st.school_year <= s.current_school_year ORDER BY school_year DESC", $user_uid);


    return $result;
}

function get_self_enroll_row($course_nid, $user_id) {
    $result = db_query("SELECT	c.teacher1_uid , c.teacher2_uid , c.allow_self_enroll, u.role_id
                    FROM dpl_ntlp_course c
                    LEFT JOIN dpl_ntlp_course_user u ON u.course_nid  = c.nid AND u.user_uid = %d
                    WHERE c.nid = %d ", $user_id, $course_nid);
    return $result;
}

function get_course_year($course_nid) {
    $result = db_query("SELECT school_year FROM {ntlp_course} WHERE nid = %d", $course_nid);

    if ($result) {
        $data = db_fetch_object($result);
    }
    return $data->school_year;
}

function is_term_exist_in_school($school_nid, $term_tid, $school_year) {
    $flag = false;
    $result = db_query("SELECT * FROM {ntlp_school_term} 
        WHERE school_nid = %d AND school_term_tid =%d AND school_year = %d", $school_nid, $term_tid, $school_year);

    if ($result->num_rows > 0) {
        $flag = true;
    }
    return $flag;
}

function get_school_first_term($school_nid, $school_year) {
    $result = db_query("SELECT st.school_term_tid, st.date_started 
        FROM {ntlp_school_term} st 
        WHERE st.school_nid = %d AND st.school_year = %d AND st.school_term_tid <> 0 
        ORDER BY st.date_started ASC LIMIT 0, 1", $school_nid, $school_year);
    if ($result) {
        $data = db_fetch_object($result);
    }
    return $data->school_term_tid;
}

function get_course_first_term($course_nid) {
    $result = db_query("SELECT ct.school_term_tid, st.date_started 
        FROM {ntlp_course_term} ct 
        INNER JOIN {ntlp_school_term} st ON st.school_term_tid = ct.school_term_tid
        WHERE ct.course_nid = %d ORDER BY st.date_started ASC LIMIT 0, 1", $course_nid);
    if ($result) {
        $data = db_fetch_object($result);
    }
    return $data->school_term_tid;
}

function get_all_users() {

    return db_query("SELECT u.* FROM {ntlp_user} nu
                    INNER JOIN {users} u ON nu.uid = u.uid");
}

function get_school_users_by_roles($school_id, $role_ids) {

    $rs = db_query("SELECT nu.uid FROM {ntlp_user} nu
                    INNER JOIN {users_roles} r ON nu.uid = r.uid
                    WHERE school_nid = {$school_id}
                    AND r.rid IN ({$role_ids})" );
    return $rs;
}

function get_parent_student_records($parent_uid, $node_nid) {

    $result = db_query("SELECT up.parent_uid, cu.course_nid, cu.user_uid, 
        u.first_name, u.last_name
        FROM {ntlp_user_parent} up 
        INNER JOIN {ntlp_course_user} cu ON cu.user_uid = up.student_uid
        INNER JOIN {ntlp_user} u ON u.uid = cu.user_uid
        WHERE up.parent_uid = %d AND cu.role_id = 5 AND cu.course_nid = %d", $parent_uid, $node_nid);

    return $result;
}
