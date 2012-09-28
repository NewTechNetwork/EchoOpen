<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');

function ntlp_group_tabs() {
    include "grouptabs.template.inc";
    global $user;
    $output = $HTMLgroup_tab;
    return $output;
}

function ntlp_proejct_library_tabs() {

    include "grouptabs.template.inc";
    global $user;
    $output = $HTMLproject_library_tab;
    return $output;
}

function ntlp_group_resource_tabs() {

    include "grouptabs.template.inc";

    if ($group_id == 0) {
//Getting Query String From URL
        $project_nid = 0;
        $group_id = 0;
        $url = $_GET['q'];
        $args = explode('/', $url);
        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i]) && empty($group_id))
                $group_id = $args[$i];
            elseif (is_numeric($args[$i]) && empty($project_nid))
                $project_nid = $args[$i];
        }
    }

//    echo 'aslkdfals;dfjasdf';
//    echo 'user role'.$userRole;
    $userRole = check_user_permission($group_id);

    set_item_url_handler('Group Resources');

    $view = views_get_view('ntlp_resources');
    $view->set_arguments(array($group_id));

    $output = $HTMLgroup_resource_tab;



//    $output = ntlp_resource_content($group_id);
    if ($userRole != NTLP_ROLEID_SITEGUEST) {

        $output = str_replace('<!-- ADD_RESOURCE_BUTTON -->', '<div class="Project-Btns">' . l('Add Resource', 'ntlp/group/resource/new/' . intval($group_id)) . '</div>', $output);
    }

    $output = str_replace('@GROUP_RESOURCE', $view->render(), $output);

    return $output;
}

//function ntlp_group_start_tab() {
//    include "grouptabs.template.inc";
//
//    $group_id = 0;
//    $url = $_GET['q'];
//    $args = explode('/', $url);
//    for ($i = 0;
//    $i < count($args);
//    $i++) {
//        if (is_numeric($args[$i]) && empty($group_id))
//            $group_id = $args[$i];
//    }
//
//    $associated_project = $row->course_short_title;
//    $associated_project_heading = 'Associated Project';
//
//    if($associated_project == null || empty($associated_project)){
//        $associated_project = '';
//        $associated_project_heading = '';
//    }
//    $output = $HTMLgroup_start_tab;
//    $row = get_group_block_detail($group_id);
//    $node = node_load($group_id);
//    $picture = $base_path.$row->filepath;
//    $group_teacher = get_group_user_detail($row->teacher1_uid);
//    $output = str_replace('@TITLE', $node->title, $output);
//    $output = str_replace('@DESCRIPTION', $node->body, $output);
//    $output = str_replace('@ASSOCIATED_PROJECT_HEADING', $associated_project_heading, $output);
//    $output = str_replace('@ASSOCIATED_COURSES', $associated_project, $output);
//    $output = str_replace('@NAME', l($group_teacher->first_name.' '.$group_teacher->last_name,'ntlp/profile/user/'.$row->teacher1_uid), $output);
//    $output = str_replace('@ASSOCIATED_PROJECT', $row->title, $output);
//
//    return $output;
//}

