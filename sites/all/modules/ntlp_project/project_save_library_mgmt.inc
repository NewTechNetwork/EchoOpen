<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


/* popup form */

function ntlp_project_save_to_library_form($form_state = null, $course_id, $project_id) {

    drupal_set_title("Save Project to Library");

    drupal_add_css(drupal_get_path('theme', 'Boldr') . '/StylishSelect/stylish-select.css', 'module', 'all', FALSE);
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/externalFile.js');
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/jquery.stylish-select.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/ntlp_project.js');

    $project_Obj = node_load($project_id);

//    print_r($project_Obj);
    $form['project_add'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div id="project_add_test">' . $HTMLform_activity_outer_name_hdr,
        '#suffix' => $HTMLform_activity_outer_name_ftr . '</div>',
    );


    $form['project_add']['project_id'] = array(
        '#type' => 'hidden',
        '#value' => $project_id,
    );
    $form['project_add']['course_nid'] = array(
        '#type' => 'hidden',
        '#value' => $course_id,
    );
    $form['project_add']['table'] = array(
        '#type' => 'item',
        '#prefix' => '<div id="BoldrProject" style="width:auto">
	<div class="ProjectDetail">
		<div class="LightestBlueBottomRightCorner">
			<div class="LightestBlueBottomLeftCorner">
				<div class="LightestBlueTopRightCorner">
					<div class="LightestBlueTopLeftCorner">
					<table cellpadding="5">',
        '#suffix' => '</table></div>
				</div>
			</div>
		</div>
	</div>
</div>',
    );

    $err = validate_project_for_library($course_id, $project_id);
    if (!is_string($err)) {



        $newname = get_new_project_name($project_Obj->title);



        $form['project_add']['table']['row1'] = array(
            '#type' => 'item',
            '#prefix' => '<tr>',
            '#suffix' => '</tr>',
        );

        $form['project_add']['table']['row1']['activity_name_heading'] = array(
            '#type' => 'item',
            '#value' => t('New Project Name'),
            '#prefix' => '<td style="color:#3570AA;font-weight:bold;">',
            '#suffix' => '</td>',
        );


        $form['project_add']['table']['row1']['project_name'] = array(
            '#type' => 'textfield',
            '#value' => $newname,
            '#id' => 'txt_project_name',
            '#prefix' => '<td>',
            '#suffix' => '</td>',
        );


//        $form['project_add']['table']['row3'] = array(
//            '#type' => 'item',
//            '#prefix' => '<tr>',
//            '#suffix' => '</tr>',
//        );
//
//        $form['project_add']['table']['row3']['td_1'] = array(
//            '#type' => 'item',
//            '#value' => t('Keywords'),
//            '#prefix' => '<td style="color:#3570AA;font-weight:bold;">',
//            '#suffix' => '</td>',
//        );
//
//        $form['project_add']['table']['row3']['td_2'] = array(
//            '#type' => 'textfield',
//            '#id' => 'keywords',
//            '#prefix' => '<td>',
//            '#suffix' => '</td>',
//        );


        $form['project_add']['table']['row4'] = array(
            '#type' => 'item',
            '#prefix' => '<tr>',
            '#suffix' => '</tr>',
        );
        $form['project_add']['table']['row4']['td'] = array(
            '#type' => 'item',
            '#prefix' => '<td>',
            '#suffix' => '</td>',
        );

        $form['project_add']['table']['row4']['save'] = array(
            '#type' => 'item',
            '#value' => '<a href="#" class="SaveBtn" onclick ="cancel_to_library()">No</a> <div style="float:right;padding:0 10px 0 0"><a href="#" class="SaveBtn" onclick ="save_to_library(' . $course_id . ',' . $project_id . ')">Yes</a></div>',
            '#prefix' => '<td>',
            '#suffix' => '</td>',
        );
    } else {

        $form['project_add']['table']['row1'] = array(
            '#type' => 'item',
            '#prefix' => '<tr>',
            '#suffix' => '</tr>',
        );

//        $message = get_requirement();
        $form['project_add']['table']['row1']['activity_name_heading'] = array(
            '#type' => 'item',
            '#value' => $err,
            '#prefix' => '<td>',
            '#suffix' => '</td>',
        );

        $form['project_add']['table']['row2'] = array(
            '#type' => 'item',
            '#prefix' => '<tr>',
            '#suffix' => '</tr>',
        );

//        $message = get_requirement();
        $form['project_add']['table']['row2']['activity_name_heading'] = array(
            '#type' => 'item',
            '#value' => '<a style="float:left !important;" href="#" class="SaveBtn" onclick ="cancel_to_library()">Close</a>',
            '#prefix' => '<td>',
            '#suffix' => '</td>',
        );
    }

    return $form;
}

