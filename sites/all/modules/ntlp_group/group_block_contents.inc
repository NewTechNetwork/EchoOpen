<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_group', 'data_access');

function _get_ntlp_group_project_detail_contents() {
    global $base_path;

    include('ntlp_group.template.inc');

//Getting Query String From URL
    $group_id = 0;
    $url = $_GET['q'];
    $args = explode('/', $url);
    for ($i = 0; $i < count($args); $i++) {
        if (is_numeric($args[$i]) && empty($group_id))
            $group_id = $args[$i];
    }

    $output = $HTMLform_group_project_detail_block;
    $row = get_group_block_detail($group_id);

    $node = node_load($group_id);

    $picture = $row->filepath;

    if ($picture == null || empty($picture)) {
        $picture = $base_path . 'themes/Boldr/Images/default/school-default.png';
    } else {
        $picture = $base_path . $picture;
    }

    $group_teacher = get_group_user_detail($node->uid);
    $output = str_replace('@TITLE', $node->title, $output);
    $output = str_replace('@PICTURE', $picture, $output);
    $output = str_replace('@DESCRIPTION', $node->body, $output);

    $course_title = $row->coursetitle;
    if ($course_title == null) {

        $output = str_replace('@ASSOCIATION', '', $output);
        $output = str_replace('@COURSEHEADING', '', $output);
        $output = str_replace('@PROJECTHEADING', '', $output);
        $output = str_replace('@COURSE', '', $output);
        $output = str_replace('@NAME', '', $output);
        $output = str_replace('@PROJECT', '', $output);
    } else {

        $output = str_replace('@ASSOCIATION', 'Associations', $output);
        $output = str_replace('@COURSEHEADING', 'Course:', $output);
        $output = str_replace('@PROJECTHEADING', 'Project:', $output);
        $output = str_replace('@COURSE', $row->coursetitle, $output);
        $output = str_replace('@NAME', '(' . l($group_teacher->first_name . ' ' . $group_teacher->last_name, 'ntlp/user/profile/user//' . $row->teacher1_uid) . ')', $output);
        $output = str_replace('@PROJECT', $row->projectname, $output);
    }
    return $output;
}

function _get_ntlp_group_activity_contents() {
    global $base_path;
    include('ntlp_group.template.inc');

//Getting Query String From URL
    $group_id = 0;
    $url = $_GET['q'];
    $args = explode('/', $url);
    for ($i = 0; $i < count($args); $i++) {
        if (is_numeric($args[$i]) && empty($group_id))
            $group_id = $args[$i];
    }

    $output = $HTMLform_group_activity_block;


    $total_members = get_total_members($group_id);


//    $total_resources = get_total_resources($group_id);
    $total_post = get_total_posts($group_id);

//    echo $total_resources->total_resources;

    $output = str_replace('@TITLE', 'Group Activity', $output);
    $output = str_replace('@BASE_PATH', $base_path, $output);

    $output = str_replace('@TOTAL_MEMBERS', $total_members->total_members, $output);
//    $output = str_replace('@TOTAL_RESOURCES', $total_resources->total_resources, $output);
    $output = str_replace('@TOTAL_POSTS', $total_post->total, $output);


    return $output;
}

