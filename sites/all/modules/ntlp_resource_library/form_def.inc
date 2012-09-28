<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_document_library', 'data_access');
require_once ('data_access.inc');
module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_school', 'data_access');

/**
 * Form controller
 */
function ntlp_library_resource_form_controller($form_state) {
    // Add the client-side behaviors for the examples.
    global $user;

    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') .'/modalframe_setup.js');

    drupal_add_js(drupal_get_path('module', 'ntlp_resource_library') . '/ntlp_resource_library.js');
    include('ntlp_module.template.inc');

    $form = array();
    $doc_lib_Obj = _get_module_detail('ntlp_network_resource_library');
    $user_role = check_user_permission($doc_lib_Obj->nid);

    
    $tab = check_plain(arg(5));        //new or edit


//    watchdog('module_page', 'mode '.$mode.' module_nid '.$module_nid .' tab '.$tab );
//    watchdog('info', 'Project Library Form Controller view state:<br />'.print_r($form_state, true));
    if(isset($form_state['values']['op'])) {
        $view_state = $form_state['values']['op'];
        $school_nid = $form_state['values']['module_node'];
    }


    if (!isset($tab) || empty($tab)) {
        $tab = 'categories';
    }



    if (isset($doc_lib_Obj->nid)) {
        $rec = get_resource_lib_module($doc_lib_Obj->nid);
        if ($rec == false) {
            drupal_set_message("Module not found, please contact system administrator");
            return;
        }
    }else {
        drupal_set_message("Module not found, please contact system administrator");
        return;
    }



    $form['main'] = array(
            '#type' => 'fieldset',
            '#title' => t(''),
            '#prefix' => $HTMLform_course_setting_start,
            '#suffix' => $HTMLform_course_setting_end,
    );
    $form['main']['module_node'] = array(
            '#type' => 'hidden',
            '#value' => $doc_lib_Obj->nid,
    );

    if($tab == 'categories') {
        $heading = "Network Resource Library: Categories";
    }else if($tab == 'setup') {
        $heading = "Network Resource Library: Setup";
    }

    if ($tab == 'categories' ) {
        $form['main']['save'] = array(
                '#type' => 'submit',
                '#value' => 'Save',
                '#prefix' => '<td>&nbsp;</td><td><table cellpadding="0" cellspacing="0" width="100%"><tr><td><div><a href="'.url('ntlp/resource/library/').'" class="greenLink">« Back to Network Resource Library</a></div><div><h2 style="padding:0px;margin:0px;color:#3570aa;font-size:18px;">'.$heading.'</h2></div></td><td align="right">',
                '#suffix' => '</td></table></td></tr><tr>',
                '#submit' => array('nltp_lib_doc_submit_'.$tab),
                '#attributes' => array('class' => 'SaveBtn'),
        );

    } else {
        $form['main']['save'] = array(
                '#type' => 'item',
                '#value' => '',
                '#prefix' => '<td>&nbsp;</td><td><table cellpadding="0" cellspacing="0" width="100%"><tr><td><div><a href="'.url('ntlp/resource/library/').'" class="greenLink">« Back to Network Resource Library</a></div><div><h2 style="padding:0px;margin:0px;color:#3570aa;font-size:18px;">'.$heading.'</h2></div></td><td align="right">',
                '#suffix' => '</td></table></td></tr><tr>',
        );
    }
    ntlp_library_resource_add_tabs($doc_lib_Obj->nid, $form['main'], $tab);

    $form['main']['form_start'] = array(
            '#type' => 'markup',
            '#value' => $HTMLform_course_setting_form_start);
    if ($tab != 'review_needed')
        $form['main']['form_start0'] = array(
                '#type' => 'markup',
                '#value' => $HTMLform_course_setting_bluebox_start);

    $form['main']['form'] = array(
            '#type' => 'fieldset',
            '#title' => t(''),
    );

    if ($tab == 'categories') {
        ntlp_library_resource_tab1_categories($form_state, $form['main']['form'], $doc_lib_Obj->nid, $rec);
    } elseif($tab == 'setup') {
        ntlp_library_resource_tab2_setup($form_state, $form['main']['form'], $doc_lib_Obj->nid, $rec);
    }

    if ($tab != 'review_needed')
        $form['main']['form_end0'] = array(
                '#type' => 'markup',
                '#value' => $HTMLform_course_setting_bluebox_end);
    $form['main']['form_end'] = array(
            '#type' => 'markup',
            '#value' => $HTMLform_course_setting_form_end
    );

    return $form;
}

