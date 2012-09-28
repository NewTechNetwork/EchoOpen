<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'constants');

function course_add_contents() {

    global $base_path;
    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');

    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/js/jquery.tipsy.js');
    drupal_add_js('
        $(function (){
            $(".addnew").tipsy({
            className : "grey-middle",
            arrowClass : "grey-arrow",
            delayIn: 0,
            delayOut: 0,
            fade: false,
            fallback: "",
            html: true,
            live: false,
            offset: 0,
            opacity: 1,
            title: function(){
                    return $(".addnewActivity").html();
                },
            trigger: "manual"
                });
        });
        
function show_addnew_tipsy(){

    $(".addnew").tipsy("show");
    setTimeout(\'$(".addnew").tipsy("hide");\' , 5000);
    return false;

}
', inline);

    if ($courseid == 0) {
        //Getting Query String From URL
        $courseid = 0;
        $url = $_GET['q'];
        $args = explode('/', $url);
        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i])) {
                $courseid = $args[$i];
                break;
            }
        }
    }


    $user_role = check_user_permission($courseid);

    if ($user_role != NTLP_ROLEID_STUDENT && $user_role != NTLP_ROLEID_SITEGUEST) {
        $html = '
<div id="ActivitiesCSS" style="margin-top:10px;margin-right:5px; width:auto !important;">
    <div class="Filter">
       <div class="yellowTopRightCorner">
        <div class="yellowBottomRightCorner">
          <div class="yellowBottomLeftCorner">
            <div class="yellowTopLeftCorner">
              <br>
              <div class="content2" style="margin-bottom: 10px; margin-top: -18px;">
                <div style="margin-top: 9px;">
                    <div class="blue_heading"> Add Content </div>
                  </div>
                  <br />
                  <div class="addcontent_panel">
                    <span class="plus_sign">+&nbsp;</span><a href="@AGENDA_EDIT_LINK">New Agenda</a>
                  </div>
                  <div class="addcontent_panel">
                    <span class="plus_sign">+&nbsp;</span><a href="@NEW_PROJECT_LINK">New Project</a>
                  </div>
                  <div class="addcontent_panel">
                    <span class="plus_sign">+&nbsp;</span><a  title="sfsdfsdfsd" onclick="show_addnew_tipsy();"  class="addnew">New Activity</a>
                         <div class="addnewActivity" style="display: none;">
                             <div style="margin-bottom:5px;"><a class="tipsygreenlinks"  href="' . url('ntlp/courses/activity/new/' . $courseid) . '&type=lo_task">Task</a></div>

                             <div style="margin-bottom:5px;"><a class="tipsygreenlinks"  href="' . url('ntlp/courses/activity/new/' . $courseid) . '&type=lo_journal">Journal</a></div>
                             <div style="margin-bottom:5px;"><a class="tipsygreenlinks"  href="' . url('ntlp/courses/activity/new/' . $courseid) . '&type=lo_workshop">Workshop</a></div>
                         </div>
                  </div>
                  <div class="addcontent_panel">
                    <span class="plus_sign">+&nbsp;</span><a href="@NEW_TOPIC_LINK">New Discussion Topic</a>
                  </div>
                </div>
            </div></div></div></div></div> </div>
';




        $date = get_tz_course($courseid, 'Y/m/d', date('Y/m/d H:i:s', time()));

        $year = date("Y", strtotime($date));
        $month = date("m", strtotime($date));
        $day = date("d", strtotime($date));

        $html = str_replace("@AGENDA_EDIT_LINK", url("ntlp/courses/agenda/$courseid/$year/$month/$day/1"), $html);
        $html = str_replace('@NEW_PROJECT_LINK', url('ntlp/courses/project/setting/' . $courseid . '/0'), $html);
        $html = str_replace('@NEW_ACTIVITY_LINK', url('ntlp/courses/activity/new/' . $courseid), $html);
        $html = str_replace('@NEW_TOPIC_LINK', url('ntlp/courses/forum/topic/' . $courseid . '/0'), $html);
    }
    return $html;
}

