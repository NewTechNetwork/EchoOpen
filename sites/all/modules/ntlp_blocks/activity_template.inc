<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


// FieldSet
$HTMLform_groupbox_hdr = t(<<<END
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
                                    </table>
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
						<h4>Grading Option</h4>
					</div>
                                        <div>&nbsp; </div>
                                        <div>&nbsp; </div>
					<div class="ActivitiesAssignment">
						<table cellpadding="8" class="table" cellspacing="0" border="0">

END
);


$HTMLform_groupbox_grading_ftr = t(<<<END
                                    </table>
					</div>
					<div class="clearall"></div>
END
);


$HTMLform_groupbox_points_hdr = t(<<<END
					<div class="InnerHeading" style="padding-top:5px;">
						<h4>Points Possible</h4>
					</div>
                                        <div class="clearall">&nbsp;</div>
					<div class="ActivitiesAssignment">
						<table cellpadding="0" class="table" cellspacing="0" border="0"  width=100%>
                                                    <tr id="row_hide1"><td>

END
);


$HTMLform_groupbox_points_ftr = t(<<<END
                                </td>
                                 </tr>
                                    </table>
					</div>
					<div class="clearall"></div>
END
);




$HTMLform_groupbox_cmapping_hdr = t(<<<END
					<div class="InnerHeading" style="padding-top:5px;">
						<h4>Curriculum Mapping</h4>
					</div>
                                        <div>&nbsp; </div><div>&nbsp; </div>
					<div class="ActivitiesAssignment">
						<table cellpadding="8" cellspacing="0"  class="table" border="0">
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
									<div class="Filter" style="display: none;">
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
<div id="ActivitiesCSS">
	<div class="HeadingPanel">
		<div class="Headings">
			<div class="BrandCrumb"><a href="@PROJECTS_LINK">« Back to Projects</a></div>



END
);

$HTMLform_activity_outer_name_ftr = t(<<<END

    </div>
		<div class="Dropdrown"> </div>
    </div>
</div>
<br clear="all" />




END
);


$HTMLform_activity_title_hdr = t(<<<END
<div id="BoldrProject">
	<div class="ProjectDetail">
		<div class="LightestBlueBottomRightCorner">
			<div class="LightestBlueBottomLeftCorner">
				<div class="LightestBlueTopRightCorner">
					<div class="LightestBlueTopLeftCorner">
END
);

$HTMLform_activity_title_ftr = t(<<<END
</div>
				</div>
			</div>
		</div>
	</div>
</div>
<br clear="all" />

END
);


$HTMLform_grading0ption_outer_title_hdr = t(<<<END
<div id="BoldrProject">
	<div class="DrivingQuestion">
		<div class="LightestBlueBottomRightCorner">
			<div class="LightestBlueBottomLeftCorner">
				<div class="LightestBlueTopRightCorner">
					<div class="LightestBlueTopLeftCorner">
END
);
$HTMLform_grading0ption_outer_title_ftr = t(<<<END

					</div>
				</div>
			</div>
		</div>
	</div>
</div>
<div class="clearall"></div>
<br class="all" />
END
);
$HTMLform_grading0ption_Academic_title_hdr = t(<<<END
<div id="BoldrProject">
	<div class="AcademicStandardsCovered">
		<div class="LightestBlueBottomRightCorner">
			<div class="LightestBlueBottomLeftCorner">
				<div class="LightestBlueTopRightCorner">
					<div class="LightestBlueTopLeftCorner">
END
);
$HTMLform_grading0ption_Academic_title_ftr = t(<<<END
					</div>
				</div>
			</div>
		</div>
	</div>
</div>
<br class="all" />
END
);

$HTMLform_NeedtoKnow_title_hdr = t(<<<END
<div id="BoldrProjectFirstColumn">
	<div class="LightestGreenBottomRightCorner">
		<div class="LightestGreenBottomLeftCorner">
			<div class="LightestGreenTopRightCorner">
				<div class="LightestGreenTopLeftCorner" style="208px">

END
);

