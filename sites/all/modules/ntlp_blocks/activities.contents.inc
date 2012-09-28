<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');

function activities_block_contents() {
    include "activities.template.inc";
    global $user;
    $user_id = $user->uid;

    if ($courseid == 0) {
        //Getting Query String From URL
        $courseid = 0;
        $url = $_GET['q'];
        $args = explode('/', $url);
        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i])) {
                $courseid = $args[$i];
                break;
            }
        }
    }

    set_item_url_handler('Activities');

    if (isset($user->roles[NTLP_ROLEID_PARENT])) {  // if parent come here show their student record
        $user_id = $_SESSION['PARENT_STUDENT_UID'];
        if (!check_user_exist_in_course($user_id, $courseid)) {
            drupal_goto("ntlp/courses/my");
        }
    }

    return render_myactivities($courseid, 'all', $user_id);
}

function render_myactivities($courseid, $project_id, $user_id, $activity_types='all') {
    include 'activity_template.inc';
    global $user;

    $url = $_GET['q'];
    $args = explode('/', $url);
    for ($i = 0; $i < count($args); $i++) {
        if (is_numeric($args[$i])) {
//            if ($last_string_arg == 'node')
            $courseid = $args[$i];
            break;
        }
    }

    if (isset($user->roles[NTLP_ROLEID_PARENT])) {  // if parent come here show their student record
        $user_role = NTLP_ROLEID_STUDENT;
    } else {
        $user_role = check_user_permission($courseid);
    }
//    echo 'user role '.$user_role.' user uid '.$user_id;
    //ACTIVITY FILTER START
    $filterOpts = array(
        'Status' => array(
            'All' => array(
                'type' => 'checkbox',
                'group' => 'act_type',
                'value' => 1,
                'name' => 'all',
                'default' => true,
            ),
            'Task' => array(
                'type' => 'checkbox',
                'group' => 'act_type',
                'value' => 'task',
                'name' => 0,
            ),
            'Journal' => array(
                'type' => 'checkbox',
                'group' => 'act_type',
                'value' => 'journal',
                'name' => 1,
            ),
            'Image' => array(
                'type' => 'checkbox',
                'group' => 'act_type',
                'value' => 'image',
                'name' => 2,
            ),
            'Workshop' => array(
                'type' => 'checkbox',
                'group' => 'act_type',
                'value' => 'workshop',
                'name' => 3,
            ),
            'Quiz' => array(
                'type' => 'checkbox',
                'group' => 'act_type',
                'value' => 'quiz',
                'name' => 4,
            ),
            'Poll' => array(
                'type' => 'checkbox',
                'group' => 'act_type',
                'value' => 'poll',
                'name' => 5,
            ),
        ),
    );

    $url = '?q=ntlp/courses/activities/filter/callback/' . $courseid;
    $url = $project_id ? $url . '/' . $project_id : $url;

    $opts = render_custom_filter(
                    $filterOpts,
                    array('start' => $HTMLCoursesProjectsActivities_filter_cellStart, 'end' => $HTMLCoursesProjectsActivities_filter_cellEnd),
                    array('template' => $HTMLCoursesProjectsActivities_filter_heading, 'placeholder' => '@HEADING'),
                    $HTMLCoursesProjectsActivities_filter_row_end,
                    $url,
                    'activityTableHolder'
    );

    if ($project_id > 0)
        $output = $HTMLCoursesProjectsActivities;
    else
        $output = $HTMLCoursesActivities;
    $opts = str_replace('@OPTIONS', $opts, $HTMLCoursesProjectsActivities_filter);

    // ACTIVITY FILTER END
    $output = str_replace('@FILTER_PLACEHOLDER', $opts, $output);

//    $output = t('You do not have the required permissions.');
    $display_id = 'default';
//    $output = $HTMLcontainer;
//    if (user_access('allow submission for an activity')) {
//    echo $user_role;
    if (isset($user_role) && $user_role == NTLP_ROLEID_STUDENT) {

        $view = views_get_view('ntlp_activities_student');

        $cols = get_course_outcomes($courseid);
        while ($col = db_fetch_object($cols)) {
            $id[] = $view->add_item($display_id, 'field', 'ntlp_course_activity', 'outcome_tid',
                            array('outcome_tid' => $col->tid, 'outcome_name' => $col->name));
        }

//        echo 'course id '.$courseid, 'current_semester '.get_current_selected_term().' student '.$user_id. 'project id '.$project_id;
        if (isset($view)) {
            $output = str_replace(
                            '@ACTIVITY_TABLE_PLACEHOLDER',
                            $view->preview($display_id, array($courseid, $project_id, $user_id, 'S', get_current_selected_course_term())) . '</div>',
                            $output
            );
        }

        $output = str_replace('@LINK_EXISTING', '', $output);
        $output = str_replace('@LINK_NEW', '', $output);
    } else {
//        if (user_access('grade submission')) {
        if (isset($user_role) && $user_role == NTLP_ROLEID_TEACHER
                || $user_role == NTLP_ROLEID_NETWORKMANAGER
                || $user_role == NTLP_ROLEID_SITEADMIN
                || $user_role == NTLP_ROLEID_SCHOOLLEADER
                || $user_role == NTLP_ROLEID_NETWORKSTAFF
                || $user_role == NTLP_ROLEID_SITEGUEST) {
//           $view_activities = views_embed_view('ntlp_activities', 'default', $courseid);

            $view = views_get_view('ntlp_activities');
//            $view->set_display($display_id);
//            $view->set_arguments(array($courseid));
//            $view_activities = "<div>Teacher's view</div>";

            if (isset($view)) {
                $output = str_replace(
                                '@ACTIVITY_TABLE_PLACEHOLDER',
                                $view->preview($display_id, array($courseid, $project_id, 'all', 'all', get_current_selected_course_term($courseid))),
                                $output
                );
            } else {
                $output = str_replace('@ACTIVITY_TABLE_PLACEHOLDER', '', $output);
            }

            if ($project_id > 0) {

                if (isset($user_role) && $user_role == NTLP_ROLEID_TEACHER
                        || $user_role == NTLP_ROLEID_NETWORKMANAGER
                        || $user_role == NTLP_ROLEID_SITEADMIN
                        || $user_role == NTLP_ROLEID_SCHOOLLEADER
                        || $user_role == NTLP_ROLEID_NETWORKSTAFF) {

                    $output = str_replace('@LINK_EXISTING', l('Add Existing Activity', 'ntlp/courses/activity/include/' . $courseid . '/' . $project_id), $output);
                    $output = str_replace('@LINK_NEW', l('Add New Activity', 'ntlp/courses/activity/new/' . $courseid), $output);
                } else {
                    $output = str_replace('@LINK_EXISTING', '', $output);
                    $output = str_replace('@LINK_NEW', '', $output);
                }
            } else {
                $output = str_replace('@LINK_EXISTING', '', $output);

                if (isset($user_role) && $user_role == NTLP_ROLEID_TEACHER
                        || $user_role == NTLP_ROLEID_NETWORKMANAGER
                        || $user_role == NTLP_ROLEID_SITEADMIN
                        || $user_role == NTLP_ROLEID_SCHOOLLEADER
                        || $user_role == NTLP_ROLEID_NETWORKSTAFF) {

                    $output = str_replace('@LINK_NEW', l('Add New Activity', 'ntlp/courses/activity/new/' . $courseid), $output);
                } else {
                    $output = str_replace('@LINK_NEW', "", $output);
                }
            }
        } else {
            $output = t('You do not have access to view assignments or submit grades.');
        }
    }

//    $output = str_replace('@ACTIVITIES_VIEW', $view_activities, $output);
    return $output;
}


