<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_school', 'data_access');

require_once ('data_access.inc');

/**
 * Form controller
 */
function ntlp_library_doc_form_controller($form_state) {
    // Add the client-side behaviors for the examples.
    global $user;

    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');

    drupal_add_js(drupal_get_path('module', 'ntlp_document_library') . '/ntlp_document_library.js');
    include('ntlp_module.template.inc');

    $form = array();

    $school_nid = get_this_school();        //new or edit

    if ($school_nid > 0) {
        $doc_lib_Obj = get_doc_lib_record($school_nid);
        $user_role = check_user_permission($doc_lib_Obj->nid);

        if ($user_role == NTLP_ROLEID_NETWORKSTAFF || $user_role = NTLP_ROLEID_NETWORKMANAGER) {
            
        } else {
            drupal_goto('ntlp/render/error/page/You are not Authorize to Manage School Resource Library', array('destination' => $_GET['q']));
        }

        $tab = check_plain(arg(5));        //new or edit
//    watchdog('module_page', 'mode '.$mode.' module_nid '.$module_nid .' tab '.$tab );
//    watchdog('info', 'Project Library Form Controller view state:<br />'.print_r($form_state, true));
        if (isset($form_state['values']['op'])) {
            $view_state = $form_state['values']['op'];
            $school_nid = $form_state['values']['module_node'];
        }


        if (!isset($tab) || empty($tab)) {
            $tab = 'permission';
        }



        if (isset($doc_lib_Obj->nid)) {
            $rec = get_lib_module($doc_lib_Obj->nid);
            if ($rec == false) {
                drupal_set_message("Module not found, please contact system administrator");
                return;
            }
        } else {
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
            '#value' => $school_nid,
        );

        if ($tab == 'permission') {
            $heading = "School Resource Library: Permissions";
        } else if ($tab == 'setup') {
            $heading = "School Resource Library: Setup";
        }

        if ($tab == 'permission') {
            $form['main']['save'] = array(
                '#type' => 'submit',
                '#value' => 'Save',
                '#prefix' => '<td>&nbsp;</td><td><table cellpadding="0" cellspacing="0" width="100%"><tr><td><div><a href="' . url('ntlp/document/resource/library') . '" class="greenLink">« Back to School Resource Library</a></div><div><h2 style="padding:0px;margin:0px;color:#3570aa;font-size:18px;">' . $heading . '</h2></div></td><td align="right">',
                '#suffix' => '</td></table></td></tr><tr>',
                '#submit' => array('nltp_lib_doc_submit_' . $tab),
                '#attributes' => array('class' => 'SaveBtn'),
            );
        } else {
            $form['main']['save'] = array(
                '#type' => 'item',
                '#value' => '',
                '#prefix' => '<td>&nbsp;</td><td><table cellpadding="0" cellspacing="0" width="100%"><tr><td><div><a href="' . url('ntlp/document/resource/library') . '" class="greenLink">« Back to School Resource Library</a></div><div><h2 style="padding:0px;margin:0px;color:#3570aa;font-size:18px;">' . $heading . '</h2></div></td><td align="right">',
                '#suffix' => '</td></table></td></tr><tr>',
            );
        }
        ntlp_library_doc_add_tabs($school_nid, $form['main'], $tab);

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

        if ($tab == 'permission') {
            ntlp_library_doc_tab1_permission($form_state, $form['main']['form'], $school_nid, $rec);
        } elseif ($tab == 'setup') {
            ntlp_library_doc_tab2_setup($form_state, $form['main']['form'], $school_nid, $rec);
        }

        if ($tab != 'review_needed')
            $form['main']['form_end0'] = array(
                '#type' => 'markup',
                '#value' => $HTMLform_course_setting_bluebox_end);
        $form['main']['form_end'] = array(
            '#type' => 'markup',
            '#value' => $HTMLform_course_setting_form_end
        );
    }else {
        drupal_set_message('No School Selected for this user', 'error');
    }
    return $form;
}

/**
 * Form definition
 */
