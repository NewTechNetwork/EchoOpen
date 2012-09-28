<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTMLcontainer = t(<<<END
<!--Agenda-->
<div id="AgendaMainPanel">
	<div class="AgendaHeading">
		<div class="AgendaHeadingIcon"></div>
		<div class="AgendaHeadingData">
			<h1>Events</h1>
		</div>
		<div class="AgendaHeadingLinksPanel">
                    <div class="AgendaHeadingLinksPanelLeftBg"> </div>
                    <div class="AgendaHeadingLinksPanelCenterBg">
                        <div>
                            <ul>
                                <li class="first">@DAY1_LINK</li>
                                <li>@DAY2_LINK</li>
                                <li>@DAY3_LINK</li>
                                <li>@DAY4_LINK</li>
                                <li class="last">@DAY5_LINK</li>
                            </ul>
                        </div>
                    </div>
                    <div class="AgendaHeadingLinksPanelRightBg"> </div>
		</div>
	</div>
	<div>
		<div class="AgendaInnerContent">
			<div class="AgendaInnerLeftCornerBg">
				<div>
					<h2>@EVENTS_TITLE</h2>
					<ul>@EVENTS_BODY&nbsp;<div class="AgendaViewMoreViewAll">@VIEW_MORE_LINK</div></ul>
					<div align="right" class="AgendaViewMoreViewAll">
                                        @VIEW_ALL_LINK&nbsp;&nbsp;&nbsp;&nbsp; </div>
				</div>
			</div>
		</div>
	</div>
</div>
END
);

$HTMLmain_container = t(<<<END
<div id="Courses-Agenda">

    <div class="dialog-confirm-ntk dialog-confirm" style="background-color:#fff;display:none;" title="Delete Event">
        <p>You are about to permanently delete this event.</p>
        <p>Are you sure you want to permanently delete this item?</p>
    </div>

  <table cellpadding="0" cellspacing="0" border="0" width="840px">
    <tr>
      <td colspan="3" align="right">
        <!-- ADD EVENT -->
        <!-- EDIT EVENT -->
        <input type="button" id="events-save" onclick="save_events()" style="display:none" class="SaveBtn" value="Save" />
      </td>
    </tr>
    <tr>
      <td colspan="3" style="height:10px;font-size:10px;">&nbsp;</td>
    </tr>
    <tr>
      <td valign="top" style="width:219px">
        <table cellpadding="0" class="Calender" cellspacing="1" border="0" width="210px">
          <tr>
            <td class="MonthsAndYears">
                <a href="#" style="cursor: pointer; cursor: hand;" class="LeftArrow previous">&nbsp;</a>
                <span id="current-title">@CALENDAR_TITLE</span>
                <a href="#" style="cursor: pointer; cursor: hand;" class="RightArrow next">&nbsp;</a>
                <input type="hidden" id="current-day" value="@CALENDAR_DD" />
                <input type="hidden" id="current-month" value="@CALENDAR_MM" />
                <input type="hidden" id="current-year" value="@CALENDAR_YY" />
            </td>
          </tr>
          <tr><td>
            <div id="calendar">@CALENDAR_BODY</div>
          </td></tr>
        </table>
      </td>
      <td style="width:10px;">&nbsp;</td>
      <td valign="top"><div class="C-ATopRightCorner">
          <div class="C-ABottomRightCorner">
            <div class="C-ABottomLeftCorner">
              <div class="C-ATopLeftCorner" id="events-body">
              @EVENTS_BODY
              </div>
            </div>
          </div>
        </div></td>
    </tr>
  </table>
</div>
END
);

$HTMLmain_calendar_body = t(<<<END
    <table cellpadding="0" class="Calender" cellspacing="1" border="0" width="100%">
      <tr class="Days">
        <td>SUN</td>
        <td>MON</td>
        <td>TUE</td>
        <td>WED</td>
        <td>THU</td>
        <td>FRI</td>
        <td>SAT</td>
      </tr>
      @CALENDAR_DAYS
    </table>
END
);

$HTMLmain_events_body = t(<<<END
   <table cellpadding="0" cellspacing="0" border="0" width="100%">
      <tr>
        <td class="Heading"><h2>Events</h2>
          <h3>@TODAY_DAY<span>&nbsp;|&nbsp;</span>@TODAY_DATE</h3></td>
        <td style="float:right;display:none" class="Btn"><a href="#" class="PrintBtn">&nbsp;</a></td>
        <td style="float:right;margin-top:5px;margin-right:10px">&nbsp; @DELETE &nbsp;</td>
      </tr>
      <tr>
        <td colspan="2" class="SubHeading"><h5>@EVENTS_TITLE</h5></td>
      </tr>
      <tr>
        <td colspan="2" >
            <ul>@EVENTS_BODY</ul>
        </td>
      </tr>
    </table>
END
);

