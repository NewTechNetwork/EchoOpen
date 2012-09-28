<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_blocks', 'default_header.contents');

drupal_add_css(drupal_get_path('theme', 'Boldr') . '/StylishSelect/stylish-select.css', 'module', 'all', FALSE);
drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/externalFile.js');
drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/jquery.stylish-select.js');
drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/js/jquery.tipsy.js');
module_load_include('inc', 'ntlp_grade', 'data_access');

function guest_enrollment($form_state, $action, $course_nid) {
    // Send the Modal Frame javascript for child windows to the page.
    modalframe_child_js();

    $path = drupal_get_path('module', 'ntlp_people_finder');
    drupal_add_js($path . '/people_finder.js', 'module');


    $form['student_enrollment'] = array(
            '#title' => t('People Finder - Guests'),
            '#type' => 'fieldset',
            '#id' => 'searchp',
    );

    $form['student_enrollment']['finder_name'] = array(
            '#title' => t('Search'),
            '#type' => 'textfield',
            '#size'=> '40',
            '#id' => 'finder_name',
            '#attributes' => array(
                    'onkeydown' => "student_search(window.event.keyCode, 'guests');"
            ),
    );

    $form['student_enrollment']['student_list'] = array(
            '#type' => 'item',
            '#value' => '<div id="searchstudent">'.get_guest_list($course_nid).'</div>',
    );

    $form['student_enrollment']['course_id'] = array(
            '#type' => 'hidden',
            '#id' => 'finder_course_id',
            '#value' => $course_nid,
    );
    $form['student_enrollment']['selected_users_ids'] = array(
            '#type' => 'hidden',
            '#id' => 'selected_users_ids',
//            '#value' => '',
    );
    $form['student_enrollment']['selected_users_names'] = array(
            '#type' => 'hidden',
            '#id' => 'selected_users_names',
//            '#value' => '',
    );

    $form['student_enrollment']['submit'] = array(
            '#type' => 'item',
            '#value' => '<a onclick="selected_users(\''.$action.'\', '.NTLP_ROLEID_SITEGUEST.');">Submit</a>',
    );

    return $form;

}

