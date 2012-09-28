<?php

module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_school', 'data_access');

function _get_node_title($nid) {

    $result = db_query("SELECT title FROM dpl_node WHERE nid = %d", $nid);

    if ($result) {
        $data = db_fetch_object($result);
        return $data->title;
    } else {
        return '';
    }
}

