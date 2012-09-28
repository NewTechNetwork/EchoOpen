<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
require_once "data_access.inc";

function ntlp_courses_activity_rubric_view_form($form_state, $course_id, $activity_id) {
    global $user, $base_path;
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/ntlp_courses_activity.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/rubric_activity.js');
    include("activity_template.inc");

//    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/ntlp_courses_activitys_activity.js');
    $user_id = $user->uid;
    $last_url = $_SERVER['HTTP_REFERER'];

    set_item_url_handler('Activity View');

    $rubric_obj = get_rubric_activity_records($activity_id);
    $selected_coursetype = $rubric_obj->type;

    $submission_record = get_rubric_user_submission_grade($rubric_obj->school_term_time, $activity_id, $user_id);


    $user_exists = check_rubric_user_activity($user_id, $activity_id);

    if ($user_exists) {

        if ($rubric_obj != null) {

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
                '#prefix' => '<style>#ActivitiesCSS {width:auto !important;}</style><div style="float:left"><table cellpadding="0"  cellspacing="0"><tr><td style="padding:0px;"><div class="Heading"><b>' . t("Activity:") . '&nbsp;</b></div></td><td style="padding:0px;"><h6 style="padding:0px;margin:0px;font-weight:normal;font-size:18px;">',
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

            $dateformat = "l, m/d/Y | g:i a";

            //$duedate = date($dateformat, strtotime($rubricObj->due_date));
            $duedate = get_tz_course($course_id, $dateformat, ($rubric_obj->due_date));

            $form['info']['due_date'] = array(
                '#type' => 'item',
                '#value' => $duedate,
                '#prefix' => '<th style="width:127px"> ' . t('Due Date:') . '</th><td>',
                '#suffix' => '</td>',
            );

            $form['info']['activity_description'] = array(
                '#type' => 'item',
                '#value' => $rubric_obj->body,
                '#prefix' => '<tr><th> ' . t('Description') . '</th><td colspan="3"><div style="overflow:auto;max-height:500px;background-color:#fff">',
                '#suffix' => '</div></td></tr>',
            );


            $get_unit_topic_name = get_unit_topic_name($rubric_obj->curriculum_unit_tid);

            $form['info']['activity_unit'] = array(
                '#type' => 'item',
                '#value' => $get_unit_topic_name->name,
                '#prefix' => '<tr class="rubric_detail" style="display:none"><th>' . t('Unit/Topic') . '</th><td colspan="3">',
                '#suffix' => '</td>',
            );

            $selected_project = _get_selected_projects($rubric_obj->project_nid);

            $form['info']['activity_associated_project'] = array(
                '#type' => 'item',
                '#value' => $selected_project->title,
                '#prefix' => '<tr class="rubric_detail" style="display:none"><th>' . t('Associated Project') . '</th><td colspan="3">',
                '#suffix' => '</td>',
            );

            $form['info']['activit_academic_standards'] = array(
                '#type' => 'item',
                '#value' => $rubric_obj->curriculum_standards,
                '#prefix' => '<tr class="rubric_detail" style="display:none"><th>' . t('Academic Standards') . '</th><td colspan="3">',
                '#suffix' => '</td>',
            );


            $form['info']['show_hide_button'] = array(
                '#type' => 'item',
                '#prefix' => '',
                '#suffix' => '</table><div style="width:822px" align="right">
                            <span id="hide_close" style="display:none;float:right">
                                <a class="greenLink" href="#" onclick="javascript:hide_detail(); return false">« Close</a>
                            </span>
                            <span style="float:right" id="hide_open">
                                <a class="greenLink" href="#" onclick="javascript:show_detail(); return false">« Open</a>
                            </span></div></td></tr>',
            );

            $form['assessment'] = array(
                '#type' => 'fieldset',
                '#title' => '',
                '#tree' => TRUE, // Don't forget to set #tree!
                '#prefix' => $HTMLform_activity_rubric_hdr,
                '#suffix' => $HTMLform_activity_rubric_ftr,
            );

            $dateformat = "m/d/Y | g:i a";


            $user_Info = get_activity_username($submission_record->graded_by_uid);

//            echo $submission_record->date_graded;
//            echo '<br >'.date($dateformat, strtotime($submission_record->date_graded));

            $graded_date = get_tz_course($course_id, $dateformat, $submission_record->date_graded);

            if ($submission_record->is_graded != 1 || empty($submission_record->is_graded)) {

                $grade_detail = 'Not Graded';
            } else {

                $grade_detail = $graded_date . ' by ' . l($user_Info->first_name . '  ' . $user_Info->last_name, 'ntlp/user/profile/' . $submission_record->graded_by_uid, array('attributes' => array('class' => 'greenLink')));
            }

            $form['assessment']['activity_name'] = array(
                '#type' => 'item',
                '#value' => '',
                '#prefix' => '<div><table cellpadding="0" width="100%"  cellspacing="0"><tr><td style="padding:0px;"><div class="Heading"><b>' . t("Assessment") . '&nbsp;</b></div></td><td style="float:right;font-size:13px;font-weight:bold; padding-right:20px; ">Grade Submitted: ' . $grade_detail,
                '#suffix' => '</td></tr></table></div>',
            );


            $school_nid = get_this_school();

            $form['assessment_info'] = array(
                '#type' => 'fieldset',
                '#title' => '',
                '#tree' => TRUE, // Don't forget to set #tree!
                '#prefix' => $HTMLform_rudic_title_hdr,
                '#suffix' => $HTMLform_rudic_title_ftr,
            );

            $form['assessment_info']['points'] = array(
                '#type' => 'item',
                '#prefix' => '<div class"ActivitiesAssignment"><table cellpadding="8" class="table" cellspacing="0" border="0" width="100%"><th  style="width:141px">Points</th><td colspan="3">', // This is our wrapper div.
                '#suffix' => '</td></table></div>',
            );


            $outcome_array = get_activity_outcome($activity_id);


            if ($outcome_array != null) {

                foreach ($outcome_array as $outcome_obj) {

                    $grade_outcomes = get_assignment_outcomes($outcome_obj->tid, $activity_id, $user_id);

                    $select_max_points = get_selected_max_points($outcome_obj->tid, $activity_id);

                    $form['assessment_info']['points']['outcome_' . $outcome_obj->tid] = array(
                        '#type' => 'item',
                        '#value' => isset($select_max_points->max_points) ? clean_score($grade_outcomes->points) . '/' . $select_max_points->max_points : '',
                        '#prefix' => "<div style='float:left;'><table cellpadding='8' class='table' cellspacing='0' border='0' width='324px'><tr><th style='width:141px'>" . $outcome_obj->name . "</th><td>",
                        '#suffix' => '</td></tr></table></div>',
                    );

                    $total_submission_points += $grade_outcomes->points;
                    $total_acitivty_points += $select_max_points->max_points;
                }
            }

            if ($outcome_array != null) {
                $total_points_percent = $total_submission_points * 100 / $total_acitivty_points;

                $grade = _get_course_grade($total_points_percent, $course_id, "L");
            }


            $form['assessment_info']['grade_points']['grade_heading'] = array(
                '#type' => 'item',
                '#value' => isset($submission_record->nid) ? $grade . '  (' . round($total_points_percent) . ' %)' : '',
                '#prefix' => "<table cellpadding='8' class='table' cellspacing='0' border='0' width='100%'><tr><th style = 'width: 141px;'>" . t('Grade:') . "</th><td colspan='3'>",
                '#suffix' => '</td></tr>',
            );

            $form['assessment_info']['grade_points']['Teacher_comments_heading'] = array(
                '#type' => 'item',
                '#value' => isset($submission_record->nid) ? $submission_record->teachers_notes : '',
                '#prefix' => "<tr><th style = 'width: 141px;border-bottom:none'>" . t('Teacher Comments:') . "</th><td colspan='3' style='border-bottom:none'>",
                '#suffix' => '</td></tr></table>',
            );


            $form['submitted_work'] = array(
                '#type' => 'item',
                '#value' => '',
                '#prefix' => '<div id="ActivitiesCSS"  style="float:left"><table cellpadding="0"  cellspacing="0"><tr><td style="padding:0px;"><div class="Heading" style="font-size:14px;color:#3570aa;"><b>' . t("Submitted Work") . '&nbsp;',
                '#suffix' => '</b></div></td></tr></table></div><br clear="all" />',
            );


            $form['grade'] = array(
                '#type' => 'fieldset',
                '#title' => '',
                '#tree' => TRUE,
                '#prefix' => $HTMLform_grade_fieldset_1_start,
                '#suffix' => $HTMLform_rubric_fieldset_1_end,
            );




            $form['grade']['performance'] = array(
                '#type' => 'fieldset',
                '#title' => '',
                '#tree' => TRUE, // Don't forget to set #tree!
                '#prefix' => $HTMLform_performance_fielset_2_start,
                '#suffix' => $HTMLform_performance_fielset_2_end,
            );

            $school_nid = get_this_school();

            $form['grade']['performance']['pnt_posble'] = array(
                '#type' => 'item',
                '#prefix' => '<div class="leftcorner"><div class="rightcorner" style="height:22px;"><h4 style="font-size:14px;color:#3570aa;float:left; margin:0px; padding:0px">' . t('Feedback for Others'),
                '#suffix' => '</h4>',
            );

            cache_set("pplfndInstructor_". session_id(), $course_additional_teacherids, 'cache', CACHE_TEMPORARY);
            $form['grade']['performance']['add_students'] = array(
                '#type' => 'item',
                '#value' => dlg('Select Instructors', 'ntlp/people_finder/get_rubric_users/' .
                        $school_nid . '/' .
                        $activity_id . '/' .
                        "pplfndInstructor_". session_id() . '/' .
                        NTLP_ROLEID_STUDENT,
                        350, 600),
                '#prefix' => '<div class="peopleFinderBtn" style="float:right !important;margin-right:5px;" id="pplfndInstructor">',
                '#suffix' => '</div></div></div>',
            );

            $form['grade']['performance']['details_box'] = array(
                '#type' => 'fieldset',
                '#title' => '',
                '#tree' => TRUE, // Don't forget to set #tree!
                '#prefix' => $HTMLform_teacher_textarea_title_hdr,
                '#suffix' => $HTMLform_teacher_textarea_title_ftr,
            );



            $form['grade']['performance']['details_box']['person_to_evaluate_heading'] = array(
                '#type' => 'item',
                '#prefix' => '<div id="ActivitiesCSS"> <div class="ActivitiesAssignment"><table width="800" class="table" cellpadding="5" cellspacing="0"><tr><th style="background-color:#D6E9F5; color:#009900;text-align:left" > Person to evaluate',
                '#suffix' => '</th>',
            );


            $form['grade']['performance']['details_box']['evaluate_button'] = array(
                '#type' => 'item',
                '#prefix' => '<th style="border-left:1px solid #fff;background-color:#D6E9F5; color:#009900">',
                '#suffix' => '</th>',
            );

            $form['grade']['performance']['details_box']['status'] = array(
                '#type' => 'item',
                '#prefix' => '<th style="border-left:1px solid #fff;background-color:#D6E9F5; color:#009900;text-align:left">Status',
                '#suffix' => '</th>',
            );

            $categories_info = get_rubric_categories($activity_id);

            if ($categories_info != null || !empty($categories_info)) {

                foreach ($categories_info as $category_data) {

                    $form['grade']['performance']['details_box'][$category_data->category_ID . '_heading'] = array(
                        '#type' => 'item',
                        '#prefix' => '<th style="border-left:1px solid #fff;background-color:#D6E9F5; color:#009900;text-align:left">' . $category_data->category_name . '<div>(' . round($category_data->max_points) . ')</div> ',
                        '#suffix' => '</th>',
                    );
                }
            }


            $form['grade']['performance']['details_box']['total_heading'] = array(
                '#type' => 'item',
                '#prefix' => '<th style="border-left:1px solid #fff;background-color:#D6E9F5; color:#009900;text-align:left">Total',
                '#suffix' => '</th>',
            );

            $form['grade']['performance']['details_box']['delete'] = array(
                '#type' => 'item',
                '#prefix' => '<th style="border-left:1px solid #fff;background-color:#D6E9F5; color:#009900">',
                '#suffix' => '</th></tr>',
            );

            // it will fetch all the users regarding this activity
            $result = get_activity_users($activity_id, "n.last_name, n.first_name");

            // it will fetch all those users whom he has given the feedback
            $feedback_users = get_user_feedback_uids($activity_id, $user_id);

            if ($result->num_rows > 0) {

                while ($course_student = db_fetch_object($result)) {

                    if (isset($feedback_users)) {

                        if (in_array($course_student->user_uid, $feedback_users)) {

                            $user_info = get_activity_username($course_student->user_uid);

                            $picture = $user_info->picture;

                            if ($picture == null) {

                                $picture = $base_path . 'themes/Boldr/Images/common/small_person_image_not_found.jpg';
                            } else {

                                $picture = $base_path . $picture;
                            }


                            $form['grade']['performance']['details_box']['course_student_' . $course_student->user_uid] = array(
                                '#type' => 'item',
                                '#prefix' => '<tr><td style="border-left:1px solid #9ECCE9"><div style="float:left"><img src="' . $picture . '" width="30" /></div><div style="float:left;padding:5px">' . l($course_student->first_name . ' ' . $course_student->last_name, 'ntlp/user/profile/' . $course_student->user_uid),
                                '#suffix' => '</div></td>',
                            );

                            if ($rubric_obj->self_evaluation == 0 && $user_id == $course_student->user_uid) {

                                $evaluation_button = '';
                            } else {

                                $evaluation_button = l('Evaluate', 'ntlp/courses/activity/student/evaluate/' . $course_id . '/' . $activity_id . '/' . $course_student->user_uid, array('attributes' => array('class' => 'AddOutComesBtn')));
                            }
                            $form['grade']['performance']['details_box']['evaluate_button_' . $course_student->user_uid] = array(
                                '#type' => 'item',
                                '#prefix' => '<td style="border-left:1px solid #9ECCE9">' . $evaluation_button,
                                '#suffix' => '</td>',
                            );


                            $check_status = check_evaluation_status($course_student->user_uid, $user_id, $activity_id);

                            if ($check_status) {
                                $status = 'Complete';
                            } else {

                                $status = 'Incomplete';
                            }

                            $form['grade']['performance']['details_box']['status_' . $course_student->user_uid] = array(
                                '#type' => 'item',
                                '#value' => $status,
                                '#prefix' => '<td style="border-left:1px solid #9ECCE9">',
                                '#suffix' => '</td>',
                            );

                            $categories = get_rubric_categories_score($activity_id);

                            $score_rs = get_rubric_submission_cat_score($user_id, $course_student->user_uid, $activity_id);
                            $total_score = 0;
                            if ($score_rs->num_rows > 0) {
                                while ($score = db_fetch_object($score_rs)) {

                                    $form['grade']['performance']['details_box']['cat_score' . $course_student->user_uid . $score->category_ID] = array(
                                        '#type' => 'item',
                                        '#value' => round($score->score),
                                        '#prefix' => '<td style="border-left:1px solid #9ECCE9">',
                                        '#suffix' => '</td>',
                                    );


                                    $avg_score[$score->category_ID][] += $score->score;
//                                   echo '<br> score '.$score->score;
                                    $total_score += $score->score;
                                }
                            } else {

                                $category = get_rubric_categories($activity_id);
                                if ($category != null) {
                                    foreach ($category as $category_name) {

                                        $form['grade']['performance']['details_box']['cat_score' . $course_student->user_uid . $category_name->category_ID] = array(
                                            '#type' => 'item',
                                            '#value' => 0,
                                            '#prefix' => '<td style="border-left:1px solid #9ECCE9">',
                                            '#suffix' => '</td>',
                                        );
                                    }
                                }
                            }

                            $form['grade']['performance']['details_box']['total_' . $course_student->user_uid] = array(
                                '#type' => 'item',
                                '#value' => $total_score,
                                '#prefix' => '<td style="border-left:1px solid #9ECCE9">',
                                '#suffix' => '</td>',
                            );

                            $avgTotalScore += $total_score;

                            $total_score = 0;

                            $form['grade']['performance']['details_box']['delete_' . $course_student->user_uid] = array(
                                '#type' => 'item',
                                '#value' => l('delete', 'ntlp/delete/rubric/user/' . $course_id . '/' . $activity_id . '/' . $course_student->user_uid . '/' . $user_id, array('attributes' => array('class' => 'LightGreenBoxDeletebtn ntlp_tooltip_n'))),
                                '#prefix' => '<td style="border-left:1px solid #9ECCE9" class="views-field-delete-node">',
                                '#suffix' => '</td></tr>',
                            );
                        }
                    }
                }
            }


            $form['grade']['performance']['details_box']['avaerage_school'] = array(
                '#type' => 'item',
                '#value' => '',
                '#prefix' => '<tr><th>Average Score Given:',
                '#suffix' => '</th><td colspan="2">&nbsp;</td>',
            );

            $i = 0;


            if ($avg_score != null) {
                foreach ($avg_score as $cat_id => $avg) {

                    $form['grade']['performance']['details_box']['avaerage_school_' . $i] = array(
                        '#type' => 'item',
                        '#value' => round(array_sum($avg) / count($avg)),
                        '#prefix' => '<td style="border-left:1px solid #9ECCE9;border-right:1px solid #9ECCE9"><b>',
                        '#suffix' => '</b></td>',
                    );


                    $user_avg_score[$cat_id] = round(array_sum($avg) / count($avg));

                    $i++;
                }
            }

            $form['grade']['performance']['details_box']['avaerage_total_score'] = array(
                '#type' => 'item',
                '#value' => ($avgTotalScore != null) ? round($avgTotalScore / count($avg)) : '',
                '#prefix' => '<td><b>',
                '#suffix' => '</b></td>',
            );



            $form['grade']['performance']['details_box']['table_end'] = array(
                '#type' => 'item',
                '#value' => '',
                '#suffix' => '</tr></table></div></div><br clear="all" />',
            );


            $form['grade']['feedback_i_received'] = array(
                '#type' => 'item',
                '#value' => '',
                '#prefix' => '<br clear="all" /><div class="leftcorner"><div class="rightcorner" style="border:none"><div><table cellpadding="0" width="810px"  cellspacing="0"><tr><td style="padding:0px;"><div class="Heading" style="font-size:14px;color:#3570aa;"><b>' . t("Feedback I've Received") . '&nbsp;</b>',
                '#suffix' => '</div></td> <td class="Project-Btns">' . l('Print', '', array('attributes' => array('onclick' => 'javascript: window.print(); return false;'))) . '</tr></table></div></div></div>',
            );

//            $form['grade']['feedback_grade'] = array(
//                    '#type'   => 'fieldset',
//                    '#title'  => '',
//                    //  '#prefix' => $HTMLform_grade_fieldset_1_start,
////                    '#suffix' => $HTMLform_rubric_fieldset_1_end,
//            );
//
//            $form['grade']['feedback_grade']['feedback_performance'] = array(
//                    '#type'   => 'fieldset',
//                    '#title'  => '',
//                    '#tree'   => TRUE, // Don't forget to set #tree!
//                    '#prefix' => $HTMLform_performance_fielset_2_start,
//                    '#suffix' => $HTMLform_performance_fielset_2_end,
//            );

            $form['grade']['feedback_details_box'] = array(
                '#type' => 'fieldset',
                '#title' => '',
                '#tree' => TRUE, // Don't forget to set #tree!
                '#prefix' => $HTMLform_teacher_textarea_title_hdr,
                '#suffix' => $HTMLform_teacher_textarea_title_ftr,
            );

//            $form['grade']['feedback_details_box']['evaluation_content'] = array(
//                    '#type'   => 'item',
//                    '#title'  => '',
//                    '#tree'   => TRUE, // Don't forget to set #tree!
////                    '#prefix'   => '', // Don't forget to set #tree!
////                    '#suffix'   => '</table>', // Don't forget to set #tree!
//
//
//            );

            $submission_ids = get_rubric_submisssion_id($activity_id, $user_id);
            if (!empty($submission_ids)) {
                $graded = $submission_record->is_graded;

                if ($graded == 1) {
                    $form['grade']['feedback_details_box']['overall_score_heading'] = array(
                        '#type' => 'item',
                        '#prefix' => '<table><tr><td style="width:30%;padding-right:10px">' . $HTMLform_rubric_activity_title_hdr . '<div style="height:186px"><table width="100%" cellpadding="8" cellspacing="0"><tr><th style="text-align:center"> Average Overall Score',
                        '#suffix' => '</th></tr>',
                    );


                    $overall_points = get_rubric_overall_avg($user_id, $activity_id);

                    $form['grade']['feedback_details_box']['overall_scores'] = array(
                        '#type' => 'item',
                        '#prefix' => '<tr><td style="text-align:center;border:none;padding-top:22px"><div style="background-color:#fff;margin:0 40px 0 40px;padding:20px 5px 20px 5px;font-size:20px">' . $overall_points['avg_cat_feedback_points'] . '/' . $overall_points['avg_cat_max_points'],
                        '#suffix' => '</div></td></tr></table></div>' . $HTMLform_rubric_activity_title_ftr . '</td>',
                    );


                    $form['grade']['feedback_details_box']['average_cat_scores_heading'] = array(
                        '#type' => 'item',
                        '#prefix' => '<td valign="top">' . $HTMLform_rubric_activity_title_hdr . '<table cellpadding="5" class="table"><tr><th style="text-align:center" colspan="2"> Average Overall Score',
                        '#suffix' => '</th></tr>',
                    );


                    $form['grade']['feedback_details_box']['average_cat_scores'] = array(
                        '#type' => 'item',
                        '#prefix' => '<tr><td colspan="2"><div style="height:150px;">',
                        '#suffix' => '</div></td></tr></table>' . $HTMLform_rubric_activity_title_ftr . '</td></tr>',
                    );

//            $categories =  get_rubric_categories_score($activity_id);
//            if($categories != null || !empty($categories )) {
                    $catsRs = get_rubric_view_submission_cat_avg_score($user_id, $activity_id);
                    if ($catsRs != null || !empty($catsRs)) {
                        while ($cat = db_fetch_object($catsRs)) {

//                    $avg = $user_avg_score[$cat->category_ID];
//
//
//                    $totalCatPoints += $cat->max_points;

                            $form['grade']['feedback_details_box']['average_cat_scores']['average_cat_scores' . $cat->category_ID] = array(
                                '#type' => 'item',
                                '#value' => round($cat->score) . '/' . round($cat->max_points),
                                '#prefix' => "<div style='float:left;width:263px'><table cellpadding='0' cellspacing='0' class='table' style='width:auto' border='0'><tr><th style='width:160px;height:50px;vertical-align:middle;padding-right:5px'>" . $cat->category_name . "</th><td style='width:100px'>",
                                '#suffix' => '</td></tr></table></div>',
                            );
                        }
                    }

                    $form['grade']['feedback_details_box']['rubric_feedback_data'] = array(
                        '#type' => 'fieldset',
                        '#title' => '',
                        '#tree' => TRUE, // Don't forget to set #tree!
                        '#prefix' => '<tr><td colspan="2">' . $HTMLform_rubric_activity_title_hdr,
                        '#suffix' => $HTMLform_rubric_activity_title_ftr . '</td></tr></table>',
                    );

                    $form['grade']['feedback_details_box']['rubric_feedback_data']['rubric_feedback_data1'] = array(
                        '#type' => 'fieldset',
                        '#title' => '',
                        '#tree' => TRUE, // Don't forget to set #tree!
                        '#prefix' => '<table cellpadding="5" width="100%">',
                        '#suffix' => '</table>',
                    );

//                $submission_ids =  get_rubric_submisssion_id($activity_id, $user_id);

                    if ($submission_ids != null || !empty($submission_ids)) {
                        $i = 1;
                        foreach ($submission_ids as $submission_id => $graded_uid) {


                            $rs = get_rubric_view_user_feedback($activity_id, $user_id, $submission_id);

                            $form['grade']['feedback_details_box']['rubric_feedback_data']['rubric_feedback_data1'][$i] = array(
                                '#type' => 'item',
                                '#value' => 'Evaluation &nbsp;' . $i,
                                '#prefix' => '<tr><td colspan="3"><b style="color:#3570AA;font-size:15px">',
                                '#suffix' => '</b></td></tr><tr><th>&nbsp;</th><th style="text-align:left;border-right:1px solid #D5E9F5;border-left:1px solid #D5E9F5">Comments</th><th style="text-align:center">Score</th></tr>',
                            );

                            while ($feedbackData = db_fetch_object($rs)) {

                                $form['grade']['feedback_details_box']['rubric_feedback_data']['rubric_feedback_data1'][$feedbackData->category_ID . '_CateogryName_' . $submission_id] = array(
                                    '#type' => 'item',
                                    '#value' => $feedbackData->category_name,
                                    '#prefix' => '<tr><th width="138px">',
                                    '#suffix' => '</th>',
                                );


                                $form['grade']['feedback_details_box']['rubric_feedback_data']['rubric_feedback_data1'][$feedbackData->category_ID . '_Comment_' . $submission_id] = array(
                                    '#type' => 'item',
                                    '#value' => $feedbackData->comments,
                                    '#prefix' => '<td class="rubric_Comments" style="border-right:1px solid #9ECCE9">',
                                    '#suffix' => '</td>',
                                );

                                $form['grade']['feedback_details_box']['rubric_feedback_data']['rubric_feedback_data1'][$feedbackData->category_ID . '_Score_' . $submission_id] = array(
                                    '#type' => 'item',
                                    '#value' => round($feedbackData->score) . '/' . round($feedbackData->max_points),
                                    '#prefix' => '<td style="text-align:center">',
                                    '#suffix' => '</td></tr>',
                                );


                                $totalfeedback_recieved += $feedbackData->score;
                                $totalcategory_points += $feedbackData->max_points;
                            }

                            $form['grade']['feedback_details_box']['rubric_feedback_data']['rubric_feedback_data1']['total_points' . $i] = array(
                                '#type' => 'item',
                                '#value' => '<b style="color:#fff">Total &nbsp; &nbsp;' . $totalfeedback_recieved . '/' . $totalcategory_points . '</b>',
                                '#prefix' => '<tr><td colspan="3" style="text-align:right;background-color:#EF7921">',
                                '#suffix' => '</td></tr>',
                            );


                            $totalfeedback_recieved = 0;
                            $totalcategory_points = 0;


                            $i++;
                        }
                    }
                } else {
                    $form['grade']['feedback_details_box']['rubric_feedback_data']['no_feedback_recieved'] = array(
                        '#type' => 'item',
                        '#value' => 'No Feedback Found',
                        '#prefix' => '<table><tr><td>',
                        '#suffix' => '<td></tr></table>',
                    );
                }
            } else {
                $form['grade']['feedback_details_box'] ['no_feedback_recieved'] = array(
                    '#type' => 'item',
                    '#value' => 'No Feedback Found',
                    '#prefix' => '',
                    '#suffix' => '',
                );
            }


//            if($graded == 1) {
//                $form['grade']['feedback_details_box'] ['feedback_recieved'] = array(
//                        '#type' => 'item',
//                        '#suffix' => '</table>',
//
//                );
//
//            }else {
//
//                $form['grade']['feedback_details_box'] ['feedback_recieved'] = array(
//                        '#type' => 'markup',
//                        '#prefix' => ' No after Feedback Found',
//                        '#suffix' => '</table>',
//
//                );
//
//            }


            return $form;
        } else {

            drupal_set_message('No Activity Found ');
            drupal_goto('ntlp/goback');
        }
    } else {

        drupal_set_message('You are not enrolled in this activity');
        drupal_goto('ntlp/goback');
    }
}

?>