function _get_ntlp_group_admin_contents() {

    global $base_path;
    include('ntlp_group.template.inc');

//Getting Query String From URL
    $group_id = 0;
    $url = $_GET['q'];
    $args = explode('/', $url);
    for ($i = 0; $i < count($args); $i++) {
        if (is_numeric($args[$i]) && empty($group_id))
            $group_id = $args[$i];
    }

    $output = $HTMLform_group_admin_block;

    $total_admins = get_group_moderators($group_id);
    $node = node_load($group_id);
    while ($row = db_fetch_object($total_admins)) {
        if ($node->uid == $row->uid) {
            $owner = 'owner';
        } else {

            $owner = '';
        }

        $user_detail = get_group_user_detail($row->uid);
        $user_data = $user_detail->picture;

        if ($user_data != null && !empty($user_data)) {
            $admin_picture = $base_path . $user_detail->picture;
        } else {

            $admin_picture = $base_path . 'themes/Boldr/Images/common/person_image_not_found.jpg';
        }

        $admin_detail .= '<div class="Adminspacer">&nbsp;</div>
            <div class="AdminMainPanel">
            <div class="AdminPersonImg">
                <img src="' . $admin_picture . '" width="32px" alt="Admin Image 1" />
            </div>
            <div class="AdminPersonNameAndOwner">
                <div class="AdminPersonName">' . l($user_detail->first_name . ' ' . $user_detail->last_name, 'ntlp/user/profile/' . $row->uid) . '</div>
                <div class="AdminPersonOwner">' . $owner . '</div>
            </div>
            </div>';
    }

    $output = str_replace('@ADMIN_DETAIL', $admin_detail, $output);
    $output = str_replace('@BASE_PATH', $base_path, $output);

    return $output;
}

function _get_ntlp_group_link_contents() {
    global $user, $base_path;
    $group_nid = arg(3);

    include('ntlp_group.template.inc');
    modalframe_parent_js();

    $output = $HTMLform_group_link_block;

    $result = db_query('SELECT * FROM {ntlp_group_resource} WHERE group_nid = %d', $group_nid);

    $links = '';
    if ($result->num_rows) {
        while ($link = db_fetch_object($result)) {
            $linkNode = node_load($link->nid);
            $links .= '<li>' . l($linkNode->title, $link->path, array('attributes' => array('target' => '_blank'))) . '</li>';
        }
    }


    $output = str_replace('<!-- LINKS -->', $links, $output);
    $output = str_replace('@BASE_PATH', $base_path, $output);

    $rs = get_group_admins($group_nid);

//    if( $userRole != NTLP_ROLEID_SITEGUEST ) {
    if (is_user_group_admin($group_nid, $user)) {
        $output = str_replace('<!-- EDIT_LINK -->', dlg('Edit>>', 'ntlp/group/links/edit/' . $group_nid, 0, 500), $output);
    }


    return $output;
}

function _ntlp_group_links_edit_content($group_nid) {
    modalframe_child_js();

    drupal_add_js(drupal_get_path('module', 'ntlp_group') . '/ntlp_group.js');

    return drupal_get_form('groups_links_popup_form', $group_nid);
}

