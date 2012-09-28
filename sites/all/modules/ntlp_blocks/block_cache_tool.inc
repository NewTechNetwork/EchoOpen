<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_blocks', 'block_cache_tool');

/**
 * Implementation of hook_cron().
 */
// Get all courses and generate their enrollments and grades in DW table
//function ntlp_blocks_cron() {
//    //Disable cron
//    return;
//
//    global $user;
//    $start_time = time();
////    watchdog('cron', 'Block cache rebuilding started!');
//
//    //Get active users, whose blocks are already generated
//    $crs = db_query("SELECT u.uid FROM {users} u, {ntlp_user} nu, {ntlp_dw_block} b
//        WHERE u.uid = nu.uid AND u.uid = b.uid
//        AND u.status = 1 AND b.update_on = 0 AND nu.password_change = 0 AND nu.is_active = 1
//        GROUP BY u.uid LIMIT 0, 200");
//
////    //Reset when very small number of records left to process
////    if ($crs->num_rows < 100) {
////        //Reset event blocks so that they are rebuilt, to make sure we get updated data after date rollover.
////        db_query("UPDATE {ntlp_dw_block} SET update_on = 0 WHERE bid = 'recent_events_block_contents'");
////    }
//
//    $i = 0;
//    $old_user = $user->uid;
//    $old_school_nid = $_SESSION['NTLP_THIS_SCHOOL'];
////    $featured_block_html = _ntlp_home_page_feature_content();
//
//    while ($crec = db_fetch_object($crs)) {
//        $user = user_load(array('uid' => $crec->uid));
//        $school_nid = get_user_school($user->uid);
//        $_SESSION['NTLP_THIS_SCHOOL'] = $school_nid;
//
//        $events_block_html = recent_events_block_contents();
//        $groups_block_html = recent_groups_block_contents();
//        $course_block_html = mycourses_block_contents();
////        cache_set('recent_events_block_contents:' . $user->uid, $events_block_html, 'ntlp_cache', time() + (60*6));
////        cache_set('recent_groups_block_contents:' . $user->uid, $groups_block_html, 'ntlp_cache');
////        cache_set('mycourses_block_contents:' . $user->uid, $course_block_html, 'ntlp_cache');
//
//        db_query("UPDATE {ntlp_dw_block} SET block_body = '%s', update_on = 1 WHERE uid = %d AND bid = 'recent_events_block_contents'",
//                $events_block_html, $user->uid);
//        db_query("UPDATE {ntlp_dw_block} SET block_body = '%s', update_on = 1 WHERE uid = %d AND bid = 'recent_groups_block_contents'",
//                $groups_block_html, $user->uid);
//        db_query("UPDATE {ntlp_dw_block} SET block_body = '%s', update_on = 1 WHERE uid = %d AND bid = 'mycourses_block_contents'",
//                $course_block_html, $user->uid);
////        db_query("UPDATE {ntlp_dw_block} SET block_body = '%s', update_on = 1 WHERE uid = %d AND bid = '_ntlp_home_page_feature_content'",
////                $featured_block_html, $user->uid);
//
//        $i ++;
////        if ($i >= 500) break;
//    }
//    $_SESSION['NTLP_THIS_SCHOOL'] = $old_school_nid;
//    $user = user_load(array('uid' => $old_user));
////    watchdog('cron', 'Block cache rebuilt for '.$i.' existing users.');
//    $end_time = time();
//    print '<p>Cache built for '.$i.' users in '.time_duration($end_time - $start_time).'</p>';
//}

