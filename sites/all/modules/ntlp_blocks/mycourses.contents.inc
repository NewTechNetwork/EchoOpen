<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');
include_once( 'data_access.inc' );

function mycourses_block_pure($all = FALSE, $print = TRUE) {
    global $user, $base_path;

//watchdog('cookies', 'Request:'.print_r($_REQUEST, true));
//Allow admin to administer pages and blocks
    if ($user->uid == 1)
        return;

    $school_nid = get_user_school($user->uid);
    if ($school_nid > 0) {
        $user_role = check_user_permission($school_nid);
    } else {
        drupal_goto("ntlp/render/error/page/Courses inaccessible, you are not enrolled in any School or your School ($school_nid) not found. Please contact with Site Administrator.");
//        print("Courses inaccessible, you are not enrolled in any School or your School ($school_nid) not found. Please contact with Site Administrator.");
//        print_r($user);
//        var_dump(debug_backtrace());
    }
//    print_r($user_role);

    $u = get_user_role($user->uid);
    if (user_access('allow access to contents of my kids')) {
        $children = _get_parent_children($user->uid);
        return taking_block($all, $children[0]->uid, false);
    } else if ($u->rid == NTLP_ROLEID_STUDENT) {
        return taking_block($all, $user->uid, false);
    }

    include "mycourses.template.inc";
    $user_id = $user->uid;
//    $rs = get_user_courses($user_id, NTLP_ROLEID_TEACHER, true);
    $rs = get_user_courses_forhomepage($user_id, NTLP_ROLEID_TEACHER);

//Generate URL for today's agenda
    $date = get_tz($school_nid, 'Y/m/d', date('Y/m/d H:i:s', time()));
//    $today = getdate(); // Get today's date
    $pyear = date("Y", strtotime($date));
    $pmonth = date("m", strtotime($date));
    $pday = date("d", strtotime($date));

    $date_in_url = "/$pyear/$pmonth/$pday";
    $c = 0;

    $found = false;
    while ($rec = db_fetch_object($rs)) {
//echo 'found:'.$rec->title;
        $found = true;
//      Agenda Title field was removed as per customer request
//      No need to get agenda now, we just need to generate the url
//        $agenda = get_agenda($rec->nid, $today['year'], $today['mon'], $today['mday']);
        $agenda_url = 'ntlp/courses/agenda/' . $rec->course_nid . "/$pyear/$pmonth/$pday";

        $recAct = get_tea_activities_tograde($rec->course_nid);
        $recRes = get_resources_new($rec->course_nid, $user_id);
//        $rActs = '';
//        while ($recAct = db_fetch_object($rsAct)) {
//            $rActs = $HTMLact;
//            $rActs = str_replace('@ACTIVITY_NAME', l($recAct->title, 'ntlp/activity/view/'.$recAct->nid), $rActs);
//        }
//        if (isset($agenda->title))                //Agenda Title field was removed as per customer request
//            $agenda_title = $agenda->title;
//        else
        $agenda_title = "Today's Agenda";
//        if (isset($agenda->title))
//            $agenda_title = $agenda->title;
//        else

        $rsAnn = get_announcement($rec->course_nid, $pyear, $pmonth, $pday);
        $announcement = "None";
        if ($announcement)
            if (!empty($rsAnn->body))
                $announcement = $rsAnn->body;

// if ($c < 6) {
        $course_title = (strlen($rec->title) >= 18 ? substr($rec->title, 0, 18) . '...' : $rec->title);
        $rCbox = $HTMLcoursebox;
        $rCbox = str_replace('@COURSE_NAME', l($course_title, 'ntlp/courses/home/' . $rec->course_nid), $rCbox);
        $rCbox = str_replace('@COURSE_PERIOD', 'period ' . ($rec->period ? $rec->period : '*' ), $rCbox);
        $rCbox = str_replace('@COURSE_GRADEBOOK', l('Course Gradebook', 'ntlp/courses/grade/' . $rec->course_nid), $rCbox);
        $rCbox = str_replace('@COUNT_ACTIVITY', l(intval($recAct->num_activities), 'ntlp/courses/activity/' . $rec->course_nid), $rCbox);
        $rCbox = str_replace('@RESORUCES_NEW', l(intval($recRes->num_resources), 'ntlp/courses/resource/' . $rec->course_nid), $rCbox);
        $rCbox = str_replace('@AGENDA_TITLE', l($agenda_title, $agenda_url), $rCbox);
        $rCbox = str_replace('@ANNOUNCEMENT', $announcement . '&nbsp;', $rCbox);
//        $rCbox = str_replace('@ACTIVITY_LIST', $rActs, $rCbox);
        $output .= $rCbox;
//  }
        $c++;
    }
    if ($found) {
//Fill up the remaining boxes with Empty/Placeholder course box
        while ($c < 6) {
            if ($c % 2 == 0) {
                $output .= '<div style="padding:0 0 5px 0 "><img src="' . $base_path . 'sites/all/files/banner/boldr-homepage-banner' . (round($c / 2) + 1) . '.png" /></div>';
                $c+=2;
            } else {


                $rCbox = $HTMLcourse_placeholder;
//            $rCbox = str_replace('@COURSE_NAME', 'None', $rCbox);
//            $rCbox = str_replace('@COURSE_PERIOD', '', $rCbox);
//            $rCbox = str_replace('@COURSE_GRADEBOOK', 'Course Gradebook', $rCbox);
//            $rCbox = str_replace('@COUNT_ACTIVITY', '0', $rCbox);
//            $rCbox = str_replace('@RESORUCES_NEW', '0', $rCbox);
//            $rCbox = str_replace('@AGENDA_TITLE', 'None', $rCbox);
//            $rCbox = str_replace('@ANNOUNCEMENT', 'None', $rCbox);
                $output .= $rCbox;
                $c++;
            }
        }
    } else {
        $output .= '<div style="padding:0 0 5px 0 "><img src="' . $base_path . 'sites/all/files/banner/boldr-homepage-banner1.png" /></div>';
        $output .= '<div style="padding:0 0 5px 0 "><img src="' . $base_path . 'sites/all/files/banner/boldr-homepage-banner2.png" /></div>';
        $output .= '<div style="padding:0 0 5px 0 "><img src="' . $base_path . 'sites/all/files/banner/boldr-homepage-banner3.png" /></div>';
//$output = 'You are not enrolled in any classes';
    }

    if ($print)
        print $output;
    else
        return $output;
}

