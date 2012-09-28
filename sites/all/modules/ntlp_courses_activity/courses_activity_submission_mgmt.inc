<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_school', 'data_access');

function ntlp_activity_basic_submission_frm($course_nid, $activity_nid) {
    return drupal_get_form('ntlp_activity_basic_submission_from', $course_nid, $activity_nid);
}

function ntlp_activity_basic_submission_from($form_state = null, $course_nid, $activity_nid) {

    global $base_path, $user;

    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_announcement') . '/ntlp_announcement.js', 'module');
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/ntlp_courses_activity.js');

    require_once('activity_submission_template.inc');

    $user_role = check_user_permission($course_nid);

    if (!$form_state['values']) {
        set_item_url_handler('Activity Submission');
    }

    if ($user_role != NTLP_ROLEID_STUDENT) {

        drupal_set_message('You should enrolled into Activity to make submission');
        drupal_goto('ntlp/goback');
    } else {
        if (!is_student_access_this_activity($user->uid, $activity_nid)) {
            drupal_goto('ntlp/render/error/page/Sorry, you are not authorized to access this Activity', array('destination' => $_GET['q']));
        }
    }


    $activity_obj = get_activity_records($activity_nid);
    /* check some rules to get if student is not hacking this submission url */
    $submission_result = get_user_submission($activity_obj->school_term_tid, $activity_nid, $user->uid, true);
    $submitted_record = db_fetch_object($submission_result);
    if (!is_submission_allowed_for_student($activity_obj, $user->uid, $submitted_record->is_final, $submitted_record->date_opened)) {
        drupal_goto('ntlp/courses/activity/view/' . $activity_obj->course_nid . "/" . $activity_obj->nid, array('tab' => 'work'));
    }


    $form['main'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '',
        '#suffix' => '',
    );

    $form['main']['activity_name'] = array(
        '#type' => 'item',
        '#value' => $activity_obj->title,
        '#prefix' => '<div class="Activity_Detail_heading_ph">
	<div class="left">
    	<h1>',
        '#suffix' => '</h1>
    </div>
</div><br clear="all" />',
    );

    if ($activity_obj->project_nid != 0 && !empty($activity_obj->project_nid)) {
        $show_project = '<a href="' . url('ntlp/courses/projects/' . $activity_obj->course_nid . '/' . $activity_obj->project_nid) . '">' . _get_node_title($activity_obj->project_nid) . '</a>';
    } else {
        $show_project = '';
    }

    if ($activity_obj->available_from == '0000-00-00 00:00:00' && $activity_obj->available_to == '0000-00-00 00:00:00') {
        $activity_availability = 'Hide';
    } else {
        $activity_availability = 'Show';
    }

    if ($user_role != NTLP_ROLEID_STUDENT && $user_role != NTLP_ROLEID_PARENT && $user_role != NTLP_ROLEID_SITEGUEST) {
        $show_view = ' <td style="width:30px;  font-weigh: bold;">View:</th>
                    <td style="width:30px">' . $activity_availability . '</td>';
    } else {

        $show_view = '';
    }
    
    if ($activity_obj->due_date != '0000-00-00 00:00:00') {
        $activity_due_date = get_tz_course($activity_obj->course_nid, "D m/d/Y @ g:i a", $activity_obj->due_date);
    } else {
        $activity_due_date = 'Fix';
    }
    $activity_summary_bar = '
                <td style="width:55px; text-align:left; font-weigh: bold;">Due Date:</th>
                    <td style="width:160px; text-align:left;">' . $activity_due_date . ' </td>
                <td style="width:30px; font-weigh: bold; ">Type:</th>
                  <td style="width:80px; text-align:left;">' . get_activity_name($activity_obj->type) . '</td>
                <td style="width:55px;  font-weigh: bold;">' . (!empty($show_project) ? "Project:" : "") . '</th>
                    <td style="white-space: nowrap; width:160px; overflow: hidden; text-overflow: ellipsis !important; "> ' . $show_project . '</td>' . $show_view;

    $form['main']['activity_short_detail'] = array(
        '#type' => 'item',
        '#value' => '<tr> ' . $activity_summary_bar . '</tr>',
        '#prefix' => $HTMLform_activity_title_hdr . '<table width="100%" cellspacing="0" cellpadding="5" border="0" class="infotable" style="table-layout: fixed; border-bottom: medium none ! important;">',
        '#suffix' => '</table>' . $HTMLform_activity_title_ftr,
    );


    // activity outcome
    if ($activity_obj->is_graded != 0) {
        $outcome_array = get_activity_outcome($activity_nid);
        if ($outcome_array != null) {
            $count = 1;
            foreach ($outcome_array as $outcome_obj) {

                $activity_outcome .= '<li>' . $outcome_obj->name . ' (' . $outcome_obj->max_points . ' pts)</li>';
                $count++;
            }
        }
    } else {
        $activity_outcome = 'This Activity is not Graded';
    }


    // activity_status
    $OVERVIEW_ONCLICK = 'window.location = \'?q=ntlp/courses/activity/view/' . $course_nid . '/' . $activity_nid . '&tab=overview\';';
    $OVERVIEW_CLASS = '';

    $WORK_ONCLICK_ = 'window.location = \'?q=ntlp/courses/activity/view/' . $course_nid . '/' . $activity_nid . '&tab=work\';';
    $WORK_CLASS = 'class= "GradesTabActive"';


    $activity_tab_panel = $HTML_ACTIVITY_TAB_PANEL;
    $activity_tab_panel = str_replace('@OVERVIEW_CLASS', $OVERVIEW_CLASS, $activity_tab_panel);
    $activity_tab_panel = str_replace('@OVERVIEW', $OVERVIEW_ONCLICK, $activity_tab_panel);
    $activity_tab_panel = str_replace('@WORK_CLASS', $WORK_CLASS, $activity_tab_panel);
    $activity_tab_panel = str_replace('@WORK', $WORK_ONCLICK, $activity_tab_panel);
    $activity_tab_panel = str_replace('@GRADE_PUBLISHED_DATETIME_BUTTON', '', $activity_tab_panel);


    $form['main']['form'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#prefix' => str_replace('@ACTIVITY_TAB_PANEL', $activity_tab_panel, $HTML_activity_submission_edit_basic_panel_START),
        '#suffix' => $HTML_activity_submission_edit_basic_panel_END,
    );

    if ($activity_obj->type == 'lo_journal') {
        ntlp_activity_journal_edit_submission($form_state, $form['main']['form'], $activity_obj, $form);
    } else if ($activity_obj->type == 'lo_task' || $activity_obj->type == 'lo_workshop') {
        ntlp_activity_task_and_work_edit_submission($form_state, $form['main']['form'], $activity_obj, $form);
    }
    return $form;
}

