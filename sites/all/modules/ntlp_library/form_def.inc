<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_school', 'data_access');
/**
 * Form controller
 */


function ntlp_module_form_controller($form_state) {
    // Add the client-side behaviors for the examples.
    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') .'/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'image_uploader') . '/image_uploader.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_library') . '/ntlp_library.js');
    include('ntlp_module.template.inc');

    $module_nid = 0;
    $form = array();
    $mode = check_plain(arg(4));        //new or edit

    $module_Obj = _get_project_library_detail();
    $user_role = check_user_permission($module_Obj->nid);

    if($user_role != NTLP_ROLEID_NETWORKSTAFF && $user_role != NTLP_ROLEID_NETWORKMANAGER ) {
        drupal_goto('ntlp/render/error/page/You are not authorized to manage Project Library', array('destination' => $_GET['q']));
    }
    
    $module_nid = $module_Obj->nid;
    $tab = check_plain(arg(6));        //new or edit

//    watchdog('module_page', 'mode '.$mode.' module_nid '.$module_nid .' tab '.$tab );
//    watchdog('info', 'Project Library Form Controller view state:<br />'.print_r($form_state, true));
    if(isset($form_state['values']['op'])) {
        $view_state = $form_state['values']['op'];
        $module_nid = $form_state['values']['module_node'];
    }


    if (!isset($tab) || empty($tab)) {
        $tab = 'basic_stats';
    }

    if (isset($mode)) {

        if (!empty($module_nid)) {
            $rec = get_lib_module($module_nid);
            if ($rec == false) {
                drupal_set_message("Module not found, please contact system administrator");
                return;
            }
        }

    }

    $form['main'] = array(
            '#type' => 'fieldset',

            '#prefix' => $HTMLform_course_setting_start,
            '#suffix' => $HTMLform_course_setting_end,
    );
    $form['main']['module_node'] = array(
            '#type' => 'hidden',
            '#value' => $module_nid,
            '#prefix' => '<div style="display:none">',
            '#suffix' => '</div>',

    );

    if($tab == 'basic_stats') {
        $heading = 'Manage Project Library: Basic Stats';
    }else if($tab == 'review_needed') {
        $heading = 'Manage Project Library: Review Needed';
    }else if($tab == 'archive_requests') {
        $heading = 'Manage Project Library: Archive Requests';
    }else if($tab == 'archive_projects') {
        $heading = 'Manage Project Library: Archived Projects';
    }else if($tab == 'setup') {
        $heading = 'Manage Project Library: Setup';
    }

    if ($tab == 'setup') {

        $form['main']['save'] = array(
                '#type' => 'item',
//                '#value' => 'Save',
                '#prefix' => '<td>&nbsp;</td><td><table cellpadding="0" cellspacing="0" width="100%"><tr><td><div><a href="'.url('ntlp/library/project').'" class="greenLink">« Back to Project Library</a></div><div><h2 style="padding:0px;margin:0px;color:#3570aa;font-size:18px;">'.$heading.'</h2></div></td><td align="right">',
                '#suffix' => '</td></table></td></tr><tr>',
                '#submit' => array('course_form_submit_'.$tab),
//                '#attributes' => array('class' => 'SaveBtn'),
        );
    } else {
        $form['main']['save'] = array(
                '#type' => 'item',
                '#value' => '',
                '#prefix' => '<td>&nbsp;</td><td><table cellpadding="0" cellspacing="0" width="100%"><tr><td><div><a href="'.url('ntlp/library/project').'" class="greenLink">« Back to Project Library</a></div><div><h2 style="padding:0px;margin:0px;color:#3570aa;font-size:18px;">'.$heading.'</h2></div></td><td align="right">',
                '#suffix' => '</td></table></td></tr><tr>',
        );
    }

    ntlp_module_add_tabs($module_nid, $form['main'], $tab);

    $form['main']['form_start'] = array(
            '#type' => 'markup',
            '#value' => $HTMLform_course_setting_form_start);
    if ($tab != 'review_needed')
        $form['main']['form_start0'] = array(
                '#type' => 'markup',
                '#value' => $HTMLform_course_setting_bluebox_start);

    $form['main']['form'] = array(
            '#type' => 'fieldset',

    );

    if ($tab == 'basic_stats') {
        ntlp_module_form_tab1_basic_stats($form_state, $form['main']['form'], $module_nid, $rec);
    } elseif ($tab == 'review_needed') {
        ntlp_module_form_tab2_review_needed($form_state, $form['main']['form'], $module_nid, $rec);
    } elseif ($tab == 'archive_requests') {
        ntlp_module_form_tab3_archive_requests($form_state, $form['main']['form'], $module_nid, $rec);
    } elseif ($tab == 'archive_projects') {
        ntlp_module_form_tab4_archived_projects($form_state, $form['main']['form'], $module_nid, $rec);
    } elseif($tab == 'setup') {
        ntlp_module_form_tab5_setup($form_state, $form['main']['form'], $module_nid, $rec);
    }

    if ($tab != 'review_needed')
        $form['main']['form_end0'] = array(
                '#type' => 'markup',
                '#value' => $HTMLform_course_setting_bluebox_end);
    $form['main']['form_end'] = array(
            '#type' => 'markup',
            '#value' => $HTMLform_course_setting_form_end);

    return $form;
}

