<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_school', 'data_access');

function render_single_resource_view($resource_nid) {
    include "ntlp_resource.tpl.inc";
    global $base_path, $user;
    $root = $base_path;

    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_resources') . '/ntlp_resources.js');
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/jquery.ui.all.css');

    $url = $_GET['q'];
    $args = explode('/', $url);

    drupal_add_js('
            $(function(){
                    $("#content_table").attr("style" , "table-layout:fixed;");
                    });
        ', inline);


    for ($i = 0; $i < count($args); $i++) {
        if (is_numeric($args[$i]) && empty($courseid))
            $courseid = $args[$i];
        elseif (is_numeric($args[$i]) && empty($studentid))
            $project_id = $args[$i];
    }
    $pproject_id = _get_project_id_by_resource($resource_nid);


    set_item_url_handler('Resource View');

    if (!is_resource_exist($resource_nid)) {
        //Load Node
        $node = node_load($resource_nid);

        //Load current user
        if (isset($user->roles[NTLP_ROLEID_PARENT])) {  // if parent come here show their student record
            $user_obj = user_load($_SESSION['PARENT_STUDENT_UID']);
        } else {
            $user_obj = $user;
        }
        $user_role = check_resource_permission($node, $user_obj);

//        watchdog('res_role', "User:" . print_r($user_obj, true) . "<BR>Role:$user_role");

        if (isset($user_role)) {

            $resource = get_resource_detail($node->nid);
            $user_data = get_user_data($resource->uid);
            $content = $HTMLContentDocument;
            switch ($resource->type) {
                case "lo_document":
                    $content = str_replace("@IMG_SRC", 'img_document_big.png', $content);
                    if ($resource->resource_type == 'U') {
                        if (empty($resource->attachment_name)) {
                            $file_path = explode("_", $resource->filename);
                            unset($file_path[0]);
                            unset($file_path[1]);
                            $resource->attachment_name = implode($file_path, '_');
                        } else {
                            $resource->attachment_name = $resource->attachment_name;
                        }
                        $resource->path = $resource->filepath;
                    } else {
                        if (!empty($resource->path) && $resource->path != '') {
                            $resource->attachment_name = empty($resource->attachment_name) ? 'Google Document' : $resource->attachment_name;
                        } else {
                            $resource->path = "#";
                            $resource->attachment_name = $resource->attachment_name . "&nbsp;<Broken Google Document link>";
                        }
                    }
                    $content = str_replace("@RESOURCE_VIEW", '', $content);
                    $resource_icon =  '<div style="float:left; width: 16px;" class="'._get_resource_type_icons($resource->type,  $resource->filepath, $resource->resource_type ).'"></div> &nbsp;';
                    $resource_link = '<a class="green_link" href="' . $resource->path . '" target="_blank" >'.$resource->attachment_name.'</a>';
                    $content = str_replace('@RESOURCES_ATTACHMENT', $resource_icon . $resource_link , $content);
                    $content = str_replace("@RESOURCE_DISPLAY_VIEW", $resource->path ? 'block' : 'none', $content);
                    break;
                case "lo_link":
                    $content = str_replace("@IMG_SRC", 'img_link_big.png', $content);
                    $content = str_replace("@RESOURCE_VIEW", "<img src='" . $root . "themes/Boldr/Images/resourceStuff/img_link.png'>&nbsp;" . l($resource->title, $resource->path, array('attributes' => array('target' => '_blank', 'class' => 'green_link'))), $content);
                    break;
                case "lo_image":
                    if (empty($resource->attachment_name)) {
                        $resource->attachment_name = $resource->filename;
                    }
                    $content = str_replace("@IMG_SRC", 'img_picture_big.png', $content);

                    if ($resource->resource_type == 'U') {
                        if ($resource->fid > 0) {
                            $image = $root . $resource->filepath;
                        } else {
                            $image = $root . 'sites/default/files/avatars/FileNotFound.jpg';
                        }
                    }

                    $image = '<p align=center><img  src="' . $image . '" style=" max-width:800px; width:auto !important; width:800px;  max-height:600px; height:auto !important; height:600px;"/></p>';
                    $content = str_replace("@RESOURCE_VIEW", '', $content);
                    $content = str_replace("@ATTACHMENT_TYPE", '', $content);
                    $content = str_replace("@RESOURCES_ATTACHMENT", $image, $content);
                    break;
                case "lo_page":
                    $content = str_replace("@IMG_SRC", 'img_page_big.png', $content);
                    $content = str_replace("@RESOURCE_VIEW", '<div style="word-wrap:break-word;">' . $resource->path . '</div>', $content);
                    //$content = str_replace("@RESOURCE_DESC", '<div>'.$resource->body.'</div>', $content);

                    $result = get_resource_attachment($resource->nid);
                    
                    if ($result->num_rows > 0) {
                        //$attachments = '<br>';
                        while ($rs_attachment = db_fetch_object($result)) {
                            if ($rs_attachment->type == 'F') {
                                if (empty($rs_attachment->name)) {
                                    $file_path = explode("_", $rs_attachment->filename);
                                    if (count($file_path) > 2) {
                                        unset($file_path[0]);
                                        unset($file_path[1]);
                                    }
                                    $rs_attachment->name = implode($file_path, '_');
                                }
                                $resource_icon_and_link .=  '<div><div style="float:left; width: 16px;" class="'._get_resource_type_icons($resource->type,  $rs_attachment->filename, $rs_attachment->type ).'"></div> &nbsp; <a class="green_link" href="' . $rs_attachment->filepath . '" target="_blank" >'.$rs_attachment->name.'</a></div><br />';
                            } else {
                                if (empty($rs_attachment->name)) {
                                    $rs_attachment->name = 'Google Document';
                                }
                                $resource_icon_and_link .=  '<div><div style="float:left; width: 16px;" class="'._get_resource_type_icons($resource->type,  $rs_attachment->name, $rs_attachment->type ).'"></div> &nbsp; <a class="green_link" href="' . $rs_attachment->path . '" target="_blank" >'.$rs_attachment->name.'</a></div><br />';
                            }
                        }
                        
                        $content = str_replace("@RESOURCES_ATTACHMENT",$resource_icon_and_link, $content);
                        //$content = str_replace("<!-- @RESOURCE_ATTACHMENTS -->", $attachments, $content);
                    } else {
                        $content = str_replace("@RESOURCES_ATTACHMENT", '', $content);
                    }
                    break;
                case "lo_video":
                    if (empty($resource->attachment_name)) {
                        $resource->attachment_name = $resource->filename;
                    }
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
                    }
                    $content = str_replace("@ATTACHMENT_TYPE", '', $content);
                    $content = str_replace("@RESOURCES_ATTACHMENT", '', $content);
                    $content = str_replace("@RESOURCE_VIEW", '<div style="margin:0 auto; align:center; width: 640px; height:360px; margin-top:15px;" align="center">' . $VIDEO . '</div>', $content);
                    break;
                //---

                case "lo_share_idea":
                    $content = str_replace("@IMG_SRC", 'img_idea_big.png', $content);
                    $content = str_replace("@RESOURCE_VIEW", "", $content);
                    break;
                default:
                    $content = str_replace("@IMG_SRC", 'img_document_big.png', $content);
                    $content = $HTMLContentPage;
                    $content = str_replace("@RESOURCE_VIEW", '<div>' . $resource->path . '</div>', $content);
                    break;
            }

            $content = str_replace("@POSTED_BY_IMAGE", '<img src="' . is_user_image_exist($user_data->picture) . '"; height="32" width="32" />', $content);

            if (isset($user->roles[NTLP_ROLEID_PARENT])) {
                $content = str_replace("@POSTED_BY_NAME", $user_data->first_name . ' ' . $user_data->last_name, $content);
            } else {
                $content = str_replace("@POSTED_BY_NAME", l($user_data->first_name . ' ' . $user_data->last_name, 'ntlp/user/profile/' . $user_data->uid, array('attributes' => array('style' => 'color: #009900;'))), $content);
            }
            $content = str_replace("@RESOURCE_TITLE", $resource->title, $content);

            //Check if user has permission to delete the resource
            //Role maybe set for student or parent's kid, but they are not allowed to delete
            //On delete button, keep the user on the same page
            if ((isset($user_role) && ($user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_SCHOOLLEADER || $user_role == NTLP_ROLEID_SITEADMIN ||
                    $user_role == NTLP_ROLEID_NETWORKMANAGER || $user_role == NTLP_ROLEID_NETWORKSTAFF)) || is_node_owner($resource->nid, $user)) {
                $content = str_replace("@DELETE", '<a onclick="onclick_remove_resource(' . $resource->course_nid . ',' . $resource->nid . ', true)" > </a>', $content);
            } else {
                $content = str_replace("@DELETE", "", $content);
            }
            $content = str_replace("@DELETE", "<a id='comment_delete_$comment->cid' onclick='nltp_node_delete($comment->cid, $comment->nid)'>DELETE</a>", $content);

            $likethispost = flag_get_counts('node', $resource->nid);
            $like_info = "";
            if ($likethispost['likethispost'] > 1) {
                $like_info = '<div style="float:right;margin-top:-10px;font-weight:bold"> <img src="' . $base_path . 'themes/Boldr/Images/common/img_like.png" /> ' . $likethispost['likethispost'] . ' people like this </div>';
            } else if ($likethispost['likethispost'] == 1) {
                $like_info = '<div style="float:right;margin-top:-10px;font-weight:bold"> <img src="' . $base_path . 'themes/Boldr/Images/common/img_like.png" /> ' . $likethispost['likethispost'] . ' person likes this </div>';
            }

            $content = str_replace("<!-- @LIKE_THIS_RESOURCE -->", $like_info, $content);

            if (isset($user->roles[NTLP_ROLEID_PARENT])) {

                $content = str_replace("@ADDLIKING", '', $content);
                $content = str_replace("@ADDNEWCOMMENT", '', $content);
            } else {
                $content = str_replace("@ADDLIKING", '<span>' . flag_create_link('likethispost', $resource->nid) . '</span>', $content);
                $content = str_replace("@ADDNEWCOMMENT", "<a onclick='focus_func($resource->nid)' class='ntlp_tooltip_n' title='Add Comment'> <img src='" . $root . "themes/Boldr/Images/AddComment.jpg' alt='Comment' /></a>", $content);
            }

            $content = str_replace("@DATEDIFF", ago($resource->changed), $content);
            $content = str_replace("@ROOT", $root, $content);

            $resource_description = '<div class="library_project_despcription"><strong>Description</strong>
                <p><div style="width:100%; position:relative;">
                    <div style="width:auto;">
                        <div style="word-wrap:break-word;">
                                <div>' .parse_text_to_rtf( $resource->body ). '</div>
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

            $nodeComments = view_comments($node);
            $nodeComments = "<div id=\"comments-wrapper-$resource->nid\">$nodeComments</div>";
            $content = str_replace("@COMMENTS", $nodeComments, $content);
            $commentsadd = "<textarea style='display:none' id='txt_$resource->nid' rows='2' cols='70'></textarea>";
            $commentsadd .= "<a style='display:none' id='comment_new_$resource->nid' class='CommentBtn' onclick='new_post_comments($resource->nid)'>Comment</a>";
            $content = str_replace("@ADDCOMMENTSFORM", $commentsadd, $content);

            $resource_back_link = "<a href='?q=ntlp/courses/projects/$courseid/$pproject_id'>" . _get_node_title($pproject_id) . "</a>";
            $resource_visible = ($user_role != NTLP_ROLEID_STUDENT && !isset($user->roles[NTLP_ROLEID_PARENT])) ? '<strong> Visibility: </strong>' . ($resource->availability == 'H' ? 'Hide' : 'Show') : "";


            $header_content = $HTMLHeader;

            //Display edit button based on permissions
            if ((isset($user_role) && ($user_role == NTLP_ROLEID_TEACHER || $user_role == NTLP_ROLEID_NETWORKMANAGER || $user_role == NTLP_ROLEID_NETWORKSTAFF
                    || $user_role == NTLP_ROLEID_TEACHER)) || is_node_owner($resource->nid, $user)) {

                $resouce_manage_options = $HTMLresource_manage_options;

                $resouce_manage_options = str_replace('@MANAGE_RESOURCE_EDIT', l('Edit Resource', 'ntlp/courses/resource/edit/' . $resource->course_nid . '/' . $resource->nid), $resouce_manage_options);
                $resouce_manage_options = str_replace('@MANAGE_RESOURCE_COPY', dlg('Copy Resource', 'ntlp/resources/copy/'. $resource->course_nid . '/' . $resource->nid, 150, 350), $resouce_manage_options);
                $resouce_manage_options = str_replace('@MANAGE_RESOURCE_DELETE', dlg('Delete Resource', 'ntlp/node/delete/' . $resource->course_nid . '/' . $resource->nid, 500, 500), $resouce_manage_options);

                $header_content = str_replace("@EDIT", $resouce_manage_options, $header_content);
            } else {
                $header_content = str_replace("@EDIT", "", $header_content);
            }
            $header_content = str_replace("@BACK_LINK", $resource_back_link, $header_content);
            $header_content = str_replace("@VISIBILITY", $resource_visible, $header_content);

            $header_content = str_replace("@RESOURCE_NID", $resource->nid, $header_content);
            $mycontent = $header_content . $content . $HTMLFooter;
            return $mycontent;
        } else {
            $message = "You are not authorized to view this resource.";
            drupal_set_message($message, 'error');
            drupal_goto($last_state_url);
        }
    } else {
        $message = "This resource is no longer available";
        drupal_set_message($message, 'error');
        drupal_goto($last_state_url);
    }
}