function _view_user_all_activites() {
    return _all_notification(false);
}

function _all_notification($limit = false) {
    module_load_include('inc', 'ntlp_courses_activity', 'data_access');
    global $user;

    if (isset($user->roles[NTLP_ROLEID_PARENT])) {
        $result = get_parent_students($user->uid);

        $student_ids = array();
        while($uid = db_fetch_object($result))
            $student_ids[] = $uid->student_uid;

        if (count($student_ids) > 0)
            $student_ids = implode(",", $student_ids);
        else
            $student_ids = "";
    }

    $my_course_announcement_activities = get_user_course_announcement_activities($user->uid, $student_ids);
    $my_courses_activity_activities = get_user_courses_activity_activities($user->uid);
    $my_course_project_activities = get_user_courses_project_activities($user->uid, $student_ids);
    $my_course_resource_activities = get_user_course_resources_activities($user->uid);
    $my_group_resource_activities = get_user_group_resources_activities($user->uid);
    $comments_on_my_posts_activities = get_user_posts_commented_activities($user->uid);
    $school_events_activities = get_user_school_events_activities($user->uid);
    $network_events_activities = get_user_network_events_activities($user->uid);
    $my_course_resource_liked_activities = get_user_course_resources_liked_activities($user->uid);
    $my_groups_resource_liked_activites = get_user_group_resources_liked_activities($user->uid);
    $my_owned_groups_activities = get_user_owned_group_activities($user->uid);
    $my_course_activity_graded_activities = get_user_courses_activity_grade_activities($user->uid, $student_ids);
    $my_libray_project_exemplary_activities = get_user_library_project_exemplary_activities($user->uid);
    $my_libray_project_archived_activities = get_user_library_project_archived_activities($user->uid);

    $selected_activities =  array_merge_by_key($my_group_resource_activities,
                                        $my_courses_activity_activities,
                                        $my_course_project_activities,
                                        $my_course_announcement_activities,
                                        $my_course_resource_activities,
                                        $comments_on_my_posts_activities,
                                        $school_events_activities,
                                        $network_events_activities,
                                        $my_course_resource_liked_activities,
                                        $my_groups_resource_liked_activites,
                                        $my_owned_groups_activities,
                                        $my_course_activity_graded_activities,
                                        $my_libray_project_exemplary_activities,
                                        $my_libray_project_archived_activities);

    $activities_wo = array();
    $activities_wu = array();
    $notification_days = 7;

    //Get activities excluding the originator of the activity
    if (isset($selected_activities) && count($selected_activities) > 0) {

        $filters = array(
            'aid' => $selected_activities,
            'created' => array('gt' => strtotime("-{$notification_days} days")),
            'uid' => array('exclude' => $user->uid) //exclude the user's own generated events
        );
        
        $limit =  ($limit ? $limit : NULL);
        $activities_wo = activity_get_activity(ACTIVITY_ALL, $filters, $limit);
    }
    
    $activities = _get_activities_group_by_date($activities_wo, $activities_wu, array('SORT' => 'DESC', 'AUTHORITY' => TRUE));

    $attributes = array('class' => 'ntlp_notification_table', 'width' => '100%');
    $output = '';

    if ($limit) {
        $output = "<table>";
        $i = 1;
        foreach ($activities as $date => $actData) {
            foreach ($actData as $act) {
                $activity_message = activity_token_replace($act);

                $output .= '<tr>
                                <td rowspan=2 ><div style="float:left;margin-right:5px;"><img align="left" src="' . is_user_image_exist(get_activity_username($act['data']['uid'])->picture) . '" width="50" height="50" /></div></td>
                                <td style="vertical-align:top">' . $activity_message . '</td>
                            </tr>
                            <tr>
                                <td style="color:#666666">'. ago($act['created']) . '</td>
                            </tr>
                            <tr>
                                <td >&nbsp;</td>
                                <td >&nbsp;</td>
                            </tr>';
                $i++; 
                if ($i > $limit) break;
            }
            if ($i > $limit) break;
        }

        $output .= "</table>";
    }
    else {
        $notification_title = '<div style="min-height:300px;" >';
        foreach ($activities as $date => $actData) {

            $date = date('M j', strtotime($date));
            $rows = array();
            foreach ($actData as $act) {

                $activity_message = activity_token_replace($act);

                $row = array(
                    array('data' => $activity_message),
                );
                $rows[] = $row;
            }

            $output .= '<h1 style="color:#3570AA;background-color:#9ECCE9;padding:5px">' . _get_named_date($date) . '</h1>';
            $output .= theme('table', $display_headers, $rows, $attributes);
        }
         $output .= '</div>'; 
    }

    if (count($activities) <= 0) {
        $output = "No notifications in the last {$notification_days} days.";
    } elseif ($limit) {
        $output .= '<br clear="all" /><div style="text-align: center; line-height: 25px;">' . l('View Recent Notifications', 'ntlp/activity/notifications/all', array('attributes' => array('class' => 'tipsygreenlinks'))) . '</div>';
    }

    return $notification_title . $output;
}