function ntlp_library_doc_tab1_permission($form_state, &$form, $module_nid, $rec) {

    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    modalframe_parent_js();

    drupal_add_js(drupal_get_path('module', 'ntlp_document_library') . '/ntlp_document_library.js');
    $form['library_document_info'] = array(
        '#type' => 'item',
        '#value' => t('School Resource Library: Permissions '),
        '#prefix' => '<tr><th style="text-align:left;" colspan="3">',
        '#suffix' => '</th></tr>',
    );



    $data = _get_permission_list($rec->nid, $form);

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

    $form['permissions'] = array(
        '#type' => 'hidden',
        '#id' => 'categories_perm',
    );

    $form['library_document_new_category'] = array(
        '#type' => 'item',
        '#value' => dlg('Add New Category', 'ntlp/library/add/category/' . $rec->nid . '/0', 450, 600, 'whitename'),
        '#prefix' => '<tr><td class="Activities_btn" colspan="3">',
        '#suffix' => '</td></tr>',
    );

    $form['confirm_delete'] = array(
            '#type' => 'item',
            '#value' => '<div id="dialog-confirm-cat" title="Delete Category" style="display:none"><span class="msg"></span></div>',
    );
}

function ntlp_library_doc_tab2_setup($form_state, &$form, $module_nid, $rec) {
    global $base_path;

    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'image_uploader') . '/image_uploader.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_library') . '/ntlp_library.js');

    include('ntlp_module.template.inc');


    $form['save_btn'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#submit' => array('nltp_lib_doc_submit_setup'),
        '#prefix' => '<tr><td colspan="2">',
        '#suffix' => '</td></tr>',
        '#attributes' => array('class' => 'SaveBtn'),
    );

    $form['module_id'] = array(
        '#type' => 'hidden',
        '#value' => $rec->nid,
        '#id' => 'module_id',
    );

    $form['title'] = array(
        '#type' => 'textfield',
        '#value' => $rec->title,
        '#prefix' => '<tr><th>' . t('Title') . '</th><td >',
        '#suffix' => '</td></tr>',
    );

    $form['description'] = array(
        '#type' => 'textfield',
        '#value' => $rec->body,
        '#prefix' => '<tr><th>' . t('Description') . '</th><td >',
        '#suffix' => '</td></tr>',
    );

    if ($file_obj = _dpl_files_obj($rec->icon_fid)) {
        $picture = $base_path . $file_obj->filepath;
    }

    $form['picture_id'] = array(
        '#type' => 'hidden',
        '#value' => $rec->icon_fid,
        '#id' => 'picture_fid',
    );

    $form['archive_projects_view'] = array(
        '#type' => 'item',
        //'#value' => t('<--- Change Icon'),
        '#prefix' => '<tr><th>' . t('Icon') . '</th><td> <div id="module_image" align="left">' . drupal_get_form('ntlp_activeimage', $picture, 0, 150) . '</div> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;',
        '#suffix' => '</td></tr>',
    );
}

function ntlp_library_doc_add_tabs($module_nid, &$form, $tab = '') {
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
    $roption = str_replace("@LINK", url("ntlp/document/library/manage/$module_nid"), $roption);
    $roption = str_replace("@PROMPT", "Permission", $roption);
    $roption = str_replace("@CLASS", ($tab != 'setup') ? "class='active'" : "", $roption);
    $form['tabs']['basic'] = array(
        '#type' => 'markup',
        '#value' => $roption,
    );

    $roption = $HTMLform_course_settings_tabs_option;
    $roption = str_replace("@LINK", url("ntlp/document/library/manage$url/setup"), $roption);
    $roption = str_replace("@PROMPT", "Setup", $roption);
    $roption = str_replace("@CLASS", ($tab == 'setup') ? "class='active'" : "", $roption);
    $form['tabs']['data_export'] = array(
        '#type' => 'markup',
        '#value' => $roption,
    );
}