/**
 * Form definition
 */
function ntlp_module_form_tab1_basic_stats($form_state, &$form, $module_nid, $rec) {

    $totalReviewsNeeded = get_reviews_needed_count();
    $totalArchiveReqs = get_archive_request_needed_count();

    $form['library_project_info'] = array(
            '#type' => 'item',
            '#value' => t('Review Needed : ' . $totalReviewsNeeded . ' &nbsp;&nbsp;&nbsp;&nbsp; Archive Request ' . $totalArchiveReqs ),
            '#prefix' => '<tr><th style="text-align:left;" colspan="3">',
            '#suffix' => '</th></tr>',

    );

    $publish_project =  _get_published_project($rec->nid);
    $form['library_project_published'] = array(
            '#type' => 'item',
            '#value' => '<span style="font-size:18px;font-weight:bold;color:#3570AA">'.sizeof($publish_project).'</span> <br/> Published Projects',
            '#prefix' => '<tr><td style="color:#3570AA;border:none" align="center">',
            '#suffix' => '</td>',
    );

    $archived_project = _get_archived_project($rec->nid);
    $form['library_project_archived'] = array(
            '#type' => 'item',
            '#value' => '<span style="font-size:18px;font-weight:bold;color:#3570AA">'.sizeof($archived_project).'</span> <br/> Archived Projects',
            '#prefix' => '<td style="color:#3570AA;border:none" align="center">',
            '#suffix' => '</td>',
    );

    $examplary_project = _get_exemplary_project($rec->nid);

    $form['library_project_examplary'] = array(
            '#type' => 'item',
            '#value' => '<span style="font-size:18px;font-weight:bold;color:#3570AA">'.sizeof($examplary_project).'</span> <br/> Examplary Projects',
            '#prefix' => '<td style="color:#3570AA;border:none" align="center">',
            '#suffix' => '</td></tr>',
    );

    $form['library_project_activity_levels_heading'] = array(
            '#type' => 'item',
            '#value' => t('Activity Levels'),
            '#prefix' => '<tr><th colspan="3" style="text-align:left;">',
            '#suffix' => '</th></tr>',
    );


    $form['library_project_activity_levels_1'] = array(
            '#type' => 'item',
            '#value' => t('Current Month'),
            '#prefix' => '<tr><th>',
            '#suffix' => '</th>',
    );

    $totalLikies = get_likes_count_by_date();
    $totalCopies = get_copies_count();
    $totalComments = get_comments_count();

    $form['library_project_month_status'] = array(
            '#type' => 'item',
            '#value' => t('4 Post | ' . $totalCopies . ' Copies | ' . $totalLikies . ' Likes | ' . $totalComments . ' Comments'),
            '#prefix' => '<td colspan="2">',
            '#suffix' => '</td></tr>',
    );

    $form['library_project_activity_levels_2'] = array(
            '#type' => 'item',
            '#value' => t('Monthly Average'),
            '#prefix' => '<tr><th style="border:1px solid #9ECCE9">',
            '#suffix' => '</th>',
    );

    $totalLikiesAvg = round(get_likes_count_by_date(true, array('THIS_MONTH'=>false)));
    $totalCopiesAvg = round(get_copies_count(true, array('THIS_MONTH'=>false)));
    $totalCommentsAvg = round(get_comments_count(true, array('THIS_MONTH'=>false)));

    $form['library_project_month_average'] = array(
            '#type' => 'item',
            '#value' => t('12 Post | ' . $totalCopiesAvg . ' Copies | ' . $totalLikiesAvg . ' Likes | ' . $totalCommentsAvg . ' Comments'),
            '#prefix' => '<td colspan="2">',
            '#suffix' => '</td></tr>',
    );

}