/*
 * @param
 *      $dateIn (date yyyy-mm-dd)
 *
 * return (today, yesterday, else $dateIn)
 */

function _get_named_date($dateIn) {
    $today = strtotime(date('Y-m-d'));
    $yesterday = $today - (24 * 60 * 60);
    $dateInStamp = strtotime($dateIn);

    if ($dateInStamp == $today) {
        $dateIn = 'Today';
    } else if ($dateInStamp >= $yesterday && $dateInStamp <= $today) {
        $dateIn = 'Yesterday';
    }

    return $dateIn;
}

/*
 * @param
 *      $activities Array
 *      Array of activities fetched by activity_get_activity function.
 *
 * @param
 *      $filter Array
 *        - [SORT] = asc, desc
 *          Sort activites by date ASC or DESC.
 *
 * @return
 *      $activities array grouped by date.
 *
 */

function _get_activities_group_by_date($activities_wo, $activities_wu, $filters = array()) {
    global $user;
    $sortfunc = 'ksort';
    if (isset($filters['SORT']) and in_array(strtolower($filters['SORT']), array('asc', 'desc'))) {
        $sfuncs = array('asc' => 'ksort', 'desc' => 'krsort');
        $sortfunc = $sfuncs[strtolower($filters['SORT'])];
    }

    //Merge both type of activities
    $activities_array = array();
    foreach ($activities_wo as $act) {
        $date = date('Y-m-d', $act['data']['created']);
        $time = date('h:i:s a', $act['data']['created']);
        $act['data']['time'] = $time;
        $activities_array[$date][] = $act;
    }
    foreach ($activities_wu as $act) {
        $date = date('Y-m-d', $act['data']['created']);
        $time = date('h:i:s a', $act['data']['created']);
        $act['data']['time'] = $time;
        $activities_array[$date][] = $act;
    }

    //Sort activities in each day
    foreach ($activities_array as $date => $actdata) {
        $sortfunc($activities_array[$date]);
    }
    //Sort days
    $sortfunc($activities_array);
    return $activities_array;
}

