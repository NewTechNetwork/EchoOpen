<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_course', 'data_access');
module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_school', 'permission');

require_once ('data_access.inc');

function school_resource_library_category_contents() {
    return drupal_get_form('library_document_category_form');
}

function library_document_category_form() {
    global $base_path, $user;
    include("document.template.inc");
    include('ntlp_admincourse.template.inc');

    $school_id = get_this_school();
    $module_Obj = _get_module_detail('ntlp_school_document_library', $school_id);
    if (empty($module_Obj)) {
        watchdog("lib_doc", 'Creating School Resource Library for School:' . $school_id);
        $dateformat = "%Y-%m-%d %H:%M:%S";
        $nodetype = 'ntlp_group_library_schooldoc';
        $time = strftime($dateformat, strtotime(date('Y-m-d')));
        $group_node = new stdClass();
        $group_node->changed = $time;
        $group_node->status = 1;
        $group_node->promote = 0;
        $group_node->sticky = 0;
        $group_node->format = 1;       // Filtered HTML
        $group_node->uid = $user->uid;
        $group_node->title = get_school_name($school_id);
        $group_node->type = $nodetype;   // Your specified content type
        $group_node->created = $time;
        node_save($group_node);

        create_module_detail($group_node->nid, 'ntlp_school_document_library', $school_id);
        $module_Obj = _get_module_detail('ntlp_school_document_library', $school_id);
    }
    watchdog("lib_doc", 'user school_id ' . $school_id . ' module_nid ' . $module_Obj->nid);

    $user_role = check_user_permission($module_Obj->nid);
    watchdog("lib_doc", 'user role ' . $user_role . ' module nid ' . $module_Obj->nid);

//    echo 'document lib '.$user_role .' module nid '.$module_Obj->nid;
    if ($user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_NETWORKMANAGER
            || $user_role == NTLP_ROLEID_NETWORKSTAFF || $user_role == NTLP_ROLEID_SITEADMIN
            || $user_role == NTLP_ROLEID_STUDENT || $user_role == NTLP_ROLEID_SCHOOLLEADER
            || $user_role == NTLP_ROLEID_PARENT) {

        $module_categories = get_module_categories($module_Obj->nid);
    } else {
        drupal_goto('ntlp/render/error/page/You are not authorized to see this School Resource Library', array('destination' => $_GET['q']));
    }
    set_item_url_handler('School Resource Library Home');

//    print_r($module_categories);
    $form['categories'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div id="project_add_test">' . $HTMLform_activity_outer_name_hdr,
        '#suffix' => $HTMLform_activity_outer_name_ftr . '</div>',
    );


    $form['categories']['project_id'] = array(
        '#type' => 'hidden',
        '#value' => $project_id,
    );

    $form['categories']['course_nid'] = array(
        '#type' => 'hidden',
        '#value' => $course_id,
    );



    if (isset($module_categories)) {
        $count = 0;
        foreach ($module_categories as $category) {

            if (is_user_access_this_category($school_id, $category->name)) {

                $count++;
                $resources = _get_category_resrouces($category->tid);
                $form['categories']['table' . $category->tid] = array(
                    '#type' => 'item',
                    '#prefix' => $HTMLform_activity_inner_name_hdr . '<div class="subData">',
                    '#suffix' => '</div>' . $HTMLform_activity_inner_name_ftr,
                );

                $form['categories']['table' . $category->tid]['td1_' . $category->tid] = array(
                    '#type' => 'item',
                    '#value' => '<a href="' . url('ntlp/document/library/search/' . $category->tid) . '" >' . $category->name . '</a>(' . sizeof($resources) . ')',
                    '#prefix' => '<h2>',
                    '#suffix' => '</h2>',
                );

                $form['categories']['table' . $category->tid]['td2_' . $category->tid] = array(
                    '#type' => 'item',
                    '#value' => $category->description,
                    '#prefix' => '<p>',
                    '#suffix' => '</p>',
                );
//            }
            }
        }

        if ($count == 0) {
            $form['categories']['table'] = array(
                '#type' => 'item',
                '#prefix' => $HTMLform_activity_inner_name_hdr . '<div class="subData">There are no Category created as yet.',
                '#suffix' => '</div>' . $HTMLform_activity_inner_name_ftr,
            );
        }
    }

    return $form;
}

function school_resource_library_search_contents() {
    $output = '';
    $subjects = _get_school_resource_library_keywords(3);


    if (!empty($subjects) && !empty($subjects[0])) {
        $example_subject = '<span style="color:#3570AA;font-weight:bold">Example:</span>';
        foreach ($subjects as $subject) {

            $example_subject .= '<a class="greenLink" href="' . url('ntlp/document/library/search/' . $subject->tid) . '">' . $subject->name . '</a>, ';
        }
        $example_subject = rtrim(trim($example_subject), ',');
    } else {
        $example_subject = '';
    }

    $category_id = arg(4);
    $cat = "";
    if ($category_id > 0) {
        $cat = "/$category_id";
    }

    $output = '<div id="project_add_test">
        <div class="network_blue_border_middle_right" style="width:614px">
                    <div class="network_blue_border_middle_left">
                    <div class="network_blue_border_bottom_center">
                    <div class="network_blue_border_top_center">
                    <div class="network_blue_border_top_left">
                    <div class="network_blue_border_top_right">
                    <div class="network_blue_border_bottom_left">
                    <div class="network_blue_border_bottom_right"><div>
                    <div>
                      <table>
                        <tr>
                            <td class="view-filters">
                               <input type="text" class="form-text" id="search_field" style="width:500px" title="What are you looking for?" 
 onkeydown="if (event.keyCode == 13) {
                                    if(!document.getElementById(\'search_field\').value == \'\'){
                                        window.location =\'?q=ntlp/document/library/search' . $cat . '&keys=\'+document.getElementById(\'search_field\').value;
                                       }
                                    }"
 />
                            </td>
                            <td>
                                <input value="Search"  type="button" class="SearchBtn" 
                                    onclick="
                                    if(document.getElementById(\'search_field\').value == \'\'  || document.getElementById(\'search_field\').value == \'What are you looking for?\'){
                                            //do nothing..
                                        }else{
                                            window.location = \'?q=ntlp/document/library/search' . $cat . '&keys=\'+document.getElementById(\'search_field\').value;
                                          }" />
                            </td>
                        </tr>
                    </table>
                        <div style="clear: both;"></div>
                        <div style="font-size:2px">&nbsp;</div>
                        <div>
                            <!-- ' . $example_subject . ' -->
                        </div>
                    </div>
                    <div style="font-size:10px">&nbsp;</div>
              </div></div></div></div></div></div></div></div></div></div><div style="font-size:10px">&nbsp;</div>';

    return $output;


}

function ntlp_resource_category_info() {
    global $user;
    include 'document.template.inc';
    $category_id = 0;
    $output = '';

    foreach (explode('/', $_GET['q']) as $q) {
        if (is_numeric($q)) {
            $category_id = $q;
        }
    }

    if ($category_id) {
        $resources_total = count(_get_category_resrouces($category_id));
        $cat = taxonomy_get_term($category_id);

        $cuurent_user_role = get_user_role($user->uid);
        $output = $HTMLResourceCategoryInfo;
        $category_name = str_replace("%2C", ",", $cat->name);
        $output = str_replace('@CATEGORY_NAME', $category_name, $output);
        $output = str_replace('@TOTAL_RESOURCES', $resources_total, $output);
        $output = str_replace('@CATEGORY_DETAILS', $cat->description, $output);
    }

    return $output;
}
