<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTMLform_course_setting_start = t(<<<END
<div id="ActivitiesCSS">
   <table cellpadding="0" cellspacing="0" width="100%" border="0">
    <tr>
END
);

$HTMLform_course_setting_end = t(<<<END
    </tr></table>
</div>
END
);
$HTMLform_course_setting_form_start = t(<<<END
  <td valign="top">
END
);

$HTMLform_course_setting_bluebox_start = t(<<<END
    <div class="LightBlueBottomRightCorner">
            <div class="LightBlueBottomLeftCorner">
                    <div class="LightBlueTopRightCorner">
                            <div class="LightBlueTopLeftCorner">
                            <div class="ActivitiesAssignment">
                            <table cellpadding="8" class="table" cellspacing="0" border="0" style="border-bottom:none !important" width="100%">
    

END
);

$HTMLform_course_setting_bluebox_end = t(<<<END
                            </table>
                            </div>
                            </div>
                    </div>
            </div>
    </div>
END
);
$HTMLform_course_setting_yellowbox_start = t(<<<END
<div class="CourseGradeMainPanel">
  <div class="yellowBoxTopLeftCorner">
    <div class="yellowBoxTopRightCorner">
      <div class="yellowBoxBottomLeftCorner">
        <div class="yellowBoxBottomRightCorner">
          <table cellpadding="7" cellspacing="0" border="0">
END
);

$HTMLform_course_setting_yellowbox_end = t(<<<END
          </table>
        </div>
      </div>
    </div>
  </div>
</div>
   <div style="font-size:5px">&nbsp;</div>

END
);
$HTMLform_course_setting_greenbox_start = t(<<<END
   <div style="font-size:5px">&nbsp;</div>
   <table class="edit_courses" cellpadding="0" cellspacing="0" width="100%">
	<tr>
    	<td>
        	<table width="100%" bgcolor="#D3FFC2" cellpadding="0" cellspacing="0" border="0">
            	<tr>
                	<td class="LightGreenBoxTopLeft">
                    &nbsp;
                    </td>
                	<td>
                    </td>
                	<td class="LightGreenBoxTopRight">
                    &nbsp;
                    </td>
                </tr>
            </table>
        </td>
    	<td style="width:10px;" rowspan="3">&nbsp;</td>
    	<td>
	        <table width="100%" bgcolor="#D3FFC2" cellpadding="0" cellspacing="0" border="0">
                    <tr>
                	<td class="LightGreenBoxTopLeft">
                    &nbsp;
                    </td>
                	<td>
                    </td>
                	<td class="LightGreenBoxTopRight">
                    &nbsp;
                    </td>
                </tr>
            </table>
        </td>
    </tr><tr>
    	<td class="LightGreenBox" bgcolor="#D3FFC2" valign="top" style="padding:5px 10px 5px 10px;width:50%">
        
END
);

$HTMLform_course_setting_greenbox_end = t(<<<END
        </td>
END
);
$HTMLform_course_setting_greenboxtwo_start = t(<<<END
        <td bgcolor="#D3FFC2" valign="top" class="LightGreenBox" style="padding:5px 10px 5px 10px;width:50%">
END
);

$HTMLform_course_setting_greenboxtwo_end = t(<<<END
         </td>
    </tr><tr>
    	<td>
        <table width="100%" bgcolor="#D3FFC2" cellpadding="0" cellspacing="0" border="0">
            	<tr>
                	<td class="LightGreenBoxBottomLeft">
                    &nbsp;
                    </td>
                	<td>
                    </td>
                	<td class="LightGreenBoxBottomRight">
                    &nbsp;
                    </td>
                </tr>
            </table>
        </td>
    	<td>
           <table width="100%" bgcolor="#D3FFC2" cellpadding="0" cellspacing="0" border="0">
            	<tr>
                	<td class="LightGreenBoxBottomLeft">
                    &nbsp;
                    </td>
                	<td>
                    </td>
                	<td class="LightGreenBoxBottomRight">
                    &nbsp;
                    </td>
                </tr>
            </table>
        </td>
    </tr>
</table>
END
);
$HTMLform_course_setting_form_end = t(<<<END
  </td>
END
);

$HTMLform_course_setting_tabs_start = t(<<<END
   <td  valign="top" style="padding-right:10px;width:220px">
       <div class="left_tab_menu">
END
);

