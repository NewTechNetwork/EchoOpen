<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
require_once ('data_access.inc');
/**
 * Form controller
 */

function ntlp_use_myaccount_settings($form_state) {
    // Add the client-side behaviors for the examples.
    global $user;

    include('ntlp_user_profile.template.inc');



    $school_nid = get_this_school();        //new or edit
    $form = array();
    if($school_nid > 0 ) {


        $tab = check_plain(arg(4));        //new or edit

        if (!isset($tab) || empty($tab)) {
            $tab = 'account';
        }

        $form['main'] = array(
                '#type' => 'fieldset',
                '#title' => t(''),
                '#prefix' => $HTMLform_course_setting_start,
                '#suffix' => $HTMLform_course_setting_end,
        );


        if($tab == 'account') {
            $heading = "User Account Settings";

            $form['main']['save'] = array(
                    '#type' => 'submit',
                    '#value' => 'Save',
                    '#prefix' => '<td>&nbsp;</td><td><table cellpadding="0" cellspacing="0" width="100%"><tr><td><div><h2 style="padding:0px;margin:0px;color:#3570aa;font-size:18px;">'.$heading.'</h2></div></td><td align="right">',
                    '#suffix' => '</td></table></td></tr><tr>',
                    '#submit' => array('nltp_lib_doc_submit_'.$tab),
                    '#attributes' => array('class' => 'SaveBtn'),
            );

        }

        ntlp_myaccount_doc_add_tabs($school_nid, $form['main'], $tab);

        $form['main']['form_start'] = array(
                '#type' => 'markup',
                '#value' => $HTMLform_course_setting_form_start
        );

        $form['main']['form_start0'] = array(
                '#type' => 'markup',
                '#value' => $HTMLform_course_setting_bluebox_start
        );

        $form['main']['form'] = array(
                '#type' => 'fieldset',
                '#title' => t(''),
        );

        $form['main']['form']['wrapper'] = array(
                '#type' => 'item',
                '#prefix' => '<tr><td colspan="2">',
                '#suffix' => '</td></tr>',
        );

        ntlp_use_myaccount_tab_content($form_state, $form['main']['form']['wrapper'], $tab);


        $form['main']['form_end0'] = array(
                '#type' => 'markup',
                '#value' => $HTMLform_course_setting_bluebox_end);

        $form['main']['form_end'] = array(
                '#type' => 'markup',
                '#value' => $HTMLform_course_setting_form_end
        );

    }else {
        drupal_set_message('No School Selected for this user', 'error');
    }
    return $form;
}

function ntlp_myaccount_doc_add_tabs($module_nid, &$form, $tab = '') {
    include('ntlp_user_profile.template.inc');

    $form['tabs'] = array(
            '#type' => 'fieldset',
            '#title' => t(''),
            '#prefix' => $HTMLform_course_setting_tabs_start,
            '#suffix' => $HTMLform_course_setting_tabs_end,
    );
    $roption = $HTMLform_course_settings_tabs_option;
    $roption = str_replace("@LINK", url("ntlp/myaccount/user/settings/account"), $roption);
    $roption = str_replace("@PROMPT", "Account Settings", $roption);
    $roption = str_replace("@CLASS", ($tab == 'account') ? "class='active'" : "", $roption);
    $form['tabs']['account'] = array(
            '#type' => 'markup',
            '#value' => $roption,
    );

    global $user;
    $school_nid = get_user_school($user->uid);
    $role_rs = get_user_role($user->uid);
    $role_id = $role_rs->rid;
    $allowed = get_selected_school_permissions($school_nid, $role_id, 'Receive Email Notifications');
    
    if ($allowed == 1) {
        $roption = $HTMLform_course_settings_tabs_option;
    }
}



function ntlp_use_myaccount_tab_content($form_state, &$form, $tab) {
    if( $tab == 'account' ) {
        ntlp_use_myaccount_tab_account($form_state, $form);
    } 
}

