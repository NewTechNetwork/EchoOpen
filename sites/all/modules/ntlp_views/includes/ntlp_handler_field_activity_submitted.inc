<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


/**
 * We do not try to run db_rewrite_sql() on this subquery so the count is potentially more than the user can see.
 */
class ntlp_handler_field_activity_submitted extends views_handler_field {
    function query() {
        $table = $this->query->ensure_table('ntlp_course_activity');
        $sql = "SELECT (sum(case when s.is_final=1 then 1 else 0 end)/(SELECT count(*) from dpl_ntlp_activity_user au where au.activity_nid =  $table.nid))*100
            FROM dpl_ntlp_activity_submission s
            INNER JOIN dpl_ntlp_course_activity a ON s.activity_nid = a.nid
            INNER JOIN dpl_ntlp_activity_user au ON s.activity_nid = au.activity_nid and au.user_uid = s.user_uid
            INNER JOIN dpl_ntlp_course_user cu ON a.course_nid = cu.course_nid and cu.user_uid = s.user_uid
            WHERE s.activity_nid = $table.nid
            GROUP BY s.activity_nid";
        $this->query->add_field('', "($sql)", 'submissions_percent');
        $this->field_alias = 'submissions_percent';
  }

  function render($values) {
    $value = $values->{$this->field_alias};
    if ((int) $value === 0) {
      return '';
    }
    return number_format($value, 0, '.', '').'%';
  }


}
?>