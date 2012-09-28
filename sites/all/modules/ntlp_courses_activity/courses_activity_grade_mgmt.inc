<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_school', 'data_access');

function ntlp_basic_activity_grade_frm($course_nid, $activity_nid, $student_uid) {
    return drupal_get_form('ntlp_basic_activity_grade_from', $course_nid, $activity_nid, $student_uid);
}

function ntlp_basic_activity_grade_from($form_state = null, $course_nid, $activity_nid, $student_uid) {


    global $base_path, $user;

    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_announcement') . '/ntlp_announcement.js', 'module');
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/ntlp_courses_activity.js');

    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/rubric_activity.js');

    /* css */
    drupal_add_css(drupal_get_path('module', 'jquery_ui') . '/jquery.ui/themes/base/ui.all.css');

    require_once('activity_grade_template.inc');
    set_item_url_handler('Activity Submission');

    $user_role = check_user_permission($course_nid);

    if ($user_role == NTLP_ROLEID_STUDENT || $user_role == NTLP_ROLEID_PARENT) {

        drupal_goto('ntlp/render/error/page/Sorry, you are not authorized to access this Page. Please Contact with Site Administrator', array('destination' => $_GET['q']));
    }

    $edit_mode = ($user_role != NTLP_ROLEID_SITEGUEST && is_user_enrolled($course_nid, $student_uid));

    $activity_obj = get_activity_records($activity_nid);

    $form['main'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '',
        '#suffix' => '',
    );

    $form['main']['student_uid'] = array(
        '#type' => 'hidden',
        '#value' => $student_uid,
    );

    $form['main']['activity_name'] = array(
        '#type' => 'item',
        '#value' => $activity_obj->title,
        '#prefix' => '<div class="Activity_Detail_heading_ph"><div class="left"><h1>',
        '#suffix' => '</h1></div></div><br clear="all" />',
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
    
    if ($activity_obj->due_date != '0000-00-00 00:00:00') {
        $activity_due_date = get_tz_course($activity_obj->course_nid, "D m/d/Y @ g:i a", $activity_obj->due_date);
    } else {
        $activity_due_date = 'Fix';
    }
    
    $activity_summary_bar = '
                <td style="width:55px; text-align:left; font-weight: bold;">Due Date:</th>
                    <td style="width:160px; text-align:left;">' . $activity_due_date . ' </td>
                <td style="width:30px; font-weight: bold;">Type:</th>
                  <td style="width:80px; text-align:left;">' . get_activity_name($activity_obj->type) . '</td>
                <td style="width:55px; font-weight: bold;">' . (!empty($show_project) ? "Project:" : "") . '</th>
                    <td style="white-space: nowrap; width:160px; overflow: hidden; text-overflow: ellipsis !important; "> ' . $show_project . '</td>
                <td style="width:30px; font-weight: bold;">View:</th>
                    <td style="width:30px;">' . $activity_availability . '</td>';

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
    $WORK_CLASS = '';

    $STUDENT_ONCLICK = 'window.location = \'?q=ntlp/courses/activity/grade/' . $course_nid . '/' . $activity_nid . '/' . $student_uid . '\';';
    $STUDENT_CLASS = 'class= "GradesTabActive"';


    $activity_tab_panel = $HTML_ACTIVITY_GRADE_TAB_PANEL;
    $activity_tab_panel = str_replace('@OVERVIEW_CLASS', $OVERVIEW_CLASS, $activity_tab_panel);
    $activity_tab_panel = str_replace('@OVERVIEW', $OVERVIEW_ONCLICK, $activity_tab_panel);
    $activity_tab_panel = str_replace('@WORK_CLASS', $WORK_CLASS, $activity_tab_panel);
    $activity_tab_panel = str_replace('@WORK', $WORK_ONCLICK_, $activity_tab_panel);
    $activity_tab_panel = str_replace('@STUDENT_CLASS', $STUDENT_CLASS, $activity_tab_panel);
    $activity_tab_panel = str_replace('@STUDENT', $STUDENT_ONCLICK, $activity_tab_panel);

    // activity publish button
    if ($activity_obj->is_published == 0) {
        $publish_status = "Publish Grades";
        $StatusID = 1;
    } else {
        $publish_status = "Unpublish Grades";
        $StatusID = 0;
    }

    /* published date setting */
    if ($activity_obj->is_published == 1) {

        if (!empty($activity_obj->published_date)) {
            $published_date = '<div class="Activity_grade_Published published_grade_box" style="float: left;"> Published ' . get_tz(get_user_school($user->uid), DATE_FORMAT_LAST_SAVED, $activity_obj->published_date) .'</div>';
        } else {
            $published_date = '';
        }
    } else {
        $published_date = '';
    }

    $GRADE_PUBLISHED_DATETIME_BUTTON = $published_date . (($edit_mode) ? '
            <div style="float: left;">&nbsp;&nbsp;</div>
            <div style="float: left;" align="right">
                <a onmousedown="this.className=SubmitAssignmentBtnClicked" onmouseup="this.className=SubmitAssignmentBtn" onclick="publish_this_activity(' . $activity_obj->nid . ',' . $StatusID . '); return false;" class="SubmitAssignmentBtn">' . $publish_status . '</a>
            </div>'
            : "");

    $activity_tab_panel = str_replace('@GRADE_PUBLISHED_DATETIME_BUTTON', $GRADE_PUBLISHED_DATETIME_BUTTON, $activity_tab_panel);

    $form['main']['form'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#prefix' => str_replace('@ACTIVITY_TAB_PANEL', $activity_tab_panel, $HTML_activity_submission_edit_basic_panel_START),
        '#suffix' => $HTML_activity_submission_edit_basic_panel_END,
    );

    if ($activity_obj->type == 'lo_rubric') 
        ntlp_activity_rubric_grade_submission($form_state, $form['main']['form'], $activity_obj, $student_uid, $edit_mode);
    else
        ntlp_activity_basic_grade_submission($form_state, $form['main']['form'], $activity_obj, $student_uid, $edit_mode);

    return $form;
}

function ntlp_activity_rubric_grade_submission($form_state, &$form, $activity_obj, $student_uid, $edit_mode) {
    global $user;

    if (isset($_GET['sub'])) {
        $submenu = $_GET['sub'];
    } else {
        $submenu = 'teacherview';
    }

    jquery_ui_add('ui.accordion');

    drupal_add_js('$(function() {
        $( "#accordion" ).accordion({
            collapsible: true
        });
        });', 'inline');

    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/rubric_activity.js');

    include('activity_grade_template.inc');

    $user_object = get_user_record($student_uid);
    $submission_result = get_user_submission($activity_obj->school_term_tid, $activity_obj->nid, $student_uid);

    if ($submission_result->num_rows > 0) {
        $act_submitted_Obj = db_fetch_object($submission_result);
    }

    $rubricObj = get_rubric_activity_records($activity_obj->nid);

    $form['activity_obj'] = array(
        '#type' => 'hidden',
        '#value' => serialize($activity_obj),
    );

    $select_max_points = get_selected_max_points($rubricObj->outcome_tid, $activity_obj->nid);

    $form['selected_outcome'] = array(
        '#type' => 'hidden',
        '#id' => 'selected_outcome',
        '#value' => $select_max_points->max_points,
    );

    $form['act_main_button'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => $HTML_ACTIVITY_GRADE_USER_BUTTON_BAR_START . '<div><img src="' . is_user_image_exist($user_object->picture) . '" width="50", height="50"></div><div>',
        '#suffix' => ' <h1 style="color:#000;">' . $user_object->first_name . ' ' . $user_object->last_name . '</h1></div>',
    );

    $user_recevied_eval_arr = get_user_recieved_evaluation_records($activity_obj->nid, $student_uid);

    if (!empty($user_recevied_eval_arr) && $user_recevied_eval_arr != null) {

        foreach ($user_recevied_eval_arr as $user_received_Obj) {

            $form['user_received_submission_ids_' . $user_received_Obj->submission_nid] = array(
                '#type' => 'hidden',
                '#name' => 'user_received_sub_nid[]',
                '#id' => $user_received_Obj->submission_nid,
                '#value' => intval($user_received_Obj->score),
            );

            $form['activity_max_score'] = array(
                '#type' => 'hidden',
                '#id' => 'max_category_scores',
                '#value' => intval($user_received_Obj->max_points),
            );

            $form['submission_nids_'.$user_received_Obj->submission_nid] = array(
                '#type' => 'hidden',
                '#name' => 'ids[]',
                '#value' => $user_received_Obj->submission_nid,
            );
        }
    }


    // building a sequence activity user array
    $record = get_activity_users($activity_obj->nid, "n.last_name, n.first_name");
    $i = 1;
    while ($act_urec = db_fetch_object($record)) {
        $act_users[$i++] = $act_urec->user_uid;
    }


    # It will Fetch Next User ID
    $next_user = get_another_user_submission($activity_obj->nid, $student_uid, $act_users, 1);

    if ($submenu != 'studentview' && $edit_mode) { //hide this submit buttons if sub menutab student is selected
        $form['convert_to_grade_btn'] = array(
            '#type' => 'item',
//            '#value' => '<a class="convert_to_grades_btn" onclick="get_average_score(' . $overall_points['avg_cat_max_points'] . ',' . $overall_points['avg_cat_feedback_points'] . ',' . $rubricObj->outcome_tid . ')" > Convert to Grades </a>',
            '#value' => '<a class="convert_to_grades_btn" onclick="get_user_recieved_score_to_grade(' . $rubricObj->outcome_tid . ')" > Convert to Grades </a>',
            '#prefix' => ' <div class="last" style="width:495px;"><table><tr><td style="padding-top: 10px;">',
            '#suffix' => '</td><td>&nbsp;</td> ',
        );

        $form['save_and_close_btn'] = array(
            '#type' => 'submit',
            '#value' => t('Save & Close'),
            '#submit' => array('activity_rubric_submission_grade_save'),
            '#prefix' => '<td style="padding-top: 11px;">',
            '#suffix' => '</td><td></td> ',
            '#attributes' => array('class' => 'SaveAndCloseBtn', 'onmousedown' => "this.className='SaveAndCloseBtnClicked'", 'onmouseup' => "this.className='SaveAndCloseBtn'"),
        );

        if ($next_user != 0 || $next_user != null) {
            $form['save_and_next_btn'] = array(
                '#type' => 'submit',
                '#value' => t('Save & Next Student'),
                '#attributes' => array('class' => 'SaveAndNextBtn', 'onmousedown' => "this.className='SaveAndNextBtnClicked'", 'onmouseup' => "this.className='SaveAndNextBtn'"),
                '#submit' => array('activity_rubric_submission_grade_save'),
                '#prefix' => '<td style="padding-top: 11px;">',
                '#suffix' => '</td></tr></table></div>' . $HTML_ACTIVITY_GRADE_USER_BUTTON_BAR_END,
            );

            $form['next_user'] = array(
                '#type' => 'hidden',
                '#value' => $next_user,
            );
        } else {
            $form['save_and_next_btn'] = array(
                '#type' => 'submit',
                '#value' => t('Save & Next Student'),
                '#attributes' => array('class' => 'SaveAndNextBtn_Disabled', 'disabled' => 'true'),
                '#prefix' => '<td style="padding-top: 11px;">',
                '#suffix' => '</td></table></div>' . $HTML_ACTIVITY_GRADE_USER_BUTTON_BAR_END,
            );
        }
    }


    if ($submenu == 'teacherview') {
        // activity_status
        $TEACHER_ONCLICK = 'window.location = \'?q=ntlp/courses/activity/grade/' . $activity_obj->course_nid . '/' . $activity_obj->nid . '/' . $student_uid . '&sub=teacherview\';';
        $TEACHER_CLASS = 'class= "GradesTabActive"';

        $STUDENT_ONCLICK = 'window.location = \'?q=ntlp/courses/activity/grade/' . $activity_obj->course_nid . '/' . $activity_obj->nid . '/' . $student_uid . '&sub=studentview\';';
        $STUDENT_CLASS = '';
    } else {
        $TEACHER_ONCLICK = 'window.location = \'?q=ntlp/courses/activity/grade/' . $activity_obj->course_nid . '/' . $activity_obj->nid . '/' . $student_uid . '&sub=teacherview\';';
        $TEACHER_CLASS = '';

        $STUDENT_ONCLICK = 'window.location = \'?q=ntlp/courses/activity/grade/' . $activity_obj->course_nid . '/' . $activity_obj->nid . '/' . $student_uid . '&sub=studentview\';';
        $STUDENT_CLASS = 'class= "GradesTabActive"';
    }


    $under_menu_panel = $HTML_ACTIVITY_RUBRIC_GRADE_UNDERMENU_PANEL;
    $under_menu_panel = str_replace('@TEACHER_CLASS', $TEACHER_CLASS, $under_menu_panel);
    $under_menu_panel = str_replace('@TEACHER_ONCLICK', $TEACHER_ONCLICK, $under_menu_panel);
    $under_menu_panel = str_replace('@STUDENT_CLASS', $STUDENT_CLASS, $under_menu_panel);
    $under_menu_panel = str_replace('@STUDENT_ONCLICK', $STUDENT_ONCLICK, $under_menu_panel);


    $form['view_teacher_student'] = array(
        '#type' => 'item',
        '#value' => $under_menu_panel,
        '#prefix' => '<tr><td colspan="3"><div class="studentpanel" style=" border-top: 0 none !important; border:10px solid #3570aa; background-color:#3570aa; -moz-border-radius:0 0 5px 5px; -webkit-border-radius:0 0 5px 5px; ">
            <div class="">
              <div class="">
                <div class="">
                  <div class="">
                    <div class="">
                      <div class="studentpanel_center" style="background-color:#fff; border:10px solid #fff; -moz-border-radius:5px; -webkit-border-radius:5px">
                        <div style="font-size: 3px;">&nbsp;</div>
                          <div class="student_Panel">

                          <div><table width="100%" cellpadding="0" cellspacing="0"><tr><td colspan="3"> ',
        '#suffix' => '</td></tr>',
    );

    if ($submenu != 'teacherview') {
        $form['act_journal_desc'] = array(
            '#type' => 'item',
            '#value' => t(''),
            '#prefix' => '<tr><td valign="top" style="width: 60%;">' . ntlp_activity_submission_rubric_view_content($activity_obj, $student_uid),
            '#suffix' => '</td>',
        );
    } else {
        $form['act_journal_desc'] = array(
            '#type' => 'item',
            '#value' => t(''),
            '#prefix' => '<tr><td valign="top" style="width: 60%;">' . ntlp_activity_grade_rubric_teacher_view_content($activity_obj, $student_uid),
            '#suffix' => '</td>',
        );
    }

    $form['blank'] = array(
        '#type' => 'item',
        '#value' => '<td style="width: 10px;">&nbsp;</td>',
    );

    if ($submenu != 'studentview') {
        $form['grade']['block'] = array(
            '#type' => 'fieldset',
            '#prefix' => '<td valign="top">',
            '#suffix' => '</td></tr></table></div></div>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div></td></tr>',
        );

        ntlp_user_activity_grade_outcome($form_state, $form['grade']['block'], $activity_obj, $act_submitted_Obj, $student_uid, $edit_mode);
    } else {
        $form['grade']['block'] = array(
            '#type' => 'item',
            '#value' => get_user_graded_outcome($activity_obj, $student_uid),
            '#prefix' => '<td valign="top">',
            '#suffix' => '</td></tr></table></div></div>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div></td></tr>',
        );
    }
}