function multi_user($form_state, $action, $school_nid, $course_nid, $cache_id, $role_id, $is_multi = TRUE) {
    global $user;
    $school_nid = (int)$school_nid;
    $course_nid = (int)$course_nid;

    $users = cache_get($cache_id, 'ntlp_cache')->data;
    $another_var = cache_get($cache_id, 'ntlp_cache')->data;
    $users = $another_var;

    $disclude = (int)arg(7);
    $block_self_selection = arg(8);
    $max_selected = arg(9);

    if (isset($block_self_selection))
        $block_self_selection = 1;
    else
        $block_self_selection = 0;

    $filters['block_self_selection'] = $block_self_selection;

    watchdog("cache get", "id: $cache_id, " . $users);
    $users = empty($users) ? 0 :$users;
    $role_id = (int)$role_id;
    
    // Send the Modal Frame javascript for child windows to the page.
    modalframe_child_js();
    module_load_include('inc', 'ntlp_groups',' filter_functions');
    include_once "people_finder_template.inc";

    $path = drupal_get_path('module', 'ntlp_people_finder');
    drupal_add_js($path . '/people_finder.js', 'module');

    $already_selected_list = array();
    if( $users ) {
        $users = trim($users, ',');
        $already_selected_list = explode(',',$users);
        $already_selected_list = array_unique($already_selected_list);
    }

    $filters['discludeList'] = $already_selected_list;

    if (!isset($max_selected))
        $max_selected = -1;
    else
        $max_selected -= count($already_selected_list);

    $filters['max_selected'] = $max_selected;

    $form['student_enrollment'] = array(
            '#title' => t(''),
            '#type' => 'fieldset',
            '#id' => 'searchp',
            '#prefix' => '<div id="multi_people_finder">',
            '#suffix' => '</div>',
    );
    $users_clicked = isset($_POST['selectedUsers']) ? $_POST['selectedUsers'] : array();
    $form['student_enrollment']['tabs_all_selected'] = array(
            '#type' => 'item',
            '#value' => '<ul>
                            <li id="showallresultBtn" class="active"><a style="text-decoration:none" onclick="show_all_results()">Show All</a></li>
                            <li id="showselectedBtn"><a style="margin-left: 0px;" onclick="show_only_checked()" id="selectedPplBtn">Selected ('.count($users_clicked).')</a></li>
                        </ul>',
            '#prefix' => '<div class="tabs">',
            '#suffix' => '</div><br clear="all" /><div class="content"><div class="top_right"><div class="bot_left"><div class="bot_right"><div class="ppl_finder_data">',

    );

    $form['student_enrollment']['finder_name'] = array(
            '#type' => 'textfield',
            '#size'=> '40',
            '#id' => 'finder_name',
            '#attributes' => array(
                    'onkeyup' => "user_search();",
            ),
            '#prefix' => '<table id="pplSearch"><tr><th classd="box_search" style="color:#3570AA">Search</th><td>',
            '#suffix' => '</td></tr></table>',

        );

        //FILTER START
        drupal_add_js("var school_nid='$school_nid';",'inline');
        if( user_access('allow access to contents of all schools') ) {
            $extraJson['sid'] = 'school_nid';
        }
        $extraJson['selectedUsers'] = "get_selected_ids()";
        $extraJson['name'] = 'filter_name';
        $filterOpts = array(
                '#json' => $extraJson,
                'Type' => array(
                        'All' => array(
                                'type'=>'checkbox',
                                'group'=>'ppltype',
                                'value' => 1,
                                'name' => 'all',
                                'default' => true,
                        ),
                        'Staff' => array(
                                'type'=>'checkbox',
                                'group'=>'ppltype',
                                'value' => 'staff',
                                'name' => 0,
                        ),

                        'Network Manager' => array(
                                'type'=>'checkbox',
                                'group'=>'ppltype',
                                'value' => 'network_manager',
                                'name' => 1,
                        ),
                        'Student 6th' => array(
                                'type'=>'checkbox',
                                'group'=>'ppltype',
                                'value' => '6_grade',
                                'name' => 2,
                        ),
                        'Student 7th' => array(
                                'type'=>'checkbox',
                                'group'=>'ppltype',
                                'value' => '7_grade',
                                'name' => 3,
                        ),
                        'Student 8th' => array(
                                'type'=>'checkbox',
                                'group'=>'ppltype',
                                'value' => '8_grade',
                                'name' => 4,
                        ),
                        'Student 9th' => array(
                                'type'=>'checkbox',
                                'group'=>'ppltype',
                                'value' => '9_grade',
                                'name' => 5,
                        ),
                        'Student 10th' => array(
                                'type'=>'checkbox',
                                'group'=>'ppltype',
                                'value' => '10_grade',
                                'name' => 6,
                        ),
                        'Student 11th' => array(
                                'type'=>'checkbox',
                                'group'=>'ppltype',
                                'value' => '11_grade',
                                'name' => 7,
                        ),
                        'Student 12th' => array(
                                'type'=>'checkbox',
                                'group'=>'ppltype',
                                'value' => '12_grade',
                                'name' => 8,

                            ),
                ),
        );

        if ($action == 'single_student' || $action == 'student_multi') {

            unset($filterOpts['Type']["Staff"]);
            unset($filterOpts['Type']["Network Manager"]);
        }

        $show_staff_only = ($action == 'primaryteacher_multi' ||
                        $action == 'teacher_multi');
        
        if ($show_staff_only) {

            foreach(array(6, 7, 8, 9, 10, 11, 12) as $grade) {

                unset($filterOpts['Type']["Student {$grade}th"]);
            }
            unset($filterOpts['Type']["All"]["default"]);

            $filterOpts['Type']["Staff"]["default"] = true;

            $roles = array();
            $roles[] = NTLP_ROLEID_TEACHER;
            $roles[] = NTLP_ROLEID_SITEADMIN;
            $roles[] = NTLP_ROLEID_SCHOOLLEADER;

            $role_id = $roles;
        }
        else {
            //remove grade years which are not present
            if($grds = search_users_inschool('', $school_nid, $role_id, array('RETURN_GRADS_ARRAY' => true, 'discludeList' => $already_selected_list))) {

                foreach(array(6, 7, 8, 9, 10, 11, 12) as $grade) {
                    $found = false;
                    foreach($grds as $grade_year) {

                        $grade_year_grade = _get_student_grade_by_grad_year($grade_year);

                        if($grade_year_grade == $grade) {
                            $found = true;
                        }
                    }
                    if(!$found) {
                        unset($filterOpts['Type']["Student {$grade}th"]);
                    }
                }
            }
        }

        if( isset($user->roles[NTLP_ROLEID_PARENT]) || isset($user->roles[NTLP_ROLEID_STUDENT]) ) {
            unset($filterOpts['Type']['Network Manager']);
        }
        $urlFilter = '?q=ntlp/people_finder/filter' . ($is_multi ? '/multi' : '/single' );


        $fopts = render_custom_filter(
                $filterOpts,

                array('start' => $HTMLPeopleFinder_filter_cellStart, 'end' => $HTMLPeopleFinder_filter_cellEnd),
                array( 'template' => $HTMLPeopleFinder_filter_heading, 'placeholder' => '@HEADING' ),
                $HTMLPeopleFinder_filter_row_end,
                "$urlFilter/'+school_nid+'/$course_nid/$cache_id/$role_id&action=$action&course_nid=$course_nid&disclude=$disclude&show_staff_only=$show_staff_only&block_self_selection=$block_self_selection&max_selected=$max_selected",
                'searchstudent'
        );


        $fopts = str_replace('@OPTIONS', $fopts, $HTMLPeopleFinder_filter);

        if( user_access('allow access to contents of all schools') ) {
            $school_selector = get_all_school_control( false, array('style' => 'width: 370px;') );
            $fopts = str_replace('@SELECT_SCHOOL_LABEL','Select School:' , $fopts);
            $fopts = str_replace('@SCHOOL_SELECTOR',$school_selector , $fopts);
        } else {
            $fopts = str_replace('@SELECT_SCHOOL_LABEL','' , $fopts);
            $fopts = str_replace('@SCHOOL_SELECTOR','', $fopts);
        }

        //FILTER END

    $filters['action'] = $action;
    $filters['disclude'] = $disclude;
    $filters['course_nid'] = $course_nid;

    $form['student_enrollment']['filter'] = array(
        '#type' => 'item',
        '#value' => '<div id="pplFilter">'.$fopts.'</div>',
    );

    $form['student_enrollment']['student_list'] = array(
            '#type' => 'item',
            '#value' => '<div id="searchstudent">'.get_users_list($school_nid, $course_nid, $users, $role_id, $is_multi, $filters, $show_staff_only).'</div>',
    );

    $form['student_enrollment']['course_id'] = array(
            '#type' => 'hidden',
            '#id' => 'finder_course_id',
            '#value' => $course_nid,
    );
    $form['student_enrollment']['selected_users_ids'] = array(
            '#type' => 'hidden',
            '#id' => 'selected_users_ids',
    );
    $form['student_enrollment']['selected_users_names'] = array(
            '#type' => 'hidden',
            '#id' => 'selected_users_names',
    );

    $form['student_enrollment']['max_selected'] = array(
        '#type' => 'hidden',
        '#id' => 'max_selected',
        '#value' => $max_selected,
    );

    $form['student_enrollment']['submit'] = array(
            '#type' => 'item',
            '#value' => '<table align="right"><tr><td colspan="3" style="font-size:5px;">&nbsp;</td></tr><tr><td><a style="margin:0px;" class="addEvents_btn" onmousedown=this.className="addEvents_btnClicked" onmouseup=this.className="addEvents_btn"  onclick="selected_users(\''.$action.'\', '.$role_id.', \'' . $cache_id . '\', '.$disclude.', \'' . implode(",",$already_selected_list) . '\');">Submit</a></td><td>&nbsp;</td>',
    );
    $form['student_enrollment']['cancel'] = array(
            '#type' => 'item',
            '#value' => '<td><a class="cancelBtn" onclick="parent.Drupal.modalFrame.close();" >Cancel</a></td></tr></table>',
    );
    
    
    return $form;

}