function taking_block($all = FALSE, $user_id = false, $print = true) {
    include "mycourses.template.inc";
    global $user, $base_path;


// CHanges by Saif I commented this because.. we are not getting a user id from url..
// as the parameter defined and this $user_id parameter wont be empty for it so its no use.
//
    if (user_access('allow access to contents of my kids') && !empty($user_id)) {
        $_SESSION['PARENT_STUDENT_UID'] = $user_id;
    }
    if (user_access('allow access to contents of my kids') && empty($user_id)) {
        foreach (explode('/', $_GET['q']) as $q) {
            if (is_numeric($q)) {
                $user_id = $q;
            }
        }
        $_SESSION['PARENT_STUDENT_UID'] = $user_id;
    } else if (empty($user_id)) {
        $user_id = $user->uid;
    }

    $school_id = get_this_school();
    $rs = get_user_courses_forhomepage($user_id, NTLP_ROLEID_STUDENT);

//    $date = get_tz_course($courseid, 'Y/m/d', date('Y/m/d H:i:s',time()));
    $date = get_tz($school_id, 'Y/m/d', date('Y/m/d H:i:s', time()));
    $pyear = date("Y", strtotime($date));
    $pmonth = date("m", strtotime($date));
    $pday = date("d", strtotime($date));

//Generate URL for today's agenda
    $date_in_url = "/$pyear/$pmonth/$pday";
    $c = 0;

    $found = false;
    while ($rec = db_fetch_object($rs)) {
//echo 'found:'.$rec->title;
        $found = true;
//      Agenda Title field was removed as per customer request
//      No need to get agenda now, we just need to generate the url
//        $agenda = get_agenda($rec->nid, $today['year'], $today['mon'], $today['mday']);
        $agenda_url = 'ntlp/courses/agenda/' . $rec->course_nid . "/$pyear/$pmonth/$pday";

//        $rsAct = get_tea_activities_tograde($rec->course_nid, $pyear, $pmonth, $pday);
//        $rActs = '';
//        while ($recAct = db_fetch_object($rsAct)) {
//            $rActs = $HTMLact;
//            $rActs = str_replace('@ACTIVITY_NAME', l($recAct->title, 'ntlp/activity/view/'.$recAct->nid), $rActs);
//        }
//        if (isset($agenda->title))                //Agenda Title field was removed as per customer request
//            $agenda_title = $agenda->title;
//        else
        $agenda_title = "Today's Agenda";
//        if (isset($agenda->title))
//            $agenda_title = $agenda->title;
//        else

        $rsAnn = get_announcement($rec->course_nid, $pyear, $pmonth, $pday);
        $announcement = "None";
        if ($announcement)
            if (!empty($rsAnn->body))
                $announcement = $rsAnn->body;

// if ($c < 6) {
//        $rsOS = get_outcome_student($rec->course_nid, $user_id);
//Disabling it, until further notice
//        $gb = '';
//        if( $recOS = db_fetch_object($rsOS) ) {
//            if ($rec->grade > 100) $gv = 100; else $gv = $rec->grade;
//            $grade = get_gradescale( $gv );
//            $gb = $gv ? "{$grade} ({$gv}%)" : '';
//        }
        $recAct = getStudentPendingActivities($school_id, $rec->course_nid, $user_id);
        $recRes = get_resources_new($rec->course_nid, $user_id);
        $course_title = (strlen($rec->title) >= 18 ? substr($rec->title, 0, 18) . '...' : $rec->title);
        $rCbox = $HTMLcourseboxTaking;
        $rCbox = str_replace('@COURSE_NAME', l($course_title, 'ntlp/courses/home/' . $rec->course_nid), $rCbox);
        $rCbox = str_replace('@COURSE_PERIOD', 'period ' . ($rec->period ? $rec->period : '*'), $rCbox);
        $rCbox = str_replace('@COURSE_GRADEBOOK', l('Course Gradebook', 'ntlp/courses/grade/' . $rec->course_nid), $rCbox);
//        $rCbox = str_replace('@COURSE_GRADEBOOK', $gb, $rCbox);
        $rCbox = str_replace('@COUNT_ACTIVITY', l($recAct->num_activities, 'ntlp/courses/activity/' . $rec->course_nid), $rCbox);
        $rCbox = str_replace('@RESORUCES_NEW', l($recRes->num_resources, 'ntlp/courses/resource/' . $rec->course_nid), $rCbox);
        $rCbox = str_replace('@AGENDA_TITLE', l($agenda_title, $agenda_url), $rCbox);
        $rCbox = str_replace('@ANNOUNCEMENT', $announcement . '&nbsp;', $rCbox);
//        $rCbox = str_replace('@ACTIVITY_LIST', $rActs, $rCbox);
        $output .= $rCbox;
//  }
        $c++;
    }
    if ($found) {
//Fill up the remaining boxes with Empty/Placeholder course box
        while ($c < 6) {
            if ($c % 2 == 0) {
                $output .= '<div style="padding:0 0 5px 0 "><img src="' . $base_path . 'sites/all/files/banner/boldr-homepage-banner' . (round($c / 2) + 1) . '.png" /></div>';
                $c+=2;
            } else {


                $rCbox = $HTMLcourse_placeholder;
//            $rCbox = str_replace('@COURSE_NAME', 'None', $rCbox);
//            $rCbox = str_replace('@COURSE_PERIOD', '', $rCbox);
//            $rCbox = str_replace('@COURSE_GRADEBOOK', 'Course Gradebook', $rCbox);
//            $rCbox = str_replace('@COUNT_ACTIVITY', '0', $rCbox);
//            $rCbox = str_replace('@RESORUCES_NEW', '0', $rCbox);
//            $rCbox = str_replace('@AGENDA_TITLE', 'None', $rCbox);
//            $rCbox = str_replace('@ANNOUNCEMENT', 'None', $rCbox);
                $output .= $rCbox;
                $c++;
            }
        }
    } else {
        $output .= '<div style="padding:0 0 5px 0 "><img src="' . $base_path . 'sites/all/files/banner/boldr-homepage-banner1.png" /></div>';
        $output .= '<div style="padding:0 0 5px 0 "><img src="' . $base_path . 'sites/all/files/banner/boldr-homepage-banner2.png" /></div>';
        $output .= '<div style="padding:0 0 5px 0 "><img src="' . $base_path . 'sites/all/files/banner/boldr-homepage-banner3.png" /></div>';
//$output = 'You are not enrolled in any classes';
    }

    if ($print) {
        print $output;
    } else {
        return $output;
    }
}

