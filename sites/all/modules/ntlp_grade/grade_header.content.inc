<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


function ntlp_grades_render_header() {
    drupal_add_css(drupal_get_path('theme', 'Boldr') . '/StylishSelect/stylish-select.css', 'module', 'all', FALSE);
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/externalFile.js');
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/jquery.stylish-select.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_blocks') . '/default_header.js');

    include "grade_header.template.inc";

    global $user, $base_path;

    $school_id = get_this_school();
//    $user_role = check_user_permission($courseid);
    $tabName = arg(2);

    $container = $HTMLcontainer;



    if (user_access("access student selector")) {
        $selectors .= str_replace('<!-- student_selector_combo -->', get_parent_child(), $HTMLStudentSelectorCombo);
    }
    if ($tabName == 'gradebook' || $tabName == 'students' || $tabName == 'student_snapshot') {

        $selectors .= get_school_control();
    }
    
    $HTMLSelectorsAndHeading = str_replace('<!-- selectors -->', $selectors, $HTMLSelectorsAndHeading);

    $HTMLSelectorsAndHeading = str_replace('<!-- heading -->', 'Grades', $HTMLSelectorsAndHeading);
    $container = str_replace('<!-- selectors_and_heading -->', $HTMLSelectorsAndHeading, $container);

    // HEADING AND SELECTORS END

    $container = str_replace('@LINK_HOME', l('Home', 'ntlp/home'), $container);
    $container = str_replace('@GRADE', l('Grades', '/ntlp/grades/gradebook'), $container);

    return $container;
}

function ntlp_grades_render_tabs($active_tab = "NOP") {
    global $user;
    drupal_add_css(drupal_get_path('theme', 'Boldr') . '/StylishSelect/stylish-select.css', 'module', 'all', FALSE);
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/externalFile.js');
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/jquery.stylish-select.js');

    include "grade_header.template.inc";



    if ($active_tab == "NOP") {
//        $get = $_GET['q'];
//        $br = explode("/",$get);
//        $cnt = count($br);
        $tab = arg(2);
        if ($tab == 'student_snapshot') {
            $tab = 'students';
        }
        $active_tab = "@IS_" . strtoupper($tab);
    }
    $output = $HTML_grade_tabs;

    if (user_access('view grades')) {

        $my_gradebook_tab = '<li class="@IS_GRADEBOOK"><a class="@IS_GRADEBOOK" href="' . url("/ntlp/grades/gradebook") . '">My Course Gradebooks</a></li>';
        $student_tab = '<li class="@IS_STUDENTS"><a class="@IS_STUDENTS" href="' . url('/ntlp/grades/students') . '">All Students</a></li>';


        if ($active_tab == '@IS_GRADEBOOK') {
            $my_gradebook_tab = str_replace($active_tab, "active", $my_gradebook_tab);
        } else if ($active_tab == '@IS_STUDENTS') {
            $student_tab = str_replace($active_tab, "active", $student_tab);
        }

        $output = str_replace($active_tab, "active", $output);          //Set the active tab
        $output = str_replace("@GRADEBOOK_TAB", $my_gradebook_tab, $output);
        $output = str_replace("@STUDENTS_TAB", $student_tab, $output);
    } else {
        $my_gradebook_tab = '<li class="active"><a class="active" href="' . url("/ntlp/grades/gradebook") . '">My Course Gradebooks</a></li>';
        if (isset($user->roles[NTLP_ROLEID_STUDENT]) || isset($user->roles[NTLP_ROLEID_PARENT])) {
            $output = str_replace("@GRADEBOOK_TAB", $my_gradebook_tab, $output);
            $output = str_replace("@STUDENTS_TAB", '', $output);
        }
//        $output = str_replace("@GRADEBOOK_TAB", '', $output);
//        $output = str_replace("@STUDENTS_TAB", '', $output);
    }

    // HEADING AND SELECTORS
    if ($tab == 'gradebook' || $tab == 'students' || $tab == 'student_snapshot') {
        if ($tab == 'student_snapshot') {
            $selectors = str_replace('<!-- school_year_combo -->', get_school_years_by_id(arg(3), array('class' => 'my-dropdown'), true), $HTMLSchoolYearCombo);
            //Why this role check was placed here?
//            if(isset($user->roles[NTLP_ROLEID_STUDENT])){
            $selectors .= str_replace('<!-- term_selector_combo -->', get_school_term_combo(), $HTMLTermSelectorCombo);
//            }
        } else {
            $selectors = str_replace('<!-- school_year_combo -->', get_school_years_by_id(get_this_school(), array('class' => 'my-dropdown')), $HTMLSchoolYearCombo);

//JR
//            if ($tabName == 'students') {
            $term_tid = get_current_selected_term();        //Call it here so that it sets the session var.
            $selectors .= str_replace('<!-- term_selector_combo -->', get_school_term_combo(), $HTMLTermSelectorCombo);
            //          }
        }
    }
    $output = str_replace('<!-- GRADES_TOOLS_NEW -->', $selectors, $output);
    return $output;
}

?>