function ntlp_activity_basic_grade_submission($form_state, &$form, $activity_obj, $student_uid, $edit_mode) {
    global $user;

    include('activity_grade_template.inc');
    $user_object = get_user_record($student_uid);
    $submission_result = get_user_submission($activity_obj->school_term_tid, $activity_obj->nid, $student_uid);

    if ($submission_result->num_rows > 0) {
        $act_submitted_Obj = db_fetch_object($submission_result);
    }

    $form['act_main_button'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => $HTML_ACTIVITY_GRADE_USER_BUTTON_BAR_START . '<div class="StudentProfileImage"><img src="' . is_user_image_exist($user_object->picture) . '" width="50", height="50"></div><div>',
        '#suffix' => ' <h1 style="color:#000;">' . $user_object->first_name . ' ' . $user_object->last_name . '</h1></div>',
    );

    // building a sequence activity user array
    $record = get_activity_users($activity_obj->nid);
    $i = 1;
    while ($act_urec = db_fetch_object($record)) {
        $act_users[$i++] = $act_urec->user_uid;
    }

    # It will Fetch Next User ID
    $next_user = get_another_user_submission($activity_obj->nid, $student_uid, $act_users, 1);

    if ($edit_mode) {
        $form['save_and_close_btn'] = array(
            '#type' => 'submit',
            '#value' => t('Save & Close'),
            '#submit' => array('activity_submission_grade_save'),
            '#prefix' => ' <div class="last"><table><tr><td style="padding-top: 10px;">',
            '#suffix' => '</td><td>&nbsp;</td> ',
            '#attributes' => array('class' => 'SaveAndCloseBtn', 'onmousedown' => "this.className='SaveAndCloseBtnClicked'", 'onmouseup' => "this.className='SaveAndCloseBtn'"),
        );

        if ($next_user != 0 || $next_user != null) {
            $form['save_and_next_btn'] = array(
                '#type' => 'submit',
                '#value' => t('Save & Next Student'),
                '#submit' => array('activity_submission_grade_save'),
                '#prefix' => '<td style="padding-top: 10px;">',
                '#suffix' => '</td></table></div>' . $HTML_ACTIVITY_GRADE_USER_BUTTON_BAR_END,
                '#attributes' => array('class' => 'SaveAndNextBtn', 'onmousedown' => "this.className='SaveAndNextBtnClicked'", 'onmouseup' => "this.className='SaveAndNextBtn'"),
            );

            $form['next_user'] = array(
                '#type' => 'hidden',
                '#value' => $next_user,
            );
        } else {
            $form['save_and_next_btn'] = array(
                '#type' => 'submit',
                '#value' => t('Save & Next Student'),
                '#attributes' => array('class' => 'SaveAndNextBtn_Disabled', 'disabled' => 'true'),
                '#prefix' => '<td style="padding-top: 10px;">',
                '#suffix' => '</td></table></div>' . $HTML_ACTIVITY_GRADE_USER_BUTTON_BAR_END,
            );
        }
    }

    if ($activity_obj->type == 'lo_journal') {


        $form['act_journal_desc'] = array(
            '#type' => 'item',
            '#value' => t($activity_obj->body),
            '#prefix' => '<tr><td valign="top" style="width: 60%;">' . $HTML_SUBMISSION_GRADE_JOURNAL_DESCRIPTION_START,
            '#suffix' => $HTML_SUBMISSION_GRADE_JOURNAL_DESCRIPTION_END,
        );
    } else {
        $form['act_journal_desc'] = array(
            '#type' => 'item',
            '#value' => '',
            '#prefix' => '<tr><td valign="top" style="width: 60%;">',
            '#suffix' => '',
        );
    }



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


    $form['act_journal']['student_work'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => $HTML_activity_submission_grade_student_panel_START,
        '#suffix' => $HTML_activity_submission_grade_student_panel_END,
    );

    $activity_submitted_status = get_activity_submission_and_save_status($act_submitted_Obj);

    if ($activity_obj->submission_type == 'D') {
        $form['act_journal']['student_work']['heading'] = array(
            '#type' => 'item',
            '#value' => '<div style="background-color:#f1dcdb;"><h1 style=" padding:0 4px 4px 7px; margin-top:0px; font-weight:normal; font-size: 14px; color:#000; ">Student Work '. $activity_submitted_status . ' </h1></div>',
            '#prefix' => '<div>',
            '#suffix' => '</div>',
        );
    }
    $msg = '';
    if ($act_submitted_Obj->nid > 0) {
    $get_attachment = get_attachment($act_submitted_Obj->nid, 'ntlp_submission_attachment', 'submission_nid');

    }
    if(($act_submitted_Obj->body == '') && ($activity_obj->submission_type == 'D') && ($get_attachment == Null) ){

            $msg ="<span style='color:#888; line-height: 40px;'>No work has been submitted yet.</span>";

    }
    else {
        $msg =$act_submitted_Obj->body;
    }

    $form['act_journal']['student_work']['work'] = array(
        '#type' => 'item',
        '#value' => $msg,
        '#prefix' => '<div style="padding-left:5px;">',
        '#suffix' => '</div><br clear="all">',
    );

    if ($act_submitted_Obj != false && $act_submitted_Obj->nid > 0) {

        $obj = get_attachment($act_submitted_Obj->nid, 'ntlp_submission_attachment', 'submission_nid');
//------------
        if ($obj != null) {
//            $divdata = '<table>';
            foreach ($obj as $attchmentObj) {

                if ($attchmentObj->type == 'G') {
                    $hiddendata .= 'G,' . $attchmentObj->id . ',' . $attchmentObj->path . ';';
                    $file_name = ($attchmentObj->attachment_name != "" ? $attchmentObj->attachment_name : $attchmentObj->path) ;
                    $divdata .= '<div id="G_' . $attchmentObj->id . '"><a href="' . $attchmentObj->path . '" target="_blank" >' . $file_name . '</a></div><br />';
                    //attchement view without delete button
                    $attchement_view_data .= '<li><span><a href="' . $attchmentObj->path . '" target="_blank" >' . $attchmentObj->path . '</a></span> </li>';
                } elseif ($attchmentObj->type == 'L') {
                    $hiddendata .= 'L,' . $attchmentObj->id . ',' . $attchmentObj->path . ';';
                    $divdata .= '<div id="L_' . $attchmentObj->id . '"> <a href="#" onclick="detele_attach(\'L_' . $attchmentObj->id . '\');" ><img src="' . $base_path . '/sites/all/modules/ntlp_courses_activity/theme/delete.PNG" border="0" /></a><a href="' . $attchmentObj->path . '" target="_blank" >' . $attchmentObj->path . '</a></div><br />';
                    //attchement view without delete button
                    $attchement_view_data .= '<li> <span><a href="' . $attchmentObj->path . '" target="_blank" >' . $attchmentObj->path . '</a></span> </li>';
                } elseif ($attchmentObj->type == 'F') {
                    $hiddendata .= 'F,' . $attchmentObj->id . ',' . $attchmentObj->path . ';';
                        $file_image ='img_document';

                    if ($file_obj = _get_dpl_files_obj($attchmentObj->fid)) {
                        $file_path = $file_obj->filepath;

                        $file_image = check_and_the_file_type_image($file_obj->filemime,$file_path);

//                        $activity_attched_doc .= '<li> <span><img src=' . $base_path . 'themes/Boldr/Images/resourceStuff/' . $file_image . '.png border="0" /><a href="' . $picture . '" target="_blank">' . $file_obj->filename . '</a></span> </li>';
                    }
                    //    $divdata .= '<div id="F_' . $attchmentObj->id . '"> <a href="#" onclick="detele_attach(\'F_' . $attchmentObj->id . '\');" ><img src="' . $base_path . '/sites/all/modules/ntlp_courses_activity/theme/delete.PNG" border="0" /></a><a href="' . $base_path . $file_path . '" target="_blank">' . $file_obj->filename . '</a></div><br />';
                    //    attchement view without delete button
                    $divdata  .= '<span><a href="' . $base_path . $file_path . '" target="_blank" >' . $attchmentObj->path . '</a></span> <br />';
                    $fids .= ',' . $attchmentObj->fid;
	                }
            }




        }
        //-----------
    }

    $form['act_journal']['student_work']['attached_files_hidden'] = array(
        '#type' => 'hidden',
        '#value' => $hiddendata,
        '#id' => 'attachted_files_hidden',
    );

    if (!empty($divdata)) {
        $form['act_journal']['student_work']['attached_files'] = array(
            '#type' => 'markup',
            '#value' => '<h2 class="Activity_Detail-headings">Attachments:</h2><div id="urllist">' . $divdata . '</div>',
            '#prefix' => '<div style="padding-left:5px;">',
            '#suffix' => '</div>',
        );
    }
    if (!empty($act_submitted_Obj->note_to_teacher)) {
        $form['act_journal']['student_work']['student_comments'] = array(
            '#type' => 'item',
            '#value' => t($act_submitted_Obj->note_to_teacher),
            '#prefix' => $HTML_ACTIVITY_GRADE_STUDENT_COMMENTS_START,
            '#suffix' => $HTML_ACTIVITY_GRADE_STUDENT_COMMENTS_END,
        );
    }


    $form['blank'] = array(
        '#type' => 'item',
        '#value' => '<td style="width: 10px;">&nbsp;</td>',
    );


    $form['grade']['block'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<td valign="top">',
        '#suffix' => '</td>',
    );

    ntlp_user_activity_grade_outcome($form_state, $form['grade']['block'], $activity_obj, $act_submitted_Obj, $student_uid, $edit_mode);
}

