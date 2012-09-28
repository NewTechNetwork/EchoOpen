<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information 
// please contact New Technology Network Licensing at: 
// webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


// $Id: page.tpl.php,v 1.28.2.1 2009/04/30 00:13:31 goba Exp $
module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_blocks', 'activities.content');
drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/js/jquery.tipsy.js');
global $base_path, $user, $site_name;
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
    <head>
        <?php print $head ?>
        <title><?php print $head_title ?></title>
        <?php print $styles ?><?php print $scripts ?>
        <script type="text/javascript" src="<?php echo $base_path ?>themes/Boldr/js/page_js.js"></script>
        <script type="text/javascript"><?php /* Needed to avoid Flash of Unstyle Content in IE */ ?>
        </script>

        <?php
        if ($user->uid) {
            $user_Detail_Obj = get_user_school_info($user->uid);
            ?>
            <script type="text/javascript">


                var _gaq = _gaq || []; // please see this page http://code.google.com/apis/analytics/docs/gaJS/gaJSApi_gaq.html
                _gaq.push(['_setAccount', 'UA-5818281-61']);
                (function() {
                    var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
                    ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
                    var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);
                })();

                _gaq.push(['_setCustomVar',
                    1,         // This custom var is set to slot #1.  Required parameter.
                    'School',  // The name of the custom variable.  Required parameter.
                    '<?php print str_replace(' ', '-', $user_Detail_Obj->school_name) ?>_<?php print $user_Detail_Obj->school_nid ?>_', // The value of the custom variable. Required parameter.

                    3          // Sets the scope to visitor-level.  Optional parameter.
                ]);
                _gaq.push(['_trackPageview']);

            </script>
        </head>
        <body class="bg">
            <table width="100%" class="maintable" cellpadding="0" cellspacing="0" border="0" >
                <tr>
                    <td>
                        <table border="0" cellpadding="0" width="890px" align="center" cellspacing="0">
                            <tr>
                                <td style="height:30px" valign="top" colspan="2"><div style="padding:6px 0px 0 0px;">
                                        <div style="float:left"  id="websiteName">
                                            <?php
                                            $stitle = $user_Detail_Obj->school_name;

                                            $maxlen = 42;
                                            $maxlen = $maxlen - (strlen($user_Detail_Obj->first_name . ' ' . $user_Detail_Obj->last_name) - 12);

                                            if (strlen($user_Detail_Obj->school_name) > $maxlen)
                                                $stitle = substr($user_Detail_Obj->school_name, 0, $maxlen) . "...";
                                            print $stitle;
                                            ?> </div>
                                        <div style="float:right">

                                            <!-- Change Secondary link with our own html instead of calling theme function -->
                                            <ul id="subnavlist" class="links">
                                                <?php if ($user->uid == 1) {
                                                    ?>
                                                    <li class="first">
                                                        <a title="" href="<?php echo $base_path;?>?q=admin">Administer</a>
                                                    </li>
                                                <?php } ?>
                                                <li class="last">
                                                </li></ul>
                                        </div>

                                        <div id="UserLogin" style="float:right;">
                                            <table width="300" border="0" cellspacing="8" style="margin-top: -3px;">
                                                <tr>
                                                    <td> <?php if ($user->uid) : ?>
                                                            <?php $url_profile = 'ntlp/user/profile/' . $user->uid; ?>
                                                            <div  style="margin-top:-4px;">
                                                                Hi, <?php print $user_Detail_Obj->first_name . ' ' . $user_Detail_Obj->last_name ?>!
                                                            </div>
                                                        </td>
                                                    <?php if (!isset($user->roles[NTLP_ROLEID_PARENT])) { ?>
                                                        <td>
                                                            <?php
                                                                if ($user_Detail_Obj->gdocs_status == 1 ||
                                                                    $user_Detail_Obj->gmail_status == 1 ||
                                                                    $user_Detail_Obj->gsite_status == 1) {

                                                                    $sid = $user_Detail_Obj->school_nid . '_';
                                                                    $domain = variable_get('grupal_domain' . $sid, '');
                                                                    $allDocslink = 'https://docs.google.com/a/' . $domain;
                                                                    $mail_link = 'https://mail.google.com/a/' . $domain;
                                                                    $sites_link = 'https://sites.google.com/a/' . $domain;

                                                                ?>

                                                                <div id="gapps" class="gapps-title" title="Google Apps" style="width:22px;">
                                                                    <div  id="gapps-tipsy" class="gapps-default_png" onclick="javascript:gapps_tipsy_call();">
                                                                    </div>
                                                                </div>

                                                                <div id="gapps-data" style="display: none;">
                                                                    <?php if ($user_Detail_Obj->gmail_status == 1) { ?>
                                                                        <div style="margin-bottom:5px;"><a href="<?php echo $mail_link; ?>" target="_blank" class="tipsygreenlinks" >Gmail</a></div>
                                                                    <?php } ?>
                                                                    <?php if ($user_Detail_Obj->gdocs_status == 1) { ?>
                                                                        <div style="margin-bottom:5px;"><a href="<?php echo $allDocslink; ?>" target="_blank" class="tipsygreenlinks" >Google Docs</a></div>
                                                                    <?php } ?>
                                                                    <?php if ($user_Detail_Obj->gsite_status == 1) { ?>
                                                                        <div style="margin-bottom:5px;"><a href="<?php echo $sites_link; ?>" target="_blank" class="tipsygreenlinks" >Google Sites</a></div>
                                                                    <?php } ?>
                                                                </div>
                                                            <?php } ?>
                                                        </td>
                                                    <?php } ?>
                                                        <td>
                                                            <div id="settings" class="settings-title" title="Options">
                                                            <div style="width:22px;" class="settings-default_png" id="settings-tipsy" onclick="javascript:settings_tipsy_call();">
                                                            </div>
                                                            </div>
                                                            <div id="settings-data" style="display: none;">
                                                                <div style="margin-bottom:5px;"> <?php print l('My Profile', $url_profile, array('attributes' => array('class' => 'tipsygreenlinks'))); ?></div>
                                                                <div style="margin-bottom:5px;"><?php print l('My Settings', 'ntlp/myaccount/user/settings', array('attributes' => array('class' => 'tipsygreenlinks'))); ?></div>
                   
                                                                <div style="margin-bottom:5px;">
                                                                <?php endif; ?>
                                                            </div>
                                                            <div style="margin-bottom:5px;"> <?php
                                                            if (user_access('administer my school')) {
                                                                print l('Manage', 'ntlp/school/management/users', array('attributes' => array('class' => 'tipsygreenlinks')));
                                                            }
                                                                ?>
                                                            </div>
                                                        </div>
                                                    </td>
                                                    <td style="text-align: right;">
                                                          <div  style="margin-top:-4px;">
                                                        <?php print l('Log Out', 'logout'); ?>
                                                          </div>
                                                    </td>
                                                </tr>
                                            </table>
                                        </div>

                                    </div></td>
                            </tr>
                            <tr>
                                <td colspan="2"><div style="padding:10px 0px 0 0px">
                                        <div id="logo" style="float:left">
                                            <?php if ($logo) {
                                                ?>
                                                <a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><img src="<?php print $logo ?>" alt="<?php print t('Home') ?>" /></a>
                                            <?php } ?>
                                            <?php if ($site_name) {
                                                ?>
                                                <h1 class='site-name'><a href="<?php print $front_page ?>" title="<?php print t('Home') ?>"><?php print $site_name ?></a></h1>
                                            <?php } ?>
                                        </div>
                                        <div style="float:right">
                                            <table align="center" cellpadding="0" cellspacing="0" border="0">
                                                <tr>
                                                    <td id="menu">
                                                        <div><div id="search" class="container-inline">
                                                                <div class="form-item">
                                                                    &nbsp;<br>
                                                                </div>
                                                                &nbsp;<br><br>
                                                                        </div>
                                                                        </div>
                                                                        </td>
                                                                        </tr>
                                                                        <tr>
                                                                            <td>
                                                                                <?php

                                                                                function mythemename_activelink($primary_links) {
                                                                                    if (module_exists('path')) {
                                                                                        $alias = explode('/', request_uri()); // Might as well save a db hit
                                                                                        $alias = $alias[1];
                                                                                        foreach ($links as $key => $link) {
                                                                                            if ($link['href'] == $alias) {
                                                                                                $links[$key]['attributes']['class'] = 'active';
                                                                                            }
                                                                                            return $links;
                                                                                        }
                                                                                    }
                                                                                }
                                                                                ?>
                                                                                <?php print get_primary_link_html(); ?>
                                                                            </td>
                                                                        </tr>
                                                                        </table>
                                                                        </div>
                                                                        </div></td>
                                                                        </tr>
                                                                        </table>
                                                                        <table cellpadding="0" cellspacing="0" align="center" border="0" width="890px">
                                                                            <tr>
                                                                                <td colspan="3">
                                                                                    <?php
                                                                                    print $msgbox;
                                                                                    print $ntlp_module_header;
                                                                                    print $ntlp_module_tabs;
                                                                                    ?></td>
                                                                            </tr>
                                                                            <tr>
                                                                                <td class="contentMiddleLeftbg"></td>
                                                                                <td class="contentMiddleCenterbg"><!-- Courses Panel -->
                                                                                    <table border="0" width="846px" align="center" cellpadding="0" cellspacing="0" id="content_table">
                                                                                        <tr>
                                                                                            <td valign="top"><?php if ($mission) { ?>
                                                                                                    <div id="mission"><?php print $mission ?></div>
                                                                                                <?php } ?>
                                                                                                <div id="main">
                                                                                                    <div class="tabs"><?php print $tabs ?></div>
                                                                                                    <?php
                                                                                                    if ($show_messages) {
                                                                                                        print $messages;
                                                                                                    }
                                                                                                    ?>
                                                                                                    <?php print $help ?> <?php print $content; ?> <?php print $feed_icons; ?> </div></td>
                                                                                        </tr>
                                                                                    </table></td>
                                                                                <td class="contentMiddleRightbg"></td>
                                                                            </tr>
                                                                            <tr>
                                                                                <td class="contentBottomLeftbg"></td>
                                                                                <td class="contentBottomCenterbg"></td>
                                                                                <td class="contentBottomRightbg"></td>
                                                                            </tr>
                                                                        </table>

                                                                        </td>
                                                                        </tr>
                                                                        <tr>
                                                                            <td>
                                                                                <table cellspacing="0" cellpadding="0" border="0" width="890px" align="center">
                                                                                    <tr>
                                                                                        <td>
                                                                                            <?php print $footer_message ?> <?php print $footer ?>
                                                                                        </td>
                                                                                    </tr>
                                                                                </table>
                                                                            </td>
                                                                        </tr>
                                                                        </table>
                                                                        <?php
                                                                        if (isset($_COOKIE['ntlploginattempt'])) {
                                                                            if (intval($_COOKIE['ntlploginattempt']) > 0) {
                                                                                $timediff = strftime('%M', time() - $_COOKIE['ntlploginattempt']);
//                    echo $timediff;
                                                                                if (intval($timediff) > 5) {
                                                                                    setcookie('ntlploginattempt', 0, time() - 3600, ini_get('session.cookie_path'), ini_get('session.cookie_domain'), ini_get('session.cookie_secure') == '1');
                                                                                }
                                                                            }
                                                                        }
                                                                        ?>
                                                                    <?php } else {
                                                                        ?>
                                                                        <?php
                                                                        setcookie('ntlploginattempt', time(), null, ini_get('session.cookie_path'), ini_get('session.cookie_domain'), ini_get('session.cookie_secure') == '1');
                                                                        ?>
                                                                        </head>
                                                                        <body class="bg2" >
                                                                            <div class="loginpage">
                                                                                <div class="gap">
                                                                                    &nbsp;
                                                                                </div>
                                                                            </div>
                                                                            <div class="loginpage">
                                                                                <div style="height:55px;font-size:55px;">
                                                                                    &nbsp;
                                                                                </div>
                                                                                <table cellpadding="0" cellspacing="0" border="0" width="100%">
                                                                                    <tr>
                                                                                        <td class="loginpage_bg_topleft">&nbsp;</td>
                                                                                        <td class="loginpage_bg_topcenter">&nbsp;</td>
                                                                                        <td class="loginpage_bg_topright">&nbsp;</td>
                                                                                    </tr>
                                                                                    <tr>
                                                                                        <td class="loginpage_bg_middleleft">&nbsp;</td>
                                                                                        <td class="loginpage_bg_middlecenter">
                                                                                            <table width="100%" cellpadding="0" cellspacing="0" border="0">
                                                                                                <tr>
                                                                                                    <td style="width:50%" valign="middle" align="center"><img src="<?php echo $base_path ?>sites/default/files/images/logo_loginpage.jpg" /><br /></td>
                                                                                                    <td valign="top"> <?php print $content; ?>
                                                                                                    </td>
                                                                                                </tr>
                                                                                            </table>
                                                                                        </td>
                                                                                        <td class="loginpage_bg_middleright">&nbsp;</td>
                                                                                    </tr>
                                                                                    <tr>
                                                                                        <td class="loginpage_bg_bottomleft">&nbsp;</td>
                                                                                        <td class="loginpage_bg_bottomcenter">&nbsp;</td>
                                                                                        <td class="loginpage_bg_bottomright">&nbsp;</td>
                                                                                    </tr>
                                                                                </table>
                                                                            </div>
                                                                            <div class="loginpage">
                                                                                <div style="height:22px;font-size:22px;">&nbsp;</div>
                                                                                <div align="center" class="login_panel_ftr">
										<tr>
                                                                                        <td align="center" class="copyright">
												Echo Open was developed and released as open source software to the education community by <a href="http://www.newtechnetwork.org/" class="greenLink">New Tech Network</a>.<br /></td></tr><tr><td><br />Echo Open software Copyright © 2012 KnowledgeWorks Foundation
Echo Open trademark and logo are trademarks of New Technology Network LLC<br /></td></tr><tr><td><br />
 
The Echo Open software is licensed under the GNU GPLv2.  For licensing, trademarks, and attribution information please the documentation provided in the distribution package.
                                                                                        </td>
                                                                                    </tr>
                                                                                    </table>
                                                                                    </div>
                                                                                </div>
                                                                            </div>
                                                                        <?php } ?>

                                                                        <?php print $closure ?>
                                                                        <div style="float:left;font-size:8px;">
                                                                            <?php
                                                                            print $_SERVER["SERVER_ADDR"];
                                                                            ?>
                                                                        </div>
                                                                    </body>
                                                                    </html>


