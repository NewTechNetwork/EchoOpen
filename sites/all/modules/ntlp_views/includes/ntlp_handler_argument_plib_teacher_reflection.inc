<?php
class ntlp_handler_argument_plib_teacher_reflection extends views_handler_argument_string {
  function query() {
    $this->ensure_my_table();
    if ($this->argument) {
        if (intval($this->argument) == 1) {
            $this->query->add_where(0, "$this->table_alias.teacher_comment_cid <> NULL or $this->table_alias.teacher_comment_cid <> 0 and $this->argument=$this->argument");
        }
    }
  }
}

?>