<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


function load_project_record($project_nid) {

    $result = db_query("SELECT c.*, n.* , nr.*  FROM {ntlp_course} c
        INNER JOIN {node} n ON n.nid = c.nid
        INNER JOIN {node_revisions} nr ON nr.nid = n.nid
        WHERE c.nid=%d ", $project_nid);

    if ($result) {
        return db_fetch_object($result);
    } else {
        false;
    }
}

function get_selected_outcomes($projectnid) {

    $result = db_query("SELECT t.tid FROM {term_data} t, {ntlp_gb_course_outcome} o
        WHERE t.tid = o.tid AND t.vid = %d AND o.course_nid=%d", 6, //SELECT vid FROM dpl_vocabulary WHERE name = 'NTLP GB Outcome'
            $projectnid);

    while ($note = db_fetch_array($result)) {
        $selectedoutcome[] = $note['tid'];
    }


    return $selectedoutcome;
}

function _load_project_outcomes($course_id) {

    $result = db_query('SELECT co.tid, td.name, co.weight_percent, co.custom_name FROM {ntlp_gb_course_outcome} co
            INNER JOIN {term_data} td ON td.tid = co.tid WHERE co.course_nid = %d', $course_id);

    while ($note = db_fetch_object($result)) {

        $activityoutcome[] = $note;
    }

    return $activityoutcome;
}

function get_vocabulary_vid($name) {

    $result = db_query("SELECT vid FROM dpl_vocabulary WHERE name = '%s'", $name);

    if ($result) {
        return db_fetch_object($result);
    } else {
        return false;
    }
}

/*
 * Add a Section in Project and return its id
 */

function get_term_id($name, $descripiton = null) {

    $obj = get_vocabulary_vid('NTLP Course Section');
    $result = db_query("INSERT INTO {term_data} (vid, name, description, weight) VALUES (%d, '%s', '%s', 0)"
            , $obj->vid, $name, $descripiton);

    $tid = db_last_insert_id('{term_data}', 'tid');

    return $tid;
}

function get_project_section($project_id) {

//    $result =  db_query("SELECT td.name, td.description, cs.tid, cs.course_nid, cs.section_order
//        FROM {ntlp_course_section} cs
//        INNER JOIN {term_data} td ON td.tid = cs.tid
//        WHERE course_nid = %d", $project_id);

    $result = db_query("SELECT td.name, td.description, cs.tid, cs.course_nid, cs.section_order, cs.availability
        FROM dpl_ntlp_course_section cs
        INNER JOIN dpl_term_data td ON td.tid = cs.tid
        WHERE course_nid = %d order by section_order", $project_id);

    while ($section = db_fetch_object($result)) {

        $project_sections[] = $section;
    }

//    print_r($project_sections);


    return $project_sections;
}

function get_last_order_section($project_id) {

    $result = db_query("SELECT section_order FROM {ntlp_course_section} where course_nid =  %d order by section_order desc limit 1", $project_id);

    return db_fetch_object($result);
}

function copy_section_data($section_tid) {

    $term_Obj = taxonomy_get_term($section_tid);

    $result = db_query("INSERT INTO {term_data} (vid, name, description, weight) VALUES (%d, '%s', '%s', %d)", $term_Obj->vid, $term_Obj->name, $term_Obj->description, $term_Obj->weight);

    return db_last_insert_id('{term_data}', 'tid');
}

function copy_resource_node($resource_node_nid, $course_id) {
    global $user;
    $dateformat = "%Y-%m-%d %H:%M:%S";
    $ctime = strftime($dateformat, strtotime(time()));

    $nodeResource = node_load($resource_node_nid);

    $resource_node = new stdClass();
    $resource_node->type = $nodeResource->type;   // Your specified content type
    $resource_node->title = $nodeResource->title;
    $resource_node->body = $nodeResource->body;
    $resource_node->created = $ctime;
    $resource_node->changed = $ctime;
    $resource_node->status = 1;
    $resource_node->sticky = 0;
    $resource_node->format = 1;       // Filtered HTML
    $resource_node->comment = 2;      // Filtered HTML
    $resource_node->uid = $user->uid;

    node_save($resource_node);
    return $resource_node->nid;
}

function copy_activity_node($activity_node_nid) {
    global $user;

    $dateformat = "%Y-%m-%d %H:%M:%S";
    $ctime = strftime($dateformat, time());

    $nodeActivity = node_load($activity_node_nid);

    $activity_node = new stdClass();
    $activity_node->title = $nodeActivity->title;
    $activity_node->type = $nodeActivity->type;   // Your specified content type
    $activity_node->created = $ctime;
    $activity_node->changed = $ctime;
    $activity_node->body = $nodeActivity->body;
    $activity_node->status = 1;
    $activity_node->sticky = 0;
    $activity_node->format = 1;       // Filtered HTML
    $activity_node->promote = 0;
    $activity_node->uid = $user->uid; // UID of content owner

    node_save($activity_node);
    return $activity_node->nid;
}

function get_new_project_name($project_name) {

    $name = "";


    $number = preg_replace("/[^0-9]/", '', $project_name);

    if ($number != null) {

        $name = str_replace($number, ($number + 1), $project_name);
    } else {

        $name = $project_name . '01';
    }
    return $name;
}

function _get_upload_file_detail($fid) {
    $result = db_query("SELECT * FROM {files} WHERE fid = %d", $fid);
    if ($result) {
        return db_fetch_object($result);
    } else {
        return false;
    }
}

function _get_project_groups_for_student_user($project_nid, $user) {
    $rows = false;

    $sql = "SELECT n.*, COUNT(ncr.nid) as new_count
            FROM {ntlp_group} ng
            LEFT JOIN {ntlp_course_resource} ncr ON ncr.course_nid =  ng.nid
            INNER JOIN {node} n ON n.nid = ng.nid
            INNER JOIN dpl_og_uid ogu ON ogu.nid = ng.nid
            WHERE ng.project_nid = %d AND ogu.uid = " . $user->uid . " AND ogu.is_active = 1 GROUP BY n.nid ORDER BY new_count DESC, n.title";

    $result = db_query($sql, $project_nid);

    if ($result->num_rows > 0) {
        while ($row = db_fetch_object($result)) {
            $rows[] = $row;
        }
    }
    return $rows;
}

function _get_project_groups($project_id, $limit = 5, $header=array()) {
    global $user;
//    $sql = 'SELECT n.*, (SELECT count(ncr.nid) new_count FROM {ntlp_group} ng1
//                            INNER JOIN {node} n ON n.nid = ng1.nid
//                            INNER JOIN {ntlp_course_resource} ncr ON ncr.course_nid = ng1.nid
//                            WHERE ng1.project_nid = %d GROUP BY ng1.nid
//                        ) new_count
//            FROM {ntlp_group} ng
//            INNER JOIN {node} n ON n.nid = ng.nid
//            WHERE ng.project_nid = %d' . tablesort_sql($header) . ' LIMIT ' . $limit;
//
    //saifuddin repalce above query with below as its return subquery error Subquery returns more than 1 row in

    $sql = "SELECT n.*, COUNT(ncr.nid) as new_count
            FROM {ntlp_group} ng
            LEFT JOIN {ntlp_course_resource} ncr ON ncr.course_nid =  ng.nid
            INNER JOIN {node} n ON n.nid = ng.nid
            WHERE ng.project_nid = %d GROUP BY n.nid " . tablesort_sql($header) . ' LIMIT ' . $limit;
    ;

    $result = db_query($sql, $project_id);
    $rows = array();
    if ($result) {
        while ($row = db_fetch_object($result)) {
            $rows[] = $row;
        }
    }

    return $rows;
}

function _get_course_project_titles($course_id) {
    $sql = 'SELECT
                *
            FROM
                dpl_ntlp_course nc
            WHERE
                    nc.course_nid = %d
                AND nc.is_project = 1
                AND LENGTH(nc.course_short_title) > 0
            GROUP BY
                nc.course_short_title
            ';

    $rows = array();
    if ($result = db_query($sql, $course_id)) {
        while ($row = db_fetch_object($result)) {
            $rows[] = $row;
        }
    }

    return $rows;
}

/*
 * @param $resource_ids
 *      Can be an array of IDs or a single integer value.
 */

function delete_project_section_resource($resource_ids, $section_tid) {

    watchdog('delproj_res', $resource_ids, $section_tid);
//    $resource_ids = is_array($resource_ids) ? $resource_ids : array($resource_ids) ;
//    foreach($resource_ids as $resId) {
//        $res_ids[] = (int)$resId;
//    }
//    $resource_ids = implode(',', $res_ids);
    db_query('DELETE FROM {ntlp_project_resource} WHERE nid = %d AND section_tid = %d', $resource_ids, $section_tid);
//        foreach($res_ids as $resid) {
//            node_delete($resid);
//        }
//    }
}

/*
 * @param $section_tid
 *      section id.
 * @return
 *      Array of ids
 */

function _get_section_resources($section_tid) {
    $sql = 'SELECT nid FROM {ntlp_project_resource} WHERE section_tid = %d';
    $resource_ids = array();
    if ($result = db_query($sql, $section_tid)) {
        while ($row = db_fetch_object($result)) {
            $resource_ids[] = $row->nid;
        }
    }

    return $resource_ids;
}

function _get_section_course_id($section_tid) {
    $sql = 'SELECT course_nid FROM {ntlp_course_section} WHERE tid = %d LIMIT 1';
    if ($result = db_query($sql)) {
        $row = db_fetch_object($result);
        return $row->course_nid;
    }

    return 0;
}

function get_course_active_projects($course_id) {

//    $rs = db_query("SELECT n.nid, n.title FROM dpl_ntlp_course c
//INNER JOIN dpl_node n ON n.nid =  c.nid
//
//where c.is_project = 1 AND c.availability = 'S'
//AND
//(
//	n.nid IN
//	(
//		SELECT ac.project_nid FROM dpl_ntlp_course_activity ac
//		INNER JOIN dpl_node na ON na.nid = ac.project_nid
//		WHERE ac.project_nid = n.nid
//		AND na.changed BETWEEN UNIX_TIMESTAMP(DATE_SUB(CURRENT_DATE, INTERVAL 1 WEEK)) AND UNIX_TIMESTAMP()
//	)
//	OR
//	n.nid IN
//	(
//		SELECT cr.course_nid FROM dpl_ntlp_course_resource cr
//		INNER JOIN dpl_node nr ON nr.nid = cr.course_nid
//		WHERE cr.course_nid = n.nid
//		AND nr.changed BETWEEN UNIX_TIMESTAMP(DATE_SUB(CURRENT_DATE, INTERVAL 1 WEEK)) AND UNIX_TIMESTAMP()
//	)
//) and c.course_nid = $course_id");
    $dateformat = "%Y-%m-%d";
    $current_date = strftime($dateformat, time());

    $rs = db_query("SELECT nc.nid, n.title, nc.from_date, nc.to_date FROM dpl_ntlp_course nc
    INNER JOIN dpl_node n ON n.nid = nc.nid
    WHERE nc.is_project = 1 AND nc.availability = 'S' AND nc.course_nid = %d
    AND nc.from_date <= '%s' AND '%s' <= nc.to_date", $course_id, $current_date, $current_date);

    return $rs;
}

function get_project_activities_count($project_nid) {

    $result = db_query("SELECT COUNT(*) AS count_act FROM {ntlp_course_activity} WHERE project_nid=%d
        AND project_cat IS NOT NULL
        AND project_cat <> 0", $project_nid);

    if ($result->num_rows > 0) {
        if ($record = db_fetch_object($result)) {
            return $record->count_act;
        }
    }

    return 0;
}

function _get_resource_type_image($type) {
    global $base_path;
    $image = $base_path . 'themes/Boldr/Images/resourceStuff/';
    switch ($type) {
        case 'lo_document':
            $image .= 'img_document.png';
            break;
        case 'lo_image':
            $image .= 'img_picture.png';
            break;
        case 'lo_link':
            $image .= 'img_link.png';
            break;
        case 'lo_page':
            $image .= 'img_page.png';
            break;
        case 'lo_share_idea':
            $image .= 'img_idea.png';
            break;
        case 'lo_video':
            $image .= 'img_video.png';
            break;
        case 'lo_task':
            $image .= 'img_activity.png';
            break;
        case 'lo_journal':
            $image .= 'img_activity.png';
            break;
        case 'lo_rubric':
            $image .= 'img_activity.png';
            break;
        case 'lo_workshop':
            $image .= 'img_activity.png';
            break;
    }

    return $image;
}

function _get_project_course_id($project_id) {
    $result = db_query("SELECT c.course_nid from dpl_ntlp_course c where nid = %d", $project_id);

    if ($result) {
        return db_fetch_object($result)->course_nid;
    } else {
        false;
    }
}

function get_project_outcome_names_and_activities($project_id) {
    $sql = "SELECT td.tid, td.name, COUNT(*) AS activity_count
            FROM {ntlp_gb_activity_outcome} ao
            INNER JOIN {ntlp_course_activity} ca ON ao.activity_nid=ca.nid
            INNER JOIN {term_data} td ON td.tid=ao.tid
            WHERE ca.project_nid = $project_id
            GROUP BY ao.tid;";

    return db_query($sql);
}

function _get_project_id_by_resource($resource_nid) {
    $result = db_query("SELECT project_nid FROM dpl_ntlp_project_resource  WHERE nid = %d", $resource_nid);

    if ($result) {
        return db_fetch_object($result)->project_nid;
    } else {
        false;
    }
}

function get_course_projects($course_nid) {
    $course_projects = false;
    $result = db_query("SELECT c.nid, n.title FROM {ntlp_course} c
        INNER JOIN {node} n ON n.nid = c.nid
        WHERE c.course_nid = %d AND is_project = 1", $course_nid);

    if ($result->num_rows > 0) {
        while ($row = db_fetch_object($result)) {
            $course_projects[] = $row;
        }
    }

    return $course_projects;
}

function get_link_resource_url($resource_nid) {
    $path = false;
    $result = db_query("SELECT cr.path FROM {ntlp_course_resource} cr
        WHERE cr.nid = %d", $resource_nid);

    if ($result) {
        $data = db_fetch_object($result);
        $path = $data->path;
    }
    return $path;
}

function check_and_fix_resource_orders($resource_result_obj) {
    $fix_it = false;

    if (!empty($resource_result_obj)) {
        foreach ($resource_result_obj as $resource) {
            $project_nid = $resource->project_nid;

            $current_order = $resource->resource_order;
            $counter = 0;

            //Nested loop for checking a same order in different resource
            foreach ($resource_result_obj as $temp_record) {
                if ($counter < 2) {
                    if ($current_order == $temp_record->resource_order) {
                        $counter++;
                    }
                } else {
                    $fix_it = true;
                    break;
                }
            }
            // checking if resource_order found 0 or less then 0
            if ($resource->resource_order <= 0) {
                $fix_it = true;
                break;
            }
        }

        if (!$fix_it) {
            foreach ($resource_result_obj as $resource) {
                $fixit_order_total += $resource->orderfix;
                $resource_order_total += $resource->resource_order;
            }

            if ($fixit_order_total != $resource_order_total) {
                $fix_it = true;
            }
        }

        if ($fix_it) {
            fix_order($resource_result_obj);
        }
    }

    return $fix_it;
}

function get_template_library_projects() {
    $data = array();
    
    $result = db_query("SELECT lp.*, n.title FROM dpl_ntlp_library_project lp 
        INNER JOIN dpl_node n ON n.nid = lp.nid 
        WHERE lp.is_template = 1");
    
    while($record = db_fetch_object($result)){
        $data[] = $record;
    }
    
    return $data;
}