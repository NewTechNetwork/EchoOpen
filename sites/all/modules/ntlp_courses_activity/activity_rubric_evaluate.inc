<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
require_once "data_access.inc";

function ntlp_rubric_activity_evaluate_form($form_state, $course_id, $activity_id, $feedback_for_uid) {
    global $user, $base_path;


    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/ntlp_courses_activity.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/rubric_activity.js');
    drupal_add_js(drupal_get_path('module', 'jquery_ui') . '/jquery.ui/ui/ui.slider.js');

//    drupal_add_css(drupal_get_path('module', 'jquery_ui') . '/jquery.ui/themes/smoothness/ui.all.css');
    drupal_add_css(drupal_get_path('module', 'jquery_ui') . '/jquery.ui/themes/base/ui.all.css');

    set_item_url_handler('Activity Evaluation');

//    $feedback_for_uid = check_plain(arg(7));
    $feedback_by_uid = $user->uid;



//    get_rubric_submisssion_id($activity_id, $feedback_for_uid);

    $oldPostResult = db_query('/* INSERT UPDATE DELETE */SELECT rs.* FROM dpl_ntlp_activity_rubric_submission rs
        INNER JOIN dpl_ntlp_activity_rubric_category arc ON arc.category_ID = rs.category_id
        INNER JOIN dpl_node n ON n.nid = rs.submission_nid
        WHERE n.uid = %d AND rs.feedback_for = %d AND arc.activity_nid = %d',
                    $feedback_by_uid, $feedback_for_uid, $activity_id); // old evaluated post

    $oldPostObjArray = array();
    if ($oldPostResult->num_rows) { // get all the data for categories
        while ($oldPost = db_fetch_object($oldPostResult)) {
            $oldPostObjArray[$oldPost->category_id] = $oldPost;
        }
    }
    $oldPostTeacherObj = db_fetch_object(db_query('/* INSERT UPDATE DELETE */SELECT rsf.* FROM dpl_ntlp_activity_rubric_submission_for rsf
        INNER JOIN dpl_ntlp_activity_rubric_submission rs ON rs.submission_nid = rsf.submission_nid
        INNER JOIN dpl_ntlp_activity_rubric_category arc ON arc.category_ID = rs.category_id
        INNER JOIN dpl_node n ON n.nid = rsf.submission_nid
        WHERE n.uid = %d AND rsf.feedback_for = %d AND arc.activity_nid = %d LIMIT 1',
                            $feedback_by_uid, $feedback_for_uid, $activity_id));
    $submission_nid = isset($oldPostObjArray[0]) ? $oldPostObjArray[0]->submission_nid : $oldPostTeacherObj->submission_nid;
//    $submission_nid = $oldPostObjArray[0]->submission_nid;

    include("activity_template.inc");

    $activityObj = get_activity_records($activity_id, true);


    $form['name'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => new_activity_title($activityObj->course_nid),
        '#suffix' => $HTMLform_activity_outer_name_ftr,
    );

    $form['name']['activity_name'] = array(
        '#type' => 'item',
        '#value' => 'Submit Feeback ' . $activityObj->title,
        '#prefix' => '<div style="float:left">',
        '#suffix' => '</div>',
    );

    $form['course_id'] = array(
        '#type' => 'hidden',
        '#value' => $course_id,
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div>',
    );
    $form['activity_id'] = array(
        '#type' => 'hidden',
        '#value' => $activity_id,
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div>',
    );
    $form['feedback_for_uid'] = array(
        '#type' => 'hidden',
        '#value' => $feedback_for_uid,
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div>',
    );
    $form['feedback_by_uid'] = array(
        '#type' => 'hidden',
        '#value' => $feedback_by_uid,
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div>',
    );

    $activityRubricObj = get_rubric_records($activityObj->nid);

    if ($activityRubricObj->require_stu_comments == null || empty($activityRubricObj->require_stu_comments)) {
        $require_student_comments = 'No';
    } else {
        $require_student_comments = ($activityRubricObj->require_stu_comments == 1) ? 'Yes' : 'No';
    }

    $form['require_stu_comments'] = array(
        '#type' => 'hidden',
        '#value' => $require_student_comments,
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div>',
    );

    if ($activityRubricObj->self_evaluation == 0 && $feedback_by_uid == $feedback_for_uid) {
        drupal_goto('ntlp/courses/activity/view/' . $activityObj->course_nid . '/' . $activityObj->nid, array('tab' => 'work'));
    }

    $feedback_users = get_user_feedback_uids($activity_id, $feedback_by_uid);

    $next_user_id = get_user_feedback_uid($feedback_for_uid, $feedback_users, 1);

    if ($activityRubricObj->self_evaluation == 0 && $feedback_by_uid == $next_user_id) {

        $next_user_id = get_user_feedback_uid($next_user_id, $feedback_users, 1);
    }



    $form['info'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<table cellpadding="0" cellspacing="0" border="0" width="100%" >',
        '#suffix' => '</table>',
    );


    $OVERVIEW_ONCLICK = 'window.location = \'?q=ntlp/courses/activity/view/' . $course_id . '/' . $activity_id . '&tab=overview\';';
    $OVERVIEW_CLASS = '';

    $WORK_ONCLICK = 'window.location = \'?q=ntlp/courses/activity/view/' . $course_id . '/' . $activity_id . '&tab=work\';';
    $WORK_CLASS = '';

    $Evaluate_ONCLICK = 'window.location = \'?q=ntlp/courses/activity/student/evaluate/' . $course_id . '/' . $activity_id . '/' . $feedback_for_uid . '\'';
    $Evaluate_CLASS = 'class= "GradesTabActive"';

    $panel = $HTML_RUBRIC_ACTIVITY_TAB_PANEL;
    $panel = str_replace('@OVERVIEW_CLASS', '', $panel);
    $panel = str_replace('@OVERVIEW', $OVERVIEW_ONCLICK, $panel);
    $panel = str_replace('@WORK_CLASS', $WORK_CLASS, $panel);
    $panel = str_replace('@WORK', $WORK_ONCLICK, $panel);
    $panel = str_replace('@EVALUATE_CLASS', $Evaluate_CLASS, $panel);
    $panel = str_replace('@EVALUATE', $Evaluate_ONCLICK, $panel);

    $form['info']['rubric_status_bar'] = array(
        '#type' => 'item',
        '#value' => $panel,
        '#prefix' => '<tr><td style="border:none">',
        '#suffix' => '</td>',
    );

    $student_info = get_activity_username($feedback_for_uid);
    $picture = is_user_image_exist($student_info->picture);


    $form['info']['user_panel'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => $HTML_ACTIVITY_RUBRIC_USER_BUTTON_BAR_START . '<div class="StudentProfileImage"><img src="' . is_user_image_exist($student_info->picture) . '" width="50", height="50"></div><div>',
        '#suffix' => ' <h1 class="StudentProfileFirstName">' . $student_info->first_name . ' ' . $student_info->last_name . '</h1></div>',
    );

    $form['info']['clear_all'] = array(
        '#type' => 'item',
        '#value' => '<a style="position:relative; top:4px; left:6px" onclick="clear_rubric_evaluation_fields();" >Clear All</a>',
        '#prefix' => ' <div class="lastlong"><table><tr><td style="padding-top: 15px;"><div class="CancelBtn">',
        '#suffix' => '</div></td><td>&nbsp;</td> ',
    );

    $form['info']['save'] = array(
        '#type' => 'submit',
        '#value' => t('Submit & Close'),
        '#validate' => array('ntlp_rubric_activity_evaluate_validate'),
        '#submit' => array('ntlp_rubric_activity_evaluate_submit'),
        '#prefix' => '<td style="padding-top: 8px; padding-right: 11px;">',
        '#suffix' => '</td>',
        '#attributes' => array('class' => 'SubmitAssignmentBtn', 'onmousedown' => "this.className='SubmitAssignmentBtnClicked'", 'onmouseup' => "this.className='SubmitAssignmentBtn'"),
    );

    $form['info']['next_user_id'] = array(
        '#type' => 'hidden',
        //   '#name'
        '#value' => $next_user_id,
    );

    if (empty($next_user_id)) {
        $form['info']['save_next'] = array(
            '#type' => 'submit',
            '#value' => t('Submit & Next'),
            '#prefix' => '<td style="padding-top: 8px;">',
            '#suffix' => '</td></table></div>' . $HTML_ACTIVITY_RUBRIC_USER_BUTTON_BAR_END,
            '#attributes' => array('class' => 'SubmitAssignmentBtn_Disabled', 'disabled'=>'true'),
            
        );
    } else {
        $form['info']['save_next'] = array(
            '#type' => 'submit',
            '#value' => t('Submit & Next'),
            '#validate' => array('ntlp_rubric_activity_evaluate_validate'),
            '#submit' => array('ntlp_rubric_activity_evaluate_submit'),
            '#prefix' => '<td style="padding-top: 8px;">',
            '#suffix' => '</td></table></div>' . $HTML_ACTIVITY_RUBRIC_USER_BUTTON_BAR_END,
            '#attributes' => array('class' => 'SubmitAssignmentBtn', 'onmousedown' => "this.className='SubmitAssignmentBtnClicked'", 'onmouseup' => "this.className='SubmitAssignmentBtn'"),
        );
    }

    $form['grade'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#prefix' => $HTMLform_grade_fieldset_1_start,
        '#suffix' => $HTMLform_grade_fieldset_1_end,
    );

    $form['grade']['performance'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => $HTMLform_performance_fielset_2_start,
        '#suffix' => $HTMLform_performance_fielset_2_end,
    );


    $form['grade']['details_box'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => $HTMLform_teacher_textarea_title_hdr . '<div class="ActivitiesAssignment">',
        '#suffix' => '</div>' . $HTMLform_teacher_textarea_title_ftr,
    );



    $form['grade']['details_box']['heading'] = array(
        '#type' => 'item',
        '#value' => t('Please provide an honest evaluation of this student on these areas:'),
        '#prefix' => '<table width="100%" class="table" cellpadding="8" cellspacing="0" border="0"><tr><td valign="top" alight"left" colspan="5" style="background:white; border:none;text-align:left"><b>',
        '#suffix' => '</b></td></tr>',
    );

    $no_of_levels = get_this_activity_levels_size($activity_id);
    $form['grade']['details_box']['level_heading'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => '<tr><td style="width:20%">',
        '#suffix' => '</td>',
    );
//243560 rubric activity_nid

    $slider_script = '';
    $slider_script .= 'var total_levels = 0;';
    $slider_script .= 'var cat_levels = new Array();';

    $slider_load_script = '';
    $slider_load_script .= 'var slider_scores = new Array();';
    $slider_load_script .= 'var slider_ids = new Array();';

    $rubric_categories = get_rubric_categories_records($activity_id);

    if ($rubric_categories != null) {

        $category_count = 0;

        foreach ($rubric_categories as $category) {

            $flag = true;
            $category_levels = get_rubric_categories_levels_records($category->category_ID);
            $count = 1;

            $column_width = (80 / sizeof($category_levels));

            $slider_script .= 'total_levels = ' . sizeof($category_levels) . ';';

            foreach ($category_levels as $level) {

                $slider_script .= 'cat_levels.push("' . $level->level_percent . '");';

                if ($flag) {
                    $total_width = 100 - $level->level_percent;
                    $flag = false;
                }
                $next_percent = $category_levels[$count]->level_percent;

                $level_percent = '';

                $show_next_level = ($next_percent - 1);
                if (empty($next_percent)) {
                    $next_percent = 100;
                    $show_next_level = $next_percent;
                }

                $percent = (($next_percent - $level->level_percent) / $total_width) * 100;

//                $level_percent = '<font style="font-family: verdana; font-size: 75%; color: rgb(77, 159, 48);">  (' . $level->level_percent . '-' . $show_next_level . ' %)</font>';
                $level_percent = '<font style="font-family: verdana; font-size: 75%; color: rgb(77, 159, 48);">  (' . $level->level_percent . ' %)</font>';
                $percent = $percent * .8;

                if ($count == sizeof($category_levels)) {
                    $border_style = 'border-right: 1px solid  #9ECCE9;';
                } else {
                    $border_style = '';
                }

                $form['grade']['details_box']['level_heading_' . $level->level_ID] = array(
                    '#type' => 'item',
                    '#value' => $level->level_name . ' <br><span style="text-align:center">' . ((isset($user->roles[NTLP_ROLEID_STUDENT])) ? '' : $level_percent) . '</span>',
                    '#prefix' => '<th style="padding: 0px;'.$border_style.'background:#fff; padding-bottom:10px;"><div  style="width:100%; height:100%; background-color:#fff;  "><div class="new-form" style="">',
                    '#suffix' => '</div></div></th>',
                        //'#attributes' => array('class' => 'new-form'),
                );

                $count++;
            }

            if ($category_count == 0)
                drupal_add_js($slider_script, 'inline');

            $category_count++;
        }
    }



    if ($rubric_categories != null) {

        foreach ($rubric_categories as $category) {

            $form['grade']['details_box']['category_' . $category->category_ID] = array(
                '#type' => 'item',
                '#value' => $category->category_name,
                '#prefix' => '</tr><tr><th style="font-size:17px; width:20%; text-align:center; vertical-align:middle">',
                '#suffix' => '</th>',
            );

            $category_levels = get_rubric_categories_levels_records($category->category_ID);
            
            $column_width = (81 / sizeof($category_levels));
            
            if (!empty($category_levels)) {
                $count = 1;
                $flag = true;
                foreach ($category_levels as $level) {

                    if ($flag) {
                        $total_width = 100 - $level->level_percent;

                        $start_percent = $level->level_percent;
                        $flag = false;
                    }

                    $next_percent = $category_levels[$count]->level_percent;

                    if (empty($next_percent)) {
                        $next_percent = 100;
                    }

                    $percent = (($next_percent - $level->level_percent) / $total_width) * 100;
                    $percent = $percent * .8;

//                    echo '_percent ' . $next_percent . ' percent ' . $percent . '<br>';
                    $form['grade']['details_box']['level_' . $category->category_ID . '_' . $level->level_ID] = array(
                        '#type' => 'item',
                        '#value' => $level->description,
                        '#prefix' => '<td style="text-align:left; vertical-align:top; border-right:1px solid #9ECCE9;width:' . $column_width . '%">',
                        '#suffix' => '</td>',
                    );
                    $count++;
                }
            }

            $slider_load_script .= 'slider_scores.push("' . (isset($oldPostObjArray[$category->category_ID]) ? intval($oldPostObjArray[$category->category_ID]->score) : 0) . '");
                ';
            $slider_load_script .= 'slider_ids.push("' . $category->category_ID . '");

';

            $slider_info = array(
                'id' => $category->category_ID,
                'min' => 0,
                'max' => 100,
                'step' => 1,
                'value' => 0,
            );

            $form['grade']['details_box']['slider_info'][] = array(
                '#type' => 'hidden',
                '#name' => 'slider_info[]',
                '#value' => implode('|', $slider_info),
            );

            $count_var = 1;
            $level_percent = '';
            $level_value = '<input id="maxpoints_' . $category->category_ID . '" type="hidden" value ="' . $category->max_points . '" /><br>';
            for ($i = 0; $i < sizeof($category_levels); $i++) {
                $level_percent = $category_levels[$i]->level_percent;

                if ($i + 1 < sizeof($category_levels))
                    $next_level_percent = $category_levels[$i + 1]->level_percent;
                else
                    $next_level_percent = 100;


                $level_value .= '<td style="border-bottom:none;width:' . $column_width . '%; text-align: left;">';

                //create inner table to show scale lines
                $level_value .= '<table style="width:100%"><tr>';
                $score_val = get_score($level_percent, $category->max_points);
                if ($next_level_percent - $level_percent > 0) {
                    $colwidth = 100 / (($next_level_percent - $level_percent) / 5);
                } else {
                    $colwidth = 100;
                }
                $level_value .= '<td style="border-bottom:none;width:' . $colwidth . '%; text-align: left;">' . $score_val . '</td>';
                for ($j = $level_percent + 5, $show_dot = true; $j < $next_level_percent; $j += 5, $show_dot = !$show_dot) {
                    if ($show_dot)
                        $level_value .= '<td style="border-bottom:none;width:' . $colwidth . '%; text-align: left;"> ' . '. </td>';
                    else {
                        $score_val = get_score($j, $category->max_points);
                        $level_value .= '<td style="border-bottom:none;width:' . $colwidth . '%; text-align: left;"> ' . $score_val . ' </td>';
                    }
                }

                if ($next_level_percent == 100)
                    $level_value .= '<td style="border-bottom:none;text-align: right">' . intval($category->max_points) . '</td>';

                $level_value .= '</tr></table>';

                $level_value .= '<br></td>';
            }


            $form['grade']['details_box']['marking_' . $slider_info['id']] = array(
                '#type' => 'item',
                '#value' => '<table style="width: 100%;">
                                <tr>
                                    
                                    <td colspan="' . sizeof($category_levels) . '" class="change_slider" style="border-bottom:none;"><div id="slider_' . $slider_info['id'] . '" style="background:RGB(255,255,197);border:2px solid #FFBB00 !important;"></div></td>
                                    
                                </tr>
                                <tr>
                                    ' . $level_value . '
                                 <!--    <td style="border-bottom:none; text-align: left;">' . $slider_info['min'] . '</td>
                                    <td style="border-bottom:none"></td>
                                    <td style="text-align: right;border-bottom:none;">' . clean_score($slider_info['max']) . '</td> -->
                                </tr>
                              </table>',
                '#prefix' => '</tr><tr><th>Score : <br><br> <!-- <span  style="border-style:solid; border-color:orange; padding-top: 3px; padding-right: 8px; padding-bottom: 3px; padding-left: 8px; background:#DBE6EC; font-size: 120%; color:#111111;" id="slider_value_' . $category->category_ID . '"> </span> --></th><td colspan="4"> ',
                '#suffix' => '</td></tr>',
            );
/* score filling same categoery id mutlitple times */
            $form['grade']['details_box']['score_' . $category->category_ID] = array(
                '#type' => 'hidden',
                '#name' => 'scores[' . $category->category_ID . ']',
                '#value' => isset($oldPostObjArray[$category->category_ID]) ? intval($oldPostObjArray[$category->category_ID]->score) : 0,
                '#id' => 'score_' . $category->category_ID,
            );

            $form['grade']['details_box']['comment_' . $category->category_ID] = array(
                '#type' => 'textarea',
                '#value' => isset($oldPostObjArray[$category->category_ID]) ? $oldPostObjArray[$category->category_ID]->comments : '',
                '#name' => 'comments[' . $category->category_ID . ']',
                '#prefix' => '<tr><th>Comments :</th><td colspan="4">',
                '#suffix' => '</td></tr>',
                '#required' => TRUE,
                '#wysiwyg' => FALSE,
//                '#required' => (isset($activityRubricObj->require_stu_comments) && $activityRubricObj->require_stu_comments == 1) ? TRUE : FALSE ,
            );
            $form['grade']['details_box']['spacer_' . $category->category_ID] = array(
                '#type' => 'item',
                '#value' => '',
                '#prefix' => '<tr><td colspan="5" style="border-left:1px solid #fff !important;border-right:1px solid #fff !important;">',
                '#suffix' => '</td></tr>',
            );
        }
    }
    $form['grade']['details_box']['private_comment'] = array(
        '#type' => 'textarea',
        '#value' => $oldPostTeacherObj ? $oldPostTeacherObj->comment_to_teacher : '',
        '#prefix' => '<tr><td style="border-right:1px solid #9ECCE9;color:#3570AA;font-weight:bold;text-align:right"> Private Comments to Teacher :</td><td colspan="4">',
        '#suffix' => '</td></tr></table>',
        '#wysiwyg' => FALSE,
    );
    if ($submission_nid) {
        $form['submission_nid'] = array(
            '#type' => 'hidden',
            '#value' => $submission_nid,
        );
    }

    drupal_add_js($slider_load_script, 'inline');

    return $form;
}

