<?php
class ntlp_handler_argument_node_promote extends views_handler_argument_numeric {
  function query() {
    $node_alias = $this->query->ensure_table('node');
    if ($this->argument) {
        if (intval($this->argument) == 1) {
            $this->query->add_where(0, "$node_alias.promote = 1");
        }
    }
  }
}

?>