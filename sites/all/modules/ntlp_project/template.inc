<?php

$HTMLrow = t(<<<END
<tr>
        <td><input id="@DOC_NAME" type="checkbox" name="gdocs[]" value="@DOC_LINK" /></td>
        <td class="googleIcons" valign="top" align="left"><img src="@DRUPAL_BASEthemes/Boldr/Images/Icons/@ICON_DOC.jpg" /></td>
        <td class="mailName"><a href="@DOC_LINK" target=_"blank">@DOC_NAME</a></td>
        <td align="right" class="GoogleLastUpdates">@LAST_UPDATED</td>
</tr>

END
);

$HTMLseparator = t(<<<END
<tr><td colspan="3" class="dottedLine">&nbsp;</td></tr>
END
);

$HTMLcontainer = t(<<<END
<table cellpadding="0" cellspacing="0" border="0" width="100%">
<tr>
<td class="googleContent">
  <table cellspacing="0" cellpadding="0" border="0" align="center" width="100%">
      <tr>
        <td class="GoogleInnertopleftcorner"></td>
        <td colspan="2" id="GoogleInnertoprightcorner"> <span id="GoogleHeading">@FEED_TITLE</span><span id="GoogleNewMessage">@DOCS_COUNT</span></td>
      </tr>
      <tr>
        <td class="googleInnerRoundedCornerLeft"></td>
        <td valign="top" class="googleInnerBottomBg">

        <table cellpadding="0" cellspacing="0" border="0" width="100%">
        @DOCUMENTS
            <tr>
                <td colspan="3" class="LastLine">&nbsp;</td>
            </tr>
            <tr>
                    <td colspan="3" align="right" class="googleView"><a href="@DOCS_LINK">View All @FEED_TITLE>></a></td>
            </tr>
        </table>
        </td>
        <td class="googleInnerRoundedCornerRight"></td>
      </tr>
    </table>
  </td>
</tr>
</table>
END
);










$HTMLproject_briefcase_start = t(<<<END

<div id="BoldrProject">
    <div class="ProjectResources">
        <div class="Project-BriefCaseHeading">
            <h2> Project Briefcase </h2>
	</div>
        <div class="clearall"></div>
            <div class="ProjectResources-TopRightCornerBg">
                <div class="ProjectResources-TopLeftCornerBg">

END
);

$HTMLproject_briefcase_end = t(<<<END
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
END
);


$HTMLproject_briefcase_heading_start = t(<<<END

   <div class="ProjectResources-HeadingAndEditBtn">
        <div class="ProjectResources-Heading">
        <h3>Project Resources</h3>
END
);

$HTMLproject_briefcase_heading_end= t(<<<END
</div>
END
);

$HTMLproject_briefcase_editBtn_start = t(<<<END

  <div class="ProjectResources-EditBtn">
  <a href="#">&nbsp;</a>
END
);

$HTMLproject_briefcase_editBtn_end= t(<<<END

    </div>
</div>
<div class="clearall"></div>
END
);

$HTMLproject_briefcase_project_sections_start= t(<<<END
<div class="ProjectResources-Content">
END
);

$HTMLproject_briefcase_project_sections_end= t(<<<END

    </div>
END
);


$HTMLproject_briefcase_project_sections_1_start = t(<<<END

<div class="FacilitatorMaterials">
    <div class="FacilitatorMaterials-TopRightCorner">
        <div class="FacilitatorMaterials-TopLeftCorner">
           
            

END
);
$HTMLproject_briefcase_project_sections_1_end = t(<<<END

END
);





