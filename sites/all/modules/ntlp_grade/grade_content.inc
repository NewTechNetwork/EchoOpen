<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_group', 'filter_functions');

function mycourses_gradebook_contents($all = FALSE) {
    global $user;
    module_load_include('inc', 'ntlp_blocks', 'mycourses.contents');
    
    $school_year = get_this_school_year();

    $attributes = array('class' => 'ntlp_table', 'width' => '100%', 'cellpadding' => '8');

    $header = array(
        array('data' => 'Period', 'field' => 'nc.period', 'sort' => 'asc'),
        array('data' => 'Title', 'field' => 'n.title', 'sort' => 'desc'),
        array('data' => 'Short Title', 'field' => 'course_short_title', 'sort' => 'desc'),
        array('data' => 'Teacher'),
        array('data' => 'Enrollment'),
        array('data' => 'Status'),
    );

    $term_tid = get_current_selected_term();
    $courses = _get_teacher_courses($user->uid, $header, $tab_name == 'teaching', $school_year, $term_tid);

    $totCourses = count($courses);
    for ($i = 0; $i < $totCourses; $i++) {
        $courses[$i]->total_students = _get_courses_enrollment($courses[$i]->nid);
    }

    if (sizeof($courses) > 0) {
        foreach ($courses as $obj) {
            $semestername = $obj->semester_name;
            $rs_user = get_user($obj->teacher1_uid);
            $rs_user2 = get_user($obj->teacher2_uid);

            $tname = $rs_user->first_name . ' ' . $rs_user->last_name;
            $tname2 = '';
            if ($obj->teacher1_uid != $obj->teacher2_uid && $rs_user2) {
                $tname2 = $rs_user2->first_name . ' ' . $rs_user2->last_name;
            }
            $tname = l($tname, "ntlp/user/profile/$obj->teacher1_uid");
            $tname2 = !empty($tname2) ? '<br> ' . l($tname2, "ntlp/user/profile/$obj->teacher2_uid") : ' ';
            $primaryteacher = $tname . $tname2;
            $rows[] = array(
                array('data' => $obj->period),
                array('data' => l($obj->title, 'ntlp/courses/home/' . $obj->nid)),
                array('data' => l($obj->course_short_title, 'ntlp/courses/home/' . $obj->nid)),
                array('data' => $primaryteacher),
                array('data' => $obj->total_students),
                array('data' => ($obj->is_term_active == 1) ? 'Active' : 'Inactive'),
            );
        }

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
        $output .= theme_table($header, $rows, $attributes, $semestername);
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

function grade_students() {
    include('grades_template.inc');

    drupal_add_js(drupal_get_path('module', 'ntlp_grade') . '/ntlp_grades_students.js');
    global $user, $base_path;
    $school_nid = get_this_school();

    $schoolyearoption = get_school_years();
    $output = $HTMLgrade_student_box;

    //Run cron, but first clear the dw table
    ntlp_school_cron($school_nid, true);

    //  ADDING FILTER START
    $options = array(
        '#json' => array('byName' => 'Drupal.myFilterName || ""'),
        '#ajaxCallback' => 'function(){$(\'#studentSearchWait\').fadeOut(); Drupal.myFilterName = \'\';}',
        'Current Students' => array(
            'All' => array(
                'type' => 'checkbox',
                'group' => 'current_students',
                'value' => 1,
                'name' => 'all',
                'default' => true,
            ),
            'Grade 6' => array(
                'type' => 'checkbox',
                'group' => 'current_students',
                'value' => 6,
                'name' => 0,
            ),
            'Grade 7' => array(
                'type' => 'checkbox',
                'group' => 'current_students',
                'value' => 7,
                'name' => 1,
            ),
            'Grade 8' => array(
                'type' => 'checkbox',
                'group' => 'current_students',
                'value' => 8,
                'name' => 2,
            ),
            'Grade 9' => array(
                'type' => 'checkbox',
                'group' => 'current_students',
                'value' => 9,
                'name' => 3,
            ),
            'Grade 10' => array(
                'type' => 'checkbox',
                'group' => 'current_students',
                'value' => 10,
                'name' => 4,
            ),
            'Grade 11' => array(
                'type' => 'checkbox',
                'group' => 'current_students',
                'name' => 5,
                'value' => 11,
            ),
            'Grade 12' => array(
                'type' => 'checkbox',
                'group' => 'current_students',
                'name' => 6,
                'value' => 12,
            ),
        ),
        'Course Grades' => array(
            'All' => array(
                'type' => 'checkbox',
                'group' => 'courses_grade',
                'value' => 1,
                'name' => 'all',
                'default' => true,
            ),
            '100-90%' => array(
                'type' => 'checkbox',
                'group' => 'courses_grade',
                'value' => 100,
                'name' => 0,
            ),
            '89-80%' => array(
                'type' => 'checkbox',
                'group' => 'courses_grade',
                'value' => 89,
                'name' => 1,
            ),
            '79-70%' => array(
                'type' => 'checkbox',
                'group' => 'courses_grade',
                'value' => 79,
                'name' => 2,
            ),
            '69-60%' => array(
                'type' => 'checkbox',
                'group' => 'courses_grade',
                'value' => 69,
                'name' => 3,
            ),
            '59-0%' => array(
                'type' => 'checkbox',
                'group' => 'courses_grade',
                'value' => 59,
                'name' => 4,
            ),
        ),
    );

    $opts = render_custom_filter(
            $options, array('start' => $HTML_grade_school_filter_cellStart, 'end' => $HTML_grade_school_filter_cellEnd), array('template' => $HTML_grade_school_filter_heading, 'placeholder' => '@HEADING'), $HTML_grade_school_filter_row_end, url('ntlp/grade/student/filter'), 'school_grade_students'
    );

    $output = str_replace('@FILTER_PLACEHOLDER', str_replace('@OPTIONS', $opts, $HTML_grade_school_filter), $output);

    //  ADDING FILTER END
    //   echo drupal_get_path('theme', 'Boldr');

    $schools_year = '<select name ="school_year">';
    $schools_year .= $schoolyearoption;
    $schools_year .= '</select>';
    $output = str_replace('@SCHOOL_YEAR', $schools_year, $output);

    if ($school_nid > 0) {
        $student_grades = get_grade_students($school_nid);
        $output = str_replace('@STUDENT_NAME', $student_grades, $output);
    } else {
        $output = str_replace('@STUDENT_NAME', "", $output);
        drupal_set_message("You 're Not Enrolled in any School", "error");
    }

    return $output;
}

function _get_grade_list() {
    global $user;

    $head = array(
        array('data' => t('Period'), 'field' => 'period', 'sort' => 'asc'),
        array('data' => t('Title'), 'field' => 'course_short_title', 'sort' => 'asc'),
    );


    $result = get_semester_courses($user->uid, $head);

    if ($result->num_rows > 0) {
        $semester_name = '';
        while ($record = db_fetch_object($result)) {

            if (($semester_name != $record->name)) {
                $semesters[] = $record->name;
            }

            $rows[$record->name][] = array(
                array('data' => $record->period ? $record->period : '*'),
                array('data' => l($record->course_short_title, 'ntlp/courses/grade/' . $record->course_nid)),
            );

            $semester_name = $record->name;
        }

        $tables = '';
        $semesters = array_unique($semesters);

        // Sorting an Array
        sort($semesters);
        $tables .= '<div>&nbsp;</div>';
        foreach ($semesters as $semester) {
            $attributes = array('class' => 'ntlp_table', 'width' => '100%', 'cellpadding' => '8');
            $tables .= '<div class="student_Panel">
                <div id="ActivitiesCSS" style="width:auto">
                  <div class="BlueBorderBgTopCenter">
                    <div class="BlueBorderBgMiddleRight">
                      <div class="BlueBorderBgBottomCenter">
                        <div class="BlueBorderBgMiddleLeft">
                          <div class="BlueBorderBgTopRightCorner">
                            <div class="BlueBorderBgBottomLeftCorner">
                              <div class="BlueBorderBgBottomRightCorner">
                                <div class="BlueBorderBgTopLeftCorner ntlp_table">';
            $tables .= "<h1>{$semester}</h1>";
            $tables .= theme_table($head, $rows[$semester], $attributes);
            $tables .= '</div>
                    </div>
                  </div>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
    </div>';
        }
    } else {
        drupal_set_message("You are Not enrolled in any courses ", "error");
    }
    return $tables;
}

function get_grade_students($school_nid, $filters=array()) {
    global $base_path;
    $total_students = 0;

//        array('data' => t('Name'), 'field' => 'last_name', 'sort' => 'asc', 'width' => '160', 'style' => 'border-bottom: 14px solid  #fff !important;'),
    $head = array(
        array('data' => t('Name'), 'width' => '160', 'style' => 'border-bottom: 14px solid  #fff !important;'),
        array('data' => t('Grade'), 'style' => 'border-bottom: 14px solid  #fff !important;color:#3491CC !important'),
        array('data' => t('Courses'), 'style' => 'border-bottom: 14px solid  #fff !important;color:#3491CC!important'),
        array('data' => t('Detailed Report'), 'style' => 'border-bottom: 14px solid  #fff !important;color:#3491CC !important'),
    );


    $limit = 50;

    $filtered_user_records = get_users_for_school($school_nid, $head, $limit, array_merge($filters, array('BY_SCHOOL_YEAR' => TRUE)));
    $percent_filters = $filters['GRADE_YEARS']['percent'];

    $i = 0;
    while ($data = db_fetch_object($filtered_user_records)) {
        $record = '';
        $i++;
//drupal_set_message($_SESSION['CURRENT_SCHOOL_TERM']);
        //$courses = _get_filtered_courses_for_each_user($data->uid, $school_nid, get_current_selected_term(), array_merge($filters, array('BY_SCHOOL_YEAR' => TRUE)));
        $term_tid = get_current_selected_term();        //Call it here so that it sets the session var.

        $courses = _get_filtered_courses_for_each_user($data->uid, $school_nid, $term_tid, array_merge($filters, array('BY_SCHOOL_YEAR' => TRUE)));
        if ($courses) {
            foreach ($courses as $data) {

                if ($data->picture == null || empty($data->picture)) {
                    $picture = $base_path . 'themes/Boldr/Images/common/small_person_image_not_found.jpg';
                } else {
                    $picture = $base_path . $data->picture;
                }

                $grade_letter = $data->grade_letter;
                $percent = ' (' . format_course_grade_percent($data->grade_percent, $data->course_nid) . ')';

                if (is_not_graded_course($data->course_nid)) {
                    $grade_letter = '(Not Graded)';
                    $percent = ' ';
                }

                $record .= '<div style ="width:210px ;float:left !important;padding-top:10px !important;border-bottom: 1px dashed #9dccea!important;">' .
                        l($data->course_short_title, 'ntlp/courses/grade/' . $data->course_nid . '/' . $data->uid, array('attributes' => array('style' => 'color:#359800'))) .
                        '&nbsp;<b>' .
                        $grade_letter . $percent .
                        '</b></div>';
            }
        }
        $rows[] = array(
            array('data' => '<div style="float:left;background-color:#d7e9f5 !important"><img src="' . $picture . '" alt="User Image" width="32px" height="32px"></div><div style="float:left;padding:8px 0px 0px 10px;color:#359800;">' . l($data->last_name . ', ' . $data->first_name, 'ntlp/user/profile/' . $data->uid, array('attributes' => array('class' => 'greenLink'))) . '</div>', 'style' => 'background-color:#d7e9f5 !important; border-top: 1px solid #9ecbe8 !important;border-bottom: 14px solid  #fff !important;border-right: 1px solid #9ecbe8 !important;border-left: 1px solid #9ecbe8 !important; margin-bottom:5px !important;'),
            array('data' => ($grade = (int) _get_student_grade_by_grad_year($data->grad_year)) ? $grade . 'th' : 'Not defined', 'style' => 'background-color:#d7e9f5 !important; border-top: 1px solid #9ecbe8 !important;border-bottom: 14px solid  #fff  !important;border-right: 1px solid #9ecbe8 !important;border-left: 1px solid #9ecbe8 !important;padding-bottom: 1em !important; '),
            array('data' => $record, 'style' => 'background-color:#d7e9f5 !important; border-top: 1px solid #9ecbe8 !important;border-bottom: 14px solid  #fff !important;border-right: 1px solid #9ecbe8 !important;border-left: 1px solid #9ecbe8 !important;padding-bottom: 1em !important; '),
            array('data' => l('View', 'ntlp/grades/student_snapshot/' . $data->uid, array('attributes' => array('class' => 'SaveBtn', 'style' => 'color:#FFF !important'))), 'style' => 'background-color:#d7e9f5 !important; border-top: 1px solid #9ecbe8 !important;border-bottom: 14px solid  #fff  !important;border-right: 1px solid #9ecbe8 !important;border-left: 1px solid #9ecbe8 !important;padding-bottom: 1em !important;', 'align' => 'center'),
        );
    }

    if (!$rows) {
        $rows[] = array(array('data' => '<strong>No students found.</strong>', 'colspan' => '4', 'align' => 'center'));
    }

    $attributes = array('class' => 'ntlp_table', 'width' => '100%', 'cellpadding' => '8', 'cellspacing' => '0', 'border-top:1px solid #9ecbe8; border-bottom:1px solid #9ecbe8; border-left:1px solid #9ecbe8 !important; border-right:1px solid #9ecbe8 !important;');

    $output = theme('table', $head, $rows, $attributes);
    $output .= theme('pager', NULL, $limit, 0);



    return '<div id="school_grade_students">' . $output . '</div>';
}

function ntlp_grade_student_filter() {
    $school_nid = get_this_school();
    $grad_years = array();



    if (isset($_REQUEST['current_students']) and !isset($_REQUEST['current_students']['all'])) {
        foreach ($_REQUEST['current_students'] as $grade) {

            $grad_years['year'][] = _get_student_grad_year_by_grade((int) $grade);
        }
    }

    if (isset($_REQUEST['courses_grade']) and !isset($_REQUEST['courses_grade']['all'])) {
        foreach ($_REQUEST['courses_grade'] as $grade) {

            $grad_years['percent'][] = _get_student_grad_year_by_grade((int) $grade);
//            $grad_years[] = $grade;
        }
    }


    $filters = array(
        'GRADE_YEARS' => $grad_years
    );

    if (isset($_REQUEST['byName'])) {
        $byName = trim($_REQUEST['byName']);
        if (!empty($byName)) {
            $filters['BY_NAME'] = $byName;
        }
    }


    $table = get_grade_students($school_nid, $filters);
    print replace_table_header_url('ntlp/grade/student/filter', 'ntlp/grades/students', $table);
}

function get_gradebook_selection_yellow_bar($tab_name, $sort_option) {
    include "grade_header.template.inc";
    global $user;


    if (isset($user->roles[NTLP_ROLEID_NETWORKMANAGER]) || isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])) {
        $school_nid = get_this_school();
    } else {
        $school_nid = get_user_school($user->uid);
    }

    if (isset($_GET['yearby'])) {
        $year_by = $_GET['yearby'];
    } else {
        if (isset($user->roles[NTLP_ROLEID_NETWORKMANAGER]) || isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])) {
            $year_by = get_this_school_year();
        } else {
            $year_by = get_school_year($school_nid);
        }
    }

    if (isset($_GET['termby'])) {
        $term_by = $_GET['termby'];
    } else {
        $term_by = get_current_selected_term();
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

    $yellow_bar .= $HTMLGradebook_yellow_bar;

    if ($tab_name == 'teaching') {

        $user_role = NTLP_ROLEID_TEACHER;

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
                || isset($user->roles[NTLP_ROLEID_NETWORKSTAFF])) {

            $result = get_school_years_for_teacher($user->uid, $user_role);
            if ($result) {
                while ($data = db_fetch_object($result)) {
                    $options_array[$data->school_year] = ($data->school_year - 1) . ' - ' . $data->school_year;
                }
            }
        }


        $year_select = '<select id="view_by" onchange="window.location = \'?q=ntlp/grades/gradebook&viewby=\'+this.value;">
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

        if (isset($user->roles[NTLP_ROLEID_PARENT])) {
            $children_selector = '<div><strong>Student: </strong> &nbsp;' . get_parent_child_selector($user->uid) . '</div>';
        }


        $output = str_replace('<!--VIEW_COURSES_BY-->', $view_course_by, $yellow_bar);
        $output = str_replace('<!--PARENT_CHILD_SELECTOR-->', $children_selector, $output);
        $output = str_replace('<!--SCHOOL_YEAR-->', $school_year, $output);
        $output = str_replace('<!--SCHOOL_TERM-->', $selected_value, $output);
    }


    return $output;
}
