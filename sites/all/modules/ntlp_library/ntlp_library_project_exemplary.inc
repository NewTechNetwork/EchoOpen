<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


define("NTLP_Subjects", 8);
define("NTLP_Posts", 2);

function _get_ntlp_library_project_community_favorites_contents() {
    return drupal_get_form('_get_ntlp_library_project_community_favorites_form');
}

function _get_ntlp_library_project_community_favorites_form() {
    include("ntlp_library_exemplary.tpl.inc");
    global $base_path,$user;
    if ($courseid == 0) {
        //Getting Query String From URL
        $courseid = 0;
        $project_id = 0;
        $url = $_GET['q'];
        $args = explode('/', $url);
        for ($i = 0;
        $i < count($args);
        $i++) {
            if (is_numeric($args[$i]) && empty($courseid))
                $courseid = $args[$i];
            elseif (is_numeric($args[$i]) && empty($project_id))
                $project_id = $args[$i];
        }
    }

    $project_id = $courseid;

//    echo $project_id;
    

    $form['hall_of_fame'] = array(
            '#type'   => 'fieldset',
            '#title'  => '',
            '#tree'   => TRUE, // Don't forget to set #tree!
            '#prefix' => '',
            '#suffix' => '',
    );


    $form['hall_of_fame']['table'] = array(
            '#type'   => 'item',
            '#value'  => '',
            '#prefix' => str_replace('@TITLE', 'Community Favorites', $HTMLHeader),
            '#suffix' => $HTMLFooter,
    );

    $result = _get_n_most_copied_projects(5);
   
    if($result->num_rows) {
        $i=0;
        while($row = db_fetch_object($result)) {

            $form['hall_of_fame']['table']["tr$i"] = array(
            '#type'   => 'item',
            '#value'  => '',
            '#prefix' => '<tr>',
            '#suffix' => '</tr>',
    );


            $form['hall_of_fame']['table']["tr$i"]['td1'] = array(
            '#type'   => 'item',
            '#value'  => '&nbsp;',       //'<img src="test.jpg">',
            '#prefix' => '<td>',
            '#suffix' => '</td>',
    );


            $form['hall_of_fame']['table']["tr$i"]['td2'] = array(
            '#type'   => 'item',
                    '#value'  => l($row->title. ' ('.intval($row->count).')','ntlp/library/project/view/'.$row->src_project),
            '#prefix' =>  '<td><div class="ntlp_text_style" style="width:196px"><div class="ntlp_text_style_inner">',
            '#suffix' =>  '</div></div></td>',
    );

            $i++;

        }
    }

    return $form;
}
 

function _get_ntlp_library_project_recent_additions_contents() {
    return drupal_get_form('_get_ntlp_library_project_recent_additions_form');
}