function ntlp_activity_task_and_work_edit_submission($form_state, &$form, $activity_obj, &$root_form) {
    global $user;

    $user_uid = $user->uid;

    include('activity_submission_template.inc');


    if ($activity_obj->submission_type == 'O') {

        drupal_set_message('This Activity is marked as "Offline" you cannot submit it online');
        drupal_goto('ntlp/goback');
    }

    $submission_result = get_user_submission($activity_obj->school_term_tid, $activity_obj->nid, $user_uid);

    if ($submission_result->num_rows > 0) {
        $act_submitted_Obj = db_fetch_object($submission_result);
    }

    if ($activity_obj->allow_resubmission == 0) {
        $activity_message = '<h2 style="font-weight: normal;margin:0px;padding:0px;font-size:12px" class="teacherComment">You can submit <b>only once</b>.</h2>';
    } else if ($activity_obj->allow_resubmission == 1) {
        $activity_message = '<h2 style="font-weight: normal;margin:0px;padding:0px;font-size:12px" class="teacherComment">You can submit <b>more than once</b>.</h2>';
    } else if ($activity_obj->submission_type == 'F') {
        $activity_message = '<h2 style="font-weight: normal;margin:0px;padding:0px;font-size:12px" class="teacherComment">Work is completed offline. <b> No submission required. </b>.</h2>';
    }
    $form['act_submission_nid'] = array(
        '#type' => 'hidden',
        '#value' => $act_submitted_Obj->nid,
    );

    $form['activity_obj'] = array(
        '#type' => 'hidden',
        '#value' => serialize($activity_obj),
    );

    $form['act_sub_version_num'] = array(
        '#type' => 'hidden',
        '#value' => ($act_submitted_Obj->version_num != null) ? $act_submitted_Obj->version_num : 0,
    );

    $form['act_sub_date_opened'] = array(
        '#type' => 'hidden',
        '#value' => $act_submitted_Obj->date_opened,
    );

    $form['act_journal_status'] = array(
        '#type' => 'item',
        '#value' => t($activity_message),
        '#prefix' => '<tr><td valign="top" style="width:65%"><div style="float: left; color: rgb(153, 153, 153);padding-top:6px">',
        '#suffix' => '</div>',
    );


    $form['act_journal_save'] = array(
        '#type' => 'submit',
        '#value' => t('Save Draft'),
        '#submit' => array('activity_submission_save'),
        '#attributes' => array('class' => 'SaveDraftBtn', 'style' => 'float:right;'),
        '#prefix' => '<div style="float: right;padding-left: 5px;"><table cellpadding="0" cellspacing="0"><tr><td style="padding-top:4px;">',
        '#suffix' => '</td><td>&nbsp;</td>',
    );


    $form['act_journal_save_submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save & Submit'),
        '#submit' => array('activity_submission_save'),
        '#attributes' => array('class' => 'AddOutComesBtn', 'onmousedown' => "this.className='AddOutComesBtnClicked'", 'onmouseup' => "this.className='AddOutComesBtn'"),
        '#prefix' => '<td>',
        '#suffix' => '</td></tr></table></div><br clear="all">',
    );


    $form['act_journal'] = array(
//        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '',
        '#suffix' => '',
    );

    $form['act_journal']['student_work'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => $HTML_activity_submission_edit_student_panel_START,
        '#suffix' => $HTML_activity_submission_edit_student_panel_END,
    );


    $activity_submitted_status = get_activity_submission_and_save_status($act_submitted_Obj);


    $form['act_journal']['student_work']['heading'] = array(
        '#type' => 'item',
        '#value' => '<h1 style="font-size:12px; float:left;">Student Work </h1>' . $activity_submitted_status,
        '#prefix' => '<div class="activity_detail_section"><table cellpadding="4" class="activity_details"><tr style="background-color: #f1dcdb; margin-bottom:10px;"><td>',
        '#suffix' => '</td></tr>',
    );

    $form['act_journal']['student_work']['work'] = array(
        '#type' => 'textarea',
        '#default_value' => $act_submitted_Obj->body,
        '#prefix' => '<tr><td><div style="width: 520px;">',
        '#suffix' => '</div><br clear="all"></td></tr>',
    );

