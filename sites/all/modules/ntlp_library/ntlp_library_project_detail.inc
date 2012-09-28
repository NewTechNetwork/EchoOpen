<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


define("NTLP_Subjects", 8);
define("NTLP_Posts", 2);

module_load_include('inc', 'ntlp_project', 'data_access');
module_load_include('inc', 'ntlp_school', 'data_access');

include 'data_access.inc';

function _get_ntlp_library_project_commentary_contents($project_nid) {
    return drupal_get_form('ntlp_library_project_commentary_form', $project_nid);
}

function ntlp_library_project_commentary_form($form_state = null, $project_id) {
    drupal_add_js(drupal_get_path('module', 'ntlp_library') . '/ntlp_library.js');
    include("activity_template.inc");
    include "ntlp_library.tpl.inc";
    global $base_path, $user;
    $root = $base_path;


    // Get the query string from the referer URL
    $reff_query = explode('?q=', $_SERVER['HTTP_REFERER']);
    $reff_query = $reff_query[1];
    $lib_project_obj = _get_library_project($project_id);
    $form['project_commentary'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div id="wrapper-project-commentary">' . $HTMLform_activity_title_hdr,
        '#suffix' => $HTMLform_activity_title_ftr . '</div>',
    );

    $form['project_commentary']['table'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => '<div>',
        '#suffix' => '</div>',
    );


    $type = 'node';
    $lib_project_node = node_load($project_id);
    $form['project_commentary']['table']['tr2'] = array(
        '#type' => 'item',
        '#value' => 'Teacher\'s Personal Reflection',
        '#prefix' => $HTMLform_projectDetail_hdr . '<table width="100%"><tr><th colspan="2" style="text-align:left;padding:5px"><h3 style="padding:0px;margin:0px;color:#3491CC">',
        '#suffix' => '</h3></th></tr>',
    );

    if ($lib_project_obj->teacher_comment_cid != 0) {

        $objComment = _comment_load($lib_project_obj->teacher_comment_cid);
        $user_detail = get_comments_user_detail($objComment->uid);
        $dateformat = "%b %d, %Y";
        $ctime = strftime($dateformat, $objComment->timestamp);
    } else {
        $ctime = '';
    }


    $content = $HTMLContent_STAFF_NETWORK;
    $comment_type = 'posting';
    $content = str_replace("@ADDREVIEWPOST", '', $content);

    if (is_project_designer($user->uid, $project_id)) {

        if ($lib_project_obj->teacher_comment_cid > 0) {
            $commentadd = '<table cellspacing="0" border="0" width="820px" cellpadding="5"><tr><td rowspan="2" valign="top" style="border:none;width:35px"><img width="32" height="32" src="' . is_user_image_exist($user_detail->picture) . '"> ' . '</td><td style="border:none;width:100px" valign="top"><b style="color:#3491CC">' . $user_detail->first_name . ' ' . $user_detail->last_name . '</b><br /><small>' . $ctime . '</small></td>';
            $commentadd .= "<td style='border:none'><div id='comment_box' style='display:none'>";
            $commentadd .= "<input type='hidden' id='comment_cid_" . $lib_project_node->nid . "' value='" . $lib_project_obj->teacher_comment_cid . "'>";
            $commentadd .= "<textarea id='txt_post_" . $lib_project_node->nid . "' rows='2' style='width:650px'>" . $objComment->comment . "</textarea>";
            $commentadd .= "<br clear='all' /><br clear='all' /><a class='SaveBtn' onclick=library_comments_post_new($lib_project_node->nid,'$comment_type')>Save</a></div>";

            $commentadd .= "<div id='comment_item' style='display:block'><div id='node-posting'>" . $objComment->comment . '</div>';
            $commentadd .= "<br><a id='btn_submit' class='SaveBtn' onclick='showhide_comments()'>Edit</a></div></td></tr></table>";
        } else {

            $commentadd = '<table cellspacing="0" border="0" width="820px" cellpadding="5"><tr><td style="border:none;width:35px"><img width="32" height="32" src="' . is_user_image_exist($user_detail->picture) . '"> ' . '</td><td style="border:none;width:100px" valign="top"><b style="color:#3491CC">' . $user_detail->first_name . ' ' . $user_detail->last_name . '</b><br /><small>' . $ctime . '</small></td>';

            $commentadd .= "<td style='border:none'><div id='comment_box' style='display:block'>";
            $commentadd .= "<input type='hidden' id='comment_cid_" . $lib_project_node->nid . "' value='" . $lib_project_obj->teacher_comment_cid . "'>";
            $commentadd .= "<textarea id='txt_post_" . $lib_project_node->nid . "' rows='2'  style='width:650px;overflow:auto'>" . $objComment->comment . "</textarea>";
            $commentadd .= "<br clear='all' /><br clear='all' /><a class='SaveBtn' onclick=library_comments_post_new($lib_project_node->nid,'$comment_type')>Save</a></div>";

            $commentadd .= "<div id='comment_item' style='display:none'><div id='node-posting'>" . $objComment->comment . '</div>';
            $commentadd .= "<br><br><a id='btn_submit' class='SaveBtn' onclick='showhide_comments()'>Edit</a></div></td></tr></table>";
        }
    } else {
        $commentadd = '<table cellspacing="0" border="0" width="100%" cellpadding="5"><tr><td rowspan="2" valign="top" style="border:none;width:35px"><img width="32" height="32" src="' . is_user_image_exist($user_detail->picture) . '"> ' . '</td><td style="border:none;width:100px" valign="top"><b style="color:#3491CC">' . $user_detail->first_name . ' ' . $user_detail->last_name . '</b><br /><small>' . $ctime . '</small></td>';
        $commentadd .= "<td style='border:none'><div id='comment_item' style='display:block'><div id='node-posting'>" . $objComment->comment . '</div></div></td></tr></table>';
    }

    $content = str_replace("@ADDCOMMENTPOST", $commentadd, $content);

    $form['project_commentary']['table']['comment'] = array(
        '#type' => 'item',
        '#value' => $content,
        '#prefix' => '<tr><td style="border:none">',
        '#suffix' => '</td></tr></table>' . $HTMLform_projectDetail_ftr,
    );

//////////////
    $form['project_commentary']['table']['tr3'] = array(
        '#type' => 'item',
        '#value' => 'Reviewer\'s Personal Reflection',
        '#prefix' => $HTMLform_projectDetail_hdr . '<table width="100%"><tr><th colspan="2" style="text-align:left;border:none;padding:5px"><h3 style="padding:0px;margin:0px;color:#3491CC">',
        '#suffix' => '</h3></th></tr>',
    );
    $objComment = _comment_load($lib_project_obj->reviewer_comment_cid);
    $user_detail = get_comments_user_detail($objComment->uid);
    $content = $HTMLContent_STAFF_NETWORK;
    $comment_type = 'reviewing';
    $content = str_replace("@ADDCOMMENTPOST", '', $content);

    if (user_access("review project in library")) {
        if ($lib_project_obj->reviewer_comment_cid > 0) {
            $commentadd = '<table cellspacing="0" border="0" width="820px" cellpadding="5"><tr><td rowspan="2" valign="top" style="border:none;width:35px"><img width="32" height="32" src="' . is_user_image_exist($user_detail->picture) . '"> ' . '</td><td style="border:none;width:100px" valign="top"><b style="color:#3491CC">' . $user_detail->first_name . ' ' . $user_detail->last_name . '</b><br /><small>' . $ctime . '</small></td>';
            $commentadd .= "<td style='border:none'><div id='review_box' style='display:none'>";
            $commentadd .= "<input type='hidden' id='review_cid_" . $lib_project_node->nid . "' value='" . $lib_project_obj->reviewer_comment_cid . "'>";
            $commentadd .= "<textarea style=\"font-family: arial;width:99.5%\" id='txt_review_" . $lib_project_node->nid . "' rows='2'>" . $objComment->comment . "</textarea>";
            if ($lib_project_obj->is_exemplary == 1)
                $commentadd .= "<br><input type='checkbox' id='is_exemplary' checked='checked'> Exemplary Project";
            else
                $commentadd .= "<br><input type='checkbox' id='is_exemplary'> Exemplary Project";
            $commentadd .= "<br><a class='CommentBtn' onclick=library_comments_post_new($lib_project_node->nid,'$comment_type')>Save</a></div>";

            $commentadd .= "<div id='review_item' style='display:block'><div style=\"margin-left:5px\" id='node-reviewing'>";
            $commentadd .= $objComment->comment;
            if ($lib_project_obj->is_exemplary == 1)
                $commentadd .= "<div class=\"img\" style='float:left;text-align:center'><img src=\"" . $root . "themes/Boldr/Images/ProjectsImages/exemplary_project.png\"  height=\"50\" /><br><div class=\"text\" style=\"text-align:center\"><small>Exemplary<br>Project</small></div></div>";
            $commentadd .= '</div>';
            $commentadd .= "<br><a id='btn_submit' class='SaveBtn' onclick='showhide_reviewing()'>Edit</a></div></td></tr></table>";
        }else {
            $commentadd = '<table cellspacing="0" border="0" width="820px" cellpadding="5"><tr><td rowspan="2" valign="top" style="border:none;width:35px"><img width="32" height="32" src="' . is_user_image_exist($user_detail->picture) . '"> ' . '</td><td style="border:none;width:100px" valign="top"><b style="color:#3491CC">' . $user_detail->first_name . ' ' . $user_detail->last_name . '</b><br /><small>' . $ctime . '</small></td>';
            $commentadd .= "<td style='border:none><div id='review_box' style='display:block'>";
            $commentadd .= "<input type='hidden' id='review_cid_" . $lib_project_node->nid . "' value='" . $lib_project_obj->reviewer_comment_cid . "'>";
            $commentadd .= "<textarea style=\"font-family: arial;width:99.5%\" id='txt_review_" . $lib_project_node->nid . "' rows='2'>" . $objComment->comment . "</textarea>";
            if ($lib_project_obj->is_exemplary == 1)
                $commentadd .= "<br><input type='checkbox' id='is_exemplary' checked='checked'> Exemplary Project";
            else
                $commentadd .= "<br><input type='checkbox' id='is_exemplary'> Exemplary Project";
            $commentadd .= "<br><a class='SaveBtn' onclick=library_comments_post_new($lib_project_node->nid,'$comment_type')>Save</a></div>";

            $commentadd .= "<div id='review_item' style='display:none'><div id='node-reviewing'>" . $objComment->comment . '</div>';
            $commentadd .= "<br><a id='btn_submit' class='SaveBtn' onclick='showhide_reviewing()'>Edit</a></div></td></tr></table>";
        }
    }else {
        $commentadd = '<table cellspacing="0" border="0" width="820px" cellpadding="5"><tr><td rowspan="2" valign="top" style="border:none;width:35px"><img width="32" height="32" src="' . is_user_image_exist($user_detail->picture) . '"></td><td valign="top" style="border:none;width:100px"> <b style="color:#3491CC">' . $user_detail->first_name . ' ' . $user_detail->last_name . '</b><br /><small>' . $ctime . '</small></td>';
        $commentadd .= "<td style='border:none'><div id='review_item' style='display:block'><div id='node-reviewing'>" . $objComment->comment . '</div>';
        if ($lib_project_obj->is_exemplary == 1)
            $commentadd .= "<div style='float:right'><div class=\"img\" style=\"text-align:center\"><img src=\"" . $root . "themes/Boldr/Images/ProjectsImages/exemplary_project.png\"  height=\"50\" /></div><div class=\"text\" style=\"text-align:center\"><small>Exemplary<br>Project</small></div></div></td></tr></table>";
        else
            $commentadd .= "</td></tr></table>";
    }
///////////////

    $content = str_replace("@ADDREVIEWPOST", $commentadd, $content);
    $form['project_commentary']['table']['review'] = array(
        '#type' => 'item',
        '#value' => $content,
        '#prefix' => '<tr><td style="border:none">',
        '#suffix' => '</td></tr></table>' . $HTMLform_projectDetail_ftr,
    );

//////////////

    $form['project_commentary']['table']['heading_2'] = array(
        '#type' => 'item',
        '#value' => 'Community Comments',
        '#prefix' => $HTMLform_projectDetail_hdr . '<table><tr><th colspan="2" style="text-align:left;border:none;padding:5px"><h3 style="padding:0px;margin:0px;color:#3491CC">',
        '#suffix' => '</h3></th></tr>',
    );

    $form['project_commentary']['table']['tr4'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => '<tr><td style="border:none;font-size:5px">&nbsp;</td></tr><tr>',
        '#suffix' => '</tr></table>' . $HTMLform_projectDetail_ftr,
    );


    $type = 'node';
    $lib_project_node = node_load($project_id);
//    $result = db_query("SELECT teacher_comment_cid,reviewer_comment_cid  FROM {ntlp_library_project} WHERE nid = %d", $project_id);
//    $rows = db_fetch_array($result);
    $comment_cid[] = $lib_project_obj->teacher_comment_cid;
    $comment_cid[] = $lib_project_obj->reviewer_comment_cid;
    $lib_project_node->skip_comment_cid = $comment_cid;

    $node_cmt_content = $HTMLContent;
    $comment_type = "comments";
    $nodeComments = ntlp_view_comments($lib_project_node);
    $node_cmt_content = str_replace("@COMMENTS", '<div id=\'node-comments\'>' . $nodeComments . '</div>', $node_cmt_content);
    $commentsadd = "<textarea style='display:block;width:805px' id='txt_comment_" . $lib_project_node->nid . "' rows='2'></textarea>";
    $commentsadd .= "<a style='display:block; color:white;' id='comment_new_" . $lib_project_node->nid . "' class='CommentBtn' onclick=library_comments_post_new($lib_project_node->nid,'$comment_type')>Comment</a>";
    $node_cmt_content = str_replace("@ADDCOMMENTSFORM", $commentsadd, $node_cmt_content);

    $form['project_commentary']['table']['tr4']['td1'] = array(
        '#type' => 'item',
        '#value' => $node_cmt_content,
        '#prefix' => '<td colspan"2" style="border:none">',
        '#suffix' => '</td>',
    );

    return $form;
}

