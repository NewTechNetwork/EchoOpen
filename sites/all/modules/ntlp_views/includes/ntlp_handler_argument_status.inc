<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


class ntlp_handler_argument_status extends views_handler_argument_string {
  function query() {
    $this->ensure_my_table();
    if ($this->argument) {
//        $filter = "$this->table_alias.available_from <= CURDATE() AND $this->table_alias.available_to >= CURDATE()";
        $filter1 = "($this->table_alias.availability_mode = 'S' AND $this->table_alias.available_from <= NOW() AND $this->table_alias.available_to >= NOW())";
        $filter2 = "($this->table_alias.availability_mode = 'M' AND $this->table_alias.available_from <> '0000-00-00 00:00:00')";
        if ($this->argument == 'S') {
            $this->query->add_where(0, $filter1 .' OR '. $filter2);
        } else {
            $this->query->add_where(0, 'NOT ('. $filter1 .' OR '. $filter2 . ')');
        }
    }
  }
}

?>