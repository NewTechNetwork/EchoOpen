<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');

function agenda_block_contents($pcourse_nid=0, $pyear=0, $pmonth=0, $pday=0) {
    drupal_add_js(drupal_get_path('module', 'ntlp_agenda') . '/ntlp_agenda.js');

    include "agenda.template.inc";
    global $user, $base_url;
    $homepage = true;

    if ($pcourse_nid == 0) {
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

        //Get all week days
        $date = get_tz_course($courseid, 'Y/m/d', date('Y/m/d H:i:s',time()));

        $pyear = date("Y", strtotime($date));
        $pmonth = date("m", strtotime($date));
        $pday = date("d", strtotime($date));
    } else {
        $homepage = false;

        $courseid = $pcourse_nid;
        
        $date = date('Y/m/d', mktime(0, 0, 0, $pmonth, $pday, $pyear));
    }

    $user_role = check_user_permission($courseid);

    //$date = get_tz_course($courseid,'Y/m/d',$date);
    $ts = strtotime($date);
    // calculate the number of days since Monday
    $dow = date('w', $ts);
    $offset = $dow - 1;
    if ($offset < 0)
        $offset = 6;
    // calculate timestamp for the Monday
    $ts = $ts - $offset * 86400;
    // loop from Monday till Sunday

    $rAbox = $HTMLcontainer;
    for ($i = 1; $i < 6; $i++, $ts+=86400) {

        if (date("Y/m/d", $ts) == $date) {    //If today

            $dayCSS['attributes']['class'] = 'active';
        } else {
            $dayCSS['attributes']['class'] = '';
        }
        $rAbox = str_replace('@DAY' . $i . '_LINK', l(date("D (m/j)", $ts), 'ntlp/courses/agenda/' . $courseid . '/' . date('Y/m/d', $ts), $dayCSS), $rAbox);
    }

    $rAbox = str_replace('@DAY' . $i . '_LINK', l(date("D M j", $ts), 'ntlp/courses/agenda/' . $courseid . '/' . date('Y/m/d', $ts), $dayCSS), $rAbox);

    // Calling Get Agenda Function
    $rec = get_agenda($courseid, $pyear, $pmonth, $pday);

    if ($rec) {
        $rAbox = str_replace('@AGENDA_TITLE', '', $rAbox);
        if ($homepage) {
            $rAbox = str_replace('@AGENDA_BODY', "<div id=\"agendaBodyWrapper\" style=\"height: 100px; width:100%; overflow: hidden; overflow-x: hidden; overflow-y: hidden;\"> <div id='custom_list_agenda'>$rec->body </div> </div>", $rAbox);
            $rAbox = str_replace('@VIEW_MORE_LINK', "<a href='javascript:;'>More ></a>", $rAbox);
        } else {
            $rAbox = str_replace('@AGENDA_BODY', $rec->body, $rAbox);
            $rAbox = str_replace('@VIEW_MORE_LINK', '', $rAbox);
        }
    } else {
        $rAbox = str_replace('@AGENDA_TITLE', '', $rAbox);
        $rAbox = str_replace('@AGENDA_BODY', 'No agenda found for today', $rAbox);
        $rAbox = str_replace('@VIEW_MORE_LINK', '', $rAbox);
    }
    if ($homepage) {
        $rAbox = str_replace('@VIEW_ALL_LINK', l('View All Agendas >', 'ntlp/courses/agenda/' . $courseid . '/' . date('Y/m/d')), $rAbox);
    } else {
        if (isset($user_role) && $user_role == NTLP_ROLEID_NETWORKMANAGER || $user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_NETWORKSTAFF) {
            $rAbox = str_replace('@VIEW_ALL_LINK', l('Edit', 'ntlp/courses/agenda/edit/' . $courseid . '/' . date('Y/m/d', strtotime($date)), $dayCSS), $rAbox);
        } else {
            $rAbox = str_replace('@VIEW_ALL_LINK', '', $rAbox);
        }
    }
    return $rAbox;
}

function agenda_main_contents($pcourse_nid, $pyear, $pmonth, $pday) {
    include "agenda.template.inc";
    global $user, $base_url;
    $courseid = $pcourse_nid;

    $user_role = check_user_permission($courseid);

    $date = date('Y/m/d', mktime(0, 0, 0, $pmonth, $pday, $pyear));

    // parse about any English textual datetime description into a Unix timestamp
    $ts = strtotime($date);
    $html = $HTMLmain_agenda_body;
    $html = str_replace('@TODAY_DAY', date("l", $ts), $html);
    $html = str_replace('@TODAY_DATE', date("M j, Y", $ts), $html);

    // Calling Get Agenda Function
    $rec = get_agenda($courseid, $pyear, $pmonth, $pday);

    if ($rec) {
        $html = str_replace('@AGENDA_TITLE', '', $html);
        $html = str_replace('@AGENDA_BODY', $rec->body, $html);
    } else {
        $html = str_replace('@AGENDA_TITLE', '', $html);
        $html = str_replace('@AGENDA_BODY', 'No agenda found for today', $html);
        $html = str_replace('@VIEW_MORE_LINK', '', $html);
    }
    return $html;
}

?>
