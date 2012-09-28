<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


// $Id: node.tpl.php,v 1.4.2.1 2009/08/10 10:48:33 goba Exp $

/**
 * @file node.tpl.php
 *
 * Theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: Node body or teaser depending on $teaser flag.
 * - $picture: The authors picture of the node output from
 *   theme_user_picture().
 * - $date: Formatted creation date (use $created to reformat with
 *   format_date()).
 * - $links: Themed links like "Read more", "Add new comment", etc. output
 *   from theme_links().
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct url of the current node.
 * - $terms: the themed list of taxonomy term links output from theme_links().
 * - $submitted: themed submission information output from
 *   theme_node_submitted().
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $teaser: Flag for the teaser state.
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 */

module_load_include('inc', 'ntlp_document_library', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');

global $base_path, $user;
$root = $base_path;
$dateformat = "F j, Y";
$module_obj = _get_module_detail('ntlp_network_resource_library');

drupal_add_js('themes/Boldr/flowplayer/flowplayer-3.1.4.min.js');
drupal_add_js(drupal_get_path('module', 'ntlp_school') .'/modalframe_setup.js');
modalframe_parent_js();

$lib_resource_detail = _get_library_resource_detail($node->nid);


if($categ_Obj = _get_categroy_by_resrouce_id($node->nid)) {
    $school_Obj = _get_school_by_doclib_category($categ_Obj->tid);
}

//if($node->type != 'lo_page') {
    $node_contents = $lib_resource_detail->body;
//}

$resouce_node = node_load($node->nid);
$node_related_taxo = taxonomy_node_get_terms($resouce_node);

//watchdog('resrouce_node', print_r($node, true));
$library_cate_Obj   = _get_lib_vocabulary_vid("NTLP Library Category");
//$ntlp_post_Obj      = _get_lib_vocabulary_vid("NTLP Posts");

foreach($node_related_taxo as $Obj) {
    if($Obj->vid == $library_cate_Obj->vid) {
        $category_array[] = $Obj;
    }
    if($Obj->vid == NTLP_KEYWORDS_FOR_NETWORK_RESOURCE_LIBRARY) {
        $keywords_array[] = $Obj;
    }
}

if($category_array != null) {
    $i = 0;
    foreach($category_array as $subject) {

        if(empty($category_array[$i+1])) {
            $project_subjects .= '<a href="'.url('ntlp/resource/library/search/'.$subject->tid).'">'.str_replace('%2C', ',', $subject->name).'</a>';
        }else {
            $project_subjects .= '<a href="'.url('ntlp/resource/library/search/'.$subject->tid).'">'.str_replace('%2C', ',', $subject->name).'</a>, &nbsp;';

        }
        $i++;
//        $project_subjects .= '<a href="'.url('ntlp/resource/library/search', array( 'query' => array('subject' => $subject->tid))).'">'.$subject->name.'</a> , ';
    }
}
if($keywords_array != null) {
    $i = 0;
    foreach($keywords_array as $keyword) {

        if(empty($keywords_array[$i+1])) {
            $project_keywords .= '<a href="'.url('ntlp/resource/library/search/'.$keyword->tid).'">'.$keyword->name.'</a>';
        }else {
            $project_keywords .= '<a href="'.url('ntlp/resource/library/search/'.$keyword->tid).'">'.$keyword->name.'</a>, &nbsp;';

        }
        $i++;
//        $project_keywords .= '<a href="'.url('ntlp/resource/library/search', array( 'query' => array('subject' => $keyword->tid))).'">'.$keyword->name.'</a> , ';
    }
}
if(isset($module_obj)) {
    $edit_link = '<a href="'.url('ntlp/resource/library/edit/resource/'.$node->nid).'">Edit</a>';
    $delete_link =  dlg('Delete', 'ntlp/document/library/resource/delete/popup/'.$node->nid,450, 600);
}else {
    $edit_link = '';
    $delete_link = '';
}

if($categ_Obj != false) {
    $resource_title = l($node->title, 'ntlp/resource/library/view/'.$node->nid.'/'.$categ_Obj->tid);
}else {
    $resource_title = $node->title;
}

switch ($node->type) {
    case 'lo_link':
        $display_attachment = resoruce_attachment_data($node->type, $lib_resource_detail);
        break;
    case 'lo_image':
        $display_attachment =  '<img src="'.resoruce_attachment_data($node->type, $lib_resource_detail) .'" width="115" alt="Featured Project" />';
        break;
    case 'lo_document':
        $display_attachment = '<div>'.resoruce_attachment_data($node->type, $lib_resource_detail).'</div>';
        break;
    case 'lo_video':
        if( $display_attachment = resoruce_attachment_data($node->type, $lib_resource_detail) ) {
            drupal_add_js("flowplayer(\"player{$node->nid}\", \"{$base_path}themes/Boldr/flowplayer/flowplayer-3.1.5.swf\");",'inline','footer');
            $display_attachment = "<table cellpadding=\"0\" cellspacing=\"0\" border=\"0\" width=\"100%\"><tr><td class=\"Flash\">
                                    <div id=\"page\"><a href=\"$display_attachment\" class=\"Boldrplayer\"
                                    id=\"player{$node->nid}\"><img src=\"{$base_path}themes/Boldr/Images/flashImg.jpg\" alt=\"Image\" width=\"200\" /></a></div>
                                    </td></tr></table>";
        }
        break;
    case 'lo_page':
        $display_attachment = '<img width="115" src="'.$base_path.'themes/Boldr/Images/loginpage_logo.jpg" />';
        break;
}


?>
<!--Share Stuff-->
<table width="100%" cellpadding="5" style="border-top: 1px solid #D6E9F5;">
    <tr>
<!--        <td style="width:120px" rowspan="2">
        <?php print $display_attachment ?>
        </td>
        -->
        <td>
            <div style="float:left;font-size: 15px;color:#090;font-weight: bold">
                <?php print $resource_title ?>
                <?php if($node->promote):?>
                <img src="<?php print $base_url ?>themes/Boldr/Images/ProjectsImages/exemplary_project.png" width="25" />
                <?php endif ?>
            </div>
            <!--            <div class="greenLink" style="float:right">
            <?php print $edit_link .' | '. $delete_link?>
                        </div>
            -->
        </td>
    </tr>
    <tr>
        <td>
            <table width="100%" align="left">
                <tr>
                    <td valign="top" style="width:100px">
                        <b style="color:#3570aa">Categories:</b>
                    </td>
                    <td class="greenLink" style="width:85%">
                        <?php print $project_subjects ?>
                    </td>
                </tr>
                <tr>
                    <td valign="top">
                        <b style="color:#3570aa">Keywords:</b>
                    </td>
                    <td class="greenLink">
                        <?php print $project_keywords ?>
                    </td>
                </tr>
                <tr>
                    <td valign="top">
                        <b style="color:#3570aa">Submitted:</b>
                    </td>
                    <td>
                        <?php print date($dateformat, $node->created); ?>
                    </td>
                </tr>
                <?php if(!empty($node_contents)): ?>
                <tr>
                    <td colspan="2">
                        <div class="GroupsDiscriptionTopRightCorner">
                            <div class="GroupsDiscriptionTopLeftCorner">
                                <div class="GroupsDiscriptionBottomRightCorner">
                                    <div class="GroupsDiscriptionBottomLeftCorner" style="padding:5px">
                                        <?php print $node_contents; ?>
                                    </div>
                                </div>
                            </div>
                        </div>
                    </td>
                </tr>
                <?php endif ?>
            </table>
        </td>
    </tr>
</table>
<br clear="all" />
