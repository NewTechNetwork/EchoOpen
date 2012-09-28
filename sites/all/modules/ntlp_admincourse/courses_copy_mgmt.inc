<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_project', 'data_access');

function courses_copy_frm($course_id) {
    return drupal_get_form('courses_copy_form', $course_id);
}

function courses_copy_form($form_state = null, $course_id) {

    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_admincourse') . '/ntlp_admincourse.js');

    modalframe_child_js();

    $form = array();

    $confimation = true;
    ahah_helper_register($form, $form_state);

    drupal_add_js(drupal_get_path('module', 'ntlp_admincourse') . '/admincourse_ahah_helper.js', 'footer');

    drupal_add_js('
    function func() {
    
    $("#comfirmation_box").hide();
    $("#courses-copy-test-wrapper").show();
    }', 'inline');

    $confirmation_message = '
<p>Not all data from a Course can be copied to a new course. Before
proceeding, please make sure you understand the following implications
for the NEW COURSE:</p>        
<p><b>ONLY PROJECTS, ACTIVITIES, AND SOME BASIC COURSE SETTINGS WILL BE COPIED.<b> 
        <br/>THE FOLLOWING CONTENT WILL NOT BE COPIED:
        <ul>
        <li>student enrollment</li>
        <li>gradebook scores</li>
        <li>share stuff/all stuff content</li>
        <li>discussions</li>
        <li>announcements</li>
        </ul>

</p>
<p>
<b>ALL EXISTING COURSE LEARNING OUTCOMES WILL BE COPIED</b>, but
if you make any changes to the new Course Learning Outcomes it will
impact the Outcomes available in the copied Activities.

Activities will be copied with the following parameters:

<ul>
        <li><b>ALL ACTIVITIES WILL BE PLACED INTO THE SAME TERM;</b> you may need to
change the Term assignment for some Activities later.</li>
        <li><b>ALL ACTIVITIES WILL BE HIDDEN</b> so students cannot see them until
you change the visibility to “show”.</li>
        <li><b>NO DUE DATES</b> will be given to Activities, so you’ll need to set
individual due dates for each activity before students can see them.</li>
        <li><b>NO STUDENTS WILL BE ASSIGNED</b> to any Activities at this time;
you’ll need to assign students to each activity before students can
see them.</li>
</ul>
</p>
<p>
Please confirm that you understand these parameters before you proceed
with the next step.</p>';

    $form['courses_copy_confirmation'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div id="comfirmation_box" style="width:600px;height:300;">',
        '#suffix' => '</div>',
    );

    $form['courses_copy_confirmation']['message'] = array(
        '#type' => 'item',
        '#value' => $confirmation_message,
        '#prefix' => '<div>',
        '#suffix' => '</div>',
    );

    $form['courses_copy_confirmation']['btn'] = array(
        '#type' => 'item',
        '#value' => '<div style="float:left; margin-right: 10px;"><a class="SubmitAssignmentBtn" onclick="func()">I understand</a></div><div style="float:left;"><a class="cancelBtn" onclick="copy_course_cancel()">Cancel</a></div> &nbsp; ',
        '#prefix' => '<div>',
        '#suffix' => '</div>',
    );




    $form['courses_copy'] = array(
        '#type' => 'fieldset',
        '#tree' => TRUE,
        '#value' => '',
        '#prefix' => '<div id="dialog-validate-outcome" style="display: none;" title="Warning">
            <span class="msg"></span>
            </div>
            <div style="display:none;" id="courses-copy-test-wrapper">
            <div style="width:600px;height:300;" >
            <div id="BoldrProject" style="width:auto">
                <div class="ProjectDetail">
		<div class="LightestBlueBottomRightCorner">
			<div class="LightestBlueBottomLeftCorner">
				<div class="LightestBlueTopRightCorner">
					<div class="LightestBlueTopLeftCorner">',
        '#suffix' => '</div></div></div>
				</div>
			</div>
		</div>
	</div>
</div>',
    );

    $form['courses_copy']['course_id'] = array(
        '#type' => 'hidden',
        '#value' => $course_id,
    );


    $form['courses_copy']['tdata1'] = array(
        '#type' => 'textfield',
        '#value' => '',
        '#prefix' => '<div style="margin-bottom: 10px; color:#3570AA;font-weight:bold;white-space:nowrap; margin-left: 10px;"><div style="float:left;width:120px;">' . t('New Course Name:') . '</div><div style="padding-left: 160px;">',
        '#suffix' => '</div></div>',
        '#id' => 'txt_course_name',
    );


    if (isset($form_state['storage']['courses_copy']['row_2'])) {
        $selected_year = $form_state['storage']['courses_copy']['row_2'];
    } else {
        $selected_year = get_this_school_year();
    }

    $years_option = _get_all_school_years(get_this_school());
    if (empty($years_option)) {
        $years_option[0] = 'No Years Found';
    }

    watchdog('check_formst', print_r($form_state, true) . ' year ' . $selected_year);
    $form['courses_copy']['row_2'] = array(
        '#type' => 'select',
        '#options' => $years_option,
        '#default_value' => get_this_school_year(),
        '#ahah' => array(
            'path' => ahah_helper_path(array('courses_copy', 'row_3')),
            'wrapper' => 'school-terms-wrapper',
            'progress' => 'none',
        ),
        '#prefix' => '<div class="clearfix" style="color:#3570AA;font-weight:bold;white-space:nowrap; margin-left: 10px;"><div style="float:left;width:120px;">' . t('Select Year:') . '</div><div style="padding-left: 162px;"><span class="comboboxBgLeft" style="width:100px;">',
        '#suffix' => '</span></div></div>',
        '#attributes' => array('class' => 'my-dropdown'),
        '#id' => 'cbo_course_year',
    );






    $school_term_result = get_school_year_term(get_this_school(), $selected_year, true);
//print_r($school_term_result);
    if (count($school_term_result) > 0) {
        
        foreach ($school_term_result as $data) {
            $term_options["0"] = "Select Term";
            $term_options[$data['school_term_tid']] = $data['name'];
        }
    } else {
        $term_options = array("0" => "No Term Found");
    }

    $form['courses_copy']['row_3'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => '<div style="float: left;margin-bottom: 10px;" id="school-terms-wrapper">',
        '#suffix' => '</div>',
    );

    $form['courses_copy']['row_3']['terms'] = array(
        '#type' => 'select',
        '#options' => $term_options,
        '#default_value' => '',
        '#prefix' => '<div style="color:#3570AA;font-weight:bold;white-space:nowrap; margin-left: 10px;"><div style="width:120px;float:left;">' . t('School Year Terms:') . '</div><div style="padding-left: 162px;"><span class="comboboxBgLeft" style="width:300px;">',
        '#suffix' => '</span></div></div>',
        '#attributes' => array('class' => 'my-dropdown'),
        '#id' => 'cbo_school_term',
    );


    $form['courses_copy']['rowheading'] = array(
        '#type' => 'item',
        '#prefix' => '<div style="color:#3570AA;font-weight:bold;white-space:nowrap; margin-left: 10px;"><div style="width:120px;float:left;">' . t('Select an Option:') . '</div>',
        '#suffix' => '</div></div>',
        '#id' => 'radio_course1',
    );

    $form['courses_copy']['row1'] = array(
        '#type' => 'radios',
        '#default_value' => 1,
        '#options' => array('1' => t('Copy Content and Settings'),
            '2' => t('Copy content, settings and student enrollment to new course'),
            '3' => t('Copy student enrollment to new course')),
        '#prefix' => '<div style="color:#3570AA;font-weight:bold;white-space:nowrap; margin-left: 10px;margin-top: 10px;">',
        '#suffix' => '</div>',
        '#id' => 'selected_option',
    );


    $form['courses_copy']['tdata2'] = array(
        '#type' => 'item',
        '#value' => '<a style="text-decoration:none; font-weight: normal;" class="AddOutComesBtn" onclick="copy_course_data(' . $course_id . ')">Copy Course</a>',
        '#prefix' => '<div class="clearfix" style="padding-top:133px; margin: 0 0 0 10px; width: 200px;"><div style="font-size:5px;">&nbsp;</div><div style="float:left; margin-right: 10px;">',
        '#suffix' => '</div>',
        '#attributes' => array('style' => 'margin-top: 10px;'),
    );


    $form['courses_copy']['tdata3'] = array(
        '#type' => 'item',
        '#value' => '<a class="cancelBtn" onclick="copy_course_cancel()">Cancel</a>',
        '#prefix' => '<div style="float:left">',
        '#suffix' => '</div></div>',
        '#attributes' => array('style' => 'margin-top: 10px;'),
    );

    return $form;
}

function courses_copy_data($src_course_nid, $course_name, $course_opt, $selected_year) {

    global $user;

    $selected_term_tid = arg(7);

    watchdog("copy_course", 'course_nid ' . $src_course_nid . ' coursename ' . $course_name);

    $course_node = node_load($src_course_nid);
    $dateformat = "%Y-%m-%d %H:%M:%S";
    $ctime = strftime($dateformat, strtotime(date('Y-m-d')));

    $node = new stdClass();
    $node->title = $course_name;
    $node->body = $course_node->body;
    $node->type = 'ntlp_course';   // Your specified content type
    $node->created = $ctime;
    $node->changed = $ctime;
    $node->status = 1;
    $node->promote = 0;
    $node->sticky = 0;
    $node->format = 1;
    $node->uid = $user->uid;

    node_save($node);
    $new_course_nid = $node->nid;


    /* Copy Students OF Course */
    if ($course_opt == "CSE" || $course_opt == "CE") {
        /*         * *  no enrollement copy with new course... */
//        $course_user_rs = db_query("select * from {ntlp_course_user} where course_nid = %d AND role_id=%d", $src_course_nid, NTLP_ROLEID_STUDENT);
//        while ($course_user_data = db_fetch_object($course_user_rs)) {
//
//            if ($course_user_data->user_uid != $user->uid) {
//                db_query("insert into {ntlp_course_user}
//           (course_nid, user_uid, modifiedon_date, modifiedby_uid, role_id, period)
//           values
//            (%d, %d, '%s', %d, %d, %d)", $new_course_nid, $course_user_data->user_uid, $course_user_data->modifiedon_date, $course_user_data->modifiedby_uid, $course_user_data->role_id, $course_user_data->period);
//            }
//        }
    }

    $course_result = db_query("SELECT * FROM {ntlp_course} WHERE nid=%d", $src_course_nid);
    $src_course_Obj = db_fetch_object($course_result);

    //copying course details
    db_query("INSERT INTO {ntlp_course}
        (nid, additional_notes, course_short_title, availability, driving_question,
        academic_standards, school_nid, is_project, course_nid, teacher1_uid, teacher2_uid, is_weighted_grades,
        period, school_year, picture_fid, is_advisory_course, school_term_tid, allow_self_enroll, topic_unit_tid)
        VALUES (%d, '%s', '%s', '%s', '%s', '%s', %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)", $new_course_nid, $src_course_Obj->additional_notes, $src_course_Obj->course_short_title, $src_course_Obj->availability, $src_course_Obj->driving_question, $src_course_Obj->academic_standards, $src_course_Obj->school_nid, 0, 0, $src_course_Obj->teacher1_uid, $src_course_Obj->teacher2_uid, $src_course_Obj->is_weighted_grades, $src_course_Obj->period, $selected_year, $src_course_Obj->picture_fid, $src_course_Obj->is_advisory_course, $selected_term_tid, $src_course_Obj->allow_self_enroll, $src_course_Obj->topic_unit_tid);


    if ($course_opt == "CSE" || $course_opt == "CS") {

        //Add taxonomy data (for subjects)
        $course_term_node_result = db_query("SELECT * FROM {term_node} tn
                                            INNER JOIN {term_data} td ON tn.tid=td.tid WHERE tn.nid=%d AND td.vid=%d", $src_course_nid, NTLP_VOCABULARY_SUBJECTS);
        if ($course_term_node_result->num_rows > 0) {

            $subjects = array();
            while ($course_term_node = db_fetch_object($course_term_node_result)) {

                $subjects[] = $course_term_node->tid;
            }

            if (count($subjects))
                taxonomy_node_save($node, $subjects);
        }

        // getting all course projects
        $course_project_result = db_query("SELECT c.*, n.title FROM {ntlp_course} c, {node} n
            WHERE c.nid = n.nid AND c.course_nid=%d AND c.is_project = 1", $src_course_nid);

        watchdog('copy_course12', 'course project found ' . $course_project_result->num_rows);

        if ($course_project_result->num_rows > 0) {

            while ($src_course_project = db_fetch_object($course_project_result)) {

                project_copy_data($src_course_nid, $src_course_project->nid, $src_course_project->title, $new_course_nid, $selected_term_tid, false, $course_opt);
            } // project copy end
        }

        watchdog('copy_course', 'copying course learning outcomes ');

        //Add Course Learning Outcomes
        $course_outcomes = db_query("SELECT co.*, td.name, so.tid AS so_tid 
            FROM {ntlp_gb_course_outcome} co 
            INNER JOIN {term_data} td ON td.tid = co.tid
            LEFT JOIN {ntlp_gb_outcome} so ON co.tid = so.tid AND so.school_nid = %d AND so.school_year = %d
            WHERE co.course_nid = %d", $src_course_Obj->school_nid, $selected_year, $src_course_nid);
        
        $oc_tid = 0;
        if ($course_outcomes->num_rows > 0) {
            while ($course_outcomes_data = db_fetch_object($course_outcomes)) {
                //If Schoolwide term then use as is
                if (isset($course_outcomes_data->so_tid)) {
                    $oc_tid = $course_outcomes_data->tid;
                } else {
                    //If Coursewide term then create a copy
                    $new_oc = array(
                        'vid' => VOCAB_NTLP_GB_OUTCOME,
                        'name' => $course_outcomes_data->name,
                    );

                    taxonomy_save_term($new_oc);
                    $oc_tid = $new_oc['tid'];
                }
                db_query("INSERT INTO {ntlp_gb_course_outcome} (tid, course_nid, weight_percent, custom_name, sort_order)
                    VALUES (%d, %d, %f, '%s', %d)", $oc_tid, $new_course_nid, $course_outcomes_data->weight_percent, $course_outcomes_data->custom_name, $course_outcomes_data->sort_order);
            }
        }

        // copying activities for course only
        $course_activities_result = db_query("SELECT * FROM {ntlp_course_activity} WHERE project_nid = 0 AND course_nid = %d", $src_course_nid);

        if ($course_activities_result->num_rows > 0) {

            while ($activity_rec = db_fetch_object($course_activities_result)) {

                watchdog("activity_copy", "copying " . $activity_num++ . " of " . $course_activities_result->num_rows);
                $new_cactivity_nid = copy_activity_node($activity_rec->nid);

                activity_copy_data($activity_rec->nid, $new_cactivity_nid, $new_course_nid, 0, 0, 0, $selected_term_tid, $course_opt);
            }
        }

        /*         * *  no share stuff resource copied with new course... */
        // copying resoruce for course only
//        $course_resource_result = db_query("SELECT cr.* FROM {ntlp_course_resource} cr 
//            WHERE course_nid = %d AND cr.nid NOT IN (SELECT pr.nid FROM {ntlp_project_resource} pr 
//            INNER JOIN {ntlp_course} p ON p.nid = pr.project_nid 
//            WHERE p.course_nid =%d AND p.is_project = 1 )", $src_course_nid, $src_course_nid);
//        if ($course_resource_result->num_rows > 0) {
//
//            while ($resourceObj = db_fetch_object($course_resource_result)) {
//
//                $resource_nid = copy_resource_node($resourceObj->nid, $new_course_nid);
//
//                watchdog('resource array', 'resource array' . $resource_nid);
//
//                $query = db_query("INSERT INTO {ntlp_course_resource}(nid, course_nid, fid, type, attachment_name, path, deleted_by, is_shared)
//                VALUES ( %d, %d, %d, '%s', '%s', '%s', %d, %d)", $resource_nid, $new_course_nid, $resourceObj->fid, $resourceObj->type, $resourceObj->attachment_name, $resourceObj->path, $resourceObj->deleted_by, $resourceObj->is_shared);
//            }
//        }
        //Add additional instructors
        $course_user_rs = db_query("select * from {ntlp_course_user}
            where course_nid = %d
            AND role_id = %d", $src_course_nid, NTLP_ROLEID_TEACHER);
        while ($course_user_data = db_fetch_object($course_user_rs)) {

            if ($course_user_data->user_uid != $user->uid) {

                enroll_user_in_course($new_course_nid, $course_user_data->user_uid, $course_user_data->role_id);
            }
        }
    }

    // Adding a course user table data where need to define a course moderator / primary Teacher...
    db_query("INSERT INTO {ntlp_course_user} (course_nid, user_uid, modifiedon_date, modifiedby_uid, role_id)
    VALUES( %d, %d, '%s', %d, %d)", $new_course_nid, $user->uid, strftime($dateformat, time()), $user->uid, NTLP_ROLEID_TEACHER);

    /*     * *  setting a course term based on selection. */
    db_query("INSERT INTO {ntlp_course_term} (school_term_tid, course_nid)
                VALUES (%d, %d)", $selected_term_tid, $new_course_nid);

    /*     * *  this was previous logic when based on year selection copied all the selector year term into course term. */
//    if ($src_course_Obj->school_year == $selected_year) {
//        $course_term_result = db_query("SELECT * FROM {ntlp_course_term} WHERE course_nid = %d", $src_course_nid);
//
//        if ($course_term_result->num_rows > 0) {
//            while ($course_term_Obj = db_fetch_object($course_term_result)) {
//                db_query("INSERT INTO {ntlp_course_term} (school_term_tid, course_nid)
//                VALUES (%d, %d)", $course_term_Obj->school_term_tid, $new_course_nid);
//            }
//        }
//    } else {
//        $school_terms_result = get_school_terms($src_course_Obj->school_nid, $selected_year);
//        if ($school_terms_result->num_rows > 0) {
//            while ($school_term_obj = db_fetch_object($school_terms_result)) {
//
//                db_query("INSERT INTO {ntlp_course_term} (school_term_tid, course_nid)
//                VALUES (%d, %d)", $school_term_obj->tid, $new_course_nid);
//            }
//        }
//    }
    // create custome outcome if no outcome added in new_school year

    $outcome_arr = check_current_course_outcome_with_newyear($src_course_nid, $selected_year);
    if (count($outcome_arr) > 0) {
        foreach ($outcome_arr as $outcome) {

            $new_term = array('vid' => $outcome->vid, 'name' => $outcome->name, 'description' => $outcome->description);
            if (taxonomy_save_term($new_term)) {

                db_query("UPDATE {ntlp_gb_course_outcome} SET tid = %d  WHERE tid = %d AND course_nid = %d", $new_term['tid'], $outcome->tid, $new_course_nid);
                db_query("UPDATE {ntlp_gb_activity_outcome} ao, {ntlp_course_activity} a SET ao.tid = %d 
                    WHERE ao.tid = %d AND ao.activity_nid = a.nid AND a.course_nid = %d ", $new_term['tid'], $outcome->tid, $new_course_nid);
            }
        }
    }
    drupal_set_message("<div><p>Course: <b>$course_name</b> - created successfully</p>" .
            l('View Course', 'ntlp/courses/home/' . $new_course_nid) . '</div>');
}

function copy_course_validation($course_nid, $select_school_year) {
    $message = "";
    $warning = 0;

//    if (!check_target_year_terms($selected_school_year)) {
//        $message = '<p>Terms are not available in the selected year, cannot copy course.</p>';
//        $warning = 2;
//    } else {
    $records = check_current_course_outcome_with_newyear($course_nid, $select_school_year);
    watchdog('copy_val', print_r($records, true));
    if (count($records) > 0) {
        $message = '<div style="width:auto; height:auto;"><p>The following schoolwide outcomes do not exist in the destination school year:</p><ul>';
        foreach ($records as $record) {
            $message .= '<li>' . $record->name . '</li>';
        }
        $message .= '</ul></div><p>If you proceed, these will be created as “custom” outcomes in the new course and will not be associated with schoolwide outcome averages.  Are you sure you want to continue?</p>';
        $warning = 1;
    }
//    }

    drupal_json(array('warning' => $warning,
        'message' => $message,
    ));
}

?>