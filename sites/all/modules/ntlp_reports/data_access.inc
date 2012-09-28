<?php

function get_report_basic($report_nid) {
    //Get report basic info
    $result = db_query("SELECT * FROM {ntlp_report} r WHERE r.nid = %d", $report_nid);
    if ($result->num_rows > 0) {
        $rec = db_fetch_object($result);
        return $rec;
    }
    return FALSE;
}

function get_report_info($report_nid, $format) {
    //Get report details
    $result = db_query("SELECT * FROM {ntlp_report} r, {ntlp_report_attachment} a, {files} f WHERE r.nid = a.nid AND a.fid = f.fid AND a.nid = %d AND a.output_format = '%s'", $report_nid, $format);
    if ($result->num_rows > 0) {
        $rec = db_fetch_object($result);
        return $rec;
    }
    return FALSE;
}

function get_subreports($report_nid) {
    //Get result set containing records of sub-reports
    $result = db_query("SELECT * FROM {ntlp_report} r, {ntlp_report_attachment} a, {files} f WHERE r.nid = a.nid AND a.fid = f.fid AND a.nid = %d AND a.output_format = 'SUB'", $report_nid);
    if ($result->num_rows > 0) {
        return $result;
    }
    return FALSE;
}

function get_report_parameters($report_nid, $format) {
    //Get report parameters
    $result = db_query("SELECT name, datatype, source, default_value FROM {ntlp_report_param} WHERE report_nid = %d AND output_format = '%s'", $report_nid, $format);
    if ($result->num_rows > 0) {
        $params = array();
        while ($prec = db_fetch_object($result)) {
            $params[$prec->name] = $prec;
        }
        return $params;
    }
    return FALSE;
}

function get_report_parameters_byfile($file_id) {
    //Get report parameters by file id
    $result = db_query("SELECT p.output_format, p.name, p.datatype, p.source, p.default_value FROM {ntlp_report_param} p, {ntlp_report_attachment} a WHERE p.report_nid = a.nid AND p.output_format = a.output_format AND a.fid = %d", $file_id);
    if ($result->num_rows > 0) {
        $params = array();
        while ($prec = db_fetch_object($result)) {
            $params[$prec->name] = $prec;
        }
        return $params;
    }
    return FALSE;
}

function get_ntlp_reports($category = null, $all_types = false) {

    if (isset($category))
        $category = "'" . $category . "'";
    else
        $category = '"ntlp_report_school" , "ntlp_report_network"'; //, "ntlp_subreport"';

    if ($all_types)
        $type_filter = '';
    else
        $type_filter = ' AND r.app_location = "REP" ';

    $query = 'SELECT n.*, r.type as report_type, r.app_location FROM {node} n, {ntlp_report} r WHERE n.status = 1 AND n.nid = r.nid AND n.type IN ( ' . $category . ') ' . $type_filter . ' ORDER BY n.title';

    $rows = array();
    if ($result = db_query($query)) {
        if ($result->num_rows > 0) {
            while ($row = db_fetch_object($result)) {
                $rows[] = $row;
            }
        }
    }
    return $rows;
}

function get_app_locations() {
    $option = array(
        "GSD" => "Course: Gradebook: Student Detail",
        "GTV" => "Course: Gradebook: Teacher View",
        "GSS" => "Grades: Student Snapshot",
        "SDE" => "School Mgmt: Data Export",
        "REP" => "Reports",
    );
    return $option;
}

function get_app_location_bycode($code) {
    $options = get_app_locations();
    return $options[$code];
}