$HTMLform_course_setting_tabs_end = t(<<<END
        </div>
    </td>
END
);

$HTMLform_course_settings_tabs_option_fornew = t(<<<END
        <div style="margin-bottom:1px">
   <a @CLASS>
            <div class="left_tab">
                <div class="right_tab">
                    <div class="arrow_tab">
                        <div class="tab_text">
                            @PROMPT
                        </div>
                    </div>
                </div>
            </div>
        </a>
        </div>
END
);

$HTMLform_course_settings_tabs_option = t(<<<END
        <div style="margin-bottom:1px">
   <a href="@LINK" @CLASS>
            <div class="left_tab">
                <div class="right_tab">
                    <div class="arrow_tab">
                        <div class="tab_text">
                            @PROMPT
                        </div>
                    </div>
                </div>
            </div>
        </a>
        </div>
END
);
$HTMLform_course_gradebook_fieldset1_start = t(<<<END
<table cellspacing="5"><tr>
   <td valign="top">
<div id="ActivitiesCSS">
<div class="LightBlueBottomRightCorner">
            <div class="LightBlueBottomLeftCorner">
                    <div class="LightBlueTopRightCorner">
                            <div class="LightBlueTopLeftCorner">


END
);

$HTMLform_course_gradebook_fieldset1_end = t(<<<END
                            </div> </div>
			</div>
		</div>
	</div>
        </td></tr>
END
);
$HTMLform_course_gradebook_fieldset_learning_outcomes_start = t(<<<END
<tr>
   <td valign="top">
<div id="ActivitiesCSS">
<div class="LightBlueBottomRightCorner">
            <div class="LightBlueBottomLeftCorner">
                    <div class="LightBlueTopRightCorner">
                            <div class="LightBlueTopLeftCorner">
                            <table>


END
);

$HTMLform_course_gradebook_fieldset_learning_outcomes_end = t(<<<END
   </table>
   </div> </div>
			</div>
		</div>
	</div>
        </td></tr>
END
);

//$HTMLform_course_gradebook_fieldset2_start = t(<<<END
//<tr>
//   <td valign="top">
//<div id="ActivitiesCSS">
//<div class="LightBlueBottomRightCorner">
//            <div class="LightBlueBottomLeftCorner">
//                    <div class="LightBlueTopRightCorner">
//                            <div class="LightBlueTopLeftCorner">
//                            <table>
//
//
//END
//);
//
//$HTMLform_course_gradebook_fieldset2_end = t(<<<END
//   </table>
//   </div> </div>
//			</div>
//		</div>
//	</div>
//        </td>
//END
//);
//$HTMLform_course_gradebook_fieldset4_start = t(<<<END
//   <td valign="top">
//<div id="ActivitiesCSS">
//<div class="LightBlueBottomRightCorner">
//            <div class="LightBlueBottomLeftCorner">
//                    <div class="LightBlueTopRightCorner">
//                            <div class="LightBlueTopLeftCorner">
//
//
//END
//);
//
//$HTMLform_course_gradebook_fieldset4_end = t(<<<END
//   </div> </div>
//			</div>
//		</div>
//	</div>
//        </td>
//        </tr>
//END
//);
$HTMLform_course_gradebook_fieldset5_start = t(<<<END
   <tr><td valign="top">
<div id="ActivitiesCSS">
<div class="LightBlueBottomRightCorner">
            <div class="LightBlueBottomLeftCorner">
                    <div class="LightBlueTopRightCorner">
                            <div class="LightBlueTopLeftCorner">


END
);

$HTMLform_course_gradebook_fieldset5_end = t(<<<END
   </div> </div>
			</div>
		</div>
	</div>
        </td>
        </tr>
        </table>
END
);

$HTMLform_course_student_self_enroll_bar_start = t(<<<END
<div class="CourseGradeMainPanel" style="margin-bottom:4px;">
  <div class="yellowBoxTopLeftCorner">
    <div class="yellowBoxTopRightCorner">
      <div class="yellowBoxBottomLeftCorner">
        <div class="yellowBoxBottomRightCorner">
          
       
        
END
);

$HTMLform_course_student_self_enroll_bar_end = t(<<<END
        </div>
      </div>
    </div>
  </div>
</div>
END
);     
