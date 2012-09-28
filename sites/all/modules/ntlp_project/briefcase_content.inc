<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');

function ntlp_briefcase_edit_form($form_state, $edit = 0) {

    global $base_path;
    include('briefcase_template.inc');
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
//    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/ntlp_project.js');

    if ($courseid == 0) {
//Getting Query String From URL
        $courseid = 0;
        $studentid = 0;
        $url = $_GET['q'];
        $args = explode('/', $url);

        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i]) && empty($courseid))
                $courseid = $args[$i];
            elseif (is_numeric($args[$i]) && empty($studentid))
                $project_id = $args[$i];
        }

    }

    watchdog('proj_briefcase', 'user role in project '.$user_role);
    watchdog('info','ntlp_briefcase_edit_form '.print_r($form_state, true));
    watchdog('briefcase','edit'. $edit);
    watchdog('briefcase','op value'. $form_state['values']['edit_button']);

    $view_state = '';
    if(isset($form_state['values']['edit_button'])) {
        $view_state = $form_state['values']['edit_button'];

    }else {
        if($edit == 1) {
            $view_state ='Manage';
        }else {
            $view_state ='';
        }
    }

    if(isset($form_state['values']['project_id'])) {
        $project_id = $form_state['values']['project_id'];
    }

    if(isset($form_state['values']['course_id'])) {
        $courseid = $form_state['values']['course_id'];
    }

    $user_role = check_user_permission($project_id);
    
    watchdog('briefcase_edit2', 'project_id '. $form_state['values']['project_id'].' course id '.$form_state['values']['course_id']);
    $project_section_arr = get_project_section($project_id);
    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/project_ahah_helper.js', 'footer');

    $has_edit_role = false;
    if(isset($user_role)) {
        if($user_role == NTLP_ROLEID_TEACHER
                || $user_role == NTLP_ROLEID_NETWORKMANAGER
                || $user_role == NTLP_ROLEID_SITEADMIN
                || $user_role == NTLP_ROLEID_SCHOOLLEADER
                || $user_role == NTLP_ROLEID_NETWORKSTAFF) {

            $has_edit_role = true;
        }
    }
    $form['dialog'] = array(
        '#type'   => 'item',
        '#value' => '<div id="dialog-show-confirmation" style="display: none;" title="Warning"><span class="msg"></span><div class="error-msg"></div></div>',
    );
    if($edit == 0) {

        if($has_edit_role) {
            $form['edit_button'] = array(
                    '#type' => 'submit',
                    '#value' => ($view_state != 'Manage') ? t('Manage') : t('Save'),
                    '#attributes' => array('class' => 'PB_manageBtn', 'onmousedown'=>'this.value = this.value==\'Manage\' ? \'Save\' : \'Manage\'','style'=>'position:absolute;right:0;top:15px;margin:0px'),
                    '#ahah' => array(
                            'path' => ahah_helper_path(array('briefcase')),
                            'wrapper' => 'new-row-wrapper',
                            'effect' => 'fade',
                            'progress' => array('type' => 'none', 'message' => t('')),
                    ),
                    '#prefix' => $HTML_Briefcase_Edit_start,
                    '#suffix' => $HTML_Briefcase_Edit_end,
            );
        }

        $form['project_id'] =array(
                '#type' => 'hidden',
                '#value' => $project_id,

        );

        $form['course_id'] =array(
                '#type' => 'hidden',
                '#value' => $courseid,

        );
    }
    $form['briefcase'] = array(
            '#type' => 'fieldset',
//            '#tree' => TRUE,
            '#prefix' => '<div id="new-row-wrapper">'.$HTML_Briefcase_start,
            '#suffix' => $HTML_Briefcase_end.'</div>',
    );

    if($view_state != 'Manage') {

        watchdog('briefcase_view1', 'project section array '.print_r($project_section_arr, true));

        if($user_role == NTLP_ROLEID_TEACHER
                || $user_role == NTLP_ROLEID_NETWORKMANAGER
                || $user_role == NTLP_ROLEID_SITEADMIN
                || $user_role == NTLP_ROLEID_SCHOOLLEADER
                || $user_role == NTLP_ROLEID_NETWORKSTAFF) {

            $form['briefcase']['section_'.$project_section_arr[0]->tid] =array(
                    '#type' => 'item',
                    '#value' => user_access('edit project') ? $project_section_arr[0]->name : '',
                    '#prefix' =>  $HTML_Briefcase_section1_start,
                    '#suffix' =>  $HTML_Briefcase_section1_end,
            );

            $first_view = views_get_view('ntlp_project_section_resources');
//        if(user_access('edit project')) {
            if($has_edit_role) {
                $first_view->set_arguments(array($courseid,$project_section_arr[0]->tid));
            }else {
                $first_view->set_arguments(array($courseid,$project_section_arr[0]->tid, 'S'));
            }
            $first_view->resource_edit_mode = false;
            $first_view->resource_teacher_section = 0;


            $form['briefcase']['view_'.$project_section_arr[0]->tid] =array(
                    '#type' => 'item',
                    '#value' => $first_view->preview(),
                    '#prefix' =>  $HTML_Briefcase_view1_start.'<div id="resource_views_'.$project_section_arr[0]->tid.'" >',
                    '#suffix' =>  '</div>'.$HTML_Briefcase_view1_end,
            );
        }else {
            $form['briefcase']['section_'] =array(
                    '#type' => 'item',
                    '#value' => '',
//                    '#prefix' =>  $HTML_Briefcase_section1_start,
//                    '#suffix' =>  $HTML_Briefcase_section1_end,
                    '#prefix' =>' </div><div class="clearall"></div>
                        <div class="ProjectResources-Content">
                        <div class="FacilitatorMaterials">',
                    '#suffix' =>'</div>',
            );
        }

        for($i = 1 ; $i < sizeof($project_section_arr); $i++) {

            if( $project_section_arr[$i]->availability == 'S' || $has_edit_role) {
                $sectionShowHidden = '';
                if($has_edit_role) {
                    $sectionShowHidden = '<div style="float: right;width:150px"><div style="float: left;font-size:12px;color:#000;font-weight:normal">'.($project_section_arr[$i]->availability == 'S' ? 'Show' : 'Hidden').'</div></div>';
                }
                $form['briefcase']['section_'.$project_section_arr[$i]->tid] =array(
                        '#type' => 'item',
                        '#value' => $project_section_arr[$i]->name.$sectionShowHidden,
                        '#prefix' =>  $HTML_Briefcase_section_Box_start.'<div class="Project-StudentMaterialsHeading"><h3>',
                        '#suffix' =>  '</h3></div>'.$HTML_Briefcase_section_Box_end,
                );

                watchdog('view_briefcase', $courseid.'/'.$project_section_arr[$i]->tid);

                $resource_view = $project_section_arr[$i]->tid;
                $resource_view = views_get_view('ntlp_project_section_resources');

                if($has_edit_role) {
                    $resource_view->set_arguments(array($courseid, $project_section_arr[$i]->tid));
                }else {
                    $resource_view->set_arguments(array($courseid, $project_section_arr[$i]->tid, 'S'));
                }
                $resource_view->resource_edit_mode = false;
                $resource_view->resource_teacher_section = 0;

                $form['briefcase']['view_'.$project_section_arr[$i]->tid] =array(
                        '#type' => 'item',
                        '#value' => $resource_view->preview(),
                        '#prefix' =>  $HTML_Briefcase_section_Boxview_start.'<div id="resource_views_'.$project_section_arr[$i]->tid.'" >',
                        '#suffix' =>  '</div>'.$HTML_Briefcase_section_Boxview_end,
                );
            }
        }


    } else {


        $form['briefcase']['section_'.$project_section_arr[0]->tid] =array(
                '#type' => 'item',
                '#value' => $project_section_arr[0]->name,
                '#prefix' =>  $HTML_Briefcase_section1_start,
                '#suffix' =>  $HTML_Briefcase_section1_end,
        );

        $form['briefcase']['desc_Heading'] =array(
                '#type' => 'item',
                '#prefix' =>  '<div style="background-color:#fff;border-bottom:3px solid #3491cc;"><table cellpadding="2px" cellspacing="2" border="0" width="100%"><tr><th style="color:#3570aa;text-align:right;width:120px;">Description<br /><span style="color:#333;font-size:10px">(max. 100 characters)',
                '#suffix' =>  '</span></th>',
        );

//        watchdog('info','Project Description'.$project_section_arr[0]->description);

        $form['briefcase']['desc_section_'.$project_section_arr[0]->tid] =array(
                '#type' => 'textarea',
                '#value' => $project_section_arr[0]->description,
                '#prefix' =>  '<td>',
                '#suffix' =>  '</td></tr>',
                '#resizable' => false,
                '#attributes' => array('style' => 'width:450px;height:30px;overflow:auto;'),
        );


        $form['briefcase']['add_existing_resource'] =array(
                '#type' => 'item',
                '#value' =>   dlg('Add Existing Resource', 'ntlp/project/existing/resource/'.$courseid.'/'.$project_id.'/'.$project_section_arr[0]->tid, 400, 600),
                //array('attributes' => array('class' => 'popups-form-noupdate'))),
                '#prefix' =>  '<tr><td align="right" colspan="3"><table><tr><td><div style="float:right" class="AddExistingResourceBtn">',
                '#suffix' =>  '</div></td>',
        );

        $form['briefcase']['add_new_resource'] =array(
                '#type' => 'item',
                '#value' => l('Add New Resource','ntlp/project/resource/new/'.$courseid.'/'.$project_id.'/'.$project_section_arr[0]->tid),
                '#prefix' =>  '<td><div style="float:right" class="AddNewResourceBtn">',
                '#suffix' =>  '</div></td></tr></table></td></tr></table></div>',
        );



        $view = views_get_view('ntlp_project_section_resources');
        $view->set_arguments(array($courseid, $project_section_arr[0]->tid));

        $view->resource_edit_mode = true;
        if (intval($project_section_arr[0]->section_order) == 0)
            $first_view->resource_teacher_section = $project_section_arr[0]->tid;
        else
            $first_view->resource_teacher_section = 0;

        $form['briefcase']['view_'.$project_section_arr[0]->tid] =array(
                '#type' => 'item',
                '#value' => $view->preview(),
                '#prefix' =>  $HTML_Briefcase_view1_start.'<div id="resource_views_'.$project_section_arr[0]->tid.'" >',
                '#suffix' =>  '</div>'.$HTML_Briefcase_view1_end,
        );


        modalframe_parent_js();
        $form['briefcase']['add_section_btn'] =array(
                '#type' => 'item',
                '#value' =>   dlg("New Category", 'ntlp/project/add/section/'.$project_id, 300, 500),
                //array('attributes' => array('class' => 'popups-form-reload',) ))  ,
                '#prefix' =>  '<div style="background-color:white" class="AddSectionGreenBtn">',
                '#suffix' =>  '</div><br clear="all" />',
        );

        $last_section_order = get_last_order_section($project_id);

        for($j =1; $j < sizeof($project_section_arr); $j++) {

            $form['briefcase']['desc_section_Heading'.$project_section_arr[$j]->tid] =array(
                    '#type' => 'item',
//                    '#value' => $project_section_arr[$j]->name,
                    '#prefix' =>  $HTML_Briefcase_section_Box_start,
                    '#suffix' =>  $HTML_Briefcase_section_Box_end,
            );

            $form['briefcase']['section_'.$project_section_arr[$j]->tid] =array(
                    '#type' => 'textfield',
                    '#value' => $project_section_arr[$j]->name,
                    '#prefix' =>  '<div style="background-color:#fff;border-bottom:3px solid #3491cc;"><table cellpadding="2px" cellspacing="2" border="0" width="100%"><tr valign="middle"><th style="color:#3570aa;text-align:right;border-bottom:1px solid #CFE6F4;width:140px;padding: 5px 5px 5px 0">Category</th><td style="border-bottom:1px solid #CFE6F4;padding: 5px 0 5px 0"><div style="float:left">',
                    '#suffix' =>  '</div>',
                    '#attributes' => array('style' => 'width:440px;'),
            );


            $form['briefcase']['slider'.$project_section_arr[$j]->tid] =array(
                    '#type' => 'hidden',
                    '#value' => $project_section_arr[$j]->section_order,
                    '#id' => 'slider_'.$j,

            );


            if($project_section_arr[$j]->section_order == 1) {
                $section_order = $project_section_arr[$j]->section_order;
                $project_section_tid = $project_section_arr[$j]->tid;

                $prefix = "<a  onclick='slide_down($section_order,$project_section_tid, $project_id )'> <img src='".$base_path."themes/Boldr/Images/resourceStuff/ArrowGreenDown.png' /> </a>";
            }
            else if($project_section_arr[$j]->section_order == $last_section_order->section_order) {

                $section_order = $project_section_arr[$j]->section_order;
                $project_section_tid = $project_section_arr[$j]->tid;

                $prefix = "<a  onclick='slide_up($section_order,$project_section_tid, $project_id )'> <img src='".$base_path."themes/Boldr/Images/resourceStuff/ArrowGreenUp.png' /> </a>";

            }
            else {

                $section_order = $project_section_arr[$j]->section_order;
                $project_section_tid = $project_section_arr[$j]->tid;
                $prefix = "<a  onclick='slide_up($section_order,$project_section_tid, $project_id )'> <img src='".$base_path."themes/Boldr/Images/resourceStuff/ArrowGreenUp.png' />  </a>&nbsp;'";
                $prefix .= "<a  onclick='slide_down($section_order,$project_section_tid, $project_id )'> <img src='".$base_path."themes/Boldr/Images/resourceStuff/ArrowGreenDown.png' /> </a>";

            }

            $form['briefcase']['up_down'.$project_section_arr[$j]->tid] =array(
                    '#type' => 'item',
                    '#value' => $prefix,
                    '#prefix' => '<div style="float:right"><div style="float:left" class="slideup_down">',
                    '#suffix' => '</div>',
                    //              '#suffix' =>  '</div></td></tr>',

            );

            $form['briefcase']['radioBtnSH_'.$project_section_arr[$j]->tid] =array(
                    '#type' => 'radios',
                    //'#prefix' => '<div style="float:right">',
                    '#prefix' => '<div class="show_hide_radiobtn" style="float:left">',
                    '#suffix' => '</div>',
                    '#default_value' => $project_section_arr[$j]->availability,
                    '#options' => array('S' => t('Show'), 'H' => t('Hide')),
            );
            $form['briefcase']['secDelBtn_'.$project_section_arr[$j]->tid] =array(
                    '#type' => 'item',
//                    '#value' => l('ntlp/project/section/delete/'.$project_section_arr[$j]->tid),
                    '#prefix' => '<div style="float:right" class="ntlp_delete_link1">',
                    '#value' => '<a onclick="delete_project_section('.$project_section_arr[$j]->tid.')">X</a>',
                    '#suffix' => '</div></td></tr>',
            );
            $form['briefcase']['desc_section_'.$project_section_arr[$j]->tid] =array(
                    '#type' => 'textarea',
                    '#value' => $project_section_arr[$j]->description,
                    '#prefix' =>  '<tr><th style="border-bottom:1px solid #CFE6F4;color:#3570aa;text-align:right;width:100px;padding: 5px 5px 5px 0">Description<br /><span style="color:#333;font-size:10px">(max. 100 characters)</span></th><td style="border-bottom:1px solid #CFE6F4;padding: 5px 0 5px 0">',
                    '#suffix' =>  '</td></tr>',
                    '#resizable' => false,
                    '#attributes' => array('style' => 'width:440px;height:30px;overflow:auto;'),

            );
            $form['briefcase']['add_existing_resource_'.$j] =array(
                    '#type' => 'item',
                    '#value' =>   dlg('Add Existing Resource', 'ntlp/project/existing/resource/'.$courseid.'/'.$project_id.'/'.$project_section_arr[$j]->tid, 300, 500),
                    //array('attributes' => array('class' => 'popups-form-noupdate'))),
                    '#prefix' =>  '<tr><td align="right" colspan="2"><table><tr><td><div style="float:right" class="AddExistingResourceBtn">',
                    '#suffix' =>  '</div></td>',
            );

            $form['briefcase']['add_new_resource_'.$j] =array(
                    '#type' => 'item',
                    '#value' =>   l('Add New Resource','ntlp/project/resource/new/'.$courseid.'/'.$project_id.'/'.$project_section_arr[$j]->tid),
                    '#prefix' =>  '<td><div style="float:right" class="AddNewResourceBtn">',
                    '#suffix' =>  '</div></td></tr></table></td></tr></table></div>',
            );

            watchdog('edit_briefcase', $courseid.'/'.$project_section_arr[$j]->tid);
            $view = views_get_view('ntlp_project_section_resources');

            $view->set_arguments(array($courseid, $project_section_arr[$j]->tid));
            $view->resource_edit_mode = true;
            $view->resource_teacher_section = 0;

            $form['briefcase']['view_'.$project_section_arr[$j]->tid] =array(
                    '#type' => 'item',
                    '#value' => $view->preview(),
                    '#prefix' =>  $HTML_Briefcase_section_Boxview_start.'<div id="resource_views_'.$project_section_arr[$j]->tid.'" >',
                    '#suffix' =>  '</div>'.$HTML_Briefcase_section_Boxview_end,
            );
        }
    }
    return $form;
}