function mycourses_block_contents($all = FALSE) {
    global $user;
    include "mycourses.template.inc";
    drupal_add_js(drupal_get_path('module', 'ntlp_blocks') . '/mycourses.js');

    $back_url = array();

//    $a = new stdClass();
//    $a->url = 'ntlp/home';
//    $a->page_name = 'HOME';
//
//    $back_url[] = $a;
//
//    $_SESSION['BACK_URL'] = $back_url;
//
//    set_item_url_handler('HOME');
//    if (user_access('create course activity')) {



    $rClink = '';
    if ($all == TRUE && user_access('create ntlp_course content')) {
        $rClink = $HTMLnewlink;
        $rClink = str_replace('@LINK_NEW', l('Create New Course', "ntlp/courses/new/0"), $rClink);
    }
    $rCbox = $HTMLcontainer;
    $rCbox = str_replace('@LINK_NEW', $rClink, $rCbox);
    $rCbox = str_replace('@COURSE_BLOCKS', mycourses_block_pure($all, false), $rCbox);


//Generate tab buttons
    $enabledBtns = array();

    $u = get_user_role($user->uid);
    $activeTabClass = ' class="CoursesTabActive"';
    $is_active_teaching = $u->rid == NTLP_ROLEID_TEACHER || $u->rid == NTLP_ROLEID_SCHOOLLEADER || $u->rid == NTLP_ROLEID_SITEADMIN ||
            user_access('administer all schools') || user_access('administer my school') ? $activeTabClass : '';
    $is_active_taking = $u->rid == NTLP_ROLEID_STUDENT && !$is_active_teaching ? $activeTabClass : '';


    $is_teaching = '<a id="tab-teaching" onclick="open_teaching()" ' . $is_active_teaching . ' href="#Teaching">Teaching</a>';
    $is_taking = '<a id="tab-taking" onclick="open_taking()" ' . $is_active_taking . ' href="#Taking">Taking</a>';

    if ($u->rid == NTLP_ROLEID_TEACHER || $u->rid == NTLP_ROLEID_SCHOOLLEADER || $u->rid == NTLP_ROLEID_SITEADMIN || user_access('administer all schools') || user_access('administer my school')) {
        $tmpBtn = str_replace('@TAB_BUTTON', $is_teaching, $HTMLCoursesTabBtn);
        $enabledBtns[] = $tmpBtn;
    }

    if ($u->rid == NTLP_ROLEID_TEACHER || $u->rid == NTLP_ROLEID_SCHOOLLEADER || $u->rid == NTLP_ROLEID_SITEADMIN || (user_access('administer all schools') || user_access('administer my school'))) {
        $tmpBtn = str_replace('@TAB_BUTTON', $is_taking, $HTMLCoursesTabBtn);
        $enabledBtns[] = $tmpBtn;
    }

    if ($u->rid == NTLP_ROLEID_STUDENT) {

//        $rs = get_user_courses_forhomepage($user->uid, NTLP_ROLEID_STUDENT);
//        if ($rs->num_rows > 0) {
//            $tmpBtn = str_replace('@TAB_BUTTON', $is_taking, $HTMLCoursesTabBtn);
//            $enabledBtns[] = $tmpBtn;
//        }
    }

    if (user_access('administer all schools') || user_access('administer my school') || $u->rid == NTLP_ROLEID_TEACHER) {
        $tmpBtn = str_replace('@TAB_BUTTON', $HTMLCoursesTabBtn);
        $enabledBtns[] = $tmpBtn;
    }

    if (user_access('allow access to contents of my kids')) {
        $children = _get_parent_children($user->uid);


// set first student uid record into the session for student selector
        if (!empty($children[0]->uid)) {
            $_SESSION['PARENT_STUDENT_UID'] = $children[0]->uid;
        }

        $class = 'CoursesTabActive';
        foreach ($children as $child) {
            $lnk = '<a class="childrenTab ' . $class . '" id="child-tab-' . $child->uid . '" onclick="open_child_tab(' . $child->uid . ')" href="#Children">' . $child->first_name . '</a>';
            $enabledBtns[] = str_replace('@TAB_BUTTON', $lnk, $HTMLCoursesTabBtn);
            $class = '';
        }
    }

    $new_block_start = '<div style="position: absolute; top: 0px; right: 0px;">
        <table cellpadding="0" cellspacing="0" border="0">
                <tr>
                    <td>
                        <div class="CoursesTabLeft"></div>
                    </td>
                    <td>';

    $new_block_end = '</td>
                    <td>
                       <div class="CoursesTabRight"></div>
                    </td>
                </tr>
            </table>
             </div>';

    $empty_space_div = '<div class="new_courses_home_right">&nbsp;</div>';

    if (!empty($enabledBtns)) {
        $enabledBtns = implode($HTMLCoursesTabBtnSeparator, $enabledBtns);

        $rCbox = str_replace('@TAB_PLACEHOLDER', $new_block_start . '<div class="CoursesTabCenter" style="width:auto"><ul>' . $enabledBtns . '</ul></div>' . $new_block_end, $rCbox);
    } else {

        $rCbox = str_replace('@TAB_PLACEHOLDER', $empty_space_div, $rCbox);
    }

    return $rCbox;
}

function get_school_terms_combo($school_nid) {
    $output = '';
    $schoolTermsCombo = '';
    if ($schoolTerms = getSchoolWideCourses($school_nid)) {
        $output .= '<select onchange="getSchoolSnapshotBySchoolTerm( this.value );">
                                <option>Select course</option>
                            ';
        foreach ($schoolTerms as $schoolTerm) {
            $output .= "
                    <option value=\"{$schoolTerm->school_term_tid}\" " . (($school_term_id == $schoolTerm->school_term_tid ) ? ' selected' : '') . ">{$schoolTerm->name} </option>
                    ";
        }
        $output .= '</select>';
    }

    print $output;
}


