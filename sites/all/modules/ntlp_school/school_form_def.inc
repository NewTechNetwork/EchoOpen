<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'reports');
module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_grade', 'data_access');
module_load_include('inc', 'ntlp_school', 'basic_grade_report');
module_load_include('inc', 'ntlp_school', 'timezone');


/* Form To  Course Settings In School Management  START */

function school_course_settings_form($form_state) {
    global $base_path;
    jquery_ui_add(array('ui.dialog', 'ui.draggable', 'ui.datepicker'));

    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/school_ahah_helper.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/ntlp_school.js');
    /*
     *  Date Files
     */
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/date_picker.js');
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/jquery.ui.datepicker.css', 'module', 'all', FALSE);
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/demos.css', 'module', 'all', FALSE);
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/jquery.ui.all.css', 'module', 'all', FALSE);

    include('school_management.template.inc');
    $school_nid = get_this_school();

    if (isset($form_state['values'])) {

        $view_state = $form_state['values'];
        $count = $form_state['values']['main']['count'];
        $button_name = $form_state['values']['op'];
        $count_outcomes = $form_state['values']['learning_placeholder']['learning_placeholder_data']['count_outcomes'];
        $school_nid = $form_state['values']['main']['school_id'];
        $school_year = $form_state['values']['main']['school_year_selector'];

        drupal_add_js('init_jquery_dialog();');
    } else {

        if (isset($_GET['foryear'])) {
            $school_year = $_GET['foryear'];
            if (!is_this_school_year_exist($school_nid, $school_year)) {
                drupal_goto('ntlp/school/management/course');
            }
        } else {
            $school_year = get_school_year($school_nid);
        }
    }

    $school_rs = get_school_record($school_nid);

    $term_data = get_term_record($school_year, $school_nid);

    if (!$view_state) {
        $count = count($term_data);
    }

    if (empty($count)) {

        $count = 0;
    }

    $form = array();
    $form['title'] = array(
        '#type' => 'item',
        '#value' => t('Course Settings'),
        '#prefix' => '<table width="100%" style="table-layout:fixed;"><tr><td style="color:#3570AA;font-size:18px;font-weight:bold;">',
        '#suffix' => '</td>',
    );

    $form['save'] = array(
        '#type' => 'item',
        '#value' => t(''),
//        '#executes_submit_callback' => TRUE,
//        '#submit' => array('school_course_settings_submit'),
//        '#attributes' => array('class' => 'SubmitAssignmentBtn', 'style' => 'display:none !important;'),
        '#prefix' => '<td><span id="loading" style="display:none;float:left;margin-left: 180px;">Saving data..<img  src="' . $base_path . 'themes/Boldr/Images/common/throbber-active.gif"></span><a onclick="check_term_changes(' . $school_nid . ',' . $school_year . ', 0)" class="Activities_btn"> Save Changes</a> ',
        '#suffix' => '</td></tr></table>',
    );
    $form['main'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => '<div id="add-user-wrapper"><div id="dialog-confirm-massaction" style="background-color:#fff;display:none;" title="WARNING:">
    <p><span class="msg">Are you sure you want to select this action?</span></p>
</div>' . $HTMLform_school_setting_bluebox_start,
        '#suffix' => $HTMLform_school_setting_bluebox_end . '</div>',
    );


    $form['main']['school_id'] = array(
        '#type' => 'hidden',
        '#value' => $school_nid,
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div>',
    );
    $form['main']['school_year'] = array(
        '#type' => 'hidden',
        '#value' => $school_year,
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div>',
    );


    watchdog('ifo', 'count term' . $count);
    $count = ($button_name == 'Add New Term') ? $count + 1 : $count;
    $form['main']['count'] = array(
        '#type' => 'hidden',
        '#value' => $count,
        '#prefix' => '<div style="display:none">',
        '#suffix' => '</div>',
    );

    $form['main']['heading1'] = array(
        '#type' => 'item',
        '#value' => t('Define Terms'),
        '#prefix' => '<tr><td colspan="5"><div><div style="float:left;color:#3570AA;font-size:15px;font-weight:bold;width: 200px;">',
        '#suffix' => '</div><div style="float:right;">',
    );

    $school_year_options = _get_all_school_years($school_nid);

    if ($school_year_options) {
        $form['main']['school_year_selector'] = array(
            '#type' => 'select',
            '#options' => $school_year_options,
            '#default_value' => $school_year,
                '#prefix' => '<div style="float:left;width:210px;"><div style="float:left; width:190px;">
                <span style="float: left; line-height: 20px; font-size: 14px; color: #5E8FAD;">School Year:&nbsp;</span>
                <span style="width:100px;" class="comboboxBgLeft">',
                '#suffix' => '</span></div></div>',
            '#attributes' => array('class' => 'my-dropdown', 'onchange' => 'window.location = \'?q=ntlp/school/management/course&foryear=\'+this.value;'),
        );
    }
    $form['main']['new_school_year'] = array(
        '#type' => 'item',
        '#value' => dlg('+ Create New School Year', 'ntlp/school/create/new/year/' . get_this_school(), 500, 500) . '</div>',
        '#prefix' => '<div style="float:right; margin: 5px 0 0 0 ;">',
        '#suffix' => '</div></div></div></td></tr>',
    );

    $form['main']['heading2'] = array(
        '#type' => 'item',
        '#value' => t('A "term" is defined in the system as a period of time ending when a final, recorded grade is due, such as a semester or trimester - not a quarter. Please define the terms for your current school year below'),
        '#prefix' => '<tr><td colspan="5">',
        '#suffix' => '</td></tr>',
    );


//    watchdog('info', 'check value of count inside formstate' . $count);
    if ($count > 0) {
        $form['main']['termname_heading'] = array(
            '#type' => 'item',
            '#value' => t('End Date'),
            '#prefix' => '<tr ><th style="text-align:left">' . t('Term Name') . '</th>
                <th style="text-align:left">' . t('Term Abbreviation') . '</th>
                    <th style="text-align:left">' . t('Export ID') . '</th>
                        <th style="text-align:left">' . t('Start Date') . '</th>
                            <th style="text-align:left">',
            '#suffix' => '</th></tr>',
        );
    }
    for ($i = 0; $i <= ($count - 1); $i++) {

        $form['main']['termname_' . $i] = array(
            '#type' => 'textfield',
            '#default_value' => !empty($term_data[$i]['name']) ? $term_data[$i]['name'] : $form_state['clicked_button']['#post']['termname'][$i],
            '#prefix' => '<tr><td>',
            '#suffix' => '</td>',
            '#size' => 25,
            '#name' => 'termname[' . $i . ']',
        );

//        watchdog('info','checking tids'.$term_data[$i]['tid']);

        $form['main']['tids_' . $i] = array(
            '#type' => 'hidden',
            '#value' => !empty($term_data[$i]['school_term_tid']) ? $term_data[$i]['school_term_tid'] : $form_state['post']['tids'][$i],
            '#name' => 'tids[' . $i . ']',
            '#prefix' => '<div style="display:none">',
            '#suffix' => '</div>',
        );

        $form['main']['termabrr_' . $i] = array(
            '#type' => 'textfield',
            '#value' => !empty($term_data[$i]['term_abbrv']) ? $term_data[$i]['term_abbrv'] : $form_state['post']['termabb'][$i],
            '#prefix' => '<td>',
            '#suffix' => '</td>',
            '#size' => 25,
            '#name' => 'termabb[' . $i . ']',
        );

        $form['main']['exportid_' . $i] = array(
            '#type' => 'textfield',
            '#default_value' => !empty($term_data[$i]['export_id']) ? $term_data[$i]['export_id'] : $form_state['post']['exportid'][$i],
            '#prefix' => '<td>',
            '#suffix' => '</td>',
            '#size' => 10,
            '#name' => 'exportid[' . $i . ']',
        );


        //$start_date = date('m/j/Y', strtotime($term_data[$i]['date_started']));
        $start_date = get_tz($school_nid, 'm/j/Y', ($term_data[$i]['date_started']));
        //$start_date =   convert_datetime_tz($school_nid,$term_data[$i]['date_started'],true,'m/j/Y');


        $default_startdate = $form_state['post']['enddate'][$i];

        if (empty($default_startdat)) {
            $default_startdat = date('m/j/Y');
        }

        $form['main']['startdate_' . $i] = array(
            '#type' => 'textfield',
            '#value' => !empty($term_data[$i]['date_started']) ? $start_date : $default_startdat,
            '#prefix' => '<td width="120">',
            '#suffix' => '</td>',
            '#size' => 10,
            '#required' => true,
            '#name' => 'startdata[' . $i . ']',
            '#attributes' => array('class' => 'ntlp_datepicker'),
        );

        //$end_date = date('m/j/Y', strtotime($term_data[$i]['date_closed']));
        $end_date = get_tz($school_nid, 'm/j/Y', ($term_data[$i]['date_closed']));
        $default_enddate = $form_state['post']['enddate'][$i];

        if (empty($default_enddate)) {
            $default_enddate = date('m/j/Y');
        }
        if ($term_data[$i]['school_term_tid'] != null) {
            $delete_image = '<a onclick="delete_school_term(' . $term_data[$i]['school_term_tid'] . ',' . $school_nid . ',' . $school_year . ')"><img src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png" ></a>';
        } else {
            $delete_image = '';
        }
        $form['main']['enddate_' . $i] = array(
            '#type' => 'textfield',
            '#value' => !empty($term_data[$i]['date_closed']) ? $end_date : $default_enddate,
            '#prefix' => '<td width="120"><div style="width:120px;float:left;">',
            '#suffix' => '</div><div style="float:right;">' . $delete_image . '</div></td></tr>',
            '#size' => 10,
            '#required' => true,
            '#name' => 'enddate[' . $i . ']',
            '#attributes' => array('class' => 'ntlp_datepicker'),
        );
    }

    $form['main']['add_term'] = array(
        '#type' => 'button',
        '#value' => t('Add New Term'),
//            '#executes_submit_callback' => TRUE,
//            '#submit' => array('school_course_settings_submit'),
        '#attributes' => array('class' => 'SubmitAssignmentBtn'),
        '#ahah' => array(
            'event' => 'click',
            'path' => ahah_helper_path(array('main')),
            'wrapper' => 'add-user-wrapper',
            'method' => '',
            'effect' => 'fade',
        ),
        '#prefix' => '<tr><td colspan="4">' . (($count > 0) ? '' : '<div style="border:1px solid #F17B23;padding:3px">' . t('No terms have been created for this school year yet.  Teachers will not be able to create courses for this school year until terms have been defined.') . '</div>') . '</td><td>',
        '#suffix' => '</td></tr>',
    );


    $form['grade'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => $HTMLform_school_setting_bluebox_start,
        '#suffix' => $HTMLform_school_setting_bluebox_end,
    );
    $form['grade']['heading_grade'] = array(
        '#type' => 'item',
        '#value' => t('Grade Settings'),
        '#prefix' => '<tr><td colspan="2" style="color:#3570AA;font-size:15px;font-weight:bold;">',
        '#suffix' => '</td></tr>',
    );



    $form['grade']['grade_rounding'] = array(
        '#type' => 'radios',
        '#attribute' => array('class' => 'radiobtnfloat'),
        '#prefix' => '<tr><th width="343">Grade Rounding</th><td>',
        '#suffix' => '</td></tr>',
        '#options' => array('0' => 'No Decimal', '1' => 'One Decimal'),
        '#default_value' => isset($school_rs) ? $school_rs->grade_rounding : '0',
    );

    $form['grade']['grade_scale'] = array(
        '#type' => 'radios',
        '#attribute' => array('class' => 'radiobtnfloat'),
        '#prefix' => '<tr><th width="363">Allow Course Grade Scales to override Default Grade Scale:</th><td>',
        '#suffix' => '</td></tr>',
        '#options' => array('1' => 'Yes', '0' => 'No'),
        '#default_value' => isset($school_rs) ? $school_rs->grade_scale : '0',
    );

    $form['learning_placeholder'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => '<table><tr>',
        '#suffix' => '</tr></table>',
    );


    $form['learning_placeholder']['learning_grad'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => '<td valign="top">' . $HTMLform_school_setting_bluebox_start,
        '#suffix' => $HTMLform_school_setting_bluebox_end . '</td><td style="width:10px">&nbsp;</td>',
    );

    $form['learning_placeholder']['learning_grad']['gradelabel_main_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<tr><td colspan="2" style="color:#3570AA;font-size:15px;font-weight:bold;">Default Grade Scale',
        '#suffix' => '</td></tr>',
    );

    $form['learning_placeholder']['learning_grad']['gradelabel_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<tr><th style="text-align:left" align="left">Grade Label',
        '#suffix' => '</th>',
    );
    $form['learning_placeholder']['learning_grad']['grade_lowerlimit_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="text-align:left" align="left">Grade Lower Limit cutoff',
        '#suffix' => '</th></tr>',
    );


    $grades = get_label_outcomes($school_nid, $school_year);

    for ($j = 0; $j <= 14; $j++) {

        $form['learning_placeholder']['learning_grad']['gradelabel_' . $j] = array(
            '#type' => 'textfield',
            '#prefix' => '<tr><td>',
            '#suffix' => '</td>',
            '#size' => 5,
            '#value' => isset($grades) ? $grades[$j]['label'] : $form_state['post']['label'][$j],
            '#name' => 'grade_label[' . $j . ']',
        );
        $form['learning_placeholder']['learning_grad']['gradecutoff_' . $j] = array(
            '#type' => 'textfield',
            '#prefix' => '<td>',
            '#suffix' => '</td></tr>',
            '#size' => 5,
            '#value' => isset($grades) ? $grades[$j]['lower_limit'] : $form_state['post']['grade_limit'][$j],
            '#name' => 'grade_limit[' . $j . ']',
        );
    }

    $form['learning_placeholder']['learning_placeholder_data'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => '<td valign="top" id="school-outcomes-wrapper">' . get_school_outcomes_data_block($school_nid, $school_year),
        '#suffix' => '</td>',
    );


    return $form;
}

