<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'constants');
require_once "data_access.inc";
/* popup form */

function ntlp_project_add_form($form_state) {
    modalframe_parent_js();

    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');

    drupal_set_title("New Project");
    $course_id = check_plain(arg(3));

    ahah_helper_register($form, $form_state);

//    watchdog('add_project', print_r($form_state, true));
    if (isset($form_state['clicked_button']['#post']['op'])) {
        $op = $form_state['clicked_button']['#post']['op'];
        $course_id = $form_state['clicked_button']['#post']['project_add']['course_id'];
    }

    $form['project_add'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div style="width: auto;" id="BoldrProject">
	<div class="ProjectDetail">
		<div class="LightestBlueBottomRightCorner">
			<div class="LightestBlueBottomLeftCorner">
				<div class="LightestBlueTopRightCorner">
					<div class="LightestBlueTopLeftCorner">
      <div id="project-add-test">',
        '#suffix' => '</div>              </div>
				</div>
			</div>
		</div>
	</div>
</div>',
//            '#prefix' => '<div id="project-add-test">'.$HTMLform_activity_outer_name_hdr,
//            '#suffix' => $HTMLform_activity_outer_name_ftr.'</div>',
    );

    if ($op == 'Create Project') {
        if (!empty($form_state['clicked_button']['#post']['project_add']['projectname'])) {
            $record = project_add_data($form, $form_state);
            $form['project_add']['project_status'] = array(
                '#type' => 'item',
                '#value' => "<p>Project: <strong>$record->title</strong> created successfully</p>" .
                l('View Project', 'ntlp/courses/projects/' . $course_id . '/' . $record->nid),
            );
            return $form;
        } else {
            $form['project_add']['project_status'] = array(
                '#type' => 'item',
                '#value' => "<p>Project name can not be empty.</p>",
            );
        }
    }

    $form['project_add']['course_id'] = array(
        '#type' => 'hidden',
        '#value' => $course_id,
    );

    $form['project_add']['projectname'] = array(
        '#type' => 'textfield',
        '#value' => '',
        '#size' => '30',
        '#field_prefix' => '<span style="color:#3570AA;font-weight:bold" >' . t('Project Name') . '</span>',
        '#prefix' => '<div style="margin-bottom:10px">',
        '#suffix' => '</div>',
    );


    $form['project_add']['proj_save_button'] = array(
        '#type' => 'submit',
        '#value' => t('Create Project'),
        '#ahah' => array(
            'path' => ahah_helper_path(array('project_add')),
            'wrapper' => 'project-add-test',
//                    'method' => 'replace',
        ),
        '#attributes' => array('class' => 'AddOutComesBtn', 'onmousedown' => "this.className='AddOutComesBtnClicked'", 'onmouseup' => "this.className='AddOutComesBtn'"),
    );


    return $form;
}

function project_add_data($form, $form_state) {
    global $user;

//    watchdog("crte_project", print_r($form_state, true));
    $dateformat = "%Y-%m-%d %H:%M:%S";

    $course_id = $form_state['clicked_button']['#post']['project_add']['course_id'];
    $project_name = $form_state['clicked_button']['#post']['project_add']['projectname'];


//    watchdog('crte_project2', $course_id . ' project name ' . $project_name);

//  load_project_record($course_id);
    $ctime = time();

    $duration_from_date = strftime($dateformat, time());
    $duration_to_date = strftime($dateformat, time());

//    watchdog('add_project', 'projectname ' . $project_name . ' from date ' . $duration_from_date . ' to date ' . $duration_to_date);

    $project_node = new stdClass();
    $project_node->title = $project_name;
    $project_node->type = 'ntlp_project';   // Your specified content type
    $project_node->created = $ctime;
    $project_node->changed = $ctime;
    $project_node->status = 1;
    $project_node->promote = 0;
    $project_node->sticky = 0;
    $project_node->format = 1;
    $project_node->uid = $user->uid;

    node_save($project_node);
    $school_nid = get_this_school();

    db_query("INSERT INTO {ntlp_course} (nid, school_nid, is_project, course_nid, availability, from_date, to_date)
       VALUES (%d, %d, %d, %d, '%s', '%s', '%s')",
            $project_node->nid, $school_nid, 1, $course_id, 'H', $duration_from_date, $duration_to_date);



    $obj = get_vocabulary_vid('NTLP Course Section');
    $new_term = array('vid' => $obj->vid, 'name' => 'Facilitator Materials');
    taxonomy_save_term($new_term);

//    watchdog('crte_project1', 'first section created ' . print_r($new_term, true));

    db_query("INSERT INTO {ntlp_course_section} VALUES (%d, %d, %d, '%s')",
            $new_term['tid'], $project_node->nid, 0, 'H');

    return $project_node;
}