function _view_upcoming_activities() {

    include('activities.template.inc');
    $output = $HTML_Upcoming_Activities;

    $url = $_GET['q'];
    $args = explode('/', $url);
    for ($i = 0; $i < count($args); $i++) {
        if (is_numeric($args[$i]) && empty($courseid))
            $courseid = $args[$i];
    }

    $rs = get_upcoming_activities($courseid);

    while ($activity = db_fetch_object($rs)) {

        if (strlen($activity->title) > 28) {
            $truc_text = '...';
        } else {
            $truc_text = '';
        }
        if ($activity->due_date != '0000-00-00 00:00:00') {
            $duedate = get_tz_course($courseid, NTLP_DATE_TZ_MJY, $activity->due_date);
        } else {
            $duedate = 'Fix';
        }
        $upcoming_activity .= '<li><a href="?q=ntlp/courses/activity/view/'.$courseid.'/'.$activity->nid.'" >'.substr($activity->title, 0, 28).$truc_text.'</a>
            '.(($activity->project_title != null) ? '<br><span style="color: #000; font-size: 11px;">'.$activity->project_title.'</span>' : '').
        '<br><span style="color: #3570aa; font-size: 75%">Due Date: '.$duedate.' </span>
        </li>';
    }

    if ($upcoming_activity == '')
        $upcoming_activity = '<div style="margin:10px">No upcoming activities</div>';

    $output = str_replace('@LIST_UPCOMING_ACTIVITIES',$upcoming_activity, $output);

    return $output;


}