function ntlp_view_comments($node) {
//    print_r($node);
    include("comment.template.inc");
    global $user;
    global $base_path;
    if (user_access('view comment')) {

        // Pre-process variables.
        $nid = $node->nid;
        if (empty($nid)) {
            $nid = 0;
        }
        $mode = _comment_get_display_setting('mode', $node);
        $order = _comment_get_display_setting('sort', $node);
        $comments_per_page = _comment_get_display_setting('comments_per_page', $node);


        if ($cid && is_numeric($cid)) {
            // Single comment view.

            $query = 'SELECT c.cid, c.pid, c.nid, c.subject, c.comment, c.format, c.timestamp, c.name, c.mail, c.homepage, u.uid, u.name AS registered_name, u.signature, u.signature_format, u.picture, u.data, c.status FROM {comments} c INNER JOIN {users} u ON c.uid = u.uid WHERE c.cid = %d';
            $query_args = array($cid);
            if (!user_access('view comment')) {
                $query .= ' AND c.status = %d';
                $query_args[] = COMMENT_PUBLISHED;
            }

            $query = db_rewrite_sql($query, 'c', 'cid');
            $result = db_query($query, $query_args);

            if ($comment = db_fetch_object($result)) {
                $comment->name = $comment->uid ? $comment->registered_name : $comment->name;
                $links = module_invoke_all('link', 'comment', $comment, 1);
                drupal_alter('link', $links, $node);

                $output .= theme('comment_view', $comment, $node, $links);
            }
        } else {
            // Multiple comment view
            $query_count = 'SELECT COUNT(*) FROM {comments} c WHERE c.nid = %d';
            $query = 'SELECT c.cid as cid, c.pid, c.nid, c.subject, c.comment, c.format, c.timestamp, c.name, c.mail, c.homepage, u.uid, u.name AS registered_name, u.signature, u.signature_format, u.picture, u.data, c.thread, c.status FROM {comments} c INNER JOIN {users} u ON c.uid = u.uid WHERE c.nid = %d';

            //skip for following comments
            if (!empty($node->skip_comment_cid)) {
                $k = 0;
                foreach ($node->skip_comment_cid as $key) {
                    if ($key > 0) {

                        if ($k > 0)
                            $comment_cid .= ' and c.cid != ' . $key;
                        else
                            $comment_cid = ' c.cid != ' . $key;
                        $k++;
                    }
                }
                if (!empty($comment_cid))
                    $query .= ' and (' . $comment_cid . ')';
            }

            $query_args = array($nid);
            if (!user_access('view comment')) {
                $query .= ' AND c.status = %d';
                $query_count .= ' AND c.status = %d';
                $query_args[] = COMMENT_PUBLISHED;
            }

            if ($order == COMMENT_ORDER_NEWEST_FIRST) {
                if ($mode == COMMENT_MODE_FLAT_COLLAPSED || $mode == COMMENT_MODE_FLAT_EXPANDED) {
                    $query .= ' ORDER BY c.cid DESC';
                } else {
                    $query .= ' ORDER BY c.thread DESC';
                }
            } else if ($order == COMMENT_ORDER_OLDEST_FIRST) {
                if ($mode == COMMENT_MODE_FLAT_COLLAPSED || $mode == COMMENT_MODE_FLAT_EXPANDED) {
                    $query .= ' ORDER BY c.cid';
                } else {
                    // See comment above. Analysis reveals that this doesn't cost too
                    // much. It scales much much better than having the whole comment
                    // structure.
                    $query .= ' ORDER BY SUBSTRING(c.thread, 1, (LENGTH(c.thread) - 1))';
                }
            }
            $query = db_rewrite_sql($query, 'c', 'cid');
            $query_count = db_rewrite_sql($query_count, 'c', 'cid');

            // Start a form, for use with comment control.
            $result = pager_query($query, $comments_per_page, 0, $query_count, $query_args);

            $divs = 0;
            $num_rows = FALSE;
            $comments = '';
            drupal_add_css(drupal_get_path('module', 'comment') . '/comment.css');
            while ($comment = db_fetch_object($result)) {
                $comment = drupal_unpack($comment);
                $comment->name = $comment->uid ? $comment->registered_name : $comment->name;
                $comment->depth = count(explode('.', $comment->thread)) - 1;

                $content = $HTMLComment;
                $root = $base_path;
                $user_data = get_user_data($comment->uid);

//                if ($comment->picture != "")
//                    if (empty($user_data->first_name))
//                        $content = str_replace("@POSTED_BY_IMAGE", '<img src="' . $root . $comment->picture . '" alt="' . $user_data->name . '" height="32" width="32" />', $content);
//                    else
//                        $content = str_replace("@POSTED_BY_IMAGE", '<img src="' . $root . $comment->picture . '" alt="' . $user_data->first_name . ' ' . $user_data->last_name . '" height="32" width="32" />', $content);
//                else
//                    $content = str_replace("@POSTED_BY_IMAGE", '<img src="' . $root . 'sites/default/files/avatars/picture-1.jpg" height="32" width="32" />', $content);

                $content = str_replace("@POSTED_BY_IMAGE", '<img src="' . is_user_image_exist($comment->picture) . '"; height="32" width="32" />', $content);

                $content = str_replace("@COMMENTS", $comment->comment, $content);
                if (empty($user_data->first_name))
                    $content = str_replace("@HEADING", $user_data->name . ':', $content);
                else
                    $content = str_replace("@HEADING", $user_data->first_name . ' ' . $user_data->last_name . ':', $content);
//                comment_owner($comment->cid);
//                $content = str_replace("@DELETE", l("Delete", 'ntlp/comments/delete/'.$comment->cid, array('attributes' => array('class' => 'popups-form-reload',) )), $content);
//                $content = str_replace("@DATEDIFF",ago($comment->timestamp), $content);
                $dateformat = "%b %d, %Y";
                $ctime = strftime($dateformat, $comment->timestamp);
                $content = str_replace("@DATEDIFF", $ctime, $content);

//                //checking a permission for delete comments
//                if(user_access('delete comment')) {
//                    $content = str_replace("@DELETE","<a class='ntlp_tooltip_n' title='Delete Comment' id='comment_delete_$comment->cid' onclick='library_delete_comments($comment->cid, $comment->nid)'></a>", $content);
//                }else {
//                    if(is_comment_owner($comment->cid, $user)) {
//                        $content = str_replace("@DELETE","<a class='ntlp_tooltip_n' title='Delete Comment' id='comment_delete_$comment->cid' onclick='library_delete_comments($comment->cid, $comment->nid)'></a>", $content);
//                    }else {
//                        $content = str_replace("@DELETE", "", $content);
//                    }
//                }
                $contentDon .= $content;
            }
            return $contentDon;
        }
    }
}

