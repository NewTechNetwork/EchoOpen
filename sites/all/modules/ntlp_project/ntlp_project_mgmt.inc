<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_courses_activity', 'data_access');

function ntlp_courses_project_frm($course_nid) {
    return drupal_get_form('ntlp_courses_project_form', $course_nid);
}

function ntlp_courses_project_form($form_state, $course_nid) {

    global $user;

    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/ntlp_project.js');
    drupal_add_css(drupal_get_path('module', 'jquery_ui') . '/jquery.ui/themes/base/ui.all.css');
    jquery_ui_add('ui.accordion');


    $user_role = check_user_permission($course_nid);

    watchdog('user_role', 'user role in course ' . $user_role);
    $sort_option = $_GET['viewby'];

    if (isset($sort_option)) {
        $_SESSION['ntlp_course_project_:_projects_by'] = $sort_option;
    } else {
        $sorted_by = $_SESSION['ntlp_course_project_:_projects_by'];
        drupal_goto('ntlp/courses/project/' . $course_nid, array('viewby' => $sorted_by));
    }

    if (empty($sort_option)) {
        $sort_option = 'date';
    }

    set_item_url_handler('Project Main');

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


    if ($user_role != NTLP_ROLEID_STUDENT && !isset($user->roles[NTLP_ROLEID_PARENT]) && $user_role != NTLP_ROLEID_SITEGUEST) {
//        $create_button = '<a onmousedown=this.className="Activities_btnClicked" onmouseup=this.className="Activities_btn"   class="Activities_btn"  href="' . url('ntlp/courses/activity/new/' . $course_nid) . '">Create New Project</a>';
        $create_button = '+ ' . l("Create New Project", 'ntlp/courses/project/setting/' . $course_nid . '/0', array('attributes' => array('class' => 'green_link', 'style' => 'font-weight:bold;')));
    } else {
        $create_button = '';
    }

    $option = array("date" => "Date"
        , "title" => "Title"
        , "visibility" => "Visibility"
        , "stats" => "Stats"
    );

    $project_shown_text = "";
    $project_shown_float = "left";

    if ($user_role == NTLP_ROLEID_STUDENT || isset($user->roles[NTLP_ROLEID_PARENT])) {
        unset($option['visibility']);
        unset($option['stats']);

        $project_shown_text = " shown";
        $project_shown_float = "right";
    }


    $projects_data = ntlp_courses_projects_render($course_nid, $sort_option, $user_role);
    $projects_total = get_projects_total_counts($course_nid, $viewby_option, $user_role);

    $form['main']['home_page_header'] = array(
        '#type' => 'select',
        '#options' => $option,
        '#default_value' => $sort_option,
        '#attributes' => array('class' => 'view_project_select',
            'onchange' => 'window.location = \'?q=ntlp/courses/project/' . $course_nid . '&viewby=\'+this.value;'),
        '#prefix' => '<tr><td colspan="2"><div class="teacherProfile Activity_Detail-Grading" style="margin:0px;">
                            <div class="yellowBacktopleft">
                              <div class="yellowBacktopright">
                                <div class="yellowBackbottomleft">
                                  <div class="yellowBackbottomright" style="padding:0px;">
                                  <div style="padding:10px">
                                   <table cellspacing="0" cellpadding="0" border="0" width="100%"><tr valign="top" ><td valign="middle" style="width: 110px;"> View Projects By:</td><td>',
        '#suffix' => '</td><td align="right" style="color:#666" id ="" valign="middle">
                                        <div style=" width: 137px; float: ' . $project_shown_float . '; margin-left: 210px;">Total Projects' . $project_shown_text . ': <b  style="color:#333">' . count($projects_total) . '</b></div>',
    );

    $form['main']['create_button'] = array(
        '#type' => 'item',
        '#value' => $create_button,
        '#prefix' => '<div style="float: right;">',
        '#suffix' => '</div></td></tr></table>

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
    $EXPAND_STATE = $EXPAND_STATE . '<div class="GradesTabCenter" style=" ' . (($sort_option == 'general' || $projects_total <= 0) ? 'display:none;' : '') . ' float: left; white-space: nowrap;">';
    $EXPAND_STATE = $EXPAND_STATE . '<ul style="margin-top:20px;">';
    $EXPAND_STATE = $EXPAND_STATE . '<li><a id="expand_all" style="font-size: 11px;" class="GradesTabActive" onclick="expand_all_panels(1); ">Expand All</a></li>';
    $EXPAND_STATE = $EXPAND_STATE . '<li>|</li>';
    $EXPAND_STATE = $EXPAND_STATE . '<li><a id="collapse_all" style="font-size: 11px;" onclick="expand_all_panels(0); ">Collapse All</a></li>';
    $EXPAND_STATE = $EXPAND_STATE . '</ul> </div>';

    if ($sort_option != 'stats' && $sort_option != 'title') {
        $form['main']['heading'] = array(
            '#type' => 'item',
            '#value' => '',
            '#prefix' => $EXPAND_STATE,
            '#suffix' => '</td></tr>',
        );
    }
    $form['main']['activity_data'] = array(
        '#type' => 'item',
        '#value' => $projects_data,
        '#prefix' => '<tr><td colspan="2" id="activity_data">',
        '#suffix' => '</td></tr>',
    );

    return $form;
}

function ntlp_courses_project_stats($course_nid) {

    print ntlp_courses_projects_render($course_nid, 'stats', check_user_permission($course_nid));
}

function ntlp_courses_projects_render($course_nid, $sort_option, $user_role) {
    global $user, $base_path;

    include('project_main.template.inc');

    $dateformat = "m/d/Y";

    $html = '<div id="notaccordion">';

    $query = "SELECT DISTINCT(c.nid), n.title, n.nid, c1.availability, c1.from_date, c1.to_date,
            c1.picture_fid, c1.academic_standards, nr.body, c1.driving_question
        FROM dpl_ntlp_course c
        INNER JOIN dpl_node n ON c.nid = n.nid
        INNER JOIN dpl_node_revisions nr ON nr.nid = c.nid
        LEFT JOIN dpl_ntlp_course c1 ON n.nid = c1.nid
        WHERE (c.is_project <> 0) AND (c.course_nid = %d) @AVAILABILITY @AND
        GROUP BY c.nid @ORDER_BY";

    if ($user_role == NTLP_ROLEID_STUDENT || isset($user->roles[NTLP_ROLEID_PARENT])) {

        $query = str_replace('@AVAILABILITY', " AND (c.availability = 's') ", $query);
    } else {
        $query = str_replace('@AVAILABILITY', "", $query);
    }

    if ($sort_option == 'date') {

        $date_option_arr = array('current' => 'Current', 'past' => 'Past');

        foreach ($date_option_arr as $key => $value) {
            $temp_query = $query;
            $due_date_condition = '';

            if ($key == 'current') {
                $due_date_condition = ' AND (c1.to_date >= CURDATE())';
            } else if ($key == 'past') {
                $due_date_condition = ' AND (c1.to_date< CURDATE()) ';
            }

            $temp_query = str_replace("@AND", $due_date_condition, $temp_query);
            $temp_query = str_replace("@ORDER_BY", ' ORDER BY c1.from_date DESC, n.title ASC ', $temp_query);

            $project_result = db_query($temp_query, $course_nid);

            $html .= '<div class="ActivitiesPanel notaccordion">

              <div style="padding-top:20px">
                <div class="tbl_heading"><div style="float:left">
                    <div style="float:left">
                        <span class="ui-icon ui-icon-triangle-1-s"></span>
                    </div>
                    <a style="color:#000 !important; text-decoration:none !important;" class="activity_accordion">' . $value . '</a></div></div>
               </div>
              </div>';

            $html .= '<div style="clear:both"><table cellspacing="0" class="StateTable" cellpadding="0" border="0" width="100%" >';
            $html .= '<tr id = "row_' . $key . '"><td colspan="2" >' . $activity_rows . '</td></tr></table><div style="font-size: 10px;">&nbsp;</div>';
            if ($key == 'current')
                $html .= '<table width="100%"><tr>';
            else
                $html .= '<table style="margin-left:10px;" width="100%"><tr>';
            $count = 0;
            while ($data = db_fetch_object($project_result)) {
                $project_date = '';
                if ($key == 'current') {
                    $html .= '<td>';

                    if ($user_role != NTLP_ROLEID_STUDENT && $user_role != NTLP_ROLEID_SITEGUEST
                            && !isset($user->roles[NTLP_ROLEID_PARENT])) {
                        $visibility = '<div id="visibility_div" style="width: 120px; float: right;">
                                <span style="color: rgb(153, 153, 153); "><strong> Visibility: </strong> ' . ($data->availability == 'H' ? 'Hide' : 'Show') . ' </span> &nbsp;
                            <span>
                        <a href="' . url('ntlp/courses/project/setting/' . $course_nid . '/' . $data->nid) . '" >
                            <img src="' . $base_path . 'themes/Boldr/Images/common/edit.png" alt="editimage"/>
                        </a></span></div>';
                    } else {
                        $visibility = '';
                    }

                    $project_goto_link = url('ntlp/courses/projects/' . $course_nid . '/' . $data->nid);

                    $project_date = get_tz_course($course_nid, NTLP_DATE_TZ_FJY, $data->from_date) . ' - ' . get_tz_course($course_nid, NTLP_DATE_TZ_FJY, $data->to_date);
                    $current_proj_html = $HTML_CURRENT_PROJECT_DISPLAY;
                    $current_proj_html = str_replace('@VISIBILITY_HTML', $visibility, $current_proj_html);
                    $current_proj_html = str_replace('@PROJECT_NAME', l($data->title, 'ntlp/courses/projects/' . $course_nid . '/' . $data->nid, array('attributes' => array('class' => 'pro_name_green'))), $current_proj_html);
                    $current_proj_html = str_replace('@PROJECT_IMAGE_PATH', get_ntlp_project_image_path($data->picture_fid), $current_proj_html);
                    $current_proj_html = str_replace('@PROJECT_GOTO_LINK', $project_goto_link, $current_proj_html);
                    $current_proj_html = str_replace('@PROJECT_FROM_AND_TO_DATE', $project_date, $current_proj_html);
                    $current_proj_html = str_replace('@PROJECT_DESCRIPTION', $data->driving_question, $current_proj_html);

                    $html .= $current_proj_html;
                    $html .= '</td></tr>';
                } else {

                    if ($count % 3 == 0)
                        $html .= '</tr> <tr><td colspan="5">&nbsp;</td></tr> <tr>';

                    $html .= '<td width="25%" style="margin-left:10px; vertical-align: top;">';

                    if ($user_role != NTLP_ROLEID_STUDENT && $user_role != NTLP_ROLEID_SITEGUEST
                            && !isset($user->roles[NTLP_ROLEID_PARENT])) {
                        $edit_link = '&nbsp;<a  href="' . url('ntlp/courses/project/setting/' . $course_nid . '/' . $data->nid) . '" >
                            <img src="' . $base_path . 'themes/Boldr/Images/common/edit.png" alt="editimage"/>
                        </a>';

                        $visibility_status = '<strong> Visibility: </strong>' . ($data->availability == 'H' ? 'Hide' : 'Show');
                    } else {
                        $edit_link = '';
                        $visibility_status = '';
                    }



                    if (strlen($data->title) > 25) {
                        $pro_name = substr($data->title, 0, 25) . '...';
                    } else {
                        $pro_name = $data->title;
                    }

                    $project_goto_link = url('ntlp/courses/projects/' . $course_nid . '/' . $data->nid);
                    $project_date = date(NTLP_DATE_TZ_SHORT, strtotime($data->from_date)) . ' - ' . date(NTLP_DATE_TZ_SHORT, strtotime($data->to_date));

                    $past_proj_html = $HTML_PAST_PROJECT_DISPLAY;
                    $past_proj_html = str_replace('@PROJECT_NAME', l($pro_name, 'ntlp/courses/projects/' . $course_nid . '/' . $data->nid), $past_proj_html);

                    $past_proj_html = str_replace('@PROJECT_IMAGE_PATH', get_ntlp_project_image_path($data->picture_fid), $past_proj_html);
                    $past_proj_html = str_replace('@PROJECT_GOTO_LINK', $project_goto_link, $past_proj_html);
                    $past_proj_html = str_replace('@PROJECT_FROM_AND_TO_DATE', $project_date, $past_proj_html);
                    $past_proj_html = str_replace('@PROJECT_DESCRIPTION', $data->driving_question, $past_proj_html);
                    $past_proj_html = str_replace('@VISIBILITY', $visibility_status, $past_proj_html);
                    $past_proj_html = str_replace('@EDIT_LINK', $edit_link, $past_proj_html);

                    $html .= $past_proj_html;
                    $html .= '</td>';

                    if ($count % 3 == 0 || $count % 3 == 1) {
                        $html .= '<td width="12.5%">&nbsp;</td>';
                    }

                    $count++;
                }
            }
            $html .= '</table>';
            $html .= '</div>';
        }
    } else if ($sort_option == 'title') {

        $query = str_replace("@AND", $due_date_condition, $query);
        $query = str_replace("@ORDER_BY", ' ORDER BY n.title', $query);

        $project_result = db_query($query, $course_nid);

        $html .= '<div style="font-size: 10px;">&nbsp;</div>';

        while ($data = db_fetch_object($project_result)) {

            if ($user_role != NTLP_ROLEID_STUDENT && $user_role != NTLP_ROLEID_SITEGUEST
                    && !isset($user->roles[NTLP_ROLEID_PARENT])) {

                $visibility = '<div id="visibility_div" style="width: 120px; float: right;">
                                <span style="color: rgb(153, 153, 153); "><strong> Visibility: </strong> ' . ($data->availability == 'H' ? 'Hide' : 'Show') . ' </span> &nbsp;
                            <span>
                        <a href="' . url('ntlp/courses/project/setting/' . $course_nid . '/' . $data->nid) . '" >
                            <img src="' . $base_path . 'themes/Boldr/Images/common/edit.png" alt="editimage"/>
                        </a></span></div>';
            } else {
                $visibility = '';
            }

            $project_goto_link = url('ntlp/courses/projects/' . $course_nid . '/' . $data->nid);

            $project_date = date(NTLP_DATE_TZ_FJY, strtotime($data->from_date)) . ' - ' . date(NTLP_DATE_TZ_FJY, strtotime($data->to_date));
            $current_proj_html = $HTML_CURRENT_PROJECT_DISPLAY;
            $current_proj_html = str_replace('@VISIBILITY_HTML', $visibility, $current_proj_html);
            $current_proj_html = str_replace('@PROJECT_NAME', l($data->title, 'ntlp/courses/projects/' . $course_nid . '/' . $data->nid, array('attributes' => array('class' => 'pro_name_green'))), $current_proj_html);
            $current_proj_html = str_replace('@PROJECT_GOTO_LINK', $project_goto_link, $current_proj_html);
            $current_proj_html = str_replace('@PROJECT_IMAGE_PATH', get_ntlp_project_image_path($data->picture_fid), $current_proj_html);
            $current_proj_html = str_replace('@PROJECT_FROM_AND_TO_DATE', $project_date, $current_proj_html);
            $current_proj_html = str_replace('@PROJECT_DESCRIPTION', $data->driving_question, $current_proj_html);

            $html .= $current_proj_html;
        }
    } else if ($sort_option == 'visibility') {

        if ($user_role == NTLP_ROLEID_STUDENT || isset($user->roles[NTLP_ROLEID_PARENT])) {
            drupal_goto('ntlp/courses/project/' . $course_nid, array('viewby' => 'date'));
        }
        $visibility_arr = array('show' => 'Show', 'hide' => 'Hide');

        foreach ($visibility_arr as $key => $value) {
            $temp_query = $query;
            if ($key == 'show') {
                $visible_condition = " AND (c.availability = 'S')";
            } else {
                $visible_condition = " AND (c.availability = 'H')";
            }

            $temp_query = str_replace("@AND", $visible_condition, $temp_query);
            $temp_query = str_replace("@ORDER_BY", ' ORDER BY c1.from_date DESC', $temp_query);

            $html .= '<div class="ActivitiesPanel notaccordion">

              <div style="padding-top:20px">
                <div class="tbl_heading"><div style="float:left">
                    <div style="float:left">
                        <span class="ui-icon ui-icon-triangle-1-s"></span>
                    </div>
                    <a style="color:#000 !important; text-decoration:none !important;" class="activity_accordion">' . $value . '</a></div></div>
               </div>
              </div>';

            $html .= '<div style="clear:both"><table cellspacing="0" class="StateTable" cellpadding="0" border="0" width="100%" >';
            $html .= '<tr id = "row_' . $key . '"><td colspan="2" >' . $activity_rows . '</td></tr></table><div style="font-size: 10px;">&nbsp;</div>';

            $project_result = db_query($temp_query, $course_nid);

            while ($data = db_fetch_object($project_result)) {

                if ($user_role != NTLP_ROLEID_STUDENT && $user_role != NTLP_ROLEID_SITEGUEST
                        && !isset($user->roles[NTLP_ROLEID_PARENT])) {
                    $visibility = '<div id="visibility_div" style="width: 120px; float: right;">
                                <span style="color: rgb(153, 153, 153); "><strong> Visibility: </strong> ' . ($data->availability == 'H' ? 'Hide' : 'Show') . ' </span> &nbsp;
                            <span>
                        <a href="' . url('ntlp/courses/project/setting/' . $course_nid . '/' . $data->nid) . '" >
                            <img src="' . $base_path . 'themes/Boldr/Images/common/edit.png" alt="editimage"/>
                        </a></span></div>';
                } else {
                    $visibility = '';
                }

                $project_goto_link = url('ntlp/courses/projects/' . $course_nid . '/' . $data->nid);

                $project_date = '';
                $project_date = date(NTLP_DATE_TZ_FJY, strtotime($data->from_date)) . ' - ' . date(NTLP_DATE_TZ_FJY, strtotime($data->to_date));
                $current_proj_html = $HTML_CURRENT_PROJECT_DISPLAY;
                $current_proj_html = str_replace('@VISIBILITY_HTML', $visibility, $current_proj_html);
                $current_proj_html = str_replace('@PROJECT_NAME', l($data->title, 'ntlp/courses/projects/' . $course_nid . '/' . $data->nid, array('attributes' => array('class' => 'pro_name_green'))), $current_proj_html);
                $current_proj_html = str_replace('@PROJECT_IMAGE_PATH', get_ntlp_project_image_path($data->picture_fid), $current_proj_html);
                $current_proj_html = str_replace('@PROJECT_GOTO_LINK', $project_goto_link, $current_proj_html);
                $current_proj_html = str_replace('@PROJECT_FROM_AND_TO_DATE', $project_date, $current_proj_html);
                $current_proj_html = str_replace('@PROJECT_DESCRIPTION', $data->driving_question, $current_proj_html);

                $html .= $current_proj_html;
            }
            $html .= '</div>';
        }
    } else if ($sort_option == 'stats') {

        if ($user_role == NTLP_ROLEID_STUDENT || isset($user->roles[NTLP_ROLEID_PARENT])) {
            drupal_goto('ntlp/courses/project/' . $course_nid, array('viewby' => 'date'));
        }

        $html .= '<div style="clear:both"><table cellspacing="0" class="StateTable" cellpadding="0" border="0" width="100%" >';
        $html .= '<tr id = "row_' . $key . '"><td colspan="2" >' . $activity_rows . '</td></tr></table><div style="font-size: 10px;">&nbsp;</div>';

        $sort_order = strtolower(isset($_GET['order']) ? $_GET['order'] : 'c1.from_date DESC, n.title ');
        $sort_dir = isset($_GET['sort']) ? $_GET['sort'] : 'asc';
        $sort_dir = $sort_dir == 'none' ? 'asc' : $sort_dir;
        $sort_field = $sort_order;

        if ($sort_order == "dates")
            $sort_field = 'c1.from_date';
        if ($sort_order == "project")
            $sort_field = 'n.title';
        if ($sort_order == "duration days")
            $sort_field = 'date_diff';
        if ($sort_order == "resources")
            $sort_field = 'resource_count';
        if ($sort_order == "activities")
            $sort_field = 'activity_count';
        if ($sort_order == "groups")
            $sort_field = 'project_group_count';
        if ($sort_order == "need to knows")
            $sort_field = 'ntk_count';

        $project_result = get_stats_view_results($course_nid, $sort_field . ' ' . $sort_dir);

        $head = array(
            array('data' => t('Dates'), 'field' => 'c1.from_date', 'style' => 'vertical-align:middle !important;', 'sort' => 'desc', 'width' => '16%'),
            array('data' => t('Project'), 'field' => 'n.title', 'style' => 'vertical-align:middle !important;', 'width' => '32%'),
            array('data' => t('Duration Days'), 'width' => '', 'field' => 'date_diff', 'style' => 'vertical-align:middle !important; text-align:center'),
            array('data' => t('Resources'), 'width' => '', 'field' => 'resource_count', 'style' => 'vertical-align:middle !important; text-align:center'),
            array('data' => t('Activities'), 'width' => '', 'field' => 'activity_count', 'style' => 'vertical-align:middle !important; text-align:center'),
            array('data' => t('Groups'), 'width' => '', 'field' => 'project_group_count', 'style' => 'vertical-align:middle !important; text-align:center'),
            array('data' => t('Need to Knows'), 'width' => '', 'field' => 'ntk_count', 'style' => 'vertical-align:middle !important; text-align:center'),
        );

        while ($data = db_fetch_object($project_result)) {

            if ($user_role == NTLP_ROLEID_TEACHER || isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])
                    || $user_role == NTLP_ROLEID_NETWORKSTAFF) {
                $edit_link = '<span style="float:right;">
                        <a href="' . url('ntlp/courses/project/setting/' . $course_nid . '/' . $data->nid) . '" >
                            <img style="margin:0px;" src="' . $base_path . 'themes/Boldr/Images/common/edit.png" alt="editimage"/>
                        </a></span>';
            } else {
                $edit_link = '';
            }
            $activity_count = $data->activity_count;
            $resource_count = $data->resource_count;
            $project_dur_days = count_days($data->from_date, $data->to_date);
            $project_groups_counts = $data->project_group_count;
            $project_ntk_counts = $data->ntk_count;

            $temp_activity_counts += $activity_count;
            $temp_resource_counts += $resource_count;
            $temp_dur_days_counts += $project_dur_days;
            $temp_groups_counts += $project_groups_counts;
            $temp_ntk_counts += $project_ntk_counts;

            if (strlen($data->title) > 32) {
                $truc_text = '...';
            } else {
                $truc_text = '';
            }
            $title = substr($data->title, 0, 32) . $truc_text;
            $rows[] = array(
                array('data' => get_tz_course($data->course_nid, NTLP_DATE_TZ_MIN, $data->from_date) . ' - ' . get_tz_course($data->course_nid, NTLP_DATE_TZ_MIN, $data->to_date)),
                array('data' => '<div style="float:left; width:85%;">' . l($title, 'ntlp/courses/projects/' . $course_nid . '/' . $data->nid) . '</div>' . $edit_link),
                array('data' => $project_dur_days, 'align' => 'center'),
                array('data' => $resource_count, 'align' => 'center'),
                array('data' => $activity_count, 'align' => 'center'),
                array('data' => $project_groups_counts, 'align' => 'center'),
                array('data' => $project_ntk_counts, 'align' => 'center'),
            );
        }
        $rows[] = array(
            array('data' => 'Averages', 'colspan' => '2', 'style' => 'background:#CCCCCC !important; text-align:right;'),
            array('data' => (($project_result->num_rows > 0) ? round($temp_dur_days_counts / $project_result->num_rows) : 0), 'align' => 'center', 'style' => 'background:#CCCCCC !important;'),
            array('data' => (($project_result->num_rows > 0) ? round($temp_resource_counts / $project_result->num_rows) : 0 ), 'align' => 'center', 'style' => 'background:#CCCCCC !important;'),
            array('data' => (($project_result->num_rows > 0) ? round($temp_activity_counts / $project_result->num_rows) : 0 ), 'align' => 'center', 'style' => 'background:#CCCCCC !important;'),
            array('data' => (($project_result->num_rows > 0) ? round($temp_groups_counts / $project_result->num_rows) : 0 ), 'align' => 'center', 'style' => 'background:#CCCCCC !important;'),
            array('data' => (($project_result->num_rows > 0) ? round($temp_ntk_counts / $project_result->num_rows) : 0 ), 'align' => 'center', 'style' => 'background:#CCCCCC !important;'),
        );
    }
    $output = theme('table', $head, $rows, $attributes = array('style' => 'border-left: 1px solid #D6E9F5 !important; border-right: 1px solid #D6E9F5 !important;', 'class' => 'ntlp_table', 'width' => '100%', 'cellpadding' => '5'));
    $output = '<div id="project_stats_table">' . $output . '</div>';
    $html .= $output;
    $html .= '</div>';

    return $html;
}