function activity_submission_grade_save($form, $form_state) {
    global $user;

    $operation = $form_state['clicked_button']['#post']['op'];
    $student_uid = $form_state['clicked_button']['#post']['main']['student_uid'];
    $activity_obj = unserialize($form_state['clicked_button']['#post']['main']['form']['activity_obj']);
    // next enrolled user id in activities
    $next_user = $form_state['clicked_button']['#post']['main']['form']['next_user'];

    $teacher_notes = $form_state['clicked_button']['#post']['main']['form']['grade']['block']['teacher_comments'];
    $grade_incomplete_chk = $form_state['clicked_button']['#post']['chk_incomplete'];
    $grade_late_chk = $form_state['clicked_button']['#post']['main']['form']['grade']['block']['grade_late_chk'];
    $outcomearray = $form_state['clicked_button']['#post']['disabled_outcome'];

    $outcomekey = $form_state['values']['main']['form']['grade']['block']['outcome_table'];

    if ($act_rec = get_activity_records($activity_obj->nid, true)) {
        $grade_id = save_activity_user_grade($activity_obj->nid, $student_uid, $teacher_notes, $user->uid, ($grade_incomplete_chk == 1 ? 0 : 1), ($grade_late_chk == 1 ? 1 : 0));

        if ($outcomearray != null && $outcomekey != null) {
            // Separating Key For outcome From  an Array in order to combine with an other array

            $arr = array();
            $i = 0;
            while (list($key, $val) = each($outcomekey)) {
                if (strpos($key, 'outcome_') === 0) {
                    $arr[$i] = $key;
                    $i++;
                }
            }

            $com = array_combine($arr, $outcomearray);
            $activityoutcomearray = get_outcomes_values($com, $activity_obj->nid);

            save_activity_user_grade_scores($grade_id, $activityoutcomearray);
//                drupal_set_message('Scores saved successfully!');
        } else {
            
            $activityoutcome = get_activity_outcome($activity_obj->nid);
            $outcomes = array();
            if (sizeof($activityoutcome) > 0) {
                foreach ($activityoutcome as $outcome) {
                    $outcomes[$outcome->tid] = 0;
                }
            }
            save_activity_user_grade_scores($grade_id, $outcomes);
//                drupal_set_message('Scores saved successfully!');
        }
                    //TRIGGER_POINT: UPDATE STUDENT RECORD FOR CURRENT COURSE TERMS
        dw_set_student_course_grade_toupdate($student_uid, $activity_obj->course_nid, $act_rec->school_term_tid);
        
        if ($activity_obj->is_published) {
            ntlpsubmissionactivity_submit("grade", $activity_obj->nid, $student_uid, $user->uid);
        }
    } else {
        drupal_set_message('Activity not found, unable to save grade!');
    }

    if ($operation == 'Save & Close') {
        drupal_goto('ntlp/courses/activity/view/' . $activity_obj->course_nid . '/' . $activity_obj->nid, array('tab' => 'work'));
    } else if ($operation == 'Save & Next Student') {
        if (isset($next_user) && $next_user != 0) {
            drupal_goto('ntlp/courses/activity/grade/' . $activity_obj->course_nid . '/' . $activity_obj->nid . '/' . $next_user);
        }
    }
}

