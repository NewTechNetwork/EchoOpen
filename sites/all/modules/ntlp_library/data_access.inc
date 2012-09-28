<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


function get_lib_module($module_nid) {

    watchdog("ntlp_module", '$module nid ' . $module_nid);


    $rs = db_query("SELECT n.title, nr.body, c.*
            FROM {node} n
            INNER JOIN {node_revisions} nr ON nr.nid = n.nid
            INNER JOIN {ntlp_module} c ON n.nid = c.nid
            WHERE c.nid = %d", $module_nid);

    watchdog("ntlp_module", 'reslult ' . print_r($rs, true));

    if ($rs->num_rows > 0) {
        return db_fetch_object($rs);
    } else {
        return false;
    }
}

function _get_dpl_files_obj($fid) {

    $result = db_query("SELECT * FROM {files} WHERE fid = %d", $fid);

    if ($result->num_rows > 0) {
        return db_fetch_object($result);
    } else {
        return false;
    }
}

function _get_library_project($project_nid) {
    $res = db_query("SELECT * FROM {ntlp_library_project} lp
        INNER JOIN {node} n ON n.nid = lp.nid
        INNER JOIN {node_revisions} nr ON nr.nid = lp.nid
        INNER JOIN {ntlp_course} c ON c.nid = lp.nid
        WHERE lp.nid =%d ", $project_nid);
    return $result = db_fetch_object($res);
}

function is_project_designer($uid, $project_id) {
    $flag = false;


    $result = db_query("SELECT * FROM {ntlp_library_project} WHERE nid = %d AND submitted_by = %d", $project_id, $uid);

    if ($result->num_rows > 0) {
        $flag = true;
    } else {
        $flag = false;
    }

    return $flag;
}

function is_project_archived($project_id) {

    $result = db_query("SELECT * FROM {ntlp_library_project}
        WHERE nid=%d AND archived_on != '0000-00-00 00:00:00'", $project_id);

    if ($result->num_rows > 0) {
        return true;
    } else {
        return false;
    }
}

function _get_project_copied_count($project_nid) {

    $count = db_result(db_query("SELECT COUNT(*) FROM {ntlp_library_project_usage}
        WHERE action = 'C' AND src_project =%d", $project_nid));

    return $count;
}

function _get_community_favorites($project_id) {

    return $result = db_query("SELECT n.title,count(f.content_id) as count,f.content_id
        FROM {flag_content} f
        left join {node} n on n.nid=f.content_id
        group by f.content_id order by count desc limit 5");
}

function _get_recent_additions() {

    return $result = db_query("
        SELECT n.title,d.nid,d.submitted_on FROM {ntlp_library_project} d
        left join {node} n on n.nid=d.nid
        order by submitted_on desc LIMIT 5");
}

function _get_most_active_schools() {

    $result = db_query("SELECT school_nid,count(school_nid) as count from {ntlp_library_project} l
            INNER JOIN {ntlp_course} c on l.src_project_nid = c.nid where school_nid!=0 
            GROUP BY school_nid order by count desc");
    return $row = db_fetch_array($result);
}

function _get_most_active_contributor() {

    $result = db_query("
        SELECT p.submitted_by,count(p.submitted_by) as count,u.first_name,u.last_name,u.uid
        FROM {ntlp_library_project} p
        left join {ntlp_user} u on p.submitted_by=u.uid
        GROUP BY p.submitted_by order by count desc");
    return $row = db_fetch_array($result);
}

function _get_most_copied_project() {

    $result = db_query("SELECT s.src_project,count(s.src_project) as count,n.title
            FROM {ntlp_library_project_usage} s
            left join {node} n on n.nid=s.src_project
            where s.action = 'C'
            GROUP BY s.src_project order by count desc");
    return $row = db_fetch_object($result);
}

function _get_n_most_copied_projects($n = 0) {

    // saifuddin don't know whey we using condition.. so I just removed it.
    //   $project_briefcase_template_exclusion = " AND s.src_project <> 37060 ";

    $sql = "SELECT s.src_project,count(s.src_project) as count,n.title
            FROM {ntlp_library_project_usage} s
            left join {node} n on n.nid=s.src_project
            INNER JOIN {ntlp_library_project} lp ON lp.nid = s.src_project
            where s.action = 'C' AND lp.is_deleted <> 1 AND lp.archived_by = 0 
            GROUP BY s.src_project order by count desc";

    if ($n > 0)
        $sql .= " LIMIT $n";

    return db_query($sql);
}

function get_comments_user_detail($user_uid) {

    $result = db_query("SELECT * FROM dpl_users us
        LEFT JOIN dpl_ntlp_user u on us.uid=u.uid
        WHERE us.uid = %d", $user_uid);

    return db_fetch_object($result);
}

function get_total_projects() {

    $rs = db_query("SELECT count(nid) as total FROM {ntlp_library_project}");


    return db_fetch_object($rs);
}

function get_exemplary_total_projects($is_exemplary = 1) {

    $rs = db_query("SELECT count(nid) as total FROM {ntlp_library_project} WHERE is_exemplary = %d", $is_exemplary);

    return db_fetch_object($rs);
}

function _get_course_nid($nid) {

    $rs = db_query("SELECT course_nid FROM {ntlp_course} WHERE nid = %d", $nid);

    return db_fetch_object($rs);
}

/* Functions added by Muhammad Mahad Azad */

/*
 * The following function can be used as:
 *      _date_range_helper( $fieldName, array( 'THIS_MONTH' => true ) )
 *      _date_range_helper( $fieldName, array( 'DATE' => array('YEAR' => 2010, 'MONTH' => 4, 'DAY' => 10)) )
 *
 * @param $field
 * @param [$filter = array('THIS_MONTH'=>true)]
 * @param $isStamp
 *
 * @return string
 */

function _date_range_helper($field, $filter = array('THIS_MONTH' => true), $isStamp = false) {
    $where = ' 1=1 ';
    $field = $isStamp ? "FROM_UNIXTIME($field)" : $field;

    if (isset($filter['DATE'], $filter['DATE']['YEAR'], $filter['DATE']['MONTH'], $filter['DATE']['DAY'])) {
        $unixtimestamp = @mktime(0, 0, 0, $filter['DATE']['MONTH'], $filter['DATE']['DAY'], $filter['DATE']['YEAR']);
        $where = " UNIX_TIMESTAMP(DATE($field))=$unixtimestamp ";
    } else if (isset($filter['THIS_MONTH']) and $filter['THIS_MONTH']) {
        $where = " YEAR($field)=YEAR(NOW()) AND MONTH($field)=MONTH(NOW()) ";
    }

    return $where;
}

function get_likes_count_by_date($average = false, $filter = array('THIS_MONTH' => true)) {
    $where = _date_range_helper('timestamp', $filter, true);
    $groupBy = $average ? "GROUP BY MONTH(FROM_UNIXTIME(timestamp))" : '';

    $sql = "SELECT
                COUNT(*) as total
                FROM
                        (SELECT fid FROM dpl_flags WHERE dpl_flags.name = 'likethispost') likeTbl
                        INNER JOIN
                        dpl_flag_content
                        ON
                        dpl_flag_content.fid = likeTbl.fid

                        INNER JOIN
                        dpl_ntlp_library_project
                        ON
                        dpl_ntlp_library_project.nid = dpl_flag_content.content_id
                WHERE
                        content_type = 'node'
                AND $where
                $groupBy
                ";

    $rs = db_query($sql);

    $total = 0;
    $i = 0;
    while ($val = db_fetch_object($rs)) {
        $total += $val->total;
        $i++;
    }

    if ($i) {
        return $total / $i;
    }

    return 0;
}

function get_copies_count($average = false, $filter = array('THIS_MONTH' => true)) {
    $where = _date_range_helper('action_on', $filter);
    $groupBy = $average ? "GROUP BY MONTH(action_on)" : '';

    $sql = "SELECT
                COUNT(*) AS total
            FROM
                dpl_ntlp_library_project
                INNER JOIN
                dpl_ntlp_library_project_usage
                ON dpl_ntlp_library_project_usage.src_project = dpl_ntlp_library_project.nid
            WHERE
                action = 'C'
                AND $where
                $groupBy
            ";

    $rs = db_query($sql);

    $total = 0;
    $i = 0;

    while ($val = db_fetch_object($rs)) {
        $total += $val->total;
        $i++;
    }

    if ($i) {
        return $total / $i;
    }

    return 0;
}

function get_comments_count($average = false, $filter = array('THIS_MONTH' => true)) {
    $where = _date_range_helper('timestamp', $filter, true);
    $groupBy = $average ? "GROUP BY MONTH(FROM_UNIXTIME(timestamp))" : '';

    $sql = "SELECT
                COUNT(*) AS total
            FROM
                dpl_ntlp_library_project
                INNER JOIN
                dpl_comments
                ON dpl_ntlp_library_project.nid = dpl_comments.nid
             WHERE
                $where
                $groupBy
            ";

    $rs = db_query($sql);

    $total = 0;
    $i = 0;
    while ($val = db_fetch_object($rs)) {
        $total += $val->total;
        $i++;
    }

    if ($i) {
        return $total / $i;
    }

    return 0;
}

function get_reviews_needed_count() {
    $sql = "SELECT COUNT(*) AS total FROM dpl_ntlp_library_project lp
            INNER JOIN dpl_node n ON lp.nid = n.nid
            INNER JOIN dpl_node_revisions nr ON lp.nid = nr.nid
            WHERE lp.reviewer_comment_cid = 0 ";

    $rs = db_query($sql);
    if ($val = db_fetch_object($rs)) {
        return $val->total;
    }

    return 0;
}

function get_archive_request_needed_count() {
    $sql = "SELECT COUNT(*) AS total  FROM {ntlp_library_project} lp
        INNER JOIN {node} n ON lp.nid = n.nid
        INNER JOIN {node_revisions} nr ON lp.nid = nr.nid
        WHERE lp.archive_request_on != '0000-00-00 00:00:00' AND lp.archived_on = '0000-00-00 00:00:00' ";

    $rs = db_query($sql);
    if ($val = db_fetch_object($rs)) {
        return $val->total;
    }

    return 0;
}

function _get_recent_comments($table_name = 'ntlp_library_project', $limit = 3) {
    $limit = (int) $limit;

    if (!in_array($table_name, array('ntlp_library_project'))) {
        return false;
    }
    $table_name = '{' . $table_name . '}';

    $sql = "SELECT
                n.nid, n.type, n.title, c.cid, c.uid comment_by, c.comment , c.name, nu.first_name, nu.last_name, u.picture, c.homepage, c.timestamp
            FROM
                {node} n

            INNER JOIN {comments} c ON c.nid = n.nid
            LEFT JOIN {users} u ON u.uid = c.uid
            LEFT JOIN {ntlp_user} nu ON nu.uid = u.uid

            WHERE
                n.nid IN( SELECT nlp.nid FROM {$table_name} nlp )

            ORDER BY
                c.timestamp DESC

            LIMIT $limit
            ";

    $recs = array();
    if ($result = db_query($sql)) {
        while ($row = db_fetch_object($result)) {
            $recs[] = $row;
        }
    }

    return $recs;
}

function get_parent_subject($subject_tid) {

    $result = db_query("SELECT td.name as child, ptd.name as parent
        FROM {term_data} td
        LEFT JOIN {term_hierarchy} th on td.tid = th.tid
        LEFT JOIN {term_data} ptd on th.parent = ptd.tid
        WHERE td.tid = %d", $subject_tid);

    $data = db_fetch_object($result);

    if ($data->parent != null) {
        return $data->parent;
    } else {
        return false;
    }
}

function _get_library_projects_taxonomies($lib_project_nid) {
    $sql = "SELECT t.* FROM dpl_term_node r
        INNER JOIN dpl_term_data t ON r.tid = t.tid
        INNER JOIN dpl_vocabulary v ON t.vid = v.vid
        WHERE r.nid = %d ORDER BY t.vid";

    $rows = array();
    if ($result = db_query($sql, $lib_project_nid)) {
        while ($row = db_fetch_object($result)) {
            $rows[] = $row;
        }
    }

    return $rows;
}

function _get_project_library_keywords($school_nid, $limit = false) {
    $sql = ' SELECT
                td.tid, td.vid, td.name
            FROM
                dpl_ntlp_library_project nlp
                INNER JOIN dpl_term_node tn ON tn.nid = nlp.nid
                INNER JOIN dpl_term_data td ON td.tid = tn.tid
                INNER JOIN dpl_ntlp_course nc ON nc.nid = nlp.nid
            WHERE
                    nc.school_nid = %d
                AND td.vid = (SELECT vid FROM dpl_vocabulary WHERE name = \'NTLP Posts\')
            ORDER BY
                td.tid DESC
             ' . ($limit ? 'LIMIT 0,' . (int) $limit : '');

    $rows = array();
    if ($result = db_query($sql, $school_nid)) {
        while ($row = db_fetch_object($result)) {
            $rows[] = $row;
        }
    }

    return $rows;
}

function get_lib_project_activity_outcome($activity_id) {

    $result = db_query("SELECT td.tid, td.name, oc.max_points  FROM {ntlp_course_activity} act
        INNER JOIN {ntlp_gb_activity_outcome} oc ON act.nid = oc.activity_nid
        INNER JOIN {term_data} td ON oc.tid = td.tid
        WHERE act.nid = %d", $activity_id);

    while ($obj = db_fetch_object($result)) {
        $outcomearray[] = $obj;
    }
    return $outcomearray;
}

function _get_project_resource_type($nid) {

    $result = db_query("SELECT type FROM dpl_node WHERE nid = %d", $nid);

    if ($result) {
        $data = db_fetch_object($result);

        $resource_type = $data->type;
    }

    if ($resource_type == 'lo_journal' || $resource_type == 'lo_task'
            || $resource_type == 'lo_workshop' || $resource_type == 'lo_rubric') {
        return 'Activity';
    } else if ($resource_type == 'lo_image' || $resource_type == 'lo_video'
            || $resource_type == 'lo_link' || $resource_type == 'lo_page'
            || $resource_type == 'lo_document' || $resource_type == 'lo_share_idea') {
        return 'Resource';
    }
}

function get_project_submitted_by_name($project_id) {
    $result = db_query("SELECT u.first_name,u.last_name, u.uid FROM {ntlp_user} u  WHERE u.uid IN (
        SELECT submitted_by  FROM {ntlp_library_project} WHERE nid = %d 
        )    ORDER BY u.last_name, u.first_name", $project_id);

    if ($result) {
        return db_fetch_object($result);
    }
    else
        return null;
}

function get_ntlp_project_library_image_path($picture_fid, $is_examplary) {
    global $base_path;

    $image_path = $base_path . 'themes/Boldr/Images//default/shared_project_generic.jpg';

    if ($picture_fid != null && $picture_fid != 0) {

        $file_obj = get_file_detail($picture_fid);
        $file_path = $file_obj->filepath;

        if (file_exists($_SERVER['DOCUMENT_ROOT'] . $base_path . $file_path)) {
            $image_path = $base_path . $file_path;
        } else {
            if ($is_examplary == 1) {
                $image_path = $base_path . 'themes/Boldr/Images/default/exemplary_project_generic.jpg';
            }
        }
    } else {
        if ($is_examplary == 1) {
            $image_path = $base_path . 'themes/Boldr/Images/default/exemplary_project_generic.jpg';
        }
    }

    return $image_path;
}