<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_project', 'data_access');

function courses_delete_frm($course_id) {
    return drupal_get_form('courses_delete_form', $course_id);
}

function courses_delete_form($form = null, $course_id) {

    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_admincourse') . '/ntlp_admincourse.js');
    modalframe_child_js();
    drupal_set_title("Confirm Delete Course");

    $form = array();

    $form['courses_delete'] = array(
        '#type' => 'fieldset',
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
    </div>
',
    );


    $form['courses_delete']['main_table'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => '<table align="center" width="98%" border="0">',
        '#suffix' => '</table>',
    );

    $form['courses_delete']['course_id'] = array(
        '#type' => 'hidden',
        '#value' => $course_id,
    );

    $form['courses_delete']['main_table']['rows'] = array(
        '#type' => 'item',
        '#prefix' => '<tr>',
        '#suffix' => '</tr>',
    );

    $form['courses_delete']['main_table']['rows']['tdata1'] = array(
        '#type' => 'item',
        '#value' => t('You are about to delete this course.<br /><br />All Projects, resources, activities, discussions, and student enrollment for this course will be permanently removed. Are you sure you want to delete this course?<br />'),
        '#prefix' => '<td style="color:#3570AA;font-weight:bold">',
        '#suffix' => '</td>',
        '#id' => 'txt_course_name',
    );

    $form['courses_delete']['main_table']['row_1'] = array(
        '#type' => 'item',
        '#prefix' => '<tr>',
        '#suffix' => '</tr>',
    );

    $form['courses_delete']['main_table']['row_1']['tdata1'] = array(
        '#type' => 'item',
        '#value' => '<table><tr><td><a class="SaveBtn" onclick="delete_course_data(' . $course_id . ')">Yes</a></td><td>&nbsp;</td><td><a class="SaveBtn" onclick="delete_course_cancel()">No</a></td>',
        '#prefix' => '<td>',
        '#suffix' => '</td>',
    );
    return $form;
}

function courses_delete_data($course_nid) {

    global $user;
    $dateformat = "%Y-%m-%d %H:%M:%S";

    watchdog("course_delete", 'selected course id ' . $course_nid);

    //update block cache block for users
    ntlp_update_block_cache('ntlp_course', $course_nid);


    $course_result = db_query("SELECT * FROM {ntlp_course} WHERE nid=%d", $course_nid);
    $course_obj = db_fetch_object($course_result);


    db_query("INSERT INTO {ntlp_course_history} (nid, additional_notes, course_short_title, availability,
            driving_question, academic_standards, school_nid, is_project, course_nid,
            teacher1_uid, teacher2_uid, is_weighted_grades, period, school_year, picture_fid, topic_unit_tid,
            deleted_on, deleted_by, allow_self_enroll )
            VALUES (%d, '%s', '%s', '%s', '%s', '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, '%s', %d, %d)",
            $course_obj->nid, $course_obj->additional_notes, $course_obj->course_short_title,
            $course_obj->availability, $course_obj->driving_question,
            $course_obj->academic_standards, $course_obj->school_nid, $course_obj->is_project, $course_obj->course_nid,
            $course_obj->teacher1_uid, $course_obj->teacher2_uid, $course_obj->is_weighted_grades, $course_obj->period,
            $course_obj->school_year, $course_obj->picture_fid, $course_obj->topic_unit_tid,
            strftime($dateformat, time()), $user->uid, $course_obj->allow_self_enroll);
    
//    db_query("DELETE FROM {og} WHERE nid = %d", $course_nid);
//    db_query("DELETE FROM {og_ancestry} WHERE group_nid = %d", $course_nid);
//    db_query("DELETE FROM {og_uid} WHERE nid = %d", $course_nid);
//    db_query("DELETE FROM {og_term} WHERE nid = %d", $course_nid);
//
//
//    db_query("DELETE FROM {ntlp_course} WHERE nid = %d", $course_nid);
//
//    db_query("DELETE FROM {ntlp_course_term} WHERE course_nid = %d", $course_nid);
//    db_query("DELETE FROM {ntlp_course_unit} WHERE course_id = %d", $course_nid);
//    db_query("DELETE FROM {ntlp_course_grade} WHERE course_nid = %d", $course_nid);
//
//    db_query("DELETE FROM {ntlp_course_user} WHERE course_nid = %d", $course_nid);
//
//    db_query("DELETE FROM {ntlp_course_announcement} WHERE course_nid = %d", $course_nid);
//    db_query("DELETE FROM {ntlp_course_agenda} WHERE course_nid = %d", $course_nid);
//deleting process activity record
    $course_activity_result = db_query("SELECT * FROM {ntlp_course_activity} WHERE course_nid = %d", $course_nid);

    if ($course_activity_result->num_rows > 0) {
        while ($activityObj = db_fetch_object($course_activity_result)) {

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

//            db_query("DELETE FROM {ntlp_activity_submission} WHERE activity_nid = %d", $activity_Obj->nid);
//            db_query("DELETE FROM {ntlp_activity_user} WHERE activity_nid = %d", $activity_Obj->nid);
//            db_query("DELETE FROM {ntlp_activity_attachment} WHERE activity_nid = %d", $activity_Obj->nid);
//            db_query("DELETE FROM {ntlp_submission_attachment} WHERE submission_nid = %d", $activity_Obj->nid);
        }
    }

    db_query("DELETE FROM {ntlp_course_activity} WHERE course_nid = %d", $course_nid);


//$course_projects = get_all_course_projects($course_nid);
//
//    if($course_projects) {
//
//        foreach($course_projects as $project) {
//            project_delete_data($project->course_nid, $project->nid);
//        }
//    }
    // marking deleting a resource in course
    $course_resources = get_all_course_resources($course_nid);

    if ($course_resources) {

        foreach ($course_resources as $resourceObj) {

            db_query("UPDATE {ntlp_course_resource} SET deleted_by = %d, deleted_on = '%s'
                WHERE nid = %d", $user->uid, strftime($dateformat, time()), $resourceObj->nid);
        }
    }

    // course project section record
    $course_project_result = db_query("SELECT * FROM {ntlp_course} WHERE course_nid = %d", $course_nid);

    if ($course_project_result->num_rows > 0) {
        while ($projectObj = db_fetch_object($course_project_result)) {

            project_delete_data($projectObj->course_nid, $projectObj->nid, 'all', true);
        }
    }


    db_query("UPDATE {node} SET status = 0 WHERE nid = %d ", $course_nid);
//    db_query("DELETE FROM {ntlp_course_resource} WHERE course_nid = %d", $course_nid);

    db_query("DELETE FROM {ntlp_course} WHERE nid = %d", $course_nid);

//    node_delete($course_nid);

    drupal_set_message('Course deleted successfully!');
//    drupal_goto('ntlp/courses/my');
//    print "<div>OK</div>";
}