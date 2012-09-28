<?php

function get_all_courses($school_nid) {

    $rs = db_query("SELECT n.title,c.nid FROM {ntlp_course} c
        INNER JOIN {node} n on c.nid=n.nid
        WHERE is_project = %d AND school_nid= %d
        ORDER BY n.title", 0, $school_nid);

    while ($row = db_fetch_array($rs)) {
        $courses[$row['nid']] = $row['title'];
    }

    return $courses;
}

function get_group_image_file_path($group_nid) {

    $rs = db_query('SELECT c.picture_fid,f.filepath,f.fid FROM {ntlp_group} c
                    INNER JOIN {files} f  ON f.fid = c.picture_fid
                    WHERE nid = %d', $group_nid);

    return db_fetch_object($rs);
}

function get_group_block_detail($group_nid) {

    $rs = db_query("SELECT g.* , n.type, n.title, n.uid, cn.title AS coursetitle, pn.title AS projectname, f.filepath
                    FROM {ntlp_group} g
                    INNER JOIN {node} n ON g.nid = n.nid
                    LEFT JOIN {ntlp_course} c ON c.nid = g.course_nid
                    LEFT JOIN {node} cn ON cn.nid = c.nid
                    LEFT JOIN {node} pn ON pn.nid = g.project_nid
                    LEFT JOIN {files} f ON f.fid = g.picture_fid
                    WHERE g.nid = %d", $group_nid);

    return db_fetch_object($rs);
}

function get_total_members($group_nid) {

    $rs = db_query("SELECT count(uid) as total_members FROM {og_uid} WHERE nid = %d", $group_nid);

    return db_fetch_object($rs);
}

function get_total_posts($group_nid) {

    $rs = db_query("SELECT count(nid) as total  FROM {ntlp_course_resource} WHERE course_nid = %d", $group_nid);

    return db_fetch_object($rs);
}

function get_total_resources($group_id) {

    $rs = db_query("SELECT   count(course_nid) as total_resources FROM  dpl_ntlp_course_resource where course_nid =  %d", $group_id);

    return db_fetch_object($rs);
}

function get_group_admins($group_nid) {

    $rs = db_query("SELECT uid FROM {og_uid} WHERE  nid = %d AND is_active=1 AND is_admin=1", $group_nid);

    return $rs;
}


function get_group_moderators($group_nid, $sort = false) {

    $rs = db_query("SELECT og.uid, n.uid as creator FROM {og_uid} og
        INNER JOIN {node} n on n.nid =  og.nid WHERE is_admin = 1 and og.nid = %d" .
            ($sort ? ' ORDER BY creator' : ''), $group_nid);

    return $rs;
}

function get_group_user_detail($user_uid) {

    $rs = db_query("SELECT n.* ,u.picture FROM {ntlp_user} n INNER JOIN
                    {users} u  ON n.uid = u.uid WHERE n.uid = %d", $user_uid);

    return db_fetch_object($rs);
}

//function ntlp_group_tabs() {
//    include "grouptabs.template.inc";
//    global $user;
//
//    $output = $HTMLgroup_tab;
//
//    return $output;
//
//}


function _get_group_resource($head, $group_id) {

    $result = db_query("SELECT node.nid, node.type, node.uid, node.status, node.created, node.changed, res.type as dtype,
        revs.title, revs.body, revs.teaser, res.course_nid, res.section_tid, res.path, res.fid
        FROM {node} node
        LEFT JOIN {node_revisions} revs ON node.nid = revs.nid
        LEFT JOIN {ntlp_course_resource} res ON node.nid = res.nid
        WHERE res.course_nid =%d" . tablesort_sql($head), $group_id);

    return $result;
}

function get_resource_user($uid) {

    $rs = db_query("SELECT first_name,last_name FROM {ntlp_user} WHERE uid = %d", $uid);

    return db_fetch_object($rs);
}

//
//function get_group_user_roles() {
//
//    $rs = db_query("SELECT rid, name
//        from {role} r
//        WHERE rid > 2");    //to filter out anonymous and authenticated user
//
//   return $rs;
//}
//
/*
 * filters that can be used used:
 *      SCHOOL_ID => school_id
 *      USER_ID => user_id
 *      SCHOOL_GROUPS
 *      NETWORK_GROUPS
 *      USER_IS_ADMIN => user_id
 */
function _get_groups_details($filter = array(), $head = array(), $limit) {
    global $user;

    $where = '1=1';
    $whereOR = array();
    $innerJoin = '';

    if (!isset($_GET['sort'], $_GET['order'])) {
        $_GET['sort'] = 'asc';
        $_GET['order'] = 'nid';
    }

    if (sizeof($filter) < 1) {
        $filter['SCHOOL_GROUPS'] = 1;
        $filter['NETWORK_GROUPS'] = 1;
        $filter['USER_IS_ADMIN'] = false;
    }

    if ($filter['USER_IS_ADMIN']) {
        $where .= ' AND oua.uid = ' . ((int) $filter['USER_IS_ADMIN']);
        $whereOR[] = '  oua.is_admin = 1';
        $innerJoin .= ' INNER JOIN dpl_og_uid oua ON oua.nid = ng.nid ';
    }
    if (isset($filter['USER_ID'])) {
        $where .= isset($filter['USER_ID']) ? ' AND ou.is_active = 1 AND ou.uid = ' . ((int) $filter['USER_ID']) : '';
        $innerJoin .= isset($filter['USER_ID']) ? ' INNER JOIN dpl_og_uid ou ON ou.nid = ng.nid ' : '';
    }
    if (isset($filter['SCHOOL_GROUPS'])) {
        $whereOR[] = ' (ng.showin_school_directory = ' . $filter['SCHOOL_GROUPS'] . ' AND ns.nid = ' . get_this_school() . ')';
    }

    /* Changes by Saifuddin to restrict parent role AND Student to see NTN wide groups */
    if (!isset($user->roles[NTLP_ROLEID_PARENT]) && !isset($user->roles[NTLP_ROLEID_STUDENT])) {
        if (isset($filter['NETWORK_GROUPS'])) {
            $whereOR[] = ' ng.showin_network_directory = ' . $filter['NETWORK_GROUPS'];
        }

        if (isset($user->roles[NTLP_ROLEID_SITEADMIN]) || isset($user->roles[NTLP_ROLEID_SCHOOLLEADER])) {
            //$whereOR[] = '( is_closed=1 AND ns.nid = ' . get_this_school() . ' )';
//            $whereOR[] = '( ns.nid = ' . get_this_school() . ' )';
        }
    }

    if ($whereOR) {
        $where .= ' AND ((' . implode(' OR ', $whereOR) . '))';
    }

    $sql = "SELECT ng.nid,ng.is_closed, ng.showin_school_directory, ng.showin_network_directory,
        n4.body as group_title,  n.nid school_nid, nr.body,  n1.title, n.title school_name, tmtbl.total_members
    FROM dpl_ntlp_group ng $innerJoin
    INNER JOIN  dpl_node_revisions n4 ON n4.nid = ng.nid
    INNER JOIN dpl_node n1 ON n1.nid = ng.nid
    INNER JOIN dpl_ntlp_school ns ON ns.nid = ng.school_nid
    INNER JOIN dpl_node n ON n.nid = ns.nid
    INNER JOIN dpl_node_revisions nr ON nr.nid = ns.nid
    LEFT JOIN ( SELECT og2.nid, COUNT(*) total_members FROM dpl_og_uid og2 GROUP BY og2.nid ) tmtbl ON tmtbl.nid = ng.nid
    WHERE $where ";
    
//    $sql = "SELECT ng.nid, ng.is_closed, ng.showin_school_directory, ng.showin_network_directory,
//        n4.body as group_title,  n.nid school_nid, nr.body,  n1.title, n.title school_name, tmtbl.total_members
//    FROM dpl_ntlp_group ng $innerJoin
//    INNER JOIN  dpl_node_revisions n4 ON n4.nid = ng.nid
//    INNER JOIN dpl_node n1 ON n1.nid = ng.nid
//    INNER JOIN dpl_ntlp_school ns ON ns.nid = ng.school_nid
//    INNER JOIN dpl_node n ON n.nid = ns.nid
//    INNER JOIN dpl_node_revisions nr ON nr.nid = ns.nid
//    LEFT JOIN ( SELECT og2.nid, COUNT(*) total_members FROM dpl_og_uid og2 GROUP BY og2.nid ) tmtbl ON tmtbl.nid = ng.nid
//    WHERE $where ";
    watchdog('sql', $sql . '<br>-->WHERE' . $where . '<br>-->Filters' . print_r($filter, true));
//   $sql = "SELECT
//                    ng.nid,
//                    n.nid school_nid,
//                    n1.title,
//                    n.title school_name,
//                    tmtbl.total_members,
//                    restbl.total_resources
//            FROM
//                    dpl_ntlp_group ng
//
//            $innerJoin
//
//                    INNER JOIN
//                    dpl_node n1
//                    ON n1.nid = ng.nid
//
//                    INNER JOIN
//                    dpl_ntlp_school ns
//                    ON ns.nid = ng.school_nid
//
//                    INNER JOIN dpl_node n
//                    ON n.nid = ns.nid
//
//                    LEFT JOIN
//                    (
//                            SELECT
//                                    og2.nid, COUNT(*) total_members
//                            FROM
//                                    dpl_og_uid og2
//                            GROUP BY
//                                    og2.nid
//
//                    ) tmtbl
//                    ON tmtbl.nid = ng.nid
//
//                    LEFT JOIN
//                    (
//                            SELECT
//                                    ncr.course_nid, COUNT(*) total_resources
//                            FROM
//                                    dpl_ntlp_course_resource ncr
//                            GROUP BY
//                                    ncr.course_nid
//                    ) restbl
//                    ON restbl.course_nid = ng.nid
//            WHERE
//            $where ";
    $recs = array();
    if ($result = pager_query($sql, $limit)) {
        while ($row = db_fetch_object($result)) {
            $recs[] = $row;
        }
    }


    return $recs;
}

function is_group_exist($group_id) {

    $flag = false;
    $result = db_query("SELECT * FROM {ntlp_group} WHERE nid = %d", $group_id);
    if ($result->num_rows > 0) {
        $flag = true;
    } else {
        $flag = false;
    }
    return $flag;
}

function _get_group_members($group_id, $filters = array(), $header = array(), $limit = 0) {
    $where = '';
    $isActive = array();

    if (isset($filters['ACTIVE'])) {
        $isActive[] = 1;
    }
    if (isset($filters['INACTIVE'])) {
        $isActive[] = 0;
    }

    if (isset($filters['DISINCLUDE']) and is_array($filters['DISINCLUDE'])) {
        $where = ' AND ou.uid NOT IN(' . implode(',', $filters['DISINCLUDE']) . ')';
    }

    if ($isActive) {
        $where .= ' AND ou.is_active IN (' . implode(',', $isActive) . ')';
    }
    if ($limit > 0) {
        $limit = ' LIMIT 0, '.$limit;
    } else {
        $limit = "";
    }

    $order_by = (isset($_GET['order']) || isset($_GET['sort'])) ? tablesort_sql($header) : ' ORDER BY nu.last_name, nu.first_name';

    $sql = "SELECT
                ou.nid, ou.uid,nu.school_nid, ou.is_active, nu.first_name,
                nu.last_name, ou.created, ou.changed, ou.is_admin,
                (SELECT COUNT(n.nid) FROM dpl_node n
                 INNER JOIN dpl_ntlp_course_resource oga ON n.nid = oga.nid
                 WHERE n.status = 1 AND oga.course_nid = %d AND n.uid = ou.uid) group_post_count
            FROM
                {og_uid} ou

                INNER JOIN
                dpl_ntlp_user nu
                ON nu.uid = ou.uid

            WHERE
                ou.nid = %d
            $where
            $order_by
            $limit";

    $recs = array();
    if ($result = db_query($sql, $group_id, $group_id)) {
        while ($row = db_fetch_object($result)) {
            $recs[] = $row;
        }
    }

    return $recs;
}

function check_group_user($group_id) {
    global $user;

    $rs = db_query("select * from {og_uid} where nid = %d and uid = %d", $group_id, $user->uid);

    if ($rs->num_rows > 0) {
        return true;
    } else {

        return false;
    }
}

function is_group_member($nid, $uid) {
    return db_result(db_query('SELECT is_active FROM dpl_og_uid ou WHERE ou.nid = %d AND ou.uid = %d', $nid, $uid));
}

function check_group_permissions($school_group, $network_group, $group_status) {
    $access_type = "Public";

    if ($group_status == 1 && $school_group == 1 && $network_group == 0) {
        $access_type = "School Restricted";
    } elseif ($group_status == 0 && $school_group == 1 && $network_group == 0) {
        $access_type = "School Public";
    } elseif ($group_status == 1 && $school_group == 0 && $network_group == 1) {
        $access_type = "Network Restricted";
    } elseif ($group_status == 0 && $school_group == 0 && $network_group == 1) {
        $access_type = "Network Public";
    } elseif ($group_status == 1 && $school_group == 0 && $network_group == 0) {
        $access_type = "Private";
    }

    return $access_type;
}

function _group_permissions($school_group, $network_group, $group_status) {

    if ($group_status == 1 && $school_group == 1 && $network_group == 0) {
        $perm_type = 1;
    } elseif ($group_status == 0 && $school_group == 1 && $network_group == 0) {
        $perm_type = 0;
    } elseif ($group_status == 1 && $school_group == 0 && $network_group == 1) {
        $perm_type = 3;
    } elseif ($group_status == 0 && $school_group == 0 && $network_group == 1) {
        $perm_type = 2;
    } elseif ($group_status == 1 && $school_group == 0 && $network_group == 0) {
        $perm_type = 4;
    }

    return $perm_type;
}

function get_user_school_name($uid) {

    $rs = db_query("SELECT n.title AS school_name, dr.name as role_name FROM dpl_ntlp_user g
            INNER JOIN dpl_node n  ON n.nid = g.school_nid
            INNER JOIN dpl_users_roles r On r.uid = g.uid
            INNER JOIN dpl_role dr ON dr.rid = r.rid
            WHERE g.uid =%d", $uid);


    return db_fetch_object($rs);
}

function get_group_school_nid($group_nid) {
    $rs = db_query("select school_nid from {ntlp_group} where nid = %d", $group_nid);
    $data = db_fetch_object($rs);

    return $data->school_nid;
}

/**
 * Low level function for managing membership
 *
 * @param $gid node ID of a group
 * @param $uid user ID of user
 * @param $args an array with details of this membership. Recognized array keys are:
     is_active, is_admin, created. Other values are passed to hook implementations.
 */
function ntlp_og_save_subscription($gid, $uid, $args = array()) {
  if ($uid > 0) {
    $sql = "SELECT COUNT(*) FROM {og_uid} WHERE nid = %d AND uid = %d";
    $cnt = db_result(db_query($sql, $gid, $uid));
//    echo "<br>Count:$cnt";
    $time = time();
    $subscription = array(
      'nid' => $gid,
      'uid' => $uid,
      'created' => isset($args['created']) ? $args['created'] : $time,
      'changed' => $time
    );
    unset($args['created']);
    $subscription += $args;
//    echo "<br>Subscriptions:".print_r($subscription, true);

    if ($cnt == 0) {
//      drupal_write_record('og_uid', $subscription);
//      module_invoke_all('og', 'user insert', $gid, $uid, $args);
        db_query("INSERT INTO {og_uid} (nid, og_role, is_active, is_admin, uid, created, changed) VALUES (%d, 0, %d, %d, %d, %d, %d)",
                $gid, $args['is_active'], $args['is_admin'], $uid, $time, $time);
    }
    else {
//      drupal_write_record('og_uid', $subscription, array('nid', 'uid'));
//      module_invoke_all('og', 'user update', $gid, $uid, $args);
        db_query("UPDATE {og_uid} SET is_active = %d, is_admin = %d, changed = %d
            WHERE nid = %d AND uid = %d",
            $args['is_active'], $args['is_admin'], $time, $gid, $uid);
    }
  }
}

function ntlp_og_delete_subscription($gid, $uid, $args = array()){
  $sql = "DELETE FROM {og_uid} WHERE nid = %d AND uid = %d";
  db_query($sql, $gid, $uid);
//  module_invoke_all('og', 'user delete', $gid, $uid, $args);
}


function get_group_info($group_id) {
    return db_fetch_object(db_query("SELECT * FROM {ntlp_group} WHERE nid = %d", $group_id));
}

// If is admin then return its role id
function is_user_group_admin($group_id, $user) {
    $flag = false;

    $result = db_query("SELECT 1
        FROM {og_uid} ou
        WHERE ou.nid = %d AND ou.uid = %d
        AND ou.is_admin = 1 AND ou.is_active = 1", $group_id, $user->uid);

    if ($result->num_rows > 0) {
        $flag = true;
    } else {

        if (isset($user->roles[NTLP_ROLEID_SCHOOLLEADER]) || isset($user->roles[NTLP_ROLEID_SITEADMIN])) {

            $site_admin_query = db_query("SELECT nid FROM {ntlp_group}
            WHERE nid = %d AND school_nid = %d ", $group_id, get_this_school());

            if ($site_admin_query->num_rows > 0) {

                $flag = true;
            } else {
                $flag = false;
            }
        }
    }
    if (isset($user->roles[NTLP_ROLEID_NETWORKSTAFF]) || isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])) {
        $flag = true;
    }
//    watchdog('group_admin', 'is_user_group_admin ' . $group_id . ' user ' . $user->uid . ' flag ' . $flag);
    return $flag;
}

function is_user_group_member($group_id, $user) {

    $flag = false;


    $result = db_query("SELECT * FROM {og_uid} ou
        WHERE ou.nid = %d AND ou.uid = %d AND ou.is_active = 1", $group_id, $user->uid);


    if ($result->num_rows > 0) {

        $flag = true;
    } else {

        if (isset($user->roles[NTLP_ROLEID_SCHOOLLEADER]) || isset($user->roles[NTLP_ROLEID_SITEADMIN])) {

            $result = db_query("SELECT nid FROM {ntlp_group}
            WHERE nid = %d AND school_nid = %d AND showin_network_directory = 0 ", $group_id, get_this_school());

            if ($result->num_rows > 0) {
                $flag = true;
            } else {
                $flag = false;
            }
        }
    }
    return $flag;
}

// $role_in_group = %d (0=Regular User, 1=Moderator)
function get_group_users($group_nid, $role_in_group = 0) {

    $group_nid = (int) $group_nid;
    $role_in_group = (int) $role_in_group;

    $sql = "SELECT ou.*, u.first_name, u.last_name, nu.name
            FROM {og_uid} ou
            INNER JOIN {ntlp_group} g ON ou.nid = g.nid
            INNER JOIN {ntlp_user} u ON ou.uid = u.uid
            INNER JOIN {users} nu ON u.uid = nu.uid
            WHERE ou.nid = $group_nid AND ou.is_admin = $role_in_group";

    $rs = db_query($sql);
    return $rs;
}

function get_group($group_nid) {

    $rs = db_query("SELECT n.title, nr.body body, g.*
            FROM {node} n
            INNER JOIN {node_revisions} nr ON nr.nid = n.nid
            INNER JOIN {ntlp_group} g ON n.nid = g.nid
            WHERE g.nid = %d", intval($group_nid));

    $result = db_fetch_object($rs);
    if (isset($result)) {
        return $result;
    } else {
        return false;
    }
    return $result;
}

function get_course_group_owners_and_members($course_id, $group_nid, $group_uid) {

    $sql = "SELECT ou.is_admin, ou.uid, CONCAT(u.first_name, ' ' , u.last_name) AS admin_name,
                 (SELECT ncu.period FROM {ntlp_course_user} ncu WHERE u.uid=ncu.user_uid AND ncu.course_nid=$course_id AND ncu.role_id=5) AS period
                FROM dpl_og_uid ou
                INNER JOIN dpl_ntlp_user u ON u.uid = ou.uid
                WHERE ou.nid = $group_nid
                AND (ou.is_admin = 0 OR ou.uid=$group_uid) ORDER BY admin_name";

    $rs = db_query($sql);
    return $rs;
}

