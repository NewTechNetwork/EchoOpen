<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTML_groups_tabs = t(<<<END
<h1 class="pageHeading">Groups</h1>
<div class="Agenda">
    <div class="AgendaMenu">
        <ul>
            <li class="@IS_MY">
                <a class="@IS_MY" href="@MY_GROUP_URL">My Groups</a>
            </li>
            <li class="@IS_ALL">
                <a class="@IS_ALL" href="@ALL_GROUP_URL">All Groups</a></li>
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

$HTML_group_page_tabs = t(<<<END

   <h1 class = "pageHeading"> @GROUP_TITLE </h1>
<div class="Agenda">
    <div class="AgendaMenu">
        <ul>
            <li class="@IS_HOME">
                <a class="@IS_HOME" href="@START_URL">Start</a>
            </li>
          <!--  <li class="@IS_RESOURCES">
                <a class="@IS_RESOURCES" href="@RESOURCES_URL">Resources</a></li>
            </li> -->
            <li class="@IS_DISCUSSIONS">
                <a class="@IS_DISCUSSIONS" href="@DISCUSSIONS_URL">Discussions</a></li>
            </li>
            <li class="@IS_EVENTS">
                <a class="@IS_EVENTS" href="@EVENTS_URL">Events</a></li>
            </li>
             <li class="@IS_MEMBER">
                <a class="@IS_MEMBER" href="@MEMBERS_URL">Members</a></li>
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



$HTMLcontainer = t(<<<END
<div>
    <div class="BoldrBrandCrumb">
    <ul>
        <li>@LINK_HOME >&nbsp;</li>
        <li>@LINK_GROUPS >&nbsp;</li>
    </ul>
    </div>
    <br clear="all" />
    <div style="float:right;margin:0 0 0 5px" class="creategroup_Btn">
    @LINK_CREATE_GROUPS
    </div>
    <div style="float:right;margin:0 0 0 5px" class="creategroup_Btn">
  <!--  @LINK_JOIN_GROUPS -->
    </div>
    <div class="BoldrHeading">
    	@GROUP_TOOLS
    </div>
</div>
END
);

$HTMLgroup_manage = t(<<<END
<div style="float:right">
    <div class="anchorBaseCombobox">
    <div id="btnManageCourse"></div>
    <div id="manageCourse" class="hideMODrpDown" style="display:none">
    <div class="hideMODrpDownInnerBg">
            <ul>
                <li class="ManageOption">@MANAGE_COURSE_EDIT</li>
                <li class="ManageOption">@MANAGE_COURSE_COPY</li>
                <li class="ManageOption">@MANAGE_COURSE_DELETE</li>
            </ul>
        </div>
    </div>
    </div>
</div>
END
);

?>
