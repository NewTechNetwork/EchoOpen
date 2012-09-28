<?php

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
<div id="ActivitiesCSS">
	<div class="HeadingPanel">
		<div class="Headings">
			@PROJECTS_LINK



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
    <div id="btnManageproject" class="fixGreenCombo"></div>
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


?>
