<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


/* popup form */

function ntlp_project_delete_form($form_state = null, $course_id, $project_id) {

    drupal_set_title("Confirm Delete Project");

    $project_Obj = node_load($project_id);

    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/ntlp_project.js');

    $form['project_delete'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '
      <div class="ntlp_people_finder">
      <div class="content">
        <div class="top_right">
        <div class="top_left">
        <div class="bot_left">
            <div class="bot_right_2">',
        '#suffix' => '</div>
          </div></div>
        </div>
      </div>
    </div>',
    );

    $form['project_delete']['course_id'] = array(
        '#type' => 'hidden',
        '#value' => $course_id,
    );

    $form['project_delete']['project_id'] = array(
        '#type' => 'hidden',
        '#value' => $project_id,
    );

    $form['project_delete']['table'] = array(
        '#type' => 'item',
        '#prefix' => '<table>',
        '#suffix' => '</table>',
    );

    $form['project_delete']['table']['row1'] = array(
        '#type' => 'item',
        '#prefix' => '<tr>',
        '#suffix' => '</tr>',
    );
    $form['project_delete']['table']['row1']['td'] = array(
        '#type' => 'item',
        '#value' => t('You are about to delete the project "' . $project_Obj->title . '". This will delete the project and all its associated resources. If you don’t want to lose the resources, be sure to save them before deleting this project or cancel this action.<br/><br/>Are you sure you want to delete this project?'),
        '#prefix' => '<td style="color:#3570AA;font-weight:bold; padding:5px">',
        '#suffix' => '</td>',
    );
    $form['project_delete']['table']['row3'] = array(
        '#type' => 'item',
        '#prefix' => '<tr>',
        '#suffix' => '</tr>',
    );
    $form['project_delete']['table']['row3']['delete'] = array(
        '#type' => 'item',
        '#value' => '<br><br><table style="margin-left:5px;"><tr><td><a style="margin-right:5px;" class="AddOutComesBtn" onclick="delete_project(' . $course_id . ',' . $project_id . ',' . "'project'" . ' )">Delete Project</a></td><td>&nbsp;</td><td><input type="button" style="margin: 0px !important;" class="CancelBtn" onclick="delete_project_cancel()" value="Cancel"></td>',
        '#prefix' => '<td>',
        '#suffix' => '</td>',
    );

    return $form;
}

