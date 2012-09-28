<?php

function get_request_type($request_type) {

    $rs = db_query("SELECT td.name,td.tid FROM {vocabulary} vo LEFT JOIN {term_data} td ON td.vid=vo.vid
        WHERE vo.name='%s'",$request_type);

    while ($note = db_fetch_array($rs)) {
        $term[$note['tid']] = $note['name'];
    }
    return $term;

}
?>
