<?php

/* popup form */
function ntlp_project_myprojects_form($form_state = null, $useruid){
    $course_id = check_plain(arg(3));


        $project_node = node_load($projectnid);

    $project_node->title = $projectname;
    $project_node->body = $projectname_projsummary;



    $form['project_add'] = array(
            '#type'   => 'fieldset',
            '#title'  => '',
            '#tree'   => TRUE, // Don't forget to set #tree!
            '#prefix' => '<div id="project_add_test">'.$HTMLform_activity_outer_name_hdr,
            '#suffix' => $HTMLform_activity_outer_name_ftr.'</div>',
    );

    $form['project_add']['course_id'] = array(
            '#type' => 'hidden',
            '#value' =>  $course_id,
    );

    $form['project_add']['projectname'] = array(
            '#type' => 'item',
            '#title' => 'my projects',
            '#value' =>  '',
            '#size' => '30',
            '#field_prefix' => t('Project Name'),
    );



    return $form;
}




?>