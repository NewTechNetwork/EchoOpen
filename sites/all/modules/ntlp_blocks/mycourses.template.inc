<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTMLnewlink = t(<<<END
<div class="Project-Btns">
    @LINK_NEW
</div>
END
);

$HTMLcoursebox = t(<<<END
<div class="CoursesContentInnerItems">
  <!-- Courses Inner Heading -->
  <div class="CoursesInnerHeadingBg">
    <div class="CoursesInnerHeadingBgLeftCorner">
      <div>
        <div class="CoursesInnerPeriodsHeading">
          <div>@COURSE_PERIOD&nbsp;</div>
        </div>
        <div class="CoursesInnerTabs">
          <div class="CoursesInnerTabsLeftBg"></div>
          <div class="CoursesInnerTabsCenterBg">
            <div>@COURSE_GRADEBOOK</div>
          </div>
          <div class="CoursesInnerTabsRightBg"></div>
        </div>
      </div>
      <div>
        <div class="CoursesInnerDataHeading" style="float:left;text-overflow:ellipsis;
	overflow:hidden;
	white-space:nowrap;
	width: 163px;
	padding: 0px 0 0 5px;">@COURSE_NAME</div>
	<div style="clear:both"></div>
        <div class="CoursesInnerDataActivitiesAndItems">
            <b>@COUNT_ACTIVITY</b>&nbsp;&nbsp;activities to be graded
            <span>&nbsp;&nbsp;|&nbsp;&nbsp;</span>
            <b>@RESORUCES_NEW</b>&nbsp;&nbsp;new items shared
        </div>
      </div>
    </div>
  </div>
  <br clear="all" />
  <!-- Courses Inner Content -->
  <div class="CoursesInnerDataContentBottomCornerleft">
    <div class="CoursesInnerDataContentBottomCornerright">
      <div class="CoursesInnerDataContentFieldTrip"><div class="CoursesInnerDataContentFieldTripInner"><div class="title">@ANNOUNCEMENT</div></div></div>
      <div class="dottedLine">&nbsp;</div>
      <div class="CoursesInnerDataContentAnnouncement">@AGENDA_TITLE</div>
    </div>
  </div>
</div>
END
);

$HTMLcourseboxTaking = t(<<<END
<div class="CoursesContentInnerItems">
  <!-- Courses Inner Heading -->
  <div class="CoursesInnerHeadingBg">
    <div class="CoursesInnerHeadingBgLeftCorner">
      <div>
        <div class="CoursesInnerPeriodsHeading">
          <div>@COURSE_PERIOD&nbsp;</div>
        </div>
        <div class="CoursesInnerTabs">
          <div class="CoursesInnerTabsLeftBg"></div>
          <div class="CoursesInnerTabsCenterBg">
            <div>@COURSE_GRADEBOOK</div>
          </div>
          <div class="CoursesInnerTabsRightBg"></div>
        </div>
      </div>
      <div>
        <div class="CoursesInnerDataHeading">@COURSE_NAME</div>
        <div class="CoursesInnerDataActivitiesAndItems">
            <b>@COUNT_ACTIVITY</b>&nbsp;&nbsp;upcoming activities
            <span>&nbsp;&nbsp;|&nbsp;&nbsp;</span>
            <b>@RESORUCES_NEW</b>&nbsp;&nbsp;new items shared
        </div>
      </div>
    </div>
  </div>
  <br clear="all" />
  <!-- Courses Inner Content -->
  <div class="CoursesInnerDataContentBottomCornerleft">
    <div class="CoursesInnerDataContentBottomCornerright">
      <div class="CoursesInnerDataContentFieldTrip"><div class="CoursesInnerDataContentFieldTripInner">@ANNOUNCEMENT</div></div>
      <div class="dottedLine">&nbsp;</div>
      <div class="CoursesInnerDataContentAnnouncement">@AGENDA_TITLE</div>
    </div>
  </div>
</div>
END
);



$HTMLcourse_placeholder = t(<<<END
    <div class="CoursesEmptyMain">
	<div class="CoursesEmptyBottomRightCorner">
    	<div class="CoursesEmptyTopRightCorner">
        	<div class="CoursesEmptyBottomLeftCorner">
            	<div class="CoursesEmptyTopLeftCorner">
                <table cellpadding="5" cellspacing="0" align="center" border="0" height="125px"><tr><td>
                    <div>"What is now proved was once only imagined."</div>
                    <div>- William Blake</div>
                </td></tr></table>
                </div>
            </div>
        </div>
    </div>
    </div>
END
);

