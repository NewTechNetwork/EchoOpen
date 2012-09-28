<?php

//Prepare url for getting contents for Module header
//Module Header: The area displayed about the white body.
//This includes:
//  Breadcrumbs
//  Course/Group/Project title
//  Related links
//  School and Term Selector
//  Tabs or Tab placeholder

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

    echo $module_path;
    $fn = find_module_render_function(substr($module_path, 0, -1), '_render_header'); //Remove last '_' before passing
    $print_r($fn);
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
    watchdog('fn', "$module_path, $suffix");
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

?>