function project_delete_data($course_id, $project_id, $condition, $nomessage = false) {
    global $user;
    $dateformat = "%Y-%m-%d %H:%M:%S";

//    $project_add = $form_state['clicked_button']['#post']['project_delete'];
//    $course_id = $project_add['course_id'];
//    $project_id = $project_add['project_id'];



    if (user_access('delete project')) {

//        node_delete($project_id);

        $project_result = db_query("SELECT * FROM {ntlp_course} WHERE nid=%d", $project_id);
        if ($projectObj = db_fetch_object($project_result)) {

            db_query("INSERT INTO {ntlp_course_history} (nid, additional_notes, course_short_title, availability,
                from_date, to_date, driving_question, academic_standards, school_nid, is_project, course_nid,
                teacher1_uid, teacher2_uid, is_weighted_grades, period, school_year, picture_fid, topic_unit_tid,
                deleted_on, deleted_by, allow_self_enroll )
                VALUES (%d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s', %d, %d)",
                    $projectObj->nid, $projectObj->additional_notes, $projectObj->course_short_title,
                    $projectObj->availability, $projectObj->from_date, $projectObj->to_date, $projectObj->driving_question,
                    $projectObj->academic_standards, $projectObj->school_nid, $projectObj->is_project, $projectObj->course_nid,
                    $projectObj->teacher1_uid, $projectObj->teacher2_uid, $projectObj->is_weighted_grades, $projectObj->period,
                    $projectObj->shcool_year, $projectObj->picture_fid, (isset($projectObj->topic_unit_tid) ? $projectObj->topic_unit_tid : 0 ),
                    strftime($dateformat, time()), $user->uid, $projectObj->allow_self_enroll);

            db_query("DELETE FROM {ntlp_course} WHERE nid=%d", $project_id);


            if ($condition == 'all') { // only delete if user request to delete project resources
//                $project_resources = _get_project_resources($project_id);
//
//                if ($project_resources) {
//
//                    foreach ($project_resources as $resource) {
//
//                        $data = db_result(db_query("SELECT COUNT(nid) FROM dpl_ntlp_project_resource
//                        WHERE nid = %d", $resource->nid));
//
//                        if ($data > 1) {
//                            // checking if the same resource exist in more then one project then don't delete
//                        } else {
//
//                            db_query("DELETE FROM {ntlp_project_resource} WHERE nid=%d", $resource->nid);
//
//                            db_query("UPDATE {ntlp_course_resource} SET deleted_by = %d AND deleted_on = '%s'
//                            WHERE nid = %d", $user->uid, strftime($dateformat, time()), $resource->nid);
//                        }
//                    }
//                }
//                
    //        db_query("DELETE FROM {ntlp_project_resource} WHERE project_nid=%d", $project_id);
                // delete project section
    //        $result = db_query("SELECT * FROM {ntlp_course_section} WHERE course_nid=%d", $project_id);
    //        if(db_affected_rows($result)>0) {
    //            while($sectionRows = db_fetch_object($result)) {
    //
    //                db_query("DELETE FROM {term_data} WHERE tid =%d", $sectionRows->tid);
    //            }
    //            db_query("DELETE FROM {ntlp_course_section} WHERE course_nid =%d", $project_id);
    //        }
                //copying project activities
                $result = db_query("SELECT * FROM {ntlp_course_activity} WHERE project_nid=%d", $project_id);
                if (db_affected_rows($result) > 0) {
                    while ($activityObj = db_fetch_object($result)) {
    //                node_delete($activityRows->nid);

                        db_query("INSERT INTO {ntlp_course_activity_history} (nid, course_nid, project_nid, activity_order,
                        submission_type, allow_resubmission, availability_mode, available_from, available_to, due_date,
                        is_graded, grading_method, curriculum_unit_tid, curriculum_standards, school_term_tid, timecreated,
                        timemodified, created_by, modified_by, deleted_on, deleted_by,
                        is_published, project_cat, allow_late_submission, published_date)
                        VALUES(%d, %d, %d, %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d, '%s', %d, '%s',
                        '%s', %d, %d, '%s', %d,
                        %d, %d, %d, '%s')",
                                $activityObj->nid, $activityObj->course_nid, $activityObj->project_nid, $activityObj->activity_order,
                                $activityObj->submission_type, $activityObj->allow_resubmission, $activityObj->availability_mode,
                                $activityObj->available_from, $activityObj->available_to, $activityObj->due_date,
                                $activityObj->is_graded, $activityObj->grading_method, $activityObj->curriculum_unit_tid,
                                $activityObj->curriculum_standards, $activityObj->school_term_tid, $activityObj->timecreated,
                                $activityObj->timemodified, $activityObj->created_by, $activityObj->modified_by,
                                strftime($dateformat, time()), $user->uid,
                                $activityObj->is_published, $activityObj->project_cat, $activityObj->allow_late_submission, $activityObj->published_date);
                    }
                    db_query("DELETE FROM {ntlp_course_activity} WHERE project_nid=%d", $project_id);
                }
            }
            if (!$nomessage) {
                drupal_set_message('Project successfully deleted!');
            }
        } else {
            drupal_set_message('The project you are trying to delete is not available!');
        }
    }
    }

function _get_project_resources($project_id) {
    $flag = false;
    $result = db_query("SELECT * FROM {ntlp_project_resource} WHERE project_nid = %d", $project_id);

    if ($result->num_rows > 0) {
        while ($record = db_fetch_object($result)) {
            $flag[] = $record;
        }
    } else {
        $flag = false;
    }
    return $flag;
}

function get_project_resource_count($project_id) {

    $result = db_query("
            SELECT COUNT(*) AS count_res FROM dpl_ntlp_project_resource pr
            INNER JOIN dpl_ntlp_course_resource cr ON cr.nid = pr.nid
            WHERE pr.project_nid = %d
            AND  cr.deleted_on IS NULL;", $project_id);

    if ($result->num_rows > 0) {
        if ($record = db_fetch_object($result)) {
            return $record->count_res;
        }
    }

    return 0;
}
