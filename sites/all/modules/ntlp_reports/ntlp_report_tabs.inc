<?php

function ntlp_reports_render_tabs($active_tab = "NOP"){
    include('report_main.template.inc');

    $output =  $HTMLCourse_tabs;

    if ($active_tab == "NOP") {
        $get = $_GET['q'];
        $br = explode("/", $get);
        $cnt = count($br);
        $active_tab = "@IS_" . strtoupper(arg(2));
    }

    $runreport_url = url("ntlp/reports");
    $managereport_url = url("ntlp/reports/manage");

     foreach ($br as $obj) {
        if ($obj == 'reports') {
            $active_tab = "@IS_RUN_REPORT";
        } else if ($obj == 'manage' || 'new' || 'edit') {
            $active_tab = "@IS_MNG_REPORT";
        }
    }

    $report_tab = '<li class="@IS_RUN_REPORT"><a class="@IS_RUN_REPORT" href="' . $runreport_url . '">Run Reports</a></li>';
    $manage_report_tab = '<li class="@IS_MNG_REPORT"><a class="@IS_MNG_REPORT" href="' . $managereport_url . '">Manage Reports</a></li>';


     if ($active_tab == '@IS_RUN_REPORT') {
            $report_tab = str_replace($active_tab, "active", $report_tab);
        } else if ($active_tab == '@IS_MNG_REPORT') {
            $manage_report_tab = str_replace($active_tab, "active", $manage_report_tab);
        }

    if (user_access('view ntlp reports')) {
        $output = str_replace("@RUN_REPORTS", $report_tab, $output);
    } else {
        $output = str_replace("@RUN_REPORTS", '', $output);
    }
    if (user_access('manage ntlp reports')) {
        $output = str_replace("@MANAGE_REPORTS", $manage_report_tab, $output);
    } else {
        $output = str_replace("@MANAGE_REPORTS", '', $output);
    }

    if (user_access('create ntlp reports'))
    {
        $output = str_replace('<!-- CREATE_REPORT -->', '<div style="width:117px; float:right;"><a style="font-weight:bold;" href="' . url('ntlp/reports/new') . '" class="SubmitAssignmentBtn" >Create Report</a></div>', $output);
    }
    else
    {
        $output = str_replace('<!-- CREATE_REPORT -->', '', $output);
    }

    return $output;
}


function ntlp_reports_render_header(){
    include('report_main.template.inc');
    global $user;
    
    $container = $HTMLSelectorsAndHeading;
    $container = str_replace('@LINK_HOME', l('Tools', 'ntlp/tools'), $container);
    $container = str_replace('@LINK_REPORTS', l('Reports', 'ntlp/reports'), $container);
    $container = str_replace('<!-- heading -->', 'Reports', $container);

     return $container;
}


function ntlp_reports_runreports_tab()  {
    global $user;

    include('report_main.template.inc');
    drupal_add_js(drupal_get_path('module', 'ntlp_blocks') . '/default_header.js');
    modalframe_parent_js();
    
    $yellow_bar = $HTMLReport_yellow_bar;

    if (isset($user->roles[NTLP_ROLEID_NETWORKSTAFF]) || isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])) {
        $school_nid = get_this_school();
    } else {
        $school_nid = get_user_school($user->uid);
    }
    $options_array = _get_all_school_years($school_nid);
    $current_year = get_this_school_year();
    drupal_add_js("
                function change_school_year(obj){
                    $.post('?q=ntlp/change/school/year/'+obj.value, function(){
                        window.location.reload();
                    });
                }
            ", 'inline');

    $year_select = '<select id="year_by" onchange="change_school_year(this);">';
    if ($options_array) {
        foreach ($options_array as $k => $v) {
            $year_select .= '<option value="' . $k . '" ' . ($current_year == $v ? 'selected="selected"' : '') . '>' . $v . '</option>';
        }
    }
    $year_select .= '</select>';

    $school = get_schools();
    $school_selector = '<select id="view_by" onchange="onchange_schoolselector(this.value);">';
    while ($rec = db_fetch_object($school)) {
        $school_selector .= '<option value="' . $rec->nid . '"' . ($school_nid == $rec->nid ? 'selected="selected"' : '') . '>' . (strlen($rec->school_name) >= 40 ? substr($rec->school_name, 0, 40) . '...' : $rec->school_name) . '</option>';
    }
    $school_selector .= '</select>';

    $output  .= str_replace('<!--SCHOOL-->', $school_selector, $yellow_bar );
    $output = str_replace('<!--SCHOOL_YEAR-->', $year_select, $output);

    $header = array(
        array('data' => '', 'colspan' => '2'),
        array('data' => ''),
        array('data' => ''),
    );

    $reports = get_ntlp_reports("ntlp_report_school");
    foreach ($reports as $report) {
        $rows[] = array(
            array('data' => $report->title, 'colspan' => '2'),
            array('data' => 'Visibility: ' . ($report->status == 1 ? 'Show' : 'Hide')),
            array('data' => dlg('RUN REPORT', 'ntlp/reports/inputform/' . $report->nid . '/PDF', 450, 600, 'green_link')),
        );
    }

    if (isset($user->roles[NTLP_ROLEID_NETWORKSTAFF]) || isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])) {
        $reports = get_ntlp_reports("ntlp_report_network");
        foreach ($reports as $report) {
            $rows[] = array(
                array('data' => $report->title, 'colspan' => '2'),
                array('data' => 'Visibility: ' . ($report->status == 1 ? 'Show' : 'Hide')),
                array('data' => dlg('RUN REPORT', 'ntlp/reports/inputform/' . $report->nid . '/PDF', 450, 600, 'green_link')),
            );
        }
    }

    $attributes = array('class' => 'reports_table ', 'width' => '100%', 'cellpadding' => '8');
    $output .= theme_table(null, $rows, $attributes);


    return $output ;
}

