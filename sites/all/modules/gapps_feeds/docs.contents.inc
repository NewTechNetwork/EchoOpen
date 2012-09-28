<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


require_once('OAuth.php');
module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'grupal', 'appsapis');

function get_gdocs_block_contents() {

    //Theme document entries
    try {
        $domain = "";
        $feed = retrieve_google_docs($domain);
        return renderDocs($feed, $domain);
    } catch (Exception $e) {
        return "Fatal error encountered in retrieving Google Docs";
    }

//    $data['docs']['html'] = listEntries($feed);
//    $data['docs']['xml'] = $feed->saveXML();
}

function retrieve_google_docs(&$domain) {
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
//    $params = array('max-results' => 10, 'xoauth_requestor_id' => $user->name);
    $params = array('xoauth_requestor_id' => $user->name);

    $query = $base_feed . '?' . implode_assoc('=', '&', $params);
    $request = OAuth2Request::from_consumer_and_token($consumer, NULL, 'GET', $base_feed, $params);

    // Sign the constructed OAuth request using HMAC-SHA1
    $request->sign_request(new OAuth2SignatureMethod_HMAC_SHA1(), $consumer, NULL);

    $httpClient = new Zend_Http_Client();
    $httpClient->setHeaders($request->to_header());

    $docsService = new Zend_Gdata_Docs($httpClient);
    $feed = $docsService->getDocumentListFeed($query);

    return $feed;

}

/**  * Creates an HTML list of each <entry>'s title.  *
 * @param Zend_Gdata_Feed $feed A Gdata feed object
 * @return string The HTML of entries
 */
function renderDocs($feed, $domain) {
    global $base_path;
    include "template.inc";
    $html = '';
    $count = 0;
    $documents = '';
    foreach($feed->entries as $entry) {
        $count ++;
        if ($count > 1)
            $documents .= $HTMLseparator;
        if ($count > 10)
            break;
        // Find the URL of the HTML view of the document.
        foreach ($entry->link as $link) {
            if ($link->getRel() === 'alternate') {
                $alternateLink = $link->getHref();
            }
        }
        $doctype = '';
        foreach ($entry->category as $cat) {
            if (strpos($cat->getScheme(), '#kind'))
                $doctype = $cat->getLabel();
//            if ($cat->getLabel() === 'document') //spreadsheet, presentation
        }
        $lastupdated = date('M j, Y g:i a',strtotime($entry->getUpdated()));

        if(strlen($entry->title->text) > 30){
            $new_doc_title = substr($entry->title->text, 0 , 30) . "...";
        }
        else {
            $new_doc_title = $entry->title->text ;
        }

        $rHTMLrow = $HTMLrow;
        $rHTMLrow = str_replace('@DRUPAL_BASE', $base_path, $rHTMLrow);
        $rHTMLrow = str_replace('@ICON_DOC', $doctype.'Icon', $rHTMLrow);
        $rHTMLrow = str_replace('@DOC_TITLE', $entry->title->text, $rHTMLrow);
        $rHTMLrow = str_replace('@DOC_LINK', $alternateLink, $rHTMLrow);
        $rHTMLrow = str_replace('@DOC_NAME', $new_doc_title, $rHTMLrow);
        $rHTMLrow = str_replace('@LAST_UPDATED', $lastupdated, $rHTMLrow);
        $documents .= $rHTMLrow;
        //$str .= "<li><a href=\"{$alternateLink}\" target=\"new\">{$entry->title->text}</a></li>";
    }

    if ($count == 0)
        $rMsgCount = "(No documents found)";
    else {
        if ($count == 1)
            $rMsgCount = "(1 document found)";
        else {
            $count = 0;
            foreach($feed->entries as $entry) {
                $count ++;
    }
            $rMsgCount = "($count documents found)";
        }
    }

    $allDocslink = 'https://docs.google.com/a/'.$domain;

    $rHTMLcontainer = $HTMLcontainer;
    $rHTMLcontainer = str_replace('@FEED_TITLE', 'Docs', $rHTMLcontainer);
    $rHTMLcontainer = str_replace('@DOCS_COUNT', $rMsgCount, $rHTMLcontainer);
    $rHTMLcontainer = str_replace('@DOCUMENTS', $documents, $rHTMLcontainer);
    $rHTMLcontainer = str_replace('@DOCS_LINK', $allDocslink, $rHTMLcontainer);
    $rHTMLcontainer = str_replace('@DRUPAL_BASE', $base_path, $rHTMLcontainer);
    $html .= $rHTMLcontainer;

    return $html;
}

?>