function groups_links_popup_form($form_state, $group_nid) {
    $form['groups'] = array(
        '#type' => 'fieldset',
    );
    $form['groups']['link'] = array(
        '#type' => 'item',
        '#id' => 'mainwrapper',
        '#prefix' => '<div style="height: 220px; overflow-y: auto;">',
        '#suffix' => '</div>',
    );

    $result = db_query('SELECT * FROM {ntlp_group_resource} WHERE group_nid = %d', $group_nid);

    for ($i = 0; $i < $result->num_rows; $i++) {
        $link = db_fetch_object($result);
        $linkNode = node_load($link->nid);
        $form['groups']['link']['old'][$i] = array(
            '#type' => 'item',
            '#value' => '',
            '#prefix' => '<div class="ntlp_people_finder">
  <div class="content">
    <div class="top_right">
      <div class="top_left">
        <div class="bot_left">
          <div class="bot_right_2" style="padding-top:0px;"><div style="padding:5px;">
                            <div style="clear: both; font-size: 12px; font-weight: bold; margin-bottom: 10px;color:#3570AA">' . 'Link ' . ($i + 1)
            . '<span style="float:right"><a class="ntlp_delete_link ui-dialog-titlebar-close" onclick="$(\'#group_nid_x\').val(\'' . $link->nid . ',\'+$(\'#group_nid_x\').val());$(this.parentNode.parentNode.parentNode).remove();">&nbsp;</a></span>
                            </div>',
            '#suffix' => '</div></div></div></div></div></div></div><div>&nbsp;</div>',
        );

        $form['groups']['link']['old'][$i]['title'][$link->nid] = array(
            '#type' => 'textfield',
            '#size' => 30,
            '#name' => "title[$link->nid]",
            '#value' => isset($form_state['post']['title'][$link->nid]) ? $form_state['post']['title'][$link->nid] : $linkNode->title,
            '#prefix' => '<div style="float: left; width: 70px;color:#3570AA">Link Name:</div> <div style="float: left;">',
            '#suffix' => '</div>
                          <div style="clear: both;font-size:5px">&nbsp;</div>',
        );

        $form['groups']['link']['old'][$i]['url'][$link->nid] = array(
            '#type' => 'textfield',
            '#size' => 30,
            '#name' => "url[$link->nid]",
            '#value' => isset($form_state['post']['url'][$link->nid]) ? $form_state['post']['url'][$link->nid] : $link->path,
            '#prefix' => '<div style="float: left; width: 70px;color:#3570AA">URL:</div> <div style="float: left;">',
            '#suffix' => '</div>
                          <div style="clear: both;"></div>
                         ',
        );
    }

    $totalNewLinks = count($form_state['post']['title']['new']) + 1;
    for ($ii = 0; $ii < $totalNewLinks; $ii++) {
        $form['groups']['link']['new'][$ii] = array(
            '#type' => 'item',
            '#value' => '',
            '#prefix' => '<div class="ntlp_people_finder">
  <div class="content">
    <div class="top_right">
      <div class="top_left">
        <div class="bot_left">
          <div class="bot_right_2" style="padding-top:0px;"><div style="padding:5px;">
                            <div style="clear: both; font-size: 12px; font-weight: bold; margin-bottom: 10px;color:#3570AA;">' . 'Link ' . ($i + $ii + 1)
            . '<span style="float:right"><a class="ntlp_delete_link ui-dialog-titlebar-close" onclick="$(\'#group_nid_x\').val(\'' . $link->nid . ',\'+$(\'#group_nid_x\').val());$(this.parentNode.parentNode.parentNode).remove();">&nbsp;</a></span>
                            </div>',
            '#suffix' => '</div> </div>
        </div>
      </div>
    </div>
  </div>
</div><div>&nbsp;</div>',
        );

        $form['groups']['link']['new'][$ii]['title'][] = array(
            '#type' => 'textfield',
            '#size' => 30,
            '#name' => 'title[new][]',
            '#value' => isset($form_state['post']['title']['new'][$ii]) ? $form_state['post']['title']['new'][$ii] : '',
            '#prefix' => '<div style="float: left; width: 70px;color:#3570AA;">Link Name:</div> <div style="float: left;">',
            '#suffix' => '</div>
                          <div style="clear: both;font-size:5px;">&nbsp;</div>',
        );

        $form['groups']['link']['new'][$ii]['url'][] = array(
            '#type' => 'textfield',
            '#size' => 30,
            '#name' => 'url[new][]',
            '#value' => isset($form_state['post']['url']['new'][$ii]) ? $form_state['post']['url']['new'][$ii] : '',
            '#prefix' => '<div style="float: left; width: 70px;color:#3570AA;">URL:</div> <div style="float: left;">',
            '#suffix' => '</div>',
        );
    }

    $form['groups']['group_nid'] = array(
        '#type' => 'hidden',
        '#value' => $group_nid,
    );

    $form['groups']['group_nid_x'] = array(
        '#type' => 'hidden',
        '#id' => 'group_nid_x',
    );

    $form['groups']['dublicate_row'] = array(
        '#type' => 'button',
        '#button_type' => 'button',
        '#value' => 'add link',
        '#attributes' => array('class' => 'echo_crete_group_btn'),
    );
    $form['groups']['submit'] = array(
        '#type' => 'submit',
        '#value' => 'done',
        '#attributes' => array('onclick' => 'parent.Drupal.modalFrame.close();parent.location.reload();', 'class' => 'echo_crete_group_btn'),
    );

    return $form;
}