function get_school_outcomes_data_block($school_nid, $school_year) {
    include('school_management.template.inc');
    $outcomes_data = '';
    $outcome_data = get_term_outcome_record($school_nid, $school_year);


    $count_outcomes = count($outcome_data);


    $container = $HTMLform_school_setting_bluebox_start1;

    $container = str_replace('@ADD_OUTCOME_LINK', dlg('+ Add Outcome', 'ntlp/school/add/outcome/' . $school_nid . '/' . $school_year, 500, 500, 'new_green_link'), $container);

    for ($i = 0; $i < $count_outcomes; $i++) {

        $outcome_entry = $HTMLform_school_outcome_item;

        $outcome_entry = str_replace('@OUTCOME_NAME', $outcome_data[$i]['name'], $outcome_entry);
        $outcome_entry = str_replace('@OUTCOME_SHORT_NAME', $outcome_data[$i]['short_name'], $outcome_entry);
        $outcome_entry = str_replace('@OUTCOME_DESCRIPTION', $outcome_data[$i]['description'], $outcome_entry);
        $outcome_entry = str_replace('@DELETE_OUTCOME_LINK', '<a onclick="delete_school_outcome(' . $outcome_data[$i]["tid"] . ',' . $school_nid . ',' . $school_year . ');"><img height="14" width="14"  class="down" src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png" /></a>', $outcome_entry);
        $outcome_entry = str_replace('@EDIT_OUTCOME_LINK', dlg('&nbsp;&nbsp;&nbsp;&nbsp;', 'ntlp/school/edit/outcome/' . $school_nid . '/' . $school_year . '/' . $outcome_data[$i]["tid"], 600, 500, 'Edit_pencil new_green_link', true), $outcome_entry);

        if ($i == 0 || $count_outcomes == 1) {
            $outcome_entry = str_replace('@ORDER_CHANGE_UP', '<a onclick="change_outcome_order(\'up\',' . $school_nid . ',' . $school_year . ',' . $outcome_data[$i]["tid"] . ',' . $outcome_data[$i]["sort_order"] . ')"><img height="14" width="14"  class="down" src="' . $base_path . 'themes/Boldr/Images/resourceStuff/ArrowGrayUp.png" /></a>', $outcome_entry);
        } else {
            $outcome_entry = str_replace('@ORDER_CHANGE_UP', '<a onclick="change_outcome_order(\'up\',' . $school_nid . ',' . $school_year . ',' . $outcome_data[$i]["tid"] . ',' . $outcome_data[$i]["sort_order"] . ')"><img height="14" width="14"  class="down" src="' . $base_path . 'themes/Boldr/Images/resourceStuff/ArrowGreenUp.png" /></a>', $outcome_entry);
        }
        if (($count_outcomes - 1) == $i || $count_outcomes == 1) {
            $outcome_entry = str_replace('@ORDER_CHANGE_DOWN', '<a onclick="change_outcome_order(\'down\',' . $school_nid . ',' . $school_year . ',' . $outcome_data[$i]["tid"] . ',' . $outcome_data[$i]["sort_order"] . ')"><img height="14" width="14"  class="down" src="' . $base_path . 'themes/Boldr/Images/resourceStuff/ArrowGrayDown.png" /></a>', $outcome_entry);
        } else {
            $outcome_entry = str_replace('@ORDER_CHANGE_DOWN', '<a onclick="change_outcome_order(\'down\',' . $school_nid . ',' . $school_year . ',' . $outcome_data[$i]["tid"] . ',' . $outcome_data[$i]["sort_order"] . ')"><img height="14" width="14"  class="down" src="' . $base_path . 'themes/Boldr/Images/resourceStuff/ArrowGreenDown.png" /></a>', $outcome_entry);
        }
        $outcomes_data .= $outcome_entry;
    }


    $container = str_replace('<!-- @OUTCOME_ENTRIES -->', $outcomes_data, $container);

    return $container;
}

function school_course_settings() {
    return drupal_get_form('school_course_settings_form');
}

/* Form To  Course Settings In School Management  END */

function school_course_settings_form_validate($form, $form_state) {
    $start_date = $form_state['clicked_button']['#post']['startdata'];
    $end_date = $form_state['clicked_button']['#post']['enddate'];
    $end_school_year = $form_state['clicked_button']['#post']['main']['school_year'];

    $term_name = $form_state['clicked_button']['#post']['termname'];
    $start_school_year = $end_school_year - 1;
    $totalDates = count($start_date);

    for ($i = 0; $i < $totalDates; $i++) {

        $sdate = date("Y", strtotime($start_date[$i]));
        $smonth = date("m", strtotime($start_date[$i]));
        $edate = date("Y", strtotime($end_date[$i]));
        $emonth = date("m", strtotime($end_date[$i]));


        if (($sdate >= $start_school_year && $sdate <= $end_school_year) && ($edate >= $start_school_year && $edate <= $end_school_year)) {

            if (strtotime($start_date[$i]) > strtotime($end_date[$i])) {
                form_set_error('', 'Term dates for ' . $term_name[$i] . ' are not valid.');
            }
        } else {
            form_set_error('', 'Term dates for ' . $term_name[$i] . ' are not valid.');
        }
    }

//    variable_set('jquery_update_compression_type', 'none');
    jquery_ui_add(array('ui.dialog', 'ui.draggable', 'ui.datepicker'));

    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/school_ahah_helper.js');
    /*
     *  Date Files
     */
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/date_picker.js');
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/jquery.ui.datepicker.css', 'module', 'all', FALSE);
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/demos.css', 'module', 'all', FALSE);
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/jquery.ui.all.css', 'module', 'all', FALSE);


//        if( strtotime($start_date[$i]) <  $end_school_year || strtotime($start_date[$i] > $start_school_year ) {
//
//        }elseif()
//    }
}

function ntlp_school_create_new_year_frm($school_nid) {

    return drupal_get_form('ntlp_school_create_new_year_form', $school_nid);
}

