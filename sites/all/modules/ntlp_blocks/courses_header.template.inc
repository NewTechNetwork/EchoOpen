<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTMLcontainer = t(<<<END
<div>
    <div class="BoldrBrandCrumb">
        <ul>
            <li>@LINK_HOME >&nbsp;</li>
            <li>@LINK_MYCOURSES >&nbsp;</li>
        </ul>
    </div>

    <br clear="all" />
    
    <!-- selectors_and_heading -->
    <div class="BoldrHeading" >
    	@COURSE_TOOLS
    </div>
</div>
<!-- term_selector -->

END
);

$HTMLSelectorsAndHeading = '
    <div style="width:100%;" class="clearfix">
       <div style="float:left; width:480px;">
            <h1 class="pageHeading" style=""><!-- heading --> </h1>
         </div>
        <div style="float:right;" class="clearfix">
            <!-- selectors -->
        </div>
    </div>';

$HTMLSchoolYearCombo = '
    <div style="float:right; margin-right:1em">
           <span style="float: left; line-height: 20px; font-size: 14px; color: #5E8FAD;">Year:&nbsp;</span> 
            <span style="width:100px;" class="comboboxBgLeft">
            <!-- school_year_combo -->
            </span>
    </div>';
$HTMLTermSelectorCombo = '
    <div style="float:right;margin-left:1em;margin-right:1em">
            <span style="float: left; line-height: 20px; font-size: 14px; color: #5E8FAD;">Term:&nbsp;</span>
            <!-- term_selector_combo -->
    </div>';


$HTMLStudentSelectorCombo = '
    <div style="float:right;margin-left:1em;margin-right:1em">
            <span style="float: left; line-height: 20px; font-size: 14px; color: #5E8FAD;">Student:&nbsp;</span>
            <span style="width:310px;" class="comboboxBgLeft">
            <!-- student_selector_combo -->
            </span>
    </div>';


//Old version
//<h1>@COURSE_NAME<span>&nbsp;@COURSE_PERIOD (<label id="course_term">@COURSE_TERM</label>) | @COURSE_TEACHER</span>

$HTMLcourse_tools = t(<<<END
<div style="float:left;">
<h1>@COURSE_NAME<span>&nbsp;@COURSE_PERIOD | @COURSE_TEACHER</span></h1>
</div>
END
);

//if($user->roles[6] == 'Staff'){
//$HTMLcourse_tools .= t(<<<END
//<div style="float:right"><span style="width:150px;position:relative" class="comboboxBgLeft">
//        <select class="my-dropdown">
//            <option value="1">Edit Course Settings</option>
//            <option value="2">&lt;a href=&quot;?q=nav/courses/edit/@CID&quot;&gt;Delete Course&lt;/a&gt;</option>
//            <option value="3">&lt;a href=&quot;?q=nav/courses/edit/@CID&quot;&gt;Copy Course&lt;/a&gt;</option>
//        </select><a href="?q=nav/courses/edit/@CID" class="comboBoxLink"></a>
//    </span></div>
//END
//);
//}
$HTMLcourse_manage = t(<<<END
<div style="float:right;margin:0 5px 0 0">
    <div class="anchorBaseCombobox">
    <div id="btnManageCourse"></div>
    <div id="manageCourse" class="hideMODrpDown" style="display:none">
    <div class="hideMODrpDownInnerBg">
            <ul>
                <li class="ManageOption">@MANAGE_COURSE_EDIT</li>
                <li class="ManageOption">@MANAGE_COURSE_COPY</li>
                <li class="ManageOption">@MANAGE_COURSE_DELETE</li>
            </ul>
        </div>
    </div>

    </div>
</div>
END
);

//-----------------------------------------------------------------------------
$HTMLCourseHome_tabs = t(<<<END

<div class="Agenda">
<!-- TERM_SELECTOR -->
    <div class="AgendaMenu">
        <ul>

            @COURSE_HOME_TAB
            @COURSE_AGENDA_TAB
            @COURSE_PROJECTS_TAB
            @COURSE_GRADE_TAB
            @COURSE_ACTIVITIES_TAB
            @COURSE_DISCUSSION_TAB
            <!-- @COURSE_RESOURCES_TAB -->
            
        </ul>
        <div class="AgendaRightShade">&nbsp;</div>
    </div>
    <div class="AgendaContents">
        <table border="0" cellpadding="0" cellspacing="0" style="width: 100%">
        <tbody>
            <tr>
                <td class="AgendaMainContentAreaMiddleLeftCorner">&nbsp;</td>
                <td class="AgendaMainContentAreaMiddleCenterCorner">&nbsp;</td>
                <td class="AgendaMainContentAreaMiddleRightCorner">&nbsp;</td>
            </tr>
        </tbody>
        </table>
    </div>
</div>

END
);

$HTMLCourse_tabs= t(<<<END

<div class="Agenda">
<!-- CREATE_COURSE -->
 
    <div class="AgendaMenu">
        <ul>
            @COURSE_TEACHING_TAB
            @COURSES_TRAINING_TAB
            <!-- @COURSES_OPEN_TAB -->
            @COURSES_ALL_TAB
        </ul>
        <div class="AgendaRightShade">&nbsp;</div>
        
        

    </div>
    <div class="AgendaContents">
        <table border="0" cellpadding="0" cellspacing="0" style="width: 100%">
        <tbody>
            <tr>
                <td class="AgendaMainContentAreaMiddleLeftCorner">&nbsp;</td>
                <td class="AgendaMainContentAreaMiddleCenterCorner">&nbsp;</td>
                <td class="AgendaMainContentAreaMiddleRightCorner">&nbsp;</td>
            </tr>
        </tbody>
        </table>
    </div>
</div>

END
);


$HTMLCourse_yellow_bar= t(<<<END
       <div id="BoldrProject" style="margin-bottom:10px;">
      <div class="ProjectResources" style="width:848px !important;">
        <div class="ProjectResources-Content" style="padding:0px">
          <div class="ProjectResources-Activities">
        	 <div class="Project-ActivitiesFilter" style="padding:0px;">
              <div class="ProjectResources-YellowBottomRightCorner">
                <div class="ProjectResources-YellowBottomLeftCorner">
                  <div class="ProjectResources-YellowTopRightCorner">
                    <div class="ProjectResources-YellowTopLeftCorner" style="padding: 5px;">
                        <div class="">
                        <table width="100%" border="0">
                          <tr>
                            <!--VIEW_COURSES_BY--> <!--SCHOOL--> 
                            <td><!--PARENT_CHILD_SELECTOR--></td>
                            <td><div><strong>School Year: </strong><!--SCHOOL_YEAR--></div> </td>
                            <td><div><strong>Term: </strong><!--SCHOOL_TERM--></div></td>
                          </tr>
                        </table>
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
