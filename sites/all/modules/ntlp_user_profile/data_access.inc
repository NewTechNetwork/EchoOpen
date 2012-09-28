<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


# Fetching User Record For User Profile View Page


function arrayToObject($array) {
    if(!is_array($array)) {
        return $array;
    }

    $object = new stdClass();
    if (is_array($array) && count($array) > 0) {
        foreach ($array as $name=>$value) {
            $name = strtolower(trim($name));
            if (!empty($name)) {
                $object->$name = arrayToObject($value);
            }
        }
        return $object;
    }
    else {
        return FALSE;
    }
}

function get_advisor_record($user_id) {

    $result = db_query("SELECT n.title, nu.uid, nu.first_name, nu.last_name
        FROM dpl_ntlp_course_user uc
        INNER JOIN dpl_node n ON n.nid = uc.course_nid
        INNER JOIN dpl_ntlp_course nc ON n.nid = nc.nid
        INNER JOIN dpl_ntlp_user nu ON nu.uid = nc.teacher1_uid
        WHERE uc.user_uid = ".$user_id." AND nc.is_advisory_course = 1 order by n.created LIMIT 1");

    if($result->num_rows > 0 ) {
        return db_fetch_object($result);
    }else{
        return false;
    }
}


function _get_my_roles($useruid) {
    $sql = "select rid from {users_roles} where uid=%d";
    $result = db_query($sql,$useruid);
    if($result) {
        while($rows = db_fetch_object($result)) {
            $myroles[] = $rows->rid;
        }
        return $myroles;
    }
    else return false;

}

function load_profile_record($user_id) {
    $sql = "SELECT *,r.name as role_name,u.name as myname
        FROM {users} u
        LEFT JOIN {ntlp_user} n ON n.uid = u.uid
        LEFT JOIN {users_roles} ur on ur.uid = u.uid
        LEFT JOIN {role} r on r.rid = ur.rid
        WHERE u.uid = %d";
    $result =  db_query($sql, $user_id);
    if(db_affected_rows($result)>0)
        return db_fetch_object($result);
    else return false;
}


# Checking User For User Profile View Page
function is_user_exist($user_id) {
    $result = db_query("SELECT * FROM {ntlp_user} where uid = %d", $user_id);
    if((db_affected_rows($result)>0)) {
        return true;
    }
    else {
        return false;
    }

}


function _get_load_hosts() {
    return $hostoption = array(
            ''=>"SELECT",
            'aol'=>"AOL",
            'yahoo'=>'Yahoo',
            'msn'=>'MSN',
            'icq'=>'ICQ',
            'google'=>'Google',
            'japper'=>'Japper',
            'skype'=>'Skype',);

}


# Separating IM Name From Host Name From im_name Field
function checking_im_name($im_name) {
    $sid = explode('@',$im_name);
    $id = $sid[0];
    $im_host = $sid[1];
    $shost = explode('.',$im_host);
    $host = $shost[0];
    $arr['id'] = $id;
    $arr['host'] = $host;
    return $arr;
}

function _get_file_detail($fid) {
    $result = db_query("SELECT * FROM {files} WHERE fid = %d", $fid);
    if($result) {
        return db_fetch_object($result);
    }else {
        return false;
    }
}


function get_notify_user_settings($user_uid,$role_id=0) {

    return db_query("SELECT n.notification_id, n.text,
IF(nuc.notify_event_id IS NULL,n.default_web,setting_web ) n_w, n.enable_web,
IF(nuc.notify_event_id IS NULL,n.default_email,nuc.setting_email) n_e , n.enable_email FROM {ntlp_notification}  n
        LEFT JOIN {ntlp_notification_user_settings}  nuc ON n.notification_id = nuc.notify_event_id AND nuc.user_uid = {$user_uid} WHERE role_{$role_id}=1;");

}
function update_notification_email_exclusion_for_user($notification_id, $user_uid, $exclude_email) {
    db_query("INSERT INTO {ntlp_notification_user_settings} (user_uid, notify_event_id,setting_email,setting_web)
        VALUES ({$user_uid},{$notification_id},{$exclude_email},(SELECT default_web FROM dpl_ntlp_notification WHERE notification_id = {$notification_id}))
        ON DUPLICATE KEY UPDATE user_uid=VALUES(user_uid), notify_event_id=VALUES(notify_event_id),
            setting_email=VALUES(setting_email)");

}

function update_notification_web_exclusion_for_user($notification_id, $user_uid, $exclude_web) {

    db_query("INSERT INTO {ntlp_notification_user_settings} (user_uid, notify_event_id,setting_web, setting_email)
        VALUES ({$user_uid},{$notification_id},{$exclude_web},(SELECT default_email FROM dpl_ntlp_notification WHERE notification_id = {$notification_id}))
        ON DUPLICATE KEY UPDATE user_uid=VALUES(user_uid), notify_event_id=VALUES(notify_event_id),
            setting_web=VALUES(setting_web)");

}

function filter_notification_users($recipient_ids,$notification_id,$email=true){
//$recipient_ids: output of get_recipients()
//$notification_id: Event Triggered
//$email : true if getting list to recieve reciepients for emailing. false for Web notification
    $sql = "SELECT uid, name FROM (
        SELECT u.uid, u.name,
            IF(nuc.setting_email IS NULL, n.default_email,nuc.setting_email) n_e,
            IF(nuc.setting_web IS NULL, n.default_web,nuc.setting_web) n_w,
            nuc.setting_email, nuc.setting_web, nuc.notify_event_id, n.default_email,n.default_web
          FROM dpl_users  u
            LEFT JOIN dpl_ntlp_notification  n ON n.notification_id = $notification_id
            LEFT JOIN dpl_ntlp_notification_user_settings nuc ON
            nuc.user_uid = u.uid  AND nuc.notify_event_id = n.notification_id
        WHERE u.uid IN ($recipient_ids)
        HAVING ". ($email?'n_e':'n_w') ."  =1 ) user_list";

    $recipient_rs= db_query($sql);
    
    while (($stu_obj = db_fetch_object($recipient_rs))) {
        $recipients[$stu_obj->uid] = $stu_obj->name;
    }
    return $recipients;


}

function get_notify_user_settings_by_notification($user_uid, $role_id, $notification_id) {

    return db_query("SELECT n.notification_id, n.text, 
            IF(nuc.notify_event_id IS NULL,n.default_web,setting_web ) n_w, n.enable_web,
            IF(nuc.notify_event_id IS NULL,n.default_email,nuc.setting_email) n_e , n.enable_email 
            FROM {ntlp_notification}  n
        LEFT JOIN {ntlp_notification_user_settings}  nuc ON n.notification_id = nuc.notify_event_id 
            AND nuc.user_uid = {$user_uid} 
            WHERE role_{$role_id}=1 AND n.notification_id = {$notification_id};");

}
?>