//
//function activity_table($activities) {
//    global $user;
//  $display_headers = array(
//    'created' => array('field' => 'created', 'data' => t('Date')),
//    t('Message'),
//    t('Node Type'),
//    t('User'),
//    t('Link'),
//  );
//
//  $rows = array();
//  foreach ($activities as $activity) {
//
//    if ($activity_message = activity_token_replace($activity)) {
//
//
//      $row = array(
//        array('data' => theme('activity_timestamp', $activity['created']), 'class' => 'activity-table-timestamp'),
//        array('data' => $activity_message ),
//        array('data' => theme_activity_node_type($activity['type']) ),
//        array('data' => theme_activity_username($user)),
//
//        array('data' => $activity['type'] == 'lo_journal'? str_replace( 'node/','ntlp/courses/activity/view/1/',$activity_message): ''),
//
//      );
//      $rows[] = $row;
//    }
//  }
//  $output  = theme('table', $display_headers, $rows, array('class' => 'activity-table'));
//  $output .= theme('pager');
//
//  return $output;
//}
//
//
//function my_theme_activity($message, $item) {
//  // $item is the unprocessed activity item so that themers can do more with it.
//  return $item['mark'] .'<span class="activity activity-module-'. $item['module'] .' activity-type-'. $item['type'] .' activity-operation-'. $item['operation'] .'">'. $message .'</span>';
//}

function get_user_course_announcement_activities($user_id, $student_ids = "") {

    $aid = array();

//    //check user controlled exclusion list
//    $result = db_query("SELECT * FROM {ntlp_notification_user_web_exclusions}
//            WHERE user_uid = {$user_id} AND notify_event_id=1;");
//
//    if ($result->num_rows > 0) {
//        return $aid;
//    }

    $sql = "SELECT DISTINCT ac.aid FROM {ntlp_course_user} cu
            INNER JOIN {ntlp_activity_context} ac ON cu.course_nid = ac.cid
            INNER JOIN {activity} a ON ac.aid=a.aid
            WHERE a.type='ntlp_announcement'
            AND cu.user_uid " . (!empty($student_ids) ? " IN ({$student_ids}) " : " = {$user_id}") . "
            AND cu.role_id IS NOT NULL";

    $result = db_query($sql);
    while ($row = db_fetch_object($result)) {
        $aid[] = $row->aid;
    }

    return $aid;
}

function get_user_courses_activity_activities($user_id, $student_ids = "") {

    $aid = array();

    $sql = "SELECT DISTINCT ac.aid FROM {ntlp_activity_user} au
            INNER JOIN {ntlp_activity_context} ac ON au.activity_nid = ac.cid
            INNER JOIN {activity} a ON ac.aid=a.aid
            WHERE a.type IN ('lo_task', 'lo_literacy', 'lo_journal', 'lo_workshop', 'lo_rubric')
            AND au.user_uid " . (!empty($student_ids) ? " IN ({$student_ids}) " : " = {$user_id}") ;

    $aid = array();
    $result = db_query($sql);
    while ($row = db_fetch_object($result)) {
        $aid[] = $row->aid;
    }

    return $aid;
}

function get_user_courses_project_activities($user_id, $student_ids = "") {

    $aid = array();

    $sql = "SELECT DISTINCT ac.aid FROM {ntlp_course_user} cu
            INNER JOIN {ntlp_activity_context} ac ON cu.course_nid = ac.cid
            INNER JOIN {activity} a ON ac.aid=a.aid
            WHERE cu.user_uid " . (!empty($student_ids) ? " IN ({$student_ids}) " : " = {$user_id}") . "
            AND cu.role_id IS NOT NULL AND a.type='ntlp_project'";

    watchdog('courses_project_2', print_r($sql, true));
    $aid = array();
    $result = db_query($sql);
    while ($row = db_fetch_object($result)) {
        $aid[] = $row->aid;
    }

    return $aid;
}

