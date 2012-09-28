<?php

/**
 * Menu callback for admin/settings/ntlp
 */
function ntlp_admin_settings() {
    $form['ntlp']['school'] = array(
            '#type' => 'item',
            '#description' => 'Provides NTLP School Maintenance options',
            '#value' => l('NTLP School Maintenance', 'ntlp/admin/ntlp_school'),
    );

    $link = "ntlp/activity/new/1";
    $form['ntlp']['course'] = array(
            '#type' => 'item',
            '#description' => 'Provides NTLP Course Activity Maintenance options in demo course',
            '#value' => l('NTLP Course Activity Maintenance', $link),
    );

$form['ntlp']['create_agenda'] = array(
            '#type' => 'item',
            '#description' => 'Provides NTLP Course Agenda Maintenance options',
            '#value' => l('Create Course Agenda ', 'agenda-calendar'),
    );


    return $form;
}

?>