function get_teacher_courses_tab_content($tab_name, $from_gradestab = false) {
    global $user;

    $courses_view_table = 'courses_view_table';
    drupal_add_js('$(document).ready(function() {
    $("#' . $courses_view_table . '").tablesorter( {sortList: [[0,0]]} );
    });', 'inline');
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/jquery.tablesorter/jquery.tablesorter.js');
    drupal_add_css(drupal_get_path('module', 'ntlp_school') . '/jquery.tablesorter/themes/blue/styles.css', 'module', 'all', FALSE);

    set_item_url_handler('Courses');
    if ($tab_name == 'teaching' || $tab_name == 'training') {
        $sort_option = $_GET['viewby'];

//        if (isset($sort_option)) {
//            $_SESSION['ntlp_courses_:_view_by'] = $sort_option;
//        } else {
//            $sort_option = $_SESSION['ntlp_courses_:_view_by'];
//            drupal_goto('ntlp/courses/' . $tab_name, array('viewby' => $sort_option));
//        }

        if (empty($sort_option)) {
            $sort_option = 'ct';
        }
    } else if ($tab_name == 'all') {

        if (isset($_GET['viewby'])) {
            $sort_option = $_GET['viewby'];
        } else {
            $sort_option = get_this_school();
        }
    }

    if ($tab_name != 'all' && $tab_name != 'open') {
        if ($sort_option == 'ct') {
            if ($tab_name == 'teaching') {
                $header = array(
//array('data' => 'Course Title', 'field' => 'n.title', 'sort' => 'desc', 'width' => '30%'),
                    array('data' => 'Course Title', 'width' => '30%'),
                    array('data' => 'Enrollment', 'width' => '10%'),
                    array('data' => 'Activities to be Graded', 'width' => '10%'),
                    array('data' => 'New Share Stuff Posts', 'width' => '10%'),
                    array('data' => 'New Discussion Posts', 'width' => '10%'),
                );
            } else if ($tab_name == 'training') {

                $header = array(
//array('data' => 'Course Title', 'field' => 'n.title', 'sort' => 'desc', 'width' => '30%'),
                    array('data' => 'Course Title', 'width' => '30%'),
                    array('data' => 'Instructor(s)', 'width' => '10%'),
                    array('data' => 'Upcoming Activities', 'width' => '10%'),
                    array('data' => 'New Share Stuff Posts', 'width' => '10%'),
                    array('data' => 'New Discussion Posts', 'width' => '10%'),
                );
            }
        } else {
            if ($tab_name == 'teaching') {
                $header = array(
                    array('data' => 'Course Title'),
                    array('data' => 'Term(s)'),
                );
            } else if ($tab_name == 'training') {
                $header = array(
                    array('data' => 'Course Title'),
                    array('data' => 'Instructor(s)'),
                    array('data' => 'Term(s)'),
                );
            }
        }
    } else {
//table header for all courses tab.
        if ($tab_name != 'open') {
            $header = array(
                array('data' => 'Course Title', 'width' => '42%'),
                array('data' => 'Instructor(s)', 'width' => '20%'),
                array('data' => 'Enrollment', 'width' => '19%'),
                array('data' => 'Self-Enroll', 'width' => '19%'),
            );
        } else {

            $header = array(
                array('data' => 'Course Title', 'width' => '50%'),
                array('data' => 'Instructor(s)', 'width' => '20%'),
                array('data' => 'Self-Enroll'),
            );
        }
    }

//    $attributes = array('class' => 'ntlp_table', 'width' => '100%', 'cellpadding' => '8');
    $attributes = array('id' => $courses_view_table, 'class' => 'ntlp_table tablesorter', 'width' => '100%', 'cellpadding' => '8');

    switch ($tab_name) {
        case 'training':
        case 'teaching':

// get_course_selection_yellow_bar() generates yellow bar on course tab
            $output .= get_course_selection_yellow_bar($tab_name, $sort_option);
            return $output .= _generate_main_courses_table($header, $tab_name, $sort_option, $attributes);

        case 'open':
            $output .= get_course_selection_yellow_bar($tab_name, $sort_option);
            return $output .= _generate_main_courses_table($header, $tab_name, $sort_option, $attributes);

        case 'all':

            $output .= get_course_selection_yellow_bar($tab_name, $sort_option);
            $output .= _generate_main_courses_table($header, $tab_name, $sort_option, $attributes);
    }

    return $output;
}

function _is_current_term($term_id) {
    if ($r = get_current_school_term(get_this_school())) {
        $row = db_fetch_object($r);
        return $term_id == $row->school_term_tid;
    }

    return false;
}

/* this is function is used to render the courses on courses multiple tab
 * 1. Course I Tech Tab
 * 2. My Traning Course Tab
 * 3. All Courses Tab
 */