function ntlp_group_members_tabs($isAjax = false, $group_id = 0, $filters = array()) {
    global $user;

    include_once "filter_functions.inc";
    include "grouptabs.template.inc";

    modalframe_parent_js();

    if ($group_id == 0) {
        foreach (explode('/', $_GET['q']) as $q) {
            if (is_numeric($q)) {
                $group_id = $q;
            }
        }
    }

    set_item_url_handler('Group Members');

    $output = $HTMLgroup_member_tab;

    if ($group_id) {

        if (!$isAjax) {
            // STUDENT ACTIVITY FILTER START
//            $filterOpts = array(
//                    'Member Type' => array(
//                            'All' => array(
//                                    'type'=>'checkbox',
//                                    'group'=>'type',
//                                    'value' => 1,
//                                    'name' => 'all',
//                                    'default' => true,
//                            ),
//                            'Members' => array(
//                                    'type'=>'checkbox',
//                                    'group'=>'type',
//                                    'value' => 'members',
//                                    'name' => 0,
//                            ),
//                            'Pending Members' => array(
//                                    'type'=>'checkbox',
//                                    'group'=>'type',
//                                    'value' => 'pending_members',
//                                    'name' => 1,
//                            ),
//                    ),
//            );
//            $opts = render_custom_filter(
//                    $filterOpts,
//                    array('start' => $HTMLgroup_member_filter_cellStart, 'end' => $HTMLgroup_member_filter_cellEnd),
//                    array( 'template' => $HTMLgroup_member_filter_heading, 'placeholder' => '@HEADING' ),
//                    $HTMLgroup_member_filter_row_end,
//                    '?q=ntlp/groups/member/filter/'.$group_id,
//                    'ntlpGroupsMemberTable'
//            );
//            $opts = str_replace('@OPTIONS', $opts, $HTMLgroup_member_filter);
//            $output = str_replace('@FILTER_PLACEHOLDER', $opts, $output);
            $output = str_replace('@FILTER_PLACEHOLDER', '', $output);
        } else {
            $output = '';
        }
        // STUDENT ACTIVITY FILTER END

        $head = array(
            array('data' => 'Name', 'field' => 'last_name', 'sort' => 'asc'),
            array('data' => 'Organization', 'field' => 'school_name', 'sort' => 'asc'),
            array('data' => 'Type', 'field' => 'role_name', 'sort' => 'asc'),
            array('data' => ''),
        );

        $memberType = array('Pending Member', 'Member');

        $filters['DISINCLUDE'] = array($user->uid);

        //Show only 100 records and show a link "Show All", which loads all records if clicked
        $total_members = get_total_members($group_id);
        $showalllink = false;
        if ($total_members->total_members > 100) {
            $showall = $_GET['all'];
            if (isset($_GET['all']) && $showall == '1') {
        $groupMembers = _get_group_members($group_id, $filters);
            } else {
                $showalllink = true;
                $groupMembers = _get_group_members($group_id, $filters, array(), 100);
            }
        } else {
            $groupMembers = _get_group_members($group_id, $filters);
        }
        if ($groupMembers) {
            $rows = array();
            foreach ($groupMembers as $member) {
                $is_active = (int) $member->is_active;
//                echo $is_active.'<br>';
                if (!$member->is_active && is_user_group_admin($group_id, $user)) {
                    $actionBtns = '<div style="float: left;">' . dlg('Approve', 'ntlp/group/approve/' . "$group_id/$member->uid", 300, 400, 'SubmitBtn') . '</div>' .
                            '<div style="float: left;margin-top:2px">' . dlg('Reject', 'ntlp/group/reject/' . "$group_id/$member->uid", 300, 400, 'SubmitBtn') . '</div>';
                } else {

                    if ($member->is_active && is_user_group_admin($group_id, $user))
                        $actionBtns = '<div style="float: left;">' . dlg('Remove', 'ntlp/group/reject/' . "$group_id/$member->uid", 300, 400, 'SubmitBtn') . '</div>';
                    else 
                        $actionBtns = ' ';
                }

                $school_data = get_user_school_name($member->uid);

                $rows[] = array(
                    array('data' => l($member->last_name . ', ' . $member->first_name, "ntlp/user/profile/$member->uid")),
                    array('data' => $school_data->school_name),
                    array('data' => $school_data->role_name),
                    array('data' => "<div class=\"creategroup_Btn\">$actionBtns</div>", 'style' => 'width: 220px;'),
                );
            }
        }
        if (!$rows) {
            $rows[] = array(
                array('data' => '<strong>No record found.</strong>', 'colspan' => 3, 'align' => 'center')
            );
        }

        $memberTable = '<br clear="all" /><div id="ntlpGroupsMemberTable">' . theme_table($head, $rows, array('class' => 'ntlp_table', 'width' => '100%', 'cellpadding' => '8')) . '</div>';
        if (!$isAjax) {
            $output = str_replace('@MEMBERS_LIST_PLACEHOLDER', $memberTable, $output);
        } else {
            $output = $memberTable;
        }

        if ($showalllink) {
                $output .= '<div style="float:left; padding: 10px 0 10px 0;">'.
                    l('Show All >>', "ntlp/groups/member/$group_id",
                    array('attributes' => array('style' => 'float: right;color:#090;'),
                          'query' => array('all' => 1))).
                    '</div>';
        }
        return $output;
    }
}

function ntlp_group_member_approve($group_nid, $uid) {
    modalframe_child_js();

    //update block cache block for users


    db_query('UPDATE {og_uid} SET is_active = 1 WHERE nid = %d AND uid = %d', $group_nid, $uid);

    ntlp_update_block_cache('ntlp_group', $group_nid);

    $output .= '<div style="width: auto;" id="BoldrProject">
                    <div class="ProjectDetail">
                            <div class="LightestBlueBottomRightCorner">
                                    <div class="LightestBlueBottomLeftCorner">
                                            <div class="LightestBlueTopRightCorner">
                                                    <div class="LightestBlueTopLeftCorner">
                                                            Member Approved';

    $output .= '                                            <div style="display:table" align="right"><br />
                                                                <input type="button" value="Close" class="PostBtn" onclick="parent.Drupal.modalFrame.close(); parent.location.reload();" />
                                                            </div>
                                                     </div>
                                            </div>
                                    </div>
                            </div>
                    </div>
                </div>';

    return $output;
}

function ntlp_group_member_reject($group_nid, $uid) {
    modalframe_child_js();

    $is_active = db_fetch_object(db_query('SELECT is_active FROM {og_uid} WHERE nid = %d AND uid = %d', $group_nid, $uid))->is_active;

    db_query('DELETE FROM {og_uid} WHERE nid = %d AND uid = %d', $group_nid, $uid);
    ntlp_update_block_cache('ntlp_group', $group_nid);
    
    $output .= '<div style="width: auto;" id="BoldrProject">
                    <div class="ProjectDetail">
                            <div class="LightestBlueBottomRightCorner">
                                    <div class="LightestBlueBottomLeftCorner">
                                            <div class="LightestBlueTopRightCorner">
                                                    <div class="LightestBlueTopLeftCorner">
                                                            Member ' . ($is_active == 1 ? 'Removed' : 'Rejected');

    $output .= '                                            <div style="display:table" align="right"><br />
                                                                <input type="button" value="Close" class="PostBtn" onclick="parent.Drupal.modalFrame.close(); parent.location.reload();" />
                                                            </div>
                                                     </div>
                                            </div>
                                    </div>
                            </div>
                    </div>
                </div>';

    return $output;
}

function ntlp_groups_member_filter($group_id) {
    $filters = array();
    if (!isset($_POST['type']['all']) and count($_POST['type']) > 0) {
        foreach ($_POST['type'] as $type) {
            if ($type == 'members') {
                $filters['ACTIVE'] = true;
            } else if ($type == 'pending_members') {
                $filters['INACTIVE'] = true;
            }
        }
    }
    print ntlp_group_members_tabs(true, $group_id, $filters) . '
        <script type="text/javascript">
            Drupal.behaviors.modalFrameSetup();
        </script>
    ';
}