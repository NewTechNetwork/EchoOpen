<?php

function count_ntlp_resources($project_nid, $section_nid) {

    $rs = db_query("SELECT count(nid) as total FROM {ntlp_project_resource} WHERE project_nid = %d AND section_tid = %d", $project_nid, $section_nid);

    $result = db_fetch_object($rs);

    $total = $result->total;
    return $total;
}

function get_resource_detail($resource_nid) {

    $result = db_query("SELECT cr.*, pr.*,
                nr.title, cr.type as resource_type, n.type, n.changed, nr.body, f.*
        FROM {ntlp_course_resource} cr
        INNER JOIN {node} n ON n.nid = cr.nid
        INNER JOIN {node_revisions} nr ON nr.nid = cr.nid
        LEFT JOIN {ntlp_project_resource} pr ON pr.nid = cr.nid
        LEFT JOIN {files} f ON f.fid = cr.fid
        WHERE cr.nid = %d", $resource_nid);

    return db_fetch_object($result);
}

function check_resource($resource_nid, $course_nid) {

    $rs = db_query("select * from {ntlp_course_resource} where  nid = %d and course_nid = %d", $resource_nid, $course_nid);
    if ($rs->num_rows > 0) {

        return true;
    } else {
        return false;
    }
}

function get_resource_owner($resource_nid) {

    $rs = db_query("select course_nid from {ntlp_course_resource} where nid = {$resource_nid}");

    return db_fetch_object($rs);
}

function is_resource_exist_in_prjoect($resource_nid, $project, $project_section) {
    $flag = false;

    $result = db_query("SELECT * FROM {ntlp_project_resource}
        WHERE project_nid = %d AND section_tid = %d AND nid = %d ", $project, $project_section, $resource_nid);

    if ($result->num_rows > 0) {
        $flag = true;
    } else {
        $flag = false;
    }

    return $flag;
}

function _get_resource_order($course_nid, $section_tid) {
//    $result = db_query("SELECT Max(resource_order) AS resource_order FROM {ntlp_project_resource} WHERE project_nid = %d AND section_tid = %d ", $project_id, $section_tid);

    $result = db_query("SELECT MAX(pr.resource_order) AS resource_order
        FROM dpl_ntlp_project_resource pr
        INNER JOIN dpl_ntlp_course_resource cr ON pr.nid = cr.nid
        WHERE cr.course_nid = %d AND pr.section_tid = %d
        UNION SELECT MAX(a.activity_order) AS resource_order
        FROM dpl_ntlp_course_activity a
        WHERE a.course_nid = %d AND a.project_cat =%d
        ORDER BY resource_order DESC LIMIT 0, 1 ", $course_nid, $section_tid, $course_nid, $section_tid);

    if ($result) {
        $rec = db_fetch_object($result);
        return $rec;
    } else {
        return false;
    }
}

function is_project_exist_course($course_nid, $project_nid) {
    $flag = false;
    $result = db_query("SELECT * FROM dpl_ntlp_course WHERE course_nid = %d AND nid = %d", $course_nid, $project_nid);

    if ($result->num_rows > 0) {
        $flag = true;
    }

    return $flag;
}

function fix_order($object) {

    $i = 1;
    foreach ($object as $obj) {
        if ($obj->restype == "R") {
            db_query("UPDATE dpl_ntlp_project_resource pr SET pr.resource_order = %d WHERE nid = %d", $i, $obj->nid);
        } else {
            db_query("UPDATE dpl_ntlp_course_activity pr SET pr.activity_order = %d WHERE nid = %d", $i, $obj->nid);
        }

        $i++;
    }
}

?>
