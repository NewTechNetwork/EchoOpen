<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTMLform_resource_hdr = t(<<<END

<div id="ActivitiesCSS">
    <div class="HeadingPanel">
        <div class="Headings">
          
        </div>
    </div>
</div>

<div style="position:relative">
<table cellpadding="0"  cellspacing="0" >
    <tr>
        <td>
            <div class="Heading"><b></b></div>
        </td>
        <td>
            <div class="Heading">
                <div style ="color:#3570AA; font-size:18px; padding:0px; margin:0 0 10px 0">@RESOURCE_TITLE</div>
            </div>
        </td>
    <tr>
</table>
<div id="ActivitiesCSS">
    <div class="LightBlueBottomRightCorner">
        <div class="LightBlueBottomLeftCorner">
            <div class="LightBlueTopRightCorner">
                <div class="LightBlueTopLeftCorner">
                  <div class="ActivitiesAssignment">
                   <table cellpadding="8" cellspacing="0" border="0" width="820px" class="table clearfix">

END
);


$HTMLform_resource_ftr = t(<<<END
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
</div>
END
);


$HTMLform_groupbox_hdr = t(<<<END

<div id="ActivitiesCSS">
	<div class="HeadingPanel">
		<div class="Headings">
			<div class="BoldrBrandCrumb"><a href="@ACTIVITIES_LINK">« Back to Resources</a></div> 
  </div>

	</div>
</div>
<br clear="all" />

<table cellpadding="0"  cellspacing="0" ><tr><td><div class="Heading"><b></b></div></td><td><div class="Heading"><h2 style ="color:#3570AA;font-size:20px">Add Resource </h2></div></td><tr></table>

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
		<div class="AgendaShareStuffIcon">
		</div>
		<div class="AgendaShareStuffHeading">
                    @ALL_OR_SHARE
                    @ALL_STUFF_LINK
		</div>
		<div class="AgendaShareStuffTopRightCorner">
		</div>
	</div>
        <div style="clear:both"></div>
	<div id="AgendaShareStuffContent">
<div style="background-color:#3570aa;display:table;width:100%">
<table width="100%" border="0" cellpadding="0" cellspacing="0">

END
);


$HTMLform_resource_groupbox_ftr = t(<<<END
   </table>
   </div>
   </div>
   </div>
END
);

$HTMLSearchFilter = t(<<<EOD
   <div style="margin-top: 8px; width: 100%; margin-left: 15px;" id="BoldrProject">
      <div style="width: 215px;" class="ProjectResources">
        <div style="padding: 0px;" class="ProjectResources-Content">
          <div class="ProjectResources-Activities">
        	 <div class="Project-ActivitiesFilter">
              <div style="width: auto;" class="ProjectResources-YellowBottomRightCorner">
                <div class="ProjectResources-YellowBottomLeftCorner">
                  <div class="ProjectResources-YellowTopRightCorner">
                    <div class="ProjectResources-YellowTopLeftCorner">
                        <div class="blue_heading"> Filters </div>
                      <table cellspacing="0" cellpadding="0" border="0" width="100%">
                        <tbody>
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

?>
