<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');

require_once "data_access.inc";

function ntlp_activity_copy($course_nid, $activity_nid) {


    return drupal_get_form('ntlp_activity_copy_form', $course_nid, $activity_nid);
}

function ntlp_activity_copy_form($form_state = NULL, $course_nid, $activity_nid) {
    global $user;
    modalframe_child_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/ntlp_courses_activity.js');

    ahah_helper_register($form, $form_state);
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/course_ahah_helper.js', 'footer');

    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/date_picker.js');

    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/jquery.ui.datepicker.css', 'module', 'all', FALSE);
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/demos.css', 'module', 'all', FALSE);
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/jquery.ui.all.css', 'module', 'all', FALSE);

    $activityObj = get_activity_records($activity_nid, true);
    $courseObj = node_load($course_nid);

    $newName = get_new_activity_name($activityObj->title);

    $form['copy_activity'] = array(
        '#type' => 'fieldset',
        '#tree' => true,
        '#prefix' => '<div id="dialog-show-confirmation" style="display: none;" title="Warning">
    <span class="msg">Are you sure you want to submit the form?</span>
</div><div class="ntlp_people_finder">
                               <div class="content">
                                   <div class="top_right">
                                       <div class="top_left">
                                           <div class="bot_left">
                                               <div class="bot_right_2"><div style="width:280px;height:220px" >',
        '#suffix' => '</div></div></div></div></div></div></div>',
    );

    $form['copy_activity']['activity_nid'] = array(
        '#type' => 'hidden',
        '#value' => $activity_nid,
        '#id' => 'activity_nid',
    );

    $form['copy_activity']['activity_name'] = array(
        '#type' => 'textfield',
        '#value' => $newName,
        '#prefix' => '<div style="margin-bottom: 10px; color:#3570AA;font-weight:bold;white-space:nowrap; margin-left: 10px;"><div style="float:left;width:120px;">' . t('Activity Name') . '</div><div style="padding-left: 160px;">',
        '#suffix' => '</div></div>',
        '#id' => 'activity_name',
    );

    $years_option = get_school_years_for_staff(get_this_school());
    if (empty($years_option)) {
        $years_option[0] = 'No Years Found';
    }

    $form['copy_activity']['year'] = array(
        '#type' => 'select',
        '#options' => $years_option,
        '#default_value' => get_this_course_year($course_nid),
        '#prefix' => '<div class="clearfix" style="color:#3570AA;font-weight:bold;white-space:nowrap; margin-left: 10px;"><div style="float:left;width:120px;">' . t('Select Year') . '</div><div style="padding-left: 162px;"><span class="comboboxBgLeft" style="width:100px;">',
        '#suffix' => '</span></div></div>',
        '#ahah' => array(
            'path' => ahah_helper_path(array('copy_activity', 'courses')),
            'wrapper' => 'courses-wrapper',
            'progress' => 'none',
        ),
        '#attributes' => array('class' => "my-dropdown"),
    );


    $form['copy_activity']['courses'] = array(
        '#type' => 'item',
        '#prefix' => '<div style="float: left;margin-bottom: 10px;" id="courses-wrapper">',
        '#suffix' => '</div>',
    );

    if (isset($form_state['storage']['copy_activity']['year'])) {
        $selected_year = $form_state['storage']['copy_activity']['year'];
    } else {
        $selected_year = get_this_course_year($course_nid);
    }



    $teacher_courses = get_teacher_courses_in_year($user->uid, $selected_year);


    if (sizeof($teacher_courses) > 1) {
        foreach ($teacher_courses as $course) {
            $course_options[0] = 'Select Course';
            $course_options[$course->nid] = truncate_data($course->title, 40);
        }
    }


    if (empty($course_options)) {
        $course_options[0] = 'No Course Available';
        $message = t('You must enroll into the course before you copy this Activity');
    }

    if (isset($form_state['storage']['copy_activity']['courses']['course_nid'])) {
        $selected_course_nid = $form_state['storage']['copy_activity']['courses']['course_nid'];
    } else {
        $selected_course_nid = $course_nid;
    }

    $form['copy_activity']['courses']['course_nid'] = array(
        '#type' => 'select',
        '#options' => $course_options,
        '#default_value' => $selected_course_nid,
        '#id' => 'selected_course_nid',
        '#prefix' => '<div style="color:#3570AA;font-weight:bold;white-space:nowrap; margin-left: 10px;"><div style="width:120px;float:left;">' . t('Course Name') . '</div><div style="padding-left: 162px;"><span class="comboboxBgLeft" style="width:300px;">',
        '#suffix' => '</span></div></div>',
        '#ahah' => array(
            'path' => ahah_helper_path(array('copy_activity', 'courses', 'courses_terms_block')),
            'wrapper' => 'course-term-wrapper',
            'progress' => 'none',
        ),
        '#attributes' => array('class' => 'my-dropdown'),
    );

    if (isset($selected_course_nid) && $selected_course_nid > 0) {
        if (!is_course_exist_in_year($selected_course_nid, $selected_year)) {
            $selected_course_nid = 0;
        }
    } else {
        $selected_course_nid = 0;
    }



    if ($selected_course_nid > 0) {
        $course_term_result = get_course_terms($selected_course_nid);

        if ($course_term_result->num_rows > 0) {
            $course_terms = array("0" => "Select Term");
            while ($data = db_fetch_object($course_term_result)) {
                $course_terms[$data->tid] = $data->name;
            }
        } else {
            $course_terms = array("0" => "No Term Found");
        }
    } else {
        $course_terms = array("0" => "No Term Found");
    }

    $form['copy_activity']['courses']['courses_terms_block'] = array(
        '#type' => 'item',
        '#prefix' => '<div id="course-term-wrapper">',
        '#suffix' => '</div>',
    );

    $form['copy_activity']['courses']['courses_terms_block']['course_term'] = array(
        '#type' => 'select',
        '#options' => $course_terms,
        '#id' => 'selected_term_tid',
        '#prefix' => '<div style="color:#3570AA;font-weight:bold;white-space:nowrap; margin-left: 10px;float: left;margin-top: 10px;"><div style="width:120px;float:left;">' . t('Course Term') . '</div><div style="padding-left: 162px;"><span class="comboboxBgLeft" style="width:300px;">',
        '#suffix' => '</span></div></div>',
        '#attributes' => array('class' => 'my-dropdown', 'diseable' => true),
    );


    if (!isset($message)) {

        $option = '<div style="float:left;padding-right:6px;"><span><a class="SaveBtn" onclick="update_activity();">Copy</a></span></div>';
        $form['copy_activity']['copy_resource_button'] = array(
            '#type' => 'item',
            '#value' => '<div style="padding-top:55px;width:100%; margin-left:10px;" class="clearfix">' . $option . '<div style="float:left;padding-left:2px;"><span><a class="CancelBtn" style="text-align:center;" onclick="parent.Drupal.modalFrame.close();"><span style="position:relative;top:3px;">Cancel</span></a></span></div></div>',
        );
    } else {
        $form['copy_activity']['copy_resource_button'] = array(
            '#type' => 'item',
            '#value' => $message,
            '#prefix' => "<div style='width:100%;padding-top:10px;margin-left:75px;' class='clearfix'><div style='float:left;width:350px;border:1px solid #F17B23;'>",
            '#suffix' => "</div></div>",
        );
    }

    return $form;
}

