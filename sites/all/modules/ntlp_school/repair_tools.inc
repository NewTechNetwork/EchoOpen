<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'cron_grades');

//Use this function to call repair/upgrade/update scripts needed after a database or business logic change
//Call it using URL: ntlp/admin/upgrade
function ntlp_upgrade_scripts() {
    ntlp_populate_outcomes_for_all_school_years();
}


function add_default_permissions($param_school_nid = 0) {
    $role = array(NTLP_ROLEID_PARENT, NTLP_ROLEID_STUDENT, NTLP_ROLEID_TEACHER, NTLP_ROLEID_SITEADMIN, NTLP_ROLEID_NETWORKSTAFF, NTLP_ROLEID_SCHOOLLEADER);
//    $role = array(NTLP_ROLEID_PARENT);

    $default_parent_perm = array(
        'Change Password',
        'Join Groups at my school',
        'Display Instant Messaging ID field',
        'View Network Events',
        'Display phone number field',
        'Display private website field'
    );

    $default_student_perm = array(
        'Join Groups at my school',
        'Create groups at my school',
        'Join network-wide groups',
        'View Network Events',
        'Display phone number field',
        'Display private website field',
        'Display Instant Messaging ID field'
    );

    $default_teacher_perm = array(
        'Change Password',
        'Create network-wide groups',
        'Share/View profiles across the network',
        'Display Instant Messaging ID field',
        'Create Network Events',
        'Display phone number field',
        'Display private website field',
        'Join Groups at my school',
        'Create groups at my school',
        'Join network-wide groups',
        'View Network Events'
    );

    $default_siteadmin_perm = array(
        'Create network-wide groups',
        'Display phone number field',
        'Display private website field',
        'Display Instant Messaging ID field',
        'Create school wide events',
        'View Network Events',
        'Create Network Events',
        'Join Groups at my school',
        'Create groups at my school',
        'Join network-wide groups',
        'Change Password',
        'Share/View profiles across the network'
    );

    $default_ntnstaff_perm = array(
        'Change Password',
        'Create network-wide groups',
        'Share/View profiles across the network',
        'Display phone number field',
        'Display private website field',
        'Display Instant Messaging ID field',
        'Create Network Events',
        'Join Groups at my school',
        'Create groups at my school',
        'Join network-wide groups',
        'View Network Events'
    );


    $default_manager_perm = array(
        'Change Password',
        'Create network-wide groups',
        'Share/View profiles across the network',
        'Display phone number field',
        'Display private website field',
        'Display Instant Messaging ID field',
        'Create school wide events',
        'View Network Events',
        'Create Network Events',
        'Join Groups at my school',
        'Create groups at my school',
        'Join network-wide groups'
    );


    if ($param_school_nid == 0) {
        $school_query = db_query('select nid from {ntlp_school}');
        while ($data = db_fetch_object($school_query)) {

            $school_ids[] = $data->nid;
        }
    } else {
        $school_ids[] = $param_school_nid;
    }

    foreach ($role as $role_id) {

        if ($role_id == NTLP_ROLEID_PARENT) {
            $default_title = $default_parent_perm;
        } elseif ($role_id == NTLP_ROLEID_STUDENT) {
            $default_title = $default_student_perm;
        } elseif ($role_id == NTLP_ROLEID_TEACHER) {
            $default_title = $default_teacher_perm;
        } elseif ($role_id == NTLP_ROLEID_SITEADMIN) {
            $default_title = $default_siteadmin_perm;
        } elseif ($role_id == NTLP_ROLEID_SCHOOLLEADER) {
            $default_title = $default_manager_perm;
        } elseif ($role_id == NTLP_ROLEID_NETWORKSTAFF) {
            $default_title = $default_ntnstaff_perm;
        }



        $rs = db_query("SELECT rid,perm FROM {permission} where rid = %d", $role_id);
        $record = db_fetch_object($rs);
        $perm = $record->perm;
        $perm_array = explode(',', $perm);



        foreach ($school_ids as $key => $school_id) {

            foreach ($default_title as $title) {

                $perm_title = ' ' . $school_id . ' ' . strtolower($title);
                if (!in_array($perm_title, $perm_array)) {
                    $perm_array[] = ' ' . $school_id . ' ' . strtolower($title);
                }
            }

            $permission = implode(',', $perm_array);
//            $permission = $perm_array;
        }

        db_query("UPDATE {permission} SET perm = '%s' where rid = %d", $permission, $role_id);
    }

//    print_r($perm_arrays);
}