function get_student_list($course_nid) {
    $find = array();
    $string = isset($_GET['name']) ? $_GET['name'] : '';
    $echo = isset($_GET['name']);

    $output = '';

    $res = search_for_enrollment($string, $course_nid, NTLP_ROLEID_STUDENT);

    $output .= '<div style="background-color:white;height:150px;overflow-x:hidden;"><table class="ntlp_table" cellpadding="0" cellspacing="0" width="100%"><tr><th></th><th>Name</th><th>Email</strong></th></tr>';

    if($res != null) {
        // Using Counter so that we can get multiple date for same hidden field
        $i=0;
        while ($row = db_fetch_array($res)) {
            $output .= '<tr><td><input type="checkbox" name="uid[]" id="chk_'.$row['uid'].'" />
                    <input type="hidden" id="uid_'.$row['uid'].'" value="'.$row['uid'].'" />
                    <input type="hidden" id="name_'.$row['uid'].'" value="'.$row['first_name'].' '.$row['last_name'].'" /></td>
                    <td>'.$row['last_name'].', '.$row['first_name'].'</td>
                    <td>'.$row['mail'].'</td></tr>';
            $i++;
        }
    }
    else {

        $output .= '<tr><td colspan="2">No record found</td></tr>';
    }
    $output .= '</table></div>';
    if ($echo)
        echo $output;
    else
        return $output;
}

