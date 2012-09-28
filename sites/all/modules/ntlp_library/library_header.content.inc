<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'constants');

function ntlp_library_render_header() {
    drupal_add_css(drupal_get_path('theme', 'Boldr') . '/StylishSelect/stylish-select.css', 'module', 'all', FALSE);
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/externalFile.js');
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/jquery.stylish-select.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_blocks') . '/default_header.js');

    include "library_header.template.inc";

    global $user, $base_path;
    $user_id = $user->uid;

    $get = $_GET['q'];
    $br = explode("/",$get);
    $cnt = count($br);
    $active_tab = $br[$cnt-2];
    $tab = $br[$cnt-1];

    if($br[2] == 'project') {

        $container = $HTMLcontainer;
        $container = str_replace('@LINK_HOME', l('Home', 'ntlp/home'), $container);
        $container = str_replace('@LINK_LIBRARY', l('Library', 'ntlp/library'), $container);
        
        $module_Obj = _get_project_library_detail();
        $user_role = check_user_permission($module_Obj->nid);
        if($user_role == NTLP_ROLEID_NETWORKSTAFF || $user_role == NTLP_ROLEID_NETWORKMANAGER
                || $user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_SITEADMIN
                || $user_role == NTLP_ROLEID_SCHOOLLEADER ) {
          
        }else{
              drupal_goto('ntlp/render/error/page/You are not authorized to Access Project Library', array('destination' => $_GET['q']));
        }
        $container = str_replace('@LINK_PROJECT_LIBRARY', l('Project Library', 'ntlp/library/project'), $container);

        if($user_role == NTLP_ROLEID_NETWORKSTAFF || $user_role == NTLP_ROLEID_NETWORKMANAGER ) {
            $container = str_replace('@LIBRARY_PROJECT_TOOLS', l('Manage', 'ntlp/library/projects/manage'), $container);
        }else {
            $container = str_replace('@LIBRARY_PROJECT_TOOLS', '', $container);
        }
    }else {
        $container = str_replace('@LINK_PROJECT_LIBRARY', '', $container);
        $container = str_replace('@LIBRARY_PROJECT_TOOLS', '', $container);
    }
    return $container;

}

?>
