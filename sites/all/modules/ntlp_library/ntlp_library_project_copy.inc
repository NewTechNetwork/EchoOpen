<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');

function ntlp_library_projects_copy_frm($project_nid) {

    return drupal_get_form('ntlp_library_projects_copy_form', $project_nid);
}

/* popup form */

function ntlp_library_projects_copy_form($form= null, $project_nid) {

    global $user;
    modalframe_child_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');

    $lib_proj = node_load($project_nid);



    drupal_add_js(drupal_get_path('module', 'ntlp_library') . '/ntlp_library.js');

    $form['copy_project'] = array(
        '#type' => 'fieldset',
        '#tree' => true,
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

    $form['copy_project']['project_nid'] = array(
        '#type' => 'hidden',
        '#value' => $lib_proj->nid,
        '#id' => 'project_nid'
    );
    $form['copy_project']['table'] = array(
        '#type' => 'item',
        '#prefix' => '<table align="center" cellpadding="5">',
        '#suffix' => '</table>',
    );


    $form['copy_project']['table']['row1'] = array(
        '#type' => 'item',
        '#prefix' => '<tr>',
        '#suffix' => '</tr>',
    );

    $form['copy_project']['table']['row1']['activity_name_heading'] = array(
        '#type' => 'item',
        '#value' => t('Project Name'),
        '#prefix' => '<td style="color:#3570AA;font-weight:bold">',
        '#suffix' => '</td>',
    );


    $form['copy_project']['table']['row1']['activity_name'] = array(
        '#type' => 'textfield',
        '#value' => $lib_proj->title,
        '#id' => 'selected_project_name',
        '#prefix' => '<td>',
        '#suffix' => '</td>',
    );

    $form['copy_project']['table']['row2'] = array(
        '#type' => 'item',
        '#prefix' => '<tr>',
        '#suffix' => '</tr>',
    );

    $form['copy_project']['table']['row2']['course_name_heading'] = array(
        '#type' => 'item',
        '#value' => t('Course Name'),
        '#prefix' => '<td style="color:#3570AA;font-weight:bold">',
        '#suffix' => '</td>',
    );


    $result = get_user_courses($user->uid, NTLP_ROLEID_TEACHER);

    while ($course_obj = db_fetch_object($result)) {

        $options[$course_obj->course_nid] = $course_obj->title;
    }


    if (empty($options)) {
        $options[] = "No Course Found";
        $message = "You should enroll into the course before you copy";
    }


    $form['copy_project']['table']['row2']['course_nid'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#id' => 'selected_course_nid',
        '#prefix' => '<td>',
        '#suffix' => '</td>',
    );


    $form['copy_project']['table']['row3'] = array(
        '#type' => 'item',
        '#prefix' => '<tr>',
        '#suffix' => '</tr>',
    );
    $form['copy_project']['table']['row3']['td'] = array(
        '#type' => 'item',
        '#prefix' => '<td>',
        '#suffix' => '</td>',
    );

    if (!isset($message)) {

        $form['copy_project']['table']['row3']['save'] = array(
            '#type' => 'item',
            '#value' => '<a class="AddOutComesBtn" href="#" onclick ="copy_project()">Copy Project</a>',
            '#prefix' => '<td>',
            '#suffix' => '</td>',
        );
    } else {

        $form['copy_project']['table']['row3']['save'] = array(
            '#type' => 'item',
            '#value' => $message,
            '#prefix' => '<td>',
            '#suffix' => '</td>',
        );
    }


    return $form;
}

function ntlp_library_project_template_data($scr_proj_nid, $target_course_nid, $target_proj_nid) {
 
    copy_lib_project_data($scr_proj_nid, $target_course_nid, $target_proj_nid, 1);
}

function ntlp_library_project_copy_data($project_nid, $course_id, $project_name) {
    global $user;

    watchdog("lib_proj_copy", 'project id ' . $project_nid . ' course ' . $course_id . ' ' . $project_name);
    $dateformat = DATE_FORMAT;
//    $lib_project_Obj = _get_library_project($project_nid);
//    $lib_project_id = $project_nid; //$lib_project_Obj->src_project_nid;

    $nodeProject = node_load($project_nid);

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
    $project_node->context = $course_id;
    
    node_save($project_node);


    $result = db_query("SELECT * FROM {ntlp_course} WHERE nid=%d", $project_nid);
    $projectObj = db_fetch_object($result);

//copying project details
    db_query("INSERT INTO {ntlp_course}
        (nid,additional_notes,course_short_title,availability,from_date,to_date,driving_question,academic_standards,
        school_nid,is_project,course_nid,teacher1_uid,teacher2_uid,is_weighted_grades,period,school_year,picture_fid)
        VALUES (%d,'%s','%s','%s','%s','%s','%s','%s',%d,%d,%d,%d,%d,%d,%d,%d,%d)", $project_node->nid, $projectObj->additional_notes, $projectObj->course_short_title, $projectObj->availability, $ctime, $ctime, $projectObj->driving_question, $projectObj->academic_standards, $projectObj->school_nid, $projectObj->is_project, $course_id, $projectObj->teacher1_uid, $projectObj->teacher2_uid, $projectObj->is_weighted_grades, $projectObj->period, $projectObj->shcool_year, $projectObj->picture_fid
    );

    copy_lib_project_data($project_nid, $course_id, $project_node->nid, 0);

    drupal_set_message("<div><p>Project: <b>$project_name</b> - created successfully</p>" .
            l('View Project', 'ntlp/courses/projects/' . $course_id . '/' . $project_node->nid) . '</div>');
}

function copy_lib_project_data($lib_project_id, $target_course_nid, $target_proj_nid, $only_briefcase = 0) {
    global $user;

//copying project section details
    $result = db_query("SELECT * FROM {ntlp_course_section} WHERE course_nid=%d", $lib_project_id);
    if (db_affected_rows($result) > 0) {
        while ($sectionRows = db_fetch_object($result)) {
            $new_section_tid = copy_section_data($sectionRows->tid);
            db_query("INSERT INTO {ntlp_course_section}(tid,course_nid,section_order, availability)
            VALUES (%d,%d,%d, %d)", $new_section_tid, $target_proj_nid, $sectionRows->section_order, $sectionRows->availability);

            $resourceResult = _get_project_section_resources($lib_project_id, $sectionRows->tid);

            if ($resourceResult) {
                foreach ($resourceResult as $resourseRows) {

                    if ($resourseRows->restype == 'R') { // copy resource
                        $resource_nid = copy_resource_node($resourseRows->nid, $target_proj_nid);

                        if ($deleted_on == null) {

                            $deleted_on = 'null';
                        }

                        db_query("INSERT INTO {ntlp_course_resource}
                        (SELECT %d, %d, fid, type, attachment_name, path, deleted_by, deleted_on, is_shared
                        FROM {ntlp_course_resource} WHERE nid = %d)", $resource_nid, $target_course_nid, $resourseRows->nid);

                        $query = db_query("INSERT INTO {ntlp_project_resource}(nid, project_nid,
                        resource_order, section_tid, availability) VALUES(%d, %d, '%d', '%d', 'H')", $resource_nid, $target_proj_nid, $resourseRows->resource_order, $new_section_tid);

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
                        copy_activity_into_section($target_proj_nid, $resourseRows->nid, $new_section_tid, $resourseRows->resource_order, $target_course_nid);
                    }
                }
            }
////////////// copying project resources ends here //////////////
        }
    }
    $dateformat = DATE_FORMAT;
    $action = "C";
    $action_on = strftime($dateformat, time());

    db_query("INSERT INTO {ntlp_library_project_usage} (src_project, action, action_by, action_on, target_project )
        VALUES ( %d, '%s', %d, '%s', %d )", $lib_project_id, $action, $user->uid, $action_on, $target_proj_nid);


    return;
}

function get_selected_school_courses($school_nid) {
    $result = db_query("SELECT * FROM {ntlp_course} WHERE school_nid = %d", $school_nid);

    if ($result->num_rows > 0) {
        while ($data = db_fetch_object($result)) {
            $array[] = $data;
        }
        return $array;
    } else {
        return false;
    }
}

?>