<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_courses_activity', 'data_access');
module_load_include('inc', 'ntlp_blocks', 'default_header.contents');

//This method is called from themes/Boldr/module_header.inc
function ntlp_courses_render_header() {
    drupal_add_css(drupal_get_path('theme', 'Boldr') . '/StylishSelect/stylish-select.css', 'module', 'all', FALSE);
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/externalFile.js');
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/jquery.stylish-select.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_blocks') . '/default_header.js');

    include "courses_header.template.inc";

    global $user, $base_path;
    $user_id = $user->uid;

//Getting Query String From URL
    $courseid = 0;
    $url = $_GET['q'];
    $args = explode('/', $url);
    for ($i = 0; $i < count($args); $i++) {
        if (is_numeric($args[$i])) {
            $courseid = $args[$i];

            if (isset($user->roles[NTLP_ROLEID_PARENT])) {
                $student_id = $args[$i + 1];
            }

            break;
        }
    }

    $user_role = check_user_permission($courseid);

//    $user_role =  check_user_permission($courseid );
//    watchdog("course_home", "course home user role ".$user_role);

    $rec = get_course($courseid, $user_role);
    $term_name = "In-active";
    $term_tid = 0;
//    if ($r = get_current_school_term($rec->school_nid)) {
    if ($r = get_current_course_term($courseid)) {
        $row = db_fetch_object($r);
        $term_name = $row->name;
        $term_tid = $row->school_term_tid;
    }

    $selectors = '';
    $rs_user = get_user($rec->teacher1_uid);
    $rs_user2 = get_user($rec->teacher2_uid);

    $tname = $rs_user->first_name . ' ' . $rs_user->last_name;
    $tname2 = '';
    if ($rec->teacher1_uid != $rec->teacher2_uid && $rs_user2) {
        $tname2 = $rs_user2->first_name . ' ' . $rs_user2->last_name;
    }

    $container = $HTMLcontainer;
    $container = str_replace('@LINK_HOME', l('Home', 'ntlp/home'), $container);
    $container = str_replace('@LINK_MYCOURSES', l('Courses', 'ntlp/courses/my'), $container);

    if (in_array($args[2], array('teaching', 'training', 'all', 'open'))) {
//        $selectors .= get_school_control();
        // get_parent_child

//        if (user_access("access student selector")) {
//
//            $selectors .= str_replace('<!-- student_selector_combo -->', get_parent_child(), $HTMLStudentSelectorCombo);
//        }

        $HTMLSelectorsAndHeading = str_replace('<!-- selectors -->', $selectors, $HTMLSelectorsAndHeading);
        $HTMLSelectorsAndHeading = str_replace('<!-- heading -->', 'Courses', $HTMLSelectorsAndHeading);

        $container = str_replace('<!-- selectors_and_heading -->', $HTMLSelectorsAndHeading, $container);
    }

    if ($rec) {
        // If course exists then display course header
        $output = $HTMLcourse_tools;
        if (strlen($rec->title) > 35)
            $ctitle = substr($rec->title, 0, 35) . "...";
        else
            $ctitle = $rec->title;
        $output = str_replace('@COURSE_NAME', $ctitle, $output);
        $output = str_replace('@COURSE_PERIOD', get_period_title(intval($rec->period)), $output);
//        $output = str_replace('@COURSE_TERM', $term_name, $output);
        $tname = "<a href='" . url("ntlp/user/profile/$rec->teacher1_uid") . "'>" . $tname . "</a>";
        $tname2 = !empty($tname2) ? " & <a href='" . url("ntlp/user/profile/$rec->teacher2_uid") . "'>" . $tname2 . "</a>" : '';
        $output = str_replace('@COURSE_TEACHER', $tname . $tname2, $output);

        //Set the course's current term as default term, if no or invalid term is selected
        $selected_term = get_current_selected_course_term();
        if (!is_term_in_course($courseid, $selected_term)) {
//          print '<br>Selected Term: '.print_r($selected_term, true);
//          print '<br>Term not in course, changing it to:'.$term_tid;
            change_course_term($term_tid);
        }
        //drupal_set_message("fff");
//            $selectors = str_replace('<!-- term_selector_combo -->', get_school_term_combo(), $HTMLTermSelectorCombo);

        $HTMLSelectorsAndHeading = str_replace('<!-- selectors -->', $selectors, $HTMLSelectorsAndHeading);

        $container = str_replace('<!-- term_selector -->', $HTMLSelectorsAndHeading, $container);
        //     }

        if (isset($user_role)) {
            if ($user_role == NTLP_ROLEID_TEACHER
                    || $user_role == NTLP_ROLEID_NETWORKMANAGER
                    || $user_role == NTLP_ROLEID_SITEADMIN
                    || $user_role == NTLP_ROLEID_SCHOOLLEADER
                    || $user_role == NTLP_ROLEID_NETWORKSTAFF) {
                $output .= $HTMLcourse_manage;
                $output = str_replace('@MANAGE_COURSE_EDIT', l('Edit Course Settings', 'ntlp/courses/edit/' . $courseid), $output);
                $output = str_replace('@MANAGE_COURSE_COPY', dlg('Copy Course', 'ntlp/course/copy/popup/' . $courseid, 450, 600, 'anchorLink'), $output);

                // delete course commented because it have to be fix first
                $output = str_replace('@MANAGE_COURSE_DELETE', dlg('Delete Course', 'ntlp/course/delete/popup/' . $courseid, 450, 600, 'anchorLink'), $output);
                $output = str_replace('@MANAGE_COURSE_DELETE', '', $output);
            }
        }
        $container = str_replace('@COURSE_TOOLS', $output, $container);
    }


//    echo $args[2];
    if (in_array($args[2], array('teaching', 'training', 'all'))) {
//        $school_nid = get_this_school();
        $school_nid = get_user_school($user->uid);

//        echo 'school nid '.$school_nid;
//        watchdog("course_header", 'checking permission for '.$user->uid .' and school nid'.$school_nid.' user role '.print_r($user->roles, true));
        if ($school_nid > 0) {
            $user_role = check_user_permission($school_nid);
        } else {
            drupal_goto('ntlp/render/error/page/You are not enrolled in any School please Contact with Site Administrator', array('destination' => $_GET['q']));
        }

        $container = str_replace('@COURSE_TOOLS', $output, $container);


    } else {
        $container = str_replace('@COURSE_TOOLS', '', $container);
        $container = str_replace('@CREATE_COURSE', '', $container);
    }
//    $rClink = '';
//    if ($all == TRUE && user_access('create ntlp_course content')) {
//        $rClink = $HTMLnewlink;
//        $rClink = str_replace('@LINK_NEW', l('Create New Course', 'ntlp_admincourse'), $rClink);
//    }
    return $container;
//    }else {
//        drupal_set_message("you're not allowed to view this page..");
//    }
}

