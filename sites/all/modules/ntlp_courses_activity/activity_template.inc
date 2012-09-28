<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


// FieldSet
$HTMLform_groupbox_hdr = t(<<<END
<br/>
    
<div id="ActivitiesCSS">
    <div class="LightBlueBottomRightCorner">
            <div class="LightBlueBottomLeftCorner">
                    <div class="LightBlueTopRightCorner">
                            <div class="LightBlueTopLeftCorner">
                           <div class="ActivitiesAssignment">

END
);

// End Of Field Set
$HTMLform_groupbox_ftr = t(<<<END
                                </div>
                            </div>
			</div>
		</div>
	</div>
</div>
END
);
$HTMLform__activity_rubric_hdr = t(<<<END

<div id="ActivitiesCSS" style="width:100% !important">
    <div class="LightBlueBottomRightCorner">
            <div class="LightBlueBottomLeftCorner">
                    <div class="LightBlueTopRightCorner">
                            <div class="LightBlueTopLeftCorner">
                           <div class="ActivitiesAssignment">

END
);

// End Of Field Set
$HTMLform_activity_rubric_ftr = t(<<<END
                                </div>
                            </div>
			</div>
		</div>
	</div>
</div>
END
);

$HTMLform_groupbox_grading_hdr = t(<<<END
					<div class="InnerHeading">
						<h4>Grading Options</h4>
					</div><br clear="all" />
					<div class="ActivitiesAssignment">
                                        <div class="groupboxGrading">
						<table cellpadding="8" class="table" width="820px" cellspacing="0" border="0">

END
);


$HTMLform_groupbox_grading_ftr = t(<<<END
                                    </table>
                                        </div>
					</div>
					<div class="clearall"></div>
END
);


$HTMLform_groupbox_rubric_hdr = t(<<<END
					
					<div class="ActivitiesAssignment">
                                        <div class="groupboxGrading">
						<table cellpadding="8" class="table" width="820px" cellspacing="0" border="0">

END
);


$HTMLform_groupbox_rubric_ftr = t(<<<END
                                    </table>
                                        </div>
					</div>
					<div class="clearall"></div>
END
);


$HTMLform_groupbox_points_hdr = t(<<<END
                                        <div id="pos_pnt"  @display_pos_pnt>
					<div class="InnerHeading" style="padding-top:5px;">
						<h4 id="possible">Maximum Possible Points</h4>
					</div><br clear="all" />
					<div class="ActivitiesAssignment">
						<table class="row_hide1 table" cellpadding="0" cellspacing="0" border="0"  width="820px">
                                                    <tr id="row_hide1"><td>

END
);


$HTMLform_groupbox_points_ftr = t(<<<END
                                </td>
                                 </tr>
                                    </table>
					</div>
					<div class="clearall"></div>
</div>
END
);




$HTMLform_groupbox_cmapping_hdr = t(<<<END
					<div class="InnerHeading" style="padding-top:5px;">
						<h4>Curriculum Mapping</h4>
					</div>
                                        <br clear="all" />
					<div class="ActivitiesAssignment">
						<table cellpadding="8" cellspacing="0" width="820px"  class="table" border="0">
END
);


$HTMLform_groupbox_cmapping_ftr = t(<<<END
                                    </table>
					</div>

END
);


$HTMLform_groupbox_sfilter_hdr = t(<<<END
					<div id="ActivitiesCSS" style="margin-left:12px">
	<div class="BlueBorderBgTopCenter">
		<div class="BlueBorderBgMiddleRight">
			<div class="BlueBorderBgBottomCenter">
				<div class="BlueBorderBgMiddleLeft">
					<div class="BlueBorderBgTopRightCorner">
						<div class="BlueBorderBgBottomRightCorner">
							<div class="BlueBorderBgBottomLeftCorner">
								<div class="BlueBorderBgTopLeftCorner">
									<div class="InnerHeading" style="padding:5px 0 5px 5px;">
										<h4>Student</h4>
									</div>
									<div class="GradeAll"><a href="#" class="GradeAllBtn">&nbsp;</a></div>
									<br clear="all" />
									<div class="Filter">
										<div class="yellowTopRightCorner">
											<div class="yellowBottomRightCorner">
												<div class="yellowBottomLeftCorner">
													<div class="yellowTopLeftCorner">
														<table cellpadding="0" cellspacing="0" width="100%" border="0" >
															<tr>
																<td><div>
																		<div class="FilterHeading">
																			<h5>Filter</h5>
																		</div>
																		<div class="CloseLink"> <a href="#">&laquo; Close</a> </div>
																	</div>
																	<div class="clearall"></div>
																	<div class="Status">
																		<div><b>Status</b></div>
																		<div>
																			<input type="checkbox" checked="checked" />
																			<span>All</span></div>
																		<div>
																			<input type="checkbox" />
																			<span>Submitted</span></div>
																		<div>
																			<input type="checkbox" />
																			<span>Incomplete</span></div>
																		<div>
																			<input type="checkbox" />
																			<span>Graded</span></div>
																	</div></td>
															</tr>

END
);