function ntlp_briefcase_edit_form_submit($form, $form_state) {


    $op = $form_state['values']['edit_button'];

    if($op != 'Manage') {

        watchdog('brifcase', $form_state['post']['project_id']);
        watchdog('brifcase','briefcase formstate '. print_r($form_state, true));
        $post_array = $form_state['post'];

        watchdog('post_array', print_r($post_array, true));
        foreach($post_array as $key=>$value) {

            if(substr($key,0,8) == 'section_') {
                $newkey = substr($key,8,strlen($key));
                $section_arry[$newkey] = $value;

            }

            if(substr($key,0,13) == 'desc_section_') {
                $newkey = substr($key,13,strlen($key));
                $section_desc[$newkey] = $value;
            }

            if(substr($key,0,11) == 'radioBtnSH_') {
                $newkey = substr($key,11,strlen($key));
                $section_availability[$newkey] = $value;
            }

        }
        watchdog('section_array', print_r($section_arry, true));
        watchdog('section_desc', print_r($section_desc, true));

        if(isset($section_arry)) {

            $section_data = merge_array($section_arry, $section_desc);
            watchdog('section_data', print_r($section_data, true));
            foreach($section_data as $sectionObj) {

                if(isset($sectionObj->name)) {

                    db_query("UPDATE {term_data} SET name ='".$sectionObj->name."', description='".$sectionObj->description."'
                    WHERE tid =".$sectionObj->section_tid);
                }else {

                    db_query("UPDATE {term_data} SET description='".$sectionObj->description."'
                    WHERE tid =".$sectionObj->section_tid);
                }

                if( isset($section_availability[$sectionObj->section_tid]) ) {
                    db_query('UPDATE {ntlp_course_section} SET availability=\'%s\' WHERE tid = %d',
                            $section_availability[$sectionObj->section_tid],
                            $sectionObj->section_tid
                    );
                }
            }
        }else {

            foreach($section_desc as $skey=>$svalue) {

                db_query("UPDATE {term_data} SET description='".$svalue."'
                    WHERE tid =".$skey);
            }
        }
    }
}


function merge_array($section_array, $section_desc) {

    foreach($section_desc as $key=>$value) {

        $sectionObj = new stdClass();

        if (array_key_exists($key,$section_array)) {

            $sectionObj->section_tid = $key;
            $sectionObj->name = $section_array[$key];
            $sectionObj->description = $value;
            $arr[] = $sectionObj;
        }else {
            $sectionObj->section_tid = $key;
            $sectionObj->description = $value;
            $arr[] = $sectionObj;
        }


    }

    return $arr;
}

function ntlp_project_section_delete($course_nid, $project_nid, $section_tid) {

    if( $res_ids = _get_section_resources($section_tid) ) {

        watchdog('section_res', 'section tid '. $section_tid.' section resource fetched '.print_r($res_ids, true));

        foreach($res_ids as $resource_id) {
            delete_project_section_resource($resource_id, $section_tid);

        }
    }

    //update Activity
    db_query('UPDATE {ntlp_course_activity} set  project_cat = %d,project_nid = %d WHERE course_nid = %d AND project_nid= %d and  project_cat = %d', 0,0,$course_nid, $project_nid, $section_tid) ;

    if( db_query('DELETE FROM {ntlp_course_section} WHERE tid = %d', $section_tid) ) {
        taxonomy_del_term($section_tid);
    }

    
    print get_project_sections_data($course_nid, $project_nid);
}
