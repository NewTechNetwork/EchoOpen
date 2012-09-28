<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


function ntlp_project_section_form() {

    $project_id = check_plain(arg(4));

    $form['project_section_main'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => $HTMLform_activity_outer_name_hdr,
        '#suffix' => $HTMLform_activity_outer_name_ftr,
    );


    $form['project_section_main']['newsectionlink'] = array(
        '#type' => 'item',
        '#value' => '<tr><td><div>' . l("New Category", 'ntlp/project/add/section/' . $project_id,
                array('attributes' => array('class' => 'popups-form',))) . '</td></tr></div>',
    );


    $view = views_get_view('ntlp_projects');
    $view->set_arguments(array($project_id));

    $form['project_section_main']['project_view'] = array(
        '#type' => 'item',
        '#value' => $view->render(),
    );

    return $form;
}

/* popup form */

function ntlp_project_section_add_form($form_state) {

    drupal_set_title("New Project Section");
    $project_id = check_plain(arg(4));

    $form['section_add'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div id="section_add_test"><div id="ActivitiesCSS" style="width:auto">
  <div class="LightBlueBottomRightCorner">
    <div class="LightBlueBottomLeftCorner">
      <div class="LightBlueTopRightCorner">
        <div class="LightBlueTopLeftCorner">
          <div class="ActivitiesAssignment">' . $HTMLform_activity_outer_name_hdr,
        '#suffix' => $HTMLform_activity_outer_name_ftr . '</div>
        </div>
      </div>
    </div>
  </div>
</div></div>',
    );

    $form['section_add']['course_id'] = array(
        '#type' => 'hidden',
        '#value' => $project_id,
    );

    $form['section_add']['sectionname'] = array(
        '#type' => 'textfield',
        '#value' => '',
        '#size' => '57',
        '#field_prefix' => t('<b style="color:#3570AA;">Section Name<b>'),
        '#suffix' => '<br clear="all" />',
    );
    $form['section_add']['errormsg'] = array(
        '#type' => 'item',
        '#value' => '<div style="float:left;width:200px;color:red;" id="edit-section-add-errormsg"></div>',
    );
    $form['section_add']['proj_save_button'] = array(
        '#type' => 'item',
        '#value' => '<a class="SubmitAssignmentBtn" onclick="add_project_section_process(' . $project_id . ')">Add Section</a>',
        '#prefix' => '<div style="float:right">',
        '#suffix' => '</div>',
    );

    return $form;
}

function ntlp_project_section_add_data($project_nid, $category_title, $category_id_order, $visibility) {
    global $user;

    watchdog('info', 'section ' . $category_title . ' project id ' . $project_nid);

    /* get_term_id method adding section into term_data and return tid */
    $section_tid = get_term_id($category_title);

    $record = _get_section_order($category_id_order);

    if ($record != false) {
        $set_new_cat_order = $record->section_order + 1;
    } else {
        $set_new_cat_order = 0;
    }

    fix_category_orders($project_nid, $set_new_cat_order);

    db_query("INSERT INTO {ntlp_course_section} (tid, course_nid, section_order, availability)
        VALUES (%d, %d, %d, '%s')",
            $section_tid, $project_nid, $set_new_cat_order, $visibility);

//    drupal_set_message('New Category Created Successfully');
}

function _get_section_order($category_tid) {

    $result = db_query("SELECT section_order  FROM {ntlp_course_section} WHERE  tid = %d", $category_tid);
    if ($result) {
        $rec = db_fetch_object($result);
        return $rec;
    } else {
        return false;
    }
}

function _get_last_section_order($course_nid) {
    $result = db_query("SELECT Max(section_order) AS sec_order FROM {ntlp_course_section} WHERE course_nid = %d", $course_nid);

    if ($result) {
        $rec = db_fetch_object($result);
        return $rec;
    } else {
        return false;
    }
}

function fix_category_orders($project_nid, $set_new_cat_order) {


    db_query("UPDATE dpl_ntlp_course_section SET section_order = section_order + 1 
        WHERE section_order >= %d AND course_nid = %d ", $set_new_cat_order, $project_nid);
}

function show_hide_section($section_tid, $mode, $course_nid) {

    $project_nid = arg(7);
    $section_only = arg(8);
    watchdog('section_only', $section_only);
    if ($section_only == 'true') {
        watchdog('section_only1', $section_only);
        db_query("UPDATE {ntlp_course_section} SET availability = '%s' WHERE tid = %d ", $mode, $section_tid);
    } else {
        watchdog('section_only2', $section_only);

        db_query("UPDATE {ntlp_course_section} SET availability = '%s' WHERE tid = %d ", $mode, $section_tid);
        db_query("UPDATE {ntlp_project_resource} SET availability = '%s' WHERE section_tid = %d", $mode, $section_tid);

        db_query("UPDATE {ntlp_course_activity} SET availability_mode = '%s'
        WHERE project_cat = %d", (($mode == 'H') ? 'M' : 'S'), $section_tid);
    }
    print get_project_sections_data($course_nid, $project_nid);
}

