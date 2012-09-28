<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


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
				<div class="ProjectResources-HeadingAndEditBtn">
END
);


$HTML_Briefcase_Edit_start = t(<<<END
<div class="ProjectResources-EditBtn" style="position:relative">
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
    
       
END
);

$HTML_Briefcase_section_Box_end = t(<<<END
    

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

END
);



$HTML_Briefcase = t(<<<END

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


$HTML_Project_detail_panel = t(<<<END
<div class="project clearfix" style="clear:both;">
    <div class="pro_img"><img title="" src="@PROJECT_IMAGE_PATH" width="125px" height="125px"></div>
    <div class="pro_desc">
        <div class="pro_title ">
            <div class="section-top-visibility" style="margin-top: 4px; margin-right: 5px; width: 200px; float: right;">
                @PROJECT_VISIBLE
             </div>
            <span style="float: left; width:484px;" class="pro_name" title="@PROJECT_FULL" >@PROJECT_NAME</span>
            <div style="width: 100%;" class="pro_date_small">@PROJECT_START_DATE &ndash; @PROJECT_END_DATE</div>
        </div>
        <div id="PromptBox" style="clear: both; padding-top:1px;">
            <div class="Filter">
                <div class="yellowTopRightCorner">
                    <div class="yellowBottomRightCorner">
                        <div class="yellowBottomLeftCorner">
                            <div class="yellowTopLeftCorner">
                                <div class="pro_prompt">
                                    <div style="margin-bottom:5px;"><strong>Driving Question</strong></div>
                                    @PROJECT_DRIVING_QUESTION_DATA
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


$HTML_Project_setting_panel = t(<<<END
<!--
   <div style="clear: both; float: right; width: 100%; margin-bottom: 10px;" class="">
    <div style="float: right; margin-left: 1em; margin-right: 1em;">
        <span class="comboboxBgLeft" style="width: 145px;">
            <select style="display: none;" onchange="change_term_value(this);" class="my-dropdown">
                <option id="gofile" selected="" value="">@PROJEC_SETTING</option>
                <option value="">@COPY_PROJECT</option>
                <option value="">@POST_TO_PROJECT</option>
                <option value="">@DELETE_PROJECT</option>
            </select>
        </span>
    </div>
</div>

-->
<div class="anchorBaseCombobox clearfix" style="margin-bottom: 10px;">
    <div id="btnManageproject" ></div>
    <div id="manageProject" class="hideMODrpDown" style="display:none">
        <div class="hideMODrpDownInnerBg">
                <ul>
                    <li class="ManageOption">@PROJEC_SETTING</li>
                    <li class="ManageOption">@COPY_PROJECT</li>
                    <li class="ManageOption">@POST_TO_PROJECT</li>
                    <li class="ManageOption">@DELETE_PROJECT</li>
                </ul>
        </div>
    </div>
</div>


END
);


$HTML_Project_brieface_header = t(<<<END
     <!--Project header section -->
    <div style="width: 514px; margin-bottom: 5px; float:left; ">
        <div style="float: left;" class="briefcase_heading">Project Briefcase </div>
        <div style="float: right; width: 321px; margin: 2px 0px 0px;">
            <div style="font-size: 12px; color: rgb(0, 0, 0); float: left; padding-left: 25px;">
              @PROJECT_VIEW_STATE
            </div>

            <div style="width: 90px; float: right;">
                <span>@NEW_CATEGORY</span>

            </div>

            <!--<span class="green_link">
                        	<a id="myiframe" title="" href="#TB_inline?height=330&width=620&inlineId=hiddenModalContent&modal=true" class="green_link thickbox"> New Category</a></span>--> </div>
    </div>
END
);

$HTML_Project_brieface_section_block = t(<<<END
<div id="dialog-show-confirmation" style="display: none;" title="Message:">
    <span class="msg">Are you sure you want to submit the form?</span>
</div>
<div class="pro_briefcase">
     <div id="blue_rounded">
            <div class="wide_resourcesDarkBlueTopLeft">
                <div class="wide_resourcesDarkBlueTopRight">
                    <div class="wide_resourcesDarkBlueBottomLeft">
                        <div class="wide_resourcesDarkBlueBottomRight">
                            <div class="brief_body">
                                
                              @CATEGORYIES
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
</div>
END
);




$HTML_Project_briefcase_section = t(<<<END
<div style="margin-bottom: 10px;">
    <div id="category">
        <div class="TopRightCorner">
            <div class="TopLeftCorner">
                <div style="width: 99%; padding: 10px 0px 4px 10px; height: 22px;" class="clearfix">
                    @EDIT_IN_PLACE_CATEGORY_TITLE
                    
                    <div style="float: right; width: 144px;"> <span style="font-weight: normal; padding-right: 9px;" class="show_checkbox">
                            @SECTION_SHOW_HIDE_CHECKBOX
                             </span> <span style="margin-right: 5px; margin-left: 2px;"> @SECTION_UP_ARROW </span> <span> @SECTION_DOWN_ARROW </span> <span style="margin-left: 8px;">@SECTION_DELETE_OPTION</span> </div>
                </div>
            </div>
        </div>
        <div class="brief_body_data">
            <table cellspacing="0" cellpadding="0" width="100%" id="table_second">
                <tbody>
                    @CATEGORY_RESOURCES_ENTRIES
                    <tr>
                        <td style="background-color: rgb(241, 241, 241);" colspan="6">
                            @ADD_NEW_TOOLTIP
                                <div class="@SECTION_TOOLTIP_CLASS" style="display: none;">
                                    @ADD_NEW_PROJECT_RESOURCE
                                    @ADD_NEW_ACT_TASK
                                    @ADD_NEW_ACT_JOURNAL
                                    @ADD_NEW_ACT_WORKSHOP
                                </div>
                            <div style="float: right; padding-right: 90px;"> @ADD_COPY_TOOLTIP </div>
                        </td>
                        <div class="@ITEMS_TOOLTIP_CLASS" style="display: none;">
                            <div style="margin-bottom:5px;">@COPY_ACTIVITY</div>
                            <div style="margin-bottom:4px;">@COPY_RESOURCE</div>
                        </div>
                    </tr>
                </tbody></table>
        </div>
    </div>
</div>    
END
);

$HTML_Project_briefcase_section_resource_entry = t(<<<END
   <tr>
       <td width="10%" class="icon_align"><div  class="@RESOURCE_TYPE_IMAGE_PATH"></div></td>
       <td width="50%" class="green_link" onmouseover="resource_desc_tooltip(@RESOURCE_ID, '@TOOLTIP_TEXT');">@RESOURCE_NAME</td>
       <td width="10%" class="icon_align">@RESOURCE_EDIT_LINK</td>
        <td width="15%" class="icon_align">@CHK_BOX_RESOURCE_SHOW_HIDE</td>
       <td width="10%" class="icon_align"><span style="margin-right: 5px;">@RESOURCE_UP_ARROW_LINK  </span> <span>@RESOURCE_DOWN_ARROW_LINK </span> </td>
       <td width="10%" class="icon_align">@RESOURCE_DELETE_LINK</td>
   </tr>

END
);


$HTML_Project_briefcase_need_to_know_block = t(<<<END
    <div id="dialog-confirm-ntk" style="display: none;" title="Delete Need to Know">
                <p>Are you sure you want to delete this item?</p>
            </div>
        <div class="blocks">
            <div class="greenBacktopleft">
                <div class="greenBacktopright">
                    <div class="greenBackbottomleft">
                        <div class="greenBackbottomright">
                            <div class="">
                                <div style="width: 100%; padding-bottom:5px;" class="clearfix">
                                    <div style="width: 50%; float: left;" class="block_title">Need to Knows</div>
                                    <div style="width: 65px; float: right;">
                                        <span class="">@ADD_NEW_NTK_POST</span>
                                    </div>
                                </div>
                                <div >
                                    <table cellspacing="0" cellpadding="8" border="0" width="100%">
                                        <tbody>

                                           @NEED_TO_KNOW_POST_ENTRY
                                        </tbody></table>
                                </div>
                                <div class="pro_viewall">
                                    <span>@NEED_TO_KNOW_VIEW_ALL</span>
                                </div>&nbsp;

                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
END
);

$HTML_need_to_know_entry = t(<<<END
   <div style="width:100%; padding-bottom: 5px;" class="">
        <div style="border-bottom:1px dashed #CCC; padding-bottom: 5px; width:280px;" class="clearfix" >
            <div style="float:left; width:255px; padding-right: 8px;">
                <div style="padding-bottom:5px;">
                    <div style="width:100%; position:relative;">
                        <div style="width:auto;">
                            <div style="word-wrap:break-word;">
            @POST_DATA
        </div>
                        </div>
                    </div>
                </div>

            <span>@USER_NAME</span>
            <span> (@POST_DATE) </span>
        </div>
            <div style="float:left; width:14px;; vertical-align:top;"> @NTK_POST_DELETE</div>
        </div>
   </div>
 
END
);
$HTML_Project_briefcase_project_groups_block = t(<<<END
        <div class="blocks">
            <div class="greenBacktopleft">
                <div class="greenBacktopright">
                    <div class="greenBackbottomleft">
                        <div class="greenBackbottomright">

                            <div style="width: 100%;" class="clearfix">
                                <div>
                                    <div style="width: 50%; float: left;" class="block_title">Project Groups</div>
                                        <div style="width: 86px; float: right;">
                                            <span style="">  </span>
                                            <span class="">@PROJECT_GROUP_NEW_GROUP</span>
                                        </div>
                                </div>
                                <div style="width: 100%; float:left;">
                                    <table cellpadding="5" border="0" width="100%">
                                        <tbody><tr class="">
                                                <td style="border:none;">@PROJECT_GROUP_COUNT</td>
                                            </tr>
                                        </tbody></table>
                                </div>
                                <div class="pro_viewall">
                                    <span>@PROJECT_GROUP_VIEW_ALL</span>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
END
);

$HTML_Project_briefcase_project_learning_outcome_block = t(<<<END
       <div class="blocks">
            <div class="greenBacktopleft">
                <div class="greenBacktopright">
                    <div class="greenBackbottomleft">
                        <div class="greenBackbottomright">
                            <div class="">
                                <div> <span class="block_title">Learning Outcomes</span></div>
                                <div>
                                    <table border="0" width="100%">
                                        <tbody>
                                            @LEARNING_OUTCOME
                                        </tbody></table>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
END
);

$HTML_Project_briefcase_project_academic_standards_block = t(<<<END
       <div class="blocks">
            <div class="greenBacktopleft">
                <div class="greenBacktopright">
                    <div class="greenBackbottomleft">
                        <div class="greenBackbottomright">

                            <div style="min-height: 58px;" class="">
                                <div> <span class="block_title">Curriculum Standards</span> </div>
                                <div>
                                    <table cellspacing="0" cellpadding="0" width="100%">
                                        <tbody><tr>
                                                <td style="border-bottom: medium none;">
                            	@PROJECT_ACADEMIC_STANDARDS_TEXT
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
END
);

