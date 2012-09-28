<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTMLreport_buttons = t(<<<END
<div style="float:right;margin:0 5px 0 0">
    <div class="anchorBaseCombobox">
    <div id="btnReports"></div>
    <div id="studentCourseReport" class="hideMODrpDown" style="display:none">
    <div class="hideMODrpDownInnerBg">
            <ul>
                <li class="ManageOption">@STUDENT_COURSE_REPORT</li>
                <li class="ManageOption">@STUDENT_COURSE_DETAILED_REPORT</li>
            </ul>
        </div>
    </div>

    </div>
</div>
END
);

//$HTMLoutcome_box = t(<<<END
//    <div class="Filter">
//            <div class="yellowTopRightCorner">
//                    <div class="yellowBottomRightCorner">
//                            <div class="yellowBottomLeftCorner">
//                                    <div class="yellowTopLeftCorner">
//                                            <table cellpadding="0" cellspacing="0" width="60%" border="0" >
//                                            @OUTCOME_LINES
//                                            </table>
//                                    </div>
//                            </div>
//                    </div>
//            </div>
//    </div>
//END
//);

$HTMLoutcome_line = t(<<<END
  <tr>
    <td><div class="innertableMainDiv" style="float:left">
        <div class="percentHeading">
          <h4>@OUTCOME_NAME
          <span style="font-size: 11px; font-weight: normal;">@OUTCOME_ACTIVITY_COUNT</span>
        </div>
        <br clear="all" />
        <div class="percentlistbg">
          <div class="percentlistRightbg">
            <div class="percentlistLeftbg">
              <div style="width:@OUTCOME_GRADE_PERCENT%;" class="percentlist"> </div>
            </div>
          </div>
          <div class="percentlisttext"> @OUTCOME_GRADE </div>
        </div>
      </div></td>
  </tr>
END
);

$HTMLcontainer = t(<<<END
   <div id="dialog-show-confirmation" style="display: none;" title="Warning">
                        <span class="msg">Are you sure you want to submit the form?</span>
                    </div>
<div id="Courses-Grades">
  <div class="mid-right-blueCorner">
    <div class="mid-left-blueCorner">
      <div class="bottom-center-blueCorner">
        <div class="bottom-right-blueCorner">
          <div class="bottom-left-blueCorner">
            <div class="top-right-blueCorner">
              <div class="top-left-blueCorner">
                <div class="StudentGradeMainPanel">
                  <div class="student">
                    @STUDENT_BOX
                    <div class="StudentDetail">
                      <table cellpadding="0" width="100%" cellspacing="0" border="0">
                        <tr>
                          <td valign="top" style="border-bottom:1px dotted #9ecce9" class="GradeBlock" width="173px" rowspan="6">
                            <div class="whitebg">
                              <div align="center" class="current-grade">
                                <h2 style="color:#3660a6;font-size:16px;">@HEADING</h2>
                              </div>
                              @COURSE_GRADE_DATA
                            </div></td>
                          <td valign="top">
                            <table width="100%" class="innertable" cellpadding="0" cellspacing="0" border="0">
                                @OUTCOME_LINES
                            </table>
                          </td>
                        </tr>
                      </table>
                    </div>
                  </div>
                </div>
              </div>
            </div>
            <div class="ActivitiesPanel">
              <div class="GradesTabCenter" style="width:100%">
                <div style="float:left;white-space:nowrap">
                  <ul>
                    <li>@TAB_STUDENTS</li>
                    <li>@TAB_SEPARATOR</li>
                    <li>@TAB_ACTIVITIES</li>
                  </ul>
                </div>
                <div style="float:right;display:block">
                    @DOWNLOAD_COURSE_PDF
                </div>
              </div>
              <div style="font-size:10px;">&nbsp;</div>
              <div id="gradebook_tab">
              
              @GRADES_VIEW
              </div>
              <div style="font-size:5px;">&nbsp;</div>
              <br clear="all" /><br clear="all" />
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>
</div>
END
);


//                              <div align="center" class="grade">@STUDENT_GSCALE</div>
//                              <div align="center" class="percent">@STUDENT_GRADE</div>
$HTMLstudent_box = t(<<<END
    <div class="StudentHeading">
      <div>
        <div style="float:left"> <img width="50" height="50" src="@USER_PICTURE_PATH" alt="Student" /> </div>
        <div style="float:left">
          <h2> @STUDENT_NAME </h2>
        </div>
        <div style="float:right;display:block">
        @REPORT_STUDENT_PDF
        </div>

      </div>
    </div>
END
);


$HTML_grade_school_filter_heading = t(<<<EOD
        <tr><td class="StatusHeading" style="white-space:nowrap;width:150px;">@HEADING</td><td>
EOD
);
$HTML_grade_school_filter_row_end = t(<<<EOD
        </td></tr>
EOD
);
$HTML_grade_school_filter_cellStart = t(<<<EOD
        <div class="ChkBox">
EOD
);
$HTML_grade_school_filter_cellEnd = t(<<<EOD
        </div>
EOD
);

$HTML_GRADEBOOK_FILTER = t(<<<END
   <div style="margin-top: 8px; width: 100%;" id="BoldrProject">
      <div style="width: auto;" class="ProjectResources">
        <div style="padding: 0px;" class="ProjectResources-Content">
          <div class="ProjectResources-Activities">
        	 <div style="padding: 0px;" class="Project-ActivitiesFilter">
              <div style="width: auto;" class="ProjectResources-YellowBottomRightCorner">
                <div class="ProjectResources-YellowBottomLeftCorner">
                  <div class="ProjectResources-YellowTopRightCorner">
                    <div class="ProjectResources-YellowTopLeftCorner">
                      <table cellspacing="0" cellpadding="0" border="0" width="100%">
                        <tbody><tr>
                          <td><div class="Project-ActivitiesFilterHeading">
                              <h4>
                                Filter
                              </h4>
                            </div>
                            <div class="Project-ActivitiesFilterCloseLink">
                              <a class="filter_link">&laquo; Close</a>
                            </div></td>
                        </tr>
                        <tr>
                          <td>
                            <table cellspacing="0" cellpadding="0" border="0" width="100%" class="Project-ActivitiesStatusType filter_hide">
                        <tbody><tr>
                            <td>
                                <div>
                                    <div class="clearall"></div>
                                        
                                </div>
                                @OPTIONS
                                 
                            </td>
                         </tr>
                         </tbody></table>
                          </td>
                        </tr>
                      </tbody></table>
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