<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


global $base_path;
$HTMLform_activity_outer_name_hdr = t(<<<END
<div id="ActivitiesCSS">
	<div class="HeadingPanel">
		<div class="Headings ec" style="width:842px">

END
);

$HTMLform_activity_outer_name_ftr = t(<<<END

  </div>
		
	</div>
</div>
END
);

$HTMLform_activity_manage_options = t(<<<END
<div id="dialog-activity-delete" style="display: none;" title="Warning">
    <span class="msg">Are you sure you want to submit the form?</span>
</div>

<div class="anchorBaseCombobox" style="float:right">
<div id="btnManageActivities"></div>
<div id="manageActivities" class="hideMODrpDown" style="display:none">
<div class="hideMODrpDownInnerBg">
	<ul>
        <li class="ManageOption">@MANAGE_ACTIVITY_EDIT</li>
        <li class="ManageOption">@MANAGE_ACTIVITY_COPY</li>
        <li class="ManageOption">@MANAGE_ACTIVITY_DELETE</li>
	</ul>
    </div>
</div>
</div>
END
);


$HTMLform_activity_title_hdr = t(<<<END
<div id="ActivitiesCSS">
	<div class="LightBlueBottomRightCorner">
		<div class="LightBlueBottomLeftCorner">
			<div class="LightBlueTopRightCorner">
				<div class="LightBlueTopLeftCorner">
					<div class="ActivitiesAssignment">
END
);

$HTMLform_activity_title_ftr = t(<<<END
					</div>
				</div>
			</div>
		</div>
	</div>
</div>

END
);

$HTMLform_activity_gradeall_title_hdr = t(<<<END

   
   <div id="ActivitiesCSS">
	<div class="LightBlueBottomRightCorner">
		<div class="LightBlueBottomLeftCorner">
			<div class="LightBlueTopRightCorner">
				<div class="LightBlueTopLeftCorner">
					<div class="ActivitiesAssignment">


END
);

$HTMLform_activity_gradeall_title_ftr = t(<<<END
					</div>
				</div>
			</div>
		</div>
	</div>
</div>

END
);


$HTMLform_grading0ption_outer_title_hdr = t(<<<END
   

<div id="ActivitiesCSS" style="width:100%">
	<div class="LightBlueBottomRightCorner">
		<div class="LightBlueBottomLeftCorner">
			<div class="LightBlueTopRightCorner">
				<div class="LightBlueTopLeftCorner">
                                <div class="ActivitiesAssignment">
         <div class="dialog-confirm-ntk" style="background-color:#fff;display:none;" title="Confirmation">
        <p>You are about to award all students 100% on this activity. Any scores already entered will be replaced. Are you sure you want to proceed?</p>
    </div>
END
);

$HTMLform_grading0ption_conf_dlg = t(<<<END
<div class="dialog-confirm-ntk" style="background-color:#fff;display:none;" title="Confirmation">
        <p>You are about to award all students 100% on this activity. Any scores already entered will be replaced. Are you sure you want to proceed?</p>
</div>

<div id="dialog-show-confirmation" style="display: none;" title="Warning">
    <span class="msg">Are you sure you want to submit the form?</span>
</div>
END
);

$HTMLform_grading0ption_outer_title_ftr = t(<<<END
                        </div>
   			</div>
			</div>
		</div>
	</div>
</div>


END
);


$HTMLform_grading0ption_inner_title_hdr = t(<<<END

   <div class="InnerHeading">
						<h4>Grading Options</h4>
					</div><br clear="all" />
					<div class="ActivitiesAssignment">
						<table cellpadding="8" cellspacing="0" border="0" width="820px" class="table">

END
);

$HTMLform_grading0ption_inner_title_ftr = t(<<<END
</table>
					</div>
					<div class="clearall"></div>

END
);

$HTMLform_grading0ption2_inner_title_hdr = t(<<<END
<div class="InnerHeading" style="padding-top:5px;">
						<h4>Points Possible</h4>
					</div><br clear="all" />
					<div class="ActivitiesAssignment">
                                              <table cellpadding="0" cellspacing="0" width="820px" class="table" style="border-bottom:none;" border="0">
                                              <tr><td style="width:auto;border:none;">
END
);

$HTMLform_grading0ption2_inner_title_ftr = t(<<<END
   </td></tr></table>
					</div>
					<div class="clearall"></div>

END
);