function ntlp_use_myaccount_tab_account($form_state, &$form) {
    global $user;
    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') .'/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_document_library') . '/ntlp_document_library.js');

    include('ntlp_user_profile.template.inc');
    include('profile.template.inc');

    $userObj = get_user($user->uid);
    $userSchoolObj = get_user_school_info($user->uid);



    $form['account'] = array(
            '#type' => 'markup',
            '#prefix' => '<table width="100%">',
            '#suffix' => '</table>',
    );

    $form['account']['first_name'] = array(
            '#type' => 'markup',
            '#value' => $userObj->first_name,
            '#prefix' => '<tr><th>First Name</th><td>',
            '#suffix' => '</td></tr>',
    );

    $form['account']['last_name'] = array(
            '#type' => 'markup',
            '#value' => $userObj->last_name,
            '#prefix' => '<tr><th>Last Name</th><td>',
            '#suffix' => '</td></tr>',
    );

    $form['account']['username'] = array(
            '#type' => 'markup',
            '#value' => $userObj->name,
            '#prefix' => '<tr><th>Username</th><td>',
            '#suffix' => '</td></tr>',
    );

    $form['account']['password'] = array(
            '#type' => 'item',
            '#value' => check_changepassword_permissions() ? dlg('Change Password', 'ntlp/myaccount/user/settings/changepassword/'.uniqid(), 300, 500) : 'You are not authorized to change your password.',
            '#prefix' => '<tr><th>Password</th><td>',
            '#suffix' => '</td></tr>',
    );

    $form['account']['school'] = array(
            '#type' => 'markup',
            '#value' => $userSchoolObj->school_name,
            '#prefix' => '<tr><th>School</th><td>',
            '#suffix' => '</td></tr>',
    );

    if( in_array( NTLP_ROLEID_STUDENT ,array_keys($user->roles) ) ) {
        $form['account']['grad_year'] = array(
                '#type' => 'markup',
                '#value' => "Class of $userObj->grad_year",
                '#prefix' => '<tr><th>Graduation Year</th><td>',
                '#suffix' => '</td></tr>',
        );
    }

    if( in_array( NTLP_ROLEID_PARENT, array_keys($user->roles) ) ) {
        ///////// PARENT CHILDREN START /////////
        $form['account']['children'] = array(
                '#type' => 'item',
                '#value' => '',
                '#prefix' => '<tr><th>My Students</th><td>'.str_replace('@HEADING', '', $HTMLBlueBoxStart)
                        .$HTMLWhiteBoxCourseLinkStart,
                '#suffix' => $HTMLBlueBoxEnd
                        .$HTMLWhiteBoxCourseLinkEnd.'</td></tr>',
        );


        if($children = _get_parent_children($user->uid)) {
            foreach( $children as $child ) {
                $user_pic = $child->picture ? $child->picture : 'themes/Boldr/Images/common/small_person_image_not_found.jpg';
                $form['account']['children']['child_'.$child->uid] = array(
                        '#type' => 'item',
                        '#value' => '<div class="AdminMainPanel">
                                        <div class="AdminPersonImg" style="float: left;">
                                            <img src='.$base_path.$user_pic.' width="32px" alt="Admin Image 1" />
                                        </div>
                                        <div class="AdminPersonNameAndOwner" style="float: left; margin-left: 5px;">
                                            <div class="AdminPersonName">'.l($child->first_name.' '.$child->last_name,'ntlp/user/profile/'.$child->uid).'</div>
                                            <div style="clear: both;"></div>
                                            <div style="float: left; font-size: 11px; margin-top: -5px;">
                                                Class of '. $child->grad_year .'
                                            </div>
                                        </div>
                                        <div style="clear: both;"></div>
                                     </div>',
                        '#prefix' => '<li><a href="#">',
                        '#suffix' => '</a></li>',
                );
            }
        }
        ///////// PARENT CHILDREN END /////////
    }



}

