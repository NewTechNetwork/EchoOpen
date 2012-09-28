<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTMLHeader = t(<<<END
<!--Share Stuff-->
<div id="AgendaShareStuff" style-"text-align:left;">
	<!-- Agenda ShareStuff Content -->
	<div id="AgendaShareStuffContent" style="padding:0 0 0 7px;">
        <div class="resourcesDarkBlueTopLeft">
        <div class="resourcesDarkBlueTopRight">
        <div class="resourcesDarkBlueBottomLeft">
        <div class="resourcesDarkBlueBottomRight">
<div style="padding:7px 0 0 7px">
<div class="WhiteBoxMainDiv">
END
);


$HTMLFooter = t(<<<END
</div></div></div></div></div></div>
			</div>
		</div>

END
);


$HTMLContent = t(<<<END
							@COMMENTS
                            @ADDCOMMENTSFORM

END
);

$HTMLContent_STAFF_NETWORK = t(<<<END
                            @ADDCOMMENTPOST
                            @ADDREVIEWPOST

END
);

$HTMLProjectLibrarySearchFilter_header = t(<<<EOD
                            <div class="StatusHeading" style="width:auto;padding-top:10px;padding-bottom:10px;"> @HEADING</div>
EOD
);

$HTMLProjectLibrarySearchFilter = t(<<<EOD
   <div style="margin-top: 8px; width: 100%;" id="BoldrProject">
      <div style="width: auto;" class="ProjectResources">
        <div style="padding: 0px;" class="ProjectResources-Content">
          <div class="ProjectResources-Activities">
        	 <div style="padding: 0px;" class="Project-ActivitiesFilter">
              <div style="width: auto;" class="ProjectResources-YellowBottomRightCorner">
                <div class="ProjectResources-YellowBottomLeftCorner">
                  <div class="ProjectResources-YellowTopRightCorner">
                    <div class="ProjectResources-YellowTopLeftCorner">
                      <table cellspacing="0" cellpadding="0" border="0" width="100%">
                        <tbody><tr>
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
                            <table cellspacing="0" cellpadding="0" border="0" width="100%" class="Project-ActivitiesStatusType filter_hide">
                        <tbody><tr>
                            <td>
				<div>
                                    <div class="clearall"></div>

                                </div>
                                @FILTER_OPTIONS

				</td>
			</tr>
                         </tbody></table>
				</td>
			</tr>
                      </tbody></table>
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

$HTMLProjectLibrarySearchFilter_cellStart = t(<<<EOD
        <div>
EOD
);

$HTMLProjectLibrarySearchFilter_cellEnd = t(<<<EOD
        </div>
EOD
);

$HTMLProjectLibrarySearchFilter_rowEnd = t(<<<EOD

<br>
EOD
);

