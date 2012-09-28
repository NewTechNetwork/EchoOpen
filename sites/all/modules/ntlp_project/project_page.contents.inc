<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_courses_activity', 'data_access');
module_load_include('inc', 'ntlp_group', 'data_access');

include('briefcase_content.inc');

function project_details_contents() {

    return drupal_get_form('ntlp_project_detail_form');
}

function project_briefcase_contents() {

    return drupal_get_form('ntlp_briefcase_edit_form');
}

function project_need_to_know_contents() {

    return drupal_get_form('need_to_know_form') . '
        <div style="display: none;" id="dialog-confirm-ntk" title="Delete Need to Know?">
            <p>Are you sure you want to delete this item?</p>
        </div>';
}

function need_to_know_form($form_state) {

    watchdog('info', 'project_id ' . $form_state['clicked_button']['#post']['need_to_know']['project_id']);
    include('activity_template.inc');
    if ($courseid == 0) {
        //Getting Query String From URL
        $courseid = 0;
        $project_id = 0;
        $url = $_GET['q'];
        $args = explode('/', $url);
        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i]) && empty($courseid))
                $courseid = $args[$i];
            elseif (is_numeric($args[$i]) && empty($project_id))
                $project_id = $args[$i];
        }
    }

    if (isset($form_state['clicked_button']['#post']['need_to_know']['project_id'])) {
        $project_id = $form_state['clicked_button']['#post']['need_to_know']['project_id'];
    }

    $form['need_to_know'] = array(
        '#type' => 'fieldset',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div id="need_to_know_test">' . $HTMLform_NeedtoKnow_title_hdr . '<div class="NeedToKnow">',
        '#suffix' => '</div>' . $HTMLform_NeedtoKnow_title_ftr . '</div>',
    );


    $form['need_to_know']['Heading'] = array(
        '#type' => 'item',
        '#value' => t('Need To Know'),
        '#prefix' => '<div class="Heading"><h3>',
        '#suffix' => '</h3></div>',
    );
    $form['need_to_know']['message'] = array(
        '#type' => 'textfield',
        '#title' => '',
//            '#size' => '80',
        '#attributes' => array('class' => 'need_to_know_field'),
        '#default_value' => '',
        '#prefix' => '<div class="textBox">',
        '#suffix' => '<i>(max. 150 characters)</i></div>',
    );

    $form['need_to_know']['project_id'] = array(
        '#type' => 'hidden',
        '#value' => $project_id,
        '#id' => 'project_id',
    );


    $form['need_to_know']['ntk_post'] = array(
        '#type' => 'submit',
        '#value' => t('Post'),
        '#ahah' => array(
            'path' => ahah_helper_path(array('need_to_know')),
            'wrapper' => 'need_to_know_test',
            'progress' => array('type' => 'throbber', 'message' => t('')),
        ),
        '#submit' => array('need_to_know_data'),
        '#prefix' => '<div align="right" style="float:right" class="profile_PostBtn">',
        '#suffix' => '</div><br clear="all" /><div class="BorderBottom">&nbsp;</div>',
    );

    $view = views_get_view('ntlp_project_needtoknow');

    $view->set_arguments(array($project_id));


    $view->display['default']->display_options['items_per_page'] = 3;

    $form['need_to_know']['ntk_view'] = array(
        '#type' => 'item',
        '#value' => $view->render(),
        '#prefix' => '<div id="ntkContent">',
        '#suffix' => '</div>',
    );

    return $form;
}


function project_group_contents() {

    global $base_path;

    //Getting Query String From URL
    $url = $_GET['q'];
    $args = explode('/', $url);
    for ($i = 0; $i < count($args); $i++) {
        if (is_numeric($args[$i]) && empty($courseid))
            $courseid = $args[$i];
        elseif (is_numeric($args[$i]) && empty($project_id))
            $project_id = $args[$i];
    }

    $groups = _get_project_groups($project_id);

    include('project_group_template.inc');
    $output = $HTML_Project_Group;

    $tempRow = '';
    foreach ($groups as $group) {
        $tempRow .= $HTML_Group_Post_Box;
        $tempRow = str_replace('@GROUP_NAME', l($group->title, 'ntlp/groups/home/' . $group->nid), $tempRow);
        $tempRow = str_replace('@GROUP_POST', $group->new_count . ' New Posts', $tempRow);
    }
    $output = str_replace('@ROOT', $base_path, $output);
    if ($groups) {
        $output = str_replace('@GROUP_POST_LINK', $tempRow, $output);
        $output = str_replace('@VIEW_ALL_LINK', dlg('View All »', 'ntlp/project/groups/popup/' . $project_id, 300, 450), $output);
    } else {
        $output = str_replace('@GROUP_POST_LINK', '', $output);
        $output = str_replace('@VIEW_ALL_LINK', '', $output);
    }

    return $output;
}

function get_project_image_path($project_nid) {
    $result = db_query("SELECT picture_fid FROM {ntlp_course} WHERE nid = %d", $project_nid);

    if ($result) {
        $obj = db_fetch_object($result);
        return $file_Obj = _get_upload_file_detail($obj->picture_fid);
    } else {
        return false;
    }
}

