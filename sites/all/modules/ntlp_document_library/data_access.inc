<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


function count_library_document($school_nid, $section_nid) {

    $rs = db_query("SELECT count(nid) as total FROM {ntlp_project_resource}
        WHERE project_nid = %d AND section_tid = %d", $school_nid, $section_nid);

    $result = db_fetch_object($rs);

    $total = $result->total;
    return $total;
}

function get_lib_vocabulary_vid($str) {
    $result = db_query("SELECT * FROM {vocabulary} WHERE name = '%s'", $str);
    return db_fetch_object($result);
}

function clean_array($selected_oc_id) {
    $selected_oc_id = str_replace("x", "", $selected_oc_id);
    $selected_ids = explode(",", $selected_oc_id);
    $selected_ids = array_unique($selected_ids);
    foreach ($selected_ids as $key => $value) {
        if ($value == "") {
            unset($selected_ids[$key]);
        }
    }
    return $selected_ids;
}

function get_school_module($school_nid, $module_type) {

    $result = db_query("SELECT nid FROM {ntlp_module} WHERE school_nid = %d and type = '%s'", $school_nid, $module_type);
    return db_fetch_object($result);
}

function get_library_categories($school_nid, $module_nid) {
    $vid_name = "NTLP Library Category";

//    "select * from dpl_ntlp_library_category c, dpl_term_data t where module_nid = 1315 and c.category_tid = t.tid;

    $sql = "SELECT td.tid, REPLACE(td.name, '%2C', ',') as name FROM {ntlp_library_category} lc
        INNER JOIN {term_data} td on lc.category_tid = td.tid
        INNER JOIN {vocabulary} v on v.vid = td.vid
        WHERE lc.module_nid=%d and v.name='%s'
        ORDER BY td.name";

    $rs = db_query($sql, $module_nid, $vid_name);


    while ($note = db_fetch_array($rs)) {
        if (is_user_access_this_category($school_nid, $note['name'])) {
            $course_subjects[$note['tid']] = $note['name'];
        }
    }
    return $course_subjects;
}

function get_module_categories($module_nid) {

    $sql = "SELECT lc.*, td.tid, td.vid, REPLACE(td.name, '%2C', ',') as name FROM {ntlp_library_category} lc
        INNER JOIN {term_data} td on lc.category_tid = td.tid
        WHERE lc.module_nid=%d ORDER BY td.name ASC";

    $rs = db_query($sql, $module_nid);


    while ($note = db_fetch_object($rs)) {
        $module_categories[] = $note;
    }
    return $module_categories;
}

function _get_library_resource_detail($lr_nid) {
    $rs = db_query("SELECT n.title,n.promote, nr.body, lr.type, lr.* , lr.path, n.type as node_type, f.*
            FROM {ntlp_library_resource} lr
            INNER JOIN {node} n ON lr.nid = n.nid
            INNER JOIN {node_revisions} nr ON lr.nid = nr.nid
            LEFT JOIN {files} f ON f.fid = lr.fid
            WHERE lr.nid = %d", $lr_nid);

    $record = db_fetch_object($rs);
    return $record;
}

function _get_library_resource_category($resource_nid) {
    $rs = db_query("SELECT td.tid,td.name
            FROM {ntlp_library_resource} lc
            INNER JOIN {term_data} td on lc.category_tid=td.tid
            WHERE lc.nid = %d", $resource_nid);

    while ($record = db_fetch_object($rs)) {
        $cat_array[] = $record->tid;
    }
    return $cat_array;
}

function get_user_school_load($uid) {

    $result = db_query("SELECT * FROM {ntlp_user} WHERE uid =%d", $uid);
    return db_fetch_object($result);
}

function _get_module_detail($type, $school_nid = 0) {
    $result = db_query("SELECT * FROM {ntlp_module}
        WHERE type ='%s' AND school_nid = %d", $type, $school_nid);
    return db_fetch_object($result);
}

function create_module_detail($nid, $type, $school_nid) {
    db_query("INSERT INTO {ntlp_module} (nid, icon_fid, type, school_nid)
        VALUES (%d, 0, '%s', %d)", $nid, $type, $school_nid);
}

function get_doc_lib_record($school_nid) {


    $rs = db_query("SELECT n.title, nr.body, c.*
            FROM {node} n
            INNER JOIN {node_revisions} nr ON nr.nid = n.nid
            INNER JOIN {ntlp_module} c ON n.nid = c.nid
            WHERE c.school_nid = %d", $school_nid);

    if ($rs->num_rows > 0) {
        return db_fetch_object($rs);
    } else {
        return false;
    }
}

function _dpl_files_obj($fid) {

    $result = db_query("SELECT * FROM {files} WHERE fid = %d", $fid);

    if ($result->num_rows > 0) {
        return db_fetch_object($result);
    } else {
        return false;
    }
}

function _get_school_doc_lib_module() {
//NTLP Library Category
    db_query("SELECT n.*, nr.*, nm.*, lc.* FROM {ntlp_module} nm
        INNER JOIN {ntlp_library_category} lc ON lc.module_nid = nm.nid
        INNER JOIN {node} n ON n.nid = nm.nid
        INNER JOIN {node_revisions} nr ON nr.nid = nm.nid");
}

function _get_category_resrouces($category_id) {
//INNER JOIN {ntlp_library_resource} lr ON lr.nid = tn.nid
    $result = db_query("SELECT tn.*, n.*, nr.* FROM {term_node} tn
        INNER JOIN {node} n ON n.nid = tn.nid
        INNER JOIN {node_revisions} nr ON nr.nid = tn.nid
        WHERE tn.tid = %d AND n.type IN ('lo_image', 'lo_document', 'lo_video', 'lo_link', 'lo_page') ", $category_id);

    while ($record = db_fetch_object($result)) {
        $categories[] = $record;
    }

    return $categories;
}

function _get_categroy_by_resrouce_id($resource_nid) {
    $result = db_query("SELECT * FROM dpl_term_node tn, dpl_term_data td 
        WHERE tn.tid = td.tid AND tn.nid = %d AND td.vid= %d", $resource_nid, NTLP_LIABRARY_SCHOOL_DOCUMENT_CATEGORY);
    if ($result) {
        return db_fetch_object($result);
    } else {
        return false;
    }
}

function _get_category_resrouce_detail($resource_id) {

    $result = db_query("SELECT n.type as node_type, lr.type as resource_type, n.*, nr.*, lr.*, f.* FROM {ntlp_library_resource} lr
       INNER JOIN {node} n ON n.nid = lr.nid
       INNER JOIN {node_revisions} nr ON nr.nid = lr.nid
       LEFT JOIN {files} f ON f.fid = lr.fid
       WHERE lr.nid = %d", $resource_id);

    return db_fetch_object($result);
}

function get_lib_vocabulary_by_vid($resource_nid, $vocabulary) {
    $result = db_query("SELECT t.tid,t.name FROM {term_node} n
        LEFT JOIN {term_data} t on t.tid=n.tid
        LEFT JOIN {vocabulary} v on v.vid=t.vid
        WHERE n.nid=%d AND v.name = '%s'", $resource_nid, $vocabulary);
    while ($term = db_fetch_object($result)) {
        $terms[$term->tid] = $term->name;
    }
    return $terms;
}

function _get_lib_vocabulary_vid($str) {
    $result = db_query("SELECT * FROM {vocabulary} WHERE name = '%s'", $str);
    return db_fetch_object($result);
}

function get_lib_tid_id($category_tids) {

    foreach ($category_tids as $value) {

        $result = db_query("SELECT * FROM {term_data} WHERE tid=%d ", $value);

        while ($term = db_fetch_object($result)) {

            $incoming_terms = new stdClass();
            $incoming_terms->tid = $term->tid;
            $incoming_terms->vid = $term->vid;
            $incoming_terms->name = $term->name;
            $incoming_terms->description = $term->description;
            $incoming_terms->weight = 0;
            $cats[] = $incoming_terms;
        }
    }
//    watchdog('seted categories', print_r($cats, true));
    return $cats;
}

function get_lib_termids($incoming_terms, $vid) {

    if ($incoming_terms != null) {
        foreach ($incoming_terms as $term) {
            $query = db_query("SELECT tid FROM {term_data} WHERE name='%s' AND vid=%d", $term, $vid);
            $possible_term = db_fetch_object($query);

            //if the term doesn't already exist, add it to the cloud
            if ($query->num_rows == 0) {
                $new_term = array('vid' => $vid, 'name' => $term);
                if (taxonomy_save_term($new_term)) {

                    $incoming_terms = new stdClass();
                    $incoming_terms->tid = $new_term['tid'];
                    $incoming_terms->vid = $vid;
                    $incoming_terms->name = $term;
                    $incoming_terms->description = '';
                    $incoming_terms->weight = 0;

                    $terms_array[] = $incoming_terms;
                }
            } else {
                $incoming_terms = new stdClass();
                $incoming_terms->tid = $possible_term->tid;
                $incoming_terms->vid = $vid;
                $incoming_terms->name = $term;
                $incoming_terms->description = '';
                $incoming_terms->weight = 0;

                $terms_array[] = $incoming_terms;
            }
        }
    }

//    watchdog('terms_array',print_r($terms_array,true));
    return $terms_array;
}

function _get_school_by_doclib_category($category_tid) {

    $result = db_query("SELECT * FROM {ntlp_module} nm
        WHERE nm.nid = (SELECT module_nid FROM {ntlp_library_category}
        WHERE category_tid = %d)", $category_tid);

    return db_fetch_object($result);
}

function resoruce_attachment_data($node_type, $lib_resource_detail) {
    global $base_path;

    if ($node_type == 'lo_image') {
        if ($lib_resource_detail) {
            if ($lib_resource_detail->type == "U") {
                if ($lib_resource_detail->fid != 0) {
                    $file_obj = _dpl_files_obj($lib_resource_detail->fid);
                    if ($file_obj) {
                        $resource_attachment = $base_path . $file_obj->filepath;
                    } else {
                        $resource_attachment = $base_path . 'themes/Boldr/Images/ProjectsImages/ImageNotAvailable.jpg';
                    }
                } else {
                    $resource_attachment = $base_path . 'themes/Boldr/Images/ProjectsImages/ImageNotAvailable.jpg';
                }
            } else if ($lib_resource_detail->type == "L") {
                $resource_attachment = $lib_resource_detail->path;
            }
        }
    } else if ($node_type == 'lo_link') {

//        $resource_attachment = '<a href="'.$lib_resource_detail->path.'"> Attached Link </a>';
    } else if ($node_type == 'lo_page') {
//        print_r($lib_resource_detail);
        if ($lib_resource_detail) {
            $resource_attachment = $lib_resource_detail->path;
        }
    } else if ($node_type == 'lo_document') {
        if ($lib_resource_detail) {
            if ($lib_resource_detail->type == "U") {
                if ($lib_resource_detail->fid != 0) {
                    $file_obj = _dpl_files_obj($lib_resource_detail->fid);
                    if ($file_obj) {
                        $resource_attachment = '<a target="_blank" href="' . $base_path . $file_obj->filepath . '"> Document Attached</a>';
                    } else {
                        $resource_attachment = '<img src="' . $base_path . 'themes/Boldr/Images/ProjectsImages/ImageNotAvailable.jpg" width="115">';
                    }
                } else {
                    $resource_attachment = '<img src="' . $base_path . 'themes/Boldr/Images/ProjectsImages/ImageNotAvailable.jpg" width="115">';
                }
            } else if ($lib_resource_detail->type == "L") {
                $resource_attachment = '<a href="' . $lib_resource_detail->path . '"> Document Link</a>';
            }
        }
    } else if ($node_type == 'lo_video') {
        if ($lib_resource_detail) {
            if ($lib_resource_detail->type == "U") {
                if ($lib_resource_detail->fid != 0) {
                    $file_obj = _dpl_files_obj($lib_resource_detail->fid);
                    if ($file_obj) {
                        $resource_attachment = $base_path . $file_obj->filepath;
                    } else {
                        $resource_attachment = $base_path . 'themes/Boldr/Images/ProjectsImages/ImageNotAvailable.jpg';
                    }
                } else {
                    $resource_attachment = $base_path . 'themes/Boldr/Images/ProjectsImages/ImageNotAvailable.jpg';
                }
            } else if ($lib_resource_detail->type == "L") {
                $resource_attachment = $lib_resource_detail->path;
            }
        }
    }
    return $resource_attachment;
}

function get_resource_details($school_nid, $opts = array(), $module_type = 'ntlp_school_document_library') {

    $args = array();
    $where = '';
    // Default options
    $defaultOpts = array(
        'OFFSET' => 0,
        'LIMIT' => 5,
        'COUNT' => false,
        'WITH_TITLE' => false,
        'ORDER' => array('FIELD' => '', 'SORT' => 'ASC'),
        'BY_CATEGORY' => '',
    );
    // Overridding the default options
    $opts = array_merge($defaultOpts, $opts);

    // Extra Where Clause
    if ($school_nid) {
        $where = ' AND nm.school_nid = %d ';
        $args[] = $school_nid;
    }

    if ($opts['BY_CATEGORY']) {
        $where = ' AND lc.category_tid = ' . ((int) $opts['BY_CATEGORY']);
    }

    // ORDER BY clause
    $orderBy = '';
    if ($opts['ORDER']['FIELD'] && in_array(strtoupper($opts['ORDER']['SORT']), array('ASC', 'DESC'))) {
        $orderBy = " ORDER BY {$opts['ORDER']['FIELD']} {$opts['ORDER']['SORT']}";
    }

    // SELECT Fields
    $select = '*';
    if ($opts['COUNT']) {
        $select = 'COUNT(*) AS total';
    } else if ($opts['WITH_TITLE']) {
        $select = 'n.nid, n.type, lc.category_tid, REPLACE(td.name, "%2C", ",") as name, n.title, n.created, n.changed';
    }

    // Extra INNER JOIN
    $subInnerJoin = '';
    if ($opts['WITH_TITLE']) {
        $subInnerJoin = 'INNER JOIN
                            {node} n
                            ON
                            n.nid = lr.nid';
    }


    $sql = "SELECT
            $select
            FROM
                {ntlp_module} nm

                INNER JOIN
                {ntlp_library_category} lc
                ON
                lc.module_nid = nm.nid

                INNER JOIN
                {term_node} tn
                ON
                tn.tid = lc.category_tid

                INNER JOIN
                {term_data} td
                ON
                td.tid = lc.category_tid
    
                INNER JOIN
                {ntlp_library_resource} lr
                ON
                lr.nid = tn.nid

            $subInnerJoin

            WHERE
                    nm.type = '$module_type'
                    AND ISNULL(lr.deleted_on)

            $where
            $orderBy

            LIMIT
                %d, %d
            ";

    watchdog('lib_doc', 'Popular resources SQL:' . $sql);
    $recs = array();
    $args[] = $opts['OFFSET'];
    $args[] = $opts['LIMIT'];

    if ($result = db_query($sql, $args)) {
        if ($opts['COUNT']) {
            $recs = db_fetch_object($result);
        } else {
            while ($rec = db_fetch_object($result)) {
                $recs[] = $rec;
            }
        }
    }

    return $recs;
}

function _school_resourse_terms($school_id, $module_type = 'ntlp_school_document_library') {
    $sql = "SELECT td.tid, td.vid, REPLACE(td.name, '%2C', ',') as name
            FROM  {ntlp_module} nm
                INNER JOIN {ntlp_library_category} lc ON lc.module_nid = nm.nid
                INNER JOIN {term_data} td ON td.tid = lc.category_tid
            WHERE nm.type = '$module_type' AND nm.school_nid = %d";

    $rows = array();
    if ($result = db_query($sql, $school_id)) {
        while ($row = db_fetch_object($result)) {
            $rows[] = $row;
        }
    }

    return $rows;
}

function _get_school_resource_library_keywords($limit = false) {
    $sql = ' SELECT td.tid, td.name, td.vid FROM {ntlp_project_resource} nlr
                INNER JOIN dpl_term_node tn ON tn.nid = nlr.nid
                INNER JOIN dpl_term_data td ON td.tid = tn.tid
                WHERE td.vid = (SELECT  vid FROM dpl_vocabulary WHERE name = \'NTLP Posts\')
             ' . ($limit ? 'LIMIT 0,' . (int) $limit : '');

    $rows = array();
    if ($result = db_query($sql)) {
        while ($row = db_fetch_object($result)) {
            $rows[] = $row;
        }
    }

    return $rows;
}

function get_lib_resource_catid($category_tid) {
    $rs = db_query("SELECT *
                    FROM dpl_ntlp_library_category c
                      INNER JOIN dpl_ntlp_module m
                        ON c.module_nid = m.nid
                    WHERE category_tid = %d", $category_tid);

    return db_fetch_object($rs);
}

function is_user_access_this_category($school_nid, $category_name) {
    global $user;
    $flag = false;
    $category_name = str_replace(",", '%2C', $category_name);
    $user_permission = $school_nid . ' ' . $category_name;
    if (user_access($school_nid . ' ' . strtolower($category_name)) || isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])
            || isset($user->roles[NTLP_ROLEID_NETWORKSTAFF]) || isset($user->roles[NTLP_ROLEID_SCHOOLLEADER])
            || isset($user->roles[NTLP_ROLEID_SITEADMIN])) {

        $flag = true;
    }
    return $flag;
}

function is_document_library_resource_exist($resource_nid) {
    $flag = false;

    $result = db_query("SELECT nid FROM {ntlp_library_resource} WHERE deleted_on is NULL AND nid = %d", $resource_nid);

    if ($result->num_rows > 0) {
        $flag = true;
    }

    return $flag;
}
