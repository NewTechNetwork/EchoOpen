<?php
$HTML_help_tabs = t(<<<END

<div class="Agenda">
    <div class="AgendaMenu">
        <ul>
            <li class="@IS_FAQ">
                <a class="@IS_FAQ" href="@FAQ_URL">FAQ</a>
            </li>
            <!-- <li class="@IS_FEEDBACK">
                <a class="@IS_FEEDBACK" href="@FEEDBACK_URL">Feedback</a></li>
            </li> -->
            <!-- support request -->
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
        <li>@LINK_HELP >&nbsp;</li>
    </ul>
    </div>
 </div>
    <br clear="all" />

END
);


?>