function get_guest_list($course_nid) {
    $find = array();
    $string = isset($_GET['name']) ? $_GET['name'] : '';
    $echo = isset($_GET['name']);

    $output = '';

    $res = search_for_enrollment($string, $course_nid, NTLP_ROLEID_TEACHER);

    $output .= '<div style="height:150px;overflow-x:hidden;"><table cellpadding="0" cellspacing="0" width="100%"><tr><th></th><th>Name</th><th>Email</strong></th></tr>';

    if($res != null) {
        // Using Counter so that we can get multiple date for same hidden field
        $i=0;
        while ($row = db_fetch_array($res)) {
            $output .= '<tr><td><input type="checkbox" name="uid[]" id="chk_'.$row['uid'].'" />
                    <input type="hidden" id="uid_'.$row['uid'].'" value="'.$row['uid'].'" />
                    <input type="hidden" id="name_'.$row['uid'].'" value="'.$row['first_name'].' '.$row['last_name'].'" /></td>
                    <td>'.$row['last_name'].', '.$row['first_name'].'</td>
                    <td>'.$row['mail'].'</td></tr>';
            $i++;
        }
    }
    else {

        $output .= '<tr><td colspan="2">No record found</td></tr>';
    }
    $output .= '</table></div>';
    if ($echo)
        echo $output;
    else
        return $output;
}

