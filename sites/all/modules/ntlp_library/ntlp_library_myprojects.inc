<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


function ntlp_library_myprojects_form() {



    $form['project_add'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div id="project_add_test">' . $HTMLform_activity_outer_name_hdr,
        '#suffix' => $HTMLform_activity_outer_name_ftr . '</div>',
    );

    $form['project_add']['course_id'] = array(
        '#type' => 'hidden',
        '#value' => $course_id,
    );

    $data = _get_my_project_list();

    $form['project_add']['projectname'] = array(
        '#type' => 'item',
        '#value' => $data,
        '#prefix' => '<div>',
        '#suffix' => '</div>',
    );



    return $form;
}

function _get_my_project_list() {
    global $user, $base_path;
    $dateformat = "m/d/Y";
    $head = array(
        array('data' => t('Date Posted'), 'field' => 'submitted_on', 'sort' => 'asc'),
        array('data' => t('Title')),
        array('data' => t('Subject')),
        array('data' => t('Stats')),
    );

    $sql = "SELECT * FROM {ntlp_library_project} WHERE submitted_by =" . $user->uid . tablesort_sql($head);


    $limit = 10;
    $result = pager_query($sql, $limit);
    while ($project_rec = db_fetch_object($result)) {
        $project_stats = null;
        $subject = null;
        $project_node = node_load($project_rec->nid);
        $taxObj = taxonomy_get_term($project_rec->category_tid);


        $node_related_taxo = taxonomy_node_get_terms($project_node);

        $project_subject_Obj = _get_lib_vocabulary_vid("NTLP Subjects");


        if (isset($node_related_taxo)) {
        foreach ($node_related_taxo as $Obj) {
            if ($Obj->vid == $project_subject_Obj->vid) {
                $subject_array[] = $Obj;
            }
        }
        }

        if (isset($subject_array)) {
        foreach ($subject_array as $Obj) {
                $subject .= $Obj->name . ", ";
        }
            $subject = rtrim($subject, ", ");
        }

        $likethispost = flag_get_counts('node', $project_rec->nid);
        $project_copy_counts = _get_project_copied_count($project_rec->nid);
        $project_comments_counts = comment_num_all($project_rec->nid);

        $project_stats = '<div>';
        if ($likethispost['likethispost'] > 0) {
            $project_stats .= '<img src="' . $base_path . 'themes/Boldr/Images/common/img_like.png" /> ' . $likethispost['likethispost'] . ' likes ';
        }

        if ($project_copy_counts > 0) {
            $project_stats .= '<img src="' . $base_path . 'themes/Boldr/Images/common/img_copies.png" /> ' . $project_copy_counts . ' copies ';
        }

        if ($project_comments_counts > 0) {
            $project_stats .= '<img src="' . $base_path . 'themes/Boldr/Images/common/img_comment.png" /> ' . $project_comments_counts . ' comments ';
        }
        $project_stats .= '</div>';

//        $likethispost['likethispost'].' likes '._get_project_copied_count($project_rec->nid).' copies '.comment_num_all($project_rec->nid).' comments')
        $rows[] = array(
            array('data' => date($dateformat, strtotime($project_rec->submitted_on))),
            array('data' => l($project_node->title, 'ntlp/library/project/view/' . $project_node->nid)),
            array('data' => $subject),
            array('data' => $project_stats),
        );
    }

    $attributes = array('class' => 'ntlp_table', 'width' => '100%', 'cellpadding' => '8');
    $output = theme('table', $head, $rows, $attributes);

    $output .= theme('pager', NULL, $limit, 0);

    return $output;
}

function ntlp_resource_get_search_contents() {

    return drupal_get_form('ntlp_resource_contents_search_form');
}

function ntlp_resource_contents_search_form() {
    $view = views_get_view('ntlp_library_projects');

    $form['search_page'] = array(
        '#type' => 'item',
        '#value' => $view->render(),
    );

    return $form;
}

function ntlp_project_filter_block() {
    include 'ntlp_library.tpl.inc';
    drupal_add_js(drupal_get_path('module', 'ntlp_library') . '/ntlp_library.js');

    $select_options["All Subjects"] = "0";
    foreach (get_course_subject () as $tid => $name) {
        $select_options["$name"] = $tid;
    }
    
    //FILTER START
    $filterOpts = array(
        'Type' => array(
            '#autoUncheckDefault' => true,
            'All Projects' => array(
                'type' => 'radio',
                'group' => 'project_details',
                'value' => 1,
                'name' => 'all',
                'default' => !(isset($_GET['exp'])),
            ),
            'Exemplary Projects' => array(
                'type' => 'radio',
                'group' => 'project_details',
                'value' => 'exemplary_projects',
                'name' => 0,
                'default' => isset($_GET['exp']),
            ),
            ),
        'Course Subject' => array(
            'Course Subject' => array(
                'type' => 'combobox',
                'options' => $select_options,
                'value' => 'subject',
                'name' => 0,
                'default' => isset($_GET['subject']) ? $_GET['subject'] : '0',
            ),
            ),
        
    );
    //FILTER END

    $args = array();
    if (isset($_GET['subject'])) {
        $args['subject'] = $_GET['subject'];
    }

    if (isset($_GET['keys'])) {
        $args['keys'] = $_GET['keys'];
    }
    $fopts = render_custom_filter(
                    $filterOpts,
                    array('start' => $HTMLProjectLibrarySearchFilter_cellStart, 'end' => $HTMLProjectLibrarySearchFilter_cellEnd),
                    array('template' => $HTMLProjectLibrarySearchFilter_header, 'placeholder' => '@HEADING'),
                    $HTMLProjectLibrarySearchFilter_rowEnd,
                    url('ntlp/library/project/search/filter', array('query' => $args)),
                    'ntlpLibrarySearchWrapper'
    );

    $fopts = str_replace('@FILTER_OPTIONS', $fopts, $HTMLProjectLibrarySearchFilter);
    return $fopts;
}

function ntlp_library_project_search_filter_callback() {

    /*
      project_details[0]          exemplary_projects

      subject                     subject
     */

    $args[] = isset($_REQUEST['subject']) && $_REQUEST['subject'] != 0  ? $_REQUEST['subject'] : 'all';
    $args[] = 'all';
    $args[] = (isset($_REQUEST['project_details']) and in_array('exemplary_projects', $_REQUEST['project_details'])) ? 1 : 'all';
    $args[] = 'all';

    $view->get_total_rows = TRUE;

    $view = views_get_view('ntlp_library_projects');
    $view_output = $view->preview('default', $args);

    $result_text = ($view->total_rows > 0) ? "Viewing <b>" . $view->total_rows . "</b> project" : "";
    $result_text .= ($view->total_rows > 1) ? "s" : "";

    echo $view_output . '
        <script type="text/javascript">
            $(\'div[name=search_result_text]\').html(\'' . $result_text . '\');
            fixProjectLibrarySearchResultPager();
        </script>
    ';
}