function ntlp_user_activity_grade_outcome($form_state, &$form, $activity_obj, $act_submitted_obj, $student_uid, $edit_mode) {

    include('activity_grade_template.inc');

    $last_save_date = (isset($act_submitted_obj->date_graded)) ? date(DATE_FORMAT_ACTIVITY_STATUS, strtotime($act_submitted_obj->date_graded)) : '';
    $form['outcome_table'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => str_replace('@GRADE_LAST_SAVED', $last_save_date, $HTML_ACTIVITY_GRADE_OUTCOME_BLOCK_START),
        '#suffix' => '',
    );

    $outcome_array = get_activity_outcome($activity_obj->nid);

    // check late sumbmission
    if ($activity_obj->grading_method == 'C') {
        $activity_grading_method = 1; // setting a value 1 if this activity is auto grading.. true
    } else {
        $activity_grading_method = 0;
    }


    $form['outcome_table']['activity_grading_method'] = array(
        '#type' => 'hidden',
        '#id' => 'activity_grading_method',
        '#value' => $activity_grading_method,
    );


    if ($outcome_array != null) {

        foreach ($outcome_array as $outcome_obj) {

            $grade_outcomes = get_assignment_outcomes($outcome_obj->tid, $activity_obj->nid, $student_uid);

            $grading_points = $grade_outcomes->points;
            $activity_excuse = get_excused_assignment_outcomes($outcome_obj->tid, $activity_obj->nid, $student_uid);

            if (isset($activity_excuse->submission_nid)) {
                $grading_points = "EX";
            } else {
                $grading_points = clean_score($grading_points);
            }

            $form['outcome_table']['outcome_' . $outcome_obj->tid] = array(
                '#type' => ($edit_mode) ? 'textfield' : 'item',
                '#size' => '5',
                '#name' => 'disabled_outcome[]',
                '#id' => 'outcome_' . $outcome_obj->tid,
                '#value' => $grading_points,
                '#maxlength' => 6,
                '#prefix' => '<tr><td class="td_lbl_style" style="width: 10px;">',
                '#suffix' => '</td><td style="width:10px;">/</td><td style="width:10px;">' . $outcome_obj->max_points . ' </td><td style="font-size: 14px;">' . $outcome_obj->name . '</td></tr>',
                '#attributes' => array('onblur' => 'validate_input(this);'),
            );

            if ($activity_obj->grading_method == 'C') {

                $form['outcome_hidden' . $outcome_obj->tid] = array(
                    '#type' => 'hidden',
                    '#name' => 'auto_graded_outcome[]',
                    '#id' => 'h_outcome_' . $outcome_obj->tid,
                    '#value' => $outcome_obj->max_points,
                );
            }

            if ($act_submitted_obj->is_graded == 1 && $act_submitted_obj->is_complete == 0) {
                $attr = array('disabled' => "disabled", 'onblur' => 'validate_input(this);');
                $form['outcome_table']['outcome_' . $outcome_obj->tid]['#attributes'] = $attr;
            }
        }
    }

    $form['outcome_table_end'] = array(
        '#type' => 'item',
        '#prefix' => '',
        '#suffix' => $HTML_ACTIVITY_GRADE_OUTCOME_BLOCK_END,
    );

    $form['grade_incomplete_chk'] = array(
        '#type' => ($edit_mode) ? 'checkbox' : 'item',
        '#title' => t('Incomplete'),
        '#name' => 'chk_incomplete',
        '#default_value' => ($act_submitted_obj->is_graded == 1 && $act_submitted_obj->is_complete == 0),
        '#prefix' => $HTML_ACTIVITY_GRADE_CHECK_BOXES_START . ' <td><div class="ChkBox" style="background-color:#FCE6F8;border:1px solid #FBF3AB;padding:5px;white-space:nowrap;"><span>',
        '#suffix' => '</span></div></td> <td>&nbsp;</td>',
        '#attributes' => array('onclick' => 'disabled_outcomes_foruser()'),
    );

    if (!$edit_mode) {
        $inc_val = "<b>Incomplete:</b> ";
        $form['grade_incomplete_chk']['#value'] = $inc_val . (($act_submitted_obj->is_graded == 1 && $act_submitted_obj->is_complete == 0) ? 'x' : "");

        unset($form['grade_incomplete_chk']['#title']);
    }


    $form['grade_late_chk'] = array(
        '#type' => ($edit_mode) ? 'checkbox' : 'item',
        '#title' => t('Late'),
        '#default_value' => ($act_submitted_obj->is_late == 1),
        '#prefix' => '<td><div class="ChkBox" style="padding: 5px; background-color: rgb(255, 255, 204); border: 1px solid rgb(251, 243, 171); white-space: nowrap;"><span>',
        '#suffix' => '</span></div></td>' . $HTML_ACTIVITY_GRADE_CHECK_BOXES_END,
    );

    if (!$edit_mode) {
        $late_val = "<b>Late:</b> ";
        $form['grade_late_chk']['#value'] = $late_val . ($act_submitted_obj->is_late == 1 ? 'x' : "");

        unset($form['grade_late_chk']['#title']);
    }

    if ($edit_mode || strlen($act_submitted_obj->teachers_notes) > 0) {
        $form['teacher_comments'] = array(
            '#type' => ($edit_mode) ? 'textarea' : 'item',
            '#value' => ($edit_mode) ? t($act_submitted_obj->teachers_notes) : nl2br(t($act_submitted_obj->teachers_notes)),
            '#wysiwyg' => FALSE,
            '#prefix' => $HTML_GRADE_TEACHER_COMMENTS_BLOCK_START,
            '#suffix' => $HTML_GRADE_TEACHER_COMMENTS_BLOCK_END,
        );
    }
}

