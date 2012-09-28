<?php

$HTML_library_tabs = t(<<<END

<div class="Agenda">
    <div class="AgendaMenu">
        <ul>
            <li class="@IS_PROJECT">
                <a class="@IS_PROJECT" href="@PROJECT_URL">All Projects</a>
            </li>
            <li class="@IS_MYPROJECTS">
                <a class="@IS_MYPROJECTS" href="@MYPROJECTS_URL">My Projects</a></li>
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
        <li>@LINK_LIBRARY >&nbsp;</li>
        <li>@LINK_PROJECT_LIBRARY >&nbsp;</li>
    </ul>
    </div>
    <br clear="all" />
    <div class="BoldrHeading manageBtn" style="float:right">
    	@LIBRARY_PROJECT_TOOLS
    </div>
</div>
<br clear=all />
END
);

?>
