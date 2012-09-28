<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTML_courses_project_filter_heading = t(<<<EOD
        <div class="Status">
                                    <div><b>@HEADING</b></div>
EOD
);
$HTML_courses_project_filter_row_end = t(<<<EOD
        </div>
EOD
);
$HTML_courses_project_filter_cellStart = t(<<<EOD
        <div>
EOD
);
$HTML_courses_project_filter_cellEnd = t(<<<EOD
        </div>
EOD
);
$HTML_courses_project_filter = t(<<<EOD
    <div class="Filter">
        <div class="yellowTopRightCorner">
            <div class="yellowBottomRightCorner">
                <div class="yellowBottomLeftCorner">
                    <div class="yellowTopLeftCorner">
                        <table cellpadding="0" cellspacing="0" width="100%" border="0" >
                        <tr>
                            <td>
                                <div>
                                     <div class="FilterHeading">
                                            <h5>Filter</h5>
                                     </div>
                                     <div class="CloseLink">
                                           <a href="#" class="filter_link">&laquo; Close</a>
                                     </div>
                                 </div>
                                <div class="clearall"></div>
                                 <div class="filter_hide">@OPTIONS</div>
                            </td>
                         </tr>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
<div style="clear:both"></div>
EOD
);

$HTMLcontainer = t(<<<END

<style>
.views-field-title {
    width:300px;
}
.views-table th {
    width:300px;
}
</style>
<div id="ActivitiesCSS">
	<div class="HeadingPanel" style="float:inherit">
		<div class="Headings" style="float:inherit">
			<div class="Heading" style="float:left"><b>Projects</b></div>
			<div class="Project-Btns" onmousedown=this.className="Project-BtnsClicked" onmouseup=this.className="Project-Btns">

                           @LINK_NEW
                        </div>
		</div>
	</div>
<br clear="all" />
<br clear="all" />
<div id="ActivitiesCSS">
									@FILTER_PLACEHOLDER
									<br clear="all" />
									<div class="ReportView" style="width:100% !important">
@PROJECTS_VIEW
									</div>
								</div>
								</div>

END
);


$HTML_All_Active_Projects = t(<<<END

<div id="AgendaActivitiesAndNotisfication">
        	<div class="TopRightOrangeCorner">
        		<div class="TopLeftOrangeCorner">
        			<div class="AgendaActivitiesAndNotisfication">
        				<div class="AgendaOrangeBoxHeading">
        					<h2>Active Projects</h2>
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
        						@LIST_PROJECTS
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


$HTML_library_landing_page = t(<<<END
 <style>
       .boldr_table {
        margin-top:63px;
       }
       </style>
<div style="position:absolute;top:109px;margin-left:-18px">
    <div class="BoldrBrandCrumb">
        <ul>
            <li><a href="?q=ntlp/home" class="greenLink">Home</a>&nbsp;>&nbsp;</li>
            <li style="color:#009900 !important">Library</li>
        </ul>
    </div>

    <div class="BoldrHeading"  style="clear:both;margin-top:20px">
    	<h1 class="pageHeading">Library</h1>
    </div>
</div>
<table cellpadding="0" cellspacing="0" border="0" width="100%">
<tr>
    <td align="center">

<table>
<tr>
    <td @1WIDTH>

    <div id="GroupsAndEvents" @DISPLAY1>

<div class="GroupsAndEventsHeadingBg" style="width:265px;">
	<h2><a href="?q=ntlp/document/resource/library">School Resource Library</a></h2>
</div>
<div class="GroupsAndEventsTopRightCorner"></div>
<div class="clearAll"></div>
<div class="GroupAndEventsRightBottomCorner">
<div class="GroupAndEventsLeftBottomCorner" style="padding-top:5px;height:auto !important">
	<div class="totalItems">
    </div>
    <div style="font-size:14px;padding-top:5;padding-bottom:5px;">
		Documents and other resources shared by your school community.
	</div>
       
</div>
</div>
</div>
    </td>
    <td width="279px">
    <div id="GroupsAndEvents" width="280px">
<div class="GroupsAndEventsHeadingBg" style="width:275px;">
	<!-- <h2><a href="?q=ntlp/resource/library/">Network Resource Library</a></h2> -->
	 <h2><a href="@NETWORK_RESOURCE_LIB_LINK">Network Resource Library</a></h2>
</div>
<div class="GroupsAndEventsTopRightCorner"></div>
<div class="clearAll"></div>
<div class="GroupAndEventsRightBottomCorner">
<div class="GroupAndEventsLeftBottomCorner" style="padding-top:5px;height:auto !important">
	<div class="totalItems">
    </div>
    <div style="font-size:14px;padding-top:5;padding-bottom:5px;">
		Resources shared by Network Staff for educators across the Network.
	</div>

</div>
</div>
</div>
    </td>
</tr>
</table>
</td>
</tr>
</table>

END
);

?>
