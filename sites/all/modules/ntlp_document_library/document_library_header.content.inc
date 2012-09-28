<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');

function ntlp_document_render_header() {
    drupal_add_css(drupal_get_path('theme', 'Boldr') . '/StylishSelect/stylish-select.css', 'module', 'all', FALSE);
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/externalFile.js');
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/jquery.stylish-select.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_blocks') . '/default_header.js');

    include "document_library_header.template.inc";

    global $user, $base_path;

    $get = $_GET['q'];
    $br = explode("/",$get);
    $cnt = count($br);
    $active_tab = "@IS_".strtoupper($br[$cnt-2]);
    $group_id = $br[$cnt-1];

    $school_id = get_this_school();
    $module_Obj =  _get_module_detail('ntlp_school_document_library', $school_id);

    if ($user->uid == 1) return;        //All admin user to administer pages
    $user_role = check_user_permission($module_Obj->nid);

    $container = $HTMLcontainer;
    $container = str_replace('@LINK_HOME', l('Home', 'ntlp/home'), $container);
    $container = str_replace('@LINK_LIBRARY', l('Library', 'ntlp/library'), $container);
    $container = str_replace('@LINK_DOCUMENT_LIBRARY', l('School Resource Library', 'ntlp/document/resource/library'), $container);

//    if(user_access('ntlp add category')) {
//        $container = str_replace('@MANAGE_BTN', l('Manage','ntlp/document/library/manage'), $container);
//    }else {
//        $container = str_replace('@MANAGE_BTN', '', $container);
//    }
//
    if($user_role == NTLP_ROLEID_NETWORKMANAGER
            || $user_role == NTLP_ROLEID_NETWORKSTAFF
            || $user_role == NTLP_ROLEID_SCHOOLLEADER
            || $user_role == NTLP_ROLEID_SITEADMIN ) {

        $container = str_replace('@MANAGE_BTN', l('Manage','ntlp/document/library/manage'), $container);

    }else {
        $container = str_replace('@MANAGE_BTN', '', $container);
    }

//    if(user_access('ntlp add resources')) {
//     ||
    if($user_role == NTLP_ROLEID_NETWORKMANAGER
            || $user_role == NTLP_ROLEID_NETWORKSTAFF
            || $user_role == NTLP_ROLEID_SITEADMIN
            || $user_role == NTLP_ROLEID_SCHOOLLEADER
            || $user_role == NTLP_ROLEID_TEACHER) {

        $container = str_replace('@ADD_RESOURCE_BTN', l('Add Resource','ntlp/document/library/add/resource'), $container);
    }else {
        $container = str_replace('@ADD_RESOURCE_BTN', '', $container);
    }

    $container = str_replace('@TITLE', 'School Resource Library', $container);
    if($user_role == NTLP_ROLEID_NETWORKMANAGER || $user_role == NTLP_ROLEID_NETWORKSTAFF ) {
        $container = str_replace('@DOCUMENT_LIBRARY_TOOLS', get_school_control(), $container);
    }else {
        $container = str_replace('@DOCUMENT_LIBRARY_TOOLS', '', $container);
    }

    return $container;

}

?>
