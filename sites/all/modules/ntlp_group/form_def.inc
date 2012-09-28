<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'ui_tools');
module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_course', 'data_access');
module_load_include('inc', 'ntlp_group', 'data_access');

/**
 * Form controller
 */
function group_form_controller($form_state) {
    global $user;
    //Set default main menu for this form
    $mainmenu = menu_get_item('ntlp/groups');
    menu_set_item(NULL, $mainmenu);

    // Add the client-side behaviors for the examples.
    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_group') . '/ntlp_group.js', 'footer');
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/fileupload_ahah_helper.js', 'footer');
    drupal_add_js(drupal_get_path('module', 'image_uploader') . '/image_uploader.js');



    include('ntlp_group.template.inc');

    $group_nid = 0;
    $form = array();
    $mode = check_plain(arg(2));        //new or edit
    $group_nid = check_plain(arg(3));
    $tab = check_plain(arg(4));        //new or edit

    if (isset($form_state['values']['op'])) {
        watchdog('info', 'Group Form Controller view state:<br />' . print_r($form_state, true));
        $view_state = $form_state['values']['op'];
        $group_nid = $form_state['values']['group_node'];
    }else{
        if(isset($form_state['values']['group_node'])){
            $group_nid = $form_state['values']['group_node'];
        }
    }
    if (!isset($tab) || empty($tab)) {
        $tab = 'settings';
    }

    watchdog('group_data', print_r($form_state, true));
    if (isset($mode)) {
        if ($mode == 'edit') {
            if (!empty($group_nid)) {
                $rec = get_group($group_nid);
                if ($rec == false) {
                    drupal_set_message("Group not found, please contact system administrator");
                    return;
                }
            }
        }
    }

    if (!is_user_group_admin($group_nid, $user)) {

        drupal_set_message('You are not authorized to edit this group ', 'error');
        drupal_goto('ntlp/goback');
    }
//firep('Mode:'.$mode);
//firep('group:'.$group_nid);
    $form['main'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#prefix' => $HTMLform_group_setting_start,
        '#suffix' => $HTMLform_group_setting_end,
    );



    $form['main']['group_node'] = array(
        '#type' => 'hidden',
        '#id' => 'group_id',
        '#value' => $group_nid,
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div>',
    );
    if ($tab == 'settings' || $tab == 'grades') {
        $form['main']['save'] = array(
            '#type' => 'submit',
            '#value' => 'Save',
            '#attributes' => array('class' => 'SaveBtn'),
            '#prefix' => '<table cellpadding="0" cellspacing="0" width="100%" border="0">
      <tr><td></td><td align="left">

<h2 style="PADDING-BOTTOM: 0px; MARGIN: 0px; PADDING-LEFT: 0px; PADDING-RIGHT: 0px; COLOR: #3570aa; FONT-SIZE: 18px; PADDING-TOP: 0px">Edit Group Settings: Basic</h2></div>',
            '#suffix' => '</td></tr><tr>',
            '#submit' => array('group_form_submit_' . $tab),
        );
    } else {
        $form['main']['save'] = array(
            '#type' => 'item',
            '#value' => '',
            '#prefix' => '<table cellpadding="0" cellspacing="0" width="100%" border="0">
    <tr><td></td><td align="left"><DIV><A class=greenLink href="#">« Back to Group</A></DIV>
<DIV>
<H2 style="PADDING-BOTTOM: 0px; MARGIN: 0px; PADDING-LEFT: 0px; PADDING-RIGHT: 0px; COLOR: #3570aa; FONT-SIZE: 18px; PADDING-TOP: 0px">Edit Group Settings: Members</H2></DIV>',
            '#suffix' => '</td></tr><tr>',
        );
    }
    add_group_tabs($group_nid, $form['main'], $tab);

    $form['main']['form_start'] = array(
        '#type' => 'markup',
        '#value' => $HTMLform_group_setting_form_start);

    $form['main']['form'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
//            '#prefix' => $HTMLform_group_setting_form_start,
//            '#suffix' => $HTMLform_group_setting_form_end,
    );

    if (isset($view_state)) {
        $group_nid = $form_state['values']['group_node'];
        group_form_tab3_members($form_state, $form['main']['form'], $group_nid, $rec, true);
    } else {
        if ($tab == 'settings') {
            group_form_tab1_settings($form_state, $form['main']['form'], $group_nid, $rec);
        } elseif ($tab == 'members') {
            group_form_tab3_members($form_state, $form['main']['form'], $group_nid, $rec);
        }
    }
    $form['main']['form_end'] = array(
        '#type' => 'markup',
        '#value' => $HTMLform_group_setting_form_end,
        '#suffix' => '</tr></table>',
//
    );
    return $form;
}