$HTMLform_grading0ption3_inner_title_hdr = t(<<<END

   <div class="InnerHeading" style="padding-top:5px;">
						<h4>Curriculum Mapping</h4>
					</div><br clear="all" />
					<div class="ActivitiesAssignment">
						<table cellpadding="8" cellspacing="0" border="0" width="820px" class="table">
END
);


$HTMLform_grading0ption3_inner_title_ftr = t(<<<END
</table>
					</div>
END
);





$HTMLform_students_outer_title_hdr = t(<<<END
<div id="ActivitiesCSS">
	<div class="BlueBorderBgTopCenter">
		<div class="BlueBorderBgMiddleRight">
			<div class="BlueBorderBgBottomCenter">
				<div class="BlueBorderBgMiddleLeft">
					<div class="BlueBorderBgTopRightCorner">
						<div class="BlueBorderBgBottomRightCorner">
							<div class="BlueBorderBgBottomLeftCorner">
                                                            <div class="BlueBorderBgTopLeftCorner">
                                                                <div style="padding:10px 10px 10px 10px">
END
);

$HTMLform_students_outer_title_ftr = t(<<<END
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

$HTMLform_students_inner_title_hdr = t(<<<END

<div class="InnerHeading">
    <h4>Student</h4>
</div>


END
);


$HTML_student_activity_filter_heading = t(<<<EOD
        <div class="Status">
                                    <div><b>@HEADING</b></div>
EOD
);
$HTML_student_activity_filter_row_end = t(<<<EOD
        </div>
EOD
);
$HTML_student_activity_filter_cellStart = t(<<<EOD
        <div>
EOD
);
$HTML_student_activity_filter_cellEnd = t(<<<EOD
        </div>
EOD
);
$HTML_student_activity_filter = t(<<<EOD
    <br clear="all" />
    <div class="Filter">
        <div class="yellowTopRightCorner">
            <div class="yellowBottomRightCorner">
                <div class="yellowBottomLeftCorner">
                    <div class="yellowTopLeftCorner">
                        <table cellpadding="0" cellspacing="0" width="100%" border="0" >
                        <tr>
                            <td>
                                <div>
                                     <div class="FilterHeading">
                                            <h5>Filter</h5>
                                     </div>
                                     <div class="CloseLink">
                                           <a class="filter_link">&laquo; Close</a>
                                     </div>
                                 </div>
                                <div class="clearall"></div>
                                <div class="filter_hide">@OPTIONS
                                </div>
                            </td>
                         </tr>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
<div class="clearall"></div>
<div style="font-size:10px">&nbsp;</div>
EOD
);

$HTMLform_students_inner_title_ftr = t(<<<END
   <br clear="all" />
    <div class="Filter">
        <div class="yellowTopRightCorner">
            <div class="yellowBottomRightCorner">
                <div class="yellowBottomLeftCorner">
                    <div class="yellowTopLeftCorner">
                        <table cellpadding="0" cellspacing="0" width="100%" border="0" >
                        <tr>
                            <td>
                                <div>
                                     <div class="FilterHeading">
                                            <h5>Filter</h5>
                                     </div>
                                     <div class="CloseLink">
                                           <a href="#">&laquo; Close</a>
                                     </div>
                                 </div>
                                <div class="clearall"></div>
                                
                                <div class="Status">
                                    <div><b>Status</b></div>
                                    <div>
                                         <input type="checkbox" checked="checked" />
                                         <span>All</span>
                                    </div>
                                    <div>
                                        <input type="checkbox" />
                                        <span>Submitted</span>
                                    </div>
                                    <div>
                                            <input type="checkbox" />
                                            <span>Incomplete</span>
                                    </div>
                                    <div>
                                        <input type="checkbox" />
                                        <span>Graded</span>
                                    </div>
                                </div>
                                
                            </td>
                         </tr>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
<div class="clearall"></div>
									
END
);



$HTMLform_students2_inner_title_hdr = t(<<<END

<div class="StudentStatus">
										<table cellpadding="5" cellspacing="0" border="0" width="100%">
											<tr>
												<th>
													Student<a href="#"><img style="border:none;" src="/themes/Boldr/Images/ActivitiesImages/StatusGreenArrow.jpg" /></a>
												</th>
												<th style="width:107px">
													Status
												</th>
												<th style="width:97px">
													Grade
												</th>
												<th>
													Date Submitted
												</th>
											</tr>
											<tr>
												<td><a href="#">Amy Andrews</a></td>
												<td>Graded</td>
												<td>B (86%)</td>
												<td style="border:none;">12/1/2009</td>
											</tr>
											<tr>
												<td><a href="#">Amy Andrews</a></td>
												<td>Submitted</td>
												<td><a href="#" class="GradeBtn">&nbsp;</a></td>
												<td style="border:none;">12/1/2009</td>
											</tr>
											<tr>
												<td><a href="#">Amy Andrews</a></td>
												<td>Incomplete</td>
												<td><a href="#" class="GradeBtn">&nbsp;</a></td>
												<td style="border:none;">12/1/2009</td>
											</tr>
											<tr bgcolor="#ffcccc">
												<td><a href="#">Amy Andrews</a></td>
												<td>Graded</td>
												<td>D (71%)</td>
												<td style="border:none;">12/1/2009</td>
											</tr>



END
);

$HTMLform_students2_inner_title_ftr = t(<<<END

</table>
									</div>
									<table cellpadding="0" cellspacing="0" border="0" width="100%"><tr><td>
									<div class="bottomPager"> <span>Student Submissions 1 - 4 of 121</span> <a href="#">|< First</a> <a href="#">«Previous</a> <a href="#">Next&raquo;</a> <a href="#">Last >|</a> </div></td></tr></table>
								</div>

END
);

$HTMLform_teacher_textarea_title_hdr = t(<<<END

                <div id="ActivitiesCSS">
	<div class="LightBlueBottomRightCorner">
		<div class="LightBlueBottomLeftCorner">
			<div class="LightBlueTopRightCorner">
				<div class="LightBlueTopLeftCorner">
					<div class="ActivitiesAssignment">
                                        <table cellpadding="8" cellspacing="0" width="820px" border="0" class="table">


END
);

$HTMLform_teacher_textarea_title_ftr = t(<<<END
   </table>

</div>
		</div>	</div>
		</div>
	</div>
</div>

END
);


$HTMLform_submission_attachment_options = t(<<<END
<div class="anchorBaseCombobox">
<div id="btnsubmissionAttachments"></div>
<div id="managesubmissionAttachments" class="hideMODrpDown" style="display:none">
<div class="hideMODrpDownInnerBg">
	<ul>
        <li class="ManageOption">@MANAGE_ATTACH_GOOGLE</li>
        <li class="ManageOption">@MANAGE_ATTACH_DOCUMENT</li>
	</ul>
    </div>
</div>
</div>
END
);
//        <li class="ManageOption">@MANAGE_ATTACH_LINK</li>


$HTMLform_activity_attachment_options = t(<<<END
<div class="anchorBaseCombobox">
<div id="btnAttachments" class="fixGreenCombo"></div>
<div id="manageAttachments" class="hideMODrpDown" style="display:block">
<div class="hideMODrpDownInnerBg">
	<ul>
        <li class="ManageOption">@MANAGE_ATTACH_GOOGLE</li>
        <li class="ManageOption">@MANAGE_ATTACH_DOCUMENT</li>
	</ul>
    </div>
</div>
</div>
END
);
//        <li class="ManageOption">@MANAGE_ATTACH_LINK</li>

$HTML_activity_short_detail_start = t(<<<END
<div style="width: 100%;" id="ActivitiesCSS">
  <div class="LightBlueBottomRightCorner">
    <div class="LightBlueBottomLeftCorner">
      <div class="LightBlueTopRightCorner">
        <div class="LightBlueTopLeftCorner">
          <div class="ActivitiesAssignment">
            <table width="100%" cellspacing="0" cellpadding="8" border="0" class="table" style="border-bottom: medium none ! important;">
END
);

$HTML_activity_short_detail_end = t(<<<END
 </table>
          </div>
        </div>
      </div>
    </div>
  </div>
</div>
END
);

$HTML_activity_description_start = t(<<<END

<div id="ActivitiesCSS" style="width: auto;">
    <div class="BlueBorderBgTopCenter">
        <div class="BlueBorderBgMiddleRight">
            <div class="BlueBorderBgBottomCenter">
                <div class="BlueBorderBgMiddleLeft">
                    <div class="BlueBorderBgTopRightCorner">
                        <div class="BlueBorderBgBottomLeftCorner">
                            <div class="BlueBorderBgBottomRightCorner">
                                <div class="BlueBorderBgTopLeftCorner">
                                    <table cellspacing="0" cellpadding="0" border="0" class="Activity_Detail">

END
);

$HTML_activity_student_overview_panel = t(<<<END
<div class="student_Panel">
    @ACTIVITY_TAB_PANEL
    <br clear="all">
    <div id="ActivitiesCSS" style="width: auto;margin:0px;">
        <div class="BlueBorderBgTopCenter">
            <div class="BlueBorderBgMiddleRight">
                <div class="BlueBorderBgBottomCenter">
                    <div class="BlueBorderBgMiddleLeft">
                        <div class="BlueBorderBgTopRightCorner">
                            <div class="BlueBorderBgBottomLeftCorner">
                                <div class="BlueBorderBgBottomRightCorner">
                                    <div class="BlueBorderBgTopLeftCorner">
                                        <table cellspacing="0" cellpadding="0" width="100%" border="0" class="Activity_Detail">
                                            <tr>
                                                <td valign="top">
                                                    <div>
                                                    <div class="activity_overview_message" style="margin:0px;">
                                                        @ACTIVITY_MESSAGE
                                                   </div>
                                                   </div>
                                                   <div>&nbsp;</div>
                                                   <div>
                                                        @ACTIVITY_RESPONSE_INFO
                                                        <h2 class="Activity_Detail-headings">@ACTIVIITY_DESCRIPTION_HEADING</h2>
                                                            <p class="Activity_Detail-description">@ACTIVITY_DESCRIPTION</p>
                                                   </div>
                                                   <div>&nbsp;</div>
                                                   @ATIVITY_ATTACHMENTS
                                                   
                                                </td>
                                                <td style="width: 10px;">&nbsp;</td>
                                                <td valign="top" style="width:265px;">
                                                    <div style="margin:0px;width:265px;" class="activity_outcomes_block clearfix">
                                                        
                                                                        <h2 class="Activity_Detail-headings">Grading:</h2>

                                                                            <ul>
                                                                                @ACTIVITY_LEARNING_OUTCOMES
                                                                                
                                                                            </ul>
                                                                  
                                                        </div>
                                                        <div style="font-size: 10px;">&nbsp;</div>
                                                        @ACTIVITY_STANDARD
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
</div>
END
);

$HTML_activity_work_panel_filter = t(<<<END

<div style="width: 100%;" id="BoldrProject">
                                            <div style="width: 100%;" class="ProjectResources">
                                              <div class="ProjectResources-Content" style="padding: 0px;">
                                                <div class="ProjectResources-Activities">
                                                  <div class="Project-ActivitiesFilter" style="padding: 0px;">
                                                    <div style="width: 100%;" class="ProjectResources-YellowBottomRightCorner">
                                                      <div class="ProjectResources-YellowBottomLeftCorner">
                                                        <div class="ProjectResources-YellowTopRightCorner">
                                                          <div class="ProjectResources-YellowTopLeftCorner">
                                                            <table width="100%" cellspacing="0" cellpadding="0" border="0">
                                                              <tbody>
                                                                <tr>
                                                                  <td><div class="Project-ActivitiesFilterHeading">
                                                                      <h4> Filter </h4>
                                                                    </div>
                                                                    <div class="Project-ActivitiesFilterCloseLink"> <a class="filter_close"> «
                                                                      Close </a> </div></td>
                                                                </tr>
                                                                <tr>
                                                                  <td><table width="100%" cellspacing="0" cellpadding="0" border="0" class="Project-ActivitiesStatusType filter_hide">
                                                                      <tbody>
                                                                        <tr>
                                                                          <td class="StatusHeading" style="white-space: nowrap;">Type</td>
                                                                          <td><div class="ChkBox">
                                                                              <label>
                                                                                <input type="checkbox">
                                                                                All</label>
                                                                            </div>
                                                                            <div class="ChkBox">
                                                                              <label>
                                                                                <input type="checkbox">
                                                                                Submitted</label>
                                                                            </div>
                                                                            <div class="ChkBox">
                                                                              <label>
                                                                                <input type="checkbox">
                                                                                Re-Submitted</label>
                                                                            </div>
                                                                            <div class="ChkBox">
                                                                              <label>
                                                                                <input type="checkbox">
                                                                                Not Yet Submitted</label>
                                                                            </div>
                                                                            <div class="ChkBox"><span style="padding: 5px; background-color: rgb(255, 255, 204); border: 1px solid rgb(251, 243, 171); white-space: nowrap;">
                                                                              <label>
                                                                                <input type="checkbox">
                                                                                Submitted Late</label>
                                                                              </span></div>
                                                                            <div class="ChkBox"><span style="padding: 5px; background-color: rgb(252, 230, 248); border: 1px solid rgb(251, 243, 171); white-space: nowrap;">
                                                                              <label>
                                                                                <input type="checkbox">
                                                                                Incomplete</label>
                                                                              </span></div></td>
                                                                        </tr>
                                                                      </tbody>
                                                                    </table></td>
                                                                </tr>
                                                              </tbody>
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
END
);

$HTML_activity_student_work_panel = t(<<<END
   <div class="student_Panel">
       @ACTIVITY_TAB_PANEL
         <br clear="all">
<div id="ActivitiesCSS" style="width: auto; margin: 0px;">
                          <div class="BlueBorderBgTopCenter">
                            <div class="BlueBorderBgMiddleRight">
                              <div class="BlueBorderBgBottomCenter">
                                <div class="BlueBorderBgMiddleLeft">
                                  <div class="BlueBorderBgTopRightCorner">
                                    <div class="BlueBorderBgBottomLeftCorner">
                                      <div class="BlueBorderBgBottomRightCorner">
                                        <div style="padding: 10px;" class="BlueBorderBgTopLeftCorner">
                                          @FILTER_AREA

                                          @GRADE_ALL_FORM
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

$HTML_ACTIVITY_STANDARDS = t(<<<END
<div class="activity_outcomes_block" style="margin:0px;">
    <h2 class="Activity_Detail-headings">Standards:</h2>
    <p class ="Activity_Detail-description">
        @ACTIVITY_STANDARDS_DESC
    </p>
</div>

END
);

$HTML_ACTIVITY_TAB_PANEL = t(<<<END
<div class="GradesTabCenter" style="width: 100%;">
        <div style="float: left; white-space: nowrap; height: 30px;">
            <ul>
                <li><a id="grades_tab_students" @OVERVIEW_CLASS onclick="@OVERVIEW" >Overview</a></li>
                @WORK_AND_ASSESSMENT_LINK
            </ul>
        </div>
        <div id="publishlink" style="float: right; display: block; height: 30px; padding-top: 10px;">
            @GRADE_PUBLISHED_DATETIME_BUTTON
        </div>
    </div>

END
);


$HTML_activity_student_work_submission_view_panel = t(<<<END
<div class="student_Panel">
    @ACTIVITY_TAB_PANEL
    <br clear="all">
    <div style="width: auto; margin: 0px;" id="ActivitiesCSS">
    <div class="BlueBorderBgTopCenter">
      <div class="BlueBorderBgMiddleRight">
        <div class="BlueBorderBgBottomCenter">
          <div class="BlueBorderBgMiddleLeft">
            <div class="BlueBorderBgTopRightCorner">
              <div class="BlueBorderBgBottomLeftCorner">
                <div class="BlueBorderBgBottomRightCorner">
                  <div class="BlueBorderBgTopLeftCorner">


                            <div style="font-size: 3px;">&nbsp;</div>

                                <table cellspacing="0" cellpadding="0" border="0" class="Activity_Detail" width="100%">
                                    <tr>
                                        <td valign="top" style="width: 60%;">
                                            @STUDENT_SUBMISSION_VIEW_EDIT_FORM

                                        </td>
                                        <td style="width: 10px;">&nbsp;</td>
                                        <td valign="top">
                                            @ACTIVITY_GRADE_OUTCOME_TEACHER_COMMENT_BLOCK

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
</div>
END
);

$HTML_TEACHER_COMMENTS_BLOCK = t(<<<END
<h2 class="Activity_Detail-headings">Teacher Comments:</h2>
<div style="font-size: 5px;">&nbsp;</div>
<div class="teacherProfile">
    <div class="greenBacktopleft">
        <div class="greenBacktopright">
            <div class="greenBackbottomleft">
                <div class="greenBackbottomright"> @TEACHER_COMMENTS</div>
            </div>
        </div>
    </div>
</div>
END
);

/* @var $HTML_SUBMISSION_VIEW_PAGE_BASIC <type> */
$HTML_SUBMISSION_VIEW_PAGE_BASIC = t(<<<END
<div style="font-size: 5px;">&nbsp;</div>
<div style="float: left; color: rgb(153, 153, 153); padding-top: 5px;">@ACTIVITY_STUTUS_MESSAGE</div>
<div style="float: right;">@EDIT_SUBMISSION_BUTTON</div>
<br clear="all">
<div style="font-size: 5px;">&nbsp;</div>
<div style="width: auto;" class="student_Panel">
    <div id="ActivitiesCSS" style="width: auto;">
        <div class="BlueBorderBgTopCenter">
            <div class="BlueBorderBgMiddleRight">
                <div class="BlueBorderBgBottomCenter">
                    <div class="BlueBorderBgMiddleLeft">
                        <div class="BlueBorderBgTopRightCorner">
                            <div class="BlueBorderBgBottomLeftCorner">
                                <div class="BlueBorderBgBottomRightCorner">
                                    <div class="BlueBorderBgTopLeftCorner">
                                        <!-- <div>
                                                <div style="padding-right: 5px;">
                                                <div>
                                                    <h1 style="font-weight: normal; font-size: 17px; color: rgb(153, 153, 153); ">@STUDENT_WORK_HEADING
                                                        @SUBMITTED_DATE</h1>
                                                    <p class="Activity_Detail-description">@SUBMITTED_WORK</p>
                                                    </div>
                                                    <div>&nbsp;</div>
                                                    @ACTIVITY_ATTACHEMENTS
                                                    <div>&nbsp;</div>
                                                    @STUDENT_COMMENTS_FOR_TEACHER_BLOCK
                                                </div>
                                        </div> -->

                                        <div style="border: 2px solid #d9d9d9; margin-bottom:5px;">
                                          <table width=100%>
                                                <tr style="background-color: #d9d9d9;">
                                                    <td class="heading"><h2 class="Activity_Detail-headings" style="color: #000000;font-size: 14px;font-weight: normal;margin-top: 0;padding: 0 4px 4px 7px;">@STUDENT_WORK_HEADING @SUBMITTED_DATE</h2></td>
                                                </tr>
                                                <tr>
                                                    <td><div style="padding:5px;"><p class="Activity_Detail-description">@SUBMITTED_WORK</p></div></td>
                                                </tr>
                                                <tr>
                                                    <td><div style="padding:5px;">@ACTIVITY_ATTACHEMENTS</div></td>
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
</div>
END
);


$HTML_SUBMISSION_JOURNAL_VIEW = t(<<<END
@JOURNAL_DESCRIPTION_BLOCK

<div style="font-size: 5px;">&nbsp;</div>
<div style="float: left; color: rgb(153, 153, 153); padding-top: 5px;">@ACTIVITY_STUTUS_MESSAGE</div>
<div style="float: right;">@EDIT_SUBMISSION_BUTTON</div>
<br clear="all">

   <div style="" class="activity_detail_section">
      <table class="activity_details">
            <tr style="background-color: #f1dcdb;">
                <td>
                <h1 class="Activity_Detail-headings">Student Work </h1></td>
                <td>@SUBMITTED_DATE</td>
            </tr>
            <tr>
                <td colspan="2"><div style="padding:5px;">@SUBMITTED_WORK</div></td>
               
            </tr>
            <tr>
                <td colspan="2"><div style="padding:5px;">@ACTIVITY_ATTACHEMENTS</div></td>
            </tr>
            <tr>
                <td colspan="2"><div style="padding:5px;">@STUDENT_COMMENTS_FOR_TEACHER_BLOCK</div></td>
            </tr>

      </table>
   </div>

END
);

$HTML_SUBMISSION_RUBRIC_VIEW = t(<<<END
<!-- @HIDDEN_FIELDS -->
<div>
    <div id="notaccordion">
        <div class="ActivitiesPanel notaccordion">
            <div style="margin-top:20px">
                <div class="tbl_heading">
                    <div style="float:left">
                        <div style="float:left"><span class="ui-icon ui-icon-triangle-1-s"></span></div>
                            <font style="color: black ! important; font-size: 18px; font-weight: normal; line-height: 18px; margin-left: 5px; margin-right: 5px;">Feedback Submitted by Me</font>
                    </div>
                    </div>
                </div>
            </div>
        <div>
        @SELECTED_ACTIVITY_USER
        </div>
    </div>
    <div id="notaccordion">
        <div class="ActivitiesPanel notaccordion">
            @AVERAGE_SCORES_FROM_OTHERS_TABLE
        </div>
    </div>
</div>
END
);

$HTML_ACTIVITY_OUTCOME_AND_TEACHER_COMMENTS_BLOCK = t(<<<END
<table width="100%"  cellspacing="0" cellpadding="0" border="0">
<tbody>
    <tr>
        <td style="width: 50%;"><h2 class="Activity_Detail-headings">Grade:</h2></td>
        <td align="right"><div style="" class="postedDateBox"> @LAST_SAVED</div></td>
    </tr>
</tbody>
</table>
<div class="activity_outcomes_block">
    <table cellspacing="0" cellpadding="5" border="0" width="100%" class="popup_tbl">
        @GRADES_OUTCOMES
    </table>
</div>
<div style="font-size: 5px;">&nbsp;</div>
@TEACHER_COMMENTS_BLOCK
END
);


$HTML_Comments_By_Category = t(<<<END
 <div class="studentpanel">
                                    <div class="studentpanel_topleft">
                                      <div style="height: 30px;" class="studentpanel_topright">
                                        <div class="popup_profile">
                                          <div>
                                            <h1 style="margin: 4px 0pt;color:#fff">@CATEGORY_HEADING</h1>
                                          </div>
                                        </div>
                                      </div>
                                    </div>
                                    <div class="studentpanel_middleleft">
                                      <div class="studentpanel_middleright">
                                        <div class="studentpanel_bottomcenter">
                                          <div class="studentpanel_bottomleft">
                                            <div class="studentpanel_bottomright">
                                              <div class="studentpanel_center">
                                                          @CATEGORY_COMMENT_RECEIVED
                                                
                                              </div>
                                            </div>
                                          </div>
                                        </div>
                                      </div>
                                    </div>
                                  </div><div style="font-size: 5px;">&nbsp;</div>
END
);

$HTML_CATEGORY_COMMENTS_BLOCK = t(<<<END
<div class="lightgray_panel">
    <div class="lightgray_topleft">
      <div class="lightgray_topright">
          <div class="lightgray_bottomleft">
              <div class="lightgray_bottomright">
                @COMMENTS
              </div>
          </div>
      </div>
    </div>
</div>
<div style="font-size: 5px;">&nbsp;</div>
END
);

$HTML_ACTIVITY_ATTACHEMENT = t (<<<END
<div>
    <h2 class="Activity_Detail-headings">Attachments:</h2>
        <ul class="Activity_Detail-list">
            @SUBMITTED_ATTACHEMENTS
        </ul>
</div>
END
);

$HTML_STUDENT_COMMENTS_FOR_TEACHER = t (<<<END
<div>
    <h2 class="Activity_Detail-headings">@STUDENT_COMMENTS_HEADING</h2>
    <div class="lightgray_panel">
        <div class="lightgray_topleft">
            <div class="lightgray_topright">
                <div class="lightgray_bottomleft">
                    <div class="lightgray_bottomright" style="padding:0px"><div style="padding:5px;">@STUDENT_COMMENTS</div></div>
                </div>
            </div>
        </div>
    </div>
</div>
END
);


$HTML_JOURNAL_PROMPT_DESC_BLOCK = t (<<<END
   <div style="border:1px solid #d9d9d9;">
      <table class="activity_details">
            <tr style="background-color: #d9d9d9;">
                <td class="heading"><h2 class="Activity_Detail-headings">Journal Prompt</h2></td>
            </tr>
            <tr>
                <td><div style="padding:5px;"><p class="Activity_Detail-description">@JOURNAL_PROMPT_DESC</p></div></td>
            </tr>
            <tr>
                <td><div style="padding:5px;">@JOURNAL_ATTACHMENT_BLOCK</div></td>
            </tr>
      </table>
   </div>
END
);