function _generate_main_courses_table($header, $tab_name, $sort_option, $attributes = array()) {
    global $user, $base_path;

    include "mycourses.template.inc";



    $output = '<div id="dialog-confirm-outcome" style="display: none;" title="Are you sure?">
                        <span class="msg">Are you sure you want to submit the form?</span>
                    </div>';

    if ($tab_name != 'all' && $tab_name != 'open') {
        if ($sort_option == 'ct') {
            $school_year = get_this_school_year();
            $courses = get_courses_for_user($user->uid, $school_year, $tab_name, $sort_option);

            if ($tab_name == 'teaching') {

                if (sizeof($courses) > 0) {
                    foreach ($courses as $course) {
                        $edit_link = '<span style="float:right;">
                            <a href="' . url('ntlp/courses/edit/' . $course->nid) . '" >
                                <img src="' . $base_path . 'themes/Boldr/Images/common/edit.png" alt="editimage"/>
                            </a></span>';
                        $new_resource_obj = get_resources_new($course->nid, $user->uid);
                        $courses_discussion_topic = get_forum_topic_by_nid($course->nid);

                        $count_new_topic = 0;
                        foreach ($courses_discussion_topic as $topic) {
                            if ($topic->last_comment_timestamp > $user->access) {
                                $count_new_topic += 1;
                            }
                        }

                        $recAct = get_tea_activities_tograde($course->nid);
                        
                        $rows[] = array(
                            array('data' => l(truncate_course_title($course->title), 'ntlp/courses/home/' . $course->nid) . ' ' . $edit_link . ' (Per ' . (($course->period == 0 ) ? '*' : $course->period) . ')'),
                            array('data' => $course->total_students), // gen table
                            array('data' => l(intval($recAct->num_activities), 'ntlp/courses/activity/' . $course->nid)),
                            array('data' => (($new_resource_obj) ? $new_resource_obj->num_resources : '')),
                            array('data' => $count_new_topic),
                        );
                    }

                    $output .= $HTML_main_courses_tab_table_corner_start;
                    $output .= theme_table($header, $rows, $attributes);

                    $output .= $HTML_main_courses_tab_table_corner_end;
                }
            } else if ($tab_name == 'training') {

                if (sizeof($courses) > 0) {
                    foreach ($courses as $course) {

                        $courses_discussion_topic = get_forum_topic_by_nid($course->nid);
                        $instructors = _generate_courses_teachers_html($course->nid);
                        $new_resource_obj = get_resources_new($course->nid, $user->uid);

                        $count_new_topic = 0;
                        foreach ($courses_discussion_topic as $topic) {
                            if ($topic->changed > $user->access) {
                                $count_new_topic += 1;
                            }
                        }

                        $rows[] = array(
                            array('data' => l(truncate_course_title($course->title), 'ntlp/courses/home/' . $course->nid) . ' ' . $edit_link . ' (Per ' . (($course->period == 0 ) ? '*' : $course->period) . ')'),
                            array('data' => $instructors),
                            array('data' => 0),
                            array('data' => (($new_resource_obj) ? $new_resource_obj->num_resources : '')),
                            array('data' => $count_new_topic),
                        );
                    }

                    $output .= $HTML_main_courses_tab_table_corner_start;
                    $output .= theme_table($header, $rows, $attributes);
                    $output .= $HTML_main_courses_tab_table_corner_end;
                }
            }
        } else if ($sort_option == 'ay') {

            if ($tab_name == 'training') {
                $user_role_de = NTLP_ROLEID_STUDENT;
            } else if ($tab_name == 'teaching') {
                $user_role_de = NTLP_ROLEID_TEACHER;
            }

            if (isset($user->roles[NTLP_ROLEID_TEACHER]) || isset($user->roles[NTLP_ROLEID_SCHOOLLEADER])
                    || isset($user->roles[NTLP_ROLEID_SITEADMIN]) || isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])
                    || isset($user->roles[NTLP_ROLEID_NETWORKSTAFF])
                    || isset($user->roles[NTLP_ROLEID_SYSTEM_ADMIN])) {

                $result = get_school_years_for_teacher($user->uid, $user_role_de);
            } else if (isset($user->roles[NTLP_ROLEID_STUDENT]) || isset($user->roles[NTLP_ROLEID_PARENT])) {
                $result = get_school_years_for_student($user->uid);
            }


            $output = $HTML_main_courses_tab_table_corner_start;
            if ($result->num_rows > 0) {
                while ($data = db_fetch_object($result)) {  //multiple Header
                    $courses = array();
                    $courses = get_courses_for_user($user->uid, $data->school_year, $tab_name, $sort_option);
                    if (sizeof($courses) > 0) {


                        $courses_view_table = 'courses_view_table' . $data->school_year;
                        $attributes['id'] = $courses_view_table;
                        drupal_add_js('$(document).ready(function() {
                    $("#' . $courses_view_table . '").tablesorter( {sortList: [[0,0]]} );
                    });', 'inline');

                        $rows = array();
                        foreach ($courses as $course) {

                            if ($course->course_term_count == $course->school_term_count) {
                                $course_terms = 'Year long';
                            } else {
                                $course_terms = _generate_courses_terms_html($course->nid);
                            }
                            if ($tab_name == 'teaching') {
                                $rows[] = array(
                                    array('data' => l(truncate_course_title($course->title), 'ntlp/courses/home/' . $course->nid) . ' (Per ' . (($course->period == 0 ) ? '*' : $course->period) . ')'),
                                    array('data' => $course_terms),
                                );
                            } else if ($tab_name == 'training') {
                                $instructors = _generate_courses_teachers_html($course->nid);
                                $rows[] = array(
                                    array('data' => l(truncate_course_title($course->title), 'ntlp/courses/home/' . $course->nid) . ' (Per ' . (($course->period == 0 ) ? '*' : $course->period) . ')'),
                                    array('data' => $instructors),
                                    array('data' => $course_terms),
                                );
                            }
                        }
                        $output .= theme_table($header, $rows, $attributes, ($data->school_year - 1) . ' - ' . $data->school_year);
                    }
                }
            }
            $output .= $HTML_main_courses_tab_table_corner_end;
        } else {

            if ($tab_name == 'teaching') {
                $courses = get_courses_for_user($user->uid, $sort_option, $tab_name, $sort_option);
                if (sizeof($courses) > 0) {
                    foreach ($courses as $course) {
                        $edit_link = '<span style="float:right;margin:-15px 0px 0px  0px">
                            <a href="' . url('ntlp/courses/edit/' . $course->nid) . '" >
                            </a></span>';

                        if ($course->course_term_count == $course->school_term_count) {
                            $course_terms = 'Year long';
                        } else {
                            $course_terms = _generate_courses_terms_html($course->nid);
                        }

                        $rows[] = array(
                            array('data' => l(truncate_course_title($course->title), 'ntlp/courses/home/' . $course->nid) . ' ' . $edit_link . ' (Per ' . (($course->period == 0 ) ? '*' : $course->period) . ')'),
                            array('data' => $course_terms),
                        );
                    }

                    $output .= $HTML_main_courses_tab_table_corner_start;
                    $output .= theme_table($header, $rows, $attributes);
                    $output .= $HTML_main_courses_tab_table_corner_end;
                }
            } else if ($tab_name == 'training') {

                $courses = get_courses_for_user($user->uid, $sort_option, $tab_name, $sort_option);
                if (sizeof($courses) > 0) {
                    foreach ($courses as $course) {
                        $edit_link = '<span style="float:right;margin:-15px 0px 0px  0px">
                            <a href="' . url('ntlp/courses/edit/' . $course->nid) . '" >
                            </a></span>';

                        $instructors = _generate_courses_teachers_html($course->nid);

                        if ($course->course_term_count == $course->school_term_count) {
                            $course_terms = 'Year long';
                        } else {
                            $course_terms = _generate_courses_terms_html($course->nid);
                        }

                        $rows[] = array(
                            array('data' => l(truncate_course_title($course->title), 'ntlp/courses/home/' . $course->nid) . ' ' . $edit_link . ' (Per ' . (($course->period == 0 ) ? '*' : $course->period) . ')'),
                            array('data' => $instructors),
                            array('data' => $course_terms),
                        );
                    }

                    $output .= $HTML_main_courses_tab_table_corner_start;
                    $output .= theme_table($header, $rows, $attributes);
                    $output .= $HTML_main_courses_tab_table_corner_end;
                }
            }
        }
    } else {

        if (isset($sort_option)) {
            $school_nid = $sort_option;
        } else {
            $school_nid = get_this_school();
        }

        if (isset($_GET['yearby'])) {
            $year_by = $_GET['yearby'];
            if (!is_this_school_year_exist($school_nid, $year_by)) {
                $year_by = get_school_year($school_nid);
            }
        } else {
            $year_by = get_school_year($school_nid);
        }

        if (isset($_GET['termby'])) {
            $term_by = $_GET['termby'];
        } else {
            $term_by = get_current_selected_term();
        }
        if (!is_term_exist_in_school($school_nid, $term_by, $year_by)) {
            $current_result = get_current_school_term($school_nid, $year_by);
            if ($current_result) {
                $data = db_fetch_object($current_result);
                $term_by = $data->school_term_tid;
            } else {
                $term_by = get_school_first_term($school_nid, $year_by);
            }
        }

//        watchdog('all_course', 'school_nid ' . $school_nid . ' year select ' . $year_by . ' term ' . $term_by);
        if ($tab_name != 'open') {
            $courses = _get_all_school_courses($school_nid, $header, $year_by, $term_by);
        } else {
            $school_nid = get_this_school();
            $courses = get_all_open_courses($school_nid, $header, get_school_year($school_nid), get_current_selected_term());
        }


        if (sizeof($courses) > 0) {
            foreach ($courses as $course) {

//                echo $course->title .' is_term_active '.$course->is_term_active .' self enroled '.$course->allow_self_enroll.'<br />';
                $instructors = _generate_courses_teachers_html($course->nid);

                if ($tab_name != 'open') {

                    $course_title = l(truncate_course_title($course->title), 'ntlp/courses/home/' . $course->nid) . ' (Per ' . (($course->period == 0 ) ? '*' : $course->period) . ')';
                    if (isset($user->roles[NTLP_ROLEID_TEACHER])) {

                        if ($course->school_nid == get_network_school()) {
                            $self_enrole_result = get_self_enroll_row($course->nid, $user->uid);
                            if ($self_enrole_result) {
                                $course_info = db_fetch_object($self_enrole_result);
                                if ($course_info->allow_self_enroll == 1 && $course_info->role_id == null) {
                                    /* if has no role in course then no link apply on course title */
                                    $course_title = truncate_course_title($course->title) . ' (Per ' . (($course->period == 0 ) ? '*' : $course->period) . ')';
                                } else if ($course_info->allow_self_enroll == 1 && $course_info->role_id != null) {
                                    /* if has any role in course apply link on course title */
                                    $course_title = l(truncate_course_title($course->title), 'ntlp/courses/home/' . $course->nid) . ' (Per ' . (($course->period == 0 ) ? '*' : $course->period) . ')';
                                }
                            }
                        }
                    }

                    $rows[] = array(
                        array('data' => $course_title),
                        array('data' => $instructors),
                        array('data' => _get_courses_enrollment($course->nid)),
                        array('data' => (($course->is_term_active) ? _get_self_enroll_text($course->title, $course->nid, $user->uid) : '')),
                    );
//                    // UPDATE the history TABLE, stating that this USER viewed this node.
//                    node_tag_new($course->nid);
                } else {
                    $rows[] = array(
                        array('data' => l(truncate_course_title($course->title), 'ntlp/courses/home/' . $course->nid) . ' (Per ' . (($course->period == 0 ) ? '*' : $course->period) . ')'),
                        array('data' => $instructors),
                        array('data' => (_get_self_enroll_text($course->title, $course->nid, $user->uid))),
                    );
                }
            }

            $output .= $HTML_main_courses_tab_table_corner_start;
            $output .= theme_table($header, $rows, $attributes);
            $output .= $HTML_main_courses_tab_table_corner_end;
        } else {
            if ($tab_name == 'open') {
                $output = "Sorry. There are not any open courses at the moment.";
            } else {
                $output = '';
            }
        }
    }

    return $output;
}

