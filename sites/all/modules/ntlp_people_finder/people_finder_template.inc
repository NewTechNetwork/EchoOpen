<?php
$HTMLPeopleFinderFilter = t(<<<EOD
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
                              <a href="#">
                                &laquo;
                                Close
                              </a>
                            </div></td>
                        </tr>
                        <tr>
                          <td><table cellpadding="0" class="Project-ActivitiesStatusType" cellspacing="0" width="100%" border="0">
                              <tr>
                                <td class="StatusHeading" style="white-space:nowrap">Status</td>
                                <td><div class="ChkBox">
                                    <input name="status" checked="checked" type="checkbox" />
                                    All
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Student
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Teacher
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Parent
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Admin
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Partner
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Network Staff
                                  </div>
                                  </td>
                              </tr>
                              <tr>
                                <td colspan="2" class="yellowDottedLine">&nbsp;</td>
                              </tr>
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



$HTMLPeopleFinder_filter_heading = t(<<<EOD
    <table>
        <tr>
            <th align="left" style="width: 50px;">@HEADING</th>
EOD
);
$HTMLPeopleFinder_filter_row_end = t(<<<EOD
        </tr>
    </table>
EOD
);
$HTMLPeopleFinder_filter_cellStart = t(<<<EOD
        <td style="font-weight: normal;">
EOD
);
$HTMLPeopleFinder_filter_cellEnd = t(<<<EOD
        </td>
EOD
);
$HTMLPeopleFinder_filter = t(<<<EOD
      <div style="font-size:11px;">&nbsp;</div>
   <div class="filter">
                <div class="top_left_fltr">
                  <div class="top_right_fltr">
                    <div class="bottom_left_fltr">
                      <div class="bottom_right_fltr">
                        <div class="filter_heading_panel">
                          <h3>
                            Filter
                          </h3>
                        </div>
                        <div class="filter_content_panel">
                          <table cellpadding="0" cellspacing="0" width="100%" border="0">
                              <tr>
                                <td>@OPTIONS</td>
                              </tr>
                              <tr>
                                <th colspan="10" style="height:35px" align="left">
                                    <div style="float: left">@SELECT_SCHOOL_LABEL</div>
                                    <div style="float: left; margin-left: -5px; margin-top: -5px;">@SCHOOL_SELECTOR</div>
                                </th>
                              </tr>
                            </table>
                        </div>
                      </div>
                    </div>
                  </div>
                </div>
              </div>
EOD
);
