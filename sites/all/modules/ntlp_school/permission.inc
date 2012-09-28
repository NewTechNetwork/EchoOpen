<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


require_once "constants.inc";

function check_resource_permission($node, $user) {
//    watchdog('chk_res_xs', 'Node:'.print_r($node, true).'<br><br>User:'.print_r($user, true));
    $records = is_user_access_this_resource($node, $user);
//    watchdog('chk_res_xs', print_r($records, true));
    if ($records) {
        return $records;
    } else if (isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])) {
        return NTLP_ROLEID_NETWORKMANAGER;
    } else if (isset($user->roles[NTLP_ROLEID_NETWORKSTAFF])) {
        return NTLP_ROLEID_NETWORKSTAFF;
    } else {
        $in_this_school = check_user_school($user);
        if (isset($user->roles[NTLP_ROLEID_TEACHER]) && $in_this_school) {
            return NTLP_ROLEID_SITEGUEST;
        } else if (isset($user->roles[NTLP_ROLEID_SITEADMIN]) && $in_this_school) {
            return NTLP_ROLEID_TEACHER;
        } else if (isset($user->roles[NTLP_ROLEID_SCHOOLLEADER]) && $in_this_school) {
            return NTLP_ROLEID_TEACHER;
            //No reason to return parent role here.
//        }else if(isset($user->roles[NTLP_ROLEID_PARENT]) && check_user_school($user)) {
//            return NTLP_ROLEID_SITEGUEST;
        } else {
//            watchdog('check_res_role', 'Not found');
        }
    }
}

function check_profile_access_permission($profile_user_obj) {
    global $user;

    switch ($profile_user_obj->role_name) {

        case 'Staff':
            $result = check_permission_for_staff($profile_user_obj, $user);

            if ($result) {
                return $result;
            } else {
                drupal_goto('ntlp/render/error/page/You are not authorized to view this Staff profile.', array('destination' => $_GET['q']));
            }
            break;

        case 'Student':
            $result = check_permission_for_student($profile_user_obj, $user);

            if ($result) {
                return $result;
            } else {
                drupal_goto('ntlp/render/error/page/You are not authorized to view this Student profile.', array('destination' => $_GET['q']));
            }
            break;

        case 'Parent':
            check_permission_for_parent($profile_user_obj, $user);
            break;

        case 'School Leader':
            break;
    }
}

function check_permission_for_student($profile_user_obj, $user) {
    $flag = false;
    $current_user_school = get_this_school();

    if (isset($user->roles[NTLP_ROLEID_STUDENT])) { //Student
        watchdog('ntlp_profile', " for student case $profile_user_obj->school_nid checing a same school for profile permission $current_user_school");
        if ($profile_user_obj->school_nid == $current_user_school) { //Students at their school
            $flag = true;
        } else {
            $school_nid = get_user_school($profile_user_obj->uid); // Staff at other schools where they are virtually enrolled
            watchdog("ntlp_profile", 'check profile status ' . user_access($school_nid . ' share/view profiles across the network'));

            if (check_user_permission_for_access($profile_user_obj->rid, $school_nid . ' share/view profiles across the network')) {
                $flag = true;
            } else {
                $flag = false;
            }
        }
    } else if (isset($user->roles[NTLP_ROLEID_PARENT])) { // Their own parents can access their student
        watchdog('parent_checking', 'parent check to see student profile ' . $current_user_school);
        $flag = check_user_relation($profile_user_obj->uid, $user->uid);
    } else if (isset($user->roles[NTLP_ROLEID_TEACHER]) || isset($user->roles[NTLP_ROLEID_SCHOOLLEADER]) || isset($user->roles[NTLP_ROLEID_SITEADMIN])) {

        $current_user_school = get_this_school();
        watchdog('ntlp_profile', " for teacher case $profile_user_obj->school_nid checing a same school for profile permission $current_user_school");
        if ($profile_user_obj->school_nid == $current_user_school) { // Staff at their school
            $flag = true;
        } else {

            $school_nid = get_user_school($profile_user_obj->uid); // Staff at other schools where they are virtually enrolled
            watchdog("ntlp_profile", 'check profile status ' . user_access($school_nid . ' share/view profiles across the network'));

            if (check_user_permission_for_access($profile_user_obj->rid, $school_nid . ' share/view profiles across the network')) {
                $flag = true;
            } else {
                $flag = false;
            }
        }
    } else if (isset($user->roles[NTLP_ROLEID_NETWORKSTAFF]) || isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])) { //NETWORK staff
        $flag = true;
    }

    return $flag;
}

