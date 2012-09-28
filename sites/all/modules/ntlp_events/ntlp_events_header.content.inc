<?php

function ntlp_events_render_header() {
    drupal_add_css(drupal_get_path('theme', 'Boldr') . '/StylishSelect/stylish-select.css', 'module', 'all', FALSE);
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/externalFile.js');
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/jquery.stylish-select.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_blocks') . '/default_header.js');

    include "ntlp_events_header.template.inc";

    global $user, $base_path;

    $container = $HTMLcontainer;

    $container = str_replace('@LINK_TOOL', l('Home', 'ntlp/home'), $container);

    return $container;

}
