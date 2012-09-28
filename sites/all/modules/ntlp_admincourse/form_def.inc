<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'ui_tools');
module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');

/**
 * Form controller
 */
function course_form_controller($form_state) {
    global $user;
    // Add the client-side behaviors for the examples.
    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_admincourse') . '/ntlp_admincourse.js');
    drupal_add_css(drupal_get_path('theme', 'Boldr') . '/StylishSelect/stylish-select.css', 'module', 'all', FALSE);
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/externalFile.js');
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/jquery.stylish-select.js');
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/jquery.ui.all.css', 'module', 'all', FALSE);
    include('ntlp_admincourse.template.inc');

    $course_nid = 0;
    $school_nid = 0;
    $course_school_year = 0;
    $form = array();
    $mode = check_plain(arg(2));        //new or edit
    $tab = 'settings';
    
    if (isset($form_state['values'])) {
//        watchdog('info', 'Course Form Controller view state:<br />'.print_r($form_state, true));
        $view_state = $form_state['values']['op'];
        $course_nid = $form_state['values']['course_node'];
        $school_nid = $form_state['values']['school_node'];
        $course_school_year = $form_state['values']['main']['form']['course_school_year'];
    } else {
        set_item_url_handler('Course Setting');
    }
    if (isset($mode)) {
        if ($mode == 'edit') {
            $course_nid = check_plain(arg(3));      //Get course nid from url
            $tab = check_plain(arg(4));

            $user_role = check_user_permission($course_nid);
            $rec = get_course($course_nid);

            /* show last saved status */
            $status_html = '<span style="float: left; padding: 5px; margin-left: 160px; margin-top: 5px; margin-bottom: 8px; font-size: 12px; background-color: rgb(239, 239, 222);">';
            $status_html .=' Last saved ' . get_tz_course($school_nid, DATE_FORMAT_LAST_SAVED, $rec->changed);
            $status_html .= '</span>';

            if (!isset($tab) || empty($tab)) {
                $tab = 'settings';
            }

            if ($user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_NETWORKMANAGER
                    || $user_role == NTLP_ROLEID_NETWORKSTAFF || $user_role == NTLP_ROLEID_SITEADMIN
                    || $user_role == NTLP_ROLEID_SCHOOLLEADER) {

                if (!empty($course_nid)) {
                    $rec = get_course($course_nid);
                    if ($rec == false) {
                        drupal_set_message("Course not found, please contact system administrator");
                        return;
                    }
                    $school_nid = $rec->school_nid;
                    $course_school_year = $rec->school_year;
//                    print_r($rec);
                }
            } else {
                drupal_set_message("You are not Allowed to Edit This Course, Please contact system administrator", 'error');
                drupal_goto("ntlp/goback");
            }
        } else {
            //Its only at this point we get "current" school
            //In other tabs we must take course's school and not the current school
            if (isset($user->roles[NTLP_ROLEID_NETWORKMANAGER]) || isset($user->roles[NTLP_ROLEID_NETWORKSTAFF])) {
                $school_nid = get_this_school();    //get_user_school($user->uid);
            } else {
                $school_nid = get_user_school($user->uid);
            }
        }
    }
    // load school record
    $rec_school = get_school_record($school_nid);
    
    // get school current selected year 
    if ($course_school_year == 0) {
        $course_school_year = get_school_year($school_nid);
    }

    $form['main'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => $HTMLform_course_setting_start,
        '#suffix' => $HTMLform_course_setting_end,
    );
    $form['main']['course_node'] = array(
        '#type' => 'hidden',
        '#value' => $course_nid,
    );
    $form['main']['school_node'] = array(
        '#type' => 'hidden',
        '#value' => $school_nid,
    );
    $form['main']['course_school_year'] = array(
        '#type' => 'hidden',
        '#value' => $course_school_year,
    );

    if ($tab == 'settings' || $tab == 'grades' || $tab == 'members') {
        $form['main']['save'] = array(
            '#type' => 'submit',
            '#value' => 'Save',
            '#prefix' => '<td><div style="margin-bottom:5px"><span style="padding-bottom: 8px;padding-top: 8px;float: left;position: relative;color:#3570aa;font-size:18px;">' . ($mode == 'edit' ? 'Edit Course Settings' : 'Create Course') . '</span></div></td><td><table cellpadding="0" cellspacing="0" width="100%"><tr><td></td><td align="right"><div class="clearfix" style="width: 100%;">'.$status_html.'<div style="float: right;width: 235px;padding-top: 5px;"><span style="float:left;padding-right: 5px;">',
            '#suffix' => '</span>',
            '#submit' => array('course_form_submit_' . $tab),
            '#validate' => array('course_form_validate_' . $tab),
            '#attributes' => array('class' => 'SaveBtn'),
        );

        $form['main']['save_close'] = array(
            '#type' => 'submit',
            '#value' => 'Save & Close',
            '#prefix' => '<span style="float:left;">',
            '#suffix' => '</span>',
            '#submit' => array('course_form_submit_' . $tab),
            '#validate' => array('course_form_validate_' . $tab),
            '#attributes' => array('class' => 'AddOutComesBtn', 'style'=> 'margin-top: 0px !important;'),
        );

        $form['main']['cancel'] = array(
            '#type' => 'item',
            '#value' => '<input type="button"  onclick="window.location = \'?q=ntlp/goback\'" class="CancelBtn" value="' . t('Cancel') . '" />',
            '#prefix' => '<span style="float:right;">',
            '#suffix' => '</span></div></div></td></table></td></tr><tr>',
        );
    } else {
        $form['main']['save'] = array(
            '#type' => 'item',
            '#value' => $status_html,
            '#prefix' => '<td><div style="margin-bottom:5px"><span style="padding-bottom: 8px;padding-top: 8px;float: left;position: relative;color:#3570aa;font-size:18px;">' . ($mode == 'edit' ? 'Edit Course Settings' : 'Create Course') . '</span></div></td><td><table cellpadding="0" cellspacing="0" width="100%"><tr><td></td><td align="right"><div style="margin-left:248px;">',
            '#suffix' => '</div></td></table></td></tr><tr>',
        );
    }
    add_tabs($course_nid, $form['main'], $tab);

    $form['main']['form_start'] = array(
        '#type' => 'markup',
        '#value' => $HTMLform_course_setting_form_start
    );

    //yellow bar goes here 
    if ($tab == 'members') {


        $form['main']['form_start']['self_enroll'] = array(
            '#prefix' => $HTMLform_course_student_self_enroll_bar_start, //YELLOW box start
            '#suffix' => $HTMLform_course_student_self_enroll_bar_end, //YELLOW box end
        );

        $form['main']['form_start']['self_enroll']['student_self_enroll'] = array(
            '#type' => 'checkbox',
            '#prefix' => '<div style="height:17px; "><div style="float:left; width:30px;">',
            '#suffix' => '</div><div style="float: left; width: 200px; margin-top: 3px;">' . t('Enable Self-Enrollment') . '</div></div>',
            '#value' => $rec->allow_self_enroll,
            '#attributes' => array('onclick' => 'allow_self_enroll(' . $course_nid . ')'),
            '#id' => 'student_self_enroll',
        );
    }
    //yellow bar end

    if ($tab != 'grades')
        $form['main']['form_start0'] = array(
            '#type' => 'markup',
            '#value' => $HTMLform_course_setting_bluebox_start
        );

    $form['main']['form'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
//            '#prefix' => $HTMLform_course_setting_form_start,
//            '#suffix' => $HTMLform_course_setting_form_end,
    );
    if ($course_nid != 0) {
        $form['#action'] = url('ntlp/courses/edit/' . $course_nid . '/' . $tab);
    } else {
        $form['#action'] = url('ntlp/admincourse/new');
    }
//    watchdog('school_year', print_r($form_state['values'], true));
    if (isset($view_state)) {
        $course_nid = $form_state['values']['course_node'];
        course_form_tab3_members($form_state, $form['main']['form'], $school_nid, $course_nid, $course_school_year, $rec, $rec_school, true);
    } else {
        if ($tab == 'settings') {
            course_form_tab1_settings($form_state, $form['main']['form'], $school_nid, $course_nid, $course_school_year, $rec, $rec_school);
        } elseif ($tab == 'grades') {
            course_form_tab2_grades($form_state, $form['main']['form'], $school_nid, $course_nid, $course_school_year, $rec, $rec_school);
        } elseif ($tab == 'members') {
            course_form_tab3_members($form_state, $form['main']['form'], $school_nid, $course_nid, $course_school_year, $rec, $rec_school);
        } elseif ($tab == 'guestacs') {
            course_form_tab4_guestacs($form_state, $form['main']['form'], $school_nid, $course_nid, $course_school_year, $rec, $rec_school);
        }
    }
    if ($tab != 'grades')
        $form['main']['form_end0'] = array(
            '#type' => 'markup',
            '#value' => $HTMLform_course_setting_bluebox_end);

    $form['main']['form_end'] = array(
        '#type' => 'markup',
        '#value' => $HTMLform_course_setting_form_end);

    if ($tab == 'members') {
        $form['main']['form']['endrow'] = array(
            '#type' => 'markup',
            '#prefix' => '</table>
                                <table style="width: 100%; border-left:1px solid #9ECCE9;border-right:1px solid #9ECCE9" cellpadding="8" cellspacing="0"><tr>
                                <td>',
            '#suffix' => '
                                </td>
                              </tr>',
        );

        if (intval($rec->period) == 0) {
            $periodoption = array('-1' => '&nbsp;', 0 => '*', 1 => '1', 2 => '2', 3 => '3', 4 => '4', 5 => '5', 6 => '6', 7 => '7', 8 => '8');
            $form['main']['form']['endrow']['period'] = array(
                '#type' => 'select',
                '#options' => $periodoption,
                '#prefix' => '<h3 style="margin:0px;color:#3570AA">Mass Action</h3>
                                    <div style="padding:5px; border:1px solid #3570AA;float:left;width:447px">
                                        <div>
                                            <span style="float: left; margin-top: 5px; margin-right: 1em;">Assign Period</span>
                                            <span style="margin-right:1em;width:40px;" class="comboboxBgLeft" id="periodComboBox">',
                '#suffix' => '      </span>
                                        </div>',
                '#attributes' => array('class' => 'my-dropdown'),
            );
        }
        $form['main']['form']['endrow']['change_status'] = array(
            '#type' => 'select',
            '#options' => array(
                'none' => '&nbsp;',
                'drop' => 'Drop',
                'reenroll' => 'Re-Enroll',
                'remove' => 'Remove'
            ),
            '#prefix' => '  <div>
                                    <span style="float: left; margin-top: 5px;margin-left:20px; margin-right: 1em;">Change Status</span>
                                    <span id="actionsComboBox" style="margin-right:1em;width:75px;" class="comboboxBgLeft">',
            '#suffix' => '      </span>
                                </div>
                             </div>',
            '#attributes' => array('class' => 'my-dropdown'),
        );

        $form['main']['form']['endrow']['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Submit',
            '#submit' => array('course_form_submit_members'),
            '#prefix' => '<div style="float: right;padding-top:5px">',
            '#suffix' => '</div>
                    <div id="dialog-confirm-sbmtFrm" style="display: none;" title="Are you sure?">
                        <span class="msg">Are you sure you want to submit the form?</span>
                    </div>',
            '#attributes' => array('class' => 'AddOutComesBtn', 'onclick' => 'confirmFormSubmit();return false;', 'onmousedown' => "this.className='AddOutComesBtnClicked'", 'onmouseup' => "this.className='AddOutComesBtn'"),
        );
    }
    return $form;
}