/**
 * Form definition
 */
function group_form_tab1_settings($form_state, &$form, $group_nid, $rec) {
    global $user, $base_path;

    drupal_add_js(drupal_get_path('module', 'ntlp_group') . '/group_ahah_helper.js', 'module', 'footer');

    $school_nid = get_user_school($user->uid);


    if (isset($form_state['values']['course'])) {
        $course_nid = $form_state['values']['course'];
        watchdog('info', 'Get value of Course_NID' . $course_nid);
    }

    $group_moderator_teacherids = '0';
    if (!empty($group_nid) && isset($rec)) {
        if ($rec != false) {
            $group_full_title = $rec->title;
            $group_description = $rec->body;
            $group_moderator_teacherids = '';
            $group_moderator_teachernames = '';
            if ($rec_users = get_group_users($group_nid, 1)) {      //Lookup for Moderators
                while ($row = db_fetch_array($rec_users)) {
                    $node_rs = node_load($group_nid);

                    if ($rs_user = get_user($row['uid'])) {

                        if ($node_rs->uid == $rs_user->uid) {
                            $administrator = $rs_user->last_name . ', ' . $rs_user->first_name;
                        } else {
                            $group_moderator_teacherids .= '|' . $rs_user->uid;
                            $group_moderator_teachernames .= '<br />' . $rs_user->last_name . ', ' . $rs_user->first_name;
                        }
                    }
                }
                if (strlen($group_moderator_teacherids) > 1)
                    $group_moderator_teacherids = substr($group_moderator_teacherids, 1);
                if (strlen($group_moderator_teachernames) > strlen('<br />'))
                    $group_moderator_teachernames = substr($group_moderator_teachernames, strlen('<br />'));
            }
//            firep($group_moderator_teacherids);
//            firep($group_moderator_teachernames);
            if (strlen($group_moderator_teacherids) <= 0) {
                $group_moderator_teacherids = '0';            //To make the called url proper
            }
        }
    }


    $form['group_full_title'] = array(
        '#type' => 'textfield',
        '#required' => TRUE,
        '#default_value' => $group_full_title,
        '#prefix' => '</table><table cellpadding="5" class="table" cellspacing="0"><tr><th style="width:160px">' . t('Full Title') . '</th><td>',
        '#suffix' => '</td></tr>',
    );

    $form['group_description'] = array(
        '#type' => 'textarea',
        '#wysiwyg' => FALSE,
        '#cols' => 50,
        '#value' => $group_description,
        '#resizable' => False,
        '#required' => TRUE,
        '#prefix' => '<tr><th style="width:160px">' . t('Description') . '</th><td>',
        '#suffix' => '</td></tr>',
    );


    $pictures = get_group_image_file_path($group_nid);

//    $pictures = _get_fid_detail($school_rs->fid);

    $picture = $pictures->filepath;
    if ($picture == null || empty($picture)) {
        $picture = $base_path . 'themes/Boldr/Images/default/school-default.png';
    } else {
        $picture = $base_path . $picture;
    }

    $form['file_fid'] = array(
        '#type' => 'hidden',
        '#id' => 'file_fid',
        '#value' => (isset($rec)) ? $getImagePath->fid : '',
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div>',
    );

    if ($picture != null) {

        $testing = drupal_get_form('ntlp_activeimage', $picture, 0, 150);
        $testing = preg_match('@<form .*?>([\s\S]*)</form>@', $testing, $matches);
        $testing = $matches[1];
        $testing = str_replace('name="form_build_id"', '', $testing);
        $testing = str_replace('name="form_id"', '', $testing);
        $testing = str_replace('name="form_token"', '', $testing);

        $form['picture'] = array(
            '#type' => 'item',
//                '#id' => 'changeimage',
            '#value' => $testing,
            '#prefix' => '<tr><th>Group Image</th><td><div id="group_image" align="left"><div align="center" class="teacherImage">',
            '#suffix' => '</div></div></td></tr>',
        );
    } else {
        $form['picture'] = array(
            '#type' => 'item',
            '#value' => '<img src="' . $picture . '" width="150" alt="Picture of " />',
            '#prefix' => '<tr><th>Group Image</th><td><div id="group_image" align="left"><div align="center" class="teacherImage">',
            '#suffix' => '</div></div></td></tr>',
        );
    }


//    if(isset($getImagePath)) {
//        $picture = $base_path.$getImagePath->filepath;
//    }else {
//        $picture ='';
//    }
//
//    if($picture == null) {
//        $form['picture'] = array(
//                '#type' => 'item',
//                '#id' => 'changeimage',
//                '#value' => '<div id="group_image" align="left"></div>',
//                '#prefix'=>'<tr><th>Group Image</th><td>'.dlg('Attach Image','ntlp/image/uploader/edit',0,90),
//                '#suffix'=>'</td></tr>',
//        );
//    }else {
//        $form['picture'] = array(
//                '#type' => 'item',
//                '#value' => '<div id="group_image" align="left"><img src="'.$picture.'" width="80" height="80" alt="group_image" /></div>'.dlg('Change Image','ntlp/image/uploader/edit',0,90),
//                '#prefix'=>'<tr><th>Group Image</th><td>',
//                '#suffix'=>'</td></tr>',
//        );
//    }
//    $category_option = _get_group_category();
//    $form['category'] = array(
//            '#type' => 'select',
//            '#options' => $category_option,
//            '#default_value' => (isset($rec))  ? $rec->category_tid : $rec->category_tid,
//            '#prefix'=>'<tr><th>Category</th><td><span style="width:200px;" class="comboboxBgLeft">',
//            '#suffix'=>'</span></td></tr>',
//            '#attributes' => array('class' => 'my-dropdown'),
//    );
//

  watchdog('group_data 1', print_r($form_state, true));
    if (isset($user->roles[NTLP_ROLEID_STUDENT])) {
        $course_rs = get_user_courses($user->uid, false);
    } else if (!isset($user->roles[NTLP_ROLEID_PARENT])) {
        $course_rs = get_user_courses($user->uid, NTLP_ROLEID_TEACHER, false);
    }
//   $course_nids['0'] = 'Select Course';

    $course_nids['0'] = 'Select Course';
    if (!empty($course_rs)) {
        while ($data_rs = db_fetch_object($course_rs)) {

            $course_nids[$data_rs->course_nid] = $data_rs->title;
        }
    }
    if (isset($rec)) {

        $course_nid = $rec->course_nid;
    }




    $form['course'] = array(
        '#type' => 'select',
        //'#options' => $course_nids,
        '#options' => truncate_dropdown_data($course_nids, 25),
        '#default_value' => (!empty($rec)) ? $rec->course_nid : $course_nids,
        '#ahah' => array(
            'event' => 'change',
            // This is the "magical path". Note that the parameter is an array of
            // the parents of the form item of the wrapper div!
            'path' => ahah_helper_path(array('main', 'form', 'project_row', 'project_div')),
            'wrapper' => 'add-group-wrapper',
            'method' => 'replace',
            'progress' => 'none',
        ),
        '#prefix' => '<tr><th style="width:160px">' . t('Course') . '</th><td><span style="width:200px;" class="comboboxBgLeft">',
        '#suffix' => '</span></td></tr>',
        '#attributes' => array('class' => 'my-dropdown'),
    );

    $form['project_row'] = array(
        '#type' => 'item',
        '#prefix' => '<tr><th style="width:160px">' . t('Project') . '</th><td>',
        '#suffix' => '</td></tr>',
    );
    $form['project_row']['project_div'] = array(
        '#type' => 'item',
        '#prefix' => '<div id="add-group-wrapper">',
        '#suffix' => '</div>',
    );

    if (isset($course_nid) && $course_nid > 0) {
        $projectsdata = _get_load_projects($course_nid);

        $form['project_row']['project_div']['project_nid'] = array(
            '#type' => 'select',
            '#id' => 'group-project-nid',
            '#options' => truncate_dropdown_data($projectsdata, 25),
            '#default_value' => (isset($rec)) ? $rec->project_nid : $course_nids,
            '#prefix' => '<span style="width:200px;" class="comboboxBgLeft">',
            '#suffix' => '</span>',
            '#attributes' => array('class' => 'my-dropdown'),
        );
    }



    $perm_type = array(
        "0" => '<font style="color:#3570AA;font-weight:bold">SCHOOL PUBLIC: All students and staff in my school can join without my permission</font><br />',
        "1" => '<font style="color:#3570AA;font-weight:bold">SCHOOL RESTRICTED: All students and staff in the school can join, but they need to request permission first </font><br />',
        "2" => '<font style="color:#3570AA;font-weight:bold">NETWORK PUBLIC: Anyone in the Network can join without my permission</font><br />',
        "3" => '<font style="color:#3570AA;font-weight:bold">NETWORK RESTRICTED: Anyone in the Network can join, but they need to request permission first</font><br />',
        "4" => '<font style="color:#3570AA;font-weight:bold">PRIVATE: No one can access my group until I make them a member</font><br />');

    if (!isset($user->roles[NTLP_ROLEID_NETWORKMANAGER]) && !isset($user->roles[NTLP_ROLEID_NETWORKSTAFF])) {
        unset($perm_type['2']);
        unset($perm_type['3']);
    }

    $default_perm_type = _group_permissions($rec->showin_school_directory, $rec->showin_network_directory, $rec->is_closed);

    $form['display_group_in_school_directory'] = array(
        '#type' => 'radios',
        '#attributes' => array('style' => 'width:10px;'),
        '#options' => $perm_type,
        '#prefix' => '<tr><th style="width:160px">' . t('Permissions') . '</th><td class="Echo_GroupPermission">',
        '#suffix' => '</td></tr>',
        '#default_value' => isset($rec) ? $default_perm_type : variable_get('display_group_in_school_directory', 4),
    );

//    $form['access_rights'] = array(
//            '#type' => 'radios',
//            '#options' => array(OG_OPEN =>'Open (anyone can join)', OG_MODERATED => 'Closed (user must request to join)'),
//            '#attributes' => array('onclick' => 'onchange_rights(this)'),
//            '#default_value' => (isset($rec)) ? $rec->is_closed : '',
//            '#required' => TRUE,
//            '#prefix' => '<tr><th style="width:160px">'.t('Permissions').'</th><td>',
//            '#suffix' => '</td></tr>',
//    );
//
//
//
//
//
//    $form['showin_school_dir'] = array(
////            '#name' => 'group_dir_options',
//            '#type' => 'checkbox',
//            '#default_value' => (isset($rec)) ? $rec->showin_school_directory : 0,
//            '#title' => 'Display group in School Groups directory',
//            '#prefix' => '<tr><th></th><td>',
//            '#suffix' => '</td></tr>',
////            '#attributes' => array('style' => 'margin-left:2em;', 'disabled' => 'disabled'),  //Not required, after discussion with customer
//    );
//    $form['showin_network_dir'] = array(
////          '#name' => 'group_dir_options',
//            '#type' => 'checkbox',
//            '#default_value' => (isset($rec)) ? $rec->showin_network_directory : 0,
//            '#title' => 'Display group in Network directory',
//            '#prefix' => '<tr><th></th><td>',
//            '#suffix' => '</td></tr>',
////          '#attributes' => array('style' => 'margin-left:2em;', 'disabled' => 'disabled'),  //Not required, after discussion with customer
//
//    );

    $form['group_admin_heading'] = array(
        '#type' => 'item',
        '#value' => t('Administrator'),
        '#prefix' => '<tr><th style="width:160px">',
        '#suffix' => '</th>',
    );
    $form['group_admin'] = array(
        '#type' => 'item',
        '#value' => $administrator,
        '#prefix' => '<td style="width:160px">',
        '#suffix' => '</td></tr>',
    );

    $form['group_moderator_heading'] = array(
        '#type' => 'item',
        '#value' => t('School Moderator(s)'),
        '#prefix' => '<tr><th style="width:160px">',
        '#suffix' => '</th>',
    );

    $form['group_moderator_value'] = array(
        '#type' => 'item',
        '#value' => $group_moderator_teachernames,
        '#prefix' => '<td><div id="moderator_view" class="SelectSchoolModerator">',
        '#suffix' => '</div>',
    );
    $form['group_moderator_ids'] = array(
        '#type' => 'hidden',
        '#id' => 'group_moderator_ids',
        '#value' => $group_moderator_teacherids,
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div>',
    );

//    global $user;
    $school_nid = get_user_school($user->uid);
    $uids = str_replace('|', ',', $group_moderator_teacherids);
    cache_set("group_moderator_pplfinder_". session_id(), $uids, 'cache', CACHE_TEMPORARY);
    $form['group_moderator_pplfinder'] = array(
        '#type' => 'item',
        '#value' => dlg('Select School Moderator(s)', 'ntlp/people_finder/teacher_multi/' .
                $school_nid . '/' .
                $group_nid . '/' .
                "group_moderator_pplfinder_". session_id() . '/' .
                NTLP_ROLEID_TEACHER,
                350, 600),
        '#prefix' => '<div class="SelectSchoolModerator">',
        '#suffix' => '</div></td></tr>',
    );
}