function ntlp_school_create_new_year_form($form_state = null, $school_nid) {

    include('school_management.template.inc');
    modalframe_child_js();

    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/ntlp_school.js');

    $form['main'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => '<div id="add-user-wrapper">' . $HTMLform_school_setting_bluebox_start,
        '#suffix' => $HTMLform_school_setting_bluebox_end . '</div>',
    );

    $max_school_year = get_max_school_year($school_nid);
    if ($max_school_year != 0) {
       $temp_year = $max_school_year;
    } else {
       $temp_year = date("Y");
   }
    for ($i = 0; $i < 3; $i++) {
        $temp_year = ($temp_year + 1);
        $year_selection[$temp_year] = ($temp_year - 1) . ' - ' . $temp_year;
    }

    $form['main']['school_nid'] = array(
        '#type' => 'hidden',
        '#value' => $school_nid,
        '#id' => 'school_nid',
    );

    $form['main']['heading'] = array(
        '#type' => 'select',
        '#options' => $year_selection,
        '#default_value' => $selected_year,
        '#prefix' => '<div style="width:400px;height:20px;padding-bottom: 10px;"><div style="float:left;width:120px;">' . t('Select New Year') . '</div><div style="padding-left: 162px;"><span class="comboboxBgLeft" style="width:200px;">',
        '#suffix' => '</span></div></div>',
        '#attributes' => array('class' => "my-dropdown"),
        '#id' => 'selected_new_year'
    );

    $form['main']['year_content'] = array(
        '#type' => 'item',
        '#value' => '<input type="checkbox" name="content[]" value="gs" /> ' . t('Grading Scale (Copy Current Year\'s Grading Scale)') . '
            <br/><input type="checkbox" name="content[]" value="lo" /> ' . t('Learning Outcomes (Copy Current Year\'s Schoolwide Learning Outcomes)'),
        '#prefix' => '<div style="height:20px;padding-bottom: 10px;"><div style="float:left;width:450px;" id="testCheck">',
        '#suffix' => '</div></div>',
    );

    $option = '<div style="float:left;padding-right:2px;"><span><a class="PermanentlyDeleteBtn" onclick="create_new_year();">Create School Year</a></span></div>';
    $form['main']['new_year_btn'] = array(
        '#type' => 'item',
        '#value' => '<div style="padding-top:52px;width:400px;" class="clearfix">' . $option . '<div style="float:left;padding-left:2px;"><span><a class="CancelBtn" style="text-align:center;" onclick="parent.Drupal.modalFrame.close();"><span style="position:relative;top:3px;">Cancel</span></a></span></div></div>',
    );

    return $form;
}

