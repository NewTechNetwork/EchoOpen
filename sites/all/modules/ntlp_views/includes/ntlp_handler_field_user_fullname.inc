<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


class ntlp_handler_field_user_fullname extends views_handler_field {
    
  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = array('table' => 'ntlp_user', 'field' => 'uid');
    $this->additional_fields['first_name'] = array('table' => 'ntlp_user', 'field' => 'first_name');
    $this->additional_fields['last_name'] = array('table' => 'ntlp_user', 'field' => 'last_name');
    $this->additional_fields['nid'] = array('table' => 'og_uid', 'field' => 'nid');
    $this->additional_fields['is_admin'] = array('table' => 'og_uid', 'field' => 'is_admin');
  }

  function options(&$options) {
    parent::options($options);
    $options['show_role_in_group'] = false;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['show_role_in_group'] = array(
        '#type' => 'radios',
        '#title' => 'Show user\'s role?',
        '#description' => t('Select Yes to display the user\s role in the Group with user name'),
        '#options' => array(
            'Y' => 'Yes',
            'N' => 'No',
            ),
        '#default_value' => isset($this->options['show_role_in_group']) ? $this->options['show_role_in_group'] : false,
        '#required' => FALSE,
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function click_sort($order) {
    $tableu = $this->query->ensure_table('ntlp_user');
    $this->query->add_orderby($tableu, 'last_name', $order);
    $this->query->add_orderby($tableu, 'first_name', $order);
  }

 function render($values) {
    $nid = $values->{$this->aliases['nid']};
    $is_admin = $values->{$this->aliases['is_admin']};
    $show_role = isset($this->options['show_role_in_group']) ? $this->options['show_role_in_group'] : false;

    $fname = $values->{$this->aliases['first_name']};
    $lname = $values->{$this->aliases['last_name']};
    $uid = $values->{$this->aliases['uid']};
    $fullname = '';
    if (isset($fname)) {
        $fullname .= $fname;
    }
    if (isset($lname)) {
        $fullname = $lname.', '.$fullname;
    }

    //We need to know who is the owner of this Group
    //Calling "node_load" for each record would slow the query
    //therefore we save the owner_uid in class variable
    if (!isset($this->og_owner)) {
        $objNode = node_load($nid);
        $this->og_owner = $objNode->uid;
    }

    if ($show_role) {
        if ($is_admin)
            if ($this->og_owner == $uid)
                $fullname .= ' (owner + admin)';
            else
                $fullname .= ' (admin)';
    }
    return $fullname;
//    $link = url("user/$uid");
//    $output = "<a id='user_$uid' href='$link'>$fullname</a>";
//    return $output;
  }
}

?>