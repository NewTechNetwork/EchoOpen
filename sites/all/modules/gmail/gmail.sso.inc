<?php
/*
 * gmail - Gmail block for NTLP with OAuth3
 * Copyright © 2009 Shahbaz Ahmer <shahbaz.ahmer@parallelmode.com>
 *
 * $Id: gmail.info 11 2009-12-20 03:15:00Z shahbaz.ahmer@parallelmode.com $
 *
 */

module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'grupal', 'settings');

/**
 * Menu callback for single sign in service.
 */
function gmail_request_token() {
    global $user;
    global $base_url, $base_path;

    if (!$user->name) {
        drupal_set_message(t('You must be logged in before accessing Google Apps.'), 'warning');
        drupal_goto();
        return false;
    }

    $user_id = $user->uid;
    $sid = get_user_school($user_id);
    if (!$sid) {
        drupal_set_message("User is not a member of any NTLP school.");
        drupal_goto();
        return false;
    }
    $sid .= '_';
    $domain = variable_get('grupal_domain'.$sid, '');
    $oauthsecret = variable_get('grupal_oauth_ckey'.$sid, '');
    $store  = OAuthStore::instance('Google');

    // We must check if this user has a authorized token
    // if they do not we need a link that says: grant access to my gmail

    // TODO:invalid old token link
    // NOTE: if they need to revoke token or if current token doesn't work this link should show up.
    if (!function_exists('getallheaders'))
    {
        function getallheaders()
        {
            return array();
        }
    }

    try {
        //echo 'requestRequestToken...<br>';
        $token = OAuthRequester::requestRequestToken($domain, $oauthsecret, $user_id);

    } catch (OAuthException $e) {
        if ($e->getMessage() == 'User not in token table.') {
            // return the grant access link since user doesn't even have an entry in the tokens table
            print 'User has no token.';
            die();
        }
        print $e->getMessage();
        // If Server Returns a 400 or something is the token useless?
    }

    // Call back location after getting the first token
    $callback_uri = $base_url.$base_path.'sites/all/modules/gmail/oauth3/service/obtain_auth.php?consumer_key='.rawurlencode($domain).'&usr_id='.intval($user_id);
//    $callback_uri = $drupalHost.$drupalRoot.'/?q=gmail/obtainauth';

    // Now redirect to the autorization uri and get us authorized
    if (!empty($token['authorize_uri']))
    {
        // Redirect to the server, add a callback to our server
        if (strpos($token['authorize_uri'], '?')) {
                $uri = $token['authorize_uri'] . '&';
        } else {
                $uri = $token['authorize_uri'] . '?';
        }
        // WARNING: google specific code
        // hd parameter tells us we want access to THIS domain's email
        $uri .= 'oauth_token='.rawurlencode($token['token']).'&oauth_callback='.rawurlencode($callback_uri).'&hd='.$domain;
    } else {
        // No authorization uri, assume we are authorized, exchange request token for access token
//      $uri = $callback_uri . '&oauth_token='.rawurlencode($token['token']).'&hd='.$domain;
        $uri = $callback_uri.'/'.rawurlencode($token['token']);
    }
    header('Location: '.$uri);
    exit();

    // Access will resume when we get back to the obtain_auth.php page

    // The call back functions obtain the access token that we keep for later
    // (make sure teh time on the expriation is a long one and that we can renew
    //  gracefully)
}

/**
 * Menu callback for single sign out service.
 *
 * Google sends auth_token as http get parameter, but drupal url format was not compatible with
 * so have to place this routine in independent php file (located in service folder)
 */
function gmail_obtain_auth($oauth_token) {
  global $user;

  if (!$user->name) {
	drupal_set_message(t('You must be logged in before accessing Google Apps.'), 'warning');
	drupal_goto();
  }

//  if (!user_access('use google apps')) {
//    //drupal_access_denied();
//  }

    //$consumer_key = $_GET['consumer_key'];
    $oauth_token  = $_GET['oauth_token'];
//    $user_id      = $_GET['usr_id'];
    $user_id = $user->uid;
    $sid = get_user_school($user_id).'_';
    $consumer_key = variable_get('grupal_domain'.$sid, '');

    try {
        OAuthRequester::requestAccessToken($consumer_key, $oauth_token, $user_id);
        // The token was succefully authorized and the gmail block will now be able
        // to show you your emails
        $message='The OAuth token was successfully authorized.';// TODO: send where to redirect back to;
        // THE CALLBACK should have been passed data to return to where you were
//        redirect($drupalRoot, $message, $delay=3);
        drupal_set_message($message, 'warning');
	drupal_goto();
    } catch (OAuthException $e) {
        $message = 'The OAuth token was not accepted by Google Apps.';
        $message .= "Due to $e";
        drupal_set_message($message, 'error');
    }
}
