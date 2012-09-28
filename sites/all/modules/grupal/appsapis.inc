<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


/* 
 * grupal - Google Apps integrations for Drupal
 * Copyright © 2009 Sam Johnston <samj@samj.net> http://samj.net/
 *                  Australian Online Solutions Pty Ltd http://www.aos.net.au/
 * 
 * $Id: appsapis.inc 59 2009-09-05 10:05:35Z samj@samj.net $
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

/**
 * Add Zend Framework to include path
 */
//$drupalRoot='/drupal';
module_load_include('inc', 'grupal', 'settings');

//require_once(dirname(__FILE__).'/settings.php');

global $base_path;
$path = $_SERVER["DOCUMENT_ROOT"].$base_path;
/**
 * @see Zend_Loader
 */
$path = dirname(__FILE__).'/../../../../';
require_once $path.'Zend/Loader.php';

/**
 * @see Zend_Gdata
 */
Zend_Loader::loadClass('Zend_Gdata', $path);
Zend_Loader::loadClass('Zend_Gdata_Docs', $path);
require_once $path.'Zend/Gdata.php';

/**
 * @see Zend_Gdata_ClientLogin
 */
Zend_Loader::loadClass('Zend_Gdata_ClientLogin', $path);

/**
 * @see Zend_Gdata_Gapps
 */
Zend_Loader::loadClass('Zend_Gdata_Gapps', $path);

/**
 * Returns a HTTP client object with the appropriate headers for communicating
 * with Google using the ClientLogin credentials supplied.
 *
 * @param  string $user The username, in e-mail address format, to authenticate
 * @param  string $pass The password for the user specified
 * @return Zend_Http_Client
 */
function getClientLoginHttpClient($user, $pass) 
{
  $service = Zend_Gdata_Gapps::AUTH_SERVICE_NAME;
  $client = Zend_Gdata_ClientLogin::getHttpClient($user, $pass, $service);
  return $client;
}
?>