function ntlp_activity_grade_rubric_teacher_view_content($activityObj, $student_uid) {
    global $user, $base_path;

    if ($student_uid == 0) {
        $user_uid = $user->uid;
    } else {
        $user_uid = $student_uid;
    }
    include('activity_grade_template.inc');

    $container = $HTML_GRADE_RUBRIC_TEACHER_VIEW;

    $hidden_fields = '<input type="hidden" name="activity_nid" id="activity_nid" value="' . $activityObj->nid . '">';
    $hidden_fields .= '<input type="hidden" name="course_nid" id="course_nid" value="' . $activityObj->course_nid . '">';
    $hidden_fields .= '<input type="hidden" name="user_id" id="user_id" value="' . $user_uid . '">';

    $container = str_replace('<!-- @HIDDEN_FIELDS -->', $hidden_fields, $container);

    $container = str_replace('@FEEDBACK_FROM_OTHERS_HEADING', '<font style="color: black !important;font-size: 18px;font-weight: normal;line-height: 18px;margin-left: 5px;margin-right: 5px;">Feedback from Others</font>', $container);
    $container = str_replace('@FEEDBACK_SUBMITTED_BY_THIS_STUDENT_HEADING', '<font style="color: black !important;font-size: 18px;font-weight: normal;line-height: 18px;margin-left: 5px;margin-right: 5px; text-decoration:none !important;">Feedback Submitted by This Student</font>', $container);

    $container = str_replace('@SELECTED_ACTIVITY_USER', get_rubric_feedback_submitted_by_user($activityObj, $student_uid), $container);
    $container = str_replace('@AVERAGE_SCORES_FROM_OTHERS_TABLE', get_rubric_average_score_and_feedback_teacher($activityObj, $student_uid), $container);

    return $container;
}

