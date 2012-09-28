<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_school', 'data_access');

function ntlp_resource_library_render_header() {

    drupal_add_css(drupal_get_path('theme', 'Boldr') . '/StylishSelect/stylish-select.css', 'module', 'all', FALSE);
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/externalFile.js');
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/jquery.stylish-select.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_blocks') . '/default_header.js');

    include "ntlp_resource_library_header.template.inc";

    global $user, $base_path;

    $module_Obj =  _get_module_detail('ntlp_network_resource_library');
    $user_role = check_user_permission($module_Obj->nid);

    $container = $HTMLcontainer;
    $container = str_replace('@LINK_HOME', l('Home', 'ntlp/home'), $container);
    $container = str_replace('@LINK_LIBRARY', l('Library', 'ntlp/library'), $container);
    $container = str_replace('@LINK_RESOURCE_LIBRARY', l('Network Resource Library', 'ntlp/resource/library'), $container);

    if ($user->uid == 1) return true;   //All admin to administer pages
    
    if($user_role == NTLP_ROLEID_NETWORKMANAGER || $user_role == NTLP_ROLEID_NETWORKSTAFF) {
        $container = str_replace('@MANAGE_BTN', l('Manage','ntlp/resource/library/manage'), $container);
    }else {
        $container = str_replace('@MANAGE_BTN', '', $container);
    }

//    if(user_access('NETWORK: add resources')) {
    if($user_role == NTLP_ROLEID_NETWORKMANAGER || $user_role == NTLP_ROLEID_NETWORKSTAFF) {
        $container = str_replace('@ADD_RESOURCE_BTN', l('Add Resource','ntlp/resource/library/add/resource'), $container);
    }else {
        $container = str_replace('@ADD_RESOURCE_BTN', '', $container);
    }

    $container = str_replace('@TITLE', 'Network Resource Library', $container);

//    $container = str_replace('@RESOURCE_LIBRARY_TOOLS', get_school_control(), $container);


    return $container;

}

?>
