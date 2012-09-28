<?php
/* 
 * grupal - Google Apps integrations for Drupal
 * Copyright © 2009 Sam Johnston <samj@samj.net> http://samj.net/
 *                  Australian Online Solutions Pty Ltd http://www.aos.net.au/
 * 
 * $Id: grupal.admin.inc 45 2009-03-09 19:00:29Z samj@samj.net $
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

require_once('saml2.php');

/**
 * Menu callback for single sign in service.
 */
function grupal_single_sign_in() {
    global $user;
    $sid = get_user_school($user->uid).'_';

    if (!(variable_get('grupal_enable_sso'.$sid, '0') && variable_get('grupal_enable_single_sign_in'.$sid, '1'))) {
        drupal_set_message('This service is not enabled.', 'error');
        drupal_access_denied();
    }

    if (!$user->name) {
        drupal_set_message(t('You must be logged in before accessing Google Apps.'), 'warning');
        drupal_goto();
    }

    if (!user_access('use google apps')) {
        drupal_access_denied();
    }

    if (! $html = samlResponse(remove_domain($user->name), variable_get('grupal_domain'.$sid, ''),
      variable_get('grupal_sso_certificate'.$sid, ''), variable_get('grupal_sso_private_key'.$sid, ''))) {
        drupal_set_message(t('There was a problem generating a SAML assertion.'), 'error');
        drupal_goto();
    }

    print $html;
}

/**
 * Menu callback for single sign out service.
 */
function grupal_single_sign_out() {
    global $user;
    $sid = get_user_school($user->uid).'_';

    if (!(variable_get('grupal_enable_sso'.$sid, '0') && variable_get('grupal_enable_single_sign_out'.$sid, '1'))) {
    // No point confusing users returning from Google Apps with error message.
     drupal_set_message('This service is not enabled.', 'error');
    // drupal_access_denied();
        drupal_goto();
    }

    if ($user->name) {
        drupal_goto('logout');
    } else {
        drupal_goto();
    }
}

/**
 * Menu callback for password changes.
 */
function grupal_password() {
    global $user;
    $sid = get_user_school($user->uid).'_';

    if (!(variable_get('grupal_enable_sso'.$sid, '0') && variable_get('grupal_enable_password'.$sid, '1'))) {
        drupal_set_message('This service is not enabled.', 'error');
        drupal_access_denied();
    }

    if ($user->uid) {
        drupal_goto("user/" . $user->uid . "/edit");
    } else {
        drupal_access_denied();
    }
}

//function dgrupal_direct_sign_in() {
//    include 'direct_signin.template.inc';
//    $mainForm = $HTMLform;
//
//    $samlRequest = $_GET["SAMLRequest"];
//    //If SAML parameters still null, then authnRequest has not yet been
//    //received by the Identity Provider, so user should not be logged in.
//    if (empty($samlRequest)) {
//        $output = $HTMLsigned_out;
//        $domainName = $_GET["dn"];
//        $output = str_replace("@domainName", $domainName, $output);
//
//    } else {
//        $output = $HTMLsign_in;
//        $error = $_GET["error"];
//        if (!empty($error)) {
//            $output .= '<p><font color="red"><b>$error</b></font></p>';
//        }
//        $issueInstant = $_GET["issueInstant"];
//        $providerName = $_GET["providerName"];
//        $acsURL = $_GET["acsURL"];
//        $relayState = $_GET["RelayState"];
//
//        $output = str_replace("@samlRequest", $samlRequest, $output);
//        $output = str_replace("@relayState", $relayState, $output);
//
//        $url = request_uri();
//        $url = str_replace('grupal%2Fdsignin', 'grupal/signin', $url);
//        $output = str_replace("@URL", $url, $output);
//    }
//    $mainForm = str_replace("@BODY", $output, $mainForm);
//    print $mainForm;
//}

