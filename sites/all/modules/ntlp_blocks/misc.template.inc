<?php

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

$HTMLGroups_row = t(<<<EOD
    <tr>
            <td><h3>@GROUP_TITLE</h3><span style = "color:#666666;font-size:11px;font-weight:bold;">@NO_GROUP_FOUND</span></td><td align="right">@NEW_COUNT @NEW_POST</td>
    </tr>
EOD
);

$HTMLGroups = t(<<<EOD
<div class="HomePageGroups">
    <div id="GroupsAndEvents">
        <div class="GroupsAndEventsHeadingBg">
                <h2>Groups</h2>
        </div>
        <div class="GroupsAndEventsTopRightCorner"></div>
        <div class="clearAll"></div>
        <div class="GroupAndEventsRightBottomCorner">
            <div class="GroupAndEventsLeftBottomCorner" style="position:relative">
            <div style="position:absolute;bottom:10px;right:10px">
                <a href="@VIEW_ALL_GROUPS" class="greenLink">View All Groups >></a>
            </div>
                <table class="GroupsData" cellpadding="0" cellspacing="0" border="0" width="100%">
                        @GROUPS_ROW_PLACEHOLDER
                </table>
            </div>
        </div>
    </div>
</div>
EOD
);
