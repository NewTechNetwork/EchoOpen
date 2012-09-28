<?php

$HTMLform_activity_title_hdr = t(<<<END
<div id="dialog-show-confirmation" style="display: none;" title="Warning">
    <span class="msg">Are you sure you want to submit the form?</span>
</div>
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

$HTML_ACTIVITY_GRADE_TAB_PANEL = t(<<<END
<div class="GradesTabCenter" style="width: 100%;">
        <div style="float: left; white-space: nowrap; height: 30px;">
            <ul>
                <li><a id="grades_tab_students" @OVERVIEW_CLASS onclick="@OVERVIEW" >Overview</a></li>
                <li>|</li>
                <li><a id="grades_tab_activities" @WORK_CLASS onclick="@WORK">Work and Assessment</a></li>
                <li>></li>
                <li><a id="grades_student_activities" @STUDENT_CLASS onclick="@STUDENT">Student</a></li>
            </ul>
        </div>
        <div id="publishlink" style="float: right; display: block; height: 30px; padding-top: 10px;">
             @GRADE_PUBLISHED_DATETIME_BUTTON
        </div>
    </div>

END
);

$HTML_activity_submission_grade_student_panel_START = t(<<<END
<div style="width: auto;" class="student_Panel">
    <div id="ActivitiesCSS" style="width: auto;">
        <div class="BlueBorderBgTopCenter">
            <div class="BlueBorderBgMiddleRight">
                <div class="BlueBorderBgBottomCenter">
                    <div class="BlueBorderBgMiddleLeft">
                        <div class="BlueBorderBgTopRightCorner">
                            <div class="BlueBorderBgBottomLeftCorner">
                                <div class="BlueBorderBgBottomRightCorner">
                                    <div class="BlueBorderBgTopLeftCorner">
                                        <div  style="border:2px solid #f1dcdb;">
                                        



END
);

$HTML_activity_submission_grade_student_panel_END = t(<<<END

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


$HTML_SUBMISSION_GRADE_JOURNAL_DESCRIPTION_START = t(<<<END
   <div style="font-size: 5px;">&nbsp;</div>
<div class="teacherProfile Activity_Detail-Grading">
  <div class="" style="border:2px solid #dfdfdf;">
    <div class="">
      <div class="">
        <div class="">
         <h2 class="" style=" background-color: #DFDFDF; margin: 0px; padding: 2px 2px 2px 4px; font-size:13px; ">Journal Prompt</h2>
            <p class="">
                <div style="padding: 0 0 10px 10px;">
END
);

$HTML_SUBMISSION_GRADE_JOURNAL_DESCRIPTION_END = t(<<<END
                </div>
           </p>
        </div>
      </div>
    </div>
  </div>
</div>
<div style="font-size: 5px;">&nbsp;</div>
END
);

$HTML_ACTIVITY_GRADE_USER_BUTTON_BAR_START = t(<<<END
<tr>
    <td colspan="3">
        <div class="studentpanel" style="-moz-border-radius: 5px 5px 0 0 ; -webkit-border-radius: 5px 5px 0 0; border:10px solid #3570AA; height:72px; background-color:#3570AA;">
            <div style="-moz-border-radius:5px; -webkit-border-radius:5px; background-color:#fff; height:50px; border:10px solid #fff; ">
                <div class="">
                    <div class="popup_profile">
END
);

$HTML_ACTIVITY_GRADE_USER_BUTTON_BAR_END = t(<<<END
                    </div>
                </div>
            </div>
        </div>
    </td>
</tr>
END
);

$HTML_ACTIVITY_GRADE_OUTCOME_BLOCK_START = t(<<<END
<table width="100%"  cellspacing="0" cellpadding="0" border="0">
<tbody>
    <tr>
        <td style="width: 50%;"><h2 class="Activity_Detail-headings">Grade:</h2></td>
        <td align="right"><div style="color: rgb(153, 153, 153); text-align: left; white-space: nowrap; font-size: 11px;"> @GRADE_LAST_SAVED</div></td>
    </tr>
</tbody>
</table>
<div class="teacherProfile">
    <div class="greenBacktopleft">
        <div class="greenBacktopright">
            <div class="greenBackbottomleft">
                <div class="greenBackbottomright">
                    <table cellspacing="0" cellpadding="5" border="0" width="100%" class="popup_tbl">
                        

END
);


$HTML_ACTIVITY_GRADE_OUTCOME_BLOCK_END = t(<<<END
                    </table>
                </div>
            </div>
        </div>
    </div>
</div>
<div style="font-size: 5px;">&nbsp;</div>

END
);

$HTML_ACTIVITY_GRADE_CHECK_BOXES_START = t(<<<END
<div class="teacherProfile">
    <div class="greenBacktopleft">
        <div class="greenBacktopright">
            <div class="greenBackbottomleft">
                <div class="greenBackbottomright">
                    <table>
END
);

$HTML_ACTIVITY_GRADE_CHECK_BOXES_END = t(<<<END
                    </table>
                </div>
            </div>
        </div>
    </div>
</div>
END
);

$HTML_GRADE_TEACHER_COMMENTS_BLOCK_START = t(<<<END
<h2 class="Activity_Detail-headings">Teacher Comments:</h2>
<div style="font-size: 5px;">&nbsp;</div>
<div class="teacherProfile">
    <div class="greenBacktopleft">
        <div class="greenBacktopright">
            <div class="greenBackbottomleft">
                <div class="greenBackbottomright"> 
END
);

$HTML_GRADE_TEACHER_COMMENTS_BLOCK_END = t(<<<END
                </div>
            </div>
        </div>
    </div>
</div>
END
);

$HTML_ACTIVITY_GRADE_STUDENT_COMMENTS_START = t(<<<END
 <h2 class="Activity_Detail-headings">Student Comments/Notes:</h2>
    <div class="lightgray_panel">
        <div class="lightgray_topleft">
            <div class="lightgray_topright">
                <div class="lightgray_bottomleft">
                    <div class="lightgray_bottomright">
END
);

$HTML_ACTIVITY_GRADE_STUDENT_COMMENTS_END = t(<<<END
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
END
);


$HTML_GRADE_RUBRIC_TEACHER_VIEW = t(<<<END
<!-- @HIDDEN_FIELDS -->
<div>
    <div id="notaccordion">
        <div class="ActivitiesPanel notaccordion">
            <div style="margin-top:20px">
                <div class="tbl_heading">
                    <div style="float:left">
                        <div style="float:left">
                            <span class="ui-icon ui-icon-triangle-1-s"></span>
                        </div>
                        @FEEDBACK_FROM_OTHERS_HEADING
                    </div>
                </div>
            </div>
        </div>
        <div>
            <div>
                <h3 style="padding: 0px; margin: 0pt 0pt 5px; font-size: 15px; font-weight: bold; color: rgb(53, 112, 170);">Average Scores</h3>
                    @AVERAGE_SCORES_FROM_OTHERS_TABLE
            </div>
        </div>
        <div class="ActivitiesPanel notaccordion">
            <div style="margin-top:40px">
                <div class="tbl_heading">
                    <div style="float:left">
                        <div style="float:left">
                            <span class="ui-icon ui-icon-triangle-1-s"></span>
                        </div>
                            @FEEDBACK_SUBMITTED_BY_THIS_STUDENT_HEADING
                    </div>
                </div>
            </div>
        </div>
       <div>
            @SELECTED_ACTIVITY_USER
        </div>
    </div>
</div>
END
);


$HTML_ACTIVITY_RUBRIC_GRADE_UNDERMENU_PANEL = t(<<<END
 <div style="width: 100%;" class="GradesTabCenter">
        <div style="float: left; white-space: nowrap; height: 30px;">
            <ul>
                <li style="color:#000000;font-size:11px;font-weight:normal;">View:</li>
                <li><a style="font-size:11px;" id="grades_tab_teacher" @TEACHER_CLASS onclick="@TEACHER_ONCLICK" >Teacher</a></li>
                <li>|</li>
                <li><a style="font-size:11px;" id="grades_tab_student" @STUDENT_CLASS onclick="@STUDENT_ONCLICK">Student</a></li>
            </ul>
        </div>
    </div>
END
);

