<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


function ntlp_library_project_request_archive_frm($project_nid) {
    return drupal_get_form("ntlp_library_project_request_archive_form", $project_nid);
}
/* popup form */
function ntlp_library_project_request_archive_form($form = null, $project_nid) {
    modalframe_child_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_library') . '/ntlp_library.js');

    $form['project_archive'] = array(
            '#type'   => 'fieldset',
            '#title'  => '',
            '#tree'   => TRUE, // Don't forget to set #tree!
            '#prefix' => '<div id="project_add_test"><div id="ActivitiesCSS" style="width:auto">
  <div class="LightBlueBottomRightCorner">
    <div class="LightBlueBottomLeftCorner">
      <div class="LightBlueTopRightCorner">
        <div class="LightBlueTopLeftCorner">
          <div class="ActivitiesAssignment">
           '.$HTMLform_activity_outer_name_hdr,
            '#suffix' => $HTMLform_activity_outer_name_ftr.'          </div>
        </div>
      </div>
    </div>
  </div>
</div>
</div>',
    );

    $form['project_archive']['project_nid'] = array(
            '#type' => 'hidden',
            '#value' =>  $project_nid,
            '#id' => 'project_nid',
    );

    $form['project_archive']['projectname'] = array(
            '#type' => 'radios',
            '#options' => array("1"=>t('Yes'),"0"=> t('No')),
            '#default_value' =>  0,
            '#prefix' => '<div class="rdio_btn"><b style="color:#3570AA;font-size:15px;">Do you want to archive this project?</b></div><div>',
            '#suffix' => '</div>',
            '#attributes' => array(
                    'onclick' => 'enable_reason(this)'),


    );

    $form['project_archive']['reason'] = array(
            '#type' => 'textfield',
            '#disabled' => TRUE,
            '#value' => '',
            '#prefix' => '<br clear="all" /><br clear="all" /><div id="reason_value" ><b style="color:#3570AA" >Briefly explain why:</b>',
            '#suffix' => '</div>',
            '#id' => 'txt_reason',
    );

    $form['project_archive']['additional_notes'] = array(
            '#type' => 'textfield',
            '#value' => '',
            '#prefix' => '<div ><b style="color:#3570AA" >Additional Notes:</b>',
            '#suffix' => '</div>',
            '#id' => 'txt_additional_notes',

    );

    $form['project_archive']['save_action'] = array(
            '#type' => 'item',
            '#value' => '<a class="AddOutComesBtn" onclick="save_archive_request()" > Archive Request</a>',
            '#prefix' => '<br clear="all" /><div>',
            '#suffix' => '</div>',

    );



    return $form;
}


function ntlp_library_project_request_archive_data($project_nid, $reason_txt, $txt_additional_notes) {

    global $user;
//    echo 'here i am ';
    $dateformat="%Y-%m-%d %H:%M:%S";

    watchdog('requrest_archive', $project_nid .' '. $reason_txt .' '. $txt_additional_notes);
    $archive_request_on = strftime($dateformat, time());

    db_query("UPDATE {ntlp_library_project} SET archive_request_on = '%s', archive_reason = '%s', archive_notes = '%s'
        WHERE nid = %d", $archive_request_on, $reason_txt, $txt_additional_notes, $project_nid);

    drupal_set_message("<div><p>Project: <b>".$project_name."</b> - request to archive sent successfully</p></div>");
    ntlpprojectactivity_submit("archive", $project_nid, -2, $user->uid);
    module_invoke_all('ntlpcontent', node_load($project_nid), 'project_archive_request');

    return;
}



?>