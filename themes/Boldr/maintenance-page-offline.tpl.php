<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


  if (!$db_is_active) {
    global $base_path;
    echo  '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>echo</title>
<link href="'.$base_path.'themes/Boldr/CSS/server_error.css" rel="stylesheet" type="text/css" />
</head>
<body>
<div class="server_error">
<div class="server_error_main_container">
	<h1>
    	"Hold your fire. There&acute;s no life forms.<br />Its must have short-circuited."
    </h1>
    <h2>
    	Our server is a little too busy right now. Here are your options:
    </h2>
    <ul>
    	<li>Hang out for a minute and then refresh this page.</li>
    	<li>Logout out and log back in.</li>
    	<li>Take a break and try to figure out where the quote above came from.</li>
    </ul>
     <p>
     	Hint: #2 is probably your best bet. <a href="?q=logout" class="greenLink">Logout Now</a>
     </p>
</div>
<div class="server_error_arrow">
</div>
<div class="server_error_logo">
	<img src="'.$base_path.'themes/Boldr/Images/server_error/network_error_logo.png" alt="network" />
</div>
</div>
</body>
</html>
';
  }
?>