function _get_ntlp_library_project_student_work_contents() {
//    return drupal_get_form('ntlp_library_project_student_work_form');
}

function ntlp_library_project_student_work_form() {
    global $base_path, $user;
    include("activity_template.inc");


    if ($courseid == 0) {

        $reff_query = explode('?q=', $_SERVER['HTTP_REFERER']);
        $reff_query = $reff_query[1];

        //Getting Query String From URL
        $courseid = 0;
        $project_id = 0;
        $url = $reff_query;
        $args = explode('/', $url);
        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i]) && empty($courseid))
                $courseid = $args[$i];
            elseif (is_numeric($args[$i]) && empty($project_id))
                $project_id = $args[$i];
        }
    }

    $project_id = $courseid;


    $form['student_work'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div id="edit_project_details_test">' . $HTMLform_activity_title_hdr,
        '#suffix' => $HTMLform_activity_title_ftr . '</div>',
    );


    $form['student_work']['table'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => '<table width="100%">',
        '#suffix' => '</table>',
    );

    $form['student_work']['table']['tr1'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => '<tr>',
        '#suffix' => '</tr>',
    );


    $form['student_work']['table']['tr1']['td1'] = array(
        '#type' => 'item',
        '#value' => t('Sample of Student associated with this project'),
        '#prefix' => '<td>',
        '#suffix' => '</td>',
    );


    $form['student_work']['table']['tr1']['td2'] = array(
        '#type' => 'item',
        '#value' => l('Add Sample', 'ntlp/course/resource/project/' . $project_id), //ntlp/course/resources/'.$courseid
        '#prefix' => '<td><div style="float:right" class="ProjectResourcesAddExistingBtn">',
        '#suffix' => '</div></td>',
    );


    $form['student_work']['table']['tr2'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => '<tr>',
        '#suffix' => '</tr>',
    );

    $view = views_get_view('ntlp_project_section_resources');
    $view->set_arguments(array($project_id, 0));

    $form['student_work']['table']['tr2']['td1'] = array(
        '#type' => 'item',
        '#value' => $view->preview(),
        '#prefix' => '<td colspan="2">',
        '#suffix' => '</td>',
    );

    return $form;
}

function _get_ntlp_library_project_header($lib_project_Obj, $subpage = 'pd') {
    global $user, $base_path;
    include("library_project_template.inc");
    drupal_add_js(drupal_get_path('module', 'ntlp_library') . '/ntlp_library.js');

    if (isset($_GET['res'])) {
        $res_nid = $_GET['res'];
    } else {
        $res_nid = '';
    }

    $header_container = $HTML_Lib_Project_Detail_header;


    $node_related_taxo = _get_library_projects_taxonomies($lib_project_Obj->nid);
    watchdog("lib_proj_tax", '$node_related_taxo ' . print_r($node_related_taxo, true));

    foreach ($node_related_taxo as $Obj) {

        if ($Obj->vid == NTLP_VOCABULARY_SUBJECTS) {
            $subject_array[] = $Obj;
        } else if ($Obj->vid == NTLP_KEYWORDS_FOR_PROJECT_LIBRARY) {
            $keywords_array[] = $Obj;
        }
    }


    if ($subject_array != null) {

        $i = 0;
        foreach ($subject_array as $subject) {

            if (empty($subject_array[$i + 1])) {

                $parent = get_parent_subject($subject->tid);
                if (!$parent) {
                    $project_subject .= '<a class="green_link" href="' . url('ntlp/library/project/search', array('query' => array('subject' => $subject->tid))) . '">' . $subject->name . '</a>';
                } else {

                    $project_subject .= '<a class="green_link" href="' . url('ntlp/library/project/search', array('query' => array('subject' => $subject->tid))) . '">' . $parent . '-> ' . $subject->name . '</a>';
                }
            } else {
                $parent = get_parent_subject($subject->tid);

                if (!$parent) {
                    $project_subject .= '<a class="green_link" href="' . url('ntlp/library/project/search', array('query' => array('subject' => $subject->tid))) . '">' . $subject->name . '</a>, &nbsp;';
                } else {
                    $project_subject .= '<a class="green_link" href="' . url('ntlp/library/project/search', array('query' => array('subject' => $subject->tid))) . '">' . $parent . '-> ' . $subject->name . '</a>, &nbsp;';
                }
            }

            $i++;
        }
    }


    if ($keywords_array != null) {
        $i = 1;
        foreach ($keywords_array as $keyword) {
            if (sizeof($keywords_array) == $i) {
                $keywords .= '<a class="green_link" href="' . url('ntlp/library/project/search', array('query' => array('keyword' => $keyword->tid))) . '">' . $keyword->name . '</a> ';
            } else {

                $keywords .= '<a class="green_link" href="' . url('ntlp/library/project/search', array('query' => array('keyword' => $keyword->tid))) . '">' . $keyword->name . '</a>, ';
            }
            $i++;
        }
    }
    if ($lib_project_Obj->published_on == '0000-00-00 00:00:00') {
        $published_status = ' Not Published Yet';
    } else {
        $published_status = get_tz(get_this_school(), 'm/d/Y', $lib_project_Obj->published_on);
    }

    $header_container = str_replace('@LIB_RPOJECT_SUMMARY', $lib_project_Obj->body, $header_container);
    $header_container = str_replace('@PROJECT_LIB_SUBJECTS', $project_subject, $header_container);
    $header_container = str_replace('@PROJECT_LIB_KEYWORDS', $keywords, $header_container);


    $user_obj = _get_user_info($lib_project_Obj->submitted_by);
    $project_author = '<a class="green_link" href="' . url('ntlp/user/profile/' . $lib_project_Obj->submitted_by) . '"> ' . $user_obj->first_name . ' ' . $user_obj->last_name . '</a>';

    $header_container = str_replace('@PROJECT_LIB_AUTHOR', $project_author, $header_container);
    $header_container = str_replace('@PROJECT_PUBLISHED_DATE', $published_status, $header_container);


    $likethispost = flag_get_counts('node', $lib_project_Obj->nid);

    $header_container = str_replace('@PROJECT_LIKES', (($likethispost['likethispost'] >= 1) ? $likethispost['likethispost'] : '0'), $header_container);
    $header_container = str_replace('@PROJECT_COPYIES_COUNT', _get_project_copied_count($lib_project_Obj->nid), $header_container);
    $header_container = str_replace('@PROJECT_COMMENTS_COUNT', comment_num_all($lib_project_Obj->nid), $header_container);

    $project_title_area = $HTML_Lib_Project_title_area;



    if (is_project_designer($user->uid, $lib_project_Obj->nid)) {
        $project_combo = $HTML_Lib_project_setting_combo;

        if (isset($user->roles[NTLP_ROLEID_NETWORKMANAGER]) || isset($user->roles[NTLP_ROLEID_NETWORKSTAFF])) {
            $project_combo = str_replace('<!-- @EDIT_PROJECT -->', '<li class="ManageOption">' . l('Edit Project', 'ntlp/library/project/setting/0/' . $lib_project_Obj->nid) . '</li>', $project_combo);
            $project_combo = str_replace('@COPY_PROJECT', dlg('Copy Project', 'ntlp/library/project/copy/popup/' . $lib_project_Obj->nid, 450, 600), $project_combo);
            $project_combo = str_replace('@REQUEST_ARCHIVE', dlg('Request Archive', 'ntlp/library/project/request/popup/' . $lib_project_Obj->nid, 450, 600), $project_combo);
            $project_combo = str_replace('@FIXGREEN_COMBO_CLASS', '', $project_combo);
        } else {

            $project_combo = str_replace('@COPY_PROJECT', dlg('Copy Project', 'ntlp/library/project/copy/popup/' . $lib_project_Obj->nid, 450, 600), $project_combo);
            $project_combo = str_replace('@REQUEST_ARCHIVE', dlg('Request Archive', 'ntlp/library/project/request/popup/' . $lib_project_Obj->nid, 450, 600), $project_combo);
            $project_combo = str_replace('@FIXGREEN_COMBO_CLASS', 'class="fixGreenCombo"', $project_combo);
        }
    } else {

        if (user_access('archive library project')) {

            if (is_project_archived($lib_project_Obj->nid)) {
                $project_combo = $HTML_Lib_project_setting_combo;
                $project_combo = str_replace('<!-- @EDIT_PROJECT -->', '<li class="ManageOption">' . l('Edit Project', 'ntlp/library/project/setting/0/' . $lib_project_Obj->nid) . '</li>', $project_combo);
                $project_combo = str_replace('@COPY_PROJECT', dlg('Copy Project', 'ntlp/library/project/copy/popup/' . $lib_project_Obj->nid, 450, 600), $project_combo);
                $project_combo = str_replace('@REQUEST_ARCHIVE', dlg('Publish', 'ntlp/library/project/popup/published/' . $lib_project_Obj->nid, 450, 600), $project_combo);
                $project_combo = str_replace('@FIXGREEN_COMBO_CLASS', '', $project_combo);
            } else {
                $project_combo = $HTML_Lib_project_setting_combo;
                $project_combo = str_replace('<!-- @EDIT_PROJECT -->', '<li class="ManageOption">' . l('Edit Project', 'ntlp/library/project/setting/0/' . $lib_project_Obj->nid) . '</li>', $project_combo);
                $project_combo = str_replace('@COPY_PROJECT', dlg('Copy Project', 'ntlp/library/project/copy/popup/' . $lib_project_Obj->nid, 450, 600), $project_combo);
                $project_combo = str_replace('@REQUEST_ARCHIVE', dlg('Archive', 'ntlp/library/project/popup/archived/' . $lib_project_Obj->nid, 450, 600), $project_combo);
                $project_combo = str_replace('@FIXGREEN_COMBO_CLASS', '', $project_combo);
            }
        } else {
            $project_combo = dlg('Copy Project', 'ntlp/library/project/copy/popup/' . $lib_project_Obj->nid, 450, 600, Activities_btn);
        }
    }

    $project_images = get_library_project_image($lib_project_Obj->nid);
    if ($project_images->is_exemplary == 1) {
        $image = '<img align="absmiddle" src="' . $base_path . 'themes/Boldr/Images/ProjectsImages/exemplary_project.png" width="42" height="42">';
    } else {
        $image = '';
    }
    $project_title_area = str_replace('@PROJECT_DEFAULT_IMAGE', $image, $project_title_area);
    $project_title_area = str_replace('@PROJECT_TITLE', $lib_project_Obj->title, $project_title_area);
    $project_title_area = str_replace('@LIKE_IMAGE', flag_create_link('likethispost', $lib_project_Obj->nid), $project_title_area);
    $project_title_area = str_replace('@PROJECT_SETTING_COMBO', $project_combo, $project_title_area);

    $header_container = str_replace('@PROJCT_TITLE_AND_COMBO', $project_title_area, $header_container);

    $project_detail_onclick_action = "window.location = '?q=ntlp/library/project/view/" . $lib_project_Obj->nid . "&sub=pd';";
    $project_commentary_onclick_action = "window.location = '?q=ntlp/library/project/view/" . $lib_project_Obj->nid . "&sub=com';";

    if (isset($res_nid) && $res_nid != '') {
        $library_project_tab = '<ul>';
        $library_project_tab .= '<li> <span ><a style=" font-size: 14px; color: rgb(53, 112, 170); font-weight: bold; text-decoration: none;" onclick="' . $project_detail_onclick_action . '" > Project Details</a></span></li>';
        $library_project_tab .= '<li>|</li>';
        $library_project_tab .= '<li> <span style="font-size: 14px; color: rgb(128, 210, 61); text-decoration: none; font-weight: bold;" > <a onclick="' . $project_commentary_onclick_action . '">Commentary </a></span> </li>';
        $library_project_tab .= '</ul>';
    } else {
        if ($subpage == 'pd') {
            $library_project_tab = '<ul>';
            $library_project_tab .= '<li> <span style="font-size: 14px; color: rgb(53, 112, 170); font-weight: bold; text-decoration: none;">Project Details </span></li>';
            $library_project_tab .= '<li>|</li>';
            $library_project_tab .= '<li> <span style="font-size: 14px; color: rgb(128, 210, 61); text-decoration: none; font-weight: bold;" > <a onclick="' . $project_commentary_onclick_action . '">Commentary </a></span> </li>';
            $library_project_tab .= '</ul>';
        } else {
            $library_project_tab = '<ul>';
            $library_project_tab .= '<li> <span style="font-size: 14px; color: rgb(128, 210, 61); font-weight: bold; text-decoration: none;"><a onclick="' . $project_detail_onclick_action . '" > Project Details</a> </span></li>';
            $library_project_tab .= '<li>|</li>';
            $library_project_tab .= "<li> <span style='font-size: 14px; color: rgb(53, 112, 170); text-decoration: none; font-weight: bold;' > Commentary </a> </span> </li>";
            $library_project_tab .= '</ul>';
        }
    }

    $header_container = str_replace('@LIBRARY_PROJECT_TAB', $library_project_tab, $header_container);


    return $header_container;
}

