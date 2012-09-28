<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


require_once "data_access.inc";
/* popup form */

function ntlp_project_copy_form($form_state = null, $course_id, $project_id) {
    global $user;

    drupal_set_title("Copy Project");

    $project_Obj = node_load($project_id);

    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/ntlp_project.js');

    ahah_helper_register($form, $form_state);
    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/project_ahah_helper.js', 'footer');

    $newname = get_new_project_name($project_Obj->title);
    $form['project_add'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div id="dialog-show-confirmation" style="display: none;" title="Warning">
            <span class="msg"></span>
            </div><div id="project-copy-wrapper" style="width:560px;height:220px;" >
            <div id="BoldrProject" style="width:auto">
	<div class="ProjectDetail">
		<div class="LightestBlueBottomRightCorner">
			<div class="LightestBlueBottomLeftCorner">
				<div class="LightestBlueTopRightCorner">
					<div class="LightestBlueTopLeftCorner">
					',
        '#suffix' => '</div>
                    </div>
                 </div>
              </div>
            </div>
        </div>
    </div>',
    );


    $form['project_add']['project_id'] = array(
        '#type' => 'hidden',
        '#value' => $project_id,
    );
    $form['project_add']['course_nid'] = array(
        '#type' => 'hidden',
        '#value' => $course_id,
    );



    $form['project_add']['activity_name_heading'] = array(
        '#type' => 'textfield',
        '#value' => $newname,
        '#prefix' => '<div style="margin-bottom: 10px; color:#3570AA;font-weight:bold;white-space:nowrap; margin-left: 10px;"><div style="float:left;width:120px;">' . t('New Project Name') . '</div><div style="padding-left: 160px;">',
        '#suffix' => '</div></div>',
        '#id' => 'txt_project_name',
        '#size' => 50,
    );



     $school_years = get_school_years_for_staff(get_this_school());

    if (isset($form_state['storage']['project_add']['school_year'])) {
        $selected_year = $form_state['storage']['project_add']['school_year'];
    } else {
        $selected_year = get_this_course_year($course_id);
    }

    if (isset($form_state['storage']['project_add']['courses'])) {
        $selected_course_nid = $form_state['storage']['project_add']['courses'];
    } else {
        $selected_course_nid = $course_id;
    }

    if (isset($selected_course_nid) && $selected_course_nid > 0) {
        if (!is_course_exist_in_year($selected_course_nid, $selected_year)) {
            $selected_course_nid = 0;
        }
    } else {
        $selected_course_nid = 0;
    }

    $form['project_add']['school_year'] = array(
        '#type' => 'select',
        '#options' => $school_years,
        '#default_value' => $selected_year,
        '#ahah' => array(
            'path' => ahah_helper_path(array('project_add')),
            'wrapper' => 'project-copy-wrapper',
            'progress' => 'none',
        ),
        '#prefix' => '<div class="clearfix" style="color:#3570AA;font-weight:bold;white-space:nowrap; margin-left: 10px;"><div style="float:left;width:120px;">' . t('School Year') . '</div><div style="padding-left: 162px;"><span class="comboboxBgLeft" style="width:100px;">',
        '#suffix' => '</span></div></div>',
        '#attributes' => array('class' => 'my-dropdown'),
    );

    $teacher_courses = get_teacher_courses_in_year($user->uid, $selected_year);

    if (sizeof($teacher_courses) > 0) {
        foreach ($teacher_courses as $course) {
            $options[0] = 'Select Course';
            $options[$course->nid] = $course->title;
        }
    } else {
        $options[0] = 'No Course Available';
    }


    $form['project_add']['courses'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => $selected_course_nid,
        '#ahah' => array(
            'path' => ahah_helper_path(array('project_add')),
            'wrapper' => 'project-copy-wrapper',
            'progress' => 'none',
        ),
        '#prefix' => '<div class="clearfix" style="color:#3570AA;font-weight:bold;white-space:nowrap; margin-left: 10px;"><div style="float:left;width:120px;">' . t('Select Course') . '</div><div style="padding-left: 162px;"><span class="comboboxBgLeft" style="width:200px;">',
        '#suffix' => '</span></div></div>',
        '#id' => 'selected_course_nid',
        '#attributes' => array('class' => 'my-dropdown'),
    );

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

    $form['project_add']['courses_terms'] = array(
        '#type' => 'select',
        '#options' => $course_terms,
        '#default_value' => $selected_term_tid,
        '#prefix' => '<div class="clearfix" style="color:#3570AA;font-weight:bold;white-space:nowrap; margin-left: 10px;"><div style="float:left;width:120px;">' . t('Select Term') . '</div><div style="padding-left: 162px;"><span class="comboboxBgLeft" style="width:200px;">',
        '#suffix' => '</span></div></div>',
        '#id' => 'selected_term_tid',
        '#attributes' => array('class' => 'my-dropdown'),
    );

    $form['project_add']['row3'] = array(
        '#type' => 'item',
        '#value' => '<a style="margin-right:6px; float: right; text-decoration: none;" href="#" class="AddOutComesBtn" onclick ="copy_project(' . $course_id . ',' . $project_id . ')">Copy Project</a>',
        '#prefix' => '<div style="float:right;padding-top:30px; margin: 0 0 0 10px; width: 200px;"><div style="font-size:5px;">&nbsp;</div><div style="float:left; margin-right: 5px;">',
        '#suffix' => '</div>',
    );
    
     $form['project_add']['tdata3'] = array(
        '#type' => 'item',
        '#value' => '<input type="submit" value="Cancel" class="CancelBtn" onclick="parent.Drupal.modalFrame.close();return false;">',
        '#prefix' => '<div style="float:left">',
        '#suffix' => '</div></div>',
        '#attributes' => array('style' => 'margin-top: 10px;'),
    );



    return $form;
}

