<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_courses_activity', 'data_access');
require_once ('data_access.inc');

function ntlp_document_library_frm() {
    return drupal_get_form('ntlp_document_library_add_resource_form');
}

function ntlp_document_library_add_resource_form($form_state) {


    global $base_path, $user;
    include("document.template.inc");
    include('ntlp_admincourse.template.inc');

    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_document_library') . '/ntlp_document_library.js');

    ahah_helper_register($form, $form_state);

    $path = $base_path;

    $school_moduleObj = NULL;
    $library_type = NULL;
    $module_nid = 0;

    $resource_nid = intval(arg(5));
    $school_nid = get_this_school();

    if (!isset($form_state['values'])) {
        set_item_url_handler('Resource');
    }

    if ($library_type == NULL) {
        $library_type = arg(1);
    }
    if ($school_moduleObj == NULL) {
        //Store Back URL only when opening the page for the first time, as this will also be called from Validate


        if ($library_type == 'document') {
            $school_moduleObj = get_school_module($school_nid, 'ntlp_school_document_library');
        } else {
            $school_moduleObj = _get_module_detail('ntlp_network_resource_library');
        }
    }
    if ($module_nid == 0) {
        $module_nid = $school_moduleObj->nid; // module_nid
    }

    if (isset($resource_nid) && $resource_nid > 0) {
        $library_resourceObj = _get_library_resource_detail($resource_nid);
    }

    $form['add_resources'] = array(
        '#type' => 'item',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div id="add-resources-wrapper" align="left" >' . str_replace('@BACK_LINK', get_back_url(), $HTMLform_groupbox_hdr),
        '#suffix' => $HTMLform_groupbox_ftr . '</div>',
    );

    $form['add_resources']['save_button'] = array(
        '#type' => 'button',
        '#executes_submit_callback' => TRUE,
        '#attributes' => array('class' => 'SaveBtn'),
        '#submit' => array('ntlp_document_library_add_resource_submission'),
        '#validate' => array('ntlp_document_library_add_resource_form_validate'),
        '#value' => t('Save'),
        '#prefix' => '<div style="position:absolute;top:0px;right:0px">',
        '#suffix' => '',
    );

    $form['add_resources']['cancel_button'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#validate' => array('ntlp_document_library_add_resource_form_validate'),
        '#attributes' => array('class' => 'CancelBtn'),
        '#prefix' => '',
        '#suffix' => '</div>',
    );


    $form['add_resources']['school_nid'] = array(
        '#type' => 'hidden',
        '#value' => $school_nid,
    );

    $form['add_resources']['resource_nid'] = array(
        '#type' => 'hidden',
        '#value' => $resource_nid,
    );



    $form['add_resources']['resource_type'] = array(
        '#type' => 'hidden',
        '#value' => $resource_type,
    );

    $form['add_resources']['module_nid'] = array(
        '#type' => 'hidden',
        '#value' => $module_nid,
    );
    $form['add_resources']['library_type'] = array(
        '#type' => 'hidden',
        '#value' => $library_type,
    );

    $options = array(
        'lo_link' => 'Link',
        'lo_image' => 'Images',
        'lo_video' => 'Videos',
        'lo_document' => 'Documents',
        'lo_page' => 'Pages'
    );

    if (isset($library_resourceObj)) {
        $resource_type = $library_resourceObj->node_type;
    } else {
        $resource_type = 'lo_page';
    }

    $form['add_resources']['selected_type'] = array(
        '#type' => 'radios',
        '#options' => $options,
        '#default_value' => $resource_type,
        '#prefix' => '<tr><th style="border-top:1px solid #9ECCE9">' . t('Type') . '</th><td style="border-right:1px solid #9fcce9;padding-left:20px;border-top:1px solid #9ECCE9" colspan="3"><div id="resourceRadioBtn">',
        '#suffix' => '</div></td></tr>',
        '#attributes' => array('onclick' => 'load_attributes(this);'),
    );


    if (isset($library_resourceObj->title)) {
        $resource_name = $library_resourceObj->title;
    } else {
        $resource_name = '';
    }

    $form['add_resources']['resource_name'] = array(
        '#type' => 'textfield',
//            '#value'=> (isset($library_resourceObj))?$library_resourceObj->title:'',
        '#default_value' => $resource_name,
        '#prefix' => '<tr id="resources"><th>' . t('Resource Name') . '</th><td colspan="3">',
        '#suffix' => '</td></tr>',
//            '#default_value' =>  variable_get('resource_name', ''),
    );

    $form['add_resources']['resource_radio'] = array(
        '#type' => 'radios',
        '#options' => array('U' => 'Upload', 'L' => 'Link'),
        '#prefix' => '<tr id="attach_resource_option" style="display:none" ><th><span id="resource_heading"></span></th><td colspan="3" style="padding:0px">',
        '#suffix' => '</td></tr>',
        '#id' => 'resource_radio2',
        '#attributes' => array('onclick' => 'show_attachement_options(this);'),
        '#default_value' => 'U',
    );
// there I am checking.. the session variable.. if it has a value for google_radio then set this..
    if (isset($library_resourceObj)) {
        if ($library_resourceObj->fid > 0) {
            $google_radio = 'upload';
        } else {
            if ($library_resourceObj->type == 'G') {
                $google_radio = 'gdocs';
            } else {
                $google_radio = 'upload';
            }
        }
    } else {
        $google_radio = 'upload';
    }
// which function sir... this one... the load function..no at the end of this function
    //  watchdog('ses_value', print_r($variables_to_restore, true) );    //remove session variable, at the end of this func
    $form['add_resources']['google_radio'] = array(
        '#type' => 'radios',
        '#options' => array('upload' => 'Upload', 'gdocs' => 'Google Doc'),
        '#prefix' => '<tr id="document_attach_option" style="display:none" ><th >' . t('Documents') . '</th><td colspan="3" style="paddding:0px">',
        '#suffix' => '</td></tr>',
        '#attributes' => array('onclick' => 'show_google_option(this)'),
        '#default_value' => $google_radio,
    );

    $form['add_resources']['attached_files'] = array(
        '#type' => 'item',
        '#prefix' => '<tr style="display:none" id="resources-wrapper"><td></td><td colspan="3">',
        '#suffix' => '</td></tr>',
    );

    $display_link = '';
    $hiddendata = '';
    $attached_files_data = '';
    if (isset($library_resourceObj) && $library_resourceObj->fid != 0) {
        $hiddendata .= 'F,' . $library_resourceObj->fid . ',' . $library_resourceObj->filename . ';';
        $attached_files_data .= '<div id="F_' . $library_resourceObj->fid . '"> <a onclick="other_delete_attachment(\'F_' . $library_resourceObj->fid . '\');" ><img src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png" border="0" /></a>&nbsp;&nbsp;' . $library_resourceObj->filename . '</div>';
        $display_link = 'style="display:none;"';
    }


    if ($resource_type == 'lo_document' && $library_resourceObj->type == 'G') {
        $filename = empty($library_resourceObj->attachment_name) ? $library_resourceObj->title : $library_resourceObj->attachment_name;
        $display = '';
        $hiddendata .= 'G,' . $library_resourceObj->nid . ',' . $filename . ',' . $library_resourceObj->path . ';';

//        $divdata .= '<div id="G_' . $resource_Obj->nid . '"> <a onclick="doc_delete_attachment(\'G_' . $resource_Obj->nid . '\');" ><img src="' . $base_path . 'sites/all/modules/ntlp_courses_activity/theme/delete.PNG" border="0" /></a><a href=' . $resource_Obj->path . '>' . $resource_Obj->path . '</a></div>';
        $divdata = '<div id="G_' . $library_resourceObj->nid . '">' .
                '<img style="position:relative;top:2px;margin-right:4px;" onclick="doc_delete_attachment(\'G_' . $library_resourceObj->nid . '\');" width="14" height="14" src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png" border="0" />' .
                '<a href="' . $library_resourceObj->path . '" target="_blank">' . $filename . '</a></div>';
    } else {
        $hiddendata = '';
        $display = 'style="display:none"';
    }

    $form['#fileupload_embed'] = true;
    $form['#fileupload_auto_upload'] = true;

    $attach_fileupload_link = '<div style="width: 100px;  cursor: pointer !important;" > <span style="float:left;text-decoration:none;color: #3399CC;"><a class="project_resource_Green_text" ' . $display_link . ' id="attach_link"><span style="color:#000">+</span>Attach file</a></span></div>';

    $form['add_resources']['upload_control'] = array(
        '#prefix' => '<tr id="upload_files_option" style="display:none;"><th >' . t('Attach') . '</th><td colspan="3" onmouseover="place_fileupload_control(\'edit-add-resources-upload-control\', this);" onmouseout="place_fileupload_control_mouseout(this);" >' . $attach_fileupload_link,
        '#suffix' => '<div id="attach_links" style="float:left;width:auto;">' . $attached_files_data . '</div></td></tr>',
    );

    fileupload_get_control($form, $form_state, array('add_resources', 'upload_control'));

    $form['add_resources']['attached_files_hidden'] = array(
        '#type' => 'hidden',
        '#value' => $hiddendata,
        '#id' => 'attached_files_hidden',
    );

//    $form['add_resources']['google_urls'] = array(
//        '#type' => 'hidden',
//        '#prefix' => '<tr id="google_docs_row" style="display:none;"><th>Attach Google Docs</th><td colspan="3"><div id="google_urllist" style="height:20px;">',
//        '#suffix' => '</div></td></tr>',
//    );
    $form['add_resources']['google_radio']['google_urls'] = array(
        '#type' => 'item',
        '#prefix' => '<div id="google_docs_row" ' . $display . '>
                        <div id="google_urllist" style="height:20px;">' . $divdata . '</div>
                      </div>',
    );

    $form['add_resources']['google_hAttachedFiles'] = array(
        '#type' => 'hidden',
        '#value' => $hiddendata,
        '#id' => 'attached_files_hidden_for_google_docs',
            //'#attributes'=> array('style' => 'display:block;'),
    );

    $display_google_attach_link = (!empty($divdata) ? 'display:none;' : 'display:block;');
    $form['add_resources']['google_attached'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => '<tr id="google_doc_attached" style="' . $display_google_attach_link . 'display:table-row;"><th style="">Attach</th><td colspan="3"  >' . dlg("+ Attach Google Doc", 'ntlp/document/library/resource/attachment/google', 400, 600, 'project_resource_Green_text'),
        '#suffix' => '</td></tr>',
    );

    if (isset($library_resourceObj)) {
        if ($library_resourceObj->node_type != 'lo_page') {
            $other_resource_link = $library_resourceObj->path;
        }
    } else {
        $other_resource_link = '';
    }
//      $variables_to_restore->other_resource_link
    $form['add_resources']['other_resource_link'] = array(
        '#type' => 'textfield',
        '#size' => '90',
        '#default_value' => $other_resource_link,
        '#prefix' => '<tr  id="other_resource_link" style="display:none"><th style="width:113px;">' . t('URL') . '</th><td colspan="3">',
        '#suffix' => '</td></tr>',
    );

    if (isset($library_resourceObj) && $library_resourceObj->node_type == "lo_link") {
        $attach_link_for_link = $library_resourceObj->path;
    } else {
        $attach_link_for_link = '';
    }
    $form['add_resources']['attach_link_for_link'] = array(
        '#type' => 'textfield',
        '#size' => '90',
//            '#default_value' => (isset($library_resourceObj->node_type) && $library_resourceObj->node_type == 'lo_link' ?  $library_resourceObj->path : ''),
        '#default_value' => $attach_link_for_link,
        '#prefix' => '<tr id="attach_link_for_link"  ><th style="width:113px">' . t('Link (URL)') . '</th><td  colspan="3">',
        '#suffix' => '</td></tr>',
    );

    if (isset($library_resourceObj) && $library_resourceObj->node_type == 'lo_page') {

        $page_data = $library_resourceObj->path;
    } else {
        $page_data = '';
    }
    $form['add_resources']['page_data'] = array(
        '#type' => 'textarea',
        '#rows' => '5',
        '#cols' => '50',
//            '#default_value' => (isset($library_resourceObj->path) ?  $library_resourceObj->path : ''),
        '#default_value' => $page_data,
        '#resizable' => false,
        '#prefix' => '<tr id="page_data" style="display:none"><th style="width:113px;">' . t('Page Content') . '</th><td colspan="3"><div style="width:600px; float:left;">',
        '#suffix' => '</div></td></tr>',
    );

    //Multiple file attachment control for Page resource
    $hiddendata = '';
    $attached_files_data = '';
    if (isset($library_resourceObj) && $library_resourceObj->node_type == 'lo_page') {
        $rs_attachment = get_resource_attachment($resource_nid);
//        echo 'Attachments: '.$rs_attachment->num_rows.', '. $resource_nid .'<br>';
        if ($rs_attachment->num_rows > 0) {
            while ($rec = db_fetch_object($rs_attachment)) {
                $hiddendata .= 'F,' . $rec->fid . ',' . $rec->filename . ';';
                $attached_files_data .= '<div id="F_' . $rec->fid . '"><a onclick="page_delete_attachment(\'F_' . $rec->fid . '\');" ><img src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png" border="0" /></a>&nbsp;&nbsp;<a href="' . $base_path . $rec->filepath . '" target="_blank">' . $rec->filename . '</a></div>';
            }
        }
    }
    $form['#fileupload_embed'] = true;
    $form['#fileupload_auto_upload'] = true;

    $attach_fileupload_link = '<div style="width: 100px;  cursor: pointer !important;" > <span style="float:left;text-decoration:none;color: #3399CC;"><a class="project_resource_Green_text" id="attach_link"><span style="color:#000">+</span>Attach file</a></span></div>';

    $form['add_resources']['pageattachments_upload_control'] = array(
        '#prefix' => '<tr id="page_attachments" style="display:none"><th >' . t('Attachments') . '</th><td colspan="3" onmouseover="place_fileupload_control(\'edit-add-resources-pageattachments-upload-control\', this);" onmouseout="place_fileupload_control_mouseout(this);" >' . $attach_fileupload_link,
        '#suffix' => '<br><div id="page_attach_links" style="float:left;width:auto;" class="attach_link_forum" >' . $attached_files_data . '</div></td></tr>',
    );

    fileupload_get_control($form, $form_state, array('add_resources', 'pageattachments_upload_control'), '', true);

    $form['add_resources']['page_attached_files_hidden'] = array(
        '#type' => 'hidden',
        '#value' => $hiddendata,
        '#id' => 'page_attached_files_hidden',
    );



    if (isset($library_resourceObj->body)) {
        $description = $library_resourceObj->body;
    } else {
        $description = '';
    }

    $form['add_resources']['description'] = array(
        '#type' => 'textarea',
        '#wysiwyg' => FALSE,
        '#rows' => '5',
        '#cols' => '70',
//            '#default_value' => (isset($library_resourceObj->body) ?  $library_resourceObj->body : ''),
        '#default_value' => $description,
        '#resizable' => false,
        '#prefix' => '<tr id="description" ><th style="width:113px;">' . t('Description') . '
                    <div style="font-style:italic; font-size:10px;">(optional)</div></th><td colspan="3">',
        '#suffix' => '</td></tr>',
    );

    $form['add_resources']['resoruce_uploadFiles'] = array(
        '#type' => 'hidden',
        '#value' => '',
        '#prefix' => '<tr style="display:none" ><td colspan="4">',
        '#suffix' => '</td></tr>',
    );


    if (isset($library_resourceObj)) {

        if (isset($resource_nid)) {

            if (arg(1) == 'document') {
                $keywords_array = get_lib_vocabulary_by_vid($resource_nid, "NTLP Keywords for School Document Library");
            } else {
                $keywords_array = get_lib_vocabulary_by_vid($resource_nid, "NTLP Keywords for Network Resource Library");
            }
            $category_array = get_lib_vocabulary_by_vid($resource_nid, "NTLP Library Category");

            $count = 0;
            //get user selected keywords
            if ($keywords_array != null) {
                foreach ($keywords_array as $value) {
                    if ($count > 0)
                        $keywords .= ', ' . trim($value);
                    else
                        $keywords .= trim($value);
                    $count+=1;
                }
            }
        }
    }else {
        $keywords = '';
    }

//    if(temp_selected_categories)
    $form['add_resources']['keywords'] = array(
        '#type' => 'textfield',
        '#default_value' => $keywords,
        '#prefix' => '<tr><th style="width:160px;">' . t('Keywords') . '</th><td colspan="3" >',
        '#suffix' => '</td></tr>',
    );


    if (isset($library_resourceObj)) {
        $featured = $library_resourceObj->promote;
    } else {
        $featured = '';
    }
    $form['add_resources']['featured_resource'] = array(
        '#type' => 'checkbox',
        '#default_value' => $featured,
        '#prefix' => '<tr><th style="width:160px;">' . t('Featured Resource') . '</th><td colspan="3" >',
        '#suffix' => '</td></tr>',
    );


    //user categories stars here
    $form['add_resources']['add_outcome'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<tr><th style="width:160px;">' . t('Category') . '</th><td colspan="3" >',
        '#suffix' => '</td></tr>',
    );


    if (!empty($category_array)) {
        foreach ($category_array as $key => $value) {
            $category_selected .= $key . ',';
        }
    }

    $form['add_resources']['add_outcome']['selected_oc_id'] = array(
        '#type' => 'hidden',
        '#id' => 'selected_oc_id',
        '#default_value' => (!empty($category_selected)) ? ($category_selected) : '',
    );



    /////////////////////
    if (!empty($resource_nid) && $resource_nid > 0) {
        if (empty($category_array)) {
            //user categories empty starts here
            $cat_status = 'user defined categories are emtpy';

            $options = get_library_categories($school_nid, $module_nid);

            // filter categories according to the permissions
            $maxCategory = sizeof($options);
            $outcomediv1 = '<span class="comboboxBgLeft" style="width:150px;">
        <select id="%ID%" onchange="onchange_outcome_hidden(this,%INDEX%)">';
            $outcomediv1 .= '<option value="x">None</option>';
            if (isset($options)) {
                foreach ($options as $key => $value) {
                    $outcomediv1 .= '<option value="' . $key . '">' . $value . '</option>';
                }
            }
            $outcomediv1 .= '</select></span>';
            $form['add_resources']['add_outcome']['hide_outcome1'] = array(
                '#type' => 'hidden',
                '#id' => 'oc_td1',
                '#value' => $outcomediv1,
            );


            $tr_count = 1;
            $outcomediv1 = str_replace("%INDEX%", $tr_count, $outcomediv1);
            $outcomediv1 = str_replace("%ID%", $tr_count, $outcomediv1);
            $form['add_resources']['add_outcome']['add_outcome'] = array(
                '#type' => 'item',
                '#value' => '
                    <table class="table" cellpadding="8" width="100%" style="border:0px;" cellspacing="0" border="0">
                        <tbody id="ocContainer">
                             <tr id="outcome_' . $tr_count . '">
                                 <td style="border:none;">' . $outcomediv1 . '</td>
                             </tr>
                             
                             <tr id="ocLastRow">
                                <td style="border:none">
                                    <a class="AddOutComesBtn" onclick="get_outcome_combo(' . $maxCategory . ');" style="font-weight: normal;">Add Category</a>
                                </td>
                             </tr>
                    </table>',
            );
            //user categories empty ends here
        } else {
            //user categories not empty starts here

            $options = get_library_categories($school_nid, $module_nid);
            // filter categories according to the permissions
            $maxCategory = sizeof($options);


            if (isset($category_array)) {
                //if user selected categories are availale for edit
                $i = 0;
                foreach ($category_array as $key => $value) {
                    $tr_count = $i;

                    $outcomediv1 = '<span class="comboboxBgLeft" style="width:150px;">
        <select id="%ID%" onchange="onchange_outcome_hidden(this,%INDEX%)">';
                    $outcomediv1 .= '<option value="x">None</option>';
                    if (isset($options)) {
                        foreach ($options as $key2 => $value2) {
                            if ($category_array[$key] == $value2)
                                $outcomediv1 .= '<option value="' . $key2 . '" selected>' . $value2 . '</option>';
                            else
                                $outcomediv1 .= '<option value="' . $key2 . '">' . $value2 . '</option>';
                        }
                    }
                    $outcomediv1 .= '</select></span>';
                    $form['add_resources']['add_outcome']['hide_outcome1'] = array(
                        '#type' => 'hidden',
                        '#id' => 'oc_td1',
                        '#value' => $outcomediv1,
                    );

                    $outcomediv1 = str_replace("%INDEX%", $tr_count, $outcomediv1);
                    $outcomediv1 = str_replace("%ID%", $tr_count, $outcomediv1);
                    $outcomediv .= '<tr id="outcome_' . $tr_count . '"><td style="border:none">' . $outcomediv1 . '</td></tr>';
                    $i++;
                }
            }



            $form['add_resources']['add_outcome']['add_outcome'] = array(
                '#type' => 'item',
                '#value' => '<table class="table" cellpadding="8" width="292px" style="border:0px;" cellspacing="0" border="0">
  <tbody id="ocContainer">
    ' . $outcomediv . '
    <tr id="ocLastRow">
      <td style="border:none"><a class="SubmitAssignmentBtn" style="float:left !important" onclick="get_outcome_combo(' . $maxCategory . ');">Add Category</a></td>
      <td style="border:none">&nbsp</td>
    </tr>
</table>',
            );
        }
    } else {
        //user categories for new form starts here
        $cat_status = 'its a new form dummy';

        $options = get_library_categories($school_nid, $school_moduleObj->nid);
        // filter categories according to the permissions

        $maxCategory = sizeof($options);
        $outcomediv1 = '<span class="comboboxBgLeft" style="width:150px;">
        <select id="%ID%" onchange="onchange_outcome_hidden(this,%INDEX%)">';
        $outcomediv1 .= '<option value="x">None</option>';
        if (isset($options)) {
            foreach ($options as $key => $value) {
                $outcomediv1 .= '<option value="' . $key . '">' . $value . '</option>';
            }
        }
        $outcomediv1 .= '</select></span>';
        $form['add_resources']['add_outcome']['hide_outcome1'] = array(
            '#type' => 'hidden',
            '#id' => 'oc_td1',
            '#value' => $outcomediv1,
        );


        $tr_count = 1;
        $outcomediv1 = str_replace("%INDEX%", $tr_count, $outcomediv1);
        $outcomediv1 = str_replace("%ID%", $tr_count, $outcomediv1);
        $form['add_resources']['add_outcome']['add_outcome'] = array(
            '#type' => 'item',
            '#value' => '<table class="table" cellpadding="8" width="292px" style="border:0px;" cellspacing="0" border="0">
  <tbody id="ocContainer">
    <tr id="outcome_' . $tr_count . '"><td style="border:none">' . $outcomediv1 . '</td></tr>
        <tr id="ocLastRow">
      <td style="border:none"><a class="AddOutComesBtn" onclick="get_outcome_combo(' . $maxCategory . ');">Add Category</a></td>
      <td style="border:none">&nbsp</td>
    </tr>
</table>',
        );
    }
    // you mean here rightyes
//    variable_del('library_resource:'.session_id());
    cache_set("library_resource:" . session_id(), null, 'cache', CACHE_TEMPORARY);
    return $form;
}

