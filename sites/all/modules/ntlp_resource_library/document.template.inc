<?php

$HTMLform_groupbox_hdr = t(<<<END

<div id="ActivitiesCSS">
	<div class="HeadingPanel">
		<div class="Headings">
			<div class="BrandCrumb"><a href="ntlp/document/resource/library">« Back to School Resource Library</a></div>
  </div>

	</div>
</div>
<br clear="all" />

<table cellpadding="0"  cellspacing="0" ><tr><td><div class="Heading"><b></b></div></td>
    <td>
        <div class="Heading"><h2 style ="color:#3570AA;font-size:20px">Add Resource </h2></div>
    </td>
<tr></table>

<div id="ActivitiesCSS">
    <div class="LightBlueBottomRightCorner">
            <div class="LightBlueBottomLeftCorner">
                    <div class="LightBlueTopRightCorner">
                            <div class="LightBlueTopLeftCorner">
                           <div class="ActivitiesAssignment">
                           <table cellpadding="8" cellspacing="0" border="0" width="820px">

END
);


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



$HTMLform_groupbox_associated_project_hdr = t(<<<END

<div id="ActivitiesCSS">
    <div class="LightBlueBottomRightCorner">
            <div class="LightBlueBottomLeftCorner">
                    <div class="LightBlueTopRightCorner">
                            <div class="LightBlueTopLeftCorner">
                           <div class="ActivitiesAssignment">
                           <table cellpadding="0" cellspacing="0" border="0" width="820px">

END
);



$HTMLform_groupbox_associated_project_ftr = t(<<<END
   </table>
   </div>
                            </div>
			</div>
		</div>
	</div>
</div>
END
);


$HTMLform_resource_groupbox_hdr = t(<<<END
<div id="AgendaShareStuff">
	<div id="AgendaShareStuffHeading">
		<div class="AgendaShareStuffHeading">
		<h2>Share Stuff</h2>
		</div>
		<div class="AgendaShareStuffTopRightCorner">
		</div>
	</div>
        <div style="clear:both"></div>
	<div id="AgendaShareStuffContent">
<div style="background-color:#3570aa;min-height:500px;height:auto">
<table width="100%" border="0" cellpadding="0" cellspacing="0">

END
);


$HTMLform_resource_groupbox_ftr = t(<<<END
   </table>
<div class="AgendaShareStuffBottomRightCorner">
			<div class="AgendaShareStuffBottomLeftCorner" style="font-size:0px;">&nbsp;
                  </div></div>
         </div>
                  </div>
            </div>

END
);

$HTMLform_activity_outer_name_hdr = t(<<<END
<div id="AgendaShareStuff">

    <!-- Agenda ShareStuff Heading -->
    <div id="AgendaShareStuffHeading">

        <div class="AgendaShareStuffHeading">
            <h2 style="float: left;">
		 All Categories
            </h2>
        </div>
        <div class="AgendaShareStuffTopRightCorner">
        </div>
    </div>
    <div style="clear: both;"></div>
    <!-- Agenda ShareStuff Content -->
    <div id="AgendaShareStuffContent">
        <div class="AgendaShareStuffBottomRightCorner">
            <div class="AgendaShareStuffBottomLeftCorner">
                <div class="WhiteBoxMainDiv">
                    <table cellspacing="0" cellpadding="0" border="0" bgcolor="#ffffff" width="100%">
                        <tbody><tr>
                            <td class="AgendaTopLeftWhiteCorner"></td>
                            <td></td>
                            <td class="AgendaTopRightWhiteCorner"></td>
                        </tr>
                        <tr>
                            <td></td>
                            <td>
                                <table cellspacing="0" cellpadding="0" border="0" width="100%">
                                    <tbody><tr>
                                        <td valign="top">
END
);


$HTMLform_activity_outer_name_ftr = t(<<<END
 </td>

                                    </tr>
                                </tbody></table>
                            </td>
                            <td></td>
                        </tr>
                        <tr>
                            <td class="AgendaBottomLeftWhiteCorner"></td>
                            <td></td>
                            <td class="AgendaBottomRightWhiteCorner"></td>
                        </tr>
                    </tbody></table>
                    <div class="gap"></div>
                </div>
            </div>
        </div>
    </div>
</div>
END
);


$HTMLform_activity_inner_name_hdr = t(<<<END

                                            <div>
                                                <table cellspacing="0" cellpadding="0" border="0" bgcolor="#cee5f3" width="100%">
                                                    <tbody><tr>
                                                        <td class="AgendaLightBlueTopLeftCorner"></td>
                                                        <td></td>
                                                        <td class="AgendaLightBlueTopRightCorner"></td>
                                                    </tr>
                                                    <tr>
                                                        <td></td>
                                                        <td class="AgendaLightBlueMiddleContent">
END
);


$HTMLform_activity_inner_name_ftr = t(<<<END
</td>
                                                        <td></td>
                                                    </tr>
                                                    <tr>
                                                        <td class="AgendaLightBlueBottomLeftCorner"></td>
                                                        <td></td>
                                                        <td class="AgendaLightBlueBottomRightCorner"></td>
                                                    </tr>
                                                </tbody></table>
                                            </div>
                                            <div style="font-size:5px;">&nbsp;</div>
END
);

$HTMLCategoryResourceFilter = t(<<<EOD
   <div id="BoldrProject" style="width:100%;padding-right:10px;">
      <div class="ProjectResources" style="width:100%">
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
                              <a href="#">
                                &laquo;
                                Close
                              </a>
                            </div></td>
                        </tr>
                        <tr>
                          <td><table cellpadding="0" class="Project-ActivitiesStatusType" cellspacing="0" width="100%" border="0">
                              <tr>
                                <td class="StatusHeading" style="white-space:nowrap">Type</td>
                                <td><div class="ChkBox">
                                    <input name="status" checked="checked" type="checkbox" />
                                    All
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Links
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Images
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Videos
                                  </div>
                                   <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Documents
                                  </div>
                                   <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Pages
                                  </div>
                                  </td>
                              </tr>
                              <tr>
                                <td colspan="2" class="yellowDottedLine">&nbsp;</td>
                              </tr>
                              <tr>
                                <td class="TypeHeading" style="white-space:nowrap">Show Only</td>
                                <td><div class="ChkBox">
                                    <input name="type" checked="checked" type="checkbox" />
                                    Feature Items
                                  </div></td>
                              </tr>
                            </table></td>
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

$HTMLResourceCategoryInfo = t(<<<EOD
   <table width="100%">
       <tr>
            <td>
             @BACK_TO_LINK
 
              </td>
        </tr>
         <tr><td>
                      <div class="GroupsDiscriptionTopRightCorner">
                          <div class="GroupsDiscriptionTopLeftCorner">
                              <div class="GroupsDiscriptionBottomRightCorner">
                                  <div class="GroupsDiscriptionBottomLeftCorner" style="padding:5px">
                <table width="100%"><tr><td><h1 style="padding:0px;margin:0px;color:#3570aa">@CATEGORY_NAME</h1></td><td align="right">
                <small style="font-weight:bold">(@TOTAL_RESOURCES resources)</small></td></tr><tr><td colspan="2">
           @CATEGORY_DETAILS</td></tr></table>
   </div>
                              </div>
                          </div>
                      </div>
                      </td></tr><tr><td style="font-size:5px;">
                      &nbsp;
                      </td></tr></table>

EOD
);