function grupal_direct_sign_in() {
    global $user;
//    die('User:'.$username.' Id:'.$user->uid);
    if ($user->uid) {
        $url = request_uri();
        $url = str_replace('grupal%2Fdsignin', 'grupal/signin', $url);
        if (isset($_GET['SAMLRequest']) && isset($_GET['RelayState'])) {
            $samlRequest = $_GET["SAMLRequest"];
            $relayState = $_GET["RelayState"];
            drupal_goto('grupal/signin', array(
                'SAMLRequest' => $samlRequest,
                'RelayState' => $relayState));
        }
        return;
    } else {
        print drupal_get_form('direct_sign_in_form');
    }
}
function grupal_direct_sign_out() {
    $domainName = $_COOKIE['GRUPALDSO'];
    setcookie ("GRUPALDSO", "", time() + (30));       //reset cookie
//    watchdog('dsignout', print_r($domainName, true));
    if (!empty($domainName)) {
        if ($domainName === "logout") {
//        watchdog('dsignout', 'Boldr Login');
            drupal_goto('ntlp/home');
        } else {
            include 'direct_signin.template.inc';
//            watchdog('dsignout', 'direct_sign_in_form');

            $output = str_replace("@domainName", $domainName, $HTMLsignout);
            print $HTMLform_start;
            print $output;
            print $HTMLform_end;
        }
    } else {
        global $user;
        if ($user->name) {
            $domainName = $_GET["dn"];
            if (!empty($domainName)) {
                drupal_goto('logout', 'dn='.$domainName);
            }
        } else {
            drupal_goto();
        }
    }
}

function direct_sign_in_form($form_state) {
    include 'direct_signin.template.inc';

    if (form_get_errors()) {
//    print 'Error:';
        print ($_SESSION['messages']);
    }
    $samlRequest = $_GET["SAMLRequest"];
    //If SAML parameters still null, then authnRequest has not yet been
    //received by the Identity Provider, so user should not be logged in.
    if (empty($samlRequest)) {
        $output = $HTMLsigned_out;
        $domainName = $_GET["dn"];
        $output = str_replace("@domainName", $domainName, $output);

    } else {
        $output = $HTMLsign_in;
        $error = $_GET["error"];
        if (!empty($error)) {
            $output .= '<p><font color="red"><b>$error</b></font></p>';
        }
//        $issueInstant = $_GET["issueInstant"];
//        $providerName = $_GET["providerName"];
//        $acsURL = $_GET["acsURL"];
        $relayState = $_GET["RelayState"];

        $form['#prefix'] = $HTMLform_start;

        $form['SAMLRequest'] = array(
            '#type' => 'hidden',
            '#value' => $samlRequest,
        );
        $form['RelayState'] = array(
            '#type' => 'hidden',
            '#value' => $relayState,
        );
        $form['name'] = array(
            '#type' => 'textfield',
            '#required' => TRUE,
            '#size' => 30,
            '#maxlength' => USERNAME_MAX_LENGTH,
            '#attributes' => array('style' => 'dir="ltr" style="WIDTH: 250px"'),
            '#prefix' => $HTMLsignin_namep,
        );
        $form['pass'] = array(
            '#type' => 'password',
            '#required' => TRUE,
            '#size' => 30,
            '#attributes' => array('style' => 'dir="ltr" style="WIDTH: 250px"'),
            '#prefix' => $HTMLsignin_pwdp,
            '#suffix' => $HTMLsignin_pwds,
        );
        $form['submit'] = array(
                '#type' => 'submit',
                '#value' => 'Log in',
                '#suffix' => $HTMLsignin_submits,
                '#submit' => array('direct_sign_in_submit'),
                '#attributes' => array('class' => 'wpsButtonText', 'style' => 'CURSOR: pointer', 'align' => 'absMiddle', 'border' => '0'),
        );
        $form['cancel'] = array(
                '#type' => 'submit',
                '#value' => 'Cancel',
                '#suffix' => $HTMLsignin_cancels,
                '#attributes' => array('class' => 'wpsButtonText', 'style' => 'CURSOR: pointer', 'align' => 'absMiddle', 'border' => '0'),
        );
        $form['#suffix'] = $HTMLform_end;
        $form['#validate'] = user_login_default_validators();

    }

    return $form;
//    $mainForm = str_replace("@BODY", $output, $mainForm);
//    print $mainForm;
}

function direct_sign_in_submit($form, $form_state) {
    $username  = $form_state['values']['name'];
    $samlRequest  = $form_state['values']['SAMLRequest'];
    $relayState  = $form_state['values']['RelayState'];
    global $user;
//    die('User:'.$username.' Id:'.$user->uid);
    if ($user->uid) {
        drupal_goto('grupal/signin', array(
            'SAMLRequest' => $samlRequest,
            'RelayState' => $relayState));
        return;
    }
}