function get_period_title($period) {
    $name = "";
    switch ($period) {
        case 1:
            $name = '1st';
            break;
        case 2:
            $name = '2nd';
            break;
        case 3:
            $name = '3rd';
            break;
        case 4:
            $name = '4th';
            break;

        default:
            $name = $period ? $period . 'th' : '*';
            break;
    }
    $name .= ' Period';
    return $name;
}

//function _get_school_term() {
//
//    db_query("SELECT td.tid,td.name FROM {vocabulary} tv
//        INNER JOIN {term_data} td on tv.vid = td.vid
//        WHERE tv.name = 'NTLP School Term'");
//}
//
//function nav_home_render_header() {
//    return "ntlp/HOME";
//}
//
//This method is called from themes/Boldr/module_header.inc
//Displays no tabs
function ntlp_courses_render_tabs($active_tab = "NOP", $course_nid = 0) {
    global $user;
    include "courses_header.template.inc";

//    $mainmenu = menu_get_item("ntlp/courses/my");
//    menu_set_item(NULL, $mainmenu);

    if ($course_nid == 0) {
        $url = $_GET['q'];
        $args = explode('/', $url);
        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i])) {
                $course_nid = $args[$i];
                break;
            }
        }
    }


    if (isset($user->roles[NTLP_ROLEID_PARENT])) {  // if parent come here show their student record
        $user_role = NTLP_ROLEID_PARENT;
        $user_id = $_SESSION['PARENT_STUDENT_UID'];
        if (!check_user_exist_in_course($user_id, $course_nid, true)) {
            drupal_goto("ntlp/courses/my");
        }
//            is_user_access_this_course($node, $user);
    }