function check_permission_for_staff($profile_user_obj, $user) {
    $flag = false;
    $current_user_school = get_this_school();

    if (isset($user->roles[NTLP_ROLEID_STUDENT])) { //Student
        watchdog('ntlp_profile', " for student case $profile_user_obj->school_nid checing a same school for profile permission $current_user_school");
        if ($profile_user_obj->school_nid == $current_user_school) { //Students at their school
            $flag = true;
        } else {

            $school_nid = get_user_school($profile_user_obj->uid);
            watchdog("ntlp_profile", 'check profile status ' . user_access($school_nid . ' share/view profiles across the network'));

            if (check_user_permission_for_access($profile_user_obj->rid, $school_nid . ' share/view profiles across the network')) {
                $flag = true;
            } else {
                $flag = false;
            }
        }
    } else if (isset($user->roles[NTLP_ROLEID_PARENT])) { // Parents at their school
        watchdog('parent_checking', 'parent check to see teacher profile ' . $current_user_school);
        if ($profile_user_obj->school_nid == $current_user_school) { //Parents at their school
            $flag = true;
        } else {
            $flag = false;
        }
    } else if (isset($user->roles[NTLP_ROLEID_TEACHER]) || isset($user->roles[NTLP_ROLEID_SCHOOLLEADER]) || isset($user->roles[NTLP_ROLEID_SITEADMIN])) {

        $current_user_school = get_this_school();
        watchdog('ntlp_profile', " for teacher case $profile_user_obj->school_nid checing a same school for profile permission $current_user_school");
        if ($profile_user_obj->school_nid == $current_user_school) { //Staff at their school
            $flag = true;
        } else {

            $school_nid = get_user_school($profile_user_obj->uid);
            watchdog("ntlp_profile", 'check_user_permission_for_role ' . check_user_permission_for_access($profile_user_obj->rid, $school_nid . ' share/view profiles across the network'));
            if (check_user_permission_for_access($profile_user_obj->rid, $school_nid . ' share/view profiles across the network')) {
                $flag = true;
            } else {
                $flag = false;
            }
        }
    } else if (isset($user->roles[NTLP_ROLEID_NETWORKSTAFF]) || isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])) { //NETWORK staff
        $flag = true;
    }

    return $flag;

//   School Staff profiles are viewable only by:
//  ▪ Students from other schools (if allowed by BOTH school admins)
//  ▪ Staff at all other schools
}

/* This function check school wise permission */

function check_user_permission_for_access($role_id, $permission) {
    $flag = false;
    watchdog("chk_user_perm 1", 'role id ' . $role_id . ' permission ' . $permission);

    if ($role_id == NTLP_ROLEID_NETWORKMANAGER || $role_id == NTLP_ROLEID_NETWORKSTAFF) { // return ture in case of NETWORK roles
        $flag = true;
    } else {
        watchdog('chk_user_perm 2', 'parameter recieved role_id ' . $role_id . ' permission ' . $permission);

        $result = db_query("SELECT lower(perm) perm FROM {permission} WHERE rid = %d", $role_id);
        while ($row = db_fetch_object($result)) {
            $perms = explode(', ', $row->perm);
        }

        watchdog("select_role_perm", print_r($perms, true));
        if ($perms && in_array(strtolower($permission), $perms)) { // if specific permission set to this role then return true
            $flag = true;
        } else {
            $flag = false;
        }
    }
    watchdog("chk_user_perm 3", 'role id ' . $role_id . ' permission ' . $permission . 'status: ' . ($flag ? 'true' : 'false'));
    return $flag;
}

function check_user_relation($student_id, $parent_id) {
    $flag = false;

    watchdog('check_user_rel', 'Student ID: ' . $student_id . ' Parent ID: ' . $parent_id);
    $result = db_query("SELECT * FROM {ntlp_user_parent}
        WHERE parent_uid = %d AND student_uid = %d", $parent_id, $student_id);


    watchdog('check_user_rel', print_r($result, true));
    if ($result->num_rows > 0) {
        $flag = true;
    } else {
        $flag = false;
    }
    return $flag;
}

function check_permission_for_parent($profile_user_Obj, $user) {
//        Parent profiles are viewable only by:
    //            ▪ Parents at their school
    //            ▪ Their own students
    //            ▪ Staff at their school
    //            ▪ NETWORK staff
}

function display_instant_messaging_ID_field_permission() {

}

//////////
// Event permisssions
function check_event_permission($node, $user, $type) {
    watchdog('chk_event_xs', 'Node:' . print_r($node, true) . '<br><br>User:' . print_r($user, true));
    if (isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])) {
        return NTLP_ROLEID_NETWORKMANAGER;
    } else if (isset($user->roles[NTLP_ROLEID_NETWORKSTAFF])) {
        return NTLP_ROLEID_NETWORKMANAGER;
    } else {
        $in_this_school = check_user_school($user);
        if (isset($user->roles[NTLP_ROLEID_SITEADMIN]) && $in_this_school) {
            return NTLP_ROLEID_TEACHER;
        } else if (isset($user->roles[NTLP_ROLEID_SCHOOLLEADER]) && $in_this_school) {
            return NTLP_ROLEID_TEACHER;

            //No reason to return parent role here.
            //    }else if(isset($user->roles[NTLP_ROLEID_PARENT]) && check_user_school($user)) {
            //        return NTLP_ROLEID_SITEGUEST;
        } else {
            if (is_user_adminof_this_event($node, $user, $type)) {
                return NTLP_ROLEID_TEACHER;
            } else {
                if (isset($user->roles[NTLP_ROLEID_TEACHER]) && $in_this_school) {
                    return NTLP_ROLEID_SITEGUEST;
                    //          watchdog('check_res_role', 'Not found');
                }
            }
        }
    }
}

