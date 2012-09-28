<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


class ntlp_handler_field_group_managelinks extends views_handler_field {
  function construct() {
    parent::construct();
//    $this->additional_fields['og_owner'] = array('table' => 'node', 'field' => 'uid');
    $this->additional_fields['uid'] = array('table' => 'og_uid', 'field' => 'uid');
    $this->additional_fields['nid'] = array('table' => 'og_uid', 'field' => 'nid');
    $this->additional_fields['is_admin'] = array('table' => 'og_uid', 'field' => 'is_admin');
//    $this->additional_fields['og_selective'] = array('table' => 'og', 'field' => 'og_selective');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    global $user;

//    $og_owner = $values->{$this->aliases['og_owner']};
    $nid = $values->{$this->aliases['nid']};
    $uid = $values->{$this->aliases['uid']};
    $is_admin = $values->{$this->aliases['is_admin']};
//    $og_access = $values->{$this->aliases['og_selective']};
//    $link = isset($this->options['manage_link']) ? $this->options['manage_link'] : 'A';
    $output = "<div>";

    //We need to know who is the owner of this Group
    //Calling "node_load" for each record would slow the query
    //therefore we save the owner_uid in class variable
    if (isset($this->og_owner)) {
//        $og_owner = $this->og_owner;
//        watchdog('vog', "Loading owner:$og_owner for node:$nid");
    }
    else {
        $objNode = node_load($nid);
        $this->og_owner = $objNode->uid;
//        $og_owner = $this->og_owner;
        $account = user_load(array('uid' => $user->uid));
        $this->user_isadmin = og_is_group_admin($objNode, $account);
//        watchdog('vog', "Setting owner:$og_owner for node:$nid");
    }

//    $objNode = node_load($nid);
//    $account = user_load(array('uid' => $uid));
    
    if ($this->user_isadmin) {      //Only show links to Admin users
        if ($this->og_owner != $uid) {        //Owner cannot be removed as admin
            if ($is_admin) {
                $link = url("ntlp/group/remove/$nid/$uid");
                $linkname = 'Remove Admin';
                $output .= "<a class='jslink' id='mg_admin_$uid' onclick='onchange_admin($nid, $uid, 0)'>$linkname</a>&nbsp;&nbsp;";
            } else {
                $link = url("ntlp/group/join/$nid/$uid");      
                $linkname = 'Make Admin';
                $output .= "<a class='jslink' id='mg_admin_$uid' onclick='onchange_admin($nid, $uid, 1)'>$linkname</a>&nbsp;&nbsp;";
            }
            if (is_group_member($nid, $uid)) {
                $link = url("ntlp/group/remove/$nid/$uid");
                $linkname = 'Remove';
                $output .= "<a class='jslink' id='mg_join_$uid' onclick='drop_member($nid, $uid)'>$linkname</a>";
            } else {
                $link = url("ntlp/group/join/$nid/$uid");       //This condition would never hit.
                $linkname = 'Join';
                $output .= "<a class='jslink' id='mg_join_$uid' onclick='enroll_member($nid, $uid)'>$linkname</a>";
            }
        }
    }
    $output .= '</div>';
    return $output;
  }
}