function groups_links_popup_form_submit($form, $form_state) {
    global $user;
//    print_r($form_state);
    $postedData = $form_state['clicked_button']['#post'];
    $group_nid = $postedData['group_nid'];
    $oldIds = array_keys($postedData['title']);
    unset($oldIds[array_search('new', $oldIds)]);
    $delIds = rtrim($postedData['group_nid_x'], ',');
    if ($delIds) {
        $delIds = explode(',', $delIds);
    }

//    print_r($postedData);
// UPDATE OLD LINKS
    if ($oldIds) {
        foreach ($oldIds as $oldId) {
            db_query(
                    "UPDATE {ntlp_group_resource} SET path = '%s' WHERE group_nid = %d AND nid = %d",
                    $postedData['url'][$oldId],
                    $group_nid,
                    $oldId
            );

            $old = node_load($oldId);
            $old->title = $postedData['title'][$oldId];
            node_save($old);
        }
    }

// DELETE SELECTED LINKS
    if ($delIds) {
        db_query('DELETE FROM {ntlp_group_resource} WHERE nid IN(' . implode(',', $delIds) . ') AND group_nid = %d', $group_nid);
        foreach ($delIds as $nid) {
            node_delete($nid);
        }
    }

// ADD NEW LINKS
    if (isset($postedData['title']['new'])) {
        $totalNewLinks = count($postedData['title']['new']);
        for ($n = 0; $n < $totalNewLinks; $n++) {
            if (!empty($postedData['title']['new'][$n]) && !empty($postedData['url']['new'][$n])) {
                $new = new stdClass();
                $new->title = $postedData['title']['new'][$n];
                $new->uid = $user->uid;
                $new->type = 'lo_link';
                $new->description = '';

                node_save($new);
                if ($new->nid) {
                    db_query(
                            "INSERT INTO {ntlp_group_resource}(deleted_by, is_shared, type, nid, group_nid, path) VALUES(0, 0, 'L', %d, %d, '%s')",
                            $new->nid,
                            $group_nid,
                            $postedData['url']['new'][$n]
                    );
                }
            }
        }
    }
}

function ntlp_group_resources_filter() {
    include 'ntlp_group.template.inc';
    $group_nid = intval(arg(3));

//  ADDING FILTER START
    $options = array(
        'Status' => array(
            'All' => array(
                'type' => 'checkbox',
                'group' => 'resource_status',
                'value' => 1,
                'name' => 'all',
                'default' => true,
            ),
            'Flagged' => array(
                'type' => 'checkbox',
                'group' => 'resource_status',
                'value' => 1,
                'name' => 'flagged',
            ),
        ),
        'Type' => array(
            'All' => array(
                'type' => 'checkbox',
                'group' => 'resource_type',
                'value' => 1,
                'name' => 'all',
                'default' => true,
            ),
            'Links' => array(
                'type' => 'checkbox',
                'group' => 'resource_type',
                'name' => 0,
                'value' => 'links',
            ),
            'Images' => array(
                'type' => 'checkbox',
                'group' => 'resource_type',
                'name' => 1,
                'value' => 'images',
            ),
            'Videos' => array(
                'type' => 'checkbox',
                'group' => 'resource_type',
                'name' => 2,
                'value' => 'videos',
            ),
            'Documents' => array(
                'type' => 'checkbox',
                'group' => 'resource_type',
                'name' => 3,
                'value' => 'documents',
            ),
            'Pages' => array(
                'type' => 'checkbox',
                'group' => 'resource_type',
                'name' => 4,
                'value' => 'pages',
            ),
        ),
    );

    $opts = render_custom_filter(
                    $options,
                    array('start' => $HTMLGroupResources_filter_cellStart, 'end' => $HTMLGroupResources_filter_cellEnd),
                    array('template' => $HTMLGroupResources_filter_heading, 'placeholder' => '@HEADING'),
                    $HTMLGroupResources_filter_row_end,
                    url('ntlp/group/resource/filter/' . $group_nid),
                    'ntlp_group_resource_content'
    );

    $output = str_replace('@OPTIONS', $opts, $HTMLGroupResources_filter);


//    $form['group_resource'] = array(
//            '#type' => 'fieldset',
//            '#tree' => TRUE,
//    );
//
//    $form['group_resource']['resource_filter'] = array(
//            '#type' => 'item',
//            '#value' => $HTMLGroupResourcesFilter,
//    );
//
//    $form['group_resource']['resource_table'] = array(
//            '#type' => 'item',
//            '#value' => get_group_resource_list($group_id),
//    );
//    $form['group_resource']['resource_table'] = array(
//            '#type' => 'item',
//            '#value' => all_notification(),
//    );
//    get_group_resource_list($group_id),
    set_item_url_handler('Group Resources');
//    return drupal_render($form);
    return $output;
}

