<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'reports');
module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_grade', 'data_access');
module_load_include('inc', 'ntlp_school', 'basic_grade_report');
module_load_include('inc', 'ntlp_school', 'timezone');

/* Form To  Create New User In School Management START */

function school_user_registration_form($form_state, $school_nid) {

    include('school_management.template.inc');
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/ntlp_course_agenda.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/date_picker.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/school_ahah_helper.js');

// Course NID 0 For people finder In order to find all students
    $course_nid = 0;
//    check_user_permi

    /* Last URL */
    $last_url = $_SERVER['HTTP_REFERER'];


    ahah_helper_register($form, $form_state);

//     ahah_helper_register($form, $form_state);
//    $user_role = check_user_permission($school_nid);
//
//    if($user_role == NTLP_ROLEID_SITEADMIN || $user_role == NTLP_ROLEID_SCHOOLLEADER
//            || $user_role == NTLP_ROLEID_NETWORKMANAGER || $user_role == NTLP_ROLEID_NETWORKSTAFF) {
//
//    }else {
//        drupal_set_message("You're Not Allowed to access School Setting Page ");
//        return ;
//    }

    _user_password_dynamic_validation();

    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');

//    watchdog('info', 'user value:<br />'.print_r($form_state, true));
    if (isset($form_state['values'])) {

        $view_state = $form_state['values'];
        $roles_values = $form_state['values']['main']['user_role'];
        $button_name = $form_state['values']['op'];
        $school_nid = $form_state['values']['main']['school_id'];
//        watchdog('info', 'role value:<br />'.print_r($form_state, true));
    }

    $edit = check_plain(arg(4));

    if ($edit == 'edit') {
        $user_id = check_plain(arg(5));
        $school_nid = get_this_school();

        $user_record = get_user_record($user_id);
        $roles_value = $user_record->rid;

        if ($rec_users = get_parent_students($user_id)) {

            while ($row = db_fetch_array($rec_users)) {

//Don't fetch the primary teacher
                $user_Obj = get_user_detail_($row['student_uid']);
                $course_additional_teacherids .= ',' . $user_Obj->uid;
                $course_additional_teachernames .= '<br />' . $user_Obj->first_name . ' ' . $user_Obj->last_name;
            }
        }
    } else {
        $edit = '';
    }

    if (isset($form_state['values']['current_url'])) {
        $query_string = $form_state['values']['current_url'];
    } else {
        $query_str = $_SERVER['QUERY_STRING'];
        $q_array = explode("=", $query_str);

        $query_string = $q_array[1];
    }

    $form = array(
        '#cache' => TRUE,
        '#action' => url($query_string),
//        $_SERVER['QUERY_STRING']
    );

    $form['current_url'] = array(
        '#type' => 'hidden',
        '#value' => $query_string,
    );

    $form['back_to_school_management'] = array(
        '#type' => 'item',
        '#value' => l('« Back to School Management', 'ntlp/school/management/users'),
        '#prefix' => '<div style="float:left;padding:0px;"><div class="BoldrBrandCrumb">',
        '#suffix' => '</div>',
    );

    if ($edit == 'edit') {

        $title = t('Edit User');
    } else {

        $title = t('Create a new User');
    }

    $form['title'] = array(
        '#type' => 'item',
        '#value' => $title,
        '#prefix' => '<h2 style="color:#3370A7;margin-top:0px;font-size:20px;">',
        '#suffix' => '</h2></div>',
    );

    $form['cancel'] = array(
        '#type' => 'button',
        '#value' => t('Cancel'),
        '#executes_submit_callback' => TRUE,
        '#attributes' => array('class' => 'CancelBtn'),
        '#prefix' => '<div style="float:right">',
        '#suffix' => '',
    );


    if ($edit == 'edit') {
        $form['create'] = array(
            '#type' => 'button',
            '#value' => t('Save Changes'),
            '#executes_submit_callback' => TRUE,
            '#submit' => array('school_user_registration_submit'),
            '#attributes' => array('class' => 'saveandnotifyBtn'),
            '#prefix' => '',
            '#suffix' => '</div><br clear="all" />',
        );
    } else {
        $form['create'] = array(
            '#type' => 'button',
            '#value' => t('Create User'),
            '#executes_submit_callback' => TRUE,
            '#submit' => array('school_user_registration_submit'),
            '#attributes' => array('class' => 'saveandnotifyBtn'),
            '#prefix' => '',
            '#suffix' => '</div><br clear="all" />',
        );
    }
    $form['main'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => '<div id="add-user-wrapper">' . $HTMLform_school_setting_bluebox_start,
        '#suffix' => $HTMLform_school_setting_bluebox_end . '</div>',
    );

    $form['main']['school_id'] = array(
        '#type' => 'hidden',
        '#value' => $school_nid,
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div>',
    );
    $form['main']['user_id'] = array(
        '#type' => 'hidden',
        '#value' => $user_id,
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div>',
    );
    $form['main']['last_url'] = array(
        '#type' => 'hidden',
        '#value' => $last_url,
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div></table><table class="table" cellpadding="8" cellspacing="0" border="0" width="100%">',
    );




//    $form['main']['cancel'] = array(
//            '#type' => 'button',
//            '#value' => t('Cancel '),
//            '#executes_submit_callback' => TRUE,
//            '#submit' => array(''),
//            '#attributes' => array('class' => 'SaveBtn'),
//            '#prefix' => '',
//            '#suffix' => '</td></tr>',
//    );
//    $role_ids = NTLP_ROLEID_PARENT .','. NTLP_ROLEID_STUDENT .','. NTLP_ROLEID_TEACHER .','.  NTLP_ROLEID_SITEADMIN .','. NTLP_ROLEID_NETWORKSTAFF.','.NTLP_ROLEID_NETWORKMANAGER;


    if ($roles_value == NTLP_ROLEID_TEACHER) {

        $role_ids = NTLP_ROLEID_TEACHER . ',' . NTLP_ROLEID_SITEADMIN . ',' . NTLP_ROLEID_SITEGUEST . ',' . NTLP_ROLEID_SCHOOLLEADER;
    } elseif ($roles_value == NTLP_ROLEID_SITEADMIN) {

        $role_ids = NTLP_ROLEID_TEACHER . ',' . NTLP_ROLEID_SITEADMIN . ',' . NTLP_ROLEID_SITEGUEST . ',' . NTLP_ROLEID_SCHOOLLEADER;
    } elseif ($roles_value == NTLP_ROLEID_SITEGUEST) {

        $role_ids = NTLP_ROLEID_TEACHER . ',' . NTLP_ROLEID_SITEADMIN . ',' . NTLP_ROLEID_SITEGUEST . ',' . NTLP_ROLEID_SCHOOLLEADER;
    } elseif ($roles_value == NTLP_ROLEID_SCHOOLLEADER) {

        $role_ids = NTLP_ROLEID_TEACHER . ',' . NTLP_ROLEID_SITEADMIN . ',' . NTLP_ROLEID_SITEGUEST . ',' . NTLP_ROLEID_SCHOOLLEADER;
    } elseif ($roles_value == NTLP_ROLEID_PARENT) {
        $role_ids = NTLP_ROLEID_PARENT;
    } elseif ($roles_value == NTLP_ROLEID_STUDENT) {

        $role_ids = NTLP_ROLEID_STUDENT;
    } else {
        $role_ids = NTLP_ROLEID_PARENT . ',' . NTLP_ROLEID_STUDENT . ',' . NTLP_ROLEID_TEACHER . ',' . NTLP_ROLEID_SITEADMIN . ',' . NTLP_ROLEID_SITEGUEST . ',' . NTLP_ROLEID_SCHOOLLEADER;
    }

    $roles = get_all_users_role($role_ids);

    // get the value of role and

    if (isset($form_state['values']['main']['user_role'])) {
        $default_role_id = $form_state['values']['main']['user_role'];
    } elseif ($edit != 'edit') {

        $default_role_id = NTLP_ROLEID_STUDENT;
    }



    $form['main']['user_role'] = array(
        '#type' => 'select',
        '#options' => $roles,
        '#attributes' => array('class' => 'my-dropdown'),
        '#ahah' => array(
            'path' => ahah_helper_path(array('main')),
            'wrapper' => 'add-user-wrapper',
            'method' => 'replace',
            'progress' => 'none',
        ),
        '#default_value' => ($edit != null) ? $user_record->rid : $default_role_id,
//            '#default_value' => NTLP_ROLEID_STUDENT,
        '#prefix' => '<tr><th style="width:160px">' . t('User Role') . '</th><td colspan="5"><span style="width:110px;" class="comboboxBgLeft">',
        '#suffix' => '</span></td></tr>',
    );


    $form['main']['first_name'] = array(
        '#type' => 'textfield',
//            '#required' => TRUE,
        '#size' => 25,
        '#default_value' => ($edit != null) ? $user_record->first_name : $form_state['values']['main']['first_name'],
        '#prefix' => '<tr><th style="width:160px">' . t('First Name*') . '</th><td>',
        '#suffix' => '</td>',
    );
    $form['main']['mi'] = array(
        '#type' => 'textfield',
        '#size' => 2,
        '#default_value' => ($edit != null) ? $user_record->middle_name : $form_state['values']['main']['mi'],
        '#prefix' => '<th style="width:160px">' . t('MI') . '</th><td>',
        '#suffix' => '</td>',
    );
    $form['main']['last_name'] = array(
        '#type' => 'textfield',
//            '#required' => TRUE,
        '#size' => 25,
        '#default_value' => ($edit != null) ? $user_record->last_name : $form_state['values']['main']['last_name'],
        '#prefix' => '<th style="width:160px">' . t('Last Name*') . '</th><td>',
        '#suffix' => '</td></tr>',
    );




    $user_name = explode('@', $user_record->mail);

    $user = $user_name[0];
    $domain = '@' . variable_get('grupal_domain' . $school_nid . '_', '');
//    echo variable_get('grupal_domain'.$school_nid.'_', '');
    if ($edit != 'edit') {
        if ($roles_values == NTLP_ROLEID_TEACHER || $roles_values == NTLP_ROLEID_SITEADMIN || $roles_values == NTLP_ROLEID_SITEGUEST || $roles_values == NTLP_ROLEID_SCHOOLLEADER || $roles_values == NTLP_ROLEID_STUDENT) {
            $domain = '@' . variable_get('grupal_domain' . $school_nid . '_', '');
        } elseif ($roles_values == NTLP_ROLEID_PARENT) {

            $user = $user_record->mail;
            $domain = '';
        }
    } else {

        if ($roles_value == NTLP_ROLEID_TEACHER || $roles_value == NTLP_ROLEID_SITEADMIN || $roles_value == NTLP_ROLEID_SITEGUEST || $roles_value == NTLP_ROLEID_SCHOOLLEADER || $roles_value == NTLP_ROLEID_STUDENT) {
            $domain = '@' . variable_get('grupal_domain' . $school_nid . '_', '');
        } elseif ($roles_value == NTLP_ROLEID_PARENT) {

            $user = $user_record->mail;
            $domain = '';
        }
    }


    $form['main']['user_name'] = array(
        '#type' => 'textfield',
//            '#required' => TRUE,
        '#size' => 25,
        '#default_value' => ($edit == 'edit') ? $user : $form_state['values']['main']['user_name'],
        '#prefix' => '<tr><th style="width:160px">' . t('User Name*') . '</th><td width="550">',
        '#suffix' => $domain . '</td>',
//            '#attributes' => 'onclick '
    );

    $form['main']['password'] = array(
        '#type' => 'password_confirm',
//          '#required' => ($edit == 'edit') ? FALSE : TRUE,
        '#size' => 25,
        '#prefix' => '<th style="width:160px">' . t('Password*') . '</th><td colspan="3">',
        '#suffix' => '</td></tr>',
    );


    $form['main']['password_change'] = array(
        '#type' => 'checkbox',
        '#title' => t('Require password change on first login ?'),
        '#prefix' => '<tr><td  align = "left" colspan="6">',
        '#suffix' => '</td></tr>',
    );


    if ($roles_values == NTLP_ROLEID_TEACHER || $roles_values == NTLP_ROLEID_SITEADMIN
            || $roles_values == NTLP_ROLEID_NETWORKSTAFF || $roles_values == NTLP_ROLEID_SCHOOLLEADER
            || $user_record->rid == NTLP_ROLEID_TEACHER || $user_record->rid == NTLP_ROLEID_SCHOOLLEADER
            || $user_record->rid == NTLP_ROLEID_SITEADMIN || $user_record->rid == NTLP_ROLEID_NETWORKSTAFF) {

        $form['main']['staff_id'] = array(
            '#type' => 'textfield',
            '#size' => 5,
            '#default_value' => ($edit != null) ? $user_record->user_code : $form_state['values']['main']['staff_id'],
            '#prefix' => '<tr><th style="width:160px">' . t('Staff ID') . '</th><td>',
            '#suffix' => '</td>',
        );
        $form['main']['job_title'] = array(
            '#type' => 'textfield',
            '#size' => 50,
            '#default_value' => ($edit != null) ? $user_record->job_title : $form_state['values']['main']['job_title'],
            '#prefix' => '<th style="width:160px">' . t('Job Title') . '</th><td colspan="3">',
            '#suffix' => '</td></tr>',
        );
    }

    if ($roles_values == NTLP_ROLEID_PARENT || $user_record->rid == NTLP_ROLEID_PARENT) {


        $form['main']['student_finder_heading'] = array(
            '#type' => 'item',
            '#value' => t('Associated Student(s)'),
            '#prefix' => '<tr><th style="width:160px;white-space:nowrap;">',
            '#suffix' => '</th>',
        );
        $form['main']['student_name'] = array(
            '#type' => 'item',
            '#value' => $course_additional_teachernames,
            '#prefix' => '<td colspan="2"><div id="student_names">',
            '#suffix' => '</div>',
        );
        $form['main']['student_ids'] = array(
            '#type' => 'hidden',
            '#id' => 'student_ids',
            '#value' => $course_additional_teacherids,
        );
        cache_set("student_people_finder_" . session_id(), $course_additional_teacherids, 'cache', CACHE_TEMPORARY);
        $form['main']['student_people_finder'] = array(
            '#type' => 'item',
            '#value' => dlg('Select People', 'ntlp/people_finder/teacher_multi/' .
                    $school_nid . '/' .
                    $course_nid . '/' .
                    "student_people_finder_" . session_id() . '/' .
                    NTLP_ROLEID_STUDENT, 350, 600),
            '#prefix' => '<div class="peopleFinderBtn">',
            '#suffix' => '</div></td></tr>',
        );
    }

    if ($roles_values == NTLP_ROLEID_STUDENT || $user_record->rid == NTLP_ROLEID_STUDENT || $default_role_id == NTLP_ROLEID_STUDENT) {

        $grad = get_school_grad_years($user_record->grad_year);

//        watchdog('info', 'gradyyear' . print_r($grad, true));
        $form['main']['grad_year'] = array(
            '#type' => 'select',
            '#options' => $grad,
            '#attributes' => array('class' => 'my-dropdown'),
            '#default_value' => ($edit != null) ? $user_record->grad_year : $form_state['values']['main']['grad_year'],
            '#prefix' => '<tr><th style="width:160px">' . t('Grad Year') . '</th><td colspan="2"><span style="width:110px;" class="comboboxBgLeft">',
            '#suffix' => '</span></td></tr>',
        );

        $form['main']['staff_id'] = array(
            '#type' => 'textfield',
            '#size' => 25,
            '#default_value' => ($edit != null) ? $user_record->user_code : $form_state['values']['main']['staff_id'],
            '#prefix' => '<tr><th style="width:160px">' . t('Student ID') . '</th><td>',
            '#suffix' => '</td></tr>',
        );
    }

    return $form;
}