function ntlp_reports_managereports_tab() {
    drupal_add_js(drupal_get_path('module', 'ntlp_reports') . '/ntlp_reports.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/ntlp_courses_activity.js');
    jquery_ui_add('ui.accordion');
    drupal_add_css(drupal_get_path('module', 'jquery_ui') . '/jquery.ui/themes/base/ui.all.css');
    
    $activity_rows = ntlp_render_manage_reports_table("ntlp_report_school");

    $html = '<div id="dialog-managereport" style="background-color:#fff;display:none;" title="Manage Reports">
                <p><span class="msg" style="color:#000000;"></span></p>
            </div>';
    $html .= '<div id="notaccordion">';
    $html .= '<div class="ActivitiesPanel notaccordion">
                    <div style="margin-top:20px">
                    <div class="echo_tbl_heading">
                        <div style="float:left">
                            <div style="float:left"><span class="ui-icon ui-icon-triangle-1-s"></span></div>
                            <a>School-Level Reports</a>
                        </div>
                   </div>
                   </div>
                   </div>';
    $html .= '<div style="clear:both">
                            <table cellspacing="0" class="StateTable" cellpadding="0" border="0" width="100%"  >';
    $html .= '<tr id = "row_1"><td colspan="2" >' . $activity_rows . '</td></tr></table><div style="font-size: 10px;">&nbsp;</div></div>';

    $network_reports = ntlp_render_manage_reports_table("ntlp_report_network");

    $html .= '<div class="ActivitiesPanel notaccordion">
                    <div style="margin-top:20px">
                    <div class="echo_tbl_heading">
                        <div style="float:left">
                            <div style="float:left"><span class="ui-icon ui-icon-triangle-1-s"></span></div>
                            <a>Network-Wide Reports</a>
                        </div>
                   </div>
                   </div>
                   </div>';
    $html .= '<div style="clear:both">
                            <table cellspacing="0" class="StateTable" cellpadding="0" border="0" width="100%"  >';
    $html .= '<tr id = "row_2"><td colspan="2" >' . $network_reports . '</td></tr></table><div style="font-size: 10px;">&nbsp;</div></div>';

    return $html;
}

function ntlp_render_manage_reports_table($node_type = NULL) {
    global $base_path, $user;

    if (in_array(NTLP_ROLEID_NETWORKMANAGER, $user->roles) || in_array(NTLP_ROLEID_NETWORKSTAFF, $user->roles)) {
        $school_nid = get_this_school();
    } else {
        $school_nid = get_user_school($user->uid);
    }
    $head = array(
        array('data' => t('Report Title'), 'width' => '40%'),
        array('data' => t('Location'), 'width' => '25%'),
        array('data' => t('Date Last Modified'), 'width' => '20%'),
        array('data' => t('Actions'), 'width' => '10%'),
    );

    $row_data = get_ntlp_reports($node_type, true);
    foreach ($row_data as $row) {
//        $download_link = '<span style="float:left;margin:0px 4px 4px 0px">
//                    <a target="_blank" href="' . url('ntlp/reports/source/' . $row->nid . '/PDF') . '">
//                        <img src="' . $base_path . 'themes/Boldr/Images/common/downloadreport.png" height="16" width="16" alt="download"/>
//                    </a></span>';
        $edit_link = '<span style="float:left;margin:0px 4px 4px 0px">
                    <a href="' . url('ntlp/reports/edit/' . $row->nid) . '">
                        <img src="' . $base_path . 'themes/Boldr/Images/common/edit.png" height="16" width="16" alt="edit"/>
                    </a></span>';
        $delete_link = '<span style="float:left;margin:0px 4px 4px 0px">
                    <a onclick="delete_report(' . $row->nid . ')">
                        <img src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png" height="16" width="16" alt="delete"/>
                    </a></span>';
        
        $rows[] = array(
            array('data' => $row->title),
            array('data' => get_app_location_bycode($row->app_location)),
            array('data' => get_tz($school_nid, null, $row->changed)),
            array('data' => $edit_link . $delete_link),
        );
    }

    $output = theme('table', $head, $rows, $attributes = array('class' => 'ntlp_table', 'width' => '100%', 'cellpadding' => '5'));
    return $output;
}

