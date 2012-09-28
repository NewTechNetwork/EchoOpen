<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');

function events_main_contents($event_nid) {
    include "events.template.inc";
    global $user, $base_url;

    // Calling Get Events Function
    $rec = get_event($event_nid);

    if ($rec) {
        $date = date('Y/m/d', mktime(0, 0, 0, $rec->month, $rec->day, $rec->year));

        // parse about any English textual datetime description into a Unix timestamp
        $ts = strtotime($date);
        $html = $HTMLmain_events_body;
        $html = str_replace('@TODAY_DAY', date("l", $ts), $html);
        $html = str_replace('@TODAY_DATE', date("M j, Y", $ts), $html);

        $event_date_range = "";
        if ($rec->event_end > $rec->event_start)
            $event_date_range = "   (" . date(NTLP_DATE_TZ_SHORT, strtotime ($rec->event_start)) . " to " . date(NTLP_DATE_TZ_SHORT, strtotime ($rec->event_end)) . ")";

        $html = str_replace('@EVENTS_TITLE', $rec->title . $event_date_range, $html);     //title not required
        $html = str_replace('@EVENTS_BODY', $rec->body, $html);

        $isadmin = is_user_adminof_this_event($rec->nid, $user, $rec->type); // checking permission for group
        $isschoolstaff = ((isset($user->roles[NTLP_ROLEID_SITEADMIN]) || isset($user->roles[NTLP_ROLEID_TEACHER]) || isset($user->roles[NTLP_ROLEID_SCHOOLLEADER]))
                    && check_user_school($user) && $rec->type != 'ntlp_network_event');
        $iseventowner = ($user->uid == $rec->uid);

        if ($isadmin || $isschoolstaff || $iseventowner) {
            $html = str_replace("@DELETE", '<a class="ntlp_delete_link" href=javascript:onclick_remove_event(' . $rec->nid . '); > </a>', $html);
        } else {
            $html = str_replace("@DELETE", '', $html);
        }


    } else {
        $html = str_replace('@EVENTS_TITLE', 'Event not found', $html);
        $html = str_replace('@EVENTS_BODY', '', $html);
    }

    return $html;
}

