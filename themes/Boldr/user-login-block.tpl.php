<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


    $offline = false;

    // Check user status when a login attempt is failed, so that we can display a proper message
    $msg = "Oops! The username and/or password was incorrect.";
    if (!empty($_POST)) {
        if (!empty($_POST['name'])) {
            $urec = get_user_record_byname($_POST['name']);
            if ($urec) {
                if ($urec->is_active == 1 && $urec->status == 1) {              //Active (Enabled)
//                    $msg = "Oops! The username and/or password was incorrect.";
                } elseif ($urec->is_active == 1 && $urec->status == 0) {        //Active (Disabled)
                    $msg = "This account has been temporarily disabled. <br />Please contact your site Tech Manager";
                } elseif ($urec->is_active == 0 && $urec->status == 0) {        //Inactive
                    $msg = "This account is no longer active.";
                }
            }
        }
    }
?>
<div id="block-user-0" class="clear-block block block-user">
    <div class="greenmsgbox_new" style="width: 422px; margin: 0 auto; min-height: 25px; background: #5fa741; border:1px solid #e9e9e9; display: none;">
        <div style="color: white; font-size: 12px; font-weight: bold; text-align: center; padding: 6px 0 6px 0;">
        	<?php echo $msg; ?>
        </div>
    </div>
    <?php if ($offline) { ?>
        <div class="server_error">
        <div class="server_error_main_container">
            <h1>
                Sorry, echo is down for scheduled maintenance, we will be back shortly.
            </h1>
        </div></div>
    <?php } else { ?>
    <form action="<?php echo $_SERVER['REDIRECT_URL']; ?>?destination=<?php echo urlencode($_SERVER['REDIRECT_URL']); ?>"  accept-charset="UTF-8" method="post" id="user-login-form">
        <div class="loginpage_loginpanel">
            <div class="loginpage_loginpanel_topcenter">
                <div class="loginpage_loginpanel_middleleft">
                    <div class="loginpage_loginpanel_middleright">
                        <div class="loginpage_loginpanel_bottomcenter">
                            <div class="loginpage_loginpanel_topleft">
                                <div class="loginpage_loginpanel_topright">
                                    <div class="loginpage_loginpanel_bottomleft">
                                        <div class="loginpage_loginpanel_bottomright">

                                            <table cellpadding="0" cellspacing="0" border="0" align="center" width="100%">
                                                <tr>
                                                    <td align="center">
                                                        <input type="text" name="name" id="edit-name" title="username (email)"  value="username (email)" class="fieldfocus textbox form-text required" />
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td>&nbsp;
                                                        
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td align="center">
                                                        <input type="password" name="pass" id="edit-pass" title="password" value="password" class="fieldfocus textbox form-text required" />
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td>&nbsp;
                                                        
                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td align="center">
                                                        <input type="submit" name="op" id="edit-submit" value="Sign In"  class="signinBtn form-submit" />

                                                    </td>
                                                </tr>
                                                <tr>
                                                    <td>&nbsp;
                                                        
                                                    </td>
                                                </tr>
                                                <tr><?php global $base_path ?>
                                                    <td align="center">
                                                        <?php //print dlg('>> Help. I forgot my password.', 'user/forgot/password', 400, 500, 'forgetpassword_link' ) ?>
                                                    </td>
                                                </tr>
                                            </table>

                                            <input type="hidden" name="form_build_id" id="<?php form_clean_id('edit-'. drupal_get_token()  .'-form-token') ?>" value="<?php drupal_get_token() ?>"  />
                                            <input type="hidden" name="form_id" id="edit-user-login-block" value="user_login_block"  /> <script language="javascript" type="text/javascript">
                                                $("input.fieldfocus").focus(function () {
                                                    $("input.fieldfocus").val('');
                                                    $("input.fieldfocus").removeClass('fieldfocus');
                                                });
                                                $("input.fieldfocus").change(function () {
                                                    $("input.fieldfocus").val();
                                                });
                                                //    $("input.fieldfocus").blur(function () {
                                                //	var as =$(this).attr("title");
                                                //	if ($(this).val() == '' ) {
                                                //	$(this).val(as);
                                                //	}
                                                //    });
                                                <?php if( !empty($_POST) ): ?>
                                                $(document).ready(function() {
                                                    var as =$('input.fieldfocus').attr("title");
                                                    if($('input.fieldfocus').val() == as) {
                                                        $('div.greenmsgbox_new').fadeIn('slow');
                                                    }
                                                });
                                                <?php endif ?>
                                            </script>



                                        </div>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>

    </form>
    <?php } ?>
</div>
