<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


function ntlp_school_render_header() {

    drupal_add_js(drupal_get_path('module', 'ntlp_blocks') . '/default_header.js');

    include "school_header.template.inc";

    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');

    global $user, $base_path;


    $get = $_GET['q'];
    $br = explode("/", $get);
    $cnt = count($br);
    $active_tab = $br[$cnt - 2];
//    $group_id = $br[$cnt-1];



    $container = $HTMLcontainer;
    $container = str_replace('@LINK_HOME', l('Home', 'ntlp/home'), $container);
    $container = str_replace('@LINK_SCHOOLMANAGEMENT', l('School Management', 'ntlp/school/management/users'), $container);

    if ($active_tab == 'management') {
        if (isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])
                || isset($user->roles[NTLP_ROLEID_NETWORKSTAFF]) || isset($user->roles[NTLP_ROLEID_SYSTEM_ADMIN])) {

            $selectors .= get_school_control();
        }
    }

    $container = str_replace('<!-- selectors -->', $selectors, $container);

    return $container;
}

function ntlp_school_management_render_tabs($active_tab = "NOP", $course_nid = 0) {
    include "school_header.template.inc";
    if ($active_tab == "NOP") {
        $get = $_GET['q'];
        $br = explode("/", $get);
        $cnt = count($br);
        $active_tab = "@IS_" . strtoupper($br[$cnt - 1]);
        $tab = $br[$cnt - 2];
    }

    $output = $HTMLSchool_tabs;

    $output = str_replace("@USERS_URL", url("/ntlp/school/management/users"), $output);
    $output = str_replace("@GENERAL_URL", url("/ntlp/school/management/general"), $output);
    $output = str_replace("@COURSE_URL", url("/ntlp/school/management/course"), $output);
    $output = str_replace("@EXPORT_URL", url("/ntlp/school/management/export"), $output);


    $output = str_replace($active_tab, "active", $output);
    $output = str_replace("@IS_USERS", "", $output);
    $output = str_replace("@IS_GENERAL", "", $output);
    $output = str_replace("@IS_COURSE", "", $output);
    $output = str_replace("@IS_EXPORT", "", $output);

    return $output;
}

?>