function group_form_tab3_members($form_state, &$form, $group_nid, $rec, $ajax = false) {
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    modalframe_parent_js();

    $group_memberids = '0';
    if (!empty($group_nid) && isset($rec)) {
        if ($rec != false) {
            $group_memberids = '';
            if ($rec_users = get_group_users($group_nid, 0)) {      //Lookup for Regular members
                while ($row = db_fetch_array($rec_users)) {
                    if ($rs_user = get_user($row['uid'])) {
                        $group_memberids .= ',' . $rs_user->uid;
                    }
                }
                if (strlen($group_memberids) > 1)
                    $group_memberids = substr($group_memberids, 1);
            }
            if (strlen($group_memberids) <= 0) {
                $group_memberids = '0';            //To make the url call properly
            }
        }
    }

    include('ntlp_group.template.inc');
//    ahah_helper_register($form, $form_state);
//    $form['heading'] = array(
//            '#type' => 'item',
//            '#value' => t('Edit group Settings: members'),
//            '#prefix' => '<tr><td>',
//            '#suffix' => '</td></tr>',
//    );

    $form['members'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<tr><td><div id="members_view">',
        '#suffix' => '</div>',
    );

    global $user;
    $school_nid = get_user_school($user->uid);

    $form['members']['group_ids'] = array(
        '#type' => 'hidden',
        '#id' => 'group_moderator_ids',
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div>',
    );

    $form['members']['group_members'] = array(
        '#type' => 'hidden',
        '#id' => 'moderator_view',
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div>',
    );
    $admins = array();
    if ($rec_users = get_group_users($group_nid, 1)) {      //Lookup for Admins
        while ($row = db_fetch_array($rec_users)) {
            $admins[] = $row['uid'];
        }
    }
    $exclude_list = implode(',', array_merge(array($group_memberids), $admins));

    cache_set("pf_add_members_". session_id(), $exclude_list, 'cache', CACHE_TEMPORARY);
    $form['members']['add_members_link'] = array(
        '#type' => 'item',
        '#value' => '<span id="pplFndr">' . dlg('Add Members', 'ntlp/people_finder/enrollments/' .
                $school_nid . '/' .
                $group_nid . '/' .
                "pf_add_members_". session_id() . '/' .
                NTLP_ROLEID_STUDENT . '/1',
                350, 600) . '</span>',
        '#id' => 'pf_add_members',
        '#prefix' => '<div class="AddMembersBtn">',
        '#suffix' => '</div><br clear="all" />',
    );

    $form['members']['display_view'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#prefix' => '<div id="group_views">',
        '#suffix' => '</div>',
    );

//    $view = views_get_view('ntlp_group_members');
//    $view->set_arguments(array($group_nid));
    $header = array(
        array('data' => 'Name', 'style' => 'text-align: left;', 'field' => 'last_name'),
        array('data' => 'Number of Posts', 'style' => 'text-align: left;', 'field' => 'group_post_count'),
        array('data' => 'Joined Date', 'style' => 'text-align: center;', 'field' => 'created'),
        array('data' => ''),
        array('data' => ''),
    );
    $group = node_load($group_nid);
    $members = _get_group_members($group_nid, array(), $header);

    $rows = array();
    foreach ($members as $member) {
        $nameFieldExtra = $group->uid == $member->uid ? 'owner' : '';
        $nameFieldExtra .= ! empty($nameFieldExtra) ? ' + ' : '';
        $nameFieldExtra .= $member->is_admin ? 'admin' : '';
        $nameFieldExtra = !empty($nameFieldExtra) ? " ($nameFieldExtra)" : '';

        $adminLinks = '';
        $approveRemove = '';

        if ($group->uid != $member->uid) {
            $adminLinks .= $member->is_admin ? '<a onclick="onchange_admin(' . $group_nid . ', ' . $member->uid . ', 0)" class="jslink">Remove Admin</a>' : '<a onclick="onchange_admin(' . $group_nid . ', ' . $member->uid . ', 1)" class="jslink">Make Admin</a>';

            if ($member->is_active == 1) {
                $approveRemove = '<div style="float: left;">' . dlg('Remove', 'ntlp/group/reject/' . "$group_nid/$member->uid", 300, 400, 'SubmitBtn') . '</div>';
            } else {
                $approveRemove = '<div style="float: left;">' . dlg('Approve', 'ntlp/group/approve/' . "$group_nid/$member->uid", 300, 400, 'SubmitBtn') . '/' . dlg('Reject', 'ntlp/group/reject/' . "$group_nid/$member->uid", 300, 400, 'SubmitBtn') . '</div>';
            }
        }
        $rows[] = array(
            array('data' => l("$member->last_name, $member->first_name", "ntlp/user/profile/$member->uid") . $nameFieldExtra),
            array('data' => "$member->group_post_count", 'align' => 'center'),
            array('data' => get_tz($school_nid, NTLP_DATE_TZ_TIME, date(NTLP_DATE_TZ_SQL, $member->created)), 'align' => 'center'),
            array('data' => $adminLinks, 'align' => 'center'),
            array('data' => $approveRemove, 'align' => 'center'),
        );
    }

    $membersList = theme('table', $header, $rows, array('class' => 'ntlp_table', 'cellpadding' => 8, 'width' => '100%'));

    $form['members']['display_view']['enrolled'] = array(
        '#type' => 'item',
//            '#value' => $view->preview(),
        '#value' => $membersList,
    );
}