//    $attach_fileupload_link = dlg('+ Attach file', 'ntlp/image/uploader/edit', 275, 425);
    $attach_fileupload_link = '<span style="float:left;color:#000">+&nbsp;</span><a style="float:left;text-decoration:none;color: #3399CC;" id="attach_link">Attach file</a>';
    $attach_googledoc_link = '<span style="color:#000">+&nbsp;</span>'.dlg("Link Google doc", 'ntlp/courses/activity/submission/attachment/G', 300, 600);

    $root_form['#fileupload_embed'] = true;
    $root_form['#fileupload_auto_upload'] = true;

    $form['act_journal']['student_work']['upload_control'] = array(
        '#prefix' => '<tr><td><div><table border="0" width="100%"><tr id="attach_link_tr"><td><div style="float:left;"><h2 class="Activity_Detail-headings">Attachments:</h2></div></td>
            <td onmouseover="place_fileupload_control(\'edit-main-form-act-journal-student-work-upload-control\', this);" onmouseout="place_fileupload_control_mouseout(this);">' . $attach_fileupload_link,
        '#suffix' => '</td>
            <td><div style="float:left;width:40px">&nbsp;</div><div style="float:left; ">' . $attach_googledoc_link . '</div></td></tr></table></div></td></tr>',
    );

    fileupload_get_control($root_form, $form_state, array('main', 'form', 'act_journal', 'student_work', 'upload_control'), '', true);

    if ($act_submitted_Obj != false && $act_submitted_Obj->nid > 0) {

        $obj = get_attachment($act_submitted_Obj->nid, 'ntlp_submission_attachment', 'submission_nid');
        if ($obj != null) {
            foreach ($obj as $attchmentObj) {
                if ($attchmentObj->type == 'G') {
                    $hiddendata .= 'G,' . $attchmentObj->id . ',' . $attchmentObj->path . ';';
                    $file_name = ($attchmentObj->attachment_name != "" ? $attchmentObj->attachment_name : $attchmentObj->path) ;
                    $divdata .= '<div id="G_' . $attchmentObj->id . '">'.
                        '<img style="position:relative;top:2px;margin-right:4px;" onclick="detele_attach(\'G_'.$attchmentObj->id. '\');" width="14" height="14" src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png" border="0" />'.
                        '<a href="' . $attchmentObj->path . '" target="_blank">' . $file_name . '</a></div>';
                } elseif ($attchmentObj->type == 'L') {
                    $hiddendata .= 'L,' . $attchmentObj->id . ',' . $attchmentObj->path . ';';
                    $divdata .= '<div id="L_' . $attchmentObj->id . '">'.
                        '<img style="position:relative;top:2px;margin-right:4px;" onclick="detele_attach(\'L_'.$attchmentObj->id. '\');" width="14" height="14" src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png" border="0" />'.
                        '<a href="' . $attchmentObj->path . '" target="_blank">' . $attchmentObj->path . '</a></div>';
                } elseif ($attchmentObj->type == 'F') {
                    $hiddendata .= 'F,' . $attchmentObj->fid . ',' . $attchmentObj->path . ';';
                    $file_path = '';
                    if ($file_obj = _get_dpl_files_obj($attchmentObj->fid)) {
                        $file_path = $file_obj->filepath;
//                        $file_image = check_and_the_file_type_image($file_obj->filemime);
                    }
                    $divdata .= '<div id="F_' . $attchmentObj->fid . '">'.
                        '<img style="position:relative;top:2px;margin-right:4px;" onclick="detele_attach(\'F_'.$attchmentObj->fid. '\');" width="14" height="14" src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png" border="0" />'.
                        '<a href="' . $base_path . $file_path . '" target="_blank">' . $attchmentObj->path . '</a></div>';
                    $fids .= ',' . $attchmentObj->fid;
                }
            }
        }
    }

    $form['act_journal']['student_work']['attached_files_hidden'] = array(
        '#type' => 'hidden',
        '#value' => $hiddendata,
        '#id' => 'attachted_files_hidden',
    );


    $form['act_journal']['student_work']['attached_files'] = array(
        '#type' => 'markup',
        '#value' => '<div id="urllist">' . $divdata . '</div>',
        '#prefix' => '<tr><td><div>',
        '#suffix' => '</div></td></tr>',
    );

    $form['act_journal']['student_work']['comment_notes'] = array(
        '#type' => 'textarea',
        '#default_value' => $act_submitted_Obj->note_to_teacher,
        '#wysiwyg' => FALSE,
        '#prefix' => '<tr><td><div style="font-size: 5px;">&nbsp;</div><div><h2 class="Activity_Detail-headings">Comments/Notes:</h2>',
        '#suffix' => '</div></td></tr></table></div>',
        '#attributes' => array('style' => 'width:514px'),
    );

    $form['blank'] = array(
        '#type' => 'item',
        '#value' => '<td style="width: 10px;">&nbsp;</td>',
    );


    $form['grade'] = array(
        '#type' => 'item',
        '#value' => get_user_graded_outcome($activity_obj),
        '#prefix' => '<td valign="top" style="width:35%">',
        '#suffix' => '</td>',
    );
}