function ntlp_block_cache($string, $block_type) {
//$html_content = $string();
//return $html_content;

    global $user;
    $cache_vusers_obj = cache_get($string.':VALID_USERS', 'ntlp_cache');
    $cache_obj = 0;
    if ($cache_vusers_obj) {
        $cache_vusers = $cache_vusers_obj->data;
    }
//    watchdog('cachevu', 'GET1 >'.$string.':'.$block_type.':'. '<br>' . print_r($cache_vusers, true));
    //If user id exists in Valid_Users then fetch html block
    if (is_array($cache_vusers)) {
        if (array_key_exists($user->uid, $cache_vusers)) {
            if ($cache_vusers[$user->uid] == 1) {
                $cache_obj = cache_get($string.':'.$user->uid, 'ntlp_cache');
            }
        }
    }
//    watchdog('cachevu', 'GET >'.$string.':'.$block_type.':'. '<br>' . print_r($cache_vusers, true));
//    watchdog('cache', 'GET >'.$string.':'.$block_type.':'.$user->uid . '<br>' . print_r($cache_obj, true));
            //Disabling Ajax
//            $html_content = '<script>
//
//                function update_' . $string . '(){
//                     $.ajax({
//                            type: "GET",
//                            url: "?q=ntlp/render/update/block/' . $string . '",
//                            success:  function(data){
//                                //$("#' . $string . '").html(data);
//                                var block = document.getElementById("'.$string.'");
//                                block.innerHTML = data;
//                            }
//                     });
//                }
//
//                $(document).ready (function (){
//                    setTimeout("update_' . $string . '()", 7000);
//                });
//                </script>';
//

    if ($cache_obj) {
        $html_content = '<!-- cached -->'.$cache_obj->data;
        } else {
        $html_content = $string();
        //Put in cache and expire in 6 hours
        cache_set($string.':'.$user->uid, $html_content, 'ntlp_cache', time() + (6*60*60));
//        watchdog('cache', 'SET >'.$string.':'.$block_type.':'.$user->uid . '<br>' . $html_content);

        $cache_vusers[$user->uid] = 1;      //Mark that cache is valid
        cache_set($string.':VALID_USERS', $cache_vusers, 'ntlp_cache');
    }
    return $html_content;
}

//function ntlp_ajax_block_render_func($string) {
////    watchdog('ajax_block', 'ntlp_ajax_block_render_func' . $string);
//    global $user;
//    $html_content = $string();
////    db_query("UPDATE {ntlp_dw_block} SET block_body = '%s', update_on = 1
////            WHERE uid = %d AND bid = '%s' ", $html_content, $user->uid, $string);
//    echo $html_content;
//}

