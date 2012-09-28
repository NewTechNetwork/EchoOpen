<?php

require_once "data_access.inc";

function ntlp_activity_delete($course_nid, $activity_nid) {
    modalframe_child_js();
    return drupal_get_form('ntlp_activity_delete_form', $course_nid, $activity_nid);
}

function ntlp_activity_delete_form($form = NULL, $course_nid, $activity_nid) {

    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/ntlp_courses_activity.js');
    drupal_set_title("Confirm Delete Activity");

    $activityObj = get_activity_records($activity_nid, true);

    $form = array();


    $newName =get_new_activity_name($activityObj->title);

    $form['delete_activity'] = array(
            '#type' => 'fieldset',
            '#tree' => true,
             '#prefix' => '<div class="ntlp_people_finder">
                               <div class="content">
                                   <div class="top_right">
                                       <div class="top_left">
                                           <div class="bot_left">
                                               <div class="bot_right_2">',
        '#suffix' => '</div></div></div></div></div></div>',

    );

    $form['delete_activity']['activity_nid'] = array(
            '#type' => 'hidden',
            '#value' => $activity_nid,
    );

    $form['delete_activity']['course_nid'] = array(
            '#type' => 'hidden',
            '#value' => $course_nid,
    );


    $form['delete_activity']['table'] = array(
            '#type' => 'item',
            '#prefix' => '<table>',
            '#suffix' => '</table>',

    );

    $form['delete_activity']['table']['row1'] = array(
            '#type' => 'item',
            '#prefix' => '<tr>',
            '#suffix' => '</tr>',
    );

    $form['delete_activity']['table']['row1']['td'] = array(
            '#type' => 'item',
            '#value' => t('You are about to delete this activity.<br /><br />All student work and grades associated with this activity will be permanently removed. Are you sure you want to delete this activity?<br />'),
            '#prefix' => '<td style="color:#3570AA;font-weight:bold">',
            '#suffix' => '</td>',
            '#id' => 'txt_activity_name',
    );

    $form['delete_activity']['table']['row3'] = array(
            '#type' => 'item',
            '#prefix' => '<tr>',
            '#suffix' => '</tr>',

    );
    $form['delete_activity']['table']['row3']['delete'] = array(
            '#type' => 'item',
            '#value' => '<table><tr><td><a class="SaveBtn" onclick="delete_activity()">Yes</a></td><td>&nbsp;</td><td><a class="SaveBtn" onclick="delete_activity_cancel()">No</a></td>',
            '#prefix' => '<td>',
            '#suffix' => '</td>',
    );

    return $form;
}

function ntlp_activity_delete_data($course_nid, $activity_nid) {
    global $user;
    $dateformat="%Y-%m-%d %H:%M:%S";

    if(user_access('delete ntlp activity')) {
        $activityObj = get_activity_records($activity_nid);

//        if (is_activity_graded($activity_nid)){
//            drupal_set_message('You cannot delete a graded activity.  If you want to delete this activity, please remove any student scores first.');
//            print "ERROR";
//        }
//        else
//        {
            $activity_attachments = get_attachment($activity_nid ,'ntlp_activity_attachment', 'activity_nid');

            db_query("INSERT INTO {ntlp_course_activity_history} (nid, course_nid, project_nid, activity_order,
                        submission_type, allow_resubmission, availability_mode, available_from, available_to, due_date,
                        is_graded, grading_method, curriculum_unit_tid, curriculum_standards, school_term_tid, timecreated,
                        timemodified, created_by, modified_by, deleted_on, deleted_by,
                        is_published, project_cat, allow_late_submission, published_date)
                        VALUES(%d, %d, %d, %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d, '%s', %d, '%s',
                        '%s', %d, %d, '%s', %d,
                        %d, %d, %d, '%s')",
                            $activityObj->nid, $activityObj->course_nid, $activityObj->project_nid,  $activityObj->activity_order,
                            $activityObj->submission_type, $activityObj->allow_resubmission, $activityObj->availability_mode,
                            $activityObj->available_from, $activityObj->available_to, $activityObj->due_date,
                            $activityObj->is_graded, $activityObj->grading_method, $activityObj->curriculum_unit_tid,
                            $activityObj->curriculum_standards, $activityObj->school_term_tid, $activityObj->timecreated,
                            $activityObj->timemodified, $activityObj->created_by, $activityObj->modified_by,
                            strftime($dateformat, time()), $user->uid,
                            $activityObj->is_published, $activityObj->project_cat, $activityObj->allow_late_submission, $activityObj->published_date);

            db_query("DELETE FROM {ntlp_course_activity} WHERE nid = %d", intval($activity_nid));

            print "OK";
//        }//else Delete Activity
    }
//    drupal_goto("ntlp/courses/activity/".$course_nid);
//    drupal_set_message('Activity deleted successfully!');
}
function ntlp_is_activity_graded ($course_nid, $activity_nid){
    
    print  is_activity_graded($activity_nid);
    
}
