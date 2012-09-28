<?php

// Change the undelete form such that it asks for:
// 1. School Name (in combo)
// 2. Course Title (in combo, refreshed using AHAH)
// 3. Project Title (in combo, refreshed using AHAH)
//
// The form will be in the same style modal.
//------------------------------------------------------------

/* popup form */
//function ntlp_project_undelete_form($form_state = null,$course_id, $project_id ) {
//
//    drupal_set_title("Confirm Delete Project");
//
//    $project_Obj = node_load($project_id);
//
//    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/ntlp_project.js');
//
//    $form['project_delete'] = array(
//            '#type'   => 'fieldset',
//            '#title'  => '',
//            '#tree'   => TRUE, // Don't forget to set #tree!
//            '#prefix' => '<div id="project_add_test">'.$HTMLform_activity_outer_name_hdr,
//            '#suffix' => $HTMLform_activity_outer_name_ftr.'</div>',
//    );
//
//    $form['project_delete']['course_id'] = array(
//            '#type' => 'hidden',
//            '#value' =>  $course_id,
//    );
//
//    $form['project_delete']['project_id'] = array(
//            '#type' => 'hidden',
//            '#value' =>  $project_id,
//    );
//
//    $form['project_delete']['table'] = array(
//            '#type' => 'item',
//            '#prefix' => '<table>',
//            '#suffix' => '</table>',
//
//    );
//
//    $form['project_delete']['table']['row1'] = array(
//            '#type' => 'item',
//            '#prefix' => '<tr>',
//            '#suffix' => '</tr>',
//
//    );
//    $form['project_delete']['table']['row1']['td'] = array(
//            '#type' => 'item',
////            '#value' => t('You are about to delete this project.<br /><br />Please indicate whether you would like to delete the only the project, or the project and its resources. Are you sure you want to delete this project?<br />'),
//            '#value' => t('You are about to delete this project.<br /><br />Are you sure you want to delete this project?<br />'),
//            '#prefix' => '<td style="color:#3570AA;font-weight:bold">',
//            '#suffix' => '</td>',
//
//    );
//    $form['project_delete']['table']['row3'] = array(
//            '#type' => 'item',
//            '#prefix' => '<tr>',
//            '#suffix' => '</tr>',
//
//    );
//    $form['project_delete']['table']['row3']['delete'] = array(
//            '#type' => 'item',
//            '#value' => '<table><tr><td><a class="SaveBtn" onclick="delete_project('.$course_id.','.$project_id.')">Yes</a></td><td>&nbsp;</td><td><a class="SaveBtn" onclick="delete_project_cancel()">No</a></td>',
//            '#prefix' => '<td>',
//            '#suffix' => '</td>',
//    );
//
//
//
//
//    return $form;
//}


function project_undelete_data($course_id, $project_id ) {
    global $user;
    $dateformat="%Y-%m-%d %H:%M:%S";

    if(user_access('delete project')) {

//        node_delete($project_id);

        $project_result = db_query("SELECT * FROM {ntlp_course} WHERE nid=%d", $project_id);
        $projectObj = db_fetch_object($project_result);

        db_query("INSERT INTO {ntlp_course_history} (nid, additional_notes, course_short_title, availability,
            from_date, to_date, driving_question, academic_standards, school_nid, is_project, course_nid, 
            teacher1_uid, teacher2_uid, is_weighted_grades, period, school_year, picture_fid, deleted_on, deleted_by )
            VALUES (%d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s', %d)",
                $projectObj->nid, $projectObj->additional_notes, $projectObj->course_short_title,
                $projectObj->availability, $projectObj->from_date, $projectObj->to_date, $projectObj->driving_question,
                $projectObj->academic_standards, $projectObj->school_nid, $projectObj->is_project, $projectObj->course_nid,
                $projectObj->teacher1_uid, $projectObj->teacher2_uid, $projectObj->is_weighted_grades, $projectObj->period,
                $projectObj->shcool_year, $projectObj->picture_fid ,strftime($dateformat, time()), $user->uid );

        db_query("DELETE FROM {ntlp_course} WHERE nid=%d", $project_id);

        //copying project activities
        $result = db_query("SELECT * FROM {ntlp_course_activity} WHERE project_nid=%d", $project_id);
        if(db_affected_rows($result)>0) {
            while($activityObj = db_fetch_object($result)) {
//                node_delete($activityRows->nid);

                db_query("INSERT INTO {ntlp_course_activity_history} (nid, course_nid, project_nid, activity_order, 
                    submission_type, allow_resubmission, availability_mode, available_from, available_to, due_date,
                    is_graded, grading_method, curriculum_unit_tid, curriculum_standards, school_term_tid, timecreated,
                    timemodified, created_by, modified_by, deleted_on, deleted_by)
                    VALUES(%d, %d, %d, %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d, '%s', %d, '%s',
                    '%s', %d, %d, '%s', %d )",
                        $activityObj->nid, $activityObj->course_nid, $activityObj->project_nid,  $activityObj->activity_order,
                        $activityObj->submission_type, $activityObj->allow_resubmission, $activityObj->availability_mode,
                        $activityObj->available_from, $activityObj->available_to, $activityObj->due_date, 
                        $activityObj->is_graded, $activityObj->grading_method, $activityObj->curriculum_unit_tid,
                        $activityObj->curriculum_standards, $activityObj->school_term_tid, $activityObj->timecreated,
                        $activityObj->timemodified, $activityObj->created_by, $activityObj->modified_by, 
                        strftime($dateformat, time()), $user->uid);

            }
            db_query("DELETE FROM {ntlp_course_activity} WHERE project_nid=%d", $project_id);
        }

        drupal_set_message('Project successfully un-deleted!');
    }
}

?>