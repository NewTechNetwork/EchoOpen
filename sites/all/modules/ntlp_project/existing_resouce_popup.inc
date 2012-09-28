<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


/* popup form */

function ntlp_add_existing_resource_form($form_state = null, $course_id, $project_id, $section_tid) {
    modalframe_child_js();
    drupal_set_title("Add Existing Resource");
//    $course_id = check_plain(arg(4));
//    $project_id = check_plain(arg(5));
//    $section_tid = check_plain(arg(6));

    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/ntlp_project.js');

    $form['project_add'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => $HTMLform_activity_outer_name_hdr,
        '#suffix' => $HTMLform_activity_outer_name_ftr,
    );

    $form['project_add']['project_id'] = array(
        '#type' => 'hidden',
        '#value' => $project_id,
        '#id' => 'project_id',
    );

    $form['project_add']['section_tid'] = array(
        '#type' => 'hidden',
        '#value' => $section_tid,
        '#id' => 'section_tid',
    );

    $form['project_add']['course_id'] = array(
        '#type' => 'hidden',
        '#value' => $course_id,
        '#id' => 'course_id',
    );

//    $form['project_add']['resource_name'] = array(
//            '#type' => 'textfield',
//            '#value' =>  '',
//            '#size' => '30',
//            '#id' => 'searching_text',
//            '#field_prefix' => t('Search'),
//            '#attributes' => array('onkeypress' => "resources_search(".$course_id.");"),
//    );

    $form['project_add']['search_result'] = array(
        '#type' => 'item',
        '#value' => '<div id="search_resources">' . get_searched_resrouces('', $course_id) . '</div>',
    );


    $form['project_add']['save_button'] = array(
        '#type' => 'item',
        '#value' => '<div class="Activities_btn"><a onclick="add_existing_resource()" >Add Resource</a></div>',
//            '#submit' => array('add_existing_resrouce_data'),
    );

    return $form;
}

