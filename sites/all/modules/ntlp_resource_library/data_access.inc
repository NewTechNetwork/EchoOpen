<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'constants');

function get_resource_lib_record($school_nid) {

    $rs = db_query("SELECT n.title, nr.body, c.*
            FROM {node} n
            INNER JOIN {node_revisions} nr ON nr.nid = n.nid
            INNER JOIN {ntlp_module} c ON n.nid = c.nid
            WHERE c.school_nid = %d", $school_nid);

    if ($rs->num_rows > 0) {
        return db_fetch_object($rs);
    } else {
        return false;
    }
}

function get_node_owner_data($uid) {
    $sql = "SELECT * FROM dpl_users us left join dpl_ntlp_user nus on us.uid=nus.uid WHERE us.uid =%d";
    $user_data = db_fetch_object(db_query($sql, $uid));
    return $user_data;
}

function get_resource_lib_module($module_nid) {




    $rs = db_query("SELECT n.title, nr.body, c.*
            FROM {node} n
            INNER JOIN {node_revisions} nr ON nr.nid = n.nid
            INNER JOIN {ntlp_module} c ON n.nid = c.nid
            WHERE c.nid = %d", $module_nid);

    if ($rs->num_rows > 0) {
        return db_fetch_object($rs);
    } else {
        return false;
    }
}

function _get_resource_library_keywords($limit = false) {
    $sql = ' SELECT td.tid, td.name, td.vid FROM {ntlp_library_resource} nlr
                INNER JOIN dpl_term_node tn ON tn.nid = nlr.nid
                INNER JOIN dpl_term_data td ON td.tid = tn.tid
                WHERE td.vid = (SELECT  vid FROM dpl_vocabulary WHERE name = \'NTLP Posts\')
             ' . ($limit ? 'LIMIT 0,' . (int) $limit : '');

    $rows = array();
    if ($result = db_query($sql)) {
        while ($row = db_fetch_object($result)) {
            $rows[] = $row;
        }
    }

    return $rows;
}

function render_library_resource_view($category, $resource_nid, $keywords, $resource_edit, $can_edit, $user_data) {
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/jquery.ui.all.css');
    include "ntlp_resource.tpl.inc";
    global $base_path, $user;
    $root = $base_path;

    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_resources') . '/ntlp_resources.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_resource_library') . '/ntlp_resource_library.js');

    $url = $_GET['q'];
    $args = explode('/', $url);

    drupal_add_js('
            $(function(){
                    $("#content_table").attr("style" , "table-layout:fixed;");
                    });
        ', inline);

    set_item_url_handler('Resource View');
    if (is_library_resource_exist($resource_nid)) {

        $school_id = $user_data->school_nid;

        $resource = _get_category_resrouce_detail($resource_nid);


        $content = $HTMLContentDocument;
        switch ($resource->node_type) {
            case "lo_document":
                $content = str_replace("@IMG_SRC", 'img_document_big.png', $content);

                if ($resource->resource_type == 'U') {
                    $file_path = explode("_", $resource->filepath);
                    $file_name = $file_path[sizeof($file_path) - 1];
                    $content = str_replace("@RESOURCE_VIEW", '<img style="margin-right:5px;" src="' . $root . get_resource_file_icon($file_name) . '"><a href="' . $root . $resource->filepath . '" target="_blank" >' . $file_name . '</a>', $content);
                    $content = str_replace("@RESOURCE_DISPLAY_VIEW", $resource->filepath ? 'block' : 'none', $content);
                } else {
                    if (!empty($resource->path) && $resource->path != '') {
                        $docname = empty($resource->attachment_name) ? 'Google Document' : $resource->attachment_name;
                        $content = str_replace("@RESOURCE_VIEW", '<a class="green_link" href="' . $resource->path . '" target="_blank" >' . $docname . '</a>', $content);
                    } else {
                        $content = str_replace("@RESOURCE_VIEW", 'FILE NOT FOUND', $content);
                    }
                }

                break;
            case "lo_link":
                $content = str_replace("@IMG_SRC", 'img_link_big.png', $content);
                $content = str_replace("@RESOURCE_VIEW", "<img src='" . $root . "themes/Boldr/Images/resourceStuff/img_link.png'>&nbsp;" . l($resource->title, $resource->path, array('attributes' => array('target' => '_blank', 'class' => 'green_link'))), $content);
                break;
            case "lo_image":
                $content = str_replace("@IMG_SRC", 'img_picture_big.png', $content);

                if ($resource->resource_type == 'U') {
                    if ($resource->fid > 0) {
                        $image = $root . $resource->filepath;
                    } else {
                        $image = $root . 'sites/default/files/avatars/FileNotFound.jpg';
                    }
                } elseif (!empty($resource->path)) {
                    $image = $resource->path;
                } else {
                    $image = $root . 'sites/default/files/avatars/FileNotFound.jpg"';
                }

                $image = '<p align=center><img  src="' . $image . '" style=" max-width:800px; width:auto !important; width:800px;  max-height:600px; height:auto !important; height:600px;"/></p>';
                $content = str_replace("@RESOURCE_VIEW", $image, $content);

                break;
            case "lo_page":
                $content = str_replace("@IMG_SRC", 'img_page_big.png', $content);
                $content = str_replace("@RESOURCE_VIEW", '<div style="word-wrap:break-word;">' . $resource->path . '</div>', $content);

                $result = get_resource_attachment($resource->nid);

                if ($result->num_rows > 0) {
                    $attachments = '<br>';
                    while ($rs_attachment = db_fetch_object($result)) {

                        $attachments .= '<img src="' . get_attachment_type_image($rs_attachment->filemime, $rs_attachment->filename) . '" />&nbsp;&nbsp;<a href="' . $base_path . $rs_attachment->filepath . '" class="greenLink1" style="font-family: tahoma;font-weight: normal;" target="_blank">' . $rs_attachment->filename . '</a><br>';
                    }
                    $content = str_replace("<!-- @RESOURCE_ATTACHMENTS -->", $attachments, $content);
                } else {
                    $content = str_replace("<!-- @RESOURCE_ATTACHMENTS -->", '', $content);
                }
                break;
            case "lo_video":

                $content = str_replace("@IMG_SRC", 'img_video_big.png', $content);

                if ($resource->resource_type == 'U') {
                    $VIDEO = '
                        <a href="' . $root . $resource->filepath . '" class="Boldrplayer" id="player_' . $resource->nid . '" style="display:block;width:640px;height:360px">
                        <img src="' . $root . 'themes/Boldr/Images/flashImg.jpg" alt="Video" style="width: 640px;height:360px;" /></a>';
                    flowplayer_add('#player_' . $resource->nid, array('clip' => array(
                            'autoPlay' => TRUE,
                        ),
                    ));
                } else if ($resource->resource_type == 'E') {

                    $resource_path = $resource->path;

                    $resource_path = str_replace("width=", " width=640px ", $resource_path);
                    $resource_path = str_replace("height=", " height=360px ", $resource_path);

                    $resource_path = str_replace("width:", " width:640px; ", $resource_path);
                    $resource_path = str_replace("height:", " height:360px; ", $resource_path);

                    $VIDEO = $resource_path;
                } else {
                    $content = str_replace("@RESOURCE_VIEW", '
                        <a href="' . $resource->path . '" class="Boldrplayer" id="player_' . $resource->nid . '" style="display:block;width:320px;height:240px">
                        <img src="' . $root . 'themes/Boldr/Images/flashImg.jpg" alt="Video" style="width: 320px;" /></a>'
                            , $content);
                    flowplayer_add('#player_' . $resource->nid, array('clip' => array(
                            'autoPlay' => TRUE,
                        ),
                    ));
                }

                $content = str_replace("@RESOURCE_VIEW", '<div style="margin:0 auto; align:center; width: 640px; height:360px; margin-top:15px;" align="center">' . $VIDEO . '</div>', $content);

                break;
            case "lo_share_idea":
                $content = str_replace("@IMG_SRC", 'img_idea_big.png', $content);
                $content = str_replace("@RESOURCE_VIEW", "", $content);
                break;
            default:
                $content = str_replace("@IMG_SRC", 'img_document_big.png', $content);
                $content = str_replace("@RESOURCE_VIEW", '<div>' . $resource->path . '</div>', $content);
                break;
        }

        $link_options['attributes']['class'] = 'green_link';
        $author = l($user_data->first_name . ' ' . $user_data->last_name, 'ntlp/user/profile/' . $user_data->uid, $link_options);

        $content = str_replace("@AUTHOR", $author . ' (' . get_school_name($school_id) . ')', $content);
        $content = str_replace("@RESOURCES_KEYWORDS", $keywords, $content);
        $content = str_replace("@RESOURCES_SUBMITTED", get_tz($school_id, NTLP_DATE_TZ_FJY, date('Y-m-d H:i:s', $resource->created)), $content);

        $content = str_replace("@RESOURCE_TITLE", $resource->title, $content);

        $content = str_replace("@ROOT", $root, $content);

        $content = str_replace("@ADDLIKING", flag_create_link('likethispost', $resource->nid), $content);

        $likethispost = flag_get_counts('node', $resource->nid);
        $content = str_replace("@LIKES", (isset($likethispost['likethispost']) ? $likethispost['likethispost'] : '0'), $content);

        $views_count = get_views_count($resource_nid);
        $content = str_replace("@VIEWS", $views_count, $content);

        $resource_description = '<div class="library_project_despcription"><strong>Description</strong>
            <p><div style="width:100%; position:relative;">
                <div style="width:auto;">
                    <div style="word-wrap:break-word;">
                            <div>' . $resource->body . '</div>
                    </div>
                </div>
            </div>
            </p>
         </div>';

        if (!empty($resource->body)) {
            if ($resource->type == "lo_share_idea") {
                $content = str_replace("@RESOURCE_DESCRIPTION", $resource_description, $content);
            } elseif ($resource->teaser != "") {
                $content = str_replace("<!-- @RESOURCE_MESSAGE -->", $resource->teaser, $content);
                $content = str_replace("@RESOURCE_DESCRIPTION", $resource_description, $content);
            } else {
                $content = str_replace("@RESOURCE_DESCRIPTION", $resource_description, $content);
            }
        } else {
            $content = str_replace("@RESOURCE_DESCRIPTION", '', $content);
        }

        $header_content = $HTMLHeader;

        //Display edit button based on permissions
        if ($can_edit) {

            $resouce_manage_options = $HTMLresource_manage_options;

            $resouce_manage_options = str_replace('@MANAGE_RESOURCE_EDIT', l('Edit Resource', $resource_edit), $resouce_manage_options);
            $resouce_manage_options = str_replace('@MANAGE_RESOURCE_DELETE', dlg('Delete Resource', 'ntlp/document/library/resource/delete/' . $resource->nid, 500, 500), $resouce_manage_options);

            $header_content = str_replace("@EDIT_RESOURCE", $resouce_manage_options, $header_content);
        } else {
            $header_content = str_replace("@EDIT_RESOURCE", "", $header_content);
        }
        $header_content = str_replace("@RESOURCE_NID", $resource->nid, $header_content);

        if ($category) {
            $header_content = str_replace("@BACK_LINK", '<ul><li>' . str_replace('%2C', ',', $category) . ' &gt;&nbsp;</li><li style="color:#000; font-weight:bold;"> Resource</li></ul>', $header_content);
        } else {
            $back_url = get_back_url_value();
            $header_content = str_replace("@BACK_LINK", '<a href="' . url('ntlp/goback') . '">« Back to ' . $back_url->page_name . '</a>', $header_content);
        }
        $mycontent = $header_content . $content . $HTMLFooter;

        return $mycontent;
    } else {
        $message = "This resource is no longer available";
        drupal_set_message($message, 'error');
        drupal_goto('ntlp/goback');
    }
}

function is_library_resource_exist($resource_nid) {
    $flag = false;

    $result = db_query("SELECT nid FROM {ntlp_library_resource} WHERE deleted_on is NULL AND nid = %d", $resource_nid);

    if ($result->num_rows > 0) {
        $flag = true;
    } else {
        $flag = false;
    }

    return $flag;
}

function is_resource_owner($node_id, $user) {

    $flag = false;

    $node_Obj = node_load($node_id);

    if ($user->uid == $node_Obj->uid) {
        $flag = true;
    } else {
        $flag = false;
    }

    return $flag;
}

function get_views_count($resource_nid, $increment_count = true) {

    $result = db_query("SELECT views_count from {ntlp_library_resource_stats} WHERE nid = %d", $resource_nid);

    $views_count = 0;

    if ($row = db_fetch_object($result)) {
        $views_count = $row->views_count;
    }

    if ($increment_count) {
        if ($views_count++ > 0)
            db_query("UPDATE {ntlp_library_resource_stats} SET views_count=%d WHERE nid=%d", $views_count, $resource_nid);
        else
            db_query("INSERT INTO {ntlp_library_resource_stats} (nid, views_count) VALUES (%d, %d)", $resource_nid, $views_count);
    }

    return $views_count;
}