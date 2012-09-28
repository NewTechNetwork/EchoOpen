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
$HTMLform_projectDetail_hdr = t(<<<END
<div id="ActivitiesCSS">
    <div class="LightBlueBottomRightCorner">
            <div class="LightBlueBottomLeftCorner">
                    <div class="LightBlueTopRightCorner">
                            <div class="LightBlueTopLeftCorner">
                            <div class="ActivitiesAssignment">

END
);

// End Of Field Set
$HTMLform_projectDetail_ftr = t(<<<END
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
   <style>
       .panels-flexible-region-25-center {
        width:100% !important;
       }
   </style>
<div id="AgendaShareStuff" style="width:840px;">
END
);

$HTMLform_activity_title_ftr = t(<<<END
</div>
<br clear="all" />
END
);
$HTMLform_activity_heading_hdr = t(<<<END
	<!-- Agenda ShareStuff Heading -->
	<div id="AgendaShareStuffHeading" style="width:100%;">
        <table cellpadding="0px" cellspacing="0" border="0" width="100%"><tr>@FEATURED_PROJ_HEADER
                <td>
		<div class="AgendaShareStuffHeading" style="width:100%">
END
);

$HTMLform_activity_heading_ftr = t(<<<END
		</div>
                    </td><td style="width:5px">
		<div class="AgendaShareStuffTopRightCorner">
		</div>
                </td>
                </tr>
                </table>
	</div>
    <div style="clear:both"></div>
END
);
$HTMLform_activity_description_hdr = t(<<<END
	<div id="AgendaShareStuffContent" style="width:100%;">
		<div class="AgendaShareStuffBottomRightCorner">
			<div class="AgendaShareStuffBottomLeftCorner">
				<div class="WhiteBoxMainDiv" style="width:100%">
					<table cellpadding="0" bgcolor="#FFFFFF" cellspacing="0" border="0" width="100%">
						<tr>
							<td class="AgendaTopLeftWhiteCorner"></td>
							<td></td>
							<td class="AgendaTopRightWhiteCorner"></td>
						</tr>
						<tr>
						  <td></td>
							<td>
                                                        <table cellpadding="0" cellspacing="0" border="0" width="100%">
                            <tr>
END
);

$HTMLform_activity_description_ftr = t(<<<END
							</tr></table></td>
							<td></td>
						</tr>
						<tr>
							<td class="AgendaBottomLeftWhiteCorner"></td>
							<td></td>
							<td class="AgendaBottomRightWhiteCorner"></td>
						</tr>
					</table>
					<div class="gap"></div>
				</div>
		  </div>
		</div>
	</div>
END
);

$HTMLform_activity_dataLeft_hdr = t(<<<END
                               <td valign="top">
                            <div>
							  <table cellpadding="0" bgcolor="#cee5f3" cellspacing="0" border="0" width="100%" style="height:90px">
									<tr>
										<td class="AgendaLightBlueTopLeftCorner" ></td>
										<td></td>
										<td class="AgendaLightBlueTopRightCorner"></td>
									</tr>
									<tr>
										<td></td>
										<td class="AgendaLightBlueMiddleContent">
END
);

$HTMLform_activity_dataLeft_ftr = t(<<<END
</td>
										<td></td>
									</tr>
									<tr>
										<td class="AgendaLightBlueBottomLeftCorner"></td>
										<td></td>
										<td class="AgendaLightBlueBottomRightCorner"></td>
									</tr>
								</table>
							</div>
                            </td>
END
);

$HTMLform_activity_dataRight_hdr = t(<<<END

<td valign="top" style="width:105px;">
  <table cellpadding="0" bgcolor="#cee5f3" cellspacing="0" align="right" border="0" width="100px" style="height:90px;">
                <tr>
                        <td class="AgendaLightBlueTopLeftCorner" ></td>
                        <td></td>
                        <td class="AgendaLightBlueTopRightCorner"></td>
                </tr>
                <tr>
                        <td></td>
                        <td class="AgendaLightBlueMiddleContent" align="center">

END
);

$HTMLform_activity_dataRight_ftr = t(<<<END

</td>
                                        <td></td>
                                </tr>
                                <tr>
                                        <td class="AgendaLightBlueBottomLeftCorner"></td>
                                        <td></td>
                                        <td class="AgendaLightBlueBottomRightCorner"></td>
                                </tr>
                        </table>
                            </td>

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
				<div class="LightestGreenTopLeftCorner">

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

?>
