<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


require_once('OAuth.php');
module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'grupal', 'appsapis');

function get_gsites_block_contents() {

    //Theme document entries
    try {
        $domain = "";
        $feed = retrieve_google_sites($domain);
        return renderSites($feed, $domain);
    } catch (Exception $e) {
        return "Fatal error encountered in retrieving Google Sites";
    }

//    $data['docs']['html'] = listEntries($feed);
//    $data['docs']['xml'] = $feed->saveXML();
}

function retrieve_google_sites(&$domain)
{
    global $user, $base_url;

    try {
        $user_id = $user->uid;
        $sid = get_user_school($user_id);
        if (!$sid) {
            drupal_set_message("User is not a member of any NTLP school.");
            return '';
        }
        $sid .= '_';
        $domain = variable_get('grupal_domain'.$sid, '');
        $oauthsecret = variable_get('grupal_oauth_ckey'.$sid, '');
    } catch(Exception $e) {
        drupal_set_message("Unexpected error: User is not a member of any NTLP school.");
        return '';
    }

    $data['docs']['html'] = '';
    $data['docs']['xml'] = '';

    sleep(1);	//To avoid flooding on Google Apps site

    watchdog('gsite', "Opening for Domain:$domain/$oauthsecret, User:".$user->name);
    $consumer = new OAuth2Consumer($domain, $oauthsecret, NULL);
    // Setup OAuth request based our previous credentials and query

    //$base_feed = 'http://www.google.com/m8/feeds/contacts/default/full/';
    $base_feed = 'https://sites.google.com/feeds/site/'.$domain;
    $params = array('max-results' => 10, 'xoauth_requestor_id' => $user->name);

    $query = $base_feed . '?' . implode_assoc('=', '&', $params);
    $request = OAuth2Request::from_consumer_and_token($consumer, NULL, 'GET', $base_feed, $params);

    // Sign the constructed OAuth request using HMAC-SHA1
    $request->sign_request(new OAuth2SignatureMethod_HMAC_SHA1(), $consumer, NULL);

    $httpClient = new Zend_Http_Client();
    $httpClient->setHeaders($request->to_header());

    $docsService = new Zend_Gdata($httpClient);
    $feed = $docsService->getFeed($query);

    return $feed;
//    $data['docs']['html'] = listEntries($feed);
//    $data['docs']['xml'] = $feed->saveXML();
}

function renderSites($feed, $domain)
{
    global $base_path;
    include "template.inc";
    $html = '';
    $count = 0;
    $documents = '';
    foreach($feed->entries as $entry)
    {
        $count ++;
        if (count > 1)
            $documents .= $HTMLseparator;

        // Find the URL of the HTML view of the document.
        foreach ($entry->link as $link)
        {
            if ($link->getRel() === 'alternate')
            {
                $alternateLink = $link->getHref();
            }
        }
        $lastupdated = date('M j, Y g:i a',strtotime($entry->getUpdated()));

        $rHTMLrow = $HTMLrow;
        $rHTMLrow = str_replace('@DRUPAL_BASE', $base_path, $rHTMLrow);
        $rHTMLrow = str_replace('@ICON_DOC', 'siteIcon', $rHTMLrow);
        $rHTMLrow = str_replace('@DOC_LINK', $alternateLink, $rHTMLrow);
        $rHTMLrow = str_replace('@DOC_NAME', $entry->title->text, $rHTMLrow);
        $rHTMLrow = str_replace('@LAST_UPDATED', $lastupdated, $rHTMLrow);
        $documents .= $rHTMLrow;
    }

    if ($count == 0)
        $rMsgCount = "(No sites found)";
    else {
        if ($count == 1)
            $rMsgCount = "(1 site found)";
        else
            $rMsgCount = "($count sites found)";
    }
    
    $allDocslink = 'https://sites.google.com/a/'.$domain;

    $rHTMLcontainer = $HTMLcontainer;
    $rHTMLcontainer = str_replace('@FEED_TITLE', 'Sites', $rHTMLcontainer);
    $rHTMLcontainer = str_replace('@DOCS_COUNT', $rMsgCount, $rHTMLcontainer);
    $rHTMLcontainer = str_replace('@DOCUMENTS', $documents, $rHTMLcontainer);
    $rHTMLcontainer = str_replace('@DOCS_LINK', $allDocslink, $rHTMLcontainer);
    $rHTMLcontainer = str_replace('@DRUPAL_BASE', $base_path, $rHTMLcontainer);
    $html .= $rHTMLcontainer;

    return $html;
}

?>