/**
 * Form definition
 */
function course_form_tab1_settings($form_state, &$form, $school_nid, $course_nid, $course_school_year, $rec, $rec_school) {
    global $user;

    $rs_user = get_user($user->uid);
    $subject_tid1 = 0;
    $subject_tid2 = 0;
    $course_owner_id = $user->uid;
    $course_primary_teacherid = $user->uid;
    $course_primary_teachername = $rs_user->last_name . ', ' . $rs_user->first_name;

    if (!empty($course_nid) && isset($rec)) {
        if ($rec != false) {
            //Load node to get subject taxonomy data
            $course_node = node_load($course_nid);
            $course_owner_id = $course_node->uid;
            $course_subjects = taxonomy_node_get_terms($course_node);

            foreach ($course_subjects as $cs) {
                if ($cs->vid == NTLP_VOCABULARY_SUBJECTS) {
                    if (empty($subject_tid1))
                        $subject_tid1 = $cs->tid;
                    elseif (empty($subject_tid2))
                        $subject_tid2 = $cs->tid;
                }
            }
            $course_full_title = $rec->title;
            $course_short_title = $rec->course_short_title;
            $course_active = $rec->availability;
            $course_description = $rec->body;
//            $course_term = $rec->school_term_tid;         // We should pick the active course term from school
            $course_period = $rec->period;
            $is_advisory_course = $rec->is_advisory_course;
            $course_additional_notes = $rec->additional_notes;
            $course_primary_teacherid = 0;
            $course_primary_teachername = '';
            $course_primary_teacherid2 = 0;
            $course_primary_teachername2 = '';
            if ($rs_user = get_user($rec->teacher1_uid)) {
                $course_primary_teacherid = $rec->teacher1_uid;
                $course_primary_teachername = get_instructor_namelink($rec->teacher1_uid, $rs_user->first_name,  $rs_user->last_name);
            }
            if ($rec->teacher1_uid != $rec->teacher2_uid && $rs_user = get_user($rec->teacher2_uid)) {
                $course_primary_teacherid2 = $rec->teacher2_uid;
                $course_primary_teachername2 = get_instructor_namelink($rec->teacher2_uid, $rs_user->first_name,  $rs_user->last_name);
            }
            $course_additional_teacherids = '';
            $course_additional_teachernames = '';

            if ($rec_users = get_course_users($course_nid, NTLP_ROLEID_TEACHER)) {
                while ($row = db_fetch_array($rec_users)) {
                    if ($course_primary_teacherid != $row['uid'] && $course_primary_teacherid2 != $row['uid']) {     //Don't fetch the primary teacher
                        if ($rs_user = get_user($row['uid'])) {
                            $course_additional_teacherids .= ',' . $rs_user->uid;
                            $course_additional_teachernames .=
                                '<div id="' . $row['uid'] . '">
                                    <a onclick="remove_secondary_user($(this),' . $row['uid'] . ',\'pplfndInstructor_' . session_id() . '\');">
                                        <img height="14" width="14" class="down" src="themes/Boldr/Images/DeleteBtn.png"/>
                                    </a>&nbsp; ' .$rs_user->first_name . ' ' . $rs_user->last_name  . '<br/>
                                </div>' ;
                        }
                    }
                }
                if (strlen($course_additional_teacherids) > 1)
                    $course_additional_teacherids = substr($course_additional_teacherids, 1);
            }
//            firep($course_additional_teacherids);
//            firep($course_additional_teachernames);
            if (strlen($course_additional_teacherids) <= 0) {
                $course_additional_teacherids = '0';            //To make the called url proper
            }
        }
    }

    $form['course_full_title'] = array(
        '#type' => 'textfield',
        '#default_value' => $course_full_title,
        '#size' => 30,
        '#prefix' => '<tr><th style="width:160px">' . t('Full Title*') . '</th><td>',
        '#suffix' => '</td>',
    );
    $form['course_heading'] = array(
//            '#type' => 'item',
        '#value' => t('Course ID'),
        '#prefix' => '<th rowspan="2" style="width:250px;padding:5px"><table class="coursesInnertable" width="100%" cellpadding="0" cellspacing="0" border="0"><tr><td style="border:1px dotted #fff;border-collapse:collapse;background-color:#63a2d8;color:#fff;width:50%; text-align:center;height:20px;">',
        '#suffix' => '</td><td style="border:1px dotted #fff;border-collapse:collapse;background-color:#63a2d8;color:#fff;width:50%;text-align:center">' . t('Active*') . '</td></tr>',
    );

    $form['course_id'] = array(
        '#type' => 'item',
        '#value' => empty($course_nid) ? 'n/a' : $course_nid,
        '#prefix' => '<tr><td style="border:1px dotted #fff;border-collapse:collapse;text-align:center;height:44px;color:#000;font-weight:normal">',
        '#suffix' => '</td>',
    );

    $form['course_active'] = array(
        '#type' => 'radios',
        '#options' => array('S' => 'Yes', 'H' => 'No'),
        '#default_value' => empty($course_nid) ? 'S' : $course_active,
        '#prefix' => '<td style="border:1px dotted #fff;border-collapse:collapse; text-align:center">',
        '#suffix' => '</td></tr></table></th></tr>',
    );

    $form['course_short_title'] = array(
        '#type' => 'textfield',
        '#size' => 30,
        '#default_value' => $course_short_title,
        '#prefix' => '<tr><th style="width:160px">' . t('Short Title*') . '</th><td>',
        '#suffix' => '</td></tr>',
    );

    $form['course_description'] = array(
        '#type' => 'textarea',
        '#wysiwyg' => FALSE,
        '#default_value' => $course_description,
        '#resizable' => false,
        '#attributes' => array('style' => 'width:400px;overflow:auto;height:100px'),
        '#prefix' => '<tr><th style="width:160px">' . t('Description') . '<br clear="all" /></th><td colspan="2">',
        '#suffix' => '<span style="font-size:10px;color:#000;">Max characters = 500</span></td></tr>',
    );

    $subjectoption = get_course_subject();
    $subjectoption[0] = 'None';
    $form['course_subject1'] = array(
        '#type' => 'select',
        '#options' => $subjectoption,
        '#default_value' => $subject_tid1,
        '#required' => TRUE,
        '#prefix' => '<tr><th style="width:160px">' . t('Subject*') . '</th><td colspan="2"><div style="float:left;width:150px;" class="comboboxBgLeft">',
        '#suffix' => '</div>',
        '#attributes' => array('class' => 'my-dropdown'),
    );

    $form['course_subject2'] = array(
        '#type' => 'select',
        '#options' => $subjectoption,
        '#default_value' => $subject_tid2,
        '#required' => TRUE,
        '#prefix' => '<div style="float:left;margin-left:1em;width:150px;" class="comboboxBgLeft">',
        '#suffix' => '</div><br/><br/><div style="font-size:10px;color:#000;">At least one subject is required.</div></td></tr>',
        '#attributes' => array('class' => 'my-dropdown'),
    );

    $course_term_inuse = is_any_course_term_used($course_nid);
    if (!$course_term_inuse || $course_nid == 0) {
        $schoolyearoption = _get_all_school_years($school_nid);
        if (empty($course_nid)) {
//            $course_school_year = get_this_school_year();
            $url = url("ntlp/admincourse/new");
        } else {
            $url = url("ntlp/courses/edit/{$course_nid}");
        }
        $form['course_school_year'] = array(
            '#type' => 'select',
            '#options' => $schoolyearoption,
            '#default_value' => $course_school_year,
            '#required' => TRUE,
            '#prefix' => '<tr><th style="width:160px">' . t('School Year*') . '</th><td colspan="2"><span style="width:110px;" class="comboboxBgLeft">',
            '#suffix' => '</span></td></tr>',
//            '#attributes' => array('class' => 'my-dropdown', 'onchange' => "window.location = '" . $url . "&year='+this.value;"),
            '#attributes' => array('class' => 'my-dropdown'),
            '#ahah' => array(
                'path' => ahah_helper_path(array('main', 'form', 'school_term_start', 'school_term_w')),
                'wrapper' => 'school-terms-wrapper',
                'progress' => 'none',
            ),
        );
    } else {
        if (intval($course_school_year) > 0)
            $fullyear = intval($course_school_year) - 1 . ' - ' . $course_school_year;

        $form['course_school_year_heading'] = array(
            '#type' => 'item',
            '#prefix' => '<tr><th style="width:160px">' . t('School Year*') . '</th><td colspan="2">',
            '#suffix' => '</td></tr>',
            '#value' => $fullyear,
        );
    }

    //watchdog('term_set', 'schol_nid ' . $school_nid . ' current_school_year ' . $course_school_year);
    $rs_st = get_school_terms($school_nid, $course_school_year);
    $first_term = '';

    //Get Course Terms - selected from school terms
    $dateformat = "m/d/Y";
    $course_termopts = array();
    if (!empty($course_nid)) {
        $rs_ct = get_course_terms($course_nid);

        while ($rec_ct = db_fetch_array($rs_ct)) {
            if (empty($first_term))
                $first_term = $rec_ct['tid'];
            $course_termopts[$rec_ct['tid']] = $rec_ct['name'];
        }
    }
    $termoption = array();
    $form['school_term_start'] = array(
        '#type' => 'markup',
        '#value' => '<tr class="course_terms_semester"><th style="width:160px">' . t('Course Terms*') . '</th>',
    );

//    watchdog('course_set', print_r($form_state, true));
    $form['school_term_start']['school_term_w'] = array(
        '#type' => 'item',
        '#prefix' => '<td colspan="2" id="school-terms-wrapper">',
        '#suffix' => '</td>',
    );

    $term_found = false;

    while ($rec_st = db_fetch_array($rs_st)) {

        $term_found = true;
        $termoption[$rec_st['tid']] = $rec_st['name'];
        if (empty($first_term))
            $first_term = $rec_st['tid'];

        $course_term_inuse = false;
        if ($course_termopts != null) {
            $courseopt_exist = array_key_exists($rec_st['tid'], $course_termopts);
            if ($courseopt_exist)
                $course_term_inuse = is_course_term_used($course_nid, $rec_st['tid']);
        } else {
            $courseopt_exist = false;
        }

        $form['school_term_start']['school_term_w']['school_term_' . $rec_st['tid']] = array(
            '#type' => 'checkbox',
            '#title' => $rec_st['name'] . ' - (' . get_tz($school_nid, $dateformat, ($rec_st['date_started']))
            . ' - ' . get_tz($school_nid, $dateformat, ($rec_st['date_closed'])) . ')',
            '#default_value' => empty($course_nid) ? '' : $courseopt_exist,
            '#attributes' => $course_term_inuse ? array('disabled' => 'disabled') : array(),
        );

//        watchdog('term_rec', print_r($rec_st, true));
    }
    if (!$term_found) {
        $form['school_term_start']['school_term_w']['school_term_notfound'] = array(
            '#type' => 'markup',
            '#value' => 'No terms are setup for this school year.',
        );
    }

    $form['school_term_start']['school_term_end'] = array(
        '#type' => 'markup',
        '#value' => '</tr>',
    );
    $periodoption = array(0 => ' - ', 1 => '1', 2 => '2', 3 => '3', 4 => '4', 5 => '5', 6 => '6', 7 => '7', 8 => '8');
    $form['course_period'] = array(
        '#type' => 'select',
        '#options' => $periodoption,
        '#default_value' => $course_period,
        '#required' => TRUE,
        '#prefix' => '<tr><th style="width:160px">' . t('Period*') . '</th><td colspan="2"><span style="margin-right:1em;width:40px;" class="comboboxBgLeft">',
        '#suffix' => '</span>',
        '#attributes' => array('class' => 'my-dropdown'),
    );
    $form['course_period_is_multiple'] = array(
        '#type' => 'checkbox',
        '#title' => 'This course includes students across multiple periods',
        '#value' => (isset($course_period) ? ($course_period == 0 ? 1 : 0 ) : 0),
        '#prefix' => '<span>',
        '#suffix' => '</span></td></tr>',
    );

    $form['is_advisory_course'] = array(
        '#type' => 'radios',
        '#options' => array(1 => 'Yes', 0 => 'No'),
        '#default_value' => empty($is_advisory_course) ? 0 : 1,
        '#prefix' => '<tr class="radio_btn_float"><th style="width:160px">' . t('Advisory Course') . '</th><td colspan="2">',
        '#suffix' => '</td></tr>',
    );

    $form['course_primary_instructor_heading'] = array(
        '#type' => 'item',
        '#value' => t('Primary Instructor*<br clear="all" />'),
        '#prefix' => '<tr><th style="width:160px">',
        '#suffix' => '</th>',
    );

    if (!empty($course_primary_teacherid)) {
        $primary_instr_ids = $course_primary_teacherid . (!empty($course_primary_teacherid2) ? ',' . $course_primary_teacherid2 : '');
    } else {
        $primary_instr_ids = $course_primary_teacherid2;
    }

    $instructor_ids = explode(",", $primary_instr_ids);
    if (sizeof($instructor_ids) > 0) {
        foreach ($instructor_ids as $uid) {
            if (!empty($uid)) {
                $rs_user = get_user($uid);

                $course_primary_teachernames .= get_instructor_namelink($uid, $rs_user->first_name,  $rs_user->last_name);
            }
        }
    }

    $form['course_primary_instructor_value'] = array(
        '#type' => 'item',
        '#value' => $course_primary_teachernames,
        '#prefix' => '<td colspan="2"><div id="primary_instructor_view">',
        '#suffix' => '</div>',
    );

    $form['course_owner_id'] = array(
        '#type' => 'hidden',
        '#id' => 'course_owner_id',
        '#value' => $course_owner_id,
    );

    $form['course_primary_instructor_ids'] = array(
        '#type' => 'hidden',
        '#id' => 'course_primary_instructor_ids',
        '#value' => $primary_instr_ids,
    );

    cache_set("pplfndPrimaryInstructor_" . session_id(), $primary_instr_ids, 'ntlp_cache', CACHE_TEMPORARY);
    $form['course_primary_instructor_pplfinder'] = array(
        '#type' => 'item',
        '#value' => dlg('Select Instructors', 'ntlp/people_finder/primaryteacher_multi/' .
                $school_nid . '/' .
                $course_nid . '/' .
                "pplfndPrimaryInstructor_" . session_id() . '/' .
                NTLP_ROLEID_TEACHER. '/1/0/2', 350, 600),
        '#prefix' => '<div class="addInstructorBtn" id="pplfndPrimaryInstructor">',
        '#suffix' => '<span style="font-size:10px;color:#000;">2 Max</span></div></td></tr>',
    );

    $form['course_additional_instructor_heading'] = array(
        '#type' => 'item',
        '#value' => t('Additional Instructors'),
        '#prefix' => '<tr><th style="width:160px;white-space:nowrap;">',
        '#suffix' => '</th>',
    );
    $form['course_additional_instructor_value'] = array(
        '#type' => 'item',
        '#value' => ($course_additional_teachernames == '' ) ? '<span style="color:gray; font-style:italic;">None selected as yet</span>' : $course_additional_teachernames,
        '#prefix' => '<td colspan="2"><div id="additional_instructor_view">',
        '#suffix' => '</div>',
    );
    $form['course_additional_instructor_ids'] = array(
        '#type' => 'hidden',
        '#id' => 'course_additional_instructor_ids',
        '#value' => $course_additional_teacherids,
    );
    cache_set("pplfndInstructor_" . session_id(), $course_additional_teacherids, 'ntlp_cache', CACHE_TEMPORARY);
    $form['course_additional_instructor_pplfinder'] = array(
        '#type' => 'item',
        '#value' => dlg('Select Instructors', 'ntlp/people_finder/teacher_multi/' .
                $school_nid . '/' .
                $course_nid . '/' .
                "pplfndInstructor_" . session_id() . '/' .
                NTLP_ROLEID_TEACHER. '/1', 350, 600),
        '#prefix' => '<div class="addInstructorBtn" id="pplfndInstructor">',
        '#suffix' => '</div></td></tr>',
    );

    $form['course_additional_notes'] = array(
        '#type' => 'textarea',
        '#wysiwyg' => FALSE,
        '#size' => '512',
        '#default_value' => $course_additional_notes,
        '#resizable' => false,
        '#attributes' => array('style' => 'width:400px;overflow:auto;height:100px'),
        '#prefix' => '<tr><th style="width:160px">' . t('Additional Notes') . '<br clear="all" /></th><td colspan="2">',
        '#suffix' => '<span style="font-size:10px;color:#000;">Max characters = 500</span></td></tr>',
    );
}