function _get_ntlp_library_project_info_contents() {
    return drupal_get_form('ntlp_library_project_info_form');
}

function ntlp_library_project_info_form() {

    include("activity_template.inc");


    drupal_add_css(drupal_get_path('theme', 'Boldr') . '/StylishSelect/stylish-select.css', 'module', 'all', FALSE);
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/externalFile.js');
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/jquery.stylish-select.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/modalframe_setup.js');

    drupal_add_js(drupal_get_path('module', 'ntlp_library') . '/ntlp_library.js');

    global $base_path, $user;

    if ($courseid == 0) {
        //Getting Query String From URL
        $courseid = 0;
        $project_id = 0;
        $url = $_GET['q'];
        $args = explode('/', $url);
        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i]) && empty($courseid))
                $courseid = $args[$i];
            elseif (is_numeric($args[$i]) && empty($project_id))
                $project_id = $args[$i];
        }
    }


    modalframe_parent_js();
    $project_id = $courseid;

//    if(check_user_permission($project_id)) {
    set_item_url_handler("Library Project");
    $lib_project_node = node_load($project_id);


    $lib_project_Obj = _get_library_project($project_id);
    $node_related_taxo = taxonomy_node_get_terms($lib_project_node);

    foreach ($node_related_taxo as $Obj) {

        if ($Obj->vid == NTLP_Subjects) {
            $subject_array[] = $Obj;
        }
        if ($Obj->vid == NTLP_Posts) {
            $keywords_array[] = $Obj;
        }
    }


    $projectObj = load_project_record($src_project_id);


    $course_Obj = load_project_record($projectObj->course_nid);
    $subject_txo = taxonomy_get_term($course_Obj->subject);


    $form['project_info'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div id="edit_project_details_test">',
        '#suffix' => '</div>',
    );

    watchdog('plib', 'Is project owner: ' . $user->uid . ' of ' . $project_id);
    if (is_project_designer($user->uid, $project_id)) {

        $combo_options = '<li class="ManageOption">' . dlg('Copy Project', 'ntlp/library/project/copy/popup/' . $project_id, 450, 600) . '</li>';
        $combo_options .= '<li class="ManageOption">' . dlg('Request Archive', 'ntlp/library/project/request/popup/' . $project_id, 450, 600) . '</li>';
    } else {

        $combo_options = '<li class="ManageOption">' . dlg('Copy Project', 'ntlp/library/project/copy/popup/' . $project_id, 450, 600) . '</li>';
    }

    if (user_access('archive library project')) {

        if (is_project_archived($project_id)) {
//            $options = array("C" => "Copy Project", "P"=> "Publish"); // for NETWORK Staff only
            $combo_options = '<li class="ManageOption">' . dlg('Copy Project', 'ntlp/library/project/copy/popup/' . $project_id, 450, 600) . '</li>';
            $combo_options .= '<li class="ManageOption">' . dlg('Publish', 'ntlp/library/project/popup/published/' . $project_id, 450, 600) . '</li>';
        } else {
//            $options = array("C" => "Copy Project", "A" => "Archive"); // for NETWORK Staff only
            $combo_options = '<li class="ManageOption">' . dlg('Copy Project', 'ntlp/library/project/copy/popup/' . $project_id, 450, 600) . '</li>';
            $combo_options .= '<li class="ManageOption">' . dlg('Archive', 'ntlp/library/project/popup/archived/' . $project_id, 450, 600) . '</li>';
        }
    }


    $form['project_info']['div'] = array(
        '#type' => 'item',
        '#value' => $combo_options,
        '#id' => 'project_archive',
        '#prefix' => '<div style="float:right;">
                <div class="anchorBaseCombobox">
                <div id="btnManageCourse" class="fixGreenCombo"></div>
                <div id="manageCourse" class="hideMODrpDown" style="display:none">
                <div class="hideMODrpDownInnerBg">
                <ul>

    ',
        '#suffix' => '</ul></div></div></div></div><div style="clear:both"></div>',
    );
    $form['project_info']['table'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => $HTMLform_activity_title_hdr,
        '#suffix' => $HTMLform_activity_title_ftr,
    );

    $featured_proj_header = ' ';

    $header = $HTMLform_activity_heading_hdr;

    if ($lib_project_Obj->is_exemplary)
        $header = str_replace("@FEATURED_PROJ_HEADER", $featured_proj_header, $header);
    else
        $header = str_replace("@FEATURED_PROJ_HEADER", '', $header);

    $form['project_info']['table']['tr1'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => $header,
        '#suffix' => $HTMLform_activity_heading_ftr,
    );

    $form['project_info']['table']['tr1']['td1'] = array(
        '#type' => 'item',
        '#value' => $lib_project_node->title,
        '#prefix' => '<h2 style="float:left">',
        '#suffix' => '</h2>',
    );

    $likethispost = flag_get_counts('node', $project_id);


    $form['project_info']['table']['tr1']['td2'] = array(
        '#type' => 'item',
        '#value' => '<ul><li><div class="img"><img src="' . $base_path . 'themes/Boldr/Images/common/img_like.png" /></div><div class="text">' . $likethispost['likethispost'] . ' Likes </div></li><li><div class="img"><img src="' . $base_path . 'themes/Boldr/Images/common/img_copies.png" /></div><div class="text">' . _get_project_copied_count($project_id) . ' Copies </div></li><li><div class="img"><img src="' . $base_path . 'themes/Boldr/Images/common/img_comment.png" /></div><div class="text"> ' . comment_num_all($project_id) . ' Comments</div></li></ul>',
        '#prefix' => '<div style="float:right" class="moreExemplary"><div class="likesTopRightCommentsCopies">',
        '#suffix' => '</div></div>',
    );


    $form['project_info']['table']['tr2'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => $HTMLform_activity_description_hdr,
        '#suffix' => $HTMLform_activity_description_ftr,
    );
    $form['project_info']['table']['tr2']['leftData'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => $HTMLform_activity_dataLeft_hdr . '<DIV>',
        '#suffix' => '</DIV>' . $HTMLform_activity_dataLeft_ftr,
    );

    $user_obj = _get_user_info($lib_project_Obj->submitted_by);

    $form['project_info']['table']['tr2']['leftData']['td1'] = array(
        '#type' => 'item',
        '#value' => '<a href="' . url('ntlp/user/profile/' . $lib_project_Obj->submitted_by) . '"> ' . $user_obj->first_name . ' ' . $user_obj->last_name . '</a>',
        '#prefix' => '<div><div class="SubHeading" style="float:left"><div>Designed by :</div></div><div class="SubHeading" style="float:left">',
        '#suffix' => '</div></div><br clear="all" />',
    );

    if ($subject_array != null) {


        $i = 0;
        foreach ($subject_array as $subject) {

            if (empty($subject_array[$i + 1])) {

                $parent = get_parent_subject($subject->tid);
                if (!$parent) {
                    $project_subject .= '<a href="' . url('ntlp/library/project/search', array('query' => array('subject' => $subject->tid))) . '">' . $subject->name . '</a>';
                } else {

                    $project_subject .= '<a href="' . url('ntlp/library/project/search', array('query' => array('subject' => $subject->tid))) . '">' . $parent . '-> ' . $subject->name . '</a>';
                }
            } else {
                $parent = get_parent_subject($subject->tid);

                if (!$parent) {
                    $project_subject .= '<a href="' . url('ntlp/library/project/search', array('query' => array('subject' => $subject->tid))) . '">' . $subject->name . '</a>, &nbsp;';
                } else {
                    $project_subject .= '<a href="' . url('ntlp/library/project/search', array('query' => array('subject' => $subject->tid))) . '">' . $parent . '-> ' . $subject->name . '</a>, &nbsp;';
                }
            }

            $i++;
        }

//            echo $parent;
//        if($parent == 0) {
//            $project_subject .= '<a href="'.url('taxonomy/term/'.$subject->tid).'">'.$subject->name.'</a> , ';
//        }else {
//            $project_subject .= '<a href="'.url('taxonomy/term/'.$subject->tid).'">'.$parent.' - '.$subject->name.'</a> , ';
//
//        }
    }

    $form['project_info']['table']['tr2']['leftData']['td2'] = array(
        '#type' => 'item',
        '#value' => '<div class="SubHeading" style="float:left">' . $project_subject . '</div>',
        '#prefix' => '<div><div class="SubHeading" style="float:left"><div>Subjects : </div></div>',
        '#suffix' => '</div><br clear="all" />',
    );


    if ($keywords_array != null) {
        foreach ($keywords_array as $keyword) {
//        $keywords .= $keyword->name.', ';
            $keywords .= '<a href="' . url('taxonomy/term/' . $keyword->tid) . '">' . $keyword->name . '</a> , ';
        }
    }


    $form['project_info']['table']['tr2']['leftData']['td3'] = array(
        '#type' => 'item',
        '#value' => '<div class="SubHeading" style="float:left">' . $keywords . '</div>',
        '#prefix' => '<div><div class="SubHeading" style="float:left"><div>Keywords : </div></div>',
        '#suffix' => '</div><br clear="all" />',
    );

    if ($lib_project_Obj->published_on == '0000-00-00 00:00:00') {
        $published_status = ' Not Published Yet';
    } else {
        $published_status = date('m/d/Y', mktime($lib_project_Obj->published_on));
    }



    $form['project_info']['table']['tr2']['leftData']['td4'] = array(
        '#type' => 'item',
        '#value' => '<div class="SubHeading" style="float:left">' . $published_status . '</div>',
        '#prefix' => '<div><div class="SubHeading" style="float:left"><div>Published : </div></div>',
        '#suffix' => '</div><br clear="all" />',
    );

    $form['project_info']['table']['tr2']['rightData'] = array(
        '#type' => 'item',
        '#value' => '',
        '#prefix' => $HTMLform_activity_dataRight_hdr . '<table>',
        '#suffix' => '</table>' . $HTMLform_activity_dataRight_ftr,
    );


    $form['project_info']['table']['tr2']['rightData']['likethispost'] = array(
        '#type' => 'item',
        '#value' => flag_create_link('likethispost', $project_id),
        '#prefix' => '<tr><td>',
        '#suffix' => '</td></tr>',
//        '#suffix' => '</td></tr><tr><td><img src="' . $base_path . 'themes/Boldr/Images/common/img_comment.png" /></td><td><a onclick="set_library_project_comment(\'project_commantary\', \'' . $project_id . '\');" class="greenLink">Comment</a></td></tr>',
    );



    return $form;
