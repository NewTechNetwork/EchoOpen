<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_school', 'data_access');

function _ntlp_courses_activity_view_frm($course_nid, $activity_nid) {

    return drupal_get_form('ntlp_courses_activity_view_form', $course_nid, $activity_nid);
}

function ntlp_courses_activity_view_form($form_state, $course_nid, $activity_nid) {

    global $base_path, $user;
    $user_role = check_user_permission($course_nid);

    $tab_option = $_GET['tab'];

    if (empty($tab_option)) {
        $tab_option = 'overview';
    }

    $user_uid = $user->uid;
    if (isset($user->roles[NTLP_ROLEID_PARENT])) {
        $user_uid = $_SESSION['PARENT_STUDENT_UID'];
    }

    drupal_add_js(drupal_get_path('module', 'ntlp_announcement') . '/ntlp_announcement.js', 'module');
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/ntlp_courses_activity.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/rubric_activity.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');

    /* css */
    drupal_add_css(drupal_get_path('module', 'jquery_ui') . '/jquery.ui/themes/base/ui.base.css');
    drupal_add_css(drupal_get_path('module', 'jquery_ui') . '/jquery.ui/themes/base/ui.theme.css');
    drupal_add_css(drupal_get_path('module', 'jquery_ui') . '/jquery.ui/themes/base/ui.accordion.css');
    modalframe_parent_js();

    require_once('activity_view_template.inc');

    jquery_ui_add('ui.accordion');

    drupal_add_js('$(function() {
        $( "#accordion" ).accordion({
            collapsible: true
        });
        });', 'inline');

    set_item_url_handler('Activity View');

    if ($user_role == NTLP_ROLEID_STUDENT || isset($user->roles[NTLP_ROLEID_PARENT])) {
        if (!is_student_access_this_activity($user_uid, $activity_nid)) {
            drupal_goto('ntlp/render/error/page/Sorry, you are not authorized to access this Activity', array('destination' => $_GET['q']));
        }
    }

    $activity_obj = get_activity_records($activity_nid);

    $form['main'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => $HTMLform_activity_outer_name_hdr,
        '#suffix' => $HTMLform_activity_outer_name_ftr,
    );

    if (isset($user_role) && $user_role == NTLP_ROLEID_TEACHER
            || $user_role == NTLP_ROLEID_NETWORKMANAGER
            || $user_role == NTLP_ROLEID_SITEADMIN
            || $user_role == NTLP_ROLEID_SCHOOLLEADER
            || $user_role == NTLP_ROLEID_NETWORKSTAFF) {

        $manage_options = $HTMLform_activity_manage_options;
        $manage_options = str_replace('@MANAGE_ACTIVITY_EDIT', l('Edit Activity', 'ntlp/courses/activity/edit/' . $course_nid . '/' . $activity_nid), $manage_options);
        $manage_options = str_replace('@MANAGE_ACTIVITY_COPY', dlg('Copy Activity', 'ntlp/courses/activity/copy/' . $course_nid . '/' . $activity_nid, 450, 600), $manage_options);
        //$manage_options = str_replace('@MANAGE_ACTIVITY_DELETE', dlg('Delete Activity', 'ntlp/courses/activity/delete/' . $course_nid . '/' . $activity_nid, 450, 600), $manage_options);
        $manage_options = str_replace('@MANAGE_ACTIVITY_DELETE', '<a href="javascript:ntlp_activity_delete(' . $course_nid . ',' . $activity_nid . ')"  >Delete Activity</a>', $manage_options);


        $form['main']['manage_options'] = array(
            '#type' => 'markup',
            '#value' => $manage_options,
        );
    }
    $form['main']['activity_name'] = array(
        '#type' => 'item',
        '#value' => $activity_obj->title,
        '#prefix' => '<div class="Activity_Detail_heading_ph"><div class="left"><h1>',
        '#suffix' => '</h1></div></div><br clear="all" />',
    );


    if ($activity_obj->available_from == '0000-00-00 00:00:00' && $activity_obj->available_to == '0000-00-00 00:00:00') {
        $activity_availability = 'Hide';
    } else {
        $activity_availability = 'Show';
    }

    if ($activity_obj->project_nid != 0 && !empty($activity_obj->project_nid)) {
        $show_project = '<a href="' . url('ntlp/courses/projects/' . $activity_obj->course_nid . '/' . $activity_obj->project_nid) . '">' . _get_node_title($activity_obj->project_nid) . '</a>';
    } else {
        $show_project = '';
    }

    if ($user_role != NTLP_ROLEID_STUDENT && !isset($user->roles[NTLP_ROLEID_PARENT])) {
        $show_view = ' <td style="width:30px; font-weight: bold;">View:</th>
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
                <td style="width:55px; text-align:left;  font-weight: bold;">Due Date:</th>
                    <td style="width:160px; text-align:left;">' . $activity_due_date . ' </td>
                <td style="width:30px;  font-weight: bold;">Type:</th>
                  <td style="width:150px; text-align:left;">' . get_activity_name($activity_obj->type) . '</td>
                <td style="width:55px; font-weight: bold;">' . (!empty($show_project) ? "Project:" : "") . '</th>
                    <td style="white-space: nowrap; width:160px; overflow: hidden; text-overflow: ellipsis !important; "> ' . $show_project . '</td>' . $show_view;


    $form['main']['activity_short_detail'] = array(
        '#type' => 'item',
        '#value' => '<tr> ' . $activity_summary_bar . '</tr>',
        '#prefix' => $HTMLform_activity_title_hdr . '<table width="100%" cellspacing="0" cellpadding="5" border="0" class="infotable" style="table-layout: fixed; border-bottom: medium none ! important;">',
        '#suffix' => '</table>' . $HTMLform_activity_title_ftr,
    );

    // activity attched files
    $obj = get_attachment($activity_obj->nid, 'ntlp_activity_attachment', 'activity_nid');

    if ($obj != null) {
//            $divdata = '<table>';
        foreach ($obj as $attchmentObj) {

            if ($attchmentObj->type == 'G') {
                $file_name = ($attchmentObj->attachment_name != "" ? $attchmentObj->attachment_name : $attchmentObj->path);
                $activity_attched_doc .= '<div id="G_' . $attchmentObj->id . '" <a href=' . $attchmentObj->path . ' >' . $file_name . '</a></div>';
            } elseif ($attchmentObj->type == 'L') {
                $activity_attched_doc .= '<div id="L_' . $attchmentObj->id . '"> <a href=' . $attchmentObj->path . ' >' . $attchmentObj->path . '</a></div>';
            } elseif ($attchmentObj->type == 'F') {

                $file_id = get_attachment_filepath($activity_nid, $attchmentObj->id);


                if ($file_obj = _get_dpl_files_obj($file_id->fid)) {
                    $picture = $file_obj->filepath;
                    $file_image = check_and_the_file_type_image($file_obj->filemime, $file_obj->filepath);
                }
                $activity_attched_doc .= '<li> <span>" target="_blank">&nbsp;' . $attchmentObj->path . '</a></span> </li>';
            }
        }
    }

    if (isset($activity_attched_doc) && !empty($activity_attched_doc)) {
        $attachment = $HTML_ACTIVITY_ATTACHEMENT;
        $attachment = str_replace('@SUBMITTED_ATTACHEMENTS', $activity_attched_doc, $attachment);
    } else {
        $attachment = '';
    }

    // activity standards
    if ($activity_obj->curriculum_standards != '') {
        $standards = $HTML_ACTIVITY_STANDARDS;
        $standards = str_replace('@ACTIVITY_STANDARDS_DESC', wordwrap($activityObj->curriculum_standards, 40, "\n", true), $standards);
    } else {
        $standards = '';
    }

    // activity outcome
    if ($activity_obj->is_graded != 0) { // if activity ungraded
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

    // checking a activity submission type and display a message accordingly
    if ($activity_obj->submission_type == 'O') {

        $activity_message = '<h2 style="font-weight: normal;" class="teacherComment">Work is completed offline. <b>No submission required.</b></h2>';
    } else {
        if ($activity_obj->allow_resubmission == 0) {
            $activity_message = '<h2 style="font-weight: normal;" class="teacherComment">Work is submitted online. You can <b>only submit once</b>.</h2>';
        } else if ($activity_obj->allow_resubmission == 1) {
            $activity_message = '<h2 style="font-weight: normal;" class="teacherComment">Work is submitted online. You can <b>submit multiple times</b>.</h2>';
        }
    }


    $response_info = "";

    if ($activity_obj->type == 'lo_journal') {
        $description_heading = "Journal Prompt:";
    } else {
        $description_heading = "Activity Description:";
    }

    if (empty($activity_obj->body)) {
        $description_heading = '';
        $description_body = '';
    } else {
        $description_body = wordwrap($activity_obj->body, 88, "\n", true);
    }

    $OVERVIEW_ONCLICK = 'window.location = \'?q=ntlp/courses/activity/view/' . $course_nid . '/' . $activity_nid . '&tab=overview\';';
    $WORK_ONCLICK = 'window.location = \'?q=ntlp/courses/activity/view/' . $course_nid . '/' . $activity_nid . '&tab=work\';';

    if ($tab_option == 'overview') {

        $OVERVIEW_CLASS = 'class= "GradesTabActive"';
        $WORK_CLASS = '';
    } else {

        $OVERVIEW_CLASS = '';
        $WORK_CLASS = 'class= "GradesTabActive"';
    }


    if ($user_role == NTLP_ROLEID_STUDENT) {

        if (check_activity_work_assesement_link_for_student($activity_obj->school_term_tid, $activity_obj->nid, $user->uid, $activity_obj->type, $activity_obj->submission_type, $activity_obj->allow_late_sub, $activity_obj->due_date)) {
            $WORK_ONCLICK = 'window.location = \'?q=ntlp/courses/activity/sub/' . $course_nid . '/' . $activity_nid . '\';';
        }
    }

    $work_and_assessment_link = '<li>|</li>
            <li><a id="grades_tab_activities" @WORK_CLASS onclick="@WORK">Work and Assessment</a></li>';

    // activity publish button
    if ($activity_obj->is_published == 0) {
        $publish_status = "Publish Grades";
        $StatusID = 1;
    } else {
        $publish_status = "Unpublish Grades";
        $StatusID = 0;
    }

    $activity_tab_panel = $HTML_ACTIVITY_TAB_PANEL;

    $activity_tab_panel = str_replace('@WORK_AND_ASSESSMENT_LINK', $work_and_assessment_link, $activity_tab_panel);

    $activity_tab_panel = str_replace('@OVERVIEW_CLASS', $OVERVIEW_CLASS, $activity_tab_panel);
    $activity_tab_panel = str_replace('@OVERVIEW', $OVERVIEW_ONCLICK, $activity_tab_panel);
    $activity_tab_panel = str_replace('@WORK_CLASS', $WORK_CLASS, $activity_tab_panel);
    $activity_tab_panel = str_replace('@WORK', $WORK_ONCLICK, $activity_tab_panel);

    /* published date setting */
    if ($activity_obj->is_published == 1) {

        if (!empty($activity_obj->published_date)) {
            $published_date = 'Published ' . get_tz(get_user_school($user->uid), DATE_FORMAT_LAST_SAVED, $activity_obj->published_date);
        } else {
            $published_date = '';
        }
    } else {
        $published_date = '';
    }

    $GRADE_PUBLISHED_DATETIME_BUTTON = '<div class="Activity_Detail-Published" style="float: left;">
                <!-- Grades Published 11/29/2010 @ 12:43pm -->
                ' . $published_date . (($user_role != NTLP_ROLEID_SITEGUEST) ? '
            </div><div style="float: left;">&nbsp;&nbsp;</div>
            <div style="float: left;" align="right">
                <a  onclick="publish_this_activity(' . $activity_obj->nid . ',' . $StatusID . '); return false;" class="SubmitAssignmentBtn">' . $publish_status . '</a>
            </div>' : '');
    if ($user_role != NTLP_ROLEID_STUDENT && !isset($user->roles[NTLP_ROLEID_PARENT])) {
        $activity_tab_panel = str_replace('@GRADE_PUBLISHED_DATETIME_BUTTON', $GRADE_PUBLISHED_DATETIME_BUTTON, $activity_tab_panel);
    } else {
        $activity_tab_panel = str_replace('@GRADE_PUBLISHED_DATETIME_BUTTON', '', $activity_tab_panel);
    }

    if ($tab_option == 'overview') {
        $html_container = $HTML_activity_student_overview_panel;

        $html_container = str_replace('@ACTIVITY_TAB_PANEL', $activity_tab_panel, $html_container);
        $html_container = str_replace('@ACTIVITY_MESSAGE', $activity_message, $html_container);
        $html_container = str_replace('@ACTIVITY_RESPONSE_INFO', $response_info, $html_container);
        $html_container = str_replace('@ACTIVIITY_DESCRIPTION_HEADING', $description_heading, $html_container);
        $html_container = str_replace('@ACTIVITY_DESCRIPTION', $description_body, $html_container);
        $html_container = str_replace('@ATIVITY_ATTACHMENTS', $attachment, $html_container);
        $html_container = str_replace('@ACTIVITY_LEARNING_OUTCOMES', $activity_outcome, $html_container);
        $html_container = str_replace('@ACTIVITY_STANDARD', $standards, $html_container);
    } else if ($tab_option == 'work') {


        if ($user_role != NTLP_ROLEID_STUDENT && !isset($user->roles[NTLP_ROLEID_PARENT])) {
            $html_container = $HTML_activity_student_work_panel;
            $html_container = str_replace('@ACTIVITY_TAB_PANEL', $activity_tab_panel, $html_container);
            $html_container = str_replace('@FILTER_AREA', '', $html_container);
            $html_container = str_replace('@GRADE_ALL_FORM', drupal_get_form('ntlp_courses_activity_grade_all_form', $course_nid, $activity_nid), $html_container);
        } else {

            if ($activity_obj->type == 'lo_rubric') {
                $content = ntlp_activity_submission_rubric_view_content($activity_obj);
            } else if ($activity_obj->type == 'lo_journal') {
                $content = ntlp_activity_submission_journal_view_content($activity_obj);
            } else {
                $content = ntlp_activity_submission_task_workshop_view_content($activity_obj);
            }

            $html_container = $HTML_activity_student_work_submission_view_panel;

            $html_container = str_replace('@ACTIVITY_TAB_PANEL', $activity_tab_panel, $html_container);
            $html_container = str_replace('@STUDENT_SUBMISSION_VIEW_EDIT_FORM', $content, $html_container);
            $html_container = str_replace('@ACTIVITY_GRADE_OUTCOME_TEACHER_COMMENT_BLOCK', get_user_graded_outcome($activity_obj), $html_container);
        }
    }

    $form['main']['student_panel'] = array(
        '#type' => 'item',
        '#value' => $html_container,
        '#prefix' => '<div> ',
        '#suffix' => '</div>',
    );

    return $form;
}

function _get_node_title($nid) {

    $result = db_query("SELECT title FROM dpl_node WHERE nid = %d", $nid);

    if ($result) {
        $data = db_fetch_object($result);
        return $data->title;
    } else {
        return '';
    }
}

function ntlp_activity_submission_task_workshop_view_content($activity_obj) {
    global $user;

    $user_uid = $user->uid;

    if (isset($user->roles[NTLP_ROLEID_PARENT])) {
        $user_uid = $_SESSION['PARENT_STUDENT_UID'];
    }

    include('activity_view_template.inc');

    $attchment_heading = '';

    $container = $HTML_SUBMISSION_VIEW_PAGE_BASIC;

    if ($activity_obj->submission_type == 'D') {

        $attchment_heading = 'Attachments:';

        $submission_result = get_user_submission($activity_obj->school_term_tid, $activity_obj->nid, $user_uid);
        $submitted_record = db_fetch_object($submission_result);

        $description = $activity_obj->body;
        $user_submitted_attachement = get_user_submitted_attachements($user_uid, $submitted_record);


        //handling a edit submission button for student.
        if (isset($activity_obj->allow_late_submission)) {
            $allow_late_submission = $activity_obj->allow_late_submission;
        } else {
            $allow_late_submission = 0; // assign zero mean not allowed
        }


        if (strtotime($activity_obj->due_date) < strtotime('now') && $allow_late_submission == 0) {
            $activity_status_message = 'Your teacher <b>does not allow late submissions</b> for this activity.</b>';
        } else {
            if ($activity_obj->allow_resubmission == 1) {
                $activity_status_message = 'You can submit <b>more than once</b>.';
            } else
                $activity_status_message = 'You can submit <b>only once</b>.';
        }

//        if (check_activity_work_assesement_link_for_student($activityObj->nid, $user_uid)) {
//            $submission_button = '<a href="' . url('ntlp/courses/activity/sub/' . $activityObj->course_nid . '/' . $activityObj->nid) . '" class="SubmitAssignmentBtn">Edit Submission</a>';
//        } else {
//            $submission_button = '';
//        }
        if (is_submission_allowed_for_student($activity_obj, $user_uid, $submitted_record->is_final, $submitted_record->date_opened)) {
            $submission_button = '<a onmousedown=this.className="SubmitAssignmentBtnClicked" onmouseup=this.className="SubmitAssignmentBtn"   href="' . url('ntlp/courses/activity/sub/' . $activity_obj->course_nid . '/' . $activity_obj->nid) . '" class="SubmitAssignmentBtn">Edit Submission</a>';
        } else {
            $submission_button = "";
        }

        if ($activity_obj->is_graded == 0) {

            if ($activity_obj->submission_type == 'D') {
                $activity_status_message = 'Work is completed online. <b> No submission required. </b>';
            } else {
                $activity_status_message = 'Work is completed offline. <b> No submission required. </b>';
            }
        }

        if (!empty($user_submitted_attachement)) {
            $attachment = $HTML_ACTIVITY_ATTACHEMENT;
            $attachment = str_replace('@SUBMITTED_ATTACHEMENTS', $user_submitted_attachement, $attachment);
        } else {
            $attachment = '';
        }

        if (!empty($submitted_record->note_to_teacher)) {
            $student_to_teacher_comment_block = $HTML_STUDENT_COMMENTS_FOR_TEACHER;
            $student_to_teacher_comment_block = str_replace('@STUDENT_COMMENTS_HEADING', 'Student Comments/Notes:', $student_to_teacher_comment_block);
            $student_to_teacher_comment_block = str_replace('@STUDENT_COMMENTS', $submitted_record->note_to_teacher, $student_to_teacher_comment_block);
        } else {
            $student_to_teacher_comment_block = '';
        }

        if ($activity_obj->is_graded == 1) {
            $container = str_replace('@EDIT_SUBMISSION_BUTTON', $submission_button, $container);
            $container = str_replace('@ATTCHMENT_HEADING', $attchment_heading, $container);
            $container = str_replace('@ACTIVITY_ATTACHEMENTS', $attachment, $container);
            $container = str_replace('@ACTIVITY_STUTUS_MESSAGE', $activity_status_message, $container);
            $container = str_replace('@STUDENT_WORK_HEADING', 'Student Work', $container);
            $container = str_replace('@SUBMITTED_WORK', $submitted_record->body, $container);

            $container = str_replace('@STUDENT_COMMENTS_FOR_TEACHER_BLOCK', $student_to_teacher_comment_block, $container);
            $container = str_replace('@SUBMITTED_DATE', get_activity_submission_and_save_status($submitted_record), $container);
        } else {
            $container = str_replace('@EDIT_SUBMISSION_BUTTON', '', $container);
            $container = str_replace('@ATTCHMENT_HEADING', '', $container);
            $container = str_replace('@ACTIVITY_ATTACHEMENTS', '', $container);
            $container = str_replace('@ACTIVITY_STUTUS_MESSAGE', $activity_status_message, $container);
            $container = str_replace('@STUDENT_WORK_HEADING', '', $container);
            $container = str_replace('@SUBMITTED_WORK', '', $container);
            $container = str_replace('@STUDENT_COMMENTS_FOR_TEACHER_BLOCK', '', $container);
            $container = str_replace('@SUBMITTED_DATE', '', $container);
        }
    } else {
        $activity_status_message = 'Work is completed offline. No Submission required.</b>.';
        $container = str_replace('@EDIT_SUBMISSION_BUTTON', '', $container);
        $container = str_replace('@ATTCHMENT_HEADING', $attchment_heading, $container);
        $container = str_replace('@ACTIVITY_ATTACHEMENTS', '', $container);
        $container = str_replace('@ACTIVITY_STUTUS_MESSAGE', $activity_status_message, $container);
        $container = str_replace('@STUDENT_WORK_HEADING', '', $container);

        $container = str_replace('@STUDENT_COMMENTS_FOR_TEACHER_BLOCK', $student_to_teacher_comment_block, $container);
        $container = str_replace('@SUBMITTED_WORK', '', $container);
        $container = str_replace('@SUBMITTED_DATE', '', $container);
    }

    return $container;
}

function ntlp_activity_submission_rubric_view_content($activity_obj, $student_uid = 0) {
    global $user;

    include('activity_view_template.inc');

    $container = $HTML_SUBMISSION_RUBRIC_VIEW;

    $hidden_fields = ' <div id="dialog-confirm-user-delete" style="display: none;" title="Warning">
            <span class="msg"></span></div><input type="hidden" name="activity_nid" id="activity_nid" value="' . $activity_obj->nid . '">';
    $hidden_fields .= '<input type="hidden" name="course_nid" id="course_nid" value="' . $activity_obj->course_nid . '">';
    $hidden_fields .= '<input type="hidden" name="user_id" id="user_id" value="' . $user->uid . '">';

    $container = str_replace('<!-- @HIDDEN_FIELDS -->', $hidden_fields, $container);

    if (isset($user->roles[NTLP_ROLEID_STUDENT]) || isset($user->roles[NTLP_ROLEID_PARENT])) {
        $user_uid = $user->uid;
        if (isset($user->roles[NTLP_ROLEID_PARENT])) {
            $user_uid = $_SESSION['PARENT_STUDENT_UID'];
        }
        // this applies here to check if student recieved any evualtion.
        $rec_eval_results = get_rubric_view_submission_cat_avg_score($user_uid, $activity_obj->nid);

        $submission_result = get_user_submission($activity_obj->school_term_tid, $activity_obj->nid, $user_uid, true);
        //
    } else if (isset($user->roles[NTLP_ROLEID_TEACHER]) || isset($user->roles[NTLP_ROLEID_SITEADMIN])
            || isset($user->roles[NTLP_ROLEID_NETWORKMANAGER]) || isset($user->roles[NTLP_ROLEID_NETWORKSTAFF])) {
        //
        $submission_result = get_user_submission($activity_obj->school_term_tid, $activity_obj->nid, $student_uid, true);
        // this applies here to check if student recieved any evualtion.
        $rec_eval_results = get_rubric_view_submission_cat_avg_score($student_uid, $activity_obj->nid);
        //
    }

    $submitted_record = db_fetch_object($submission_result);


    /* if this activity is grade and recieved feedback is not null then only show feedback from other menu */
//    watchdog('submit_eval', print_r($rec_eval_results, true));
    if ($submitted_record->is_graded == 1 && $rec_eval_results->num_rows > 0) {
        $html_feedback_from_other = '
            <div class="ActivitiesPanel notaccordion">
                <div style="margin-top:20px">
                    <div class="tbl_heading">
                        <div style="float:left">
                            <div style="float:left">
                                <span class="ui-icon ui-icon-triangle-1-s"></span>
                            </div>
                            <font style="color: black ! important; font-size: 18px; font-weight: normal; line-height: 18px; margin-left: 5px; margin-right: 5px;">Feedback from Others</font>
                        </div>
                    </div>
                </div>
            </div>
            <div>' . get_rubric_feedback_from_others_average_score_block($activity_obj, $student_uid) . '</div>';
    } else {
        $html_feedback_from_other = '';
    }

    $container = str_replace('@SELECTED_ACTIVITY_USER', get_rubric_activity_selected_users_table($activity_obj, $student_uid), $container);
    $container = str_replace('@AVERAGE_SCORES_FROM_OTHERS_TABLE', $html_feedback_from_other, $container);

    return $container;
}

function ntlp_activity_submission_journal_view_content($activity_obj) {
    global $user, $base_path;

    $user_uid = $user->uid;

    if (isset($user->roles[NTLP_ROLEID_PARENT])) {
        $user_uid = $_SESSION['PARENT_STUDENT_UID'];
    }

    include('activity_view_template.inc');

    $container = $HTML_SUBMISSION_JOURNAL_VIEW;

    $submission_result = get_user_submission($activity_obj->school_term_tid, $activity_obj->nid, $user_uid);
      $submitted_record = db_fetch_object($submission_result);

    $user_submitted_attachement = get_user_submitted_attachements($user_uid, $submitted_record);

    if (isset($activity_obj->allow_late_submission)) {
        $allow_late_submission = $activity_obj->allow_late_submission;
    } else {
        $allow_late_submission = 0; // assign zero mean not allowed
    }

    if (strtotime($activity_obj->due_date) < strtotime('now') && $allow_late_submission == 0) {
        $activity_status_message = 'Your teacher <b>does not allow late submissions</b> for this activity.</b>';
    } else {
        if ($activity_obj->allow_resubmission == 1) {
            $activity_status_message = 'You can submit <b>more than once</b>.';
        } else {
            $activity_status_message = 'You can submit <b>only once</b>.';
        }
    }


    if (is_submission_allowed_for_student($activity_obj, $user_uid, $submitted_record->is_final, $submitted_record->date_opened)) {
        $submission_button = '<a onmousedown=this.className="SubmitAssignmentBtnClicked" onmouseup=this.className="SubmitAssignmentBtn"  href="' . url('ntlp/courses/activity/sub/' . $activity_obj->course_nid . '/' . $activity_obj->nid) . '" class="SubmitAssignmentBtn">Edit Submission</a>';
    } else {
        $submission_button = "";
    }

    if ($activity_obj->is_graded == 0) {

        $activity_status_message = 'Work is completed online. <b> No submission required. </b>';
    }

    if (!empty($user_submitted_attachement)) {
        $attachment = $HTML_ACTIVITY_ATTACHEMENT;
        $attachment = str_replace('@SUBMITTED_ATTACHEMENTS', $user_submitted_attachement, $attachment);
    } else {
        $attachment = '';
    }

    if (!empty($submitted_record->note_to_teacher)) {
        $student_to_teacher_comment_block = $HTML_STUDENT_COMMENTS_FOR_TEACHER;
        $student_to_teacher_comment_block = str_replace('@STUDENT_COMMENTS_HEADING', 'Student Comments/Notes:', $student_to_teacher_comment_block);
        $student_to_teacher_comment_block = str_replace('@STUDENT_COMMENTS', $submitted_record->note_to_teacher, $student_to_teacher_comment_block);
    } else {
        $student_to_teacher_comment_block = '';
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
                    $file_image = check_and_the_file_type_image($file_obj->filemime, $file_obj->filepath);
                }
                $activity_attched_doc .= '<li> <span><img src=' . $base_path . 'themes/Boldr/Images/resourceStuff/' . $file_image . '.png border="0" /><a href="' . $picture . '" target="_blank">' . $file_obj->filename . '</a></span> </li>';
            }
        }
    }

    if (isset($activity_attched_doc) && !empty($activity_attched_doc)) {
        $acitivity_attachment = '<h2 class="Activity_Detail-headings" style="margin-left:12px;">Attachments:</h2>
        <ul class="Activity_Detail-list" style="margin-left:11px;">' .
                $activity_attched_doc
                . '</ul>';
    } else {
        $acitivity_attachment = '';
    }


    if (!empty($activity_obj->body)) {

        $description_block = str_replace('@JOURNAL_PROMPT_DESC', $activity_obj->body, $HTML_JOURNAL_PROMPT_DESC_BLOCK);

        if (!empty($acitivity_attachment)) {
            $description_block = str_replace('@JOURNAL_ATTACHMENT_BLOCK', $acitivity_attachment, $description_block);
        } else {
            $description_block = str_replace('@JOURNAL_ATTACHMENT_BLOCK', '', $description_block);
        }
    } else if (!empty($acitivity_attachment)) {

        $description_block = str_replace('@JOURNAL_PROMPT_DESC', '', $HTML_JOURNAL_PROMPT_DESC_BLOCK);
        $description_block = str_replace('@JOURNAL_ATTACHMENT_BLOCK', $acitivity_attachment, $description_block);
    } else {
        $description_block = '';
    }

    if ($activity_obj->is_graded == 1) {


        $container = str_replace('@EDIT_SUBMISSION_BUTTON', $submission_button, $container);
        $container = str_replace('@ACTIVITY_ATTACHEMENTS', $attachment, $container);
        $container = str_replace('@STUDENT_COMMENTS_FOR_TEACHER_BLOCK', $student_to_teacher_comment_block, $container);
    } else {


        $container = str_replace('@EDIT_SUBMISSION_BUTTON', '', $container);
        $container = str_replace('@JOURNAL_PROMPT_DESC', '', $container);
        $container = str_replace('@ACTIVITY_ATTACHEMENTS', '', $container);
        $container = str_replace('@STUDENT_COMMENTS_FOR_TEACHER_BLOCK', '', $container);
    }



    $container = str_replace('@JOURNAL_DESCRIPTION_BLOCK', $description_block, $container);

    $container = str_replace('@ACTIVITY_STUTUS_MESSAGE', $activity_status_message, $container);
    $container = str_replace('@SUBMITTED_WORK', $submitted_record->body, $container);
    $container = str_replace('@SUBMITTED_DATE', get_activity_submission_and_save_status($submitted_record), $container);

    return $container;
}

