<?php
class ntlp_handler_field_gradeall_link extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['course_nid'] = array('table' => 'ntlp_course_activity', 'field' => 'course_nid');
    $this->additional_fields['nid'] = array('table' => 'ntlp_course_activity', 'field' => 'nid');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $cnid = $values->{$this->aliases['course_nid']};
    $anid = $values->{$this->aliases['nid']};

    $output = '';
    $link = url("ntlp/courses/activity/gradeall/$cnid/$anid");
    $output = "<a class='jslink' id='grade_$auid' href='$link'>Score</a>";
    return $output;
  }
}
?>