$HTMLform_NeedtoKnow_title_ftr = t(<<<END
                                </div>
			</div>
		</div>
	</div>
</div>
<div class="clearall"></div>
<br class="all" />

END
);

$HTMLform_grading0ption2_inner_title_hdr = t(<<<END
<div id="BoldrProject">
	<div class="LearningOutcomes">
		<div class="LightestBlueBottomRightCorner">
			<div class="LightestBlueBottomLeftCorner">
				<div class="LightestBlueTopRightCorner">
					<div class="LightestBlueTopLeftCorner">
END
);

$HTMLform_grading0ption2_inner_title_ftr = t(<<<END
					</div>
				</div>
			</div>
		</div>
	</div>
</div>
<br class="all" />
END
);

$HTMLform_grading0ption3_inner_title_hdr = t(<<<END

   <div class="InnerHeading" style="padding-top:5px;">
						<h4>Curriculum Mapping</h4>
					</div><br clear="all" />
					<div class="ActivitiesAssignment">
						<table cellpadding="8" cellspacing="0" border="0" class="table">
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





END
);

$HTMLform_students2_inner_title_ftr = t(<<<END



END
);

$HTMLform_teacher_textarea_title_hdr = t(<<<END

                <div id="ActivitiesCSS">
	<div class="LightBlueBottomRightCorner">
		<div class="LightBlueBottomLeftCorner">
			<div class="LightBlueTopRightCorner">
				<div class="LightBlueTopLeftCorner">
					<div class="ActivitiesAssignment">
                                        <table cellpadding="8" cellspacing="0" border="0" class="table">


END
);

$HTMLform_teacher_textarea_title_ftr = t(<<<END
   </table>

</div>
			</div>
		</div>
	</div>
</div>

END
);


$HTMLform_activity_manage_options = t(<<<END
<div class="anchorBaseCombobox">
<div id="btnManageproject"></div>
<div id="manageProject" class="hideMODrpDown" style="display:none">
<div class="hideMODrpDownInnerBg">
	<ul>
        <li class="ManageOption">@MANAGE_PROJECT_COPY</li>
        <li class="ManageOption">@MANAGE_PROJECT_SAVE_LIBRARY</li>
        <li class="ManageOption">@MANAGE_PROJECT_DELETE</li>
	</ul>
    </div>
</div>
</div>
END
);

$HTMLCoursesProjectsActivities_filter_heading = t(<<<EOD
    <tr>
    <td class="StatusHeading">@HEADING</td>
    <td>
EOD
);
$HTMLCoursesProjectsActivities_filter_row_end = t(<<<EOD
    </td></tr>
    <tr>
            <td class="yellowDottedLine" colspan="2">&nbsp;</td>
    </tr>
EOD
);
$HTMLCoursesProjectsActivities_filter_cellStart = t(<<<EOD
    <div class="ChkBox">
EOD
);
$HTMLCoursesProjectsActivities_filter_cellEnd = t(<<<EOD
    </div>
EOD
);
$HTMLCoursesProjectsActivities_filter = t(<<<EOD
<div class="Project-ActivitiesFilter filter_hide" style="display:none;">
    <div class="ProjectResources-YellowBottomRightCorner">
            <div class="ProjectResources-YellowBottomLeftCorner">
                    <div class="ProjectResources-YellowTopRightCorner">
                            <div class="ProjectResources-YellowTopLeftCorner">
                                    <table cellspacing="0" cellpadding="0" border="0" width="100%">
                                            <tbody><tr>
                                                    <td><div class="Project-ActivitiesFilterHeading">
                                                                    <h4>Filter</h4>
                                                            </div>
                                                            <div class="Project-ActivitiesFilterCloseLink"> <a onclick="close_filter()">« Close</a> </div></td>
                                            </tr>
                                            <tr>
                                                    <td><table cellspacing="0" cellpadding="0" border="0" width="100%" class="Project-ActivitiesStatusType">
                                                                    <tbody>
                                @OPTIONS
                                            </tbody></table></td>
                                            </tr>
                                    </tbody></table>
                            </div>
                    </div>
            </div>
    </div>
</div>
EOD
);

