<?php


$HTML_Briefcase_start = t(<<<END
   <br clear="all" />
      <div id="BoldrProject">
	<div class="ProjectResources">
		<div class="Project-BriefCaseHeading">
			<h2> Project Briefcase </h2>
		</div>
		<div class="clearall"></div>
		<div class="ProjectResources-TopRightCornerBg">
			<div class="ProjectResources-TopLeftCornerBg">
			<div class="ProjectResources-BottomLeftCornerBg" style="background-image:none">
			<div class="ProjectResources-BottomRightCornerBg" style="background-image:none">
                            <div class="ProjectResources-HeadingAndEditBtn">
					<div class="ProjectResources-Heading">
						<h3>Project Resources</h3>
					</div>
END
);


$HTML_Briefcase_Edit_start = t(<<<END
<div class="ProjectResources-EditBtn">
END
);


$HTML_Briefcase_Edit_end = t(<<<END
        </div>
    
END
);

//$HTML_Briefcase_section_start = t(<<<END
//<div class="ProjectResources-Content">
//END
//);
//
//$HTML_Briefcase_section_end = t(<<<END
//<div class="clearall" ></div>
//END
//);


$HTML_Briefcase_section1_start = t(<<<END
   </div>
<div class="clearall"></div>
<div class="ProjectResources-Content">

    <div class="FacilitatorMaterials">
    <div class="FacilitatorMaterials-TopRightCorner">
        <div class="FacilitatorMaterials-TopLeftCorner">
        <div class="Project-FacilitatorMaterialsHeading"><h3>
END
);

$HTML_Briefcase_section1_end = t(<<<END
  </h3>
</div>
        
END
);

$HTML_Briefcase_view1_start = t(<<<END
<div class="Project-FacilitatorMaterialsContent">
       
END
);
$HTML_Briefcase_view1_end = t(<<<END
        </div>
        </div>
    </div>
</div>
<div class="clearall" ></div>

END
);


$HTML_Briefcase_section_Box_start = t(<<<END
<div class="StudentMaterials">
    <div class="StudentMaterials-TopRightCorner">
        <div class="StudentMaterials-TopLeftCorner">
            <div class="Project-StudentMaterialsHeading">
                <h3>
       
END
);

$HTML_Briefcase_section_Box_end = t(<<<END
    </h3>
</div>

END
);

$HTML_Briefcase_section_Boxview_start = t(<<<END
     <div class="Project-StudentMaterialsContent">
     
END
);

$HTML_Briefcase_section_Boxview_end = t(<<<END
        </div>
        </div>
    </div>
</div>
<div class="clearall" ></div>

END
);




$HTML_Briefcase_end = t(<<<END

           </div>
        </div>
    </div>
</div>
</div>
</div>

END
);



$HTML_Briefcase= t(<<<END

<div id="BoldrProject">
	<div class="ProjectResources">
		<div class="Project-BriefCaseHeading">
			<h2> Project Briefcase </h2>
		</div>
		<div class="clearall"></div>
		<div class="ProjectResources-TopRightCornerBg">
			<div class="ProjectResources-TopLeftCornerBg">
				<div class="ProjectResources-HeadingAndEditBtn">
					<div class="ProjectResources-Heading">
						<h3>Project Resources</h3>
					</div>
					<div class="ProjectResources-EditBtn"> @EDIT_BUTTON </div>
				</div>
				<div class="clearall"></div>
				<div class="ProjectResources-Content">
					<!--/*Facilitator Materials*/-->
					@FIRST_SECTION
					<div class="clearall" ></div>
					<!--/*Student Materials*/-->
					@SECTION_BOX
					<div class="clearall" ></div>
				</div>
			</div>
		</div>
	</div>
</div>

END
);

$HTML_Briefcase_first_section_start = t(<<<END
   <!--/*Facilitator Materials*/-->
<div class="FacilitatorMaterials">
    <div class="FacilitatorMaterials-TopRightCorner">
        <div class="FacilitatorMaterials-TopLeftCorner">
            <div class="Project-FacilitatorMaterialsHeading">
                <h3>@SECTION_NAME</h3>
            </div>
        <div class="Project-FacilitatorMaterialsContent">
                @SECTION_VIEW
        </div>
      
  </div>
    </div>
</div>
END
);


$HTML_Briefcase_first_section = t(<<<END
   <!--/*Facilitator Materials*/-->
<div class="FacilitatorMaterials">
    <div class="FacilitatorMaterials-TopRightCorner">
        <div class="FacilitatorMaterials-TopLeftCorner">
            <div class="Project-FacilitatorMaterialsHeading">
                <h3>@SECTION_NAME</h3>
            </div>
        <div class="Project-FacilitatorMaterialsContent">
                @SECTION_VIEW
        </div>
        </div>
    </div>
</div>

END
);

$HTML_Briefcase_section_Box = t(<<<END
<!--/*Student Materials*/-->
<div class="StudentMaterials">
    <div class="StudentMaterials-TopRightCorner">
        <div class="StudentMaterials-TopLeftCorner">
            <div class="Project-StudentMaterialsHeading">
                <h3>@SECTION_NAME</h3>
            </div>
        <div class="Project-StudentMaterialsContent">
           @SECTION_VIEW
        </div>
        </div>
    </div>
</div>


END
);

$HTMLLibProjectActivities = t(<<<EOD
<div id="BoldrProject">
	<div class="ProjectResources">
		<div class="DataGap">
			<div class="ProjectResources-Content">
				<!--/*Activities*/-->
				<div class="ProjectResources-Activities">
					<table cellspacing="0" cellpadding="0" border="0" width="100%">
						<tbody><tr>
                                                            <td class="ProjectResources-WhiteTopLeftCorner_green"></td>
                                                            <td class="ProjectResources-WhiteTopCenterCorner_green">
                                                                <div style="float:left" class="Project-ActivitiesHeading">
                                                                    <h3>Associated Activities</h3>
                                                                </div>
                                                            </td>
                                                            <td class="ProjectResources-WhiteTopRightCorner_green"></td>
                                                        </tr>
                                                        <tr>
                                                            <td class="ProjectResources-WhiteMiddleLeftCorner"></td>
                                                            <td class="ProjectResources-WhiteMiddleCenterCorner">
								<div class="clearall"></div>
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