function add_group_tabs($group_nid, &$form, $tab = '') {
    include('ntlp_group.template.inc');
    if (empty($group_nid)) {
        $url = "/new/0";
    } else {
        $url = "/edit/$group_nid";
    }
    $form['tabs'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#prefix' => $HTMLform_group_setting_tabs_start,
        '#suffix' => $HTMLform_group_setting_tabs_end,
    );
    $roption = $HTMLform_group_settings_tabs_option;
    $roption = str_replace("@LINK", url("ntlp/groups$url"), $roption);
    $roption = str_replace("@PROMPT", "Basic", $roption);
    $roption = str_replace("@CLASS", ($tab != 'members') ? "class='active'" : "", $roption);
    $form['tabs']['basic'] = array(
        '#type' => 'markup',
        '#value' => $roption,
    );

    $roption = $HTMLform_group_settings_tabs_option;
    $roption = str_replace("@LINK", url("ntlp/groups/edit/$group_nid/members"), $roption);
    $roption = str_replace("@PROMPT", "Members", $roption);
    $roption = str_replace("@CLASS", ($tab == 'members') ? "class='active'" : "", $roption);
    $form['tabs']['members'] = array(
        '#type' => 'markup',
        '#value' => $roption,
    );
}

function save_group_picture($fid, $group_id) {
    global $base_path;


    watchdog('uploadimage', 'fid ' . $fid);
    $file_Obj = _get_file_detail($fid);
    db_query("UPDATE {ntlp_group} SET picture_fid = %d WHERE nid = %d", $file_Obj->fid, $group_id);

    watchdog('uploadimage', 'file updated successfully');
    echo drupal_get_form('ntlp_activeimage', $base_path . $file_Obj->filepath, 197, 150);
}

/**
 * Get array of id and title
 * @param array $data <p>
 * The array of data
 * </p>
 * 
 * @param int $length <p>
 * The integer to trucate title in characters.
 * </p>
 * @return array of data with trucated title as value and id
 */
function truncate_dropdown_data($data, $length) {
    $new_title = array();
    foreach ($data as $key => $value) {
        if (strlen($value) > $length) {
            $new_project_title = substr($value, 0, $length) . "...";
            $new_title[$key] = $new_project_title;
        } else {
            $new_title[$key] = $value;
        }
    }

    return $new_title;
}

?>
