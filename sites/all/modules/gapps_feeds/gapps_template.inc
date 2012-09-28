<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


global $base_path;

$HTML_GMAIL = t(<<<END
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
                        <span id="GoogleHeading">Mail</span><span id="GoogleNewMessage"></span>
                    </td>
                  </tr>
                  <tr>
                    <td class="googleInnerRoundedCornerLeft"></td>
                    <td valign="top" class="googleInnerBottomBg">

                    <table cellpadding="0" cellspacing="0" border="0" width="100%">
                    
                        <tr>
                            <td colspan="4" align="center"><img src=$base_path/themes/Boldr/Images/common/loading2.gif /></td>
                        </tr>
                        <tr>
                           <td colspan="4" align="right" class="googleView"></td>
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

$HTML_DOCS = t(<<<END
<table cellpadding="0" cellspacing="0" border="0" width="100%">
<tr>
<td class="googleContent">
  <table cellspacing="0" cellpadding="0" border="0" align="center" width="100%">
      <tr>
        <td class="GoogleInnertopleftcorner"></td>
        <td colspan="2" id="GoogleInnertoprightcorner"> <span id="GoogleHeading">Google Docs</span><span id="GoogleNewMessage"></span></td>
      </tr>
      <tr>
        <td class="googleInnerRoundedCornerLeft"></td>
        <td valign="top" class="googleInnerBottomBg">

        <table cellpadding="0" cellspacing="0" border="0" width="100%">
                    <tr>
                    <td colspan="3" align="center"><img src=$base_path/themes/Boldr/Images/common/loading2.gif /></td>
            </tr>
            <tr>
                    <td colspan="3" align="right" class="googleView"></td>
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


$HTML_SITES = t(<<<END
<table cellpadding="0" cellspacing="0" border="0" width="100%">
<tr>
<td class="googleContent">
  <table cellspacing="0" cellpadding="0" border="0" align="center" width="100%">
      <tr>
        <td class="GoogleInnertopleftcorner"></td>
        <td colspan="2" id="GoogleInnertoprightcorner"> <span id="GoogleHeading">Google Sites</span><span id="GoogleNewMessage"></span></td>
      </tr>
      <tr>
        <td class="googleInnerRoundedCornerLeft"></td>
        <td valign="top" class="googleInnerBottomBg">

        <table cellpadding="0" cellspacing="0" border="0" width="100%">
        
            <tr>
                    <td colspan="3" align="center"><img src=$base_path/themes/Boldr/Images/common/loading2.gif /></td>
            </tr>
            <tr>
                    <td colspan="3" align="right" class="googleView"></td>
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