function get_stats_view_results($course_nid, $order_by) {

    $query = "SELECT DISTINCT(c.nid), n.title, n.nid, c1.from_date, c1.to_date,
                (SELECT COUNT(*) FROM {ntlp_course_activity} WHERE project_nid=c.nid
                AND project_cat IS NOT NULL
                AND project_cat <> 0) AS activity_count,

                (SELECT COUNT(*) AS count_res FROM dpl_ntlp_project_resource pr
                INNER JOIN dpl_ntlp_course_resource cr ON cr.nid = pr.nid
                WHERE pr.project_nid = c.nid
                AND  cr.deleted_on IS NULL) AS resource_count,

                (c1.to_date - c1.from_date) AS date_diff,

                (SELECT COUNT(*) as new_count
                            FROM {ntlp_group} ng
                            LEFT JOIN {ntlp_course_resource} ncr ON ncr.course_nid =  ng.nid
                            INNER JOIN {node} n ON n.nid = ng.nid
                            WHERE ng.project_nid = c.nid) AS project_group_count,

                (SELECT COUNT(*) AS count_ntk FROM {og_ancestry} og_an
                        INNER JOIN {node} n ON n.nid = og_an.nid
                        WHERE og_an.group_nid = c.nid) AS ntk_count

        FROM dpl_ntlp_course c
        INNER JOIN dpl_node n ON c.nid = n.nid
        LEFT JOIN dpl_ntlp_course c1 ON n.nid = c1.nid
        WHERE (c.is_project <> 0) AND (c.course_nid = $course_nid) 
        GROUP BY c.nid ORDER BY $order_by";

    return db_query($query);
}

