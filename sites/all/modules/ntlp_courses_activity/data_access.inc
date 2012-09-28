<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'gapps_feeds', 'docs.contents.inc');
module_load_include('inc', 'ntlp_school', 'data_access');

function outcomes_fetching($course_id) {

    $result = db_query("select tid from {ntlp_gb_course_outcome} WHERE course_nid = %d", $course_id);

    while ($note = db_fetch_array($result)) {

        $outcome[] = $note;
    }

    return $outcome;
}

function get_activity_outcome($activity_id) {

//    $result =  db_query("SELECT td.tid, td.name, oc.max_points  FROM {ntlp_course_activity} act
//        INNER JOIN {ntlp_gb_activity_outcome} oc ON act.nid = oc.activity_nid
//        INNER JOIN {term_data} td ON oc.tid = td.tid
//        LEFT JOIN {ntlp_gb_course_outcome} outc ON outc.tid = td.tid
//        WHERE act.nid = %d ", $activity_id);

    $result = db_query("SELECT td.tid, td.name, oc.max_points  FROM {ntlp_course_activity} act
        INNER JOIN {ntlp_gb_activity_outcome} oc ON act.nid = oc.activity_nid
        INNER JOIN {term_data} td ON oc.tid = td.tid
        INNER JOIN {ntlp_gb_course_outcome} outc ON outc.tid = td.tid AND outc.course_nid = act.course_nid
        WHERE act.nid = %d ORDER BY outc.sort_order", $activity_id);

    while ($obj = db_fetch_object($result)) {
        $outcomearray[] = $obj;
    }
    return $outcomearray;
}

#  Get All Course Outcomes Activity Edit

function get_all_outcomes($course_nid) {
    $result = db_query("select act.tid, td.name  from {ntlp_gb_course_outcome} act
                        INNER JOIN {term_data} td ON act.tid = td.tid WHERE course_nid =%d
                        ORDER BY act.sort_order", $course_nid);

    while ($note = db_fetch_object($result)) {

        $arr[] = $note;
    }

    return $arr;
}

function get_rubric_outcome_name($course_nid) {
    $result = db_query("select act.tid,td.name  from {ntlp_gb_course_outcome} act
                        INNER JOIN {term_data} td ON act.tid = td.tid WHERE course_nid =%d
                        ORDER BY act.sort_order", $course_nid);

    while ($note = db_fetch_object($result)) {

        $arr[$note->tid] = $note->name;
    }

    return $arr;
}

function get_rubric_activity($nid) {
    return db_fetch_object(db_query('SELECT * FROM {ntlp_course_activity_rubric} WHERE activity_nid = %d LIMIT 1', $nid));
}

# Get Selected Activity Outcomes For Activity Edit

function get_selected_outcomes_tid($activity_id) {

    $result = db_query("SELECT tid FROM {ntlp_gb_activity_outcome} WHERE activity_nid=%d", $activity_id);

    while ($note = db_fetch_object($result)) {

        $arr[] = $note->tid;
    }
    return $arr;
}

# Get Selected max Points For Activity Edit

function get_selected_max_points($tid, $activity_nid) {

    $result = db_query("SELECT max_points FROM {ntlp_gb_activity_outcome} WHERE tid=%d AND activity_nid = %d", $tid, $activity_nid);


    return db_fetch_object($result);
}

function check_outcomes($activity_id) {

    $result = db_query('SELECT activity_nid FROM {ntlp_gb_activity_outcome} WHERE activity_nid =%d', $activity_id);

    $note = db_fetch_object($result);

    if (isset($note)) {

        return true;
    } else {

        return false;
    }
}


function get_activity_type($activity_id) {

    $result = db_query("SELECT act.nid, n.type, act.course_nid
        FROM {ntlp_course_activity} act
        INNER JOIN {node} n ON act.nid = n.nid
        WHERE act.nid=%d", $activity_id);


    return db_fetch_object($result);
}


function get_activity_record($activity_nid) {

    $result = db_query("SELECT * FROM {ntlp_course_activity} WHERE nid = %d", $activity_nid);

    if ($result->num_rows) {
        $row = db_fetch_object($result);
        return $row;
    }

    return false;
}

function _get_resource_course_nid($res_nid) {
    return db_result(db_query('SELECT course_nid FROM {ntlp_course_resource} WHERE nid = %d', $res_nid));
}

//function unit_autocomplete($arg, $string) {
//    $matches = array();
//    $unit_vid  = get_vocablary_vid("NTLP Course Unit");
//
//    //watchdog('info', 'cehckig string'.$string);
//
//    $sql = t("SELECT t.* FROM dpl_term_node r
//            INNER JOIN dpl_term_data t ON r.tid = t.tid
//            INNER JOIN dpl_vocabulary v ON t.vid = v.vid WHERE r.vid = ".$unit_vid->vid." AND  r.nid = $arg  and t.name LIKE '%".$string."%'");
//
//    $res = db_query($sql);
//
//    while ($row = db_fetch_array($res)) {
//        $matches[$row['name']]= $row['name'];
//    }
//    print drupal_json($matches);
//}
function unit_autocomplete($course_nid, $string) {

    $matches = array();
//    $sql = t("SELECT id, name FROM {ntlp_course_unit} WHERE course_id = " . $arg . " AND name LIKE '%" . $string . "%'");
//    $sql = "SELECT td.tid, td.name FROM {term_node} tn
//        INNER JOIN {term_data} td ON td.tid = tn.tid
//        WHERE tn.nid = ".$arg." AND td.name LIKE '%" . $string . "%'";
//    >> For debugging
//    $sql = "SELECT ts.tid, t.name, c.nid, ta.tid, tda.name, v.vid, v.name
    $sql = "SELECT tda.tid, tda.name
        FROM dpl_term_node ts
        INNER JOIN dpl_term_data t ON t.tid = ts.tid
        INNER JOIN dpl_term_node tc ON tc.tid = ts.tid
        INNER JOIN dpl_ntlp_course c ON tc.nid = c.nid
        %s
        INNER JOIN dpl_term_data tda ON tda.tid = ta.tid
        INNER JOIN dpl_vocabulary v ON tda.vid = v.vid
        WHERE ts.nid = %d /*CURRENT COURSE*/ 
        AND t.vid = 8 /*SUBJECT VOCABULARY*/
        AND tda.vid = 22 /*CURRICULUM UNIT*/
        AND tda.name LIKE '%s'
        GROUP BY c.nid, ta.tid";

    //Get topics from activity's association
    $asql = " INNER JOIN dpl_ntlp_course_activity a ON a.course_nid = c.nid INNER JOIN dpl_term_node ta ON a.curriculum_unit_tid = ta.tid ";
    $res = db_query($sql, $asql, $course_nid, '%' . $string . '%');

    if ($res->num_rows > 0) {
        while ($row = db_fetch_array($res)) {
            if (!empty($row['name'])) {
                $matches[$row['name']] = $row['name'];
            }
        }
    }
    
    //Get topics from project's association
    $psql = " INNER JOIN dpl_ntlp_course p ON p.course_nid = c.nid AND p.is_project = 1 INNER JOIN dpl_term_node ta ON p.topic_unit_tid = ta.tid ";
    $res = db_query($sql, $psql, $course_nid, '%' . $string . '%');

    if ($res->num_rows > 0) {
        while ($row = db_fetch_array($res)) {
            if (!empty($row['name'])) {
                $matches[$row['name']] = $row['name'];
            }
        }
    }
    
    if (count($matches) > 0) {
        asort($matches);
        print drupal_json($matches);
    } else {
        print drupal_json('');
    }
}

function _get_load_project_byId($project_id) {

    $projectoption = array(0 => "None");
//    $rs = db_query("SELECT nid, name FROM {ntlp_course} WHERE is_project = 'YES' AND course_id = %d", $course_id);
    $rs = db_query("SELECT c.nid, n.title FROM {ntlp_course} c INNER JOIN {node} n ON
         c.nid = n.nid WHERE c.nid = %d AND is_project =1", $project_id);

    if ($rs) {
        $note = db_fetch_array($rs);
        $projectoption['nid'] = $note['nid'];
        $projectoption['title'] = $note['title'];
        return $projectoption;
    } else {
        return false;
    }
}

function _get_load_projects($course_id) {
    global $user;

    if ($user->roles[NTLP_ROLEID_STUDENT]) {
        $_WHERE = " AND availability = 'S' ";
    } else {
        $_WHERE = "";
    }

    $projectoption = array(0 => "General Purpose");
    $projectoption['---'] = '-----------';

    $rs = db_query("SELECT c.nid, n.title FROM {ntlp_course} c INNER JOIN {node} n ON c.nid = n.nid WHERE course_nid = %d AND is_project=1 " . $_WHERE, $course_id);

    $cno = 0;
    while ($note = db_fetch_array($rs)) {
        $projectoption[$note['nid']] = $note['title'];
        $cno = $note['nid'];
    }

    return $projectoption;
}

function _get_course_section($project_id) {

    $rs = db_query("SELECT c.tid, td.name FROM {ntlp_course_section} c
        INNER JOIN {term_data} td ON c.tid = td.tid
        WHERE course_nid = %d ", $project_id);

    if ($rs) {
        while ($note = db_fetch_array($rs)) {
            $sectionoption[$note['tid']] = $note['name'];
        }
        return $sectionoption;
    } else {
        return false;
    }
}

function _get_selected_projects($projectid) {

    //   $projectoption = array(0=>"None");
//    $rs = db_query("SELECT nid, name FROM {ntlp_course} WHERE is_project = 'YES' AND course_id = %d", $course_id);
    $rs = db_query("SELECT n.nid, n.title FROM  {node} n WHERE n.nid = %d", $projectid);

    return db_fetch_object($rs);
}

function get_nodeid_from_url($str) {

    if (strpos($str, "/") != 0) {
        $var = substr($str, strpos($str, "/") + 1, strlen($str));
    } else {
        return $str;
    }
    return get_nodeid_from_url($var);
}

function is_activity_exist($activity_id) {

    $result = db_query("SELECT * FROM {ntlp_course} where nid = %d", $course_id);

    if (db_fetch_object($result)) {
        return true;
    } else {
        return false;
    }
}

function is_course_exist($course_id) {
    $flag = false;
    $result = db_query("SELECT * FROM {ntlp_course} where nid = %d", $course_id);

    if ($result->num_rows > 0) {
        $flag = true;
    } else {
        $flag = false;
    }
    return $flag;
}

function get_course_activities($course_id) {

    $result = db_query("SELECT act.nid, n.title, r.body
        FROM {ntlp_course_activity} act
        INNER JOIN {node} n ON act.nid = n.nid
        INNER JOIN {node_revisions} r ON act.nid =r.nid
        WHERE act.course_nid = %d", $course_id);

    while ($obj = db_fetch_object($result)) {
        $activityarray[] = $obj;
    }
    return $activityarray;
}

function get_school_term_name($tid) {

    $result = db_query("SELECT name FROM {term_data} WHERE tid =%d", $tid);

    return db_fetch_object($result);
}

function get_activity_records($activity_id, $lite_version = false) {

    if ($lite_version) {
        $result = db_query("SELECT act.*, n.type, n.title
            FROM {ntlp_course_activity} act
            INNER JOIN {node} n ON act.nid = n.nid
            WHERE act.nid=%d ", $activity_id);
    } else {
        $result = db_query("SELECT act.*, n.type, u.name ,nr.title, nr.body
            FROM {ntlp_course_activity} act
            INNER JOIN {node_revisions} nr ON nr.nid = act.nid
            LEFT JOIN {term_data} u ON u.tid = act.curriculum_unit_tid
            INNER JOIN {node} n ON act.nid = n.nid
            WHERE act.nid=%d ", $activity_id);
    }
    return db_fetch_object($result);
}

function get_rubric_activity_records($activity_id) {

    $result = db_query("SELECT act.*,actr.*, n.type, u.name ,nr.title, nr.body
        FROM {ntlp_course_activity} act
        INNER JOIN {ntlp_course_activity_rubric} actr ON actr.activity_nid = act.nid
        INNER JOIN {node_revisions} nr ON nr.nid = act.nid
        LEFT JOIN {term_data} u ON u.tid = act.curriculum_unit_tid
        INNER JOIN {node} n ON act.nid = n.nid
        WHERE act.nid=%d ", $activity_id);

    return db_fetch_object($result);
}

function _load_course_outcomes($course_id) {

    $result = db_query('SELECT co.tid, td.name, co.weight_percent, co.custom_name FROM {ntlp_gb_course_outcome} co
            INNER JOIN {term_data} td ON td.tid = co.tid WHERE co.course_nid = %d
            ORDER BY co.sort_order', $course_id);

    while ($note = db_fetch_object($result)) {

        $activityoutcome[] = $note;
    }

    return $activityoutcome;
}


function _get_load_activity_type() {

    $rs = db_query("SELECT machine_name, friendly_name FROM {ntlp_activity_type}");

    while ($note = db_fetch_array($rs)) {
        $activitytype[$note['machine_name']] = $note['friendly_name'];
    }
    return $activitytype;
}

function check_and_update_unit($course_id, $unit) {

    $result_set = db_query("SELECT id FROM {ntlp_course_unit} WHERE name = '%s' AND course_id = %d ", $unit, $course_id);
    $obj = db_fetch_object($result_set);


    if (!empty($obj)) {
        $unit_id = $obj->id;
    } else {
        db_query("INSERT INTO {ntlp_course_unit} (course_id, name) VALUES(%d, '%s')", $course_id, $unit);
        $rsobj = db_last_insert_id('{ntlp_course_unit}', 'id');
        $unit_id = $rsobj->id;
    }
    return $unit_id;
}



function get_outcomes_values($com, $activity_nid) {

    $activityoutcome = get_activity_outcome($activity_nid);

    while (list($key, $val) = each($com)) {

        foreach ($activityoutcome as $outcome) {
            //  print_r($outcome);

            if ($key == 'outcome_' . $outcome->tid) {
                $activityoutcomearray[$outcome->tid] = $val;
            }
        }
    }
    return $activityoutcomearray;
}

function check_activity_availability($start_date, $end_date) {
    $dateformat = "Y-m-d G:i:s";
    $flag = false;
    $today = date($dateformat);

    if ($today >= $start_date && $today <= $end_date) {
        $flag = true;
    } else {

        $flag = false;
    }

    return $flag;
}

function check_activity_due_date($duedate) {
    $dateformat = "Y-m-d G:i:s";
    $flag = false;
    $today = date($dateformat);

//    if($today >= $duedate && $today <= $duedate ) {
    if ($today >= $duedate) {
        echo $today . ' ' . $duedate . ' true';
        $flag = true;
    } else {
        echo $today . ' ' . $duedate . ' false';
        $flag = false;
    }
    return $flag;
}

function get_activity_name($activity_type) {

    $friendly_name = cache_get($activity_type . "_get_name");

    if ($friendly_name)
        return $friendly_name->data . '';

    $result = db_query("SELECT friendly_name FROM {ntlp_activity_type} WHERE machine_name ='%s'", $activity_type);
    
    $friendly_name = db_fetch_object($result)->friendly_name;
    cache_set($activity_type . "_get_name", $friendly_name);
    return $friendly_name;
}

function get_links($var) {

    $array = split(';', $var);

    foreach ($array as $element) {
        $elem = trim($element);
        $attach = explode(',', $elem);
        $link[] = $attach;
    }
    sort($link);

    return $link;
}

function get_google_docs($isPrint = true) {

    $find = array();

    $type = isset($_GET['type']) ? $_GET['type'] : 'radio';
    $domain = '';
    $feed = retrieve_google_docs($domain);

    if ($isPrint) {
        print renderGoogleDocs($feed, $domain, $type);
    } else {
        return renderGoogleDocs($feed, $domain, $type);
    }
}

/** * Creates an HTML list of each <entry>'s title.  *
 * @param Zend_Gdata_Feed $feed A Gdata feed object
 * @return string The HTML of entries
 */
function renderGoogleDocs($feed, $domain, $type) {
    global $base_path;
    include "template.inc";
    $html = '';
    $count = 0;
    $documents = '';

    foreach ($feed->entries as $entry) {
        //print_r($entry);
        $count++;
        if ($count > 1)
            $documents .= $HTMLseparator;

        // Find the URL of the HTML view of the document.
        foreach ($entry->link as $link) {
            if ($link->getRel() === 'alternate') {
                $alternateLink = $link->getHref();
            }
        }
        $doctype = '';
        foreach ($entry->category as $cat) {
            if (strpos($cat->getScheme(), '#kind'))
                $doctype = $cat->getLabel();
//            if ($cat->getLabel() === 'document') //spreadsheet, presentation
        }

        if (strlen($entry->title->text) > 54) {
            $title_doc = substr($entry->title->text, 0, 54) . "...";
        } else {
            $title_doc = $entry->title->text;
        }

        $lastupdated = date('M j, Y g:i a', strtotime($entry->getUpdated()));

        $rHTMLrow = $HTMLrow;
        $rHTMLrow = str_replace('@DRUPAL_BASE', $base_path, $rHTMLrow);
        $rHTMLrow = str_replace('@TYPE', $type, $rHTMLrow);
        $rHTMLrow = str_replace('@ICON_DOC', $doctype . 'Icon', $rHTMLrow);
        $rHTMLrow = str_replace('@DOC_LINK', $alternateLink, $rHTMLrow);
        $rHTMLrow = str_replace('@DOC_NAME', $title_doc, $rHTMLrow);
        $rHTMLrow = str_replace('@LAST_UPDATED', $lastupdated, $rHTMLrow);
        $documents .= $rHTMLrow;
        //$str .= "<li><a href=\"{$alternateLink}\" target=\"new\">{$entry->title->text}</a></li>";
    }

    if ($count == 0)
        $rMsgCount = "(No documents found)";
    else {
        if ($count == 1)
            $rMsgCount = "(1 document found)";
        else
            $rMsgCount = "($count documents found)";
    }

//    $allDocslink = 'http://docs.google.com/a/' . $domain;

    $rHTMLcontainer = $HTMLcontainer;
    $rHTMLcontainer = str_replace('@FEED_TITLE', 'Docs', $rHTMLcontainer);
    $rHTMLcontainer = str_replace('@DOCS_COUNT', $rMsgCount, $rHTMLcontainer);
    $rHTMLcontainer = str_replace('@DOCUMENTS', $documents, $rHTMLcontainer);
//    $rHTMLcontainer = str_replace('@DOCS_LINK', $allDocslink, $rHTMLcontainer);
    $rHTMLcontainer = str_replace('@DRUPAL_BASE', $base_path, $rHTMLcontainer);
    $html .= $rHTMLcontainer;

    return $html;
}

/** * Convert Date From Database into a drupal  form even type *
 * @param $pdate  separate The date
 * @return $arr  Returns the date into a drupal format
 */
function dateconversion($from_date) {


    $pdate = explode('-', $from_date);

    $year = $pdate[0];
    $month = $pdate[1];

// Separating time From day
    $convertdate = $pdate[2];
    $arrdate = explode(' ', $convertdate);
    $day = $arrdate[0];

// Separating Time in Hour and Minutes
    $ptime = explode(':', $arrdate[1]);
    $hour = $ptime[0];
    $minute = $ptime[1];



    $arr['year'] = $year;
    $arr['month'] = $month;
    $arr['day'] = $day;
    $arr['hour'] = $hour;
    $arr['minute'] = $minute;


    return $arr;
}

# Fetching UserName For Submission Grade and Submission Grade View Page

function get_activity_username($uid) {

    $result = db_query("SELECT n.uid,u.picture, n.school_nid, n.first_name, n.last_name FROM {ntlp_user} n
                        INNER JOIN {users} u ON u.uid = n.uid WHERE n.uid = %d ORDER BY n.last_name, n.first_name", $uid);


    return db_fetch_object($result);
}


# Getting outcome Weight For Submission Grade and Submission Grade View Page

function get_activity_outcome_weight($activitynid) {

    $result = db_query("SELECT max_points FROM {ntlp_gb_activity_outcome}
                     WHERE activity_nid= %d", $activitynid);


    while ($note = db_fetch_object($result)) {

        $totalpoints += $note->max_points;
    }

    return $totalpoints;
}

function ntlp_course_tabs() {
    include "coursetabs.template.inc";
    global $user;


    $output = $HTMLcourse_tab;

    return $output;
}

# Fetching Assignment Outcomes for Grade Submission

function get_assignment_outcomes($tid, $activity_id, $user_uid) {

    $result = db_query("SELECT ag.points FROM {ntlp_gb_activity_grade} ag, {ntlp_gb_grade} g 
        WHERE ag.grade_id = g.id AND ag.tid = %d AND g.activity_nid = %d AND g.user_uid = %d", 
        $tid, $activity_id, $user_uid);

    return db_fetch_object($result);
}

function get_excused_assignment_outcomes($tid, $activity_id, $user_uid) {

    $result = db_query("SELECT ag.* FROM {ntlp_gb_activity_excuse} ag, {ntlp_gb_grade} g 
        WHERE ag.grade_id = g.id AND ag.tid = %d AND g.activity_nid = %d AND g.user_uid = %d", 
        $tid, $activity_id, $user_uid);

    return db_fetch_object($result);
}

function get_grade_activity_outcomes($tid, $activity_id) {

    $result = db_query("SELECT max_points from {ntlp_gb_activity_outcome} WHERE tid = %d AND activity_nid  = %d", $tid, $activity_id);

    return db_fetch_object($result);
}

function get_teacher_name($activity_id) {

    $result = db_query("SELECT u.first_name,u.last_name from {ntlp_user} u  WHERE u.uid = %d ORDER BY u.last_name, u.first_name", $activity_id);

    return db_fetch_object($result);
}

function new_activity_title($course_id) {
    include "activity_template.inc";
    global $user;

    $output = $HTMLform_activity_outer_name_hdr;

    $title = str_replace('@ACTIVITIES_LINK', '', $output);
    $stitle = str_replace('@ACTIVITY_STATUS', 'Edit Activity', $output);

    return $title;
}

function grade_title($course_id) {
    include "activity_template.inc";
    global $user;

    $output = $HTMLform_activity_outer_name_hdr;

    $title = str_replace('@ACTIVITIES_LINK', url('ntlp/courses/activity/' . $course_id), $output);
    $stitle = str_replace('@ACTIVITY_STATUS', 'Edit Activity', $output);

    return $title;
}

#function for datepicker start here

function leading_zeros($value, $places=2) {
// Function written by Marcus L. Griswold (vujsa)
// Can be found at http://www.handyphp.com
// Do not remove this header!

    if (is_numeric($value)) {
        for ($x = 1; $x <= $places; $x++) {
            $ceiling = pow(10, $x);
            if ($value < $ceiling) {
                $zeros = $places - $x;
                for ($y = 1; $y <= $zeros; $y++) {
                    $leading .= "0";
                }
                $x = $places + 1;
            }
        }
        $output = $leading . $value;
    } else {
        $output = $value;
    }
    return $output;
}

function _get_hours() {
    for ($i = 1; $i < 13; $i++) {
        $hrs = leading_zeros($i);
        $hours[$hrs] = $hrs;
    }
    return $hours;
}

function _get_minutes() {
    for ($i = 0; $i < 60; $i+=5) {
        $mins = leading_zeros($i);
        $minutes[$mins] = $mins;
    }
    return $minutes;
}

function _get_meridian() {
    $meridian['AM'] = 'AM';
    $meridian['PM'] = 'PM';
    return $meridian;
}

function _user_activity_submission_by_course_nid($school_term_tid, $course_nid, $userid) {
    $sql = 'SELECT nca.nid
            FROM {ntlp_course_activity} nca
                INNER JOIN {ntlp_gb_grade} ng ON ng.activity_nid = nca.nid
            WHERE nca.course_nid = %d AND ng.user_uid = %d';

    $rows = array();

    if ($result = db_query($sql, $course_nid, $userid)) {
        while ($row = db_fetch_object($result)) {
            $rows[] = $row;
        }
    }

    return $rows;
}

/*
 * Check Student that is he allow to resubmit the assignment
 *
 */

function check_student_for_activity($user_id, $activity_id) {
    $flag = false;
    $result = db_query('select user_uid from {ntlp_activity_user} where user_uid = %d AND activity_nid = %d ', $user_id, $activity_id);

    if ($result->num_rows > 0) {
        $flag = false;
    } else {
        $flag = true;
    }
}

function get_my_school($user_uid) {
    $result = db_query("SELECT school_nid FROM {ntlp_user} where uid=%d", $user_uid);
    $rows = db_fetch_array($result);
    return $rows['school_nid'];
}

function get_school_courses($course_nid, $user_uid) {
    $school_nid = get_my_school($user_uid);
    $result = db_query("SELECT nid,course_short_title FROM dpl_ntlp_course WHERE is_project = 0 and school_nid=%d", $school_nid);

    if ($result) {
        while ($rows = db_fetch_object($result)) {
            $options[$rows->nid] = $rows->course_short_title;
        }
    }
    return $options;
}

function get_file_fid($activity_nid) {

    $result = db_query("SELECT * FROM {ntlp_activity_attachment} WHERE type = '%s' AND activity_nid = %d ", 'F', $activity_nid);

    return db_fetch_object($result);
}

function get_submission_file_fid($activity_nid) {

    $result = db_query("SELECT * FROM {ntlp_submission_attachment} WHERE type = '%s' AND submission_nid = %d ", 'F', $activity_nid);

    return db_fetch_object($result);
}

function get_attachment_filepath($activity_nid, $id) {

    $result = db_query("SELECT at.*,f.* FROM {ntlp_activity_attachment} at INNER JOIN

{files} f ON f.fid = at.fid where at.activity_nid = %d and at.type = '%s' AND id = %d", $activity_nid, 'F', $id);

    return db_fetch_object($result);
}

function get_submission_attachment_filepath($activity_nid, $id) {

    $result = db_query("SELECT at.*,f.* FROM {ntlp_submission_attachment} at INNER JOIN

{files} f ON f.fid = at.fid where at.submission_nid = %d and at.type = '%s' AND id = %d", $activity_nid, 'F', $id);

    return db_fetch_object($result);
}


function get_vocablary_vid($string) {

    $rs = db_query("SELECT vo.vid from {vocabulary} vo WHERE vo.name='%s'", $string);

    return db_fetch_object($rs);
}

function get_unit_topic_name($tid) {

    $rs = db_query("select name from {term_data} where tid = %d ", $tid);

    return db_fetch_object($rs);
}

function get_upcoming_activities($courseid, $limit = 5) {
    global $user;

    if (isset($user->roles[NTLP_ROLEID_STUDENT])) {
        $join = "INNER JOIN dpl_ntlp_activity_user au ON au.activity_nid = ca.nid";
        $student_condition = "AND au.user_uid = " . $user->uid . "
            AND ((ca.availability_mode = 'S' AND ca.available_from <= CURDATE()
            AND ca.available_to >= CURDATE()) OR (ca.availability_mode = 'M'
            AND ca.available_from <> '0000-00-00 00:00:00'))";
    } else {
        $student_condition = "";
        $join = "";
    }
//    $rs = db_query_range( "SELECT n.title, n.nid FROM {ntlp_course_activity} a
//        INNER JOIN {node} n  on n.nid = a.nid
//        WHERE a.course_nid = %d AND UNIX_TIMESTAMP(a.available_to) >= UNIX_TIMESTAMP()
//        ORDER BY a.available_to DESC", $courseid, 0, $limit );


    $dateformat = "%Y-%m-%d";
    $current_date = strftime($dateformat, time());

    $rs = db_query("SELECT n.title, n.nid, ca.due_date, project_node.title as project_title
        FROM dpl_ntlp_course_activity ca
        INNER JOIN dpl_node n  ON n.nid = ca.nid
        LEFT JOIN dpl_node project_node ON project_node.nid = ca.project_nid
        " . $join . " WHERE ca.due_date >= '%s'" . $student_condition .
                    " AND ca.course_nid = %d order by ca.due_date LIMIT 0, 5", $current_date, $courseid);

    return $rs;
}

function get_selected_course_term($course_id) {

    $rs = db_query("SELECT school_term_tid from {ntlp_course_term} where course_nid = %d ", $course_id);
    $term_id = db_fetch_object($rs);
    $school_term_tid = $term_id->school_term_tid;



    return $school_term_tid;
}

function ge_rebric_catid($activity_nid, $cat_Order) {

    $rs = db_query("/* INSERT / UPDATE / DELETE */ select category_ID from {ntlp_activity_rubric_category} where  activity_nid = %d and category_order = %d", $activity_nid, $cat_Order);

    $data = db_fetch_object($rs);
    $catid = $data->category_ID;
    return $catid;
}

function ge_rebric_levelid($activity_nid, $level_order) {

    $rs = db_query("/* INSERT / UPDATE / DELETE */ select level_ID  from {ntlp_activity_rubric_level} where  activity_nid = %d and level_order = %d", $activity_nid, $level_order);

    $data = db_fetch_object($rs);
    $levelID = $data->level_ID;
    return $levelID;
}

function get_rubric_categories($activity_id) {

    $rs = db_query("select category_name, category_ID, max_points  from {ntlp_activity_rubric_category} where activity_nid = %d ", $activity_id);
    while ($data = db_fetch_object($rs)) {

        $cat_name[] = $data;
    }
    return $cat_name;
}

function get_rubric_categories_score($activity_id) {

    $rs = db_query("select category_name, category_ID, max_points  from {ntlp_activity_rubric_category} where activity_nid = %d ", $activity_id);
    while ($data = db_fetch_object($rs)) {

        $cat_name[] = $data;
    }
    return $cat_name;
}

function is_student_access_this_activity($user_uid, $activity_nid) {
    $flag = false;
    $rs = db_query("SELECT * FROM {ntlp_activity_user} au
        INNER JOIN {ntlp_course_activity} a ON a.nid = au.activity_nid
        INNER JOIN {ntlp_course_user} cu ON cu.course_nid = a.course_nid AND cu.user_uid = au.user_uid
        WHERE activity_nid = %s AND au.user_uid = %d AND cu.role_id = %d", 
            $activity_nid, $user_uid, NTLP_ROLEID_STUDENT);

    if ($rs->num_rows > 0) {
        $flag = true;
    }
    return $flag;
}

function check_evaluation_status($feedback_for, $feedback_by, $activity_nid) {
//    $flag = false;
    $rs = db_query("
SELECT  n.uid FROM {ntlp_activity_rubric_category} s
INNER JOIN {ntlp_activity_rubric_submission} rs ON rs.category_id = s.category_ID
INNER JOIN {node n ON}   n.nid = rs.submission_nid
 WHERE  s.activity_nid = %d AND rs.feedback_for = %d AND n.uid = %d", $activity_nid, $feedback_for, $feedback_by);

    if ($rs->num_rows > 0) {
        $flag = true;
    } else {
        $flag = false;
    }
    return $flag;
}

function check_rubric_user_activity($user_uid, $activity_nid) {
//    $flag = false;
    $rs = db_query("select * from {ntlp_activity_user} where activity_nid = %d and user_uid = %d", $activity_nid, $user_uid);

    if ($rs->num_rows > 0) {
        $flag = TRUE;
    } else {
        $flag = FALSE;
    }
    return $flag;
}

function get_rubric_submission_cat_avg_score($feedback_for, $activity_nid) {

//    $rs =db_queryd("select s.score from {ntlp_activity_rubric_submission} s
//        INNER JOIN {node} sb ON sb.nid = s.submission_nid  where s.category_id  = %d and s.feedback_for = %d and sb.uid = %d",$cat_id, $feedback_for, $feedback_by);
    $rs = db_query('SELECT arc.category_ID, arc.category_name, AVG(ars.score) score, arc.max_points
        FROM {ntlp_activity_rubric_category} arc
        INNER JOIN {ntlp_activity_rubric_submission} ars ON ars.category_id = arc.category_ID
        WHERE ars.feedback_for = %d AND arc.activity_nid = %d
        GROUP BY arc.category_ID', $feedback_for, $activity_nid);

    return $rs;
}

function get_rubric_view_submission_cat_avg_score($feedback_for, $activity_nid) {

    $rs = db_query('SELECT arc.category_ID, arc.category_name, AVG(ars.score) score, arc.max_points
        FROM {ntlp_activity_rubric_category} arc
        INNER JOIN {ntlp_activity_rubric_submission} ars ON ars.category_id = arc.category_ID
        INNER JOIN {ntlp_activity_rubric_submission_for} arns ON arns.submission_nid = ars.submission_nid
        WHERE arns.exclude_feedback = 0 and ars.feedback_for = %d AND arc.activity_nid = %d
        GROUP BY arc.category_ID', $feedback_for, $activity_nid);
    return $rs;
}

function get_rubric_submission_avg_score($feedback_for, $activity_nid) {

    $rs = db_query('SELECT AVG(score) AS avg_score, COUNT(score) AS score_count FROM
        (SELECT SUM(ars.score) score
        FROM {ntlp_activity_rubric_category} arc
        INNER JOIN {ntlp_activity_rubric_submission} ars ON ars.category_id = arc.category_ID
        INNER JOIN {ntlp_activity_rubric_submission_for} arns ON arns.submission_nid = ars.submission_nid
        INNER JOIN {node} n ON n.nid = ars.submission_nid
        WHERE arns.exclude_feedback = 0 and ars.feedback_for = %d AND arc.activity_nid = %d
        GROUP BY n.uid) AS stu_scores', $feedback_for, $activity_nid);
    return db_fetch_object($rs);
}

function get_rubric_activity_total_points($activity_nid) {
    $rs = db_query('SELECT SUM(arc.max_points) total_points
        FROM {ntlp_activity_rubric_category} arc
        WHERE arc.activity_nid = %d', $activity_nid);
    return db_fetch_object($rs);
}

function get_rubric_submission_cat_score($feedback_for, $feedback_by, $activity_nid) {
    $rs = db_query('SELECT max(rs.submission_nid) submission_nid
        FROM dpl_ntlp_activity_rubric_submission rs
        INNER JOIN dpl_ntlp_activity_rubric_category arc ON arc.category_ID = rs.category_id
        INNER JOIN dpl_node n ON n.nid = rs.submission_nid
        WHERE n.uid = %d AND rs.feedback_for = %d AND arc.activity_nid = %d',
                    $feedback_for, $feedback_by, $activity_nid);

    if ($data = db_fetch_object($rs)) {

        $submission_nid = $data->submission_nid;


        $rs = db_query('SELECT rs.*, arc.*, n.created, n.changed
            FROM dpl_ntlp_activity_rubric_submission rs
            INNER JOIN dpl_ntlp_activity_rubric_category arc ON arc.category_ID = rs.category_id
            INNER JOIN dpl_node n ON n.nid = rs.submission_nid
            WHERE n.uid = %d AND rs.feedback_for = %d AND arc.activity_nid = %d AND rs.submission_nid = %d',
                        $feedback_for, $feedback_by, $activity_nid, $submission_nid);
        return $rs;
    }
}

function get_rubric_submisssion_id($activity_id, $user_uid) {

    $submission_id = array();
    $rs = db_query("SELECT DISTINCT s.submission_nid,n.uid, nars.exclude_feedback  FROM {ntlp_activity_rubric_category} c  INNER JOIN
    {ntlp_activity_rubric_submission} s ON s.category_id = c.category_ID
    INNER JOIN {ntlp_activity_rubric_submission_for} nars ON nars.submission_nid = s.submission_nid
    INNER JOIN dpl_node n  ON n.nid = s.submission_nid
    WHERE nars.exclude_feedback = 0 and c.activity_nid = %d and s.feedback_for = %d", $activity_id, $user_uid);
    while ($data = db_fetch_object($rs)) {

        $submission_id[$data->submission_nid] = $data;
    }


    return $submission_id;
}

function get_rubric_evaluated_records_for_user($activity_id, $user_uid) {

    $submission_id = array();
    $rs = db_query("SELECT DISTINCT s.submission_nid,n.uid, nars.exclude_feedback, s.comments, s.category_id, c.category_name
        FROM {ntlp_activity_rubric_category} c  INNER JOIN
    {ntlp_activity_rubric_submission} s ON s.category_id = c.category_ID
    INNER JOIN {ntlp_activity_rubric_submission_for} nars ON nars.submission_nid = s.submission_nid
    INNER JOIN dpl_node n  ON n.nid = s.submission_nid
    WHERE nars.exclude_feedback = 0 and c.activity_nid = %d and s.feedback_for = %d ORDER BY c.category_name ASC",
                    $activity_id, $user_uid);
    while ($data = db_fetch_object($rs)) {

        $submission_id[$data->category_id] = $data;
    }


    return $submission_id;
}

function _get_user_evaluated_record_by_category($activity_id, $user_uid, $category_id) {

    $evaluated_records = array();

    $rs = db_query("SELECT DISTINCT s.submission_nid,n.uid, nars.exclude_feedback,
        s.comments, s.category_id, c.category_name
        FROM dpl_ntlp_activity_rubric_category c
        INNER JOIN dpl_ntlp_activity_rubric_submission s ON s.category_id = c.category_ID
        INNER JOIN dpl_ntlp_activity_rubric_submission_for nars ON nars.submission_nid = s.submission_nid
        INNER JOIN dpl_node n  ON n.nid = s.submission_nid
        WHERE nars.exclude_feedback = 0 AND c.activity_nid = %d
        AND s.feedback_for = %d AND c.category_id = %d  ORDER BY c.category_name ASC", $activity_id,
                    $user_uid, $category_id);

    while ($data = db_fetch_object($rs)) {
        $evaluated_records[] = $data;
    }


    return $evaluated_records;
}

function get_rubric_graded_submisssions($activity_id, $user_uid) {

    $submission_id = array();
    $rs = db_query("SELECT DISTINCT s.submission_nid,n.uid, nars.exclude_feedback  FROM {ntlp_activity_rubric_category} c  INNER JOIN
    {ntlp_activity_rubric_submission} s ON s.category_id = c.category_ID
    INNER JOIN {ntlp_activity_rubric_submission_for} nars ON nars.submission_nid = s.submission_nid
    INNER JOIN dpl_node n  ON n.nid = s.submission_nid
    WHERE c.activity_nid = %d and s.feedback_for = %d ", $activity_id, $user_uid);
    while ($data = db_fetch_object($rs)) {

        $submission_id[$data->submission_nid] = $data;
    }


    return $submission_id;
}

function get_rubric_submission_record($activity_id, $feedback_by, $feedback_for) {

    $rs = db_query("SELECT *  FROM dpl_ntlp_activity_rubric_category c
                    INNER JOIN dpl_ntlp_activity_rubric_submission s ON c.category_ID = s.category_id
                    INNER JOIN dpl_node n ON n.nid = s.submission_nid
                    WHERE c.activity_nid = %d AND n.uid = %d AND s.feedback_for= %d", $activity_id, $feedback_by, $feedback_for);

    $data = db_fetch_object($rs);
    $submission_id = $data->submission_nid;
    return $submission_id;
}

function get_user_feedback($activity_nid, $feedback_user, $submission_id) {
    $rs = db_query("SELECT
          rc.category_ID,
          rc.category_name,
          rc.max_points,
          rcs.*,
          rcsf.comment_to_teacher,
          rcsf.exclude_feedback
        FROM dpl_ntlp_activity_rubric_category rc
          INNER JOIN dpl_ntlp_activity_rubric_submission rcs
            ON rcs.category_id = rc.category_ID
          INNER JOIN dpl_ntlp_activity_rubric_submission_for rcsf
            ON rcsf.submission_nid = rcs.submission_nid
        WHERE rc.activity_nid = %d
            AND rcs.feedback_for = %d
            AND rcs.submission_nid = %d", $activity_nid, $feedback_user, $submission_id);

    return $rs;
}

function get_rubric_view_user_feedback($activity_nid, $feedback_user, $submission_id) {
    $rs = db_query("SELECT
          rc.category_ID,
          rc.category_name,
          rc.max_points,
          rcs.*,
          rcsf.comment_to_teacher,
          rcsf.exclude_feedback
        FROM dpl_ntlp_activity_rubric_category rc
          INNER JOIN dpl_ntlp_activity_rubric_submission rcs
            ON rcs.category_id = rc.category_ID
          INNER JOIN dpl_ntlp_activity_rubric_submission_for rcsf
            ON rcsf.submission_nid = rcs.submission_nid
        WHERE  rcsf.exclude_feedback =0 AND rc.activity_nid = %d
            AND rcs.feedback_for = %d
            AND rcs.submission_nid = %d", $activity_nid, $feedback_user, $submission_id);

    return $rs;
}

function get_rubric_level($activity_id) {

    $rs = db_query("select * from {ntlp_activity_rubric_level} where activity_nid = %d", $activity_id);
    while ($data = db_fetch_object($rs)) {
        $levelArray[$data->level_order - 1] = $data->level_name;
    }
    return $levelArray;
}

function get_rubric_level_percent($activity_id) {

    $rs = db_query("select * from {ntlp_activity_rubric_level} where activity_nid = %d", $activity_id);
    while ($data = db_fetch_object($rs)) {
        $levelArray[$data->level_order - 1] = $data->level_percent;
    }
    return $levelArray;
}

function get_rubric_records($activity_id) {

    $rs = db_query('select * from {ntlp_course_activity_rubric} where activity_nid = %d', $activity_id);

    return db_fetch_object($rs);
}

function get_cat_avg_score($activity_id, $avg_score) {

    $categories = get_rubric_categories_score($activity_id);

    if ($categories != null || !empty($categories)) {

        foreach ($categories as $cat) {

            $total['totalpoints'] += $cat->max_points;
            $total['securepoints'] += $avg_score[$cat->category_ID];
        }
    }
    return $total;
}

function get_rubric_users($head, $limit, $activity_nid) {

    $order_by = (isset($_GET['order']) || isset($_GET['sort'])) ? tablesort_sql($head) : ' ORDER BY nu.last_name, nu.first_name';
    $rs = pager_query("SELECT
          nu.uid,
          du.picture,
          nu.first_name,
          nu.last_name
        FROM {ntlp_activity_user} au
          INNER JOIN dpl_ntlp_user nu
            ON au.user_uid = nu.uid
          INNER JOIN {users} du
            ON au.user_uid = du.uid
        WHERE au.activity_nid = %d" . $activity_nid . $order_by, $limit);

    return $rs;
}

function get_user_feedback_recieved($activity_nid, $user_id) {
    /* $rs = db_query('SELECT COUNT(DISTINCT rsf.submission_nid) AS totalstudent, AVG(rs.score) as score
      FROM dpl_ntlp_activity_rubric_submission rs
      INNER JOIN dpl_ntlp_activity_rubric_category rc ON rs.category_ID = rc.category_id
      INNER JOIN dpl_ntlp_activity_rubric_submission_for rsf ON rsf.submission_nid = rs.submission_nid
      WHERE rc.activity_nid = %d AND rs.feedback_for = %d', $activity_nid, $user_id); */


    //Changes by saifuddin

    $rs = db_query('SELECT COUNT(DISTINCT rsf.submission_nid) AS totalstudent,
        SUM(rs.score) / COUNT(DISTINCT rsf.submission_nid) AS score
         FROM dpl_ntlp_activity_rubric_submission rs
         INNER JOIN dpl_ntlp_activity_rubric_category rc ON rs.category_ID = rc.category_id
         INNER JOIN dpl_ntlp_activity_rubric_submission_for rsf ON rsf.submission_nid = rs.submission_nid
         WHERE rc.activity_nid = %d AND rs.feedback_for = %d', $activity_nid, $user_id);

    return db_fetch_object($rs);
}

function get_user_feedback_submitted($activity_nid, $user_id) {

    /* $rs = db_query('SELECT COUNT(DISTINCT rsf.feedback_for) AS totalstudent, AVG(rs.score) as score
      FROM dpl_ntlp_activity_rubric_submission rs
      INNER JOIN dpl_ntlp_activity_rubric_category rc ON rs.category_ID = rc.category_id
      INNER JOIN dpl_ntlp_activity_rubric_submission_for rsf ON rsf.submission_nid = rs.submission_nid
      INNER JOIN dpl_node n ON n.nid = rs.submission_nid
      WHERE rc.activity_nid = %d AND n.uid =  %d', $activity_nid, $user_id); */

    //update this query by saifuddin on Oct 14

    $rs = db_query('SELECT COUNT(DISTINCT rsf.feedback_for) AS totalstudent, (SUM(rs.score) / COUNT(DISTINCT rsf.feedback_for)) as score
        FROM dpl_ntlp_activity_rubric_submission rs
        INNER JOIN dpl_ntlp_activity_rubric_category rc ON rs.category_ID = rc.category_id
        INNER JOIN dpl_ntlp_activity_rubric_submission_for rsf ON rsf.submission_nid = rs.submission_nid
        INNER JOIN dpl_node n ON n.nid = rs.submission_nid
        WHERE rc.activity_nid = %d AND n.uid = %d', $activity_nid, $user_id);

    return db_fetch_object($rs);
}

function get_rubric_user_submission_grade($school_term_tid, $activity_id, $user_id) {

    $rs = db_query("SELECT * FROM {ntlp_activity_submission}
        WHERE activity_nid = %d AND user_uid = %d", $activity_id, $user_id);

    if ($rs->num_rows > 0) {
        $data = db_fetch_object($rs);
        return $data;
    } else {

        return false;
    }
}

function get_rubric_overall_avg($feedback_for, $activity_nid) {

    $rs = db_query("SELECT AVG(max_points) max_points, AVG(score) score, totalstudents
        FROM (SELECT SUM(arc.max_points) max_points, SUM(ars.score) score,
        COUNT(DISTINCT n.uid) totalstudents
        FROM dpl_ntlp_activity_rubric_category arc
        INNER JOIN dpl_ntlp_activity_rubric_submission ars ON ars.category_id = arc.category_ID
        INNER JOIN {ntlp_activity_rubric_submission_for} arns ON arns.submission_nid = ars.submission_nid
        INNER JOIN dpl_node n ON n.nid = ars.submission_nid
        WHERE arns.exclude_feedback = 0 and ars.feedback_for = %d AND arc.activity_nid = %d
        GROUP BY n.uid) tb1", $feedback_for, $activity_nid);

    $data = db_fetch_object($rs);

    $arr['avg_cat_max_points'] = round($data->max_points);
    $arr['avg_cat_feedback_points'] = round($data->score);
    $arr['avg_cat_feedback_pointstotalstudents'] = $data->totalstudents;

    return $arr;
}

function get_graded_rubric_overall_avg($feedback_for, $activity_nid) {

    $rs = db_query("SELECT
  AVG(max_points) max_points,
  AVG(score) score,
  totalstudents
FROM (SELECT SUM(arc.max_points) max_points, SUM(ars.score) score, COUNT(DISTINCT n.uid) totalstudents
      FROM {ntlp_activity_rubric_category} arc
        INNER JOIN {ntlp_activity_rubric_submission} ars ON ars.category_id = arc.category_ID
        INNER JOIN {ntlp_activity_rubric_submission_for} narsf ON narsf.submission_nid = ars.submission_nid
        INNER JOIN dpl_node n ON n.nid = ars.submission_nid
      WHERE ars.feedback_for = %d AND arc.activity_nid = %d AND narsf.exclude_feedback = 0
      GROUP BY n.uid) tb1", $feedback_for, $activity_nid);

    $data = db_fetch_object($rs);

    $arr['avg_cat_max_points'] = round($data->max_points);
    $arr['avg_cat_feedback_points'] = round($data->score);
    $arr['avg_cat_feedback_pointstotalstudents'] = $data->totalstudents;

    return $arr;
}

function get_attachment($nid, $tablename, $field_name) {

    $result = db_query("SELECT id, " . $field_name . ", type, attachment_name, path, is_deleted, fid FROM {" . $tablename . "}
        WHERE " . $field_name . " = %d", $nid);

    while ($note = db_fetch_object($result)) {
        $attachment[] = $note;
    }
    return $attachment;
}

//
//function get_user_feedback_uids($activity_id, $feedback_by) {
//
//    $rs =db_query(" SELECT DISTINCT feedback_for FROM {ntlp_activity_rubric_category} c
//                    INNER JOIN {ntlp_activity_rubric_submission} s ON c.category_ID = s.category_id
//                    INNER JOIN {node} n ON n.nid = s.submission_nid WHERE c.activity_nid = %d AND n.uid = %d", $activity_id, $feedback_by);
//    while($data = db_fetch_object($rs)) {
//
//        $feedback_users[$data->feedback_for] = $data->feedback_for;
//    }
//
//    return $feedback_users;
//}

function get_user_feedback_uids($activity_id, $feedback_by) {
    $rs = db_query("SELECT user_uid FROM {ntlp_rubric_selected_user} ru , 
        {ntlp_user}  u WHERE activity_nid = %d  AND added_by_uid =%d AND ru.user_uid = u.uid ORDER BY u.last_name, u.first_name", $activity_id, $feedback_by);
    $i = 0;
    while ($data = db_fetch_object($rs)) {

        $feedback_users[$i++] = $data->user_uid;
    }

    return $feedback_users;
}

function get_user_feedback_uid_next($activity_id, $feedback_by, $current_uid) {
    $rs = db_query("SELECT user_uid FROM {ntlp_rubric_selected_user}
        WHERE activity_nid = %d  AND added_by_uid =%d AND user_uid > %d limit 0,1",
                    $activity_id, $feedback_by, $current_uid);

    while ($data = db_fetch_object($rs)) {

        $next_feedback_uid = $data->user_uid;
    }
    return $next_feedback_uid;
}

function get_activity_users_lite($activity_nid) {

    return db_query("SELECT u.uid, u.name
                FROM {ntlp_activity_user} au
                INNER JOIN {users} u ON au.user_uid=u.uid
                WHERE au.activity_nid = {$activity_nid}");
}

function get_activity_users($activity_nid, $orderby=null) {

    $rs = db_query("SELECT au.user_uid, n.first_name, n.last_name, cu.period, cu.role_id
                FROM {ntlp_activity_user} au
                INNER JOIN {ntlp_user} n ON n.uid = au.user_uid
                INNER JOIN {ntlp_course_activity} ca ON ca.nid = au.activity_nid
                INNER JOIN {ntlp_course_user} cu ON cu.user_uid = au.user_uid AND cu.course_nid = ca.course_nid
                WHERE au.activity_nid = %d
                ORDER BY %s",
                    $activity_nid, (($orderby == null) ? "cu.period, n.last_name, n.first_name" : $orderby));

    return $rs;
}

function get_activity_users_without_submissions($activity_nid) {
    $rs = db_query("SELECT au.user_uid FROM {ntlp_activity_user} au WHERE au.activity_nid = %d", $activity_nid);
    return $rs;
}

function check_user_rubric_activity($feedback_by, $feedback_for, $activity_nid) {
    $flag = false;
    $rs = db_query("select * from {ntlp_rubric_selected_user} where activity_nid = %d and user_uid = %d and added_by_uid = %d", $activity_nid, $feedback_for, $feedback_by);

    if ($rs->num_rows > 0) {
        $flag = false;
    } else {
        $flag = true;
    }
    return $flag;
}

function check_and_the_file_type_image($file_path,$filename=null) {
    $file_image = 'img_document';
    $exp_array = explode('/', $file_path);
    $file_type = $exp_array[0];

    if ( $file_type == 'text') {
        $file_image = "img_document";
    }else if ($file_type == 'application' ) {

        if ($filename!=null){
            $file_arr = explode('.', $filename);
            $extension = $file_arr[sizeof($file_arr) - 1];

            if (strpos(" doc, docx, odt", $extension))
                $file_image = "word_document";
            elseif (strpos(" ppt, pptx, odp", $extension))
                $file_image = "ppt_document";
            elseif (strpos(" xls, xlsx, ods", $extension))
                $file_image = "xls_document";
            elseif (strpos(" pdf", $extension))
                $file_image =  'pdf_document';
        }

    } else if ($file_type == 'image') {
        $file_image = "img_picture";
    } else if ($file_type == 'video') {
        $file_image = "img_video";
    } else {
        $file_image = "img_document";
    }

    return $file_image;
}

function get_course_school_nid($course_id) {
    $rs = db_query("select school_nid from {ntlp_course} where nid = %d", $course_id);
    $data = db_fetch_object($rs);

    return $data->school_nid;
}

function get_project_school_nid($project_id) {
    $rs = db_query("SELECT school_nid  FROM dpl_ntlp_course WHERE school_nid  <>0  AND course_nid IN (SELECT course_nid FROM dpl_ntlp_course WHERE nid = %d) LIMIT 0,1 ", $project_id);
    $data = db_fetch_object($rs);
    return $data->school_nid;
}


//Returns submission record, if it exists for a user for an activity
//Return only last submission
function get_user_submission($current_term_tid, $activity_nid, $user_uid, $lite_version = false) {
    global $user;

    if (isset($user->roles[NTLP_ROLEID_TEACHER]) || isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])
            || isset($user->roles[NTLP_ROLEID_SCHOOLLEADER]) || isset($user->roles[NTLP_ROLEID_SITEADMIN])
            || isset($user->roles[NTLP_ROLEID_NETWORKSTAFF])) {

        $AND = ' AND s.is_final = 1 ';
        $ORDER = ' s.date_submitted DESC, s.date_opened DESC ';
    } else if (isset($user->roles[NTLP_ROLEID_STUDENT]) || isset($user->roles[NTLP_ROLEID_PARENT])) {
        $AND = '';
        $ORDER = ' s.version_num DESC ';
    }

    if ($lite_version) {
        $rs = db_query("SELECT s.nid, s.is_final, s.date_submitted,
                g.is_complete, g.is_late, (CASE WHEN g.id IS NULL THEN 0 ELSE 1 END) is_graded, g.date_graded
            FROM {ntlp_course_activity} a 
                LEFT JOIN {ntlp_activity_submission} s ON s.activity_nid = a.nid AND s.user_uid = %d $AND
                LEFT JOIN {ntlp_gb_grade} g ON g.activity_nid = a.nid AND g.user_uid = %d 
                WHERE a.nid = %d
                ORDER BY $ORDER
                LIMIT 0, 1",
                $user_uid, $user_uid, $activity_nid);
    } else {
        $rs = db_query("SELECT s.*, a.course_nid, a.grading_method, nr.body, n.*, 
                g.is_complete, g.is_late, (CASE WHEN g.id IS NULL THEN 0 ELSE 1 END) is_graded, g.date_graded, g.teachers_notes
            FROM {ntlp_course_activity} a 
            LEFT JOIN {ntlp_activity_submission} s ON s.activity_nid = a.nid AND s.user_uid = %d $AND
            LEFT JOIN {ntlp_gb_grade} g ON g.activity_nid = a.nid AND g.user_uid = %d 
            LEFT JOIN {node_revisions} nr ON nr.nid = s.nid
            LEFT JOIN {node} n ON n.nid = s.nid
            WHERE a.nid = %d 
            ORDER BY $ORDER
            LIMIT 0, 1",
            $user_uid, $user_uid, $activity_nid);
}


    return $rs;
}

function get_course_user_activities($course_nid, $term_tid, $student_uid) {

    $rs = db_query("SELECT a.nid, a.grading_method, a.is_published
        FROM {ntlp_course_activity} a
        INNER JOIN {ntlp_activity_user} au ON a.nid = au.activity_nid 
        INNER JOIN {ntlp_course_user} cu ON a.course_nid = cu.course_nid 
        WHERE au.user_uid = %d AND cu.user_uid = %d AND cu.role_id = %d 
        AND a.course_nid = %d AND a.school_term_tid = %d",
        $student_uid, $student_uid, NTLP_ROLEID_STUDENT, $course_nid, $term_tid);

    return $rs;
}


//Gets a submission record based on the value of $skip
//$skip = 1, gets next record
//$skip = -1, gets previous record
//$activity_id is the current activity id
function get_another_user_submission($activity_id, $user_id, $activity_users, $skip) {
    if ($activity_users != null) {
        foreach ($activity_users as $key => $value) {
//            echo "[$key -> $value == $user_id, Next:".$activity_users[$key + $skip]."]";
            if ($value == $user_id) {
                $auid = $activity_users[$key + $skip];
                break;
            } else {
                $auid = 0;
            }
        }
//        $subs = get_user_submission($activity_id, $auid);
//        if ($us = db_fetch_object($subs)) {
//            return $us->nid;
//        }
    }
    return $auid;
}

function get_node($nid) {

    $rs = db_query("SELECT * FROM {node} n
        INNER JOIN {node_revisions} nr.nid = n.nid
        WHERE nid = %d", $nid);
    if ($rs->num_rows > 0) {
        return db_fetch_object($rs);
    } else {
        return false;
    }
}


function check_feedback_for_validation($activity_id, $feedback_for_uid) {
    global $user;
    $flag = false;


    $result = db_query("SELECT * FROM {ntlp_rubric_selected_user}
        WHERE activity_nid = %d AND user_uid = %d AND added_by_uid = %d", $activity_id, $feedback_for_uid,
                    $user->uid);

    if ($result->num_rows > 0) {
        $flag = true;
    } else {
        $flag = false;
    }

    return $flag;
}

function get_activity_submission_and_save_status($activity_Submission_Obj) {

    $activityObj = get_activity_records($activity_Submission_Obj->activity_nid, true);
    $status_html = '';

    if (isset($activity_Submission_Obj->date_opened)) {
        if ($activity_Submission_Obj->date_submitted != null
                && $activity_Submission_Obj->date_submitted != '0000-00-00 00:00:00') { // if acitivity submitted by student
            $status_html = '<span style="margin-top: 3px;white-space: nowrap; float: right; font-size: 12px; color: #000;">
                Submitted ' . get_tz_course($activityObj->course_nid, DATE_FORMAT_ACTIVITY_STATUS, $activity_Submission_Obj->date_submitted) . '</span>';
        } else {
            // checking a date_opened
            if ($activity_Submission_Obj->date_opened != null
                    && $activity_Submission_Obj->date_opened != '0000-00-00 00:00:00') {
                // show the last save status
                $status_html = '<span style="margin-top: 7px;float: right; font-size: 12px; color: #999999;">
            Last Saved ' . get_tz_course($activityObj->course_nid, DATE_FORMAT_ACTIVITY_STATUS, $activity_Submission_Obj->date_opened) . '</span>';
            } else {
                $status_html = '';
            }
        }
    }
    return $status_html;
}

function get_student_course_activity_outcomes($course_id, $student_id, $selected_term_id, $outcome_rows=array(), $orderby=NULL, $filters=array(), $split_outcomes = false) {

    $user_role = check_user_permission($course_id);

    if (empty($outcome_rows)) {
        $school_oc = get_course_outcomes($course_id);

        $outcome_rows = array();
        while ($oc = db_fetch_object($school_oc)) {
            if (!empty($oc)) {
                $outcome_rows[] = $oc;
            }
        }
    }

    $role_specific_condition = "";
    $role_specific_fields_condition = "";

    if ($user_role != NTLP_ROLEID_NETWORKMANAGER && $user_role != NTLP_ROLEID_NETWORKSTAFF
            && $user_role != NTLP_ROLEID_TEACHER && (isset($user->roles[NTLP_ROLEID_PARENT]) || $user_role != NTLP_ROLEID_SITEGUEST)) { //for students, parents and non teacher roles
        $role_specific_condition .= " AND ((ntlp_course_activity.available_from <= NOW() AND ntlp_course_activity.available_to >= NOW() ) OR ntlp_course_activity.is_published = 1)";

        $role_specific_fields_condition .= " AND ntlp_course_activity.is_published = 1";
    }


    $query = 'SELECT ntlp_activity_submission.nid as submission_nid, ntlp_course_activity.nid AS activity_id, ntlp_course_activity.due_date AS due_date, node_ntlp_course_activity.title AS activity_title, ntlp_activity_submission.is_final, ntlp_course_activity.is_graded AS not_gradeable, ntlp_activity_submission.date_submitted AS date_submitted, ntlp_activity_submission.version_num AS version_num, ntlp_activity_submission.note_to_teacher AS note_to_teacher, ntlp_course_activity.grading_method AS grading_method, ntlp_course_activity.is_published, gb_grade.is_complete, gb_grade.is_late, (CASE WHEN gb_grade.id IS NULL THEN 0 ELSE 1 END) is_graded, gb_grade.date_graded, gb_grade.teachers_notes ';
    if (sizeof($outcome_rows) > 0) {
        $query .= ", ";
    }
    for ($i = 0; $i < sizeof($outcome_rows); $i++) {
        if ($split_outcomes) {
            $query .= " (CASE WHEN (SELECT COUNT(*) FROM dpl_ntlp_gb_grade g, dpl_ntlp_gb_activity_outcome x, dpl_ntlp_gb_activity_excuse ex WHERE ex.grade_id = g.id AND ex.tid = @OUTCOME_ID AND g.activity_nid = x.activity_nid $role_specific_fields_condition AND g.activity_nid = ntlp_course_activity.nid AND g.user_uid = ntlp_activity_user.user_uid ) THEN 'EX' ELSE (SELECT TRIM(TRAILING '.' FROM TRIM(TRAILING '0' FROM p.points)) FROM dpl_ntlp_gb_activity_grade p, dpl_ntlp_gb_grade g, dpl_ntlp_gb_activity_outcome x WHERE p.grade_id = g.id AND p.tid = x.tid AND g.activity_nid = x.activity_nid $role_specific_fields_condition AND g.activity_nid = ntlp_course_activity.nid AND g.user_uid = ntlp_activity_user.user_uid AND p.tid = @OUTCOME_ID AND x.tid = @OUTCOME_ID ) END ) AS outcome_graded_@OUTCOME_ID, ";
            $query .= "(SELECT x.max_points FROM dpl_ntlp_gb_activity_outcome x WHERE x.activity_nid = ntlp_course_activity.nid AND x.tid = @OUTCOME_ID ) AS outcome_maxscore_@OUTCOME_ID ";
        } else {
            $query .= " (CASE WHEN (SELECT COUNT(*) FROM dpl_ntlp_gb_activity_grade p, dpl_ntlp_gb_grade g, dpl_ntlp_gb_activity_outcome x, dpl_ntlp_gb_activity_excuse ex WHERE p.grade_id = g.id AND ex.grade_id = g.id AND ex.tid = @OUTCOME_ID AND g.activity_nid = x.activity_nid AND g.activity_nid = ntlp_course_activity.nid AND g.user_uid = ntlp_activity_user.user_uid ) THEN 'EX' ELSE (SELECT CONCAT( TRIM(TRAILING '.' FROM TRIM(TRAILING '0' FROM p.points)), ' / ', x.max_points ) FROM dpl_ntlp_gb_activity_grade p, dpl_ntlp_gb_grade g, dpl_ntlp_gb_activity_outcome x WHERE p.grade_id = p.id AND p.tid = x.tid AND g.activity_nid = x.activity_nid $role_specific_fields_condition AND g.activity_nid = ntlp_course_activity.nid AND g.user_uid = ntlp_activity_user.user_uid AND p.tid = @OUTCOME_ID AND x.tid = @OUTCOME_ID ) END ) AS outcome_" . ($i + 1) . " ";
        }
        $query = str_replace('@OUTCOME_ID', $outcome_rows[$i]->tid, $query);
        if ($i + 1 < sizeof($outcome_rows))
            $query .= ", ";
    }

    $query .= "FROM {ntlp_course_activity} ntlp_course_activity  
        INNER JOIN {node} node_ntlp_course_activity ON ntlp_course_activity.nid = node_ntlp_course_activity.nid 
        INNER JOIN {ntlp_activity_user} ntlp_activity_user ON ntlp_course_activity.nid = ntlp_activity_user.activity_nid 
        LEFT JOIN {node} node_ntlp_course_activity_1 ON ntlp_course_activity.project_nid = node_ntlp_course_activity_1.nid 
        LEFT JOIN {ntlp_activity_submission} ntlp_activity_submission ON ntlp_activity_submission.activity_nid = ntlp_course_activity.nid AND ntlp_activity_submission.user_uid = %d AND ntlp_activity_submission.is_final = 1
        LEFT JOIN {ntlp_gb_grade} gb_grade ON gb_grade.activity_nid = ntlp_course_activity.nid AND gb_grade.user_uid = %d
        WHERE ntlp_activity_user.user_uid = %d AND ntlp_course_activity.course_nid = %d 
        AND ntlp_course_activity.school_term_tid = %d
        $role_specific_condition";


    if (empty($orderby)) {
        $query .= " ORDER BY ntlp_course_activity.due_date DESC";
    } else {
        $query .= " ORDER BY $orderby";
        $query .= ($orderby == 'is_published' ? "  ntlp_course_activity.due_date DESC" : "");
    }
    watchdog('gb_query', $query . '<br>' . "$student_id, $student_id, $student_id, $course_id, $selected_term_id");

    $rs = db_query($query, $student_id, $student_id, $student_id, $course_id, $selected_term_id);

//    watchdog("get_student_course_activity_outcomes", $query);
    return $rs;
}

function get_course_activity_last_graded_date($selected_term_id, $course_id, $student_id) {

    global $user;

    $query = "/* SLAVE */SELECT g.date_graded FROM {ntlp_gb_grade} g
                INNER JOIN {ntlp_course_activity} ca ON ca.nid = g.activity_nid
                WHERE	g.graded_by_uid = %d
                AND	ca.course_nid=%d
                AND	ca.school_term_tid=%d
                AND	g.user_uid = %d";

    $rs = db_query($query, $user->uid, $course_id, $selected_term_id, $student_id);

    return $rs;
}

function get_activity_students_last_graded_date($activity_id) {
    global $user;

    $query = "/* SLAVE */SELECT date_graded FROM {ntlp_gb_grade}
            WHERE graded_by_uid = %d AND activity_nid = %d
            ORDER BY date_graded DESC
            LIMIT 1";

    $rs = db_query($query, $user->uid, $activity_id);

    return $rs;
}

function get_user_submitted_evaluate_ids($activity_id, $user_uid) {

    $submission_id = array();

    $rs = db_query("SELECT DISTINCT s.submission_nid,n.uid, nars.exclude_feedback, s.feedback_for
        FROM dpl_ntlp_activity_rubric_category c
        INNER JOIN dpl_ntlp_activity_rubric_submission s ON s.category_id = c.category_ID
        INNER JOIN dpl_ntlp_activity_rubric_submission_for nars ON nars.submission_nid = s.submission_nid
        INNER JOIN dpl_node n  ON n.nid = s.submission_nid
        WHERE c.activity_nid = %d AND n.uid = %d", $activity_id, $user_uid);

    while ($data = db_fetch_object($rs)) {

        $submission_id[$data->submission_nid] = $data;
    }


    return $submission_id;
}

function get_user_submitted_feedback_for_another_user($activity_nid, $feedback_reciever_uid, $feedback_submitter_uid, $submission_id) {
    $rs = db_query("SELECT rc.category_ID, rc.category_name, rc.max_points, rcs.*,
        rcsf.comment_to_teacher, rcsf.exclude_feedback
        FROM dpl_ntlp_activity_rubric_category rc
        INNER JOIN dpl_ntlp_activity_rubric_submission rcs ON rcs.category_id = rc.category_ID
        INNER JOIN dpl_ntlp_activity_rubric_submission_for rcsf ON rcsf.submission_nid = rcs.submission_nid
        INNER JOIN dpl_node n ON n.nid = rcs.submission_nid
        WHERE rc.activity_nid = %d AND rcs.feedback_for = %d AND rcs.submission_nid = %d AND n.uid = %d",
                    $activity_nid, $feedback_reciever_uid, $submission_id, $feedback_submitter_uid);

    return $rs;
}

function get_user_submitted_feedback_cat_avg_score($feedback_for, $activity_nid) {

    $rs = db_query('SELECT arc.category_ID, arc.category_name, AVG(ars.score) score, arc.max_points
        FROM dpl_ntlp_activity_rubric_category arc
        INNER JOIN dpl_ntlp_activity_rubric_submission ars ON ars.category_id = arc.category_ID
        INNER JOIN dpl_ntlp_activity_rubric_submission_for arns ON arns.submission_nid = ars.submission_nid
        INNER JOIN dpl_node n ON n.nid = ars.submission_nid
        WHERE arns.exclude_feedback = 0 AND n.uid = %d AND arc.activity_nid = %d
        GROUP BY arc.category_ID', $feedback_for, $activity_nid);
    return $rs;
}

function get_user_submitted_feedback_avg_score($feedback_by, $activity_nid) {

    $rs = db_query('SELECT AVG(score) AS avg_score, COUNT(score) AS score_count FROM
        (SELECT SUM(ars.score) score
        FROM dpl_ntlp_activity_rubric_category arc
        INNER JOIN dpl_ntlp_activity_rubric_submission ars ON ars.category_id = arc.category_ID
        INNER JOIN dpl_ntlp_activity_rubric_submission_for arns ON arns.submission_nid = ars.submission_nid
        INNER JOIN dpl_node n ON n.nid = ars.submission_nid
        WHERE arns.exclude_feedback = 0 AND n.uid = %d AND arc.activity_nid = %d
        GROUP BY ars.feedback_for) AS stu_scores',
                    $feedback_by, $activity_nid);
    return db_fetch_object($rs);
}

function get_user_feedback_uid($user_id, $activity_users, $skip) {
    if ($activity_users != null) {
        foreach ($activity_users as $key => $value) {

            if ($value == $user_id) {
                $auid = $activity_users[$key + $skip];
                break;
            }
        }
        return $auid;
    }
    return "";
}

function get_course_activities_detail($selected_term_id, $course_id, $user_role, $order_by=null) {
    $query = "SELECT ca.nid, ca.course_nid, ca.due_date, n.type AS node_type, n.nid AS node_nid, n.title AS node_title,  ca.is_graded AS is_graded, ca.submission_type,
                (SELECT (SUM(CASE WHEN s.version_num >= '1' AND s.is_final = 1 THEN 1 ELSE 0 END)/(SELECT COUNT(*) FROM dpl_ntlp_activity_user au WHERE au.activity_nid = ca.nid))*100
                    FROM {ntlp_activity_submission} s
                INNER JOIN dpl_ntlp_course_activity a ON s.activity_nid = a.nid
                INNER JOIN dpl_ntlp_activity_user au ON s.activity_nid = au.activity_nid AND au.user_uid = s.user_uid
                INNER JOIN dpl_ntlp_course_user cu ON a.course_nid = cu.course_nid AND cu.user_uid = s.user_uid
                WHERE s.activity_nid = ca.nid GROUP BY s.activity_nid) AS submissions_percent,
                ca.available_from AS available_from, ca.available_to AS available_to, ca.is_published AS is_published, ca.published_date 
            FROM dpl_ntlp_course_activity ca
            INNER JOIN dpl_node n ON ca.nid = n.nid
            WHERE (ca.course_nid = %d)
            AND (ca.school_term_tid = %d) " .
            (($order_by != null) ? "ORDER BY $order_by, node_title ASC" : "node_title ASC");

    $rs = db_query($query, $course_id, $selected_term_id);

    return $rs;
}

function get_user_recieved_evaluation_records($activity_nid, $feedback_for) {

    $rs = db_query("SELECT SUM(arc.max_points) max_points, SUM(ars.score) score, ars.submission_nid,
        narsf.exclude_feedback
      FROM {ntlp_activity_rubric_category} arc
        INNER JOIN {ntlp_activity_rubric_submission} ars ON ars.category_id = arc.category_ID
        INNER JOIN {ntlp_activity_rubric_submission_for} narsf ON narsf.submission_nid = ars.submission_nid
        INNER JOIN dpl_node n ON n.nid = ars.submission_nid
      WHERE ars.feedback_for = %d AND arc.activity_nid = %d
      GROUP BY n.uid", $feedback_for, $activity_nid);

    while ($data = db_fetch_object($rs)) {
        $arr[] = $data;
    }

    return $arr;
}

function _get_rubric_submission_last_submission_date($activity_nid, $student_uid) {
    $rs = db_query("SELECT n.changed as rubric_submitted_date FROM dpl_ntlp_activity_rubric_category arc
        INNER JOIN dpl_ntlp_activity_rubric_submission ars ON ars.category_id = arc.category_ID
        INNER JOIN dpl_ntlp_activity_rubric_submission_for arns ON arns.submission_nid = ars.submission_nid
        INNER JOIN dpl_node n ON n.nid = ars.submission_nid
        WHERE arc.activity_nid = %d AND n.uid = %d ORDER BY n.changed DESC LIMIT 0 , 1;");

    if ($rs->num_rows > 0) {
        $result = db_fetch_object($rs);

        return $result->rubric_submitted_date;
    } else {
        return false;
    }
}


function is_activity_graded($activity_nid) {
    
    $rs = db_query("SELECT id FROM {ntlp_gb_grade} g
            INNER JOIN {ntlp_activity_user} au ON g.user_uid = au.user_uid AND g.activity_nid = au.activity_nid
            WHERE g.activity_nid = %d", $activity_nid);

    return ($rs->num_rows > 0);
}

function get_activity_response($activity_nid) {

    return db_query("SELECT response_type, response_length FROM {ntlp_activity_response} WHERE activity_nid=$activity_nid");
}

function create_activity_submission($school_term_tid, $course_nid, $activity_nid, $type, $body, $uid, $note_to_teacher, $is_final, $version, $reset_old = true) {
    $submission_node = new stdClass();
    $submission_node->title = 'Submission';
    $submission_node->type = $type;   // Your specified content type
    $submission_node->created = time();
    $submission_node->changed = time();
    $submission_node->body = $body;
    $submission_node->status = 1;
    $submission_node->sticky = 0;
    $submission_node->format = 1;       // Filtered HTML
    $submission_node->uid = $uid;

    node_save($submission_node);

    if ($reset_old == true) {
        //Mark all previous submissions as 'old/inactive'
        db_query("UPDATE {ntlp_activity_submission} SET is_final = 0 WHERE activity_nid = %d AND user_uid = %d",
                $activity_nid, $uid);
    }
    if ($is_final == 1) {
        //Mark all previous submissions as 'old/inactive'
        db_query("UPDATE {ntlp_activity_submission} SET is_final = 0 WHERE activity_nid = %d AND user_uid = %d",
                $activity_nid, $uid);
        $date_submitted = '"' . strftime(DATE_FORMAT, time()) . '"';
    } else {
        $date_submitted = "NULL";
    }
    
    db_query("INSERT INTO {ntlp_activity_submission} (nid, activity_nid, user_uid, 
        date_opened, date_submitted, note_to_teacher, is_final, version_num)
        VALUES (%d, %d, %d, 
        '%s', {$date_submitted}, '%s', %d, '%s')",
            intval($submission_node->nid), intval($activity_nid), intval($uid), 
            strftime(DATE_FORMAT, time()), $note_to_teacher, $is_final, $version);

            
    //update block cache block for TEACHERS of the course
    ntlp_update_block_cache('ntlp_course', $course_nid, NTLP_ROLEID_TEACHER);
    
    return $submission_node;
}

function get_graded_students_on_activity($activity_nid) {

    $result = db_query("SELECT * FROM {ntlp_gb_grade} WHERE activity_nid = %d", $activity_nid);
    return $result;
}
