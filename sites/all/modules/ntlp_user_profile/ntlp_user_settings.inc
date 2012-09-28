<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


/////proflie settings start
module_load_include('inc', 'ntlp_school', 'data_access');
function ntlp_user_settings_form() {
    _user_password_dynamic_validation();
    include('profile.template.inc');
    global $base_path;
    global $user;
    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_announcement') . '/ntlp_announcement.js', 'module');
    drupal_add_js(drupal_get_path('module', 'image_uploader') . '/image_uploader.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_user_profile') . '/ntlp_user_profile.js');

    $admin = user_access('administer users');
    $useruid = check_plain(arg(3));
    $view_state = '';

    $profileObj = load_profile_record($useruid);
    //print_r($profileObj);
    if($profileObj == false) {
        drupal_set_message('User does not exist.');
    }else {
        if(!$admin) {
            drupal_set_message('you are not authorised to view this page');

        }else {
            $form['user_profile'] = array(
                    '#type'   => 'fieldset',
                    '#title'  => '',
                    '#tree'   => TRUE, // Don't forget to set #tree!
                    '#prefix' => '<div id="user_profile_test">',
                    '#suffix' => '</td></tr></table></div>',
            );

            //print_r($profileObj);

            $form['user_profile']['submit'] = array(
                    '#type' => 'submit',
                    '#value' => t('Save'),
                    '#submit' => array('ntlp_user_settings_submit'),
                    '#attributes' => array('class' => 'SaveBtn'),
                    '#prefix' => '<div style="float:right">',
            );
            if (user_access('administer users')) {
                $form['user_profile']['delete'] = array(
                        '#type' => 'submit',
                        '#value' => t('Delete'),
                        '#weight' => 31,
                        '#attributes' => array('class' => 'SaveBtn'),
                        '#submit' => array('user_edit_delete_submit'),
                        '#suffix' => '</div><table cellpadding="2" cellspacing="2" border="0">
                                <tr><td valign="top">',
                );
            }



            ///////// fieldset for About Me starts here /////////
            $form['account'] = array(
                    '#type'   => 'fieldset',
                    '#tree'   => TRUE,
                    '#prefix'=>$HTMLaboutme_hdr,
                    '#suffix'=>$HTMLaboutme_ftr,
            );

            $form['account']['uid'] = array(
                    '#type' => 'hidden',
                    '#value' => $useruid,
            );

            $form['account']['firstname'] = array(
                    '#type' => 'textfield',
                    '#value' => $profileObj->first_name,
                    '#size' => '60',
                    '#prefix'=>'<tr>
                          <th>First Name</th>
                          <td>',
                    '#suffix'=>'</td>
                        </tr>',
            );

            $form['account']['lastname'] = array(
                    '#type' => 'textfield',
                    '#value' => $profileObj->last_name,
                    '#size' => '60',
                    '#prefix'=>'<tr>
                          <th>Last Name</th>
                          <td>',
                    '#suffix'=>'</td>
                        </tr>',
            );


            $form['account']['mail'] = array(
                    '#type' => 'textfield',
                    '#value' => $profileObj->mail,
                    '#size' => '60',
                    '#prefix'=>'<tr>
                          <th>E-mail Address</th>
                          <td>',
                    '#suffix'=>'</td>
                        </tr>',
            );

            $form['account']['name'] = array(
                    '#type' => 'textfield',
                    '#size' => '60',
                    '#value' => $profileObj->init,
                    '#maxlength' => USERNAME_MAX_LENGTH,
                    '#prefix'=>'<tr>
                          <th>Username</th>
                          <td>',
                    '#suffix'=>'</td>
                        </tr>',
                    '#required' => TRUE,
            );

            $form['account']['pass'] = array(
                    '#type' => 'password_confirm',
                    // '#required' => TRUE,
                    '#size' => 25,
                    '#prefix'=>'<tr>
                          <th>Password</th>
                          <td>',
                    '#suffix'=>'</td>
                        </tr>',
            );


            unset($options);
            $options[0] = 'Active';
            $options[1] = 'Blocked';
            $form['account']['status'] = array(
                    '#type' => 'radios',
                    '#value' => $profileObj->status,
                    '#options' => $options ,
                    '#prefix'=>'<tr>
                          <th>Status</th>
                          <td>',
                    '#suffix'=>'</td>
                        </tr>',
            );


            $myroles = _get_my_roles($useruid);
            //print_r($myroles);
            if (user_access('administer permissions')) {
                $roles = user_roles(TRUE);

                // The disabled checkbox subelement for the 'authenticated user' role
                // must be generated separately and added to the checkboxes element,
                // because of a limitation in D6 FormAPI not supporting a single disabled
                // checkbox within a set of checkboxes.
                // TODO: This should be solved more elegantly. See issue #119038.
                $checkbox_authenticated = array(
                        '#type' => 'checkbox',
                        '#title' => $roles[DRUPAL_AUTHENTICATED_RID],
                        '#default_value' => TRUE,
                        '#disabled' => TRUE,
                );

                unset($roles[DRUPAL_AUTHENTICATED_RID]);

                if ($roles) {
                    $default = empty($edit['roles']) ? array() : array_keys($edit['roles']);
                    $form['account']['roles'] = array(
                            '#type' => 'checkboxes',
                            '#default_value' => $myroles,
                            '#options' => $roles,
                            DRUPAL_AUTHENTICATED_RID => $checkbox_authenticated,
                            '#prefix'=>'<tr>
                          <th>Roles</th>
                          <td>',
                            '#suffix'=>'</td>
                        </tr>',
                    );
                }
            }




            $result = get_schools();
            $arrSchool[]='Select';
            while($school = db_fetch_object($result)) {
                $arrSchool[$school->nid] = $school->school_name;
            }

            $form['account']['ntlp_schools'] = array(
                    '#type' => 'select',
                    '#options' => $arrSchool,
                    '#default_value' => $profileObj->school_nid,
                    '#prefix'=>'<tr>
                          <th>School</th>
                          <td>',
                    '#suffix'=>'</td>
                        </tr>',
            );
            $year_grad = date('Y',strtotime('+4 years',$profileObj->created));
            $form['account']['school_grad_year'] = array(
                    '#type' => 'item',
                    '#value' => 'Class of '.$year_grad,
                    '#prefix'=>'<tr>
                          <th>Graduation Year</th>
                          <td>',
                    '#suffix'=>'</td>
                        </tr>',
            );

            $form['account']['grad_year'] = array(
                    '#type' => 'hidden',
                    '#value' => $year_grad,
            );

            $form['#uid'] = $uid;

            ///////// fieldset for About Me ends here /////////
        }



    } //condition of valid user ends here


