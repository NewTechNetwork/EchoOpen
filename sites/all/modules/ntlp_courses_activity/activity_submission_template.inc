<?php
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

$HTML_ACTIVITY_TAB_PANEL = t(<<<END
<div class="GradesTabCenter" style="width: 100%;">
        <div style="float: left; white-space: nowrap; height: 30px;">
            <ul>
                <li><a id="grades_tab_students" @OVERVIEW_CLASS onclick="@OVERVIEW" >Overview</a></li>
                <li>|</li>
                <li><a id="grades_tab_activities" @WORK_CLASS onclick="@WORK">Work and Assessment</a></li>
            </ul>
        </div>
        <div id="publishlink" style="float: right; display: block; height: 30px; padding-top: 10px;">
            @GRADE_PUBLISHED_DATETIME_BUTTON
        </div>
    </div>

END
);

$HTML_activity_submission_edit_student_panel_START = t(<<<END
<div style="width: 550px;" class="student_Panel">
    <div id="ActivitiesCSS" style="width: auto;">
        <div class="BlueBorderBgTopCenter">
            <div class="BlueBorderBgMiddleRight">
                <div class="BlueBorderBgBottomCenter">
                    <div class="BlueBorderBgMiddleLeft">
                        <div class="BlueBorderBgTopRightCorner">
                            <div class="BlueBorderBgBottomLeftCorner">
                                <div class="BlueBorderBgBottomRightCorner">
                                    <div class="BlueBorderBgTopLeftCorner">
                                        <div>
                                            
                                           

END
);

$HTML_activity_submission_edit_student_panel_END = t(<<<END
                                            
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
</td>
END
);

$HTML_activity_submission_edit_basic_panel_START = t(<<<END
<div class="student_Panel">
       @ACTIVITY_TAB_PANEL
         <br clear="all">
<div class="studentpanel_middleleft">
    <div class="studentpanel_middleright">
        <div class="studentpanel_bottomcenter">
            <div class="studentpanel_bottomleft">
                <div class="studentpanel_bottomright">
                    <div class="studentpanel_center">
                        <div style="font-size: 3px;">&nbsp;</div>
                        <div class="student_Panel">
                            <table cellspacing="0" cellpadding="0" border="0" class="Activity_Detail" width="100%">
                               
END
);

$HTML_activity_submission_edit_basic_panel_END = t(<<<END
                            </table>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
END
);


$HTML_SUBMISSION_EDIT_JOURNAL_DESCRIPTION_BLOCK = t(<<<END
   <div style="border: 2px solid #d9d9d9; margin-bottom:5px;">
      <table width=100%>
            <tr style="background-color: #d9d9d9;">
                <td class="heading"><h2 class="Activity_Detail-headings">Journal Prompt</h2></td>
            </tr>
            <tr>
                <td><div style="padding:5px;"><p class="Activity_Detail-description">@JOURNAL_DESCRIPTION</p></div></td>
            </tr>
            <tr>
                <td><div style="padding:5px;">@JOURNAL_ATTACHMENT_BLOCK</div></td>
            </tr>
      </table>
   </div>
<div style="font-size: 5px;">&nbsp;</div>
END
);


$HTML_ACTIVITY_GRADE_USER_BUTTON_BAR_START = t(<<<END

   <div class="studentpanel">
    <div class="studentpanel_topleft">
        <div class="studentpanel_topright">
            <div class="popup_profile">
END
);

$HTML_ACTIVITY_GRADE_USER_BUTTON_BAR_END = t(<<<END
            </div>
        </div>
    </div>
</div>
END
);