function ntlp_module_form_tab2_review_needed($form_state, &$form, $module_nid, $rec) {
    include('ntlp_module.template.inc');

    $form['review_needed'] = array(
            '#prefix' => $HTMLform_course_setting_bluebox_start,
            '#suffix' => $HTMLform_course_setting_bluebox_end,
    );


    $form['review_needed']['description'] = array(
            '#type' => 'item',
            '#value' => t('Select a Project Title to see more details and submit a review'),
            '#prefix' => '<tr><th style="text-align:left" class="gradesRadio">',
            '#suffix' => '</th></tr>',


    );

    $data = review_needed_list($form);
    $form['review_needed']['review_needed_view'] = array(
            '#type' => 'item',
            '#value' =>  $data,
            '#prefix' => '<tr><td class="gradesRadio">',
            '#suffix' => '</td></tr>',
    );



}

function ntlp_module_form_tab3_archive_requests($form_state, &$form, $module_nid, $rec, $ajax = false) {
    include('ntlp_module.template.inc');

    $form['archived_recquest'] = array(
            '#type'   => 'fieldset',
            '#tree'   => TRUE, // Don't forget to set #tree!
            '#suffix' => '',
    );

    $form['archived_recquest']['archived_projects'] = array(
            '#type' => 'hidden',
            '#id' => 'request_archive_projects',
            '#prefix' => '<div style="display:none">',
            '#suffix' => '</div>',

    );

    $data = archive_request_list($form);

    $form['archived_recquest']['archived_request_view'] = array(
            '#type' => 'item',
            '#value' => $data,
            '#prefix' => '<tr><td class="gradesRadio">',
            '#suffix' => '</td></tr>',
    );


    if(user_access('archive library project')) {
        $form['archived_recquest']['archive_btn'] = array(
                '#type' => 'submit',
                '#value' => t('Archive'),
                '#submit' => array('nltp_module_submit_archive'),
                '#prefix' => '<tr><td class="gradesRadio">',
                '#suffix' => '</td></tr>',
                '#attributes' => array('class' => 'PostBtn'),


        );

    }

}