function _get_ntlp_library_project_recent_additions_form() {
    include("ntlp_library_exemplary.tpl.inc");
    global $base_path,$user;

    if ($courseid == 0) {
        //Getting Query String From URL
        $courseid = 0;
        $project_id = 0;
        $url = $_GET['q'];
        $args = explode('/', $url);
        for ($i = 0;
        $i < count($args);
        $i++) {
            if (is_numeric($args[$i]) && empty($courseid))
                $courseid = $args[$i];
            elseif (is_numeric($args[$i]) && empty($project_id))
                $project_id = $args[$i];
        }
    }

    $project_id = $courseid;
    $form['hall_of_fame'] = array(
            '#type'   => 'fieldset',
            '#title'  => '',
            '#tree'   => TRUE, // Don't forget to set #tree!
            '#prefix' => '',
            '#suffix' => '',
    );


    $form['hall_of_fame']['table'] = array(
            '#type'   => 'item',
            '#value'  => '',
            '#prefix' => str_replace('@TITLE', 'Recent Additions', $HTMLHeader),
            '#suffix' => $HTMLFooter,
    );

 
    $result = _get_recent_additions();


    if($result->num_rows) {
        $i=0;
        while($recent_additions = db_fetch_object($result)) {
        $course_nid = _get_course_nid($recent_additions->nid);

            $form['hall_of_fame']['table']['tr'.$i] = array(
                    '#type'   => 'item',
                    '#value'  => '',
                    '#prefix' => '<tr>',
                    '#suffix' => '</tr>',
            );

            $form['hall_of_fame']['table']['tr'.$i]['td1'] = array(
                    '#type'   => 'item',
                    '#value'  => '&nbsp;',           //'<img src="test.jpg">',
                    '#prefix' => '<td>',
                    '#suffix' => '</td>',
            );

            $lib_project_node = node_load($recent_additions->nid);
            $node_related_taxo = taxonomy_node_get_terms($lib_project_node );

            $subject_array = array();
            foreach($node_related_taxo as $Obj) {
                if($Obj->vid == NTLP_Subjects) {
                    $subject_array[] = $Obj;
                }
            }
            $first_subject = '';
            if (count($subject_array) > 0)
                $first_subject = '('.$subject_array[0]->name.')';

            $form['hall_of_fame']['table']['tr'.$i]['td2'] = array(
                    '#type'   => 'item',
                    '#value'  => l($recent_additions->title,'ntlp/library/project/view/'.$recent_additions->nid).' '.$first_subject,
                    '#prefix' =>  '<td><div class="ntlp_text_style" style="width:196px"><div class="ntlp_text_style_inner">',
                    '#suffix' =>  '</div></div></td>',
            );
            
            $i+=1;
        }
    }

    return $form;
}


function _get_ntlp_library_project_hall_of_fame_contents() {
    return drupal_get_form('_get_ntlp_library_project_hall_of_fame_form');
}


function _get_ntlp_library_project_hall_of_fame_form() {
    include("ntlp_library_exemplary.tpl.inc");
    global $base_path,$user;

    if ($courseid == 0) {
        //Getting Query String From URL
        $courseid = 0;
        $project_id = 0;
        $url = $_GET['q'];
        $args = explode('/', $url);
        for ($i = 0;
        $i < count($args);
        $i++) {
            if (is_numeric($args[$i]) && empty($courseid))
                $courseid = $args[$i];
            elseif (is_numeric($args[$i]) && empty($project_id))
                $project_id = $args[$i];
        }
    }

    $project_id = $courseid;
    $form['hall_of_fame'] = array(
            '#type'   => 'fieldset',
            '#title'  => '',
            '#tree'   => TRUE, // Don't forget to set #tree!
            '#prefix' => '',
            '#suffix' => '',
    );


    $form['hall_of_fame']['table'] = array(
            '#type'   => 'item',
            '#value'  => '',
            '#prefix' =>  str_replace('@TITLE', 'Hall Of Fame', $HTMLHeader),
            '#suffix' => $HTMLFooter,
    );

    //////////////
    $form['hall_of_fame']['table']['tr1h']['td1'] = array(
            '#type'   => 'item',
            '#value'  => '<strong>Most Active School</strong>',
            '#prefix' => '<tr class="Hall_Of_Fame"><td colspan="2">',
            '#suffix' => '</td></tr>',
    );

    $form['hall_of_fame']['table']['tr1']['td1'] = array(
            '#type'   => 'item',
            '#value'  => '&nbsp;',   //'<img src="test.jpg">',
            '#prefix' => '<tr class="Hall_Of_Fame"><td>',
            '#suffix' => '</td>',
    );

    $most_active_schools = _get_most_active_schools();
    $school_name = get_school_name($most_active_schools['school_nid']);

    $form['hall_of_fame']['table']['tr1']['td2'] = array(
            '#type'   => 'item',
            '#value'  => $school_name .' ('.$most_active_schools['count'].' projects)',
            '#prefix' =>  '<td><div class="ntlp_text_style" style="width:196px"><div class="ntlp_text_style_inner">',
            '#suffix' =>  '</div></div></td></tr>',
    );

    $form['hall_of_fame']['table']['tr3h']['td1'] = array(
            '#type'   => 'item',
            '#value'  => '<strong>Most Active Contributor</strong>',
            '#prefix' => '<tr class="Hall_Of_Fame"><td colspan="2">',
            '#suffix' => '</td></tr>',
    );

    $form['hall_of_fame']['table']['tr3']['td1'] = array(
            '#type'   => 'item',
            '#prefix' => '<tr class="Hall_Of_Fame"><td>',
            '#suffix' => '</td>',
    );

    $most_active_contributor = _get_most_active_contributor();
    $most_active_contributor['count'];
    $form['hall_of_fame']['table']['tr3']['td2'] = array(
            '#type'   => 'item',
            '#value'  => l($most_active_contributor['first_name'] .' '.$most_active_contributor['last_name'] .' ('.$most_active_contributor['count'].' projects)','ntlp/user/profile/'.$most_active_contributor['uid']),
            '#prefix' =>  '<td><div class="ntlp_text_style" style="width:196px"><div class="ntlp_text_style_inner">',
            '#suffix' =>  '</div></div></td></tr>',
    );

    //////////////

    $form['hall_of_fame']['table']['tr4h']['td1'] = array(
            '#type'   => 'item',
            '#value'  => '<strong>Most Copied Project</strong>',
            '#prefix' => '<tr class="Hall_Of_Fame"><td colspan="2">',
            '#suffix' => '</td></tr>',
    );

    $form['hall_of_fame']['table']['tr4']['td1'] = array(
            '#type'   => 'item',
            '#value'  => '&nbsp;',   //'<img src="test.jpg">',
            '#prefix' => '<tr class="Hall_Of_Fame"><td>',
            '#suffix' => '</td>',
    );



    $most_copied_project = _get_most_copied_project();

     $course_nid = _get_course_nid($most_copied_project->src_project);

    $form['hall_of_fame']['table']['tr4']['td2'] = array(
            '#type'   => 'item',
            '#value'  => l($most_copied_project->title .' ('.$most_copied_project->count.' copies)','ntlp/library/project/view/'.$most_copied_project->src_project),
            '#prefix' =>  '<td><div class="ntlp_text_style" style="width:196px"><div class="ntlp_text_style_inner">',
            '#suffix' =>  '</div></div></td></tr>',
    );

    //hall of fame ends here


    return $form;
}

