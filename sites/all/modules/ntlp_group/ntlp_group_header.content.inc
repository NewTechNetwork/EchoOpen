<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');

function ntlp_groups_render_header() {
    drupal_add_css(drupal_get_path('theme', 'Boldr') . '/StylishSelect/stylish-select.css', 'module', 'all', FALSE);
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/externalFile.js');
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/jquery.stylish-select.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_blocks') . '/default_header.js');

    include "ntlp_group_header.template.inc";

    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js', 'footer');

    global $user, $base_path;


    $get = $_GET['q'];
    $br = explode("/", $get);
    $cnt = count($br);
    $active_tab = "@IS_" . strtoupper($br[$cnt - 2]);
    $group_id = $br[$cnt - 1];


    $check_group = is_group_exist($group_id);
//
//     if($check_group == false){
//
//         drupal_goto('ntlp/goback')
//     }


    $container = $HTMLcontainer;
    $container = str_replace('@LINK_HOME', l('Home', 'ntlp/home'), $container);
    $container = str_replace('@LINK_GROUPS', l('Groups', 'ntlp/groups/my', array('attributes' => array('class'=> 'green_bold'))), $container);

//    if ($check_group) {


    if (is_numeric($group_id)) {
        $userRole = check_user_permission($group_id); // checking permission for group
    }

    $current_user_role = get_user_role($user->uid);





    $sameSchool = true;
    if (is_numeric($group_id)) {
        $groupObj = get_group_info($group_id);
        $sameSchool = $groupObj->school_nid == get_user_school($user->uid);
        watchdog('group_perm', 'group school_nid ' . $groupObj->school_nid . ' user school nid ' . get_user_school($user->uid) . ' For Group:' . $group_id);
    }

    if ($sameSchool) {

        if ($br[2] == 'all' || $br[2] == 'my') {
            if (check_user_permission_for_access($current_user_role->rid, get_this_school() . ' create groups at my school')) {
                $container = str_replace('@LINK_CREATE_GROUPS', dlg('Create Group', 'ntlp/group/add', 400, 600, 'anchorLink'), $container);
            } else {
                $container = str_replace('@LINK_CREATE_GROUPS', '', $container);
            }
        } else {
            $container = str_replace('@LINK_CREATE_GROUPS', '', $container);
        }
    } else {
        if ($current_user_role == NTLP_ROLEID_NETWORKMANAGER || $current_user_role == NTLP_ROLEID_NETWORKSTAFF) {
            $container = str_replace('@LINK_CREATE_GROUPS', dlg('Create Group', 'ntlp/group/add', 400, 600, 'anchorLink'), $container);
        } else {
            $container = str_replace('@LINK_CREATE_GROUPS', '', $container);
        }
    }


    if (is_numeric($group_id)) {
        $user_exit = check_group_user($group_id);

        if ($br[2] == 'home') {
            if (!$user_exit) {

                if (check_user_permission_for_access($current_user_role->rid, get_this_school() . ' join groups at my school')
                        || $current_user_role == NTLP_ROLEID_NETWORKMANAGER || $current_user_role == NTLP_ROLEID_NETWORKSTAFF) {
                    $container = str_replace('<!--  @LINK_JOIN_GROUPS -->', dlg('Join Group', 'ntlp/group/join/' . $group_id, 400, 500, 'anchorLink'), $container);
                }
            }
        }

        $output .= $HTMLgroup_manage;

//        if( $userRole != NTLP_ROLEID_SITEGUEST ) {

        if (is_user_group_admin($group_id, $user) || isset($user->roles[NTLP_ROLEID_NETWORKSTAFF])
                || isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])) {

            $output = str_replace('@MANAGE_COURSE_EDIT', l('Edit Group Settings', 'ntlp/groups/edit/' . $group_id), $output);
            $output = str_replace('@MANAGE_COURSE_COPY', ' ', $output);
            $output = str_replace('@MANAGE_COURSE_DELETE', dlg('Delete Group', 'ntlp/group/delete/popup/' . $group_id, 200, 200, 'anchorLink'), $output);
            $container = str_replace('@GROUP_TOOLS', $output, $container);
        } else {
            $container = str_replace('@GROUP_TOOLS', '', $container);
        }
    } else {

        $container = str_replace('@GROUP_TOOLS', '', $container);
    }

//    } else{
//
//        drupal_goto('ntlp/groups/my');
//    }
//    $rClink = '';
//    if ($all == TRUE && user_access('create ntlp_course content')) {
//        $rClink = $HTMLnewlink;
//        $rClink = str_replace('@LINK_NEW', l('Create New Course', 'ntlp_admincourse'), $rClink);
//    }
    return $container;
}

function ntlp_groups_render_tabs($active_tab = "NOP") {
    drupal_add_css(drupal_get_path('theme', 'Boldr') . '/StylishSelect/stylish-select.css', 'module', 'all', FALSE);
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/externalFile.js');
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/jquery.stylish-select.js');

    include "ntlp_group_header.template.inc";


    $tab = arg(2);
    $not_active_tabs = arg(2);



    if ($tab == 'forum') {
        $tab = 'discussions';
    } else if ($tab == 'edit') {
        $tab = 'home';
    } else if ($tab == 'resource') {
        $tab = 'resources';
    }


    if ($active_tab == "NOP") {
        $get = $_GET['q'];
        $br = explode("/", $get);
        $cnt = count($br);
//        $active_tab = "@IS_".strtoupper($br[$cnt-2]);
        $active_tab = "@IS_" . strtoupper($tab);
        foreach ($br as $q) {
            if (is_numeric($q)) {
                $group_id = $q;
                break;
            }
        }
    }

    $node = node_load($group_id);

    if (is_numeric($group_id)) {

        if (is_group_exist($group_id)) {


            if ($tab == 'home' || $tab == 'resources' || $tab == 'discussions' || $tab == 'member' || $tab == 'events') {
                $output = $HTML_group_page_tabs;

                $output = str_replace("@GROUP_TITLE", $node->title, $output);

                if ($not_active_tabs != 'edit') {
                    $output = str_replace($active_tab, "active", $output);          //Set the active tab
                }
                $output = str_replace("@START_URL", url("/ntlp/groups/home/" . $group_id), $output);
                $output = str_replace("@RESOURCES_URL", url('/ntlp/groups/resources/' . $group_id), $output);
                $output = str_replace("@DISCUSSIONS_URL", url("ntlp/groups/forum/$group_id"), $output);
                $output = str_replace("@MEMBERS_URL", url('/ntlp/groups/member/' . $group_id), $output);
                $output = str_replace("@EVENTS_URL", url('/ntlp/groups/events/' . $group_id), $output);

                $output = str_replace("@IS_HOME", "", $output);
                $output = str_replace("@IS_RESOURCES", "", $output);
                $output = str_replace("@IS_DISCUSSIONS", "", $output);
                $output = str_replace("@IS_MEMBER", "", $output);
                $output = str_replace("@IS_EVENTS", "", $output);
            }
        } else {
            drupal_set_message("This Group is not Exist");
            drupal_goto('ntlp/groups/my');
        }
    } else {
        $active_tab = "@IS_" . strtoupper($tab);

        $output = $HTML_groups_tabs;

        if ($not_active_tabs != 'edit') {
            $output = str_replace($active_tab, "active", $output);
        }

        $output = str_replace("@MY_GROUP_URL", url("/ntlp/groups/my"), $output);
        $output = str_replace("@ALL_GROUP_URL", url("/ntlp/groups/all"), $output);

        $output = str_replace("@IS_MY", "", $output);
        $output = str_replace("@IS_ALL", "", $output);
    }


    return $output;
}

?>