$HTMLform_groupbox_sfilter_ftr = t(<<<END
                                   </table>
                                        </div>
                                </div>
                        </div>
                </div>
        </div>
        </div>
        <div class="clearall"></div>
        <br clear="all" />

END
);

$HTMLform_groupbox_sfilter_view_hdr = t(<<<END

<div class="InnerHeading" style="padding:5px 0 5px 5px;">
										<h4>Student</h4>
									</div>
									<div class="GradeAll"><a href="#" class="GradeAllBtn">&nbsp;</a></div>
									<br clear="all" />
									<div class="Filter">
										<div class="yellowTopRightCorner">
											<div class="yellowBottomRightCorner">
												<div class="yellowBottomLeftCorner">
													<div class="yellowTopLeftCorner">
														<table cellpadding="0" cellspacing="0" width="100%" border="0" >
															<tr>
																<td><div>
																		<div class="FilterHeading">
																			<h5>Filter</h5>
																		</div>
																		<div class="CloseLink"> <a href="#">&laquo; Close</a> </div>
																	</div>
																	<div class="clearall"></div>
																	<div class="Status">
																		<div><b>Status</b></div>
																		<div>
																			<input type="checkbox" checked="checked" />
																			<span>All</span></div>
																		<div>
																			<input type="checkbox" />
																			<span>Submitted</span></div>
																		<div>
																			<input type="checkbox" />
																			<span>Incomplete</span></div>
																		<div>
																			<input type="checkbox" />
																			<span>Graded</span></div>
																	</div></td>
															</tr>


END
);

$HTMLform_groupbox_sfilter_view_ftr = t(<<<END
</table>
													</div>
												</div>
											</div>
										</div>
									</div>
									<div class="clearall"></div>
									<br clear="all" />
END
);





$HTMLform_activity_outer_name_hdr = t(<<<END
<div id="ActivitiesCSS" style="width:auto">
	<div class="HeadingPanel" >
		<div class="Heading">
			@ACTIVITIES_LINK


END
);

$HTMLform_activity_outer_name_ftr = t(<<<END

  </div>
	</div>
</div>




END
);


$HTMLform_activity_rubric_hdr = t(<<<END
<div id="ActivitiesCSS">
	<div class="HeadingPanel" >
		<div class="Heading">
			


END
);

$HTMLform_activity_rubric_ftr = t(<<<END

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
						<table cellpadding="0" cellspacing="0" border="0" width="100%" >


END
);

$HTMLform_activity_title_ftr = t(<<<END

 				</table>
					</div>
				</div>
			</div>
		</div>
	</div>
</div>

END
);
$HTMLform_rudic_title_hdr = t(<<<END
<div id="ActivitiesCSS">
	<div class="LightBlueBottomRightCorner">
		<div class="LightBlueBottomLeftCorner">
			<div class="LightBlueTopRightCorner">
				<div class="LightBlueTopLeftCorner">
					<div class="ActivitiesAssignment">


END
);

$HTMLform_rudic_title_ftr = t(<<<END
					</div>
				</div>
			</div>
		</div>
	</div>
</div>

END
);

$HTMLform_rubric_activity_title_hdr = t(<<<END
<div id="ActivitiesCSS" style="width:auto; !important">
	<div class="LightBlueBottomRightCorner">
		<div class="LightBlueBottomLeftCorner">
			<div class="LightBlueTopRightCorner">
				<div class="LightBlueTopLeftCorner">
					<div class="ActivitiesAssignment">
END
);

$HTMLform_rubric_activity_title_ftr = t(<<<END

					</div>
				</div>
			</div>
		</div>
	</div>
</div>

END
);


