<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_course', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_resource_library', 'data_access');

require_once ('data_access.inc');

function library_category_resources_contents() {
    return drupal_get_form('library_category_resrouces_form');
}

function library_category_resrouces_form() {
    global $base_path;

    include('ntlp_doc_lib_resource.template.inc');

    //Getting Query String From URL
    $category_tid = 0;
    $url = $_GET['q'];
    $args = explode('/', $url);
    for ($i = 0; $i < count($args); $i++) {
        if (is_numeric($args[$i]) && empty($category_tid))
            $category_tid = $args[$i];
    }
    set_item_url_handler('School Resource Library');

    $form['categories'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div id="project_add_test"><div><div style="float:left;font-size:17px;color:#3570aa">Resources</div><div style="float:right"><b style="color:#3570aa">Sort by:</b>&nbsp;&nbsp;<a class="greenLink" href="#">Date</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a class="greenLink" href="#">Title</a></div></div><br clear="all" />',
        '#suffix' => '</div>',
    );

    $form['categories']['course_nid'] = array(
        '#type' => 'hidden',
        '#value' => $category_tid,
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


    if (isset($category_tid)) {
        $term_obj = taxonomy_get_term($category_tid);

        /* check the permission for user on category.. */
        if (!is_user_access_this_category(get_this_school(), $term_obj->name)) {
//            drupal_goto('ntlp/goback');
            drupal_goto('ntlp/render/error/page/You are not Authorize to see this Document Library Category');
        }
    }

    $form['categories']['table']['row1']['td2'] = array(
        '#type' => 'item',
        '#value' => ntlp_school_library_resource_search($category_tid),
        '#prefix' => '<td>',
        '#suffix' => '</td>',
    );

    return $form;
}

//school resource library view
function ntlp_document_lib_resource_view_contents() {
    global $user, $base_path;
    include('ntlp_doc_lib_resource.template.inc');

    $dateformat = "F j, Y";

    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_document_library') . '/ntlp_document_library.js');

    $resource_nid = intval(arg(5));

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
    $category_tid = intval(arg(6));

    if (isset($category_tid)) {
        // check category existance and node relation code
    }
    $school_id = get_this_school();

    /* if category provided in url then check category permission for role */
    if ($category_tid > 0) {
        $term_obj = taxonomy_get_term($category_tid);

        /* check the permission for user on category.. */
        if (!is_user_access_this_category($school_id, $term_obj->name)) {
            drupal_goto('ntlp/render/error/page/You are not Authorize to see this Document Library Category' . 'ntlp_document_lib_resource_view_contents');
        }
        $back_url_link = '<a style="font-weight:bold;" class="green_link" href="' . url('ntlp/document/library/search/' . $category_tid) . '">' . $term_obj->name . '</a>';
    } else {
        
//      $back_url_link = '<a style="font-weight:bold;" class="green_link" href="' . url('ntlp/goback') . '">« Back to ' . $back_url->page_name . '</a>';
        $back_url_link = false;
    }

    $keywords = get_resource_keywords($resource_nid);


//    $module_Obj = _get_module_detail('ntlp_school_document_library', $school_id);

    $user_role = check_user_permission($school_id);

    $resource_edit = 'ntlp/document/library/edit/resource/' . $resource_nid;

    $can_edit = ($user_role >= NTLP_ROLEID_TEACHER || is_resource_owner($resource_nid, $user));

    $container = render_library_resource_view($back_url_link, $resource_nid, $keywords, $resource_edit, $can_edit, get_user_data($resource_node->uid));

    $container .= '<div style="display: none;" id="dialog-confirm-del-resource" title="Delete Resource?">
                    <p>Are you sure you want to delete this resource?</p>
                   </div>';

    return $container;
}

function ntlp_document_resource_delete_frm($resource_nid) {
    return drupal_get_form('ntlp_document_resource_delete_form', $resource_nid);
}

function ntlp_document_resource_delete_form($form= null, $resource_nid) {

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
        '#value' => '<a onclick="delete_resource_data(' . $resource_nid . '); parent.Drupal.modalFrame.close(); parent.window.location.reload();">Yes</a> &nbsp; <a onclick="delete_resource_cancel()">No</a>',
        '#prefix' => '<td>',
        '#suffix' => '</td>',
    );



    return $form;
}

function ntlp_document_resource_delete_data($resource_nid) {
    global $user;
    watchdog('resource_delete', 'selected resource id for delete ' . $resource_nid);


    db_query("DELETE FROM {term_node} WHERE nid = %d", $resource_nid);
    db_query("UPDATE {ntlp_library_resource} SET deleted_by = %d, deleted_on = '%s' WHERE nid = %d", $user->uid, strftime(DATE_FORMAT, time()), $resource_nid);


//    drupal_goto('ntlp/goback');
}

function get_resource_keywords($resource_nid) {

    $resource_node = node_load($resource_nid);
    //Get Resource Keywords 
    $node_related_taxo = taxonomy_node_get_terms($resource_node);

    foreach ($node_related_taxo as $Obj) {

        if ($Obj->vid == NTLP_KEYWORDS_FOR_SCHOOL_DOCUMENT_LIBRARY) {
            $keywords_array[] = $Obj;
        }
    }

    if ($keywords_array != null) {
        $i = 0;
        foreach ($keywords_array as $keyword) {
            $keywords .= '<a class="green_link" href="' . url('ntlp/document/library/search/' . $keyword->tid) . '">' . $keyword->name . '</a>';
            if (!empty($keywords_array[$i + 1]))
                $keywords .= ', &nbsp;';
            $i++;
        }
    }

    return $keywords;
}

?>