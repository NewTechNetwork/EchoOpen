<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


function ntlp_group_add_popup_frm() {

    return drupal_get_form('ntlp_group_add_form');
}

//popup
function ntlp_group_add_form() {
    global $user;
    modalframe_child_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_group') . '/ntlp_group.js');

    // this form is also used for project page.. to add a group in project
    $project_nid = arg(3);

    $form['add_group'] = array(
        '#type' => 'fieldset',
        '#tree' => TRUE,
        '#prefix' => '<div>',
        '#suffix' => '</div>',
    );

    $form['add_group']['main_table'] = array(
        '#type' => 'item',
        '#tree' => TRUE,
        '#prefix' => '<div class="ntlp_people_finder">
      <div class="content">
        <div class="top_right">
        <div class="top_left">
        <div class="bot_left">
            <div class="bot_right_2">
<table cellpadding="5" align="center">',
        '#suffix' => '</table></div>
          </div></div>
        </div>
      </div>
    </div>',
    );

    $school_nid = get_this_school();

    $form['add_group']['school_nid'] = array(
        '#type' => 'hidden',
        '#value' => $school_nid,
    );

    $form['add_group']['project_nid'] = array(
        '#type' => 'hidden',
        '#value' => $project_nid,
        '#id' => 'hidden_project_nid',
    );


    $form['add_group']['main_table'] ['title'] = array(
        '#type' => 'textfield',
        '#attributes' => array('style' => 'width: 350px'),
        '#prefix' => '<tr><th style="color:#3570AA;text-align:left">Group Title</th><td>',
        '#suffix' => '</td></tr>',
        '#id' => 'txt_group_title',
    );

    if (!isset($user->roles[NTLP_ROLEID_TEACHER]) && !isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])
            && !isset($user->roles[NTLP_ROLEID_NETWORKSTAFF]) && !isset($user->roles[NTLP_ROLEID_SCHOOLLEADER])
            && !isset($user->roles[NTLP_ROLEID_SITEADMIN])) {

        $moderator_option = _get_moderator();
        $form['add_group']['main_table'] ['school_moderator'] = array(
            '#type' => 'select',
            '#attributes' => array('style' => 'width: 350px'),
            '#options' => $moderator_option,
            '#prefix' => '<tr><th style="color:#3570AA;text-align:left">School Moderator</th><td>',
            '#suffix' => '</td></tr>',
            '#id' => 'combo_school_moderator',
        );
    }

    $perm_type = array(
        "0" => '<font style="color:#3570AA;font-weight:bold">SCHOOL PUBLIC: All students and staff in my school can join without my permission</font><br />',
        "1" => '<font style="color:#3570AA;font-weight:bold">SCHOOL RESTRICTED: All students and staff in the school can join, but they need to request permission first </font><br />',
        "2" => '<font style="color:#3570AA;font-weight:bold">NETWORK PUBLIC: Anyone in the Network can join without my permission</font><br clear="all">',
        "3" => '<font style="color:#3570AA;font-weight:bold">NETWORK RESTRICTED: Anyone in the Network can join, but they need to request permission first</font><br />',
        "4" => '<font style="color:#3570AA;font-weight:bold">PRIVATE: No one can access my group until I make them a member</font><br />');

    if (!isset($user->roles[NTLP_ROLEID_NETWORKMANAGER]) && !isset($user->roles[NTLP_ROLEID_NETWORKSTAFF])) {
        unset($perm_type['2']);
        unset($perm_type['3']);
    }
    $form['add_group']['main_table']['display_group_in_school_directory'] = array(
        '#type' => 'radios',
        '#options' => $perm_type,
        '#prefix' => '<tr><td colspan="2" class="Echo_GroupPermission" style="color:#3570AA; text-align:left;">',
        '#suffix' => '</td></tr>',
        '#default_value' => variable_get('display_group_in_school_directory', 4),
        '#attributes' => array('style' => 'vertical-align:top;  height: 13px !important; margin-right: 20px;'),
    );


    $form['add_group']['main_table']['empty'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => '<tr><td></td><td>',
        '#suffix' => '</td></tr>',
    );

    $form['add_group']['main_table']['create_btn'] = array(
        '#type' => 'item',
        '#value' => l('Create', '', array('attributes' => array('onclick' => 'create_group(' . $school_nid . '); return false;
                ', 'class' => 'echo_crete_group_btn'))),
        '#prefix' => '<tr><td colspan="2"><div style="width:156px;">',
        '#suffix' => '',
    );
    $form['add_group']['main_table']['cancel_btn'] = array(
        '#type' => 'item',
        '#value' => l('Cancel', '', array('attributes' => array('onclick' => 'parent.Drupal.modalFrame.close();return false;', 'class' => 'CancelBtn', 'style' => 'padding: 4px 5px 5px 10px; font-weight: normal !important;'))),
        '#prefix' => '',
        '#suffix' => '</div></td></tr>',
    );

    return $form;
}

function ntlp_group_add_group($title, $school_moderator, $group_perm) {
    global $user;

    $project_nid = 0; // initialize with zero for insert query if no project_nid define

    $project_nid = arg(6);
    watchdog('add_group', 'a group creating in project_nid ' . $project_nid . ' ' . print_r(arg(), true));

    // Category become 0 because category are removed as per request.

    $category = 0;

    if ($group_perm == 0) {

        $membership_type = 0;
        $school_directory = 1;
        $network_directory = 0;
    } elseif ($group_perm == 1) {

        $membership_type = 1;
        $school_directory = 1;
        $network_directory = 0;
    } elseif ($group_perm == 2) {

        $membership_type = 0;
        $school_directory = 0;
        $network_directory = 1;
    } elseif ($group_perm == 3) {

        $membership_type = 1;
        $network_directory = 1;
        $school_directory = 0;
    } elseif ($group_perm == 4) {

        $membership_type = 1;
        $network_directory = 0;
        $school_directory = 0;
    }

    $school_nid = get_this_school();

    $nodetype = 'ntlp_group';
    $dateformat = "%Y-%m-%d %H:%M:%S";
    $time = strftime($dateformat, strtotime(date('Y-m-d h:i:s')));



    $group_node = new stdClass();
//    $group_node->changed = $time;
//    $group_node->created = $time;
    $group_node->status = 1;
    $group_node->promote = 0;
    $group_node->sticky = 0;
    $group_node->format = 1;       // Filtered HTML
    $group_node->uid = $user->uid; // UID of content owner
    $group_node->title = $title;
    $group_node->type = $nodetype;   // Your specified content type
//  Need to test Group Type and permissions in detail
//    $group_node->og_private = 1;
//    $group_node->og_selective = $membership_type;

    node_save($group_node);
    $group_nid = $group_node->nid;


    if (!empty($group_nid)) {

        db_query("INSERT INTO {ntlp_group}(nid, school_nid, course_nid, project_nid, picture_fid,
                    is_closed, showin_school_directory, showin_network_directory, category_tid)
            VALUES(%d, %d, 0, %d, 0, %d, %d, %d, %d)", $group_nid, $school_nid, $project_nid, $membership_type,
                $school_directory, $network_directory, $category);
    }

    //Add Group creator as member and admin
//    echo "$group_nid, $user->uid, " . print_r(array('is_active' => 1, 'is_admin' => 1), true);
    ntlp_og_save_subscription($group_nid, $user->uid, array('is_active' => 1, 'is_admin' => 1));

    //Add Group moderator as member and admin
    if ($group_nid && !empty($school_moderator) && ($school_moderator != $user->uid)) {
        ntlp_og_save_subscription($group_nid, $school_moderator, array('is_active' => 1, 'is_admin' => 1));

        $group_mod[] = $school_moderator;
        ntlpgroupactivity_group('create', $group_nid, $group_mod, $user->uid, null);

        $group_node->group_admins = $group_mod;
        $group_node->context = $group_nid;
        module_invoke_all('ntlpcontent', $group_node, 'student_group_created');
    }

    ntlp_update_block_cache('ntlp_group', $group_nid);

//    echo $group_nid;
//    print ntlp_groups_content(false, array('USER_ID' => $user->uid));
//    print ntlp_groups_content();
}

function _get_group_category() {
    $vocab_data = _get_lib_vocabulary_vid("NTLP Group Category");
    $result = db_query("SELECT * FROM {term_data} WHERE vid = %d", $vocab_data->vid);

    while ($data = db_fetch_object($result)) {
        $group_category[$data->tid] = $data->name;
    }

    return $group_category;
}

function _get_moderator() {

    $result = db_query("SELECT nu.*, ur.* FROM {users_roles} ur
        INNER JOIN {ntlp_user} nu ON  ur.uid = nu.uid
        WHERE rid = %d AND nu.school_nid = %d", NTLP_ROLEID_TEACHER, get_this_school());


    while ($data = db_fetch_object($result)) {
        $group_moderator[$data->uid] = $data->first_name . ' ' . $data->last_name;
    }
    return $group_moderator;
}

?>