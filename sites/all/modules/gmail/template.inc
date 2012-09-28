<?php

$HTMLunauthorized = t(<<<END
<tr><td colspan="4" class="googleView"><a href="@AUTHORIZE_LINK">Grant Access to Gmail Inbox</a></td></tr>
END
);

$HTMLemail = t(<<<END
<tr>
	<td class="mailName"><a href="@EMAIL_LINK" target="_blank">@EMAIL_FROM</a></td>
	<td class="mailName"><a href="@EMAIL_LINK" target="_blank">@EMAIL_SUBJECT</a></td>
	<td class="mailContent">@EMAIL_TEASER</td>
	<td align="right" class="mailTime">@EMAIL_DATETIME</td>

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
        <td colspan="3" class="googleContent">
        <table width="97%" cellpadding="0" align="center" cellspacing="0" border="0">
            <tr>
              <td colspan="3">
              <table cellspacing="0" cellpadding="0" border="0" width="100%">
                  <tr>
                    <td class="GoogleInnertopleftcorner"></td>
                    <td colspan="2" id="GoogleInnertoprightcorner">
                        <span id="GoogleHeading">Mail</span><span id="GoogleNewMessage">@NEW_MESSAGES</span>
                    </td>
                  </tr>
                  <tr>
                    <td class="googleInnerRoundedCornerLeft"></td>
                    <td valign="top" class="googleInnerBottomBg">

                    <table cellpadding="0" cellspacing="0" border="0" width="100%">
                    @GMAIL_NEWMAILS
                        <tr>
                            <td colspan="4" class="LastLine">&nbsp;</td>
                        </tr>
                        <tr>
                            <td colspan="4" align="right" class="googleView"><a href="@GMAIL_COMPOSE" target="_blank">Compose Mail</a>&nbsp;|&nbsp;<a href="@GMAIL_INBOX" target="_blank">View Inbox&gt;&gt;</a></td>
                        </tr>
                    </table>
                    </td>
                    <td class="googleInnerRoundedCornerRight"></td>
                  </tr>
                </table></td>
            </tr>
            <tr>
                <td colspan="3">&nbsp;</td>
            </tr>
        </table></td>
    </tr>
</table>
END
);

?>