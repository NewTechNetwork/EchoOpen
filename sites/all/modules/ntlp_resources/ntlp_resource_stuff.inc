<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


//include "attachment_handler.inc";
module_load_include('inc', 'ntlp_school', 'fileupload');
module_load_include('inc', 'ntlp_school', 'constants');
module_load_include('inc', 'ntlp_school', 'data_access');

function ntlp_resources_share_stuff($form_state, $is_allstuff = false) {
    include("resources.template.inc");

    global $base_path, $user;
    $path = $base_path;

    if ($course_id == 0) {
        //Getting Query String From URL
        $course_id = 0;
        $url = $_GET['q'];
        $args = explode('/', $url);
        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i]) && empty($course_id))
                $course_id = $args[$i];
        }
    }

    
    modalframe_parent_js();
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_resources') . '/ntlp_resources.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_resources') . '/resources_ahah_helper.js', 'module', 'footer');
    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/js/jquery.tipsy.js');
    drupal_add_js('
     function share_tooltip(){
	$(".share_tooltip").tipsy({opacity: 1, title: "newTitle"});
     }','inline');

    drupal_add_js('
    function clearText(field){
    if (field.defaultValue == field.value){
        field.value = "";
        $("#"+field.id).css("color","black");
    } else if (field.value == "") {
        field.value = field.defaultValue;
         $("#"+field.id).css("color","#999999");
        }
}','inline');



    $node = node_load($course_id);

    // if parent try to access this wall... just redirect to the course agenda page
    $current_date = get_tz_course($course_id, 'Y/m/d', date('Y-m-d H:i:s', time()));
    $user_role = check_user_permission($course_id);
    if (isset($user->roles[NTLP_ROLEID_PARENT]) || $user_role == NTLP_ROLEID_SITEGUEST) {

        if ($node->type == 'ntlp_course') {

            $url = 'ntlp/courses/agenda/' . $course_id . '/' . $current_date;
            drupal_goto($url);
        }
    }

    if ($node->type == 'ntlp_course') {
        set_item_url_handler('Course Home');
    } else if ($node->type == 'ntlp_group') {
        set_item_url_handler('Group Home');
    }
    if (isset($form_state['values'])) {
        $view_state = $form_state['values']['op'];
    }
    if (isset($form_state['values']['add_resources_stuff']['course_id'])) {
        $course_id = $form_state['values']['add_resources_stuff']['course_id'];
    }

    $default_value = $form_state['values']['add_resources_stuff']['selected_type'];

    if (isset($view_state)) {

    } else {

        $header = $HTMLform_resource_groupbox_hdr;
        $footer = $HTMLform_resource_groupbox_ftr;
        
        if (!$is_allstuff) {

            if ($node->type == 'ntlp_course')
                $all_stuff_link = "ntlp/courses/resources/allstuff/$course_id";
            else if ($node->type == 'ntlp_group')
                $all_stuff_link = "ntlp/groups/resources/allstuff/$course_id";

            $header = str_replace("@ALL_STUFF_LINK", '
                <div align="right" style="width: 180px; margin-top: -20px; float: right;">
                    <a class="stuff-link" href="'. url($all_stuff_link) . '"> All Stuff</a>
                    <span style="margin-right: 10px;" class="stuff-link"> &gt; </span>
                </div>', $header);

            $header = str_replace("@ALL_OR_SHARE", '<h2>Share Stuff</h2>', $header);
        }
        else {
            $header = str_replace("@ALL_STUFF_LINK", '', $header);

            $header = str_replace("@ALL_OR_SHARE", '<h2>All Stuff</h2>', $header);
        }

        $form['add_resources_stuff'] = array(
            '#tree' => TRUE, // Don't forget to set #tree!
            '#prefix' => $header,
            '#suffix' => $footer,
        );

        if (!$is_allstuff) {
            
            $form['add_resources_stuff']['share_ideas'] = array(
                '#type' => 'textarea',
                '#wysiwyg' => false,
                '#resizable' => false,
                '#default_value' => 'Share your thoughts with the class',
                '#prefix' => '<tr><td align="left" style="padding-left:7px;" colspan="3">',
                '#suffix' => '</td></tr><tr><td colspan="3" style="padding-top:7px">',
                '#attributes' => array('style' => 'color: #999999;font-family:Tahoma, Arial, Helvetica, sans-serif; padding: 5px; padding-right: 0px; font-size: 12px; resize:none; ', 'class' => 'shareIdea', 'title' => 'Share your thoughts with the class',
                        'onblur' => "clearText(this)", 'onfocus' => "clearText(this)"),
            );

            //Resource types icons
            $form['add_resources_stuff']['add_links'] = array(
                '#type' => 'item',
                '#value' => '<a  class="share_tooltip" onmouseover="share_tooltip();" newTitle="Add Link" onclick = "javascript:link_field_hide();">Links</a>',
                '#prefix' => '<div style="float:left"><table width="113px" align="center" id="links" style="margin-left:10px; margin-bottom:10px;"><tr><td style="width:150px">',
                '#suffix' => '</td>',
            );


            $form['add_resources_stuff']['add_images'] = array(
                '#type' => 'item',
                '#value' => '<a class="share_tooltip" onmouseover="share_tooltip();" onclick = "javascript:image_field_hide(); return false" newTitle="Add Image">Pictures</a>',
                '#prefix' => '<td style="width:150px">',
                '#suffix' => '</td>',
            );

            $form['add_resources_stuff']['add_videos'] = array(
                '#type' => 'item',
                '#value' => '<a class="share_tooltip" onmouseover="share_tooltip();" onclick = "javascript:video_field_hide(); return false"  newTitle="Add Media">Videos</a>',
                '#prefix' => '<td style="width:150px">',
                '#suffix' => '</td>',
            );

            $form['add_resources_stuff']['add_documents'] = array(
                '#type' => 'item',
                '#value' => '<a class="share_tooltip" onmouseover="share_tooltip();" newTitle="Add Document"  onclick = "javascript:document_field_hide(); return false">Documents</a>',
                '#prefix' => '<td>',
                '#suffix' => '</td></tr></table></div>',
            );
            //-------------------------------------

            $form['add_resources_stuff'] ['title'] = array(
                '#type' => 'item',
                '#prefix' => '
                    <div id="dialog-show-confirmation" style="display: none;" title="Message:">
                        <span class="msg">Please select resource</span>
                    </div>
                        <table border="0" cellpadding="5" id="table_hide" align="left"  width="594px" style=" float:left; border:1px solid #3491ca;display:none;margin-bottom:10px;margin-left:10px"><tr id="title_resource" style="display:none;"><td colspan="3" style="color:#ffffff;font-size:16px; font-weight:bold;border-right:1px solid #3491ca;"><table style="float:left"><tr><td style="padding-right:5px" id="path"></td><td id="title">',
                '#suffix' => '</td></tr></table>',
            );

            $form['add_resources_stuff'] ['close_button'] = array(
                '#type' => 'item',
                '#prefix' => '<div align="right" id="close_button" style="display:none;float:right"><a  onclick = "javascript:close_button(); return false"><img src="' . $path . 'themes/Boldr/Images/DeleteBtn.png" alt="AttachedLink" /></a>',
                '#suffix' => '</div></td></tr>',
            );

            //Add link controls
            $form['add_resources_stuff']['website_title'] = array(
                '#type' => 'textfield',
                '#size' => '60',
                '#prefix' => '<tr id="resourcesname" style="display:none"> <td style="color:#ffffff;width:113px">' . t('Website Title') . '</td><td colspan="2" style="border-right:1px solid #3491ca;">',
                '#suffix' => '</td></tr>',
            );

            $form['add_resources_stuff']['link_url'] = array(
                '#type' => 'textfield',
                '#size' => '60',
                '#prefix' => '<tr id="linkurl" style="display:none"><td style="width:113px;color:#ffffff;">' . t('Link (URL)') . '</td><td  colspan="2" style="border-right:1px solid #3491ca;">',
                '#suffix' => '</td></tr>',
            );

            $form['add_resources_stuff']['video_upload_radio'] = array(
                '#type' => 'radios',
                '#options' => array('embd' => 'Embed', 'upld' => 'Upload'),
                '#default_value' => 'embd',
                '#prefix' => '<tr id="video_radio" style="display:none"><td colspan="2" style="border-right:1px solid #3491ca;">',
                '#suffix' => '</td></tr>',
                '#attributes' => array('onclick' => 'show_video(this)', 'class' => 'addmedia', 'style' => 'color:#ffffff !important;'),
            );

            $form['add_resources_stuff']['video_embedded_code'] = array(
                '#default_value' => 'Copy and paste <embed> or <iframe> code from other websites',
                '#type' => 'textarea',
                '#wysiwyg' => false,
                '#rows' => 2,
                '#resizable' => false,
                '#prefix' => '<tr id="video_embd" style="display:none"><td colspan="3">',
                '#suffix' => '</td></tr>',
                '#attributes' => array('style' => 'color: #999999;width:575px;font-family:Tahoma, Helvetica, Arial, sans-serif; padding: 8px; padding-right: 0px; font-size: 12px;', 'class' => 'shareIdea onClickTxtClr', 'title' => 'Copy and paste video <embed> code from other websites'
                    , 'onblur' => "clearText(this)", 'onfocus' => "clearText(this)"),
            );

            //Add Document controls
            $form['add_resources_stuff']['google_radio'] = array(
                '#type' => 'radios',
                '#options' => array('upld' => 'Upload', 'gd' => 'Google Doc'),
                '#prefix' => '<tr id="google" style="display:none"><td colspan="3" style="padding:0 0 0 4px;  border-right:1px solid #3491ca;">',
                '#suffix' => '</td></tr>',
                '#attributes' => array('onclick' => 'show_google(this)', 'class' => 'addmedia'),
                '#default_value' => 'upld',
            );

            $form['add_resources_stuff']['google_doc_link'] = array(
                '#type' => 'item',
                '#prefix' => '<tr id="google_doc_attached">
                                <td class="lightgreen_link" colspan="3" style=" border-right:1px solid #3491ca;">
                                    <span style="color: rgb(255, 255, 255);">+&nbsp;</span><span> '. dlg(" Link to Google Doc", 'ntlp/course/resources/attachment/google', 300, 600, 'lightgreen_link') . '</span>
                                    <br />
                                    <div style="color:#fff; margin-top: 13px;">This allows everyone in the class to view the document </div>
                                </td>
                              </tr>',
            );

            //Add upload control
            $form['#fileupload_embed'] = true;
            $form['#fileupload_auto_upload'] = true;

            $form['add_resources_stuff']['upload_file_link'] = array(
                '#type' => 'item',
                '#prefix' => '<tr style="" id="uploadlink">
                                <td class="lightgreen_link" colspan="3" onmouseover="place_fileupload_control(\'edit-add-resources-stuff-upload-file-link\', this);" onmouseout="place_fileupload_control_mouseout(this);" style="border-right:1px solid #3491CA; width: 120px; color: rgb(255, 255, 255);">
                                <div id="attach_links" style="float:left;padding-left:5px;"><span id="attach_link_prompt"><span style="color: rgb(255, 255, 255);">+&nbsp;</span><b><a class="lightgreen_link" id="attach_link">Upload your Image</a></b></span></div>',
                '#suffix' => '</td></tr><tr>
                                <td colspan="3" style="border-right:1px solid #3491CA;"><span id="attach_link_suffix" style="color: rgb(255, 255, 255);">Max file size = 5 MB | File type = JPG, PNG, GIF</span></td>
                              </tr>',
            );

            fileupload_get_control($form, $form_state, array('add_resources_stuff', 'upload_file_link'));


            $form['project_setting']['attached_files_hidden'] = array(
                '#type' => 'hidden',
                '#id' => 'attached_files_hidden',
            );

            $form['add_resources_stuff']['google_urls'] = array(
                '#type' => 'hidden',
                '#prefix' => '<tr id="google_docs_row" style="display:none"><td class="lightgreen_link" colspan="3">
                    <div id="google_urllist" style="height:20px;"></div>',
            );

            $form['add_resources_stuff']['google_hAttachedFiles'] = array(
                '#type' => 'hidden',
                '#value' => '',
                '#suffix' => '</td></tr>',
            );

            $form['add_resources_stuff']['course_id'] = array(
                '#type' => 'hidden',
                '#value' => $course_id,
                '#prefix' => '<tr style="display:none"><td style="border-right:1px solid #3491ca;" colspan="3">',
                '#suffix' => '</td></tr>',
            );

            $form['add_resources_stuff']['selected_type'] = array(
                '#type' => 'hidden',
                '#value' => 'lo_share_idea',
                '#id' => 'type',
                '#prefix' => '<tr style="display:none"><td style="border-right:1px solid #3491ca;" colspan="3">',
                '#suffix' => '</td></tr>',
            );

            $form['add_resources_stuff']['save_button'] = array(
                '#type' => 'submit',
                '#submit' => array('ntlp_resources_stuff_submission'),
                '#value' => t('Post'),
                '#prefix' => '</td></tr></table><div style=" padding-right:13px; float: right; width: 80px;">',
                '#attributes' => array('class' => 'PostBtn', 'onclick' => "javascript:return post_shared_resource();",  'onmousedown' => "this.className='PostBtnClicked'", 'onmouseup' => "this.className='PostBtn'"),
                '#suffix' => '</div><br clear="all" />',
            );

        }
        else {
            $form['add_resources_stuff']['dummy'] = array(
            '#type' => 'item',
            '#suffix' => '<tr><td colspan="3" style="padding-top:7px">',
            );
        }

        //--- Check for Selected Types
        $typesSort = array(
                'created' => 'created',
                'node_liked' => 'node_liked',
                'comment_count' => 'comment_count',
        );
        $types = array_keys($typesSort);

        $selected_types = array();
        if( isset( $_REQUEST['sort'] ) ) {
            foreach( $_REQUEST['sort'] as $type ) {
                if(in_array( $type, $types )) {
                    $sort_by = $typesSort[$type];
                }
            }
        }

        //--- Check for Selected Types
        $typesTotal = array(
                'links' => 'lo_link',
                'images' => 'lo_image',
                'documents' => 'lo_document',
                'videos' => 'lo_video',
        );
        $types = array_keys($typesTotal);

        $selected_types = array();
        if( isset( $_REQUEST['post_type'] ) ) {
            foreach( $_REQUEST['post_type'] as $type ) {
                if(in_array( $type, $types )) {
                    $selected_types[] = $typesTotal[$type];
                }
            }
        }

        $view = views_get_view('og_ghp_ntlp_feed');

        $view->get_total_rows = TRUE;
        
        $view_args[] = $course_id;
        if (isset($_REQUEST['post_type']) and in_array('1', $_REQUEST['post_type']))
            $view_args[] =  'all';
        else {

            //If type filter is selected then we will get value here, and then we can filter node types
            if($selected_types) {
                $filter_type = array (
                    'type' => array (
                        'operator' => 'in',
                        'value' => $selected_types,
                        'table' => 'node',
                        'field' => 'type',
                    ),
                );
                $view->display['default']->display_options['filters'] = array_merge($view->display['default']->display_options['filters'], $filter_type);
            }
        }

        if( !isset( $_REQUEST['sort'] ) ) {
            $sort_by = 'created';
        }
        $sorts = array (
                $sort_by => array (
                    'order' => 'DESC',
                    'field' => $sort_by,
                ),
            );

        $view->display['default']->display_options['sorts'] = array_intersect_key($view->display['default']->display_options['sorts'], $sorts);
        watchdog('view', print_r($view->display['default']->display_options['sorts'], true));

        $view->pager['use_pager'] = ($is_allstuff) ? 1 : 0;

        $form['add_resources_stuff']['feeds'] = array(
            '#type' => 'item',
            '#value' => $view->preview('default', $view_args),
            '#prefix' => '<div id="share_stuff_wall">',
            '#suffix' => '</div></td></tr>',
        );

        $filter_str = "'".implode("','",$view->display['default']->display_options['filters']['type']['value']) . "'";

        $search_query = (isset($_REQUEST['keys']) && $_REQUEST['keys'] != '');
        
        $view_ppl_query = "SELECT COUNT(node.uid) AS ppl_count
            FROM dpl_node node LEFT JOIN dpl_ntlp_course_resource ntlp_course_resource ON node.nid = ntlp_course_resource.nid
            " . (($search_query) ?  "
            LEFT JOIN dpl_search_index search_index ON node.nid = search_index.sid
            LEFT JOIN dpl_search_total search_total ON search_index.word = search_total.word " : "") . "
            WHERE (node.type IN (".$filter_str."))
            AND (ntlp_course_resource.is_shared <> 0)
            AND (ntlp_course_resource.deleted_on IS NULL) " . 
            (($search_query) ? " AND (search_index.word = '". $_REQUEST['keys'] . "') AND (search_index.type = 'node') " : "") . "
            AND (ntlp_course_resource.course_nid = $course_id)
            GROUP BY node.uid;
            ";

        $ppl_count = 0;
        $rs = db_query($view_ppl_query);
        while (db_fetch_array($rs))
            $ppl_count++;
        
        $result_text = ($view->total_rows > 0) ? "Viewing <b>" . $view->total_rows . "</b> post" : "";
        $result_text .= ($view->total_rows > 1) ? "s" : "";
        $result_text .= ($view->total_rows > 0 && isset($ppl_count)) ? " by <b>$ppl_count</b>". (($ppl_count>1) ? " people" : " person") : "";

        $form['add_resources_stuff']['script'] = array(
            '#type' => 'item',
            '#prefix' => '',
            '#suffix' => '<script>$(\'div[name="search_result_text"]\').html(\'' . $result_text . '\');</script>',
        );
    }

    return $form;
}

function ntlp_resources_stuff_submission($form, $form_state) {
    global $user;
    $resource_type = $form_state['clicked_button']['#post']['add_resources_stuff']['selected_type'];
    $website_title = $form_state['clicked_button']['#post']['add_resources_stuff']['website_title'];
    $course_id = $form_state['clicked_button']['#post']['add_resources_stuff']['course_id'];

    if (isset($form_state['clicked_button']['#post']['files']['add_resources_stuff'])) {
        $path = $form_state['clicked_button']['#post']['files']['add_resources_stuff'];
    }

    $share_idea_text = $form_state['clicked_button']['#post']['add_resources_stuff']['share_ideas'];

    $share_idea_text = ($share_idea_text == "Share your thoughts with the class") ? "" : $share_idea_text;

    $resource_node = new stdClass();
    $resource_node->title = ($resource_type == 'lo_link') ? $website_title : 'Shared Comment';
    $resource_node->type = $resource_type;   // Your specified content type
    $resource_node->created = $time;

    $resource_node->body = $share_idea_text;

    $resource_node->status = 1;
    $resource_node->sticky = 0;
    $resource_node->format = 1;       // Filtered HTML
    $resource_node->uid = $user->uid;
    $resource_node->comment = 2;
    $resource_node->context = $course_id;

    node_save($resource_node);
    $nid = $resource_node->nid;

    $image_attachment = get_links($form_state['clicked_button']['#post']['attached_files_hidden']);
    if ($image_attachment != null || !empty($image_attachment)) {
        foreach ($image_attachment as $l) {
            list($att_type, $id, $filename, $url) = $l;
            if (!empty($att_type) && $att_type != 'X') {
                $upload_fileid = ($id == null) ? -1 : abs($id);
                $type = 'U';
            }
        }
    } else {
        $upload_fileid = -1;
    }

    switch ($resource_type) {
        case 'lo_share_idea':
            $type = "I";
            break;

        case 'lo_link':
            $type = 'L';
            $path = $form_state['clicked_button']['#post']['add_resources_stuff']['link_url'];
            if (strpos($path, "http")===0)
                $path = $path;
            else
                $path = "http://$path";
            break;

        case 'lo_document':
            $doc_option = $form_state['clicked_button']['#post']['add_resources_stuff']['google_radio'];
            if ($doc_option == 'upld')
                $type = 'U';
            else {
                $doc_attachment = get_links($form_state['clicked_button']['#post']['add_resources_stuff']['google_hAttachedFiles']);
                if ($doc_attachment != null || !empty($doc_attachment)) {
                    foreach ($doc_attachment as $l) {
                        list($att_type, $id, $docname, $url) = $l;
                        if (!empty($att_type) && $att_type != 'X') {
                            $type = 'G';
                            $upload_fileid = 0;
                            $path = $url;
                            $filename = $docname;
                        }
                    }
                }
            }
            break;

        case 'lo_image':
            break;

        case 'lo_video':
            $video_option = $form_state['clicked_button']['#post']['add_resources_stuff']['video_upload_radio'];

            if ($video_option == 'embd') {
                $type = 'E';
                $path = $form_state['clicked_button']['#post']['add_resources_stuff']['video_embedded_code'];
            }
            else {
                $type = 'U';
            }
            break;
    }

    $query = db_query("INSERT INTO {ntlp_course_resource}
        (nid, course_nid, fid, type, attachment_name, path, deleted_by, deleted_on, is_shared )
        VALUES(%d, %d, %d, '%s', '%s', '%s', %d, NULL, %d)",
        $nid, $course_id, $upload_fileid, $type, $filename, $path, 0, 1);
    
    module_invoke_all('ntlpcontent', $resource_node, 'content_posted');
    
    // block cache update code.
    $node_type = ntlp_get_node_type($course_id);
    if ($node_type == 'ntlp_group') {
        ntlp_update_block_cache('ntlp_group', $course_id);
    }
}