function ntlp_activity_copy_data($course_nid, $activity_nid, $activity_name, $selected_term_tid) {
    global $user;
    $dateformat = "%Y-%m-%d %H:%M:%S";


    $activityObj = get_activity_records($activity_nid);

    $activity_attachments = get_attachment($activity_nid, 'ntlp_activity_attachment', 'activity_nid');


    $activity_node->title = $activity_name;
    $activity_node->type = $activityObj->type;   // Your specified content type
    $activity_node->created = time();
    $activity_node->changed = time();
    $activity_node->body = $activityObj->body;
    $activity_node->status = 1;
    $activity_node->promote = 0;
    $activity_node->sticky = 0;
    $activity_node->format = 1;       // Filtered HTML
    $activity_node->uid = $user->uid; // UID of content owner
    // add standard node organic groups

    node_save($activity_node);

    activity_copy_data($activity_nid, $activity_node->nid, $course_nid, 0, 0, 0, $selected_term_tid);

    drupal_set_message("Activity Copied " . l('Go to New Activity', 'ntlp/courses/activity/view/' . $course_nid . '/' . $activity_node->nid));
}

function activity_copy_data($src_activity_nid, $new_activity_nid, $target_course_nid, $target_project_nid, $target_project_section, $activity_order, $selected_term_tid, $course_opt = "COPY") {
    global $user;

    $activityObj = get_activity_records($src_activity_nid);

    $activity_attachments = get_attachment($src_activity_nid, 'ntlp_activity_attachment', 'activity_nid');

    $publishdate_value = $activityObj->available_from = '0000-00-00 00:00:00';
    $hidedate_value = $activityObj->available_to = '0000-00-00 00:00:00';
    $due_date = $activityObj->due_date;
    if ($due_date == null) {

        $due_date = '0000-00-00 00:00:00';
    }

    $activity_outcomes = db_query("SELECT * FROM {ntlp_gb_activity_outcome} WHERE activity_nid =%d", $src_activity_nid);
    if ($activity_outcomes->num_rows > 0) {
        while ($activity_outcomes_data = db_fetch_object($activity_outcomes)) {

            db_query("INSERT INTO {ntlp_gb_activity_outcome} (tid, activity_nid, max_points) 
                VALUES (%d, %d, %d)", $activity_outcomes_data->tid, $new_activity_nid, $activity_outcomes_data->max_points);
        }
    }

    db_query("INSERT INTO {ntlp_course_activity} (nid, course_nid, project_nid, activity_order,
            submission_type, allow_resubmission, availability_mode, available_from, available_to, 
            is_graded, grading_method, curriculum_unit_tid, curriculum_standards, school_term_tid, 
            timecreated, timemodified, created_by, modified_by, project_cat, allow_late_submission)
        VALUES({$new_activity_nid}, {$target_course_nid}, {$target_project_nid}, 
        {$activity_order}, '{$activityObj->submission_type}', {$activityObj->allow_resubmission}, 'M',
        '{$publishdate_value}', '{$hidedate_value}', {$activityObj->is_graded}, 
        '{$activityObj->grading_method}', {$activityObj->curriculum_unit_tid}, 
        '{$activityObj->curriculum_standards}', {$selected_term_tid}, 
            '" . date(DATE_FORMAT_CHECK) . "','" . date(DATE_FORMAT_CHECK) . "', {$user->uid}, 
            {$user->uid}, {$target_project_section}, " . ($activityObj->allow_late_submission == 1 ? 1 : 0 ) . ")");

    if ($activity_attachments != null) {
        foreach ($activity_attachments as $attchmentObj) {

            db_query("INSERT INTO {ntlp_activity_attachment} (activity_nid, type, path, is_deleted, fid )
                VALUES( %d, '%s', '%s', %d, %d)", intval($new_activity_nid), check_plain($attchmentObj->type), check_plain($attchmentObj->path), intval($attchmentObj->is_deleted), intval($attchmentObj->fid));
        }
    }
    if ($course_opt == "CSE" || $course_opt == "CS") {
         /***  No activities enrollment copied when do the course copy **/
    } else if($course_opt == "COPY"){
        
        /** The activities enrollment only with copy activity option 
         *  where selecting the user based on target course selection.
         */
        $result = get_course_users($target_course_nid);

        while ($rs = db_fetch_object($result)) 
        {
            db_query("INSERT INTO {ntlp_activity_user} VALUES (%d, %d)", $new_activity_nid, $rs->uid);
        }
    }




    
}

function check_activity_name($activity_name) {
    $flag = false;

    $result = db_query("SELECT * FROM {node} WHERE title = '%s'", $activity_name);
//    echo ' inside check activityname method ';
    if ($result) {
        $flag = true;
    } else {
        $flag = false;
    }
    return $flag;
}

function get_new_activity_name($activity_name) {

    $name = "";


    $number = preg_replace("/[^0-9]/", '', $activity_name);

    if ($number != null) {

        $name = str_replace($number, ($number + 1), $activity_name);
    } else {

        $name = $activity_name . '01';
    }
    return $name;
}

?>
