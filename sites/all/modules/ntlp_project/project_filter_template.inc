<?php
$HTMLCourseProjectFilter = t(<<<EOD
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
                                    Published
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Hidden
                                  </div>
                                  </td>
                              </tr>
                              <tr>
                                <td colspan="2" class="yellowDottedLine">&nbsp;</td>
                              </tr>
                              <tr>
                                <td class="TypeHeading" style="white-space:nowrap">Unit/Topic</td>
                                <td>
                                  <div class="ChkBox">
                                    <input name="type" value="All" checked="checked" type="checkbox" />
                                    All
                                  </div>
                                  @OPTIONS_PLACEHOLDER
                                </td>
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

$HTMLCourseProjectFilter_option = t(<<<EOD
    <div class="ChkBox">
        <input name="type" value="@OPTION_VALUE" type="checkbox" />@OPTION_TITLE
    </div>
EOD
);

$HTMLProjectActivitiesFilter = t(<<<EOD
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
                                    Current
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" type="checkbox" />
                                    Expired
                                  </div>
                                  </td>
                              </tr>
                              <tr>
                                <td colspan="2" class="yellowDottedLine">&nbsp;</td>
                              </tr>
                              <tr>
                                <td class="TypeHeading" style="white-space:nowrap">Type</td>
                                <td>
                                  <div class="ChkBox">
                                    <input name="type" value="All" checked="checked" type="checkbox" />
                                    All
                                  </div>
                                  <div class="ChkBox">
                                    <input name="type" value="All" checked="checked" type="checkbox" />
                                    Assignment
                                  </div>
                                  <div class="ChkBox">
                                    <input name="type" value="All" checked="checked" type="checkbox" />
                                    Journal
                                  </div>
                                  <div class="ChkBox">
                                    <input name="type" value="All" checked="checked" type="checkbox" />
                                    Quiz
                                  </div>
                                  <div class="ChkBox">
                                    <input name="type" value="All" checked="checked" type="checkbox" />
                                    Project Evaluation
                                  </div>
                                  <div class="ChkBox">
                                    <input name="type" value="All" checked="checked" type="checkbox" />
                                    Team Evaluation
                                  </div>
                                  <div class="ChkBox">
                                    <input name="type" value="All" checked="checked" type="checkbox" />
                                    Poll
                                  </div>
                                  <div class="ChkBox">
                                    <input name="type" value="All" checked="checked" type="checkbox" />
                                    Survey
                                  </div>
                                </td>
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