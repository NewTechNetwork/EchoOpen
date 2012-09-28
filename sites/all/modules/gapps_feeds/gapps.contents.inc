<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


require_once('OAuth.php');
module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'grupal', 'appsapis');

function get_gdocs_block_contents($delta)
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

    $consumer = new OAuth2Consumer($domain, $oauthsecret, NULL);
    // Setup OAuth request based our previous credentials and query

    //$base_feed = 'http://www.google.com/m8/feeds/contacts/default/full/';
    $base_feed = 'https://docs.google.com/feeds/documents/private/full';
    $params = array('max-results' => 10, 'xoauth_requestor_id' => $user->name);

    $query = $base_feed . '?' . implode_assoc('=', '&', $params);
    $request = OAuth2Request::from_consumer_and_token($consumer, NULL, 'GET', $base_feed, $params);

    // Sign the constructed OAuth request using HMAC-SHA1
    $request->sign_request(new OAuth2SignatureMethod_HMAC_SHA1(), $consumer, NULL);

    $httpClient = new Zend_Http_Client();
    $httpClient->setHeaders($request->to_header());

    $docsService = new Zend_Gdata_Docs($httpClient);
    $feed = $docsService->getDocumentListFeed($query);

    //Theme document entries
    return listEntries($feed);

//    $data['docs']['html'] = listEntries($feed);
//    $data['docs']['xml'] = $feed->saveXML();
}

/**
 * Joins key:value pairs by inner_glue and each pair together by outer_glue
 * @param string $inner_glue The HTTP method (GET, POST, PUT, DELETE)
 * @param string $outer_glue Full URL of the resource to access
 * @param array $array Associative array of query parameters
 * @return string Urlencoded string of query parameters
 */
function implode_assoc($inner_glue, $outer_glue, $array) {
  $output = array();
  foreach($array as $key => $item) {
    $output[] = $key . $inner_glue . urlencode($item);
  }
  return implode($outer_glue, $output);
}

/**  * Creates an HTML list of each <entry>'s title.  *
 * @param Zend_Gdata_Feed $feed A Gdata feed object
 * @return string The HTML of entries
 */
function listEntries($feed)
{
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
        $rHTMLrow = $HTMLrow;
        $rHTMLrow = str_replace('@DRUPAL_BASE', $base_path, $rHTMLrow);
        $rHTMLrow = str_replace('@DOC_LINK', $alternateLink, $rHTMLrow);
        $rHTMLrow = str_replace('@DOC_NAME', $entry->title->text, $rHTMLrow);
        $rHTMLrow = str_replace('@LAST_UPDATED', '', $rHTMLrow);
        $documents .= $rHTMLrow;
        //$str .= "<li><a href=\"{$alternateLink}\" target=\"new\">{$entry->title->text}</a></li>";
    }

    if ($count == 0)
        $rMsgCount = "(No documents found)";
    else {
        if ($count == 1)
            $rMsgCount = "(1 document found)";
        else
            $rMsgCount = "($count documents found)";
    }
    
    $allDocslink = 'https://docs.google.com/a/'.$domain;

    $rHTMLcontainer = $HTMLcontainer;
    $rHTMLcontainer = str_replace('@DOCS_COUNT', $rMsgCount, $rHTMLcontainer);
    $rHTMLcontainer = str_replace('@DOCUMENTS', $documents, $rHTMLcontainer);
    $rHTMLcontainer = str_replace('@DOCS_LINK', $allDocslink, $rHTMLcontainer);
    $rHTMLcontainer = str_replace('@DRUPAL_BASE', $base_path, $rHTMLcontainer);
    $html .= $rHTMLcontainer;

    return $html;
}

?>
