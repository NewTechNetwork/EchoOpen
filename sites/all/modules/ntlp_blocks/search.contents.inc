<?php

module_load_include('inc', 'ntlp_school', 'constants');

function search_block_contents($default_search_text = "What are you looking for?") {

    global $base_path;
    include 'search.template.inc';

    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/js/jquery.tipsy.js');
//    drupal_add_js('
//    $(function(){
//	$(".clear-search").tipsy({ opacity: 1,  arrowClass : "black-arrow-modal", gravity: "ne" });
//});
//','inline');

    $output = $HTMLSearchPanel;

    $output = str_replace("@ORG_DEFAULT_SEARCH_TEXT", $default_search_text, $output);

    $search_value = $_GET['keys'];

    if (isset($search_value) && $search_value != "") {
        //$default_search_text = '<font style="color:#999">'.$search_value.'</font>';
        $default_search_text = $search_value;
        $output = str_replace('@DISPLAY_DLT_BTN', 'display:block;', $output);
    }
    else
        $output = str_replace('@DISPLAY_DLT_BTN', 'display:none;', $output);

    $output = str_replace("@DRUPAL_BASE", $base_path, $output);
    $output = str_replace("@DEFAULT_SEARCH_TEXT", $default_search_text, $output);
    $output = str_replace("@SEARCH_VALUE", "value='$search_value'", $output);
    $output = str_replace("@PAGE_URL", $_GET['q'], $output);

    return $output;
}