function ntlp_school_create_new_year_data($school_nid, $new_year, $selected_option) {
    global $user;

    if ($selected_option == 'none') {

        db_query("INSERT INTO {ntlp_school_term} (school_nid, school_year, modified_by, 
          school_term_tid, date_started, date_closed, gb_closed_date) 
          VALUES (%d, %d, %d, 0, '0000-00-00 00:00:00', '0000-00-00 00:00:00', '0000-00-00 00:00:00')", $school_nid, $new_year, $user->uid);
    } else {

        db_query("INSERT INTO {ntlp_school_term} (school_nid, school_year, modified_by, 
          school_term_tid, date_started, date_closed, gb_closed_date) 
          VALUES (%d, %d, %d, 0, '0000-00-00 00:00:00', '0000-00-00 00:00:00', '0000-00-00 00:00:00')", $school_nid, $new_year, $user->uid);

        $selected_option_arr = explode(',', $selected_option);
        watchdog('new_year', print_r($selected_option_arr, true));
        $active_school_year = get_school_year($school_nid);

        foreach ($selected_option_arr as $option) {

            $option = trim($option);
            if ($option == 'gs') {
//                watchdog('new_year', print_r($selected_option_arr, true));
                db_query("DELETE FROM {ntlp_school_grade} 
                    WHERE school_nid = %d AND school_year = %d", $school_nid, $new_year);

                db_query("INSERT INTO {ntlp_school_grade}
                        (SELECT school_nid, label, lower_limit, %d FROM {ntlp_school_grade} 
                        WHERE school_nid = %d AND school_year = %d)", $new_year, $school_nid, $active_school_year);
            } else if ($option == 'lo') {
                watchdog('sc_lo_copy', print_r($selected_option_arr, true) . ' ' . $option);
                db_query("INSERT INTO {ntlp_gb_outcome}
                    (SELECT tid, school_nid, weight_percent, sort_order, short_name, %d
                    FROM {ntlp_gb_outcome} WHERE school_nid = %d AND school_year = %d)", $new_year, $school_nid, $active_school_year);
            }
        }
    }
}

/* Form General Settings In School Management START */

function school_general_settings_form($form_state = null) {

    global $user;
    
    get_permission_load();
    include('school_management.template.inc');
    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'image_uploader') . '/image_uploader.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/ntlp_school.js', 'footer');
    $school_nid = get_this_school();

    $school_rs = get_school_record($school_nid);

//    $user_role = check_user_permission($school_nid);
//
//    if($user_role == NTLP_ROLEID_SITEADMIN || $user_role == NTLP_ROLEID_SCHOOLLEADER
//            || $user_role == NTLP_ROLEID_NETWORKMANAGER || $user_role == NTLP_ROLEID_NETWORKSTAFF) {
//
//    }else {
//        drupal_set_message("You're Not Allowed to access School Setting Page ");
//        drupal_goto('ntlp/goback') ;
//    }

    $form['main'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => '<div id="dialog-confirm-massaction" style="background-color:#fff;display:none;" title="WARNING:">
                <p><span class="msg">Are you sure you want to select this action?</span></p>
            </div><table cellpadding="0" cellspacing="0" width="100%" >',
        '#suffix' => '</table>',
    );


    $form['main']['school_id'] = array(
        '#type' => 'hidden',
        '#id' => 'school_id',
        '#value' => $school_nid,
    );


    $form['main']['title'] = array(
        '#type' => 'item',
        '#value' => t('General Settings'),
        '#prefix' => '<tr><td style="color:#3570AA;font-size:18px;font-weight:bold;">',
        '#suffix' => '</td>',
    );

    $form['main']['save_changes'] = array(
        '#type' => 'submit',
        '#value' => t('Save Changes'),
//            '#executes_submit_callback' => TRUE,
        '#submit' => array('school_general_settings_save'),
        '#attributes' => array('class' => 'SubmitAssignmentBtn', 'onclick' => "$('input[type=\"checkbox\"]:disabled').attr('disabled', false)"),
        '#prefix' => '<td>',
        '#suffix' => '</td></tr><tr><td>&nbsp;</td></tr>',
    );

    $form['main']['school_info'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => '<tr><td>' . $HTMLform_school_setting_info_bluebox_start,
        '#suffix' => $HTMLform_school_setting_info_bluebox_end . '</td>',
    );
//
//
    $pictures = _get_fid_detail($school_rs->fid);

    $picture = $pictures->filepath;
    if ($picture == null || empty($picture)) {
        $picture = $base_path . 'themes/Boldr/Images/default/school-default.png';
    } else {
        $picture = $base_path . $picture;
    }
//

    if ($picture != null) {

        $testing = drupal_get_form('ntlp_activeimage', $picture, 0, 150);
        $testing = preg_match('@<form .*?>([\s\S]*)</form>@', $testing, $matches);
        $testing = $matches[1];
        $testing = str_replace('name="form_build_id"', '', $testing);
        $testing = str_replace('name="form_id"', '', $testing);
        $testing = str_replace('name="form_token"', '', $testing);

        $form['main']['school_info']['picture'] = array(
            '#type' => 'item',
//              '#id' => 'changeimage',
            '#value' => '<div id="school_image" align="left">' . $testing . '</div>',
            '#prefix' => '<tr><td rowspan="2" style="width:155px;">',
            '#suffix' => '</td>',
        );
    } else {
        $form['main']['school_info']['picture'] = array(
            '#type' => 'item',
//              '#id' => 'changeimage',
            '#value' => '<div id="school_image" align="left"><img src="' . $picture . '" width="150" alt="Picture of " /></div>',
            '#prefix' => '<tr><td rowspan="2" style="width:155px;">',
            '#suffix' => '</td>',
        );
    }

    $form['main']['school_info']['school_name'] = array(
        '#type' => 'textfield',
        '#value' => (isset($school_rs)) ? $school_rs->title : '',
        '#prefix' => '<th>School Name:</th><td colspan="3">',
        '#suffix' => '</td></tr>',
        '#size' => 50,
    );

    $cities = array("ALABAMA"
        , "ALASKA"
        , "ARIZONA"
        , "ARKANSAS"
        , "CALIFORNIA"
        , "COLORADO"
        , "CONNECTICUT"
        , "DELAWARE"
        , "FLORIDA"
        , "GEORGIA"
        , "HAWAII"
        , "IDAHO"
        , "ILLINOIS"
        , "INDIANA"
        , "IOWA"
        , "KANSAS"
        , "KENTUCKY"
        , "LOUISIANA"
        , "MAINE"
        , "MARYLAND"
        , "MASSACHUSETTS"
        , "MICHIGAN"
        , "MINNESOTA"
        , "MISSISSIPPI"
        , "MISSOURI"
        , "MONTANA"
        , "NEBRASKA"
        , "NEVADA"
        , "NEW HAMPSHIRE"
        , "NEW JERSEY"
        , "NEW MEXICO"
        , "NEW YORK"
        , "NORTH CAROLINA"
        , "NORTH DAKOTA"
        , "OHIO"
        , "OKLAHOMA"
        , "OREGON"
        , "PENNSYLVANIA"
        , "RHODE ISLAND"
        , "SOUTH CAROLINA"
        , "SOUTH DAKOTA"
        , "TENNESSEE"
        , "TEXAS"
        , "UTAH"
        , "VERMONT"
        , "VIRGINIA"
        , "WASHINGTON"
        , "WEST VIRGINIA"
        , "WISCONSIN"
        , "WYOMING");
//    $states = array('No state');
    $form['main']['school_info']['school_state'] = array(
        '#type' => 'select',
        '#options' => $cities,
        '#default_value' => (isset($school_rs)) ? $school_rs->location_tid : '',
        '#prefix' => '<tr><th>State:</th><td><span style="width:100px;" class="comboboxBgLeft">',
        '#suffix' => '</span></td>',
        '#attributes' => array('class' => 'my-dropdown'),
    );

    $form['main']['school_info']['export_id'] = array(
        '#type' => 'textfield',
        '#value' => (isset($school_rs)) ? $school_rs->school_export_id : '',
        '#prefix' => '<th>Export ID:</th><td>',
        '#suffix' => '</td></tr>',
        '#size' => 5,
    );

    $form['main']['school_year'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => '<td valign="top">' . $HTMLform_school_setting_bluebox_schoolyear_start,
        '#suffix' => $HTMLform_school_setting_bluebox_schoolyear_end . '</td></tr><tr><td colspan="2">&nbsp;</td></tr>',
    );

    $school_years = get_school_years($school_rs->current_school_year);

    if ((isset($user->roles[NTLP_ROLEID_NETWORKMANAGER]) || isset($user->roles[NTLP_ROLEID_SYSTEM_ADMIN]))) {
        $form['main']['school_year'] ['school_active_table'] = array(
            '#type' => 'item',
            '#value' => t('Active'),
            '#prefix' => '<tr><td><table width="100%" cellspacing="0" cellpadding="0" border="0" class="coursesInnertable"><tr><th style="border:1px dotted #fff;border-collapse:collapse;background-color:#63a2d8;color:#fff;width:50%;text-align:center">',
            '#suffix' => '</th>',
        );
        $form['main']['school_year'] ['school_active'] = array(
            '#type' => 'radios',
            '#options' => array('S' => 'Yes', 'H' => 'No'),
            '#default_value' => (isset($school_rs) ? (($school_rs->status == 1) ? 'S' : 'H') : 'H'),
            '#prefix' => '<tr><td style="border:1px dotted #fff;border-collapse:collapse; text-align:center">',
            '#suffix' => '</td></tr></table></td></tr>',
        );
    }

    $form['main']['school_year']['current_school_year'] = array(
        '#type' => 'select',
        '#options' => $school_years,
        '#default_value' => (isset($school_rs)) ? $school_rs->current_school_year : '',
        '#prefix' => '<tr><td style="border-bottom: 0 none;font-size: 12px;font-weight: bold;vertical-align: bottom; color: #3570AA; "><span>Active School Year</span></td></tr><td style="vertical-align: top;"><span style="width:120px;" class="comboboxBgLeft">',
        '#suffix' => '</span></td></tr>',
        '#attributes' => array('class' => 'my-dropdown', 'onchange' => 'change_school_year_call(this,' . $school_rs->current_school_year . ')'),
    );

    $form['main']['google_application_settings'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => '<tr><td colspan="2">' . $HTMLform_school_setting_bluebox_start,
        '#suffix' => $HTMLform_school_setting_bluebox_end . '</td></tr><tr><td colspan="2">&nbsp;</td></tr>',
    );

    $form['main']['google_application_settings']['heading'] = array(
        '#type' => 'item',
        '#value' => 'Google Apps Settings',
        '#prefix' => '<tr><td style="color:#3570AA;font-size:15px;font-weight:bold;">',
        '#suffix' => '</td></tr>',
    );


    $form['main']['google_application_settings']['_heading'] = array(
//            '#type' => 'item',
        '#value' => t('Google Mail'),
        '#prefix' => '<th rowspan="2" style="width:250px;padding:5px"><table class="coursesInnertable" width="810" cellpadding="0" cellspacing="0" border="0"><tr><td style="border:1px dotted #fff;border-collapse:collapse;background-color:#63a2d8;color:#fff;width:33.33%; text-align:center;height:20px;">',
        '#suffix' => '</td><td style="border:1px dotted #fff;border-collapse:collapse;background-color:#63a2d8;color:#fff;width:33.33%;text-align:center">' . t('Google Docs') . '</td><td style="border:1px dotted #fff;border-collapse:collapse;background-color:#63a2d8;color:#fff;width:33.33%;text-align:center">' . t('Google Sites') . '</td></tr>',
    );

    $form['main']['google_application_settings']['google_mail'] = array(
        '#type' => 'radios',
        '#prefix' => '<tr><td align="center" style="border:1px dotted #fff;border-collapse:collapse;text-align:center;height:44px;color:#000;font-weight:normal"><div style="margin:0 auto;width:100px">',
        '#suffix' => '</div></td>',
        '#options' => array('1' => 'On', '0' => 'Off'),
        '#default_value' => (isset($school_rs)) ? $school_rs->gmail_status : '1',
    );
    $form['main']['google_application_settings']['google_docs'] = array(
        '#type' => 'radios',
        '#prefix' => '<td style="border:1px dotted #fff;border-collapse:collapse;text-align:center;height:44px;color:#000;font-weight:normal"><div style="margin:0 auto;width:100px">',
        '#suffix' => '</div></td>',
        '#options' => array('1' => 'On', '0' => 'Off'),
        '#default_value' => (isset($school_rs)) ? $school_rs->gdocs_status : '1',
    );

    $form['main']['google_application_settings']['google_sites'] = array(
        '#type' => 'radios',
        '#options' => array('1' => 'On', '0' => 'Off'),
        '#default_value' => (isset($school_rs)) ? $school_rs->gsite_status : '1',
        '#prefix' => '<td align="center" style="border:1px dotted #fff;border-collapse:collapse; text-align:center"><div style="margin:0 auto;width:100px">',
        '#suffix' => '</div></td></tr></table></th></tr>',
    );

//    $form['main']['google_application_settings']['warning'] = array(
//            '#type' => 'item',
////            '#value' => 'Warning - Turning any Google Apps tool off will remove all integration of that tool within the systems',
//            '#prefix'=>'<tr><td>',
//            '#suffix'=>'</td></tr>',
//    );
// Account Settings Permissions
    $form['main']['account_settings'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => '<tr><td colspan="2">' . $HTMLform_school_setting_bluebox_start,
        '#suffix' => $HTMLform_school_setting_bluebox_end . '</td></tr><tr><td colspan="2">&nbsp;</td></tr>',
    );

    $form['main']['account_settings']['heading'] = array(
        '#type' => 'item',
        '#prefix' => '<tr><td colspan="7" style="color:#3570AA;font-size:15px;font-weight:bold;">Account Settings',
        '#suffix' => '</td></tr>',
    );
    $form['main']['account_settings']['user_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<tr><th style="text-align:left;width:250px"> Allow users to',
        '#suffix' => '</th>',
    );
    $form['main']['account_settings']['student_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Student',
        '#suffix' => '</th>',
    );
    $form['main']['account_settings']['parent_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Parent',
        '#suffix' => '</th>',
    );
    $form['main']['account_settings']['Teacher_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Staff',
        '#suffix' => '</th>',
    );
    $form['main']['account_settings']['otherstaff_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Other Staff',
        '#suffix' => '</th>',
    );
    $form['main']['account_settings']['school_leader_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> School Leader',
        '#suffix' => '</th>',
    );
    $form['main']['account_settings']['admin_headings'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Tech Managers',
        '#suffix' => '</th></tr>',
    );

    $account_setting_users = array('Change Password');
    $notification_setting_users = array('Receive Email Notifications');
    $event_setting_users = array('Create school wide events', 'View Network Events');
    $group_setting_users = array('Join Groups at my school', 'Create groups at my school', 'Join network-wide groups');
    $people_setting_users = array('Share/View profiles across the network', 'Display phone number field', 'Display private website field', 'Display Instant Messaging ID field');
    $count = count($account_setting_users);

    for ($i = 1; $i <= $count; $i++) {

        $form['main']['account_settings']['admin_heading'] = array(
            '#type' => 'item',
            '#prefix' => '<tr><th style="text-align:left;">' . $account_setting_users[$i - 1],
            '#suffix' => '</th>',
        );


//        get_selected_school_permissions($school_nid, NTLP_ROLEID_STUDENT, $account_setting_users[$i-1]);

        $form['main']['account_settings']['account_student_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_STUDENT, $account_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td>',
            '#name' => 'account[' . $i . '][' . NTLP_ROLEID_STUDENT . ']',
        );
        $form['main']['account_settings']['account_parent_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_PARENT, $account_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td>',
            '#name' => 'account[' . $i . '][' . NTLP_ROLEID_PARENT . ']',
        );
        $form['main']['account_settings']['account_teacher_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_TEACHER, $account_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td>',
            '#name' => 'account[' . $i . '][' . NTLP_ROLEID_TEACHER . ']',
        );
        $form['main']['account_settings']['account_other_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_NETWORKSTAFF, $account_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td>',
            '#name' => 'account[' . $i . '][' . NTLP_ROLEID_NETWORKSTAFF . ']',
        );
        $form['main']['account_settings']['account_leader_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_SCHOOLLEADER, $account_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td>',
            '#name' => 'account[' . $i . '][' . NTLP_ROLEID_SCHOOLLEADER . ']',
        );
        $form['main']['account_settings']['account_admin_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_SITEADMIN, $account_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td></tr>',
            '#name' => 'account[' . $i . '][' . NTLP_ROLEID_SITEADMIN . ']',
            '#disabled' => True,
        );
    }

// Notification Settings
    if ((isset($user->roles[NTLP_ROLEID_NETWORKMANAGER]) || isset($user->roles[NTLP_ROLEID_SYSTEM_ADMIN]))) {

        $form['main']['notification_settings'] = array(
            '#type' => 'fieldset',
            '#title' => t(''),
            '#tree' => TRUE,
            '#prefix' => '<tr><td colspan="2">' . $HTMLform_school_setting_bluebox_start,
            '#suffix' => $HTMLform_school_setting_bluebox_end . '</td></tr><tr><td colspan="2">&nbsp;</td></tr>',
        );

        $form['main']['notification_settings']['heading'] = array(
            '#type' => 'item',
            '#prefix' => '<tr><td colspan="7" style="color:#3570AA;font-size:15px;font-weight:bold">Notification Settings',
            '#suffix' => '</td></tr>',
        );
        $form['main']['notification_settings']['user_heading'] = array(
            '#type' => 'item',
            '#prefix' => '<tr><th style="text-align:left;width:250px"> Allow users to',
            '#suffix' => '</th>',
        );
        $form['main']['notification_settings']['student_heading'] = array(
            '#type' => 'item',
            '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Student',
            '#suffix' => '</th>',
        );
        $form['main']['notification_settings']['parent_heading'] = array(
            '#type' => 'item',
            '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Parent',
            '#suffix' => '</th>',
        );
        $form['main']['notification_settings']['Teacher_heading'] = array(
            '#type' => 'item',
            '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Staff',
            '#suffix' => '</th>',
        );
        $form['main']['notification_settings']['otherstaff_heading'] = array(
            '#type' => 'item',
            '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Other Staff',
            '#suffix' => '</th>',
        );
        $form['main']['notification_settings']['school_leader_heading'] = array(
            '#type' => 'item',
            '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> School Leader',
            '#suffix' => '</th>',
        );
        $form['main']['notification_settings']['admin_heading'] = array(
            '#type' => 'item',
            '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Tech Managers',
            '#suffix' => '</th></tr>',
        );

        $count = count($notification_setting_users);

        for ($i = 1; $i <= $count; $i++) {

            $form['main']['notification_settings']['admin_heading' . $i] = array(
                '#type' => 'item',
                '#prefix' => '<tr><th style="text-align:left">' . $notification_setting_users[$i - 1],
                '#suffix' => '</th>',
            );

            /* student default checkbox disabledv */
            $notification_student_disabled = array(1, 3);
            if (in_array($i, $notification_student_disabled)) {
                $disabled = True;
            } else {
                $disabled = false;
            }

            $form['main']['notification_settings']['account_student_' . $i] = array(
                '#type' => 'checkbox',
                '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_STUDENT, $notification_setting_users[$i - 1]) : '',
                '#prefix' => '<td align="center"> ',
                '#suffix' => '</td>',
                '#name' => 'notify[' . $i . '][' . NTLP_ROLEID_STUDENT . ']',
                '#disabled' => disalbed_school_permissions('notifybyemail', NTLP_ROLEID_STUDENT, $i),
            );
            $form['main']['notification_settings']['account_parent_' . $i] = array(
                '#type' => 'checkbox',
                '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_PARENT, $notification_setting_users[$i - 1]) : '',
                '#prefix' => '<td align="center"> ',
                '#suffix' => '</td>',
                '#name' => 'notify[' . $i . '][' . NTLP_ROLEID_PARENT . ']',
                '#disabled' => disalbed_school_permissions('notifybyemail', NTLP_ROLEID_STUDENT, $i),
            );
            $form['main']['notification_settings']['account_teacher_' . $i] = array(
                '#type' => 'checkbox',
                '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_TEACHER, $notification_setting_users[$i - 1]) : '',
                '#prefix' => '<td align="center"> ',
                '#suffix' => '</td>',
                '#name' => 'notify[' . $i . '][' . NTLP_ROLEID_TEACHER . ']',
                '#disabled' => disalbed_school_permissions('notifybyemail', NTLP_ROLEID_TEACHER, $i),
            );
            $form['main']['notification_settings']['account_other_' . $i] = array(
                '#type' => 'checkbox',
                '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_NETWORKSTAFF, $notification_setting_users[$i - 1]) : '',
                '#prefix' => '<td align="center"> ',
                '#suffix' => '</td>',
                '#name' => 'notify[' . $i . '][' . NTLP_ROLEID_NETWORKSTAFF . ']',
                '#disabled' => disalbed_school_permissions('notifybyemail', NTLP_ROLEID_NETWORKSTAFF, $i),
            );
            $form['main']['notification_settings']['account_leader_' . $i] = array(
                '#type' => 'checkbox',
                '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_SCHOOLLEADER, $notification_setting_users[$i - 1]) : '',
                '#prefix' => '<td align="center"> ',
                '#suffix' => '</td>',
                '#name' => 'notify[' . $i . '][' . NTLP_ROLEID_SCHOOLLEADER . ']',
                '#disabled' => disalbed_school_permissions('notifybyemail', NTLP_ROLEID_SCHOOLLEADER, $i),
            );
            $form['main']['notification_settings']['account_admin_' . $i] = array(
                '#type' => 'checkbox',
                '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_SITEADMIN, $notification_setting_users[$i - 1]) : '',
                '#prefix' => '<td align="center"> ',
                '#suffix' => '</td></tr>',
                '#name' => 'notify[' . $i . '][' . NTLP_ROLEID_SITEADMIN . ']',
                '#disabled' => disalbed_school_permissions('notifybyemail', NTLP_ROLEID_SITEADMIN, $i),
            );
        }
    }
    
// Event Settings Permission
    $form['main']['event_settings'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => '<tr><td colspan="2">' . $HTMLform_school_setting_bluebox_start,
        '#suffix' => $HTMLform_school_setting_bluebox_end . '</td></tr><tr><td colspan="2">&nbsp;</td></tr>',
    );

    $form['main']['event_settings']['heading'] = array(
        '#type' => 'item',
        '#prefix' => '<tr><td colspan="7" style="color:#3570AA;font-size:15px;font-weight:bold">Event Settings',
        '#suffix' => '</td></tr>',
    );
    $form['main']['event_settings']['user_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<tr><th style="text-align:left;width:250px"> Allow users to',
        '#suffix' => '</th>',
    );
    $form['main']['event_settings']['student_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Student',
        '#suffix' => '</th>',
    );
    $form['main']['event_settings']['parent_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Parent',
        '#suffix' => '</th>',
    );
    $form['main']['event_settings']['Teacher_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Staff',
        '#suffix' => '</th>',
    );
    $form['main']['event_settings']['otherstaff_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Other Staff',
        '#suffix' => '</th>',
    );
    $form['main']['event_settings']['school_leader_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> School Leader',
        '#suffix' => '</th>',
    );
    $form['main']['event_settings']['admin_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Tech Managers',
        '#suffix' => '</th></tr>',
    );


    $count = count($event_setting_users);

    for ($i = 1; $i <= $count; $i++) {

        $form['main']['event_settings']['admin_heading' . $i] = array(
            '#type' => 'item',
            '#prefix' => '<tr><th style="text-align:left">' . $event_setting_users[$i - 1],
            '#suffix' => '</th>',
        );

        /* student default checkbox disabledv */
        $event_student_disabled = array(1, 3);
        if (in_array($i, $event_student_disabled)) {
            $disabled = True;
        } else {

            $disabled = false;
        }



        $form['main']['event_settings']['account_student_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_STUDENT, $event_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td>',
            '#name' => 'event[' . $i . '][' . NTLP_ROLEID_STUDENT . ']',
            '#disabled' => disalbed_school_permissions('event', NTLP_ROLEID_STUDENT, $i),
        );
        $form['main']['event_settings']['account_parent_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_PARENT, $event_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td>',
            '#name' => 'event[' . $i . '][' . NTLP_ROLEID_PARENT . ']',
            '#disabled' => disalbed_school_permissions('event', NTLP_ROLEID_STUDENT, $i),
        );
        $form['main']['event_settings']['account_teacher_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_TEACHER, $event_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td>',
            '#name' => 'event[' . $i . '][' . NTLP_ROLEID_TEACHER . ']',
            '#disabled' => disalbed_school_permissions('event', NTLP_ROLEID_TEACHER, $i),
        );
        $form['main']['event_settings']['account_other_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_NETWORKSTAFF, $event_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td>',
            '#name' => 'event[' . $i . '][' . NTLP_ROLEID_NETWORKSTAFF . ']',
            '#disabled' => disalbed_school_permissions('event', NTLP_ROLEID_NETWORKSTAFF, $i),
        );
        $form['main']['event_settings']['account_leader_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_SCHOOLLEADER, $event_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td>',
            '#name' => 'event[' . $i . '][' . NTLP_ROLEID_SCHOOLLEADER . ']',
            '#disabled' => disalbed_school_permissions('event', NTLP_ROLEID_SCHOOLLEADER, $i),
        );
        $form['main']['event_settings']['account_admin_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_SITEADMIN, $event_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td></tr>',
            '#name' => 'event[' . $i . '][' . NTLP_ROLEID_SITEADMIN . ']',
            '#disabled' => disalbed_school_permissions('event', NTLP_ROLEID_SITEADMIN, $i),
        );
    }
// Groups Settings Permission
    $form['main']['group_settings'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => '<tr><td colspan="2">' . $HTMLform_school_setting_bluebox_start,
        '#suffix' => $HTMLform_school_setting_bluebox_end . '</td></tr><tr><td colspan="2">&nbsp;</td></tr>',
    );

    $form['main']['group_settings']['heading'] = array(
        '#type' => 'item',
        '#prefix' => '<tr><td colspan="7" style="color:#3570AA;font-size:15px;font-weight:bold;">Groups Settings',
        '#suffix' => '</td></tr>',
    );
    $form['main']['group_settings']['user_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<tr><th style="text-align:left;width:250px"> Allow users to',
        '#suffix' => '</th>',
    );
    $form['main']['group_settings']['student_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Student',
        '#suffix' => '</th>',
    );
    $form['main']['group_settings']['parent_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Parent',
        '#suffix' => '</th>',
    );
    $form['main']['group_settings']['Teacher_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Staff',
        '#suffix' => '</th>',
    );
    $form['main']['group_settings']['otherstaff_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Other Staff',
        '#suffix' => '</th>',
    );
    $form['main']['group_settings']['school_leader_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> School Leader',
        '#suffix' => '</th>',
    );
    $form['main']['group_settings']['admin_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Tech Managers',
        '#suffix' => '</th></tr>',
    );


    $count = count($group_setting_users);

    for ($i = 1; $i <= $count; $i++) {

        $form['main']['group_settings']['admin_heading' . $i] = array(
            '#type' => 'item',
            '#prefix' => '<tr><th style="text-align:left">' . $group_setting_users[$i - 1],
            '#suffix' => '</th>',
        );
        disalbed_school_permissions('group', NTLP_ROLEID_STUDENT, $i);

        $form['main']['group_settings']['group_student_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_STUDENT, $group_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td>',
            '#name' => 'group[' . $i . '][' . NTLP_ROLEID_STUDENT . ']',
            '#disabled' => disalbed_school_permissions('group', NTLP_ROLEID_STUDENT, $i),
        );
        $form['main']['group_settings']['group_parent_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_PARENT, $group_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td>',
            '#name' => 'group[' . $i . '][' . NTLP_ROLEID_PARENT . ']',
            '#disabled' => disalbed_school_permissions('group', NTLP_ROLEID_PARENT, $i),
        );
        $form['main']['group_settings']['group_teacher_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_TEACHER, $group_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td>',
            '#name' => 'group[' . $i . '][' . NTLP_ROLEID_TEACHER . ']',
            '#disabled' => disalbed_school_permissions('group', NTLP_ROLEID_TEACHER, $i),
        );
        $form['main']['group_settings']['group_other_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_NETWORKSTAFF, $group_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td>',
            '#name' => 'group[' . $i . '][' . NTLP_ROLEID_NETWORKSTAFF . ']',
            '#disabled' => disalbed_school_permissions('group', NTLP_ROLEID_NETWORKSTAFF, $i),
        );
        $form['main']['group_settings']['group_leader_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_SCHOOLLEADER, $group_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td>',
            '#name' => 'group[' . $i . '][' . NTLP_ROLEID_SCHOOLLEADER . ']',
            '#disabled' => disalbed_school_permissions('group', NTLP_ROLEID_SCHOOLLEADER, $i),
        );
        $form['main']['group_settings']['group_admin_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_SITEADMIN, $group_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td></tr>',
            '#name' => 'group[' . $i . '][' . NTLP_ROLEID_SITEADMIN . ']',
            '#disabled' => disalbed_school_permissions('group', NTLP_ROLEID_SITEADMIN, $i),
        );
    }
// People Profile Settings Permission
    $form['main']['people_settings'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => '<tr><td colspan="2">' . $HTMLform_school_setting_bluebox_start,
        '#suffix' => $HTMLform_school_setting_bluebox_end . '</td></tr><tr><td colspan="2">&nbsp;</td></tr>',
    );

    $form['main']['people_settings']['heading'] = array(
        '#type' => 'item',
        '#prefix' => '<tr><td colspan="7" style="color:#3570AA;font-size:15px;font-weight:bold;">People/Profile Settings',
        '#suffix' => '</td></tr>',
    );
    $form['main']['people_settings']['user_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<tr><th style="text-align:left;width:250px"> Allow users to',
        '#suffix' => '</th>',
    );
    $form['main']['people_settings']['student_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Student',
        '#suffix' => '</th>',
    );
    $form['main']['people_settings']['parent_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Parent',
        '#suffix' => '</th>',
    );
    $form['main']['people_settings']['Teacher_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Staff',
        '#suffix' => '</th>',
    );
    $form['main']['people_settings']['otherstaff_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Other Staff',
        '#suffix' => '</th>',
    );
    $form['main']['people_settings']['school_leader_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> School Leader',
        '#suffix' => '</th>',
    );
    $form['main']['people_settings']['admin_heading'] = array(
        '#type' => 'item',
        '#prefix' => '<th style="border-top: 1px solid #9ECCE9"> Tech Managers',
        '#suffix' => '</th></tr>',
    );


    $count = count($people_setting_users);

    for ($i = 1; $i <= $count; $i++) {

        $form['main']['people_settings']['admin_heading' . $i] = array(
            '#type' => 'item',
            '#prefix' => '<tr><th style="text-align:left">' . $people_setting_users[$i - 1],
            '#suffix' => '</th>',
        );


//        echo $school_nid.' '.$people_setting_users[$i-1];

        $form['main']['people_settings']['account_student_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_STUDENT, $people_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td>',
            '#name' => 'people[' . $i . '][' . NTLP_ROLEID_STUDENT . ']',
            '#disabled' => disalbed_school_permissions('people', NTLP_ROLEID_STUDENT, $i),
        );
        $form['main']['people_settings']['people_parent_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_PARENT, $people_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td>',
            '#name' => 'people[' . $i . '][' . NTLP_ROLEID_PARENT . ']',
            '#disabled' => disalbed_school_permissions('people', NTLP_ROLEID_PARENT, $i),
        );
        $form['main']['people_settings']['people_teacher_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_TEACHER, $people_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td>',
            '#name' => 'people[' . $i . '][' . NTLP_ROLEID_TEACHER . ']',
            '#disabled' => disalbed_school_permissions('people', NTLP_ROLEID_TEACHER, $i),
        );
        $form['main']['people_settings']['account_other_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_NETWORKSTAFF, $people_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td>',
            '#name' => 'people[' . $i . '][' . NTLP_ROLEID_NETWORKSTAFF . ']',
            '#disabled' => disalbed_school_permissions('people', NTLP_ROLEID_NETWORKSTAFF, $i),
        );
        $form['main']['people_settings']['account_leader_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_SCHOOLLEADER, $people_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td>',
            '#name' => 'people[' . $i . '][' . NTLP_ROLEID_SCHOOLLEADER . ']',
            '#disabled' => disalbed_school_permissions('people', NTLP_ROLEID_SCHOOLLEADER, $i),
        );
        $form['main']['people_settings']['account_admin_' . $i] = array(
            '#type' => 'checkbox',
            '#default_value' => (isset($school_nid)) ? get_selected_school_permissions($school_nid, NTLP_ROLEID_SITEADMIN, $people_setting_users[$i - 1]) : '',
            '#prefix' => '<td align="center"> ',
            '#suffix' => '</td></tr>',
            '#name' => 'people[' . $i . '][' . NTLP_ROLEID_SITEADMIN . ']',
            '#disabled' => disalbed_school_permissions('people', NTLP_ROLEID_SITEADMIN, $i),
        );
    }
    return $form;
}

function school_general_settings() {

    return drupal_get_form('school_general_settings_form');
}

/* Form General Settings In School Management END */

function school_general_settings_form_validate($form, $form_state) {

    $school_active = $form_state['clicked_button'] ['#post']['main']['school_year']['school_active'];
    $school_year = $form_state['clicked_button'] ['#post']['main']['school_year']['current_school_year'];
    $school_id = $form_state['clicked_button'] ['#post']['main']['school_id'];


    if ($school_active == 'H') {
        if(check_active_courses_in_school($school_id)){
            
             form_set_error("", t("This school has at least one active course.<br />
                 The school's status cannot be changed while there are active courses in the active School Year."));
        }
    }
}

function check_active_courses_in_school($school_nid) {
    $records = false;
    $result = db_query("SELECT c.nid,c.school_nid, c.school_year, c.is_project
        FROM {ntlp_course} c
        INNER JOIN {ntlp_school} s ON s.nid = c.school_nid AND c.school_year = s.current_school_year
        WHERE c.school_nid=%d AND c.availability = 'S' AND c.is_project <> 1;", $school_nid);

    if ($result->num_rows > 0) {
        $records = true;
    }
    
    return $records;
}

/* Form General Settings In School Management Submit Start */

function school_general_settings_save($form, $form_state) {

    global $user;

    $school_name = $form_state['clicked_button'] ['#post']['main']['school_info']['school_name'];
    $school_year = $form_state['clicked_button'] ['#post']['main']['school_year']['current_school_year'];
    $school_active = $form_state['clicked_button'] ['#post']['main']['school_year'] ['school_active'];

    $school_state = $form_state['clicked_button'] ['#post']['main']['school_info']['school_state'];
    $export_id = $form_state['clicked_button'] ['#post']['main']['school_info']['export_id'];
    $school_state = $form_state['clicked_button'] ['#post']['main']['school_info']['school_state'];
    $school_id = $form_state['clicked_button'] ['#post']['main']['school_id'];
    $google_mail = $form_state['clicked_button'] ['#post']['main']['google_application_settings']['google_mail'];
    $google_docs = $form_state['clicked_button'] ['#post']['main']['google_application_settings']['google_docs'];
    $goole_sites = $form_state['clicked_button'] ['#post']['main']['google_application_settings']['google_sites'];
    $account_setting_users = array('Change Password');

    $notification_setting_users = array('Receive Email Notifications');
    $event_setting_users = array('Create school wide events', 'View Network Events', 'Create Network Events');
    $group_setting_users = array('Join Groups at my school', 'Create groups at my school', 'join network-wide groups', 'Create network-wide groups');
    $people_setting_users = array('Share/View profiles across the network', 'Display phone number field', 'Display private website field', 'Display Instant Messaging ID field');

    $vars = array(
        'group' => 'group_setting_users',
        'people' => 'people_setting_users',
        'event' => 'event_setting_users',
        'account' => 'account_setting_users',
        'notify' => 'notification_setting_users',
    );

    $clickMap = array();

    /*  Isertion or deletion of permissions */

// role id arrays
    $roleid_array = array(NTLP_ROLEID_STUDENT, NTLP_ROLEID_PARENT, NTLP_ROLEID_TEACHER, NTLP_ROLEID_SITEADMIN, NTLP_ROLEID_NETWORKSTAFF, NTLP_ROLEID_SCHOOLLEADER);

    foreach (array_keys($vars) as $key) {
        if (array_key_exists($key, $form_state['clicked_button']['#post'])) {
            foreach (${$vars[$key]} as $row => $title) {

                foreach ($roleid_array as $role) {
                    if (isset($form_state['clicked_button']['#post'][$key][$row + 1][$role])) {
                        $clickMap[$key][$title][$role] = 1;
                    } else {
                        $clickMap[$key][$title][$role] = 0;
                    }
                }
            }
        } else {
            foreach (${$vars[$key]} as $title) {
                $clickMap[$key][$title] = array_fill_keys($roleid_array, 0);
            }
        }
    }

    foreach ($clickMap as $values) {
        foreach ($values as $title => $perms) {
            foreach ($perms as $role => $isChkd) {
                $perm_title = strtolower($title);

                $permissions = get_school_permissions($school_id, $perm_title, $role, $isChkd);

                db_query("UPDATE {permission} SET perm = '%s' where rid = %d", $permissions, $role);
            }
        }
    }

    if (isset($school_id)) {
        $school_node = node_load($school_id);
    } else {
        $school_node = new stdClass();
    }

    $school_node->title = $school_name;
    $school_node->type = 'ntlp_school';

    $school_node->status = 1;
    if ($school_active == 'H') {
        $school_node->status = 0;
    }
    $school_node->promote = 0;
    $school_node->sticky = 0;
    $school_node->format = 1;
    $school_node->uid = $user->uid;
    // $school_node->changed = convert_datetime_tz(   );

    node_save($school_node);

    $node_school_id = $school_node->nid;



    if (isset($school_id) || $school_id != null) {

        db_query("UPDATE  {ntlp_school} SET location_tid = %d, school_export_id = %d, current_school_year = %d, gdocs_status = %d, gsite_status = %d, gmail_status = %d where nid = %d", $school_state, $export_id, $school_year, $google_docs, $goole_sites, $google_mail, $node_school_id);
    }
}

function save_school_picture($fid, $school_id) {
    global $base_path;

    watchdog('uploadimage', 'fid ' . $fid);
    $file_Obj = _get_file_detail($fid);
    db_query("UPDATE {ntlp_school} SET fid = %d WHERE nid = %d", $file_Obj->fid, $school_id);

    watchdog('uploadimage', 'file updated successfully');
    echo drupal_get_form('ntlp_activeimage', $base_path . $file_Obj->filepath, 197, 200);
}

function get_school_permissions($school_id, $title, $role, $isChkd) {

    $rs = db_query("SELECT rid,	perm FROM {permission} where rid = %d", $role);

    $record = db_fetch_object($rs);
    $perm = $record->perm;
    $perm_array = explode(',', $perm);
//    $perm_array = array_map('strtolower', $perm_array);

    $perm_title = ' ' . $school_id . ' ' . $title;

//    echo "'$perm_title' $isChkd".in_array($perm_title, $perm_array).'<br />';

    if (!in_array($perm_title, $perm_array) && $isChkd == 1) {
        $perm_array[] = $perm_title;
    } else if (in_array($perm_title, $perm_array) && $isChkd == 0) {
        unset($perm_array[array_search($perm_title, $perm_array)]);
    }

    $permission_array = implode(',', $perm_array);

    return $permission_array;
}

/* Form General Settings In School Management Submit  END */

/* Adding and updating Taxonomy for course settings page */

function add_taxonomy_for_school($term_array, $vocab_str, $tids, $school_nid) {
    $i = 0;
    $post_vocab_Object = _get_lib_vocabulary_vid($vocab_str);
    $school_year = date('Y');

    if ($term_array != null) {
        foreach ($term_array as $term_Obj) {
            $term = taxonomy_get_term($tids[$i]);

            $sql = db_query("SELECT s.school_term_tid, t.tid FROM {ntlp_school_term} s
                    INNER JOIN {term_data} t ON t.tid = s.school_term_tid
                    Where s.school_nid = %d  AND s.school_term_tid = %d ", $school_nid, $tids[$i]);

            $result = db_fetch_object($sql);
            if ($sql->num_rows > 0) {
                $name = is_array($term_Obj) ? $term_Obj['name'] : $term_Obj;
                db_query("UPDATE {term_data} SET name ='%s', description ='%s'
            WHERE tid = %d", $name, $term_Obj['description'], $tids[$i]);

                $old_terms = new stdClass();
                $old_terms->tid = $result->tid;
                $old_terms->vid = $post_vocab_Object->vid;
                $old_terms->name = $name;
                $old_terms->description = $term_Obj['description'];
                $old_terms->weight = 0;

                $terms_array[] = $old_terms;


//            $result = db_fetch_object($sql);
//            if($term_Obj !=null) {
//
//                $new_term = array('vid' => $post_vocab_Object->vid, 'name' => is_array($term_Obj) ? $term_Obj['name'] : $term_Obj);
//
//                $new_term['description'] = (is_array($term_Obj)) ? $term_Obj['description'] : '';
//
//                if (taxonomy_save_term($new_term)) {
//
//                    $new_terms = new stdClass();
//                    $new_terms->tid = $new_term['tid'];
//                    $new_terms->vid = $post_vocab_Object->vid;
//                    $new_terms->weight = 0;
//                    $new_terms->name = $new_term['name'];
//                    $new_terms->description = $new_term['description'];
//
//                    $terms_array[] = $new_terms;
//                }
//
//            }
            } else {

                if ($term_Obj != null) {

                    $new_term = array('vid' => $post_vocab_Object->vid, 'name' => is_array($term_Obj) ? $term_Obj['name'] : $term_Obj);

                    $new_term['description'] = (is_array($term_Obj)) ? $term_Obj['description'] : '';

                    if (taxonomy_save_term($new_term)) {

                        $new_terms = new stdClass();
                        $new_terms->tid = $new_term['tid'];
                        $new_terms->vid = $post_vocab_Object->vid;
                        $new_terms->weight = 0;
                        $new_terms->name = $new_term['name'];
                        $new_terms->description = $new_term['description'];

                        $terms_array[] = $new_terms;
                    }
                }
            }


            $i++;
        }
    }
    return $terms_array;
}

function ntlp_school_add_outcome_frm($school_nid, $school_year) {
    return drupal_get_form('ntlp_school_add_outcome_form', $school_nid, $school_year);
}

function ntlp_school_add_outcome_form($form_state = null, $school_nid, $school_year) {

    include('school_management.template.inc');
    modalframe_child_js();
    ahah_helper_register($form, $form_state);

    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/ntlp_school.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/school_ahah_helper.js', 'footer');

    drupal_set_title('Add Outcome');

    if (isset($form_state['storage']['add_outcome']['school_oc_tid'])) {
        $selected_school_oc = $form_state['storage']['add_outcome']['school_oc_tid'];
    }

    if ($selected_school_oc > 0 && $selected_school_oc != 'new') {
        $outcome_obj = get_selected_outcome_record($selected_school_oc);
    }

    if ($selected_school_oc == 'new') {
        $height = '250px';
    } else {
        $height = '200px';
    }

    $form['add_outcome'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => '<div id="add-outcome-wrapper"><div id="dialog-confirm-action" style="background-color:#fff;display:none;" title="WARNING:">
    <p><span class="msg">Are you sure you want to select this action?</span></p></div><div>' . $HTMLform_school_setting_bluebox_start,
        '#suffix' => $HTMLform_school_setting_bluebox_end . '</div></div>',
    );

    $form['add_outcome']['heading1'] = array(
        '#type' => 'item',
        '#value' => t('Outcome Title'),
        '#prefix' => '<tr><td style="color:#3570AA;font-size:12px; border-right:1px solid #9ECCE9;">',
        '#suffix' => '',
    );


    $school_year_options = get_all_unused_school_outcome($school_nid, $school_year);

    $form['add_outcome']['school_oc_tid'] = array(
        '#type' => 'select',
        '#options' => $school_year_options,
        '#default_value' => $selected_school_oc,
        '#prefix' => '<div style="float:left; width:190px;">
            <span style="width:190px;" class="comboboxBgLeft">',
        '#suffix' => '</span></div></td>',
        '#ahah' => array(
            'path' => ahah_helper_path(array('add_outcome')),
            'wrapper' => 'add-outcome-wrapper',
            'progress' => 'none',
        ),
        '#attributes' => array('class' => 'my-dropdown'),
        '#id' => 'school_oc_tid',
    );
    if ($selected_school_oc == 'new') {
        $rowspan_change = 3;
        $total_rows = 8;
    } else {
        $rowspan_change = 3;
        $total_rows = 3;
    }
    $form['add_outcome']['description'] = array(
        '#type' => 'textarea',
        '#default_value' => (isset($outcome_obj) ? $outcome_obj->description : ''),
        '#prefix' => '<td rowspan="' . $rowspan_change . '" style="width:380px; color:#3570AA;font-size:12px;">' . t('Description'),
        '#suffix' => '</td></tr>',
        '#wysiwyg' => false,
        '#id' => 'outcome_desc',
        '#cols' => 15,
        '#rows' => $total_rows,
    );

    if ($selected_school_oc == 'new') {
        $form['add_outcome']['outcome_title'] = array(
            '#type' => 'textfield',
            '#size' => 35,
            '#maxlength' => 25,
            '#prefix' => '<tr><td style="border-right:1px solid #9ECCE9; color:#3570AA;font-size:12px;">' . t('Outcome Name'),
            '#suffix' => '</td></tr>',
            '#id' => 'new_outcome_title',
        );
    }
    $form['add_outcome']['short_title'] = array(
        '#type' => 'textfield',
        '#default_value' => (isset($outcome_obj) ? $outcome_obj->short_name : ''),
        '#size' => 35,
        '#maxlength' => 25,
        '#prefix' => '<tr><td border-right:1px solid #9ECCE9; style="color:#3570AA;font-size:12px;">' . t('Short Name'),
        '#suffix' => '</td></tr>',
        '#id' => 'outcome_short_title',
    );



    $option = '<div style="float:left;padding-right:2px;"><span><a class="SaveBtn" onclick="save_outcome(' . $school_nid . ',' . $school_year . ');">Save</a></span></div>';
    $form['op'] = array(
        '#type' => 'item',
        '#value' => '<tr><td colspan="2"><div style="width:400px;" class="clearfix">' . $option . '<div style="float:left;padding-left:2px;"><span><a class="CancelBtn" style="text-align:center;" onclick="parent.Drupal.modalFrame.close();"><span style="position:relative;top:3px;">Cancel</span></a></span></div></div></td></tr>',
    );

    return $form;
}

function ntlp_school_add_outcome_data($school_nid, $school_year, $school_oc_tid, $short_title, $desc, $new_title) {

    watchdog('add_outcome', $school_nid . ' ' . $school_year . ' ' . $school_oc_tid . ' ' . $short_title . ' ' . $desc . ' ' . $new_title);

    $new_order = (get_max_outcome_order($school_nid, $school_year) + 1);

    if ($school_oc_tid == 'new') {
        $new_term = array('vid' => VOCAB_NTLP_GB_OUTCOME, 'name' => $new_title, 'description' => $desc);
        if (taxonomy_save_term($new_term)) {
            db_query("INSERT INTO {ntlp_gb_outcome} (tid, school_nid, sort_order, short_name, school_year) 
                VALUES (%d, %d, %d, '%s', %d)", $new_term['tid'], $school_nid, $new_order, $short_title, $school_year);
        }
    } else {

        db_query("UPDATE {term_data} SET description = '%s' WHERE tid = %d", $desc, $school_oc_tid);

        db_query("INSERT INTO {ntlp_gb_outcome} (tid, school_nid, sort_order, short_name, school_year) 
                VALUES (%d, %d, %d, '%s', %d)", $school_oc_tid, $school_nid, $new_order, $short_title, $school_year);
    }
}

function ntlp_school_edit_outcome_frm($school_nid, $school_year, $outcome_tid) {
    return drupal_get_form('ntlp_school_edit_outcome_form', $school_nid, $school_year, $outcome_tid);
}

function ntlp_school_edit_outcome_form($form_state = null, $school_nid, $school_year, $outcome_tid) {

    include('school_management.template.inc');
    modalframe_child_js();

    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/ntlp_school.js');

    drupal_set_title('Edit Outcome');

    $outcome_obj = get_selected_outcome_record($outcome_tid);


    $form['add_outcome'] = array(
        '#type' => 'fieldset',
        '#title' => t(''),
        '#tree' => TRUE,
        '#prefix' => '<div id="add-outcome-wrapper"><div id="dialog-confirm-action" style="background-color:#fff;display:none;" title="WARNING:">
    <p><span class="msg">Are you sure you want to select this action?</span></p></div><div style="width: 700px;height:' . $height . ';" >' . $HTMLform_school_setting_bluebox_start . '<tr><td colspan="2"><div style="border:1px solid #F17B23;padding:3px">' . t('You are editing a Schoolwide Learning Outcome.  Any changes made here will be reflected in every course and school year where this outcome is used.') . '</div></td></tr>',
        '#suffix' => $HTMLform_school_setting_bluebox_end . '</div></div>',
    );

    $form['add_outcome']['outcome_title'] = array(
        '#type' => 'textfield',
        '#default_value' => (isset($outcome_obj) ? $outcome_obj->name : ''),
        '#size' => 35,
        '#maxlength' => 25,
        '#prefix' => '<tr><td style="border-right:1px solid #9ECCE9;  color:#3570AA;font-size:12px;">' . t('Outcome Name'),
        '#suffix' => '</td>',
        '#id' => 'new_outcome_title',
    );

    $form['add_outcome']['description'] = array(
        '#type' => 'textarea',
        '#default_value' => (isset($outcome_obj) ? $outcome_obj->description : ''),
        '#prefix' => '<td rowspan="2" style="color:#3570AA;font-size:12px;">' . t('Description'),
        '#suffix' => '</td></tr>',
        '#wysiwyg' => false,
        '#id' => 'outcome_desc',
    );


    $form['add_outcome']['short_title'] = array(
        '#type' => 'textfield',
        '#default_value' => (isset($outcome_obj) ? $outcome_obj->short_name : ''),
        '#size' => 35,
        '#maxlength' => 25,
        '#prefix' => '<tr><td style="border-right:1px solid #9ECCE9;  color:#3570AA;font-size:12px;">' . t('Short Name'),
        '#suffix' => '</td></tr>',
        '#id' => 'outcome_short_title',
    );


    $option = '<div style="float:left;padding-right:2px;"><span><a class="SaveBtn" onclick="ntlp_edit_outcome(' . $school_nid . ',' . $school_year . ',' . $outcome_tid . ');">Save</a></span></div>';
    $form['op'] = array(
        '#type' => 'item',
        '#value' => '<tr><td colspan="2"><div style="width:400px;" class="clearfix">' . $option . '<div style="float:left;padding-left:2px;"><span><a class="CancelBtn" style="text-align:center;" onclick="parent.Drupal.modalFrame.close();"><span style="position:relative;top:3px;">Cancel</span></a></span></div></div></td></tr>',
    );

    return $form;
}

function ntlp_school_edit_outcome_data($school_nid, $school_year, $oc_tid, $short_title, $desc, $new_title) {

    $new_term = array('tid' => $oc_tid, 'vid' => VOCAB_NTLP_GB_OUTCOME, 'name' => $new_title, 'description' => $desc);
    taxonomy_save_term($new_term);

    db_query("UPDATE {ntlp_gb_outcome} SET short_name = '%s' WHERE tid = %d AND school_nid = %d
            AND school_year = %d", $short_title, $oc_tid, $school_nid, $school_year);
}

function ntlp_school_outcome_delete($outcome_tid, $school_nid, $school_year) {

    $message = "";
    $success = 0;

    if (!is_outcome_used_in_courses_for_year($outcome_tid, $school_nid, $school_year)) {
        db_query("DELETE FROM {ntlp_gb_outcome} WHERE tid= %d AND school_nid = %d AND school_year = %d", $outcome_tid, $school_nid, $school_year);
        $success = 1;
    } else {
        $message = "You cannot delete this outcome because it is being used in one or more courses for the $school_year school year.";
    }

    drupal_json(array('success' => $success,
        'message' => $message,
    ));
}

function ntlp_school_term_delete($term_tid, $school_nid, $school_year) {
    $message = "";
    $success = 0;

    if (!is_term_used_in_courses_for_year($term_tid, $school_nid, $school_year)) {
        db_query("DELETE FROM {ntlp_school_term} WHERE school_nid = %d 
            AND school_year = %d AND school_term_tid = %d", $school_nid, $school_year, $term_tid);
        $success = 1;
    } else {
        $message = "This term is being used in one or more courses.  You must disassociate the term from any courses prior to deleting it.";
    }

    drupal_json(array('success' => $success,
        'message' => $message,
    ));
}

function ntlp_school_change_outcome_order($action, $school_nid, $school_year, $outcome_tid, $oc_order) {

    watchdog('slider ', $action . ' ' . $school_nid . ' ' . $outcome_tid . ' ' . $oc_order);

    if ($action == 'down') {
        $change_order = $oc_order + 1;
    } else {
        $change_order = $oc_order - 1;
    }

    $result = db_query('SELECT tid FROM {ntlp_gb_outcome} 
        WHERE school_nid = %d AND school_year = %d AND sort_order = %d', $school_nid, $school_year, $change_order);

//    watchdog('slider1 ', print_r($result, true));
    if ($result->num_rows > 0) {
        $data = db_fetch_object($result);
        $previous_tid = $data->tid;
//        watchdog('slider2 ', print_r($data, true) . ' change order ' . $change_order);
        // update current section order
        db_query("UPDATE {ntlp_gb_outcome}  SET sort_order =%d 
            WHERE school_nid = %d AND school_year = %d AND tid = %d", $change_order, $school_nid, $school_year, $outcome_tid);

        // update next section order
        db_query("UPDATE {ntlp_gb_outcome}  SET sort_order =%d 
            WHERE school_nid = %d AND school_year = %d AND tid = %d", $oc_order, $school_nid, $school_year, $previous_tid);
    }

    echo get_school_outcomes_data_block($school_nid, $school_year);
}

function ntlp_school_term_changes($school_nid, $school_year, $save_data) {
    $dateformat = "Y-m-d H:i:s";

//    watchdog('term_changes', ' school year ' . $school_nid . ' schol_year  ' . $school_year . ' post array' . print_r($_POST, true));
    $term_tids = $_POST['tids'];
    $term_startdates = $_POST['startdata'];
    $term_endates = $_POST['enddate'];
    $term_names = $_POST['termname'];
    $term_abbs = $_POST['termabb'];

    if ($save_data != 1) {
        if ($term_tids != null) {
            foreach ($term_tids as $key => $term_tid) {
                if ($term_tid > 0) {
                    $startdate = set_tz($school_nid, $dateformat, $term_startdates[$key] . " 00:00:00");
                    $enddate = set_tz($school_nid, $dateformat, $term_endates[$key] . " 23:59:59");

                    if (!check_school_term_changes($term_tid, $school_nid, $school_year, $startdate, $enddate)) {

                        if (is_term_used_in_courses_for_year($term_tid, $school_nid, $school_year)) {
                            print 'ERROR';
                            return;
                        }
                    }
                }
            }
        }
    }
    school_course_settings_data($_POST);
    print 'SUCCESS';
}

/* Form To  Course Settings In School Management  SUBMIT START */

function school_course_settings_data($post_array) {
    global $user;

    $grade_label = $post_array['grade_label'];
    $grade_limit = $post_array['grade_limit'];

    $tids = $post_array['tids'];
    $school_year = $post_array['main']['school_year'];
    $school_id = $post_array['main']['school_id'];

    $start_date = $post_array['startdata'];
    $end_date = $post_array['enddate'];
    $term_abbr = $post_array['termabb'];
    $export_id = $post_array['exportid'];

    $grade_rounding = $post_array['grade']['grade_rounding'];
    $default_grade_scale = $post_array['grade']['grade_scale'];

    $term_array = $post_array['termname'];

    $dateformat = "Y-m-d H:i:s";

    $vocab_str = 'NTLP School Term';
    $outcome_vocab_str = 'NTLP GB Outcome';

    /* Adding Term Name Taxonomy */
    $term_data = add_taxonomy_for_school($term_array, $vocab_str, $tids, $school_id);

    /* Saving Term INFO into School Term Table */
    if ($school_id != null) {
        $school_rs = get_school_record($school_id);

        if (is_array($term_data)) {
            foreach ($term_data as $key => $term_obj) {

                $query = db_query("select school_term_tid FROM {ntlp_school_term} WHERE school_term_tid = %d ", $term_obj->tid);
                $rs = db_fetch_object($query);

                $startdate = set_tz($school_id, $dateformat, $start_date[$key] . " 00:00:00");
                $enddate = set_tz($school_id, $dateformat, $end_date[$key] . " 23:59:59");

//                drupal_set_message("Start date: " . $startdate . ", " . date('m-d-Y', $start_date[$key]));
//                drupal_set_message("End date: " . $enddate . ", " . date('Y-m-d', $start_date[$key]));
                if ($query->num_rows > 0) {
                    db_query("UPDATE {ntlp_school_term} SET date_started ='%s', date_closed ='%s',  export_id = %d , term_abbrv = '%s'
                            WHERE school_term_tid = %d", $startdate, $enddate, $export_id[$key], $term_abbr[$key], $rs->school_term_tid);
                } else {

                    db_query("INSERT INTO {ntlp_school_term} (school_nid, modified_by, school_term_tid, date_started, date_closed,
                    gb_closed_date, school_year, export_id, term_abbrv)
                    VALUES(%d, %d, %d, '%s', '%s',
                    '0000-00-00 00:00:00', %d, %d, '%s')", $school_id, $user->uid, $term_obj->tid, $startdate, $enddate, $school_year, $export_id[$key], $term_abbr[$key]);
                }
            }
            db_query("DELETE FROM {ntlp_school_term} WHERE school_term_tid = 0 AND school_nid = %d AND school_year = %d", $school_id, $school_year);
        }
    }


    if ($grade_label != null || $grade_limit != null) {

        db_query("DELETE FROM {ntlp_school_grade} where school_nid = %d AND school_year = %d", $school_id, $school_year);
        foreach ($grade_label as $key => $label) {
            if ($label != null) {
                if ($grade_rounding) {
                    $gradings = $grade_limit[$key];
                } else {

                    $gradings = floor($grade_limit[$key]);
                }

                db_query("INSERT INTO {ntlp_school_grade}(school_nid, label, lower_limit, school_year)
                VALUES (%d, '%s', %f, %d)", $school_id, $label, $gradings, $school_year);
            }
        }
    }



    /* Updating the grading rounding and default grade scale field  */

    if ($grade_rounding != null || $default_grade_scale != null) {

        db_query("UPDATE {ntlp_school} SET grade_rounding = %d, grade_scale =%d WHERE nid = %d", $grade_rounding, $default_grade_scale, $school_id);
    }
    return;
}

/* Form To  Course Settings In School Management  SUBMIT END */

function ntlp_school_check_outcome_exist($school_nid, $outcome_name, $outcome_tid) {
    $success = 0;
    $message = '';
    $case = false;

    $result = db_query("SELECT * FROM {ntlp_gb_outcome} oc
        INNER JOIN {term_data} td ON td.tid = oc.tid 
        WHERE td.name= '%s' AND td.vid = %d AND oc.school_nid = %d", $outcome_name, VOCAB_NTLP_GB_OUTCOME, $school_nid);

    if ($outcome_tid == 0 || empty($outcome_tid)) {
        if ($result->num_rows > 0) {
            $success = 1;
            $message = "You cannot create a Schoolwide Learning Outcome named $outcome_name because an outcome of the same name already exists for your school.";
        }
    } else {

        if ($result->num_rows > 0) {

            if ($data = db_fetch_object($result)) {
                /* checking if found records tid not equal to current editing outcome */
                if ($data->tid != $outcome_tid) {
                    $success = 1;
                    $case = true;
                    $message = "You cannot create a Schoolwide Learning Outcome named $outcome_name because an outcome of the same name already exists for your school.";
                }
            }
        }
    }

    drupal_json(array('success' => $success,
        'message' => $message,
    ));
}