function activity_rubric_submission_grade_save($form, $form_state) {

    global $user;

    watchdog('grad_f_s', print_r($form_state['clicked_button']['#post'], true));
    $operation = $form_state['clicked_button']['#post']['op'];

    $activity_nid = $form_state['clicked_button']['#post']['activity_nid'];
    $user_id = $form_state['clicked_button']['#post']['user_id'];

    $activity_obj = unserialize($form_state['clicked_button']['#post']['main']['form']['activity_obj']);
    // next enrolled user id in activities
    $next_user = $form_state['clicked_button']['#post']['main']['form']['next_user'];

    $teacher_comment = $form_state['clicked_button']['#post']['main']['form']['grade']['block']['teacher_comments'];
    $grade_incomplete_chk = $form_state['clicked_button']['#post']['main']['form']['grade']['block']['grade_incomplete_chk'];
    $grade_late_chk = $form_state['clicked_button']['#post']['main']['form']['grade']['block']['grade_late_chk'];
    $outcomearray = $form_state['clicked_button']['#post']['disabled_outcome'];

    $exclude_entry = $form_state['clicked_button']['#post']['exclude_entry'];
    $submissions_ids = $form_state['clicked_button']['#post']['ids'];

    $outcomekey = $form_state['values']['main']['form']['grade']['block']['outcome_table'];

    /* checking is submission already exit */
    if (intval($activity_obj->nid) > 0) {
        $grade_id = save_activity_user_grade($activity_obj->nid, $user_id, $teacher_comment, $user->uid);

        if ($outcomearray != null && $outcomekey != null) {
            // Separating Key For outcome From  an Array in order to combine with an other array

            $arr = array();
            $i = 0;
            while (list($key, $val) = each($outcomekey)) {
                if (strpos($key, 'outcome_') === 0) {
                    $arr[$i] = $key;
                    $i++;
                }
            }
//            watchdog('grad_out1', 'outcome array ' . print_r($outcomearray, true) . ' outcomekey ' . print_r($outcomekey, true));
            $com = array_combine($arr, $outcomearray);
            $activityoutcomearray = get_outcomes_values($com, $activity_obj->nid);

            if ($activityoutcomearray != null) {
                db_query("DELETE FROM {ntlp_gb_activity_grade} WHERE grade_id=%d", $grade_id);
                db_query("DELETE FROM {ntlp_gb_activity_excuse} WHERE grade_id=%d", $grade_id);

                while (list($key, $val) = each($activityoutcomearray)) {
                    //Don't save blank values
                    if ($val != null) {

                        if (strtoupper($val) == "EX") {
                            db_query("INSERT INTO {ntlp_gb_activity_excuse} (tid, grade_id) VALUES(%d, %d)", check_plain($key), $grade_id);
                        } else {
                            db_query("INSERT INTO {ntlp_gb_activity_grade} (tid, grade_id, points) VALUES (%d, %d, %f)", check_plain($key), $grade_id, $val);
                        }
                    }
                }
                //TRIGGER_POINT: UPDATE STUDENT RECORD FOR CURRENT COURSE TERMS
                dw_set_student_course_grade_toupdate($user_id, $course_nid);

                ntlpsubmissionactivity_submit("grade", $activity_obj->nid, $user_id, $user->uid);
            }
//            drupal_set_message('Scores saved successfully!');
        } else {
            $grade_id = save_activity_user_grade($activity_obj->nid, $user_id, null, $user->uid, 0, null);
            
            $activityoutcome = get_activity_outcome($activity_obj->nid);
            $outcomes = array();
            if (sizeof($activityoutcome) > 0) {
                foreach ($activityoutcome as $outcome) {
                    $outcomes[$outcome->tid] = 0;
                }
            }
            save_activity_user_grade_scores($grade_id, $outcomes);
//            drupal_set_message('Submission marked as Incomplete!');
        }

        // entry exlude part
        if ($submissions_ids != null || !empty($submissions_ids)) {
            foreach ($submissions_ids as $ids) {

                if (in_array($ids, array_keys((Array) $exclude_entry))) {

                    $entry = 1;
                } else {
                    $entry = 0;
                }
                db_query("UPDATE {ntlp_activity_rubric_submission_for} SET exclude_feedback = %d where submission_nid = %d", $entry, $ids);
            }
        }
        if ($operation == 'Save & Close') {
            drupal_goto('ntlp/courses/activity/view/' . $activity_obj->course_nid . '/' . $activity_obj->nid, array('tab' => 'work'));
        } else if ($operation == 'Save & Next Student') {
            if (isset($next_user) && $next_user != 0) {
                drupal_goto('ntlp/courses/activity/grade/' . $activity_obj->course_nid . '/' . $activity_obj->nid . '/' . $next_user, array('sub' => 'teacherview'));
//                ntlp/courses/activity/grade/1225/3496/17&sub=teacherview
            }
        }
    } else {
        drupal_set_message('Activity not found');
    }
}