//    }else {
//        drupal_set_message("this user doesn't have permission to view this page.");
//
//    }
}

function _get_ntlp_library_project_detail_contents($lib_project_nid) {
    return drupal_get_form('ntlp_library_project_detail_form', $lib_project_nid);
}

function ntlp_library_project_detail_form($form_state = null, $project_id) {
    include("activity_template.inc");
    global $base_path, $user;

//    if ($courseid == 0) {
//        //Getting Query String From URL
//        $courseid = 0;
//        $project_id = 0;
//        $url = $_GET['q'];
//        $args = explode('/', $url);
//        for ($i = 0; $i < count($args); $i++) {
//            if (is_numeric($args[$i]) && empty($courseid))
//                $courseid = $args[$i];
//            elseif (is_numeric($args[$i]) && empty($project_id))
//                $project_id = $args[$i];
//        }
//    }
//    $project_id = $courseid;

    $lib_project_Obj = _get_library_project($project_id);
    $projectObj = load_project_record($project_id);

    $form['project_name'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => $HTMLform_activity_outer_name_hdr,
        '#suffix' => $HTMLform_activity_outer_name_ftr,
    );

    $form['project_name']['name'] = array(
        '#type' => 'item',
//            '#value' =>  $projectObj->title,
        '#prefix' => "<table cellpadding='0' cellspacing='0' ><tr><td><div class='Heading'><b></b></div></td><td><div class='Heading'><h2>",
        '#suffix' => "</h2></div></td></tr></table>",
    );

    /*     * ********************* Start view  form *********************** */
    $form['project_details'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div id="edit_project_details_test">',
        '#suffix' => '</div>',
    );



    $form['project_details']['project_id'] = array(
        '#type' => 'hidden',
        '#value' => $projectObj->nid,
    );



//        '.drupal_get_form('ntlp_activeimage', $picture, 197, 827).'

    $image_path = get_project_image_path($projectObj->nid);

    $project_image = $image_path->filepath;
    if ($project_image == null || empty($project_image)) {

        if ($lib_project_Obj->is_exemplary == 1) {
            $project_image = $base_path . 'themes/Boldr/Images/default/exemplary_project_generic.jpg';
        } else {
            $project_image = $base_path . 'themes/Boldr/Images/default/shared_project_generic.jpg';
        }
    } else {

        $project_image = $base_path . $image_path->filepath;
    }



    $form['project_details']['proj_image'] = array(
        '#type' => 'item',
        '#value' => '<a href="#"><img src =' . $project_image . ' height ="150" width="150" /></a>',
        '#prefix' => $HTMLform_projectDetail_hdr . '<table cellpadding="8" class="table" cellspacing="0" border="0" width="820px"><tr><td valign="top" style="width:150px">',
        '#suffix' => '</td>',
    );


    $form['project_details']['proj_edit_button'] = array(
        '#type' => 'item',
        '#value' => t('Overview'),
        '#name' => t('Project Detail Edit'),
        '#attributes' => array('class' => 'GreenBorderBtn'),
        '#prefix' => '<td valign="top"><div ><div class="InnerHeading"><h4>',
        '#suffix' => '</h4></div></div><div class="clearall">',
    );

    $form['project_details']['proj_name'] = array(
        '#type' => 'item',
        '#size' => '40',
//            '#value' =>  $projectObj->title,
//            '#prefix' => '<div class="ProjectDetail-Table"><table cellpadding="8" class="table" cellspacing="0" border="0" width="100%"><tr id="proj_name" ><th valign="top" style="width:80px;"> '.t('Name').'</th><td style="border-top:1px solid #9ecce9;border-collapse:collapse;width:160px">',
        '#prefix' => '<div class="ProjectDetail-Table"><table cellpadding="8" class="table" cellspacing="0" border="0" width="100%">',
//            '#suffix' =>'</td>',
    );

    if ($projectObj->availability == 'S') {
        $availability = 'Show';
    } else {
        $availability = 'Hide';
    }

//    $form['project_details']['proj_availability'] = array(
//            '#type' => 'item',
//            '#size' => '40',
//            '#value' =>   $availability,
//            '#prefix' => '<th valign="top" style="width:80px;"> '.t('Availability').'</th><td style="border-top:1px solid #9ecce9;border-collapse:collapse;width:160px">',
//            '#suffix' =>'</td></tr>',
//    );

    $form['project_details']['proj_unit'] = array(
        '#type' => 'item',
        '#size' => '40',
        '#value' => $projectObj->course_short_title,
        '#prefix' => '<tr><th valign="top" class="whiteBorder"> ' . t('Topic/Unit') . '</th><td>',
        '#suffix' => '</td>',
    );

    $dateformat = "D, m/d/Y G:i:s";

    $duedate = date($dateformat, strtotime($activityObj->due_date));

    $form['project_details']['proj_duration'] = array(
        '#type' => 'item',
        '#size' => '40',
        '#value' => $projectObj->from_date . ' - ' . $projectObj->to_date,
        '#prefix' => '<th valign="top" class="whiteBorder">' . t('Duration') . '</th><td>',
        '#suffix' => '</td></tr>',
    );


    $form['project_details']['proj_summary'] = array(
        '#type' => 'item',
        '#size' => '40',
        '#value' => $projectObj->body,
        '#prefix' => '<tr id="proj_summary" ><th valign="top" height="100" class="whiteBorder">' . t('Summary') . '</th><td colspan="3">',
        '#suffix' => '</td></tr>	</table></div></td></tr></table>' . $HTMLform_projectDetail_ftr,
    );

    /*     * *****************************End Edit form ********************************** */


    $form['project_question'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div id="project_question_test">' . $HTMLform_grading0ption_outer_title_hdr,
        '#suffix' => $HTMLform_grading0ption_outer_title_ftr . '</div>',
    );

    $form['project_question']['project_question_Heading'] = array(
        '#type' => 'item',
        '#value' => t('Driving Question'),
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<table cellpadding="0" width="820px" cellspacing="0" border="0"><tr><td><div class="Content"><div class="DrivingQuestion-Heading"><h3>',
        '#suffix' => '</h3></div><br clear="all" />',
    );


    $form['project_question']['proj_quest'] = array(
        '#type' => 'item',
        '#size' => '40',
        '#value' => $projectObj->driving_question,
        '#prefix' => '<div class="DrivingQuestion-ContentMainDiv">',
        '#suffix' => '</div></div></td></table>',
    );

    $form['project_question']['project_id'] = array(
        '#type' => 'hidden',
        '#value' => $project_id,
    );


    $form['grade'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div id="grade_test">' . $HTMLform_grading0ption2_inner_title_hdr,
        '#suffix' => $HTMLform_grading0ption2_inner_title_ftr . '</div>',
    );

    $form['grade']['project_id'] = array(
        '#type' => 'hidden',
        '#value' => $project_id,
    );
    $form['grade']['heading'] = array(
        '#type' => 'item',
        '#value' => t('Learning Outcomes'),
        '#prefix' => '<table cellpadding="0" cellspacing="0" width="820px" border="0" ><tr><td><div class="LearningOutcomes-Content"><div class="Content"><div class="LearningOutcomes-Heading"><h3>',
        '#suffix' => '</h3></div>',
    );




    $form['grade']['points'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div class="LearningOutcomes-ContentMainDiv">',
        '#suffix' => '</div>',
    );


    $outcomes = $lib_project_Obj->outcomes;

    $outcome_array = explode(',', $outcomes);
//    print_r($outcome_array);

    for ($i = 0; $i < sizeof($outcome_array); $i++) {

        if (!empty($outcome_array[$i])) {

            $img = '<img src="' . $base_path . 'themes/Boldr/Images/ProjectsImages/TickIcon.jpg" alt="TickIcon"  />';
            $form['grade']['points']['outcome_' . $outcome_array[$i]] = array(
                '#type' => 'item',
                '#size' => '40',
                '#value' => '',
                '#prefix' => "<div class='RepeatDiv'><div class='Icon'>" . $img . "</div><div class='text'>" . $outcome_array[$i],
                '#suffix' => "</div></div>",
            );
        }
    }


    $form['grade']['grade_edit_button'] = array(
        '#type' => 'item',
        '#value' => t(''),
        '#prefix' => '</div><div style="float:right">',
        '#suffix' => '</div></div></td></tr></table>',
    );




    $form['academic_standards'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div id="academic_standards_test">' . $HTMLform_grading0ption_Academic_title_hdr,
        '#suffix' => $HTMLform_grading0ption_Academic_title_ftr . '</div>',
    );

    $form['academic_standards']['project_id'] = array(
        '#type' => 'hidden',
        '#value' => $projectnid,
    );
    $form['academic_standards']['Heading'] = array(
        '#type' => 'item',
//                '#value' =>  $projectObj->academic_standards,
        '#prefix' => '<table cellpadding="0" width="820px" cellspacing="0" border="0"><tr><td><div class="AcademicStandardsCovered-Content"><div class="Content"><div class="AcademicStandardsCovered-Heading"><h3>' . t('Academic Standards Covered'),
        '#suffix' => '</h3></div>',
    );



    $form['academic_standards']['proj_standars'] = array(
        '#type' => 'item',
        '#value' => $projectObj->academic_standards,
        '#prefix' => '<div class="AcademicStandardsCovered-ContentMainDiv"><div class="RepeatDiv"><div class="Text">',
        '#suffix' => '</div></div></div></div>',
    );


    $form['academic_standards']['academic_edit_button'] = array(
        '#type' => 'item',
        '#value' => t(''),
        '#prefix' => '<div style="float:right">',
        '#suffix' => '</div></div></td></tr></table>',
    );



    include('briefcase_template.inc');
    $project_section_arr = get_project_section($project_id);

    $form['briefcase'] = array(
        '#type' => 'fieldset',
//            '#tree' => TRUE,
        '#prefix' => '<div id="new-row-wrapper">' . $HTML_Briefcase_start,
        '#suffix' => $HTML_Briefcase_end . '</div>',
    );



    $form['briefcase']['section_' . $project_section_arr[0]->tid] = array(
        '#type' => 'item',
        '#value' => $project_section_arr[0]->name,
        '#prefix' => $HTML_Briefcase_section1_start,
        '#suffix' => $HTML_Briefcase_section1_end,
    );

//    $view = views_get_view('ntlp_project_section_resources');
//    $view->set_arguments(array($project_id, $project_section_arr[0]->tid));

    $first_view = views_get_view('ntlp_project_section_resources');
//echo $project_id.' '.$project_section_arr[0]->tid;
    $first_view->set_arguments(array($project_id, $project_section_arr[0]->tid));


    $first_view->resource_edit_mode = false;
    $first_view->resource_teacher_section = 0;


    $form['briefcase']['view_' . $project_section_arr[0]->tid] = array(
        '#type' => 'item',
        '#value' => $first_view->preview(),
        '#prefix' => $HTML_Briefcase_view1_start,
        '#suffix' => $HTML_Briefcase_view1_end,
    );

    for ($i = 1; $i < sizeof($project_section_arr); $i++) {

        $form['briefcase']['section_' . $project_section_arr[$i]->tid] = array(
            '#type' => 'item',
            '#value' => $project_section_arr[$i]->name,
            '#prefix' => $HTML_Briefcase_section_Box_start,
            '#suffix' => $HTML_Briefcase_section_Box_end,
        );

        $view = views_get_view('ntlp_project_section_resources');
        $view->set_arguments(array($project_id, $project_section_arr[$i]->tid));
        $view->resource_edit_mode = false;
        $view->resource_teacher_section = 0;
//$first_view->set_arguments(array($courseid,$project_section_arr[0]->tid, 'S'));
        $form['briefcase']['view_' . $project_section_arr[$i]->tid] = array(
            '#type' => 'item',
            '#value' => $view->preview(),
            '#prefix' => $HTML_Briefcase_section_Boxview_start,
            '#suffix' => $HTML_Briefcase_section_Boxview_end,
        );
    }

    return $form;
}

