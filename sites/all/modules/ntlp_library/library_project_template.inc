<?php

$HTML_Lib_Project_Detail_header = t(<<<END
   
<div class="clearfix" style="margin-bottom: 20px;">
    @PROJCT_TITLE_AND_COMBO
    <!--blue detail area-->
    <div id="">
        <div class="LightBlueBottomRightCorner">
            <div class="LightBlueBottomLeftCorner">
                <div class="LightBlueTopRightCorner">
                    <div class="LightBlueTopLeftCorner">
                        <table cellspacing="0" cellpadding="5" border="0" width="100%">
                            <tbody>
                                <tr>
                                    <td colspan="4"><table cellspacing="0" cellpadding="0" border="0" width="100%">
                                            <tbody><tr>
                                                    <td width="11%" style="text-align: left; font-weight: bold; vertical-align: top;">Summary:</td>
                                                    <td width="89%" colspan="3"> @LIB_RPOJECT_SUMMARY</td>
                                                </tr>
                                            </tbody></table></td>
                                </tr>
                                <tr>
                                    <td width="50%" style="text-align: left; font-weight: bold;" colspan="2">Subject(s): @PROJECT_LIB_SUBJECTS</td>
                                    <td width="50%" style="text-align: left; font-weight: bold;" colspan="2">Keywords: @PROJECT_LIB_KEYWORDS</td>
                                </tr>
                                <tr>
                                    <td width="50%" style="text-align: left; font-weight: bold;" colspan="2">Author: @PROJECT_LIB_AUTHOR </td>
                                    <td width="50%" style="text-align: left;" colspan="2"><strong>Published:</strong> @PROJECT_PUBLISHED_DATE</td>
                                </tr>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
    <!--blue detail area ends here-->

    <div class="clearfix" style="width: 840px;">
        <!--project detail and commentary links-->
        <div class="GradesTabCenter" style="float: left; white-space: nowrap; margin: 8px 0pt 0pt;">
           @LIBRARY_PROJECT_TAB
        </div>
        <!--  project detail and commentary links ends here-->

        <!--grey stats-->
        <div style="float: right ! important; background: none repeat scroll 0% 0% rgb(221, 221, 221); width: 300px; padding: 5px; margin-top: 14px;" class="">
            <table cellspacing="0" border="0" width="100%">
                <tbody><tr>
                        <td width="39" style="text-align: left; font-weight: bold;">Likes:</td>
                        <td width="16">@PROJECT_LIKES</td>
                        <td width="20"></td>
                        <td width="51" style="text-align: left; font-weight: bold;">Copies:</td>
                        <td width="29">@PROJECT_COPYIES_COUNT</td>

                        <td width="80" style="text-align: left; font-weight: bold;">Comments:</td>
                        <td width="12">@PROJECT_COMMENTS_COUNT</td>
                    </tr>
                </tbody></table>
        </div>
        <!--grey stats ends here-->
    </div>
</div>
END
);

$HTML_Lib_Project_title_area = t(<<<END
<div class="clearfix" style="width: 100%;">
    <div style="width: 400px; float: left;margin-top:10px;">
        <span> @PROJECT_DEFAULT_IMAGE</span>
        <span class="inner_pages_main_heading"> @PROJECT_TITLE </span>
    </div>
    <div style="margin-top: 7px;">
        <div style="float: right; width: 195px; margin-bottom: 10px;" class="">
            <div style="float: right; margin-left: 1em; margin-right: 1em;">
                @PROJECT_SETTING_COMBO
            </div>
        </div>
        <div style="float: right; margin-top: 5px;">
           @LIKE_IMAGE I like this
        </div>
    </div>
</div>
END
);


$HTML_Lib_project_setting_combo = t(<<<END
<div class="anchorBaseCombobox">
    
     <div id="btnManageproject" @FIXGREEN_COMBO_CLASS ></div>
    <div id="manageProject" class="hideMODrpDown" style="display:none">
        <div class="hideMODrpDownInnerBg">
                <ul>
                    <!-- @EDIT_PROJECT -->
                    <li class="ManageOption">@COPY_PROJECT</li>
                    <li class="ManageOption">@REQUEST_ARCHIVE</li>
                </ul>
        </div>
    </div>
</div>
END
);



