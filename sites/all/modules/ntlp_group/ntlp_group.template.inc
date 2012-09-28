<?php

$HTMLform_group_setting_start = t(<<<END
<div id="ActivitiesCSS">
   
END
);

$HTMLform_group_setting_end = t(<<<END
   
</div>
END
);
$HTMLform_group_setting_form_start = t(<<<END
  <td valign="top">
    <div class="LightBlueBottomRightCorner">
            <div class="LightBlueBottomLeftCorner">
                    <div class="LightBlueTopRightCorner">
                            <div class="LightBlueTopLeftCorner">
                            <div class="ActivitiesAssignment">
                            <table cellpadding="8" class="table" cellspacing="0" border="0" width="100%">
    

END
);

$HTMLform_group_setting_form_end = t(<<<END
                            </table>
                            </div>
                            </div>
                    </div>
            </div>
    </div>
  </td>
END
);
$HTMLform_group_setting_tabs_start = t(<<<END
    <td  valign="top" style="padding-right:10px;width:220px">
       <div class="echo_left_tab_menu">
END
);

$HTMLform_group_setting_tabs_end = t(<<<END
         </div>
    </td>
END
);

$HTMLform_group_settings_tabs_option = t(<<<END
      <div style="margin-bottom:1px">
   <a href="@LINK" @CLASS>
            <div class="echo_left_tab">
                <div class="echo_right_tab">
                    <div class="echo_arrow_tab">
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




$HTMLform_group_project_detail_block = t(<<<END

<div class="pane-content">
  <div class="GroupsDiscription">
    <div class="GroupsDiscriptionTopRightCorner">
      <div class="GroupsDiscriptionTopLeftCorner">
        <div class="GroupsDiscriptionBottomLeftCorner">
          <div class="GroupsDiscriptionBottomRightCorner">
            <div class="GroupsDatapanel">
              <div class="GroupsDiscriptionHeading">
                <h2>@TITLE</h2>
              </div>
              <div align="center"><img src="@PICTURE" alt="GroupDiscription" width="200" /></div>
              <div>
                <h3>Description</h3>
              </div>
              <div>
                <p>@DESCRIPTION</p>
              </div>
              <div>
                <h3>@ASSOCIATION</h3>
              </div>
              <div>
                <ul>
                  <li> @COURSEHEADING @COURSE <a href="#">@NAME</a> </li>
                  <li> @PROJECTHEADING @PROJECT </li>
                </ul>
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



$HTMLform_group_activity_block = t(<<<END

<div id="AgendaActivitiesAndNotisfication">
    	<div class="TopRightOrangeCorner">
    		<div class="TopLeftOrangeCorner">
    			<div class="AgendaActivitiesAndNotisfication">
    				<div class="AgendaIconDiv"> <img src="@BASE_PATHthemes/Boldr/Images/AgendaActivitiesAndNotisficationIcon.jpg" alt="Activities" /> </div>
    				<div class="AgendaOrangeBoxHeading">
    					<h2>@TITLE</h2>
    				</div>
    			</div>
    		</div>
    	</div>
    	<div class="ActivitiesMainContent">
    		<table cellpadding="0" cellspacing="0" border="0" width="218px">
    			<tr>
    				<td class="MiddleLeftOrangeCorner">&nbsp;</td>
    				<td class="MiddleCenterOrangeCorner">
    					<ul class="Activities">
    						<li>@TOTAL_MEMBERS &nbsp;<span style="font-size:12px">Members</span></li>
    						<li>@TOTAL_POSTS &nbsp;<span style="font-size:12px">Posts</span></li>
    					</ul>
    				</td>
    				<td class="MiddleRightOrangeCorner">&nbsp;</td>
    			</tr>
    			<tr>
    				<td class="BottomLeftOrangeCorner">&nbsp;</td>
    				<td class="BottomCenterOrangeCorner">&nbsp;</td>
    				<td class="BottomRightOrangeCorner">&nbsp;</td>
    			</tr>
    		</table>
    	</div>
    </div>

END
);


$HTMLform_group_admin_block = t(<<<END
   
<div id="AgendaActivitiesAndNotisfication">
      <div class="TopRightOrangeCorner">
        <div class="TopLeftOrangeCorner">
          <div class="AgendaActivitiesAndNotisfication">
            <div class="AgendaIconDiv"> <img src="@BASE_PATHthemes/Boldr/Images/GroupsAdminLogo.jpg" alt="Admin" /> </div>
            <div class="AgendaOrangeBoxHeading">
              <h2>Admins</h2>
            </div>
          </div>
        </div>
      </div>
      <div class="ActivitiesMainContent">
        <table cellpadding="0" cellspacing="0" border="0" width="218px">
          <tr>
            <td class="MiddleLeftOrangeCorner"></td>
            <td class="MiddleCenterOrangeCorner">
                    	@ADMIN_DETAIL
                  <div class="Adminspacer">&nbsp;</div>
              </td>
            <td class="MiddleRightOrangeCorner"></td>
          </tr>
          <tr>
            <td class="BottomLeftOrangeCorner"></td>
            <td class="BottomCenterOrangeCorner"></td>
            <td class="BottomRightOrangeCorner"></td>
          </tr>
        </table>
      </div>
    </div>

END
);


$HTMLform_group_link_block = t(<<<END
<div id="AgendaActivitiesAndNotisfication">
    	<div class="TopRightOrangeCorner">
    		<div class="TopLeftOrangeCorner">
    			<div class="AgendaActivitiesAndNotisfication">
    				<div class="AgendaIconDiv"> <img src="@BASE_PATHthemes/Boldr/Images/GroupsLinksIcon.jpg" alt="Links" /> </div>
    				<div class="AgendaOrangeBoxHeading">
    					<h2>Links</h2>
    				</div>
    			</div>
    		</div>
    	</div>
    	<div class="ActivitiesMainContent">
    		<table cellpadding="0" cellspacing="0" border="0" width="218px">
    			<tr>
    				<td class="MiddleLeftOrangeCorner">&nbsp;</td>
    				<td class="MiddleCenterOrangeCorner">
    					<ul class="Activities">
                                            <!-- LINKS -->
    					</ul>
                                        <div align="right"><!-- EDIT_LINK --></div>
    				</td>
    				<td class="MiddleRightOrangeCorner">&nbsp;</td>
    			</tr>
    			<tr>
    				<td class="BottomLeftOrangeCorner">&nbsp;</td>
    				<td class="BottomCenterOrangeCorner">&nbsp;</td>
    				<td class="BottomRightOrangeCorner">&nbsp;</td>
    			</tr>
    		</table>
    	</div>
    </div>

END
);



$HTMLGroupResources_filter_heading = t(<<<EOD
<tr>
                            <td class="StatusHeading" style="white-space:nowrap;width:120px">@HEADING</td>
                            <td>
EOD
);

$HTMLGroupResources_filter_row_end = t(<<<EOD
 </td>
                          </tr>
                          <tr>
                            <td colspan="2" class="yellowDottedLine">&nbsp;</td>
                          </tr>
EOD
);

$HTMLGroupResources_filter_cellStart = t(<<<EOD
        <div class="ChkBox">
EOD
);

$HTMLGroupResources_filter_cellEnd = t(<<<EOD
        </div>
EOD
);
$HTMLGroupResources_filter = t(<<<EOD
<div id="BoldrProject" style="width:845px;">
  <div class="ProjectResources" style="width:845px;">
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
                                  <a href="#">&laquo;Close</a>
                                </div></td>
                            </tr>
                            <tr>
                              <td><table cellpadding="0" class="Project-ActivitiesStatusType"  cellspacing="0" width="100%" border="0">
                                        @OPTIONS
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
     </div>
   </div>
</div>
EOD
);

//$HTMLGroupResourcesFilter = t(<<<EOD
//    <div id="BoldrProject">
//      <div class="ProjectResources">
//        <div class="ProjectResources-Content" style="padding:0px">
//          <div class="ProjectResources-Activities">
//        	 <div class="Project-ActivitiesFilter" style="padding:0px;">
//              <div class="ProjectResources-YellowBottomRightCorner">
//                <div class="ProjectResources-YellowBottomLeftCorner">
//                  <div  class="ProjectResources-YellowTopRightCorner">
//                    <div  class="ProjectResources-YellowTopLeftCorner">
//                      <table cellpadding="0" cellspacing="0" border="0" width="100%">
//                        <tr>
//                          <td><div class="Project-ActivitiesFilterHeading">
//                              <h4>
//                                Filter
//                              </h4>
//                            </div>
//                            <div class="Project-ActivitiesFilterCloseLink">
//                              <a href="#">
//                                &laquo;
//                                Close
//                              </a>
//                            </div></td>
//                        </tr>
//                        <tr>
//                          <td><table cellpadding="0" class="Project-ActivitiesStatusType" cellspacing="0" width="100%" border="0">
//                              <tr>
//                                <td class="StatusHeading" style="white-space:nowrap">Status</td>
//                                <td>
//                                  <div class="ChkBox">
//                                    <input name="status" checked="radio" type="checkbox" />
//                                    All
//                                  </div>
//                                  <div class="ChkBox">
//                                    <input name="status" type="radio" />
//                                    Flagged
//                                  </div>
//                                </td>
//                              </tr>
//                              <tr>
//                                <td colspan="2" class="yellowDottedLine">&nbsp;</td>
//                               </tr>
//                              <tr>
//                                <td class="StatusHeading" style="white-space:nowrap">Type</td>
//                                <td>
//                                  <div class="ChkBox">
//                                    <input name="status" checked="checkbox" type="checkbox" />
//                                    All
//                                  </div>
//                                  <div class="ChkBox">
//                                    <input name="status" checked="checkbox" type="checkbox" />
//                                    Links
//                                  </div>
//                                  <div class="ChkBox">
//                                    <input name="status" checked="checkbox" type="checkbox" />
//                                    Images
//                                  </div>
//                                  <div class="ChkBox">
//                                    <input name="status" checked="checkbox" type="checkbox" />
//                                    Videos
//                                  </div>
//                                  <div class="ChkBox">
//                                    <input name="status" checked="checkbox" type="checkbox" />
//                                    Documents
//                                  </div>
//                                  <div class="ChkBox">
//                                    <input name="status" checked="checkbox" type="checkbox" />
//                                    Pages
//                                  </div>
//                                </td>
//                              </tr>
//                              <tr>
//                                <td colspan="2" class="yellowDottedLine">&nbsp;</td>
//                               </tr>
//                            </table>
//                          </td>
//                        </tr>
//                      </table>
//                    </div>
//                  </div>
//                </div>
//              </div>
//            </div>
//          </div>
//        </div>
//      </div>
//    </div>
//    <div style="font-size:10px">&nbsp;</div>
//EOD
//);