function _get_user_info($uid) {
    $result = db_query("SELECT * FROM {ntlp_user} WHERE uid = %d", $uid);

    if ($result) {
        return db_fetch_object($result);
    } else {
        return false;
    }
}

function ntlp_library_project_recent_comments_block() {
    
}

function ntlp_library_all_projects_search_block_content() {
    $output = '';
    $keywords = _get_project_library_keywords(get_this_school(), 3);

    $subjects = get_course_subject();
    $selectBoxCategory = '<select onchange="javascript:if(this.value != 0){window.location = \'?q=ntlp/library/project/search&subject=\'+this.value;}">';
    $selectBoxCategory .= "<option value=\"0\"></option>";
    foreach ($subjects as $tid => $name) {
        $selectBoxCategory .= "<option value=\"$tid\">$name</option>";
    }
    $selectBoxCategory .= '</select>';
//'ntlp/library/project

    foreach ($keywords as $keyword) {

        $example_subject .= '<a class="greenLink" href="' . url('ntlp/library/project/search', array('query' => array('subject' => $keyword->tid))) . '">' . $keyword->name . '</a>, ';
    }

    $example_subject = rtrim(trim($example_subject), ',');

    $output = '<div id="project_add_test"><div class="network_blue_border_middle_right" style="width:614px">
<div class="network_blue_border_middle_left">
<div class="network_blue_border_bottom_center">
<div class="network_blue_border_top_center">
<div class="network_blue_border_top_left">
                    <div class="network_blue_border_top_right">
                    <div class="network_blue_border_bottom_left">
                    <div class="network_blue_border_bottom_right"><div>
                    <div>
                      <table>
                        <tr>
                            <td class="view-filters">
                                <input type="text" class="form-text" id="search_field" style="width:500px" title="What are you looking for?" 
                                onkeydown="if (event.keyCode == 13) {
                                    if(!document.getElementById(\'search_field\').value == \'\'){
                                        window.location =\'?q=ntlp/library/project/search&keys=\'+document.getElementById(\'search_field\').value;
                                       }
                                    }" />
                            </td>
                            <td>
                                <input value="Search"  type="button" class="SearchBtn" 
                                    onclick="
                                    if(document.getElementById(\'search_field\').value == \'\'  || document.getElementById(\'search_field\').value == \'What are you looking for?\'){
                                            //do nothing..
                                        }else{
                                            window.location = \'?q=ntlp/library/project/search&keys=\'+document.getElementById(\'search_field\').value; 
                                          }"
                                        />
                                        
                            </td>
                        </tr>
                    </table>
                        <div style="clear: both;"></div>
                        <div style="font-size:2px">&nbsp;</div>
                        <!--
                        <div>
                            <span style="color:#3570AA;font-weight:bold">Example:</span>
                           ' . $example_subject . '
                        </div>
                        //-->
                    </div>
                    <div style="font-size:10px">&nbsp;</div>
                    <div>
                        <div><span style="color:#3570AA;font-weight:bold">Browse by Subject:</span> ' . $selectBoxCategory . '</div>
                    </div>
              </div></div></div></div></div></div></div></div></div></div><div style="font-size:10px">&nbsp;</div>';

    return $output;
}

function ntlp_project_library_recent_comments() {
    global $base_path;
    include 'comment.template.inc';

    $output = $HTMLRecentCommentWrapper;

    $temp = '';
    if ($comments = _get_recent_comments()) {
        foreach ($comments as $comment) {
            $user_pic = $base_path . ($comment->picture ? $comment->picture : 'themes/Boldr/Images/common/small_person_image_not_found.jpg');

            $temp .= $HTMLRecentComment;
            $temp = str_replace('@PROJECT_TITLE', l($comment->title, 'ntlp/library/project/view/' . $comment->nid), $temp);
            $temp = str_replace('@COMMENT', $comment->comment, $temp);
            $temp = str_replace('@USER_PICTURE', $user_pic, $temp);
            $temp = str_replace('@USER_NAME', l($comment->first_name . ' ' . $comment->last_name, 'ntlp/profile/user/view/' . $comment->comment_by), $temp);
        }
    }
    $output = str_replace('@COMMENTS_PLACEHOLDER', $temp, $HTMLRecentCommentWrapper);
    return $output;
}

function ntlp_library_project_activities() {
    include "briefcase_template.inc";

    if ($courseid == 0) {
        //Getting Query String From URL
        $courseid = 0;
        $project_id = 0;
        $url = $_GET['q'];
        $args = explode('/', $url);
        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i]) && empty($courseid))
                $courseid = $args[$i];
            elseif (is_numeric($args[$i]) && empty($project_id))
                $project_id = $args[$i];
        }
    }



    $container = $HTMLLibProjectActivities;

    $view = views_get_view('ntlp_activities_plib');
