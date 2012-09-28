<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');

function projects_block_contents() {
    include "projects.template.inc";
    global $user, $base_url;
    $user_id = $user->uid;

    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    modalframe_parent_js();


    set_item_url_handler('Projects');

    if ($courseid == 0) {
        //Getting Query String From URL
        $courseid = 0;
        $url = $_GET['q'];
        $args = explode('/', $url);
        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i]))
                $courseid = $args[$i];
        }
    }


    $user_role = check_user_permission($courseid);

    watchdog('project_tab', 'user role on this page ' . $user_role);
//    print_r($user_records);
    $view_projects = t('You do not have the required permissions.');
    $display_id = 'default';
    $output = $HTMLcontainer;

    if (isset($user_role) && $user_role == NTLP_ROLEID_TEACHER
            || $user_role == NTLP_ROLEID_NETWORKMANAGER
            || $user_role == NTLP_ROLEID_SITEADMIN
            || $user_role == NTLP_ROLEID_SCHOOLLEADER
            || $user_role == NTLP_ROLEID_SITEGUEST
            || $user_role == NTLP_ROLEID_NETWORKSTAFF) {


        // STUDENT ACTIVITY FILTER START
        $FilterOpts = array(
            'Status' => array(
                'All' => array(
                    'type' => 'checkbox',
                    'group' => 'status',
                    'value' => 'all',
                    'name' => 'all',
                    'default' => true,
                ),
                'Published' => array(
                    'type' => 'checkbox',
                    'group' => 'status',
                    'value' => 'published',
                    'name' => 0,
                ),
                'Hidden' => array(
                    'type' => 'checkbox',
                    'group' => 'status',
                    'value' => 'hidden',
                    'name' => 1,
                ),
            ),
        );

        if (!user_access('edit project')) {
            unset($FilterOpts['Status']['All']);
            unset($FilterOpts['Status']['Hidden']);
            $FilterOpts['Status']['Published']['default'] = TRUE;
        }

        $opts = render_custom_filter(
                        $FilterOpts,
                        array('start' => $HTML_courses_project_filter_cellStart, 'end' => $HTML_courses_project_filter_cellEnd),
                        array('template' => $HTML_courses_project_filter_heading, 'placeholder' => '@HEADING'),
                        $HTML_courses_project_filter_row_end,
                        url('ntlp/courses/project/filter/' . $courseid),
                        'ntlpCoursesProjectTable'
        );

        $opts = str_replace('@OPTIONS', $opts, $HTML_courses_project_filter);
        $output = str_replace('@FILTER_PLACEHOLDER', $opts, $output);
    } else {
        $output = str_replace('@FILTER_PLACEHOLDER', '', $output);
    }
    // STUDENT ACTIVITY FILTER END
//    watchdog('project_contens', 'views_get_view '. 'ntlp_projects before called');

    $view_projects = "<div>Projects view</div>";

    if (isset($user_role) && $user_role == NTLP_ROLEID_TEACHER
            || $user_role == NTLP_ROLEID_NETWORKMANAGER
            || $user_role == NTLP_ROLEID_SITEADMIN
            || $user_role == NTLP_ROLEID_SCHOOLLEADER
            || $user_role == NTLP_ROLEID_NETWORKSTAFF) {

        $view = views_get_view('ntlp_projects');
        $view_projects = $view->preview($display_id, array($courseid));
        $output = str_replace('@LINK_NEW', dlg("New", 'ntlp/project/add/' . $courseid, 300, 500), $output);
    } else {
        $view = views_get_view('ntlp_projects_stu');
        $view_projects = $view->preview($display_id, array($courseid, 'S'));
        $view = views_get_view('ntlp_projects');
        $output = str_replace('@LINK_NEW', "", $output);
    }
//    if (isset($view)) {
//
//        if (user_access('edit project')) {
//
//        } else {
//
//        }
//    }
//    watchdog('project_contens', 'views_get_view '. 'ntlp_projects after called');
    $output = str_replace('@PROJECTS_VIEW', '<div id="ntlpCoursesProjectTable">' . $view_projects . '</div>', $output);
    return $output;
}

function ntlp_courses_project_filter($courseid) {

    $view = views_get_view('ntlp_projects');
    $statusTypes = array(
        'published' => 'S',
        'hidden' => 'H',
    );

    $status = 'all';

    if (!isset($_REQUEST['status']['all']) and count($_REQUEST['status']) == 1) {
        foreach ($_REQUEST['status'] as $s) {
            if (in_array($s, array_keys($statusTypes)))
                $status = $statusTypes[$s];
        }

//        $view->display['default']->display_options['filters'] = array (
//                'availability' => array (
//                        'operator' => '=',
//                        'value' => $status,
//                        'table' => 'ntlp_course',
//                        'field' => 'availability',
//                ),
//        );
    }
    echo $view->preview('default', array($courseid, $status));
}

