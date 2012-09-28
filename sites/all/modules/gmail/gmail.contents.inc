<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


require_once('oauth3/OAuthRequester.php');
require_once(drupal_get_path('module', 'feedapi').'/parser_simplepie/simplepie.inc');
module_load_include('inc', 'ntlp_school', 'data_access');

function get_gmail_block_contents()
{
    include "template.inc";
    global $user, $base_path;
    
    $msgnumber = 5;		// Max messages to display

    $user_id = $user->uid;
    $sid = get_user_school($user_id);
    if (!$sid) {
        drupal_set_message("User is not a member of any NTLP school.");
        return;
    }
    $sid .= '_';
    $domain = variable_get('grupal_domain'.$sid, '');
    $oauthsecret = variable_get('grupal_oauth_ckey'.$sid, '');

    $feederror = false;
    $html = '';

    try {

        $request_uri='https://mail.google.com/mail/feed/atom';
        // Do we have a token for this user???
        // if not return error print "no token found for" exit();
        // if this is a curl call you can't use global user here
        //$user_id= 5;
        //$request_uri = 'https://mail.google.com/mail/feed/atom';

        try {
            $req = new OAuthRequester($request_uri,'GET', $params=null);
            $result = $req->doRequest($user_id, $domain, $oauthsecret); //throws OAuthException exception on an error

            // $result is an array of the form: array ('code'=>int, 'headers'=>array(), 'body'=>string)
            $feeddata = $result['body'];
            //echo "<br>Body:[".$feeddata."]<br>";
        } catch (OAuthException $e) {
            //print "Error: $e";
        }

        // TODO: how to return whatever error it says
        // should return feed body Output while still testing
        if ( empty($feeddata) or !empty($e) ) {
            //$feeddata = "FALSE Error Message:".print($e);
            // print "reasons for false or error info"; // or just log the error info
        }
    } catch (OAuthException $e) {
        echo "Error: Feed could not be obtained $e->message";
    }

    // Identify Type of Error and handle
    // These use error message strings not moodle strings because they are always in code only
    if( empty($feeddata) or strpos($feeddata,'User not in token table.')
                         or strpos($feeddata,'User has no token.')
                         or strpos($feeddata,'Error 401')
                         or strpos($feeddata,'Unauthorized')) {
        $feederror = true;

        $req_token_link = url('gmail/authorize');
        $rHTMLemail = $HTMLunauthorized;
        $rHTMLemail = str_replace('@AUTHORIZE_LINK', $req_token_link, $rHTMLemail);
        $html_emails .= $rHTMLemail;
        $rMsgCount = '';
    }


    // TODO: Revoke Token option link
    // If no error parse messages and process for display
    if (!$feederror) {
        // Parse google atom feed
        $feed = new SimplePie();
        $feed->set_raw_data($feeddata);
        $status = $feed->init();
        $msgs = $feed->get_items();

        $countmsg = true;
        $mc = 0;
        $html_emails = '';
        foreach($msgs as $msg) {
            if($countmsg and $mc == $msgnumber){
                break;
            }
            $author = $msg->get_author();
            if (isset($author)) {
                // Displaying Message Data
                if ($mc > 0) {
                    $html_emails .= $HTMLseparator;
                }
                $mc++;

                $author->get_name();
                $summary = $msg->get_description();

                // Google partners need a special gmail url
                $servicelink = $msg->get_link();
                $servicelink = str_replace('http://mail.google.com/mail','http://mail.google.com/a/'.$domain, $servicelink);

                // To Save Space given them option to show first and last or just last name
                @list($author_first,$author_last) = split(" ",$author->get_name());

                // Show first Name
                if( !$showfirstname = true) {
                    $author_first = '';
                }

                // Show last Name
                if( !$showlastname = true) {
                    $author_last = '';
                }

                $rHTMLemail = $HTMLemail;
                $eDate = getdate($msg->get_date());
                $cDate = getdate(time());
                if ($eDate[year] == $cDate[year] && $eDate[ytd] == $cDate[ytd]) {
                    $rDate = ($eDate[hour] >= 12 ? $eDate[hour]-12 : $eDate[hour])
                            . ':' . $eDate[min]
                            .($eDate[hour] >= 12 ? ' pm' : ' am');
                } else {
                    $rDate = $eDate[month].' '.$eDate[mday];
                }

                $rHTMLemail = str_replace('@EMAIL_LINK', $servicelink, $rHTMLemail);
                $rHTMLemail = str_replace('@EMAIL_FROM', $author_first.' '.$author_last, $rHTMLemail);
                $rHTMLemail = str_replace('@EMAIL_SUBJECT', $msg->get_title(), $rHTMLemail);
                $rHTMLemail = str_replace('@EMAIL_SUMMARY', substr($summary,0,50), $rHTMLemail);
                $rHTMLemail = str_replace('@EMAIL_TEASER', $msg->get_title(), $rHTMLemail);
                $rHTMLemail = str_replace('@EMAIL_DATETIME', $rDate, $rHTMLemail);
                $html_emails .= $rHTMLemail;
            }
        }
        $rMsgCount = count($msgs) > 0 ? '('.count($msgs) : '(No';
        $rMsgCount .= count($msgs) == 1 ? ' new message)' : ' new messages)';
    }

    $inboxlink =   'https://mail.google.com/a/'.$domain;
    $composelink = $inboxlink.'/?AuthEventSource=SSO#compose';

    $rHTMLcontainer = $HTMLcontainer;
    $rHTMLcontainer = str_replace('@NEW_MESSAGES', $rMsgCount, $rHTMLcontainer);
    $rHTMLcontainer = str_replace('@GMAIL_INBOX', $inboxlink, $rHTMLcontainer);
    $rHTMLcontainer = str_replace('@GMAIL_COMPOSE', $composelink, $rHTMLcontainer);
    $rHTMLcontainer = str_replace('@GMAIL_NEWMAILS', $html_emails, $rHTMLcontainer);
    $rHTMLcontainer = str_replace('@DRUPAL_BASE', $base_path, $rHTMLcontainer);
    $html .= $rHTMLcontainer;


    return $html;
}


?>