function get_requirement() {
    include "project_requirement.template.inc";

    $container = $HTMLcontainer;
    $container = str_replace('@MESSAGE', t('Please review the <a onclick="javascript:showPopup();" href="#">submission requirements</a> before you copy this Project into Project Liabrary'), $container);
    return $container;
}

function project_save_to_library_data($course_id, $src_project_nid, $project_name) {
    global $user;



    watchdog('savto_lib', 'course ' . $course_id . ' project nid ' . $src_project_nid . ' projectname ' . $project_name);
//    echo '<br>';


    $keywords_arr = array();
//    if ($keywords) {
//        $keywords_arr = explode(",", $keywords);
//    }

    $project_lib = get_project_library_nid("ntlp_library_project");

    $src_project_node = node_load($src_project_nid);

    $dateformat = "%Y-%m-%d %H:%M:%S";
    $ctime = strftime($dateformat, strtotime(date('Y-m-d')));

    $lib_project_node = new stdClass();
    $lib_project_node->title = $project_name;
    $lib_project_node->body = $src_project_node->body;
    $lib_project_node->type = 'ntlp_library_project';   // Your specified content type
    $lib_project_node->created = $ctime;
    $lib_project_node->changed = $ctime;
    $lib_project_node->status = 1;
    $lib_project_node->promote = 0;
    $lib_project_node->sticky = 0;
    $lib_project_node->format = 1;
    $lib_project_node->comment = 2;
    $lib_project_node->uid = $user->uid;

//set the vocab id and the incoming terms to it

    node_save($lib_project_node);
    $new_lib_project_nid = $lib_project_node->nid;

    $keywords_arr = get_project_subject_and_keywords($course_id, $src_project_nid);
    watchdog("proj_kw_sub", 'src project and course keywords and subject ' . print_r($keywords_arr, true));
    if ($keywords_arr) {
        foreach ($keywords_arr as $obj) {
            $terms[] = $obj->tid;
        }
        taxonomy_node_save($lib_project_node, $terms);
    }

    //copying project details start here
    /* is_active,subject, school_term_tid field missing */
    $result = db_query("SELECT * FROM {ntlp_course} WHERE nid=%d", $src_project_nid);
    $projectObj = db_fetch_object($result);
    db_query("INSERT INTO {ntlp_course}
        (nid, additional_notes, course_short_title, availability, from_date, to_date,
        driving_question, academic_standards, school_nid, is_project, course_nid, teacher1_uid,
        teacher2_uid, is_weighted_grades, period,school_year, picture_fid)
        VALUES (%d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d', '%d')",
            $new_lib_project_nid, $projectObj->additional_notes,
            $projectObj->course_short_title, $projectObj->availability, $projectObj->from_date, $projectObj->to_date,
            $projectObj->driving_question, $projectObj->academic_standards, $projectObj->school_nid,
            $projectObj->is_project, 0, $projectObj->teacher1_uid, $projectObj->teacher2_uid,
            $projectObj->is_weighted_grades, $projectObj->period, $projectObj->shcool_year, $projectObj->picture_fid
    );

    //copying project details end here
    //copying project section details starts here
    $result = db_query("SELECT * FROM {ntlp_course_section} WHERE course_nid=%d", $src_project_nid);
    if ($result->num_rows > 0) {
        while ($sectionRows = db_fetch_object($result)) {
            $new_section_tid = copy_section_data($sectionRows->tid);
            db_query("INSERT INTO {ntlp_course_section}(tid,course_nid,section_order,availability)
            VALUES ('%d', '%d', '%d', '%d')", $new_section_tid, $new_lib_project_nid, $sectionRows->section_order, $sectionRows->availability);

            ////////////// copying project resources starts here //////////////
            #removed project_nid field from the table {ntlp_course_resource}
            #$result = db_query("SELECT * FROM {ntlp_course_resource} WHERE project_nid=%d", $project_id);
//            $resourceResult = db_query("SELECT * FROM {ntlp_course_section} cs
//                INNER JOIN {ntlp_project_resource} p on cs.tid = p.section_tid
//                INNER JOIN {ntlp_course_resource} crs on p.nid = crs.nid
//                WHERE cs.tid=%d ", $sectionRows->tid);
////                WHERE cs.tid=%d and p.project_nid=%d", $sectionRows->tid,$src_project_nid);

            $resourceResult = _get_project_section_resources($course_id, $sectionRows->tid);

            watchdog('copy_proj', 'result set size ' . print_r($resourceResult, true));
            if ($resourceResult) {
                foreach ($resourceResult as $resourseRows) {

                    watchdog('copy_proj', 'resource record ' . print_r($resourseRows, true));
                    if ($resourseRows->restype == 'R') { // copy resource
                        $resource_nid = copy_resource_node($resourseRows->nid, $new_lib_project_nid);

                        if ($deleted_on == null) {

                            $deleted_on = 'null';
                        }

                        db_query("INSERT INTO {ntlp_course_resource}
                        (SELECT %d, %d, fid, type, attachment_name, path, deleted_by, deleted_on, is_shared
                        FROM {ntlp_course_resource} WHERE nid = %d)", $resource_nid, $new_lib_project_nid, $resourseRows->nid);

                        $query = db_query("INSERT INTO {ntlp_project_resource}(nid, project_nid,
                        resource_order, section_tid, availability) VALUES(%d, %d, '%d', '%d', 'H')",
                                        $resource_nid, $new_lib_project_nid, $resourseRows->resource_order, $new_section_tid);

                        $resource_attch_result = db_query("SELECT name, resource_nid , pebl_attachment_fid, type,
                        path, order_number FROM dpl_ntlp_resource_attachment
                        WHERE resource_nid = %d", $resourseRows->nid);


                        if ($resource_attch_result->num_rows > 0) {

                            while ($data = db_fetch_object($resource_attch_result)) {
                                db_query("INSERT INTO dpl_ntlp_resource_attachment (name, resource_nid, pebl_attachment_fid, type, path, order_number)
                                VALUES('%s', %d, %d, '%s', '%s', %d)", $data->name, $resource_nid,
                                        $data->pebl_attachment_fid, $data->type, $data->path, $data->order_number);
                            }
                        }
                    } else if ($resourseRows->restype == 'A') { // copy activity
                        copy_activity_into_section($new_lib_project_nid, $resourseRows->nid, $new_section_tid, $resourseRows->resource_order, $new_lib_project_nid);
                    }
                }
            }
            ////////////// copying project resources ends here //////////////
        }
    }
    //copying project section details ends here
    //copying project activities starts here
    /* $result = db_query("SELECT * FROM {ntlp_course_activity} WHERE project_nid=%d", $src_project_nid);
      if ($result->num_rows > 0) {
      while ($activityRows = db_fetch_object($result)) {
      $activity_nid = copy_activity_node($activityRows->nid);
      $dateformat = "%Y-%m-%d %H:%M:%S";
      $ctime = strftime($dateformat, time());

      $attactment_query = db_query("select * from {ntlp_activity_attachment} where activity_nid = %d ", $activityRows->nid);
      while ($attachment_data = db_fetch_object($attactment_query)) {

      db_query("INSERT INTO {ntlp_activity_attachment} (activity_nid, type, path, is_deleted, fid) VALUES(%d, '%s', '%s', %d, %d)", $activity_nid
      , $attachment_data->type, $attachment_data->path, $attachment_data->is_deleted, $attachment_data->fid);
      }

      db_query("INSERT INTO {ntlp_course_activity} (nid, course_nid,project_nid, activity_order, submission_type,
      allow_resubmission,availability_mode, available_from, available_to,
      due_date, is_graded, grading_method,curriculum_unit_tid,
      curriculum_standards, school_term_tid, timecreated, timemodified, created_by, modified_by)
      VALUES(%d,%d,%d,%d,'%s','%s','%s','%s','%s','%s','%s','%s',%d,'%s',%d,'%s','%s',%d,%d)",
      $activity_nid, $new_lib_project_nid, $new_lib_project_nid, $activityRows->activity_order, $activityRows->submission_type,
      $activityRows->allow_resubmission, $activityRows->availability_mode, $activityRows->available_from, $activityRows->available_to,
      $activityRows->due_date, $activityRows->is_graded, $activityRows->grading_method, $activityRows->curriculum_unit_tid,
      $activityRows->curriculum_standards, $activityRows->school_term_tid, $ctime, $ctime, $user - uid, $user - uid);
      }
      }
      //copying project activities ends here
     */

    $outcomes_array = _get_project_outcomes($src_project_nid);

//    watchdog("outcome_array ", print_r($outcomes_array, true));
    if ($outcomes_array != null) {
        $i = 0;
        foreach ($outcomes_array as $Obj) {

            $outcomes .= $Obj->name . ' ,';
        }
    }
    /*     * ** Saving into libarary Project ** */

    $submitted_on = strftime($dateformat, time());


    $archived_on = '0000-00-00 00:00:00';
    $archived_request_on = '0000-00-00 00:00:00';
    $deleted_on = '0000-00-00 00:00:00';
    $archived_by = null;
    $is_exemplary = null;

//    db_query("INSERT INTO {ntlp_library_project} (nid, module_nid, submitted_on, submitted_by, published_on,
//        published_by, archived_on, archived_by, is_exemplary, src_project_nid, outcomes )
//        VALUES (%d, %d, '%s', %d, '%s', %d, '%s', %d, %d, %d, '%s' )", $new_lib_project_nid, $project_lib->nid, $submitted_on,
//            $user->uid, $submitted_on, $user->uid, $archived_on, $archived_by, $is_exemplary, $src_project_nid,
//            $outcomes);



    db_query("INSERT INTO {ntlp_library_project} (nid, module_nid, submitted_on, submitted_by, 
        published_on, published_by, archived_on, archived_by, is_exemplary, src_project_nid, 
        outcomes, teacher_comment_cid, reviewer_comment_cid, archive_request_on, archive_reason,
	archive_notes, is_deleted, deleted_by, deleted_on )
        VALUES (%d, %d, '%s', %d, '%s', %d, '%s', %d, %d, %d, '%s', 0, 0, '%s', '', '', 0, 0, '%s')", 
            $new_lib_project_nid, $project_lib->nid, $submitted_on,
            $user->uid, $submitted_on, $user->uid, $archived_on, $archived_by, $is_exemplary, 
            $src_project_nid, $outcomes, $archived_request_on, $deleted_on);

    drupal_set_message("<div><p>Project: <b>$project_name</b> - created successfully</p>" .
            l('View Project', 'ntlp/library/project/view/' . $lib_project_node->nid) . '</div>');
    return;
}

//Returns the parent node id of the specified child node type
function get_project_library_nid($string) {
    $result = db_query("SELECT nid FROM {ntlp_module} WHERE type = '%s'", $string);
    return db_fetch_object($result);
}

function _get_project_outcomes($src_project_nid) {

    $result = db_query("SELECT oc.*, td.* FROM {ntlp_gb_course_outcome} oc
        INNER JOIN {term_data} td ON td.tid = oc.tid
        WHERE course_nid = %d", $src_project_nid);

    while ($rec = db_fetch_object($result)) {
        $project_outcomes[] = $rec;
    }
    return $project_outcomes;
}

function validate_project_for_library($course_nid, $project_id) {

    $flag = '';
    return checking_project_detail($course_nid, $project_id);
}

function check_project_activities($project_id) {
    $flag = true;
    $count = db_result(db_query("SELECT COUNT(*) FROM {ntlp_course_activity} WHERE project_nid = %d", $project_id));

    if ($count > 0) {
        $flag = false;
    }
    return $flag;
}

function check_additional_project_sections($project_id) {

    $flag = false;
    $result = db_query("SELECT cs.*, td.* FROM {ntlp_course_section} cs
        INNER JOIN {term_data} td ON cs.tid = td.tid
        WHERE course_nid = %d", $project_id);

    while ($row = db_fetch_object($result)) {
        $rows[] = $row;
    }

    if (sizeof($rows) > 1) {

        foreach ($rows as $obj) {

            if ($obj->name != 'Facilitator Materials') { // check other section instead of Facilitator Materials
                if (check_resources_in_project_section($obj->tid)) {
                    $flag = true;
                    break; // if resource found then break out this loop
                } else {
                    $flag = false;
                }
            }
        }
    } else {
        $flag = false;
    }

    return $flag;
}

/* checking first project section with its resources..
 * if exist return true otherwise false
 */

function check_project_first_section($project_id) {
//    Facilitator Materials
    $result = db_query("SELECT td.tid, td.name FROM {term_data} td
        INNER JOIN {ntlp_course_section} cs ON cs.tid = td.tid
        WHERE td.name= '%s' AND cs.course_nid = %d", 'Facilitator Materials', $project_id);

    if ($result) {
        $section_Obj = db_fetch_object($result);
        if (check_resources_in_project_section($section_Obj->tid)) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

function check_resources_in_project_section($section_tid) {

    $count = db_result(db_query("SELECT COUNT(*) FROM {ntlp_project_resource}
        WHERE section_tid =%d ", $section_tid));


    if ($count > 0) {
        return true;
    } else {
        return false;
    }
}

function check_learning_outcome_association($project_id) {
//    ntlp_gb_course_outcome

    $count = db_result(db_query("SELECT COUNT(*) FROM {ntlp_gb_course_outcome} WHERE course_nid =%d", $project_id));

    if ($count > 0) {
        return true;
    } else {
        return false;
    }
}

function _check_project_image($project_node) {

    if (!is_null($project_node->picture_fid) && $project_node->picture_fid != 0) {
        return true;
    } else {
        return false;
    }
}

function _check_project_topic_unit($project_node) {

    if (!is_null($project_node->topic_unit_tid) && $project_node->topic_unit_tid != 0) {
        return true;
    } else {
        return false;
    }
}

function _check_project_summary($project_node) {

    if (!is_null($project_node->body) && !empty($project_node->body)) {
        return true;
    } else {
        return false;
    }
}

function checking_project_detail($course_nid, $project_id) {
    $project_node = load_project_record($project_id);
    $flag = true;
    $message = '';


    if (is_null($project_node->picture_fid) || $project_node->picture_fid == 0) {
        $flag = false;
        $message .= '<li>Project Image</li>';
    }


    if (is_null($project_node->topic_unit_tid) || $project_node->topic_unit_tid == 0) {
        $flag = false;

        $message .= '<li>Topic/Unit</li> ';
    }
    if (check_project_activities($project_id)) {
        $flag = false;
        $message .= '<li>At least one Activity associated</li> ';
    }


    $first_messagepart = 'Before this project can be posted to the Project Library, the following items need to be included:<br> <ul>';
    $message .= '</ul><br> > <a  class="green_link" onclick="redirect_user_to(' . $course_nid . ',' . $project_id . ')"> Make these changes in the Project Settings now.</a>';

    if ($flag) {
        return $flag;
    } else {
        return $first_messagepart . $message;
    }
}

function _get_vocabulary_vid($str) {
    $result = db_query("SELECT * FROM {vocabulary} WHERE name = '%s'", $str);
    return db_fetch_object($result);
}

function get_termids($incoming_terms) {
    watchdog('income_term', $incoming_terms);
    $vocab_Object = _get_vocabulary_vid("NTLP Posts");
    foreach ($incoming_terms as $term) {
        $query = db_query("SELECT tid FROM {term_data} WHERE name='%s'", $term);
        $possible_term = db_fetch_object($query);

        //if the term doesn't already exist, add it to the cloud
        if (!$possible_term->tid) {
            $new_term = array('vid' => $vocab_Object->vid, 'name' => $term);

            if (taxonomy_save_term($new_term)) {
                $new_terms = new stdClass();
                $new_terms->tid = $new_term['tid'];
                $new_terms->vid = $vocab_Object->vid;
                $new_terms->name = $term;
                $new_terms->description = '';
                $new_terms->weight = 0;

                $terms_array[] = $new_terms;
            }
        } else {
            $old_terms = new stdClass();
            $old_terms->tid = $possible_term->tid;
            $old_terms->vid = $vocab_Object->vid;
            $old_terms->name = $term;
            $old_terms->description = '';
            $old_terms->weight = 0;

            $terms_array[] = $old_terms;
        }
    }
    return $terms_array;
}

function get_project_subject_and_keywords($course_nid, $project_nid) {

    $result = db_query("SELECT t.* FROM dpl_term_node r
        INNER JOIN dpl_term_data t ON r.tid = t.tid
        WHERE r.nid IN( %d, %d) AND t.vid IN( %d, %d)", $project_nid, $course_nid, NTLP_KEYWORDS_FOR_PROJECT_LIBRARY, NTLP_VOCABULARY_SUBJECTS);

    if ($result->num_rows > 0) {
        while ($data = db_fetch_object($result)) {
            $records[] = $data;
        }
    }

    return $records;
}

function check_project_keywords($project_nid) {
    $flag = true;
    $result = db_query("SELECT COUNT(tn.tid) as keywords_count FROM dpl_term_node tn
        INNER JOIN dpl_term_data td ON td.tid = tn.tid
        WHERE tn.nid = %d AND td.vid = %d ", $project_nid, NTLP_KEYWORDS_FOR_PROJECT_LIBRARY);

    if ($result->num_rows > 0) {
        $data = db_fetch_object($result);
        if ($data->keywords_count >= 2) {
            $flag = false;
        }
    }

    return $flag;
}

?>