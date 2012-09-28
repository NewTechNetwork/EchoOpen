<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'gapps_feeds', 'docs.contents.inc');

//define('DATE_FORMAT', '%Y-%m-%d %H:%M:%S');
//Return Grade Scales defined for a course
function get_course_grades($course_nid) {
    $i = 0;
    $rs = db_query("SELECT cg.*
        FROM {ntlp_course_grade} cg
        WHERE course_nid = %d ORDER BY lower_limit DESC", $course_nid);
    while ($rec = db_fetch_array($rs)) {
        $arr_cg[$i] = array('label' => $rec['label'], 'lower_limit' => $rec['lower_limit']);
        $i++;
    }
    return $arr_cg;
}

//Return Grade Scales defined for a school
function get_school_grades($school_nid, $school_year) {
    $i = 0;
    $rs = db_query("SELECT cg.*
        FROM {ntlp_school_grade} cg
        WHERE school_nid = %d AND school_year = %d ORDER BY lower_limit DESC", $school_nid, $school_year);
    while ($rec = db_fetch_array($rs)) {
        $arr_cg[$i] = array('label' => $rec['label'], 'lower_limit' => $rec['lower_limit']);
        $i++;
    }
    return $arr_cg;
}

function _load_course_outcomes2($course_name) {
    $result = db_query("SELECT td.tid,td.name FROM {vocabulary} tv LEFT JOIN {term_data} td on tv.vid = td.vid
        WHERE tv.name = '%s' order by td.tid", $course_name);

    while ($note = db_fetch_object($result)) {

        $activityoutcome[$note->tid] = $note->name;
    }
    //print_r($activityoutcome);

    return $activityoutcome;
}

function get_all_course_projects($course_nid) {
    $course_projects = false;

    $result = db_query("SELECT * FROM {ntlp_course} WHERE course_nid = %d", $course_nid);

    if ($result->num_rows > 0) {
        while ($record = db_fetch_object($result)) {
            $course_projects[] = $record;
        }
    }

    return $course_projects;
}

function get_all_course_resources($course_nid) {
    $course_resources = false;

    $result = db_query("SELECT nid FROM {ntlp_course_resource} WHERE course_nid = %d", $course_nid);

    if ($result->num_rows > 0) {
        while ($record = db_fetch_object($result)) {
            $course_resources[] = $record;
        }
    }

    return $course_resources;
}

function get_all_course_activities($course_nid) {
    $course_resources = false;

    $result = db_query("SELECT nid FROM {ntlp_course_activity} WHERE course_nid = %d", $course_nid);

    if ($result->num_rows > 0) {
        while ($record = db_fetch_object($result)) {
            $course_resources[] = $record;
        }
    }

    return $course_resources;
}

//Return those Subjects which are last nodes in the Subjects hierarchy
//NO NEED TO PASS THIS AS STRING PARAMETER, SHOULD CHANGE THIS
function get_course_subject() {
    $VOCABULARY_ID_SUBJECT = 8;

    $rs = db_query("SELECT DISTINCT(td.name), td.tid
        FROM {term_data} td
        INNER JOIN {term_hierarchy} th ON th.tid=td.tid
        WHERE td.vid = %d AND td.tid NOT IN
            (SELECT parent FROM {term_hierarchy})
        ORDER BY td.name", $VOCABULARY_ID_SUBJECT);

    while ($note = db_fetch_array($rs)) {
        $course_subjects[$note['tid']] = $note['name'];
    }
    return $course_subjects;
}

function get_learning_outcome($course_subject) {
    $rs = db_query("SELECT vid FROM {vocabulary} WHERE name='%s'", $course_subject);
    $note = db_fetch_array($rs);
    $course_vid = $note['vid'];
    return $course_vid;
}

function load_course_outcomes($course_nid, $course_school_year) {

    $result = db_query("SELECT td.tid, td.name, oc.weight_percent, oc.sort_order, soc.tid as is_custom,
                (SELECT count(*) FROM {ntlp_course_activity} ca INNER JOIN {ntlp_gb_activity_outcome} ao
                ON ca.nid = ao.activity_nid
                WHERE ca.course_nid = %d AND ao.tid = oc.tid) oc_cnt_in_activity
            FROM {term_data} td
            INNER JOIN {ntlp_gb_course_outcome} oc ON td.tid = oc.tid
            LEFT JOIN {ntlp_gb_outcome} soc ON soc.tid = oc.tid AND soc.school_year = %d
            WHERE oc.course_nid = %d
            ORDER by oc.sort_order", $course_nid, $course_school_year, $course_nid);

    return $result;
}

function load_school_outcomes($school_nid, $school_year) {

    $result = db_query("SELECT td.tid, td.name, oc.weight_percent
            FROM {term_data} td 
            INNER JOIN {ntlp_gb_outcome} oc ON td.tid = oc.tid
            WHERE oc.school_nid = %d AND oc.school_year = %d
            ORDER by td.name", $school_nid, $school_year);

    return $result;
}

function manage_course_outcomes($selected_oc_id, $selected_oc_custom, $selected_oc_weight, $save = true) {

    $ids_array = explode(",", $selected_oc_id);
    $custom_name_array = explode(",", $selected_oc_custom);
    $weight_array = explode(",", $selected_oc_weight);

    $outcome_array = array();
    for ($i = 0; $i < sizeof($ids_array) - 1; $i++) {      //-1 removes the last additional ','
        if ($ids_array[$i] != 'x') { // x is id of SELECT option in outcome combo
            if ($save) {
                if ($ids_array[$i] <= 0) { // 0 is the id for custom name for outcome.
                    watchdog('course_oc', 'Adding a custom outcome: ' . $custom_name_array[$i]);
                    if (!empty($custom_name_array[$i])) {
                        if ($ids_array[$i] < 0) {           //Negative value: means existing outcome
                            $ids_array[$i] = $ids_array[$i] * -1;
                            update_taxonomy_record($ids_array[$i], $custom_name_array[$i]);

                            $outcome_array[$ids_array[$i]] = $weight_array[$i];
                            watchdog('course_oc', 'Updating custom outcome: ' . $ids_array[$i]);
                        } else {
                            //add into the taxonomy.. and return the id of the
                            $tid = insert_taxonomy_record($custom_name_array[$i], "NTLP GB Outcome");

                            $outcome_array[$tid] = $weight_array[$i];
                            watchdog('course_oc', 'Created custom outcome: ' . $tid);
                        }
                    }
                } else {
                    watchdog('course_oc', 'Adding school outcome: ' . $ids_array[$i]);
                    if ($weight_array[$i] != 0 || !empty($weight_array[$i])) {
                        $outcome_array[$ids_array[$i]] = $weight_array[$i];
                    }
                }
            } else {
                //Just return weights in an array
                if ($weight_array[$i] != 0 || !empty($weight_array[$i])) {
                    $outcome_array[$i] = $weight_array[$i];
                }
            }
        }
    }

    return $outcome_array;
}

function insert_taxonomy_record($custom_name, $taxonomy_name) {

    $vocab_obj = _get_vocabolary_vid($taxonomy_name);
    $descripiton = '';

    db_query("INSERT INTO {term_data} (vid, name, description, weight) VALUES (%d, '%s', '%s', 0)"
            , $vocab_obj->vid, $custom_name, $descripiton);

    $tid = db_last_insert_id('{term_data}', 'tid');
    return $tid;
}

function update_taxonomy_record($tid, $custom_name) {

    db_query("UPDATE {term_data} SET name = '%s' WHERE tid = %d", $custom_name, $tid);
}

function _get_vocabolary_vid($vocab_name) {

    $result = db_query("SELECT vid FROM {vocabulary} WHERE name = '%s'", $vocab_name);

    if ($result) {
        return db_fetch_object($result);
    } else {
        return false;
    }
}

function get_selected_year_terms($school_nid, $school_year) {


    $rs = db_query("SELECT s.school_term_tid,t.name FROM {ntlp_school_term} s
           INNER JOIN {term_data} t ON t.tid = s.school_term_tid WHERE s.school_nid = %d AND s.school_year = %d", $school_nid, $school_year);

    while ($data = db_fetch_array($rs)) {

        $school_terms[$data['school_term_tid']] = $data['name'];
    }

    return $school_terms;
}

function get_selected_course_terms($course_nid) {


    $rs = db_query("SELECT s.school_term_tid,t.name FROM {ntlp_course_term} s
           INNER JOIN {term_data} t ON t.tid = s.school_term_tid WHERE s.course_nid = %d", $course_nid);

    while ($data = db_fetch_array($rs)) {

        $school_terms[] = $data['school_term_tid'];
    }

    return $school_terms;
}

function _get_course_members($course_nid, $role_id = 0) {

    if (!$role_id) {
        $role_id = NTLP_ROLEID_STUDENT;
    }

    $sql = 'SELECT
                users.uid AS uid,
                ntlp_user.first_name AS ntlp_user_first_name,
                ntlp_user.last_name AS ntlp_user_last_name
            FROM
                dpl_users users
                INNER JOIN dpl_ntlp_course_user ntlp_course_user ON users.uid = ntlp_course_user.user_uid
                LEFT JOIN dpl_ntlp_user ntlp_user ON users.uid = ntlp_user.uid
            WHERE
                ntlp_course_user.role_id = %d
                AND ntlp_course_user.course_nid = %d';
    $rows = array();
    if ($result = db_query($sql, $role_id, $course_nid)) {
        while ($row = db_fetch_object($result)) {
            $rows[] = $row;
        }
    }

    return $rows;
}

function is_user_moderator_in_course($course_nid, $user_uid) {
    global $user;
    $flag = false;

    $result = db_query("SELECT * FROM dpl_ntlp_course_user WHERE course_nid = %d
        AND role_id = 6 AND user_uid = %d", $course_nid, $user_uid);

    if ($result->num_rows > 0) {
        $flag = true;
    } else {
        if (isset($user->roles[NTLP_ROLEID_SCHOOLLEADER]) || isset($user->roles[NTLP_ROLEID_SITEADMIN])) {

            $site_admin_query = db_query("SELECT nid FROM {ntlp_course}
            WHERE nid = %d AND school_nid = %d ", $course_nid, get_this_school());

            if ($site_admin_query->num_rows > 0) {

                $flag = true;
            }
        }
    }

    if (isset($user->roles[NTLP_ROLEID_NETWORKSTAFF]) || isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])) {
        $flag = true;
    }

    return $flag;
}

function is_user_enrolled($course_id, $user_id) {
    $sql = "SELECT role_id from {ntlp_course_user}
        WHERE   course_nid=$course_id
        AND     user_uid=$user_id";

    $rs = db_fetch_object(db_query($sql));
    return (isset($rs) && isset($rs->role_id));
}

function check_current_course_outcome_with_newyear($scr_course_nid, $target_year) {
    $data = array();
    $result = db_query("SELECT DISTINCT ot.tid, td.name, co.weight_percent, td.description, td.vid, ot.short_name
        FROM {ntlp_gb_outcome} ot
        INNER JOIN {ntlp_gb_course_outcome} co ON co.tid = ot.tid
        INNER JOIN {term_data} td ON td.tid = ot.tid
        WHERE co.course_nid = %d 
        AND ot.tid NOT IN (SELECT got.tid FROM {ntlp_gb_outcome} got 
        WHERE got.school_year = %d)", $scr_course_nid, $target_year);

    while ($record = db_fetch_object($result)) {
        $data[] = $record;
    }

    return $data;
}

function check_target_year_terms($school_year) {
    $flag = false;
    $result = db_query("SELECT * FROM {ntlp_school_term} 
     WHERE school_year = %d AND school_term_tid <> 0", $school_year);

    if ($result->num_rows > 0) {
        $flag = true;
    }
    return $flag;
}