//    echo $courseid;
    $container = str_replace('@ACTIVITY_TABLE_PLACEHOLDER', $view->preview('default', array($courseid)), $container);
    return $container;
}

function ntlp_project_library_form($form_state = null, $project_id) {
    drupal_add_js(drupal_get_path('module', 'ntlp_library') . '/ntlp_library.js');

    if (isset($_GET['sub'])) {
        $subpage = $_GET['sub'];
    } else {
        $subpage = 'pd';
    }
    
    $project_id = intval($project_id);
    $lib_project_Obj = _get_library_project($project_id);
    
    if($lib_project_Obj->is_deleted == 1){
        drupal_goto('ntlp/render/error/page/Project is no longer available.', array('destination' => $_GET['q']));
    }

    set_item_url_handler('Project Setting');

    $form['library_project'] = array(
        '#type' => 'fieldset',
        '#title' => '',
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => '<div id="wrapper-project-commentary">',
        '#suffix' => '</div>',
    );

    $form['library_project']['project_info'] = array(
        '#type' => 'item',
        '#value' => _get_ntlp_library_project_header($lib_project_Obj, $subpage),
        '#prefix' => '',
        '#suffix' => '',
    );

    $form['library_project']['project_id'] = array(
        '#type' => 'hidden',
        '#value' => $project_id,
        '#id' => 'project_id',
    );
    if (!isset($_GET['res'])) {
        if ($subpage == 'pd') {
            $form['library_project']['project_detail'] = array(
                '#type' => 'item',
                '#value' => get_lib_project_sections_data($lib_project_Obj),
                '#prefix' => '<div id="project_detail_content"> ',
                '#suffix' => '',
            );

            $form['library_project']['outcome'] = array(
                '#type' => 'item',
                '#value' => get_lib_project_learning_outcomes_block($project_id),
                '#suffix' => '</div>',
            );


            $form['library_project']['academic_stadards'] = array(
                '#type' => 'item',
                '#value' => get_lib_project_academic_standards_block($lib_project_Obj->academic_standards),
                '#prefix' => '',
            );
        } else {
            $form['library_project']['commentary'] = array(
                '#type' => 'item',
                '#value' => _get_ntlp_library_project_commentary_contents($project_id),
                '#prefix' => '',
            );
        }
    } else {
        $resource_nid = $_GET['res'];
        $resource_type = _get_project_resource_type($resource_nid);
        if ($resource_type == 'Activity') {
            $form['library_project']['project_detail'] = array(
                '#type' => 'item',
                '#value' => get_lib_project_activity_view($resource_nid, $project_id),
                '#prefix' => '<div id="project_detail_content"> ',
                '#suffix' => '',
            );
        } else {
            $form['library_project']['project_detail'] = array(
                '#type' => 'item',
                '#value' => render_library_project_resource_view($resource_nid, $project_id),
                '#prefix' => '<div id="project_detail_content"> ',
                '#suffix' => '',
            );
        }
    }




    return $form;
}

function get_lib_project_sections_data($lib_project_Obj) {
    global $base_path, $user;
    include("library_project_template.inc");

    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/js/jquery.tipsy.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_library') . '/ntlp_library.js');

    $project_sections_panel = $HTML_Lib_Project_brieface_section_block;

    $project_section_arr = get_project_section($lib_project_Obj->nid);

    if ($project_section_arr != null) {

        foreach ($project_section_arr as $section) {

            $section_resource_count = 0;

            $project_resources = '';

            $section_resource_records = _get_project_section_resources($lib_project_Obj->nid, $section->tid);

            if ($section_resource_records) {

                $resource_count = 1;

                foreach ($section_resource_records as $resource) {

                    $hideChecked = '';
                    $option = '';
                    $restype = ($resource->restype == 'R') ? 'resource' : 'activity';

                    //this variable is used to set hidden field to find the section resources count
                    $section_resource_count += 1;

                    if (strlen($resource->title) > 40) {
                        $resource_title = substr($resource->title, 0, 40) . '...';
                    } else {
                        $resource_title = $resource->title;
                    }
                    if (empty($resource->attachment_name)) {
                        $resource_path = $resource->filepath;
                    } else {
                        $resource_path = $resource->attachment_name;
                    }
                    $resource_entry = $HTML_Lib_Project_briefcase_section_resource_entry;
                    $resource_entry = str_replace('@RESOURCE_TYPE_IMAGE_PATH', _get_resource_type_icons($resource->type, $resource_path, $resource->doc_type), $resource_entry);


                    if (strlen($resource->body) > 700) {
                        $resource_body = substr($resource->body, 0, 700) . '...';
                    } else {
                        $resource_body = $resource->body;
                    }

                    $resource_entry = str_replace('@RESOURCE_ID', $resource->nid, $resource_entry);
                    $resource_onclick_action = "window.location = '?q=ntlp/library/project/view/" . $lib_project_Obj->nid . "&res=" . $resource->nid . "'";
                    if ($resource->type == 'lo_link') {
                        $resource_entry = str_replace('@RESOURCE_NAME', '<a class="green_link res' . $resource->nid . '" title="' . strip_tags($resource_body) . '" href="' . get_link_resource_url($resource->nid) . '" target="blank" >' . $resource_title . '</a>', $resource_entry);
                    } else {
                        $resource_description = nl2br($resource_body);
                        $resource_description = preg_replace("%\n%", "", $resource_description);
                        $resource_description = preg_replace("%\r%", "", $resource_description);
                        $resource_description = htmlentities($resource_description, ENT_QUOTES);

                        $resource_entry = str_replace('@TOOLTIP_TEXT', $resource_description, $resource_entry);

                        $resource_entry = str_replace('@RESOURCE_NAME', l($resource_title, 'ntlp/courses/' . $restype . '/view/' . $course_nid . '/' . $resource->nid, array("attributes" => array('class' => 'green_link res' . $resource->nid))), $resource_entry);
                    }


                    $project_resources .= $resource_entry;

                    $resource_count++;
                }
            }

            $project_section = $HTML_Lib_Project_briefcase_section;


            $project_section = str_replace('@CATEGORYID', $section->tid, $project_section);
            $project_section = str_replace('@CATEGORY_TITLE', $section->name, $project_section);

            $project_section = str_replace('@CATEGORY_RESOURCES_ENTRIES', $project_resources, $project_section);

            $html_section .= $project_section;
        }
        $duration_days = count_days($lib_project_Obj->from_date, $lib_project_Obj->to_date);
        $project_sections_panel = str_replace('@PROJECT_IMAGE', get_ntlp_project_library_image_path($lib_project_Obj->picture_fid, $lib_project_Obj->is_exemplary), $project_sections_panel);
        $project_sections_panel = str_replace('@PROJECT_TITLE', $lib_project_Obj->title, $project_sections_panel);
        $project_sections_panel = str_replace('PROJECTCT_DURATION_DAYS', (($duration_days > 1) ? $duration_days . ' days' : $duration_days . ' day'), $project_sections_panel);
        $project_sections_panel = str_replace('@PROJECT_DRIVING_QUESTION', $lib_project_Obj->driving_question, $project_sections_panel);

        $project_sections_panel = str_replace('@CATEGORYIES', $html_section, $project_sections_panel);
    }
    return $project_sections_panel;
}

function get_lib_project_learning_outcomes_block($project_nid) {
    include("library_project_template.inc");

    $html = $HTML_Lib_Project_briefcase_learning_outcome_block;

    $result = get_project_outcome_names_and_activities($project_nid);

    $data_html = '';
    while ($data = db_fetch_object($result)) {
        $data_html .= '<tr class="">
                        <td style="border: medium none;"><span>' . $data->name . ' </span></td>
                      </tr>';
    }

    $html = str_replace("@LEARNING_OUTCOME", $data_html, $html);

    return ($data_html == '') ? $data_html : $html;
}