function ntlp_activity_journal_edit_submission($form_state, &$form, $activity_obj, &$root_form) {
    global $user;

    $user_uid = $user->uid;

    include('activity_submission_template.inc');

    $submission_result = get_user_submission($activity_obj->school_term_tid, $activity_obj->nid, $user_uid);

    if ($submission_result->num_rows > 0) {
        $act_submitted_Obj = db_fetch_object($submission_result);
    }
    

    // activity attched files
    $obj = get_attachment($activity_obj->nid, 'ntlp_activity_attachment', 'activity_nid');

    if ($obj != null) {
//            $divdata = '<table>';
        foreach ($obj as $attchmentObj) {

            if ($attchmentObj->type == 'G') {
                $activity_attched_doc .= '<div id="G_' . $attchmentObj->id . '"> <a href=' . $attchmentObj->path . ' >' . $attchmentObj->path . '</a></div>';
            } elseif ($attchmentObj->type == 'L') {
                $activity_attched_doc .= '<div id="L_' . $attchmentObj->id . '"> <a href=' . $attchmentObj->path . ' >' . $attchmentObj->path . '</a></div>';
            } elseif ($attchmentObj->type == 'F') {

                $file_id = get_attachment_filepath($activity_obj->nid, $attchmentObj->id);


                if ($file_obj = _get_dpl_files_obj($file_id->fid)) {
                    $picture = $file_obj->filepath;
                    $file_image = check_and_the_file_type_image($file_obj->filemime,$file_obj->filepath);
                }
                $activity_attched_doc .= '<li> <span><img src=' . $base_path . 'themes/Boldr/Images/resourceStuff/' . $file_image . '.png border="0" /><a href="' . $picture . '" target="_blank">' . $file_obj->filename . '</a></span> </li>';
            }
        }
    }

    if (isset($activity_attched_doc) && !empty($activity_attched_doc)) {
        $acitivity_attachment = '<div style="font-size: 5px;">&nbsp;</div>
            <div style="float:left;"><h2 class="Activity_Detail-headings" style="margin-left:12px;">Attachments:</h2></div>
        <ul class="Activity_Detail-list" style="margin-left:11px;">' .
                $activity_attched_doc
                . '</ul>';
    } else {
        $acitivity_attachment = '';
    }

    if (!empty($activity_obj->body)) {

        $description_block = str_replace('@JOURNAL_DESCRIPTION', $activity_obj->body, $HTML_SUBMISSION_EDIT_JOURNAL_DESCRIPTION_BLOCK);

        if (!empty($activity_attachment)) {
            $description_block = str_replace('@JOURNAL_ATTACHMENT_BLOCK', $acitivity_attachment, $description_block);
        } else {
            $description_block = str_replace('@JOURNAL_ATTACHMENT_BLOCK', '', $description_block);
        }
    } else if (!empty($acitivity_attachment)) {

        $description_block = str_replace('@JOURNAL_DESCRIPTION', '', $HTML_SUBMISSION_EDIT_JOURNAL_DESCRIPTION_BLOCK);
        $description_block = str_replace('@JOURNAL_ATTACHMENT_BLOCK', $acitivity_attachment, $description_block);
    } else {
        $description_block = '';
    }

    $form['act_journal_desc'] = array(
        '#type' => 'item',
        '#value' => $description_block,
        '#prefix' => '<tr><td valign="top" style="width:65%">',
        '#suffix' => '<div style="font-size: 5px;">&nbsp;</div>',
    );


    if ($activity_obj->allow_resubmission == 0) {
        $activity_message = '<h2 style="font-weight: normal;font-size:12px !important" class="teacherComment">You can submit <b>only once</b>.</h2>';
    } else if ($activity_obj->allow_resubmission == 1) {
        $activity_message = '<h2 style="font-weight: normal;font-size:12px !important" class="teacherComment">You can submit <b>more than once</b>.</h2>';
    } else if ($activity_obj->submission_type == 'F') {
        $activity_message = '<h2 style="font-weight: normal;font-size:12px !important" class="teacherComment">Work is completed offline. <b> No submission required. </b>.</h2>';
    }

    $form['act_journal_status'] = array(
        '#type' => 'item',
        '#value' => t($activity_message),
        '#prefix' => '<div style="float: left; color: rgb(153, 153, 153); padding-top: 5px;">',
        '#suffix' => '</div>',
    );

    $form['act_submission_nid'] = array(
        '#type' => 'hidden',
        '#value' => $act_submitted_Obj->nid,
    );

    $form['activity_obj'] = array(
        '#type' => 'hidden',
        '#value' => serialize($activity_obj),
    );

    $form['act_sub_version_num'] = array(
        '#type' => 'hidden',
        '#value' => ($act_submitted_Obj->version_num != null) ? $act_submitted_Obj->version_num : 0,
    );

    $form['act_sub_date_opened'] = array(
        '#type' => 'hidden',
        '#value' => $act_submitted_Obj->date_opened,
    );

    $form['act_journal_save'] = array(
        '#type' => 'submit',
        '#value' => t('Save Draft'),
        '#submit' => array('activity_submission_save'),
        '#attributes' => array('class' => 'SaveDraftBtn', 'style' => 'float:right;'),
        '#prefix' => '<div style="float: right;padding-left: 5px;"><table cellpadding="0" cellspacing="0"><tr><td style="padding-top:5px;">',
        '#suffix' => '</td><td>&nbsp;</td>',
    );

    $form['act_journal_save_submit'] = array(
        '#type' => 'submit',
        '#value' => t('Save & Submit'),
        '#submit' => array('activity_submission_save'),
        '#attributes' => array('class' => 'AddOutComesBtn', 'onmousedown' => "this.className='AddOutComesBtnClicked'", 'onmouseup' => "this.className='AddOutComesBtn'"),
        '#prefix' => '<td>',
        '#suffix' => '</td></tr></table></div><br clear="all">',
    );

    $form['act_journal'] = array(
//        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '',
        '#suffix' => '',
    );

    $form['act_journal']['student_work'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => $HTML_activity_submission_edit_student_panel_START,
        '#suffix' => $HTML_activity_submission_edit_student_panel_END,
    );

    $activity_submitted_status = get_activity_submission_and_save_status($act_submitted_Obj);

    $form['act_journal']['student_work']['heading'] = array(
        '#type' => 'item',
        '#value' => '<h1 style=""><div style="font-size:12px; float:left;">Student Work </div>' . $activity_submitted_status . '</h1>',
        '#prefix' => '<div class="activity_detail_section"><table cellpadding="4" class="activity_details"><tr style="background-color: #f1dcdb; margin-bottom:10px;"><td>',
        '#suffix' => '</td></tr>',
    );

    $form['act_journal']['student_work']['work'] = array(
        '#type' => 'textarea',
        '#default_value' => $act_submitted_Obj->body,
        '#size' => '5',
        '#prefix' => '<tr><td><div style="width: 520px;">',
        '#suffix' => '</div><br clear="all"></td></tr>',
    );

    $attach_fileupload_link = '<span style="float:left;color:#000">+&nbsp;</span><a style="float:left;text-decoration:none;color: #3399CC;" id="attach_link">Attach file</a>';
    $attach_googledoc_link = '<span style="color:#000">+&nbsp;</span>'.dlg("Link Google doc", 'ntlp/courses/activity/submission/attachment/G', 300, 600);

    $root_form['#fileupload_embed'] = true;
    $root_form['#fileupload_auto_upload'] = true;

    $form['act_journal']['student_work']['upload_control'] = array(
        '#prefix' => '<tr><td><div><table border="0" width="100%"><tr><td><div style="float:left;"><h2 class="Activity_Detail-headings">Attachments:</h2></div></td>
            <td onmouseover="place_fileupload_control(\'edit-main-form-act-journal-student-work-upload-control\', this);" onmouseout="place_fileupload_control_mouseout(this);">' . $attach_fileupload_link,
        '#suffix' => '</td>
            <td><div style="float:left;width:40px">&nbsp;</div><div style="float:left;">' . $attach_googledoc_link . '</div></td></tr></table></div></td></tr>',
    );

    fileupload_get_control($root_form, $form_state, array('main', 'form', 'act_journal', 'student_work', 'upload_control'), '', true);

    if ($act_submitted_Obj != false && $act_submitted_Obj->nid > 0) {

        $obj = get_attachment($act_submitted_Obj->nid, 'ntlp_submission_attachment', 'submission_nid');
        if ($obj != null) {

            foreach ($obj as $attchmentObj) {
                if ($attchmentObj->type == 'G') {
                    $hiddendata .= 'G,' . $attchmentObj->id . ',' . $attchmentObj->path . ';';
                    $file_name = ($attchmentObj->attachment_name != "" ? $attchmentObj->attachment_name : $attchmentObj->path) ;
                    $divdata .= '<div id="G_' . $attchmentObj->id . '">'.
                        '<img style="position:relative;top:2px;margin-right:4px;" onclick="detele_attach(\'G_'.$attchmentObj->id. '\');" width="14" height="14" src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png" border="0" />'.
                        '<a href="' . $attchmentObj->path . '" target="_blank">' . $file_name . '</a></div>';
                } elseif ($attchmentObj->type == 'L') {
                    $hiddendata .= 'L,' . $attchmentObj->id . ',' . $attchmentObj->path . ';';
                    $divdata .= '<div id="L_' . $attchmentObj->id . '">'.
                        '<img style="position:relative;top:2px;margin-right:4px;" onclick="detele_attach(\'L_'.$attchmentObj->id. '\');" width="14" height="14" src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png" border="0" />'.
                        '<a href="' . $attchmentObj->path . '" target="_blank">' . $attchmentObj->path . '</a></div>';
                } elseif ($attchmentObj->type == 'F') {
                    $hiddendata .= 'F,' . $attchmentObj->fid . ',' . $attchmentObj->path . ';';
                    if ($file_obj = _get_dpl_files_obj($attchmentObj->fid)) {
                        $file_path = $file_obj->filepath;
                    }
                    $divdata .= '<div id="F_' . $attchmentObj->fid . '">'.
                        '<img style="position:relative;top:2px;margin-right:4px;" onclick="detele_attach(\'F_'.$attchmentObj->fid. '\');" width="14" height="14" src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png" border="0" />'.
                        '<a href="' . $base_path . $file_path . '" target="_blank">' . $attchmentObj->path . '</a></div>';
                    $fids .= ',' . $attchmentObj->fid;
                }
            }
        }
    }

    $form['act_journal']['student_work']['attached_files_hidden'] = array(
        '#type' => 'hidden',
        '#value' => $hiddendata,
        '#id' => 'attachted_files_hidden',
    );


    $form['act_journal']['student_work']['attached_files'] = array(
        '#type' => 'markup',
        '#value' => '<div id="urllist">' . $divdata . '</div>',
        '#prefix' => '<tr><td><div>',
        '#suffix' => '</div></td></tr>',
    );

    $form['act_journal']['student_work']['comment_notes'] = array(
        '#type' => 'textarea',
        '#default_value' => $act_submitted_Obj->note_to_teacher,
        '#wysiwyg' => FALSE,
        '#prefix' => '<tr><td><div><h2 class="Activity_Detail-headings">Comments/Notes:</h2>',
        '#suffix' => '</div></td></tr></table></div>',
        '#attributes' => array('style' => 'width:514px'),
    );

    $form['blank'] = array(
        '#type' => 'item',
        '#value' => '<td style="width: 10px;">&nbsp;</td>',
    );


    $form['grade'] = array(
        '#type' => 'item',
        '#value' => get_user_graded_outcome($activity_obj),
        '#prefix' => '<td valign="top" style="width:35%;">',
        '#suffix' => '</td>',
    );
}

