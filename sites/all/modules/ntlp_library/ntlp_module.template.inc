<?php

$HTMLform_course_setting_start = t(<<<END
<div id="ActivitiesCSS" style="position:relative">
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
                            <table cellpadding="8" class="table" cellspacing="0" border="0" width="100%">
    

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
  <div class="seprater">&nbsp;</div>

END
);
$HTMLform_course_setting_greenbox_start = t(<<<END
      <div class="seprater">&nbsp;</div>
   <div class="CourseGradeMainPanel">
    <div class="GreenBoxTopLeftCorner">
      <div class="GreenBoxTopRightCorner">
        <div class="GreenBoxBottomLeftCorner">
          <div class="GreenBoxBottomRightCorner" style="height:300px;">
          <h4 class="greenHeading">Grading Scale</h3>
           <table class="table" cellpadding="8" width="292px" style="border:0px;" cellspacing="0" border="0">
END
);

$HTMLform_course_setting_greenbox_end = t(<<<END
              </td></tr></table>
        </div>
      </div>
    </div>
  </div>
</div>
  <div class="seprater">&nbsp;</div>
END
);
$HTMLform_course_setting_greenboxtwo_start = t(<<<END
      <div class="seprater">&nbsp;</div>
   <div class="CourseGradeMainPanel">
    <div class="GreenBoxTopLeftCorner">
      <div class="GreenBoxTopRightCorner">
        <div class="GreenBoxBottomLeftCorner">
          <div class="GreenBoxBottomRightCorner" style="height:300px;">
            <h4 class="greenHeading">Learning Outcomes</h3>
        <table class="table" cellpadding="8" width="292px" style="border:0px;" cellspacing="0" border="0">
END
);

$HTMLform_course_setting_greenboxtwo_end = t(<<<END
          </table>
        </div>
      </div>
    </div>
  </div>
</div>
  <div class="seprater">&nbsp;</div>
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

?>
