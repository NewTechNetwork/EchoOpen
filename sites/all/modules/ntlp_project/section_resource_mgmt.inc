<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


/* Delete a resource/activity from a Project Section */

function ntlp_courses_projects_delete_resource($course_nid, $project_nid, $resource_id) {
    $section_tid = arg(7);
    db_query('DELETE FROM {ntlp_project_resource} WHERE nid = %d AND section_tid = %d', $resource_id, $section_tid);

    print get_project_sections_data($course_nid, $project_nid);
}

function ntlp_courses_projects_delete_activity($course_nid, $project_nid, $activity_nid) {
    $section_tid = arg(7);
//    ntlp_activity_delete_data($course_nid, $activity_nid);
//update Activity
    db_query('UPDATE {ntlp_course_activity} set  project_cat = %d, project_nid = %d
        WHERE nid = %d AND  project_cat = %d', 0, 0, $activity_nid, $section_tid);

    print get_project_sections_data($course_nid, $project_nid);
}

/* Hide or Show a resource/activity in a Project Section */

function show_hide_resource($action, $resource_nid, $section_tid) {
    db_query("UPDATE {ntlp_project_resource} SET availability = '%s' 
        WHERE nid = %d AND section_tid = %d ", $action, $resource_nid, $section_tid);
}

function show_hide_activity($action, $activity_nid, $section_tid) {

    if ($action == 'S') {
        $amode = 'S';
        if ($arec = get_activity_record($activity_nid)) {
            $term_data = get_school_term_data($arec->school_term_tid);
            if ($term_data) {
                $afrom = $term_data->date_started;
                $ato = $term_data->date_closed;

                $school_nid = get_course_school_nid($arec->course_nid);
                $afrom = set_tz($school_nid, "Y-m-d H:i:s", $afrom);
                $ato = set_tz($school_nid, "Y-m-d H:i:s", $ato);
            }
        }
    } else {
        $amode = 'M';
        $afrom = '0000-00-00 00:00:00';
        $ato = '0000-00-00 00:00:00';
    }

    db_query("UPDATE {ntlp_course_activity} SET availability_mode = '%s', available_from = '%s', available_to = '%s'
        WHERE nid = %d", $amode, $afrom, $ato, $activity_nid);
}

/* Set Sort Order of a resource/activity in a Project Section */

function section_resource_order($slide_order, $resource_nid, $course_nid, $section_tid) {
    $resource_type = arg(7);

    $section_resource_records = _get_project_section_resources($course_nid, $section_tid);
    $fix_it = false;


    foreach ($section_resource_records as $records) {
        $project_nid = $records->project_nid;
        if ($records->nid == $resource_nid) {
            if ($slide_order == 0) { //UP
                $change_order = $records->resource_order - 1;
            } else {                 //DOWN
                $change_order = $records->resource_order + 1;  //Get next records' order
            }
                break;
            }
        }

    foreach ($section_resource_records as $records) {

        if ($records->nid == $resource_nid) {
            $current_resource_type = $records->restype;
            $current_resource_order = $records->resource_order;
    }

        if ($records->resource_order == $change_order) {
            $next_resource_nid = $records->nid;
            $next_resource_type = $records->restype;
    }
    }

    if ($next_resource_type == 'R') {
        db_query("UPDATE {ntlp_project_resource} SET resource_order = %d WHERE nid = %d AND section_tid = %d", $current_resource_order, $next_resource_nid, $section_tid);
    } else {
        db_query("UPDATE {ntlp_course_activity} SET activity_order = %d WHERE nid = %d AND project_cat = %d", $current_resource_order, $next_resource_nid, $section_tid);
    }
    if ($current_resource_type == 'R') {
        db_query("UPDATE {ntlp_project_resource} SET resource_order = %d WHERE nid = %d AND section_tid = %d", $change_order, $resource_nid, $section_tid);
    } else {
        db_query("UPDATE {ntlp_course_activity} SET activity_order = %d WHERE nid = %d AND project_cat = %d", $change_order, $resource_nid, $section_tid);
    }

    print get_project_sections_data($course_nid, $project_nid);
}

function delte_section_resource($resource_nid) {

    $result = db_query("SELECT * FROM {ntlp_course_resource} WHERE nid = %d", $resource_nid);

    $current_resource_Obj = db_fetch_object($result);

    $project_nid = $current_resource_Obj->course_nid;
    $section_tid = $current_resource_Obj->section_tid;

    if (node_delete($resource_nid)) {
        db_query("DELETE FROM {ntlp_course_resource} WHERE nid= %d", $resource_nid);
    }

    $view = views_get_view('ntlp_project_section_resources');
    $view->set_arguments(array($current_resource_Obj->course_nid, $current_resource_Obj->section_tid));
    print $view->preview();
}

?>