function _get_self_enroll_text($course_name, $course_nid, $user_id) {

    drupal_add_js(drupal_get_path('module', 'ntlp_admincourse') . '/ntlp_admincourse.js');

    $course_enroll_result = get_self_enroll_row($course_nid, $user_id);
    if ($course_enroll_result) {

        $course_info = db_fetch_object($course_enroll_result);

        if (($course_info->teacher1_uid == $user_id || $course_info->teacher2_uid == $user_id) && $course_info->allow_self_enroll == 1)
            return 'Enrolled';
        else if ($course_info->role_id == NTLP_ROLEID_STUDENT || $course_info->role_id == NTLP_ROLEID_TEACHER)
            return 'Enrolled';
        else if ($course_info->allow_self_enroll == 1 && $course_info->role_id == null)
            return '<a href="javascript:self_enroll_student(\'' . addslashes($course_name) . '\',' . $course_nid . ',' . $user_id . ',' . NTLP_ROLEID_STUDENT . ');">Sign up</a>';

        else
            return '';
    }
}

function _generate_courses_teachers_html($course_nid) {

    $primary_teachers = get_course_primary_teacher($course_nid);
//    $course_teachers_result = get_course_users($course_nid, NTLP_ROLEID_TEACHER);
    if ($primary_teachers) {

        $course_teachers = '';
        if ($record = db_fetch_object($primary_teachers)) {

            if ($record->primary_teacher_2 != null && !empty($record->primary_teacher_2)) {
                $course_teachers .= l($record->primary_teacher_1, 'ntlp/user/profile/' . $record->primary_teacher_1_uid) . ' <br /> ';
                $course_teachers .= l($record->primary_teacher_2, 'ntlp/user/profile/' . $record->primary_teacher_2_uid);
            } else {
                $course_teachers .= l($record->primary_teacher_1, 'ntlp/user/profile/' . $record->primary_teacher_1_uid);
            }
        }
    }
    return $course_teachers;
}

function _generate_courses_terms_html($course_nid) {
    $course_term_result = get_course_terms($course_nid);

    if ($course_term_result) {
        $i = 1;
        $course_terms = '';
        while ($record = db_fetch_object($course_term_result)) {

            if ($course_term_result->num_rows == $i) {
                $course_terms .= $record->name;
            } else {
                $course_terms .= $record->name . ' | ';
            }
            $i++;
        }
    }
    return $course_terms;
}