function ntlp_update_block_cache($block_type, $nid = 0, $for_role = 0) {
    global $user;

    $school_nid = get_user_school($user->uid);
    $clear_all = false;

    //Using array so that we can release/clear multiple blocks for one block_type
    $blocks = array();
//                $blocks[] = 'recent_events_block_contents';
//                $blocks[] = 'recent_groups_block_contents';
//                $blocks[] = 'mycourses_block_contents';

    if ($school_nid != 0) {

        switch ($block_type) {

            case 'ntlp_course':
                $blocks[] = 'mycourses_block_contents';

                if ($nid != 0) {
                    if ($for_role != 0) {
                        $rs = db_query("SELECT user_uid uid FROM {ntlp_course_user} WHERE course_nid = %d AND role_id = %d", $nid, $for_role);
                    } else {
                        $rs = db_query("SELECT user_uid uid FROM {ntlp_course_user} WHERE course_nid = %d", $nid);
                    }
                } else {
                    if ($for_role != 0) {
                        $rs = db_query("SELECT nu.uid FROM {ntlp_user} nu, {users_roles} ur WHERE nu.uid = ur.uid AND nu.school_nid = %d AND ur.rid = %d", $school_nid, $for_role);
                    } else {
                        $rs = db_query("SELECT uid FROM {ntlp_user} WHERE school_nid = %d", $school_nid);
                    }
                }
                break;

            case 'ntlp_event':
                $blocks[] = 'recent_events_block_contents';

                if ($nid != 0) {
                    $record = db_fetch_object(db_query("SELECT type FROM dpl_node WHERE nid = %d", $nid));

                    switch ($record->type) {

                        case 'ntlp_school_event' :

                            $rs = db_query("SELECT u.uid FROM dpl_ntlp_user u, dpl_ntlp_events e
                                WHERE u.school_nid = e.school_nid AND e.nid = %d", $nid);
                            break;

                        case 'ntlp_group_event' :

                            $rs = db_query("SELECT u.uid FROM dpl_og_uid u, dpl_ntlp_events e
                                WHERE u.nid = e.group_nid AND e.nid = %d", $nid);

                            break;

                        case 'ntlp_agenda' :

                            $rs = db_query("SELECT u.user_uid uid FROM dpl_ntlp_course_user u, dpl_ntlp_events e
                                WHERE u.course_nid = e.group_nid AND e.nid = %d", $nid);

                            break;

                        case 'ntlp_network_event' :
                            $clear_all = true;
                            break;
                    }
                }
                break;

            case 'ntlp_group':
                $blocks[] = 'recent_groups_block_contents';
                $rs = db_query("SELECT uid FROM dpl_og_uid WHERE nid = %d", $nid);
                break;
        }

        //Clear cache cells of selected users
        if ($clear_all) {
//            watchdog('cache', 'CLEAR ALL >'.$block_type);
//            cache_clear_all('*', 'ntlp_cache', TRUE);         //Do not use cache_clear_all, better way of clearing is to use an array of valid users-blocks
            foreach ($blocks as $b) {
                cache_set($b.':VALID_USERS', array(), 'ntlp_cache');
    }
        } else {
            $cells = '';
            //Invalid users
            $ivusers = array();
            while ($rec = db_fetch_object($rs)) {
                $ivusers[$rec->uid] = 0;
            }
            //Valid users list from cache
            foreach ($blocks as $b) {
                $cache_vusers_obj = cache_get($b.':VALID_USERS', 'ntlp_cache');
                if ($cache_vusers_obj) {
                    $cache_vusers = $cache_vusers_obj->data;
                }
                if (is_array($cache_vusers)) {
                    //Remove invalid users from cache cell
                    $vusers = $ivusers + $cache_vusers;     //Merge array; left array will be used to overwrite values
                    cache_set($b.':VALID_USERS', $vusers, 'ntlp_cache');
                    $cells .= $b.':'.$block_type.':<br>'.print_r($ivusers, true).'<br>'.print_r($cache_vusers, true).'<br>'.print_r($vusers, true);
                }
            }

//            watchdog('cache', 'CLEAR >'.$block_type .' >'. $rs->num_rows . '<br>'.$cells);
        }
    }
}

/**
 * A function for making time periods readable
 *
 * @author      Aidan Lister <aidan@php.net>
 * @version     2.0.1
 * @link        http://aidanlister.com/2004/04/making-time-periods-readable/
 * @param       int     number of seconds elapsed
 * @param       string  which time periods to display
 * @param       bool    whether to show zero time periods
 */
function time_duration($seconds, $use = null, $zeros = false)
{
    // Define time periods
    $periods = array (
        'years'     => 31556926,
        'Months'    => 2629743,
        'weeks'     => 604800,
        'days'      => 86400,
        'hours'     => 3600,
        'minutes'   => 60,
        'seconds'   => 1
        );

    // Break into periods
    $seconds = (float) $seconds;
    $segments = array();
    foreach ($periods as $period => $value) {
        if ($use && strpos($use, $period[0]) === false) {
            continue;
        }
        $count = floor($seconds / $value);
        if ($count == 0 && !$zeros) {
            continue;
        }
        $segments[strtolower($period)] = $count;
        $seconds = $seconds % $value;
    }

    // Build the string
    $string = array();
    foreach ($segments as $key => $value) {
        $segment_name = substr($key, 0, -1);
        $segment = $value . ' ' . $segment_name;
        if ($value != 1) {
            $segment .= 's';
        }
        $string[] = $segment;
    }

    return implode(', ', $string);
}