function ntlp_group_resources_filter_callback($group_nid) {
    $filter = array();

    if (!isset($_REQUEST['resource_type']['all'])) {
        if (isset($_REQUEST['resource_type']) and is_array($_REQUEST['resource_type'])) {
            foreach ($_REQUEST['resource_type'] as $type) {
                $filter['type'][] = $type;
            }
        }
    }
    print get_group_resource_list($group_nid, $filter);
}

function get_group_resource_list($group_nid = 0, $filter = array()) {
    global $user;
    if ($group_nid == 0) {
        $group_nid = intval(arg(3));
    }

    include "grouptabs.template.inc";

    $userRole = check_user_permission($group_nid);

    $output = $HTMLgroup_resource_tab;
//    $output = ntlp_resource_content($group_id);

    if ($userRole != NTLP_ROLEID_SITEGUEST) {

        $output = str_replace('<!-- ADD_RESOURCE_BUTTON -->', '<div style="float:right" class="creategroup_Btn">' . l('Add Resource', 'ntlp/group/resource/new/' . intval($group_nid)) . '</div>', $output);
    }

    $dateformat = NTLP_DATE_TZ_SHORT;
    $head = array(
        array('data' => t('Posted'), 'field' => 'created', 'sort' => 'asc'),
        array('data' => t('Resource'), 'field' => 'title', 'sort' => 'asc'),
        array('data' => t('Description')),
        array('data' => t('Posted By'), 'field' => 'uid', 'sort' => 'asc'),
    );

    $types = array(
        'links' => 'lo_link',
        'images' => 'lo_image',
        'videos' => 'lo_video',
        'documents' => 'lo_document',
        'pages' => 'lo_page',
    );

    $where = '';
    if (isset($filter['type']) and is_array($filter['type'])) {
        $selectedTypes = array();
        foreach ($filter['type'] as $type) {
            if (isset($types[$type])) {
                $selectedTypes[] = "'$types[$type]'";
            }
        }
        if ($selectedTypes) {
            $where .= ' AND n.type IN (' . implode(',', $selectedTypes) . ')';
        }
    }
    $sql = "SELECT g.school_nid, cr.*, nr.title, nr.body, n.created, n.uid FROM {ntlp_course_resource} cr
        INNER JOIN {node} n ON cr.nid = n.nid
        INNER JOIN {node_revisions} nr ON cr.nid = nr.nid
        INNER JOIN {ntlp_group} g ON g.nid = $group_nid
        WHERE cr.deleted_on IS NULL AND cr.course_nid = " . $group_nid . " $where " . tablesort_sql($head);

    $limit = 5;
    $result = pager_query($sql, $limit);
    
    while ($resource = db_fetch_object($result)) {
        $user_Object = _get_user_detail($resource->uid);

        
        $rows[] = array(
            array('data' => get_tz($resource->school_nid, $dateformat, date(NTLP_DATE_TZ_SQL, $resource->created))),
            array('data' => '<a href="' . url('ntlp/groups/resource/view/' . $group_nid . '/' . $resource->nid) . '">' . $resource->title . '</a>'),
            array('data' => $resource->body),
            array('data' => $user_Object->first_name . ' ' . $user_Object->last_name),
        );
    }

    $output .= theme('table', $head, $rows, $attributes = array('class' => 'ntlp_table', 'width' => '100%', 'cellpadding' => '5'));

    $output .= theme('pager', NULL, $limit, 0);

    return $output;
}

