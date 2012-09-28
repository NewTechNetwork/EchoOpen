<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


//This method is called from themes/Boldr/module_header.inc
function default_render_header() {
    include "default_header.template.inc";

    $output = $HTMLheader;
    return $output;
}

//This method is called from themes/Boldr/module_header.inc
//Displays no tabs
function default_render_tabs() {
    include "default_header.template.inc";

    $output = $HTMLtabs;
    return $output;
}

//Called by ntlp/blocks/changeschool/% from ntlp_blocks.module
//Sets the current school
function set_school($school_nid) {
    global $user;
    $found = "ERROR";
//    $temp_nid = $_SESSION['NTLP_THIS_SCHOOL'];
//    watchdog('global', 'Current School:'.$temp_nid);
  $school_node =  node_load($school_nid);
    if (user_access_to_school($school_nid)) {
        $_SESSION['NTLP_THIS_SCHOOL'] = $school_nid;
        $_SESSION['NTLP_THIS_SCHOOL_NAME'] = $school_node->title;
        $found = "OK";
    }
//    watchdog('global', 'New School:'.$_SESSION['NTLP_THIS_SCHOOL']);
    print $found;
}

function get_school_control() {
    global $user;
    include "default_header.template.inc";
    drupal_add_js(drupal_get_path('module', 'ntlp_blocks') . '/default_header.js');


    //If User is Staff and is enrolled in multiple school courses
    $soptions = '';
    $lShow = FALSE;
    $current_snid = get_this_school();
    if(isset($user->roles[NTLP_ROLEID_TEACHER])) {
        $aschools = get_user_course_schools($user->uid, NTLP_ROLEID_TEACHER);
//            print_r($aschools);
        if (count($aschools) > 1) {
            foreach ($aschools as $key => $title) {
                $soptions .= '<option value="'.$key.'"'.($current_snid == $key ? 'selected="selected"' : '').'>'.$title.'</option>';
            }
            $lShow = TRUE;
        }
    } else {
        //If User is Staff and is enrolled in multiple school courses
        if(user_access('allow access to contents of all schools')) {
            $aschools = get_schools();
            while ($rec = db_fetch_object($aschools)) {
//                print $rec->nid.'-'.$rec->school_name.',';
                $soptions .= '<option value="'.$rec->nid.'"'.($current_snid == $rec->nid ? 'selected="selected"' : '').'>'.(strlen($rec->school_name) >= 40 ? substr($rec->school_name, 0, 40).'...' : $rec->school_name).'</option>';
            }
            $lShow = TRUE;
        }
    }

    $output = "";
    if ($lShow) {
        $output = $HTMLschools;
        $output = str_replace('@SCHOOL_OPTIONS', $soptions, $output);
    }
    return $output;
}


function get_all_school_control( $showTitle = true, $attributes = array() ) { // this method used for people finder
    global $user;
    include "default_header.template.inc";

    //If User is Staff and is enrolled in multiple school courses
    $soptions = '';

    //If User is Staff and is enrolled in multiple school courses
    if(user_access('allow access to contents of all schools')) {
        $thisSchool = get_network_school();
        $aschools = get_schools();
        while ($rec = db_fetch_object($aschools)) {
//                print $rec->nid.'-'.$rec->school_name.',';
            $soptions .= '<option value="'.$rec->nid.'"'.($thisSchool == $rec->nid ? 'selected="selected"' : '').'>'.$rec->school_name.'</option>';
        }
        $lShow = TRUE;
    }


    $output = "";

    $output = $HTMLschools;

    if(!$showTitle) {
        $output = preg_replace('/<!-- TITLE START -->[\s\S]*?<!-- TITLE END -->/', '', $output);
    }


    if( $attributes ) {
        $attrs = '';
        foreach ($attributes as $attr => $value) {
            $attr = preg_replace('/\s+/', '', $attr);
            if( preg_match('/[a-zA-z]+/', $attr) ) {
                $attrs .= " $attr = \"$value\"";
            }
        }
        $wrapper = str_replace('@ATTR', $attrs, '<span class="comboboxBgLeft" @ATTR>');
        $output = preg_replace('/<!-- WRAPPER START -->[\s\S]*?<!-- WRAPPER END -->/', $wrapper, $output);
    }

    $output = str_replace('@SCHOOL_OPTIONS', $soptions, $output);

    return $output;
}
?>
