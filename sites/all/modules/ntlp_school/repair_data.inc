<?php

function repair_resource_url() {

    $searchstr = 'ntlp/resource/library/view/';
    $searchlen = strlen($searchstr);
    
    //Node_revisions
    $result = db_query("SELECT nid, path as body FROM {ntlp_course_resource} WHERE path like '%s'", "%".$searchstr."%");
    print '<br>Total records found:'.$result->num_rows.'<br>';

    if ($result) {
        while ($record = db_fetch_object($result)) {
            print '<br><br>Fixing nid:'.$record->nid;
            find_and_fixall_resourceurls($record, 'node_revisions', 'body');
        }
    }

    //Library Resource
    $result = db_query("SELECT nid, path as body FROM {ntlp_course_resource} WHERE path like '%s'", "%".$searchstr."%");
    print '<br>Total records found:'.$result->num_rows.'<br>';

    if ($result) {
        while ($record = db_fetch_object($result)) {
            print '<br><br>Fixing nid:'.$record->nid;
            find_and_fixall_resourceurls($record, 'dpl_ntlp_library_resource', 'path');
        }
    }

    //Course Resource
    $result = db_query("SELECT nid, path as body FROM {ntlp_course_resource} WHERE path like '%s'", "%".$searchstr."%");
    print '<br>Total records found:'.$result->num_rows.'<br>';

    if ($result) {
        while ($record = db_fetch_object($result)) {
            print '<br><br>Fixing nid:'.$record->nid;
            find_and_fixall_resourceurls($record, 'dpl_ntlp_course_resource', 'path');
        }
    }

    print '<br><br>';
    foreach ($fixed as $line) {
        print chr(13).chr(10);
        print $line;
        print chr(13).chr(10);
    }
}

function find_and_fixall_resourceurls($record, $table, $field) {
            
    $body = $record->body;
    $found = false;
    $replaced = false;
    while (true) {
        if ($body = strstr($body, $searchstr)) {
            $found = true;
            $t = array();
            if ($i = strpos($body, '"')) $t[] = $i;
            if ($i = strpos($body, "'")) $t[] = $i;
            if ($i = strpos($body, ' ')) $t[] = $i;
            if ($i = strpos($body, 10)) $t[] = $i;
            if ($i = strpos($body, 13)) $t[] = $i;
            if ($i = strpos($body, '>')) $t[] = $i;
            $tchar = min($t);

            if ($tchar > 0) {
                $aurl = substr($body, 0, $tchar);
            } else {
                $aurl = $body;      //link ends with field len
            }

            $args = explode('/', $aurl);
            //Swap last 2 params
            $i = $args[count($args)-2];
            $args[count($args)-2] = $args[count($args)-1];
            $args[count($args)-1] = $i;

            $nurl = implode('/', $args);

//                        print "<br>$record->nid,$tchar [".substr($body, 0, 50)."]";
//                        print "<br>".$aurl."[".$args[count($args)-2]."][".$args[count($args)-1]."]";
//                        print "<br>".$nurl;

            $rs_res = db_query("SELECT * FROM {node} WHERE nid = %d", $args[count($args)-2]);
            $rs_cat = db_query("SELECT * FROM {term_data} WHERE tid = %d", $args[count($args)-1]);
            if ($rs_res->num_rows > 0 && $rs_cat->num_rows > 0) {
                $record->body = str_replace($aurl, $nurl, $record->body);
                $replaced = true;
            } else {
                print '<br>Unable to validate nid:'.$record->nid;
//                            print '<br>'.$body;
                break;
            }
            $body = substr($body, strlen($aurl));
        } else {
            if ($found = false) print '<br>Not found for nid:'.$record->nid;
            break;
        }
    }
    if ($replaced) {
        $record->body = str_replace("'", "\'", $record->body);
        $fixed[] = "UPDATE $table SET $field = '$record->body' WHERE nid = $record->nid;";
    }
}

function repair_oldproject_dates() {

    $result = db_query("SELECT p.nid, min(st.date_started) as start, max(st.date_closed) as end
        FROM dpl_ntlp_course p
        INNER JOIN dpl_ntlp_course c ON p.course_nid = c.nid
        INNER JOIN dpl_ntlp_course_term ct ON c.nid = ct.course_nid
        INNER JOIN dpl_ntlp_school_term st ON ct.school_term_tid = st.school_term_tid
        WHERE p.is_project = 1 AND p.from_date IS NULL AND p.to_date IS NULL
        GROUP BY p.nid");
    
    print '<br>Total records found:'.$result->num_rows.'<br>';

    if ($result->num_rows > 0) {
        while ($record = db_fetch_object($result)) {
            print '<br><br>Fixing nid:'.$record->nid;
            
            db_query("UPDATE dpl_ntlp_course SET from_date = '%s', to_date = '%s' WHERE nid = %d",
                    $record->start, $record->end, $record->nid);
        }
    }
}

function repair_old_attached_filenames() {
    
    $result = db_query("select distinct f.* from dpl_files f, dpl_ntlp_resource_attachment ra where ra.pebl_attachment_fid = f.fid");
    
    print '<br>Total records found:'.$result->num_rows.'<br>';
    $i = 0;
    $nf = 0;
    if ($result->num_rows > 0) {
        while ($record = db_fetch_object($result)) {
            if (!file_exists(realpath('.').'/'.$record->filepath)) {
                print '<br><br>Fixing fid:'.$record->fid;

                $pos = strrpos($record->filepath, '/');
                $filepath = substr($record->filepath, 0, $pos+1);
                $filename = substr($record->filepath, $pos+1);
                
                $filename = substr($filename, 0, strpos($filename, '__', 2)+2).'*'; 
                
                print '<br>['.realpath('.').'/]['.$filepath.']['.$filename.']';
                $matchingfiles = glob(realpath('.').'/'.$filepath.$filename);
                if (count($matchingfiles) == 1) {
                    echo "<br>Original: " . realpath('.').'/'.$record->filepath;
                    echo "<br>Found: " . $matchingfiles[0];
                    $newpath = str_replace(realpath('.').'/', '', $matchingfiles[0]);
                    echo "<br>New Path: " . $newpath;
                    
                    db_query("UPDATE dpl_files SET filepath = '%s' WHERE fid = %d", $newpath, $record->fid);                    
                    $i++;
                    if ($i > 500) break;
                } else {
                    echo "<br>No matching file found!!";
                    $nf++;
                }
            }
        }
        echo "<br><br>Total Fixed = ".$i;
        echo "<br><br>Total Not Found = ".$nf;
    }
}