/**
 * Form definition
 */
function ntlp_library_resource_tab1_categories($form_state, &$form, $module_nid, $rec) {

    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    modalframe_parent_js();

    drupal_add_js(drupal_get_path('module', 'ntlp_document_library') . '/ntlp_document_library.js');
    $form['library_document_info'] = array(
            '#type' => 'item',
            '#value' => t('Network Resource Library: Categories '),
            '#prefix' => '<tr><th style="text-align:left;" colspan="3">',
            '#suffix' => '</th></tr>',
    );

    $data = _get_resource_lib_categories_list($rec->nid, $form);

    $form['library_document_table'] = array(
            '#type' => 'item',
            '#value' => $data,
            '#prefix' => '<tr><td class="gradesRadio">',
            '#suffix' => '</td></tr>',
    );
    $form['module_id'] = array(
            '#type' => 'hidden',
            '#value' => $module_nid,
            '#id' => 'module_id',
    );

    $form['categories'] = array(
            '#type' => 'hidden',
            '#id' => 'categories_perm',
    );

    $form['library_resource_new_category'] = array(
            '#type' => 'item',
            '#value' => dlg('+ Add New Category','ntlp/resource/library/add/category/'.$rec->nid.'/0',450, 600),
            '#prefix' => '<tr><td style="text-align:left;" colspan="3">',
            '#suffix' => '</td></tr>',
    );

    $mycontent = '<div id="dialog-confirm-cat" title="Delete Category" style="display:none">
            <p>You are about to permanently delete this category. This will also remove the resources contained in this category.</p>
            <p>Are you sure you want to permanently delete this category?</p>
            </div>';
    $form['confirm_delete'] = array(
            '#type' => 'item',
            '#value' => $mycontent,
    );

}