function ntlp_document_library_add_resource_form_validate($form, $form_state) {

    $resource_nid = $form_state['clicked_button']['#post']['add_resources']['resource_nid'];
    $resource_type = $form_state['clicked_button']['#post']['add_resources']['selected_type'];


    $resource_name = $form_state['clicked_button']['#post']['add_resources']['resource_name'];
    $description = $form_state['clicked_button']['#post']['add_resources']['description'];
    $keywords = $form_state['clicked_button']['#post']['add_resources']['keywords'];
    $featured_resource = $form_state['clicked_button']['#post']['add_resources']['featured_resource'];
    $google_radio = $form_state['clicked_button']['#post']['add_resources']['google_radio'];
    $resource_radio = $form_state['clicked_button']['#post']['add_resources']['resource_radio'];
    $selected_categories = clean_array($form_state['clicked_button']['#post']['add_resources']['add_outcome']['selected_oc_id']);
    $other_resource_link = $form_state['clicked_button']['#post']['add_resources']['other_resource_link'];
    $attach_link_for_link = $form_state['clicked_button']['#post']['add_resources']['attach_link_for_link'];
    $page_attach_data = $form_state['clicked_button']['#post']['add_resources']['pageattachments_upload_control'];
    $page_attached_files = $_POST['add_resources']['page_attached_files_hidden'];

    $school_nid = $form_state['clicked_button']['#post']['add_resources']['school_nid'];
    $module_nid = $form_state['clicked_button']['#post']['add_resources']['module_nid'];
    $library_type = $form_state['clicked_button']['#post']['add_resources']['library_type'];

    if (isset($form_state['clicked_button']['#post']['add_resources']['page_data'])) {
        $page_data = $form_state['clicked_button']['#post']['add_resources']['page_data'];

//        if($page_data == '<p>&nbsp;</p>') {
//            watchdog('page_resource', 'page_data chcking true value '.$page_data);
//        }else {
//            watchdog('page_resource', 'page_data chcking false value '.$page_data);
//        }
    }
    watchdog('resource_valid', print_r($form_state, true));
//    watchdog('resource_valid', print_r($_POST, true));

    if ($form_state['clicked_button']['#post']['op'] == 'Cancel') {

        cache_set("library_resource:" . session_id(), null, 'cache', CACHE_TEMPORARY);
        drupal_goto('ntlp/goback');
    }
    if ($resource_type == 'lo_video') {
        $resource_type_name = "Video";
    } else if ($resource_type == 'lo_image') {
        $resource_type_name = "Image";
    } else if ($resource_type == 'lo_document') {
        $resource_type_name = "Document";
    } else if ($resource_type == 'lo_page') {
        $resource_type_name = "Content";
    }

//    watchdog('resource','other_resource_link '.$form_state['clicked_button']['#post']['add_resources']['other_resource_link']);
//    watchdog('google attacment','check attachmet file'.$resource_type);

    $variables_to_restore = new stdClass;
    $variables_to_restore->resource_nid = $resource_nid;
    $variables_to_restore->resource_type = $resource_type;
    $variables_to_restore->resource_name = $resource_name;
    $variables_to_restore->description = $description;
    $variables_to_restore->school_nid = $school_nid;
    $variables_to_restore->module_nid = $module_nid;
    $variables_to_restore->keywords = $keywords;
    $variables_to_restore->featured_resource = $featured_resource;
    $variables_to_restore->other_resource_link = $other_resource_link;
    $variables_to_restore->attach_link_for_link = $attach_link_for_link;
    $variables_to_restore->pageattachments_upload_control = $page_attach_data;
    $variables_to_restore->page_attached_files = $page_attached_files;
    $variables_to_restore->library_type = $library_type;

    if ($resource_type == 'lo_document') {
        $variables_to_restore->google_radio = $google_radio;
    } else if ($resource_type == 'lo_image' || $resource_type == 'lo_video') {
        $variables_to_restore->resource_radio = $resource_radio;
    } else if ($resource_type == 'lo_page') {
        $variables_to_restore->page_data = $page_data;
    }
    $variables_to_restore->selected_categories = $selected_categories;

//    watchdog('val_func1', 'To restore:'.print_r($variables_to_restore, true));
    cache_set("library_resource:" . session_id(), $variables_to_restore, 'cache', CACHE_TEMPORARY);

    unset($attachmentdata);
    $uploaded = false;
    if (isset($form_state['clicked_button']['#post']['add_resources']['attached_files_hidden'])) {
        $attachmentdata = $form_state['clicked_button']['#post']['add_resources']['attached_files_hidden'];
    }
    if (isset($attachmentdata) && $attachmentdata != null) {
        $linksarray = get_google_links($attachmentdata);

        foreach ($linksarray as $l) {
            list($vtype, $vupload_fileid, $vupload_filename) = $l;
            if (!empty($vtype) && $vtype != 'X') {
                $uploaded = true;
            }
        }
    }

    if (empty($form_state['clicked_button']['#post']['add_resources']['resource_name'])) {
        form_set_error('add_resources][resource_name', 'Invalid resource name.');
    }

//    watchdog('sel_cate', print_r($selected_categories, true));
    if (empty($selected_categories)) {
        form_set_error('', 'You should select at least one Category.');
    }

    if ($resource_type == 'lo_image' || $resource_type == 'lo_video') {

        if (!$uploaded && empty($form_state['clicked_button']['#post']['add_resources']['other_resource_link'])) {
            form_set_error('', t('You have not included a ' . $resource_type_name . '. Please add your resource before saving.'));
        }
    } else {
        if ($resource_type == 'lo_link') {
            if (empty($form_state['clicked_button']['#post']['add_resources']['attach_link_for_link'])) {
                form_set_error('', t('You have not included a Link. Please define your link before saving.'));
            }
        } elseif ($resource_type == 'lo_document') {
            if (!$uploaded && empty($form_state['clicked_button']['#post']['add_resources']['google_hAttachedFiles'])) {
                form_set_error('', t('You have not included a Document. Please add a document before saving.'));
            }
        } else if ($resource_type == 'lo_page') {
            if (empty($form_state['clicked_button']['#post']['add_resources']['page_data'])) {
                form_set_error('page_data', t('You have not included any content. Please add your Page Content before saving.'));
            }
        }
    }
    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/fileupload_ahah_helper.js', 'footer');
    drupal_add_js(drupal_get_path('module', 'ntlp_document_library') . '/ntlp_document_library.js');

    // end of validation fucntion... looks like validate is called after load, and after validate, load is not called again
}

