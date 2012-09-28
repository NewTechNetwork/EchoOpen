<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_course', 'data_access');
require_once ('data_access.inc');

module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');

function resources_library_category_contents() {
    return drupal_get_form('resources_library_category_form');
}

function resources_library_category_form() {
    global $base_path;

    include('ntlp_resource_lib_resource.template.inc');
    if ($course_id == 0) {
//Getting Query String From URL
        $project_nid = 0;
        $course_id = 0;
        $url = $_GET['q'];
        $args = explode('/', $url);
        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i]) && empty($course_id))
                $course_id = $args[$i];
            elseif (is_numeric($args[$i]) && empty($project_nid))
                $project_nid = $args[$i];
        }
    }

    set_item_url_handler('Network Resource Library');

    $category_tid = $course_id;

    $form['categories'] = array(
            '#type'   => 'fieldset',
            '#title'  => '',
            '#tree'   => TRUE, // Don't forget to set #tree!
            '#prefix' => '<div id="project_add_test"><div><div style="float:left;font-size:17px;color:#3570aa">Resources</div><div style="float:right"><b style="color:#3570aa">Sort by:</b>&nbsp;&nbsp;<a class="greenLink" href="#">Date</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a class="greenLink" href="#">Title</a></div></div><br clear="all" />',
            '#suffix' => '</div>',
    );


    $form['categories']['project_id'] = array(
            '#type' => 'hidden',
            '#value' => $project_id,

    );

    $form['categories']['course_nid'] = array(
            '#type' => 'hidden',
            '#value' => $course_id,

    );

    $form['categories']['table'] = array(
            '#type' => 'item',
            '#prefix' => '<table cellpadding ="0" border="0" width="100%" >',
            '#suffix' => '</table>',

    );

    $form['categories']['table']['row1'] = array(
            '#type' => 'item',
            '#prefix' => '<tr>',
            '#suffix' => '</tr>',

    );
    $view = views_get_view('ntlp_network_resource_lib');

    $form['categories']['table']['row1']['td2'] = array(
            '#type' => 'item',
            '#value' =>  $view->preview('default', array($category_tid)),
            '#prefix' => '<td>',
            '#suffix' => '</td>',

    );

    return $form;
}

//ntn resource library view
function ntlp_resource_lib_view_contents() {
    global $user, $base_path;
    include('ntlp_resource_lib_resource.template.inc');

    $dateformat = "F j, Y";

    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') .'/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_resource_library') . '/ntlp_resource_library.js');

    $resource_nid = intval(arg(4));

    if (isset($resource_nid)) {
        if (is_document_library_resource_exist($resource_nid)) {
            $resource_node = node_load($resource_nid);
        } else {
            drupal_goto('ntlp/render/error/page/No Resource Found.');
        }
    } else {
        $resource_nid = false;
        drupal_goto('ntlp/render/error/page/Invalid URL Found.');
    }
    set_item_url_handler("Resource View");
    $category_tid = intval(arg(5));
    

    $module_Obj =  _get_module_detail('ntlp_network_resource_library');

    $user_role = check_user_permission($module_Obj->nid);

    $resource_node = node_load($resource_nid);

    //Get Keywords and Subjects
    $node_related_taxo = taxonomy_node_get_terms($resource_node);

    foreach($node_related_taxo as $Obj) {

        if($Obj->vid == NTLP_KEYWORDS_FOR_NETWORK_RESOURCE_LIBRARY) {
            $keywords_array[] = $Obj;
        }
    }

    if($keywords_array != null) {
        $i = 0;
        foreach($keywords_array as $keyword) {

            $resource_keywords .= '<a class="green_link" href="'.url('ntlp/resource/library/search/'.$keyword->tid).'">'.$keyword->name.'</a>';

            if(!empty($keywords_array[$i+1])) 
                $resource_keywords .= ', &nbsp;';
            
            $i++;
        }
    }

    $category = '<a style="font-weight:bold;" class="green_link" href="'.url('ntlp/resource/library/search/'.$category_tid).'">'. taxonomy_get_term($category_tid)->name .'</a>';

    $resource_edit = 'ntlp/resource/library/edit/resource/' . $resource_nid;

    $can_edit = ($user_role == NTLP_ROLEID_NETWORKMANAGER || $user_role == NTLP_ROLEID_NETWORKSTAFF
                || is_resource_owner($resource_Obj->nid, $user));
    
    $container=  render_library_resource_view($category, $resource_nid, $resource_keywords, $resource_edit, $can_edit, get_user_data($resource_node->uid));

    return $container;
}




function ntlp_network_resource_delete_frm($resource_nid) {
    return drupal_get_form('ntlp_network_resource_delete_form', $resource_nid);
}

function ntlp_network_resource_delete_form($form= null, $resource_nid) {

    $form = array();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_admincourse') . '/ntlp_admincourse.js');
    modalframe_child_js();

    $form = array();

    $form['courses_delete'] = array(
            '#type' => 'fieldset',
    );


    $form['resource_delete']['main_table'] = array(
            '#type' => 'item',
            '#value' => '',
            '#prefix' => '<table border="1">',
            '#suffix' => '</table>',
    );

    $form['resource_delete']['resource_nid'] = array(
            '#type' => 'hidden',
            '#value' => $resource_nid,
    );

    $form['resource_delete']['main_table']['rows'] = array(
            '#type' => 'item',
            '#prefix' => '<tr>',
            '#suffix' => '</tr>',
    );

    $form['resource_delete']['main_table']['rows']['tdata1'] = array(
            '#type' => 'item',
            '#value' => t('Are you sure to delete this Resource ? '),
            '#prefix' => '<td >',
            '#suffix' => '</td>',
            '#id' => 'txt_course_name',
    );

    $form['resource_delete']['main_table']['row_1'] = array(
            '#type' => 'item',
            '#prefix' => '<tr>',
            '#suffix' => '</tr>',
    );

    $form['resource_delete']['main_table']['row_1']['tdata1'] = array(
            '#type' => 'item',
            '#value' => '<a onclick="delete_resource_data('.$resource_nid.')">Yes</a> &nbsp; <a onclick="delete_resource_cancel()">No</a>',
            '#prefix' => '<td>',
            '#suffix' => '</td>',
    );



    return $form;
}


function ntlp_network_resource_delete_data($resource_nid) {

    watchdog('resource_delete', 'selected resource id for delete '.$resource_nid);


    db_query("DELETE FROM {term_node} WHERE nid = %d", $resource_nid);
    db_query("DELETE FROM {ntlp_library_resource} WHERE nid = %d", $resource_nid);


    node_delete($resource_nid);


}

?>