function get_ntlp_project_image_path($picture_fid) {
    global $base_path;

    if ($picture_fid != null && $picture_fid != 0) {

        $file_obj = get_file_detail($picture_fid);
        $file_path = $file_obj->filepath;

        if (file_exists($_SERVER['DOCUMENT_ROOT'] . $base_path . $file_path)) {
            $image_path = $base_path . $file_path;
        } else {

            $image_path = $base_path . 'themes/Boldr/Images/ProjectsImages/default_project_icon.png';
        }
    } else {
        $image_path = $base_path . 'themes/Boldr/Images/ProjectsImages/default_project_icon.png';
    }

    return $image_path;
}

function count_days($date1, $date2) {

    $diff = strtotime($date2) - strtotime($date1);
    $days = floor($diff / (60 * 60 * 24));
    return $days + 1;
}

function _ntlp_project_setting_frm($course_nid, $project_nid) {
    return drupal_get_form('ntlp_project_setting_form', $course_nid, $project_nid);
}

function ntlp_project_setting_form($form_state= null, $course_nid, $project_nid = 0) {
    global $user;
    include('project_main.template.inc');


    $user_role = check_user_permission($course_nid);


    if (!isset($form_state['values'])) {
        set_item_url_handler('Project Setting');
    }

    if ($user_role == NTLP_ROLEID_STUDENT || isset($user->roles[NTLP_ROLEID_PARENT])
            || isset($user->roles[NTLP_ROLEID_SITEGUEST]) || $user_role == NTLP_ROLEID_SITEGUEST) {
        drupal_goto('ntlp/goback');
    }
    jquery_ui_add(array('ui.dialog', 'ui.draggable', 'ui.datepicker'));

################### Date Picker Files ########################################################################################
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/date_picker.js');
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/jquery.ui.datepicker.css', 'module', 'all', FALSE);
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/demos.css', 'module', 'all', FALSE);
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/jquery.ui.all.css', 'module', 'all', FALSE);
# there is a white line coming on the lower left side of the calender due to background:
#ffffff defined on line 14 of "datepicker/jquery.ui.theme.css" file
################### END Of Date Picker Files #################################################################################


    if ($project_nid != 0) {
        $project_obj = load_project_record($project_nid);
    }

    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/ntlp_project.js');
    drupal_add_js('function textLimit(field, maxlen) {
                    if (field.value.length > maxlen){
                        //alert(field.value.length );
                        field.value = field.value.substring(0, maxlen);

                    }
                }', inline);

    drupal_add_js('function imposeMaxLength(Object, MaxLen)
{//alert(Object.value.length );
  return (Object.value.length <= MaxLen);
}', inline);



    $form['project_setting'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '',
        '#suffix' => '',
    );

    $form['project_setting']['course_nid'] = array(
        '#type' => 'hidden',
        '#value' => $course_nid,
    );

    $form['project_setting']['project_nid'] = array(
        '#type' => 'hidden',
        '#value' => $project_nid,
    );


    $form['project_setting']['header'] = array(
        '#type' => 'item',
        '#prefix' => ' <div style=" margin-bottom:15px; width:100%" class="clearfix">',
        '#suffix' => '',
    );

    if (isset($project_obj))
        $last_saved = ' <span class="published_grade_box" style="padding: 4px !important;float:right !important;margin-right: 25px;"> Last Saved ' . date('m/d/Y @ g:i a', $project_obj->changed) . '&nbsp;&nbsp; </span>';
    else
        $last_saved = '';

    $form['project_setting']['header']['submitBtn'] = array(
        '#type' => 'submit',
        '#value' => 'Save',
        '#prefix' => '<div style="float:right; height: 30px; width:125px;"><span style="float:left; clear:left;">',
        '#suffix' => ' </span>',
        '#attributes' => array('class' => 'SaveBtn'),
    );

    $form['project_setting']['header']['cancelBtn'] = array(
        '#type' => 'submit',
        '#value' => 'Cancel',
        '#prefix' => ' <span style="float: right;">',
        '#suffix' => ' </span> </div>' . $last_saved,
        '#attributes' => array('class' => 'CancelBtn'),
    );

    $form['project_setting']['header']['title'] = array(
        '#type' => 'item',
        '#prefix' => ' <div style="float:left;width:230px;" align="left"><font style="color:#3570AA;font-size: 18px;">Project Settings</font>',
        '#suffix' => '</font></div>
                     </div>',
    );



    $form['project_setting']['title'] = array(
        '#type' => 'textfield',
        '#default_value' => (isset($project_obj) ? $project_obj->title : ''),
        '#maxlength' => 50,
        '#prefix' => $HTML_PROJECT_SETTING_PAGE_HDR . '<tr><th style="width: 139px;">Title*</th><td><div style="margin-top: 7px;">',
        '#suffix' => '&nbsp;Max characters = 50</div></td></tr>',
    );


    $is_library_project = false;
    if (isset($project_obj)) {
        if ($project_obj->type == 'ntlp_library_project')
            $is_library_project = true;
    }

    $form['is_library_project'] = array(
        '#type' => 'hidden',
        '#id' => 'is_library_project',
        '#value' => $is_library_project,
    );



    if ($user_role == NTLP_ROLEID_NETWORKMANAGER && $is_library_project) {

        $submitted_by_name = '';
        $submitted_by_uid = 0;

        $submitted_by = get_project_submitted_by_name($project_obj->nid);
        if ($submitted_by) {
            $submitted_by_name = $submitted_by->first_name . ' ' . $submitted_by->last_name;
            $submitted_by_uid = $submitted_by->uid;
        }

        $form['project_setting']['author'] = array(
            '#type' => 'item',
            '#value' => $submitted_by_name,
            '#prefix' => '<tr><th style="width: 139px;">Author</th><td><div id="submitted_by_view" style="float: left; width: 150px;">',
            '#suffix' => '</div>',
        );

        $form['submitted_by_uid'] = array(
            '#type' => 'hidden',
            '#id' => 'submitted_by_uid',
            //'#default_value' => $submitted_by_uid,
            '#default_value' => $submitted_by_uid,
        );

        cache_set("pplfndPrimaryInstructor_" . session_id(), $submitted_by_uid, 'cache', CACHE_TEMPORARY);

        if ($project_obj->type == 'ntlp_project') {
            $school_nid = get_course_school_nid($course_nid);
        } else {
            $school_nid = get_this_school();
        }
        //Change Author
        $form['project_setting']['author1'] = array(
            '#type' => 'item',
            '#value' => dlg('Change Author', 'ntlp/people_finder/primaryteacher_multi/' .
                    $school_nid . '/' .
                    $course_nid . '/' .
                    "pplfndPrimaryInstructor_" . session_id() . '/' .
                    NTLP_ROLEID_TEACHER, 350, 600, 'whitename'),
            '#prefix' => '<div class="Activities_btn" style="float: left ! important; margin: 0px 0 0 !important;" id="pplfndPrimaryInstructor">',
            '#suffix' => '</div></td></tr>',
        );
    }


    if (!$is_library_project) {

        $form['project_setting']['visibility'] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($project_obj) ? ($project_obj->availability == 'S' ? 1 : 0) : 0),
            '#prefix' => '<tr><th style="width: 139px;">Visibility</th><td><span class="pro_box">A project must be set to “show” for<strong> any </strong>of its content to be seen by students.</span>  ',
            '#suffix' => '</td></tr>',
            '#title' => 'Show',
        );
    }
    $form['project_setting']['proj_duration_from'] = array(
        '#type' => 'textfield',
        '#default_value' => (isset($project_obj) ? get_tz_course($project_obj->course_nid, NTLP_DATE_TZ_SHORT, $project_obj->from_date) : ''),
        '#size' => '10',
        '#prefix' => '<tr><th style="width: 139px;">Duration Dates*</th><td><div style="float:left; width:200px;"><span style="float:left; margin-top: 2px;">Start &nbsp;&nbsp;</span>',
        '#suffix' => '</div>',
        '#attributes' => array('onclick' => 'setup_date_picker()', 'style' => 'padding-left:2px;padding-right:0px;', 'class' => 'ntlp_datepicker'),
    );

    $form['project_setting']['proj_duration_to'] = array(
        '#type' => 'textfield',
        '#default_value' => (isset($project_obj) ? get_tz_course($project_obj->course_nid, NTLP_DATE_TZ_SHORT, $project_obj->to_date) : ''),
        '#size' => '10',
        '#prefix' => '<div style="float:left; width:200px;"><span style="float:left;margin-top: 2px;">End &nbsp;&nbsp;</span>',
        '#suffix' => '</div></td></tr>',
        '#attributes' => array('class' => 'ntlp_datepicker', 'style' => 'padding-left:2px;padding-right:0px;'),
    );

    $hiddendata = '';
    $attached_files_data = '';
    if (isset($project_obj) && $project_obj != null) {

        $file_path = '';
        if ($file_obj = _get_dpl_files_obj($project_obj->picture_fid)) {
            $file_path = $file_obj->filepath;
            $hiddendata .= 'F,' . $project_obj->picture_fid . ',' . $file_obj->filename . ';';
            $attached_files_data .= '<div id="F_' . $file_obj->fid . '"> <a style="vertical-align:middle;" onclick="delete_attach(\'F_' . $file_obj->fid . '\');" ><img src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png" border="0" width="14" height="14" /></a>&nbsp;&nbsp;<a href="' . $base_path . $file_path . '" target="_blank"><span>' . $file_obj->filename . '<span></a></div>';
        }
    }

    $form['#fileupload_embed'] = true;
    $form['#fileupload_auto_upload'] = true;
