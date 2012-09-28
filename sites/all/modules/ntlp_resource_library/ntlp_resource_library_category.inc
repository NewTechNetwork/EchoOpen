<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_course', 'data_access');
module_load_include('inc', 'ntlp_document_library', 'data_access');
module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');


require_once ('data_access.inc');

function network_resource_library_category_contents() {
    return drupal_get_form('library_resource_category_form');
}

function library_resource_category_form() {
    global $base_path;
    include("document.template.inc");

    $module_Obj = _get_module_detail('ntlp_network_resource_library');
    $user_role = check_user_permission($module_Obj->nid);
    $module_categories = get_module_categories($module_Obj->nid);

    set_item_url_handler('Network Resource Library Home');

    $form['categories'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div id="project_add_test">' . $HTMLform_activity_outer_name_hdr,
        '#suffix' => $HTMLform_activity_outer_name_ftr . '</div>',
    );

    if (isset($module_categories)) {
        foreach ($module_categories as $category) {
            $resources = _get_category_resrouces($category->tid);

            $form['categories']['table' . $category->tid] = array(
                '#type' => 'item',
                '#prefix' => $HTMLform_activity_inner_name_hdr . '<div class="subData">',
                '#suffix' => '</div>' . $HTMLform_activity_inner_name_ftr,
            );

            $form['categories']['table' . $category->tid]['td1_' . $category->tid] = array(
                '#type' => 'item',
                '#value' => '<a href="' . url('ntlp/resource/library/search/' . $category->tid) . '" >' . $category->name . '</a> (' . sizeof($resources) . ')',
                '#prefix' => '<h2>',
                '#suffix' => '</h2>',
            );

            $form['categories']['table' . $category->tid]['td2_' . $category->tid] = array(
                '#type' => 'item',
                '#value' => $category->description,
                '#prefix' => '<p>',
                '#suffix' => '</p>',
            );
        }
    }

    return $form;
}

function network_resource_library_search_contents() {
    return ntlp_resource_library_search_block_content();
}

function ntlp_resource_lib_category_info() {
    include 'document.template.inc';
    $category_id = arg(4);
    $output = '';

    if ($category_id) {
        $resources_total = count(_get_category_resrouces($category_id));
        $cat = taxonomy_get_term($category_id);

        $output = $HTMLResourceCategoryInfo;
        $category_name = str_replace("%2C", ",", $cat->name);
        $output = str_replace('@BACK_TO_LINK', l('« Back to Network Resource Library', 'ntlp/resource/library/', array('attributes' => array('class' => 'greenLink'))), $output);
        $output = str_replace('@CATEGORY_NAME', $category_name, $output);
        $output = str_replace('@TOTAL_RESOURCES', $resources_total, $output);
        $output = str_replace('@CATEGORY_DETAILS', $cat->description, $output);
    }

    return $output;
}