function ntlp_document_library_add_resource_submission($form, $form_state) {
    global $user;

    //Getting Query String From URL
    $url = $_GET['q'];
    $args = explode('/', $url);


    cache_set("library_resource:" . session_id(), null, 'cache', CACHE_TEMPORARY);

    $resource_type = $form_state['clicked_button']['#post']['add_resources']['selected_type'];
    $resource_name = $form_state['clicked_button']['#post']['add_resources']['resource_name'];
    $resource_nid = $form_state['clicked_button']['#post']['add_resources']['resource_nid'];
    $description = $form_state['clicked_button']['#post']['add_resources']['description'];
    $school_nid = $form_state['clicked_button']['#post']['add_resources']['school_nid'];
    $module_nid = $form_state['clicked_button']['#post']['add_resources']['module_nid'];

    $featured_resource = $form_state['clicked_button']['#post']['add_resources']['featured_resource'];
    $keywords = $form_state['clicked_button']['#post']['add_resources']['keywords'];
    $category_tids = clean_array($form_state['clicked_button']['#post']['add_resources']['add_outcome']['selected_oc_id']);

    //for navigation


    if (empty($featured_resource)) {
        $featured_resource = 0;
    } else {
        $featured_resource = 1;
    }

    if ($resource_nid > 0) {
        $resource_node = node_load($resource_nid);
        $resource_node->title = $resource_name;
        $resource_node->body = $description;
        $resource_node->type = $resource_type;
        $resource_node->changed = $time;
    } else {
        $resource_node = new stdClass();
        $resource_node->title = $resource_name;
        $resource_node->type = $resource_type;   // Your specified content type
        $resource_node->body = $description;
        $resource_node->created = $time;
        $resource_node->changed = $time;
        $resource_node->status = 1;
        $resource_node->sticky = 0;
        $resource_node->format = 1;       // Filtered HTML
        $resource_node->comment = 2;       // Filtered HTML
        $resource_node->uid = $user->uid;  // UID of content owner
    }

    //promote is set to true to set the node as a featured resouce/document
    if ($featured_resource == 1) {
        $resource_node->promote = 1;
    } else {
        $resource_node->promote = 0;
    }

    node_save($resource_node);

    $keywords_arr = explode(",", $keywords);
    foreach ($keywords_arr as $obj) {
        if (!empty($obj)) {
            $incoming_terms[] = trim($obj);
        }
    }

    if ($args[1] == 'document') {
        $vid = NTLP_KEYWORDS_FOR_SCHOOL_DOCUMENT_LIBRARY;
    } else {
        $vid = NTLP_KEYWORDS_FOR_NETWORK_RESOURCE_LIBRARY;
    }

    $category_taxo = get_lib_tid_id($category_tids);

    if ($category_taxo != null) {
        taxonomy_node_save($resource_node, $category_taxo);
    }

    watchdog('keywords', print_r($incoming_terms, true) . ' vid ' . $vid);
    if ($incoming_terms != null) {

        db_query("DELETE FROM {term_node} WHERE nid = %d 
                AND tid IN (SELECT tid FROM {term_data} WHERE vid = %d)", $resource_node->nid, $vid);

        foreach ($incoming_terms as $term) {
            $query = db_query("SELECT tid FROM {term_data} WHERE name='%s' AND vid=%d", $term, $vid);
            $possible_term = db_fetch_object($query);
            watchdog('exist_keywords1', print_r($possible_term, true));
            //if the term doesn't already exist, add it to the cloud
            if ($query->num_rows == 0) {
                $new_term = array('vid' => $vid, 'name' => $term);
                if (taxonomy_save_term($new_term)) {
                    db_query("INSERT INTO {term_node} (nid, vid, tid) VALUES (%d, %d, %d)", $resource_node->nid, $resource_node->vid, $new_term['tid']);
                }
            } else {
                watchdog('exist_keywords2', print_r($possible_term, true));
                db_query("INSERT INTO {term_node} (nid, vid, tid) VALUES (%d, %d, %d)", $resource_node->nid, $resource_node->vid, $possible_term->tid);
            }
        }
    }


    if (isset($form_state['clicked_button']['#post']['add_resources']['attached_files_hidden'])) {
        $attachmentdata = $form_state['clicked_button']['#post']['add_resources']['attached_files_hidden'];
    }
    if (isset($attachmentdata) && $attachmentdata != null) {
        $linksarray = get_google_links($attachmentdata);
        foreach ($linksarray as $l) {
            list($vtype, $vupload_fileid, $vupload_filename) = $l;
            if (!empty($vtype) && $vtype != 'X') {
                $type = 'U';
                $fid = $vupload_fileid;
                $upload_filename = $vupload_filename;
            }
        }
    }

//
//    if ($resouce_type == 'lo_link') {
//        if (isset($form_state['clicked_button']['#post']['add_resources']['attach_link_for_link'])) {
//            $type = 'L';
//            $path = $form_state['clicked_button']['#post']['add_resources']['attach_link_for_link'];
//        }
//    } else if ($resouce_type == 'lo_image' || $resouce_type == 'lo_video') {
//        if (isset($form_state['clicked_button']['#post']['add_resources']['other_resource_link'])) {
//            $type = 'L';
//            $path = $form_state['clicked_button']['#post']['add_resources']['other_resource_link'];
//        }
//    } else if ($resouce_type == 'lo_page') {
//        if (isset($form_state['clicked_button']['#post']['add_resources']['page_data'])) {
//            $path = $form_state['clicked_button']['#post']['add_resources']['page_data'];
//            $type = 'P';
//        }
//    } else if ($resouce_type == 'lo_document') {
//
////        if(isset($form_state['clicked_button']['#post']['add_resources']['google_hAttachedFiles'])) {
////            $attachmentdata =  $form_state['clicked_button']['#post']['add_resources']['google_hAttachedFiles'];
////        }
//    }

    switch ($resource_type) {

        case 'lo_link':
            $type = 'L';
            $path = $form_state['clicked_button']['#post']['add_resources']['attach_link_for_link'];
            break;

        case "lo_video":
            $resource_radio = $form_state['clicked_button']['#post']['add_resources']['resource_radio'];
            if (isset($resource_radio)) {
                $type = $resource_radio;

                $path = $form_state['clicked_button']['#post']['add_resources']['other_resource_link'];
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
                            $fid = $vupload_fileid;
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


    watchdog("resource", "Saving: $resource_node->nid, $resource_type, $type, $fid, $upload_filename");

    if ($resource_nid > 0) {
        $query = db_query("UPDATE {ntlp_library_resource} SET fid=%d, type='%s', attachment_name = '%s', path='%s' WHERE nid=%d", abs($fid), $type, $upload_filename, $path, $resource_node->nid);
    } else {
        $query = db_query("INSERT INTO {ntlp_library_resource} (nid, category_tid, fid, type, attachment_name, path, deleted_by, deleted_on )
                VALUES(%d, %d, %d, '%s', '%s', '%s', %d, NULL)", $resource_node->nid, 0, abs($fid), $type, $upload_filename, $path, 0);
    }


    //Insert / Delete Resource Attachments for Page
    unset($attachmentdata);
    if (isset($form_state['clicked_button']['#post']['add_resources']['page_attached_files_hidden'])) {
        $attachmentdata = $form_state['clicked_button']['#post']['add_resources']['page_attached_files_hidden'];
    }
    if (isset($attachmentdata) && !empty($attachmentdata)) {
        $linksarray = get_google_links($attachmentdata);
//        watchdog("attach_data", print_r($linksarray, true));

        foreach ($linksarray as $l) {
            list($type, $upload_fileid, $upload_filename) = $l;
//            watchdog("attach_file", "$type, $upload_fileid, $upload_filename");
            if (!empty($type)) {
                if ($type == 'X') {
                    $query = db_query("DELETE FROM {ntlp_resource_attachment} WHERE resource_nid = %d AND pebl_attachment_fid = %d", $resource_node->nid, abs($upload_fileid));
                } else {
                    if (intval($upload_fileid) < 0) {
                        watchdog("attach_new", "$type, $upload_fileid, $upload_filename");
                        $query = db_query("INSERT INTO {ntlp_resource_attachment} (resource_nid, pebl_attachment_fid, type, path, order_number)
                            VALUES(%d, %d, '%s', '', 0)", $resource_node->nid, abs($upload_fileid), $type);
                    }
                }
            }
        }
    }


    if ($resource_nid > 0) {
        drupal_set_message("Library resource updated successfully. ");
//        variable_del('library_resource:'.session_id());
        drupal_goto('ntlp/goback');
    } else {
        drupal_set_message("Library resource saved successfully. ");
//        variable_del('library_resource:'.session_id());
        drupal_goto('ntlp/goback');
    }
}

function ntlp_most_popular_resource_contents() {
//    if ($school_nid == 0) {
//        //Getting Query String From URL
//        $resource_nid = 0;
//        $school_nid = 0;
//        $url = $_GET['q'];
//        $args = explode('/', $url);
//        for ($i = 0; $i < count($args); $i++) {
//            if (is_numeric($args[$i]) && empty($school_nid))
//                $school_nid = $args[$i];
//            elseif (is_numeric($args[$i]) && empty($resource_nid))
//                $resource_nid = $args[$i];
//        }
//    }
    include 'ntlp_doc_lib_resource.template.inc';

    $output = $HTMLResourcesBock;
    $output = str_replace('@BLOCK_HEADING', 'Most Popular', $output);
    $tempList = '';
    $school_nid = get_this_school();

    foreach (get_resource_details($school_nid, array('WITH_TITLE' => true)) as $resource) {
        if (is_user_access_this_category($school_nid, $resource->name)) {
            $tempList .= str_replace('@TITLE', l($resource->title, 'ntlp/document/library/resources/view/' . $resource->nid . '/' . $resource->category_tid), $HTMLResourcesBock_list);
            $tempList = str_replace('@IMAGE_URL', _get_resource_icon_url($resource->type), $tempList);
        }
    }
    $output = str_replace('@LIST_PLACEHOLDER', $tempList, $output);

    return $output;
}

function ntlp_recent_additions_resource_contents_by_category() {
    //Getting Query String From URL
    $category_id = 0;
    $url = $_GET['q'];
    $args = explode('/', $url);
    for ($i = 0; $i < count($args); $i++) {
        if (is_numeric($args[$i]) && empty($category_id)) {
            $category_id = $args[$i];
            break;
        }
    }
    return ntlp_recent_additions_resource_contents($category_id);
}

function ntlp_recent_additions_resource_contents($category = '') {
//    if ($school_nid == 0) {
//        //Getting Query String From URL
//        $resource_nid = 0;
//        $school_nid = 0;
//        $url = $_GET['q'];
//        $args = explode('/', $url);
//        for ($i = 0; $i < count($args); $i++) {
//            if (is_numeric($args[$i]) && empty($school_nid))
//                $school_nid = $args[$i];
//            elseif (is_numeric($args[$i]) && empty($resource_nid))
//                $resource_nid = $args[$i];
//        }
//    }
    include 'ntlp_doc_lib_resource.template.inc';

    $output = $HTMLResourcesBock;
    $output = str_replace('@BLOCK_HEADING', 'Recent Additions', $output);
    $tempList = '';
    $school_nid = get_this_school();

    $filters = array('WITH_TITLE' => true, 'ORDER' => array('FIELD' => 'created', 'SORT' => 'DESC'));
    if ($category) {
        $filters = array_merge($filters, array('BY_CATEGORY' => $category));
//        $school_nid = false;
    }

    foreach (get_resource_details($school_nid, $filters) as $resource) {
        if (is_user_access_this_category($school_nid, $resource->name)) {
            $tempList .= str_replace('@TITLE', l($resource->title, 'ntlp/document/library/resources/view/' . $resource->nid . '/' . $resource->category_tid), $HTMLResourcesBock_list);
            $tempList = str_replace('@IMAGE_URL', _get_resource_icon_url($resource->type), $tempList);
        }
    }
    $output = str_replace('@LIST_PLACEHOLDER', $tempList, $output);

    return $output;
}

function ntlp_total_resource_contents() {
    include 'ntlp_doc_lib_resource.template.inc';

    $output = $HTMLTotalResourcesBock;
    $output = str_replace('@BLOCK_HEADING', 'items', $output);
    $school_nid = get_this_school();

    if ($resource = get_resource_details($school_nid, array('COUNT' => true))) {
        $output = str_replace('@TOTAL', $resource->total, $output);
    } else {
        $output = str_replace('@TOTAL', 0, $output);
    }

    return $output;
}

function ntlp_resource_contents_category_keyword() {
    if ($category_id == 0) {
        //Getting Query String From URL
        $resource_nid = 0;
        $category_id = 0;
        $url = $_GET['q'];
        $args = explode('/', $url);
        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i]) && empty($category_id))
                $category_id = $args[$i];
            elseif (is_numeric($args[$i]) && empty($resource_nid))
                $resource_nid = $args[$i];
        }
    }

    $resource_node = node_load($resource_nid);

    //Get Keywords and Subjects
    $node_related_taxo = taxonomy_node_get_terms($resource_node);

    $library_cate_Obj = _get_lib_vocabulary_vid("NTLP Library Category");
    $ntlp_post_Obj = _get_lib_vocabulary_vid("NTLP Posts");

    foreach ($node_related_taxo as $Obj) {
        if ($Obj->vid == $library_cate_Obj->vid) {
            $category_array[] = $Obj;
        }
        if ($Obj->vid == $ntlp_post_Obj->vid) {
            $keywords_array[] = $Obj;
        }
    }

    include 'ntlp_doc_lib_resource.template.inc';
    $output = $HTMLResourcesBockCategoryKeyword;
    $output = str_replace('@BLOCK_HEADING', 'Categories', $output);
    $tempList = '';

    if ($category_array != null) {
        foreach ($category_array as $subject) {
            $tempList .= $HTMLResourcesBockCategoryKeyword_list;
            $tempList = str_replace('@TITLE', '<a href="' . url('ntlp/document/library/search/' . $subject->tid) . '">' . $subject->name . '</a> ', $tempList);
        }
    }
    $output = str_replace('@LIST_PLACEHOLDER', $tempList, $output);

    $output .= $HTMLResourcesBockCategoryKeyword;
    $output = str_replace('@BLOCK_HEADING', 'Keywords', $output);
    $tempList = '';

    if ($keywords_array != null) {
        foreach ($keywords_array as $keyword) {
            $tempList .= $HTMLResourcesBockCategoryKeyword_list;
            $tempList = str_replace('@TITLE', '<a href="' . url('taxonomy/term/' . $keyword->tid) . '">' . $keyword->name . '</a> ', $tempList);
        }
    }
    $output = str_replace('@LIST_PLACEHOLDER', $tempList, $output);

    return $output;
}

function _get_resource_icon_url($type) {
    global $base_path;

    $icon = '';
    switch ($type) {
        case 'lo_document':
            $icon = 'img_document.png';
            break;
        case 'lo_image':
            $icon = 'img_picture.png';
            break;
        case 'lo_link';
            $icon = 'img_link.png';
            break;
        case 'lo_page':
            $icon = 'img_page.png';
            break;
        case 'lo_video':
            $icon = 'img_video.png';
            break;
    }

    return $base_path . "themes/Boldr/Images/resourceStuff/" . $icon;
}

function ntlp_resource_filter_block() {
    drupal_add_js(drupal_get_path('module', 'ntlp_document_library') . '/ntlp_document_library.js');

    include 'ntlp_doc_lib_resource.template.inc';

    $cat = (int) (arg(4) ? arg(4) : 0);
    $keys = $_GET['keys'];

    $output = $HTMLResourcesFilter;

    $school_nid = get_this_school();
//    $selectBoxCategory = '<select onchange="window.location=\'?q=ntlp/document/library/resource/search/\'+this.value;"><option value="0">Select Category</option>';
    $cats = array();
    $cats["All"] = 0;
    foreach (_school_resourse_terms($school_nid) as $category) {
        if (is_user_access_this_category($school_nid, $category->name)) {
            $cats["$category->name"] = $category->tid;
        }
    }
//    $selectBoxCategory .= '</select>';
    //FILTER START
    $filterOpts = array(
        'Category' => array(
            'Category' => array(
                'type' => 'combobox',
                'options' => $cats,
                'value' => 'category',
                'name' => 0,
                'default' => isset($cat) ? $cat : '0',
                'reload_page' => "?q=ntlp/document/library/search/@VALUE",
                'style' => 'width:99%',
            ),
        ),
    );
    $fopts = render_custom_filter(
            $filterOpts, array('start' => '', 'end' => ''), array('template' => '', 'placeholder' => ''), '', '?q=ntlp/document/library/filter/callback/' . $cat . '/1&keys=' . $keys, 'library_document_table_view'
    );
    $output = str_replace('@CATEGORY_COMBO', $fopts, $output);

    $filterOpts = array(
        '#autoUncheckDefault' => false,
        '<b style="font-weight:bold;color:#3570AA">Resource Type</b>' => array(
            '#SelectAll' => true,
            'Link' => array(
                'type' => 'checkbox',
                'group' => 'type',
                'value' => 'link',
                'name' => 0,
                'default' => 1,
            ),
            'Images' => array(
                'type' => 'checkbox',
                'group' => 'type',
                'value' => 'images',
                'name' => 1,
            ),
            'Videos' => array(
                'type' => 'checkbox',
                'group' => 'type',
                'value' => 'videos',
                'name' => 2,
            ),
            'Documents' => array(
                'type' => 'checkbox',
                'group' => 'type',
                'value' => 'documents',
                'name' => 3,
            ),
            'Pages' => array(
                'type' => 'checkbox',
                'group' => 'type',
                'value' => 'pages',
                'name' => 4,
            ),
            'Hidden' => array(// Hidden checkbox is created so that when all checkboxes are
                'type' => 'checkbox', // unchecked or all are checked, then no value is passed in POST
                'group' => 'type',
                'value' => 'hidden',
                'name' => 99,
                'default' => 1,
                'style' => 'display:none',
            ),
        ),
    );

    $fopts = render_custom_filter(
            $filterOpts, array('start' => $HTMLResource_filter_cellStart, 'end' => $HTMLResource_filter_cellEnd), array('template' => $HTMLResource_filter_heading, 'placeholder' => '@HEADING'), $HTMLResource_filter_row_end, '?q=ntlp/document/library/filter/callback/' . $cat . '/1&keys=' . $keys, 'library_document_table_view'
    );
    $output = str_replace('@FILTER_PLACEHOLDER', $fopts, $output);


    $filterOpts = array(
        '#autoUncheckDefault' => false,
        '<b style="font-weight:bold;color:#3570AA">Other</b>' => array(
            'Featured Items Only' => array(
                'type' => 'checkbox',
                'group' => 'featured_resource',
                'value' => 'featured',
                'name' => 1,
                'select' => false,
            ),
        ),
    );

    $fopts = render_custom_filter(
            $filterOpts, array('start' => $HTMLResource_filter_cellStart, 'end' => $HTMLResource_filter_cellEnd), array('template' => $HTMLResource_filter_heading, 'placeholder' => '@HEADING'), $HTMLResource_filter_row_end, '?q=ntlp/document/library/filter/callback/' . $cat . '/1&keys=' . $keys, 'library_document_table_view'
    );
    $output = str_replace('@FILTER_OTHER_PLACEHOLDER', $fopts, $output);

    return $output;
}

function ntlp_category_resource_filter() {
    include 'document.template.inc';
    drupal_add_js(drupal_get_path('module', 'ntlp_document_library') . '/ntlp_document_library.js');

    return $HTMLCategoryResourceFilter;
}

function ntlp_document_library_filter_callback($catId = 0) {

    $typesTotal = array(
        'link' => 'lo_link',
        'images' => 'lo_image',
        'documents' => 'lo_document',
        'pages' => 'lo_page',
        'videos' => 'lo_video',
    );


    $types = array_keys($typesTotal);

    $selected_types = array();
    if (isset($_REQUEST['type'])) {
        foreach ($_REQUEST['type'] as $type) {
            if (in_array($type, $types)) {
                $selected_types[] = $typesTotal[$type];
            }
        }
    }

    $view = views_get_view('ntlp_documentlib_school_resource');

    if ($selected_types) {
        $view->display['default']->display_options['filters'] = array(
            'type' => array(
                'operator' => 'in',
                'value' => $selected_types,
                'table' => 'node',
                'field' => 'type',
            ),
        );
    }



    print $view->preview('default', array($catId)) . '
        <script type="text/javascript">
            fixDocumentSearchResultPager();
        </script>
    ';
}

function insert_doc_lib_google_records($resource_nid, $cat_tid, $fid, $type, $url) {

    if ($fid < 0) {
        $fid = abs($fid);
        if ($type != 'X') {

            db_query("INSERT INTO {ntlp_library_resource} (nid, category_tid, fid, type, path, deleted_by, deleted_on ) VALUES(
        %d, %d, %d, '%s', '%s' , 0, null)", $resource_nid, $cat_tid, $fid, $type, $url);

//            watchdog('google_records', 'course id '.$course_id. ' project id '.$project .' project section '.$project_section .' ');

            if ($project > 0) {
                $query = db_query("INSERT INTO {ntlp_library_resource} (nid, category_tid, fid, type, path, deleted_by, deleted_on ) VALUES(
                 %d, %d, %d, '%s', '%s' , 0, null)", $resource_nid, $cat_tid, $fid, $type, $url);
            }
        }
    } else {
        if ($type == 'X') {

            $query = db_query("INSERT INTO {ntlp_library_resource} (nid, category_tid, fid, type, path, deleted_by, deleted_on ) VALUES(
                 %d, %d, %d, '%s', '%s' , 0, null)", $resource_nid, $cat_tid, $fid, $type, $url);
        }
    }
}

function library_resource_delete_frm($lib_resource_nid) {
    return drupal_get_form('library_resource_delete_form', $lib_resource_nid);
}

function library_resource_delete_form($form_state = null, $lib_resource_nid) {

    modalframe_child_js();

    drupal_add_js(drupal_get_path('module', 'ntlp_document_library') . '/ntlp_document_library.js');

    drupal_set_title("Delete Resource");

    $form['resource_delete'] = array(
        '#type' => 'fieldset',
        '#tree' => TRUE, // Don't forget to set #tree!
    );

    $form['resource_delete']['message'] = array(
        '#type' => 'item',
        '#value' => 'You are about to permanently delete this resource. <p> Are you sure you want to permanently delete this resource?</p>',
        '#prefix' => '<div> ',
        '#suffix' => '</div>',
    );

    $option = '<span style="float: left;"><a class="SaveBtn" onclick="delete_resource_from_library(' . $lib_resource_nid . ')">Delete</a></span>';
    $form['resource_delete']['node_delete_button'] = array(
        '#type' => 'item',
        '#value' => $option . '<span style="float: left;padding-left: 2px;" ><a  class="SaveBtn" style="text-align:center;" onclick="parent.Drupal.modalFrame.close();">Cancel</a></span>',
        '#prefix' => '<div style="clear: both;float: left;padding-top: 10px;"> ',
        '#suffix' => '</div>',
    );

    return $form;
}

function library_resource_delete_data($resource_nid) {

    db_query("DELETE FROM dpl_ntlp_library_resource WHERE nid = %d ", $resource_nid);
    db_query("DELETE FROM dpl_term_node WHERE nid = %d ", $resource_nid);
}

function get_google_docs_frm_for_library() {
    modalframe_child_js();
    return drupal_get_form('google_docs_attachment_for_library_from');
}

function google_docs_attachment_for_library_from() {
    global $base_path;
    drupal_add_js(drupal_get_path('module', 'ntlp_document_library') . '/ntlp_document_library.js');

    drupal_set_title('Attach Google Doc');
    $path = $base_path;

    $form['googledocs'] = array(
        '#id' => 'searchp',
    );


    $form['googledocs']['search_docs'] = array(
        '#type' => 'item',
        '#value' => '<div id="googledocs">' . get_google_docs(false) . '</div>',
    );

    $form['googledocs']['submit'] = array(
        '#type' => 'item',
        '#value' => '<a onmousedown=this.className="PostBtnClicked" onmouseup=this.className="PostBtn" class="PostBtn" style="text-align:center;" onclick="attach_google_docs(); parent.Drupal.modalFrame.close();"> Submit</a>',
        '#prefix' => '<div style="float:left;width:80px;padding-top:10px">',
        '#suffix' => '</div>',
    );

    return $form;
}