///&& !empty($file_path)
    if (isset($project_obj) && $project_obj->picture_fid != 0) {
        $attach_fileupload_link = '<div id="attach_links" style="width: 180px;" >' . $attached_files_data . '<div id="upload_control_div" style="width: 180px;" onmouseover="place_fileupload_control(\'edit-project-setting-upload-control\', this);" onmouseout="place_fileupload_control_mouseout(this);"><a style="float:left;text-decoration:none;color: #3399CC;display:none;" id="attach_link"><span style="color:#000">+&nbsp;</span>Attach file</a></div></div>';
    } else {
        $attach_fileupload_link = '<div id="attach_links" style="width: 180px;" ><div id="upload_control_div" style="width: 180px;" onmouseover="place_fileupload_control(\'edit-project-setting-upload-control\', this);" onmouseout="place_fileupload_control_mouseout(this);"><a style="float:left;text-decoration:none;color: #3399CC;display:block;" id="attach_link"><span style="color:#000">+&nbsp;</span>Attach file</a></div></div>';
    }

    $form['project_setting']['upload_control'] = array(
        '#prefix' => '<tr>
            <th style="width:139px; vertical-align:top; padding:10px 10px 10px 0;" >' . t('Project Image') . '</th>
            <td style="background-color:#cfe5f3;cursor:default !important"><span class="pro_box">For best results, upload a square image that is at least 125 x 125 pixels.</span>' . $attach_fileupload_link,
        '#suffix' => '</td></tr>',
    );

    fileupload_get_control($form, $form_state, array('project_setting', 'upload_control'));


    $form['project_setting']['attached_files_hidden'] = array(
        '#type' => 'hidden',
        '#value' => $hiddendata,
        '#id' => 'attached_files_hidden',
    );
    if ($is_library_project) {
        $driving_question = 'Driving Question';
    } else {
        $driving_question = 'Driving Question*';
    }
    $form['project_setting']['driving_question'] = array(
        '#type' => 'textarea',
        '#value' => (isset($project_obj) ? $project_obj->driving_question : ''),
        '#wysiwyg' => False,
        '#maxlength' => '200',
        '#prefix' => '<tr><th style="width: 139px;">' . $driving_question . '</th><td>',
        '#suffix' => '<span style="float: left; margin-top: 6px;">Max characters = 300</span></td></tr>',
            //'#attributes' => array(),
            //'#attributes' => array('onkeyup' =>'textLimit(this,200)'),
    );

    $get_unit_topic_name = get_unit_topic_name($project_obj->topic_unit_tid);
    $form['project_setting']['topic_unit'] = array(
        '#type' => 'textfield',
        '#size' => '40',
        '#value' => (isset($project_obj) ? $get_unit_topic_name->name : ''),
        '#prefix' => '<tr><th style="width: 139px;">Topic/Unit</th><td><span class="pro_box">Associates this project with a general curriculum topic.</span>',
        '#suffix' => '</td></tr>',
        '#autocomplete_path' => 'ntlp/unit/autocomplete/' . $course_nid,
    );

    $form['project_setting']['curriculum_standards'] = array(
        '#type' => 'textarea',
        '#value' => (isset($project_obj) ? $project_obj->academic_standards : ''),
        '#wysiwyg' => False,
        '#prefix' => '<tr><th style="width: 139px;">Curriculum Standards</th><td>',
        '#suffix' => '</td></tr></table>',
    );




    return $form;
}

