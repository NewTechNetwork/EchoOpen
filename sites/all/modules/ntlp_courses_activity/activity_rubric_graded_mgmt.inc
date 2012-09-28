<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
require_once "data_access.inc";

function ntlp_courses_activity_rubric_graded_form($form_state, $course_id, $activity_id) {
    global $user, $base_path;
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/rubric_activity.js');
    include("activity_template.inc");

    $user_id = check_plain(arg(7));

    set_item_url_handler('Activity Grade');
    $rubric_obj = get_rubric_activity_records($activity_id);

    $selected_coursetype = $rubric_obj->type;
    $course_id = $rubric_obj->course_nid;
    $graded_by_uid = $user->uid;

    $submission_record = get_rubric_user_submission_grade($rubric_obj->school_term_tid, $activity_id, $user_id);


    if ($rubric_obj != null) {

        $student_info = get_activity_username($user_id);

        $picture = $student_info->picture;

        if ($picture == null) {

            $picture = $base_path . 'themes/Boldr/Images/common/small_person_image_not_found.jpg';
        } else {

            $picture = $base_path . $picture;
        }

        $form['activity_nid'] = array(
            '#type' => 'hidden',
            '#id' => 'activity_nid',
            '#value' => $activity_id,
            '#prefix' => '<div style="display:none">',
            '#suffix' => '</div>',
        );

        $form['course_nid'] = array(
            '#type' => 'hidden',
            '#id' => 'course_nid',
            '#value' => $course_id,
            '#prefix' => '<div style="display:none">',
            '#suffix' => '</div>',
        );


        $form['submission_id'] = array(
            '#type' => 'hidden',
            '#id' => 'submission_id',
            '#value' => $submission_record->nid,
            '#prefix' => '<div style="display:none">',
            '#suffix' => '</div>',
        );

        $form['user_id'] = array(
            '#type' => 'hidden',
            '#id' => 'user_id',
            '#value' => $user_id,
            '#prefix' => '<div style="display:none">',
            '#suffix' => '</div>',
        );


        $form['name'] = array(
            '#type' => 'fieldset',
            '#title' => '',
            '#tree' => TRUE, // Don't forget to set #tree!
            '#prefix' => new_activity_title($rubric_obj->course_nid),
            '#suffix' => $HTMLform_activity_outer_name_ftr,
        );


        $form['name']['activity_name'] = array(
            '#type' => 'item',
            '#value' => $rubric_obj->title,
            '#prefix' => '<div style="float:left"><table cellpadding="0"  cellspacing="0"><tr><td style="padding:0px"><div class="Heading"><b>' . t("Grade Student Submission:") . '&nbsp;</b></div></td><td style="padding:0px;"><h6 style="padding:0px;margin:0px;font-weight:normal;font-size:18px;">',
            '#suffix' => '</h6></td></tr></table></div>',
        );


        $form['info'] = array(
            '#type' => 'fieldset',
            '#title' => '',
            '#tree' => TRUE, // Don't forget to set #tree!
            '#prefix' => $HTMLform_activity_title_hdr,
            '#suffix' => $HTMLform_activity_title_ftr,
        );

        $form['info']['title'] = array(
            '#type' => 'item',
            '#prefix' => '	<tr><td>',
            '#suffix' => '</td></tr>',
        );


        $form['info']['activity_type'] = array(
            '#type' => 'item',
            '#size' => '40',
            '#value' => get_activity_name($rubric_obj->type),
            '#prefix' => '<tr><td style="border:none;"><table cellpadding="8" cellspacing="0" style="border-top:none;" border="0" width="822px" class="table"><tr><th style="width:95px;"> ' . t('Type') . '</th><td style="width:322px;">',
            '#suffix' => '</td>',
        );

        $dateformat = "l, m/d/Y | h:i:s a";


        if ($rubric_obj->due_date != '0000-00-00 00:00:00') {
            $activity_due_date = get_tz_course($course_id, $dateformat, $rubric_obj->due_date);
        } else {
            $activity_due_date = 'Fix';
        }

        //$duedate = date($dateformat, strtotime($rubricObj->due_date));
//        $duedate = get_tz_course($course_id, $dateformat, ($rubricObj->due_date));

        $form['info']['due_date'] = array(
            '#type' => 'item',
            '#value' => $activity_due_date,
            '#prefix' => '<th style="width:127px"> ' . t('Due Date:') . '</th><td>',
            '#suffix' => '</td>',
        );

        $form['info']['activity_description'] = array(
            '#type' => 'item',
            '#value' => $rubric_obj->body,
            '#prefix' => '<tr><th> ' . t('Description') . '</th><td colspan="3"><div style="overflow:auto;max-height:500px;background-color:#fff">',
            '#suffix' => '</div></td></tr></table>',
        );

        $form['assessment'] = array(
            '#type' => 'fieldset',
            '#title' => '',
            '#prefix' => $HTMLform_grade_fieldset_1_start,
            '#suffix' => $HTMLform_grade_fieldset_1_end,
        );

        $form['assessment']['performance'] = array(
            '#type' => 'fieldset',
            '#title' => '',
            '#tree' => TRUE, // Don't forget to set #tree!
            '#prefix' => $HTMLform_performance_fielset_2_start,
            '#suffix' => $HTMLform_performance_fielset_2_end,
        );

        $form['assessment']['performance']['name_table'] = array(
            '#type' => 'item',
            '#prefix' => '<div style="float:left;"><img src="' . $picture . '" height="25px" width="25px" /></div><div style="float:left; padding:5px 0 0 15px" class="Activities_Submission_Name" align="left"><h1>' . $student_info->first_name . '&nbsp;' . $student_info->last_name . '</h1></div>',
            '#suffix' => '',
        );

        $form['assessment']['details_box'] = array(
            '#type' => 'fieldset',
            '#title' => '',
            '#tree' => TRUE, // Don't forget to set #tree!
            '#prefix' => $HTMLform_teacher_textarea_title_hdr . '<div class="ActivitiesAssignment">',
            '#suffix' => '</div>' . $HTMLform_teacher_textarea_title_ftr,
        );

        $form['assessment']['details_box']['assessment_heading'] = array(
            '#type' => 'item',
            '#prefix' => '<table cellpadding="0" cellspacing="0"><tr><td align="left" style="padding:0 0 5px 0;border-bottom:none"><div class="Heading"><b style="font-size:15px;color:#3570AA">Assessment',
            '#suffix' => '</b></div></td></tr>',
        );

        $form['assessment']['details_box']['outcomes'] = array(
            '#type' => 'item',
            '#prefix' => '<tr><td style="border-bottom:none;">' . $HTMLform__activity_rubric_hdr . '<div style="display:table;border-top:1px solid #9ECCE9;border-right:1px solid #9ECCE9">',
            '#suffix' => '</div></div></div></div>' . $HTMLform_activity_rubric_ftr . '</td></tr>',
        );

        $school_nid = get_this_school();

        # It will Fetch All Outcome Tid's Matching With course ID
        $outcome_array = get_all_outcomes($course_id);

        # It will Fetch Selected Outcome Tid's Matching With Activity ID
        $selected_array = get_selected_outcomes_tid($activity_id);

        if ($outcome_array != null) {
            foreach ($outcome_array as $outcome_obj) {

                if ($selected_array != null) {

                    if (in_array($outcome_obj->tid, $selected_array)) {

                        $grade_outcomes = get_assignment_outcomes($outcome_obj->tid, $activity_id, $user_id);
                        $select_max_points = get_selected_max_points($outcome_obj->tid, $activity_id);

                        $form['assessment']['details_box']['outcomes']['outcome_' . $outcome_obj->tid] = array(
                            '#name' => 'outcome_' . $outcome_obj->tid,
                            '#name' => 'out[' . $outcome_obj->tid . ']',
                            '#type' => 'textfield',
                            '#id' => 'outcome_' . $outcome_obj->tid,
                            '#size' => 3,
                            '#value' => isset($submission_record->nid) ? clean_score($grade_outcomes->points) : '',
                            '#field_suffix' => ($select_max_points->max_points == null) ? '' : t('/ ' . $select_max_points->max_points),
                            '#prefix' => "<div style='float:left;'><table cellpadding='8' cellspacing='0' border='0' ><tr><th style='width:137px'>" . $outcome_obj->name . "</th><td style='width:220px'>",
                            '#suffix' => '</td></tr></table></div>',
                        );

                        if ($outcome_obj->tid == $rubric_obj->outcome_tid) {

                            $form['assessment']['details_box']['outcomes']['selected_outcome'] = array(
                                '#type' => 'hidden',
                                '#id' => 'selected_outcome',
                                '#value' => $select_max_points->max_points,
                                '#prefix' => '<div style="display:none">',
                                '#suffix' => '</div>',
                            );
                        }
                    } else {

                        $form['assessment']['details_box']['outcomes']['outcome_' . $outcome_obj->tid] = array(
                            '#name' => 'outcome_' . $outcome_obj->tid,
                            '#name' => 'out[' . $outcome_obj->tid . ']',
                            '#type' => 'textfield',
                            '#size' => 3,
                            '#id' => 'outcome_' . $outcome_obj->tid,
                            '#value' => '',
                            '#field_suffix' => ' / 0',
                            '#prefix' => "<div style='float:left;'><table cellpadding='8' cellspacing='0' border='0' ><tr><th style='width:137px'>" . $outcome_obj->name . "</th><td style='width:220px'>",
                            '#suffix' => '</td></tr></table></div>',
                        );

                        if ($outcome_obj->tid == $rubric_obj->outcome_tid) {

                            $form['assessment']['details_box']['outcomes']['selected_outcome'] = array(
                                '#type' => 'hidden',
                                '#id' => 'selected_outcome',
                                '#value' => $select_max_points->max_points,
                                '#prefix' => '<div style="display:none">',
                                '#suffix' => '</div>',
                            );
                        }
                    }
                } else {

                    $form['assessment']['details_box']['outcomes']['outcome_' . $outcome_obj->tid] = array(
                        '#name' => 'outcome_' . $outcome_obj->tid,
                        '#name' => 'out[' . $outcome_obj->tid . ']',
                        '#type' => 'textfield',
                        '#size' => 3,
                        '#id' => 'outcome_' . $outcome_obj->tid,
                        '#value' => '',
                        '#field_suffix' => ' / 0',
                        '#prefix' => "<div style='float:left;'><table cellpadding='8' cellspacing='0' border='0' ><tr><th style='width:200px'>" . $outcome_obj->name . "</th><td style='width:157px'>",
                        '#suffix' => '</td></tr></table></div>',
                    );
                    $form['assessment']['details_box']['outcomes']['selected_outcome'] = array(
                        '#type' => 'hidden',
                        '#id' => 'selected_outcome',
                        '#value' => empty($select_max_points->max_points) ? 0 : $select_max_points->max_points,
                        '#prefix' => '<div style="display:none">',
                        '#suffix' => '</div>',
                    );
                }
            }
        }



        $form['assessment']['details_box']['outcomes']['Teacher_comments_heading'] = array(
            '#type' => 'textarea',
            '#value' => isset($submission_record->nid) ? $submission_record->teachers_notes : '',
            '#prefix' => "<div style='clear:both'><table cellpadding='8' cellspacing='0' width='100%' border='0'><th style = 'width: 200px;'>" . t('Teacher Comments:') . "</th><td>",
            '#suffix' => '</td></tr></table></div>',
        );

        $form['assessment']['details_box']['submit_grade'] = array(
            '#type' => 'submit',
            '#value' => t('Submit Grade'),
            '#executes_submit_callback' => TRUE,
            '#attributes' => array('class' => 'AddOutComesBtn'),
            '#submit' => array('ntlp_course_activity_rubric_graded_submit'),
            '#prefix' => "<tr><td style='border-bottom:none'<div><table cellpadding='0' cellspacing='0' align='right' border='0'><tr><td style='border-bottom:none'>",
            '#suffix' => '</td></tr></table></div></td></tr></table>',
        );


        $form['feedback_i_received'] = array(
            '#type' => 'item',
            '#value' => '',
            '#prefix' => '<div id="ActivitiesCSS"  style="float:left"><table cellpadding="0"  cellspacing="0"><tr><td style="padding:0px;"><div class="Heading" style="font-size:14px;color:#3570aa;"><b>' . t("Feedback Received") . '&nbsp;</b>',
            '#suffix' => '</div></td></tr></table></div><br clear="all" />',
        );

        $form['feedback_grade'] = array(
            '#type' => 'fieldset',
            '#title' => '',
            '#prefix' => $HTMLform_grade_fieldset_1_start,
            '#suffix' => $HTMLform_grade_fieldset_1_end,
        );


        $form['feedback_grade']['feedback_performance'] = array(
            '#type' => 'fieldset',
            '#title' => '',
            '#tree' => TRUE, // Don't forget to set #tree!
            '#prefix' => $HTMLform_performance_fielset_2_start,
            '#suffix' => $HTMLform_performance_fielset_2_end,
        );

        $form['feedback_grade']['feedback_details_box'] = array(
            '#type' => 'fieldset',
            '#title' => '',
            '#tree' => TRUE, // Don't forget to set #tree!
            '#prefix' => $HTMLform_teacher_textarea_title_hdr,
            '#suffix' => $HTMLform_teacher_textarea_title_ftr,
        );

        $form['feedback_grade']['feedback_details_box']['evaluation_content'] = array(
            '#type' => 'item',
            '#title' => '',
            '#tree' => TRUE, // Don't forget to set #tree!
        );


        $form['feedback_grade']['feedback_details_box']['evaluation_content']['overall_score_heading'] = array(
            '#type' => 'item',
            '#prefix' => '<table width="100%"><tr><td style="width:25%">' . $HTMLform_rubric_activity_title_hdr . '<div style="height:186px"><table width="100%" style="border:none" class="table" cellpadding="8" cellspacing="0"><tr><th style="text-align:center" > Average Overall Score',
            '#suffix' => '</th></tr>',
        );

        $overall_points = get_graded_rubric_overall_avg($user_id, $activity_id);


        $form['feedback_grade']['feedback_details_box']['evaluation_content']['overall_scores'] = array(
            '#type' => 'item',
            '#value' => $overall_points['avg_cat_feedback_points'] . '/' . $overall_points['avg_cat_max_points'],
            '#prefix' => '<tr><td valign="top" style="border:none;padding-top:35px;" ><div style="background-color:#FFFFFF;font-size:18px;margin:0 15px;padding:20px 5px;text-align:center">',
            '#suffix' => '</div>',
        );
        $form['feedback_grade']['feedback_details_box']['evaluation_content']['overall_scores_button'] = array(
            '#type' => 'item',
            '#value' => '',
            '#prefix' => '<div class="Activities_btn" style="float:left;padding-left:10px;padding-top:10px"><a onclick="get_average_score(' . $overall_points['avg_cat_max_points'] . ',' . $overall_points['avg_cat_feedback_points'] . ',' . $rubric_obj->outcome_tid . ')" > Convert to Points </a>',
            '#suffix' => '</div></td></tr></table></div>' . $HTMLform_rubric_activity_title_ftr . '</td>',
        );

        $form['feedback_grade']['feedback_details_box']['evaluation_content']['average_cat_scores'] = array(
            '#type' => 'item',
            '#prefix' => '<td style="width:5px">&nbsp;</td><td valign="top">' . $HTMLform_rubric_activity_title_hdr . '<div style="height:186px"><table cellpadding="8" class="table" width="100%"><tr><th style="text-align:center">Average Category Scores</th></tr><tr><td style="padding-top:18px;padding-bottom:18px;"><div style="border:1px solid #9ECCE9;display:table">',
            '#suffix' => '</div></td></tr></table></div>' . $HTMLform_rubric_activity_title_ftr . '</td></tr><tr><td colspan="3" style="padding:0px;font-size:5px">&nbsp;</td></tr></table>',
        );


        $categories = get_rubric_categories_score($activity_id);

        if ($categories != null || !empty($categories)) {

            $catsRs = get_rubric_submission_cat_avg_score($user_id, $activity_id);
            while ($cat = db_fetch_object($catsRs)) {
                $form['feedback_grade']['feedback_details_box']['evaluation_content']['average_cat_scores']['average_cat_scores' . $cat->category_ID] = array(
                    '#type' => 'item',
                    '#value' => round($cat->score) . '/' . floor($cat->max_points),
                    '#prefix' => "<div style='float:left;'><table cellpadding='6' cellspacing='0' border='0'><tr><th style='width:115px'>" . $cat->category_name . "</th><td width='140px'>",
                    '#suffix' => '</td></tr></table></div>',
                );
            }
        }

        $form['feedback_grade']['feedback_details_box']['evaluation_content']['rudric_data'] = array(
            '#type' => 'fieldset',
            '#value' => '',
            '#prefix' => $HTMLform_rubric_activity_title_hdr,
            '#suffix' => $HTMLform_rubric_activity_title_ftr,
        );

        $submission_ids = get_rubric_graded_submisssions($activity_id, $user_id);

        if ($submission_ids != null || !empty($submission_ids)) {
            $i = 1;
            foreach ($submission_ids as $submission_id) {

                $user_info = get_activity_username($submission_id->uid);

                $picture = $user_info->picture;

                if ($picture == null) {

                    $picture = $base_path . 'themes/Boldr/Images/common/small_person_image_not_found.jpg';
                } else {

                    $picture = $base_path . $picture;
                }

                $rs = get_user_feedback($activity_id, $user_id, $submission_id->submission_nid);

                $form['feedback_grade']['feedback_details_box']['evaluation_content']['rudric_data'][$i] = array(
                    '#type' => 'item',
                    '#value' => '<div style="float:left"><img src="' . $picture . '" width="30" /></div><div style="float:left;padding:5px">' . $user_info->first_name . ' ' . $user_info->last_name . '</div>',
                    '#prefix' => '<table cellpadding="5" class="table"><tr><td colspan="2" style="border-bottom:1px solid #D5E9F5;border-left:1px solid #D5E9F5;border-right:1px solid #D5E9F5"><b>',
                    '#suffix' => '</b></td>',
                );

                $form['feedback_grade']['feedback_details_box']['evaluation_content']['rudric_data']['checkbox' . $i] = array(
                    '#type' => 'checkbox',
                    '#name' => 'exclude_entry[' . $submission_id->submission_nid . ']',
                    '#value' => ($submission_id->exclude_feedback == 1) ? True : False,
                    '#prefix' => '<td style="text-align:right;width:250px;border-bottom:1px solid #D5E9F5;border-left:1px solid #D5E9F5;border-right:1px solid #D5E9F5" class="checkbox">',
                    '#suffix' => '<div>exclude this entry</div></td></tr>',
                );


                $form['feedback_grade']['feedback_details_box']['evaluation_content']['rudric_data']['headingss' . $i] = array(
                    '#type' => 'item',
                    '#prefix' => '<tr><th>&nbsp;</th><th style="text-align:left;border-left:1px solid #D5E9F5">Comments</th><th style="text-align:center;border-left:1px solid #D5E9F5">Score</th>',
                    '#suffix' => '</tr>',
                );

                $form['feedback_grade']['feedback_details_box']['evaluation_content']['rudric_data']['submission_ids' . $i] = array(
                    '#type' => 'hidden',
                    '#name' => 'ids[]',
                    '#value' => $submission_id->submission_nid,
                    '#prefix' => '<div style="display:none">',
                    '#suffix' => '</div>',
                );

                while ($feedbackData = db_fetch_object($rs)) {

                    $form['feedback_grade']['feedback_details_box']['evaluation_content']['rudric_data'][$feedbackData->category_ID . '_CateogryName_' . $submission_id->submission_nid] = array(
                        '#type' => 'item',
                        '#value' => '',
                        '#prefix' => '<tr><th width="138px">' . $feedbackData->category_name . '</th>
                                <td width="70%"><div>' . $feedbackData->comments . '</div></td>
                                    <td style="text-align:center;border-left:1px solid #9ECCE9"><div>' . round($feedbackData->score) . '/' . floor($feedbackData->max_points) . '</div></td>',
                        '#suffix' => '</tr>',
                    );


                    $totalfeedback_recieved += $feedbackData->score;
                    $totalcategory_points += $feedbackData->max_points;


                    $private_comment = $feedbackData->comment_to_teacher;
                }

                $form['feedback_grade']['feedback_details_box']['evaluation_content']['rudric_data']['total_points' . $i] = array(
                    '#type' => 'item',
                    '#value' => 'Total&nbsp;&nbsp;' . $totalfeedback_recieved . '/' . $totalcategory_points,
                    '#prefix' => '<td colspan="3" style="text-align:right;background-color:#ED7B23;color:#fff"><b>',
                    '#suffix' => '</b></td></tr>',
                );


                $form['feedback_grade']['feedback_details_box']['evaluation_content']['rudric_data']['private_comment_' . $i] = array(
                    '#type' => 'item',
                    '#value' => $private_comment,
                    '#prefix' => '<tr><th><h3 style="padding: 0px; margin: 0pt 0pt 5px; font-size: 12px; font-weight: bold; color:#3570AA;">Private Comments to Teacher</h3></th><td colspan="2"><div>',
                    '#suffix' => '</div></td></tr></table>',
                );


                $totalfeedback_recieved = 0;
                $totalcategory_points = 0;

                $i++;
            }
        }
        /* $form['feedback_grade']['feedback_details_box']['feedback_submitted_heading'] = array(
          '#type' => 'item',
          '#prefix' => $HTMLform_rubric_activity_title_hdr . '<table width="100%"><tr><td style="border:none" colspan="3"><b style="font-size:15px;color:#3570AA">Feedback Submitted</b>',
          '#suffix' => '</td></tr>'
          );
          $form['feedback_grade']['feedback_details_box']['feedback_submitted'] = array(
          '#type' => 'item',
          '#prefix' => '<tr><td style="border:none" colspan="3"><div style="overflow: auto; height: 30px; background-color: rgb(255, 255, 255);">',
          '#suffix' => '</div></td></tr>',
          );
          $form['feedback_grade']['feedback_details_box']['table_end'] = array(
          '#type' => 'item',
          '#suffix' => '</table>' . $HTMLform_rubric_activity_title_ftr,
          ); */

        return $form;
    } else {

        drupal_set_message('No Activity Found ');
    }
}