function activity_submission_save($form, $form_state) {
    global $user;

    $fs = $form_state['clicked_button']['#post'];

    $operation = $fs['op'];
    $student_works = $fs['main']['form']['act_journal']['student_work']['work'];
    $note_to_teacher = $fs['main']['form']['act_journal']['student_work']['comment_notes'];
    $activity_obj = unserialize($fs['main']['form']['activity_obj']);

    /** when student press save then create another submission record to
     *  save a submission as a draft with the increment of version_num field
     * */
    $previous_vers = $fs['main']['form']['act_sub_version_num'];

    $act_submission_nid = $fs['main']['form']['act_submission_nid'];
    $attachement_data = $fs['main']['form']['act_journal']['student_work']['attached_files_hidden'];

    /* Saving a student submission open date if its already set then using same */
    if (isset($fs['work']['main']['form']['act_sub_date_opened'])
            && !empty($fs['work']['main']['form']['act_sub_date_opened'])) {
        $act_sub_date_opened = $fs['work']['main']['form']['act_sub_date_opened'];
    } else {
        $act_sub_date_opened = strftime(DATE_FORMAT, time());
    }


    /* generate a array of attached files */
    $submission_attachement = get_links($attachement_data);


    // $operation is holding a button state which button is pressed
    if ($operation != 'Save Draft') {

        $major_version = substr($previous_vers, 0, strpos($previous_vers, '.'));
        $major_version = $major_version + 1; // major only increment on Save & Submit button

        $minor_version = substr($previous_vers, strpos($previous_vers, '.') + 1, strlen($previous_vers));
//        $minor_version = $minor_version + 1; // major only increment on Save button
        $minor_version = str_pad($minor_version, 4, "0", STR_PAD_LEFT);

        $current_version = $major_version . '.' . $minor_version;

        $sumbmission_node = create_activity_submission(
                $activity_obj->school_term_tid, $activity_obj->course_nid, $activity_obj->nid, 
                $activity_obj->type . '_submission', $student_works, $user->uid, $note_to_teacher, 1, $current_version, true);


        // check late sumbmission
        if ($activity_obj->due_date >= date(DATE_FORMAT_CHECK, time())) {
            $is_late = 0;
        } else {
            $is_late = 1;
        }

        if ($is_late == 1 || $activity_obj->grading_method == 'C') {
            $grade_id = save_activity_user_grade(intval($activity_obj->nid), intval($user->uid), null, $activity_obj->created_by, 1, $is_late);


        // if activity is auto graded then grade maximum score to the user
        if ($activity_obj->grading_method == 'C') {
            $outcome_array = get_activity_outcome($activity_obj->nid);
                auto_grade_maximum_to_user_submission($grade_id, $outcome_array);
            }
        }

        ntlpsubmissionactivity_submit('submit', intval($activity_obj->nid), -1, $user->uid);

    } else {

        $major_version = substr($previous_vers, 0, strpos($previous_vers, '.'));
//        $major_version = $major_version + 1; // major only increment on Save & Submit button

        $minor_version = substr($previous_vers, strpos($previous_vers, '.') + 1, strlen($previous_vers));
        $minor_version = $minor_version + 1; // major only increment on Save button
        $minor_version = str_pad($minor_version, 4, "0", STR_PAD_LEFT);

        $current_version = $major_version . '.' . $minor_version;

        $sumbmission_node = create_activity_submission(
                $activity_obj->school_term_tid, $activity_obj->course_nid, $activity_obj->nid, 
                $activity_obj->type . '_submission', $student_works, $user->uid, $note_to_teacher, 0, $current_version, false);
    }
    /* Saving a user attached files with submission */
//    watchdog('sub_attach', print_r($submission_attachement, true));
//    echo '<br>';
//    print_r($attachement_data);
//    echo '<br>';
//    echo 'submission node '.$sumbmission_node->nid;
//    die();
    if ($attachement_data != null) {
        foreach ($submission_attachement as $l) {

            if (sizeOf($l) >= 4)
                list($type, $id, $name, $url) = $l;
            else
                list($type, $id, $url) = $l;

            if ($type == 'F' || $type == 'L' || $type == 'G' || $type == 'X' ) {
                insert_attachment_records($sumbmission_node->nid, $type, $url, $id, $name);
            }
        }
    }

    if ($operation != 'Save Draft') {
        drupal_goto('ntlp/courses/activity/view/' . $activity_obj->course_nid . "/" . $activity_obj->nid, array('tab' => 'work'));
    }
}

