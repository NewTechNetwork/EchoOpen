<?php

module_load_include('inc', 'ntlp_school', 'data_access');


function courses_term_frm() {
    return drupal_get_form('courses_term_form');
}

function courses_term_form() {
  
    $school_id = 5;

    $form = array();

    $form['courses'] = array(
            '#type' => 'fieldset',
    );


    $form['courses']['main_table'] = array(
            '#type' => 'item',
            '#value' => '',
            '#prefix' => '<table>',
            '#suffix' => '</table>',
    );

    $form['courses']['main_table']['rows'] = array(
            '#type' => 'item',
            '#prefix' => '<tr>',
            '#suffix' => '</tr>',
    );

    $form['courses']['main_table']['rows']['tdata2'] = array(
            '#type' => 'item',
            '#value' => t('School Year'),
            '#prefix' => '<td>',
            '#suffix' => '</td>',
    );

    $options = get_school_years();
    $form['courses']['main_table']['rows']['school_year'] = array(
            '#type' => 'select',
            '#options' => $options,
            '#prefix' => '<td>',
            '#suffix' => '</td>',
    );
    $current_term =  get_current_term();
    echo 'current term '.$current_term->name;
    $school_term_Obj = get_ntlp_school_term($school_id);

    foreach($school_term_Obj as $obj) {
        $term_obj = taxonomy_get_term($obj->tid);

        $form['courses']['main_table']['rows1_'.$obj->tid] = array(
                '#type' => 'item',
                '#prefix' => '<tr>',
                '#suffix' => '</tr>',
        );

        $form['courses']['main_table']['rows1_'.$obj->tid]['term_'.$obj->tid] = array(
                '#type' => 'item',
                '#value' => $term_obj->name,
                '#prefix' => '<td>',
                '#suffix' => '</td>',
        );

        $form['courses']['main_table']['rows2_'.$obj->tid] = array(
                '#type' => 'item',
                '#prefix' => '<tr>',
                '#suffix' => '</tr>',
        );

        $form['courses']['main_table']['rows2_'.$obj->tid]['view_'.$obj->tid] = array(
                '#type' => 'item',
                '#value' => 'load view here',
                '#prefix' => '<td>',
                '#suffix' => '</td>',
        );

    }

    return $form;

}


function is_current_term($term_tid) {

    $school_terms = get_ntlp_school_term(5);
}

function get_current_term() {
    $school_terms = get_ntlp_school_term(5);

//    $today = date("n-j-Y");
    $current_term ="";
    $today = date("n-j");

    foreach($school_terms as $term_Obj) {
        $start_date = $term_Obj->month.'-'.$term_Obj->day;
        $close_date = $term_Obj->to_month.'-'.$term_Obj->to_day;

        if($today >= $start_date && $today <= $close_date) {
            $current_term = $term_Obj->tid;
            break;
        }
    }

    return taxonomy_get_term($current_term);
}

?>