function course_form_tab2_grades($form_state, &$form, $school_nid, $course_nid, $course_school_year, $rec, $rec_school) {
    include('ntlp_admincourse.template.inc');
    global $user;
    $rs_user = get_user($user->uid);
    $is_wt_grades = 1;
    $grades_scale = 1;
    $sch_grade_level = array();
    $grades_rounding = 1;

    //Get variables to restore from session
    //saved here by validate handler
//    $variables_to_restore = variable_get("ADMINCOURSE:" . session_id(), false);
//    print ('Session id:'.session_id().' variables:'.print_r($variables_to_restore, true));
//    if (!$variables_to_restore) {
//        //    $variables_to_restore = new stdClass;
//        $variables_to_restore->outcomes = $selected_outcomes;
//        $variables_to_restore->grade_levels = $grade_levels;
//    }
    //Get School defaults
    if (isset($rec_school)) {
        $grades_scale = $rec_school->grade_scale;
        $grades_rounding = $rec_school->grade_rounding;
    }

    if (!empty($course_nid) && isset($rec)) {
        if ($rec != false) {
            $is_wt_grades = $rec->is_weighted_grades;

            //If Grade scales defined/implemented in school is allowed to be override
            if ($grades_scale == 1)
                $sch_grade_level = get_course_grades($course_nid);

//            watchdog('course_load', 'Grade levels loaded for course:'.$course_nid.'<br>'.print_r($sch_grade_level, true));
        }
    }

    //Get defaults, if Grades levels are not defined in course
    if (count($sch_grade_level) < 1) {
        $sch_grade_level = get_school_grades($school_nid, $course_school_year);
//        watchdog('course_load', 'Grade levels loaded for school:' . $school_nid . '<br>' . print_r($sch_grade_level, true));
    }
//    $form['use_gradebook_start'] = array(
//        '#type' => 'markup',
//        '#value' => $HTMLform_course_setting_bluebox_end.'<div>');
    $form['override_grade_level'] = array(
        '#type' => 'hidden',
        '#value' => $grades_scale,
    );
    $form['override_grade_rounding'] = array(
        '#type' => 'hidden',
        '#value' => $grades_rounding,
    );

    $form['use_gradebook'] = array(
//            '#type'   => 'fieldset',
//            '#prefix' => '<table><tr><th style="width:160px">'.t('Use Gradebook for this course').'</th><td colspan="2"><table cellpadding="8" class="table" cellspacing="0" border="0" width="100%">',
//            '#suffix' => '</table></td></tr>',
        '#prefix' => $HTMLform_course_setting_yellowbox_start, //YELLOW box start
        '#suffix' => $HTMLform_course_setting_yellowbox_end, //YELLOW box end
    );

    $form['use_gradebook']['course_grade'] = array(
        '#type' => 'radios',
        '#required' => TRUE,
        '#options' => array(1 => 'Yes', 0 => 'No',),
        '#default_value' => $is_wt_grades,
//            '#default_value' => variable_get('course_grade_scale', 1),
//            '#attributes' => array('onclick' => "show_hide_grade(this);"),
        '#prefix' => '<tr><td class="radioBtn" nowrap="nowrap"><h4 style="float:left;">' . t('Use Gradebook for this course?') . '</h4>',
        '#suffix' => '</td></tr>',
    );

//    $form['use_gradebook_end'] = array(
//        '#type' => 'markup',
//        '#value' => '</div>'.$HTMLform_course_setting_form_start);

    $form['use_weight_grades'] = array(
//            '#type'   => 'fieldset',
//            '#prefix' => '<table><tr><th style="width:160px">'.t('Use Gradebook for this course').'</th><td colspan="2"><table cellpadding="8" class="table" cellspacing="0" border="0" width="100%">',
//            '#suffix' => '</table></td></tr>',
        '#prefix' => $HTMLform_course_setting_bluebox_start,
        '#suffix' => $HTMLform_course_setting_bluebox_end,
    );

    $is_wt_grades = 1;      //Disable
//    $form['use_weight_grades']['course_weight_grades'] = array(
//        '#type' => 'radios',
////            '#title' => t('Weight Grades'),
//        '#options' => array(1 => 'Yes', 0 => 'No'),
//        '#prefix' => '<tr><th style="width:160px">'.t('Weight Grades').'</th><td class="gradesRadio">',
//        '#suffix' => '</td></tr>',
////        '#default_value' => $is_wt_grades,
////        '#attributes' => $is_wt_grades == 0 ?
////                            array('disabled' => "disabled") :
////                            array('onclick' => "show_hide_learning_outcome();"),
//        '#attributes' => array('disabled' => "disabled"),
//        '#default_value' => 1,
//    );

    $form['course_grade_scale']['grade_scale'] = array(
        '#type' => 'fieldset',
        '#prefix' => $HTMLform_course_setting_greenbox_start . '<table cellpadding="0" cellpadding="0" border="0" width="100%"><tr><td>' .
        '<table class="edit_courses_table right" cellpadding="8" cellspacing="0" border="0" width="100%"><tr class="first"><th>' . t('Grade Label') . '</th></tr>',
        '#suffix' => '</table></td>',
    );

#grade lable starts
    for ($gl = 0; $gl < 15; $gl++) {
        $form['course_grade_scale']['grade_scale']['grade_label_' . $gl] = array(
            '#type' => 'textfield',
//            '#required' => TRUE,
            // '#title' => t(''),
            '#size' => 3,
            '#value' => $sch_grade_level[$gl]['label'],
            '#maxlength' => 3,
            '#attributes' => $grades_scale == 0 ? array('disabled' => "disabled") : array(),
            '#prefix' => '<tr><td>',
            '#suffix' => '</td></tr>',
        );
    }

#grade label ends
#grade label starts
    $form['course_grade_scale']['grade_limits'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<td>' .
        '<table cellpadding="8" class="edit_courses_table" cellspacing="0" border="0" width="100%"><tr class="first"><th>' . t('Grade Lower Limit Cutoff') . '</th></tr>',
        '#suffix' => '</table></td></tr></table>' . $HTMLform_course_setting_greenbox_end, //GREEN box start,
    );
    for ($gl = 0; $gl < 15; $gl++) {
        $form['course_grade_scale']['grade_limits']['grade_lower_limit_' . $gl] = array(
            '#type' => 'textfield',
//            '#required' => TRUE,
            //'#title' => ,
            '#size' => 2,
            '#value' => $sch_grade_level[$gl]['lower_limit'],
            '#maxlength' => 2,
            '#attributes' => $grades_scale == 0 ? array('disabled' => "disabled") : array(),
            '#prefix' => '<tr><td>',
            '#suffix' => '</td></tr>',
        );
    }
    //-------------------------------------------------------------------------
    //Learning outcomes
    //Get outcomes for the course
    $js_addoc = "";
    if (!empty($course_nid) && isset($rec)) {
//        if (!$variables_to_restore) {
//        //    $variables_to_restore = new stdClass;
//            $outcomes = $variables_to_restore->outcomes;
//            if(isset($outcomes)) {
//                foreach ($outcomes as $key => $value) {
//                    $total_wt += floatval($value);
//                }
//            }
//                $custom_name = "";
//                $tid = $rec->tid;
//                if (empty($rec->is_custom)) {
//                    $custom_name = $rec->name;
//                    $tid = $rec->tid * -1;   //Change the sign to denote a custom name
//                }
//
//                $js_addoc .= "add_course_outcome($tid, $rec->weight_percent, '$custom_name');";
//            }
//        } else {
        $outcomes = load_course_outcomes($course_nid, $course_school_year);
        while ($rec = db_fetch_object($outcomes)) {
            $custom_name = "";
            $tid = $rec->tid;
            $can_delete = (intval($rec->oc_cnt_in_activity) > 0 ? 'false' : 'true');
            if (empty($rec->is_custom)) {
                $custom_name = $rec->name;
                $tid = $rec->tid * -1;   //Change the sign to denote a custom name
            }

            $js_addoc .= "add_course_outcome($tid, $rec->weight_percent, '$custom_name', $can_delete);";
        }
//        }outcome_cbo
    } else {
        $js_addoc .= "add_course_outcome(x, 0, '', true);";
    }
    //Get outcomes for the school
    $js_wt = '<script language="javascript">
        function get_oc_weight(oc_tid) {
            var oc_weight = new Array();';

    $outcomes = load_school_outcomes($school_nid, $course_school_year);
    $outcomediv1 = '<span class="comboboxBgLeft out_comes" style="width:135px;">
                    <select class="my-dropdown outcome_item" id="@ID" onchange="onchange_outcome_item(this)">';
    $outcomediv1 .= '<option value="x"> Select </option>';
    $i = 0;
    while ($rec = db_fetch_object($outcomes)) {
        $outcomediv1 .= '<option value="' . $rec->tid . '">' . $rec->name . '</option>';
        $js_wt .= "oc_weight[$rec->tid] = $rec->weight_percent;";
        $i++;
    }
    $outcomediv1 .= '<option value="0"> Custom </option>';
    $outcomediv1 .= '</select></span>';

    $outcomediv1 .= '<span id="outcome_div_@INDEX" style="display:none">
                        <input type="text" id="txt_custom_@INDEX" width="20" onBlur="onchange_custom_name(this)" >
                    </span>';

    global $base_path;
    $image = $base_path . 'themes/Boldr/Images/resourceStuff/ArrowGreen';
    $disimage = $base_path . 'themes/Boldr/Images/resourceStuff/ArrowGray';
    $delimage = $base_path . 'themes/Boldr/Images/DeleteBtn.png';
    $infoimage = $base_path . 'themes/Boldr/Images/msgicon.jpg';

//            onBlur="onchange_ocweight(this)" value=""'.
//            ($is_wt_grades == 0 ? 'disabled="disabled"' : '') . '>';
    $outcomediv2 = '
        <input type="hidden" id="txt_candelete_@INDEX">
        <input type="text" id="txt_@INDEX" style="width:40px;" class="outcome_weight"
            onBlur="onchange_ocweight(this)" value="">';
    $outcomediv3 = '<table style="border-width:0px" border="0" cellspacing="0" cellpadding="0"><tr><td style="border:none" nowrap>
        <a class="jslink" name="sortup_link" onclick="move_up(\'outcome_@INDEX\')"><img src="' . $image . 'Up.png" title="Move Up" /></a>&nbsp;
        <a class="jslink" name="sortdown_link" onclick="move_down(\'outcome_@INDEX\')"><img src="' . $image . 'Down.png" title="Move Down" /></a>&nbsp;
        <a class="jslink" name="deleteoc_link" onclick="delete_outcome(\'outcome_@INDEX\')"><img src="' . $delimage . '" title="Remove Outcome" /></a>
        </td></tr></table>';

    $js_wt .= 'return (oc_tid == 0 ? 0 : oc_weight[oc_tid]);}';

    $form['add_outcome'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '',
        '#suffix' => '</table>
            <div id="dialog-confirm-outcome" style="display: none;" title="Warning">
            <span class="msg"></span>
            </div>' . $HTMLform_course_setting_greenboxtwo_end
        . $js_wt               //Outcome Weight defaults in javascript array
        . $js_addoc . '</script>',
    );

    $form['add_outcome']['hide_outcome1'] = array(
        '#type' => 'hidden',
        '#id' => 'oc_td1',
        '#value' => $outcomediv1,
    );
    $form['add_outcome']['hide_outcome2'] = array(
        '#type' => 'hidden',
        '#id' => 'oc_td2',
        '#value' => $outcomediv2,
    );
    $form['add_outcome']['hide_outcome3'] = array(
        '#type' => 'hidden',
        '#id' => 'oc_td3',
        '#value' => $outcomediv3,
    );

    $form['add_outcome']['selected_oc_id'] = array(
        '#type' => 'hidden',
        '#id' => 'selected_oc_id',
        '#value' => '',
    );

    $form['add_outcome']['selected_oc_custom'] = array(
        '#type' => 'hidden',
        '#id' => 'selected_oc_custom',
        '#value' => '',
    );

    $form['add_outcome']['selected_oc_weight'] = array(
        '#type' => 'hidden',
        '#id' => 'selected_oc_weight',
        '#value' => '',
    );

    $form['add_outcome']['heading_row'] = array(
        '#type' => 'markup',
        '#value' => $HTMLform_course_setting_greenboxtwo_start . '<table  class="edit_courses_table" cellpadding="8" id="ocContainer"><tr>
                            <th style="width:100px;border-right:1px solid #D3FFC2;border-left:#9FCCE9">' . t('Outcomes') . '</th>
                            <th style="white-space:nowrap">' . t('Weight %') . '</th>
                            <th style="width:100px">&nbsp;</th>
                            </tr>',
    );

//    $form['add_outcome']['show_outcome'] = array(
//            '#type' => 'item',
//            '#value' => '<div id="element"></div>',
//    );

    $form['add_outcome']['add_outcome'] = array(
        '#type' => 'markup',
        '#value' => '<tr id="ocLastRow"><td><a class="AddOutComesBtn" onclick="get_outcome_combo();">Add Outcome</a></td><td>&nbsp</td><td>&nbsp</td></tr>',
//            '#attributes' => array('onclick' => 'add_outcome();'),
    );

    $form['add_outcome']['total_label'] = array(
        '#type' => 'item',
        '#value' => 'Total',
        '#prefix' => '<tr><td align="left" style="background-color:#ef7d23;color:#FFFFFF;font-weight:bold"><div style="float:left">',
        '#suffix' => '</div><div style="float:right"><a href="#"><img src="' . $infoimage . '" style="border:none" /></a></div></td>',
    );
    $form['add_outcome']['total_value'] = array(
        '#type' => 'item',
        '#id' => 'total_oc_weight',
        '#value' => '0.00%',
        '#prefix' => '<td align="center">',
        '#suffix' => '</td><td>&nbsp</td></tr>',
    );

    /*
     * This is being removed from version 1.0
     *
      #attendance panelty of this course
      //    $form['attendance_panelty'] = array(
      ////            '#type'   => 'fieldset',
      //            '#title' => t('Attendance Penalty'),
      //            '#prefix' => '<tr><th style="width:160px">'.t('Attendance Penalty').'</th><td>',
      //            '#suffix' => '</td></tr>',
      //    );

      $form['attendance_panelty_radio'] = array(
      '#type' => 'radios',
      //            '#title' => t('Use Attendance Panelty'),
      '#options' => array(1 => 'Yes',0 => 'No',),
      '#prefix' => '<tr><th style="width:160px">'.t('Use Attendance Penalty').'</th><td>',
      '#suffix' => '</td></tr>',
      '#default_value' => variable_get('course_attendance_penalty', 1),
      '#attributes' => array(
      'onclick' => "if(this.value==1)
      document.getElementById('attendance_panelty_test').style.display='block';
      else document.getElementById('attendance_panelty_test').style.display='none';",
      ),
      );

      $form['category_to_penalize'] = array(
      //            '#title' => t('Category to penalize for absences and tradies'),
      '#type' => 'select',
      '#options' => isset($outcomes) ? $outcomes : array('None'),
      '#prefix' => '<tr><th style="width:160px">'.t('Attendance Penalty<br>Category to penalize for absences and tradies').'</th><td>',
      '#suffix' => '</td></tr>',
      );


      $form['penalty_percent'] = array(
      '#type' => 'textfield',
      //            '#title' => t('penalty percentage'),
      '#size' => 2,
      '#maxlength' => 2,
      '#prefix' => '<tr><th style="width:160px">'.t('Penalty Percentage').'</th><td>',
      '#suffix' => '</td></tr>',
      );

      $form['no_of_tardies'] = array(
      '#type' => 'textfield',
      //            '#title' => t('no of tardies'),
      '#size' => 2,
      '#maxlength' => 2,
      '#prefix' => '<tr><th style="width:160px">'.t('Number of Tardies').'</th><td>',
      '#suffix' => '</td></tr>',
      );
     *
     */
}

function course_form_tab3_members($form_state, &$form, $school_nid, $course_nid, $course_school_year, $rec, $rec_school, $ajax = false) {
//    $school_nid = get_this_school();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');

    include('ntlp_admincourse.template.inc');
//    ahah_helper_register($form, $form_state);
//    $form['heading'] = array(
//            '#type' => 'item',
//            '#value' => t('Edit Course Settings: Students'),
//            '#prefix' => '<tr><td>',
//            '#suffix' => '</td></tr>',
//    );

    $form['students'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<tr><td style="border-bottom:none"><div id="students_view">',
        '#suffix' => '</div>',
    );

    if (isset($form_state['values']['op'])) {
//        watchdog('info',$form_state['values']['op']);
        $view_state = $form_state['values']['op'];
    }
    $form['students']['drop_link'] = array(
        '#type' => 'item',
        '#value' => '<a style="font-size: 16px; font-weight: bold;" href="javascript:;" class="gradeSettingLinkDisActive enrolledDroppedBtn" id="EnrolledShowBtn" onclick="show_dropped_users(' . $course_nid . ')">Dropped</a>
                        <span style="float: right; margin-left: 3px; font-size: 16px;">|</span>
                        ',
//            '#ahah' => array(
//                    'path' => ahah_helper_path(array('main', 'form', 'students')),
//                    'wrapper' => 'students_view',
//                    'progress' => array('type' => 'throbber', 'message' => t('Loading...')),
//                    'method' => 'replace',
//            ),
        '#prefix' => '<div style="float:left;width:155px;">',
    );

    $form['students']['enroll_link'] = array(
        '#type' => 'item',
        '#value' => '<a style="font-size: 16px; font-weight: bold;" href="javascript:;" class="gradeSettingLinkActive enrolledDroppedBtn" id="DroppedShowBtn" onclick="show_enrolled_users(' . $course_nid . ')">Enrolled</a>',
//            '#ahah' => array(
//                    'path' => ahah_helper_path(array('main', 'form', 'students')),
//                    'wrapper' => 'students_view',
//                    'progress' => array('type' => 'throbber', 'message' => t('Loading...')),
//                    'method' => 'replace',
//            ),
        '#suffix' => '</div>',
    );

    if (isset($view_state) && $view_state == 'Dropped') {

        $form['students']['display_view'] = array(
            '#type' => 'fieldset',
            '#title' => '',
            '#prefix' => '<br clear="all" /><div id="course_views">',
            '#suffix' => '</div>',
        );

//        $view = views_get_view('ntlp_og_dropouts');

        $form['students']['display_view']['dropped'] = array(
            '#type' => 'item',
            '#value' => show_dropped_users($course_nid, false),
//                '#value' => $view->preview('default', array($course_nid)),
        );
    } else {
//        $course_nid = 1;
        $course_members = _get_course_members($course_nid);


        if ($course_members) {
            foreach ($course_members as $member) {
                $pre_selected_list[] = $member->uid;
            }
            $pre_selected_list = array_unique($pre_selected_list);
            $pre_selected_list = implode(',', $pre_selected_list);
        } else {
            $pre_selected_list = 0;
        }

        cache_set("pf_add_students_" . session_id(), $pre_selected_list, 'ntlp_cache', CACHE_TEMPORARY);
        $form['students']['add_students_link'] = array(
            '#type' => 'item',
            '#value' => dlg("Add Students", 'ntlp/people_finder/enrollments/' . $school_nid . '/' . $course_nid . '/' . "pf_add_students_" . session_id() . '/' . NTLP_ROLEID_STUDENT . '/1', 350, 600),
            '#id' => 'pf_add_students',
            '#prefix' => '<span style="float:right;" class="add_students_link">',
            '#suffix' => '</span>',
        );

        $form['students']['display_view'] = array(
            '#type' => 'fieldset',
            '#title' => '',
            '#prefix' => '<br clear="all" /><div id="course_views">',
            '#suffix' => '</div>',
        );

//        $view = views_get_view('ntlp_og_members');

        $form['students']['display_view']['enrolled'] = array(
            '#type' => 'item',
//                '#value' => $view->preview('default', array($course_nid, $course_nid)),
            '#value' => show_enrolled_users($course_nid, false),
        );
    }
}

function course_form_tab4_guestacs($form_state, &$form, $school_nid, $course_school_year, $course_nid, $rec) {
    include('ntlp_admincourse.template.inc');
//    ahah_helper_register($form, $form_state);

    $form['heading'] = array(
        '#type' => 'item',
        //    '#value' => t('Edit Course Settings: Guest Access'),
        '#prefix' => '<tr><td align="right" class="AddGuestsBtn">',
    );
    $form['add_guests_link'] = array(
        '#type' => 'item',
        '#value' => dlg("Add Guests", 'ntlp/people_finder/guests/' . $course_nid, 350, 600),
        '#suffix' => '</td></tr>',
    );

    $form['intro'] = array(
        '#type' => 'item',
        '#value' => t('This allows you to give users outside your school the ability to view course content, participate in discussion, and share course-wide resources.  Guests cannot submit course activities or view student work/grades.  A Guest might be a member of your local community or a colleague at another Network school.  Only users with accounts in the system can be added as a Guest. Remember: parents of enrolled students, all staff members at your school, and all Network coaches automatically have access to View your course as well.'),
        '#prefix' => '<tr><td><p>',
        '#suffix' => '</p></td></tr><tr><td>',
    );

    $form['guests'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div id="course_views">',
        '#suffix' => '</div>',
    );

    $view = views_get_view('ntlp_og_guests');
//    $view->set_arguments(array($course_nid));

    $form['guests']['display_view'] = array(
        '#type' => 'item',
        '#value' => $view->preview('default', array($course_nid, $course_nid)),
    );
}

function add_tabs($course_nid, &$form, $tab = '') {
    include('ntlp_admincourse.template.inc');
    if (empty($course_nid)) {
        $url = "";
        $rHTMLform = $HTMLform_course_settings_tabs_option_fornew;
    } else {
        $url = url("ntlp/courses/edit/$course_nid");
        $rHTMLform = $HTMLform_course_settings_tabs_option;
    }
    $form['tabs'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#prefix' => $HTMLform_course_setting_tabs_start,
        '#suffix' => $HTMLform_course_setting_tabs_end,
    );
    $roption = $rHTMLform;
    $roption = str_replace("@LINK", $url, $roption);
    $roption = str_replace("@PROMPT", "General", $roption);
    $roption = str_replace("@CLASS", ($tab != 'grades' && $tab != 'members' && $tab != 'guestacs') ? "class='active'" : "", $roption);
    $form['tabs']['basic'] = array(
        '#type' => 'markup',
        '#value' => $roption,
    );
    $roption = $rHTMLform;
    $roption = str_replace("@LINK", "$url/grades", $roption);
    $roption = str_replace("@PROMPT", "Gradebook Setup", $roption);
    $roption = str_replace("@CLASS", ($tab == 'grades') ? "class='active'" : "", $roption);
    $form['tabs']['grade_settings'] = array(
        '#type' => 'markup',
        '#value' => $roption,
    );
    $roption = $rHTMLform;
    $roption = str_replace("@LINK", "$url/members", $roption);
    $roption = str_replace("@PROMPT", "Enrollment", $roption);
    $roption = str_replace("@CLASS", ($tab == 'members') ? "class='active'" : "", $roption);
    $form['tabs']['students'] = array(
        '#type' => 'markup',
        '#value' => $roption,
    );
//    $roption = $rHTMLform;
//    $roption = str_replace("@LINK", url("ntlp/courses$url/guestacs"), $roption);
//    $roption = str_replace("@PROMPT", "Guest Access", $roption);
//    $roption = str_replace("@CLASS", ($tab == 'guestacs') ? "class='active'" : "", $roption);
//    $form['tabs']['guest_access'] = array(
//            '#type' => 'markup',
//            '#value' => $roption,
//    );
//    $roption = $rHTMLform;
//    $roption = str_replace("@LINK", url("ntlp/courses$url/dataexp"), $roption);
//    $roption = str_replace("@PROMPT", "Data Export", $roption);
//    $roption = str_replace("@CLASS", ($tab == 'dataexp') ? "class='active'" : "", $roption);
//    $form['tabs']['data_export'] = array(
//            '#type' => 'markup',
//            '#value' => $roption,
//    );
}

function get_instructor_namelink($uid, $firstname, $lastname) {
    global $user;
    
//    if ($user->uid == $uid || $uid == $course_owner_id) {
    if ($user->uid == $uid) {
        $output = '<div id="' . $uid . '">' . $firstname . ' ' . $lastname . '<br/></div>';
    } else {
        $output = '<div id="' . $uid . '">
            <a onclick="remove_primary_user($(this),' . $uid . ',\'pplfndPrimaryInstructor_' . session_id() . '\');">
                <img height="14" width="14" class="down" style="margin-top:4px;" src="themes/Boldr/Images/DeleteBtn.png"/>
            </a>&nbsp; ' . $firstname . ' ' . $lastname . '<br/>
        </div>';
    }
   return $output;
}