/* Form To  Create New User In School Management START */

function school_user_registration_form_validate($form, $form_state) {
    cache_set("ntlp_user_register:" . session_id(), NULL, 'cache', CACHE_TEMPORARY);

    _user_password_dynamic_validation();

    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/school_ahah_helper.js');
    $button_value = $form_state['clicked_button']['#post']['op'];
    if ($button_value == 'Cancel') {

        $last_url = $form_state['clicked_button']['#post']['main']['last_url'];
        drupal_goto($last_url);
    } else {
        $user_role = $form_state['clicked_button']['#post']['main']['user_role'];
        $school_nid = $form_state['clicked_button']['#post']['main']['school_id'];
        $first_name = $form_state['clicked_button']['#post']['main']['first_name'];
        $user_name = $form_state['clicked_button']['#post']['main']['user_name'];
        $last_name = $form_state['clicked_button']['#post']['main']['last_name'];
        $mi = $form_state['clicked_button']['#post']['main']['mi'];

        $password1 = $form_state['clicked_button']['#post']['main']['password']['pass1'];
        $password2 = $form_state['clicked_button']['#post']['main']['password']['pass2'];

        $user_id = $form_state['clicked_button']['#post']['main']['user_id'];

        $store_into_temp = new stdClass;

        $store_into_temp->first_name = $first_name;
        $store_into_temp->last_name = $last_name;
        $store_into_temp->user_name = $user_name;
        $store_into_temp->mi = $mi;
        $store_into_temp->password1 = $password1;
        $store_into_temp->password2 = $password2;

        cache_set("ntlp_user_register:" . session_id(), $store_into_temp, 'cache', CACHE_TEMPORARY);

// Changes By Saifuddin only checking.. for parent.. to user name for valid email
        if ($user_role == NTLP_ROLEID_PARENT) {

            // changes by saif 25 Oct as user_name stored in db as lower case
            if (!valid_email_address(strtolower($user_name))) {
                form_set_error('main][user_name', t('The email address appears to be invalid.'));
            } else {
                // check the user name existence in db if its exists then prompt usre
            }
        } else {
            if ($user_name == '') {
                form_set_error('main][user_name', t('User Name cannot be empty!'));
            }
        }

        //Validate only when its not empty
        if (strlen($password1) > 0 && strlen($password2) > 0) {
            if (strlen($password1) < MIN_PASSWORD_LENGTH || strlen($password2) < MIN_PASSWORD_LENGTH) {
                form_set_error('', 'Password should be at least eight characters long.');
            }
        }

        $domain = variable_get('grupal_domain' . $school_nid . '_', '');
        if (empty($domain)) {
            form_set_error('', t('School domain not found.'));
        }

        if ($first_name == '') {
            form_set_error('main][first_name', t('First Name cannot be empty!'));
        }

        if ($last_name == '') {
            form_set_error('main][last_name', t('Last Name cannot be empty!'));
        }



        if ($button_value == 'Save Changes') {


            if (!empty($password1) && !empty($password2)) {

                if ($password1 != $password2) {
                    form_set_error('', t('Password cannot be empty!'));
                }
            }
        } else {
            if ($password1 != $password2 || empty($password1) || empty($password2)) {

                form_set_error('', t('Password cannot be empty!'));
            }
        }
    }
}

