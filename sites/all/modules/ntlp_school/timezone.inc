<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


/**
 * Description of timezone
 *
 * @author Junaid
 */
function get_tz($school_nid, $dtfmt, $datetime) {
    return convert_datetime_tz($school_nid, $datetime, true, $dtfmt);
}

function get_tz_course($course_nid, $dtfmt, $datetime) {
    $school_nid = get_course_school_nid($course_nid);
    return convert_datetime_tz($school_nid, $datetime, true, $dtfmt);
}

function set_tz($school_nid, $dtfmt, $datetime) {
    return convert_datetime_tz($school_nid, $datetime, false, $dtfmt);
}

function convert_datetime_tz($school_nid, $datetime, $get, $dtfmt=null) {
    /* JR101006
     * $get = Get Value from the DB Server for select Statement,
     * $time= Return with Time value
     */

    $dtfmt = ($dtfmt == null ? "Y-m-d g:ia" : $dtfmt);

    if (is_numeric($datetime))
        $datetime = date('Y-m-d H:i:s', $datetime);

    if (strlen($school_nid)==0) {     //!isset($school_nid))
	return date($dtfmt, strtotime($datetime));
    }

    $different = 0;

    if ($get) {
        $different = "+";
        $sdifferent = "-";
//        $datetime = date("Y-m-d H:i:s", $datetime);
    } else {
        //$datetime = date_format()
        $different = "-";
        $sdifferent = "+";
        $datetime = date("Y-m-d H:i:s", strtotime($datetime));
    }

//    $datetime = date($dtfmt, strtotime($datetime . "+01:00:00"));
//    $offset = explode(':', "-01:00:00");
    $offset = explode(':', get_school_tz_offset($school_nid));
    $offset = ($offset[0] * 3600) + ($offset[1] * 60) + $offset[2];     //HH + MM + SS
    if ($get) {
        $datetime = date($dtfmt, strtotime($datetime) + $offset);
    } else {
        $datetime = date($dtfmt, strtotime($datetime) - $offset);
    }
    return $datetime;
    
    
//    $is_dst = variable_get('date_daylight', 0);
//
//    $qry = "SELECT (('%s') %s INTERVAL IF(%d, tz.offset_dst, tz.offset) HOUR_SECOND) AS cnvDate
//    FROM {ntlp_school} s INNER JOIN {event_timezones} tz ON tz.timezone = s.local_timezone
//    WHERE s.nid = %d";
//
////    $qry = "SELECT (('$datetime') $different INTERVAL IF(tz.is_dst, tz.offset_dst, tz.offset) HOUR_SECOND
////$sdifferent  INTERVAL $shift SECOND) AS cnvDate FROM dpl_ntlp_school   s
////INNER JOIN  dpl_event_timezones tz ON tz.timezone = s.local_timezone  WHERE s.nid = 0" . $school_nid;
//
//    $squery = db_query($qry, $datetime, $different, $is_dst, $school_nid);
//
//    $result = db_fetch_object($squery);
//    if (isset($result)) {
//        $datetime = $result->cnvDate;
//        $datetime = date("Y-m-d H:i:s", strtotime($datetime));
//        //format("Y-m-d H:i:s");
//        //echo date("m/d/Y H:i:s",strtotime($t ));
//    }
//
////    drupal_set_message($datetime);
//
//
//    if ($get == true) {
//        list($year, $month, $day, $hour, $minute, $second) = explode("-", str_replace(array(" ", ":"), "-", $datetime));
//        $datetime = mktime($hour, $minute, $second, $month, $day, $year);
//        $datetime = date(($dtfmt == null ? 'm/d/Y' : $dtfmt), $datetime);
//        //$datetime = date('m/d/Y - g:ia',$datetime);
//    }
//
//    return $datetime;
}

function get_tz_offset($school_nid){

    $off_set ='00:00:00';
    $is_dst = variable_get('date_daylight', 0);

    $qry = "SELECT IF(%d, tz.offset_dst, tz.offset) as off_set
        FROM {ntlp_school} s INNER JOIN {event_timezones} tz ON tz.timezone = s.local_timezone
        WHERE s.nid = %d";
    
    if($school_nid != 0){
        $squery = db_query($qry, $is_dst, $school_nid);
        $result = db_fetch_object($squery);
        if (isset($result)) {
            $off_set = $result->off_set;
        }
    }
    return $off_set;
}

function set_tz_school($timezone, $school_nid) {
    $old_timezone = get_tz_offset($school_nid);
    $result = db_query('update {ntlp_school} SET local_timezone = %d where nid = %d', $timezone, $school_nid);
    $new_timezone = get_tz_offset($school_nid);
    $result = db_query("update {ntlp_school_term} t SET t.date_started = ((t.date_started) - INTERVAL '$new_timezone' HOUR_SECOND + INTERVAL '$old_timezone' HOUR_SECOND),t.date_closed=  ((t.date_closed) - INTERVAL '$new_timezone' HOUR_SECOND + INTERVAL '$old_timezone' HOUR_SECOND) where school_nid = $school_nid ");
    return $result;
}

function get_school_tz_offset($school_nid) {
    $rebuild = true;
    $tz_cache = cache_get('ntlp_timezone');
    if ($tz_cache) {
        $tz_data = $tz_cache->data;
//    echo "Cache size:". sizeof($tz_data) . ', Is NULL:' . ($tz_data == NULL ? 'Yes' : 'No');
//    print_r($tz_data);
        if (sizeof($tz_data) > 0 && $tz_data != NULL) {
            $rebuild = false;
        }
    }
    if ($rebuild) {
//        print_r("Rebuilding Cache...");
        $qry = "SELECT s.nid school_nid, tz.*
        FROM {ntlp_school} s INNER JOIN {event_timezones} tz ON tz.timezone = s.local_timezone";

        $squery = db_query($qry);
        unset($tz_data);
        while ($result = db_fetch_object($squery)) {
            $tz_data[$result->school_nid] = $result;
        }
        cache_set('ntlp_timezone', $tz_data);
    }

    $is_dst = variable_get('date_daylight', 0);
    if ($is_dst) {
        $offset = $tz_data[$school_nid]->offset_dst;
    }
    else
    {
        $offset = $tz_data[$school_nid]->offset;
    }
//    print_r("School:$school_nid, Value:".$offset.", Is DST:".$is_dst);
//    print_r($tz_data[$school_nid]);
    return $offset;
}