function _get_permission_list($module_nid, $form) {
    global $user, $base_path;
    $dateformat = "m/d/Y";
    $head = array(
        array('data' => t('Categories'), 'field' => 'category_tid', 'sort' => 'asc', 'width' => '20%', 'style' => 'text-align:left;'),
        array('data' => t('All'), 'width' => '5%', 'style' => 'text-align:center;'),
        array('data' => t('Teacher'), 'width' => '5%', 'style' => 'text-align:center;'),
        array('data' => t('Student'), 'width' => '5%', 'style' => 'text-align:center;'),
        array('data' => t('Parent'), 'width' => '5%', 'style' => 'text-align:center;'),
    );

    $sql = "SELECT lc.*, nm.* FROM {ntlp_library_category} lc
        INNER JOIN {ntlp_module} nm ON nm.nid = lc.module_nid
        WHERE lc.module_nid =" . $module_nid . tablesort_sql($head);

//    $result = db_query($sql);
    $limit = 10;
    $result = pager_query($sql, $limit);
    while ($archive_req = db_fetch_object($result)) {
        $taxObj = taxonomy_get_term($archive_req->category_tid);

        $perm = ' ' . $archive_req->school_nid . ' ' . strtolower($taxObj->name);
//        watchdog('gory_queyr', "SELECT * FROM {permission} WHERE FIND_IN_SET('" . $perm . "', perm) AND rid IN(" . NTLP_ROLEID_TEACHER . "," . NTLP_ROLEID_STUDENT . "," . NTLP_ROLEID_PARENT . ") ");
        $resultPerm = db_query("SELECT * FROM {permission} WHERE FIND_IN_SET('%s', perm) AND rid IN(%d, %d, %d)", $perm, NTLP_ROLEID_TEACHER, NTLP_ROLEID_STUDENT, NTLP_ROLEID_PARENT);
        $permsDefined = array();
        while ($permDefined = db_fetch_object($resultPerm)) {
            $permsDefined[$permDefined->rid] = $permDefined->pid;
        }
        $category_name = str_replace('%2C', ",", $taxObj->name);
        $rows[] = array(
            array('data' => dlg($category_name, 'ntlp/library/add/category/' . $module_nid . '/' . $archive_req->category_tid, 450, 600) . ' ' . '<span style="float:right;"><a onclick="remove_library_category('.$archive_req->category_tid.')"><img alt="DeleteIcon" src="'.$base_path.'themes/Boldr/Images/DeleteBtn.png"></a></span>'),
            array('data' => '<input ' . (isset($permsDefined[NTLP_ROLEID_TEACHER], $permsDefined[NTLP_ROLEID_STUDENT], $permsDefined[NTLP_ROLEID_PARENT]) ? 'checked' : '') . ' type="checkbox" onclick="checkAll(' . $archive_req->category_tid . ')" id="all_' . $archive_req->category_tid . '"  />', 'style' => 'text-align:center;'),
            array('data' => '<input ' . (isset($permsDefined[NTLP_ROLEID_TEACHER]) ? 'checked' : '') . ' type="checkbox" onclick="set_category_permission(' . $archive_req->category_tid . ')" id="teacher_' . $archive_req->category_tid . '" name="perm[' . $archive_req->category_tid . '][' . NTLP_ROLEID_TEACHER . ']" value="' . $perm . '" /> <input type="hidden" name="perms[' . $archive_req->category_tid . '][' . NTLP_ROLEID_TEACHER . ']" value="' . $perm . '" />', 'style' => 'text-align:center;'),
            array('data' => '<input ' . (isset($permsDefined[NTLP_ROLEID_STUDENT]) ? 'checked' : '') . ' type="checkbox" onclick="set_category_permission(' . $archive_req->category_tid . ')" id="student_' . $archive_req->category_tid . '" name="perm[' . $archive_req->category_tid . '][' . NTLP_ROLEID_STUDENT . ']" value="' . $perm . '" /> <input type="hidden" name="perms[' . $archive_req->category_tid . '][' . NTLP_ROLEID_STUDENT . ']" value="' . $perm . '" />', 'style' => 'text-align:center;'),
            array('data' => '<input ' . (isset($permsDefined[NTLP_ROLEID_PARENT]) ? 'checked' : '') . ' type="checkbox" onclick="set_category_permission(' . $archive_req->category_tid . ')" id="parent_' . $archive_req->category_tid . '"  name="perm[' . $archive_req->category_tid . '][' . NTLP_ROLEID_PARENT . ']" value="' . $perm . '" /> <input type="hidden" name="perms[' . $archive_req->category_tid . '][' . NTLP_ROLEID_PARENT . ']" value="' . $perm . '" />', 'style' => 'text-align:center;'),
        );
    }

    $output = theme('table', $head, $rows, $attributes = array('class' => 'ntlp_table', 'width' => '100%', 'cellpadding' => '5'));
    $output .= theme('pager', NULL, $limit, 0);

    return $output;
}