function ntlp_project_setting_form_validate($form, $form_state) {

    $fs = $form_state['clicked_button']['#post'];
    watchdog('project_set', print_r($fs['project_keywords'], true));
    $action = $fs['op'];
    $course_nid = $fs['project_setting']['course_nid'];
    if ($action != 'Cancel') {

        if ($fs['project_setting']['title'] == '') {
            $message.= '<li>Title (50 characters or less)</li>';
        }

        if ($fs['project_setting']['proj_duration_from'] == '' || $fs['project_setting']['proj_duration_to'] == '') {
            $message.= '<li>Duration Dates (start and ending)</li>';
        }


        if ($fs['is_library_project']) {
            if (empty($fs['submitted_by_uid'])) {
                $message.= '<li>Please Select Author</li>';
            } else if (strrpos($fs['submitted_by_uid'], ',')) {
                $message.= '<li>Please Select 1 author only</li>';
            }

            if (empty($fs['project_setting']['summary_for_library_project'])) {
                $message.= '<li>Summary for Project Library</li>';
            }

            $keywords_arr = $fs['project_keywords'];
            $count = 0;
            foreach ($keywords_arr as $key => $value) {
                if (!empty($value)) {
                    $count++;
                }
            }

            if ($count == 0) {
                $message.= '<li>Keywords</li>';
            } elseif ($count < 2) {
                $message.= '<li>Minimum two keywords</li>';
            }
        } else {
            $len_driving_question = strlen(trim($fs['project_setting']['driving_question']));
            if ($fs['project_setting']['driving_question'] == '' || $len_driving_question > 300 || $len_driving_question > 0 && $len_driving_question < 11) {
                $message.= '<li>Driving Question (30-300 characters)</li>';
            }
        }


//        if ($len_driving_question > 200) {
//            $message.= '<li>You can enter upto maximum of 200 characters for your Driving Question</li>';
//        }
//        if ($len_driving_question > 0 && $len_driving_question < 11) {
//            form_set_error('', t('You must enter a complete Driving Question before saving.'));
//        }

        if (strlen($fs['project_setting']['summary_for_library_project']) > 300) {
            $message.= '<li>You can enter upto maximum of 300 characters for your Summary for library project</li>';
        }

        $resource_file = $_POST['project_setting']['attached_files_hidden'];


        $store_into_temp = new stdClass;
//        $store_into_temp->project_nid = $project_nid;
        $store_into_temp->picture_link = $resource_file;

        cache_set("ntlp_project_setting:" . session_id(), $store_into_temp, 'cache', CACHE_TEMPORARY);

        if ($message) {
            form_set_error('', 'The following fields are required: <ul>' . $message . '</ul>');
        }
    } else {
        cache_set("ntlp_project_setting:" . session_id(), null, 'cache', CACHE_TEMPORARY);
        drupal_goto('ntlp/goback');
    }
}