function get_users_list($school_nid, $course_nid, $users, $role_id, $is_multi, $filters = array(), $show_staff_only = false) {
    $find = array();
    $string = isset($_POST['name']) ? $_POST['name'] : '';
    $echo = isset($_POST['name']);
    //$role_id = 0;

    $output = '';

    $already_selected_list = array();
    if( $users ) {
        $users = trim($users, ",");
        $already_selected_list = explode(',', $users);
        $already_selected_list = array_unique($already_selected_list);
    }

    //Course NID may be blank/empty as the source program may just want to search users in a school
//    $enrolled = array();
//    if (intval($course_nid) > 0) {
//        $res = search_users_incourse($string, $course_nid, $role_id, $filters);
//        while($rs = db_fetch_object($res)) {
//            $enrolled[] = $rs->uid;
//        }
//    }
    $filters['course_nid'] = $course_nid;
    $filters['discludeList'] = $already_selected_list;

    $res = search_users_inschool($string, $school_nid, $role_id, $filters, $show_staff_only);
    $users_clicked = isset($_POST['selectedUsers']) && !empty($_POST['selectedUsers']) ? explode(',', $_POST['selectedUsers']) : array();
    $output .= '<div style="font-size:11px;">&nbsp;</div>
                <div class="people_finder_view">
                  <div class="people_finder_view_top_left">
                      <div class="people_finder_view_top_right">
                          <div class="people_finder_view_bottom_left">
                              <div class="people_finder_view_bottom_right">'; //header
    $output .= '<div id="peopleList" style="background-color:white;height:150px;overflow-y:scroll;"><table cellpadding="2" id="pplf_user_list" class="ntlp_table" cellspacing="0" width="99%"><tr><th  style="width: 70px;"> '. ($is_multi && $filters['max_selected'] == -1 ? '<input id="chk_unchk_all" onclick="change_AllSelections();" type="checkbox" />' : '') . ' </th><th>Name</th><th>Type</th><th>Organization</th><th>Grade Level</th></tr>';
    
    if($res != null) {
        // Using Counter so that we can get multiple date for same hidden field
        $i=0;

        $max_selected = $filters['max_selected'];
        $disabled = "";
        if ($max_selected != -1) {
            $max_selected = $max_selected - count($users_clicked);
            $disabled = ($max_selected <= 0) ? 'disabled="disabled"' : '';
        }
        
        while ($row = db_fetch_array($res)) {
            $selected = "";
            if ( in_array($row['uid'], $users_clicked) ) {
                $selected = 'checked = "checked"';
            }
            if ($is_multi) {
                $output .= '<tr style="' . ($selected ? 'display:none;' : '') . '"><td><input class="pfuidcb" type="checkbox" name="uid[]" id="chk_'.$row['uid'].'"'.$selected.' '. $disabled . ' />';
            } else {
                $output .= '<tr><td><input class="pfuidcb" type="radio" name="uid[]" id="chk_'.$row['uid'].'"'.$selected.' />';
            }
            $output .= '<input type="hidden" id="uid_'.$row['uid'].'" value="'.$row['uid'].'" />
                    <input type="hidden" id="name_'.$row['uid'].'" value="'.$row['first_name'].' '.$row['last_name'].'" /></td>
                    <td name="ppluser_name">'.$row['last_name'].', '.$row['first_name'].'</td>
                    <td>'.$row['user_type'].'</td>'
                    .'<td>'.$row['organization'].'</td>'
                    .'<td>'._get_student_grade_by_grad_year($row['grad_year']).'</td>'
                    .'</tr>'
            ;
            $i++;
        }

        if($already_selected_list && !$filters['disclude']) {
            $already_selected_list = "'".implode("','", $already_selected_list)."'";
        } else {
            $already_selected_list = '';
        }
        drupal_add_js("
            var alreadySelectedList = [$already_selected_list];
        ",'inline');
    }
    else {
        $output .= '<tr><td colspan="2">No record found</td></tr>';
    }
    $output .= '</table></div></div></div></div></div></div>';
    if ($echo)
        echo $output;
    else
        return $output;
}



function ntlp_people_finder_filter_multi() {
    print ntlp_people_finder_filter(true);
}

function ntlp_people_finder_filter_single() {
    print ntlp_people_finder_filter(false);
}

function ntlp_people_finder_filter($is_multi = true) {
    global $user;
    list($role_id, $cache_id, $course_nid, $school_nid) = array_reverse(explode('/', $_GET['q']));
    $filters = array();
    if( isset($_GET['action'], $_GET['course_nid']) ) {
        if($_GET['action'] == 'get_rubric_users') {
            $filters['action'] = $_GET['action'];
            $filters['course_nid'] = (int)$_GET['course_nid'];

        }
    }
    
    if (isset($_GET['school_nid'])) {
        $school_nid = $_GET['school_nid'];
    }

    $filters['disclude'] = $_GET['disclude'];

    $users = cache_get($cache_id, 'ntlp_cache')->data;
    $another_var = cache_get($cache_id, 'ntlp_cache')->data;
    $users = $another_var;


    $show_staff_only = $_GET['show_staff_only'];

    $filters['block_self_selection'] = $_GET['block_self_selection'];

    $filters['max_selected'] = $_GET['max_selected'];
    
    $roles = array();

    if( !isset($_POST['ppltype']['all']) and count($_POST['ppltype'])) {
        if(in_array('staff',$_POST['ppltype'])) {
            $roles[] = NTLP_ROLEID_TEACHER;
            $roles[] = NTLP_ROLEID_SITEADMIN;
            $roles[] = NTLP_ROLEID_SCHOOLLEADER;
        }
        if(in_array('network_manager',$_POST['ppltype'])) {
            $roles[] = NTLP_ROLEID_NETWORKMANAGER;
        }

        if(array_intersect( array('6_grade', '7_grade', '8_grade', '9_grade','10_grade','11_grade','12_grade'), $_POST['ppltype'])) {
            $roles[] = NTLP_ROLEID_STUDENT;
            foreach( $_POST['ppltype'] as $ppltype ) {
                $ppltype = (int)$ppltype;
                if( $gradYear = _get_student_grad_year_by_grade($ppltype) ) {
                    $filters['IN_GRAD_YEAR'][] = $gradYear;
                }
            }
        }
    } else {
        $roles = array();           
    }

    return get_users_list($school_nid,$course_nid,$users,$roles, $is_multi, $filters, $show_staff_only);

}