function get_user_submitted_attachements($uid, $submissionObj) {
    global $base_path;
    if ($submissionObj != false && $submissionObj->nid > 0) {

        $obj = get_attachment($submissionObj->nid, 'ntlp_submission_attachment', 'submission_nid');
        if ($obj != null) {
//            $divdata = '<table>';
            foreach ($obj as $attchmentObj) {

                if ($attchmentObj->type == 'G') {
                    $hiddendata .= 'G,' . $attchmentObj->id . ',' . $attchmentObj->path . ';';
                    $divdata .= '<div id="G_' . $attchmentObj->id . '"> <a href="#" onclick="detele_attach(\'G_' . $attchmentObj->id . '\');" ><img src="' . $base_path . '/sites/all/modules/ntlp_courses_activity/theme/delete.PNG" border="0" /></a><a href="' . $attchmentObj->path . '" target="_blank" >' . $attchmentObj->path . '</a></div><br />';
                    //attchement view without delete button
                    $file_name = ($attchmentObj->attachment_name != "" ? $attchmentObj->attachment_name : $attchmentObj->path);
                    $attchement_view_data .= '<li><span><a href="' . $attchmentObj->path . '" target="_blank" >' . $file_name . '</a></span> </li>';
                } elseif ($attchmentObj->type == 'L') {
                    $hiddendata .= 'L,' . $attchmentObj->id . ',' . $attchmentObj->path . ';';
                    $divdata .= '<div id="L_' . $attchmentObj->id . '"> <a href="#" onclick="detele_attach(\'L_' . $attchmentObj->id . '\');" ><img src="' . $base_path . '/sites/all/modules/ntlp_courses_activity/theme/delete.PNG" border="0" /></a><a href="' . $attchmentObj->path . '" target="_blank" >' . $attchmentObj->path . '</a></div><br />';
                    //attchement view without delete button
                    $attchement_view_data .= '<li><span><a href="' . $attchmentObj->path . '" target="_blank" >' . $attchmentObj->path . '</a></span> </li>';
                } elseif ($attchmentObj->type == 'F') {
                    $hiddendata .= 'F,' . $attchmentObj->id . ',' . $attchmentObj->path . ';';
                    $file_image = 'img_document';

                    if ($file_obj = _get_dpl_files_obj($attchmentObj->fid)) {
                        $file_path = $file_obj->filepath;

                        $file_image = check_and_the_file_type_image($file_obj->filemime, $file_path);

//                        $activity_attched_doc .= '<li> <span><img src=' . $base_path . 'themes/Boldr/Images/resourceStuff/' . $file_image . '.png border="0" /><a href="' . $picture . '" target="_blank">' . $file_obj->filename . '</a></span> </li>';
                    }
//                    $divdata .= '<div id="F_' . $attchmentObj->id . '"> <a href="#" onclick="detele_attach(\'F_' . $attchmentObj->id . '\');" ><img src="' . $base_path . '/sites/all/modules/ntlp_courses_activity/theme/delete.PNG" border="0" /></a><a href="' . $base_path . $file_path . '" target="_blank">' . $file_obj->filename . '</a></div><br />';
                    //attchement view without delete button
                    $attchement_view_data .= '<li><span><a href="' . $base_path . $file_path . '" target="_blank" >' . $attchmentObj->path . '</a></span> </li>';
                    $fids .= ',' . $attchmentObj->fid;
                }
            }
        }
    }
    return $attchement_view_data;
}