//This function renders list of events that shows up and body block
//Called by main function and calenday day blocks using ajax
function events_main_list($pgroup_nid, $pyear, $pmonth, $pday, $event_types = '') {
    include "events.template.inc";
    global $user, $base_url;

    drupal_add_js(drupal_get_path('module', 'ntlp_events') . '/ntlp_events.js');

    $groupnid = $pgroup_nid;
    $date = date('Y/m/d', mktime(0, 0, 0, $pmonth, $pday, $pyear));

    // parse about any English textual datetime description into a Unix timestamp
    $ts = strtotime($date);
    $html = $HTMLbody_events_list;
    $html = str_replace('@TODAY_DAY', date("l", $ts), $html);
    $html = str_replace('@TODAY_DATE', date("M j, Y", $ts), $html);
    $html = str_replace('@PRINT', l('', 'ntlp/events/' . $pgroup_nid . '/' . $pyear . '/' . $pmonth . '/' . $pday . '/print', array('attributes' => array('class' => 'PrintBtn'))), $html);

    if ($pgroup_nid) {
        $node = node_load($pgroup_nid);
        if ($node->type == 'ntlp_group') {
            $html = str_replace('<!-- FILTER START -->', '<!--//', $html);
            $html = str_replace('<!-- FILTER END -->', '//-->', $html);
        }
    }


    $school_nid = get_this_school();
//    watchdog('event', "Open event for school: $school_nid with node: $groupnid ($pyear/$pmonth/$pday) with Visibility filter: [$event_types]");
    // Calling Get Events Function
    $rs = get_events($school_nid, $groupnid, $pyear, $pmonth, $pday, $event_types);
    $eventList = "";
    if ($rs) {
        while ($rec = db_fetch_object($rs)) {
//          group icon "ntlp_group_icon" school icon "ntlp_school_icon" network icon "ntlp_network_icon"
            //testing
            $event_icon = "ntlp_school_icon";
            $event_owner = '(' . $rec->grp_title . ' | ' . $rec->first_name . ' ' . $rec->last_name . ')';
            switch ($rec->type) {
                case 'ntlp_school_event':
                    $event_icon = "ntlp_school_icon";
                    $event_owner = '(' . $rec->school_name . ' | ' . $rec->first_name . ' ' . $rec->last_name . ')';
                    break;
                case 'ntlp_network_event':
                    $event_icon = "ntlp_network_icon";
                    $event_owner = '(' . $rec->school_name . ' | ' . $rec->first_name . ' ' . $rec->last_name . ')';
                    break;
                case 'ntlp_group_event':
                    $event_icon = "ntlp_group_icon";
                    $event_owner = '(' . $rec->grp_title . ' | ' . $rec->first_name . ' ' . $rec->last_name . ')';
                    break;
                case 'ntlp_class_event':                //Agenda; Note: We may have to merge Agenda in Events table
                    $event_icon = "ntlp_agenda_icon";
                    $event_owner = '(' . $rec->grp_title . ' | ' . $rec->first_name . ' ' . $rec->last_name . ')';
                    break;
            }

            $isadmin = is_user_adminof_this_event($rec->nid, $user, $rec->type); // checking permission for group
            $isschoolstaff = ((isset($user->roles[NTLP_ROLEID_SITEADMIN]) || isset($user->roles[NTLP_ROLEID_TEACHER]) || isset($user->roles[NTLP_ROLEID_SCHOOLLEADER]))
                    && check_user_school($user) && $rec->type != 'ntlp_network_event');
            $iseventowner = ($user->uid == $rec->uid);

            $event_date_range = "";
            if ($rec->event_end > $rec->event_start)
                $event_date_range = "   (" . date(NTLP_DATE_TZ_SHORT, strtotime ($rec->event_start)) . " to " . date(NTLP_DATE_TZ_SHORT, strtotime ($rec->event_end)) . ")";

            $newEvent = $HTMLbody_event;

            $newEvent = str_replace('@EVENT_NID', $rec->nid, $newEvent);
            $newEvent = str_replace('@EVENT_TITLE', htmlentities($rec->title) . $event_date_range, $newEvent);
            $newEvent = str_replace('@EVENT_TYPE', $event_icon, $newEvent);
            $newEvent = str_replace('@GROUP_NAME', $event_owner, $newEvent);
            $newEvent = str_replace('@EVENTS_BODY', addslashes($rec->body), $newEvent);
            $newEvent = str_replace('@EVENT_ID', $rec->nid, $newEvent);
            if ($isadmin || $isschoolstaff || $iseventowner) {
                $newEvent = str_replace("@DELETE", '<a class="ntlp_delete_link" href=javascript:onclick_remove_event(' . $rec->nid . '); > </a>', $newEvent);
            } else {
                $newEvent = str_replace("@DELETE", '', $newEvent);
            }
            $eventList .= $newEvent;
        }
    }
    if (!empty($eventList))
        $html = str_replace('@EVENTS_BODY', $eventList, $html);
    else
        $html = str_replace('@EVENTS_BODY', 'No events found for today', $html);

    return $html;
}