/*  */
/* Form To  Create New User In School Management SUBMIT START */

function school_user_registration_submit($form, $form_state) {

    $school_nid = $form['#post']['main']['school_id'];
    $user_id = $form['#post']['main']['user_id'];

    $user_role = $form['#post']['main']['user_role'];
    $first_name = $form['#post']['main']['first_name'];
    $last_name = $form['#post']['main']['last_name'];
    // changes by saif.. 25 Oct to store the username in lower case
    $user_name = strtolower($form['#post']['main']['user_name']);
    $password = $form['#post']['main']['password']['pass1'];

    $password_change = (int) $form['#post']['main']['password_change'];

    $staff_id = $form['#post']['main']['staff_id'];
    $job_title = $form['#post']['main']['job_title'];
    $grad_year = $form['#post']['main']['grad_year'];
    $student_ids = $form['#post']['main']['student_ids'];
    $mi = $form['#post']['main']['mi'];
    $school_detail = get_school_details($school_nid);
    if (empty($grad_year)) {

        $grad_year = 0;
    }

    $domain = variable_get('grupal_domain' . $school_nid . '_', '');
    $recipient_ids = explode(",", $student_ids);

    if ($user_role == NTLP_ROLEID_PARENT) {
        $email = $user_name;
    } else {
        $user_name = grupal_cleanname($user_name);
        if ($domain) {
            $email = $user_name . '@' . $domain;
        } else {
            drupal_set_message('School domain not found.', 'error');
            return;
        }
    }
    $setup_google = ($user_role == NTLP_ROLEID_PARENT
            || $user_role == NTLP_ROLEID_SITEGUEST
            || $user_role == NTLP_ROLEID_NETWORKGUEST ? FALSE : TRUE);

    if ($user_id != null) {

        $existingUser = new stdClass();
        $existingUser->name = $email;

        /* it will not update the password on Edit Mode if user left the password blank */
        if ($password != null || !empty($password)) {
            $existingUser->pass = $password;
            //Change password in ldap
            ntlp_change_password_other_user($password, $user_id);
        }

//        $existingUser->status = 1;
        $existingUser->mail = $email;
        $existingUser->init = $email;
        $existingUser->ntlp_schools = $school_nid;
//Custom fields - used by Grupal
        $existingUser->setup_google = $setup_google;
        $existingUser->firstname = $first_name;
        $existingUser->lastname = $last_name;

//          $newUser->is_new = TRUE;
        $u = user_save((object) array('uid' => $user_id), (array) $existingUser);

        db_query("UPDATE {ntlp_user} SET first_name = '%s', middle_name ='%s', last_name = '%s',
            grad_year = %d , user_code = %d, job_title = '%s', password_change = %d where uid = %d", $first_name, $mi, $last_name, $grad_year, $staff_id, $job_title, $password_change, $user_id);

//TODO: "roles" array can be used instead of doing this
        db_query("UPDATE {users_roles} SET rid = %d  where uid = %d", $user_role, $user_id);

        //TRIGGER_POINT: UPDATE STUDENT RECORD IN ALL COURSES AND TERMS
        dw_set_students_all_course_grade_toupdate($user_id);

        drupal_set_message('<div>User<div>
                        <div><strong>' . $first_name . ' ' . $last_name . '</strong><div>
                        <div>Modified Successfully<div>');
    } else {
        $data = array(
            'name' => $email,
            'pass' => $password,
            'status' => 1,
            'mail' => $email,
            'init' => $email,
            'ntlp_schools' => $school_nid,
//Custom fields - used by Grupal
            'setup_google' => $setup_google,
            'firstname' => $first_name,
            'lastname' => $last_name,
            'status_google' => FALSE,
        );

        $u = user_save(null, $data);

        //Return if Google Apps returns an error while creating the account.
//        watchdog('user_status', print_r($u, true));
//        if ($setup_google && $u->status_google == FALSE) {
//            //Disabling it, because the value in $u-status_google is not coming correctly.
//            if (!empty($u->uid)) {
//                db_query("DELETE FROM {users} WHERE uid = %d", $u->uid);
//                db_query("DELETE FROM {ntlp_user} WHERE uid = %d", $u->uid);
//            }
//            drupal_set_message('Unable to create/modify user account, please check Google Apps settings and any invalid value in user info.');
//            return;
//        }
        if ($u && intval($u->uid) > 0) {

            //When a validation error occurs, record is not created in ntlp_user table
            //We need to create a record, if its not there
            $rs = db_query("select uid from {ntlp_user} where uid = %d", $u->uid);
            if ($rs->num_rows > 0) {
                db_query("UPDATE {ntlp_user} SET first_name = '%s' , middle_name = '%s' , last_name = '%s' ,
                        grad_year = %d, job_title = '%s' , user_code =  %d, password_change = %d
                        WHERE uid = %d  ", $first_name, $mi, $last_name, $grad_year, $job_title, $staff_id, $password_change, $u->uid);
            } else {
                db_query("INSERT INTO {ntlp_user} (uid, school_nid, first_name, middle_name, last_name, grad_year,
                        job_title, user_code, password_change, is_active)
                        VALUES (%d, %d, '%s', '%s', '%s', %d,
                        '%s', %d, %d, 1) ", $u->uid, $school_nid, $first_name, $mi, $last_name, $grad_year, $job_title, $staff_id, $password_change);
            }
            //TODO: "roles" array can be used instead of doing this
            db_query("INSERT INTO {users_roles} (uid,rid) VALUES (%d, %d)", $u->uid, $user_role);

            //TRIGGER_POINT: UPDATE STUDENT RECORD IN ALL COURSES AND TERMS
            dw_set_students_all_course_grade_toupdate($u->uid);

            modalframe_parent_js();
            drupal_set_message('<div>Congratulations!<div>
                        <div>User:<strong>' . $first_name . ' ' . $last_name . '</strong>
                        &nbsp;Email:<strong>' . $email . '</strong><div><br />
                        <div>Created Successfully<div>' .
                    '<table cellspacing="8" cellpading="8" width="100%">
                         <tr>
                            <td width = "180"><div>' .
                    dlg('Print Login Information', "ntlp/print/login/info/$u->uid/$password", 300, 400, 'SubmitAssignmentBtn showPrintUser') .
                    '</div></td><td>&nbsp;</td>
                            <td width = "180">' .
                    l('Create Another User', 'ntlp/school/manage/create/user/' . $school_nid, array('attributes' => array('class' => 'SubmitAssignmentBtn', 'onclick' => 'Drupal.modalFrame.close()'))) .
                    '</td><td>&nbsp;</td>
                            <td width = "180">'
                    . l('Return to User List', 'ntlp/school/management/users', array('attributes' => array('class' => 'SubmitAssignmentBtn', 'onclick' => 'Drupal.modalFrame.close()'))) .
                    '</td><td>&nbsp;</td>
                          </tr>
                          </table>'
            );
        } else {
            drupal_set_message('System unable to create/modify user account!');
        }
    }

    $user_id = !isset($user_id) ? $u->uid : $user_id;

    if ($user_role == NTLP_ROLEID_PARENT && $recipient_ids != null) {
        if (isset($user_id)) {
            db_query('DELETE FROM {ntlp_user_parent} where parent_uid =%d', $user_id);
            foreach ($recipient_ids as $uid) {
                if (!empty($uid)) {
                    db_query("INSERT INTO {ntlp_user_parent} (parent_uid, student_uid) VALUES (%d, %d)", $u->uid, $uid);
                }
            }
        }
    }
}

//function ntlp_print_user_login_info($uid, $password) {
//    $u = user_load(array('uid' => $uid));
//    modalframe_child_js();
//    $url = ($_SERVER['HTTPS'] ? 'https' : 'http' )."://{$_SERVER['HTTP_HOST']}/";
//    return "
//    <script>
//    function hide_print(){
//        $('#hide_print').hide();
//        }
//    </script>
//    <table class=\"ntlp_table\" cellpadding=\"5\">
//        <tr>
//            <th align=\"left\">User Login</th>
//            <th align=\"left\">Password</th>
//            <th align=\"left\">Login page</th>
//        </tr>
//        <tr>
//            <td>$u->mail</td>
//            <td>$password</td>
//            <td>$url</td>
//        </tr>
//    </table>
//    <div id='hide_print'><a href=\"javascript:;\" onclick=\"print();hide_print();\">Print Info</a>";
//}

function ntlp_print_user_login_info($uid, $password) {
    global $base_path;
    $u = user_load(array('uid' => $uid));
    modalframe_child_js();
    $url = ($_SERVER['HTTPS'] ? 'https' : 'http' ) . "://{$_SERVER['HTTP_HOST']}/";
    include('school_management.template.inc');

    $output = $HTMLform_school_setting_print_login_infos;

    $output = str_replace('@BASEPATH', $base_path, $output);
    $output = str_replace('@USERNAME', $u->mail, $output);
    $output = str_replace('@PASSWORD', $password, $output);

    return $output;
}

/* Form To  Create New User In School Management SUBMIT END */





/* Form To  Export Term Data In School Management START */

function data_export($form_state) {
    include('school_management.template.inc');
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/ntlp_course_agenda.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/school_ahah_helper.js');

    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/date_picker.js');
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/jquery.ui.datepicker.css', 'module', 'all', FALSE);
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/demos.css', 'module', 'all', FALSE);
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/jquery.ui.all.css', 'module', 'all', FALSE);


    $school_nid = get_this_school();

//    if (isset($form_state['values'])) {
//        watchdog('info', 'value of form_state:<br />' . print_r($form_state, true));
//
//        $school_year = $form_state['values']['main2']['school_year'];
//    }

    $school_rs = get_school_record($school_nid);
    $school_year = $school_rs->current_school_year;

    $form['heading'] = array(
        '#type' => 'item',
        '#value' => 'Data Export',
        '#prefix' => '<div style="color:#3570AA;font-size:18px;font-weight:bold;">',
        '#suffix' => '</div><div style="font-size:12px">&nbsp;</div>',
    );

    $form['main1'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => '<div id="add-school-term-wrapper">',
        '#suffix' => '</div>',
    );

    $form['main1']['main'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => $HTMLform_school_setting_bluebox_start,
        '#suffix' => $HTMLform_school_setting_bluebox_end,
    );

    $form['main1']['main']['school_nid'] = array(
        '#type' => 'hidden',
        '#id' => 'school_nid',
        '#value' => $school_nid,
    );
    $form['main1']['main']['column_header'] = array(
        '#type' => 'radios',
        '#attribute' => array('class' => 'radiobtnfloat'),
        '#prefix' => '<tr><th>Include column header in export</th><td colspan="4">',
        '#suffix' => '</td></tr>',
        '#options' => array('1' => 'Yes', '0' => 'No'),
        '#default_value' => '1',
    );

    $school_terms = get_school_year_term($school_nid, $school_year);
    if (empty($school_terms)) {
        $school_terms[] = "No Semester Found";
    }
    $form['main1']['main']['term_name'] = array(
        '#type' => 'select',
        '#options' => $school_terms,
        '#prefix' => '<tr><th style="width:160px">' . t('Select Term to export:') . '<small style="color:#fff">(Ctrl-Click to select multiple terms)</small></th><td style="width:210px">',
        '#suffix' => '</td>',
        '#multiple' => true,
        '#attributes' => array('onclick' => 'get_term_courses()', 'style' => 'width:100%;height:150px;'),
    );

    if (empty($courses)) {
        $courses = array('No Courses Found');
    }

    $form['main1']['main']['courses'] = array(
        '#type' => 'select',
        '#options' => $courses,
        '#prefix' => '<th style="width:160px">' . t('Select Courses to export:') . '<small style="color:#fff">(Ctrl-Click to select multiple courses)</small></th><td>',
        '#suffix' => '</td></tr>',
        '#attributes' => array('style' => 'width:100%;height:150px;'),
        '#multiple' => true,
    );

    $form['main1']['main']['export'] = array(
        '#type' => 'item',
        '#value' => '<a href="#" class="SubmitBtnLong" onclick="get_courses_csv(1); return false;">Export Data (CSV)</a>',
        '#prefix' => '<tr><td colspan="4"><div class="GradesTabCenter" style="float:right">',
        '#suffix' => '</div></td></tr>',
    );
//    '#value' => '<a href="#" class="SubmitBtnLong" onclick="get_courses_csv(); return false;">Export Data (CSV)</a>

    //----------------------------------------------------------------------
    // Basic Grade Reports

    $form['main1']['heading2'] = array(
        '#type' => 'item',
        '#value' => 'Basic Grade Reports',
        '#prefix' => '<div style="color:#3570AA;font-size:18px;">',
        '#suffix' => '</div><div style="font-size:12px">&nbsp;</div>',
    );

    $form['main1']['main2'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => $HTMLform_school_setting_bluebox_dataexport_start,
        '#suffix' => $HTMLform_school_setting_bluebox_dataexport_end,
    );


    //Get year from ahah response
    if ($form_state['values']) {
        $school_year = $form_state['values']['main1']['main2']['school_year'];
    } else {
        $school_year = $school_rs->current_school_year;
    }
    $school_terms = get_school_year_term($school_nid, $school_year);
    if (empty($school_terms)) {
        $school_terms[] = "No Semester Found";
    }
    $school_years = get_school_years($school_year);

    $form['main1']['main2']['school_year'] = array(
        '#type' => 'select',
        '#options' => $school_years,
        '#default_value' => (isset($school_rs)) ? $school_year : '',
        '#ahah' => array(
            'event' => 'change',
            'path' => ahah_helper_path(array('main1')),
            'wrapper' => 'add-school-term-wrapper',
        ),
        '#prefix' => '<table cellpadding="8" class="table" cellspacing="0" border="0" width="100%"><tr><th style="width:160px">' . t('School Year:') . '</th><td style="width:210px"><span style="width:120px;" class="comboboxBgLeft">',
        '#suffix' => '</span></td>',
        '#attributes' => array('class' => 'my-dropdown'),
    );

    $form['main1']['main2']['term_id'] = array(
        '#type' => 'select',
        '#options' => $school_terms,
        '#prefix' => '<tr><th style="width:160px">' . t('Select Term to export:') . '</th><td style="width:210px"><span style="width:160px;" class="comboboxBgLeft">',
        '#suffix' => '</span></td>',
        '#attributes' => array('class' => 'my-dropdown'),
    );

    $form['main1']['main2']['export2'] = array(
        '#type' => 'item',
        '#value' => '<a href="#" class="SubmitBtnLong" onclick="get_schoolterm_grade_pdf(0); return false;">Print Report</a>',
//                     <a href="#" class="SubmitBtnLong" onclick="get_schoolterm_grade_pdf(1); return false;">Jasper Report</a>',
        '#prefix' => '<td><div class="GradesTabCenter" style="float:right">',
        '#suffix' => '</div></td></tr></table>',
//        '#attributes' => array('onclick' => 'get_schoolterm_grade_pdf(); return false;', 'class' => 'SubmitAssignmentBtn'),
    );

    return $form;
}

function data_export_form() {
    return drupal_get_form('data_export');
}

/* Form To  Export Term Data In School Management END */





/* Bulk Create Users Form  START */

function bulk_user_form($form_state) {

    include('school_management.template.inc');

    module_load_include('inc', 'ntlp_school', 'fileupload');


    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/fileupload_ahah_helper.js', 'footer');
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/ntlp_school.js');

    drupal_add_js(drupal_get_path('module', 'jquery_ui') . '/jquery.ui/ui/ui.progressbar.js');
    drupal_add_css(drupal_get_path('module', 'jquery_ui') . '/jquery.ui/themes/smoothness/ui.all.css');


//    $user_role = check_user_permission(get_this_school());
//
//    if($user_role == NTLP_ROLEID_SITEADMIN || $user_role == NTLP_ROLEID_SCHOOLLEADER
//            || $user_role == NTLP_ROLEID_NETWORKMANAGER || $user_role == NTLP_ROLEID_NETWORKSTAFF) {
//
//    }else {
//        drupal_set_message("You're Not Allowed to access School Setting Page ");
//        drupal_goto('ntlp/goback') ;
//    }
//    if (isset($form_state['values'])) {
//        $form['#fileupload_embed'] = true;
//        $form['main']['upload_control'] = array(
////                '#prefix' => '<tr><td colspan="3">',
////                '#suffix' => '</td></tr>',
//        );
//        fileupload_get_control($form, $form_state, array('main', 'upload_control'));
////        watchdog('info', 'Resource Share form:<br />'.print_r($form, true));
//    }

    $form = array();

    $form['back_to_school_management'] = array(
//            '#type' => 'markup',
        '#value' => l('« Back to School Management', 'ntlp/school/management/users'),
        '#prefix' => '<div style="float:left;padding:0px;"><div class="BoldrBrandCrumb">',
        '#suffix' => '</div>',
    );
    $form['heading'] = array(
//            '#type' => 'markup',
        '#value' => t('Bulk-Create Users'),
        '#prefix' => '<h2 style="color:#3370A7;margin-top:0px;font-size:20px;">',
        '#suffix' => '</h2></div><br clear="all" />',
    );

    $form['main'] = array(
//            '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => $HTMLform_school_setting_bluebox_start,
        '#suffix' => $HTMLform_school_setting_bluebox_end,
    );

    $form['main']['bulkUserStatus'] = array(
//            '#type' => 'item',
        '#value' => '',
        '#prefix' => '<div id="bulkUserStatus" style="display: none; padding: 3px; margin-bottom: 10px;">',
        '#suffix' => '</div>',
    );

    $form['main']['buil_an_import_file'] = array(
//            '#type' => 'item',
        '#value' => t("1. &nbsp;&nbsp; Build an import file"),
        '#prefix' => '<tr><td><div><h3 style="color:#3370A7;"> ',
        '#suffix' => '</h3></div>',
    );

    $form['main']['buil_an_import_file_desc'] = array(
//            '#type' => 'item',
        '#value' => t("You'll need to create a CSV (comma separated value) file with the user account information. Spreadsheet programs  <br /> including Google Docs and Microsoft Excel make it easy to create and edit CSV file." . l('Click here', ' ') . " for further instructions."),
        '#prefix' => '<div style="padding-left:25px">',
        '#suffix' => '</div></td></tr>',
    );


    $form['main']['select_additional_option'] = array(
//            '#type' => 'item',
        '#value' => t("2. &nbsp;&nbsp; Select Additional Options"),
        '#prefix' => '<tr><td><div><h3 style="color:#3370A7;"> ',
        '#suffix' => '</h3></div>',
    );


    $form['main']['update_existing_account'] = array(
        '#type' => 'checkbox',
        '#title' => t('If user already exists, update existing account'),
        '#prefix' => '<div style="padding-left:25px">',
        '#suffix' => '</div>',
        '#id' => 'user_check',
    );
    $form['main']['change_password'] = array(
        '#type' => 'checkbox',
        '#title' => t('Require new users to change password on first login'),
        '#prefix' => '<div style="padding-left:25px">',
        '#suffix' => '</div></td></tr>',
        '#id' => 'change_password',
    );

    $form['main']['upload_your_file'] = array(
//            '#type' => 'item',
        '#value' => t("3. &nbsp;&nbsp; Upload Your file"),
        '#prefix' => '<tr><td><div><h3 style="color:#3370A7;"> ',
        '#suffix' => '</h3></div>',
    );

    $form['main']['upload_your_file_desc'] = array(
//            '#type' => 'item',
        '#value' => t("Once you've created your CSV file according to the above instructions, select and upload it here:"),
        '#prefix' => '<div style="padding-left:25px">',
        '#suffix' => '</div></td></tr>',
    );

    $form['main']['file_fid'] = array(
        '#type' => 'hidden',
        '#id' => 'file_fid',
    );

    $form['#fileupload_embed'] = true;
    $form['main']['upload_control'] = array(
        '#prefix' => '<tr class="uploadFiles"><td>',
        '#suffix' => '</td></tr>',
    );

    fileupload_get_control($form, $form_state, array('main', 'upload_control'));

    $form['main']['progrssbar'] = array(
//            '#type' => 'markup',
        '#value' => '<div id="progressbar-users" style="height: 25px; width: 100%; position: relative;">
                            <div style="color: #0F7027; font-weight: bold; position: absolute; top: 15%; right: 35%;">
                                <span id="progressbar-users-processed"></span>
                            </div>
                            <div id="progressbar-users-percent" style="font-weight: bold; position: absolute; top: 15%; right: 1%;">0%</div>
                        </div>',
        '#prefix' => '<tr><td id="progressbar-users-row" style="display: none;">',
        '#suffix' => '</td></tr>',
//             '#attributes' => array('onclick' => '_get_csv_record(); return false;','class' => 'SubmitAssignmentBtn'),
//            '#attributes' => array('class' => 'SubmitAssignmentBtn'),
    );

    $form['main']['create_user'] = array(
//            '#type' => 'item',
        '#value' => l('Create Users', '', array('attributes' => array('onclick' => '_get_csv_record(); return false;', 'class' => 'SubmitAssignmentBtn'))),
        '#prefix' => '<tr id = "hide_button"><td>',
        '#suffix' => '</td></tr>',
//             '#attributes' => array('onclick' => '_get_csv_record(); return false;','class' => 'SubmitAssignmentBtn'),
//            '#attributes' => array('class' => 'SubmitAssignmentBtn'),
    );




//    $form['main']['create_user']= array(
//            '#type' => 'button',
//            '#value' => t('Create Users'),
//            '#executes_submit_callback' => TRUE,
//            '#submit' => array('bulk_user_save'),
//            '#attributes' => array('class' => 'SubmitAssignmentBtn'),
//            '#prefix' => '<tr><td>',
//            '#suffix' => '</td></tr>',
//    );

    return $form;
}

/* Bulk Create Users Form End  END */




/* Change Password Form in First Login START */

function change_login_password() {
    global $user;

    include('school_management.template.inc');

    _user_password_dynamic_validation();

    $form['title'] = array(
        '#type' => 'item',
        '#value' => t('New User Login Password Change'),
        '#prefix' => '<h2 style="color:#3370A7;margin-top:0px;font-size:20px;">',
        '#suffix' => '</h2></div>',
    );


    $form['save'] = array(
        '#type' => 'button',
        '#value' => t('Save'),
        '#executes_submit_callback' => TRUE,
        '#submit' => array('change_login_password_submit'),
        '#attributes' => array('class' => 'SaveBtn'),
        '#prefix' => '<div style="float:right">',
        '#suffix' => '</div><br clear="all" />',
    );


    $form['main'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => $HTMLform_school_setting_bluebox_start,
        '#suffix' => $HTMLform_school_setting_bluebox_end,
    );

    $form['main']['user_uid'] = array(
        '#type' => 'hidden',
        '#id' => 'user_id',
        '#value' => $user->uid,
    );


    $form['main']['old_password'] = array(
        '#type' => 'password',
        '#required' => TRUE,
        '#size' => 25,
        '#prefix' => '<th style="width:160px">' . t('Old Password*') . '</th><td colspan="3">',
        '#suffix' => '</td></tr>',
    );

    $form['main']['password'] = array(
        '#type' => 'password_confirm',
        '#required' => TRUE,
        '#size' => 25,
        '#prefix' => '<th style="width:160px">' . t('Password*') . '</th><td colspan="3">',
        '#suffix' => '</td></tr>',
    );

    return $form;
}

function change_login_password_validate(&$form, &$form_state) {
    global $user;

    if ($form_state['clicked_button']['#post']['main']['password']['pass1'] == '' && $form_state['clicked_button']['#post']['main']['password']['pass2'] == '') {
        form_set_error('', 'New password cannot be empty!');
    } elseif (strlen($form_state['clicked_button']['#post']['main']['password']['pass1']) < MIN_PASSWORD_LENGTH || strlen($form_state['clicked_button']['#post']['main']['password']['pass2']) < MIN_PASSWORD_LENGTH) {
        form_set_error('', 'Password should be at least eight characters long');
    } else {
        $data = unserialize(db_result(db_query('SELECT data FROM {users} WHERE uid = %d', $user->uid)));
        //    drupal_set_message('Ldap:'.$data['ldap_authentified']);
        //    drupal_set_message('Auth:'._ldapauth_auth($user->name, $form['#post']['old_password']));
        if (isset($data['ldap_authentified']) && $data['ldap_authentified'] == 1) {
            $dn = _ldapauth_auth($user->name, $form_state['clicked_button']['#post']['main']['old_password']);
            if (!$dn) {
                form_set_error('old_password', 'Old password not verified.');
            }
        } else {
//            echo 'old password '. $user->pass .' given old paswd '.md5($form_state['clicked_button']['#post']['main']['old_password']);
            if (md5($form_state['clicked_button']['#post']['main']['old_password']) != $user->pass) {
                form_set_error('old_password', 'Old password not match.');
            }
        }
    }
}

function change_login_password_submit($form, &$form_state) {
    ntlp_change_password_current_user($form_state['clicked_button']['#post']['main']['password']['pass1']);
    $form_state['storage']['success'] = TRUE;

    drupal_set_message("Password Changed Successfully");
    drupal_goto('ntlp/home');
}

//function change_login_password_submit($form, $form_state) {
//    global $user;
//
//    $uid = $user->uid;
//
//    $old_password = md5($form_state['clicked_button']['#post']['main']['old_password']);
//    $new_password = md5($form_state['clicked_button']['#post']['main']['password']['pass1']);
//
//    $rs = db_query("select uid from {users} where uid  = %d and pass = '%s'",$uid, $old_password);
//    if($rs->num_rows > 0) {
//        ntlp_change_password_current_user($newpasswd);
////        db_query("UPDATE {users} SET pass = '%s', data = null where uid = %d",$new_password, $uid);
//        db_query("UPDATE {ntlp_user} SET password_change = %d where uid = %d",0, $uid);
//
//        drupal_set_message('Password changed successfully');
//        drupal_goto('ntlp/home');
//    }else {
//
//        drupal_set_message('Old password does not match');
//        drupal_goto('ntlp/new/user/password/change');
//    }
//
//}


/* Combo box Which Gets Courses For school */
function get_courses_for_school() {

    $term_id = check_plain(arg(4));
    $school_nid = check_plain(arg(5));

    $courses = get_myschool_term_courses($school_nid, $term_id);

    $options = '';
    while ($record = db_fetch_array($courses)) {

        $courses_name[$record['nid']] = $record['title'];
        $options .= "<option value=\"{$record['nid']}\">{$record['title']}</option>";
    }


    print($options);
}



/* Merge the name and description in an array for outcomes  in course settings page */

function _merge_name_desc($name_array, $desc_array) {
    $result_array = array();

    foreach ($name_array as $key => $value) {
        if (isset($desc_array[$key])) {
            $result_array[] = array('name' => $value, 'description' => $desc_array[$key]);
        } else {
            $result_array[] = $value;
        }
    }

    return $result_array;
}

function read_cvs_file($form, $fid, $user_exists, $change_pwd) {
    global $base_path;

    $update_count = 0;
    $insert_count = 0;
    $exists_user = 0;
    $school_nid = get_this_school();
    $domain = variable_get('grupal_domain' . $school_nid . '_', '');

    $query = db_query("SELECT filepath from {files} where fid = %d", $fid);
    $file_path_result = db_fetch_object($query);
    $file_path = $_SERVER["DOCUMENT_ROOT"] . $base_path . $file_path_result->filepath;

//    watchdog('filepath', $file_path);

    $row = 1;
    $handle = fopen($file_path, "r");
    $totalRecords = count(file($file_path)) - 1;
    $calls = intval($_GET['calls']);

//    watchdog('filepath', print_r($handle, true));

    $rs = array();
    $processed_user = '';

    while (($user_data = fgets($handle)) !== FALSE) {

//        watchdog("user_data", print_r($user_data, true));
        $records = explode(",", $user_data);

//        watchdog("user_records ", print_r($records, TRUE));
//        watchdog('records ', print_r($records, true));

        if ($row > 1 && $row == ($calls + 1)) { // skip the first row
            //Do basic validation first
            $users_validation = bulk_user_validation_basic($records[0], $records[2], $records[3], $records[4], $records[5], $records[6]);
            if (empty($domain)) {
                $users_validation |= 32;
            }
            $processed_user = "$records[0] $records[1] $records[2]";
            if (!$users_validation) {        // No errors
                //Search user in database to see if it exists
                $user_role = $records[6];
                if ($user_role != NTLP_ROLEID_PARENT) {
                    $user_name = $records[4] . '@' . $domain;
                } else {
                    $user_name = $records[4];
                }

                $setup_google = ($user_role == NTLP_ROLEID_PARENT
                        || $user_role == NTLP_ROLEID_SITEGUEST
                        || $user_role == NTLP_ROLEID_NETWORKGUEST ? FALSE : TRUE);

                $sql = db_query("select u.uid, nu.is_active, nu.grad_year, nu.user_code
                    FROM {users} u, {ntlp_user} nu
                    WHERE u.uid = nu.uid AND name = '%s'", strtolower($user_name));

                if ($sql->num_rows > 0) {
                    $query_result = db_fetch_object($sql);

                    if ($user_exists == 1) {
                        $existingUser = new stdClass();
                        //changes by saif Oct 25 to store the user name in lowercase                        
                        $existingUser->name = strtolower($user_name);
//                        $existingUser->status = $records[7];      //Update is_active instead of this field
                        $existingUser->mail = $records[3];
                        $existingUser->init = $records[3];

                        //Change password only if its not empty
                        if (strlen(trim($records[5])) > 0) {
                            $existingUser->pass = $records[5];
                            //Change password in ldap
                            ntlp_change_password_other_user($records[5], $query_result->uid);
                        }

                        //Change Is_Active status only if its not empty
                        if (strlen(trim($records[7])) > 0) {
                            $user_is_active = $records[7];
                            if ($user_is_active == "x") {
                                $user_is_active = 0;
                                $existingUser->status = 0;          //Inactive
                            } elseif ($user_is_active == 1) {
                                $existingUser->status = 1;          //Active (Enabled)
                            } elseif ($user_is_active == 0) {
                                $user_is_active = 1;
                                $existingUser->status = 0;          //Active (Disabled)
                            } else {
                                $user_is_active = $query_result->is_active;
                            }
                        } else {
                            $user_is_active = $query_result->is_active;
                        }

//                        drupal_set_message("{$user_name} = {$user_is_active}:{$existingUser->status}:{$records[7]}.");
                        //Change Grad Year only if its not empty
                        if (strlen(trim($records[8])) > 0) {
                            if ($records[6] == NTLP_ROLEID_PARENT) {
                                $grade_year = 0;
                            } else {
                                $grade_year = $records[8];
                            }
                        } else {
                            $grade_year = $query_result->grad_year;
                        }

                        //Change User Code only if its not empty
                        if (strlen(trim($records[9])) > 0) {
                            $user_code = $records[9];
                        } else {
                            $user_code = $query_result->user_code;
                        }

//                        watchdog("BulkUpdate", "Updating user ". print_r($records, true));

                        $user = user_save((object) array('uid' => $query_result->uid), (array) $existingUser);

                        db_query("UPDATE {ntlp_user} SET first_name = '%s', middle_name ='%s', last_name = '%s', grad_year = %d, 
                                user_code = %d, is_active = %d, password_change = %d WHERE uid = %d", $records[0], $records[1], $records[2], $grade_year, $user_code, $user_is_active, $change_pwd, $query_result->uid);

                        db_query("UPDATE {users_roles} SET rid = %d  where uid = %d", $records[6], $query_result->uid);

                        //TRIGGER_POINT: UPDATE STUDENT RECORD IN ALL COURSES AND TERMS
                        dw_set_students_all_course_grade_toupdate($query_result->uid);

                        # It will check the User Role if User role is PARENT then it will Insert Parent Child Record
                        if ($records[6] == NTLP_ROLEID_PARENT) {

                            $child_email = explode(';', $records[8]);

//                            watchdog('child_email', print_r($child_email, true));
                            if (!empty($child_email[0])) {
                                db_query("DELETE FROM {ntlp_user_parent} WHERE parent_uid = %d", $query_result->uid);
                                foreach ($child_email as $email) {

                                    $email = trim($email);
                                    $childId = get_parent_child_uid($email);
                                    watchdog('p_stu_email', 'email ' . $email . ' id ' . $childId);

                                    if ($childId) {
                                        db_query("INSERT INTO {ntlp_user_parent} (parent_uid, student_uid) VALUES (%d, %d)", $query_result->uid, $childId);
                                    } else {

                                        drupal_set_message($email . ' does not exists in this school');
                                    }
                                }
                            }
                        }

                        $update_count++;
                    } else {

                        $exists_user++;
                        drupal_set_message('Cannot add an account for ' . $records[3] . '  because an account with this email address already exists in the NETWORK Network');

//                    $bulkStatus['exists'][] = "$records[0] $records[1] $records[2]";
//                    echo 'User &nsbp; '.$records[3].'&nbsp; Already Exits';
                    }
                } else {
                    //Do detail validation, as new user is going to be registered
                    $users_validation = bulk_user_validation($records[3], $records[6], $records[7], $records[5], $records[9], $records[4]);

                    $data = array(
                        'name' => strtolower($user_name),
                        'pass' => $records[5],
                        'status' => $records[7],
                        'mail' => $records[3],
                        'init' => $records[3],
                        'ntlp_schools' => $school_nid,
                        //Custom fields - used by Grupal
                        'setup_google' => $setup_google,
                        'firstname' => $records[0],
                        'lastname' => $records[2],
                        'status_google' => FALSE,
                    );

                    $user = user_save(null, $data);

                    //Return if Google Apps returns an error while creating the account.
//                    if ($setup_google && $user->status_google == FALSE) {
//                        $users_validation |= 64;
//                        if (!empty($user->uid)) {
//                            db_query("DELETE FROM {users} WHERE uid = %d", $user->uid);
//                            db_query("DELETE FROM {ntlp_user} WHERE uid = %d", $user->uid);
//                        }
//
//                    } else {
                    if ($records[6] == NTLP_ROLEID_PARENT) {
                        $grade_year = 0;
                    } else {
                        $grade_year = $records[8];
                    }


                    if ($user && intval($user->uid) > 0) {
                        //When a validation error occurs, record is not created in ntlp_user table
                        //We need to create a record, if its not there
                        $rs = db_query("select uid from {ntlp_user} where uid = %d", $user->uid);
                        if ($rs->num_rows > 0) {


                            db_query("UPDATE {ntlp_user} SET first_name = '%s' , middle_name = '%s' , last_name = '%s' ,
                                        grad_year = %d, user_code =  %d, password_change = %d
                                        WHERE uid = %d  ", $records[0], $records[1], $records[2], $grade_year, $records[9], $change_pwd, $user->uid);
                        } else {
                            db_query("INSERT INTO {ntlp_user} (uid, school_nid, first_name, middle_name, last_name, grad_year,
                                        user_code, password_change, is_active)
                                        VALUES (%d, %d, '%s', '%s', '%s', %d,
                                        '%s', %d, %d, 1) ", $user->uid, $school_nid, $records[0], $records[1], $records[2], $grade_year, $records[9], $change_pwd);
                        }
                    } else {
                        //                    $bulkStatus['error'][] = "$records[0] $records[1] $records[2]";
                    }

                    if (isset($school_nid)) {

                        db_query("INSERT INTO {users_roles} (uid,rid) VALUES (%d, %d)", $user->uid, $records[6]);
                    }


                    # Insert Parent Child Record
                    if ($records[6] == NTLP_ROLEID_PARENT) {

                        $child_email = explode(';', $records[8]);

                        if (!empty($child_email[0])) {

                            foreach ($child_email as $email) {

                                $email = trim($email);
                                $childId = get_parent_child_uid($email);
                                if ($childId) {

//                                    watchdog('school admin', 'inside if' . print_r($childId, true));

                                    db_query("INSERT INTO {ntlp_user_parent} (parent_uid, student_uid) VALUES (%d, %d)", $user->uid, $childId);
                                } else {
                                    drupal_set_message($email . ' does not exists in this school');
                                }
                            }
                        }
                    }

                    $insert_count++;
//                drupal_set_message($row. ' Users created successfully');
                }
            }

            //Return error code, if any validation failed or exception occurred
            if ($users_validation) {

                $errors = array();
                $errors_type = array(
                    'Email not valid' => 1,
                    'Password not valid' => 2,
                    'Role not valid' => 4,
                    'Username not valid' => 8,
                    'User Code not valid' => 16,
                    'School domain not found' => 32,
                    'Error using Google Apps' => 64,
                );

                foreach ($errors_type as $msg => $err) {
                    if ($err & $users_validation) {
                        $errors[] = $msg;
                    }
                }
                if ($errors) {
                    drupal_set_message('<b>' . $records[3] . '</b><ul><li>' . implode('</li><li>', $errors) . '</li></ul><br />');
                }
            }
            break;
        }
        $row++;
    }

//    if($update_count != 0) {
//
//        drupal_set_message( $update_count . '  users updated successfully');
//    }
//    if($insert_count != 0) {
//
//        drupal_set_message( $insert_count . '  users created successfully');
//    }
    if ($calls > $totalRecords) {
        //drupal_set_message('Total:'.$totalRecords.', Done:'.$calls);
        drupal_set_message('Bulk user import completed successfully. Processed ' . ($calls - 1) . '  user(s) from the uploaded file.  ');
    }

    $row -= 2;
    echo json_encode(array(
        'done' => $row,
        'total' => $totalRecords,
        'error' => !empty($users_validation),
        'processedUser' => $processed_user,
    ));

    exit();
}

function download_user_list($school_id) {

    $student_list = get_school_student_list($school_id);
    $csv_output .= "Student Export ID, First Name, Middle Name, Last Name, Email, Role, Grad Year, School Export ID, Status";
    $csv_output .= "\n";

    $ids_looped = array();
    while ($record = db_fetch_array($student_list)) {
        $csv_output .= $record['user_code'] . ',' . $record['first_name'] . ',' . $record['middle_name'] . ',' . $record['last_name'] . ',' . $record['mail'] . ',' . $record['name'] . ',' . $record['grad_year'] . ',' . $record['export_id'] . ',' . ($record['status'] == 1 ? 'Active' : 'Inactive');
        $csv_output .= "\n";
    }

    drupal_set_header('Content-Type: text/csv');
    drupal_set_header('Content-disposition: attachment; filename="' . date("Y-m-d") . '.csv"');
    print $csv_output;
}

function can_change_userstatus($status, $school_id, $ids) {
    $user_ids = rtrim($ids, ',');
    $message = "$status,$school_id,$ids";

    if ($status == 0) {
        //Are any of the selected users enrolled in any active courses for the active school year?
        $urs = db_query("SELECT DISTINCT c.nid
            FROM {ntlp_course} c, {ntlp_course_user} cu, {ntlp_course_term} ct, {ntlp_school_term} st, {ntlp_school} s
            WHERE c.nid = cu.course_nid AND cu.user_uid IN (%s)
            AND c.school_nid = s.nid AND c.nid = ct.course_nid AND ct.school_term_tid = st.school_term_tid
            AND s.current_school_year = st.school_year AND c.availability = 'S'", $user_ids);
        if ($urs->num_rows > 0) {
            $message = "!DROP!Some of the selected users are currently enrolled in active Courses. Do you want
                        to automatically “drop” these users from all of their active Courses AND set them to
                        Inactive? Note: This means that these users will no longer be able to login, and that
                        the users cannot be added to new courses or groups. All of their previous data such
                        as grades is archived for staff to access, but new grades cannot be entered.";
        } else {
            $message = "!OK!The selected users accounts are about to be inactivated. This means that
                        these users will no longer be able to login, and that the users cannot be
                        added to new courses or groups. All of their previous data such as grades is
                        archived for staff to access, but new grades cannot be entered. Are you sure
                        you want to inactivate these accounts?";
        }
    }
    print $message;
//    exit;
}

function change_user_status($status, $school_id, $ids) {

    $user_ids = rtrim($ids, ',');
    $ids_array = explode(',', $user_ids);
    $error = false;
    $message = "";
    $record = "";

    foreach ($ids_array as $id) {
        $active = 0;
        $enabled = 0;
        $doit = false;
        if ($status == 1) { //Active - Enabled
            $active = 1;
            $enabled = 1;
//            //Only change status if previous status is "Active - Disabled"
//            $urs = db_query("SELECT n.is_active, u.status FROM {ntlp_user} n, {users} u
//                WHERE u.uid = n.uid AND uid = %d ", $id);
//            if ($urs->num_rows > 0) {
//                $urec = db_fetch_object($urs);
//                if ($urec->is_active == 1 && $urec->status == 0) {
            $doit = true;
            $record .= "Changing Status of $id to Active(Enabled).<br>";
//                }
//            }
        } elseif ($status == 2) { //Active - Disabled
            $active = 1;
            $enabled = 0;
            //Only change status if previous status is "Active - Enabled"
            $urs = db_query("SELECT n.is_active, u.status FROM {ntlp_user} n, {users} u
                WHERE u.uid = n.uid AND u.uid = %d ", $id);
            if ($urs->num_rows > 0) {
                $urec = db_fetch_object($urs);
                if ($urec->is_active == 1 && $urec->status == 1) {
                    $doit = true;
                    $record .= "Changing Status of $id to Active(Disabled).<br>";
                } elseif ($urec->is_active == 0 && $urec->status == 0) {        //If Inactive
                    $error = true;
                    $message = "!ERROR!One or more of the selected users are Inactive. Only Active (Enabled) user accounts can be disabled. Please re-select users that meet this criteria.";
                    break;
                }
            }
        } elseif ($status == 0) { //Inactive
            $active = 0;
            $enabled = 0;
            $doit = true;
            //Drop user from all Active courses of the CURRENT school year
            $urs = db_query("SELECT DISTINCT c.nid
                        FROM {ntlp_course} c, {ntlp_course_user} cu, {ntlp_course_term} ct, {ntlp_school_term} st, {ntlp_school} s
                WHERE c.nid = cu.course_nid AND cu.user_uid = %d
                AND c.school_nid = s.nid AND c.nid = ct.course_nid AND ct.school_term_tid = st.school_term_tid
                AND s.current_school_year = st.school_year AND c.availability = 'S'", $id);
            $record .= "User $id enrolled in $urs->num_rows classes<br>";
            if ($urs->num_rows > 0) {
                while ($urec = db_fetch_object($urs)) {
                    drop_users($urec->nid, $id, false, false);
                    $record .= "Dropping $id from class $urec->nid <br>";
                }
            }

            //Make all Parent accounts Inactive, if this is the only child
            //Get all parents of this user
            $urs = db_query("SELECT count(*) cnt, parent_uid FROM {ntlp_user_parent} WHERE parent_uid IN
                (SELECT parent_uid FROM {ntlp_user_parent} WHERE student_uid = %d)
                GROUP BY parent_uid HAVING cnt = 1", $id);
            if ($urs->num_rows > 0) {
                while ($urec = db_fetch_object($urs)) {
                    //Make all Parent accounts Inactive, if this is the only child
                    //Active/Inactive PARENT User
                    db_query("UPDATE {ntlp_user} SET is_active = %d WHERE uid = %d ", $active, $urec->parent_uid);
                    //Enable/Disable PARENT User
                    db_query("UPDATE {users} SET status = %d WHERE uid = %d ", $enabled, $urec->parent_uid);
                }
            }
        }
        if ($doit) {
            //Active/Inactive User
            db_query("UPDATE {ntlp_user} SET is_active = %d WHERE uid = %d ", $active, $id);
            //Enable/Disable User
            db_query("UPDATE {users} SET status = %d WHERE uid = %d ", $enabled, $id);
        }
    }
    if ($error) {
        print $message;
        exit;
    } else {
        $record = "!OK!" . $record;
//        $record = manage_users($school_id);       //User list is now refreshed by calling filter's click function
        print $record;
        exit;
    }
}

function change_user_role($role, $school_id, $ids) {

    $user_ids = rtrim($ids, ',');

    $ids_array = explode(',', $user_ids);

    foreach ($ids_array as $id) {

        $user_role = get_user_role($id);

        if ($user_role->rid == NTLP_ROLEID_STUDENT) {

            echo 'Error:Cannot Change ' . $user_role->name . ' role';
//            $record = manage_users($school_id);
//            print $record;
            exit;
        } elseif ($user_role->rid == NTLP_ROLEID_PARENT) {

            echo 'Error:Cannot Change ' . $user_role->name . ' role';
//            $record = manage_users($school_id);
//            print $record;
            exit;
        } else {
            db_query("UPDATE {users_roles} SET rid = %d where uid = %d ", $role, $id);
        }
    }

    $record = manage_users($school_id);

    print $record;
    exit;
}

function can_delete_users($school_id, $ids) {
    $user_ids = rtrim($ids, ',');
    $message = "The selected users have data in courses, groups, or other parts of the application, so you cannot remove these users. Only user accounts with no activity can be removed. Select different accounts or consider making the accounts Inactive instead.";
    $can_delete = 0;

    //Check if user is enrolled in any course
    $crs = db_query("SELECT cu.course_nid
        FROM {ntlp_course_user} cu
        WHERE cu.user_uid IN (%s)", $user_ids);
    if ($crs->num_rows == 0) {
        //Check if user is member of any group
        $crs = db_query("SELECT gu.nid
            FROM {og_uid} gu
            WHERE gu.uid IN (%s)", $user_ids);
        if ($crs->num_rows == 0) {
            //Check if user is parent of one or more students
            $crs = db_query("SELECT p.student_uid
                FROM {ntlp_user_parent} p
                WHERE p.parent_uid IN (%s)", $user_ids);
            if ($crs->num_rows > 0) {
                $message = "The selected users have associated students, so you cannot remove these users. Select different accounts or consider making the accounts Inactive instead.";
            } else {
                //Check if user has created a node, this will just be a Warning
                $crs = db_query("SELECT n.uid
                    FROM {node} n
                    WHERE n.uid IN (%s)", $user_ids);

                $message = "You are about to permanently remove these accounts. You will not be able to recover these accounts once removed. Are you sure you want to do this?";
                //YES WE CAN DELETE
                $can_delete = 1;
                if ($crs->num_rows > 0) {
                    $message .= "<br /><br /><strong>Warning!</strong> Some of these users have created Posts or Resources.";
                }
            }
        }
    }

    drupal_json(array('canDelete' => $can_delete,
        'message' => $message,
    ));
}

function confirm_delete_users($school_id, $ids) {
    $user_ids = rtrim($ids, ',');
    $message = "An error occurred while removing these users, please try again!";
    $success = 0;

    //Check if these users do really belong to this school
    $crs = db_query("SELECT nu.uid FROM {ntlp_user} nu WHERE nu.school_nid = %d AND nu.uid IN (%s)", $school_id, $user_ids);
    if ($crs->num_rows > 0) {
        //Remove from Drupal users table
        $rs = db_query("DELETE FROM {users} WHERE uid IN
            (SELECT nu.uid FROM {ntlp_user} nu WHERE nu.school_nid = %d AND nu.uid IN (%s))", $school_id, $user_ids);
        $rs = db_query("DELETE FROM {users_roles} WHERE uid IN
            (SELECT nu.uid FROM {ntlp_user} nu WHERE nu.school_nid = %d AND nu.uid IN (%s))", $school_id, $user_ids);

        //Remove from NTLP User table
        $rs = db_query("DELETE FROM {ntlp_user} WHERE school_nid = %d AND uid IN (%s)", $school_id, $user_ids);

        $message = "User accounts successfully removed.";
        $success = 1;
        drupal_set_message($message);
    }

    drupal_json(array('success' => $success,
        'message' => $message,
    ));
}

//function grupal_cleanname($name) {
//    return eregi_replace("[^a-z0-9\.]", "", strtolower($name));
//} // function grupal_cleanname
