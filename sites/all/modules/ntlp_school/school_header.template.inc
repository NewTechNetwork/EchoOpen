<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTMLcontainer = t(<<<END
<div>
    <div class="BoldrBrandCrumb">
    <ul>
        <li>@LINK_HOME >&nbsp;</li>
        <li>@LINK_SCHOOLMANAGEMENT >&nbsp;</li>
    </ul>
    </div>
    <br clear="all" />
     <div style="position: relative;">
        <div style="float:right">
            <!-- selectors -->
        </div>
    </div>
</div>
END
);


//-----------------------------------------------------------------------------
$HTMLSchool_tabs = t(<<<END

<div class="Agenda">
    <div class="AgendaMenu">
        <ul>
            <li class="@IS_USERS">
                <a class="@IS_USERS" href="@USERS_URL">Manage Users</a></li>
            <li>
            <li class="@IS_GENERAL">
                <a class="@IS_GENERAL" href="@GENERAL_URL">General Settings</a>
            </li>
            <li class="@IS_COURSE">
                <a class="@IS_COURSE" href="@COURSE_URL">Course Settings</a></li>
            </li>
            <li class="@IS_EXPORT">
                <a class="@IS_EXPORT" href="@EXPORT_URL">Data Export</a>
            </li>
        </ul>
        <div class="AgendaRightShade">&nbsp;</div>
    </div>
    <div class="AgendaContents">
        <table border="0" cellpadding="0" cellspacing="0" style="width: 100%">
        <tbody>
            <tr>
                <td class="AgendaMainContentAreaMiddleLeftCorner">&nbsp;</td>
                <td class="AgendaMainContentAreaMiddleCenterCorner">&nbsp;</td>
                <td class="AgendaMainContentAreaMiddleRightCorner">&nbsp;</td>
            </tr>
        </tbody>
        </table>
    </div>
</div>

END
); 

?>
