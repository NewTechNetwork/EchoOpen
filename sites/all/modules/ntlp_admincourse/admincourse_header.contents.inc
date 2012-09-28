<?php
module_load_include('inc', 'ntlp_blocks', 'courses_header.contents');

//This method is called from themes/Boldr/module_header.inc
function ntlp_admincourse_render_header() {
    return ntlp_courses_render_header();
}


//This method is called from themes/Boldr/module_header.inc
//Displays no tabs
function ntlp_admincourse_render_tabs($active_tab = "NOP", $course_nid = 0) {
    return ntlp_courses_render_tabs($active_tab, $course_nid);
}

//This method is called from themes/Boldr/module_header.inc
function ntlp_courses_edit_render_header() {
    return ntlp_courses_render_header();
}


//This method is called from themes/Boldr/module_header.inc
//Displays no tabs
function ntlp_courses_edit_render_tabs($active_tab = "NOP", $course_nid = 0) {
    return ntlp_courses_render_tabs($active_tab, $course_nid);
}

?>
