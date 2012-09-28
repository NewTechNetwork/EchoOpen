<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'constants');

class ntlp_handler_field_course_joinlink extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['user_uid'] = array('table' => 'ntlp_course_user', 'field' => 'user_uid');
    $this->additional_fields['course_nid'] = array('table' => 'ntlp_course_user', 'field' => 'course_nid');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

//  function options(&$options) {
//    parent::options($options);
//    $options['manage_link'] = 'D';
//  }
//
//  function options_form(&$form, &$form_state) {
//    parent::options_form($form, $form_state);
//
//    $form['manage_link'] = array(
//        '#type' => 'radios',
//        '#title' => 'Link Type',
//        '#description' => t('Select the link for enrollment management.'),
//        '#options' => array(
//            'D' => 'Drop',
//            'R' => 'Re-enroll',
//            ),
//        '#default_value' => isset($this->options['manage_link']) ? $this->options['manage_link'] : '',
//        '#required' => TRUE,
//    );
//  }

  function render($values) {
    $nid = $values->{$this->aliases['course_nid']};
    $uid = $values->{$this->aliases['user_uid']};
//    $link = isset($this->options['manage_link']) ? $this->options['manage_link'] : 'D';
    $output = "N/A";

    $link = url("ntlp/admincourse/enroll/$nid/".NTLP_ROLEID_STUDENT."/$uid");
    $linkname = 'Re-enroll';    //href='$link'
    $output = "<a class='jslink' id='join_$uid' onclick='enroll_student($nid, $uid, ".NTLP_ROLEID_STUDENT.")'>$linkname</a>";
    return $output;
  }
}