$HTMLcourse_banner_placeholder = t(<<<END
    <div class="CoursesEmptyMain">
	<div class="CoursesEmptyBottomRightCorner">
    	<div class="CoursesEmptyTopRightCorner">
        	<div class="CoursesEmptyBottomLeftCorner">
            	<div class="CoursesEmptyTopLeftCorner">
                @BANNER_PLACEHOLDER
                </div>
            </div>
        </div>
    </div>
    </div>
END
);


$HTMLact = t(<<<END
    @ACTIVITY_NAME<br />
END
);

$HTMLCoursesTabBtn = t(<<<EOD
    <li>@TAB_BUTTON</li>
EOD
);

$HTMLCoursesTabBtnSeparator = t(<<<EOD
    <li>|</li>
EOD
);

$HTMLcontainer = t(<<<END
@LINK_NEW
<div class="CoursesMainDiv " style="position: relative;">

        <div>
           <div class="CoursesHeading">
            <div class="heading">My Courses</div>
          </div>
              

                        @TAB_PLACEHOLDER
                    
         


</div>

        <!-- Courses Content -->
        <div class="ContentbottomLeftCorner">
          <div class="ContentbottomRightCorner">
                <div id='mycourses-contents'>@COURSE_BLOCKS</div>
          </div>
        </div>
</div>
END
);



/*
 *
 *  Course Tabs
*/


$get=$_GET['q'];
$br= explode("/",$get);
$cnt=count($br);

# Getting Page Name From URl like event,node...
$target=$br[$cnt-1];
 
$HTML_course_tab .=t(<<<END
 <div class="Agenda">
<div class="AgendaMenu">
<ul>

END
);

if($target=="teachcourse") {

    $HTML_course_tab .=t(<<<END

    <li class="active">
    <a class="active" href="?q=ntlp/school/teachcourse">Courses | Teach </a></li>
END
    );



}  else {


    $HTML_course_tab .=t(<<<END

         <li>
    <a href="?q=ntlp/school/teachcourse">Courses | Teach </a></li>



END
    );

}

$HTML_course_tab .=t(<<<END

          <li>


END
);


if($target=="trainingcourse") {

    $HTML_course_tab .=t(<<<END
</li>
<li class="active">
    <a class="active" href="?q=ntlp/school/trainingcourse">Training Courses</a></li>


END
    );

}  else {

    $HTML_course_tab .=t(<<<END

      <li>
    <a href="?q=ntlp/school/trainingcourse">Training Courses</a></li>
<li>


END
    );

}


if($target=="allcourses") {

    $HTML_course_tab .=t(<<<END
</li>
<li class="active">
    <a class="active" href="?q=ntlp/school/allcourses">All Courses</a></li>


END
    );

}  else {

    $HTML_course_tab .=t(<<<END

      <li>
    <a href="?q=ntlp/school/allcourses">All Courses</a></li>
<li>


END
    );

}


$HTML_course_tab .=t(<<<END
</ul>
<div class="AgendaRightShade">
    &nbsp;</div>
</div>
<div class="AgendaContents">
    <table border="0" cellpadding="0" cellspacing="0" width="100%">
        <tbody>
            <tr>
                <td class="AgendaMainContentAreaMiddleLeftCorner">
                    &nbsp;</td>
                <td class="AgendaMainContentAreaMiddleCenterCorner">
                    &nbsp;</td>
                <td class="AgendaMainContentAreaMiddleRightCorner">
                    &nbsp;</td>
            </tr>
        </tbody>
    </table>
</div>
</div>

END
);

$HTML_main_courses_tab_table_corner_start = t(<<<END
<div class="student_Panel">
    <div id="ActivitiesCSS" style="width:auto">
        <div class="BlueBorderBgTopCenter">
            <div class="BlueBorderBgMiddleRight">
                <div class="BlueBorderBgBottomCenter">
                    <div class="BlueBorderBgMiddleLeft">
                        <div class="BlueBorderBgTopRightCorner">
                            <div class="BlueBorderBgBottomLeftCorner">
                                <div class="BlueBorderBgBottomRightCorner">
                                    <div class="BlueBorderBgTopLeftCorner">
END
);

$HTML_main_courses_tab_table_corner_end = t(<<<END
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
END
);
?>
