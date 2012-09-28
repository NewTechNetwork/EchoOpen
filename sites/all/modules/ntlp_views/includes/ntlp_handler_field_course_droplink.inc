<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


class ntlp_handler_field_course_droplink extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['user_uid'] = array('table' => 'og_uid', 'field' => 'uid');
    $this->additional_fields['course_nid'] = array('table' => 'og_uid', 'field' => 'nid');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function options(&$options) {
    parent::options($options);
    $options['manage_link'] = 'D';
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['manage_link'] = array(
        '#type' => 'radios',
        '#title' => 'Link Type',
        '#description' => t('Select the link for enrollment management.'),
        '#options' => array(
            'D' => 'Drop Students',
            'R' => 'Remove Guests',
            ),
        '#default_value' => isset($this->options['manage_link']) ? $this->options['manage_link'] : '',
        '#required' => TRUE,
    );
  }

  function render($values) {
    $nid = $values->{$this->aliases['course_nid']};
    $uid = $values->{$this->aliases['user_uid']};
    $link = isset($this->options['manage_link']) ? $this->options['manage_link'] : 'D';
    $output = "N/A";
    
    global $user;
//    if (in_array($nid, array_keys($user->og_groups))) {
        if ($link == 'D') {
            $link = url("ntlp/admincourse/drop/$nid/$uid");
            $linkname = 'Drop';
            $output = "<a class='jslink' id='join_$uid' onclick='drop_member($nid, $uid, 0)'>$linkname</a>";
        } else {
            $link = url("ntlp/admincourse/remove/$nid/$uid");
            $linkname = 'Remove';
            $output = "<a class='jslink' id='join_$uid' onclick='drop_member($nid, $uid, 1)'>$linkname</a>";
        }
//    }
    return $output;
  }
}
