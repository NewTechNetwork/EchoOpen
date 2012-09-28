<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


function boldr_theme() {
    return array(
            'user_login_block' => array(
                    'template' => 'user-login-block',
                    'arguments' => array('form' => NULL),
            )
    );
}

function phptemplate_menu_local_tasks() {
    $output = '';
    $node = new stdClass();
    if (arg(0) == 'node' && is_numeric(arg(1))) {
        $node = node_load(arg(1));
        if($node->type == 'faq') {
            return '';
        }
    }

    if ($primary = menu_primary_local_tasks()) {
        $output .= "<ul class=\"tabs primary\">\n". $primary ."</ul>\n";
    }
    if ($secondary = menu_secondary_local_tasks()) {
        $output .= "<ul class=\"tabs secondary\">\n". $secondary ."</ul>\n";
    }

    return $output;
}

function phptemplate_node_form($form) {
    switch ($form['#node']->type) {
        case 'forum':
//            watchdog('themer', 'Path:'.path_to_theme().'/node-add-forum.tpl.php');
//            return theme_node_form($form);
            return theme_render_template(path_to_theme().'/node-add-forum.tpl.php', array('form' => $form));
//        case 'lo_image':
//            echo 'lo image loaded';
//        case 'lo_video':
//            echo 'log video loaded';
//        case 'lo_page':
//            echo 'log video loaded';
//        case 'lo_link':
//        case 'lo_document':
////            watchdog('themer', 'Path:'.path_to_theme().'/node-add-forum.tpl.php');
////            return theme_node_form($form);
//            return theme_render_template(path_to_theme().'/node-add-forum.tpl.php', array('form' => $form));
        default:
            
            return theme_node_form($form);
    }
}
//Prepare url for getting contents for Module header
//Module Header: The area displayed about the white body.
//This includes:
//  Breadcrumbs
//  Course/Group/Project title
//  Related links
//  School and Term Selector
//  Tabs or Tab placeholder
function Boldr_preprocess_page(&$variables) {
//    if ($variables['node']->type == "my_content_type") {
//    $variables['template_files'][] = 'page-node-my_content_type';
    $variables['ntlp_module_header'] = get_module_header();
    $variables['ntlp_module_tabs'] = get_module_tabs();
//    print_r($variables);
//    $node = $variables['node'];
//    if ( $node && $node->type == 'forum') {
//
//        // if the node type is forum load the page-forum.tpl.php layout
//        $variables['template_files'][] = 'page-ntlp-courses-forum-view';
//    }
}

function get_module_header() {
    $module_path = "";
    $url = $_GET['q'];
    $args = explode('/', $url);
    for ($i = 0; $i < count($args); $i++) {
        if (is_numeric($args[$i])) {
            break;
        } else {
            $module_path .= ($args[$i] . '_');
        }
    }
    $fn = find_module_render_function(substr($module_path, 0, -1), '_render_header'); //Remove last '_' before passing
    $module_header = "";
    if (!empty($fn)) {
        $module_header = call_user_func($fn);
    }
    return $module_header;
}

function get_module_tabs() {
    $module_path = "";
    $url = $_GET['q'];
    $args = explode('/', $url);
    for ($i = 0; $i < count($args); $i++) {
        if (is_numeric($args[$i])) {
            break;
        } else {
            $module_path .= ($args[$i] . '_');
        }
    }
    $fn = find_module_render_function(substr($module_path, 0, -1), '_render_tabs'); //Remove last '_' before passing
    $module_tabs = "";
    if (!empty($fn)) {
        $module_tabs = call_user_func($fn);
    }
    return $module_tabs;
}

///-----------------------------------------------------------------------
function find_module_render_function($module_path, $suffix) {
//    watchdog('fn', "$module_path, $suffix");
    if (function_exists($module_path.$suffix)) {
//        call_user_func($module_path .$suffix);
        return $module_path.$suffix;
    } else {
        //Remove portion after last '_'
        $new_path = reverse_strrchr($module_path, "_");
        if ($new_path) {
            return find_module_render_function($new_path, $suffix);
        }
    }
    return "default".$suffix;
}

function reverse_strrchr($haystack, $needle) {
    $pos = strrpos($haystack, $needle);
    if($pos === false) {
        return false;
    }
    return substr($haystack, 0, $pos);
}