//This function generates the all the blocks
function events_main($node = 0, $year = 1970, $month = 1, $day = 1, $event_nid = 0) {
//echo "Course: $node ($year/$month/$day)";
    include('events.template.inc');
    global $user;
    drupal_add_css(drupal_get_path('module', 'ntlp_events') . '/ntlp_events.css');
//    variable_set('jquery_update_compression_type', 'none');
    jquery_ui_add(array('ui.dialog', 'ui.draggable', 'ui.datepicker'));

################### Date Picker Files ########################################################################################
    drupal_add_js(drupal_get_path('module', 'ckeditor') . '/ckeditor/ckeditor.js');
//    drupal_add_js(drupal_get_path('module', 'ckeditor') . '/ckeditor/ckeditor_basic.js');
    drupal_add_js(drupal_get_path('module', 'ckeditor') . '/includes/ckeditor.utils.js');

    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/date_picker.js');
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/jquery.ui.datepicker.css', 'module', 'all', FALSE);
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/demos.css', 'module', 'all', FALSE);
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/jquery.ui.all.css', 'module', 'all', FALSE);
# there is a white line coming on the lower left side of the calender due to background:
#ffffff defined on line 14 of "datepicker/jquery.ui.theme.css" file
################### END Of Date Picker Files #################################################################################
    // Calendar code based on example at http://evolt.org/node/60673 :

    $homepage = true;
    if ($node == 0) {
        //Getting Query String From URL
        $url = $_GET['q'];
        $args = explode('/', $url);
        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i])) {
                $node = $args[$i];
                break;
            }
        }


        if ($node == 0) {
            $school_nid = get_this_school();
        } else {
            $group_node = node_load($node);

            if ($group_node->type == 'ntlp_group') {
                $school_nid = get_group_school_nid($group_node->nid);
            }
            else {
                $school_nid = get_this_school();
                $node = 0;
            }
        }
        //Get all week days

        $date = get_tz($school_nid, 'Y/m/d', date('Y/m/d H:i:s',time()));
        $today = getdate(); // Get today's date

//        echo date("Y", strtotime($date)).' - '.$date.'   dt: '.date('Y/m/d g:i:s a',time());

        if ($year == 1970) {
//            $year = $today['year'];
//            $month = $today['mon'];
//            $day = $today['mday'];
            $year = date("Y", strtotime($date));
            $month = date("m", strtotime($date));
            $day = date("d", strtotime($date));

//            $date = $today['year'] . '/' . $today['mon'] . '/' . $today['mday'];
            $date = $year . '/' . $month. '/' . $day ;
          }
    } else {
        $homepage = false;
        $date = date('Y/m/d', mktime(0, 0, 0, $month, $day, $year));
    }

    $startofweek = 0;   //variable_get('ntlp_events_'. $node->nid .'_startofweek', 1);
    $showkey = 1;   //variable_get('ntlp_events_'. $node->nid .'_showkey', 1);
    $firstletter = 0;   //variable_get('ntlp_events_'. $node->nid .'_firstletter', 0);

    $month_title = t("@date", array('@date' => format_date(mktime(12, 0, 0, $month, 1, $year), 'custom', 'M Y')));

    $output = $HTMLmain_container;

    if (!isset($user->roles[NTLP_ROLEID_PARENT])) {
        if ($node == 0) {
            $user_role = get_user_role($user->uid);
            if (check_user_permission_for_access($user_role->rid, get_this_school() . ' create school wide events')
                    || check_user_permission_for_access($user_role->rid, get_this_school() . ' create network events')) {
                $output = str_replace('<!-- ADD EVENT -->', '<input type="button" id="events-add" onclick="add_events()" ' . (($event_nid!=0) ? 'style="display:none"' : '') . ' class="addEvents_btn" value="Add Events" />', $output);
            }
        } else {
            if (is_user_group_admin($node, $user) || (is_user_group_member($node, $user) == 0)
                    || isset($user->roles[NTLP_ROLEID_NETWORKSTAFF])
                    || isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])) {
                $output = str_replace('<!-- ADD EVENT -->', '<input type="button" id="events-add" onclick="add_events()" ' . (($event_nid!=0) ? 'style="display:none"' : '') . ' class="addEvents_btn" value="Add Events" />', $output);
            }
        }
    }

    if (false && $event_nid != 0) {
        $event_node = node_load($event_nid);
        //print_r ($event_node);

        $isadmin = is_user_adminof_this_event($event_node->nid, $user, $event_node->type); // checking permission for group
        $isschoolstaff = ((isset($user->roles[NTLP_ROLEID_SITEADMIN]) || isset($user->roles[NTLP_ROLEID_TEACHER]) || isset($user->roles[NTLP_ROLEID_SCHOOLLEADER]))
                    && check_user_school($user) && $event_node->type != 'ntlp_network_event');
        $iseventowner = ($user->uid == $event_node->uid);

        if ($isadmin || $isschoolstaff || $iseventowner) {
            $output = str_replace('<!-- EDIT EVENT -->', '<input type="button" id="events-edit" onclick="edit_events()" ' . (($event_nid==0) ? 'style="display:none"' : '') . ' class="SaveBtn" value="Edit" style="width:100px" width="100" />', $output);
        }
    }

    $output = str_replace('<!-- EDIT EVENT -->', '<input type="button" id="events-edit" onclick="edit_events()" style="display:none" class="SaveBtn" value="Edit" style="width:100px" width="100" />', $output);