$HTML_Lib_Project_brieface_section_block = t(<<<END
<div class="project clearfix">
    <div class="pro_img"><img height="125" width="125" src="@PROJECT_IMAGE"></div>
    <div class="pro_desc">
        <div class="pro_title clearfix"> <span style="float: left;" class="pro_name">@PROJECT_TITLE</span><span class="pro_date">Duration: PROJECTCT_DURATION_DAYS</span> </div>
            <div id="PromptBox">
                <div class="Filter">
                    <div class="yellowTopRightCorner">
                        <div class="yellowBottomRightCorner">
                            <div class="yellowBottomLeftCorner">
                                <div class="yellowTopLeftCorner">
                                    <div class="pro_prompt"><strong>Driving Question</strong> <br> <br>
                                        @PROJECT_DRIVING_QUESTION
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
</div>
<div style="width: 514px; margin-bottom: 5px;" class="briefcase_heading clearfix">
    <span style="float: left;">Project Briefcase </span>
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

$HTML_Lib_Project_briefcase_section = t(<<<END
<div style="margin-bottom: 10px;">
    <div id="category">
        <div class="TopRightCorner">
            <div class="TopLeftCorner">
                <div style="width: 99%; padding: 10px 0px 4px 10px; height: 22px;" class="clearfix">
                    <span style="overflow: hidden;white-space: nowrap;width: 290px; float: left;" id="eip@CATEGORYID" >
                    
                        <span class="pro_name_small" id="toedit@CATEGORYID"> @CATEGORY_TITLE </span>
                        </span>
                    <div style="float: right; width: 144px;"> <span style="font-weight: normal; padding-right: 9px;" class="show_checkbox">
                            
                             </span> <span style="margin-right: 5px; margin-left: 2px;"> </span> <span>  </span> <span style="margin-left: 8px;"></span> </div>
                </div>
            </div>
        </div>
        <div class="brief_body_data">
            <table cellspacing="0" cellpadding="0" width="100%" id="table_second">
                <tbody>
                    @CATEGORY_RESOURCES_ENTRIES
                    <tr>
                        <td style="background-color: rgb(241, 241, 241);" colspan="6">
                            
                            <div style="float: right; padding-right: 90px;">  </div>
                        </td>
                    </tr>
                </tbody></table>
        </div>
    </div>
</div>    
END
);

$HTML_Lib_Project_briefcase_section_resource_entry = t(<<<END
   <tr>
       <td width="10%" class="icon_align"><div class="@RESOURCE_TYPE_IMAGE_PATH"></div></td>
       <td width="50%" class="green_link" onmouseover="resource_desc_tooltip(@RESOURCE_ID, '@TOOLTIP_TEXT');">@RESOURCE_NAME</td>
       <td width="10%" class="icon_align"></td>
        <td width="15%" class="icon_align"></td>
       <td width="10%" class="icon_align"><span style="margin-right: 5px;">  </span> <span></span> </td>
       <td width="10%" class="icon_align"></td>
   </tr>

END
);


$HTML_Lib_Project_briefcase_learning_outcome_block = t(<<<END
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

$HTML_Lib_Project_briefcase_project_academic_standards_block = t(<<<END
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

$HTML_activity_overview_panel = t(<<<END
<div class="clearfix" style="">
    <div class="BoldrBrandCrumb" style="float:left; padding:0px; margin-top: -20px;">
        @BACK_LINK
    </div>
</div>
<div class="Activity_Detail_heading_ph"><div class="left"><h1><div class="form-item">
@ACTIVITY_NAME
</div>
</h1></div></div>
<div style="padding:0 0px 0 0;background-color:#fff">@EDIT</div>
<div class="student_Panel">
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
                                                        <h2 class="Activity_Detail-headings">@ACTIVIITY_DESCRIPTION_HEADING</h2>
                                                            <p class="Activity_Detail-description">@ACTIVITY_DESCRIPTION</p>
                                                   </div>
                                                   <div>&nbsp;</div>
                                                   @ATIVITY_ATTACHMENTS

                                                </td>
                                                <td style="width: 10px;">&nbsp;</td>
                                                <td valign="top" style="width:265px;">
                                                    <div style="margin:0px;" class="activity_outcomes_block">

                                                                        <h2 class="Activity_Detail-headings">Outcome:</h2>

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

$HTML_ACTIVITY_STANDARDS = t(<<<END
<div class="activity_outcomes_block" style="margin:0px;">
    <h2 class="Activity_Detail-headings">Standards:</h2>
    <p class ="Activity_Detail-description">
        @ACTIVITY_STANDARDS_DESC
    </p>
</div>

END
);

$HTML_ACTIVITY_ATTACHEMENT = t(<<<END
<div>
    <h2 class="Activity_Detail-headings">Attachments:</h2>
        <ul class="Activity_Detail-list">
            @SUBMITTED_ATTACHEMENTS
        </ul>
</div>
END
);


$HTMLContentDocument = t(<<<END
<div class="clearfix" style="">
    <div class="BoldrBrandCrumb" style="float:left; padding:0px; margin-top: -20px;">
       @BACK_LINK
    </div>
</div>
<div style="padding:0 0px 0 0;background-color:#fff">@EDIT</div>
<table cellspacing="0" cellpadding="0" border="0" align="center" width="845px" style="table-layout:fixed;">
<tbody>
    <tr>
        <td> <div><span> <img src="@ROOTthemes/Boldr/Images/@IMG_SRC" vspace="4" align="absbottom" /> </span>&nbsp;
             <span class="inner_pages_main_heading" style="font-size:20px"> @RESOURCE_TITLE </span></div>
        </td>
    </tr>
    <tr>
        <td >
          <div>
              <!-- project Description -->
                     @RESOURCE_DESCRIPTION
              <!-- project Description -->
                <div class="library_project_text"><span style="display:@RESOURCE_DISPLAY_VIEW">@RESOURCE_VIEW &nbsp;</span>
 <div><!-- @RESOURCE_ATTACHMENTS --><div>
</div>
          </div>
        </td>
     </tr>
</tbody>
</table>
END
);

$HTMLresource_manage_options = t(<<<END

<div class="anchorBaseCombobox" >
<div id="btnManageresource"></div>
<div id="manageResource" class="hideMODrpDown" style="display:none">
<div class="hideMODrpDownInnerBg">
	<ul>
        <li class="ManageOption">@MANAGE_RESOURCE_EDIT</li>
        <li class="ManageOption">@MANAGE_RESOURCE_DELETE</li>
	</ul>
    </div>
</div>
</div>

END
);