$HTMLform_grade_fieldset_1_start = t(<<<END
    <div id="ActivitiesCSS">

   <div class="DarkBlueCornerBottomRight">
    <div class="DarkBlueCornerTopRight">
         <div class="DarkBlueCornerBottomLeft">
            <div class="DarkBlueCornerTopLeft">

END
);
$HTMLform_grade_fieldset_1_end = t(<<<END


</div>
</div>
</div>
</div>
</div>

<br clear="all" />


END
);
$HTMLform_rubric_fieldset_1_start = t(<<<END
    <div id="ActivitiesCSS">

   <div class="DarkBlueCornerBottomRight">
    <div class="DarkBlueCornerTopRight">
         <div class="DarkBlueCornerBottomLeft">
            <div class="DarkBlueCornerTopLeft">

END
);
$HTMLform_rubric_fieldset_1_end = t(<<<END


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

$HTMLform_performance_fielset_2_start = t(<<<END
					<div class="ActivitiesAssignment">
                                             

END
);

$HTMLform_performance_fielset_2_end = t(<<<END
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


END
);

$HTMLform_students_inner_title_hdr = t(<<<END

<div class="InnerHeading" style="padding:5px 0 5px 5px;">
										<h4>Student</h4>
									</div>
									<div class="GradeAll"><a href="#" class="GradeAllBtn">&nbsp;</a></div>
									<br clear="all" />
									<div class="Filter">
										<div class="yellowTopRightCorner">
											<div class="yellowBottomRightCorner">
												<div class="yellowBottomLeftCorner">
													<div class="yellowTopLeftCorner">
														<table cellpadding="0" cellspacing="0" width="100%" border="0" >
															<tr>
																<td><div>
																		<div class="FilterHeading">
																			<h5>Filter</h5>
																		</div>
																		<div class="CloseLink"> <a href="#">&laquo; Close</a> </div>
																	</div>
																	<div class="clearall"></div>
																	<div class="Status">
																		<div><b>Status</b></div>
																		<div>
																			<input type="checkbox" checked="checked" />
																			<span>All</span></div>
																		<div>
																			<input type="checkbox" />
																			<span>Submitted</span></div>
																		<div>
																			<input type="checkbox" />
																			<span>Incomplete</span></div>
																		<div>
																			<input type="checkbox" />
																			<span>Graded</span></div>
																	</div></td>
															</tr>


END
);

$HTMLform_students_inner_title_ftr = t(<<<END
</table>
													</div>
												</div>
											</div>
										</div>
									</div>
									<div class="clearall"></div>
									<br clear="all" />
END
);



$HTMLform_students2_inner_title_hdr = t(<<<END

<div class="StudentStatus">
										<table cellpadding="5" cellspacing="0" border="0" width="100%">
											<tr>
												<th>
													Student<a href="#"><img style="border:none;" src="Images/ActivitiesImages/StatusGreenArrow.jpg" /></a>
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
<div align="center" style="background-color:#fff;padding:10px 10px 10px 10px; -moz-border-radius:5px;-webkit-border-radius:5px;">
<div style="width:800px;">
END
);

$HTMLform_teacher_textarea_title_ftr = t(<<<END

     </div>
     </div>
END
);



$HTMLform_activity_attachment_options = t(<<<END
<div class="anchorBaseCombobox">
<div id="btnAttachments" class="fixGreenCombo"></div>
<div id="manageAttachments" class="hideMODrpDown" style="display:none">
<div class="hideMODrpDownInnerBg">
	<ul>
            <li class="ManageOption">@MANAGE_ATTACH_DOCUMENT</li>
            <li class="ManageOption">@MANAGE_ATTACH_GOOGLE</li>
	</ul>
    </div>
</div>
</div>
END
);

//            <li class="ManageOption">@MANAGE_ATTACH_LINK</li>

$HTML_RUBRIC_ACTIVITY_TAB_PANEL = t(<<<END
<div style="width: 100%;" class="GradesTabCenter">
    <div style="float: left; white-space: nowrap; height: 30px;">
        <ul>
             <li><a id="grades_tab_students" @OVERVIEW_CLASS onclick="@OVERVIEW" >Overview</a></li>
            <li>|</li>
            <li><a id="grades_tab_activities" @WORK_CLASS onclick="@WORK">Work and Assessment</a></li>
            <li>></li>
            <li><a id="evaluate_activities" @EVALUATE_CLASS onclick="@EVALUATE">Evaluate</a></li>
        </ul>
    </div>
    <div style="float: right; display: block; height: 30px; padding-top: 10px;">
        <div style="float: left;" class="Activity_Detail-Published">
            <!-- Grades Published 11/29/2010 @ 12:43pm -->

        </div>
        <div style="float: left; width: 150px;" id="publishlink">

        </div>
    </div>
</div>
END
);

$HTML_ACTIVITY_RUBRIC_USER_BUTTON_BAR_START = t(<<<END
<tr>
    <td colspan="3">
       <div class="studentpanel studentpanel_evaluation">
            <div class="studentpanel_topleft_inner">
                <div class="studentpanel_topright_inner" style="height:62px !important;">
                <div style="height:5px; background-color: #fff;"></div>
                    <div class="popup_profile_inner">
END
);

$HTML_ACTIVITY_RUBRIC_USER_BUTTON_BAR_END = t(<<<END
                    </div>
                </div>
            </div>
        </div>
    </td>
</tr>
END
);


