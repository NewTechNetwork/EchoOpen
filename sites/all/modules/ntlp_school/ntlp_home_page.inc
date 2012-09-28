<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'reports');

/* Form To  Create New User In School Management START */

function ntlp_home_page() {

    if (!user_is_logged_in()) {
        return "";
    }
    
$HTML_homepage = t(<<<END
    <table cellspacing="0" cellpadding="0" border="0" width="100%" >
    <tr><td><table cellspacing="0" cellpadding="0" border="0" width="100%">
        <tr valign="top"><td id ="recent_events_block_contents">@RECENT_EVENTS_BLOCK</td>
        <td id ="recent_groups_block_contents">@RECENT_GROUPS_BLOCK</td></tr>
    </table></tr>
    <tr><td><table cellspacing="0" cellpadding="0" border="0" width="100%">
        <tr><td id="mycourses_block_contents">@MYCOURSES_BLOCK</td><td valign="top">
        <table><tr><td id="_ntlp_home_page_feature_content">@FEATURED_RESOURCE_BLOCK</td></tr>
        <tr><td>@RECENT_POLL_BLOCK</td></tr></table>
    </td></tr></table></td></tr>
    <tr><td><div class="mailNameHome">@GOOGLE_APPS_BLOCK</div></td></tr>
    </table>
END
);

    $block = module_invoke('poll', 'block', 'view', '0');
        
    $html = str_replace('@RECENT_EVENTS_BLOCK', ntlp_block_cache('recent_events_block_contents', 'ntlp_event'), $HTML_homepage);
    $html = str_replace('@RECENT_GROUPS_BLOCK', ntlp_block_cache('recent_groups_block_contents', 'ntlp_group'), $html);
    $html = str_replace('@MYCOURSES_BLOCK', ntlp_block_cache('mycourses_block_contents', 'ntlp_course'), $html);
    $html = str_replace('@FEATURED_RESOURCE_BLOCK', _ntlp_home_page_feature_content(), $html);
    $html = str_replace('@RECENT_POLL_BLOCK', $block['content'], $html);
    $html = str_replace('@GOOGLE_APPS_BLOCK', get_gapps_contents_for_home(), $html);
    return $html;
}

function render_gmail_block_content(){

    print get_gmail_block_contents();
}

function render_gdocs_block_content(){

    print get_gdocs_block_contents();
}

function render_gsites_block_content(){

    print get_gsites_block_contents();
}