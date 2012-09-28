<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


// $Id: better_messages.tpl.php,v 1.1.2.4 2010/03/29 09:56:01 doublethink Exp $
/*
  Available variables are:
  $content	The messages to put inside Better Messages. Drupal originally calls theme_status_messages() to theme this output.
  With this module enabled you'll always have to call theme_better_messages_content() instead of theme_status_messages().
 */
?><!-- <div id="better-messages-default">
    <div id="messages-inner">
        <table><tbody>
                <tr>
                    <td class="tl"></td>
                    <td class="b"></td>
                    <td class="tr"></td>
                </tr>
                <tr>
                    <td class="b"></td>
                    <td class="body">
                        <div class="content">
                            <?php /* If you want to theme further.. theme_better_messages_content() */ ?>
                            <?php //print $content ?>
                        </div>
                        <div class="footer">
                            <span class="message-timer"></span><a class="message-close" href="#"></a>
                        </div>
                    </td>
                    <td class="b"></td></tr>
                <tr>
                    <td class="bl"></td>
                    <td class="b"></td>
                    <td class="br"></td>
                </tr>
            </tbody></table>
    </div>
</div> -->
<div id="better-messages-default">
    <div id="messages-inner">
        <div class="custom_dialog">
            <div class="body">
                 <?php /* If you want to theme further.. theme_better_messages_content() */ ?>
                 <?php print $content ?>
            </div>
            <div class="custom_footer">
                <span class="message-timer"></span><a class="custom_button" href="#" style=" text-decoration: none !important;"><span style="top: 4px; position: relative; left: 24px;"> Close </span></a>
            </div>
        </div>
    </div>
</div>