//    $today = date('Y/m/j', time());

    $today = get_tz_course($course_nid, 'Y/m/d', date('Y-m-d H:i:s', time()));

    $home_url = url("/ntlp/courses/home/$course_nid");
    $agenda_url = url("/ntlp/courses/agenda/$course_nid/$today");
    $act_url = url("ntlp/courses/activity/$course_nid");
    $projects_url = url("ntlp/courses/project/$course_nid");

//    $result =  db_query("SELECT tid FROM `dpl_forum` f, dpl_og_ancestry o where f.nid = o.nid and group_nid = 1 group by tid");
//    $forum_Obj = db_fetch_object($result);
//    $forum_url = url("forum/$forum_Obj->tid");
    $forum_url = url("ntlp/courses/forum/$course_nid");
    $resource_url = url("ntlp/courses/resource/$course_nid");
    $grade_url = url("ntlp/courses/grade/$course_nid");

//    'course_nid '.$course_nid;

    if ($active_tab == "NOP") {
        $get = $_GET['q'];
        $br = explode("/", $get);
        $cnt = count($br);
//        $active_tab = "@IS_".strtoupper($br[$cnt-2]);
        $active_tab = "@IS_" . strtoupper(arg(2));
//        $tab = $br[$cnt-1];
    }



    foreach ($br as $obj) {
        if ($obj == 'activity') {
            $active_tab = "@IS_ACTIVITY";
        } else if ($obj == 'projects') {
            $active_tab = "@IS_PROJECT";
        } else if ($obj == 'agenda') {
            $active_tab = "@IS_EVENTS";
        } else if ($obj == 'resource') {
            $active_tab = '@IS_RESOURCE';
        } else if ($obj == 'forum') {
            $active_tab = '@IS_FORUM';
        }
    }

    $home_tab = '<li class="@IS_HOME"><a class="@IS_HOME" href="' . $home_url . '">Start</a></li>';
    $agenda_tab = '<li class="@IS_EVENTS fav"><a class="@IS_EVENTS" href="' . $agenda_url . '">Agendas</a></li>';
    $project_tab = '<li class="@IS_PROJECT fav"><a class="@IS_PROJECT" href="' . $projects_url . '">Projects</a></li>';
    $grade_tab = '<li class="@IS_GRADE"><a class="@IS_GRADE" href="' . $grade_url . '">Gradebook</a></li>';
    $forum_tab = '<li class="@IS_FORUM"><a class="@IS_FORUM" href="' . $forum_url . '">Discussions</a></li>';
    $activities_tab = '<li class="@IS_ACTIVITY"><a class="@IS_ACTIVITY" href="' . $act_url . '">Activities</a></li>';
    $resource_tab = '<li class="@IS_RESOURCE"><a class="@IS_RESOURCE" href="' . $resource_url . '">Resources</a></li>';

    if (is_course_exist($course_nid)) {
        $output = $HTMLCourseHome_tabs;
        $output = str_replace("@COURSE_NID", $course_nid, $output);
//        $output = str_replace($active_tab, "active", $output);          //Set the active tab

        if ($active_tab == '@IS_HOME') {
            $home_tab = str_replace($active_tab, "active", $home_tab);
        } else if ($active_tab == '@IS_EVENTS') {
            $agenda_tab = str_replace($active_tab, "active", $agenda_tab);
        } else if ($active_tab == '@IS_PROJECT') {
            $project_tab = str_replace($active_tab, "active", $project_tab);
        } else if ($active_tab == '@IS_GRADE') {
            $grade_tab = str_replace($active_tab, "active", $grade_tab);
        } else if ($active_tab == '@IS_FORUM') {
            $forum_tab = str_replace($active_tab, "active", $forum_tab);
        } else if ($active_tab == '@IS_ACTIVITY') {
            $activities_tab = str_replace($active_tab, "active", $activities_tab);
        } else if ($active_tab == '@IS_RESOURCE') {
            $resource_tab = str_replace($active_tab, "active", $resource_tab);
        }


        if (isset($user->roles[NTLP_ROLEID_PARENT])) {

            $output = str_replace("@COURSE_HOME_TAB", '', $output);
            $output = str_replace("@COURSE_AGENDA_TAB", $agenda_tab, $output);
            $output = str_replace("@COURSE_PROJECTS_TAB", $project_tab, $output);
            $output = str_replace("@COURSE_GRADE_TAB", $grade_tab, $output);
            $output = str_replace("@COURSE_DISCUSSION_TAB", '', $output);
            $output = str_replace("@COURSE_ACTIVITIES_TAB", $activities_tab, $output);
            $output = str_replace("@COURSE_RESOURCES_TAB", $resource_tab, $output);
        } else {
            $user_role = check_user_permission($course_nid);
            // add another role to hide home and dissucssion tab for site guest role
            if ($user_role == NTLP_ROLEID_SITEGUEST || $user->roles[NTLP_ROLEID_SITEGUEST]) {

                $output = str_replace("@COURSE_HOME_TAB", "", $output);
                $output = str_replace("@COURSE_AGENDA_TAB", $agenda_tab, $output);
                $output = str_replace("@COURSE_PROJECTS_TAB", $project_tab, $output);
                $output = str_replace("@COURSE_GRADE_TAB", $grade_tab, $output);
                $output = str_replace("@COURSE_DISCUSSION_TAB", "", $output);
                $output = str_replace("@COURSE_ACTIVITIES_TAB", $activities_tab, $output);
                $output = str_replace("@COURSE_RESOURCES_TAB", $resource_tab, $output);
            } else {
                $output = str_replace("@COURSE_HOME_TAB", $home_tab, $output);
                $output = str_replace("@COURSE_AGENDA_TAB", $agenda_tab, $output);
                $output = str_replace("@COURSE_PROJECTS_TAB", $project_tab, $output);
                $output = str_replace("@COURSE_GRADE_TAB", $grade_tab, $output);
                $output = str_replace("@COURSE_DISCUSSION_TAB", $forum_tab, $output);
                $output = str_replace("@COURSE_ACTIVITIES_TAB", $activities_tab, $output);
                $output = str_replace("@COURSE_RESOURCES_TAB", $resource_tab, $output);
            }
        }

        $output = str_replace("@IS_HOME", "", $output);
        $output = str_replace("@IS_EVENTS", "", $output);
        $output = str_replace("@IS_ACTIVITY", "", $output);
        $output = str_replace("@IS_PROJECT", "", $output);
        $output = str_replace("@IS_FORUM", "", $output);
        $output = str_replace("@IS_RESOURCE", "", $output);
        $output = str_replace("@IS_GRADE", "", $output);
    }
    $term_selector .= str_replace('<!-- term_selector_combo -->', get_course_term_combo($course_nid), $HTMLTermSelectorCombo);
    $output = str_replace("<!-- TERM_SELECTOR -->", $term_selector, $output);
    return $output;
}