function fix_multiple_rubric_submissions() {
    //Get active courses
    $crs = db_query("select count(*) cnt, rc.activity_nid, rs.submission_nid,
        rs.feedback_for, rs.category_id, rs.score, n.uid feedback_from, ca.course_nid
        FROM dpl_ntlp_activity_rubric_submission rs
        INNER JOIN dpl_ntlp_activity_rubric_category rc ON rs.category_ID = rc.category_id
        INNER JOIN dpl_ntlp_course_activity ca ON ca.nid = rc.activity_nid
        INNER JOIN dpl_ntlp_course c ON c.nid = ca.course_nid
        INNER JOIN dpl_ntlp_activity_rubric_submission_for rsf ON rsf.submission_nid = rs.submission_nid
        INNER JOIN dpl_node n ON n.nid = rs.submission_nid
        /*WHERE ca.nid = 186386459*/
        GROUP BY rs.feedback_for, n.uid, rs.category_id HAVING cnt > 1");
//        WHERE ca.nid = 186386459 AND n.uid = 12772
//        WHERE c.school_nid = 1023

    echo '<br>Count of duplicate records:' . $crs->num_rows . '<br>';

    $skip_sr = "x";
    while ($crec = db_fetch_object($crs)) {
        if ($skip_sr == $crec->submission_nid) {
            continue;
        }
        $skip_sr = $crec->submission_nid;
        //Verify each rubric submission
        $ars = db_query("select rs.submission_nid, count(rs.score) cnt, sum(rs.score) totscore
            FROM dpl_ntlp_activity_rubric_submission rs
            INNER JOIN dpl_ntlp_activity_rubric_category rc ON rs.category_ID = rc.category_id
            INNER JOIN dpl_ntlp_activity_rubric_submission_for rsf ON rsf.submission_nid = rs.submission_nid
            INNER JOIN dpl_node n ON n.nid = rs.submission_nid
            WHERE rc.activity_nid = %d AND rs.feedback_for = %d AND n.uid = %d
            GROUP BY rs.submission_nid
            ORDER BY rs.submission_nid DESC
            ", $crec->activity_nid, $crec->feedback_for, $crec->feedback_from);
        $first_sr = "x";
        $safe_to_delete = "yes";
        while ($arec = db_fetch_object($ars)) {
            if ($first_sr == 'x') {
                $first_sr = $arec->submission_nid;
                $catcount = $arec->cnt;
                continue;
            }
            if ($catcount != $arec->cnt) {
                $safe_to_delete = "no";
                break;
            }
        }
        echo "Update wrong Rubric Submissions of Activity $crec->activity_nid for User $crec->feedback_from Submitted for $crec->feedback_for<br>";

        if ($safe_to_delete == "yes") {
            //Run the loop again to delete wrong/duplicate submissions
            $ars = db_query("select rs.submission_nid, count(rs.score) cnt, sum(rs.score) totscore
                FROM dpl_ntlp_activity_rubric_submission rs
                INNER JOIN dpl_ntlp_activity_rubric_category rc ON rs.category_ID = rc.category_id
                INNER JOIN dpl_ntlp_activity_rubric_submission_for rsf ON rsf.submission_nid = rs.submission_nid
                INNER JOIN dpl_node n ON n.nid = rs.submission_nid
                WHERE rc.activity_nid = %d AND rs.feedback_for = %d AND n.uid = %d
                GROUP BY rs.submission_nid
                ORDER BY rs.submission_nid DESC
                ", $crec->activity_nid, $crec->feedback_for, $crec->feedback_from);
            while ($arec = db_fetch_object($ars)) {
                if ($first_sr != $arec->submission_nid) {
                    echo "Deleting $arec->submission_nid,";
                    db_query("DELETE FROM dpl_ntlp_activity_rubric_submission WHERE submission_nid = %d", $arec->submission_nid);
                    db_query("DELETE FROM dpl_ntlp_activity_rubric_submission_for WHERE submission_nid = %d", $arec->submission_nid);
                    db_query("DELETE FROM dpl_node WHERE nid = %d", $arec->submission_nid);
                    db_query("DELETE FROM dpl_node_revisions WHERE nid = %d", $arec->submission_nid);
                    //TRIGGER_POINT: UPDATE STUDENT RECORD FOR CURRENT COURSE TERMS
                    dw_set_allterms_course_grade_toupdate($crec->course_nid);
                }
            }
            echo "<BR>";
        } else {
            echo "NOT SAFE TO DELETE.........skipping<BR>";
        }
    }
}

function add_user_info_form($form_state) {

    include('school_management.template.inc');

    $form['main'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => $HTMLform_school_setting_bluebox_start,
        '#suffix' => $HTMLform_school_setting_bluebox_end,
    );


    $form['main']['submitbtn'] = array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('add_user_info_submit'),
//                '#validate' =>  array(''),
        '#attributes' => array('class' => 'SaveBtn'),
        '#prefix' => '<tr><td colspan="2">',
        '#suffix' => '</td></tr>',
    );

    $form['main']['school_id'] = array(
        '#type' => 'hidden',
        '#value' => $school_nid,
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div>',
    );

    $aschools = get_schools();
    while ($rec = db_fetch_object($aschools)) {
        $soptions[$rec->nid] = (strlen($rec->school_name) >= 40 ? substr($rec->school_name, 0, 40) . '...' : $rec->school_name);
    }


    $form['main']['school_names'] = array(
        '#type' => 'select',
        '#options' => $soptions,
//            '#default_value' => $duedate_minute,
        '#prefix' => '<tr><th> School Selector</th><td style="border:none"><span style="width:240px;" class="comboboxBgLeft">',
        '#suffix' => '</span>&nbsp;&nbsp;</td></tr>',
        '#attributes' => array('class' => 'my-dropdown'),
    );

    $school_role = array(NTLP_ROLEID_STUDENT => 'Student', NTLP_ROLEID_TEACHER => 'Teacher');
    $form['main']['student_roll'] = array(
        '#type' => 'select',
        '#options' => $school_role,
//            '#default_value' => $duedate_minute,
        '#prefix' => '<tr><th> User Role</th><td style="border:none"><span style="width:240px;" class="comboboxBgLeft">',
        '#suffix' => '</span>&nbsp;&nbsp;</td></tr>',
        '#attributes' => array('class' => 'my-dropdown'),
    );

    $form['main']['emails'] = array(
        '#type' => 'textarea',
        '#wysiwyg' => FALSE,
        '#prefix' => '<tr><th>' . t('Student Emails') . '</th><td >',
        '#suffix' => '</td></tr>',
        '#rows' => 2,
//                    '#default_value' =>(isset($activityObj)) ? $activityObj->body :'',
    );

    return $form;
}

function add_user_info_submit($form, $form_state) {

    $user_role = $form_state['clicked_button']['#post']['main']['student_roll'];
    $school_id = $form_state['clicked_button']['#post']['main']['school_names'];
    $user_emails = strip_tags($form_state['clicked_button']['#post']['main']['emails']);

    $email = explode(',', $user_emails);

    if (!empty($email)) {

        foreach ($email as $email_id) {

            // Fetch userId from dpl_user table
            $email_id = trim($email_id);
            $user_id = get_parent_child_uid($email_id);

            if ($user_id) {
                // check if user role is insert if not then it will insert
                $check_user_role = check_user_role($user_id);

                if (!$check_user_role) {
                    db_query("INSERT INTO {users_roles} (uid,rid) VALUES (%d, %d)", $user_id, $user_role);
                }

                // check user existance in dpl_ntlp_user table
                $check_user = check_user_existance($user_id);

                if (!$check_user) {

                    #it will get first name and last name
                    $user_name_full = explode('@', $email_id);
                    $user_name = explode('.', $user_name_full[0]);

                    $first_name = ucfirst(strtolower($user_name[0]));
                    $last_name = ucfirst(strtolower($user_name[1]));

                    db_query("
                        INSERT INTO dpl_ntlp_user
                                    (uid, school_nid,
                                     first_name, middle_name, last_name)
                        VALUES (%d, %d, '%s', '%s', '%s')", $user_id, $school_id, $first_name, '', $last_name);

//                    watchdog('fixuser', 'Email: '.$email_id.' Id'.$user_id.' Firstname:'.$first_name.' Lastname:'.$last_name. ' School id:'.$school_id);
                } else {
                    //Updating school nid with the select value
                    db_query("
                        UPDATE dpl_ntlp_user SET school_nid = %d
                        WHERE uid = %d", $school_id, $user_id);

                    //Updating DATA field will reset/clear LDAP parameters, fixing a login problem
                    db_query("
                        UPDATE dpl_users SET data=NULL
                        WHERE uid = %d", $user_id);
                }
                drupal_set_message('User Created Successfully: ' . $email_id);
            } else {
                drupal_set_message('User does not exist:' . $email_id);
            }
        }
    }
}

function ntlp_recover_course_form($form_state = null) {
    include('school_management.template.inc');

    $form = array();

    ahah_helper_register($form, $form_state);

    $form['main'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div id="example6-wrapper">' . $HTMLform_school_setting_bluebox_start, // This is our wrapper div
        '#suffix' => $HTMLform_school_setting_bluebox_end . '</div>',
        '#tree' => TRUE, // don't forget to set #tree!
    );

    $aschools = get_schools();
    $flag = true;
    while ($rec = db_fetch_object($aschools)) {

        if ($flag) {
            $choice = $rec->nid;
            $options[$rec->nid] = (strlen($rec->school_name) >= 40 ? substr($rec->school_name, 0, 40) . '...' : $rec->school_name);
            $flag = false;
        } else {
            $options[$rec->nid] = (strlen($rec->school_name) >= 40 ? substr($rec->school_name, 0, 40) . '...' : $rec->school_name);
        }
    }

    if (isset($form_state['values']['main']['school_nid'])) {
        $choice = $form_state['values']['main']['school_nid'];
    }
    $form['main']['submitbtn'] = array(
        '#type' => 'submit',
        '#value' => t('Recover This Course'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('ntlp_recover_course_submit'),
        '#attributes' => array('class' => 'SubmitAssignmentBtn'),
        '#prefix' => '<tr><td colspan="2">',
        '#suffix' => '</td></tr>',
    );


    $form['main']['school_nid'] = array(
//        '#title' => t('Choices'),
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => $choice,
        '#required' => TRUE,
        '#multiple' => FALSE,
        '#ahah' => array(
            'event' => 'change',
            'path' => ahah_helper_path(array('main')),
            'wrapper' => 'example6-wrapper',
            'method' => 'replace',
        ),
//        '#attributes' => array('class' => 'my-dropdown'),
        '#prefix' => '<tr><th>' . t('School Name') . '</th><td style="border:none"><span style="width:240px;" class="comboboxBgLeft">',
        '#suffix' => '</span></td></tr>',
    );

    watchdog('choice', $choice);

    $options2 = get_deleted_course($choice);

    $form['main']['course_nid'] = array(
        '#type' => 'select',
        '#options' => $options2,
        '#default_value' => $choice2,
        '#multiple' => FALSE,
//        '#attributes' => array('class' => 'my-dropdown'),
        '#prefix' => '<tr><th>' . t('Select Course') . '</th><td style="border:none"><span style="width:240px;" class="comboboxBgLeft">',
        '#suffix' => '</span></td></tr>',
//        '#ahah' => array(
//            'event' => 'change',
//            'path' => ahah_helper_path(array('main')),
//            'wrapper' => 'example6-wrapper',
//            'method' => 'replace',
//        ),
    );


    return $form;
}

function get_deleted_course($school_nid = 0) {

    $data = '';
    watchdog('delete_course 1', $school_nid);

    $result = db_query("SELECT n.title, c.nid, c.school_nid
        FROM dpl_ntlp_course_history c
        LEFT JOIN dpl_node n ON n.nid = c.nid
        WHERE c.school_nid = %d AND c.is_project = 0 AND n.type = 'ntlp_course'", $school_nid);

//    $result = db_query("SELECT n.title c.nid, c.school_nid
//        FROM dpl_ntlp_course_history c
//        LEFT JOIN dpl_node n ON n.nid = c.nid
//        WHERE c.school_nid = %d AND c.is_project = 0 AND n.type = 'ntlp_course' ", $school_nid);

    if ($result->num_rows > 0) {

        while ($record = db_fetch_object($result)) {
            $data[$record->nid] = $record->title;
        }
    } else {
        $data[0] = 'No Course Found';
    }
    watchdog('delete_course 2', print_r($data, true));
    return $data;
}

function ntlp_recover_course_submit($form, $form_state) {

//    print_r($form_state['clicked_button']['#post']['main']);

    $school_nid = $form_state['clicked_button']['#post']['main']['school_nid'];
    $course_nid = $form_state['clicked_button']['#post']['main']['course_nid'];


    db_query("INSERT INTO {ntlp_course} (SELECT nid,school_nid, NULL, is_project, course_nid, teacher1_uid,
        teacher2_uid, is_weighted_grades, period, additional_notes, is_advisory_course, course_short_title,
        availability, from_date, to_date, driving_question, academic_standards, school_year, picture_fid, 
        topic_unit_tid, allow_self_enroll
        FROM {ntlp_course_history} WHERE nid= %d )", $course_nid);
    db_query("DELETE FROM {ntlp_course_history} WHERE nid= %d", $course_nid);


    /*
     * activity
     */
    db_query("INSERT INTO {ntlp_course_activity} (SELECT nid, course_nid, activity_order, submission_type,
        allow_resubmission, availability_mode, available_from, available_to, due_date, is_graded,
        grading_method, curriculum_unit_tid, curriculum_standards, school_term_tid, timecreated, timemodified,
        created_by, modified_by, project_nid, 0, project_cat, allow_late_submission, published_date
        FROM {ntlp_course_activity_history} WHERE course_nid =  $course_nid )");
    db_query("DELETE FROM {ntlp_course_activity_history} WHERE course_nid= %d", $course_nid);

    /*
     * resource
     */
//    db_query("SELECT deleted_on, deleted_by FROM {ntlp_course_resource} WHERE course_nid= %d", $course_nid);
    db_query("update {ntlp_course_resource} set deleted_on=null, deleted_by=null WHERE course_nid= %d", $course_nid);

    /*
     * projects
     */
//    db_query("SELECT * FROM {ntlp_course_activity_history} WHERE course_nid= %d", $course_nid);
//    db_query("SELECT * FROM {ntlp_course_activity_history} WHERE project_nid in(51416, 1791443, 1791455, 1828447, 170116335);");

    db_query("INSERT INTO {ntlp_course} (SELECT nid, school_nid, NULL, is_project, course_nid,
        teacher1_uid, teacher2_uid, is_weighted_grades, period, additional_notes, is_advisory_course,
        course_short_title, availability, from_date, to_date, driving_question, academic_standards,
        school_year, picture_fid, topic_unit_tid, allow_self_enroll
        FROM {ntlp_course_history} WHERE course_nid =  $course_nid )");

    db_query("DELETE FROM {ntlp_course_history} WHERE course_nid= %d", $course_nid);

    db_query("UPDATE {node} set status=1 WHERE nid= %d", $course_nid);

    dw_set_allterms_course_grade_toupdate($course_nid);

    drupal_set_message('Course recovered, click <a href="' . url('ntlp/courses/home/' . $course_nid) . '">here</a> to open the course');
}

//-----------------------------------------------------------------------
// Change Activity Terms

function ntlp_change_activity_terms($form_state = null) {
    include('school_management.template.inc');

    $form = array();

    ahah_helper_register($form, $form_state);

    $form['main'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div id="course-wrapper">' . $HTMLform_school_setting_bluebox_start, // This is our wrapper div
        '#suffix' => $HTMLform_school_setting_bluebox_end . '</div>',
        '#tree' => TRUE, // don't forget to set #tree!
    );

    $aschools = get_schools();
    $flag = true;
    while ($rec = db_fetch_object($aschools)) {

        if ($flag) {
            $choice = $rec->nid;
            $options[$rec->nid] = (strlen($rec->school_name) >= 40 ? substr($rec->school_name, 0, 40) . '...' : $rec->school_name);
            $flag = false;
        } else {
            $options[$rec->nid] = (strlen($rec->school_name) >= 40 ? substr($rec->school_name, 0, 40) . '...' : $rec->school_name);
        }
    }

    if (isset($form_state['values']['main']['school_nid'])) {
        $choice = $form_state['values']['main']['school_nid'];
        $course_choice = $form_state['values']['main']['course_nid'];
    }
    $form['main']['submitbtn'] = array(
        '#type' => 'submit',
        '#value' => t('Change Term'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('ntlp_change_activity_terms_submit'),
        '#attributes' => array('class' => 'SubmitAssignmentBtn'),
        '#prefix' => '<tr><td colspan="2">',
        '#suffix' => '</td></tr>',
    );


    $form['main']['school_nid'] = array(
//        '#title' => t('Choices'),
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => $choice,
        '#required' => TRUE,
        '#multiple' => FALSE,
        '#ahah' => array(
            'event' => 'change',
            'path' => ahah_helper_path(array('main')),
            'wrapper' => 'course-wrapper',
            'method' => 'replace',
        ),
//        '#attributes' => array('class' => 'my-dropdown'),
        '#prefix' => '<tr><th>' . t('School Name') . '</th><td style="border:none"><span style="width:240px;" class="comboboxBgLeft">',
        '#suffix' => '</span></td></tr>',
    );

    watchdog('choice', 'Selected school: ' . $choice);
    $result = get_myschool_allterms_courses($choice);
    if ($result->num_rows > 0) {
        $options2[0] = 'Select Course';
        while ($record = db_fetch_object($result)) {
            $options2[$record->nid] = $record->title;
        }
    } else {
        $options2[0] = 'No Course Found';
    }

    $form['main']['course_nid'] = array(
        '#type' => 'select',
        '#options' => $options2,
        '#default_value' => $course_choice,
        '#multiple' => FALSE,
//        '#attributes' => array('class' => 'my-dropdown'),
        '#prefix' => '<tr><th>' . t('Select Course') . '</th><td style="border:none"><span style="width:240px;" class="comboboxBgLeft">',
        '#suffix' => '</span></td></tr>',
        '#ahah' => array(
            'event' => 'change',
            'path' => ahah_helper_path(array('main')),
            'wrapper' => 'course-wrapper',
            'method' => 'replace',
        ),
    );

//    $form['main']['terms'] = array(
//        '#type' => 'item',
//        '#prefix' => '<div id="term-wrapper">',
//        '#suffix' => '</div>',
//        '#tree' => TRUE, // don't forget to set #tree!
//    );

    if (!empty($course_choice) && intval($course_choice) > 0) {
        watchdog('choice', 'Selected course: ' . $course_choice);

        $course_term = get_course_terms($course_choice);
        while ($rs = db_fetch_array($course_term)) {
            $term_name[$rs['tid']] = $rs['name'];
        }
        $form['main']['term_from'] = array(
            '#prefix' => "<tr><th>" . t('Term From') . "</th><td><span class='comboboxBgLeft' style='width:200px;'>",
            '#suffix' => '</span></td></tr>',
            '#type' => 'select',
            '#options' => $term_name,
//                '#attributes' => array('class' => 'my-dropdown'),
        );
        $form['main']['term_to'] = array(
            '#prefix' => "<tr><th>" . t('Term To') . "</th><td><span class='comboboxBgLeft' style='width:200px;'>",
            '#suffix' => '</span></td></tr>',
            '#type' => 'select',
            '#options' => $term_name,
//                '#attributes' => array('class' => 'my-dropdown'),
        );
    }

    return $form;
}

function ntlp_change_activity_terms_submit($form, $form_state) {

//    print_r($form_state['clicked_button']['#post']['main']);

    $school_nid = $form_state['clicked_button']['#post']['main']['school_nid'];
    $course_nid = $form_state['clicked_button']['#post']['main']['course_nid'];
    $termfrom_tid = $form_state['clicked_button']['#post']['main']['term_from'];
    $termto_tid = $form_state['clicked_button']['#post']['main']['term_to'];

    db_query("UPDATE {ntlp_course_activity} SET school_term_tid = %d
        WHERE school_term_tid = %d AND course_nid = %d", $termto_tid, $termfrom_tid, $course_nid);

    drupal_set_message('Course activity terms changed.');
}

function repair_tvhs_user_form() {

    include('school_management.template.inc');

    $form['main'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => $HTMLform_school_setting_bluebox_start,
        '#suffix' => $HTMLform_school_setting_bluebox_end,
    );


    $form['main']['submitbtn'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('repair_TVHS_user_data'),
//                '#validate' =>  array(''),
        '#attributes' => array('class' => 'SaveBtn'),
        '#prefix' => '<tr><td>Repair TVHS User Form</td><td> ',
        '#suffix' => '</td></tr>',
    );

    $form['main']['old_user_id'] = array(
        '#type' => 'textfield',
        '#prefix' => '<tr><th> Old User ID : </th><td style="border:none">',
        '#suffix' => '&nbsp;&nbsp;</td></tr>',
    );

    $form['main']['new_user_id'] = array(
        '#type' => 'textfield',
        '#prefix' => '<tr><th> New User ID : </th><td style="border:none">',
        '#suffix' => '&nbsp;&nbsp;</td></tr>',
    );

    return $form;
}


function change_school_timezone_form() {

    include('school_management.template.inc');

    $form['main'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => $HTMLform_school_setting_bluebox_start,
        '#suffix' => $HTMLform_school_setting_bluebox_end,
    );

    $schools_result = get_schools();
    if ($schools_result->num_rows > 0) {
        while ($data = db_fetch_object($schools_result)) {
            $schools_options[$data->nid] = $data->school_name;
        }
    }
    $form['main']['ntlp_schools'] = array(
        '#type' => 'select',
        '#options' => $schools_options,
        '#default_value' => '',
        '#prefix' => '<tr><th>NTLP Schools</th><td colspan="3"><span style="width:350px;" class="comboboxBgLeft">',
        '#suffix' => '</span></td></tr>',
        '#attributes' => array('class' => 'my-dropdown'),
    );

    $school_timezone_arr = event_zonelist();
    $form['main']['school_timezone'] = array(
        '#type' => 'select',
        '#options' => $school_timezone_arr,
        '#default_value' => (isset($school_rs)) ? $school_rs->local_timezone : '',
        '#prefix' => '<tr><th>School Time Zone</th><td colspan="3"><span style="width:250px;" class="comboboxBgLeft">',
        '#suffix' => '</span></td></tr>',
        '#attributes' => array('class' => 'my-dropdown'),
    );

    $set_daylight = variable_get('date_daylight', 0);
    $set_daylight = (($set_daylight) ? 0 : 1 );

    $form['main']['timezone'] = array(
        '#type' => 'item',
        '#value' => l((($set_daylight == 1)? 'Disable' : 'Enable'), 'ntlp/school/change/daylight/' . $set_daylight), //
        '#executes_submit_callback' => false,
//        '#submit' => array('change_school_timezone_datxa'),
        '#attributes' => array('class' => 'SaveBtn'),
        '#prefix' => '<tr><th>Time Zone</th><td colspan="3">',
        '#suffix' => '</td></tr>',
    );



    $form['main']['submitbtn'] = array(
        '#type' => 'submit',
        '#value' => t('Submit'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('change_school_timezone_data'),
        '#attributes' => array('class' => 'SaveBtn'),
        '#prefix' => '<tr><td></td><td> ',
        '#suffix' => '</td></tr>',
    );

    return $form;
}

function change_school_timezone_data($form, $form_state) {
//    print_r($form_state['clicked_button']['#post']['main']);
    $result = set_tz_school($form_state['clicked_button']['#post']['main']['school_timezone'], $form_state['clicked_button']['#post']['main']['ntlp_schools']);
    cache_set('ntlp_timezone', NULL);
    if ($result)
        drupal_set_message("Timezone updated successfully");
    else
        drupal_set_message("Timezone is not updated. Please contact admin");
}

function change_daylight($daylight) {
    variable_set('date_daylight', $daylight);
    cache_set('ntlp_timezone', NULL);
    drupal_goto('ntlp/school/change/timezone');
    return;
}

function repair_project_section_resources_order() {
//    $debugMode = isset($_GET['debug']);
    $debugMode = false;
    $successCount = 0;
    $totalCount = 0;
    $i = 0;

//    $sql = "SELECT
//                    DISTINCT project_nid, section_tid,
//                    (
//                            SELECT CONCAT(MIN(npr1.resource_order),'|',MAX(npr1.resource_order),'|',COUNT(npr1.resource_order)) FROM dpl_ntlp_project_resource npr1 WHERE npr1.project_nid = npr.project_nid AND npr1.section_tid = npr.section_tid
//                    ) min_max_count
//            FROM
//                    dpl_ntlp_project_resource npr
//            WHERE
//                    npr.project_nid > 0 AND npr.project_nid < 50000
//                    AND npr.section_tid > 0
//            ORDER BY
//                    npr.project_nid";
    $sql = "SELECT npr.project_nid, npr.section_tid,
        CONCAT(MIN(resource_order),'|',MAX(resource_order),'|',COUNT(resource_order)) min_max_count
            FROM
                    dpl_ntlp_project_resource npr
            INNER JOIN dpl_ntlp_library_project p ON p.nid = npr.project_nid
            WHERE
                    npr.project_nid > 0
                    AND npr.section_tid > 0
            GROUP BY
                    npr.project_nid, section_tid
            ORDER BY
                    npr.project_nid";

    $result = db_query($sql);


    $debug = '<div><h5>Reordering resources - Running in mode:' . $debugMode . '</h5></div><table border="1" style="border-collapse: collapse;" cellpadding="3">
                <tr>
                    <th>Project ID</th>
                    <th>Section ID</th>
                    <th>Min Order</th>
                    <th>Max Order</th>
                    <th>Total Resources</th>
                    <th>Update Queries</th>
                </td>';
    if ($result->num_rows) {
        while ($row = db_fetch_object($result)) {
//            print_r($row);
//            echo '<br><bR>';

            list( $min, $max, $total_resources ) = explode('|', $row->min_max_count);

            if ($total_resources > 0) {
                $bgColor = $i % 2 ? '#FAF4E7' : '#E4F8FB';

                $debug .= "<tr style=\"background: $bgColor;\">";
                $debug .= " <td style=\"text-align: center;\">$row->project_nid</td>
                            <td style=\"text-align: center;\">$row->section_tid</td>
                            <td style=\"text-align: center;\">$min</td>
                            <td style=\"text-align: center;\">$max</td>
                            <td style=\"text-align: center;\">$total_resources</td>";


                $sql = "SELECT npr.nid FROM dpl_ntlp_project_resource npr
                    INNER JOIN dpl_node n ON n.nid = npr.nid
                    WHERE npr.project_nid = %d AND npr.section_tid = %d
                    ORDER BY n.title ASC ";

                $result2 = db_query($sql, $row->project_nid, $row->section_tid);

                if ($result2->num_rows) {

                    $order = 1;
                    $debug .= '<td style="white-space: nowrap;">';
                    while ($resource = db_fetch_object($result2)) {
                        $max++;
                        $debugUpdate = "UPDATE dpl_ntlp_project_resource SET
                                        <span style=\"color: #A20031;font-weight: bold;\">resource_order = $order</span> WHERE
                                        <span style=\"color: #A20031;font-weight: bold;\">nid = $resource->nid</span> AND
                                        <span style=\"color: #A20031;font-weight: bold;\">section_tid = $row->section_tid</span> AND
                                        <span style=\"color: #A20031;font-weight: bold;\">project_nid = $row->project_nid</span>
                                ";

                        if (!$debugMode) {
                            $updateResult = db_query('UPDATE dpl_ntlp_project_resource
                                SET resource_order = %d WHERE nid = %d
                                AND section_tid = %d AND project_nid = %d', $order, $resource->nid, $row->section_tid, $row->project_nid);
                        }

                        if ($debugMode or db_affected_rows()) {
                            $debug .= "<div>$debugUpdate</div>";
                            if (!$debugMode)
                                $successCount++;
                        } else {
                            $debug .= "<div style=\"background: #FCECEF;\">$debugUpdate</div>";
                        }
                        $totalCount++;
                        $order++;
                    }
                    $debug .= '</td>';
                }
                $debug .= '</tr>';
                $i++;
            }
        }
    }
    $debug .= '</table>';

    $debug .= "<br/>Total Quries: $totalCount<br/>Successful Quries: $successCount<br/>Failed Quries: " . ($totalCount - $successCount) . "<br/> ";
    echo $debug;
}

function ntlp_repair_school_year() {
    $result = db_query("SELECT n.nid, n.title FROM {node} n
                        INNER JOIN {ntlp_school} s ON s.nid=n.nid");

    echo 'Total School ' . $result->num_rows . ' </br>';

    while ($data = db_fetch_object($result)) {

        $courses_year_query = 'SELECT DISTINCT school_year FROM dpl_ntlp_course
            WHERE school_nid = %d AND school_year > 0';

        $school_courses_year_result = db_query($courses_year_query, $data->nid);

        echo $data->nid . ' School ' . $data->title . ' : Total School Year = ' . $school_courses_year_result->num_rows . '<br/>';
        while ($record = db_fetch_object($school_courses_year_result)) {


            $school_year_result = db_query("SELECT DISTINCT school_year FROM dpl_ntlp_school_term
                WHERE school_nid = %d AND school_year = '%s'", $data->nid, $record->school_year);

            if ($school_year_result->num_rows == 0) {
                echo ' school nid ' . $data->nid . ' year ' . $record->school_year . '';
                echo ' Not Found <br/>';
            }
        }
    }
}

function merge_subjects() {
    echo "Start merging subjects<br>";
    $subjects = array(
        1043 => 20044,
        1024 => 20025,
        51 => 20018,
        1010 => 20011,
        1000 => 20001,
        1004 => 20005,
        1022 => 20023,
        1018 => 20019,
        1041 => 20042,
        1013 => 20014,
        1006 => 20007,
        53 => 20010,
        20043 => 20042,
        20049 => 20048,
    );

    db_query("UPDATE {term_data} SET name = '%s' WHERE tid = %d", 'US History', 20010);
    db_query("UPDATE {term_data} SET name = '%s' WHERE tid = %d", 'English/SS (Integrated)', 348882);

    foreach ($subjects as $oldval => $newval) {
        echo "Checking subject:" . $oldval . "<br>";
        //Remove duplicates first
        $rs = db_query("SELECT count(*) cnt, nid FROM {term_node} WHERE tid IN (%d, %d) GROUP BY nid HAVING cnt > 1", $oldval, $newval);
        while ($rec = db_fetch_object($rs)) {
            db_query("DELETE FROM {term_node} WHERE tid = %d AND nid = %d", $oldval, $rec->nid);
            echo "DELETE FROM {term_node} WHERE tid = $oldval AND nid = $rec->nid";
            echo "<br>";
        }
        db_query("UPDATE {term_node} SET tid = %d WHERE tid = %d", $newval, $oldval);
        echo "UPDATE {term_node} SET tid = $newval WHERE tid = $oldval";

        $rs = db_query("SELECT * FROM {term_hierarchy} WHERE parent = %d", $oldval);
        if ($rs->num_rows == 0) {
            //Delete the Subject
            db_query("DELETE FROM {term_data} WHERE tid = %d", $oldval);
        }
    }
}

function get_active_activity_terms() {
    $rs = db_query("SELECT ca.school_term_tid
            FROM {ntlp_course_term} ct
            INNER JOIN {ntlp_course_activity} ca ON ct.school_term_tid = ca.school_term_tid
            INNER JOIN {node} n ON n.nid = ca.nid
            WHERE ca.grading_method = 'C' AND NOW() >= ca.due_date 
            AND n.changed > UNIX_TIMESTAMP('2011-2-1') AND ca.due_date > '2011-2-1'");
    return $rs;
}

function fix_auto_graded_activity_submission() {

    $i = 0;
    $ungrade_submission_result = db_query("SELECT ca.nid AS activity_nid, ca.created_by, au.user_uid,
        s.date_opened, s.date_submitted, s.is_final, s.note_to_teacher, s.version_num,
        g.is_complete, g.is_late, (CASE WHEN g.id IS NULL THEN 0 ELSE 1 END) is_graded, g.date_graded, g.teachers_notes
        FROM {ntlp_course_activity} ca
        INNER JOIN {ntlp_activity_user} au ON au.activity_nid = ca.nid
        LEFT JOIN {ntlp_activity_submission} s ON ca.nid = s.activity_nid AND au.user_uid = s.user_uid AND s.is_final = 1
        LEFT JOIN {ntlp_gb_grade} g ON ca.nid = g.activity_nid AND au.user_uid = g.user_uid
        INNER JOIN {node} n ON n.nid = ca.nid
        WHERE ca.grading_method = 'C' AND NOW() >= ca.due_date
        AND s.date_submitted IS NULL AND g.id IS NULL AND n.changed > UNIX_TIMESTAMP('2011-2-1') AND ca.due_date > '2011-2-1' ORDER BY ca.nid");

    while ($ungraded_submission = db_fetch_object($ungrade_submission_result)) {

        //If no submission found and not graded then mark as incomplete
        if (empty($ungraded_submission->date_submitted) && empty($ungraded_submission->date_graded)) {

            $grade_id = save_activity_user_grade($ungraded_submission->activity_nid, $ungraded_submission->user_uid, '', 
                $ungraded_submission->created_by, 0, 1);

            //Now insert all the activity outcomes with Zero scores
            $activityoutcome = get_activity_outcome($ungraded_submission->activity_nid);
            foreach ($activityoutcome as $outcome) {
                $outcomes[$outcome_obj->tid] = 0;
            }
            save_activity_user_grade_scores($grade_id, $outcomes);
//          watchdog('autograde_cron','activity_nid '.$ungraded_submission->activity_nid.' user sumbission nid '.$ungraded_submission->nid.' student_uid '.$ungraded_submission->user_uid);
            $i++;
        }
    }
    print '<br>Number of submissions marked incomplete:' . $i . '<br>';
}

function fix_incomplete_grades() {

    $ungrade_submission_result = db_query("select g.* from dpl_ntlp_gb_grade g, dpl_ntlp_course_activity a
        where g.activity_nid = a.nid and g.is_complete = 0
        and g.id not in (select grade_id id from dpl_ntlp_gb_activity_grade)
        and g.activity_nid in (select activity_nid from dpl_ntlp_gb_activity_outcome)
        and g.date_graded > '2011-2-2'");

    $i = 0;
    while ($ungraded_submission = db_fetch_object($ungrade_submission_result)) {

        if ($ungraded_submission->is_complete != 1) {
            $i++;

            //Now insert all the activity outcomes with Zero scores
            echo '<br>Processing Activity:' . $ungraded_submission->activity_nid;

            //Now insert all the activity outcomes with Zero scores
            $activityoutcome = get_activity_outcome($ungraded_submission->activity_nid);
            foreach ($activityoutcome as $outcome) {
                $outcomes[$outcome_obj->tid] = 0;
            }
            save_activity_user_grade_scores($ungraded_submission->id, $outcomes);
//            watchdog('autograde_cron','activity_nid '.$ungraded_submission->activity_nid.' user sumbission nid '.$ungraded_submission->nid.' student_uid '.$ungraded_submission->user_uid);
        }
    }
    print '<br>Number of Incomplete Submissions have been reassigned zero grades:' . $i . '<br>';
}


//Problem detected when we removed OG integration
//Course Primary instructor(s) are saved in teacher1_uid, teacher2_uid
//but their enrollment is missing from ntlp_course_user
function fix_course_creator_enrollment() {
    $t = 0;
    for ($t = 1; $t <= 2; $t++) {
        print '<br>Checking enrollments of Teacher:' . $t . '<br>';
        $crs = db_query("select c.* from dpl_ntlp_course c, dpl_node n
            where c.nid = n.nid and n.type = 'ntlp_course'
            and is_project <> 1 and teacher%d_uid <> 0 and teacher%d_uid not in
            (select user_uid from dpl_ntlp_course_user where course_nid = c.nid and role_id = 6)", $t, $t);
        print 'Found courses:' . $crs->num_rows . '<br>';

        $i = 0;
        while ($crec = db_fetch_array($crs)) {

            if (intval($crec['teacher' . $t . '_uid']) > 0) {
                $i++;

                //Now insert user enrollment
                echo '<br>Processing Course:' . $crec['nid'];
                db_query("INSERT INTO {ntlp_course_user} VALUES (%d, %d, '%s', %d, %d, null)", $crec['nid'], $crec['teacher' . $t . '_uid'], '2011-3-24', 1, 6);
            }
        }
    }
}

//Project Briefcase required activities to be included in Project Sections
//earlier it was not mapped with a Project Section
//To fix this, we need to create a new section and map all the activities with this section
function assign_unmapped_project_activities() {
    print '<p><strong>Checking Projects with Unmapped Activities</strong></p>';
    $prs = db_query("select c.nid from dpl_ntlp_course c
        inner join dpl_ntlp_course_activity a on c.nid = a.project_nid
        left join dpl_ntlp_course_section ps on a.project_cat = ps.tid and c.nid = ps.course_nid
        where c.is_project = 1 and ps.tid is null /* and c.nid = 84 */
        group by c.nid", $t, $t);
    print 'Found projects:' . $prs->num_rows . '<br>';
    $obj = get_vocabulary_vid('NTLP Course Section');
    $vid = $obj->vid;

    while ($prec = db_fetch_object($prs)) {
        db_query("INSERT INTO {term_data} (vid, name, description, weight) VALUES (%d, '%s', '%s', 0)", $vid, "Activities", "Default Project Section for Activities");
        $tid = db_last_insert_id('{term_data}', 'tid');

        $result = db_query("SELECT max(section_order) maxorder FROM {ntlp_course_section} WHERE course_nid = %d", $prec->nid);
        $rec = db_fetch_object($result);
        $maxorder = $rec->maxorder + 1;

        db_query("INSERT INTO {ntlp_course_section} (tid, course_nid, section_order, availability)
            VALUES (%d, %d, %d, '%s')", $tid, $prec->nid, $maxorder, 'S');

        $result = db_query("UPDATE {ntlp_course_activity} SET project_cat = %d
            WHERE project_nid = %d AND (project_cat IS NULL OR project_cat = 0)", $tid, $prec->nid);
    }
}

//Modify how resources are associated with Share Stuff and Project Briefcase
//in March 2011 project briefcase upgrade release.
function repair_resources_for_march2011() {
    //The shared resources in Share Stuff were delinked from project/course resources
    //in March 2011 project briefcase upgrade release.
    //Only those resources should be left visible in Share Stuff which are not associated with any project
    //and are marked as 'Shared'
    print '<p><strong>Hide Project resources from Share Stuff</strong></p>';
    db_query("UPDATE dpl_ntlp_course_resource SET is_shared = 0 WHERE nid IN
            (SELECT nid FROM dpl_ntlp_project_resource)");
    $i = db_affected_rows();
    print 'Resource delinked from Share Stuff: ' . $i . '<br>';
//Result from Master Database: 24/Apr/2011
//Query OK, 33568 rows affected (3.74 sec)
//Rows matched: 437553  Changed: 33568  Warnings: 0
    //Make a copy of each resource in dpl_ntlp_course_resource and node/node_revisions for each project resource/node
    //Fetch those resources which are associated in more than one projects
    $prs = db_query("select cr.nid, count(*) cnt from dpl_ntlp_course_resource cr, dpl_ntlp_project_resource pr
        WHERE cr.nid = pr.nid AND cr.deleted_on IS NOT NULL
        GROUP BY cr.nid HAVING cnt > 1
        ");
    print 'Found resources that are used in multiple projects/sections:' . $prs->num_rows . '<br>';

    while ($prec = db_fetch_object($prs)) {
        //Loop through all associated projects
        $rrs = db_query("select n.title, n.type nodetype, n.created, n.status, n.uid, nr.body, cr.*, pr.*
            FROM dpl_ntlp_course_resource cr, dpl_ntlp_project_resource pr, dpl_node n, dpl_node_revisions nr
            WHERE cr.nid = pr.nid AND cr.nid = n.nid AND n.nid = nr.nid AND cr.nid = %d", $prec->nid);

        $is_first = true;
        while ($rrec = db_fetch_object($rrs)) {
            //Ignore the first record, as we just need to make copies of other records
            if ($is_first) {
                $is_first = false;
                continue;
            }
            //Load current node
            //Create a node
            $resource_node = new stdClass();
            $resource_node->title = $rrec->title;
            $resource_node->type = $rrec->nodetype;   // Your specified content type
            $resource_node->created = $rrec->created;
            $resource_node->body = $rrec->body;
            $resource_node->status = $rrec->status;
            $resource_node->sticky = 0;
            $resource_node->format = 1;       // Filtered HTML
            $resource_node->uid = $rrec->uid;
            $resource_node->comment = 2;

            node_save($resource_node);
            $nid = $resource_node->nid;

            $query = db_query("INSERT INTO {ntlp_course_resource}
                (nid, course_nid, fid, type, attachment_name, path, deleted_by, deleted_on, is_shared)
                VALUES (%d, %d, %d, '%s', '%s', '%s', NULL, NULL, 0)", $nid, $rrec->course_nid, $rrec->fid, $rrec->type, $rrec->attachment_name, $rrec->path);

            $query = db_query("UPDATE {ntlp_project_resource} SET nid = %d 
                WHERE nid = %d AND project_nid = %d AND section_tid = %d", $nid, $prec->nid, $rrec->project_nid, $rrec->section_tid);
        }
    }
}

//The vocabularies for Keywords should be separate for all the three Libraries
//STATUS: NOT IMPLEMENTED; PENDING
function assign_keywords_tonew_vocabulary_march2011() {
    print '<p><strong>Assign Keywords to New Vocabularies</strong></p>';
    $prs = db_query("select td.", $t, $t);
    print 'Found mapped keywords of Resource Library:' . $prs->num_rows . '<br>';
    $obj = get_vocabulary_vid('NTLP Course Section');
    $vid = $obj->vid;

    while ($prec = db_fetch_object($prs)) {
        db_query("INSERT INTO {term_data} (vid, name, description, weight) VALUES (%d, '%s', '%s', 0)", $vid, "Activities", "Default Project Section for Activities");
        $tid = db_last_insert_id('{term_data}', 'tid');

        $result = db_query("SELECT max(section_order) maxorder FROM {ntlp_course_section} WHERE course_nid = %d", $prec->nid);
        $rec = db_fetch_object($result);
        $maxorder = $rec->maxorder + 1;

        db_query("INSERT INTO {ntlp_course_section} (tid, course_nid, section_order, availability)
            VALUES (%d, %d, %d, '%s')", $tid, $prec->nid, $maxorder, 'S');

        $result = db_query("UPDATE {ntlp_course_activity} SET project_cat = %d
            WHERE project_nid = %d AND (project_cat IS NULL OR project_cat = 0)", $tid, $prec->nid);
    }
}

//27-May-2011
//Outcomes and Grade levels are now restructured and maintained for each school year
//We need to copy all outcomes and grade levels in all the available years.
function ntlp_populate_outcomes_for_all_school_years() {
    $result = db_query("SELECT nid FROM dpl_ntlp_school");

    echo 'Total School ' . $result->num_rows . ' </br>';

    while ($data = db_fetch_object($result)) {

        $courses_year_query = 'SELECT DISTINCT school_year FROM dpl_ntlp_school_term
            WHERE school_nid = %d AND school_year > 0 AND school_year <> 2011';

        $school_courses_year_result = db_query($courses_year_query, $data->nid);

        $o = 0;
        $g = 0;
        while ($record = db_fetch_object($school_courses_year_result)) {

            db_query("INSERT INTO dpl_ntlp_gb_outcome
                (SELECT tid, school_nid, weight_percent, sort_order, short_name, %d FROM dpl_ntlp_gb_outcome
                WHERE school_nid = %d AND school_year = 2011)", $record->school_year, $data->nid);
            $o = db_affected_rows();

            db_query("INSERT INTO dpl_ntlp_school_grade
                (SELECT school_nid, label, lower_limit, %d FROM dpl_ntlp_school_grade
                WHERE school_nid = %d AND school_year = 2011)", $record->school_year, $data->nid);
            $g = db_affected_rows();
        }
        echo $data->nid . ' School ' . $data->title . ' : Total School Year = ' . $school_courses_year_result->num_rows . ":$o:$g". '<br/>';
    }
}


function repair_permission_table_data() {

    $result = db_query("SELECT * FROM {permission}");

    if ($result) {
        while ($record = db_fetch_object($result)) {
            $permissions_str = '';
            $role_id = $record->rid;
            $temp_arr = explode(",", $record->perm);

            $flag = true;

            foreach ($temp_arr as $perm) {
                if ($flag) {
                    $permissions[] = trim($perm);
                    $flag = false;
                } else {
	                $permissions[] = ' ' . trim($perm);
	            }
            }

            $permissions = array_unique($permissions);
            $permissions_str = implode(',', $permissions);
            
            db_query("UPDATE {permission} SET perm = '%s' WHERE rid = %d", $permissions_str, $role_id);
        }
        
        echo 'Permission data fixed';
    }
}

function fix_multiple_gb_grades($school_nid, $debug = 1) {
    fix_multiple_gb_grades_type1($school_nid, $debug);
    //fix_multiple_gb_grades_type2($school_nid, $debug);
}

function fix_multiple_gb_grades_type1($school_nid, $debug = 1) {
    //Get all duplicates, which were marked as graded in old submission table
    $crs = db_query("SELECT COUNT(*) cnt, g.activity_nid, g.user_uid 
        FROM dpl_ntlp_gb_grade g, dpl_ntlp_course_activity a, dpl_ntlp_course c
        WHERE g.activity_nid = a.nid AND a.course_nid = c.nid AND c.school_nid = %d
        GROUP BY g.activity_nid, g.user_uid HAVING cnt > 1", $school_nid);

    $fixed = 0;
    if ($crs->num_rows > 0) {
        print '<br>Fixing data for School id:' . $school_nid. '<br><br>';
        while ($crec = db_fetch_object($crs)) {
            //Get last graded record
            $asrs = db_query("SELECT *, (SELECT COUNT(*) FROM dpl_ntlp_gb_activity_grade WHERE grade_id = g.id) cnt
                FROM dpl_ntlp_gb_grade g WHERE activity_nid = %d AND user_uid = %d ORDER BY date_graded DESC, id DESC",
                $crec->activity_nid, $crec->user_uid);

            $correct_id = 0;
            $max = -1;
            $date_graded = '';
            $comments = '';
            while ($asrec = db_fetch_object($asrs)) {
                //Check that the id we got is the correct one
                if ($correct_id == 0 || $max < $asrec->cnt) {
                    $correct_id = $asrec->id;
                    $max = $asrec->cnt;
                }
                if (empty($date_graded) && isset($asrec->date_graded)) {    //Get value if we don't have one
                    $date_graded = $asrec->date_graded;
                }
                if (empty($comments) && isset($asrec->teachers_notes)) {    //Get value if we don't have one
                    $comments = $asrec->teachers_notes;
                }
            }

            echo "Correct Grade_id for Activity $crec->activity_nid and User $crec->user_uid is: $correct_id<br>";
            if ($debug == 0) {
                if ($correct_id > 0) {
                    db_query("DELETE FROM {ntlp_gb_grade} WHERE id <> %d AND activity_nid = %d AND user_uid = %d",
                        $correct_id, $crec->activity_nid, $crec->user_uid);

                    if (!empty($date_graded)) {
                        db_query("UPDATE {ntlp_gb_grade} SET date_graded = '%s' WHERE id = %d AND activity_nid = %d AND user_uid = %d",
                            $date_graded, $correct_id, $crec->activity_nid, $crec->user_uid);
                    }
                    if (!empty($comments)) {
                        db_query("UPDATE {ntlp_gb_grade} SET teachers_notes = '%s'
                            WHERE id = %d AND activity_nid = %d AND user_uid = %d",
                            $comments, $correct_id, $crec->activity_nid, $crec->user_uid);
                    }
                }
            }
            $fixed++;
            if ($fixed > 500) {
                break;
            }
        }
        if ($fixed > 500) {
            break;
        }
        flush();
    }
    print '<br>Type1: Number of grade records fixed:' . $fixed . '<br>';
}

function fix_multiple_gb_grades_type2($school_nid, $debug = 1) {
//    $srs = db_query("SELECT nid FROM dpl_ntlp_school ORDER BY nid");

//    while ($srec = db_fetch_object($srs)) {
        //Get all duplicates, which were marked as graded in old submission table
        $crs = db_query("SELECT COUNT(*) cnt, g.activity_nid, g.user_uid 
            FROM dpl_ntlp_gb_grade g, dpl_ntlp_course_activity a, dpl_ntlp_course c
            WHERE (SELECT SUM(is_graded) FROM dpl_ntlp_activity_submission_backup 
                    WHERE resubmit_count = 1 AND activity_nid = g.activity_nid and user_uid = g.user_uid) > 0
            AND g.activity_nid = a.nid AND a.course_nid = c.nid AND c.school_nid = %d
            GROUP BY g.activity_nid, g.user_uid HAVING cnt > 1", $school_nid);

        $fixed = 0;
        if ($crs->num_rows > 0) {
            print '<br>Fixing data for School id:' . $school_nid. '<br><br>';
            while ($crec = db_fetch_object($crs)) {
                //Get last graded record
                $asrs = db_query("SELECT *, (SELECT COUNT(*) FROM dpl_ntlp_gb_activity_grade WHERE grade_id = g.id) cnt
                    FROM dpl_ntlp_gb_grade g WHERE activity_nid = %d AND user_uid = %d ORDER BY date_graded DESC, id DESC",
                    $crec->activity_nid, $crec->user_uid);

                $correct_id = 0;
                $max = -1;
                $date_graded = '';
                $comments = '';
                while ($asrec = db_fetch_object($asrs)) {
                    //Check that the id we got is the correct one
                    if ($correct_id == 0 || $max < $asrec->cnt) {
                        $correct_id = $asrec->id;
                        $max = $asrec->cnt;
                    }
                    if (empty($date_graded) && isset($asrec->date_graded)) {    //Get value if we don't have one
                        $date_graded = $asrec->date_graded;
                    }
                    if (empty($comments) && isset($asrec->teachers_notes)) {    //Get value if we don't have one
                        $comments = $asrec->teachers_notes;
                    }
                }

                echo "Correct Grade_id for Activity $crec->activity_nid and User $crec->user_uid is: $correct_id<br>";
                if ($debug == 0) {
                    if ($correct_id > 0) {
                        db_query("DELETE FROM {ntlp_gb_grade} WHERE id <> %d AND activity_nid = %d AND user_uid = %d",
                            $correct_id, $crec->activity_nid, $crec->user_uid);

                        if (!empty($date_graded)) {
                            db_query("UPDATE {ntlp_gb_grade} SET date_graded = '%s'
                                WHERE id = %d AND activity_nid = %d AND user_uid = %d",
                                $date_graded, $correct_id, $crec->activity_nid, $crec->user_uid);
                        }
                        if (!empty($comments)) {
                            db_query("UPDATE {ntlp_gb_grade} SET teachers_notes = '%s'
                                WHERE id = %d AND activity_nid = %d AND user_uid = %d",
                                $comments, $correct_id, $crec->activity_nid, $crec->user_uid);
                        }
                    }
                }
                $fixed++;
                if ($fixed > 500) {
                    break;
                }
            }
            if ($fixed > 500) {
                break;
            }
            flush();
        }
//        if ($fixed > 0) {
//            break;  //One school at a time
//        }
//    }
    print '<br>Type2: Number of grade records fixed:' . $fixed . '<br>';
}