function insert_attachment_records($submission_nid, $type, $url, $id, $file_name="") {

//    if ($id < 0) {
    if ($type != 'X') {
        db_query("INSERT INTO {ntlp_submission_attachment} (submission_nid, type, attachment_name, path, is_deleted,fid )
                            VALUES(%d, '%s', '%s', '%s', %d, %d)", intval($submission_nid), check_plain($type), $file_name, check_url($url), 0, abs($id));
            }
//    } else {
//        if ($type == 'X') {
//            db_query("UPDATE {ntlp_submission_attachment} SET type = '%s', is_deleted = %d WHERE id = %d", check_url($type), 1, intval($id));
//        }
//    }
                }

function google_docs_attachment_form() {
    modalframe_child_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/ntlp_courses_activity.js');

    drupal_set_title('Attach Google Doc');
    $path = $base_path;

    $form['googledocs'] = array(
//            '#title' => t('Attach Google Doc '),
//        '#type' => 'fieldset',
        '#id' => 'searchp',
    );

//    $form['googledocs']['finder'] = array(
//        '#type' => 'textfield',
//        '#size' => '40',
//        '#id' => 'finder',
//        '#attributes' => array('onkeypress' => "google_docs_search();"),
//        '#prefix' => '<div>',
//        '#suffix' => '</div>',
//    );

    $form['googledocs']['search_docs'] = array(
        '#type' => 'item',
        '#value' => '<div id="googledocs" >' . get_google_docs(false) . '</div>',
    );

    $form['googledocs']['submit'] = array(
        '#type' => 'item',
        '#value' => '<a class="PostBtn" style="text-align:center;" onclick="attach_google_docs();parent.Drupal.modalFrame.close();"> Submit</a>',
        '#prefix' => '<div style="float:left;width:80px;margin-top:10px">',
        '#suffix' => '</div>',
    );

    return $form;
            }

function link_attachment_form() {
    modalframe_child_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/ntlp_courses_activity.js');

    $form['documentlink'] = array(
        '#title' => t('Document Link'),
        '#type' => 'fieldset',
    );

    $form['documentlink']['finder'] = array(
        '#title' => t('Document Link'),
        '#type' => 'textfield',
        '#size' => '40',
        '#id' => 'doc_link',
        '#value' => '',
    );

    $form['documentlink']['linkfield'] = array(
        '#type' => 'hidden',
        '#value' => '',
    );

    $form['documentlink']['submit'] = array(
        '#type' => 'item',
        '#value' => '<a href="#" onclick="atgoogledoc();"> Submit</a>',
    );

    return $form;
}
