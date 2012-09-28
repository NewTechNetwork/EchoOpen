<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_group', 'filter_functions');
module_load_include('inc', 'ntlp_school', 'constants');

function resources_block_contents() {
    include "resources.template.inc";
    global $user, $base_url;
    $user_id = $user->uid;



    if ($courseid == 0) {
        //Getting Query String From URL
        $courseid = 0;
        $url = $_GET['q'];
        $args = explode('/', $url);
        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i]))
                $courseid = $args[$i];
        }
//        if ($args[0] == 'node')
//            $courseid = $args[1];
    }

    $user_role = check_user_permission($courseid);

   
    set_item_url_handler('Resources');
    
    $view_resources = t('You do not have the required permissions.');
    $display_id = "default";
    $output = $HTMLcontainer;

    // STUDENT ACTIVITY FILTER START
    $FilterOpts = array(
            'Type' => array(
                    'All' => array(
                            'type'=>'checkbox',
                            'group'=>'type',
                            'value' => 'all',
                            'name' => 'all',
                            'default' => true,
                    ),
                    'Links' => array(
                            'type'=>'checkbox',
                            'group'=>'type',
                            'value' => 'links',
                            'name' => 0,
                    ),
                    'Images' => array(
                            'type'=>'checkbox',
                            'group'=>'type',
                            'value' => 'images',
                            'name' => 1,
                    ),
                    'Videos'=> array(
                            'type'=>'checkbox',
                            'group'=>'type',
                            'value' => 'videos',
                            'name' => 2,
                    ),
                    'Documents'=> array(
                            'type'=>'checkbox',
                            'group'=>'type',
                            'value' => 'documents',
                            'name' => 3,
                    ),
                    'Pages'=> array(
                            'type'=>'checkbox',
                            'group'=>'type',
                            'value' => 'pages',
                            'name' => 4,
                    ),
                    'Shared Ideas'=> array(
                            'type'=>'checkbox',
                            'group'=>'type',
                            'value' => 'sharedideas',
                            'name' => 5,
                    ),
            ),
    );

    $opts = render_custom_filter(
            $FilterOpts,
            array('start' => $HTML_course_resources_filter_cellStart, 'end' => $HTML_course_resources_filter_cellEnd),
            array( 'template' => $HTML_course_resources_filter_heading, 'placeholder' => '@HEADING' ),
            $HTML_course_resources_filter_row_end,
            url('ntlp/course/resources/filter/'.$courseid),
            'coursesResourcesTable'
    );

    $opts = str_replace('@OPTIONS', $opts, $HTML_course_resources_filter);
    $output = str_replace('@FILTER_PLACEHOLDER', $opts, $output);
    // STUDENT ACTIVITY FILTER END

    $view = views_get_view('ntlp_resources');
    $view_resources = "<div>Resources view</div>";

    if(isset($user_role)) {
        if($user_role == NTLP_ROLEID_TEACHER
                || $user_role == NTLP_ROLEID_NETWORKMANAGER
                || $user_role == NTLP_ROLEID_SITEADMIN
                || $user_role == NTLP_ROLEID_SCHOOLLEADER
                || $user_role == NTLP_ROLEID_STUDENT
                || $user_role == NTLP_ROLEID_NETWORKSTAFF) {

            $output = str_replace('@LINK_NEW', l('New', 'ntlp/courses/resource/new/'.$courseid), $output);

        }else if($user_role == NTLP_ROLEID_SITEGUEST) {
            $output = str_replace('@LINK_NEW', "", $output);
            
        }
    }

    if (isset($view)) {
//        $view->set_display($display_id);
//        $view->set_arguments(array($courseid));
//        $view->display['default']->handler->options['items_per_page'] = 10;
//        $view->display['default']->handler->options['display_options']['use_pager'] = TRUE;
//
//        $view_resources .= $view->render();
        $view_resources = $view->preview($display_id, array($courseid));
    }

    $output = str_replace('@RESOURCES_VIEW', '<div id="coursesResourcesTable">'.$view_resources.'</div>', $output);
    return $output;

}

function ntlp_course_resources_filter($courseid) {
    $view = views_get_view('ntlp_resources');

    $resTypes = array(
            'links' => 'lo_link',
            'images' => 'lo_image',
            'videos' => 'lo_video',
            'documents' => 'lo_document',
            'pages' => 'lo_page',
            'sharedideas' => 'lo_share_idea',
    );

    if( !isset($_REQUEST['type']['all']) and count($_REQUEST['type'])>0 and count($_REQUEST['type']) < count($resTypes) ) {
        $types = array();
        foreach( $_REQUEST['type'] as $type ) {
            $types[] = $resTypes[$type];
        }

        $view->display['default']->display_options['filters'] = array (
                'type' => array (
                        'operator' => 'in',
                        'value' => $types,
                        'table' => 'node',
                        'field' => 'type',
                ),

                'deleted_on' => array (
                        'operator' => 'IS NULL',
                        'value' => '',
                        'table' => 'ntlp_course_resource',
                        'field' => 'deleted_on',
                ),
        );
    }
    
    echo $view->preview('default', array($courseid));
}