function ntlp_course_activity_rubric_graded_submit($form, $form_state) {
    global $user;

    $outcomes = $form_state['clicked_button']['#post']['out'];
    $course_nid = $form_state['clicked_button']['#post']['course_nid'];

    $activity_nid = $form_state['clicked_button']['#post']['activity_nid'];
    $user_id = $form_state['clicked_button']['#post']['user_id'];
    $teacher_comment = $form_state['clicked_button']['#post']['details_box']['outcomes']['Teacher_comments_heading'];
    $exclude_entry = $form_state['clicked_button']['#post']['exclude_entry'];

    $submissions_ids = $form_state['clicked_button']['#post']['ids'];

    if (intval($activity_nid) > 0) {

        $grade_id = save_activity_user_grade($activity_nid, $user_id, $teacher_comment, $user->uid);
        save_activity_user_grade_scores($grade_id, $outcomes);

            //TRIGGER_POINT: UPDATE STUDENT RECORD FOR ALL COURSE TERMS
        dw_set_student_course_grade_toupdate($user_id, $course_nid);

            ntlpsubmissionactivity_submit("grade", $activity_nid, $user_id, $user->uid);

        //Save rubric submissions

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

        drupal_set_message('Student Graded Successfully');
        drupal_goto('ntlp/goback');
    } else {
        drupal_set_message('Activity not found');
    }
}

