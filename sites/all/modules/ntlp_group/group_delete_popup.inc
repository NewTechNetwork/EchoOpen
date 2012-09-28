<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


function ntlp_group_delete_popup($group_id) {

    modalframe_child_js();
    return drupal_get_form('ntlp_group_delete_form',$group_id);

}
function ntlp_group_delete_form($form = NULL, $group_id) {
  
    drupal_add_js(drupal_get_path('module', 'ntlp_group') . '/ntlp_group.js');
    
    drupal_set_title("Confirm Delete Group");

    $form['delete_activity'] = array(
            '#type' => 'fieldset',
            '#tree' => true,

    );

    $form['delete_group']['group_id'] = array(
            '#type' => 'hidden',
            '#value' => $group_id,
    );


    $form['delete_activity']['table'] = array(
            '#type' => 'item',
            '#prefix' => '<table>',
            '#suffix' => '</table>',

    );

    $form['delete_activity']['table']['row1'] = array(
            '#type' => 'item',
            '#prefix' => '<tr>',
            '#suffix' => '</tr>',
    );

    $form['delete_activity']['table']['row1']['td'] = array(
            '#type' => 'item',
            '#value' => t('You are about to delete this group.<br /><br /> Are you sure you want to delete this group?<br />'),
            '#prefix' => '<td style="color:#3570AA;font-weight:bold">',
            '#suffix' => '</td>',
            '#id' => 'txt_group_name',
    );


    $form['delete_activity']['table']['row3'] = array(
            '#type' => 'item',
            '#prefix' => '<tr>',
            '#suffix' => '</tr>',

    );
    $form['delete_activity']['table']['row3']['delete'] = array(
            '#type' => 'item',
            '#value' => '<table><tr><td>'.l('Yes', '',array('attributes' => array('class' => 'SaveBtn', 'onclick' => 'delete_group('.$group_id.'); return false;'))).'</td><td>&nbsp;</td><td>'.l('No', '',array('attributes' => array('class' => 'SaveBtn', 'onclick' => 'delete_group_cancel(); return false;'))).'</td>',
            '#prefix' => '<td>',
            '#suffix' => '</td>',
    );

    return $form;
}

