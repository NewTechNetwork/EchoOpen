<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


class ntlp_handler_field_group_postcount extends views_handler_field_numeric {

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();

    //If there is an argument defined for OG, then use it as a Group Nid
    $groupnid = 0;
    foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
        if ($handler->ui_name() == "Organic groups: Group node")
            $groupnid = $handler->get_value();
    }
    if (empty($groupnid)) $groupnid = 0;      //To protect queries in returning error

    $tableu = $this->query->ensure_table('og_uid');
    $sql = "SELECT COUNT(n.nid) FROM {node} n
        INNER JOIN {ntlp_course_resource} cr ON n.nid = cr.nid
        WHERE n.status = 1 AND cr.course_nid = $groupnid
        AND n.uid = $tableu.uid";

    $this->query->add_field('', "($sql)", 'group_post_count');
    $this->field_alias = 'group_post_count';
  }

  function render($values) {
    $value = $values->{$this->field_alias};
    $output = number_format($value, 0, '.', '');
    return $output;
  }
}