$HTMLbody_events_list = t(<<<END
    <table cellpadding="0" cellspacing="0" border="0" width="100%">
      <tr>
        <td class="Heading"><h2>Events</h2>
          <h3>@TODAY_DAY<span>&nbsp;|&nbsp;</span>@TODAY_DATE</h3></td>
        <td style="float:right;display:none" class="Btn">@PRINT</td>
      </tr>
      <!-- FILTER START -->
      <tr class="filter_hide">
        <td colspan="2" class="SubHeading">
            <div id="BoldrProject"  style="width:590px">
      <div class="ProjectResources" style="width:590px">
        <div class="ProjectResources-Content" style="padding:0px">
          <div class="ProjectResources-Activities">
        	 <div class="Project-ActivitiesFilter" style="padding:0px;">
              <div class="ProjectResources-YellowBottomRightCorner_events">
                <div class="ProjectResources-YellowBottomLeftCorner_events">
                  <div  class="ProjectResources-YellowTopRightCorner_events">
                    <div  class="ProjectResources-YellowTopLeftCorner_events">
                      <table cellpadding="0" cellspacing="0" border="0" width="100%">
                        <tr>
                          <td><div class="Project-ActivitiesFilterHeading">
                              <h4>
                                Filter
                              </h4>
                            </div>
                            <div class="Project-ActivitiesFilterCloseLink">
                              <a id="close_filter" onclick='close_filter()'>
                                &laquo;
                                Close
                              </a>
                            </div></td>
                        </tr>
                        <tr>
                          <td><table cellpadding="0" class="Project-ActivitiesStatusType" cellspacing="0" width="100%" border="0">
                              <tr>
                                <td class="StatusHeading" style="white-space:nowrap">Type</td>
                                <td>
                                  <div id="event-filters">
                                  <div class="ChkBox">
                                    <input name="status" checked="checked" onclick="onchange_filters()" type="checkbox" value="all">All</input>
                                  </div>
                                  <div class="ChkBox" style="display:none">
                                    <input name="status" onclick="onchange_filters()" type="checkbox" value="ntlp_course_event">Courses</input>
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" onclick="onchange_filters()" type="checkbox" value="ntlp_group_event">Groups</input>
                                  </div>
                                  <div class="ChkBox">
                                    <input name="status" onclick="onchange_filters()" type="checkbox" value="ntlp_school_event">School</input>
                                  </div>
                                   <div class="ChkBox">
                                    <input name="status" onclick="onchange_filters()" type="checkbox" value="ntlp_network_event">Network</input>
                                  </div>
                                  </div>
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
            <div class="clearall"></div>
            <br clear="all" />
        </td>
      </tr>
      <!-- FILTER END -->
      <tr>
        <td colspan="2" class="datalist" >
            <table width="100%" border="0" cellspacing="2" cellpadding="0">
            @EVENTS_BODY
            </table>
        </td>
      </tr>
    </table>
END
);

$HTMLbody_event = t(<<<END

<!--Share Stuff-->
    
      <tr>
        <td style="border-bottom:1px solid #8E948E;padding:15px 0 15px 0">
        <!-- group icon "ntlp_group_icon" school icon "ntlp_school_icon" network icon "ntlp_network_icon"  -->
<table width="100%" style="width:100%" cellpadding="0" cellspacing="0"><tr><td style="width:40px"><div class="@EVENT_TYPE" style="width: 40px;"></div></td><td>
<a class="greenLink" href="#" onclick="show_event(@EVENT_NID)">@EVENT_TITLE</a><br /><span style="font-size:10px;color:#71797f;">@GROUP_NAME</span>
</td><td align="right" style="width:14px">@DELETE</td></tr></table>
        </td>
      </tr>
END
);

$HTMLmain_calendar_row = t(<<<END
      <tr class="@CLASS_ROW">
        <td>@DAY_1</td>
        <td>@DAY_2</td>
        <td>@DAY_3</td>
        <td>@DAY_4</td>
        <td>@DAY_5</td>
        <td>@DAY_6</td>
        <td@CLASS_CELL>@DAY_7</td>
      </tr>
END
);

$HTMLEvents_row = t(<<<EOD
    <tr>
            <td><h5><a href="@EVENT_URL">@EVENT_TITLE</a></h5><span>@EVENT_WHOM</span></td><td class="Date" align="right">@EVENT_DATE</td>
    </tr>
EOD
);

$HTMLEvents = t(<<<EOD
<div id="GroupsAndEvents">
    <div class="GroupsAndEventsHeadingBg">
            <h2>Events</h2>
    </div>
    <div class="GroupsAndEventsTopRightCorner"></div>
    <div class="clearAll"></div>
    <div class="GroupAndEventsRightBottomCorner">
        <div class="GroupAndEventsLeftBottomCorner" style="position:relative">
            <div style="position:absolute;bottom:10px;right:10px">
                <a href="@VIEW_ALL_LINK" class="greenLink">View All Events >></a>
            </div>
            <table class="EventsData" cellpadding="0" cellspacing="0" border="0" width="100%">
                    @EVENTS_ROW_PLACEHOLDER
                  
            </table>
        </div>
    </div>
</div>
EOD
);


?>