function get_user_graded_outcome($activity_obj, $student_uid = 0) {
    global $user;

    $user_uid = $user->uid;

    if ($student_uid != 0) {
        $user_uid = $student_uid;
    }

    if (isset($user->roles[NTLP_ROLEID_PARENT])) {
        $user_uid = $_SESSION['PARENT_STUDENT_UID'];
    }

    include('activity_view_template.inc');
    $last_save = '';

    $content = $HTML_ACTIVITY_OUTCOME_AND_TEACHER_COMMENTS_BLOCK;

    $activity_outcome = get_activity_outcome($activity_obj->nid);

    $submission_result = get_user_submission($activity_obj->school_term_tid, $activity_obj->nid, $user_uid);

    if ($submission_result->num_rows > 0) {

        $submission_data = db_fetch_object($submission_result);

        if ($submission_data->is_graded == 1 && $activity_obj->is_published == 1) {
            $last_save = 'Posted ' . get_tz_course($activity_obj->course_nid, DATE_FORMAT_LAST_SAVED, $activity_obj->published_date);
        } else {
            $last_save = 'Not Posted Yet';
        }

        if ($activity_obj->is_published == 1 && isset($submission_data->teachers_notes) && !empty($submission_data->teachers_notes)) {
            $teacher_comment_blck = $HTML_TEACHER_COMMENTS_BLOCK;

            $teacher_comment_blck = str_replace('@TEACHER_COMMENTS', $submission_data->teachers_notes, $teacher_comment_blck);
        } else {
            $teacher_comment_blck = '';
        }

        if ($activity_outcome != null) {
            foreach ($activity_outcome as $outcome) {

                // Init vars
                unset($graded_outcome);
                $graded_points = '';

                //Get points for current outcome
                $graded_outcome = get_assignment_outcomes($outcome->tid, $activity_obj->nid, $user_uid);

                if (isset($graded_outcome->points)) {
                    $graded_points = clean_score($graded_outcome->points);
                }

                $activity_excuse = get_excused_assignment_outcomes($outcome->tid, $activity_obj->nid, $user_uid);

                if (isset($activity_excuse->submission_nid)) {
                    $graded_points = "EX";
                }

                if ($activity_obj->is_published != 1)
                    $graded_points = "";

                $grade_outcome .= '<tr>';
                $grade_outcome .= '<td class="td_lbl_style" style="width: 10px;">' . $graded_points . '</td><td style="width:10px;">/</td><td style="width:10px;">' . $outcome->max_points . ' </td>';
                $grade_outcome .= '<td style="font-size: 14px;">' . $outcome->name . '</td>';
                $grade_outcome .= '</tr>';
            }
        }
    } else {

        $teacher_comment_blck = '';
        $grade_outcome .= '<tr>';
        $grade_outcome .= '<td class="td_lbl_style" >&nbsp;</td><td style="width:10px;">/</td><td style="width:10px;">' . $outcome->max_points . ' </td>';
        $grade_outcome .= '<td style="font-size: 14px;">' . $outcome->name . '</td>';
        $grade_outcome .= '</tr>';
    }

    if ($activity_obj->is_graded == 0) {
        $grade_outcome .= '<tr>';
        $grade_outcome .= '<td colspan="3">&nbsp;This Activity is not Graded</td>';

        $grade_outcome .= '</tr>';
    }


    $show_grade_datetime_status = '<table width="100%"  cellspacing="0" cellpadding="0" border="0">
                                        <tr>
                                            <td style="width: 50%;"><h2 class="Activity_Detail-headings"></h2></td>
                                            <td align="right"><div style="color: rgb(153, 153, 153); text-align: left; white-space: nowrap; font-size: 11px;"> ' . $last_save . '</div></td>
                                        </tr>
                                    </table>';
    $flag = false;
    if ($activity_obj->type == 'lo_task' || $activity_obj->type == 'lo_workshop') {
        if ($activity_obj->is_graded == 0 && $activity_obj->submission_type == 'D') {
            $flag = true;
        } else {
            $flag = false;
        }
    }
    if ($flag) {
        $content = '';
        $content = $show_grade_datetime_status;
        $content .= $teacher_comment_blck;
    } else {

        $content = str_replace('@LAST_SAVED', $last_save, $content);
        $content = str_replace('@GRADES_OUTCOMES', $grade_outcome, $content);
        $content = str_replace('@TEACHER_COMMENTS_BLOCK', $teacher_comment_blck, $content);
    }
    return $content;
}

