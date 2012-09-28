<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


class ntlp_handler_argument_gbview_activity extends views_handler_argument_string {
  function query() {
    $this->ensure_my_table();
    if ($this->argument) {
        if ($this->argument == 'S') {   //Student view
            $this->query->add_where(0, 
                "($this->table_alias.available_from <= NOW() AND $this->table_alias.available_to >= NOW() )
                OR $this->table_alias.is_published = 1");
        }
    }
  }
}
