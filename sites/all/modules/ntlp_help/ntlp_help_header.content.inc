<?php

function ntlp_help_render_header() {
    drupal_add_css(drupal_get_path('theme', 'Boldr') . '/StylishSelect/stylish-select.css', 'module', 'all', FALSE);
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/externalFile.js');
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/jquery.stylish-select.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_blocks') . '/default_header.js');

    include "ntlp_help_header.template.inc";

    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');

    global $user, $base_path;


    $get = $_GET['q'];
    $br = explode("/",$get);
    $cnt = count($br);
    $active_tab = "@IS_".strtoupper($br[$cnt-2]);
    $group_id = $br[$cnt-1];

    $container = $HTMLcontainer;
    $container = str_replace('@LINK_HOME', l('Home', 'ntlp/home'), $container);
    $container = str_replace('@LINK_HELP', l('HELP', 'ntlp/help/faq'), $container);

    return $container;

}

function ntlp_help_render_tabs($active_tab = "NOP") {
    global $user;
    drupal_add_css(drupal_get_path('theme', 'Boldr') . '/StylishSelect/stylish-select.css', 'module', 'all', FALSE);
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/externalFile.js');
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/jquery.stylish-select.js');

    include "ntlp_help_header.template.inc";



    if ($active_tab == "NOP") {
        $get = $_GET['q'];
        $br = explode("/",$get);
        $cnt = count($br);
        $active_tab = "@IS_".strtoupper($br[$cnt-1]);

    }

    
    $output = $HTML_help_tabs;
    if( isset($user->roles[NTLP_ROLEID_SITEADMIN]) || isset($user->roles[NTLP_ROLEID_SCHOOLLEADER]) ) {
        $output = str_replace('<!-- support request -->','<li class="@IS_SUPPORT">
                <a class="@IS_SUPPORT" href="@SUPPORT_URL">Support Request</a></li>
            </li>', $output);
    }

    $output = str_replace($active_tab, "active", $output);
    $output = str_replace("@FAQ_URL", url("/ntlp/help/faq"), $output);
    $output = str_replace("@FEEDBACK_URL", url("/ntlp/help/feedback"), $output);
    $output = str_replace("@SUPPORT_URL", url("/ntlp/help/support"), $output);

    $output = str_replace("@IS_FAQ", "", $output);
    $output = str_replace("@IS_FEEDBACK", "", $output);
    $output = str_replace("@IS_SUPORT", "", $output);



    return $output;
}
?>
