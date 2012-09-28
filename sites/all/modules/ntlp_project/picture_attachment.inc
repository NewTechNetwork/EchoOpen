<?php


function ntlp_project_attachment() {
    return drupal_get_form('ntlp_project_attachment_form');
}


/* popup form */
function ntlp_project_attachment_form($form_state) {

    drupal_set_title("New Project");
    $course_id = check_plain(arg(3));

    $form['#attributes'] = array('enctype' => "multipart/form-data");


    $form['course_id'] = array(
            '#type' => 'hidden',
            '#value' =>  $course_id,
    );

    $form['project_id'] = array(
            '#type' => 'hidden',
            '#value' =>  $course_id,
    );

 
    $form['file_upload'] = array(
            '#type' => 'file',
            '#title' => 'Filename',
            '#id' => 'file_upload',
    );

    $form['proj_save_button'] = array(
            '#type' => 'submit',
            '#value' => t('Attach'),
//            '#ahah' => array(
//                    'path' => ahah_helper_path(array('project_add')),
//                    'wrapper' => 'project_add_test',
//                    'progress' => array('type' => 'throbber', 'message' => t('Saving project details..')),
//            ),
//            '#submit' => array('ntlp_project_attachment_data'),

    );

    return $form;
}


function ntlp_project_attachment_form_submit($form, $form_state) {

    
    //define your limits for the submission here
    watchdog('hello',print_r($form_state, true));
    $limits = array () ;

    $validators = array(
//            'file_validate_extensions' => array($limits['extensions']),
//            'file_validate_image_resolution' => array($limits['resolution']),
//            'file_validate_size' => array($limits['file_size'], $limits['user_size']),
    );
    // Save new file uploads.
//    if ($file = file_save_upload('file_upload', $validators, file_directory_path())) {
//        // Do something with $file here.
//    }

    return drupal_set_message("File Upload Successfully");
}



?>