function ntlp_use_myaccount_settings_changepassword($form_state) {
    modalframe_child_js();
    if($form_state['storage']['success']) {
        $form['msg'] = array(
                '#type' => 'item',
                '#value' => 'Password changed successfully.'
        );
        $form['close'] = array(
                '#type' => 'item',
                '#value' => '<a class="modalframe-processed" href="javascript: void(0);" onclick="parent.Drupal.modalFrame.close();parent.window.location.reload(true);">Close</a>'
        );
        return $form;
    }

    $form = array();
    _user_password_dynamic_validation();

    $form['main'] = array(
            '#type' => 'fieldset',
            '#attributes' => array('style' => 'height: 165px;'),
    );

    $form['main']['save'] = array(
            '#type' => 'submit',
            '#value' => t('Save'),
            '#attributes' => array('class' => 'SaveBtn'),
//            '#attributes' => array('class' => 'SaveBtn', 'onclick' => 'parent.Drupal.modalFrame.close();parent.window.location.reload(true);'),
            '#prefix' => '<div style="float: right;">',
            '#suffix' => '</div>',
    );


    $form['main']['old_password'] = array(
            '#type' => 'password',
            '#title' => 'Old Password',
            '#size' => 25,
            '#description' => t('Your current password.'),
            '#required' => TRUE,
    );

    $form['main']['password'] = array(
            '#type' => 'password_confirm',
            '#size' => 25,
            '#description' => t('To change the current user password, enter the new password in both fields.'),
    );


    return $form;
}

function ntlp_use_myaccount_settings_changepassword_validate(&$form, &$form_state) {
    global $user;

    if( empty($form['#post']['password']['pass1']) || empty($form['#post']['password']['pass2']) ) {
        form_set_error('', 'New password cannot be empty!');
    } elseif (strlen($form['#post']['password']['pass1']) < MIN_PASSWORD_LENGTH || strlen($form['#post']['password']['pass2']) < MIN_PASSWORD_LENGTH) {
        form_set_error('', 'Password should be at least eight characters long.');
    } else {
        $data = unserialize(db_result(db_query('SELECT data FROM {users} WHERE uid = %d', $user->uid)));
    //    drupal_set_message('Ldap:'.$data['ldap_authentified']);
    //    drupal_set_message('Auth:'._ldapauth_auth($user->name, $form['#post']['old_password']));
        if (isset($data['ldap_authentified']) && $data['ldap_authentified'] == 1) {
            $dn = _ldapauth_auth($user->name, $form['#post']['old_password']);
            if (!$dn) {
                form_set_error('old_password', 'Old password not verified.');
            }
        } else {
            if( md5($form['#post']['old_password']) != $user->pass ) {
                form_set_error('old_password', 'Old password did not match.');
            }
        }
    }
}



function ntlp_use_myaccount_settings_changepassword_submit( $form, &$form_state ) {
    ntlp_change_password_current_user($form_state['clicked_button']['#post']['password']['pass1']);
    $form_state['storage']['success'] = TRUE;
}

function ntlp_user_forgot_password( $form_state ) {
    modalframe_child_js();

    $form = array();

    $form['forgot_password'] = array(
            '#type' => 'fieldset',
    );

    $form['forgot_password']['username'] = array(
            '#type' => 'textfield',
            '#title' => 'Username/E-mail',
//        '#attributes' => array( 'class' => 'textbox' ),
    );

    $schools = array();
    if($result = get_schools()) {
        while( $school = db_fetch_object($result) ) {
            $schools[$school->nid] = $school->school_name;
        }
    }

    $form['forgot_password']['schools_combo'] = array(
            '#type' => 'select',
            '#options' => $schools,
    );

    $form['forgot_password']['submit'] = array(
            '#type' => 'submit',
            '#value' => 'Submit',
    );

    return $form;
}

function ntlp_user_forgot_password_submit( $form_state ) {

    $forgetPasswordTerm = array_shift(taxonomy_get_term_by_name('Forget Password'));

    $user = $form_state['forgot_password']['username']['#post']['username'];
    $school_nid = $form_state['forgot_password']['username']['#post']['schools_combo'];
    $school_name = $form_state['forgot_password']['schools_combo']['#options'][$school_nid];

    db_query( " INSERT INTO {ntlp_support_request}(request_type_tid, priority, details, submitted)
                VALUES( %d, 1, '%s', NOW() ) ",
            $forgetPasswordTerm->tid,
            "<p>User: $user, of School \"$school_name\" has requested to reset his password.</p>"
    );
    echo "Thanks.  We got it! Your site administrator will be in touch soon.";

}
