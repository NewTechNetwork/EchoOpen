<?php

$HTMLGroups_row = t(<<<EOD
    <tr>
            <td><h3>@GROUP_TITLE</h3></td><td align="right">@NEW_COUNT New Posts</td>
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