function resource_view_form($form_state, $course_nid, $resource_nid) {

    drupal_add_js(drupal_get_path('module', 'ntlp_resources') . '/ntlp_resources.js');

    $mycontent = render_single_resource_view($resource_nid);
    $form = array();
    $form['resource_node'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
    );

    $form['resource_node']['content'] = array(
        '#type' => 'item',
        '#value' => $mycontent,
        '#prefix' => '<div id="node-' . $resource_nid . '">',
        '#suffix' => '</div>',
    );

    return $form;
}

function is_resource_exist($resource_nid) {
    $flag = false;

    $result = db_query("SELECT nid FROM {ntlp_course_resource} WHERE deleted_on is NULL AND nid = %d", $resource_nid);

    if ($result->num_rows > 0) {
        $flag = false;
    } else {
        $flag = true;
    }

    return $flag;
}

function get_attachment_type_image($file_mime_type, $filename) {
    global $base_path;

    $mime_type = explode('/', $file_mime_type);

    if ($mime_type[0] == 'application' || $mime_type[0] == 'text') {
        $image_path = $base_path . get_resource_file_icon($filename);

    } else if ($mime_type[0] == 'image') {
        $image_path = $base_path . 'themes/Boldr/Images/resourceStuff/img_picture.png';

    } else if ($mime_type[0] == 'video') {
        $image_path = $base_path . 'themes/Boldr/Images/resourceStuff/img_video.png';
    }

    return $image_path;
}

?>