function ntlp_module_form_tab5_setup($form_state, &$form, $module_nid, $rec) {
    global $base_path;

    drupal_add_js(drupal_get_path('module', 'ntlp_school') .'/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'image_uploader') . '/image_uploader.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_library') . '/ntlp_library.js');

    include('ntlp_module.template.inc');


    $form['save_btn'] = array(
            '#type' => 'submit',
            '#value' => t('Save'),
            '#submit' => array('nltp_module_submit_setup'),
            '#prefix' => '<div style="position:absolute;top:0px;right:0px;">',
            '#suffix' => '</div>',
            '#attributes' => array('class' =>'SaveBtn'),
    );

    $form['module_id'] = array(
            '#type' => 'hidden',
            '#value' => $module_nid,
            '#id' => 'module_id',
            '#prefix' => '<div style="display:none">',
            '#suffix' => '</div>',

    );

    $form['title'] = array(
            '#type' => 'textfield',
            '#value' => $rec->title,
            '#prefix' => '<tr><th>'.t('Title').'</th><td >',
            '#suffix' => '</td></tr>',
    );

    $form['description'] = array(
            '#type' => 'textarea',
            '#value' => $rec->body,
            '#prefix' => '<tr><th>'.t('Description').'</th><td >',
            '#suffix' => '</td></tr>',
            '#resizable' => false,
            '#attributes' => array('style','width:680px;height:30px;overflow:auto;')
    );

    if($file_obj = _get_dpl_files_obj($rec->icon_fid)) {
        $picture = $base_path.$file_obj->filepath;
    }

    $form['picture_id'] = array(
            '#type' => 'hidden',
            '#value' => $rec->icon_fid,
            '#id' => 'picture_fid',
            '#prefix' => '<div style="display:none">',
            '#suffix' => '</div>',

    );

    $form['archive_projects_view'] = array(
            '#type' => 'item',
            //'#value' => t('<--- Change Icon'),
            '#prefix' => '<tr><th>'.t('Icon').'</th><td> <div id="module_image" align="left">'.drupal_get_form('ntlp_activeimage', $picture, 0 ,150).'</div> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;',
            '#suffix' => '</td></tr>',
    );



}

function ntlp_module_form_tab4_archived_projects($form_state, &$form, $module_nid, $rec) {
    include('ntlp_module.template.inc');
    drupal_add_js(drupal_get_path('module', 'ntlp_library') . '/ntlp_library.js');

    $data = archived_project_list($form);
    $form['archive_projects_view'] = array(
            '#type' => 'item',
            '#value' => $data,
            '#prefix' => '<tr><td colspan="2">',
            '#suffix' => '</td></tr>',
    );

    $form['publish_btn'] = array(
            '#type' => 'submit',
            '#value' => t('Publish'),
            '#submit' => array('nltp_module_submit_archived_project'),
            '#prefix' => '<tr><td colspan="2">',
            '#attributes' => array('class' => 'PostBtn'),


    );

    $form['arhived_project'] = array(
            '#type' => 'hidden',
            '#id' => 'publish_project_hidden',
            '#prefix' => '<div style="display:none">',
            '#suffix' => '</div>',

    );


    $form['delete_btn'] = array(
            '#type' => 'submit',
            '#value' => t('Permanently Delete'),
            '#submit' => array('nltp_module_submit_archived_project'),
            '#suffix' => '</td></tr>',
            '#attributes' => array('class' => 'PermanentlyDeleteBtn'),


    );

}

function ntlp_module_add_tabs($course_nid, &$form, $tab = '') {
    include('ntlp_module.template.inc');
    if (empty($course_nid)) {
        $url = "/new/0";
    } else {
        $url = "/edit/$course_nid";
    }
    $form['tabs'] = array(
            '#type' => 'fieldset',

            '#prefix' => $HTMLform_course_setting_tabs_start,
            '#suffix' => $HTMLform_course_setting_tabs_end,
    );
    $roption = $HTMLform_course_settings_tabs_option;
    $roption = str_replace("@LINK", url("ntlp/library/projects/manage$url"), $roption);
    $roption = str_replace("@PROMPT", "Basic Stats", $roption);
    $roption = str_replace("@CLASS", ($tab != 'review_needed' && $tab != 'archive_requests' && $tab != 'archive_projects' && $tab != 'setup') ? "class='active'" : "", $roption);
    $form['tabs']['basic'] = array(
            '#type' => 'markup',
            '#value' => $roption,
    );
    $roption = $HTMLform_course_settings_tabs_option;
    $roption = str_replace("@LINK", url("ntlp/library/projects/manage$url/review_needed"), $roption);
    $roption = str_replace("@PROMPT", "Review Needed", $roption);
    $roption = str_replace("@CLASS", ($tab == 'review_needed') ? "class='active'" : "", $roption);
    $form['tabs']['grade_settings'] = array(
            '#type' => 'markup',
            '#value' => $roption,
    );
    $roption = $HTMLform_course_settings_tabs_option;
    $roption = str_replace("@LINK", url("ntlp/library/projects/manage$url/archive_requests"), $roption);
    $roption = str_replace("@PROMPT", "Archive Requests", $roption);
    $roption = str_replace("@CLASS", ($tab == 'archive_requests') ? "class='active'" : "", $roption);
    $form['tabs']['students'] = array(
            '#type' => 'markup',
            '#value' => $roption,
    );
    $roption = $HTMLform_course_settings_tabs_option;
    $roption = str_replace("@LINK", url("ntlp/library/projects/manage$url/archive_projects"), $roption);
    $roption = str_replace("@PROMPT", "Archived Projects", $roption);
    $roption = str_replace("@CLASS", ($tab == 'archive_projects') ? "class='active'" : "", $roption);
    $form['tabs']['guest_access'] = array(
            '#type' => 'markup',
            '#value' => $roption,
    );
    $roption = $HTMLform_course_settings_tabs_option;
    $roption = str_replace("@LINK", url("ntlp/library/projects/manage$url/setup"), $roption);
    $roption = str_replace("@PROMPT", "Setup", $roption);
    $roption = str_replace("@CLASS", ($tab == 'setup') ? "class='active'" : "", $roption);
    $form['tabs']['data_export'] = array(
            '#type' => 'markup',
            '#value' => $roption,
    );
}

function nltp_module_submit_setup($form, $form_state) {

//    print_r($form_state);
    $module_id = $form_state['clicked_button']['#post']['module_id'];
    $title = $form_state['clicked_button']['#post']['title'];
    $description = $form_state['clicked_button']['#post']['description'];

    $module_node = node_load($module_id);

    $module_node->title = $title;
    $module_node->body = $description;

    node_save($module_node);

}


function review_needed_list($form) {
    $dateformat = "m/d/Y";
    $head = array(
            array('data' => t('Posted') , 'field' => 'submitted_on', 'sort' => 'asc'),
            array('data' => t('Title'), 'field' => 'title', 'sort' => 'asc'),
            array('data' => t('Subjects')),
            array('data' => t('Contributor'), 'field' => 'submitted_by', 'sort' => 'asc'),
            array('data' => t('School')),
    );

    $sql = "SELECT lp.*, nr.title, nr.body  FROM {ntlp_library_project} lp
        INNER JOIN {node} n ON lp.nid = n.nid
        INNER JOIN {node_revisions} nr ON lp.nid = nr.nid
        WHERE lp.reviewer_comment_cid = 0 ".tablesort_sql($head);

    $limit = 5;
    $result = pager_query($sql, $limit);
//    $result = db_query($sql);

    while ($archive_req = db_fetch_object($result)) {
        $subject = null;
        $lib_project_node = node_load($archive_req->nid);
        $node_related_taxo = taxonomy_node_get_terms($lib_project_node );

        foreach($node_related_taxo as $Obj) {
            if($Obj->vid == NTLP_VOCABULARY_SUBJECTS) {
                $subject = $Obj->name;
            }
        }


        $user_Obj = get_ntlp_user_load($archive_req->submitted_by);
        $school_Obj = node_load($user_Obj->school_nid);
        $rows[] =
                array(
                array('data' => date($dateformat,mktime($archive_req->submitted_on))),
                array('data' => '<a href="'.url('ntlp/library/project/view/'.$archive_req->nid).'">'.$archive_req->title.'</a>'),
                array('data' => $subject), // subject
                array('data' => $user_Obj->first_name .' '.$user_Obj->last_name),
                array('data' => $school_Obj->title),
        );
    }
//    return theme('table', $header, $rows);
    $output = theme_table($head, $rows, $attributes = array('class' => 'ntlp_table','width' => '100%','cellpadding' => '5'));
    $output .= theme('pager', NULL, $limit, 0);

    return $output;
}


function archive_request_list($form) {

    $dateformat = "m/d/Y";
    $head = array(
            array('data' => t('') ),
            array('data' => t('Request Date'), 'field' => 'uid', 'sort' => 'asc'),
            array('data' => t('Start')),
            array('data' => t('Title'), 'field' => 'title', 'sort' => 'asc'),
            array('data' => t('Contributor'),  'field' => 'created', 'sort' => 'asc'),
            array('data' => t('School'),  'field' => 'created', 'sort' => 'asc'),
            array('data' => t('Reason'),  'field' => 'created', 'sort' => 'asc'),

    );

    $sql = "SELECT lp.*, nr.title, nr.body  FROM {ntlp_library_project} lp
        INNER JOIN {node} n ON lp.nid = n.nid
        INNER JOIN {node_revisions} nr ON lp.nid = nr.nid
        WHERE lp.archive_request_on != '0000-00-00 00:00:00' AND lp.archived_on = '0000-00-00 00:00:00'".tablesort_sql($head);



//    $sql = "SELECT * FROM {ntlp_library_project} WHERE archive_request_on != '0000-00-00 00:00:00' AND archived_on = '0000-00-00 00:00:00'". tablesort_sql($head);

    $limit = 5;
    $result = pager_query($sql, $limit);

    while ($archive_req = db_fetch_object($result)) {

        $user_Obj = get_ntlp_user_load($archive_req->submitted_by);
        $school_Obj = node_load($user_Obj->school_nid);

        if($archive_req->is_exemplary != 0) {
            $exemplary = 'Star';
        }else {
            $exemplary = '';
        }

        $rows[] =
                array(
                array('data' => '<input type="checkbox" onclick="set_archived_projects('.$archive_req->nid.')" id="chk_'.$archive_req->nid.'" '),
                array('data' => date($dateformat,mktime($archive_req->archive_request_on))),
                array('data' => $exemplary),
                array('data' => '<a href="'.url('ntlp/library/project/view/'.$archive_req->nid).'">'.$archive_req->title.'</a>'),
                array('data' => $user_Obj->first_name .' '.$user_Obj->last_name),
                array('data' => $school_Obj->title),
                array('data' => $archive_req->archive_reason),
        );
    }
//    return theme('table', $header, $rows);
    $output = theme_table($head, $rows, $attributes = array('class' => 'ntlp_table','width' => '100%','cellpadding' => '5'));
    $output .= theme('pager', NULL, $limit, 0);

    return $output;
}


function archived_project_list($form) {

    drupal_add_js(drupal_get_path('module', 'ntlp_library') . '/ntlp_library.js');

    $dateformat = "m/d/Y";
    $head = array(
            array('data' => t('') ),
            array('data' => t('Request Date'), 'field' => 'uid', 'sort' => 'asc'),
            array('data' => t('Start')),
            array('data' => t('Title'), 'field' => 'title', 'sort' => 'asc'),
            array('data' => t('Contributor'),  'field' => 'created', 'sort' => 'asc'),
            array('data' => t('School'),  'field' => 'created', 'sort' => 'asc'),
            array('data' => t('Archive Date'),  'field' => 'archived_on', 'sort' => 'asc'),

    );

    $sql = "SELECT lp.*, nr.title, nr.body  FROM {ntlp_library_project} lp
        INNER JOIN {node} n ON lp.nid = n.nid
        INNER JOIN {node_revisions} nr ON lp.nid = nr.nid
        WHERE lp.archived_on != '0000-00-00 00:00:00'".tablesort_sql($head);

    $limit = 5;
    $result = pager_query($sql, $limit);

    while ($archive_req = db_fetch_object($result)) {

        $user_Obj = get_ntlp_user_load($archive_req->submitted_by);
        $school_Obj = node_load($user_Obj->school_nid);

        if($archive_req->is_exemplary != 0) {
            $exemplary = 'Star';
        }else {
            $exemplary = '';
        }

        $rows[] =
                array(
                array('data' => '<input type="checkbox" onclick="set_published_projects('.$archive_req->nid.')" id="chk_'.$archive_req->nid.'" '),
                array('data' => date($dateformat,mktime($archive_req->submitted_on))),
                array('data' => $exemplary),
                array('data' => '<a href="'.url('ntlp/library/project/view/'.$archive_req->nid).'">'.$archive_req->title.'</a>'),
                array('data' => $user_Obj->first_name .' '.$user_Obj->last_name),
                array('data' => $school_Obj->title),
                array('data' => date($dateformat,mktime($archive_req->archived_on))),
        );
    }

    $output = theme_table($head, $rows, $attributes = array('class' => 'ntlp_table','width' => '100%','cellpadding' => '5'));
    $output .= theme('pager', NULL, $limit, 0);

    return $output;
}

function nltp_module_submit_archive($form, $form_state) {
    global $user;
    $dateformat="%Y-%m-%d %H:%M:%S";
    $today = strftime($dateformat, time());

    $module_node = $form_state['clicked_button']['#post']['module_node'];
    $archived_projects = $form_state['clicked_button']['#post']['archived_recquest']['archived_projects'];

    $projects =  explode(',', $archived_projects);

    if($projects != null) {
        foreach($projects as $proj) {
            if(!empty($proj)) {
                db_query("UPDATE {ntlp_library_project} SET archived_by=%d, archived_on='%s' WHERE nid=%d",
                        $user->uid, $today, $proj );
            }
        }
    }

//    die();

}

function nltp_module_submit_archived_project($form, $form_state) {
    global $user;

    $dateformat="%Y-%m-%d %H:%M:%S";
    $today = strftime($dateformat, time());

    $module_node = $form_state['clicked_button']['#post']['module_node'];
    $selected_project = $form_state['clicked_button']['#post']['arhived_project'];
    $op = $form_state['clicked_button']['#post']['op'];

    $projects =  explode(',', $selected_project);

    if($op == 'Publish') {
        if($projects != null) {
            foreach($projects as $proj) {
                if(!empty($proj)) {

                    db_query("UPDATE {ntlp_library_project} SET archived_by=0, archived_on='0000-00-00 00:00:00',
                published_on ='%s', published_by =%d WHERE nid=%d", $today, $user->uid, $proj );
                }
            }
        }
    }else {
        if($projects != null) {
            foreach($projects as $proj) {
                if(!empty($proj)) {

                    db_query("UPDATE {ntlp_library_project} SET is_deleted= 1, deleted_on='%s', deleted_by = %d
                    WHERE nid=%d", $today, $user->uid, $proj );
                }
            }
        }
    }

}


function get_ntlp_user_load($uid) {

    $result = db_query("SELECT * FROM {ntlp_user} WHERE uid =%d", $uid);
    return db_fetch_object($result);
}


function _get_project_library_detail() {
    $result = db_query("SELECT * FROM {ntlp_module} WHERE type ='ntlp_library_project' ");
    return db_fetch_object($result);
}


function _get_published_project($module_nid) {
    $result = db_query("SELECT * FROM {ntlp_library_project}
        WHERE module_nid = %d AND published_on != '0000-00-00 00:00:00'", $module_nid);
    if($result) {
        while($obj = db_fetch_object($result)) {
            $publish_project[] = $obj;
        }

    }
    return $publish_project;
}

function _get_archived_project($module_nid) {
    $result = db_query("SELECT * FROM {ntlp_library_project}
        WHERE module_nid = %d AND published_on = '0000-00-00 00:00:00' 
        AND archived_on != '0000-00-00 00:00:00'", $module_nid);
    if($result) {
        while($obj = db_fetch_object($result)) {
            $archived_project[] = $obj;
        }

    }
    return $archived_project;
}

function _get_exemplary_project($module_nid) {
    $result = db_query("SELECT * FROM {ntlp_library_project}
        WHERE module_nid = %d AND is_exemplary = 1", $module_nid);
    if($result) {
        while($obj = db_fetch_object($result)) {
            $exemplary_project[] = $obj;
        }

    }
    return $exemplary_project;
}


?>