//    $output = str_replace("@LINK_EDIT", url("ntlp/events/edit/$node/$year/$month/$day"), $output);cgi
    $output = str_replace("@CALENDAR_TITLE", $month_title, $output);
    $output = str_replace("@CALENDAR_DD", $day, $output);
    $output = str_replace("@CALENDAR_MM", $month, $output);
    $output = str_replace("@CALENDAR_YY", $year, $output);
    $output = str_replace('@CALENDAR_BODY',
                    events_render_calendars_month($node, $year, $month, $day), $output);
    if ($event_nid == 0)
        $output = str_replace('@EVENTS_BODY',
                        events_main_list($node, $year, $month, $day), $output);
    else
        $output = str_replace('@EVENTS_BODY',
                        events_main_contents($event_nid), $output);

    $output .= '<div>
            <input id="heventid" type="hidden" value="'.$event_nid.'" />
            <input id="hnodeid" type="hidden" value=' . $node . ' />' .
//            drupal_get_form('get_temp_rte').
            '</div>';

    // add an extra three months for editors only
//    if (user_access('create ntlp_events content')) {
//
//    }
    return $output;
}

//function get_temp_rte() {
//    $form['events_desc'] = array(
//        '#type' => 'textarea',
//        '#id' => 'temp-events-text',
//        '#default_value' => '',
//    );
//    return $form;
//}

function recent_events_block_contents() {
    include('events.template.inc');
    global $user;

    //This redirection logic is implemented to redirect the user to force password change,
    //if his force-password-change flag is true
    //This can be implemented in post-login event in ntlp_school module
    $new_user = _check_new_user($user->uid);

    if ($new_user) {
        drupal_goto('ntlp/new/user/password/change');
    }


    $output = $HTMLEvents;
    $tempRow = '';

    $eventArray = get_tz_recent_events();

    if (!empty($eventArray)) {
        foreach ($eventArray as $event) {
            $event_date_range = "";
            if ($event->event_end > $event->event_start)
                $event_date_range = date(NTLP_DATE_TZ_FJY, strtotime ($event->event_start)) . " to " . date(NTLP_DATE_TZ_FJY, strtotime ($event->event_end));
            else
                $event_date_range = date( 'F j, Y', strtotime($event->event_date));

            $tempRow .= $HTMLEvents_row;
            $tempRow = str_replace('@EVENT_TITLE', $event->title, $tempRow);
            $tempRow = str_replace('@EVENT_WHOM', $event->first_name . ' ' . $event->last_name, $tempRow);
            $tempRow = str_replace('@EVENT_URL', url('ntlp/events/open/' . $event->nid), $tempRow);
            $tempRow = str_replace('@EVENT_DATE', $event_date_range, $tempRow);
        }
    } else {

//        $date = mktime(0, 0, 0, $event->month, $event->day, $event->year);
        $tempRow .= $HTMLEvents_row;
        $tempRow = str_replace('@EVENT_TITLE', '', $tempRow);
        $tempRow = str_replace('@EVENT_WHOM', 'No upcoming events', $tempRow);
        $tempRow = str_replace('@EVENT_URL', '', $tempRow);
        $tempRow = str_replace('@EVENT_DATE', '', $tempRow);
    }

    $output = str_replace('@EVENTS_ROW_PLACEHOLDER', $tempRow, $output);
    $output = str_replace('@VIEW_ALL_LINK', url('ntlp/events'), $output);

    return $output;
}

?>