function edit_project_data($form, $form_state) {

    $dateformat = NTLP_DATE_TZ_SQL;

    $projectnid = $form_state['values']['project_details']['project_id'];
    $projectname = $form_state['clicked_button']['#post']['project_details']['proj_name'];
    $project_availability = $form_state['clicked_button']['#post']['project_details']['proj_availability'];
    $projectname_unit = $form_state['clicked_button']['#post']['project_details']['proj_unit'];
    $projectname_duration = $form_state['clicked_button']['#post']['project_details']['proj_duration'];
    $projectname_projsummary = $form_state['clicked_button']['#post']['project_details']['proj_summary'];


    $proj_duration_from = $form_state['clicked_button']['#post']['project_details']['proj_duration_from'];
    $proj_duration_to = $form_state['clicked_button']['#post']['project_details']['proj_duration_to'];



//    list($publishonmonth,$publishonday,$publishonyear) = split("/",$proj_duration_from);
//
//    $publishhour = $form_state['clicked_button']['#post']['project_details']['proj_duration_from_hour'];
//    $publishminute = $form_state['clicked_button']['#post']['project_details']['proj_duration_from_minute'];
//    $publishmeridian = $form_state['clicked_button']['#post']['project_details']['proj_duration_from_meridian'];
//    if($publishmeridian == 'PM') $duehour +=12;
//
//
//    $druation_from_date = strftime($dateformat, strtotime($publishonyear.'/'.$publishonmonth.'/'.$publishonday.' '.$publishhour.':'.$publishminute.':00'));
//
//
//    /*
//     * availabe to by using hideon date..
//    */
//
//
//    list($hideonmonth,$hideonday,$hideonyear) = split("/",$proj_duration_to);
//    $hidehour = $form_state['clicked_button']['#post']['project_details']['proj_duration_to_hour'];
//    $hideminute = $form_state['clicked_button']['#post']['project_details']['proj_duration_to_minute'];
//    $hidemeridian = $form_state['clicked_button']['#post']['project_details']['proj_duration_to_meridian'];
//    if($hidemeridian == 'PM') $duehour +=12;
//
//    $druation_to_date = strftime($dateformat, strtotime($hideonyear.'/'.$hideonmonth.'/'.$hideonday.' '.$hidehour.':'.$hideminute.':00'));

    $school_nid = get_project_school_nid($projectnid);
    $duration_from_date = set_tz($school_nid, $dateformat, $proj_duration_from);
    $duration_to_date = set_tz($school_nid, $dateformat, $proj_duration_to);


//$duration_from_date = strftime($dateformat, strtotime($proj_duration_from));
//    $duration_to_date = strftime($dateformat, strtotime($proj_duration_to));

//    watchdog('project_date', 'from date ' . $duration_from_date . ' to date ' . $duration_to_date);


    $project_node = node_load($projectnid);

    $project_node->title = $projectname;
    $project_node->body = $projectname_projsummary;

    node_save($project_node);


    db_query("UPDATE {ntlp_course} SET availability='%s', course_short_title='%s',from_date='%s', to_date ='%s'
        WHERE nid = %d", $project_availability, $projectname_unit, $duration_from_date, $duration_to_date, $projectnid);
}

function edit_question_data($form, $form_state) {

    $projectnid = $form_state['values']['project_question']['project_id'];
    $qustion = $form_state['clicked_button']['#post']['project_question']['proj_quest'];

    db_query("UPDATE {ntlp_course} SET driving_question ='%s'WHERE nid = %d", $qustion, $projectnid);
//    watchdog('info', ' inside edit_question_data method ' . $qustion);
}

function edit_grade_data($form, $form_state) {

    $points = $form_state['clicked_button']['#post']['grade']['points'];
    $project_nid = $form_state['clicked_button']['#post']['project_details']['project_id'];

//    watchdog('projoc_edit', 'selected outcome ' . print_r($points, true));
//    watchdog('projoc_edit', 'project id ' . $project_nid);
    if ($points != null) {

        while (list($key, $val) = each($points)) {
            $ockey = substr($key, strpos($key, "_") + 1, strlen($key));
            $ocarray[$ockey] = $ockey;
        }
    }
    db_query("DELETE FROM {ntlp_gb_course_outcome} WHERE course_nid = %d ", $project_nid);

    if (isset($ocarray)) {
        foreach ($ocarray as $obj) {
            db_query("INSERT INTO {ntlp_gb_course_outcome} (tid, course_nid, weight_percent, custom_name, sort_order) VALUES (%d, %d, 0, '', 0)", $obj, $project_nid);
        }
    }
}

function edit_academic_standards_data($form, $form_state) {

//    watchdog('info', ' inside edit_academic_standards_data method ');

    $projectnid = $form_state['clicked_button']['#post']['project_details']['project_id'];
    $academic_standards = $form_state['clicked_button']['#post']['academic_standards']['proj_standars'];

    db_query("UPDATE {ntlp_course} SET academic_standards ='%s'WHERE nid = %d", $academic_standards, $projectnid);
}

function need_to_know_data($form, $form_state) {
    global $user;

    $projectnid = $form_state['clicked_button']['#post']['need_to_know']['project_id'];
    $message = $form_state['clicked_button']['#post']['need_to_know']['message'];

    $dateformat = "%Y-%m-%d %H:%M:%S";
    $ctime = strftime($dateformat, strtotime(date('Y-m-d')));

    $ntk_node = new stdClass();
    $ntk_node->title = $message;
    $ntk_node->type = 'ntlp_project_needtoknow';   // Your specified content type
    $ntk_node->created = time();
    $ntk_node->changed = time();
    $ntk_node->status = 1;
    $ntk_node->promote = 0;
    $ntk_node->sticky = 0;
    $ntk_node->format = 1;
    $ntk_node->uid = $user->uid;

    node_save($ntk_node);
//    watchdog('project_ntk', 'Saved:' . $ntk_node->nid . ', Project:'. $projectnid . ' / ' . $message);

    $result = db_query("INSERT INTO {og_ancestry} (nid, group_nid) VALUES (%d, %d)", $ntk_node->nid, $projectnid);

    return;
}

function ntlp_course_project_filter() {
    include 'project_filter_template.inc';
    $output = $HTMLCourseProjectFilter;

    $course_id = 0;
    foreach (explode($_GET['q']) as $q) {
        if (is_int($q)) {
            $course_id = $q;
        }
    }

    if ($course_id) {
        $courses = _get_course_project_titles();
        $tmpOptions = '';
        foreach ($courses as $course) {
            $tmpOptions .= $HTMLCourseProjectFilter_option;
            $tmpOptions = str_replace('@OPTION_TITLE', $course->course_short_title, $tmpOptions);
            $tmpOptions = str_replace('@OPTION_VALUE', $course->course_short_title, $tmpOptions);
        }
    }

    $output = str_replace('@OPTIONS_PLACEHOLDER', $tmpOptions, $output);

    return $output;
}

function ntlp_project_activities_filter() {
    include 'project_filter_template.inc';
    return $HTMLProjectActivitiesFilter;
}

function ntlp_course_project_activities() {
    global $user;
    $course_id = (int) arg(3);
    $project_id = (int) arg(4);

    if (isset($user->roles[NTLP_ROLEID_PARENT])) {
        $user_id = $_SESSION['PARENT_STUDENT_UID'];
    } else {
        $user_id = $user->uid;
    }

    $output = render_myactivities($course_id, $project_id, $user_id);
    return $output;
}

function ntlp_courses_projects_activities_filter_callback($course_nid) {
    $totalTypes = array(
        'journal' => 'lo_journal',
        'task' => 'lo_task',
        'image' => 'lo_image',
        'workshop' => 'lo_workshop',
        'quiz' => 'quiz',
        'poll' => 'poll',
    );


    $types = array();
    if (!isset($_REQUEST['act_type']['all']) and count($_REQUEST['act_type']) > 0) {
        foreach ($_REQUEST['act_type'] as $typeKey) {
            if (isset($totalTypes[$typeKey])) {
                $types[] = $totalTypes[$typeKey];
            }
        }
    }


    global $user;
    $course_id = (int) arg(5);
    $course_id = $course_id ? $course_id : 'all';

    $project_id = (int) arg(6);
    $project_id = $project_id ? $project_id : 'all';

//    $types = $types ? "'".implode("','", $types)."'" : 'all';

    $user_id = $user->uid;

    $view = views_get_view('ntlp_activities');

    $view->display['default']->display_options['filters'] = array(
        'type' => array(
            'operator' => 'in',
            'value' => $types,
            'table' => '{node}',
            'field' => 'type',
        ),
    );

    $output = $view->preview('default', array($course_id, $project_id, 'all'));
    print $output;
}