function _view_all_active_projects() {

    include('projects.template.inc');
    $output = $HTML_All_Active_Projects;

    $url = $_GET['q'];
    $args = explode('/', $url);
    for ($i = 0; $i < count($args); $i++) {
        if (is_numeric($args[$i]) && empty($courseid))
            $courseid = $args[$i];
    }

    $rs = get_course_active_projects($courseid);

    $dateformat = NTLP_DATE_TZ_MJY;
    while ($plist = db_fetch_object($rs)) {

        if (strlen($plist->title) > 28) {
            $truc_text = '...';
        } else {
            $truc_text = '';
        }
        
        $project_list .= '<li><a href="?q=ntlp/courses/projects/' . $courseid . '/' . $plist->nid . '" >' . substr($plist->title, 0, 28) . $truc_text . '</a>
            <br> <font style="font-family: verdana; font-size: 75%; color: #3570AA;"> Duration: ' . get_tz_course($courseid,$dateformat, ($plist->from_date)) . ' - ' . get_tz_course($courseid,$dateformat, ($plist->to_date)) . ' </font></li>';
    }

    if ($project_list == '')
        $project_list = '<div style="margin:10px">No active projects</div>';

    $output = str_replace('@LIST_PROJECTS', $project_list, $output);

    return $output;
}

function _ntlp_library_landing_page() {
    global $user;

    include('projects.template.inc');

    $output = $HTML_library_landing_page;

    $user_role = get_user_role($user->uid);

    set_item_url_handler("Library");


    if ($user_role->rid == NTLP_ROLEID_STUDENT || $user_role->rid == NTLP_ROLEID_PARENT) {
        $output = str_replace('@DISPLAY1', 'style="width:270px"', $output);
        $output = str_replace('@DISPLAY2', 'style="width:270px;display:none"', $output);
        $output = str_replace('@DISPLAY3', 'style="width:270px;display:none"', $output);
//        $output = str_replace('@NETWORK_RESOURCE_LIB_LINK', 'https://share.nthls.com/NetworkResources/documentlibrary.nsf?open', $output);
//        $output = str_replace('@NETWORK_RESOURCE_LIB_LINK', '?q=ntlp/resource/library/', $output);

        $output = str_replace('@1WIDTH','width="290"', $output);
//        $output = str_replace('@3WIDTH','width="290"', $output);
        
    }elseif($user_role->rid == NTLP_ROLEID_TEACHER || $user_role->rid == NTLP_ROLEID_SITEADMIN
            || $user_role->rid == NTLP_ROLEID_SCHOOLLEADER ) {
        $output = str_replace('@DISPLAY1','style="width:270px"', $output);
        $output = str_replace('@DISPLAY2','style="width:270px"', $output);
        $output = str_replace('@DISPLAY3', 'style="width:270px"', $output);
//        $output = str_replace('@NETWORK_RESOURCE_LIB_LINK', 'https://share.nthls.com/NetworkResources/documentlibrary.nsf?open', $output);
        $output = str_replace('@NETWORK_RESOURCE_LIB_LINK', '?q=ntlp/resource/library/', $output);

        $output = str_replace('@1WIDTH','width="290"', $output);
        $output = str_replace('@2WIDTH','width="290"', $output);
        $output = str_replace('@3WIDTH','', $output);


    } elseif ($user_role->rid == NTLP_ROLEID_NETWORKMANAGER || $user_role->rid == NTLP_ROLEID_NETWORKSTAFF ) {

        $output = str_replace('@DISPLAY1', 'style="width:270px"', $output);
        $output = str_replace('@DISPLAY2', 'style="width:270px"', $output);
        $output = str_replace('@DISPLAY3', 'style="width:270px"', $output);
        $output = str_replace('@NETWORK_RESOURCE_LIB_LINK', '?q=ntlp/resource/library/', $output);

        $output = str_replace('@1WIDTH', 'width="290"', $output);
        $output = str_replace('@2WIDTH', 'width="290"', $output);
        $output = str_replace('@3WIDTH', 'width="290"', $output);
    } else {
        $output = str_replace('@DISPLAY1', 'style="width:270px;display:none"', $output);
        $output = str_replace('@DISPLAY2', 'style="width:270px;display:none"', $output);
        $output = str_replace('@DISPLAY3', 'style="width:270px;display:none"', $output);
        $output = str_replace('@1WIDTH', 'width="290"', $output);
        $output = str_replace('@2WIDTH', 'width="290"', $output);
        $output = str_replace('@3WIDTH', 'width="290"', $output);
    }

    return $output;
}
