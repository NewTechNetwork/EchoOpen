<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


global $base_path, $base_url;

$HTMLform_manage_user = t(<<<END

   <div style="float:left;color:#3570AA;font-size:18px;font-weight:bold"> User List </div>
<table align="right"><tr><td>
   <div style="float:right" class="big_Btn">@CREATE_USER </div>
</td><td>
<div style="float:right" class="big_Btn">@BULK_USER </div>
</td><td>
<div style="float:right" class="big_Btn">@DOWNLOAD_USER_LIST </div>
</td></tr></table>
<div style="font-size:28px">&nbsp;</div>
  <div> @FILTER_PLACE_HOLDER</div>
   <div>@STUDENTS </div>
   <br clear="all" />
<div id="ActivitiesCSS">
      <div class="LightBlueBottomRightCorner">
        <div class="LightBlueBottomLeftCorner">
          <div class="LightBlueTopRightCorner">
            <div class="LightBlueTopLeftCorner">
<table border = "0" cellspacing="0" cellpadding="0" width="100%"><tr>
   <td><h2 style="color:#3570AA;padding:0px;margin:0px">Mass Actions</h2></td></tr>
   <tr>
   <td>
   <div style="float:left"><div style="margin-top:2px;float:left">Change Status to:&nbsp;</div><div style="float:left;width:140px">@STATUS</div><div style="float:left">@BUTTON_STATUS</div></div>
   <div style="float:right"><div style="margin-top:2px;float:left">Set role to:&nbsp;</div><div style="float:left;width:120px">@ROLE</div><div style="float:left">@BUTTON_ROLE</div></div>
   </td>
   </tr></table>

            <style>
            .ui-dialog {
                height:180px !important;
            }
            .ui-widget-content {
                background-color:#fff;
                padding:5px;
            }
            .ui-widget-header {
                background-color:#fff;
            }
            .ui-dialog-buttonpane {
                background-color:#fff;
                position:absolute;
                bottom:0px;
                right:0px;
            }
            .ui-state-default, .ui-widget-content .ui-state-default {
                border:none;
            }
            .ui-dialog-content p {
                background-color:#fff;
                margin:0px;
                padding:5px 0px 5px 0px;
            }
            .ui-dialog-title {
                color:#3570AA;
                font-size:14px;
                font-weight:bold
            }
            .ui-widget-content {
                border:none !important;
            }
            .ui-widget-header {
                background:none;
                border:none;
            }
            .ui-widget-content {
                color:#222222;
            }

            .ui-dialog-buttonpane button {
                    background-image:url(@BASEPATHthemes/Boldr/Images/SubmitBtnLong.png) !important;
                    background-position: top left;
                    background-repeat: no-repeat;
                    padding:2px 0 5px 0;
                    width:200px;
                    border:none;
                    color:#fff !important;
                    margin:0 5px 0 0;
            }
            </style>

            <div id="dialog-confirm-massaction" style="background-color:#fff;display:none;" title="Confirm status change">
                <p><span class="msg">Are you sure you want to select this action?</span></p>
            </div>

          </div>
        </div>
      </div>
    </div>
    </div>

END
);


$HTML_manage_user_school_filter_heading = t(<<<EOD
        <tr><td class="StatusHeading" style="white-space:nowrap">@HEADING</td><td>
EOD
);
$HTML_manage_user_school_filter_row_end = t(<<<EOD
        </td></tr>
EOD
);
$HTML_manage_user_school_filter_cellStart = t(<<<EOD
        <div class="ChkBox">
EOD
);
$HTML_manage_user_school_filter_cellEnd = t(<<<EOD
        </div>
EOD
);
$HTML_manage_user_school_filter = t(<<<EOD
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
                              <a class="filter_link">&laquo; Close</a>
                            </div></td>
                        </tr>
                        <tr>
                          <td>
                            <table cellpadding="0" class="Project-ActivitiesStatusType filter_hide" cellspacing="0" width="100%" border="0">
                            @OPTIONS
                            </table>
                          </td>
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
    <div style="font-size:10px">&nbsp;</div>
EOD
);

$HTMLform_school_setting_info_bluebox_start = t(<<<END
   <div id="ActivitiesCSS"  style="width:auto">
   <div class="LightBlueBottomRightCorner">
            <div class="LightBlueBottomLeftCorner">
                    <div class="LightBlueTopRightCorner">
                            <div class="LightBlueTopLeftCorner">
                            <div class="ActivitiesAssignment">
                            <table cellpadding="8" class="table" cellspacing="0" border="0" style="height:147px;" width="100%" >


END
);

$HTMLform_school_setting_info_bluebox_end = t(<<<END
                            </table>
                            </div>
                            </div>
                    </div>
            </div>
    </div>
    </div>
    <div style="font-size:5px">&nbsp;</div>
END
);

$HTMLform_school_setting_bluebox_start = t(<<<END
   <div id="ActivitiesCSS"  style="width:auto">
   <div class="LightBlueBottomRightCorner">
            <div class="LightBlueBottomLeftCorner">
                    <div class="LightBlueTopRightCorner">
                            <div class="LightBlueTopLeftCorner">
                            <div class="ActivitiesAssignment">
                            <table cellpadding="8" class="table" cellspacing="0" border="0" width="100%" >


END
);

$HTMLform_school_setting_bluebox_end = t(<<<END
                            </table>
                            </div>
                            </div>
                    </div>
            </div>
    </div>
    </div>
    <div style="font-size:5px">&nbsp;</div>
END
);
$HTMLform_school_setting_bluebox_dataexport_start = t(<<<END
   <div id="ActivitiesCSS"  style="width:auto">
   <div class="LightBlueBottomRightCorner">
            <div class="LightBlueBottomLeftCorner">
                    <div class="LightBlueTopRightCorner">
                            <div class="LightBlueTopLeftCorner">
                            <div class="ActivitiesAssignment">



END
);

