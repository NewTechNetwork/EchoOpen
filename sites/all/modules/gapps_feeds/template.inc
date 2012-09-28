<?php

$HTMLrow = t(<<<END
<tr>
        <td class="googleIcons" valign="top" align="left"><img src="@DRUPAL_BASEthemes/Boldr/Images/Icons/@ICON_DOC.jpg" /></td>
        <td class="mailName"><a href="@DOC_LINK" title="@DOC_TITLE" target=_"blank">@DOC_NAME</a></td>
        <td align="right" class="GoogleLastUpdates">@LAST_UPDATED</td>
</tr>

END
);

$HTMLseparator = t(<<<END
<tr><td colspan="3" class="dottedLine">&nbsp;</td></tr>
END
);

$HTMLcontainer = t(<<<END
<table cellpadding="0" cellspacing="0" border="0" width="100%">
<tr>
<td class="googleContent">
  <table cellspacing="0" cellpadding="0" border="0" align="center" width="100%">
      <tr>
        <td class="GoogleInnertopleftcorner"></td>
        <td colspan="2" id="GoogleInnertoprightcorner"> <span id="GoogleHeading">@FEED_TITLE</span><span id="GoogleNewMessage">@DOCS_COUNT</span></td>
      </tr>
      <tr>
        <td class="googleInnerRoundedCornerLeft"></td>
        <td valign="top" class="googleInnerBottomBg">

        <table cellpadding="0" cellspacing="0" border="0" width="100%" style="text-overflow:ellipsis;overflow:hidden">
        @DOCUMENTS
            <tr>
                    <td colspan="3" class="LastLine">&nbsp;</td>
            </tr>
            <tr>
                    <td colspan="3" align="right" class="googleView"><a href="@DOCS_LINK" target="_blank">View All @FEED_TITLE>></a></td>
            </tr>
        </table>
        </td>
        <td class="googleInnerRoundedCornerRight"></td>
      </tr>
    </table>
  </td>
</tr>
</table>
END
);

?>