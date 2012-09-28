<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


function ntlp_library_project_archive_frm($action, $project_nid) {

    return drupal_get_form('ntlp_library_project_archive_form', $action, $project_nid);
}
/* popup form */
function ntlp_library_project_archive_form($form = null, $action, $project_nid) {

    modalframe_child_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_library') . '/ntlp_library.js');


    $form['project_archived'] = array(
            '#type'   => 'fieldset',
            '#title'  => '',
            '#tree'   => TRUE, // Don't forget to set #tree!
            '#prefix' => '<div id="project_add_test">'.$HTMLform_activity_outer_name_hdr,
            '#suffix' => $HTMLform_activity_outer_name_ftr.'</div>',
    );

    $form['project_archived']['project_nid'] = array(
            '#type' => 'hidden',
            '#value' =>  $project_nid,
            '#id' => 'project_nid',
    );

    if($action == 'published') {
        $message = 'Are you sure you want to publish this project now?';
        $save_action = '<a class="SaveBtn" onclick="save_published()" > Yes</a>';


    }else {
        $message = 'Are you sure you want to archive this project now?';
        $save_action = '<a class="SaveBtn" onclick="save_archived()" > Yes</a>';
    }
    $form['project_archived']['additional_notes'] = array(
            '#type' => 'item',
            '#value' => $message ,
            '#prefix' => '<div >',
            '#suffix' => '</div>',

    );


    $form['project_archived']['save_action'] = array(
            '#type' => 'item',
            '#value' =>$save_action,
            '#prefix' => '<div>',
            '#suffix' => '</div>',
    );

    $form['project_archived']['cancel_action'] = array(
            '#type' => 'item',
            '#value' => '<div style="float:right;padding:0 10px 0 0"><a class="SaveBtn" onclick="cancel_archived()" > Cancel</a></div>',
            '#prefix' => '<div>',
            '#suffix' => '</div>',
    );



    return $form;

}

function ntlp_library_project_archived_data($project_nid) {
    global $user;
    $dateformat="%Y-%m-%d %H:%M:%S";
    $archived_on = strftime($dateformat, time());

    watchdog('project_archived', $project_nid);

    db_query("UPDATE {ntlp_library_project} SET archived_on = '%s', archived_by= %d
        WHERE nid = %d ", $archived_on, $user->uid, $project_nid  );


    drupal_set_message("<div><p>Project: <b>".$project_name."</b> -  archived successfully</p></div>");
    return;
}

function ntlp_library_project_published_data($project_nid) {
    global $user;
    watchdog('project_published', $project_nid);

    $archive_date ='0000-00-00 00:00:00';
    $dateformat="%Y-%m-%d %H:%M:%S";
    $published_on = strftime($dateformat, time());

    db_query("UPDATE {ntlp_library_project} SET archived_on = '%s', archived_by = %d, published_on = '%s', published_by = %d
        WHERE nid = %d ",$archive_date, 0, $published_on, $user->uid, $project_nid );

    drupal_set_message("<div><p>Project: <b>".$project_name."</b> -  published successfully</p></div>");
    return;
}



?>