function ntlp_project_setting_form_submit($form, $form_state) {
    global $user;
    cache_set("ntlp_project_setting:" . session_id(), null, 'cache', CACHE_TEMPORARY);
    $is_library_project = false;
    $fs = $form_state['clicked_button']['#post'];

    $course_nid = $fs['project_setting']['course_nid'];
    $project_nid = $fs['project_setting']['project_nid'];

    $proj_title = $fs['project_setting']['title'];
    $proj_visibility = $fs['project_setting']['visibility'];

    $proj_duration_from = $fs['project_setting']['proj_duration_from'];
    $proj_duration_to = $fs['project_setting']['proj_duration_to'];

    $proj_driving_question = $fs['project_setting']['driving_question'];
    $proj_topic_unit = $fs['project_setting']['topic_unit'];
    $proj_curriculum_standards = $fs['project_setting']['curriculum_standards'];

    $proj_attached_files_hidden = $fs['project_setting']['attached_files_hidden'];

    /* generate a array of attached files */
    $project_image_attachment = get_links($proj_attached_files_hidden);

    if ($project_nid != 0) {
        $project_node = node_load($project_nid);

        if ($project_node->type == 'ntlp_library_project') {
            $is_library_project = true;
        }
    } else {
        $project_node = new stdClass();
        $project_node->created = time();
    }

    $project_pic_fid = 0;
    if ($project_image_attachment != null || !empty($project_image_attachment)) {
        foreach ($project_image_attachment as $l) {
            list($type, $id, $url) = $l;
            if ($type != 'X') {
                $project_pic_fid = abs($id);
            }
        }
    }

    $project_node->title = $proj_title;

    if ($is_library_project) {
        $project_node->type = 'ntlp_library_project';   // Your specified content type
    } else {
        $project_node->type = 'ntlp_project';   // Your specified content type    
    }

    $project_node->body = $proj_summary_for_library_project;   //
    $project_node->changed = time();
    $project_node->status = 1;
    $project_node->promote = 0;
    $project_node->sticky = 0;
    $project_node->format = 1;
    $project_node->uid = $user->uid;
    $project_node->context = $course_nid;

    node_save($project_node);
    $school_nid = get_this_school();

    $proj_duration_from = set_tz($school_nid, "Y-m-d H:i:s", $proj_duration_from);
    $proj_duration_to = set_tz($school_nid, "Y-m-d H:i:s", $proj_duration_to);

    if (!$is_library_project) {
        if ($proj_topic_unit != null && $proj_topic_unit != "") {
            $unit_topic = add_taxonomy_for_unit('NTLP Course Unit', $proj_topic_unit, $course_nid);
        } else {
            $unit_topic = 0;
        }
    } else {
        $unit_topic = 0;
    }
    if ($project_nid != 0) {
        db_query("UPDATE {ntlp_course} SET availability = '%s', from_date= '%s', to_date ='%s',
            course_short_title = '%s', driving_question='%s', academic_standards='%s', picture_fid=%d, 
            topic_unit_tid = %d
            WHERE nid = %d", ($proj_visibility == 1 ? 'S' : 'H'), $proj_duration_from, $proj_duration_to, $proj_topic_unit, $proj_driving_question, $proj_curriculum_standards, $project_pic_fid, $unit_topic->tid, $project_nid);

        if ($is_library_project) {
            $author_id = $fs['submitted_by_uid'];
            db_query("UPDATE {ntlp_library_project} SET submitted_by = %d WHERE nid = %d", $author_id, $project_nid);
        }
    } else {
        db_query("INSERT INTO {ntlp_course} (nid, school_nid, is_project, course_nid, availability,
        from_date, to_date, course_short_title, driving_question, academic_standards, picture_fid, 
        topic_unit_tid) VALUES (%d, %d, %d, %d, '%s', '%s', '%s', '%s', '%s', '%s', %d, %d )", $project_node->nid, $school_nid, 1, $course_nid, ($proj_visibility == 1 ? 'S' : 'H'), $proj_duration_from, $proj_duration_to, $proj_topic_unit, $proj_driving_question, $proj_curriculum_standards, $project_pic_fid, $unit_topic->tid);

        module_invoke_all('ntlpcontent', $project_node, 'content_posted');
    }

    // save the keywords in term node for project relation

    if (sizeof($keywords) == 0 || $keywords == null) {
        db_query("DELETE FROM {term_node} WHERE nid = %d 
                AND tid IN (SELECT tid FROM {term_data} WHERE vid = %d)", $project_node->nid, NTLP_KEYWORDS_FOR_PROJECT_LIBRARY);
    }

    if ($keywords != null && sizeof($keywords) > 0) {

        db_query("DELETE FROM {term_node} WHERE nid = %d 
                AND tid IN (SELECT tid FROM {term_data} WHERE vid = %d)", $project_node->nid, NTLP_KEYWORDS_FOR_PROJECT_LIBRARY);

        $keywords = array_unique($keywords);
        foreach ($keywords as $keyword) {

            if (isset($keyword) && !empty($keyword)) { // if keyword not empty
                $keyword_tid = get_term_obj_by_name_and_vocabulary($keyword, NTLP_KEYWORDS_FOR_PROJECT_LIBRARY);

                if ($keyword_tid) {
                    db_query("INSERT INTO {term_node} (nid, vid, tid) VALUES(%d, %d, %d) ", $project_node->nid, $project_node->nid, $keyword_tid);
                } else {

                    db_query("INSERT INTO {term_data} (vid, name, description, weight) VALUES (%d, '%s', '%s', 0)"
                            , NTLP_KEYWORDS_FOR_PROJECT_LIBRARY, $keyword, '');

                    $tid = db_last_insert_id('{term_data}', 'tid');

                    db_query("INSERT INTO {term_node} (nid, vid, tid) VALUES(%d, %d, %d) ", $project_node->nid, $project_node->nid, $tid);
                }
            }
        }
    }

    if ($is_library_project) {
        drupal_goto('ntlp/library/project/view/' . $project_nid);
    } else {
        drupal_goto('ntlp/courses/projects/' . $course_nid . '/' . $project_node->nid);
    }
}

function get_projects_total_counts($course_nid, $option, $user_role) {
    global $user;

    if ($user_role == NTLP_ROLEID_STUDENT || isset($user->roles[NTLP_ROLEID_PARENT])
            || $user_role == NTLP_ROLEID_SITEGUEST) {
        $query = "SELECT DISTINCT(c.nid), n.title, n.nid, c1.availability, c1.from_date, c1.to_date,
            c1.picture_fid, c1.academic_standards, nr.body
        FROM dpl_ntlp_course c
        INNER JOIN dpl_node n ON c.nid = n.nid
        INNER JOIN dpl_node_revisions nr ON nr.nid = c.nid
        LEFT JOIN dpl_ntlp_course c1 ON n.nid = c1.nid
        WHERE (c.is_project <> 0) AND (c.course_nid = %d) AND (c.availability = 's') 
        GROUP BY c.nid ";
    } else {
        $query = "SELECT DISTINCT(c.nid), n.title, n.nid, c1.availability, c1.from_date, c1.to_date,
            c1.picture_fid, c1.academic_standards, nr.body
        FROM dpl_ntlp_course c
        INNER JOIN dpl_node n ON c.nid = n.nid
        INNER JOIN dpl_node_revisions nr ON nr.nid = c.nid
        LEFT JOIN dpl_ntlp_course c1 ON n.nid = c1.nid
        WHERE (c.is_project <> 0) AND (c.course_nid = %d)  
        GROUP BY c.nid ";
    }

    $project_result = db_query($query, $course_nid);

    while ($data = db_fetch_object($project_result)) {
        $project_record[] = $data;
    }
    return $project_record;
}

function _get_project_keywords_for_library($project_nid) {
    $records = false;
    $result = db_query("SELECT td.tid, td.name FROM dpl_term_node tn
        INNER JOIN dpl_term_data td ON td.tid = tn.tid
        WHERE tn.nid = %d AND td.vid = %d", $project_nid, NTLP_KEYWORDS_FOR_PROJECT_LIBRARY);

    if ($result->num_rows > 0) {

        while ($data = db_fetch_object($result)) {
            $records[] = $data->name;
        }
    }

    return $records;
}
