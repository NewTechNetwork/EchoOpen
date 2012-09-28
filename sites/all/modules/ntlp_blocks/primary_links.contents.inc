<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


function get_primary_link_html() {
    include 'primary_links.template.inc';



    $url = $_GET['q'];
    $args = explode('/', $url);

    $primary_links = '<ul id="mainNavigation" class="mainNavigation">';

    if ($args[1] == 'home'  || $args[1] == 'school') {
        $primary_links .= '<li class="first active-trail"><a title="" href="' . url('ntlp/home') . '">Home</a></li>';
    } else {
        $primary_links .= '<li class="first"><a title="" href="' . url('ntlp/home') . '">Home</a></li>';
    }

    if ($args[1] == 'courses' || $args[1] == 'admincourse') {
        $primary_links .= '<li class="active-trail"><a title="" href="' . url('ntlp/courses/my') . '">Courses</a></li>';
    } else {
        $primary_links .= '<li class=""><a title="" href="' . url('ntlp/courses/my') . '">Courses</a></li>';
    }

    if ($args[1] == 'events') {
        $primary_links .= '<li class="active-trail"><a title="" href="' . url('ntlp/events') . '">Events</a></li>';
    } else {
        $primary_links .= '<li class=""><a title="" href="' . url('ntlp/events') . '">Events</a></li>';
    }

    if ($args[1] == 'grades') {
        $primary_links .= '<li class="active-trail"><a title="" href="' . url('ntlp/grades') . '">Grades</a></li>';
    } else {
        $primary_links .= '<li class=""><a title="" href="' . url('ntlp/grades') . '">Grades</a></li>';
    }

    if ($args[1] == 'groups') {
        $primary_links .= '<li class="active-trail"><a title="" href="' . url('ntlp/groups/my') . '">Groups</a></li>';
    } else {
        $primary_links .= '<li class=""><a title="" href="' . url('ntlp/groups/my') . '">Groups</a></li>';
    }

    if ($args[1] == 'people' || $args[1] == 'user' || $args[1] == 'myaccount') {
        $primary_links .= '<li class="active-trail"><a title="" href="' . url('ntlp/people') . '">People</a></li>';
    } else {
        $primary_links .= '<li class=""><a title="" href="' . url('ntlp/people') . '">People</a></li>';
    }

    if ($args[1] == 'library' || $args[1] == 'document') {
        $primary_links .= '<li class="last active-trail"><a title="" href="' . url('ntlp/library') . '">Library</a></li>';
    } else {
        $primary_links .= '<li class="last "><a title="" href="' . url('ntlp/library') . '">Library</a></li>';
    }

    $primary_links .= '</ul>';

    return $primary_links;
}
