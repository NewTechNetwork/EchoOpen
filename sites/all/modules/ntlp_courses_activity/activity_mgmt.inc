<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_group', 'filter_functions');


function ntlp_course_activity_filter() {
    include 'activities.template.inc';
    return $HTMLCourseActivityFilter;
}


function ntlp_courses_activities_frm($course_nid) {

    return drupal_get_form('ntlp_courses_activities_form', $course_nid);
}

function ntlp_courses_activities_form($form_state, $course_nid) {

//    variable_set('activity_total', 0);
    global $base_path;
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/ntlp_courses_activity.js');
    drupal_add_css(drupal_get_path('module', 'jquery_ui') . '/jquery.ui/themes/base/ui.all.css');

    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/js/jquery.tipsy.js');
    jquery_ui_add('ui.accordion');

    drupal_add_js('
        $(function (){
            $(".addnew").tipsy({
            className : "grey-middle",
            arrowClass : "grey-arrow",
            delayIn: 0,
            delayOut: 0,
            fade: false,
            fallback: "",
            html: true,
            live: false,
            offset: 0,
            opacity: 1,
            title: function(){
                    return $(".addnewActivity").html();
                },
            trigger: "manual"
                });
        });

', inline);

    /* drupal_add_js('$(function() {
      $( "#accordion" ).accordion({
      collapsible: true,
      });

      var $dialog = $("<div></div>").html("data").dialog({
      autoOpen: false,
      title: "Basic Dialog"
      });

      });
      ', 'inline'); */

    $user_role = check_user_permission($course_nid);

//    watchdog('user_role', 'user role in course ' . $user_role);
    $sort_option = $_GET['sort'];

    if (isset($sort_option)) {
        $_SESSION['ntlp_course_activity_:_activities_by'] = $sort_option;
    } else {
        $sorted_by = $_SESSION['ntlp_course_activity_:_activities_by'];
        drupal_goto('ntlp/courses/activity/' . $course_nid, array('sort' => $sorted_by));
    }

    if (empty($sort_option)) {
        $sort_option = 'project';
    }

    set_item_url_handler('Activity View');

    $form['main'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div id="dialog-show-confirmation" style="display: none;" title="Message:">
                        <span class="msg">Are you sure you want to submit the form?</span>
                    </div><table cellspacing="0" cellpadding="0" border="0" width="100%" >',
        '#suffix' => '</table>',
    );

    $form['main']['course_nid'] = array(
        '#type' => 'hidden',
        '#value' => $course_nid,
        '#id' => 'course_nid',
    );

    $data = '';

    if ($user_role != NTLP_ROLEID_STUDENT && !isset($user->roles[NTLP_ROLEID_PARENT]) && $user_role != NTLP_ROLEID_SITEGUEST) {
        $create_button = '+ <a class="addnew green_link" onclick="show_addnew_tipsy();" ><strong>Add New Activity</strong></a>
            <div class="addnewActivity" style="display: none;">
                 <div style="margin-bottom:5px;"><a class="tipsygreenlinks"  href="' . url('ntlp/courses/activity/new/' . $course_nid) . '&type=lo_task">Task</a></div>
                 <div style="margin-bottom:5px;"><a class="tipsygreenlinks"  href="' . url('ntlp/courses/activity/new/' . $course_nid) . '&type=lo_journal">Journal</a></div>
                 <div style="margin-bottom:5px;"><a class="tipsygreenlinks"  href="' . url('ntlp/courses/activity/new/' . $course_nid) . '&type=lo_workshop">Workshop</a></div>
             </div>';
    } else {
        $create_button = '';
    }

    $option = array("project" => "Project"
        , "general" => "General Purpose (non-project)"
        , "lo" => "Learning Outcome"
        , "dd" => "Due Date"
        , "type" => "Type"
    );


    $activity_data = ntlp_courses_activity_render($course_nid, $sort_option, $user_role);
    $activity_total = get_activities_total_counts($course_nid, $sort_option, $user_role);



    $form['main']['home_page_header'] = array(
        '#type' => 'select',
        '#options' => $option,
        '#default_value' => $sort_option,
        '#attributes' => array(
            'onchange' => 'window.location = \'?q=ntlp/courses/activity/' . $course_nid . '&sort=\'+this.value;'),
        '#prefix' => '<tr><td colspan="2"><div class="teacherProfile Activity_Detail-Grading" style="margin:0px;">
                            <div class="yellowBacktopleft">
                              <div class="yellowBacktopright">
                                <div class="yellowBackbottomleft">
                                  <div class="yellowBackbottomright" style="padding:0px;">
                                  <div style="padding:5px">
                                   <table cellspacing="0" cellpadding="0" border="0" width="100%"><tr valign="top" ><td valign="middle" style="width: 120px;"> View Activities By:</td><td>',
        '#suffix' => '</td><td align="right" style="color:#666" id ="" valign="middle"><span>Total Activities Shown: <b>' . $activity_total . '</b></span></td>
                                ',
    );

    $form['main']['create_button'] = array(
        '#type' => 'item',
        '#value' => $create_button,
        '#prefix' => '<td colspan="2" ><div style="float: right;margin-top:5px;">',
        '#suffix' => '</td></tr></table>
                     </div>
                              </div>
                            </div>
                              </div>
                            </div>
                          </div></td></tr>',
    );

    $expandstate = $_GET['expand'];

    if (empty($expandstate)) {
        $expandstate = 0;
    }
    $expandid = 'grades_tab_students';
    $collapseid = 'grades_tab_activities';
    $EXPAND_STATE = '<tr><td colspan="2">';
//    $EXPAND_STATE=$EXPAND_STATE . '<script language="javascript"> function changecolor(caller) { var other="collapse_all"; if (caller==other) {document.getElementById("expand_all").style.color=="#80D23D"; document.getElementById("collapse_all").style.color="#3570aa";}  document.getElementById(caller.id).style.color="#3570aa"; if (document.getElementById("expand_all").style.color=="#3570aa") {document.getElementById("collapse_all").style.color=="#80D23D";} elseif (document.getElementById("expand_all").style.color=="#80D23D") {document.getElementById("expand_all").style.color=="#3570aa";} } </script>';
    $EXPAND_STATE = $EXPAND_STATE . '<div class="GradesTabCenter" style=" ' . (($sort_option == 'general' || $activity_total <= 0) ? 'display:none;' : '') . ' float: left; white-space: nowrap;">';
    $EXPAND_STATE = $EXPAND_STATE . '<ul style="margin-top:16px;">';
    $EXPAND_STATE = $EXPAND_STATE . '<li><a id="expand_all" style="font-size: 11px;" class="GradesTabActive" onclick="expand_all_panels(1); ">Expand All</a></li>';
    $EXPAND_STATE = $EXPAND_STATE . '<li>|</li>';
    $EXPAND_STATE = $EXPAND_STATE . '<li><a id="collapse_all" style="font-size: 11px;" onclick="expand_all_panels(0); ">Collapse All</a></li>';
    $EXPAND_STATE = $EXPAND_STATE . '</ul> </div>';
    $form['main']['heading'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => $EXPAND_STATE,
        '#suffix' => '</td></tr><tr><td colspan="2">&nbsp;</td></tr>',
    );

    $form['main']['activity_data'] = array(
        '#type' => 'item',
        '#value' => $activity_data,
        '#prefix' => '<tr><td colspan="2" id="activity_data">',
        '#suffix' => '</td></tr>',
    );

    return $form;
}

function ntlp_courses_activity_render($course_nid, $option, $user_role) {
    global $user;
    $dateformat = "m/d/Y";
    $html = '
    <style>
    
        /*table.ntlp_table {
         margin-top: 0 !important;
        }
        .ui-dialog {
            height:180px !important;
        }
        .ui-widget-content {
            background-color:#fff;
            padding:5px;
        }
        .ui-widget-header {
            background-color:#fff;
        }
        
        .ui-state-default, .ui-widget-content .ui-state-default {
            border:none;
        }
        .ui-dialog-content p {
            background-color:#fff;
            margin:0px;
            padding:5px 0px 5px 0px;
        }
        .ui-dialog-title {
            color:#3570AA;
            font-size:14px;
            font-weight:bold
        }
        .ui-widget-content {
            border:none !important;
        }
        .ui-widget-header {
            background:none;
            border:none;
        }
        .ui-widget-content {
            color:#222222;
        }
*/
        </style> 
      
        <div id="notaccordion">';

    $current_term_tid = get_current_selected_course_term($course_nid);
    $user_uid = $user->uid;

    if (isset($user->roles[NTLP_ROLEID_PARENT])) {
        $user_uid = $_SESSION['PARENT_STUDENT_UID'];
    }

    if ($user_role == NTLP_ROLEID_STUDENT || isset($user->roles[NTLP_ROLEID_PARENT])) {

        $query = "/* SLAVE */@SELECT ca.nid, ca.course_nid, ca.due_date, n.type AS node_type, n.nid AS node_nid, n.title AS node_title,
                s.nid AS submission_nid, s.user_uid, s.date_opened, s.date_submitted, s.is_final, s.note_to_teacher,
                s.version_num, ca.submission_type,
                ca.allow_late_submission, ca.is_published AS is_published, ca.is_graded AS act_is_graded, c.school_nid,
                g.is_complete, g.is_late, (CASE WHEN g.id IS NULL THEN 0 ELSE 1 END) is_graded, g.date_graded, g.teachers_notes
            FROM dpl_ntlp_course_activity ca
            INNER JOIN dpl_ntlp_activity_user au ON au.activity_nid = ca.nid
            LEFT JOIN {ntlp_activity_submission} s ON s.user_uid = au.user_uid AND s.activity_nid = ca.nid AND s.is_final = 1
            LEFT JOIN {ntlp_gb_grade} g ON s.activity_nid = g.activity_nid AND s.user_uid = g.user_uid
            INNER JOIN dpl_node n ON n.nid = ca.nid
            INNER JOIN dpl_ntlp_course c ON ca.course_nid = c.nid
            @JOIN
            WHERE ca.course_nid = %d AND au.user_uid = %d @AND
            AND ( (ca.availability_mode = 'S' AND ca.available_from <= NOW() AND ca.available_to >= NOW()) 
                OR (ca.availability_mode = 'M' AND ca.available_from <> '0000-00-00 00:00:00') ) 
            ORDER BY ca.due_date DESC ";
    } else {
        $query = "/* SLAVE */@SELECT ca.nid, ca.course_nid, ca.due_date, ca.published_date, ca.is_graded, n.type AS node_type, n.nid AS node_nid, n.title AS node_title,
                ca.submission_type,
                (SELECT (SUM(CASE WHEN s.version_num >= '1' AND s.is_final = 1 THEN 1 ELSE 0 END)/(SELECT COUNT(*)
               FROM dpl_ntlp_activity_user au1 WHERE au1.activity_nid = ca.nid)) * 100
                    FROM {ntlp_activity_submission} s
                        INNER JOIN dpl_ntlp_activity_user au2 ON s.activity_nid = au2.activity_nid AND au2.user_uid = s.user_uid
                        INNER JOIN dpl_ntlp_course_user cu ON cu.user_uid = s.user_uid
                        WHERE ca.course_nid = cu.course_nid AND s.activity_nid = ca.nid
                        GROUP BY s.activity_nid) AS submissions_percent,
               ca.available_from AS available_from,
               ca.available_to AS available_to,
               ca.availability_mode AS availability_mode,
               ca.is_published AS is_published,
               c.school_nid
             FROM dpl_ntlp_course_activity ca
             INNER JOIN dpl_node n ON ca.nid = n.nid
             INNER JOIN dpl_ntlp_course c ON ca.course_nid = c.nid
             @JOIN
             WHERE (ca.course_nid = %d) @AND
             ORDER BY ca.due_date DESC ";
    }

    if ($option == 'project') {

        if (($user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_NETWORKMANAGER
                || $user_role == NTLP_ROLEID_NETWORKSTAFF || $user_role == NTLP_ROLEID_SITEADMIN
                || $user_role == NTLP_ROLEID_SCHOOLLEADER || $user_role == NTLP_ROLEID_SITEGUEST) && !isset($user->roles[NTLP_ROLEID_PARENT])) {

            $result = db_query("/* SLAVE */SELECT DISTINCT c.nid, n.title, c.from_date, c.to_date
            FROM {ntlp_course} c
            INNER JOIN dpl_node n ON n.nid = c.nid
            INNER JOIN dpl_ntlp_course_activity ca ON ca.project_nid = c.nid
            WHERE c.course_nid = %d AND c.is_project = 1 AND ca.school_term_tid = %d
            ORDER BY c.from_date DESC, n.title ASC ", $course_nid, $current_term_tid);

            while ($data = db_fetch_object($result)) {

                $query = str_replace('@SELECT', 'SELECT', $query);
                $query = str_replace('@JOIN', '', $query);
                $query = str_replace('@AND', 'AND (ca.project_nid = %d) AND (ca.school_term_tid = %d)', $query);

                $activity_result = db_query($query, $course_nid, $data->nid, $current_term_tid);

                if ($activity_result->num_rows > 0) {

                    $activity_rows = ntlp_render_activity_table($current_term_tid, $activity_result, $user_role);

                    $html .= '<div class="ActivitiesPanel notaccordion">
                    <div style="margin-top:20px">
                    <div class="tbl_heading">
                        <div style="float:left">
                            <div style="float:left"><span class="ui-icon ui-icon-triangle-1-s"></span></div>
                            
                            <a href="' . url('ntlp/courses/projects/' . $course_nid . '/' . $data->nid) . '">' . $data->title . '</a> (' . date($dateformat, strtotime($data->from_date)) . ' - ' . date($dateformat, strtotime($data->to_date)) . ')
                        </div>
                        <b style="color: rgb(153, 153, 153); float: right;">Activity Count: ' . $activity_result->num_rows . ' </b>
                   </div>
                   </div>
                   </div>';
                    $html .= '<div style="clear:both">
                            <table cellspacing="0" class="StateTable" cellpadding="0" border="0" width="100%"  >';
                    $html .= '<tr id = "row_' . $data->nid . '"><td colspan="2" >' . $activity_rows . '</td></tr></table><div style="font-size: 10px;">&nbsp;</div></div>';
                } else {
                    $html .= '';
                }
            }
        } else if ($user_role == NTLP_ROLEID_STUDENT || isset($user->roles[NTLP_ROLEID_PARENT])) {

            $result = db_query("/* SLAVE */SELECT DISTINCT c.nid, n.title, n.nid, c.from_date, c.to_date
                FROM dpl_ntlp_course c
                INNER JOIN dpl_node n ON c.nid = n.nid
                LEFT JOIN dpl_ntlp_course c2 ON n.nid = c2.nid
                INNER JOIN dpl_ntlp_course_activity ca ON ca.project_nid = c.nid
                WHERE (c.is_project <> 0) AND (c.course_nid = %d) AND (c.availability = 'S') AND ca.school_term_tid = %d
                GROUP BY c.nid ORDER BY n.title ASC, c.from_date DESC", $course_nid, $current_term_tid);

            while ($data = db_fetch_object($result)) {

                $query = str_replace('@SELECT', 'SELECT', $query);
                $query = str_replace('@JOIN', '', $query);
                $query = str_replace('@AND', ' AND ca.project_nid = %d AND (ca.school_term_tid = %d)', $query);


                $activity_result = db_query($query, $course_nid, $user_uid, $data->nid, $current_term_tid);

                $count_activities += $activity_result->num_rows;

                if ($activity_result->num_rows > 0) {

                    $activity_rows = ntlp_render_activity_table($current_term_tid, $activity_result, $user_role);

                    $html .= '<div class="ActivitiesPanel notaccordion">

              <div style="padding-top:20px">
              		<div class="tbl_heading"><div style="float:left">
                            <div style="float:left"><span class="ui-icon ui-icon-triangle-1-s"></span></div>
                             <a  href="' . url('ntlp/courses/projects/' . $course_nid . '/' . $data->nid) . '">' . $data->title . '</a> (' . date($dateformat, strtotime($data->from_date)) . ' - ' . date($dateformat, strtotime($data->to_date)) . ')</div><b style="color: rgb(153, 153, 153); float: right;">Activity Count: ' . $activity_result->num_rows . ' </b></div>
               </div>
              </div>';

                    $html .= '<div style="clear:both">
                            <table cellspacing="0" class="StateTable" cellpadding="0" border="0" width="100%"  >';
                    $html .= '<tr id = "row_' . $data->nid . '"><td colspan="2" >' . $activity_rows . '</td></tr></table><div style="font-size: 10px;">&nbsp;</div></div>';
                } else {
                    $html .= '';
                }
            }
        }
    } else if ($option == 'general') {

        $query = str_replace('@SELECT', 'SELECT', $query);
        $query = str_replace('@JOIN', '', $query);
        $query = str_replace('@AND', ' AND (ca.project_nid = 0) AND (ca.school_term_tid = %d)', $query);

        if (($user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_NETWORKMANAGER
                || $user_role == NTLP_ROLEID_NETWORKSTAFF || $user_role == NTLP_ROLEID_SITEADMIN
                || $user_role == NTLP_ROLEID_SCHOOLLEADER || $user_role == NTLP_ROLEID_SITEGUEST) && !isset($user->roles[NTLP_ROLEID_PARENT])) {

            $activity_result = db_query($query, $course_nid, $current_term_tid);
        } else if ($user_role == NTLP_ROLEID_STUDENT || isset($user->roles[NTLP_ROLEID_PARENT])) {

            $activity_result = db_query($query, $course_nid, $user_uid, $current_term_tid);
        }

        $activity_rows = ntlp_render_activity_table($current_term_tid, $activity_result, $user_role);

        $html .= '<table cellspacing="0" class="StateTable" cellpadding="0" border="0" width="100%"  >';

        $html .= '<tr id = "row_' . $data->nid . '"><td colspan="2" >' . $activity_rows . '</td></tr></table><div style="font-size: 10px;">&nbsp;</div></div>';

    } else if ($option == 'lo') {

        $result = db_query("/* SLAVE */SELECT co.tid, co.weight_percent, td.name
            FROM dpl_ntlp_gb_course_outcome co
            INNER JOIN dpl_term_data td ON td.tid = co.tid
            WHERE co.course_nid = %d ORDER BY co.weight_percent DESC, td.name ASC", $course_nid);

        if (($user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_NETWORKMANAGER
                || $user_role == NTLP_ROLEID_NETWORKSTAFF || $user_role == NTLP_ROLEID_SITEADMIN
                || $user_role == NTLP_ROLEID_SCHOOLLEADER || $user_role == NTLP_ROLEID_SITEGUEST) && !isset($user->roles[NTLP_ROLEID_PARENT])) {

            while ($data = db_fetch_object($result)) {

                $query = str_replace('@SELECT', 'SELECT DISTINCT ', $query);
                $query = str_replace('@JOIN', ' INNER JOIN dpl_ntlp_gb_activity_outcome ao ON ao.activity_nid = n.nid ', $query);
                $query = str_replace('@AND', ' AND (ao.tid = %d) AND (ca.school_term_tid = %d) ', $query);

                $activity_result = db_query($query, $course_nid, $data->tid, $current_term_tid);

                $expanded = "";
                if ($data->tid == $_GET['def'])
                    $expanded = "def_mode_exp";
                else
                    $expanded = "def_mode_col";

                $html .= '<div class="ActivitiesPanel notaccordion ' . $expanded . '">

              <div style="padding-top:20px">
                <div class="tbl_heading"><div style="float:left">
                    <div style="float:left"><span class="ui-icon ui-icon-triangle-1-s"></span></div>
                    <a style="color:#000 !important; text-decoration:none !important;" class="activity_accordion" >' . $data->name . '</a> (Weight = ' . intval($data->weight_percent) . '%)</div><b style="color: rgb(153, 153, 153); float: right;">Activity Count: ' . $activity_result->num_rows . ' </b></div>
               </div>
              </div>';
                $html .= '<div style="clear:both"><table cellspacing="0" class="StateTable" cellpadding="0" border="0" width="100%"  >';
                $html .= '<tr id = "row_' . $data->tid . '"><td colspan="2" >' . ntlp_render_activity_table($current_term_tid, $activity_result, $user_role) . '</td></tr></table><div style="font-size: 10px;">&nbsp;</div></div>';
            }
        } else if ($user_role == NTLP_ROLEID_STUDENT || isset($user->roles[NTLP_ROLEID_PARENT])) {

            while ($data = db_fetch_object($result)) {
                $query = str_replace('@SELECT', 'SELECT ', $query);
                $query = str_replace('@JOIN', ' INNER JOIN dpl_ntlp_gb_activity_outcome ao ON ao.activity_nid = ca.nid ', $query);
                $query = str_replace('@AND', ' AND (ao.tid = %d) AND (g.date_graded IS NOT NULL) AND (ca.school_term_tid = %d) ', $query);

                $activity_result = db_query($query, $course_nid, $user_uid, $data->tid, $current_term_tid);

                $expanded = "";
                if ($data->tid == $_GET['def'])
                    $expanded = "def_mode_exp";
                else
                    $expanded = "def_mode_col";

                $html .= '<div class="ActivitiesPanel notaccordion ' . $expanded . '">

              <div style="padding-top:20px">
                <div class="tbl_heading"><div style="float:left">
                    <div style="float:left"><span class="ui-icon ui-icon-triangle-1-s"></span></div>
                    <a  style="color:#000 !important; text-decoration:none !important;" class="activity_accordion">' . $data->name . '</a> (Weight = ' . $data->weight_percent . '%)</div><b style="color: rgb(153, 153, 153); float: right;">Activity Count: ' . $activity_result->num_rows . '  </b></div>
               </div>
              </div>';

                $html .= '<div style="clear:both"><table cellspacing="0" class="StateTable" cellpadding="0" border="0" width="100%"  >';
                $html .= '<tr id = "row_' . $data->tid . '"><td colspan="2" >' . ntlp_render_activity_table($current_term_tid, $activity_result, $user_role) . '</td></tr></table><div style="font-size: 10px;">&nbsp;</div></div>';
            }
        }

        $count_activities += $activity_result->num_rows;
    } else if ($option == 'type') {

        $result = db_query("/* SLAVE */SELECT DISTINCT n.type AS node_type
            FROM dpl_ntlp_course_activity ca
            INNER JOIN dpl_node n ON ca.nid = n.nid 
            WHERE ca.course_nid = %d ORDER BY n.type ASC ", $course_nid);

        if (($user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_NETWORKMANAGER
                || $user_role == NTLP_ROLEID_NETWORKSTAFF || $user_role == NTLP_ROLEID_SITEADMIN
                || $user_role == NTLP_ROLEID_SCHOOLLEADER || $user_role == NTLP_ROLEID_SITEGUEST) && !isset($user->roles[NTLP_ROLEID_PARENT])) {

            while ($data = db_fetch_object($result)) {

                $query = str_replace('@SELECT', 'SELECT', $query);
                $query = str_replace('@JOIN', '', $query);
                $query = str_replace("@AND", " AND ('%s' IN (n.type)) AND (ca.school_term_tid = %d) ", $query);

                $activity_result = db_query($query, $course_nid, $data->node_type, $current_term_tid);
                $activity_rows = ntlp_render_activity_table($current_term_tid, $activity_result, $user_role);


                $html .= '<div class="ActivitiesPanel notaccordion">
              <div style="padding-top:20px">
                <div class="tbl_heading"><div style="float:left">
                    <div style="float:left"><span class="ui-icon ui-icon-triangle-1-s"></span></div>
                    <a style="color:#000 !important; text-decoration:none !important;" class="activity_accordion">' . get_activity_name($data->node_type) . '</a>
                </div>
                <b style="color: rgb(153, 153, 153); float: right;">Activity Count: ' . $activity_result->num_rows . ' </b></div>
               </div>
              </div>';
                $html .= '<div style="clear:both"><table cellspacing="0" class="StateTable" cellpadding="0" border="0" width="100%"  >';
                $html .= '<tr id = "row_' . $data->node_type . '"><td colspan="2" >' . $activity_rows . '</td></tr></table><div style="font-size: 10px;">&nbsp;</div></div>';
            }
        } else if ($user_role == NTLP_ROLEID_STUDENT || isset($user->roles[NTLP_ROLEID_PARENT])) {

            while ($data = db_fetch_object($result)) {

                $query = str_replace('@SELECT', 'SELECT', $query);
                $query = str_replace('@JOIN', '', $query);
                $query = str_replace("@AND", " AND ('%s' IN (n.type)) AND (ca.school_term_tid = %d)", $query);

                $activity_result = db_query($query, $course_nid, $user_uid, $data->node_type, $current_term_tid);
                $activity_rows = ntlp_render_activity_table($current_term_tid, $activity_result, $user_role);

                $html .= '<div class="ActivitiesPanel notaccordion">

              <div style="padding-top:20px">
                <div class="tbl_heading"><div style="float:left">
                    <div style="float:left"><span class="ui-icon ui-icon-triangle-1-s"></span></div>
                    <a style="color:#000 !important; text-decoration:none !important;" class="activity_accordion" >' . get_activity_name($data->node_type) . '</a></div><b style="color: rgb(153, 153, 153); float: right;">Activity Count: ' . $activity_result->num_rows . ' </b></div>
               </div>
              </div>';

                $html .= '<div style="clear:both"><table cellspacing="0" class="StateTable" cellpadding="0" border="0" width="100%"  >';
                $html .= '<tr id = "row_' . $data->node_type . '"><td colspan="2" >' . $activity_rows . '</td></tr></table><div style="font-size: 10px;">&nbsp;</div></div>';
            }
        }
    } else if ($option == 'dd') {

        $due_date_arr = array('up' => 'Upcoming', 'pd' => 'Past Due');

        foreach ($due_date_arr as $key => $value) {
            $temp_query = $query;

            if ($key == 'pd') {
                $due_date_condition = 'AND (ca.due_date <= NOW()) AND (ca.school_term_tid = %d)';
            } else if ($key == 'up') {
                $due_date_condition = 'AND (ca.due_date >= NOW()) AND (ca.school_term_tid = %d)';
            }

            if (($user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_NETWORKMANAGER
                    || $user_role == NTLP_ROLEID_NETWORKSTAFF || $user_role == NTLP_ROLEID_SITEADMIN
                    || $user_role == NTLP_ROLEID_SCHOOLLEADER || $user_role == NTLP_ROLEID_SITEGUEST) && !isset($user->roles[NTLP_ROLEID_PARENT])) {

                $temp_query = str_replace('@SELECT', 'SELECT', $temp_query);
                $temp_query = str_replace('@JOIN', '', $temp_query);
                $temp_query = str_replace("@AND", $due_date_condition, $temp_query);

                $activity_result = db_query($temp_query, $course_nid, $current_term_tid);
            } else if ($user_role == NTLP_ROLEID_STUDENT || isset($user->roles[NTLP_ROLEID_PARENT])) {

                $temp_query = str_replace('@SELECT', 'SELECT', $temp_query);
                $temp_query = str_replace('@JOIN', '', $temp_query);
                $temp_query = str_replace("@AND", $due_date_condition, $temp_query);
                
                $activity_result = db_query($temp_query, $course_nid, $user_uid, $current_term_tid);
            }
            $count_activities += $activity_result->num_rows;
            $activity_rows = ntlp_render_activity_table($current_term_tid, $activity_result, $user_role);


            $html .= '<div class="ActivitiesPanel notaccordion">

              <div style="padding-top:20px">
                <div class="tbl_heading"><div style="float:left">
                <div style="float:left"><span class="ui-icon ui-icon-triangle-1-s"></span></div>
                <a style="color:#000 !important; text-decoration:none !important;" class="activity_accordion">' . $value . '</a></div><b style="color: rgb(153, 153, 153); float: right;">Activity Count: ' . $activity_result->num_rows . ' </b></div>
               </div>
              </div>';

            $html .= '<div style="clear:both"><table cellspacing="0" class="StateTable" cellpadding="0" border="0" width="100%" >';
            $html .= '<tr id = "row_' . $key . '"><td colspan="2" >' . $activity_rows . '</td></tr></table><div style="font-size: 10px;">&nbsp;</div></div>';
        }
    }
    $html .= '</div>';

    return $html;
}

function ntlp_render_activity_table($current_term_tid, $resultset, $user_role, $allow_sorting = false) {
    global $base_path, $user;
    $dateformat = "m/d/Y";

    $row_count = 0;

    if (($user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_NETWORKMANAGER
            || $user_role == NTLP_ROLEID_NETWORKSTAFF || $user_role == NTLP_ROLEID_SITEADMIN
            || $user_role == NTLP_ROLEID_SCHOOLLEADER || $user_role == NTLP_ROLEID_SITEGUEST) && !isset($user->roles[NTLP_ROLEID_PARENT])) {

        if ($allow_sorting) {
            $head = array(
                array('data' => t('Due Date'), 'field' => 'due_date', 'width' => '10%'),
                array('data' => t('Activity'), 'field' => 'node_title', 'width' => '45%'),
                array('data' => t('Type'), 'field' => 'node_type', 'width' => '15%'),
                array('data' => t('View'), 'field' => 'available_from', 'width' => '10%'),
                array('data' => t('Submitted'), 'field' => 'submissions_percent', 'width' => '10%'),
                array('data' => t('Grades Published'), 'field' => 'published_date', 'width' => '10%'),
            );
        } else {
            $head = array(
                array('data' => t('Due Date'), 'width' => '10%'),
                array('data' => t('Activity'), 'width' => '45%'),
                array('data' => t('Type'), 'width' => '15%'),
                array('data' => t('View'), 'width' => '10%'),
                array('data' => t('Submitted'), 'width' => '10%'),
                array('data' => t('Grades Published'), 'width' => '10%'),
            );
        }

        if ($resultset->num_rows > 0) {
            while ($data = db_fetch_object($resultset)) {

                if ($current_nid != $data->nid) {
                    $row_count++;
                    $current_nid = $data->nid;

                    $edit_link = "";
                    if ($user_role != NTLP_ROLEID_SITEGUEST) {
                        $edit_link = '<span style="float:right;margin:-15px 0px 0px  0px">
                            <a href="' . url('ntlp/courses/activity/edit/' . $data->course_nid . '/' . $data->nid) . '" >
                                <img src="' . $base_path . 'themes/Boldr/Images/common/edit.png" alt="editimage"/>
                            </a></span>';
                    }

                    $work_and_assesment = '&nbsp; |&nbsp;
                            <a  class="default"  href="@LINK">Work and Assessment</a></span>';
                    $work_and_assesment = str_replace("@LINK", url('ntlp/courses/activity/view/' . $data->course_nid . '/' . $data->nid, array('query' => 'tab=work')), $work_and_assesment);

                    $overview_link = url('ntlp/courses/activity/view/' . $data->course_nid . '/' . $data->nid);

                    $activity_title = '<span><strong><b>' . $data->node_title . '</strong></b><br/>
                    <a class="default" href="' . $overview_link . '">Overview</a> ' . $work_and_assesment
                            . $edit_link;

                    if ($data->node_type == 'lo_journal' || $data->submission_type == 'D') {
                        $submission_percent = (!empty($data->submissions_percent) ? (round($data->submissions_percent) == 0 ? '' : round($data->submissions_percent) . '%') : '');
                    } else {
                        $submission_percent = 'N/A';
                    }

                    if ($data->due_date != '0000-00-00 00:00:00') {
                        $activity_due_date = get_tz($data->school_nid, $dateformat, $data->due_date);
                    } else {
                        $activity_due_date = 'Fix';
                    }


                    $rows[] = array(
                        array('data' => $activity_due_date),
                        array('data' => $activity_title),
                        array('data' => get_activity_name($data->node_type)),
                        array('data' => ($data->available_from == '0000-00-00 00:00:00' ? 'Hide' : 'Show')),
                        array('data' => $submission_percent),
                        array('data' => (($data->is_graded == 1) ? (isset($data->published_date) && $data->published_date != '0000-00-00 00:00:00' ? get_tz($data->school_nid, $dateformat, $data->published_date) : '') : 'N/A')),
                    );
                }
            }
        } else {

            $rows[] = array(
                array('data' => ''),
                array('data' => 'No Activities Found'),
                array('data' => ''),
                array('data' => ''),
                array('data' => ''),
            );
        }
    } else if ($user_role == NTLP_ROLEID_STUDENT || isset($user->roles[NTLP_ROLEID_PARENT])) {

        $user_uid = $user->uid;

        if (isset($user->roles[NTLP_ROLEID_PARENT])) {
            $user_uid = $_SESSION['PARENT_STUDENT_UID'];
        }

        $head = array(
            array('data' => t('Due Date'), 'width' => '80px'),
            array('data' => t('Activity'), 'width' => '375px'),
            array('data' => t('Type'), 'width' => '88px'),
            array('data' => t('Submitted'), 'width' => '100px'),
            array('data' => t('Graded'), 'width' => '100px'),
        );

        if ($resultset->num_rows > 0) {
            while ($data = db_fetch_object($resultset)) {

                $activity_class = '';
                $activity_status = '';

                if ($current_nid != $data->nid) {
                    $row_count++;
                    $current_nid = $data->nid;

                    $submission_result = get_user_submission($current_term_tid, $data->nid, $user_uid);
                    $submitted_record = db_fetch_object($submission_result);

                    if ($data->is_published == 1 && $submitted_record->is_graded == 1) {
                        if ($submitted_record->is_complete == 0) {
                        $activity_class = " incomplete";
                        $activity_status = 'Incomplete';
                    } elseif ($submitted_record->is_late == 1) {
                        $activity_class = " late";
                        $activity_status = 'Late';
                    }
                    }

                    if ($data->node_type != 'lo_rubric') {
                        if (isset($submitted_record->date_opened)) {
                            $date_submitted = ($submitted_record->date_submitted == '') ? '' : date($dateformat, strtotime($submitted_record->date_submitted));
                        } else {
                            $date_submitted = '';
                        }
                    } else {
                        $date_submitted = ($submitted_record->date_submitted == '') ? '' : date($dateformat, strtotime($submitted_record->date_submitted));
                    }

                    /* The Submitted column should only be N/A if the Activity is set to “Offline.” */
                    if ($data->submission_type == 'O') {
                        $date_submitted = 'N/A';
                    }

                    if ($data->is_published == 1 && $submitted_record->is_graded == 1) {
                        $date_graded = ($submitted_record->date_graded == '') ? '' : date($dateformat, strtotime($submitted_record->date_graded)); //jr err:date_submitted
                    } else {
                        $date_graded = '';
                    }

                    /* The Graded column should only be N/A if the Activity is a “non-graded” Activity. */
                    if ($data->act_is_graded == '0') {
                        $date_graded = 'N/A';
                    }

                    if (!empty($data->note_to_teacher) && ($date_submitted != 'N/A' && $date_submitted != '')) {
                        $note_to_teacher = $date_submitted . '&nbsp;<a title="' . strip_tags($submitted_record->note_to_teacher) . '" onclick="show_note(this.title)"><img border="0" src="' . $base_path . 'themes/Boldr/Images/common/comments.png"></a>';
                    } else {
                        $note_to_teacher = $date_submitted;
                    }

                    if (!empty($data->teachers_notes) && ($date_graded != 'N/A' && $date_graded != '')) {
                        $teacher_notes = $date_graded . '&nbsp;<a title="' . strip_tags($submitted_record->teachers_notes) . '" onclick="show_note(this.title)"><img border="0" src="' . $base_path . 'themes/Boldr/Images/common/comments.png"></a>';
                    } else {
                        $teacher_notes = $date_graded;
                    }

                    if (isset($activity_status) && $activity_status == 'Incomplete') {
                        $note_to_teacher = $activity_status;
                        $teacher_notes = '';
                    }

                    if (check_activity_work_assesement_link_for_student_nodb($data->nid, $user_uid, $data->node_type, $data->submission_type, $data->allow_late_sub, $data->due_date, $submitted_record->is_final, $date_submitted)) {
                        $work_and_assesment_link = url('ntlp/courses/activity/sub/' . $data->course_nid . '/' . $data->nid);
                    } else {
                        $work_and_assesment_link = url('ntlp/courses/activity/view/' . $data->course_nid . '/' . $data->nid, array('query' => 'tab=work'));
                    }

                    if (isset($user->roles[NTLP_ROLEID_PARENT]) || $user_role == NTLP_ROLEID_SITEGUEST) {
                        $work_and_assesment_link = url('ntlp/courses/activity/view/' . $data->course_nid . '/' . $data->nid, array('query' => 'tab=work'));
                    }

                    $overview_link = url('ntlp/courses/activity/view/' . $data->course_nid . '/' . $data->nid);

                    $activity_title = '<span>' . $data->node_title . '<br/>
                    <a class="default"  href="' . $overview_link . '">Overview</a>
                    &nbsp; |&nbsp;
                    <a  class="default"  href="' . $work_and_assesment_link . '">Work and Assessment</a></span>';

                    if ($data->due_date != '0000-00-00 00:00:00') {
                        $activity_due_date = get_tz($data->school_nid, $dateformat, $data->due_date);
                    } else {
                        $activity_due_date = 'Fix';
                    }
                    
                    $rows[] = array(
                        array('data' => $activity_due_date, 'class' => $activity_class),
                        array('data' => $activity_title, 'class' => $activity_class),
                        array('data' => get_activity_name($data->node_type), 'class' => $activity_class),
                        array('data' => $note_to_teacher, 'class' => $activity_class),
                        array('data' => $teacher_notes, 'class' => $activity_class),
                    );
                }
            }
        } else {

            $rows[] = array(
                array('data' => ''),
                array('data' => 'No Activities Found'),
                array('data' => ''),
                array('data' => ''),
                array('data' => ''),
            );
        }
    }

    $output = theme('table', $head, $rows, $attributes = array('class' => 'ntlp_table', 'width' => '100%', 'cellpadding' => '5'));
    return $output;
}

function check_activity_work_assesement_link_for_student($school_term_tid, $activity_nid, $student_uid, $activity_type, $activity_submission_type, $activity_allow_late_sub, $activity_due_date) {
    global $user;
    $flag = false;


    if (($activity_type == 'lo_journal' || $activity_submission_type == 'D') && $activity_type != 'lo_rubric') {
        $submission_result = get_user_submission($school_term_tid, $activity_nid, $student_uid, true);
        $submitted_record = db_fetch_object($submission_result);

        /* not submitted yet */
        if (empty($submitted_record->date_submitted)) {

            if ($activity_allow_late_sub == 1) {
                $flag = true;
            } else if ($activity_due_date >= date(NTLP_DATE_TZ_SQL, time())) {
                $flag = true;
            }
        }
    }
//    }

    if (isset($user->roles[NTLP_ROLEID_PARENT]) || isset($user->roles[NTLP_ROLEID_SITEGUEST])) {
        $flag = false;
    }
    return $flag;
//     echo ' url ' . $work_and_assesment_link;
}

function check_activity_work_assesement_link_for_student_nodb($activity_nid, $student_uid, $activity_type, $activity_submission_type, $activity_allow_late_sub, $activity_due_date, $is_final, $date_submitted) {
    global $user;
    $flag = false;
    if (($activity_type == 'lo_journal' || $activity_submission_type == 'D') && $activity_type != 'lo_rubric') {
        /* not submitted yet */
        if (empty($date_submitted)) {

            if ($activity_allow_late_sub == 1) {
                $flag = true;
            } else if ($activity_due_date >= date(NTLP_DATE_TZ_SQL, time())) {
                $flag = true;
            }
        }
    }

    if (isset($user->roles[NTLP_ROLEID_PARENT]) || isset($user->roles[NTLP_ROLEID_SITEGUEST])) {
        $flag = false;
    }
    return $flag;
}

function is_submission_allowed_for_student($activityObj, $student_uid, $is_final, $date_opened) {
    global $user;
    $flag = false;
    if (isset($user->roles[NTLP_ROLEID_PARENT]) || isset($user->roles[NTLP_ROLEID_SITEGUEST])) {
        return $flag;
    }

    if (($is_final != 1 || $activityObj->allow_resubmission == 1)
            && (strtotime($activityObj->due_date) >= strtotime('now')
            || $activityObj->allow_late_submission == 1)) {

        $flag = true;
    } else {

        /* not submitted yet */
        if ($is_final == 1 && empty($date_opened)) {

            if ($activityObj->allow_late_submission == 1) {
                $flag = true;
            } else if ($activityObj->due_date >= date(NTLP_DATE_TZ_SQL, time())) {
                $flag = true;
            }
        }
    }
//    }

    return $flag;
}

function get_activities_total_counts($course_nid, $sort_option, $user_role) {
    global $user;

    $current_term_tid = get_current_selected_course_term($course_nid);

    if ($user_role == NTLP_ROLEID_STUDENT || isset($user->roles[NTLP_ROLEID_PARENT])) {

        $query = "/* SLAVE */SELECT COUNT( @DISTINCT ca.nid) FROM dpl_ntlp_course_activity ca
            INNER JOIN dpl_ntlp_activity_user au ON au.activity_nid = ca.nid
            INNER JOIN dpl_node n ON n.nid = ca.nid
            LEFT JOIN {ntlp_gb_grade} g ON ca.nid = g.activity_nid AND au.user_uid = g.user_uid
            @JOIN
            WHERE ca.course_nid = %d AND au.user_uid = %d  @AND
            AND (ca.school_term_tid = %d)
            AND ((ca.availability_mode = 'S' AND ca.available_from <= NOW()
            AND ca.available_to >= NOW()) OR (ca.availability_mode = 'M'
            AND ca.available_from <> '0000-00-00 00:00:00'))";
    } else if ($user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_NETWORKMANAGER
            || $user_role == NTLP_ROLEID_NETWORKSTAFF || $user_role == NTLP_ROLEID_SITEADMIN
            || $user_role == NTLP_ROLEID_SCHOOLLEADER || $user_role == NTLP_ROLEID_SITEGUEST) {

        $query = '/* SLAVE */SELECT COUNT( @DISTINCT ca.nid) FROM dpl_ntlp_course_activity ca
            INNER JOIN dpl_node n ON ca.nid = n.nid
            @JOIN
            WHERE (ca.course_nid = %d) AND (ca.school_term_tid = %d)  @AND ';
    }

    if ($sort_option == 'project') {

        if ($user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_NETWORKMANAGER
                || $user_role == NTLP_ROLEID_NETWORKSTAFF || $user_role == NTLP_ROLEID_SITEADMIN
                || $user_role == NTLP_ROLEID_SCHOOLLEADER || $user_role == NTLP_ROLEID_SITEGUEST) {

            $sub_query = "SELECT DISTINCT c.nid
            FROM {ntlp_course} c
            INNER JOIN dpl_node n ON n.nid = c.nid
            INNER JOIN dpl_ntlp_course_activity ca ON ca.project_nid = c.nid
            WHERE c.course_nid = $course_nid AND c.is_project = 1
            AND ca.school_term_tid = " . $current_term_tid . "
            ORDER BY  n.title ASC, c.from_date DESC ";


            $query = str_replace('@DISTINCT', 'DISTINCT', $query);
            $query = str_replace('@JOIN', '', $query);
            $query = str_replace('@AND', ' AND ca.project_nid IN( ' . $sub_query . ' )', $query);

            $activity_result = db_query($query, $course_nid, $current_term_tid);

            return db_result($activity_result);
//            
        } else if ($user_role == NTLP_ROLEID_STUDENT || isset($user->roles[NTLP_ROLEID_PARENT])) {
            $user_uid = $user->uid;

            if (isset($user->roles[NTLP_ROLEID_PARENT])) {
                $user_uid = $_SESSION['PARENT_STUDENT_UID'];
            }

            $sub_query = "SELECT DISTINCT c.nid FROM dpl_ntlp_course c
            INNER JOIN dpl_node n ON n.nid = c.nid
            LEFT JOIN dpl_ntlp_course c2 ON n.nid = c2.nid
            INNER JOIN dpl_ntlp_course_activity ca ON ca.project_nid = c.nid
            WHERE (c.is_project <> 0) AND (c.course_nid = $course_nid) AND (c.availability = 'S')
            AND ca.school_term_tid = " . $current_term_tid;

            $query = str_replace('@DISTINCT', 'DISTINCT', $query);
            $query = str_replace('@JOIN', '', $query);
            $query = str_replace('@AND', ' AND ca.project_nid IN( ' . $sub_query . ' )', $query);

            $activity_result = db_query($query, $course_nid, $user_uid, $current_term_tid);

            return db_result($activity_result);
        }
    } else if ($sort_option == 'general') {

        $query = str_replace('@DISTINCT', 'DISTINCT', $query);
        $query = str_replace('@JOIN', '', $query);
        $query = str_replace('@AND', ' AND (ca.project_nid = 0)', $query);


        if ($user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_NETWORKMANAGER
                || $user_role == NTLP_ROLEID_NETWORKSTAFF || $user_role == NTLP_ROLEID_SITEADMIN
                || $user_role == NTLP_ROLEID_SCHOOLLEADER || $user_role == NTLP_ROLEID_SITEGUEST) {

            $activity_result = db_query($query, $course_nid, $current_term_tid);
        } else if ($user_role == NTLP_ROLEID_STUDENT || isset($user->roles[NTLP_ROLEID_PARENT])) {
            $user_uid = $user->uid;

            if (isset($user->roles[NTLP_ROLEID_PARENT])) {
                $user_uid = $_SESSION['PARENT_STUDENT_UID'];
            }

            $activity_result = db_query($query, $course_nid, $user_uid, $current_term_tid);
        }

        return db_result($activity_result);
    } else if ($sort_option == 'lo') {

        $sub_query = "SELECT td.tid
            FROM dpl_ntlp_gb_course_outcome co
            INNER JOIN dpl_term_data td ON td.tid = co.tid
            WHERE co.course_nid = $course_nid";

        if ($user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_NETWORKMANAGER
                || $user_role == NTLP_ROLEID_NETWORKSTAFF || $user_role == NTLP_ROLEID_SITEADMIN
                || $user_role == NTLP_ROLEID_SCHOOLLEADER || $user_role == NTLP_ROLEID_SITEGUEST) {

            $query = str_replace('@DISTINCT', 'DISTINCT', $query);
            $query = str_replace('@JOIN', ' INNER JOIN dpl_ntlp_gb_activity_outcome ao ON ao.activity_nid = n.nid ', $query);
            $query = str_replace('@AND', ' AND ( ao.tid IN (' . $sub_query . ')) ', $query);

            $activity_result = db_query($query, $course_nid, $current_term_tid);
            return db_result($activity_result);
        } else if ($user_role == NTLP_ROLEID_STUDENT || isset($user->roles[NTLP_ROLEID_PARENT])) {
            $user_uid = $user->uid;

            if (isset($user->roles[NTLP_ROLEID_PARENT])) {
                $user_uid = $_SESSION['PARENT_STUDENT_UID'];
            }

            $query = str_replace('@DISTINCT', 'DISTINCT ', $query);
            $query = str_replace('@JOIN', ' INNER JOIN dpl_ntlp_gb_activity_outcome ao ON ao.activity_nid = ca.nid ', $query);
            $query = str_replace('@AND', ' AND ( ao.tid IN (' . $sub_query . ')) AND (g.date_graded IS NOT NULL)', $query);

            $activity_result = db_query($query, $course_nid, $user_uid, $current_term_tid);

            return db_result($activity_result);
        }
    } else if ($sort_option == 'dd') {

        $due_date_arr = array('up' => 'Upcoming', 'pd' => 'Past Due');

        foreach ($due_date_arr as $key => $value) {
            $temp_query = $query;

            if ($key == 'pd') {
                $due_date_condition = 'AND (ca.due_date <= NOW())';
            } else if ($key == 'up') {
                $due_date_condition = 'AND (ca.due_date >= NOW())';
            }

            if ($user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_NETWORKMANAGER
                    || $user_role == NTLP_ROLEID_NETWORKSTAFF || $user_role == NTLP_ROLEID_SITEADMIN
                    || $user_role == NTLP_ROLEID_SCHOOLLEADER || $user_role == NTLP_ROLEID_SITEGUEST) {

                $temp_query = str_replace('@DISTINCT', 'DISTINCT', $temp_query);
                $temp_query = str_replace('@JOIN', '', $temp_query);
                $temp_query = str_replace("@AND", $due_date_condition, $temp_query);

                $activity_result = db_query($temp_query, $course_nid, $current_term_tid);
                $totalActivity += db_result($activity_result);
            } else if ($user_role == NTLP_ROLEID_STUDENT || isset($user->roles[NTLP_ROLEID_PARENT])) {
                $user_uid = $user->uid;

                if (isset($user->roles[NTLP_ROLEID_PARENT])) {
                    $user_uid = $_SESSION['PARENT_STUDENT_UID'];
                }

                $temp_query = str_replace('@DISTINCT', 'DISTINCT', $temp_query);
                $temp_query = str_replace('@JOIN', '', $temp_query);
                $temp_query = str_replace("@AND", $due_date_condition, $temp_query);

                $activity_result = db_query($temp_query, $course_nid, $user_uid, $current_term_tid);
                $totalActivity += db_result($activity_result);
            }
        }
        return $totalActivity;
    } else if ($sort_option == 'type') {

        $sub_query = "SELECT DISTINCT n.type AS node_type
            FROM dpl_ntlp_course_activity ca
            INNER JOIN dpl_node n ON ca.nid = n.nid
            WHERE ca.course_nid = $course_nid ORDER BY n.type ASC ";

        $query = str_replace('@DISTINCT', 'DISTINCT', $query);
        $query = str_replace('@JOIN', '', $query);
        $query = str_replace("@AND", " AND (n.type IN (" . $sub_query . ")) ", $query);

        if ($user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_NETWORKMANAGER
                || $user_role == NTLP_ROLEID_NETWORKSTAFF || $user_role == NTLP_ROLEID_SITEADMIN
                || $user_role == NTLP_ROLEID_SCHOOLLEADER || $user_role == NTLP_ROLEID_SITEGUEST) {

            $activity_result = db_query($query, $course_nid, $current_term_tid);
            return db_result($activity_result);
        } else if ($user_role == NTLP_ROLEID_STUDENT || isset($user->roles[NTLP_ROLEID_PARENT])) {

            $user_uid = $user->uid;

            if (isset($user->roles[NTLP_ROLEID_PARENT])) {
                $user_uid = $_SESSION['PARENT_STUDENT_UID'];
            }
            $activity_result = db_query($query, $course_nid, $user_uid, $current_term_tid);
            return db_result($activity_result);
        }
    }
}






