<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_courses_activity', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_school', 'data_access');

require_once 'data_access.inc';

function ntlp_resources_new_form($form_state) {
    include("resources.template.inc");

    global $base_path, $user;
    $path = $base_path;

    $url = $_GET['q'];
    $args = explode('/', $url);


    for ($i = 0; $i < count($args); $i++) {
        if (is_numeric($args[$i]) && empty($course_id))
            $course_id = $args[$i];
        elseif (is_numeric($args[$i]) && empty($project_id))
            $project_id = $args[$i];
    }

    if ($args[3] != 'edit') {
        if (check_plain(arg(1)) == "courses") {
            $associated_project = check_plain(arg(5));
            $associated_project_section = check_plain(arg(6));
        } else {
            $course_id = check_plain(arg(4));
            $is_project = 0;
        }
    } else {

        $resource_nid = check_plain($args[5]); // edit mode
        $resource_Obj = get_resource_detail($resource_nid);


        $associated_project = $resource_Obj->project_nid;
        $associated_project_section = $resource_Obj->section_tid;
    }


    if (!isset($form_state['values']['add_resources'])) {
        set_item_url_handler('Resource');
    }

    $resource_back_link = get_back_url();

    drupal_add_css(drupal_get_path('theme', 'Boldr') . '/StylishSelect/stylish-select.css', 'module', 'all', FALSE);
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/externalFile.js');
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/jquery.stylish-select.js');

    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_resources') . '/ntlp_resources.js');

    drupal_add_js('
    function clearText(field){
    
    if (field.defaultValue == field.value){
        field.value = "";
        $("#"+field.id).css("color","black");
    } else if (field.value == "") {
        field.value = field.defaultValue;
         $("#"+field.id).css("color","#999999");
        }
}', 'inline');

    if (isset($form_state['values'])) {
        $view_state = $form_state['values']['op'];
    }
    if (isset($form_state['values']['add_resources']['course_id'])) {
        $course_id = $form_state['values']['add_resources']['course_id'];
    }

    if (isset($form_state['values']['add_resources']['project_nid'])) {
        $associated_project = $form_state['values']['add_resources']['project_nid'];
    }
    if (!isset($associated_project)) {
        drupal_set_message('You are not allowed to add resources directly in a course. Please add resources in a project.');
        drupal_goto("ntlp/goback");
        return;
    }

    if ($args[3] != 'edit') {
        $resource_title = 'New Project Resource';
    } else {
        $resource_title = 'Edit Project Resource';
    }

    if (isset($resource_Obj))
        $last_saved = ' <span class="published_grade_box" style="padding: 4px !important;float:right !important;"> Last Saved ' . date('m/d/Y @ g:i a', $resource_Obj->changed) . '&nbsp;&nbsp; </span>';
    else
        $last_saved = '';

    $form['add_resources'] = array(
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => str_replace('@RESOURCE_TITLE', $resource_title, $HTMLform_resource_hdr),
        '#suffix' => $HTMLform_resource_ftr,
    );

    $form['add_resources']['edit_resource'] = array(
        '#type' => 'hidden',
        '#value' => $resource_nid,
    );

    $form['add_resources']['cancel_button'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#prefix' => '<div style="position:absolute;top:0px;right:0px;width:390px;"><div style="float:right;width:80px;" >',
        '#suffix' => '</div>',
        '#attributes' => array('class' => 'CancelBtn'),
    );

    $form['add_resources']['save_button'] = array(
        '#type' => 'submit',
//        '#submit' => array('ntlp_resources_new_form_submit'),
        '#value' => t('Save'),
//        '#validate' => array('ntlp_resources_new_form_validate'),
        '#prefix' => '<div style="float:right;width:80px;" >',
        '#suffix' => '</div>' . $last_saved . '</div>',
        '#attributes' => array('class' => 'PostBtn'),
    );

    $form['add_resources']['resource_name'] = array(
        '#type' => 'textfield',
        '#size' => '50',
        '#maxlength' => '50',
        '#value' => (isset($resource_Obj) ? $resource_Obj->title : ''),
        '#prefix' => '<tr id="resourcesname"><th>' . t('Title*') . '</th><td><div style="width:338px; float:left;">',
        '#suffix' => '</div><div style="margin-top: 7px;">&nbsp;Max characters = 50</div></td></tr>',
    );

    $form['add_resources']['project_nid'] = array(
        '#type' => 'hidden',
        '#value' => $associated_project,
        '#id' => 'project_nid',
    );

    $projectsdata = _get_selected_projects($associated_project);
    $form['add_resources']['project'] = array(
        '#type' => 'item',
        '#value' => $projectsdata->title,
        '#prefix' => '<tr><th class="associated_project">' . t('Project') . '</th><td>',
        '#suffix' => '</td></tr>',
    );

    if (!$user->roles[NTLP_ROLEID_STUDENT]) {

        $project_section = _get_course_section($associated_project);
        $notavailable = array("Not Available" => "Not Available");

        $form['add_resources']['project_row'] = array(
            '#type' => 'select',
            '#options' => ($project_section != null) ? $project_section : $notavailable,
            '#default_value' => $associated_project_section,
            '#prefix' => '<tr><th style="width:160px">' . t('Project Category') . '</th><td><span class="comboboxBgLeft" style="width:340px;">',
            '#suffix' => '</span></td></tr>',
            '#attributes' => array('class' => 'my-dropdown'),
        );
    }

//    watchdog('resource', 'Default resource type for nid:'.$resource_nid.' is '.(isset($resource_Obj) ? $resource_Obj->type : 'lo_link'));
    $form['add_resources']['selected_resource'] = array(
        '#type' => 'radios',
        '#options' => array(
            'lo_page' => 'Page',
            'lo_document' => 'Document',
            'lo_video' => 'Media',
            'lo_image' => 'Image',
            'lo_link' => 'Link',
        ),
        '#prefix' => '<tr><th>' . t('Type*') . '</th><td style="solid #9fcce9;padding-left:6px;" colspan="3"><div id="resourceRadioBtn">',
        '#suffix' => '</div></td></tr>',
        '#attributes' => array('onclick' => 'load_attributes(this);'),
        '#default_value' => (isset($resource_Obj) ? $resource_Obj->type : 'lo_page'),
    );

    $form['add_resources']['is_visible'] = array(
        '#type' => 'checkbox',
        '#title' => 'Show',
        '#default_value' => ($resource_Obj->availability == 'S'),
        '#prefix' => '<tr id="linkurl"><th style="width:160px;">' . t('Visibility') . '</th><td><div style="width: 140px; float: left;">',
        '#suffix' => '</div><span class="pro_box">A resource must be set to “show” for it to be seen by students.</span></td></tr>',
    );

    $form['add_resources']['attached_files'] = array(
        '#type' => 'item',
        '#prefix' => '<tr style="display:none" id="resources-wrapper"><td>',
        '#suffix' => '</td></tr>',
    );

//    watchdog("resource_load", "Resource Obj:".print_r($resource_Obj, true));

    $form['add_resources']['resource_type'] = array(
        '#type' => 'item',
        '#prefix' => '<tr id="attach_resource_option" ><th><span id="resource_heading"></span></th><td>',
        '#suffix' => '',
    );

    $form['add_resources']['resource_radio'] = array(
        '#type' => 'radios',
        '#options' => array('E' => 'Embed', 'U' => 'Upload'),
        '#prefix' => '<div id="radio_attach_options" style="display:none"><div class="clearfix">',
        '#suffix' => '</div></div>',
        '#id' => 'resource_radio2',
        '#attributes' => array('onclick' => 'show_attachement_options(this);'),
        '#default_value' => (!empty($resource_Obj->fid)) ? 'U' : 'E',
    );

    $form['add_resources']['video_embedded_code'] = array(
        '#default_value' => ($resource_Obj->path == '' && $resource_Obj->type == 'lo_video') ? 'Copy and paste <embed> or <iframe> code from other websites' : $resource_Obj->path,
        '#type' => 'textarea',
        '#wysiwyg' => false,
        '#rows' => '2',
        '#resizable' => false,
        '#prefix' => '<div id="video_embd" style="display:none"><div style="padding: 0px 5px 5px;">',
        '#suffix' => '</div></div>',
        '#attributes' => array('style' => 'color: #999999;width:575px; ', 'class' => 'shareIdea onClickTxtClr', 'title' => 'Copy and paste video <embed> code from other websites',
            'onblur' => "clearText(this)", 'onfocus' => "clearText(this)"),
    );

    $form['add_resources']['google_radio'] = array(
        '#type' => 'radios',
        '#options' => array('upload' => 'Upload', 'gdocs' => 'Google Doc'),
        '#prefix' => '<div class="clearfix" id="radio_doc_options" style="display:none; margin-bottom:10px;" >',
        '#suffix' => '</div>',
        '#attributes' => array('onclick' => 'show_google_option(this)'),
        '#default_value' => ($resource_Obj->resource_type != 'G') ? 'upload' : 'gdocs',
    );

    $form['add_resources']['google_attached'] = array(
        '#type' => 'item',
        '#prefix' => '<div><div id="google_doc_attached" style="padding: 0px 5px 5px 5px; display:none;">
                        <span>+' . dlg(" Link to a Google Doc", 'ntlp/course/resources/attachment/google', 300, 600, 'project_resource_Green_text') . '</span>
                            <span id="attach_link_tip" class="pro_box">Accepted file formats: JPG, PNG, GIF</span>
                      </div></div>',
    );

    $hiddendata = '';
    $attached_files_data = '';
    $display_link = '';
    if ($resource_Obj->type == 'lo_document' && $resource_Obj->resource_type == 'G') {
        $filename = empty($resource_Obj->attachment_name) ? $resource_Obj->title : $resource_Obj->attachment_name;
        $display = '';
        $hiddendata .= 'G,' . $resource_Obj->nid . ',' . $filename . ',' . $resource_Obj->path . ';';

//        $divdata .= '<div id="G_' . $resource_Obj->nid . '"> <a onclick="doc_delete_attachment(\'G_' . $resource_Obj->nid . '\');" ><img src="' . $base_path . 'sites/all/modules/ntlp_courses_activity/theme/delete.PNG" border="0" /></a><a href=' . $resource_Obj->path . '>' . $resource_Obj->path . '</a></div>';
        $divdata = '<div id="G_' . $resource_Obj->nid . '">' .
                '<img style="position:relative;top:2px;margin-right:4px;" onclick="doc_delete_attachment(\'G_' . $resource_Obj->nid . '\');" width="14" height="14" src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png" border="0" />' .
                '<a href="' . $resource_Obj->path . '" target="_blank">' . $filename . '</a></div>';
    } else {
        $hiddendata = '';
        $display = 'style="display:none"';
    }
    if (isset($resource_Obj) && $resource_Obj->fid != 0) {
        $filename = empty($resource_Obj->attachment_name) ? $resource_Obj->filename : $resource_Obj->attachment_name;
        $hiddendata .= 'F,' . $resource_Obj->fid . ',' . $filename . ',' . $resource_Obj->filepath . ';';
//        $attached_files_data .= '<div id="F_' . $resource_Obj->fid . '"><a onclick="other_delete_attachment(\'F_' . $resource_Obj->fid . '\');" ><img src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png" border="0" /></a>&nbsp;&nbsp;' . $resource_Obj->filename . '</div>';
        $attached_files_data = '<div id="F_' . $resource_Obj->fid . '">' .
                '<img style="position:relative;top:2px;margin-right:4px;" onclick="other_delete_attachment(\'F_' . $resource_Obj->fid . '\');" width="14" height="14" src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png" border="0" />' .
                '<a href="' . $resource_Obj->filepath . '" target="_blank">' . $filename . '</a></div>';

        $display_link = 'display:none';
    }

    $form['#fileupload_embed'] = true;
    $form['#fileupload_auto_upload'] = true;

    $form['add_resources']['google_urls'] = array(
        '#type' => 'item',
        '#prefix' => '<div id="google_docs_row" ' . $display . '>
                        <div id="google_urllist" style="height:20px;">' . $divdata . '</div>
                      </div>',
    );

    $attach_fileupload_link = '
     <div id="attach_links_panel" style="cursor: pointer !important; clear:both;">
     <div onmouseover="place_fileupload_control(\'edit-add-resources-upload-control\', this);" onmouseout="place_fileupload_control_mouseout(this);" >
       <div class="clearfix">
          <span id="attach_link_prompt"><span style="color:#000">+&nbsp;</span>
          <a id="attach_link" class="project_resource_Green_text" style="' . $display_link . '">Attach file</a>
          </span>
          <div id="attach_links" style="width:auto;">' . $attached_files_data . '</div>
          <div id="attach_link_suffix" style="padding-top:10px;' . $display_link . '">Max file size = 5 MB</div>
       </div>
       <span id="attach_link_tip" class="pro_box">Accepted file formats: JPG, PNG, GIF</span>
     </div>
     </div></td></tr>';

    $form['add_resources']['upload_control'] = array(
        '#prefix' => $attach_fileupload_link,
    );

    fileupload_get_control($form, $form_state, array('add_resources', 'upload_control'));

    $form['add_resources']['attached_files_hidden'] = array(
        '#type' => 'hidden',
        '#value' => $hiddendata,
        '#id' => 'attached_files_hidden',
    );

    $form['add_resources']['google_hAttachedFiles'] = array(
        '#type' => 'hidden',
        '#value' => $hiddendata,
        '#id' => 'google_hAttachedFiles',
    );

    $form['add_resources']['other_resource_link'] = array(
        '#type' => 'textfield',
        '#size' => '90',
        '#default_value' => (isset($resource_Obj->type) && $resource_Obj->type == 'lo_image' || $resource_Obj->type == 'lo_video' ? $resource_Obj->path : ''),
        '#prefix' => '<tr  id="other_resource_link" style="display:none"><th style="width:113px;">' . t('URL') . '</th><td>',
        '#suffix' => '</td></tr>',
    );

    $form['add_resources']['attach_link_for_link'] = array(
        '#type' => 'textfield',
        '#size' => '50',
        '#default_value' => (isset($resource_Obj->type) && $resource_Obj->type == 'lo_link' ? $resource_Obj->path : 'http://'),
        '#prefix' => '<tr id="attach_link_for_link"  ><th style="width:113px">' . t('Full URL*') . '</th><td><div style="width: 340px; float: left;">',
        '#suffix' => '</div><span class="pro_box">Ex: http://www.google.com/news</span></td></tr>',
        '#attributes' => array('style' => 'color: #999999; margin-top:5px;', 'class' => 'onClickTxtClr', 'title' => 'http://',
            'onblur' => "clearText(this)", 'onfocus' => "clearText(this)"),
    );

    $form['add_resources']['page_data'] = array(
        '#type' => 'textarea',
        '#rows' => '5',
        '#cols' => '70',
        '#default_value' => ((isset($resource_Obj->path) && $resource_Obj->type == 'lo_page') ? $resource_Obj->path : ''),
        '#resizable' => false,
        '#prefix' => '<tr id="page_data" style="display:none"><th >' . t('Page Content*') . '</th><td ><div style="float: left; width: 667px;">',
        '#suffix' => '</div></td></tr>',
    );

    //Multiple file attachment control for Page resource
    $hiddendata = '';
    $attached_files_data = '';
    if ($resource_Obj->type == 'lo_page') {
        $rs_attachment = get_resource_attachment($resource_nid);
//        echo 'Attachments: '.$rs_attachment->num_rows.', '. $resource_nid .'<br>';
        if ($rs_attachment->num_rows > 0) {
            while ($rec = db_fetch_object($rs_attachment)) {
                if ($rec->type == 'F') {
                    $filename = empty($rec->name) ? $rec->filename : $rec->name;
                    $filepath = $base_path . $rec->filepath;
                } else {
                    $filename = $rec->name;
                    $filepath = $rec->path;
                }
                $hiddendata .= $rec->id . ',' . $rec->type . ',' . $rec->fid . ',' . $filename . ',' . $filepath . ';';
                $attached_files_data .= '<div id="' . $rec->type . '_' . $rec->id . '">' .
                        '<img style="position:relative;top:2px;margin-right:4px;" onclick="page_delete_attachment(\'' . $rec->type . '_' . $rec->id . '\');" width="14" height="14" src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png" border="0" />' .
                        '<a href="' . $filepath . '" target="_blank">' . $filename . '</a></div>';
            }
        }
    }
    $form['#fileupload_embed'] = true;
    $form['#fileupload_auto_upload'] = true;

    $attach_fileupload_link = '<span style="float:left;text-decoration:none;color: #3399CC;" id="attach_link_prompt"><span style="color:#000">+&nbsp;</span><a class="project_resource_Green_text" id="attach_link">Attach file</a></span>';
    $google_attach_link = '<div style="z-index:100;position:relative;width: 200px;"><span style="color:#000">+&nbsp;</span>' . dlg("Link Google Doc", 'ntlp/course/resources/attachment/pagegoogle', 300, 600, 'project_resource_Green_text') . '</div>';

    $form['add_resources']['pageattachments_upload_control'] = array(
        '#prefix' => '<tr id="page_attachments" style="display:none"><th >' . t('Attachments') . '</th><td>
            <table style="border-bottom:none;" width="100%" border="0" cellspacing="0" cellpadding="0"><tr id="page_attachments_tr">
            <td style="border-bottom:none;" onmouseover="place_fileupload_control(\'edit-add-resources-pageattachments-upload-control\', this);" onmouseout="place_fileupload_control_mouseout(this);" >' . $attach_fileupload_link,
        '#suffix' => '</td><td style="border-bottom:none;" ><div style"width:25px"></div>' . $google_attach_link . '</td>
            <td style="border-bottom:none;" ><div class="pro_box">&nbsp; Multiple attachments allowed.</div></td></tr>
            <tr><td style="border-bottom:none;" colspan="3">
            <div id="page_attach_links" style="float:left;width:auto;" class="attach_link_forum" >' . $attached_files_data . '</div>
            </td></tr></table>
            </td></tr>',
    );

    fileupload_get_control($form, $form_state, array('add_resources', 'pageattachments_upload_control'), '', true);

    $form['add_resources']['page_attached_files_hidden'] = array(
        '#type' => 'hidden',
        '#value' => $hiddendata,
        '#id' => 'page_attached_files_hidden',
    );

    $form['add_resources']['description'] = array(
        '#type' => 'textarea',
        '#wysiwyg' => FALSE,
        '#rows' => '3',
        '#cols' => '80',
        '#default_value' => (isset($resource_Obj->body) ? $resource_Obj->body : ''),
        '#resizable' => false,
        '#prefix' => '<tr id="description" ><th>' . t('Description') . '
                    <div style="display:none;font-style:italic; font-size:10px;">(optional)</div></th><td>',
        '#suffix' => '</td></tr>',
    );

    $form['add_resources']['resoruce_uploadFiles'] = array(
        '#type' => 'hidden',
        '#value' => '',
        '#prefix' => '<tr style="display:none" ><td>',
        '#suffix' => '</td></tr>',
    );

    $form['add_resources']['course_id'] = array(
        '#type' => 'hidden',
        '#value' => $course_id,
        '#prefix' => '<tr style="display:none" ><td>',
        '#suffix' => '</td></tr>',
    );

    $form['add_resources']['selected_type'] = array(
        '#type' => 'hidden',
        '#value' => '',
        '#id' => 'type',
        '#prefix' => '<tr style="display:none" ><td>',
        '#suffix' => '</td></tr>',
    );

//        }
//    }
    cache_set("ntlp_resource:" . session_id(), null, 'cache', CACHE_TEMPORARY);
    return $form;
}

function ntlp_resources_new_form_validate($form, $form_state) {

    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/fileupload_ahah_helper.js', 'footer');
    drupal_add_js(drupal_get_path('module', 'ntlp_resources') . '/ntlp_resources.js');
//    watchdog('ntlp_res_val', print_r($form_state, true));

    $resource_nid = $form_state['clicked_button']['#post']['add_resources']['edit_resource'];
    $resource_type = $form_state['clicked_button']['#post']['add_resources']['selected_resource'];
    $resource_name = $form_state['clicked_button']['#post']['add_resources']['resource_name'];
    $resource_radio = $form_state['clicked_button']['#post']['add_resources']['resource_radio'];
    $description = $form_state['clicked_button']['#post']['add_resources']['description'];
    $google_radio = $form_state['clicked_button']['#post']['add_resources']['google_radio'];

    $other_resource_link = $form_state['clicked_button']['#post']['add_resources']['other_resource_link'];
    $attach_link_for_link = $form_state['clicked_button']['#post']['add_resources']['attach_link_for_link'];
    $page_data = $form_state['clicked_button']['#post']['add_resources']['page_data'];
    $page_attached_files = $_POST['add_resources']['page_attached_files_hidden'];
    $project = $form_state['clicked_button']['#post']['add_resources']['project_nid'];
    $project_section = $form_state['clicked_button']['#post']['add_resources']['project_row'];
    $course_nid = $form_state['clicked_button']['#post']['add_resources']['course_id'];
//    $attached_files_hidden = $form_state['clicked_button']['#post']['add_resources']['attached_files_hidden'];
    $attached_files_hidden = $_POST['add_resources']['attached_files_hidden'];
    $google_hAttachedFiles = $_POST['add_resources']['google_hAttachedFiles'];

    if ($form_state['clicked_button']['#post']['op'] == 'Cancel') {
        cache_set("ntlp_resource:" . session_id(), null, 'cache', CACHE_TEMPORARY);
        drupal_goto('ntlp/goback');
    }

    $project = load_project_record($project)->title;
//    variable_del('ntlp_resource:' . session_id());

    $store_into_temp = new stdClass;
    $store_into_temp->resource_nid = $resource_nid;
    $store_into_temp->selected_resource = $resource_type;
    $store_into_temp->resource_name = $resource_name;
    $store_into_temp->description = $description;
    $store_into_temp->is_shared = 0;
    $store_into_temp->other_resource_link = $other_resource_link;
    $store_into_temp->attach_link_for_link = $attach_link_for_link;
    $store_into_temp->last_state = $last_state_url;
    $store_into_temp->course_nid = $course_nid;
    $store_into_temp->page_attached_files = $page_attached_files;
    $store_into_temp->attached_files_hidden = $attached_files_hidden;
    $store_into_temp->google_hAttachedFiles = $google_hAttachedFiles;

    if ($resource_type == 'lo_document') {
        $store_into_temp->google_radio = $google_radio;
    } else if ($resource_type == 'lo_image' || $resource_type == 'lo_video') {
        $store_into_temp->resource_radio = $resource_radio;
    } else if ($resource_type == 'lo_page') {
        $store_into_temp->page_data = $page_data;
    }

    $store_into_temp->project = $project;
    $store_into_temp->project_section = $project_section;

//    watchdog('val_func1', 'restore:'.print_r($store_into_temp, true));
    cache_set("ntlp_resource:" . session_id(), $store_into_temp, 'cache', CACHE_TEMPORARY);

    if ($resource_type == 'lo_video') {
        $resoruce_type_name = "Video";
    } else if ($resource_type == 'lo_image') {
        $resoruce_type_name = "Image";
    } else if ($resource_type == 'lo_document') {
        $resoruce_type_name = "Document";
    }


    watchdog('vresource', 'other_resource_link ' . $form_state['clicked_button']['#post']['add_resources']['attached_files_hidden']);
    $uploaded = false;
    if (isset($attached_files_hidden) && $attached_files_hidden != null) {
        $linksarray = get_google_links($attached_files_hidden);

        foreach ($linksarray as $l) {
            list($vtype, $vupload_fileid, $vupload_filename, $vupload_filepath) = $l;
            if (!empty($vtype) && $vtype != 'X') {
                $uploaded = true;
            }
        }
    }

    if (empty($form_state['clicked_button']['#post']['add_resources']['resource_name'])) {
        form_set_error('add_resources][resource_name', 'Invalid resource name.');
    }

    if ($resource_type == 'lo_image') {
        if (!$uploaded) {
            form_set_error('', t('You have not included an image. Please add your resource before saving.'));
        }
    } elseif ($resource_type == 'lo_video') {
        if (!$uploaded && empty($form_state['clicked_button']['#post']['add_resources']['video_embedded_code'])) {
            form_set_error('', t('You have not included a ' . $resoruce_type_name . '. Please add your resource before saving.'));
        }
       
    } elseif ($resource_type == 'lo_link') {
        if ($form_state['clicked_button']['#post']['add_resources']['attach_link_for_link'] == '') {
            form_set_error('', t('You have not included a Link. Please define your link before saving.'));
        } else {
            if (!isValidURL($form_state['clicked_button']['#post']['add_resources']['attach_link_for_link'])) {
                form_set_error('', t('Please enter valid URL including http:// or https://.'));
            }
        }
    } elseif ($resource_type == 'lo_document') {
        if ($google_radio == 'gdocs') {
            $uploaded = false;
            if (isset($google_hAttachedFiles) && $google_hAttachedFiles != null) {
                $linksarray = get_google_links($google_hAttachedFiles);

                foreach ($linksarray as $l) {
                    list($vtype, $vupload_fileid, $vupload_filename, $vupload_filepath) = $l;
                    if (!empty($vtype) && $vtype != 'X') {
                        $uploaded = true;
                    }
                }
            }
        }
        if (!$uploaded) {
            form_set_error('', t('You have not included a Document. Please add a document before saving.'));
        }
//            watchdog('google_docs','google_hAttachedFiles '.$form_state['clicked_button']['#post']['add_resources']['google_hAttachedFiles']);
    } elseif ($resource_type == 'lo_page') {
        if (isset($page_attached_files) && $page_attached_files != null) {
            $linksarray = get_google_links($page_attached_files);

            foreach ($linksarray as $l) {
                list($vatt_id, $vtype, $vupload_fileid, $vupload_filename, $vupload_filepath) = $l;
                if (!empty($vtype) && $vtype != 'X') {
                    $uploaded = true;
                }
            }
        }
//        if (!$uploaded) {
//            form_set_error('', t('You have not included an attachment. Please add a resource before saving.'));
//        }
        if (empty($form_state['clicked_button']['#post']['add_resources']['page_data']) ||
                $form_state['clicked_button']['#post']['add_resources']['page_data'] == '') {
            form_set_error('add_resources][page_data', t('You have not included any content. Please add your Page Content before saving.'));
        }
    }

    //Make sure that text message does not contain a script tag
    $pos = strpos($form_state['clicked_button']['#post']['add_resources']['resource_name'], "</script>");
    if (!($pos === false)) {
        form_set_error('add_resources][resource_name', t('Please enter valid name.'));
    }
    $pos = strpos($form_state['clicked_button']['#post']['add_resources']['description'], "</script>");
    if (!($pos === false)) {
        form_set_error('add_resources][description', t('Please enter valid description.'));
    }
}

function ntlp_resources_new_form_submit($form, $form_state) {
    global $user;

    cache_set("ntlp_resource:" . session_id(), null, 'cache', CACHE_TEMPORARY);

    $resource_type = $form_state['clicked_button']['#post']['add_resources']['selected_resource'];
    $resource_name = $form_state['clicked_button']['#post']['add_resources']['resource_name'];
    $description = $form_state['clicked_button']['#post']['add_resources']['description'];
    $course_id = $form_state['clicked_button']['#post']['add_resources']['course_id'];
    $last_state_url = $form_state['clicked_button']['#post']['add_resources']['last_state_url'];
    $is_visible = $form_state['clicked_button']['#post']['add_resources']['is_visible'];

    if ($is_visible)
        $visibility = 'S';
    else
        $visibility = 'H';

    if ($deleted_by == null || empty($deleted_by)) {
        $deleted_by = 0;
    }

    if (isset($form_state['clicked_button']['#post']['add_resources']['edit_resource'])) {
        $edit_resource = $form_state['clicked_button']['#post']['add_resources']['edit_resource'];
    }

    $is_shared = 0;
    $upload_fileid = -1;
    $attachmentdata = $form_state['clicked_button']['#post']['add_resources']['attached_files_hidden'];
    if (isset($attachmentdata) && $attachmentdata != null) {
        $linksarray = get_google_links($attachmentdata);
        //watchdog("attach_data", print_r($linksarray, true));

        foreach ($linksarray as $l) {
            list($vtype, $vupload_fileid, $vupload_filename, $vupload_filepath) = $l;
            //The attachment of Type==X will be overwritten by a valid resource.
            //To delete an attachment, resource should be deleted.
            if (!empty($vtype) && $vtype != 'X') {
                $type = 'U';
                $upload_fileid = $vupload_fileid;
                $upload_filename = $vupload_filename;
                $path = $vupload_filepath;
            }
        }
    }

    switch ($resource_type) {

        case 'lo_link':
            $type = 'L';
            $path = $form_state['clicked_button']['#post']['add_resources']['attach_link_for_link'];
            break;

        case "lo_video":
            $resource_radio = $form_state['clicked_button']['#post']['add_resources']['resource_radio'];
            if (isset($resource_radio)) {
                $type = $resource_radio;

                $path = $form_state['clicked_button']['#post']['add_resources']['video_embedded_code'];
            }
            break;

        case "lo_document":
            watchdog("doc_data", print_r($form_state['clicked_button']['#post']['add_resources'], true));
            $google_radio = $form_state['clicked_button']['#post']['add_resources']['google_radio'];
            if ($google_radio == 'gdocs') {
                unset($attachmentdata);
                if (isset($form_state['clicked_button']['#post']['add_resources']['google_hAttachedFiles'])) {
                    $attachmentdata = $form_state['clicked_button']['#post']['add_resources']['google_hAttachedFiles'];
                }
                watchdog("google_data", print_r($attachmentdata, true));
                if (isset($attachmentdata) && $attachmentdata != null) {
                    $linksarray = get_google_links($attachmentdata);

                    foreach ($linksarray as $l) {
                        list($vtype, $vupload_fileid, $vupload_filename, $vupload_filepath) = $l;
                        //The attachment of Type==X will be overwritten by a valid resource.
                        //To delete an attachment, resource should be deleted.
                        if (!empty($vtype) && $vtype != 'X') {
                            $type = 'G';
                            $upload_fileid = $vupload_fileid;
                            $upload_filename = $vupload_filename;
                            $path = $vupload_filepath;
                        }
                    }
                }
            }
            break;
        case "lo_page":
            if (isset($form_state['clicked_button']['#post']['add_resources']['page_data'])) {
                $path = $form_state['clicked_button']['#post']['add_resources']['page_data'];
            }
            break;
    }

    /* Associated Project Fields */
    if ($form_state['clicked_button']['#post']['add_resources']['project_nid'] != 0) {
        $project = $form_state['clicked_button']['#post']['add_resources']['project_nid'];
    } else {
        $project = 0;
    }

    if (isset($form_state['clicked_button']['#post']['add_resources']['project_row'])) {

        if ($form_state['clicked_button']['#post']['add_resources']['project_row'] != 'Not Available') {
            $project_section = $form_state['clicked_button']['#post']['add_resources']['project_row'];
        } else {
            $project_section = 0;
        }
    }


    if (isset($edit_resource) and $edit_resource > 0) {
        $resource_node = node_load($edit_resource);
    } else {
        $resource_node = new stdClass();
    }
    $resource_node->title = $resource_name;
    $resource_node->type = $resource_type;   // Your specified content type
    $resource_node->created = $time;
    $resource_node->body = $description;
    $resource_node->status = 1;
    $resource_node->sticky = 0;
    $resource_node->format = 1;       // Filtered HTML
    $resource_node->uid = $user->uid;
    $resource_node->comment = 2;

    node_save($resource_node);
    $nid = $resource_node->nid;

    $resource_order = 0;
    if ($project > 0) {
        $record = _get_resource_order($course_id, $project_section);
        if ($record != false) {
            $resource_order = $record->resource_order + 1;
        }
    }

    //Save Course_Resource and Project_Resource
    if (isset($edit_resource) and $edit_resource > 0) {
        //Modifying an existing resource

        $query = db_query("UPDATE {ntlp_course_resource} SET course_nid = %d, fid = %d, type = '%s',
                attachment_name = '%s', path = '%s', is_shared = %d
                WHERE nid = %d", $course_id, abs($upload_fileid), $type, $upload_filename, $path, $is_shared, $edit_resource);

        if ($project > 0) {
//
//            if (!is_resource_exist_in_prjoect($edit_resource, $project, $project_section)) {
//
//                $query = db_query("INSERT INTO {ntlp_project_resource}
//                (nid, project_nid, section_tid, resource_order, availability)
//                VALUES(%d, %d, %d, %d, '%s')",
//                                $edit_resource, $project, $project_section, $resource_order, $visibility);
//            } else {
                $query = db_query("UPDATE {ntlp_project_resource}
                    SET section_tid=%d, availability='%s'
                    WHERE nid=%d", $project_section, $visibility, $edit_resource);
//            }
        }
    } else {
        //Adding a new resource
//            watchdog("resource_save", "Insert with Project:$project and section : $project_section ");
        $query = db_query("INSERT INTO {ntlp_course_resource} (nid, course_nid, fid, type, attachment_name, path, deleted_by, deleted_on, is_shared )
            VALUES(%d, %d, %d, '%s', '%s', '%s', 0, NULL, %d)", $nid, $course_id, abs($upload_fileid), $type, $upload_filename, $path, $is_shared);

        if ($project > 0) {
            $query = db_query("INSERT INTO {ntlp_project_resource}
                (nid, project_nid, section_tid, resource_order, availability)
                VALUES(%d, %d, %d, %d, '%s')",
                            $nid, $project, $project_section, $resource_order, $visibility);
        }

        // block cache update code for new resources only
        $node_type = ntlp_get_node_type($course_id);
        if ($node_type == 'ntlp_group') {
            ntlp_update_block_cache('ntlp_group', $course_id);
        }
    }

    //Insert / Delete Resource Attachments
    unset($attachmentdata);
    if (isset($form_state['clicked_button']['#post']['add_resources']['page_attached_files_hidden'])) {
        $attachmentdata = $form_state['clicked_button']['#post']['add_resources']['page_attached_files_hidden'];
    }
//    watchdog("attach_data1", print_r($attachmentdata, true));
    if (isset($attachmentdata) && !empty($attachmentdata)) {
        $linksarray = get_google_links($attachmentdata);
//        watchdog("attach_data2", count($linksarray)."<br>".print_r($linksarray, true));

        foreach ($linksarray as $l) {
            list($att_id, $type, $upload_fileid, $upload_filename, $upload_filepath) = $l;
//            watchdog("attach_data3", "$nid, $att_id, $type, $upload_fileid, $upload_filename, $upload_filepath");
            if (!empty($type)) {
                if ($type == 'X') {
                    $query = db_query("DELETE FROM {ntlp_resource_attachment} WHERE resource_nid = %d AND id = %d",
                                    $nid, abs($att_id));
                } else {
                    if ($att_id < 0) {
                        if ($type == 'G') {
                            $query = db_query("INSERT INTO {ntlp_resource_attachment} (name, resource_nid, pebl_attachment_fid, type, path, order_number)
                                VALUES('%s', %d, 0, 'G', '%s', 0)", $upload_filename, $nid, $upload_filepath);
                        } else {
                            $query = db_query("INSERT INTO {ntlp_resource_attachment} (name, resource_nid, pebl_attachment_fid, type, path, order_number)
                                VALUES('%s', %d, %d, '%s', '%s', 0)", $upload_filename, $nid, $upload_fileid, $type, $upload_filepath);
                        }
                    }
                }
            }
        }
    }
    $url = $_GET['q'];
    $args = explode('/', $url);
    if ($args[1] == 'library') {
        drupal_goto('ntlp/library/project/view/' . $course_id);
    } else {
        drupal_goto('ntlp/courses/projects/' . $course_id . '/' . $project);
    }
}

function insert_google_records($is_new, $resource_nid, $course_id, $project, $project_section, $resource_order, $fid, $type, $attachment_name, $url, $is_shared) {

    $deleted_by = 0;

    if ($fid < 0) {
        $fid = abs($fid);
        if ($type != 'X') {

//            $resource_exit = check_resource($resource_nid, $course_id);

            if ($is_new == true) {

                db_query("UPDATE {ntlp_course_resource} SET fid = %d, type ='%s', attachment_name='%s', path='%s' where nid = %d ",
                        $fid, $type, $attachment_name, $url, $resource_nid);
            } else {

                db_query("INSERT INTO {ntlp_course_resource} (nid, course_nid, fid, type, attachment_name, path, deleted_by, deleted_on, is_shared )
                    VALUES(%d, %d, %d, '%s', '%s', '%s', %d, NULL, %d)",
                        $resource_nid, $course_id, $fid, $type, $attachment_name, $url, $deleted_by, $is_shared);
            }

            if ($project > 0) {
                $project_exits = check_resource_project($resource_nid);
                if ($project_exits == true) {
                    $query = db_query("UPDATE {ntlp_project_resource} SET 
                     section_tid = %d, availability = '%s' WHERE nid = %d",
                                    $project_section, 'S', $resource_nid);
                } else {

                    $query = db_query("INSERT INTO {ntlp_project_resource} (nid, project_nid, resource_order,
                     section_tid, availability ) VALUES(%d, %d, %d, %d, '%s')", $resource_nid, $project, $resource_order,
                                    $project_section, 'S');
                }
            }
        }
    }
}

function back_to_activity_resource($course_id) {
    include "resources.template.inc";
    global $user;

    $output = $HTMLform_groupbox_hdr;

    if (arg(4) && arg(5)) { // means its ntlp/courses/projects page
        $title = str_replace('@ACTIVITIES_LINK', url('ntlp/courses/projects/' . $course_id . '/' . arg(5)), $output);
    } else {
        $title = str_replace('@ACTIVITIES_LINK', url('ntlp/courses/resource/' . $course_id), $output);
    }
    return $title;
}

function check_resource_project($project_nid) {

    $rs = db_query("select * from {ntlp_project_resource} where  nid = %d ", $project_nid);
    if ($rs->num_rows > 0) {

        return true;
    } else {
        return false;
    }
}

function get_google_links($var) {

    $array = split(';', $var);

    foreach ($array as $element) {
        $elem = trim($element);
        $attach = explode(',', $elem);
        $link[] = $attach;
    }
//    sort($link);

    return $link;
}

function isValidURL($url) {
    return preg_match('|^http(s)?://[a-z0-9-]+(.[a-z0-9-]+)*(:[0-9]+)?(/.*)?$|i', $url);
}

?>