function get_rubric_feedback_from_others_average_score_block($activityObj, $student_uid = 0) {
    global $user;

    if ($student_uid == 0) {
        $user_uid = $user->uid;
    } else {
        $user_uid = $student_uid;
    }

    if (isset($user->roles[NTLP_ROLEID_PARENT])) {
        $user_uid = $_SESSION['PARENT_STUDENT_UID'];
    }

    include('activity_view_template.inc');

    $HTML_avarage_score_block = '<h2 style="padding: 0px; margin: 0pt 0pt 5px; font-size: 15px; font-weight: bold; color: rgb(53, 112, 170);">Average Score</h2>
        <div><table width="100%" cellspacing="0" cellpadding="10" class="tbl">';


    $categories_info = get_rubric_categories($activityObj->nid);
    if ($categories_info != null || !empty($categories_info)) {

        $HTML_avarage_score_block .= '<tr><th style="border-left:1px solid #fff;background-color:#D6E9F5; text-align:center;font-size: 15px;">Total</th>';
        foreach ($categories_info as $category_data) {
            $HTML_avarage_score_block .= '<th style="border-left:1px solid #fff;background-color:#D6E9F5; color:#009900;text-align:left">' . $category_data->category_name . '<div>(' . round($category_data->max_points) . ')</div> </th>';
        }
    }
    $HTML_avarage_score_block .= '</tr>';

    $catsRs = get_rubric_view_submission_cat_avg_score($user_uid, $activityObj->nid);
    if ($catsRs != null || !empty($catsRs)) {
        while ($cat = db_fetch_object($catsRs)) {
            $avg_score[$cat->category_ID][] = round($cat->score);
            $total_score += round($cat->score);
        }
    }

    if ($avg_score != null) {

        $HTML_avarage_score_block .= '<tr>
                <td style="color: rgb(0, 0, 0); font-size: 15px; font-weight: bold; background-color: rgb(238, 238, 238);">' . $total_score . '</td>';
        foreach ($avg_score as $cat_id => $avg) {

            $HTML_avarage_score_block .= '<td style="background-color: rgb(238, 238, 238);">' . round(array_sum($avg) / count($avg)) . '</td>';
        }
        $HTML_avarage_score_block .= '</tr>';
    } else {
        $HTML_avarage_score_block .= '<tr>';
        $HTML_avarage_score_block .= '<td  style="background-color: rgb(238, 238, 238);"></td>';

        foreach ($categories_info as $category) {
            $HTML_avarage_score_block .= '<td style="background-color: rgb(238, 238, 238);"></td>';
        }
        $HTML_avarage_score_block .= '</tr>';
    }


    $HTML_avarage_score_block .= '</table></div>';
    $HTML_avarage_score_block .= '<div style="font-size: 5px;">&nbsp;</div>';
    $HTML_avarage_score_block .= '<h2 style="padding: 0px; margin: 0pt 0pt 5px; font-size: 15px; font-weight: bold; color: rgb(53, 112, 170);">Comments by Category</h2>
    <div>';

    $received_eval_record = get_rubric_evaluated_records_for_user($activityObj->nid, $user_uid);
    if (!empty($received_eval_record)) {

        $Comment_by_cat_html = '';
        foreach ($received_eval_record as $key => $value) {
            $Comment_by_cat_html .= $HTML_Comments_By_Category;
            $evaluated_record = _get_user_evaluated_record_by_category($activityObj->nid, $user_uid, $key);
            $flag = true;

            $comments = '';

            foreach ($evaluated_record as $record) {

                if ($flag) {
                    $Comment_by_cat_html = str_replace('@CATEGORY_HEADING', $record->category_name, $Comment_by_cat_html);
                }
                if (!empty($record->comments)) {
                    $comments .= str_replace('@COMMENTS', $record->comments, $HTML_CATEGORY_COMMENTS_BLOCK);
                } else {
                    $comments .= '';
                }


                $flag = false;
            }
            $Comment_by_cat_html = str_replace('@CATEGORY_COMMENT_RECEIVED', $comments, $Comment_by_cat_html);
        }
    }


    $HTML_avarage_score_block .= $Comment_by_cat_html;


    $HTML_avarage_score_block .= '</div>';
    return $HTML_avarage_score_block;
}