function get_user_course_resources_activities($user_id, $student_ids = "") {

    $role_rs = get_user_role($user_id);
    $role_id = $role_rs->rid;

    $user_check_status = get_notify_user_settings_by_notification($user_id, $role_id, NOTF_SHARE_STUFF_ITEM_POSTED_TO_COURSE);
    $aid = array();
    watchdog('course_resr_1', print_r($user_check_status, true));
    if ($user_check_status->num_rows > 0) {

        $sql = "SELECT DISTINCT ac.aid FROM {ntlp_course_user} cu
            INNER JOIN {ntlp_activity_context} ac ON cu.course_nid = ac.cid
            INNER JOIN {activity} a ON ac.aid=a.aid
            WHERE a.type IN ('lo_document', 'lo_image', 'lo_link', 'lo_video', 'lo_share_idea')
            AND cu.user_uid " . (!empty($student_ids) ? " IN ({$student_ids}) " : " = {$user_id}") . "
            AND cu.role_id IS NOT NULL";

        watchdog('course_resource_2', print_r($sql, true));
        $result = db_query($sql);
        while ($row = db_fetch_object($result)) {
            $aid[] = $row->aid;
        }
    }
    return $aid;
}

function get_user_posts_commented_activities($user_id) {

    $sql = "SELECT DISTINCT ac.aid FROM {ntlp_activity_context} ac
            INNER JOIN {activity} a ON ac.aid=a.aid
            WHERE a.module='commentactivity' AND ac.cid = {$user_id}";

    $aid = array();
    $result = db_query($sql);
    while ($row = db_fetch_object($result)) {
        $aid[] = $row->aid;
    }

    return $aid;
}


function get_user_group_resources_activities($user_id) {

    $aid = array();

    $sql = "SELECT DISTINCT ac.aid FROM {og_uid} gu
            INNER JOIN {ntlp_activity_context} ac ON gu.nid = ac.cid
            INNER JOIN {activity} a ON ac.aid=a.aid
            WHERE a.type IN ('lo_document', 'lo_image', 'lo_link', 'lo_video', 'lo_share_idea')
            AND gu.uid = {$user_id}
            AND gu.is_active = 1";

    $aid = array();
    $result = db_query($sql);
    while ($row = db_fetch_object($result)) {
        $aid[] = $row->aid;
    }

    return $aid;
}

function get_user_school_events_activities($user_id) {

    $aid = array();

    $school_nid = get_this_school();

    $sql = "SELECT DISTINCT ac.aid FROM {ntlp_activity_context} ac
            WHERE ac.cid = -1 OR ac.cid = {$school_nid}";

    $result = db_query($sql);
    while ($row = db_fetch_object($result)) {
        $aid[] = $row->aid;
    }

    return $aid;
}


function get_user_network_events_activities($user_id) {

    $aid = array();

    $sql = "SELECT DISTINCT ac.aid FROM {ntlp_activity_context} ac
            WHERE ac.cid = -1";

    $result = db_query($sql);
    while ($row = db_fetch_object($result)) {
        $aid[] = $row->aid;
    }

    return $aid;
}

function get_user_course_resources_liked_activities($user_id) {

    $sql = "SELECT DISTINCT aid FROM {ntlp_activity_context} ac
            INNER JOIN {ntlp_course_resource} cr ON ac.cid=cr.nid
            INNER JOIN {node} n ON n.nid = cr.nid
            WHERE n.uid={$user_id};";

    $aid = array();
    $result = db_query($sql);
    while ($row = db_fetch_object($result)) {
        $aid[] = $row->aid;
    }

    return $aid;
}

