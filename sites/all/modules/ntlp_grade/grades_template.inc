<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTMLoutcome_line = t(<<<END
  <tr>
    <td><div class="innertableMainDiv">
        <div class="percentHeading">
          <h4>@OUTCOME_NAME</h4>
        </div>
        <div class="percentlistbg" style="width:450px">
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
<div id="Courses-Grades">
  
                <div class="StudentGradeMainPanel" style="padding:0px;">
                <br clear="all" />
                  <div style="float:left" class="HeadingAndEditBtn"><div class="Heading"><h3 style="font-size:15px">School Wide Learning Outcomes</h3></div></div>
                  <br clear="all" />
                    <div class="StudentDetail">
                      <table cellpadding="0" style="border:none" width="815px" cellspacing="0" border="0">
                        <tr>
                        <td style="border:none" valign="top">
                          <table width="100%" class="innertable" cellpadding="0" cellspacing="0" border="0">
                           @OUTCOME_LINES
                          </table></td>
                        </tr>
                      </table>
                    </div>
                  </div>
                
</div>
END
);

$HTMLstudent_box = t(<<<END
                    <div class="StudentHeading">
                      <div>
                        <div style="float:left"> <img src="/themes/Boldr/Images/girlImg.jpg" alt="Student" /> </div>
                        <div style="float:left">
                          <h2> @STUDENT_NAME </h2>
                        </div>
                      </div>
                    </div>
END
);


$HTMLmycourse_gradebook_box = t(<<<END
<div> @SCHOOL_YEAR </div>

<div>
<table cellpadding = "0" cellspacing = "0" width = "100%">
<tr>
    <th colspan= "2" align="left"> Spring (Active)</th>

 </tr>
 <tr>
    <th align="left"> Period</th>
    <th align="left"> Title</th>
 </tr>
 @COURSES
</table>
</div>
END
);
$HTMLmycourse_gradebook_box = t(<<<END
<div>

     @SPRING_COURSES
     
</div>

END
);

$HTMLgrade_student_box = t(<<<END

<div class="student_Panel">
    <div id="ActivitiesCSS" style="width:844px">
      <div class="BlueBorderBgTopCenter">
        <div class="BlueBorderBgMiddleRight">
          <div class="BlueBorderBgBottomCenter">
            <div class="BlueBorderBgMiddleLeft">
              <div class="BlueBorderBgTopRightCorner">
                <div class="BlueBorderBgBottomLeftCorner">
                  <div class="BlueBorderBgBottomRightCorner">
                    <div class="BlueBorderBgTopLeftCorner">
                      <div class="students_panel">
                        <div class="students_Heading">
                          <!-- h2>
                            All Students Grades
                          </h2 -->
                        </div>
                        <div style="font-size:8px">&nbsp;</div>
                        <div class="students_SearchField">
                          <table>
                            <tr>
                              <td><input type="text" id="studentSearchStr" onfocus="if(this.value='Enter Student Name'){this.value='';}" value="Enter Student Name" title="Enter Student Name" /></td>
                              <td><span style="float: right; margin: 3px 0 0 3px; padding: 2px; background: #D3FFC2; display: none;" id="studentSearchWait">please wait...</span><input type="button" onclick="fitlerStudentsByName($('#studentSearchStr').val())" value="Search" class="SearchBtn" /></td>
                            </tr>
                          </table>
                        </div>
                      </div>
                      @FILTER_PLACEHOLDER
    				  <div class="student_grid">
                      
                        @STUDENT_NAME
                             
                           

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
$HTML_grade_school_filter = t(<<<EOD
    <div id="BoldrProject" style="margin-top:8px;width:100%">
      <div class="ProjectResources" style="width:auto">
        <div class="ProjectResources-Content" style="padding:0px">
          <div class="ProjectResources-Activities">
        	 <div class="Project-ActivitiesFilter" style="padding:0px;">
              <div class="ProjectResources-YellowBottomRightCorner" style="width:auto">
                <div class="ProjectResources-YellowBottomLeftCorner">
                  <div  class="ProjectResources-YellowTopRightCorner">
                    <div  class="ProjectResources-YellowTopLeftCorner">
                      <table cellpadding="0" cellspacing="0" border="0" width="100%">
                        <tr>
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
                            <table cellpadding="0" class="Project-ActivitiesStatusType filter_hide" cellspacing="0" width="100%" border="0">
                            @OPTIONS
                            </table>
                          </td>
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
  <div style="font-size:10px">&nbsp;</div>
EOD
);





