<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_course', 'data_access');
require_once ('data_access.inc');

function ntlp_document_library_category_add_frm($school_nid, $category_tid) {
    return drupal_get_form('ntlp_document_library_category_add_form',$school_nid, $category_tid);
}

//popup
function ntlp_document_library_category_add_form($form= null, $module_nid, $category_tid = 0) {
    global $base_path;

    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    modalframe_child_js();

    drupal_add_js(drupal_get_path('module', 'ntlp_document_library') . '/ntlp_document_library.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/ntlp_school.js');

    if($category_tid != 0 ) {
        $category_Obj = taxonomy_get_term($category_tid);

        $category_name = str_replace('%2C', ",", $category_Obj->name);
        $description = $category_Obj->description;
    }

    $form['add_categories'] = array(
            '#type'   => 'fieldset',
            '#title'  => '',
            '#tree'   => TRUE, // Don't forget to set #tree!
            '#prefix' => '<div id="dialog-show-confirmation" style="display: none;" title="Warning">
    <span class="msg">Are you sure you want to submit the form?</span>
</div><div id="project_add_test">',
            '#suffix' => '</div>',
    );


    $form['add_categories']['school_id'] = array(
            '#type' => 'hidden',
            '#value' => $school_id,

    );

    $form['add_categories']['module_id'] = array(
            '#type' => 'hidden',
            '#value' => $module_nid,
            '#id' => 'module_id',
    );

    $form['add_categories']['category_id'] = array(
            '#type' => 'hidden',
            '#value' => $category_tid,
            '#id' => 'category_id',

    );


    $form['add_categories']['table'] = array(
            '#type' => 'item',
            '#prefix' => '<table>',
            '#suffix' => '</table>',

    );

    $form['add_categories']['table']['row1'] = array(
            '#type' => 'item',
            '#prefix' => '<tr>',
            '#suffix' => '</tr>',

    );

    $form['add_categories']['table']['row1']['td1'] = array(
            '#type' => 'item',
            '#value' => t('Category Name'),
            '#prefix' => '<td>',
            '#suffix' => '</td>',

    );

    $form['add_categories']['table']['row1']['td2'] = array(
            '#type' => 'textfield',
            '#value' => $category_name,
            '#id' => 'txt_category_name',
            '#prefix' => '<td>',
            '#suffix' => '</td>',

    );

    $form['add_categories']['table']['row2'] = array(
            '#type' => 'item',
            '#prefix' => '<tr><td></td>',
            '#suffix' => '<td></td></tr>',

    );

    $form['add_categories']['table']['row3'] = array(
            '#type' => 'item',
            '#prefix' => '<tr>',
            '#suffix' => '</tr>',

    );

    $form['add_categories']['table']['row3']['td1'] = array(
            '#type' => 'item',
            '#value' => t('Description'),
            '#prefix' => '<td>',
            '#suffix' => '</td>',

    );

    $form['add_categories']['table']['row3']['td2'] = array(
            '#type' => 'textarea',
            '#wysiwyg' => FALSE,
            '#value' => $description,
            '#id' => 'txt_category_description',
            '#prefix' => '<td>',
            '#suffix' => '</td>',

    );

    $form['add_categories']['table']['row4'] = array(
            '#type' => 'item',
            '#prefix' => '<tr>',
            '#suffix' => '</tr>',

    );

    $form['add_categories']['table']['row4']['td1'] = array(
            '#type' => 'item',
            '#prefix' => '<td colspan="2">
                <div style="margin-right:10px;margin-top:10px;float:left"><a class="SaveBtn" onclick="library_save_category()" >Save </a></div>
                <div style="margin-right:10px;margin-top:10px;float:left"><a class="SaveBtn" onclick="library_cancel_category();" >Cancel </a></div>',
            '#suffix' => '</td>',

    );

    return $form;
}


function ntlp_document_library_category_add_data($module_nid, $category_tid = 0) {
    $category_name = $_REQUEST['name'];
    $category_descripition = $_REQUEST['desc'];
//    watchdog('save_cateogry', $module_nid. ' '. $category_name. ' '. $category_descripition .' '.$category_tid);
    
//    $module_node = node_load($module_nid);
    $category_name = str_replace(",", "%2C", $category_name);
    if($category_tid != 0) { // checking on Edit case
//        $term_obj =  taxonomy_get_term($category_tid);
//        $new_term = array('vid' => $term_obj->vid, 'name' => $category_name, 'description' => $category_descripition);

        db_query("UPDATE {term_data} SET name ='%s', description ='%s'
            WHERE tid = %d",$category_name, $category_descripition, $category_tid );
//        watchdog("document_lib", print_r($new_term, true));
    }else {

//        $module_node = node_load($module_nid);
        $termarray = array($category_name);
        $term_obj = get_term_ids($termarray, $category_descripition, "NTLP Library Category");

//        taxonomy_node_save($module_node, $term_obj);

        db_query("INSERT INTO {ntlp_library_category} (category_tid, module_nid )
            VALUES (%d, %d)", $term_obj[0]->tid, $module_nid);
    }
}


function get_term_ids($incoming_terms, $categoy_desc, $term_name="NTLP Posts") {
    $post_vocab_Object = _get_lib_vocabulary_vid($term_name);
    if($incoming_terms !=null) {
        foreach ($incoming_terms as $term) {
//            $query = db_query("SELECT tid FROM {term_data} WHERE name='%s'", $term);
//            $possible_term = db_fetch_object($query);

            //if the term doesn't already exist, add it to the cloud
//            if (!$possible_term->tid) {
            $new_term = array('vid' => $post_vocab_Object->vid, 'name' => $term, 'description' =>$categoy_desc);
            if (taxonomy_save_term($new_term)) {

                $new_terms = new stdClass();
                $new_terms->tid = $new_term['tid'];
                $new_terms->vid = $post_vocab_Object->vid;
                $new_terms->name = $term;
                $new_terms->description = $categoy_desc;
                $new_terms->weight = 0;

                $terms_array[] = $new_terms;
            }
//            }
//            else {
//                $old_terms = new stdClass();
//                $old_terms->tid = $possible_term->tid;
//                $old_terms->vid = $post_vocab_Object->vid;
//                $old_terms->name = $term;
//                $old_terms->description = $categoy_desc;
//                $old_terms->weight = 0;
//
//                $terms_array[] = $old_terms;
//            }
        }
    }

//    watchdog('terms_array',print_r($terms_array,true));
    return $terms_array;
}



?>
