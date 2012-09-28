<?php
/*  FAQ Form START   */
function help_form() {

    include('ntlp_help.template.inc');

    $form['faq'] = array(
            '#type' => 'fieldset',
            '#title' => t(''),
            '#tree' => TRUE,
            '#prefix' =>  $HTMLform_school_setting_bluebox_start,
            '#suffix' => $HTMLform_school_setting_bluebox_end,
    );

    $form['faq']['title'] = array(
            '#type' => 'item',
            '#value' => t('Display of FAQ module implemented by logicalis goes here.'),
            '#prefix' => '<tr><td>',
            '#suffix' => '</td></tr>',
    );


    return $form;
}
/*  FAQ Form END*/


/*  FEEDBACK Form START   */
function help_feedback_form() {
    include('ntlp_help.template.inc');
    $form['feedback'] = array(
            '#type' => 'fieldset',
            '#title' => t(''),
            '#tree' => TRUE,
            '#prefix' =>  $HTMLform_school_setting_bluebox_start,
            '#suffix' => $HTMLform_school_setting_bluebox_end,
    );

    $form['feedback']['title'] = array(
            '#type' => 'item',
            '#value' => t('Think you found a bug ? or have an idea for new feature? we want to hear about it!'),
            '#prefix' => '<tr><td colspan="2">'.$HTMLform_course_msgbox1_hdr,
            '#suffix' => $HTMLform_course_msgbox1_ftr.'</td></tr>',
    );

    $type_options = get_request_type('NTLP Feedback Type');

//    $type_options = array('The system is realy slow right now', 'I think i found a bug', 'I have a suggestion', 'Other');
    $form['feedback']['type'] = array(
            '#type' => 'select',
            '#options' => $type_options,
            '#prefix' => '<tr><th>Request Type*</th><td>',
            '#suffix' => '</td></tr>',
            '#required' => true,
    );

    $form['feedback']['more_detail'] = array(
            '#type' => 'textarea',
            '#value' => '',
            '#prefix' => '<tr><th>More Detail*</th><td>',
            '#suffix' => '</td></tr>',
            '#required' => true,
    );

    $form['feedback']['submit'] = array(
            '#type' => 'button',
            '#value' => 'Submit',
            '#executes_submit_callback' => TRUE,
            '#submit' => array('ntlp_help_feedback_submit'),
            '#prefix' => '<tr><td colspan="2" align="right">',
            '#suffix' => '</td></tr>',
            '#attributes' => array('class' => 'SubmitAssignmentBtn'),

    );


    return $form;
}
/*  Feedback Form END   */



/*  Feedback Form Submit START */
function ntlp_help_feedback_submit($form,$form_state) {

    global $user;


    $type = $form_state['clicked_button']['#post']['feedback']['type'];
    $detail = $form_state['clicked_button']['#post']['feedback']['more_detail'];
    $date =  strtotime("now");
     
    if($type != null) {

        db_query('INSERT INTO {ntlp_feedback} (request_type_tid, description, submitted_by, submitted)
                    VALUES (%d, "%s", %d, %d)', $type, $detail, $user->uid, $date);

    }

    drupal_set_message('Feedback successfully submitted.  Thank You!', 'status');

}

/*  Feedback Form Submit END */



/*  Support Form START   */
function help_support_form() {
    include('ntlp_help.template.inc');
    $form['support'] = array(
            '#type' => 'fieldset',
            '#title' => t(''),
            '#tree' => TRUE,
            '#prefix' =>  $HTMLform_school_setting_bluebox_start,
            '#suffix' => $HTMLform_school_setting_bluebox_end,
    );

    $form['support']['title'] = array(
            '#type' => 'item',
            '#value' => t('Site Admins: please provide as much detail as possible, and someone from the Network support team will get back to you as soon as possible'),
            '#prefix' => '<tr><td colspan="2">'.$HTMLform_course_msgbox1_hdr,
            '#suffix' => $HTMLform_course_msgbox1_ftr.'</td></tr>',
    );

//    $type_options = array('User login problem', 'System crashing', 'the system is really slow right now', 'Issues with browser compatability', 'I think i found a bug');

    $type_options = get_request_type('NTLP Support Request Type');

    $form['support']['type'] = array(
            '#type' => 'select',
            '#options' => $type_options,
            '#prefix' => '<tr><th>Request Type*</th><td>',
            '#suffix' => '</td></tr>',
            '#required' => true,
    );

    $priority_options = array('L' => 'Low', 'M' => 'Medium', 'H' => 'High');
    $form['support']['Priority'] = array(
            '#type' => 'select',
            '#options' => $priority_options,
            '#prefix' => '<tr><th>Priority*</th><td>',
            '#suffix' => '</td></tr>',
            '#required' => true,
    );

    $form['support']['more_detail'] = array(
            '#type' => 'textarea',
            '#value' => '',
            '#prefix' => '<tr><th>More Detail*</th><td>',
            '#suffix' => '</td></tr>',
            '#required' => true,
    );

    $form['support']['submit'] = array(
            '#type' => 'button',
            '#value' => 'Submit',
            '#executes_submit_callback' => TRUE,
            '#submit' => array('ntlp_help_support_submit'),
            '#prefix' => '<tr><td colspan="2" align="right">',
            '#suffix' => '</td></tr>',
            '#attributes' => array('class' => 'SubmitAssignmentBtn'),

    );


    return $form;
}
/*  Support Form END   */


/*  support Form Submit START */
function ntlp_help_support_submit($form,$form_state) {

    global $user;

    
    $type = $form_state['clicked_button']['#post']['support']['type'];
    $detail = $form_state['clicked_button']['#post']['support']['more_detail'];
    $priority = $form_state['clicked_button']['#post']['support']['Priority'];
   
    $date =  strtotime("now");

    if($type != null) {

        db_query('INSERT INTO {ntlp_support_request} (request_type_tid, priority, details, submitted_by, submitted)
                    VALUES (%d, %d , "%s", %d, %d)', $type, $priority ,$detail, $user->uid, $date);

    }

}

/*  support Form Submit END */

?>