function _generate_multi_tables($header, $arrayObjs, $fields, $seperateBy, $attributes = array()) {
    $tempSeparator = '';
    $separators = array();
    $rows = array();
    $output = '';

    foreach ($arrayObjs as $obj) {
        if ($tempSeparator != $obj->{$seperateBy}) {
            $separators[] = $obj->{$seperateBy};
            $tempSeparator = $obj->{$seperateBy};
        }

        sort($separators);

        $tempRow = array();

        foreach ($fields as $key => $value) {
            $pre = '';
            $post = '';
            if (is_array($value)) { // means we have to apply some link,etc to it
                $val = $obj->{$key};

                if (isset($value['if'], $value['if']['func'], $value['if']['return'], $value['if']['then'])) {
                    if (function_exists($value['if']['func'])) {
                        $val = (call_user_func_array($value['if']['func'], array($val)) === $value['if']['return']) ? $value['if']['then'] : ( isset($value['if']['else']) ? $value['if']['else'] : $val );
//                        $val = ($value['if']['func']($val)==$value['if']['return']) ? $value['if']['then'] : ( $value['if']['else'] ? $value['if']['else'] : $val );
                    }
                }
                if (array_key_exists('link', $value) and isset($value['link']['url'])) {
                    $pre = ''; // it will be done later
                    $post = isset($value['link']['postField']) ? $obj->{$value['link']['postField']} : '';
                    $val = l($val, $pre . $value['link']['url'] . $post);
                }
            } else {                // else its just value
                $val = $obj->{$value};
            }
            $tempRow[] = $val;
        }

        $rows[$tempSeparator][] = $tempRow;
    }

    $separators = array_unique($separators);
    foreach ($separators as $separator) {
        $output .= '<div class="student_Panel">
        <div id="ActivitiesCSS" style="width:auto">
          <div class="BlueBorderBgTopCenter">
            <div class="BlueBorderBgMiddleRight">
              <div class="BlueBorderBgBottomCenter">
                <div class="BlueBorderBgMiddleLeft">
                  <div class="BlueBorderBgTopRightCorner">
                    <div class="BlueBorderBgBottomLeftCorner">
                      <div class="BlueBorderBgBottomRightCorner">
                        <div class="BlueBorderBgTopLeftCorner">';
        $output .= theme_table($header, $rows[$separator], $attributes, $separator);
        $output .= '  </div>
                        </div>
                      </div>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
        ';
    }

    return $output;
}

//function ntlp_courses_school_year_selector($tab_name, $school_year) {
//    print 'get_teacher_courses_tab_content($tab_name, $school_year)';
//}

function get_course_selection_yellow_bar($tab_name, $sort_option) {
    include "courses_header.template.inc";
    global $user;
    drupal_add_js(drupal_get_path('module', 'ntlp_blocks') . '/default_header.js');

    if ($tab_name == 'all') {
        if (isset($sort_option)) {
            $school_nid = $sort_option;
        } else {
            $school_nid = get_this_school();
        }
    }

    if (isset($_GET['yearby'])) {
        $year_by = $_GET['yearby'];
        if (!is_this_school_year_exist($school_nid, $year_by)) {
            $year_by = get_school_year($school_nid);
        }
    } else {
        $year_by = get_school_year($school_nid);
    }

    if (isset($_GET['termby'])) {
        $term_by = $_GET['termby'];
    } else {
        $term_by = get_current_selected_term();
    }
    if (!is_term_exist_in_school($school_nid, $term_by, $year_by)) {
        $current_result = get_current_school_term($school_nid, $year_by);
        if ($current_result) {
            $data = db_fetch_object($current_result);
            $term_by = $data->school_term_tid;
        } else {
            $term_by = get_school_first_term($school_nid, $year_by);
        }
    }

    if (isset($_SESSION['PARENT_STUDENT_UID'])) {
        $parent_selected_student = $_SESSION['PARENT_STUDENT_UID'];
    }

    drupal_add_js('$(document).ready(function() {
        setSelectedIndex("year_by","' . $year_by . '");
        setSelectedIndex("view_by","' . $sort_option . '");
        setSelectedIndex("term_by","' . $term_by . '");
        setSelectedIndex("parent_student_selector","' . $parent_selected_student . '");
            });
            
function setSelectedIndex(id, v) {
       
        s = document.getElementById(id);
        if(s != null){
            for ( var i = 0; i < s.options.length; i++ ) {
                if ( s.options[i].value == v ) {
                    s.options[i].selected = true;
                    return;
                }
            }
         }
        }', 'inline', 'footer');

    $yellow_bar .= $HTMLCourse_yellow_bar;


    if ($tab_name == 'training' || $tab_name == 'teaching') {

        if ($tab_name == 'training') {
            $user_role = NTLP_ROLEID_STUDENT;
        } else {
            $user_role = NTLP_ROLEID_TEACHER;
        }
        if (isset($user->roles[NTLP_ROLEID_STUDENT]) || isset($user->roles[NTLP_ROLEID_PARENT])) {
// loading a years for student school_years <= current_school_year
            if (isset($user->roles[NTLP_ROLEID_PARENT])) {

                if (isset($_SESSION['PARENT_STUDENT_UID'])) {
                    $user_id = $_SESSION['PARENT_STUDENT_UID'];
                } else {
                    $children = _get_parent_children($user->uid);
                    $user_id = $children[0]->uid;
                    $_SESSION['PARENT_STUDENT_UID'] = $user_id;
                }
            } else {
                $user_id = $user->uid;
            }

            $result = get_school_years_for_student($user_id);

            if ($result) {
                while ($data = db_fetch_object($result)) {
                    $options_array[$data->school_year] = ($data->school_year - 1) . ' - ' . $data->school_year;
                }
            }
// loading all the school years for teacher.
        } else if (isset($user->roles[NTLP_ROLEID_TEACHER]) || isset($user->roles[NTLP_ROLEID_SCHOOLLEADER])
                || isset($user->roles[NTLP_ROLEID_SITEADMIN]) || isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])
                || isset($user->roles[NTLP_ROLEID_NETWORKSTAFF]) || isset($user->roles[NTLP_ROLEID_SYSTEM_ADMIN])) {

            $result = get_school_years_for_teacher($user->uid, $user_role);
            if ($result) {
                while ($data = db_fetch_object($result)) {
                    $options_array[$data->school_year] = ($data->school_year - 1) . ' - ' . $data->school_year;
                }
            }
        }


        $year_select = '<select id="view_by" onchange="window.location = \'?q=ntlp/courses/' . $tab_name . '&viewby=\'+this.value;">
                        <option value="ct">Current Term</option>';
        if ($options_array) {
            foreach ($options_array as $k => $v) {
                $year_select .= '<option value="' . $k . '">' . $v . '</option>';
            }
        }
        $year_select .= "<option value='ay'>All Years</option>
                        </select>";

        $view_course_by = '<td width="40%"><div><strong>View Course By: </strong> &nbsp;' . $year_select . '</div></td>';

        if ($sort_option == 'ct') {
            $current_school_year = get_school_year(get_user_school($user->uid));
            $school_year = ($current_school_year - 1 ) . "-" . $current_school_year;
            $selected_value = 'Current';
        } else if ($sort_option == 'ay') {
            $school_year = 'All';
            $selected_value = 'All';
        } else {
            $school_year = ($sort_option - 1) . " - " . $sort_option;
            $selected_value = 'All';
        }
