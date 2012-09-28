<?php




$HTMLform_activity_outer_name_hdr = t(<<<END
<div id="ActivitiesCSS">
	<div class="HeadingPanel">
		<div class="Headings">
			<div class="BrandCrumb"><a href="@ACTIVITIES_LINK">« Back to Activities</a></div>



END
);

$HTMLform_activity_outer_name_ftr = t(<<<END

  </div>
		<div class="Dropdrown"></div>
	</div>
</div>
<br clear="all" />




END
);


$HTMLform_activity_title_hdr = t(<<<END
<div id="ActivitiesCSS">
	<div class="LightBlueBottomRightCorner">
		<div class="LightBlueBottomLeftCorner">
			<div class="LightBlueTopRightCorner">
				<div class="LightBlueTopLeftCorner">
					<div class="ActivitiesAssignment">
						<table cellpadding="0" cellspacing="0" border="0" width="100%">


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


$HTMLform_grading0ption_outer_title_hdr = t(<<<END
<div id="ActivitiesCSS">
	<div class="LightBlueBottomRightCorner">
		<div class="LightBlueBottomLeftCorner">
			<div class="LightBlueTopRightCorner">
				<div class="LightBlueTopLeftCorner">
END
);
$HTMLform_grading0ption_outer_title_ftr = t(<<<END

   			</div>
			</div>
		</div>
	</div>
</div>
<br clear="all" />


END
);


$HTMLform_grading0ption_inner_title_hdr = t(<<<END

   <div class="InnerHeading">
						<h4>Grading Option</h4>
					</div><br clear="all" />
					<div class="ActivitiesAssignment">
						<table cellpadding="0" cellspacing="0" border="0" width="100%" class="table">

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
                                              <table cellpadding="0" cellspacing="0" class="table" width="100%" border="0">
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



$HTMLCourseActivityFilter = t(<<<EOD
<div id="BoldrProject">
      <div class="ProjectResources">
        <div class="ProjectResources-Content" style="padding:0px">
          <div class="ProjectResources-Activities">
        	 <div class="Project-ActivitiesFilter" style="padding:0px;">
              <div class="ProjectResources-YellowBottomRightCorner">
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
                              <a href="#">
                                &laquo;
                                Close
                              </a>
                            </div></td>
                        </tr>
                        <tr>
                          <td><table cellpadding="0" class="Project-ActivitiesStatusType" cellspacing="0" width="100%" border="0">
                             <tr>
                                <td class="TypeHeading" style="white-space:nowrap">Status</td>
                                <td>
                                  <div class="ChkBox">
                                    <input name="type" checked="checked" type="radio" />
                                    All
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" type="radio" />
                                    Available
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" type="radio" />
                                    Hidden
                                  </div>
                                </td>
                              </tr>
                              <tr>
                                <td colspan="2" class="yellowDottedLine">&nbsp;</td>
                              </tr>
                              <tr>
                                <td class="StatusHeading" style="white-space:nowrap">Type</td>
                                <td><div class="ChkBox">
                                    <input name="status" checked="checked" type="checkbox" />
                                    All
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Assignment
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Journal
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Quiz
                                  </div>
                                   <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Project Evaluation
                                  </div>
                                   <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Term Evaluation
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Poll
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Survey
                                  </div>
                                  </td>
                              </tr>
                              <tr>
                                <td colspan="2" class="yellowDottedLine">&nbsp;</td>
                              </tr>
                              <tr>
                                <td class="TypeHeading" style="white-space:nowrap">Visibility</td>
                                <td>
                                  <div class="ChkBox">
                                    <input name="type" checked="checked" type="checkbox" />
                                    All
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Available
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Hidden
                                  </div>
                                </td>
                              </tr>
                            </table></td>
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
    <div style="font-size:10px">
      &nbsp;
    </div>
EOD
);