function add_existing_resrouce_data($resource_nids, $project_nid, $section_tid, $mode) {
    global $user;

    $course_nid = _get_project_course_id($project_nid);

    if ($mode == 'resource') {
        watchdog('existing_resource', 'resource nid ' . $resource_nids . ' project id ' . $project_nid . ' section tid ' . $section_tid . ' mode ' . $mode);


        if (isset($resource_nids) && !empty($resource_nids)) {
            $ids = explode(",", $resource_nids);         //Explode multiple users using pipe (,) delimiter
            foreach ($ids as $resource_id) {
                $resource_order = 0;
                if (!empty($resource_id)) {


                    $record = _get_resource_order($course_nid, $section_tid);

                    if ($record != false) {
                        $resource_order = $record->resource_order + 1;
                    } else {
                        $resource_order = 0;
                    }

                    $resource_obj = get_resource_detail($resource_id);

                    if ($resource_obj) {

                        $resource_node = new stdClass();
                        $resource_node->title = $resource_obj->title;
                        $resource_node->type = $resource_obj->type;   // Your specified content type
                        $resource_node->created = time();
                        $resource_node->body = $resource_obj->body;
                        $resource_node->status = 1;
                        $resource_node->sticky = 0;
                        $resource_node->format = 1;       // Filtered HTML
                        $resource_node->uid = $user->uid;
                        $resource_node->comment = 2;

                        node_save($resource_node);

                        db_query("INSERT INTO {ntlp_course_resource} (nid, course_nid, fid, type, path, is_shared)
                VALUES(%d, %d, %d, '%s', '%s', %d)", $resource_node->nid, $resource_obj->course_nid,
                                $resource_obj->fid, $resource_obj->type, $resource_obj->path, $resource_obj->is_shared);

                        db_query("INSERT INTO {ntlp_project_resource}
                ( nid, project_nid, resource_order, section_tid, availability )
                VALUES(%d, %d, %d, %d, '%s')",
                                $resource_node->nid, $project_nid, $resource_order, $section_tid, 'H');

                        $resource_attch_result = db_query("SELECT name, resource_nid , pebl_attachment_fid, type,
                        path, order_number FROM dpl_ntlp_resource_attachment
                        WHERE resource_nid = %d", $resource_id);


                        if ($resource_attch_result->num_rows > 0) {

                            while ($data = db_fetch_object($resource_attch_result)) {
                                db_query("INSERT INTO dpl_ntlp_resource_attachment (name, resource_nid, pebl_attachment_fid, type, path, order_number)
                                VALUES('%s', %d, %d, '%s', '%s', %d)", $data->name, $resource_node->nid,
                                        $data->pebl_attachment_fid, $data->type, $data->path, $data->order_number);
                            }
                        }
                    }
                }
            }
        }
    } else {

        if (isset($resource_nids) && !empty($resource_nids)) {
            $ids = explode(",", $resource_nids);         //Explode multiple users using pipe (,) delimiter
            foreach ($ids as $resource_id) {
                if (!empty($resource_id)) {

                    $record = _get_resource_order($course_nid, $section_tid);

                    if ($record != false) {
                        $resource_order = $record->resource_order + 1;
                    } else {
                        $resource_order = 0;
                    }

                    copy_activity_into_section($project_nid, $resource_id, $section_tid, $resource_order);
                }
            }
        }
    }
}

function copy_activity_into_section($project_nid, $activity_nid, $section_tid, $resource_order, $course_nid = 0) {
    global $user;
    $dateformat = "%Y-%m-%d %H:%M:%S";


    $activityObj = get_activity_records($activity_nid);

    $activity_attachments = get_attachment($activity_nid, 'ntlp_activity_attachment', 'activity_nid');


    $activity_node->title = $activityObj->title;
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

    $actvitynodeid = $activity_node->nid;

    $publishdate_value = $activityObj->available_from = '0000-00-00 00:00:00';
    $hidedate_value = $activityObj->available_to = '0000-00-00 00:00:00';
    $due_date = $activityObj->due_date;
    if ($due_date == null) {

        $due_date = '0000-00-00 00:00:00';
    }

    $school_nid = get_this_school();

    db_query("INSERT INTO {ntlp_course_activity} (nid, course_nid, project_nid, activity_order, project_cat, submission_type,
        allow_resubmission, availability_mode, available_from, available_to, due_date, is_graded, grading_method,
        curriculum_unit_tid, curriculum_standards, school_term_tid, timecreated, timemodified, created_by,
        modified_by)
        VALUES(%d, %d, %d, %d, %d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s',
        %d, '%s', %d, '%s', '%s', %d, %d)",
            intval($activity_node->nid), (($course_nid == 0 ) ? $activityObj->course_nid : $course_nid), intval($project_nid), $resource_order, $section_tid,
            check_plain($activityObj->submission_type), intval($activityObj->allow_resubmission), 'M',
            check_plain($publishdate_value), check_plain($hidedate_value), check_plain($due_date), intval($activityObj->is_graded),
            check_plain($activityObj->grading_method), intval($activityObj->curriculum_unit_tid), check_plain($activityObj->curriculum_standards),
            get_current_selected_course_term((($course_nid == 0 ) ? $activityObj->course_nid : $course_nid)), strftime($dateformat, time()), strftime($dateformat, time()),
            intval($user->uid), intval($user->uid));

    if ($activity_attachments != null) {
        foreach ($activity_attachments as $attchmentObj) {
            watchdog('iam inside', 'attachments' . print_r($activity_node->nid, true));

            db_query("INSERT INTO {ntlp_activity_attachment} (activity_nid, type, path, is_deleted, fid )
                VALUES( %d, '%s', '%s', %d, %d)", intval($activity_node->nid), check_plain($attchmentObj->type), check_plain($attchmentObj->path),
                    intval($attchmentObj->is_deleted), intval($attchmentObj->fid));
        }
    }

    $activity_outcomes = get_activity_outcome($activity_nid);

    if ($activity_outcomes) {
        foreach ($activity_outcomes as $outcome) {
            db_query("INSERT INTO {ntlp_gb_activity_outcome} VALUES (%d, %d, %d )", $outcome->tid, $activity_node->nid, $outcome->max_points);
        }
    }

    if ($activityObj->type == 'lo_rubric') {


        $activity_rubric_Obj = get_rubric_records($activity_nid);

        // copy rubric activity record
        watchdog("activyt_cat1", print_r($activity_rubric_Obj, true));
        db_query("insert into {ntlp_course_activity_rubric}
                    (activity_nid, rubric_style, outcome_tid, self_evaluation, automatic_evaluation)
                    values (%d, %d, %d, %d, %d)",
                $activity_node->nid, $activity_rubric_Obj->rubric_style, $activity_rubric_Obj->outcome_tid,
                $activity_rubric_Obj->self_evaluation, $activity_rubric_Obj->automatic_evaluation);


        // copy rubric level and category

        $cat_record = get_rubric_activity_categories($activity_nid);

//        print_r($cat_record);

        watchdog("activyt_cat2", print_r($cat_record, true));

        if ($cat_record) { // if not empty
            foreach ($cat_record as $category) {

                //save category
                db_query("INSERT INTO {ntlp_activity_rubric_category}
                 (activity_nid, category_order, max_points, category_name) VALUES (%d, %d, %d, '%s')",
                        $activity_node->nid, $category->category_order, $category->max_points,
                        $category->category_name);

                $category_levels = get_rubric_activity_category_level($category->category_ID);

                $newCateory_id = db_last_insert_id('dpl_ntlp_activity_rubric_category', 'category_ID');
                watchdog("activyt_cat3", print_r($category_levels, true));

                foreach ($category_levels as $level) {

                    //save LEVEL
                    db_query("insert into {ntlp_activity_rubric_level} (activity_nid, level_order, level_name)
                             values (%d, %d, '%s')", $activity_node->nid, $level->level_order, $level->level_name);

                    /* get record of category and level relationship and then add */
                    $result = db_query("SELECT * from {ntlp_activity_rubric_level_category} WHERE
                     category_ID = %d and level_ID = %d ", $category->category_ID, $level->level_ID);

                    $newLevel_id = db_last_insert_id('dpl_ntlp_activity_rubric_level', 'level_ID');
                    if ($result->num_rows > 0) {
                        $rub_level_cate = db_fetch_object($result);

                        //save LEVEL AND CATEGORY RELATION
                        db_query("insert into {ntlp_activity_rubric_level_category}(category_ID, level_ID, description) VALUES
                             (%d, %d, '%s')", $newCateory_id, $newLevel_id, $rub_level_cate->description);
                    }
                }
            }
        }
    }
}

function _get_last_resource_order($section_tid) {
    $result = db_query("SELECT Max(resource_order) AS resource_order FROM {ntlp_project_resource} WHERE section_tid = %d ", $section_tid);

    if ($result) {
        $rec = db_fetch_object($result);
        return $rec;
    } else {
        return false;
    }
}

function get_searched_resrouces($search_text, $course_nid) {

    $find = array();

    $res = db_query("SELECT cr.nid, n.title, n.type
        FROM {ntlp_course_resource} cr
        INNER JOIN {node} n ON cr.nid = n.nid
        WHERE n.title LIKE '%" . $search_text . "' AND cr.course_nid = %d", $course_nid);

    echo '<div class="ntlp_people_finder" style="padding:10px"><div class="content"><div class="top_right"><div class="top_left"><div class="bot_left"><div class="bot_right_2"><div style="height:200px;overflow-y:auto;padding:0px;margin:10px" class="view-content" ><table cellpadding="0" class="views-table" cellspacing="0" width="100%"><thead><tr><th>Select</th><th>Resource</th></tr></thead><tbody>';

    if ($res != null) {

        // Using Counter so that we can get multiple date for same hidden filed
        $i = 0;

        while ($row = db_fetch_array($res)) {

//            print_r('<tr><td><input type="checkbox" name="uid[]" value='.$row['nid'].' /><input type="hidden" name="uids'.$row['nid'].'" value="'.$row['nid'].'" /></td><td>'.$row['title'].'<input type="hidden" name="names'.$i.'" value="'.$row['title'].'" /></td></tr>');
            print_r('<tr><td><input type="radio" name="rid[]" value=' . $row['nid'] . ' /></td><td>' . $row['title'] . '</td></tr>');
        }
    } else {

        echo 'No Record Founds';
    }
    echo '</tbody></table></div></div></div></div></div></div></div>';
}

function _get_resrouce_detail($r_nid) {

    $rs = db_query("SELECT n.title, nr.body, cr.type, cr.path, n.type as node_type FROM {ntlp_course_resource} cr
            INNER JOIN {node} n ON cr.nid = n.nid
            INNER JOIN {node_revisions} nr ON cr.nid = nr.nid
            WHERE cr.nid = %d", $r_nid);

    $record = db_fetch_object($rs);
    return $record;
}

?>