function _get_ntlp_library_total_projects_contents() {
    return drupal_get_form('_get_ntlp_library_total_projects_form');
}



function _get_ntlp_library_total_projects_form() {
    include("ntlp_library_exemplary.tpl.inc");
    global $base_path,$user;

      $form['total_projects']['table'] = array(
            '#type'   => 'item',
            '#value'  => '',
            '#prefix' => str_replace('@TITLE', 'Total Projects', $HTMLHeader),
            '#suffix' => $HTMLFooter,
    );

    //////////////
    $form['total_projects']['table']['tr1'] = array(
            '#type'   => 'item',
            '#value'  => '',
            '#prefix' => '<tr>',
            '#suffix' => '</tr>',
    );


    $total_projects = get_total_projects();

    $form['total_projects']['table']['tr1']['td1'] = array(
            '#type'   => 'item',
//          '#value'  => ,
            '#prefix' => '<td>
                            <h2 align="center" style="padding:0px;margin:2px">'.$total_projects->total.'&nbsp;Total Projects</h2>',
            '#suffix' => '</td>',
    );
    
     $form['total_projects']['table']['tr2'] = array(
            '#type'   => 'item',
            '#value'  => '',
            '#prefix' => '<tr>',
            '#suffix' => '</tr>',
    );
 $total_exemplary_projects = get_exemplary_total_projects();

    $form['total_projects']['table']['tr2']['td1'] = array(
            '#type'   => 'item',
//          '#value'  => ,
            '#prefix' => '<td>
                            <h2 align="center" style="padding:0px;margin:2px">'.$total_exemplary_projects->total.'&nbsp;Exemplary Projects</h2>',
            '#suffix' => '</td>',
    );


    return $form;
}
//
 
?>