function _get_user_detail($user_uid) {
    $result = db_query("SELECT * from {ntlp_user} WHERE uid = %d", $user_uid);
    return db_fetch_object($result);
}

function ntlp_group_filter($allGroup) {
    global $user;

//    drupal_add_js(drupal_get_path('module', 'ntlp_group') . '/filter_ntlp_group.js');
    include_once 'filter_functions.inc';
    include 'group_filter.template.inc';

    //Show "My School Group" checkbox as default for "My Groups" tab
    $isMyTab = arg(2) == 'my';

    if (isset($user->roles[NTLP_ROLEID_PARENT])) {
        $group_filter_content = str_replace('@OPTIONS', '', $HTMLGroupFilter);
    } else {

        if (isset($user->roles[NTLP_ROLEID_STUDENT])) {
            $options = array(
                'Type' => array(
                    'All' => array(
                        'type' => 'checkbox',
                        'group' => 'type',
                        'value' => 1,
                        'name' => 'all',
                        'default' => TRUE,
                    ),
//                    "{$myTabLabel}School Groups" => array(
                    "My School Groups" => array(
                        'type' => 'checkbox',
                        'group' => 'type',
                        'value' => 1,
                        'name' => 'school_group',
                    ),
                    'Groups I Administer' => array(
                        'type' => 'checkbox',
                        'group' => 'type',
                        'name' => 'administer',
                        'value' => 1,
                    ),
                ),
//                '#autoUncheckDefault' => $isMyTab ? false : true,
            );
        } else {

            $options = array(
                'Type' => array(
                    'All' => array(
                        'type' => 'checkbox',
                        'group' => 'type',
                        'value' => 1,
                        'name' => 'all',
                        'default' => TRUE,
                    ),
//                    "{$myTabLabel}School Groups" => array(
                    "My School Groups" => array(
                        'type' => 'checkbox',
                        'group' => 'type',
                        'value' => 1,
                        'name' => 'school_group',
                    ),
                    'Network Groups' => array(
                        'type' => 'checkbox',
                        'group' => 'type',
                        'value' => 1,
                        'name' => 'network_groups',
                    ),
                    'Groups I Administer' => array(
                        'type' => 'checkbox',
                        'group' => 'type',
                        'name' => 'administer',
                        'value' => 1,
                    ),
                ),
//                '#autoUncheckDefault' => $isMyTab ? false : true,
            );
        }

        $headingStart = array('template' => '<tr><td class="StatusHeading" style="white-space:nowrap">@HEADING</td><td>', 'placeholder' => '@HEADING');
        $rowEnd = '</td></tr>';

        $opts = render_custom_filter(
                        $options,
                        array('start' => '<div class="ChkBox">', 'end' => '</div>'),
                        $headingStart,
                        $rowEnd,
                        '?q=ntlp/group/filter/' . ($allGroup ? 'all' : 'my' ) . '&ajax=1',
                        'groups_table'
        );
        $group_filter_content = str_replace('@OPTIONS', $opts, $HTMLGroupFilter);
    }
    return $group_filter_content;
}

