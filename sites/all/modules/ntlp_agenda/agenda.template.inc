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
			<h1>Agenda</h1>
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
					<h2>@AGENDA_TITLE</h2>
					<ul>@AGENDA_BODY&nbsp;<div class="AgendaViewMoreViewAll" id="agendaViewMoreLink" style="display:none;" >@VIEW_MORE_LINK</div></ul>
					<div align="right" class="AgendaViewMoreViewAll">
                                        @VIEW_ALL_LINK&nbsp;&nbsp;&nbsp;&nbsp; </div>
				</div>
			</div>
		</div>
	</div>
</div>
END
);

$HTMLmain_editsave = t(<<<END
        <input @DISPLAY_EDIT onmousedown=this.className="SaveBtnClicked" onmouseup=this.className="SaveBtn"  type="button" id="agenda-edit" onclick="edit_agenda()" class="SaveBtn" value="Edit" />
        <input @DISPLAY_SAVE type="button" id="agenda-save" onclick="save_agenda()" class="SaveBtn" value="Save" onmousedown=this.className="SaveBtnClicked" onmouseup=this.className="SaveBtn" />
END
);

$HTMLmain_container = t(<<<END
<div id="Courses-Agenda">
  <table cellpadding="0" cellspacing="0" border="0" width="840px">
    <tr>
      <td colspan="3" align="right">@LINK_EDIT</td>
    </tr>
    <tr>
      <td colspan="3"  style="height:10px;font-size:10px;">&nbsp;</td>
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
      <td style="width:10px">&nbsp;</td>
      <td valign="top"><div class="C-ATopRightCorner">
          <div class="C-ABottomRightCorner">
            <div class="C-ABottomLeftCorner">
              <div class="C-ATopLeftCorner" >
                <div class="custom_list_agenda_main" id="agenda-body">
              @AGENDA_BODY
                </div>
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
    <table cellpadding="0" class="Calender" cellspacing="0" border="0" width="100%">
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

$HTMLmain_agenda_body = t(<<<END
                <table cellpadding="0" cellspacing="0" border="0" width="100%">
                  <tr>
                    <td class="Heading"><h2>Agenda</h2>
                      <h3>@TODAY_DAY<span>&nbsp;|&nbsp;</span>@TODAY_DATE</h3></td>
                    <td align="right" class="Btn" style="width:200px">
                    <!--//
                    <a href="#" class="PrintBtn">&nbsp;</a> <a href="#" class="ExportBtn">&nbsp;</a> <a href="#" class="CopyBtn">&nbsp;</a>
                    //-->
                    </td>
                  </tr>
                  <tr>
                    <td colspan="2" class="SubHeading"><h5>@AGENDA_TITLE</h5></td>
                  </tr>
                  <tr>
                    <td colspan="2">
                        <ul>@AGENDA_BODY</ul>
                    </td>
                  </tr>
                </table>
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

?>
