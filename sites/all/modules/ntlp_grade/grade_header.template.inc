<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTMLcontainer = t(<<<END
<div>
    <div class="BoldrBrandCrumb clearfix" style="width:840px;">
    <ul>
        <li>@LINK_HOME >&nbsp;</li>
        <li>@GRADE >&nbsp;</li>
 
    </ul>
    </div>
    <!-- selectors_and_heading -->
   <br/>
</div>

END
);

$HTMLSelectorsAndHeading = '
    <div class="clearfix" style="width:100%;">
        
        <div style="width:450px; float:left;">
            <h1 class="pageHeading"><!-- heading --></h1>
        </div>
        <div style="">
            <!-- selectors -->
        </div>
    </div>';

$HTMLTermSelectorCombo = '
    <div style="float:right;margin-left:1em;margin-right:1em">
            <span style="float: left; line-height: 20px; font-size: 14px; color: #5E8FAD;">Term:&nbsp;</span>
            <!-- term_selector_combo -->

    </div>';

$HTMLSchoolYearCombo = '
    <div style="float:right;margin-left:1em;margin-right:1em">
            <span style="float: left; line-height: 20px; font-size: 14px; color: #5E8FAD;">Year:&nbsp;</span>
            <span style="width:100px;" class="comboboxBgLeft">
            <!-- school_year_combo -->
            </span>
    </div>';

$HTMLStudentSelectorCombo = '
    <div style="float:right;margin-left:1em;margin-right:1em">
            <span style="float: left; line-height: 20px; font-size: 14px; color: #5E8FAD;">Student:&nbsp;</span>
            <span style="width:310px;" class="comboboxBgLeft">
            <!-- student_selector_combo -->
            </span>
    </div>';

/*
 * Grade Tabs
 *
*/


$HTML_grade_tabs = t(<<<END

<div class="Agenda">
   <!-- GRADES_TOOLS_NEW -->
    <div class="AgendaMenu">
        <ul>
           @GRADEBOOK_TAB
           @STUDENTS_TAB
             
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


$HTMLGradebook_yellow_bar= t(<<<END
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