$HTMLCoursesProjectsActivities = t(<<<EOD
<div id="BoldrProject">
	<div class="ProjectResources">
		<div class="DataGap">
			<div class="ProjectResources-Content">
				<!--/*Activities*/-->
				<div class="ProjectResources-Activities">
					<table cellspacing="0" cellpadding="0" border="0" width="100%">
						<tbody><tr>
							<td class="ProjectResources-WhiteTopLeftCorner_green"></td>
							<td class="ProjectResources-WhiteTopCenterCorner_green"><div style="float:left" class="Project-ActivitiesHeading">
									<h3>Associated Activities</h3>
								</div>
                                                        <div style="float:right" class="project_activities_btn">
                                                             @LINK_EXISTING
                                                             @LINK_NEW
                                                        </div></td>
							<td class="ProjectResources-WhiteTopRightCorner_green"></td>
						</tr>
						<tr>
							<td class="ProjectResources-WhiteMiddleLeftCorner"></td>
							<td class="ProjectResources-WhiteMiddleCenterCorner">
								<div class="clearall"></div>
								@FILTER_PLACEHOLDER
								<div id="activityTableHolder" style="padding: 10px 10px 0pt;" >
									@ACTIVITY_TABLE_PLACEHOLDER
								</div>
								<div class="clearall"></div>
								</td>
							<td class="ProjectResources-WhiteMiddleRightCorner"></td>
						</tr>
						<tr>
							<td class="ProjectResources-WhiteBottomLeftCorner"></td>
							<td class="ProjectResources-WhiteBottomCenterCorner"></td>
							<td class="ProjectResources-WhiteBottomRightCorner"></td>
						</tr>
					</tbody></table>
				</div>
			</div>
		</div>
		<div class="ProjectResources-BottomRightCornerBg" style="background-color:#3591CB">
			<div class="ProjectResources-BottomLeftCornerBg"> &nbsp; </div>
		</div>
	</div>
</div>
EOD
);

$HTMLCoursesActivities = t(<<<EOD
<div id="BoldrProject">
	<div class="ProjectResources">
            <div class="ProjectResources-Content">
                    <!--/*Activities*/-->
                     <div style="float:left" class="Project-ActivitiesHeading">
                                                <h3 style="font-size:18px;color:#3570AA;font-weight:bold;padding:0px;margin:0px;"> Activities </h3>
                                            </div>

                                            <div style="float:right" class="Activities_btn">
                                                 @LINK_EXISTING
                                                 @LINK_NEW
                                            </div>
                                            <br clear="all" />
                                            <div style="font-size:5px">&nbsp;</div>
<div id="ActivitiesCSS">
    <div class="BlueBorderBgTopCenter">
		<div class="BlueBorderBgMiddleRight">
			<div class="BlueBorderBgBottomCenter">
				<div class="BlueBorderBgMiddleLeft">
					<div class="BlueBorderBgTopRightCorner">
						<div class="BlueBorderBgBottomRightCorner">
							<div class="BlueBorderBgBottomLeftCorner">
								<div class="BlueBorderBgTopLeftCorner">
				<div class="ProjectResources-Activities">

                                           
                                            <div class="clearall"></div>
                                            @FILTER_PLACEHOLDER
                                            <div style="padding: 10px 10px 10px;" >
                                                <div id="activityTableHolder" >@ACTIVITY_TABLE_PLACEHOLDER</div>
                                            </div>
                                            <div class="clearall"></div>
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
	</div>
</div>
EOD
);

$HTMLSelectorsAndHeading = '
    <div style="width:100%; margin: 20px 0 20px 0;" class="clearfix">
       <div style="float:left; width:480px;">
            <h1 class="pageHeading" style=""><!-- heading --> </h1>
         </div>
    </div>';