function ntlp_courses_project_ntk_popup($project_id, $user_role) {
    global $base_path, $user;
    modalframe_child_js();
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/jquery.ui.all.css');

    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/ntlp_project.js');

    $need_to_know_entries = get_project_need_to_know_post($project_id);

    $rows = array();


    if ($need_to_know_entries) {
        foreach ($need_to_know_entries as $entries) {
            // print_r($entries);
            $user_obj = get_user_detail_($entries->uid);
            if (is_node_owner($entries->nid, $user) || $user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_NETWORKMANAGER
                    || $user_role == NTLP_ROLEID_NETWORKSTAFF || $user_role == NTLP_ROLEID_SITEADMIN
                    || $user_role == NTLP_ROLEID_SCHOOLLEADER) {
                $post_entry = '<a class="jslink"><img style="vertical-align:top;" src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png" width="14" height="14" onclick="onclick_ntk_remove(' . $entries->nid . ',' . $project_id . ', \'popup\')"></a>';
            } else {
                $post_entry = '';
            }
            $rows[] = array(
                array('data' => '<div style="width:523px;">
                                    <div style="width:auto;">
                                        <div style="word-wrap:break-word;">' . $entries->title . '</div></div></div>  <div> <span style="color:#999; font-size: 8pt; font-weight:bold;">' . $user_obj->first_name . ' ' . $user_obj->last_name . ' &nbsp;&nbsp;' . get_tz(get_this_school(), 'm/d/y @ g:ia', $entries->changed) . '</div>', 'width' => '94%'),
                array('data' => $post_entry, 'style' => 'vertical-align: top;')
            );
        }
    }

    $output = '<div style="height:348px; overflow:auto; border:1px solid #0070C0; width: 577px;">' . theme_table($header, $rows, array('class' => 'need-to-know', 'width' => '98%', 'cellpadding' => '5', 'align' => 'center')) . '</div>';

    return $output;
}

function ntlp_courses_project_groups_table($project_id) {
    print ntlp_courses_project_groups_popup($project_id);
}

function ntlp_courses_project_groups_popup($project_id) {
    modalframe_child_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/ntlp_project.js');

    $limit = 9999;
    $header = array(
        array('data' => 'Group Name', 'field' => 'title'),
        array('data' => 'Total Posts', 'field' => 'new_count', 'sort' => 'desc'),
        array('data' => 'Members'),
    );

    $groups = _get_project_groups($project_id, $limit, $header);
    $course_id = _get_project_course_id($project_id);
    $rows = array();
    if ($groups) {
        foreach ($groups as $group) {

            //show group owners and members (do not show moderators)
            $user_name = '';
            $group_admin_result = get_course_group_owners_and_members($course_id, $group->nid, $group->uid);

            while ($data = db_fetch_object($group_admin_result)) {

                $user_name .= '<div>';
                $user_name .= '<span class="green_title">' . $data->admin_name . '</span>';

                if ($data->is_admin == 0) {

                    $period = (!isset($data->period) || $data->period == 0) ? '*' : 'per ' . $data->period;
                    $user_name .= '<span style="color:#999999"> (' . $period . ')</span>';
                }
                $user_name .= '</div>';
            }

            $total_posts = get_total_posts($group->nid);
            foreach ($total_posts as $total_post) {

                // user names in the popup end
                $rows[] = array(
                    array('data' => l($group->title, 'ntlp/groups/home/' . $group->nid)),
                    array('data' => $total_post, 'style' => 'text-align : center !important;'),
                    array('data' => $user_name),
                );
            }
        }
    } else {
        $rows[] = array(
            array('data' => 'No Record', 'colspan' => 3, 'align' => 'center'),
        );
    }

    $output = theme_table($header, $rows, array('class' => 'ntlp_table', 'width' => '100%', 'cellpadding' => '8', 'style'=> 'margin-top: 0px !important;'));
    $output .= theme('pager', NULL, $limit, 0);

    return '<div id="course_project_groups" style="width: 564px; height: 310px; overflow: auto;">' . $output . '</div>';
}

function ntlp_project_ntk_delete_confirm($ntk_id) {

    db_query('DELETE FROM {node} WHERE nid = %d', $ntk_id);
    db_query('DELETE FROM {node_revisions} WHERE nid = %d', $ntk_id);

// Clear the page and block caches.
    cache_clear_all();

    node_delete($ntk_id);
}

function project_need_to_know_contents_ajax($project_id, $preview = false) {
    //ntlp_courses_activity/datepicker/jquery.ui.all.css

    $view = views_get_view('ntlp_project_needtoknow');
    $view->set_arguments(array($project_id));

    if (strpos($_SERVER['HTTP_REFERER'], 'ntk/popup')) {
        print $view->preview();
    } else {
        $view->display['default']->display_options['items_per_page'] = 3;
        print $view->render();
    }
}

function ntlp_project_view_frm($course_nid, $project_nid) {
    return drupal_get_form('ntlp_project_view_form', $course_nid, $project_nid);
}

function ntlp_project_view_form($form_state = null, $course_nid, $project_nid) {
    global $user;

    include('briefcase_template.inc');
    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/ntlp_project.js');

    if (isset($_GET['sub'])) {
        $submenu = $_GET['sub'];
    } else {
        $submenu = 'teacherview';
    }
    set_item_url_handler('Project Page');

    $user_role = check_user_permission($course_nid);

    $project_obj = load_project_record($project_nid);

    $form['main'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="project_description"><div id="dialog-show-confirmation" style="display: none;" title="Message:">
                        <span class="msg">Are you sure you want to submit the form?</span>
                    </div>',
        '#suffix' => '</div>',
    );

    $form['main']['course_nid'] = array(
        '#type' => 'hidden',
        '#value' => $course_nid,
        '#id' => 'project_course_nid',
    );

    $setting_panel = $HTML_Project_setting_panel;


    $setting_panel = str_replace('@PROJEC_SETTING', l('Project Settings', 'ntlp/courses/project/setting/' . $course_nid . '/' . $project_nid), $setting_panel);
    $setting_panel = str_replace('@POST_TO_PROJECT', dlg('Post to Project Library', 'ntlp/project/save/library/popup/' . $course_nid . '/' . $project_nid, 450, 600, 'anchorLink'), $setting_panel);
    $setting_panel = str_replace('@COPY_PROJECT', dlg('Copy Project', 'ntlp/project/projcopy/' . $course_nid . '/' . $project_nid, 450, 600, 'anchorLink'), $setting_panel);
    $setting_panel = str_replace('@DELETE_PROJECT', dlg('Delete Project', 'ntlp/project/delete/' . $course_nid . '/' . $project_nid . '/' . $courseid, 450, 600, 'anchorLink'), $setting_panel);

    if ($user_role != NTLP_ROLEID_STUDENT && $user_role != NTLP_ROLEID_SITEGUEST
            && !isset($user->roles[NTLP_ROLEID_PARENT])) {
        $form['main']['project_setting'] = array(
            '#type' => 'item',
            '#value' => $setting_panel,
            '#prefix' => '',
            '#suffix' => '',
        );
    }

    $form['main']['project_detail_panel'] = array(
        '#type' => 'item',
        '#value' => get_project_detail_panel($project_nid, $user_role),
        '#prefix' => '',
        '#suffix' => '',
    );


    $form['main']['project_briefcase'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => '<div class="pro_details">',
        '#suffix' => '</div>',
    );

    $form['main']['project_briefcase']['header'] = array(
        '#type' => 'item',
        '#value' => get_project_brieface_header($project_obj, $user_role),
        '#prefix' => '',
        '#suffix' => '',
    );

    $form['main']['project_briefcase']['section'] = array(
        '#type' => 'item',
        '#value' => get_project_sections_data($course_nid, $project_nid, $submenu),
        '#prefix' => '<div id="project_section_data">',
        '#suffix' => '</div>',
    );

    $form['main']['project_briefcase']['right_panel'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => '<div class="clearfix" style="float: left; width: 305px;">',
        '#suffix' => '</div>',
    );

    $form['main']['project_briefcase']['right_panel']['need_to_know'] = array(
        '#type' => 'item',
        '#value' => get_project_need_to_know_post_block($course_nid, $project_nid, $user_role),
        '#prefix' => '',
        '#suffix' => '',
    );

    $form['main']['project_briefcase']['right_panel']['groups'] = array(
        '#type' => 'item',
        '#value' => get_project_briefcase_group_block($project_nid, $user_role),
        '#prefix' => '',
        '#suffix' => '',
    );

    $form['main']['project_briefcase']['right_panel']['learning_outcome'] = array(
        '#type' => 'item',
        '#value' => get_project_learning_outcomes_block($course_nid, $project_nid),
        '#prefix' => '',
        '#suffix' => '',
    );

    $form['main']['project_briefcase']['right_panel']['academic_standards'] = array(
        '#type' => 'item',
        '#value' => get_project_academic_standards_block(nl2br($project_obj->academic_standards)),
        '#prefix' => '',
        '#suffix' => '',
    );

    return $form;
}

function get_project_briefcase_group_block($project_nid, $user_role) {
    global $user;
    include('briefcase_template.inc');

    $project_groups_arr = _get_project_groups($project_nid, 99999);
    $count_groups = count($project_groups_arr);

    $blockHTML = $HTML_Project_briefcase_project_groups_block;
    if ($count_groups == 0) {
        $project_groups_counts = 'There are no groups associated with this project yet.';
    } else {
        $project_groups_counts = '<span>There are <strong>' . $count_groups . '</strong> groups associated with this project.</span>';
    }
    if ($user_role == NTLP_ROLEID_STUDENT) {

        $student_groups = _get_project_groups_for_student_user($project_nid, $user);

        if ($count_groups > 0) {

            if ($student_groups) {
                $project_groups = '<table border="0" width="100%" cellpadding="8" cellspacing="0">';
                for ($i = 0; $i < sizeof($student_groups); $i++) {

                    if ($i == 2) {
                        break;
                    }
                    $project_groups .= '<tr class=""><td><span> 
                    <a class="green_link_new" href="' . url('ntlp/groups/home/' . $student_groups[$i]->nid) . '"> <strong> ' . $student_groups[$i]->title . '</strong> </a>
                    </span> <span style="float: right;"> ' . $student_groups[$i]->new_count . ' new posts</span></td></tr>';
                }
                $project_groups .='</table>';
                $project_groups_counts = $project_groups;
            } else {
                $project_groups_counts = '<span>You are not a member of any groups linked to this project.</span>';
            }
        }
    }

    $blockHTML = str_replace('@PROJECT_GROUP_COUNT', $project_groups_counts, $blockHTML);

    if (!isset($user->roles[NTLP_ROLEID_PARENT]) && $user_role != NTLP_ROLEID_SITEGUEST) {
        $blockHTML = str_replace('@PROJECT_GROUP_NEW_GROUP', '+ ' . dlg('Create Group', 'ntlp/group/add/' . $project_nid, 400, 600, 'green_link'), $blockHTML);
    } else {
        $blockHTML = str_replace('@PROJECT_GROUP_NEW_GROUP', '', $blockHTML);
    }

    if ($count_groups > 0 && $user_role != NTLP_ROLEID_STUDENT) {
        $blockHTML = str_replace('@PROJECT_GROUP_VIEW_ALL', dlg('View All', 'ntlp/project/groups/popup/' . $project_nid, 400, 590, 'green_link') . '&nbsp; &gt; ', $blockHTML);
    } else {
        $blockHTML = str_replace('@PROJECT_GROUP_VIEW_ALL', '', $blockHTML);
    }
    return $blockHTML;
}

function get_project_detail_panel($project_nid, $user_role) {
    global $user;
    include('briefcase_template.inc');

    $project_obj = load_project_record($project_nid);

    $panel = $HTML_Project_detail_panel;

    $panel = str_replace('@PROJECT_NAME', $project_obj->title, $panel);
    $panel = str_replace('@PROJECT_FULL', $project_obj->title, $panel);
    $panel = str_replace('@PROJECT_START_DATE', get_tz_course($project_obj->course_nid, NTLP_DATE_TZ_FJY, $project_obj->from_date), $panel);
    $panel = str_replace('@PROJECT_END_DATE', get_tz_course($project_obj->course_nid, NTLP_DATE_TZ_FJY, $project_obj->to_date), $panel);
    $panel = str_replace('@PROJECT_IMAGE_PATH', get_ntlp_project_image_path($project_obj->picture_fid), $panel);
    $panel = str_replace('@PROJECT_DRIVING_QUESTION_DATA', $project_obj->driving_question, $panel);

    if ($user_role != NTLP_ROLEID_STUDENT && !isset($user->roles[NTLP_ROLEID_PARENT])) {
        $project_visibility_html = '<strong> Visibility: </strong>' . ($project_obj->availability == 'H' ? 'Hide' : 'Show') . '<br>';
    } else {
        $project_visibility_html = '';
    }
    $panel = str_replace('@PROJECT_VISIBLE', $project_visibility_html, $panel);

    return $panel;
}

function get_project_sections_data($course_nid, $project_nid, $mode = 'teacherview') {
    global $base_path, $user;
    include('briefcase_template.inc');
    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/js/jquery.tipsy.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/ntlp_project.js');

    $user_role = check_user_permission($course_nid);
    // Send the Modal Frame javascript for parent windows to the page.
    modalframe_parent_js();

    $project_sections_panel = $HTML_Project_brieface_section_block;

    $project_section_arr = get_project_section($project_nid);

    if ($project_section_arr != null) {
        $section_count = 1;
        $section_counter_for_student = 0;
        foreach ($project_section_arr as $section) {

            $section_resource_count = 0;
            $section_show_hide_status = '';

            drupal_add_js('', inline);

            $section_down_link_image = "<a onclick='slide_down($section->section_order, $section->tid, $project_nid )'> <img height='14' width='14' title='' class='up' src='" . $base_path . "themes/Boldr/Images/resourceStuff/ArrowGreenDown.png' /> </a>";
            $section_up_link_image = "<a  onclick='slide_up($section->section_order, $section->tid, $project_nid )'> <img height='14' width='14' title='' class='down' src='" . $base_path . "themes/Boldr/Images/resourceStuff/ArrowGreenUp.png' /> </a>";

            // gray up link section when there is first section order 0 is diplayed
            if ($section->section_order == 0 || sizeof($project_section_arr) == 1 || $section_count == 1) {
                $section_up_link_image = "<img height='14' width='14' title='' class='down' src='" . $base_path . "themes/Boldr/Images/resourceStuff/ArrowGrayUp.png' />";
            }

            // gray down link section when the last section diplayed.
            if (sizeof($project_section_arr) == $section_count || sizeof($project_section_arr) == 1) {
                $section_down_link_image = "<img height='14' width='14' title='' class='up' src='" . $base_path . "themes/Boldr/Images/resourceStuff/ArrowGrayDown.png' />";
            }

            $section_delete_link = '<a class="jslink" onclick="delete_project_section(' . $course_nid . ',' . $project_nid . ',' . $section->tid . ')"><img width="14" height="14" src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png"> </a>';

            $add_existing_item_tooltip = '<div style="width: 150px; float: left; padding-left: 66px;"> + <a onclick="show_addnew_tipsy(' . $section->tid . ');"  class="addnew' . $section->tid . ' green_link_new"> Add New Item</a></div>';
            $add_copy_item_tooltip = '+ <a onclick="show_copy_tipsy(' . $section->tid . ');"  class="copyitem' . $section->tid . ' green_link_new"> Copy Existing Item</a>';

            $add_existing_activity = dlg('Activity', 'ntlp/project/copy/activity/' . $course_nid . '/' . $project_nid . '/' . $section->tid, 410, 640, 'tipsygreenlinks');
            $add_existing_resource = dlg('Project Resource', 'ntlp/project/copy/resource/' . $course_nid . '/' . $project_nid . '/' . $section->tid, 410, 640, 'tipsygreenlinks');

            $is_section_hidden = ($section->availability == 'S');
            
            if ($user_role == NTLP_ROLEID_PARENT || $user_role == NTLP_ROLEID_STUDENT
                    || $user_role == NTLP_ROLEID_SITEGUEST) {

                if ($section->availability == 'S' ||
                        (!isset($user->roles[NTLP_ROLEID_PARENT]) && $user_role == NTLP_ROLEID_SITEGUEST))
                    $section_counter_for_student++;
            } else {
                if ($mode == 'teacherview') {
                    if ($section->availability == 'S') {
                        $section_show_hide = '<input id="chk_' . $section->tid . '" type="checkbox" name="check" checked="checked" onclick="set_section_status(' . $course_nid . ',' . $project_nid . ',' . $section->tid . ')" > Show';
                    } else {
                        $section_show_hide = '<input id="chk_' . $section->tid . '" type="checkbox" name="check" onclick="set_section_status(' . $course_nid . ',' . $project_nid . ',' . $section->tid . ')" > Show';
                    }
                } else {
                    if ($section->availability == 'S') {
                        $section_counter_for_student++;
                        $section_show_hide = '<input id="chk_' . $section->tid . '" type="checkbox" name="check" checked="checked" onclick="set_section_status(' . $course_nid . ',' . $project_nid . ',' . $section->tid . ')" > Show';
                    } else {
                        $section_show_hide = '<input id="chk_' . $section->tid . '" type="checkbox" name="check" onclick="set_section_status(' . $course_nid . ',' . $project_nid . ',' . $section->tid . ')" > Show';
                    }
                }
            }

            $project_resources = '';

            $project_section = $HTML_Project_briefcase_section;

            //allow category edit only for teacher and above
            if ($user_role <= NTLP_ROLEID_STUDENT || $mode != 'teacherview')
                $project_section = str_replace('@EDIT_IN_PLACE_CATEGORY_TITLE', '
                    <span style="overflow: hidden;white-space: nowrap;width: 290px; float: left;" id="eip@CATEGORYID" >
                        <span class="pro_name_small" id="toedit@CATEGORYID" > @CATEGORY_TITLE </span>
                    </span>', $project_section);
            else
                $project_section = str_replace('@EDIT_IN_PLACE_CATEGORY_TITLE', '
                    <span style="overflow: hidden;white-space: nowrap;width: 290px; float: left;" id="eip@CATEGORYID" onclick="editinplace(@CATEGORYID, \'@CATEGORY_TITLE\')" >
                    <input id="hidden@CATEGORYID" type="hidden" value="@CATEGORYID" />
                        <span class="pro_name_small cat_hover" id="toedit@CATEGORYID" title="click to edit" onmouseover="category_hover_tooltip()" > @CATEGORY_TITLE </span>
                        <span id="txtedit@CATEGORYID" style="display:none;"><input name="" type="text" id="newText@CATEGORYID" maxlength="50" class="onedit" value=""></span>
                    </span>', $project_section);

            $section_resource_records = _get_project_section_resources($course_nid, $section->tid);

            if (check_and_fix_resource_orders($section_resource_records)) {
                $section_resource_records = _get_project_section_resources($course_nid, $section->tid);
            }

            $project_section = str_replace('@SECTION_TOOLTIP_CLASS', 'section_tooltip_' . $section->tid, $project_section);

            $project_section = str_replace('@ADD_NEW_PROJECT_RESOURCE', '<div style="margin-bottom:5px;"><a class="tipsygreenlinks" href="' .
                            url("ntlp/courses/resource/new/$course_nid/$project_nid/$section->tid") . '">Project Resource</a></div>', $project_section);

            $project_section = str_replace('@ADD_NEW_ACT_TASK', '<div style="margin-bottom:5px;"><a class="tipsygreenlinks" href="' .
                            url("ntlp/courses/activity/new/$course_nid/$project_nid/$section->tid") . '&type=lo_task">Task</a></div>', $project_section);


            $project_section = str_replace('@ADD_NEW_ACT_JOURNAL', '<div style="margin-bottom:5px;"><a class="tipsygreenlinks" href="' .
                            url("ntlp/courses/activity/new/$course_nid/$project_nid/$section->tid") . '&type=lo_journal">Journal</a></div>', $project_section);


            $project_section = str_replace('@ADD_NEW_ACT_WORKSHOP', '<div style="margin-bottom:5px;"><a class="tipsygreenlinks" href="' .
                            url("ntlp/courses/activity/new/$course_nid/$project_nid/$section->tid") . '&type=lo_workshop">Workshop</a></div>', $project_section);
            $resource_counter_for_student = 0;
            if ($section_resource_records) {

                $resource_count = 1;

                foreach ($section_resource_records as $resource) {
                    $resource_icon = '';
                    $hideChecked = '';
                    $option = '';
                    $restype = ($resource->restype == 'R') ? 'resource' : 'activity';

                    //Render Resource/Activity Sort UP/DOWN links
                    $slide_down_link_image = "<a onclick=\"onchange_resource_order(1, $resource->nid, $course_nid, $section->tid, '$restype')\"> <img height='14' width='14' title='' class='up' src='" . $base_path . "themes/Boldr/Images/resourceStuff/ArrowGreenDown.png' /> </a>";
                    $slide_up_link_image = "<a onclick=\"onchange_resource_order(0, $resource->nid, $course_nid, $section->tid, '$restype')\"> <img height='14' width='14' title='' class='down' src='" . $base_path . "themes/Boldr/Images/resourceStuff/ArrowGreenUp.png' /> </a>";

                    //this variable is used to set hidden field to find the section resources count
                    $section_resource_count += 1;

                    // gray up link resource when there is first resource order 0 is diplayed
                    if ($resource_count == 1) {
                        $slide_up_link_image = "<img height='14' width='14' title='' class='down' src='" . $base_path . "themes/Boldr/Images/resourceStuff/ArrowGrayUp.png' />";
                    }
                    // gray down link resource when the last resource diplayed.
                    if (sizeof($section_resource_records) == $resource_count || sizeof($section_resource_records) == 1) {
                        $slide_down_link_image = "<img height='14' width='14' title='' class='up' src='" . $base_path . "themes/Boldr/Images/resourceStuff/ArrowGrayDown.png' />";
                    }

                    //Render Resource/Activity edit/delete links
                    $resource_edit_link = '<a href="' . url('ntlp/courses/' . $restype . '/edit/' . $course_nid . '/' . $resource->nid) . '" >
                            <img src="' . $base_path . 'themes/Boldr/Images/common/edit.png" alt="editimage"/>
                        </a>';
                    $resource_delete_link = '<a class="jslink" onclick="onclick_resource_remove(' . $course_nid . ',' . $project_nid . ',' . $resource->nid . ',' . $section->tid . ', \'' . $restype . '\')"><img width="14" height="14" src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png"> </a>';


                    if ($resource->availability == 'H' || $resource->availability == 'M') {
                        $hideChecked = '';
                    } else {
                        $hideChecked = 'checked="checked"';
                    }


                    $resource_show_hide_chk_box = '<input type="checkbox" ' . $hideChecked . ' onclick="onchange_resource_availability(' . $resource->nid . ',' . $section->tid . ', \'' . $restype . '\')" value="" id="resource_' . $resource->nid . '" name="resource[' . $section->tid . ']" /><span class="show_checkbox"> Show </span>';

                    if (strlen($resource->title) > 40) {
                        $resource_title = substr($resource->title, 0, 40) . '...';
                    } else {
                        $resource_title = $resource->title;
                    }

                    $resource_entry = $HTML_Project_briefcase_section_resource_entry;



                    if (empty($resource->attachment_name)) {
                        $resource_path = $resource->filepath;
                    } else {
                        $resource_path = $resource->attachment_name;
                    }

                    $resource_icon = _get_resource_type_icons($resource->type, $resource_path, $resource->doc_type);
                    
//
//                      if($resource->type == 'lo_document'){
//                        if($resource->doc_type == 'G'){
//                            $resource_icon = 'icon_googledoc';
//                        }else{
//                            $resource_icon =  get_resource_file_icon_css($resource->attachment_name);
//                        }
//
//                    }else{
//                       $resource_icon = _get_resource_type_image($resource->type);
//                    }
                    $resource_entry = str_replace('@RESOURCE_TYPE_IMAGE_PATH', $resource_icon, $resource_entry);


                    if (strlen($resource->body) > 700) {
                        $resource_body = substr($resource->body, 0, 700) . '...';
                    } else {
                        $resource_body = $resource->body;
                    }

                    $resource_entry = str_replace('@RESOURCE_ID', $resource->nid, $resource_entry);

                    if ($resource->type == 'lo_link') {
                        $resource_entry = str_replace('@RESOURCE_NAME', '<a class="green_link res' . $resource->nid . '" title="' . strip_tags($resource_body) . '" href="' . get_link_resource_url($resource->nid) . '" target="blank" >' . $resource_title . '</a>', $resource_entry);
                    } else {
                        $is_activity = (
                                        $resource->type == 'lo_task' ||
                                        $resource->type == 'lo_journal' ||
                                        $resource->type == 'lo_rubric' ||
                                        $resource->type == 'lo_workshop');

                        if ($is_activity) {

                            $resource_description = $resource_body;
                            $resource_description = nl2br($resource_description);
                            $resource_description = addslashes ($resource_body);
                            $resource_description = preg_replace("%\n%", "", $resource_description);
                            $resource_description = preg_replace("%\r%", "", $resource_description);
                            $resource_description = '<div class="tooltip_desc_fix">'.$resource_description . '</div>';
                            $resource_description = htmlentities($resource_description, ENT_QUOTES);
                        }
                        else {
                            $resource_description = nl2br($resource_body);
                            $resource_description = preg_replace("%\n%", "", $resource_description);
                            $resource_description = preg_replace("%\r%", "", $resource_description);
                            $resource_description = htmlentities($resource_description, ENT_QUOTES);
                        }

                        $resource_entry = str_replace('@TOOLTIP_TEXT', $resource_description, $resource_entry);

                        $resource_entry = str_replace('@RESOURCE_NAME', l($resource_title, 'ntlp/courses/' . $restype . '/view/' . $course_nid . '/' . $resource->nid, array("attributes" => array('class' => 'green_link res' . $resource->nid))), $resource_entry);
                    }


                    if ($user_role == NTLP_ROLEID_PARENT || $user_role == NTLP_ROLEID_STUDENT
                            || $user_role == NTLP_ROLEID_SITEGUEST) {

                        $resource_entry = str_replace('@RESOURCE_EDIT_LINK', '', $resource_entry);
                        $resource_entry = str_replace('@CHK_BOX_RESOURCE_SHOW_HIDE', '', $resource_entry);
                        $resource_entry = str_replace('@RESOURCE_UP_ARROW_LINK', '', $resource_entry);
                        $resource_entry = str_replace('@RESOURCE_DOWN_ARROW_LINK', '', $resource_entry);
                        $resource_entry = str_replace('@RESOURCE_DELETE_LINK', '', $resource_entry);
                    } else {

                        if ($mode == 'teacherview') {
                            $resource_entry = str_replace('@RESOURCE_EDIT_LINK', $resource_edit_link, $resource_entry);
                            $resource_entry = str_replace('@CHK_BOX_RESOURCE_SHOW_HIDE', $resource_show_hide_chk_box, $resource_entry);
                            $resource_entry = str_replace('@RESOURCE_UP_ARROW_LINK', $slide_up_link_image, $resource_entry);
                            $resource_entry = str_replace('@RESOURCE_DOWN_ARROW_LINK', $slide_down_link_image, $resource_entry);
                            $resource_entry = str_replace('@RESOURCE_DELETE_LINK', $resource_delete_link, $resource_entry);
                        } else {
                            $resource_entry = str_replace('@RESOURCE_EDIT_LINK', '', $resource_entry);
                            $resource_entry = str_replace('@CHK_BOX_RESOURCE_SHOW_HIDE', '', $resource_entry);
                            $resource_entry = str_replace('@RESOURCE_UP_ARROW_LINK', '', $resource_entry);
                            $resource_entry = str_replace('@RESOURCE_DOWN_ARROW_LINK', '', $resource_entry);
                            $resource_entry = str_replace('@RESOURCE_DELETE_LINK', '', $resource_entry);
                        }
                    }

                    if ($user_role == NTLP_ROLEID_PARENT || $user_role == NTLP_ROLEID_STUDENT
                            || $user_role == NTLP_ROLEID_SITEGUEST) {

                        if ($resource->availability == 'S' || 
                                (!isset($user->roles[NTLP_ROLEID_PARENT]) && $user_role == NTLP_ROLEID_SITEGUEST)) {
                            $project_resources .= $resource_entry;
                            $resource_counter_for_student++;
                        }
                    } else {
                        if ($mode == 'teacherview') {
                            $project_resources .= $resource_entry;
                        } else {
                            if ($resource->availability == 'S') {
                                $project_resources .= $resource_entry;
                                $resource_counter_for_student++;
                            }
                        }
                    }
                    $resource_count++;
                }
            }

            $project_section = str_replace('@CATEGORYID', $section->tid, $project_section);
            $project_section = str_replace('@CATEGORY_TITLE', $section->name, $project_section);

            if ($user_role == NTLP_ROLEID_PARENT || $user_role == NTLP_ROLEID_STUDENT
                    || $user_role == NTLP_ROLEID_SITEGUEST) {

                if ($resource_counter_for_student == 0) {
                    $add_existing_item_tooltip = '<div style="width: 220px; float: left; padding-left: 66px;"> There are no items in this category yet.</div>';
                } else {
                    $add_existing_item_tooltip = '';
                }
                $project_section = str_replace('@SECTION_SHOW_HIDE_CHECKBOX', '', $project_section);
                $project_section = str_replace('@SECTION_DELETE_OPTION', '', $project_section);
                $project_section = str_replace('@SECTION_DOWN_ARROW', '', $project_section);
                $project_section = str_replace('@SECTION_UP_ARROW', '', $project_section);
                $project_section = str_replace('@ADD_NEW_TOOLTIP', $add_existing_item_tooltip, $project_section);
                $project_section = str_replace('@ADD_COPY_TOOLTIP', '', $project_section);
                $project_section = str_replace('@COPY_ACTIVITY', '', $project_section);
                $project_section = str_replace('@COPY_RESOURCE', '', $project_section);
            } else {

                if ($mode == 'teacherview') {
                    $project_section = str_replace('@SECTION_SHOW_HIDE_CHECKBOX', $section_show_hide, $project_section);
                    $project_section = str_replace('@SECTION_DELETE_OPTION', $section_delete_link, $project_section);
                    $project_section = str_replace('@SECTION_DOWN_ARROW', $section_down_link_image, $project_section);
                    $project_section = str_replace('@SECTION_UP_ARROW', $section_up_link_image, $project_section);
                    $project_section = str_replace('@ADD_NEW_TOOLTIP', $add_existing_item_tooltip, $project_section);
                    $project_section = str_replace('@ADD_COPY_TOOLTIP', $add_copy_item_tooltip, $project_section);
                    $project_section = str_replace('@COPY_ACTIVITY', $add_existing_activity, $project_section);
                    $project_section = str_replace('@COPY_RESOURCE', $add_existing_resource, $project_section);
                    $project_section = str_replace('@ITEMS_TOOLTIP_CLASS', 'resource_item_tooltip_' . $section->tid, $project_section);
                } else {
                    if ($resource_counter_for_student == 0) {
                        $add_existing_item_tooltip = '<div style="width: 220px; float: left; padding-left: 66px;" > There are no items in this category yet.</div>';
                    } else {
                        $add_existing_item_tooltip = '';
                    }
                    $project_section = str_replace('@SECTION_SHOW_HIDE_CHECKBOX', '', $project_section);
                    $project_section = str_replace('@SECTION_DELETE_OPTION', '', $project_section);
                    $project_section = str_replace('@SECTION_DOWN_ARROW', '', $project_section);
                    $project_section = str_replace('@SECTION_UP_ARROW', '', $project_section);
                    $project_section = str_replace('@ADD_NEW_TOOLTIP', $add_existing_item_tooltip, $project_section);
                    $project_section = str_replace('@ADD_COPY_TOOLTIP', '', $project_section);
                    $project_section = str_replace('@COPY_ACTIVITY', '', $project_section);
                    $project_section = str_replace('@COPY_RESOURCE', '', $project_section);
                }
            }

            $project_section = str_replace('@CATEGORY_RESOURCES_ENTRIES', $project_resources, $project_section);
            if (isset($user->roles[NTLP_ROLEID_PARENT]) || $user_role == NTLP_ROLEID_STUDENT) {
                if ($is_section_hidden) {
                    $html_section .= $project_section;
                }
            } else {

                if ($mode == 'teacherview') {
                    $html_section .= $project_section;
                } else {
                    if ($is_section_hidden) {
                        $html_section .= $project_section;
                    }
                }
            }
            $html_section .= '<input type="hidden" value="' . $section_resource_count . '" id="section_resource_count_' . $section->tid . '" >';
            $section_count++;
        }
    } else {

        if ($user_role == NTLP_ROLEID_PARENT || $user_role == NTLP_ROLEID_STUDENT
                || $user_role == NTLP_ROLEID_SITEGUEST) {

            $html_section = '<style>
            .brief_body{
                padding:3px;
            }
            </style>';
            $html_section .= '<div style="background-color:white;padding:5px">
                        <div><span style="font-size: 12px;margin-left: 10px;">This briefcase does not yet have any visible content. Check back soon!</span></div>
                        </div>';
        } else {

            $html_section = '<style>
            .brief_body{
                padding:3px;
            }
            </style>';
            
            
            //loading template projects.
            $template_project_link = '';
            $template_libproj_arr =  get_template_library_projects();
            if(count($template_libproj_arr) > 0){
                foreach($template_libproj_arr as $template_libproj){
                    $template_project_link .= '<div style="height: 20px;width: 450px;"><span style="font-size: 16px;margin-left: 10px;float:left;"> + <a onclick="copy_from_template_project('.$template_libproj->nid.','.$course_nid.','.$project_nid.')" class="green_link">'.$template_libproj->title.' &nbsp;</a> </span>&nbsp;<span id="loading_'.$template_libproj->nid.'" style="display:none;float:left;">copying..<img  src="'.$base_path.'themes/Boldr/Images/common/throbber-active.gif"></span></div>';
                }
            }
            if ($mode == 'teacherview') {

                $html_section .= '<div style="background-color:white;padding:5px">
                        <div><span style="font-size: 16px;font-weight: bold;margin-left: 10px;">Get Started...</span></div><br/>
                        <div><span style="font-size: 16px;margin-left: 10px;"> + ' . dlg('Create Your First Category', 'ntlp/project/category/add/' . $project_nid, 330, 620, 'green_link') . ' </span></div>'.$template_project_link.'
                        </div>';
            } else {
                $html_section .= '<div style="background-color:white;padding:5px">
                        <div><span style="font-size: 12px;margin-left: 10px;">This briefcase does not yet have any visible content. Check back soon!</span></div>
                        </div>';
            }
        }
    }

    if ($user_role == NTLP_ROLEID_PARENT || $user_role == NTLP_ROLEID_STUDENT
            || $user_role == NTLP_ROLEID_SITEGUEST) {

        if ($section_counter_for_student == 0) {
            $html_section = '<style>
            .brief_body{
                padding:3px;
            }
            </style>';
            $html_section .= '<div style="background-color:white;padding:5px">
                        <div><span style="font-size: 12px;margin-left: 10px;">This briefcase does not yet have any visible content. Check back soon!</span></div>
                        </div>';
        }
    } else {
        if ($mode != 'teacherview') {

            if ($section_counter_for_student == 0) {
                $html_section = '<style>
            .brief_body{
                padding:3px;
            }
            </style>';
                $html_section .= '<div style="background-color:white;padding:5px">
                        <div><span style="font-size: 12px;margin-left: 10px;">This briefcase does not yet have any visible content. Check back soon!</span></div>
                        </div>';
            }
        }
    }

    $project_sections_panel = str_replace('@CATEGORYIES', $html_section, $project_sections_panel);




    return $project_sections_panel;
}

function get_project_brieface_header($project_obj, $user_role) {
    global $base_path;


    include('briefcase_template.inc');

    if (isset($_GET['sub'])) {
        $submenu = $_GET['sub'];
    } else {
        $submenu = 'teacherview';
    }

    $project_briefcase_header = $HTML_Project_brieface_header;

    if ($user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_NETWORKMANAGER
            || $user_role == NTLP_ROLEID_SCHOOLLEADER || $user_role == NTLP_ROLEID_NETWORKSTAFF) {



        $teacher_onclick_action = "window.location = '?q=ntlp/courses/projects/" . $project_obj->course_nid . "/" . $project_obj->nid . "&sub=teacherview';";
        $student_onclick_action = "window.location = '?q=ntlp/courses/projects/" . $project_obj->course_nid . "/" . $project_obj->nid . "&sub=studentview';";

        if ($submenu == 'teacherview') {

            $student_tab_link = '<span class="block_name" ><a onclick="' . $student_onclick_action . '" style="font-weight: normal !important;" >Student</a></span>';
            $teacher_tab_link = '<span> Teacher</span>';

            //$project_briefcase_header = str_replace('@NEW_CATEGORY', '<span> + </span> ' . dlg('New Category', 'ntlp/project/category/add/' . $project_obj->nid, 330, 620, 'green_link'), $project_briefcase_header);
            $project_briefcase_header = str_replace('@NEW_CATEGORY', '<span> + </span> ' . dlg('New Category', 'ntlp/project/category/add/' . $project_obj->nid, 330, 620, 'green_link'), $project_briefcase_header);

            $project_briefcase_header = str_replace('@PROJECT_VIEW_STATE', '<b>View:</b> ' . $teacher_tab_link . ' | ' . $student_tab_link, $project_briefcase_header);
        } else {

            $student_tab_link = '<span> Student</span>';
            $teacher_tab_link = '<span class="block_name"  ><a onclick="' . $teacher_onclick_action . '" style="font-weight: normal !important;" >Teacher</a></span>';


            $project_briefcase_header = str_replace('@NEW_CATEGORY', '', $project_briefcase_header);
            $project_briefcase_header = str_replace('@PROJECT_VIEW_STATE', '<b>View:</b> ' . $teacher_tab_link . ' | ' . $student_tab_link, $project_briefcase_header);
        }
    } else {

        $project_briefcase_header = str_replace('@NEW_CATEGORY', '', $project_briefcase_header);
        $project_briefcase_header = str_replace('@PROJECT_VIEW_STATE', '', $project_briefcase_header);
    }

    return $project_briefcase_header;
}

function get_project_learning_outcomes_block($course_nid, $project_nid) {
    include('briefcase_template.inc');

    $html = $HTML_Project_briefcase_project_learning_outcome_block;

    $result = get_project_outcome_names_and_activities($project_nid);

    $data_html = '';
    while ($data = db_fetch_object($result)) {
        $data_html .= '<tr class="">
                        <td style="border: medium none;"><span>' . $data->name . ' </span><span>(<a class="green_link_new" href="' . url("ntlp/courses/activity/" . $course_nid) . "&sort=lo&def=" . $data->tid . '">' . $data->activity_count . ' activities</a>)</span></td>
                      </tr>';
    }

    $html = str_replace("@LEARNING_OUTCOME", $data_html, $html);

    return ($data_html == '') ? $data_html : $html;
}

function get_project_need_to_know_post_block($course_nid = 0, $project_nid, $user_role) {
    global $base_path, $user;
    include('briefcase_template.inc');

    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/ntlp_project.js');
    $html_ntk = $HTML_Project_briefcase_need_to_know_block;

    $ntk_post_array = get_project_need_to_know_post($project_nid);

    if ($ntk_post_array) {

        for ($i = 0; $i < sizeof($ntk_post_array); $i++) {

            if ($i == 3) {
                break;
            }
            $user_obj = get_user_detail_($ntk_post_array[$i]->uid);

            $ntk_post_delete = '<a><img  onclick="onclick_ntk_remove(' . $ntk_post_array[$i]->nid . ',' . $project_nid . ', \'block\')" height="14" width="14" src="' . $base_path . 'themes/Boldr/Images/DeleteBtn.png" /></a>';

            if (strlen($ntk_post_array[$i]->title) > 35) {
                $bool = true;
                $post_data = substr($ntk_post_array[$i]->title, 0, 35) . '...';
            } else {
                $bool = false;
                $post_data = $ntk_post_array[$i]->title;
            }
//            
            $post_entry = $HTML_need_to_know_entry;
            $post_entry = str_replace('@POST_DATA', $ntk_post_array[$i]->title, $post_entry);
            $post_entry = str_replace('@USER_NAME', l($user_obj->first_name . ' ' . $user_obj->last_name, 'ntlp/user/profile/' . $user_obj->uid, array("attributes" => array('class' => 'block_name'))), $post_entry);
            $post_entry = str_replace('@POST_DATE', get_tz_course($course_nid, NTLP_DATE_TZ_SHORT, $ntk_post_array[$i]->changed), $post_entry);

            // hide the delete post link for parent user;
            if (!isset($user->roles[NTLP_ROLEID_PARENT])) {

                if ($user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_NETWORKMANAGER
                        || $user_role == NTLP_ROLEID_NETWORKSTAFF || $user_role == NTLP_ROLEID_SITEADMIN
                        || $user_role == NTLP_ROLEID_SCHOOLLEADER) {
                    $post_entry = str_replace('@NTK_POST_DELETE', $ntk_post_delete, $post_entry);
                } else {
                    if (is_node_owner($ntk_post_array[$i]->nid, $user)) {
                        $post_entry = str_replace('@NTK_POST_DELETE', $ntk_post_delete, $post_entry);
                    } else {
                        $post_entry = str_replace('@NTK_POST_DELETE', '', $post_entry);
                    }
                }
            } else {
                $post_entry = str_replace('@NTK_POST_DELETE', '', $post_entry);
            }
            $ntk_post_entries .= $post_entry;
        }

        $html_ntk = str_replace('@NEED_TO_KNOW_VIEW_ALL', dlg('View All', 'ntlp/project/ntk/popup/' . $project_nid . '/' . $user_role, 430, 600, 'green_link') . '&nbsp; &gt; ', $html_ntk);

        // hide the new post link for parent user;
        if (!isset($user->roles[NTLP_ROLEID_PARENT]) && $user_role != NTLP_ROLEID_SITEGUEST) {
            $html_ntk = str_replace('@ADD_NEW_NTK_POST', '+ ' . dlg('New Post', 'ntlp/project/needtoknow/add/' . $project_nid, 200, 600, 'green_link'), $html_ntk);
        } else {
            $html_ntk = str_replace('@ADD_NEW_NTK_POST', '', $html_ntk);
        }

        $html_ntk = str_replace('@NEED_TO_KNOW_POST_ENTRY', $ntk_post_entries, $html_ntk);
    } else {
        $html_ntk = str_replace('@NEED_TO_KNOW_VIEW_ALL', '', $html_ntk);
        $html_ntk = str_replace('@ADD_NEW_NTK_POST', '', $html_ntk);

        // hide the link for parent user and site guest;
        if (!isset($user->roles[NTLP_ROLEID_PARENT]) && $user_role != NTLP_ROLEID_SITEGUEST) {
            $html_ntk = str_replace('@NEED_TO_KNOW_POST_ENTRY', '+ ' . dlg('Post the first item', 'ntlp/project/needtoknow/add/' . $project_nid, 160, 590, 'green_link'), $html_ntk);
        } else {
            $html_ntk = str_replace('@NEED_TO_KNOW_POST_ENTRY', '', $html_ntk);
        }
    }





    return $html_ntk;
}

function get_project_academic_standards_block($project_academic_standards) {
    include('briefcase_template.inc');

    if ($project_academic_standards != null || $project_academic_standards = '') {
        $project_academic_standards_html = $HTML_Project_briefcase_project_academic_standards_block;

        $project_academic_standards_html = str_replace("@PROJECT_ACADEMIC_STANDARDS_TEXT", $project_academic_standards, $project_academic_standards_html);
    }

    return $project_academic_standards_html;
}

function get_project_need_to_know_post($project_nid) {

    $records = false;

    $result = db_query("SELECT n.title, n.nid, n.uid, n.created, n.changed FROM {og_ancestry} og_an
        INNER JOIN {node} n ON n.nid = og_an.nid
        WHERE og_an.group_nid = %d ORDER BY n.changed DESC", $project_nid);

    if ($result->num_rows > 0) {
        while ($data = db_fetch_object($result)) {
            $records[] = $data;
        }
    }
    return $records;
}

function _get_project_section_resources($course_nid, $project_section_tid) {
    $flag = false;

    db_query('/* SLAVE */SET @x:=0;');
    $result = db_query("/* SLAVE */SELECT f.fid , f.filepath, cr.type AS doc_type ,cr.attachment_name, nr.body, node.nid, node.type, node.title, cr.course_nid, pr.project_nid, pr.section_tid,
            pr.availability, pr.resource_order, 'R' AS restype, (@X:=@X+1) orderfix
        FROM dpl_node node
        INNER JOIN dpl_ntlp_course_resource cr ON node.nid = cr.nid
        INNER JOIN dpl_ntlp_project_resource pr ON cr.nid = pr.nid
        LEFT join dpl_files f on cr.fid = f.fid
        INNER JOIN dpl_node_revisions nr ON node.nid = nr.nid
        WHERE (node.type IN ('lo_document', 'lo_image', 'lo_link', 'lo_page', 'lo_share_idea', 'lo_video'))
        AND (cr.deleted_on IS NULL) AND (cr.course_nid = %d) AND (pr.section_tid = %d)
    UNION (SELECT 0 AS fid, '' AS filepath , '' AS doc_type, '' AS attachment_name, anr.body, anode.nid, anode.type, anode.title, a.course_nid, a.project_nid, a.project_cat section_tid,
            a.availability_mode availability, a.activity_order resource_order, 'A' AS restype, (@X:=@X+1) orderfix
        FROM dpl_node anode
        INNER JOIN dpl_ntlp_course_activity a ON anode.nid = a.nid
        INNER JOIN dpl_node_revisions anr ON anode.nid = anr.nid
        WHERE (a.course_nid = %d) AND (a.project_cat = %d)
    )
    ORDER BY resource_order, orderfix ASC", $course_nid, $project_section_tid, $course_nid, $project_section_tid);

    if ($result->num_rows > 0) {
        while ($data = db_fetch_object($result)) {
            $flag[] = $data;
        }
    }

    return $flag;
}

function add_new_need_to_know($project_nid) {
    return drupal_get_form('add_new_need_to_know_form', $project_nid);
}

function add_new_need_to_know_form($form_state, $project_nid) {
    global $base_path;
    modalframe_child_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/ntlp_project.js');
    drupal_add_js('
        function textLimit(Event, field, MaxLen) {

            return (field.value.length < MaxLen)||(Event.keyCode == 8 ||Event.keyCode==46||(Event.keyCode>=35&&Event.keyCode<=40));

            }', 'inline');

    $form['add_ntk'] = array(
        '#type' => 'fieldset',
        '#tree' => TRUE,
        '#prefix' => '<div><div class="project_description"><div id="dialog-show-confirmation" style="display: none;" title="Message:">
                        <span class="msg">Are you sure you want to submit the form?</span>
                    </div>',
        '#suffix' => '</div>',
    );

    $form['add_ntk']['main_table'] ['message'] = array(
        '#type' => 'textarea',
        '#attributes' => array('style' => 'width: 350px'),
        '#prefix' => '<div>',
        '#suffix' => '</div>',
        '#wysiwyg' => False,
        '#max_length' => 140,
        '#resizable' => false,
        '#cols' => 70,
        '#rows' => 2,
        '#attributes' => array('onkeydown' => 'return textLimit(event, this,140);', 'style' => 'margin-left: 2px; margin-right: 2px; width: 565px; margin-top: 2px; margin-bottom: 2px; height: 32px; resize: none; overflow:hidden;'),
        '#id' => 'post_message',
    );

    $form['add_ntk']['main_table'] ['messagetooltip'] = array(
        '#type' => 'item',
        '#prefix' => '<div style="padding:5px;">Max characters = 140',
        '#suffix' => '</div>',
    );

    $form['add_ntk']['main_table'] ['project_nid'] = array(
        '#type' => 'hidden',
        '#value' => $project_nid,
        '#id' => 'project_nid',
    );


    $form['add_ntk']['main_table']['submit_btn'] = array(
        '#type' => 'item',
        '#value' => '<a class="SaveBtn" onclick="add_ntk_post()">' . t('Submit') . '</a>',
        '#prefix' => '<div class="clearfix" style="width:135px;"> <span style="float:left; margin-right:10px; width:55px;">',
        '#suffix' => '</span>',
    );

    $form['add_ntk']['main_table']['cancel_btn'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#prefix' => '<span>',
        '#suffix' => '</span>',
        '#attributes' => array('class' => 'CancelBtn', 'onclick' => 'parent.Drupal.modalFrame.close();return false;'),
    );

    return $form;
}

function add_new_need_to_know_data($project_nid, $message) {
    global $user;


    $dateformat = "%Y-%m-%d %H:%M:%S";
    $ctime = strftime($dateformat, strtotime(date('Y-m-d')));


    $ntk_node = new stdClass();
    $ntk_node->title = $message;
    $ntk_node->type = 'ntlp_project_needtoknow';   // Your specified content type
    $ntk_node->created = $ctime;
    $ntk_node->changed = $ctime;
    $ntk_node->status = 1;
    $ntk_node->promote = 0;
    $ntk_node->sticky = 0;
    $ntk_node->format = 1;
    $ntk_node->uid = $user->uid;

    node_save($ntk_node);

    db_query("INSERT dpl_og_ancestry (nid, group_nid) VALUES (%d, %d)", $ntk_node->nid, $project_nid);
}

function rename_category_title($term_id, $category_title) {

    $result = db_query("UPDATE dpl_term_data dtd
               SET dtd.name='%s'
               WHERE dtd.tid=%d", $category_title, $term_id);

    return;
}

function add_new_category($project_nid) {
    return drupal_get_form('add_new_category_form', $project_nid);
}

function add_new_category_form($form_state, $project_nid) {
    global $base_path;
    modalframe_child_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/ntlp_project.js');

    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/js/jquery.tipsy.js');

    $options = array();

    $project_section_arr = get_project_section($project_nid);
    $options['0'] = 'Place this new category below...';
    if ($project_section_arr != null || sizeof($project_section_arr) > 0) {
        foreach ($project_section_arr as $key => $value) {
            $options[$value->tid] = $value->name;
        }
    } else {
        
    }


    $form['add_category'] = array(
        '#type' => 'fieldset',
        '#tree' => TRUE,
        '#prefix' => '<div id="dialog-show-confirmation" style="display: none;" title="Message:">
                        <span class="msg">Are you sure you want to submit the form?</span>
                    </div><table width="100%" style="table-layout:fixed;">',
        '#suffix' => '',
    );

    $form['add_category']['project_nid'] = array(
        '#type' => 'hidden',
        '#value' => $project_nid,
        '#id' => 'hidden_project_nid',
    );

    $form['add_category']['add_category_title'] = array(
        '#type' => 'textfield',
        '#default_value' => '',
        '#size' => '69',
        '#maxlength' => '50',
        '#id' => 'txt_category_title',
        '#prefix' => '<tr><th width="23%" style="text-align: left; color:#3570AA; vertical-align:top;">Category Title</th><td>',
        '#suffix' => '<span style="margin-top: 6px;">Max characters = 50</sapn></td></tr>
                         <tr><td colspan="2">&nbsp;</td></tr>',
    );

    $form['add_category']['add_category_order'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => '',
        '#prefix' => '<tr><th width="23%" style=" text-align: left; color:#3570AA; vertical-align:top;">Category Order</th><td>',
        '#suffix' => '<span class="pro_box" style="float: left; width: 442px; margin-top:5px;">
                          Leave blank to put it at the top of  the briefcase. The order can be changed at any time.</span>
                          </td></tr> <tr><td colspan="2">&nbsp;</td></tr>',
        '#attributes' => array('style' => 'width: 455px'),
        '#id' => 'other_category_id',
    );

    $form['add_category']['add_category_visibility'] = array(
        '#type' => 'checkbox',
        '#default_value' => '',
        '#prefix' => '<tr><th width="23%" style="text-align: left; color:#3570AA; vertical-align:top;">Initial Visibility</th><td><div style="float:left; width:28px;">',
        '#suffix' => '</div> <div style="width: 40px; float: left; margin: 3px 0 0 0;">Show</div> <span class="pro_box" style="float: left; width: 442px; margin-top:5px;">
                          A category must be set to “show” for <strong>any</strong> of its content to be seen by students. This can be changed at any time.
                          </span></td></tr></table>',
        '#id' => 'chk_category_visibility',
    );


    $form['add_category']['submit_btn'] = array(
        '#type' => 'item',
        '#value' => '<a class= "SaveBtn" onclick ="save_new_category();">' . t('Save') . '</a>',
        '#prefix' => ' <br /> <br /><div class="clearfix" style="width:130px; float:right;"> <span style="float:left; margin-right:10px;  width:55px;">',
        '#suffix' => '</span>',
    );

    $form['add_category']['cancel_btn'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#prefix' => '<span style="float: right; width: 60px;">',
        '#suffix' => '</span></div>',
        '#attributes' => array('class' => 'CancelBtn', 'onclick' => 'parent.Drupal.modalFrame.close();return false;'),
    );
    return $form;
}

function copy_existing_item_popup_form($form_state=null, $project_nid) {
    global $base_path;
    modalframe_child_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');

    $form['copy_item'] = array(
        '#type' => 'fieldset',
        '#tree' => TRUE,
        '#prefix' => '<div id="dialog-show-confirmation" style="display: none;" title="Message:">
                        <span class="msg">Are you sure you want to submit the form?</span>
                    </div><table width="100%" style="table-layout:fixed;">',
        '#suffix' => '',
    );

    $form['copy_item']['form'] = array(
        '#type' => 'item',
        '#value' => $output,
        '#prefix' => '',
        '#suffix' => '',
    );


    return $form;
}

function _get_course_projects_resources($course_nid) {

    $result = db_query('SELECT n.type AS resource_type, pr.nid, n.title AS resource_title,
        n2.title AS associate_project, n2.nid AS project_nid
        FROM dpl_ntlp_project_resource pr
        INNER JOIN dpl_node n ON n.nid = pr.nid
        INNER JOIN dpl_node n2 ON n2.nid =  pr.project_nid
        WHERE pr.project_nid IN
        ( SELECT nid FROM dpl_ntlp_course c WHERE c.course_nid = %d AND c.is_project = 1) ', $course_nid);

    while ($resources = db_fetch_object($result)) {

        $project_resources[] = $resources;
    }

    return $project_resources;
}

function copy_existing_activity_dialog($mode, $course_nid, $project_nid, $section_tid) {
    return drupal_get_form('copy_existing_resource', $mode, $course_nid, $project_nid, $section_tid);
}

function copy_existing_resource_table($course_nid) {
    print render_project_resources_table($course_nid);
}

function copy_existing_resource($form_state = null, $mode, $course_nid, $project_nid, $section_tid) {
    global $base_path;
    modalframe_child_js();

    if ($mode == 'resource') {
        drupal_set_title('Copy an Existing Project Resource from this Course');
    } else {
        drupal_set_title('Copy an Existing Project Activity from this Course');
    }

    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/ntlp_project.js');


    $form['copy_resource'] = array(
        '#type' => 'fieldset',
        '#value' => '',
    );

    $form['copy_resource']['project_nid'] = array(
        '#type' => 'hidden',
        '#value' => $project_nid,
        '#id' => 'project_nid',
    );

    if ($mode == 'resource') {

        $form['copy_resource']['header'] = array(
            '#type' => 'item',
            '#value' => '',
            '#prefix' => '<table style="margin-bottom:10px;" width="99%" border=0"><tr><td style="color:#999999">Creates a copy that you can modify</td> <td><div style = "float:right;">',
            '#suffix' => '</div></td></tr></table>',
        );

        $output = render_project_resources_table($course_nid);
    } else if ($mode == 'activity') {

        $ajax_contents = "$.ajax({
        type: 'GET',
        url: '?q=ntlp/copy/project/activity/$course_nid',
        success:  function(data){
            $('#project_activities_table').html(data);
        }

    });";



        $form['copy_resource']['header'] = array(
            '#type' => 'item',
            '#value' => get_course_term_combo($course_nid, true, $ajax_contents),
            '#prefix' => '<table style="margin-bottom:10px;" width="99%" border=0"><tr><td style="color:#999999">Creates a copy that you can modify</td> <td><div style = "float:right;">',
            '#suffix' => '</div></td></tr></table>',
        );

        $output = render_project_activities_table($course_nid);
    }

    $form['copy_resource']['item_table'] = array(
        '#type' => 'item',
        '#value' => $output,
        '#prefix' => '<div id="resources_items_table" style = "width:100%;height:300px;border:1px solid #375F92;overflow:auto;">',
        '#suffix' => '</div>',
    );
    $form['copy_resource']['submit_btn'] = array(
        '#type' => 'item',
        '#value' => '<a class= "SaveBtn" onclick="copy_item_into_section(' . $section_tid . ',\'' . $mode . '\')" >' . t('Submit') . '</a>',
        '#prefix' => ' <br /> <br /><div class="clearfix" style="width:130px;"> <span style="float:left; margin-right:10px;  width:55px;">',
        '#suffix' => '</span>',
    );

    $form['copy_resource']['cancel_btn'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#prefix' => '<span style="float: right; width: 60px;">',
        '#suffix' => '</span></div>',
        '#attributes' => array('class' => 'CancelBtn', 'onclick' => 'parent.Drupal.modalFrame.close();return false;'),
    );

    return $form;
}

function render_project_resources_table($course_nid) {

    drupal_add_js('
    $(document).ready(function() {

        $("#proj_rsc_table").tablesorter( { sortList: [[1,1]] } );

    });', 'inline');

    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/jquery.tablesorter/jquery.tablesorter.js');
    drupal_add_css(drupal_get_path('module', 'ntlp_school') . '/jquery.tablesorter/themes/blue/styles.css', 'module', 'all', FALSE);

    $header = array(
        array('data' => 'Select', 'width' => '11%'),
        array('data' => 'Resource Title', 'width' => '39%'),
        array('data' => 'Type', 'width' => '11%'),
        array('data' => 'Project Association', 'width' => '39%'),
    );


    $all_resources = _get_course_projects_resources($course_nid);

    if ($all_resources) {

        foreach ($all_resources as $resources) {

            if (strlen($resources->resource_title) > 30) {
                $resource_title = substr($resources->resource_title, 0, 30) . '...';
            } else {
                $resource_title = $resources->resource_title;
            }
            if (strlen($resources->associate_project) > 30) {
                $associate_project = substr($resources->associate_project, 0, 30) . '...';
            } else {
                $associate_project = $resources->associate_project;
            }


            $resource_type = $resources->resource_type;
            $resource_image;
            if ($resource_type == 'lo_document') {
                $resource_image = '<img src="' . $root . 'themes/Boldr/Images/resourceStuff/img_document.png">';
            } elseif ($resource_type == 'lo_link') {
                $resource_image = '<img src="' . $root . 'themes/Boldr/Images/resourceStuff/img_link.png">';
            } elseif ($resource_type == 'lo_image') {
                $resource_image = '<img src="' . $root . 'themes/Boldr/Images/resourceStuff/img_picture.png">';
            } elseif ($resource_type == 'lo_video') {
                $resource_image = '<img src="' . $root . 'themes/Boldr/Images/resourceStuff/img_video.png">';
            } elseif ($resource_type == 'lo_page') {
                $resource_image = '<img src="' . $root . 'themes/Boldr/Images/resourceStuff/img_page.png">';
            } elseif ($resource_type == 'lo_share_idea') {
                $resource_image = '<img src="' . $root . 'themes/Boldr/Images/resourceStuff/img_idea.png">';
            }


            $rows[] = array(
                array('data' => '<input onclick="$(\'#proj_rsc_table\').trigger(\'update\');return true;" type="checkbox" name="resource[]" value="' . $resources->nid . '" >', 'align' => 'center'),
                array('data' => $resource_title, 'title' => $resources->resource_title),
                array('data' => $resource_image, 'align' => 'center'),
                array('data' => $associate_project, 'title' => $resources->associate_project),
            );
        }
    }

    $output = theme_table($header, $rows, array('id' => 'proj_rsc_table', 'class' => 'ntlp_table tablesorter', 'width' => '100%', 'cellpadding' => '8', 'style' => 'margin-top:0px !important;'));
    return '<div id="project_resource_table">' . $output . '</div>';
}

function copy_existing_activity_table($course_nid) {
    print render_project_activities_table($course_nid);
}

function render_project_activities_table($course_nid) {

    drupal_add_js('$(document).ready(function() {

    $("#proj_act_table").tablesorter( {sortList: [[1,1]]} );
    });', 'inline');

    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/jquery.tablesorter/jquery.tablesorter.js');
    drupal_add_css(drupal_get_path('module', 'ntlp_school') . '/jquery.tablesorter/themes/blue/styles.css', 'module', 'all', FALSE);

    $header = array(
        array('data' => 'Select', 'width' => '11%'),
        array('data' => 'Activity Title', 'width' => '39%'),
        array('data' => 'Type', 'width' => '11%'),
        array('data' => 'Project Association', 'width' => '39%'),
    );

    $all_activities = get_course_and_project_associate_activities($course_nid);

    if ($all_activities) {

        foreach ($all_activities as $activity) {

            if (strlen($activity->title) > 30) {
                $activity_title = substr($activity->title, 0, 30) . '...';
            } else {
                $activity_title = $activity->title;
            }
            if (strlen($activity->project_title) > 30) {
                $associate_project = substr($activity->project_title, 0, 30) . '...';
            } else {
                $associate_project = $activity->project_title;
            }


            $resource_type = $activity->type;
            $item_type;
            if ($resource_type == 'lo_task') {
                $item_type = 'Task';
            } elseif ($resource_type == 'lo_rubric') {
                $item_type = 'Peer Feedback';
            } elseif ($resource_type == 'lo_journal') {
                $item_type = 'Journal';
            } elseif ($resource_type == 'lo_workshop') {
                $item_type = 'Workshop';
            }


            $rows[] = array(
                array('data' => '<input onclick="$(\'#proj_act_table\').trigger(\'update\');return true;" type="checkbox" name="resource[]" value="' . $activity->nid . '" >', 'align' => 'center'),
                array('data' => $activity_title, 'title' => $activity->title),
                array('data' => $item_type, 'align' => 'center'),
                array('data' => $associate_project, 'title' => $activity->project_title),
            );
        }
    }

    $output = theme_table($header, $rows, array('id' => 'proj_act_table', 'class' => 'ntlp_table tablesorter', 'width' => '100%', 'cellpadding' => '8', 'style' => 'margin-top:0px !important;'));

    return '<div id="project_activities_table">' . $output . '</div>';
}

function get_course_and_project_associate_activities($course_nid) {
    $activities = false;

    $result = db_query("SELECT ca.*,  n.title, n.type, nr.body, project_node.title as project_title
        FROM dpl_ntlp_course_activity ca
        INNER JOIN dpl_node n ON ca.nid = n.nid
        INNER JOIN dpl_node_revisions nr ON nr.nid = ca.nid
        LEFT JOIN dpl_node project_node ON project_node.nid = ca.project_nid
        WHERE (ca.course_nid = %d) AND school_term_tid = %d", $course_nid, get_current_selected_course_term($course_nid));

    if ($result->num_rows > 0) {
        while ($data = db_fetch_object($result)) {

            $activities[] = $data;
        }
    }

    return $activities;
}

function parse_text_to_rtf($text) {
    $final_text = nl2br($text);
    $final_text = preg_replace("%\n%", "", $final_text);
    $final_text = preg_replace("%\r%", "", $final_text);
   // $final_text = htmlentities($final_text, ENT_QUOTES);

    return $final_text;
}