function ntlp_library_resource_tab2_setup($form_state, &$form, $module_nid, $rec) {
    global $base_path;

    drupal_add_js(drupal_get_path('module', 'ntlp_school') .'/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'image_uploader') . '/image_uploader.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_library') . '/ntlp_library.js');

    include('ntlp_module.template.inc');


    $form['save_btn'] = array(
            '#type' => 'submit',
            '#value' => t('Save'),
            '#submit' => array('nltp_lib_resource_submit_setup'),
            '#prefix' => '<tr><td colspan="2">',
            '#suffix' => '</td></tr>',
            '#attributes' => array('class' =>'SaveBtn'),
    );

    $form['module_id'] = array(
            '#type' => 'hidden',
            '#value' => $rec->nid,
            '#id' => 'module_id',
    );

    $form['title'] = array(
            '#type' => 'textfield',
            '#value' => $rec->title,
            '#prefix' => '<tr><th>'.t('Title').'</th><td >',
            '#suffix' => '</td></tr>',
    );

    $form['description'] = array(
            '#type' => 'textfield',
            '#value' => $rec->body,
            '#prefix' => '<tr><th>'.t('Description').'</th><td >',
            '#suffix' => '</td></tr>',
    );

    if($file_obj = _dpl_files_obj($rec->icon_fid)) {
        $picture = $base_path.$file_obj->filepath;
    }

    $form['picture_id'] = array(
            '#type' => 'hidden',
            '#value' => $rec->icon_fid,
            '#id' => 'picture_fid',

    );

    $form['archive_projects_view'] = array(
            '#type' => 'item',
            //'#value' => t('<--- Change Icon'),
            '#prefix' => '<tr><th>'.t('Icon').'</th><td> <div id="module_image" align="left">'.drupal_get_form('ntlp_activeimage', $picture, 0 ,150).'</div> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;',
            '#suffix' => '</td></tr>',
    );



}

function ntlp_library_resource_add_tabs($module_nid, &$form, $tab = '') {
    include('ntlp_module.template.inc');
    if (empty($module_nid)) {
        $url = "/new";
    } else {
        $url = "/$module_nid";
    }
    $form['tabs'] = array(
            '#type' => 'fieldset',
            '#title' => t(''),
            '#prefix' => $HTMLform_course_setting_tabs_start,
            '#suffix' => $HTMLform_course_setting_tabs_end,
    );
    $roption = $HTMLform_course_settings_tabs_option;
    $roption = str_replace("@LINK", url("ntlp/resource/library/manage/$module_nid"), $roption);
    $roption = str_replace("@PROMPT", "Categories", $roption);
    $roption = str_replace("@CLASS", ($tab != 'setup') ? "class='active'" : "", $roption);
    $form['tabs']['basic'] = array(
            '#type' => 'markup',
            '#value' => $roption,
    );

    $roption = $HTMLform_course_settings_tabs_option;
    $roption = str_replace("@LINK", url("ntlp/resource/library/manage$url/setup"), $roption);
    $roption = str_replace("@PROMPT", "Setup", $roption);
    $roption = str_replace("@CLASS", ($tab == 'setup') ? "class='active'" : "", $roption);
    $form['tabs']['data_export'] = array(
            '#type' => 'markup',
            '#value' => $roption,
    );
}


function _get_resource_lib_categories_list($module_nid, $form) {
    global $user;
    $dateformat = "m/d/Y";
    $head = array(
            array('data' => t('Categories'), 'field' => 'name', 'sort' => 'asc'),
            array('data' => t(''), 'width' => '80px'),
//            array('data' => t('All')),
//            array('data' => t('Teacher')),
//            array('data' => t('Student')),
//            array('data' => t('Parent')),
    );

    $sql = "SELECT lc.*, nm.*, REPLACE(td.name, '%2C', ',') as name
        FROM {ntlp_library_category} lc
        INNER JOIN {term_data} td on lc.category_tid = td.tid
        INNER JOIN {ntlp_module} nm ON nm.nid = lc.module_nid
        WHERE lc.module_nid =".$module_nid.tablesort_sql($head);

//    $result = db_query($sql);
    $limit = 5;
    $result = pager_query($sql, $limit);
    while ($archive_req = db_fetch_object($result)) {
//        $taxObj = taxonomy_get_term($archive_req->category_tid);
//        $school_Obj = node_load($archive_req->school_nid);
        $rows[] = array(
                array('data' => dlg($archive_req->name,'ntlp/library/add/category/'.$module_nid.'/'.$archive_req->category_tid,450, 600)),
                array('data' => '<a onclick="remove_resource_category('.$archive_req->category_tid.')">Delete</a>'),
//                array('data' => '<input type="checkbox" onclick="set_category_permission('.$archive_req->category_tid.')" id="teacher_'.$archive_req->category_tid.'" value="'.$school_Obj->title.' '.$taxObj->name.'_Teacher" '),
//                array('data' => '<input type="checkbox" onclick="set_category_permission('.$archive_req->category_tid.')" id="student_'.$archive_req->category_tid.'" value="'.$school_Obj->title.' '.$taxObj->name.'_Student" '),
//                array('data' => '<input type="checkbox" onclick="set_category_permission('.$archive_req->category_tid.')" id="parent_'.$archive_req->category_tid.'" value="'.$school_Obj->title.' '.$taxObj->name.'_Parent" '),

        );
    }

    $output = theme('table', $head, $rows, $attributes = array('class' => 'ntlp_table','width' => '100%','cellpadding' => '5'));

    $output .= theme('pager', NULL, $limit, 0);

    return $output;
}




function nltp_lib_resource_submit_setup($form, $form_state) {

//    print_r($form_state);
    $module_id = $form_state['clicked_button']['#post']['module_id'];
    $title = $form_state['clicked_button']['#post']['title'];
    $description = $form_state['clicked_button']['#post']['description'];

    $module_node = node_load($module_id);

    $module_node->title = $title;
    $module_node->body = $description;

    node_save($module_node);

}

function nltp_lib_resource_submit_categories($form, $form_state) {

}



?>