function ntlp_courses_teaching_render_tabs() {
    return ntlp_courses_my_tabs('TEACHING');
}

function ntlp_courses_training_render_tabs() {
    return ntlp_courses_my_tabs('TRAINING');
}

function ntlp_courses_all_render_tabs() {
    return ntlp_courses_my_tabs('ALL');
}

function ntlp_courses_open_render_tabs() {
    return ntlp_courses_my_tabs('OPEN');
}

function ntlp_courses_my_render_tabs() {
    return ntlp_courses_my_tabs();
}

function ntlp_courses_my_tabs($tab = "NOP") {
    global $user;
    include "courses_header.template.inc";
      $args = explode('/', $_GET['q']);

    $teach_url = url("ntlp/courses/teaching");
    $training_url = url("ntlp/courses/training");
    $allCourses_url = url("ntlp/courses/all");
    $openCourses_url = url("ntlp/courses/open");


    $output = $HTMLCourse_tabs;

    $active_tab = "@IS_" . strtoupper($tab);
    $output = str_replace($active_tab, "active", $output);

    $teaching_tab = '<li class="@IS_TEACHING"><a class="@IS_TEACHING" href="' . $teach_url . '">&nbsp;Teaching&nbsp;</a></li>';

    if (isset($user->roles[NTLP_ROLEID_STUDENT])) {
        $training_tab_label = 'My Courses';
    } else if (isset($user->roles[NTLP_ROLEID_PARENT])) {
        $training_tab_label = 'My Student Courses';
    } else {
        $training_tab_label = '&nbsp;&nbsp;Taking&nbsp;&nbsp;';
    }

    $training_tab = '<li class="@IS_TRAINING"><a class="@IS_TRAINING" href="' . $training_url . '">' . $training_tab_label . '</a></li>';
    $allcourse_tab = '<li class="@IS_ALL"><a class="@IS_ALL" href="' . $allCourses_url . '">All Courses</a></li>';
    $opencourse_tab = '<li class="@IS_OPEN"><a class="@IS_OPEN" href="' . $openCourses_url . '">Open Courses</a></li>';

    if ($active_tab == '@IS_TEACHING') {
        $teaching_tab = str_replace($active_tab, "active", $teaching_tab);
    } else if ($active_tab == '@IS_TRAINING') {
        $training_tab = str_replace($active_tab, "active", $training_tab);
    } else if ($active_tab == '@IS_ALL') {
        $allcourse_tab = str_replace($active_tab, "active", $allcourse_tab);
    } else if ($active_tab == '@IS_OPEN') {
        $opencourse_tab = str_replace($active_tab, "active", $opencourse_tab);
    }

    if (user_access('view teching courses') && user_access('view all courses')) {

        $output = str_replace("@COURSE_TEACHING_TAB", $teaching_tab, $output);
        $output = str_replace("@COURSES_TRAINING_TAB", $training_tab, $output);
        $output = str_replace("@COURSES_ALL_TAB", $allcourse_tab, $output);
    } else {
        
        $output = str_replace("@COURSE_TEACHING_TAB", '', $output);
        if (isset($user->roles[NTLP_ROLEID_STUDENT])) {
            $output = str_replace("<!-- @COURSES_OPEN_TAB -->", $opencourse_tab, $output);
        }
        $output = str_replace("@COURSES_TRAINING_TAB", $training_tab, $output);
        $output = str_replace("@COURSES_ALL_TAB", '', $output);
    }
    
    
    if (in_array($args[2], array('teaching', 'training', 'all', 'open'))) {

            if (isset($user->roles[NTLP_ROLEID_TEACHER])
                || isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])
                || isset($user->roles[NTLP_ROLEID_SITEADMIN])
                || isset($user->roles[NTLP_ROLEID_SCHOOLLEADER])
                || isset($user->roles[NTLP_ROLEID_NETWORKSTAFF])
                || isset($user->roles[NTLP_ROLEID_SYSTEM_ADMIN])) {

            $output = str_replace('<!-- CREATE_COURSE -->', '<div style="width:117px; float:right;"> <a style="font-weight:bold;"  href="' . url('ntlp/admincourse/new') . '" class="SubmitAssignmentBtn" >Create Course</a></div>', $output);
        } else {
            $output = str_replace('<!-- CREATE_COURSE -->', '', $output);
        }
    }

//            if (isset($user->roles[NTLP_ROLEID_NETWORKMANAGER] ) || isset($user->roles[NTLP_ROLEID_NETWORKSTAFF] )) {
//                $output = str_replace("<!-- SCHOOL_SELECTOR -->", get_school_control(), $output);
//            }
//    $selector .= str_replace('<!-- school_year_combo -->', get_school_years_by_id(get_this_school(), array('class' => 'my-dropdown')), $HTMLSchoolYearCombo);
    // need to set school term session to course term session.
    $_SESSION['CURRENT_COURSE_TERM'] = $_SESSION['CURRENT_SCHOOL_TERM'];
//    $selector .= str_replace('<!-- term_selector_combo -->', get_school_term_combo(), $HTMLTermSelectorCombo);
//    $output = str_replace('<!-- TERM_YEAR_SELECTOR_NEW -->', $selector, $output);

    $output = str_replace("@IS_TEACHING", "", $output);
    $output = str_replace("@IS_TRAINING", "", $output);
    $output = str_replace("@IS_ALL", "", $output);
//    $output = str_replace('@GET_SCHOOL', get_school_control(), $output);

    return $output;
}

?>