//  $edit = (empty($form_state['values'])) ? (array)$account : $form_state['values'];
//  $form = _user_forms($edit, $account, $category);
//  $form['_category'] = array('#type' => 'value', '#value' => $category);
//  $form['_account'] = array('#type' => 'value', '#value' => $account);
//  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'), '#weight' => 30);
//  if (user_access('administer users')) {
//    $form['delete'] = array(
//      '#type' => 'submit',
//      '#value' => t('Delete'),
//      '#weight' => 31,
//      '#submit' => array('user_edit_delete_submit'),
//    );
//  }
//  $form['#attributes']['enctype'] = 'multipart/form-data';



//}//condition for own profile user ends here
    return $form;





}
/////profile settings end




function ntlp_user_settings_submit($form, $form_state) {
    $uid = $form_state['values']['account']['uid'];
    $account = user_load($uid);
//    echo '<br>-----------account-----------<br>';
    $account_values = $form_state['values']['account'];
    unset($account_values['uid'],$account_values['grad_year'],$account_values['course_primary_instructor_id']);
//    print_r($account_values);
//    echo '<br>-----------value-----------<br>';

    $account_values['form_build_id']=$form_state['values']['form_build_id'];
    $account_values['picture_delete']=0;
    $account_values['picture_upload']='';
    $account_values['messaging_default']='phpmailer';
    $account_values['notifications_send_interval']=0;
//    $account_values['og_notifications_autosubscribe']=1;
  
    //user_module_invoke('submit', $form_state['values'], $account, $category);
    user_save($account, $account_values, $category);

    // Clear the page cache because pages can contain usernames and/or profile information:
    cache_clear_all();

    //drupal_set_message(t('The changes have been saved.'));
    return;
}

//
//function ntlp_user_settings_submit_old($form, $form_state) {
//    $useruid = $form_state['values']['user_profile']['user_id'];
//    $user_profile = $form_state['clicked_button']['#post']['user_profile'];
////  print_r($user_profile);
//    $first_name = $user_profile['about_me']['first_name'];
//    $last_name = $user_profile['about_me']['last_name'];
//    $email_address = $user_profile['about_me']['email_address'];
//    $username = $user_profile['about_me']['username'];
//    $pass = $user_profile['about_me']['pass']['pass1'];
//    $status = $user_profile['about_me']['status'];
//    $ntlp_school = $user_profile['about_me']['ntlp_school'];
//    $messaging_method = $user_profile['about_me']['messaging_method'];
//    $mail_interval = $user_profile['about_me']['mail_interval'];
//    $roles = $user_profile['about_me']['roles'];
//    $course_primary_instructor_id = $user_profile['about_me']['course_primary_instructor_id'];
//    $grad_year = $user_profile['about_me']['grad_year'];
//
//    if(!empty($roles)) {
//        $sql = "select * from {users_roles} where uid=%d";
//        $result = db_query($sql,$useruid);
//        while($rows = db_fetch_object($result)) {
//            $myroles[] = $rows->rid;
//        }
//
//        if(!empty($roles)) {
//            foreach($roles as $key) {
//                if(in_array($key, $myroles)==false)
//                    db_query("insert into {users_roles} value(%d,%d)",$useruid,$key);
//
//            }
//        }
//
//        if(!empty($myroles)) {
//            foreach($myroles as $key) {
//                if(in_array($key, $roles)==false)
//                    db_query("delete from {users_roles} where uid=%d and rid=%d",$useruid,$key);
//            }
//        }
//    }
//
//
////    $data = unserialize(db_result(db_query('SELECT data FROM {users} WHERE uid = %d', $useruid)));
////    print_r($data);
//
//
//    if(isset($useruid) and $useruid!=null) {
//        $sql = "UPDATE {ntlp_user} SET first_name='%s', last_name='%s', school_nid=%d, grad_year=%d WHERE uid=%d";
//        db_query($sql,$first_name,$last_name,$ntlp_school,$grad_year,$useruid);
////        messaging_method='%s', mail_interval='%s'
////        $messaging_method,$mail_interval,
//
////        $sql = "UPDATE {users} SET name='%s',mail='%s',status=%d WHERE uid=%d";
////        db_query($sql,$username,$email_address,$status,$useruid);
////
////        $sql = "UPDATE {ntlp_user} SET instructor_uid='%s' WHERE uid=%d";
////        db_query($sql,$course_primary_instructor_id,$useruid);
////
////        if(!empty($pass)) {
////            $sql = "UPDATE {users} SET pass='%s' WHERE uid=%d";
////            db_query($sql,md5($pass),$useruid);
////        }
//
//        //roles='%s'  . ,$roles
//    }
//    drupal_set_message('Basic Settings of '.$first_name.' '. $last_name .' saved successfully.');
//}


?>