function nltp_lib_doc_submit_setup($form, $form_state) {

//    print_r($form_state);
    $module_id = $form_state['clicked_button']['#post']['module_id'];
    $title = $form_state['clicked_button']['#post']['title'];
    $description = $form_state['clicked_button']['#post']['description'];

    $module_node = node_load($module_id);

    $module_node->title = $title;
    $module_node->body = $description;

    node_save($module_node);
}

function nltp_lib_doc_submit_permission($form, $form_state) {

    $selected_perms = $form_state['clicked_button']['#post']['perm'];
    $all_perms = $form_state['clicked_button']['#post']['perms'];

    watchdog('selected_perms ', print_r($selected_perms, true));
    watchdog('all_perms ', print_r($all_perms, true));
    // Add checked permissions.
    if (is_array($selected_perms) and count($selected_perms)) {
        foreach ($selected_perms as $cat_id => $cat_perms) {
            foreach ($cat_perms as $rid => $perm) {
                $perm = strtolower($perm);
                db_query("UPDATE {permission} SET perm = TRIM(BOTH ',' FROM REPLACE(CONCAT(',' , perm, ','), CONCAT(',', '%s', ',') , ',')) WHERE rid = %d", $perm, $rid);
                watchdog('delete_first', "UPDATE {permission} SET perm = TRIM(BOTH ',' FROM REPLACE(CONCAT(',' , perm, ','), CONCAT(',', '" . $perm . "', ',') , ',')) WHERE rid = " . $rid);
                $result = db_query("SELECT * FROM {permission} WHERE rid = %d AND FIND_IN_SET('%s', perm) ", $rid, $perm);

                watchdog('select_qye', "SELECT * FROM dpl_permission WHERE rid =" . $rid . " AND FIND_IN_SET('" . $perm . "', perm) ");
                if (!$result->num_rows) {
                    unset($all_perms[$cat_id][$rid]);
                    db_query("UPDATE {permission} SET perm = CONCAT(perm,',','%s') WHERE rid = %d", $perm, $rid);
                    watchdog('update_qye', "UPDATE dpl_permission SET perm = CONCAT(perm,',','" . $perm . "') WHERE rid = " . $rid);
                }
            }
        }
    }

    // Removed un-checked permissions.
    if (is_array($all_perms) and count($all_perms)) {
        foreach ($all_perms as $cat_id => $cats_perms) {
            foreach ($cats_perms as $rid => $perm) {
                $perm = strtolower($perm);
                db_query("UPDATE {permission} SET perm = TRIM(BOTH ',' FROM REPLACE(CONCAT(',' , perm, ','), CONCAT(',', '%s', ',') , ',')) WHERE rid = %d", $perm, $rid);
                watchdog('finally', "UPDATE {permission} SET perm = TRIM(BOTH ',' FROM REPLACE(CONCAT(',' , perm, ','), CONCAT(',', '" . $perm . "', ',') , ',')) WHERE rid = " . $rid);
            }
        }
    }
}

/**
 * _add_permissions() is a helper function to add permissions by role to the db
 */
function _add_permissions($rid, $permissions) {
    if (!is_array($permissions)) {
        $permissions = explode(', ', $permissions);
    }
    $current_perms = explode(', ', db_result(db_query("SELECT perm FROM {permission} WHERE rid=%d", $rid)));

    foreach ($permissions as $permission) {
        if (!in_array($permission, $current_perms)) {
            echo $current_perms[] = $permission;
        }
    }
    $current_perms = implode(', ', $current_perms);
    $return = db_query("UPDATE {permission} SET perm= '%s' WHERE rid=%d", $current_perms, $rid);
    return $return;
}