function ntlp_groups_content($allGroups = false, $filter = array()) {
    global $user;
    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_group') . '/ntlp_group.js');

    $mainmenu = menu_get_item("ntlp/groups/my");
    menu_set_item(NULL, $mainmenu);

    $header = array(
        array('data' => 'Group', 'field' => 'title', 'sort' => 'asc'),
        array('data' => 'Description'),
        array('data' => 'Access Type'),
        array('data' => 'Organization', 'width' => '100', 'field' => 'school_name', 'sort' => 'asc'),
        array('data' => 'Moderators', 'width' => '100'),
        array('data' => 'Members', 'width' => '50', 'field' => 'total_members', 'sort' => 'asc'),
        array('data' => 'New Posts', 'width' => '50', 'field' => 'total_resources', 'sort' => 'asc'),
        array('data' => ''),
    );

    $output = '';

    $rows = array();
    $limit = 15;

    $ajaxCall = $_GET['ajax'];

    if ($allGroups) {
        $groups = _get_groups_details($filter, $header, $limit);
    } else {
        $filter['USER_ID'] = $user->uid;
        if (!isset($_GET['ajax'])) {
        }
        $groups = _get_groups_details($filter, $header, $limit);
    }
    watchdog("group_filter", print_r($filter, true));
    if ($groups) {

        foreach ($groups as $group) {
            $is_member = is_group_member($group->nid, $user->uid);
            if (!is_user_group_admin($group->nid, $user)) {
                $actionButton = $is_member ? dlg('Un-Join', 'ntlp/group/unjoin/' . $group->nid, 300, 400, 'anchorLink') : dlg('Join', 'ntlp/group/join/' . $group->nid, 300, 400, 'anchorLink');
            } else {
                $actionButton = l('Administer', 'ntlp/groups/edit/' . $group->nid, array('attributes' => array('class' => 'anchorLink')));
            }

            // Showing the Access Type
            $access_type = check_group_permissions($group->showin_school_directory, $group->showin_network_directory, $group->is_closed);

            //Do not show Group if its private and user is not member of this group
            if (!isset($user->roles[NTLP_ROLEID_SITEADMIN]) && !isset($user->roles[NTLP_ROLEID_SCHOOLLEADER])) {
                if ($access_type == 'Private' && !$is_member)
                    continue;
            }

            $total_posts = get_total_resources($group->nid);

            $group_moderators = array();
            $mod_rs = get_group_moderators($group->nid, true);

            while ($row = db_fetch_object($mod_rs)) {
                $user_detail = get_group_user_detail($row->uid);
                $group_moderators[] = l($user_detail->first_name . ' ' . $user_detail->last_name, 'ntlp/user/profile/' . $row->uid);
            }
            $group_moderators = implode(",<br>", $group_moderators);
            
            $rows[] = array(
                array('data' => l($group->title, 'ntlp/groups/home/' . $group->nid)),
                array('data' => $group->group_title),
                array('data' => $access_type),
                array('data' => $group->school_name),
                array('data' => $group_moderators),
                array('data' => (int) $group->total_members, 'align' => 'center'),
                array('data' => $total_posts->total_resources, 'align' => 'center'),
                array('data' => "<div class=\"creategroup_Btn\">$actionButton</div>", 'align' => 'center'),
            );
        }
    } else {
        $rows[] = array(
            array('data' => 'No matching groups were found', 'colspan' => '8', 'style' => 'text-align:center; font-weight: bold;'),
        );
    }

    $attributes = array('class' => 'ntlp_table', 'width' => '100%', 'cellpadding' => '8');

    $output = theme('table', $header, $rows, $attributes);
    $output .= theme('pager', null, $limit);


    if ($ajaxCall == 1) {

        return $output;
    } else {

        return '<div id="groups_table"><div style="width: auto;" id="ActivitiesCSS"><div class="BlueBorderBgTopCenter"><div class="BlueBorderBgMiddleRight"><div class="BlueBorderBgBottomCenter"><div class="BlueBorderBgMiddleLeft"><div class="BlueBorderBgTopRightCorner"><div class="BlueBorderBgBottomLeftCorner"><div class="BlueBorderBgBottomRightCorner"><div class="BlueBorderBgTopLeftCorner "><div class="echo_BlueBorderContent">'
//          .theme_table($header, $rows, $attributes)
        . $output
        . '</div></div></div></div></div></div></div></div></div></div></div>';
    }
}

