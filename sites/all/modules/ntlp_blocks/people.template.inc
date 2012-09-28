<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTMLcontainer = t(<<<END
   
<div id="BoldrProject">
  <div class="ProjectResources">
    <div class="ProjectResources-Content" style="padding:0px">
      <div class="ProjectResources-Activities">
        <div style="display:block">
          <table>
            <tr>
              <td><input id="search-text" class="persons_name_search" value="Who are you looking for?" title="Who are you looking for?" style="width:200px"  /></td>
              <td><input type="button" value="Search" class="SearchBtn" onclick="people_search()" /></td>
            </tr>
          </table>
        </div>
        <div style="font-size:5px;">
          &nbsp;
        </div>
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

<a class="filter_close" >&laquo; Close</a>
                       </div></td>
                    </tr>
                    <tr>
                      <td><table cellpadding="0" class="Project-ActivitiesStatusType filter_hide" cellspacing="0" width="100%" border="0">
                          @ROW_ORGANIZATION
                          <tr>
                            <td colspan="2" class="yellowDottedLine">&nbsp;</td>
                          </tr>
                          <tr>
                            <td class="TypeHeading" style="white-space:nowrap">@ROLE_HEADING</td>
                            <td>
@FILTER_ROLES
                              </td>
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
<div style="font-size:10px">
  &nbsp;
</div>
<div id="view-people">
@PEOPLE_VIEW
</div>
END
);

$HTMLorg = t(<<<END
      <tr>
        <td class="StatusHeading" style="white-space:nowrap">Organizations</td>
        <td><div class="ChkBox">
            <input type="hidden" value="@DEFAULT_ORG" id="ppl-filter-org" />
           @ALL_CHECKBOX
          </div>
          <div class="ChkBox">
            <input name="filter-org" type="checkbox" checked="1" onclick="people_set_org('@MYSCHOOL', this.checked)" />
            My School
          </div>
          @OTHER_NETWORK_SCHOOL
          @NETWORK_NATIONAL
</td>
      </tr>
END
);