function get_rubric_average_score_and_feedback_teacher($activityObj, $student_uid) {
    global $user;

    $html_individual_student = '<div><table width="100%" cellspacing="0" cellpadding="10" class="tbl">';



    $categories_info = get_rubric_categories($activityObj->nid);
    if ($categories_info != null || !empty($categories_info)) {

        $html_individual_student .= '<tr><th style="border-left:1px solid #fff;background-color:#D6E9F5; text-align:center;font-size: 15px;">Total</th>';
        foreach ($categories_info as $category_data) {
            $html_individual_student .= '<th style="border-left:1px solid #fff;background-color:#D6E9F5; color:#009900;text-align:left">' . $category_data->category_name . '<div>(' . round($category_data->max_points) . ')</div> </th>';
        }
    }
    $html_individual_student .= '</tr>';

    $catsRs = get_rubric_view_submission_cat_avg_score($student_uid, $activityObj->nid);
    if ($catsRs != null || !empty($catsRs)) {
        while ($cat = db_fetch_object($catsRs)) {
            $avg_score[$cat->category_ID][] = round($cat->score);
            $total_score += round($cat->score);
        }
    }

    if ($avg_score != null) {

        $html_individual_student .= '<tr>
                <td style="background-color:#EEEEEE;color:#000000;font-size:15px;font-weight:bold;">' . $total_score . '</td>';
        foreach ($avg_score as $cat_id => $avg) {

            $html_individual_student .= '<td style="background-color:#EEEEEE;">' . round(array_sum($avg) / count($avg)) . '</td>';
        }
        $html_individual_student .= '</tr>';
    } else {
        $html_individual_student .= '<tr>';
        $html_individual_student .= '<td style="background-color:#EEEEEE;color:#000000;font-size:15px;font-weight:bold;"></td>';

        foreach ($categories_info as $category) {
            $html_individual_student .= '<td style="background-color:#EEEEEE;color:#000000;font-size:15px;font-weight:bold;"></td>';
        }
        $html_individual_student .= '</tr>';
    }

    $html_individual_student .= '</table></div>';

    $submission_ids = get_rubric_graded_submisssions($activityObj->nid, $student_uid);

    if (sizeof($submission_ids) > 0) {
        $html_individual_student .= '<div><div style="font-size: 5px;">&nbsp;</div><h3 style="color:#3570AA;font-size:15px;font-weight:bold;margin:0 0 5px;padding:0;">Feedback from Individual Students</h3>';
    } else {
        $html_individual_student .= '<div><div style="font-size: 5px;">&nbsp;</div>';
    }
    if ($submission_ids != null || !empty($submission_ids)) {
        $i = 1;
        foreach ($submission_ids as $submission_id) {

            $user_info = get_activity_username($submission_id->uid);

            $rs = get_user_feedback($activityObj->nid, $student_uid, $submission_id->submission_nid);


            $html_individual_student .= '<div style="font-size: 5px;">&nbsp;</div><div><table cellpadding="0" cellspacing="0" width="100% ">';

            $html_individual_student .= '<tr><th><div class="studentpanel_topleft">
                                      <div style="height: 32px;" class="studentpanel_topright">
                                        <div class="popup_profile">
                                          <div style="float:left;"><img align="left" src="' . is_user_image_exist($user_info->picture) . '" width="30" /></div>
                                          <div style="float:left;">
                                            <h1 style="margin: 7px 0pt;">' . $user_info->first_name . ' ' . $user_info->last_name . '</h1>
                                          </div>
                                          <div style="float: right;margin-top:5px">
                                            <input type="checkbox" name="exclude_entry[' . $submission_id->submission_nid . ']"
                                                    value="' . (($submission_id->exclude_feedback == 1) ? 1 : 0) . '" 
                                                        ' . (($submission_id->exclude_feedback == 1) ? ' checked' : '') . ' >
                                            <label style="color:white;" >Exclude this entry</label>
                                          </div>
                                        </div>
                                      </div>
                                    </div></th></tr></table><div class="studentpanel_middleleft">
                                      <div class="studentpanel_middleright">
                                        <div class="studentpanel_bottomcenter">
                                          <div class="studentpanel_bottomleft">
                                            <div class="studentpanel_bottomright">
                                              <div class="studentpanel_center"><table cellpadding="0" cellspacing="0" width="100%">';
            $html_individual_student .= '<tr><td>';

            $html_individual_student .= '<table  id="student_header" cellpadding="10" class="tbl" cellspacing="0" width="100%" ><thead><tr>';

            $html_individual_student .= '<th style="color:#000000;font-size:15px;">Total</th>';

            $totalcategory_points = 0;
            while ($feedbackData = db_fetch_object($rs)) {
                $html_individual_student .= '<th>' . $feedbackData->category_name . '<br/> (' . round($feedbackData->max_points) . ')</th>';
                $totalcategory_points += $feedbackData->score;
            }
            $html_individual_student .= '</tr></thead>';
            $html_individual_student .= '<tr><td style="background-color:#EEEEEE;color:#000000;font-size:15px;font-weight:bold;">' . $totalcategory_points . '</td>';

            $rs = get_user_feedback($activityObj->nid, $student_uid, $submission_id->submission_nid);

            $comments_to_student .= '<table id="student_header" cellpadding="0" cellspacing="0" width="100%" ><tr>';

            $comments_to_student = '';
            while ($feedbackData = db_fetch_object($rs)) {
                $html_individual_student .= '<td align="center" style="background-color:#EEEEEE;" >' . round($feedbackData->score) . '</td>';

                if (!empty($feedbackData->comments) && $feedbackData->comments != '') {
                    $comments_to_student .= '<tr><td><div style="margin: 2px;">
                                            <div class="lightgray_panel">
                                            <div class="lightgray_topleft">
                                              <div class="lightgray_topright">
                                                <div class="lightgray_bottomleft">
                                                    <div class="lightgray_bottomright"><font style="text-decoration:underline;font-size:13px;font-family:Tahoma">' . $feedbackData->category_name . '</font><br/>' . $feedbackData->comments . ' </div>
                                                </div>
                                              </div>
                                           </div>
                                         </div>
                                         </div></td></tr>';
                } else {
                    $comments_to_student .= '';
                }

                if (!empty($feedbackData->comment_to_teacher) && $feedbackData->comment_to_teacher != '') {
                    $comments_to_teacher = '<tr><td><br><h3 style="font-family:Tahoma;font-size:14px;font-weight:normal;margin:0 0 5px;padding:0;">Private Comments to Teacher</h3></td></tr><tr><td><div style="margin: 2px;">
                                            <div class="lightgray_panel">
                                            <div class="lightgray_topleft">
                                              <div class="lightgray_topright">
                                                <div class="lightgray_bottomleft">
                                                    <div class="lightgray_bottomright">' . $feedbackData->comment_to_teacher . ' </div>
                                                </div>
                                              </div>
                                           </div>
                                         </div>
                                         </div></td></tr>';
                } else {
                    $comments_to_teacher = '';
                }
            }
            $html_individual_student .= '</tr></table>';
            if (!empty($comments_to_student) && $comments_to_student != '') {
                $html_individual_student .= '</td></tr><tr><td>&nbsp;</td></tr><tr><td><h3 style="color:#3570AA;font-size:14px;font-weight:normal;margin:0 0 5px;padding:0;">Comments to Student</h3></td></tr>';
                $html_individual_student .= $comments_to_student;
            } else {
                $html_individual_student .= $comments_to_student;
            }

            $html_individual_student .= $comments_to_teacher;

            $html_individual_student .= '</table></div></div></div></div></div></div></div>';
        }
    }

    $html_individual_student .= '</div>';
    return $html_individual_student;
}