function ntlp_group_filter_callback($group = 'my') {
    global $user;
    $filter = array();

    if (!isset($_REQUEST['type']['all'])) {
        if (isset($_REQUEST['type']['school_group'])) {
            $filter['SCHOOL_GROUPS'] = true;
        }

        if (isset($_REQUEST['type']['network_groups'])) {
            $filter['NETWORK_GROUPS'] = true;
        }

        if (isset($_REQUEST['type']['administer'])) {
            $filter['USER_IS_ADMIN'] = $user->uid;
        }
    }



    print ntlp_groups_content(($group == 'my' ? false : true), $filter);
    exit();
}

function ntlp_group_join($group_id) {
    global $user;
    modalframe_child_js();
    $output = '';
    $group = node_load($group_id);

    $rs = db_query('select is_closed from {ntlp_group} where nid = %d', $group_id);
    $data = db_fetch_object($rs);
    $is_closed = $data->is_closed;

    $rs_joined = db_query('select nid from {og_uid} where nid = %d and uid = %d', $group_id, $user->uid);
    if ($rs_joined->num_rows <= 0) {

        db_query("insert into {og_uid} (nid, og_role, is_active, is_admin, uid, created, changed)
            values
            (%d, %d, %d, %d, %d, UNIX_TIMESTAMP(), UNIX_TIMESTAMP())", $group_id, 0, !$is_closed, 0, $user->uid);

        if ($is_closed) {
            $output .= " Your request to join this group has been sent to the Group Admin";
            $group_admins = array();
            $ga_rs = get_group_admins($group_id);
            while ($uid = db_fetch_object($ga_rs))
                $group_admins[] = $uid->uid;

            ntlpgroupactivity_group('request', $group_id, $group_admins, $user->uid, null);

            $group->group_admins = $group_admins;
            $group->requester = $user->uid;
            $group->context = $group_id;
            module_invoke_all('ntlpcontent', $group, 'group_membership_request');
        } else {
            $output .= '<div style="width: auto;" id="BoldrProject">
            <div class="ProjectDetail">
                <div class="LightestBlueBottomRightCorner">
                    <div class="LightestBlueBottomLeftCorner">
                        <div class="LightestBlueTopRightCorner">
                            <div class="LightestBlueTopLeftCorner">' . "You're Welcome to this Group: " . l($group->title, "ntlp/groups/home/$group_id");
        }
    } else {
        $output .= " Your request to join this group has already been sent to the Group Admin";
    }
    $output .= '<div style="display:table" align="right"><br />
                    <input type="button" value="Close" class="PostBtn" onclick="parent.Drupal.modalFrame.close(); parent.location.reload();" />
                        </div></div>
                    </div>
                </div>
            </div>
        </div></div>';
    return $output;
}

function ntlp_group_unjoin($group_nid) {
    global $user;
    modalframe_child_js();
    $output = '';

    db_query('DELETE FROM {og_uid} WHERE nid = %d AND uid = %d', $group_nid, $user->uid);

    $output .= '<div style="width: auto;" id="BoldrProject">
	<div class="ProjectDetail">
		<div class="LightestBlueBottomRightCorner">
			<div class="LightestBlueBottomLeftCorner">
				<div class="LightestBlueTopRightCorner">
					<div class="LightestBlueTopLeftCorner">
                                        You have successfully un-joined the group.
				';
    $output .= '<div style="display:table" align="right">
                    <br /><input type="button" value="Close" class="PostBtn" onclick="parent.Drupal.modalFrame.close(); parent.location.reload();" />
                </div></div>
			</div>
		</div>
	</div>
</div></div>';

    return $output;
}