function ntlp_rubric_activity_evaluate_validate($form, $form_state) {


    $require_stu_comments = $form_state['clicked_button']['#post']['require_stu_comments'];

    $scores = $form_state['clicked_button']['#post']['scores'];
    $comments = $form_state['clicked_button']['#post']['comments'];

    if ($require_stu_comments == 'Yes') {
        if ($scores != null) {
            foreach ($scores as $catID => $score) {
                if ($comments[$catID] == '' || empty($comments[$catID])) {
                    form_set_error('', t('Please complete all comment sections prior to submitting this evaluation.'));
                }
            }
        }
    }
}

function get_rubric_categories_records($activity_nid) {

    $result = db_query("SELECT * FROM {ntlp_course_activity} ca
        INNER JOIN {ntlp_course_activity_rubric} car ON car.activity_nid = ca.nid
        INNER JOIN {ntlp_activity_rubric_category} arc ON arc.activity_nid = ca.nid
        WHERE ca.nid = %d", $activity_nid);

    if ($result->num_rows > 0) {

        while ($data = db_fetch_object($result)) {
            $categories[$data->category_order - 1] = $data;
        }
        return $categories;
    } else {
        return false;
    }
}

function get_rubric_categories_levels_records($category_id) {

    $result = db_query("SELECT * FROM {ntlp_activity_rubric_level_category} arlc
        INNER JOIN {ntlp_activity_rubric_level} arl ON arl.level_ID = arlc.level_ID
        WHERE arlc.category_ID = %d", $category_id);

    if ($result->num_rows > 0) {

        while ($data = db_fetch_object($result)) {
            $category_level[$data->level_order - 1] = $data;
        }
        return $category_level;
    } else {
        return false;
    }
}

function ntlp_rubric_activity_evaluate_submit($form, $form_state) {
    global $user;

    $scores = $form_state['clicked_button']['#post']['scores'];
    $comments = $form_state['clicked_button']['#post']['comments'];
    $course_id = $form_state['clicked_button']['#post']['course_id'];
    $activity_nid = $form_state['clicked_button']['#post']['activity_id'];
    $feedback_for_uid = $form_state['clicked_button']['#post']['feedback_for_uid'];
    $feedback_by_uid = $form_state['clicked_button']['#post']['feedback_by_uid'];
    $private_comment = $form_state['clicked_button']['#post']['details_box']['private_comment'];
    $is_edit_mode = isset($form_state['clicked_button']['#post']['submission_nid']);
    $evaluation_id = $is_edit_mode ? $form_state['clicked_button']['#post']['submission_nid'] : false;
    $next_user_id = $form_state['clicked_button']['#post']['info']['next_user_id'];

    $dateformat = "%Y-%m-%d %H:%M:%S";
    $time = strftime($dateformat, strtotime(date('Y-m-d')));

    if (!$is_edit_mode) {
        $evaluation_node = new stdClass();

        //  $sumbmission_node->title = $activityObj->name;
        $evaluation_node->type = 'lo_rubric_submission';   // Your specified content type
        $evaluation_node->created = $time;
        $evaluation_node->changed = $time;
        //  $sumbmission_node->body = $submittedwork;
        $evaluation_node->status = 1;
        $evaluation_node->sticky = 0;
        $evaluation_node->format = 1;       // Filtered HTML
        $evaluation_node->uid = $feedback_by_uid;

        node_save($evaluation_node);

        $evaluation_id = $evaluation_node->nid;
    }

    if ($evaluation_id != null) {

        if ($is_edit_mode) {
            db_query("UPDATE {ntlp_activity_rubric_submission_for} SET
                feedback_for = %d, comment_to_teacher = '%s', exclude_feedback = 0 WHERE submission_nid = %d",
                    $feedback_for_uid, $private_comment, $evaluation_id);
        } else {
            db_query("insert into {ntlp_activity_rubric_submission_for}
                (submission_nid, feedback_for, comment_to_teacher, exclude_feedback)
                values (%d, %d, '%s', 0)", $evaluation_id, $feedback_for_uid, $private_comment);
        }

        //update submission row so that it gets included in the submission count for student
        $activity_obj = get_activity_record($activity_nid);
        $submission_rs = get_user_submission($activity_obj->school_term_tid, $activity_nid, $feedback_by_uid, true);
        
        if ($submission_rs->num_rows == 0 || !isset($submission_rs->nid)) {
            $sumbmission_node = create_activity_submission(
                    $activity_obj->school_term_tid, $activity_obj->course_nid, $activity_nid, 
                    'lo_rubric_submission', '', $feedback_by_uid, '', 1, '1.0000', false);
        } else {
            $submission_obj = db_fetch_object($submission_rs);
            db_query("UPDATE {ntlp_activity_submission} SET
                date_opened = '%s', version_num = '1.0000' WHERE nid = %d",
                    $time, $submission_obj->nid);
        }

        if ($scores != null) {
            foreach ($scores as $catID => $score) {
                if ($is_edit_mode) {
                    db_query("UPDATE {ntlp_activity_rubric_submission} SET
                    score = %d, comments = '%s', feedback_for = %d WHERE submission_nid = %d AND category_id = %d",
                            $score, $comments[$catID], $feedback_for_uid, $evaluation_id, $catID);
                } else {
                    db_query("INSERT INTO {ntlp_activity_rubric_submission}
                    (submission_nid,category_id, score, comments, feedback_for)
                    VALUES (%d, %d, %d, '%s', '%s')", $evaluation_id, $catID, $score, $comments[$catID], $feedback_for_uid);
                }
            }
        }
    }

    $op = $form_state['clicked_button']['#post']['op'];
//    drupal_set_message('Student Evaluated Successfully');
    if (strrpos($op, "Close"))
        drupal_goto('ntlp/courses/activity/view/' . $course_id . '/' . $activity_nid, array('tab' => 'work'));
    else {
        drupal_goto('ntlp/courses/activity/student/evaluate/' . $course_id . '/' . $activity_nid . '/' . $next_user_id);
    }
}

function get_rubric_activity_categories($activity_nid) {
    $flag = false;

    $result = db_query("SELECT * FROM dpl_ntlp_activity_rubric_category arc
        WHERE arc.activity_nid = %d", $activity_nid);

    if ($result->num_rows > 0) {

        while ($data = db_fetch_object($result)) {
            $flag[] = $data;
        }
    } else {
        $flag = false;
    }

    watchdog("categories", print_r($flag, true));
    return $flag;
}

function get_rubric_activity_category_level($category_id) {

    $flag = false;

    $result = db_query("SELECT * FROM {ntlp_activity_rubric_level_category} arlc
        INNER JOIN {ntlp_activity_rubric_level} arl ON arl.level_ID = arlc.level_ID
        WHERE arlc.category_ID = %d", $category_id);

    if ($result->num_rows > 0) {

        while ($data = db_fetch_object($result)) {
            $flag[] = $data;
        }
    } else {
        $flag = false;
    }
    watchdog("level", print_r($flag, true));
    return $flag;
}

function get_this_activity_levels_size($activity_nid) {
    $result = db_result(db_query("SELECT COUNT(level_ID) FROM {ntlp_activity_rubric_level}
        WHERE activity_nid = %d", $activity_nid));

    return $result;
}

function get_score($percent_val, $max_score) {

    return intval($max_score * $percent_val / 100);
}