function get_lib_project_academic_standards_block($project_academic_standards) {
    include("library_project_template.inc");

    if ($project_academic_standards != null || $project_academic_standards = '') {
        $project_academic_standards_html = $HTML_Lib_Project_briefcase_project_academic_standards_block;

        $project_academic_standards_html = str_replace("@PROJECT_ACADEMIC_STANDARDS_TEXT", $project_academic_standards, $project_academic_standards_html);
    }

    return $project_academic_standards_html;
}

function ntlp_library_project_contents_data($content, $lib_project_nid) {
    $html = '';
    watchdog("proj_content", $content . ' li project nid ' . $lib_project_nid);
    if ($content == 'project_detail') {
        $html = _get_ntlp_library_project_detail_contents($lib_project_nid);
    } else {
        $html = '<br>';
        $html .= _get_ntlp_library_project_commentary_contents($lib_project_nid);
    }

    print $html;
}

function render_library_project_resource_view($resource_nid, $project_id) {

    drupal_add_js(drupal_get_path('module', 'ntlp_library') . '/ntlp_library.js');
    include("library_project_template.inc");
    global $base_path, $user;
    $root = $base_path;

    drupal_add_js('
            $(function(){
                    $("#content_table").attr("style" , "table-layout:fixed;");
                    });
        ', inline);

    if (!is_resource_exist($resource_nid)) {

        $resource = get_resource_detail($resource_nid);
        $user_data = get_user_data($resource->uid);
        $content = $HTMLContentDocument;
        switch ($resource->type) {
            case "lo_document":
                $content = str_replace("@IMG_SRC", 'img_document_big.png', $content);
                if ($resource->resource_type == 'U') {
                    $file_path = explode("_", $resource->filepath);
                    $file_name = $file_path[sizeof($file_path) - 1];
                    $content = str_replace("@RESOURCE_VIEW", '<img src="' . $root . get_resource_file_icon($file_name) . '"><a href="' . $root . $resource->filepath . '" target="_blank" >' . $file_name . '</a>', $content);
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
                }

                $image = '<p align=center><img  src="' . $image . '" style=" max-width:800px; width:auto !important; width:800px;  max-height:600px; height:auto !important; height:600px;"/></p>';
                $content = str_replace("@RESOURCE_VIEW", $image, $content);
                break;
            case "lo_page":
                //$content = $HTMLContentPage;
                $content = str_replace("@IMG_SRC", 'img_page_big.png', $content);
                $content = str_replace("@RESOURCE_VIEW", '<div style="word-wrap:break-word;">' . $resource->path . '</div>', $content);
                //$content = str_replace("@RESOURCE_DESC", '<div>'.$resource->body.'</div>', $content);
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
                }
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
        $content = str_replace("@POSTED_BY_NAME", l($user_data->first_name . ' ' . $user_data->last_name, 'ntlp/user/profile/' . $user_data->uid, array('attributes' => array('style' => 'color: #009900;'))), $content);

        $content = str_replace("@RESOURCE_TITLE", $resource->title, $content);

        $content = str_replace("@DATEDIFF", ago($resource->changed), $content);
        $content = str_replace("@ROOT", $root, $content);

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


        if ($resource->type == 'lo_page') {
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
        }


        //Display edit button based on permissions
        if (isset($user->roles[NTLP_ROLEID_NETWORKMANAGER]) || isset($user->roles[NTLP_ROLEID_NETWORKSTAFF])) {

            $resouce_manage_options = $HTMLresource_manage_options;

            $resouce_manage_options = str_replace('@MANAGE_RESOURCE_EDIT', l('Edit Resource', 'ntlp/library/projectresource/edit/' . $resource->course_nid . '/' . $resource->nid), $resouce_manage_options);
            $resouce_manage_options = str_replace('@MANAGE_RESOURCE_DELETE', dlg('Delete Resource', 'ntlp/library/libprojectresource/delete/resource/' . $project_id . '/' . $resource->nid, 500, 500), $resouce_manage_options);

            $content = str_replace("@EDIT", $resouce_manage_options, $content);
        } else {
            $content = str_replace("@EDIT", "", $content);
        }

        $content = str_replace("@BACK_LINK", '<a href="' . url('ntlp/library/project/view/' . $project_id) . '" class="green_link"> < ' . _get_node_title($project_id) . ' </a>', $content);

        return $content;
    } else {

        drupal_goto('ntlp/library/project/view/' . $project_id);
    }
}

function get_lib_project_activity_view($activity_nid, $project_nid) {
    global $user;
    include("library_project_template.inc");
    // Send the Modal Frame javascript for parent windows to the page.
    modalframe_parent_js();
    set_item_url_handler("Project Library Activity View");
    $activityObj = get_activity_records($activity_nid);

    // checking a activity submission type and display a message accordingly
    if ($activityObj->submission_type == 'O') {

        $activity_message = '<h2 style="font-weight: normal;" class="teacherComment">Work is completed offline. <b>No submission required.</b></h2>';
    } else {
        if ($activityObj->allow_resubmission == 0) {
            $activity_message = '<h2 style="font-weight: normal;" class="teacherComment">Work is submitted online. You can <b>only submit once</b>.</h2>';
        } else if ($activityObj->allow_resubmission == 1) {
            $activity_message = '<h2 style="font-weight: normal;" class="teacherComment">Work is submitted online. You can <b>submit multiple times</b>.</h2>';
        }
    }

    if ($activityObj->type != 'lo_journal') {
        $description_heading = "Activity Description:";
    } else {
        $description_heading = "Journal Prompt:";
    }

    if (empty($activityObj->body)) {
        $description_heading = '';
        $description_body = '';
    } else {
        $description_body = wordwrap($activityObj->body, 88, "\n", true);
    }


    // activity attched files
    $obj = get_attachment($activityObj->nid, 'ntlp_activity_attachment', 'activity_nid');

    if ($obj != null) {
//            $divdata = '<table>';
        foreach ($obj as $attchmentObj) {

            if ($attchmentObj->type == 'G') {
                $activity_attched_doc .= '<div id="G_' . $attchmentObj->id . '"> <a href=' . $attchmentObj->path . ' >' . $attchmentObj->path . '</a></div>';
            } elseif ($attchmentObj->type == 'L') {
                $activity_attched_doc .= '<div id="L_' . $attchmentObj->id . '"> <a href=' . $attchmentObj->path . ' >' . $attchmentObj->path . '</a></div>';
            } elseif ($attchmentObj->type == 'F') {

                $file_id = get_attachment_filepath($activity_nid, $attchmentObj->id);


                if ($file_obj = _get_dpl_files_obj($file_id->fid)) {
                    $picture = $file_obj->filepath;
                    $file_image = check_and_the_file_type_image($file_obj->filemime, $file_obj->filepath);
                }
                $activity_attched_doc .= '<li> <span><img src=' . $base_path . 'themes/Boldr/Images/resourceStuff/' . $file_image . '.png border="0" /><a href="' . $picture . '" target="_blank">' . $file_obj->filename . '</a></span> </li>';
            }
        }
    }

    if (isset($activity_attched_doc) && !empty($activity_attched_doc)) {
        $attachment = $HTML_ACTIVITY_ATTACHEMENT;
        $attachment = str_replace('@SUBMITTED_ATTACHEMENTS', $activity_attched_doc, $attachment);
    } else {
        $attachment = '';
    }

    // activity outcome
    if ($activityObj->is_graded != 0) { // if activity ungraded
        $outcome_array = get_lib_project_activity_outcome($activity_nid);
        if ($outcome_array != null) {
            $count = 1;
            foreach ($outcome_array as $outcome_obj) {

                $activity_outcome .= '<li>' . $outcome_obj->name . ' </li>';
                $count++;
            }
        }
    } else {
        $activity_outcome = 'This Activity is not Graded';
    }

    // activity standards
    if ($activityObj->curriculum_standards != '') {
        $standards = $HTML_ACTIVITY_STANDARDS;
        $standards = str_replace('@ACTIVITY_STANDARDS_DESC', wordwrap($activityObj->curriculum_standards, 40, "\n", true), $standards);
    } else {
        $standards = '';
    }


    $html_container = $HTML_activity_overview_panel;
    $html_container = str_replace("@BACK_LINK", '<a href="' . url('ntlp/library/project/view/' . $project_nid) . '" class="green_link"> < ' . _get_node_title($project_nid) . ' </a>', $html_container);
    $html_container = str_replace("@ACTIVITY_NAME", $activityObj->title, $html_container);
    $html_container = str_replace('@ACTIVITY_MESSAGE', $activity_message, $html_container);
    $html_container = str_replace('@ACTIVIITY_DESCRIPTION_HEADING', $description_heading, $html_container);
    $html_container = str_replace('@ACTIVITY_DESCRIPTION', $description_body, $html_container);
    $html_container = str_replace('@ATIVITY_ATTACHMENTS', $attachment, $html_container);
    $html_container = str_replace('@ACTIVITY_LEARNING_OUTCOMES', $activity_outcome, $html_container);
    $html_container = str_replace('@ACTIVITY_STANDARD', $standards, $html_container);

    //Display edit button based on permissions
    if (isset($user->roles[NTLP_ROLEID_NETWORKMANAGER]) || isset($user->roles[NTLP_ROLEID_NETWORKSTAFF])) {

        $project_combo = dlg('Delete Activity', 'ntlp/library/libprojectresource/delete/activity/' . $project_nid . '/' . $activity_nid, 450, 600, Activities_btn);

        $html_container = str_replace("@EDIT", $project_combo, $html_container);
    } else {
        $html_container = str_replace("@EDIT", "", $html_container);
    }


    return $html_container;
}

function get_library_project_image($nid) {
    $rs = db_query("SELECT * FROM dpl_ntlp_library_project WHERE nid = %d", $nid);

    return db_fetch_object($rs);
}