//        $selected_option = $_GET['viewby'];
//
//        if ($selected_option == 'ct') {
//            $selected_value = 'Current';
//        } else {
//            $selected_value = 'All';
//        }

        if (isset($user->roles[NTLP_ROLEID_PARENT])) {
            $children_selector = '<div><strong>Student: </strong> &nbsp;' . get_parent_child_selector($user->uid) . '</div>';
        }


        $output = str_replace('<!--VIEW_COURSES_BY-->', $view_course_by, $yellow_bar);
        $output = str_replace('<!--PARENT_CHILD_SELECTOR-->', $children_selector, $output);
        $output = str_replace('<!--SCHOOL_YEAR-->', $school_year, $output);
        $output = str_replace('<!--SCHOOL_TERM-->', $selected_value, $output);
    } else if ($tab_name == 'all') {

        if (isset($sort_option)) {
            $school_nid = $sort_option;
        } else {
            $school_nid = get_this_school();
        }


        if (isset($user->roles[NTLP_ROLEID_TEACHER]) || isset($user->roles[NTLP_ROLEID_SITEADMIN])
                || isset($user->roles[NTLP_ROLEID_SCHOOLLEADER])) {
            $school_array[] = get_user_school($user->uid);
            $school_array[] = get_network_school();

            $school_selector = '<select style="width: 220px;" id="view_by" onchange="window.location = \'?q=ntlp/courses/' . $tab_name . '&viewby=\'+this.value;">';
            foreach ($school_array as $schools) {
                $school_obj = get_school_details($schools);
                $school_selector .= '<option value="' . $schools . '">' . $school_obj->title . '</option>';
            }
            $school_selector .= '</select>';
        } else if (isset($user->roles[NTLP_ROLEID_NETWORKSTAFF]) || isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])
                || isset($user->roles[NTLP_ROLEID_SYSTEM_ADMIN])) {

            $aschools = get_schools();
            $school_selector = '<select style="width: 220px;" id="view_by" onchange="onchange_schoolselector(this.value);">';
            while ($rec = db_fetch_object($aschools)) {
//                print $rec->nid.'-'.$rec->school_name.',';
                $school_selector .= '<option value="' . $rec->nid . '"' . ($school_nid == $rec->nid ? 'selected="selected"' : '') . '>' . (strlen($rec->school_name) >= 40 ? substr($rec->school_name, 0, 40) . '...' : $rec->school_name) . '</option>';
            }
            $school_selector .= '</select>';
        }

        $options_array = _get_all_school_years($school_nid);
        drupal_add_js("
                    function change_school_year(obj){
                        $.post('?q=ntlp/change/school/year/'+obj.value, function(){
                            window.location.reload();
                        });
                    }
                ", 'inline');

        if (!isset($user->roles[NTLP_ROLEID_NETWORKSTAFF]) || !isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])
                || !isset($user->roles[NTLP_ROLEID_SYSTEM_ADMIN])) {
            $year_select = '<select id="year_by" onchange="window.location = \'?q=ntlp/courses/' . $tab_name . '&yearby=\'+this.value;">';
        } else {
            $year_select = '<select id="year_by" onchange="change_school_year(this);">';
        }
        if ($options_array) {
            foreach ($options_array as $k => $v) {
                $year_select .= '<option value="' . $k . '">' . $v . '</option>';
            }
        }
        $year_select .= '</select>';

        drupal_add_js("
                    function change_term_value(obj){
                        $.post('?q=ntlp/change/school/term/'+obj.value, function(){
                            window.location.reload();
                        });
                    }
                ", 'inline');
//        watchdog("yellow_bar", "school_nid " . $school_nid . ' year ' . $year_by . ' get_this_school_year ' . get_this_school_year());
        $term_array = get_school_year_term($school_nid, $year_by);
        if (!isset($user->roles[NTLP_ROLEID_NETWORKSTAFF]) || !isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])
                || !isset($user->roles[NTLP_ROLEID_SYSTEM_ADMIN])) {
            $term_select = '<select id="term_by" onchange = "window.location = \'?q=ntlp/courses/' . $tab_name . (isset($year_by) ? '&yearby=' . $year_by : '') . '&termby=\'+this.value;" >';
        } else {
            $term_select = '<select id="term_by" onchange = "change_term_value(this);" >';
        }

        $current_result = get_current_school_term($school_nid, $year_by);
        if ($current_result) {
            $data = db_fetch_object($current_result);
            $current_running_term = $data->school_term_tid;
        }

        if ($term_array) {
            foreach ($term_array as $k => $v) {

                $term_select .= '<option value="' . $k . '" ' . (($k == $current_running_term) ? ' selected=\"selected\" ' : "") . '>' . $v . '</option>';
            }
        }
        $term_select .= '</select>';

        $output = str_replace('<!--SCHOOL-->', '<td width="40%"><div style="width:320px; float:left;"><strong>School: ' . $school_selector . '</strong></div></td>', $yellow_bar);
        $output = str_replace('<!--SCHOOL_YEAR-->', $year_select, $output);
        $output = str_replace('<!--SCHOOL_TERM-->', $term_select, $output);
    } else if ($tab_name = 'open') {

        $school_nid = get_this_school();
        $school_year = get_school_year($school_nid);

        $output = str_replace('<!--SCHOOL-->', '<td width="40%"><div style="width:320px; float:left;"><strong>School: ' . _get_node_title($school_nid) . '</strong></div></td>', $yellow_bar);
        $output = str_replace('<!--SCHOOL_YEAR-->', ($school_year - 1) . ' - ' . $school_year, $output);
        $output = str_replace('<!--SCHOOL_TERM-->', 'Current', $output);
    }


    return $output;
}

function truncate_course_title($course_t) {

    if (ctype_upper($course_t)) {
        $course_title = (strlen($course_t) >= 25 ? substr($course_t, 0, 25) . '...' : $course_t);
    } else {
        $course_title = (strlen($course_t) >= 35 ? substr($course_t, 0, 35) . '...' : $course_t);
    }

//    if()
    return $course_title;
}