function get_rubric_feedback_submitted_by_user($activityObj, $student_uid) {
    global $user;

    $html_individual_student = '<div><table width="100%" cellspacing="0" cellpadding="10" class="tbl">';

    $categories_info = get_rubric_categories($activityObj->nid);
    if ($categories_info != null || !empty($categories_info)) {

        $html_individual_student .= '<tr><th style="border-left:1px solid #fff;background-color:#D6E9F5; text-align:center;font-size: 15px;">Total</th>';
        foreach ($categories_info as $category_data) {
            $html_individual_student .= '<th style="border-left:1px solid #fff;background-color:#D6E9F5; color:#009900;text-align:left">' . $category_data->category_name . '<div>(' . round($category_data->max_points) . ')</div> </th>';
        }
    }
    $html_individual_student .= '</tr>';

    $catsRs = get_user_submitted_feedback_cat_avg_score($student_uid, $activityObj->nid);
    if ($catsRs != null || !empty($catsRs)) {
        while ($cat = db_fetch_object($catsRs)) {
            $avg_score[$cat->category_ID][] = round($cat->score);
            $total_score += round($cat->score);
        }
    }

    if ($avg_score != null) {

        $html_individual_student .= '<tr>
                <td style="background-color:#EEEEEE;color:#000000;font-size:15px;font-weight:bold;">' . $total_score . '</td>';
        foreach ($avg_score as $cat_id => $avg) {

            $html_individual_student .= '<td style="background-color:#EEEEEE;">' . round(array_sum($avg) / count($avg)) . '</td>';
        }
        $html_individual_student .= '</tr>';
    } else {
        $html_individual_student .= '<tr>';
        $html_individual_student .= '<td  style=border-left:1px solid #fff;background-color: rgb(238, 238, 238);"></td>';

        foreach ($categories_info as $category) {
            $html_individual_student .= '<td style="border-left:1px solid #fff;background-color: rgb(238, 238, 238);"></td>';
        }
        $html_individual_student .= '</tr>';
    }

    $html_individual_student .= '</table></div>';

    $evaluated_submitted_rec = get_user_submitted_evaluate_ids($activityObj->nid, $student_uid);

    if (sizeof($evaluated_submitted_rec)) {
        $html_individual_student .= '<div><div style="font-size: 5px;">&nbsp;</div><h3 style="color:#3570AA;font-size:15px;font-weight:bold;margin:0 0 5px;padding:0;">Feedback on Individual Students</h3>';
    } else {
        $html_individual_student .= '<div><div style="font-size: 5px;">&nbsp;</div>';
    }


    if ($evaluated_submitted_rec != null || !empty($evaluated_submitted_rec)) {
        $i = 1;
        foreach ($evaluated_submitted_rec as $evaluated_rec) {

            $user_info = get_activity_username($evaluated_rec->feedback_for);

            $rs = get_user_submitted_feedback_for_another_user($activityObj->nid, $evaluated_rec->feedback_for, $student_uid, $evaluated_rec->submission_nid);

            $html_individual_student .= '<div style="font-size: 5px;">&nbsp;</div><div><table cellpadding="0" cellspacing="0" width="100% ">';

            $html_individual_student .= '<tr><th style="background-color:#4576A5;text-valign:middle" align="left"><div class="studentpanel_topleft">
                                      <div style="height: 30px;" class="studentpanel_topright">
                                        <div class="popup_profile">
                                          <div><img align="left" src="' . is_user_image_exist($user_info->picture) . '" width="30" /></div>
                                          <div>
                                            <h1 style="margin: 7px 0pt;">' . $user_info->first_name . ' ' . $user_info->last_name . '</h1>
                                          </div>
                                        </div>
                                      </div>
                                    </div></th></tr></table><div class="studentpanel_middleleft">
                                      <div class="studentpanel_middleright">
                                        <div class="studentpanel_bottomcenter">
                                          <div class="studentpanel_bottomleft">
                                            <div class="studentpanel_bottomright">
                                              <div class="studentpanel_center"><table cellpadding="0" cellspacing="0" width="100% ">';
            $html_individual_student .= '<tr><td>';

            $html_individual_student .= '<table cellpadding="10" cellspacing="0" width="100%" class="tbl" ><thead><tr>';

            $html_individual_student .= '<th style="color:#000000;font-size:15px;">Total</th>';

            $totalcategory_points = 0;
            while ($feedbackData = db_fetch_object($rs)) {
                $html_individual_student .= '<th>' . $feedbackData->category_name . '<br/> (' . round($feedbackData->max_points) . ')</th>';
                $totalcategory_points += $feedbackData->score;
            }
            $html_individual_student .= '</tr></thead>';
            $html_individual_student .= '<tr><td style="background-color:#EEEEEE;color:#000000;font-size:15px;font-weight:bold;">' . $totalcategory_points . '</td>';

            $rs = get_user_submitted_feedback_for_another_user($activityObj->nid, $evaluated_rec->feedback_for, $student_uid, $evaluated_rec->submission_nid);

            $comments_to_student .= '<table cellpadding="0" cellspacing="0" width="100%" ><tr>';
            $comments_to_student = '';
            while ($feedbackData = db_fetch_object($rs)) {

                $html_individual_student .= '<td align="center" style="background-color:#EEEEEE;">' . round($feedbackData->score) . '</td>';
                if (!empty($feedbackData->comments) && $feedbackData->comments != '') {
                    $comments_to_student .= '<tr><td><div style="margin: 2px;">
                                            <div class="lightgray_panel">
                                            <div class="lightgray_topleft">
                                              <div class="lightgray_topright">
                                                <div class="lightgray_bottomleft">
                                                    <div class="lightgray_bottomright"><font style="text-decoration:underline;font-size:13px;font-family:Tahoma">' . $feedbackData->category_name . '</font><br/>' . $feedbackData->comments . ' </div>
                                                </div>
                                              </div>
                                           </div>
                                         </div>
                                         </div></td></tr>';
                } else {
                    $comments_to_student .= '';
                }
                if (!empty($feedbackData->comment_to_teacher) && $feedbackData->comment_to_teacher != '') {
                    $comments_to_teacher = '<tr><td><div style="font-size: 5px;">&nbsp;</div><h3 style="color:#3570AA;font-family:Tahoma;font-size:14px;font-weight:normal;margin:0 0 5px;padding:0;">Private Comments to Teacher</h3></td></tr><tr><td><div style="margin: 2px;">
                                            <div class="lightgray_panel">
                                            <div class="lightgray_topleft">
                                              <div class="lightgray_topright">
                                                <div class="lightgray_bottomleft">
                                                    <div class="lightgray_bottomright">' . $feedbackData->comment_to_teacher . ' </div>
                                                </div>
                                              </div>
                                           </div>
                                         </div>
                                         </div></td></tr>';
                } else {
                    $comments_to_teacher = '';
                }
            }
            $html_individual_student .= '</tr></table>';
            if (!empty($comments_to_student) && $comments_to_student != '') {
                $html_individual_student .= '</td></tr><tr><td>&nbsp;</td></tr><tr><td><h3 style="color:#3570AA;font-size:14px;font-weight:normal;margin:0 0 5px;padding:0;">Comments to Student</h3></td></tr>';
                $html_individual_student .= $comments_to_student;
            } else {
                $html_individual_student .= $comments_to_student;
            }

            $html_individual_student .= $comments_to_teacher;

            $html_individual_student .= '</table>  </div>
                                            </div>
                                          </div>
                                        </div>
                                      </div>
                                    </div></div>';
        }
    }

    $html_individual_student .= '</div>';
    return $html_individual_student;
}
