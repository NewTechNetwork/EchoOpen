<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTMLrow = t(<<<END
<tr>
       <td style="width:25px;"><input id="@DOC_NAME" type="@TYPE" name="gdocs[]" value="@DOC_LINK" /></td>
        <td class="googleIcons" valign="top" align="left"><img src="@DRUPAL_BASEthemes/Boldr/Images/Icons/@ICON_DOC.jpg" /></td>
        <td class="mailName"><a href="@DOC_LINK" target=_"blank">@DOC_NAME</a></td>
        <td align="right" class="GoogleLastUpdates">@LAST_UPDATED</td>
</tr>

END
);

$HTMLseparator = t(<<<END
<tr><td colspan="4" class="dottedLine">&nbsp;</td></tr>
END
);

$HTMLcontainer = t(<<<END

<table cellpadding="0" cellspacing="0" border="0" width="100%">
<tr>
<td class="googleContent" style=" background-color: #3591CB; color: #FFFFFF; padding: 4px;">
  <table cellspacing="0" cellpadding="0" border="0" align="center" width="100%">
      <tr>
        <td class="GoogleInnertopleftcorner"></td>
        <td colspan="2" id="GoogleInnertoprightcorner"><span id="GoogleHeading">@FEED_TITLE</span><span id="GoogleNewMessage">@DOCS_COUNT</span></td>
      </tr>
         <tr>
        <td colspan="3" class="LastLine">&nbsp;</td>
            </tr>
      <tr>
        <td class="googleInnerRoundedCornerLeft"></td>
        <td valign="top" class="googleInnerBottomBg">
          <div style="width:100%;height:200px;overflow: -moz-scrollbars-vertical;overflow-y: scroll;">
          <table cellspacing="0" cellpadding="0" border="0" width="95%">
        @DOCUMENTS
        </table>
          </div>
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