function project_copy_data($src_course_id, $src_project_id, $project_name, $copy_to_course_nid, $selected_term_tid = 0, $showmsg = true, $course_opt = "COPY") {
    global $user;

    watchdog('proj_copy', 'src_course_nid ' . $src_course_id . ' src_project_id ' . $src_project_id . ' project name
        ' . $project_name . ' copy to course nid ' . $copy_to_course_nid . ' selected_term_tid ' . $selected_term_tid);


    if (isset($form_state['values']['op'])) {
        $op = $form_state['values']['op'];
    }

    $nodeProject = node_load($src_project_id);
    $dateformat = "%Y-%m-%d %H:%M:%S";
    $ctime = strftime($dateformat, strtotime(date('Y-m-d')));

    $project_node = new stdClass();
    $project_node->title = $project_name;
    $project_node->body = $nodeProject->body;
    $project_node->type = 'ntlp_project';   // Your specified content type
    $project_node->created = $ctime;
    $project_node->changed = $ctime;
    $project_node->status = 1;
    $project_node->promote = 0;
    $project_node->sticky = 0;
    $project_node->format = 1;
    $project_node->uid = $user->uid;
    $project_node->context = $copy_to_course_nid;

    node_save($project_node);
    $new_project_nid = $project_node->nid;

    $result = db_query("SELECT * FROM {ntlp_course} WHERE nid=%d", $src_project_id);
    $projectObj = db_fetch_object($result);

    db_query("INSERT INTO {ntlp_course} (nid, additional_notes, course_short_title, availability, from_date, to_date ,
        driving_question, academic_standards, school_nid, is_project, course_nid, teacher1_uid, teacher2_uid,
        is_weighted_grades, period, school_year, picture_fid ) VALUES (%d, '%s', '%s', '%s', '%s', '%s', '%s', '%s',
        %d, %d, %d, %d, %d, %d, %d, %d, %d)", $new_project_nid, $projectObj->additional_notes, $projectObj->course_short_title, 'H', $ctime, $ctime, $projectObj->driving_question, $projectObj->academic_standards, $projectObj->school_nid, $projectObj->is_project, $copy_to_course_nid, $projectObj->teacher1_uid, $projectObj->teacher2_uid, $projectObj->is_weighted_grades, $projectObj->period, $projectObj->shcool_year, $projectObj->picture_fid);


//copying project section details
    $result = db_query("SELECT * FROM {ntlp_course_section} WHERE course_nid=%d", $src_project_id);

    if ($result->num_rows > 0) {
        while ($sectionRows = db_fetch_object($result)) {
            $new_section_tid = copy_section_data($sectionRows->tid);
            db_query("INSERT INTO {ntlp_course_section}(tid,course_nid,section_order, availability)
            VALUES (%d, %d, %d, '%s')", $new_section_tid, $new_project_nid, $sectionRows->section_order, 'H');

//            $resourceResult = db_query("SELECT p.*, crs.* FROM dpl_ntlp_course_section cs
//                INNER JOIN dpl_ntlp_project_resource p ON cs.tid = p.section_tid
//                INNER JOIN dpl_ntlp_course_resource crs ON p.nid = crs.nid
//                WHERE cs.tid=%d AND crs.deleted_on IS NULL
//                AND crs.course_nid = %d
//                AND cs.course_nid=%d
//                ORDER BY p.nid", $sectionRows->tid, $src_course_id, $src_project_id);

            $resourceResult = _get_project_section_resources($src_course_id, $sectionRows->tid);

            watchdog('copy_proj', 'result set size ' . print_r($resourceResult, true));
            if ($resourceResult) {
                foreach ($resourceResult as $resourseRows) {

                    watchdog('copy_proj', 'resource record ' . print_r($resourseRows, true));
                    if ($resourseRows->restype == 'R') { // copy resource
                        $resource_nid = copy_resource_node($resourseRows->nid, $new_project_nid);

                        if ($deleted_on == null) {

                            $deleted_on = 'null';
                        }

                        db_query("INSERT INTO {ntlp_course_resource}
                        (SELECT %d, %d, fid, type, attachment_name, path, deleted_by, deleted_on, is_shared
                        FROM {ntlp_course_resource} WHERE nid = %d)", $resource_nid, $copy_to_course_nid, $resourseRows->nid);

                        /* $query = db_query("INSERT INTO {ntlp_course_resource}(nid, course_nid,
                          fid, type, path, deleted_by, deleted_on, is_shared)
                          VALUES(%d, %d, '%s', '%s', '%s', %d, NULL, %d)",
                          $resource_nid, $copy_to_course_nid, $resourseRows->fid, $resourseRows->type,
                          $resourseRows->path, $resourseRows->deleted_by, $resourseRows->is_shared); */


                        $query = db_query("INSERT INTO {ntlp_project_resource}(nid, project_nid,
                        resource_order, section_tid, availability) VALUES(%d, %d, '%d', '%d', 'H')", $resource_nid, $new_project_nid, $resourseRows->resource_order, $new_section_tid);

                        $resource_attch_result = db_query("SELECT name, resource_nid , pebl_attachment_fid, type,
                        path, order_number FROM dpl_ntlp_resource_attachment
                        WHERE resource_nid = %d", $resourseRows->nid);


                        if ($resource_attch_result->num_rows > 0) {

                            while ($data = db_fetch_object($resource_attch_result)) {
                                db_query("INSERT INTO dpl_ntlp_resource_attachment (name, resource_nid, pebl_attachment_fid, type, path, order_number)
                                VALUES('%s', %d, %d, '%s', '%s', %d)", $data->name, $resource_nid, $data->pebl_attachment_fid, $data->type, $data->path, $data->order_number);
                            }
                        }
                    } else if ($resourseRows->restype == 'A') { // copy activity
                        $new_activity_nid = copy_activity_node($resourseRows->nid);
                        activity_copy_data($resourseRows->nid, $new_activity_nid, $copy_to_course_nid, $new_project_nid, $new_section_tid, $resourseRows->resource_order, $selected_term_tid, $course_opt);

//                        copy_activity_into_section($new_project_nid, $resourseRows->nid, $new_section_tid, $resourseRows->resource_order, $copy_to_course_nid);
                    }
                }
            }
////////////// copying project resources ends here //////////////
        }
    }
    if ($showmsg) {
        drupal_set_message("<div><p>Your project was successfully copied.</p>" .
                l('View New Project', 'ntlp/courses/projects/' . $copy_to_course_nid . '/' . $project_node->nid) . '</div>');
    }
}

?>