function get_rubric_activity_selected_users_table($activityObj, $student_uid=0) {
    global $base_path, $user;

    if ($student_uid == 0) {
        $user_uid = $user->uid;
    } else {
        $user_uid = $student_uid;
    }
    if (isset($user->roles[NTLP_ROLEID_PARENT])) {
        $user_uid = $_SESSION['PARENT_STUDENT_UID'];
    }

    $dateformat = "m/d/Y";

    $head = array(
        array('data' => t('Group Member Evaluations'), 'width' => '55%'),
        array('data' => t('Submitted'), 'width' => '28%', 'style' => 'text-align:center;'),
        array('data' => t('Total Score'), 'width' => '15%', 'style' => 'text-align:center;'),
    );

    $activityRubricObj = get_rubric_records($activityObj->nid);
    // it will fetch all the users regarding this activity
    $result = get_activity_users($activityObj->nid, "n.last_name, n.first_name");

    // it will fetch all those users whom he has given the feedback
    $feedback_users = get_user_feedback_uids($activityObj->nid, $user_uid);

    $activity_selected_users = '';
    if ($result->num_rows > 0) {

        while ($course_student = db_fetch_object($result)) {

            if (isset($feedback_users)) {

                if (in_array($course_student->user_uid, $feedback_users)) {

                    $user_info = get_activity_username($course_student->user_uid);
                    $picture = is_user_image_exist($user_info->picture);

                    $activity_selected_users .= $course_student->user_uid . ',';

                    $submitted_date = '';
                    $score_rs = get_rubric_submission_cat_score($user_uid, $course_student->user_uid, $activityObj->nid);
                    $total_score = 0;
                    if ($score_rs->num_rows > 0) {
                        while ($score = db_fetch_object($score_rs)) {

                            $avg_score[$score->category_ID][] += $score->score;
                            $total_score += $score->score;
                            $submitted_date = date($dateformat, strtotime($score->created));
                        }
                    }

                    if (!isset($user->roles[NTLP_ROLEID_PARENT]) && !isset($user->roles[NTLP_ROLEID_TEACHER])) {

                        if ($activityRubricObj->self_evaluation == 0 && $user_uid == $course_student->user_uid) {
                            $user_name_link = '<img src="' . $picture . '" width="32" height="32" />&nbsp;' . $user_info->last_name . ', ' . $user_info->first_name;
                            $delete_link = '';
                        } else {
                            $user_name_link = '<input type="hidden" value="' . (($total_score > 0) ? 1 : 0) . '" id="txt_' . $course_student->user_uid . '"><span style="float:left;" ><img src="' . $picture . '" width="32" height="32" /></span> <span style="float:left; margin-top:11px;" >&nbsp;' . l($user_info->last_name . ', ' . $user_info->first_name, 'ntlp/courses/activity/student/evaluate/' . $activityObj->course_nid . '/' . $activityObj->nid . '/' . $course_student->user_uid) . '</span>';
                            $delete_link = '<span style="float:right;  margin-top: 11px;" > <a class="rubric_user_Deletebtn" onclick="delete_rubric_user(\'' . $activityObj->course_nid . '\',\'' . $activityObj->nid . '\',\'' . $course_student->user_uid . '\',\'' . $user_uid . '\')"> </a></span>';
                        }
                    } else {
                        $user_name_link = '<img src="' . $picture . '" width="32" height="32" />&nbsp;' . $user_info->last_name . ', ' . $user_info->first_name;
                        $delete_link = '';
                    }
                    $rows[] = array(
                        array('data' => $user_name_link . $delete_link),
                        array('data' => $submitted_date, 'style' => 'text-align:center;  vertical-align:middle;'),
                        array('data' => $total_score, 'style' => 'text-align:center;font-weight:bold; vertical-align:middle;'),
                    );
                }
            }
        }
    }

    if ($avg_score != null) {
        foreach ($avg_score as $cat_id => $avg) {
            $average_score += round(array_sum($avg) / count($avg));
        }
    }

    if ($student_uid == 0 && !isset($user->roles[NTLP_ROLEID_PARENT])) { // if teacher access this same table with student view.. then hide this option
        cache_set("pplfndInstructor_" . $activityObj->nid . session_id(), trim($activity_selected_users, ','), 'cache', CACHE_TEMPORARY);
        $rows[] = array(
            array('data' => dlg('+ Add a Group Member', 'ntlp/people_finder/get_rubric_users/' .
                        get_this_school() . '/' .
                        $activityObj->nid . '/' .
                        "pplfndInstructor_" . $activityObj->nid . session_id() . '/' .
                        NTLP_ROLEID_STUDENT, 350, 600), 'style' => 'text-align:center;'
            ),
            array('data' => ''),
            array('data' => ''),
        );
    }

    $rows[] = array(
        array('data' => 'AVERAGE SCORES', 'style' => 'background-color:#EEEEEE !important;color:#000000;font-size:13px;text-align:right; font-weight:bold;'),
        array('data' => '', 'style' => 'background-color:#EEEEEE !important;text-align:center;'),
        array('data' => $average_score, 'style' => 'background-color:#EEEEEE !important;text-align:center;font-weight:bold;'),
    );

    $output = theme('table', $head, $rows, $attributes = array('class' => 'ntlp_table', 'width' => '100%', 'cellpadding' => '5'));
    return $output;
}
