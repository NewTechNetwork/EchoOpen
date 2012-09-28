<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTMLGroupFilter = t(<<<EOD
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