//$HTMLproject_briefcase_start = t(<<<END
//<div id="BoldrProject">
//	<div class="ProjectResources">
//		<div class="Project-BriefCaseHeading">
//			<h2> Project Briefcase </h2>
//		</div>
//		<div class="clearall"></div>
//		<div class="ProjectResources-TopRightCornerBg">
//			<div class="ProjectResources-TopLeftCornerBg">
//				<div class="ProjectResources-HeadingAndEditBtn">
//					<div class="ProjectResources-Heading">
//						<h3>Project Resources</h3>
//					</div>
//					<div class="ProjectResources-EditBtn"> <a href="#">&nbsp;</a> </div>
//				</div>
//				<div class="clearall"></div>
//				<div class="ProjectResources-Content">
//					<!--/*Facilitator Materials*/-->
//					<div class="FacilitatorMaterials">
//						<div class="FacilitatorMaterials-TopRightCorner">
//							<div class="FacilitatorMaterials-TopLeftCorner">
//								<div class="Project-FacilitatorMaterialsHeading">
//									<h3>FacilitatorMaterials</h3>
//								</div>
//								<div class="Project-FacilitatorMaterialsContent">
//									<table cellpadding="0" cellspacing="0" border="0" width="100%">
//										<tr>
//											<td><div class="Icon"><img src="Images/ProjectsImages/ProjectsAttachIcon.jpg" alt="Attach Icon" /></div>
//												<div class="text">Project Planning Calendar</div></td>
//										</tr>
//										<tr>
//											<td><div class="Icon"><img src="Images/ProjectsImages/ProjectsImageIcon.jpg" alt="Attach Icon" /></div>
//												<div class="text">Sample Student Work</div></td>
//										</tr>
//										<tr>
//											<td><div class="Icon"><img src="Images/ProjectsImages/ProjectsVideoIcon.jpg" alt="Attach Icon" /></div>
//												<div class="text">Project Notes</div></td>
//										</tr>
//									</table>
//								</div>
//							</div>
//						</div>
//					</div>
//					<div class="clearall" ></div>
//					<!--/*Student Materials*/-->
//					<div class="StudentMaterials">
//						<div class="StudentMaterials-TopRightCorner">
//							<div class="StudentMaterials-TopLeftCorner">
//								<div class="Project-StudentMaterialsHeading">
//									<h3>Student Materials</h3>
//								</div>
//								<div class="Project-StudentMaterialsContent">
//									<table cellpadding="0" cellspacing="0" border="0" width="100%">
//										<tr>
//											<td><div class="Icon"><img src="Images/ProjectsImages/ProjectsAttachIcon.jpg" alt="Attach Icon" /></div>
//												<div class="text">Entry Event</div></td>
//										</tr>
//										<tr>
//											<td><div class="Icon"><img src="Images/ProjectsImages/ProjectsImageIcon.jpg" alt="Attach Icon" /></div>
//												<div class="text">Calendar</div></td>
//										</tr>
//										<tr>
//											<td><div class="Icon"><img src="Images/ProjectsImages/ProjectsVideoIcon.jpg" alt="Attach Icon" /></div>
//												<div class="text">Rubric</div></td>
//										</tr>
//									</table>
//								</div>
//							</div>
//						</div>
//					</div>
//					<div class="clearall" ></div>
//					<!--/*Student Materials*/-->
//					<div class="StudentMaterials">
//						<div class="StudentMaterials-TopRightCorner">
//							<div class="StudentMaterials-TopLeftCorner">
//								<div class="Project-StudentMaterialsHeading">
//									<h3>Student Materials</h3>
//								</div>
//								<div class="Project-StudentMaterialsContent">
//									<table cellpadding="0" cellspacing="0" border="0" width="100%">
//										<tr>
//											<td><div class="Icon"><img src="Images/ProjectsImages/ProjectsAttachIcon.jpg" alt="Attach Icon" /></div>
//												<div class="text">Entry Event</div></td>
//										</tr>
//										<tr>
//											<td><div class="Icon"><img src="Images/ProjectsImages/ProjectsImageIcon.jpg" alt="Attach Icon" /></div>
//												<div class="text">Calendar</div></td>
//										</tr>
//										<tr>
//											<td><div class="Icon"><img src="Images/ProjectsImages/ProjectsVideoIcon.jpg" alt="Attach Icon" /></div>
//												<div class="text">Rubric</div></td>
//										</tr>
//									</table>
//								</div>
//							</div>
//						</div>
//					</div>
//				</div>
//			</div>
//		</div>
//	</div>
//</div>
//END
//);
?>