function get_user_group_resources_liked_activities($user_id) {

    $sql = "SELECT DISTINCT aid FROM {ntlp_activity_context} ac
            INNER JOIN {ntlp_course_resource} gr ON ac.cid=gr.nid
            INNER JOIN {node} n ON n.nid = gr.nid
            WHERE n.uid={$user_id};";

    $aid = array();
    $result = db_query($sql);
    while ($row = db_fetch_object($result)) {
        $aid[] = $row->aid;
    }

    return $aid;
}

function get_user_owned_group_activities($user_id) {

    $sql = "SELECT DISTINCT act.aid FROM {activity_targets} act
        INNER JOIN {activity} ac ON act.aid=ac.aid
        WHERE target_uid={$user_id}
        AND target_role='owner'
        AND ac.module='ntlpgroupactivity'";

    $aid = array();
    $result = db_query($sql);
    while ($row = db_fetch_object($result)) {
        $aid[] = $row->aid;
    }

    return $aid;
}

function get_user_courses_activity_grade_activities($user_id, $student_ids = "") {

    $aid = array();

    global $user;

    $sql = "SELECT DISTINCT ac.aid, ac.data FROM {activity} ac
        INNER JOIN {ntlp_activity_context} acc ON acc.aid = ac.aid
        INNER JOIN {ntlp_activity_user} au ON au.activity_nid = acc.cid
        WHERE ac.module='ntlpsubmissionactivity' AND ac.operation='grade'
        AND au.user_uid " . (!empty($student_ids) ? " IN ({$student_ids}) " : " = {$user_id}");

    $result = db_query($sql);
    
    $std_uids = array();
    if (!empty($student_ids)) {
        $std_uids = explode(',',$student_ids);
    }

    while ($row = db_fetch_object($result)) {

        $data = unserialize($row->data);

        if (count($std_uids) > 0) {
                    
            if (array_search($data['graded-user'], $std_uids) !== false)
                $aid[] = $row->aid;
        }
        else {
            if ($data['graded-user'] == $user_id)
                $aid[] = $row->aid;
        }
    }

    return $aid;
}

function get_user_library_project_exemplary_activities($user_id) {

    $aid = array();

    global $user;

    $sql = "SELECT DISTINCT ac.aid FROM {activity} ac
        INNER JOIN {ntlp_activity_context} acc ON acc.aid = ac.aid
        WHERE ac.module='ntlpprojectactivity' AND ac.operation='exemplary'
        AND acc.cid = {$user_id}";

    $result = db_query($sql);
    while ($row = db_fetch_object($result)) {
        $aid[] = $row->aid;
    }

    return $aid;
}

function get_user_library_project_archived_activities($user_id) {

    $aid = array();
    global $user;

    if (!isset($user->roles[NTLP_ROLEID_NETWORKMANAGER]))
        return $aid;

    $sql = "SELECT DISTINCT ac.aid FROM {activity} ac
        INNER JOIN {ntlp_activity_context} acc ON acc.aid = ac.aid
        WHERE ac.module='ntlpprojectactivity' AND ac.operation='archive'";

    $result = db_query($sql);
    while ($row = db_fetch_object($result)) {
        $aid[] = $row->aid;
    }

    return $aid;
}

function ntlp_activity_render_header() {
    include 'activity_template.inc';
    $url = $_GET['q'];
    $args = explode('/', $url);


    $output = str_replace('<!-- heading -->', 'Recent Notifications', $HTMLSelectorsAndHeading);



    return $output;
}

function array_merge_by_key() {
     $loop_count1 = func_num_args();    // Get the total # of arguements (parameter) that was passed to this function
     $junk_array1 = func_get_args();    // Get the value that was passed in via arguement/parameter #... in arrays
     $xyz = 0;
     
     for($x = 0; $x < $loop_count1; $x++) {
        $array_count1 = count($junk_array1[$x]);

        if ($array_count1 != 0) {
           for($y = 0; $y < $array_count1; $y++) {
              $new_array1[$xyz] = $junk_array1[$x][$y]; //Populate new array by adding all the items
              $xyz++;
           }
        }
     }

     $new_array2 = array_unique($new_array1);   //Make entries unique and generate a new array

     return $new_array2;
 }
