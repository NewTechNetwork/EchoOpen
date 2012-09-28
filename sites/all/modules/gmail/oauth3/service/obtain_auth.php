<?php

/**
 * Obtains the access token after the user grants access
 * @author Chris B Stones based off Marc Worrell's code
 *
 * The MIT License
 *
 * Copyright (c) 2007-2008 Mediamatic Lab
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

//require_once('../../../../config.php');
//require_login();
//global $USER,$CFG;
$curdir = getcwd();
//echo '<br>'.$curdir;
//echo '<br>'.$docroot;
//echo '<br>'.dirname(__FILE__);
global $base_path;
require_once(dirname(__FILE__).'/../../settings.inc');
require_once(dirname(__FILE__).'/../OAuthRequester.php');
$docroot =  $_SERVER['DOCUMENT_ROOT'].$base_path;
chdir ($docroot);
require_once($docroot.'/includes/bootstrap.inc');
require_once($docroot.'/includes/common.inc');
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);
module_load_include('inc', 'ntlp_school', 'data_access');
chdir ($curdir);

if (!function_exists('getallheaders'))
{
    function getallheaders()
    {
        return array();
    }
}

$consumer_key = $_GET['consumer_key'];
$oauth_token  = $_GET['oauth_token'];
$user_id      = $_GET['usr_id'];

try {
    OAuthRequester::requestAccessToken($consumer_key, $oauth_token, $user_id);
    // The token was succefully authorized and the gmail block will now be able
    // to show you your emails
    $message='Gmail access of your account has been successfully authorized!';
    // THE CALLBACK should have been passed data to return to where you were
    drupal_set_message($message, 'info');
    drupal_goto($base_path);
} catch (OAuthException $e) {
    $message = 'The OAuth token was not accepted by Google Apps.';
    $message .= "Due to $e";
    echo $message;
    drupal_set_message($message, 'error');
}

?>