$HTMLform_school_setting_bluebox_dataexport_end = t(<<<END

                            </div>
                            </div>
                    </div>
            </div>
    </div>
    </div>
    <div style="font-size:5px">&nbsp;</div>
END
);
$HTMLform_school_setting_bluebox_startSec = t(<<<END
   <div id="ActivitiesCSS"  style="width:auto">
   <div class="LightBlueBottomRightCorner">
            <div class="LightBlueBottomLeftCorner">
                    <div class="LightBlueTopRightCorner">
                            <div class="LightBlueTopLeftCorner">
                            <div class="ActivitiesAssignment">


END
);

$HTMLform_school_setting_bluebox_endSec = t(<<<END
                            </div>
                            </div>
                    </div>
            </div>
    </div>
    </div>
    <div style="font-size:5px">&nbsp;</div>
END
);

$HTMLform_school_setting_bluebox_start1 = t(<<<END
   <div id="ActivitiesCSS"  style="width:auto">
   <div class="LightBlueBottomRightCorner">
            <div class="LightBlueBottomLeftCorner">
                    <div class="LightBlueTopRightCorner">
                            <div class="LightBlueTopLeftCorner">
                            <div class="ActivitiesAssignment">
                            <table cellpadding="8" class="table" cellspacing="0" border="0" width="540px">
                                <tr>
                                    <td style="border: 1px solid #9ECCE9;" colspan="3">
                                        <div style="font-size:15px; float:left; width:280px;" class="course_new_heading" >Schoolwide Learning Outcomes</div>
                                        <div style="float:right; width:100px;">@ADD_OUTCOME_LINK</div>
                                    </td>
                                </tr>
                                <!-- @OUTCOME_ENTRIES -->
                            </table>
                                </div>
                            </div>
                            </div>
                        </div>
                    </div>
                </div>

END
);

$HTMLform_school_outcome_item = t(<<<END
<tr>
    <td style="border: 1px solid #9ECCE9;" valign="top">
    <div style="class="clearfix">
        <div style="float:left; width:130px;">
            <div>
                <div class="course_new_heading" >Outcome Name</div>
                <strong>@OUTCOME_NAME</strong>
            </div>
            </br>
            <div>
                <div class="course_new_heading" >Short Name:</div>
                <strong>@OUTCOME_SHORT_NAME</strong>
            </div>
        </div>
        <div style="width:300px; float:left;">
            <div class="course_new_heading" >Description of Outcome</div>
            <span style="width:250px;overflow:auto;height:50px;">@OUTCOME_DESCRIPTION</span>
        </div>
        <div style="width:84px; float:left;">
            <div style="text-align: right; padding-bottom:5px;">
                <span style="padding-right: 5px;">@EDIT_OUTCOME_LINK</span>
                <span>@DELETE_OUTCOME_LINK</span>
            </div>
            <div style="text-align: right;">
                <span style="padding-right: 5px;">@ORDER_CHANGE_UP</span>
                <span>@ORDER_CHANGE_DOWN</span>
            </div>
        </div>
    </div>
    </td>
</tr>
END
);

$HTMLform_school_setting_bluebox_schoolyear_start = t(<<<END
   <div id="ActivitiesCSS"  style="width:auto;padding-left:3px">
   <div class="LightBlueBottomRightCorner">
            <div class="LightBlueBottomLeftCorner">
                    <div class="LightBlueTopRightCorner">
                            <div class="LightBlueTopLeftCorner">
                            <div class="ActivitiesAssignment">
                            <table cellpadding="8" class="table" cellspacing="0" border="0" width="100%" style="height:147px">


END
);

$HTMLform_school_setting_bluebox_schoolyear_end = t(<<<END
                            </table>
                            </div>
                            </div>
                    </div>
            </div>
    </div>
    </div>
END
);



$HTMLform_school_setting_print_login_infos = t(<<<END
   <script>
    function hide_print(){

        $('#hide_print').hide();
        }
   </script>
<div class="UI">
    <div class="UI_topcenter">
        <div class="UI_middleleft">
            <div class="UI_middleright">
                <div class="UI_bottomcenter">
                    <div class="UI_topleft">
                        <div class="UI_topright">
                            <div class="UI_bottomleft">
                                <div class="UI_bottomright">
                                	<div class="UI_content">
                                    	<div class="UI_heading">
                                        	<h1>New User Account Information</h1>
                                        </div>
                                        <div class="UI_url">
                                        	<a href="$base_path">Log in now at $base_url</a>
                                        </div>
                                        <div class="UI_info">
                                        	<b>Username:&nbsp;&nbsp;&nbsp;</b>@USERNAME<br />
                                        	<b>Password:&nbsp;&nbsp;&nbsp;</b>@PASSWORD
                                        </div>
                                        <div id = "hide_print" class="UI_printbtn">
                                        	<input type="image" src="@BASEPATHthemes/Boldr/Images/userInfo/UI_print_btn.jpg"  onclick="print(); hide_print();" />
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
END
);


$HTMLSchoolYearCombo = '
    <div style="float:right;margin-left:1em;margin-right:1em">
            <span style="float: left; line-height: 20px; font-size: 14px; color: #5E8FAD;">School Year:&nbsp;</span>
            <span style="width:100px;" class="comboboxBgLeft">
            <!-- school_year_combo -->
            </span>
    </div>';
?>
