<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


define('NTLP_ROLEID_SITEGUEST', 3);     //New Name: Partners
define('NTLP_ROLEID_PARENT', 4);
define('NTLP_ROLEID_STUDENT', 5);
define('NTLP_ROLEID_TEACHER', 6);
define('NTLP_ROLEID_DIRECTORS', 7);
define('NTLP_ROLEID_SITEADMIN', 8);     //New Name: Tech Manager
define('NTLP_ROLEID_NETWORKGUEST', 9);
define('NTLP_ROLEID_NETWORKSTAFF', 10);
define('NTLP_ROLEID_NETWORKMANAGER', 11);
define('NTLP_ROLEID_SCHOOLLEADER', 13);
define('NTLP_ROLEID_SYSTEM_ADMIN', 12);

define("NTLP_VOCABULARY_SUBJECTS", 8);
define("NTLP_VOCABULARY_POSTS", 2);
define("NTLP_VOCABULARY_REGIONS", 3);
define("NTLP_VOCABULARY_C_UNIT", 22);

// add new vocab by saifuddin for gb outcome
define("VOCAB_NTLP_GB_OUTCOME", 6);

define("NTLP_KEYWORDS_FOR_PROJECT_LIBRARY", 23);
define("NTLP_KEYWORDS_FOR_SCHOOL_DOCUMENT_LIBRARY", 24);
define("NTLP_KEYWORDS_FOR_NETWORK_RESOURCE_LIBRARY", 25);

define('DATE_FORMAT', '%Y-%m-%d %H:%M:%S');

define('DATE_FORMAT_CHECK', 'Y-m-d H:i:s');

define('NTLP_DATE_TZ_LONG', 'l, m/d/Y g:i a');
define('NTLP_DATE_TZ_MIN', 'n/j/y');
define('NTLP_DATE_TZ_SHORT', 'm/d/Y');
define('NTLP_DATE_TZ_TIME', 'm/d/Y g:ia');
define('NTLP_DATE_TZ_SQL', 'Y-m-d H:i:s');
define('NTLP_DATE_TZ_FJY', 'F j, Y');
define('NTLP_DATE_TZ_MJY', 'M j, Y');
//variable_set('date_tz_long', 'l, m/d/Y g:i a');
//variable_set('date_tz_short', 'm/d/Y');
//variable_set('date_tz_time', 'm/d/Y g:ia');
//variable_set('date_tz_sql', 'Y-m-d H:i:s');
//variable_set('date_tz_FjY', 'F j, Y');

define('DEFAULT_LOCAL_TIMEZONE_ID', 487);
define('MIN_SCHOOL_YEAR', 1970);

define('DATE_FORMAT_ACTIVITY_STATUS', 'D m/d/Y @ g:i a');
define('DATE_FORMAT_LAST_SAVED', 'm/d/Y @ g:i a');
define('DATE_FORMAT_SUBMISSION', 'm/d/Y g:i a');
define('NTLP_LIABRARY_SCHOOL_DOCUMENT_CATEGORY', 18);

/**** School Wise permission START ***/

define('CREATE_SCHOOL_WIDE_EVENTS','create school wide events');
define('CREATE_NETWORK_EVENTS','create network events');
define('CREATE_NETWORK_EVENTS','create network events');
define('CREATE_NETWORK_EVENTS','create network events');

/* User security */
define('MIN_PASSWORD_LENGTH', 8);

//Activity Response Types
define('ACT_RES_TYPE_EXP','Explanatory');
define('ACT_RES_TYPE_NAR','Narrative');
define('ACT_RES_TYPE_ARG','Argumentative');

//Activity Response Length
define('ACT_RES_LEN_1','1 Page');
define('ACT_RES_LEN_2_4','2-4 Pages');
define('ACT_RES_LEN_5_10','5-10 Pages');
define('ACT_RES_LEN_10','10+ Pages');


/*Notification ID constants */

define('NOTF_SHARE_STUFF_ITEM_POSTED_TO_COURSE', 6);
