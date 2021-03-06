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
module_load_include('inc', 'ntlp_library', 'data_access');
module_load_include('inc', 'ntlp_project', 'data_access');
module_load_include('inc', 'ntlp_school', 'data_access');
module_load_include('inc', 'ntlp_school', 'constants');

global $base_path, $user;
$root = $base_path.'themes/Boldr/Images/';

$lib_project_obj = _get_library_project($node->nid);
$user_obj = get_user($lib_project_obj->submitted_by);
$submitted_by = l($user_obj->first_name.' '.$user_obj->last_name, 'ntlp/user/profile/'.$lib_project_obj->submitted_by);

$project_title = l($title, 'ntlp/library/project/view/'.$node->nid);

$project_image = get_ntlp_project_library_image_path($lib_project_obj->picture_fid, $lib_project_obj->is_exemplary);


$objComment = _comment_load($lib_project_obj->reviewer_comment_cid);
$reviewer_comment = $objComment->comment;
$rvwr_obj = get_user($objComment->uid);
$reviewer_url = url('ntlp/user/profile/'.$rvwr_obj->uid);
if (empty($rvwr_obj->picture))
    $reviewer_picture = "<img src='".$root."common/person_image_not_found.jpg' alt='".$rvwr_obj->first_name.' '.$rvwr_obj->last_name."' height='32' width='32' />";
else 
    $reviewer_picture = "<img src='$base_path.$rvwr_obj->picture' alt='".$rvwr_obj->first_name.' '.$rvwr_obj->last_name."' height='32' width='32' />";
//    print '<p>Reviewer CID:'.$lib_project_obj->reviewer_comment_cid.'</p>';
//    print_r($objComment);

//Get Keywords and Subjects
$node_related_taxo = _get_library_projects_taxonomies($node->nid);
//    print '<p>Subject ID:'.NTLP_VOCABULARY_SUBJECTS.'</p>';
//    print_r($node_related_taxo);
foreach($node_related_taxo as $Obj) {

    if($Obj->vid == NTLP_VOCABULARY_SUBJECTS) {
        $subject_array[] = $Obj;
    }
    if($Obj->vid == NTLP_KEYWORDS_FOR_PROJECT_LIBRARY) {
        $keywords_array[] = $Obj;
    }
}
$project_subjects = "";
$project_keywords = "";
if($subject_array != null) {
    $i = 0;
    foreach($subject_array as $subject) {

        if(empty($subject_array[$i+1])) {
            $project_subjects .= '<a href="'.url('ntlp/library/project/search', array('query'=>array('subject'=>$subject->tid))).'">'.$subject->name.'</a>';
        }else {
            $project_subjects .= '<a href="'.url('ntlp/library/project/search', array('query'=>array('subject'=>$subject->tid))).'">'.$subject->name.'</a> | ';
            
        }

        $i++;
    }
}
if($keywords_array != null) {
    $i = 0;
    foreach($keywords_array as $keyword) {

        if(empty($keywords_array[$i+1])) {
            $project_keywords .= '<a href="'.url('ntlp/library/project/search', array('query'=>array('keyword'=>$keyword->tid))).'">'.$keyword->name.'</a> ';
        }else {
            $project_keywords .= '<a href="'.url('ntlp/library/project/search', array('query'=>array('keyword'=>$keyword->tid))).'">'.$keyword->name.'</a> | ';
        }

        $i++;
    }
}

$count_flags = flag_get_counts('node', $node->nid);
$count_likes = $count_flags['likethispost'];
$count_comments = comment_num_all($node->nid);
$count_copies = _get_project_copied_count($node->nid);

?>

<!--Share Stuff-->
 <div class="gap"></div>
<div id="AgendaShareStuff">


    <div style="clear:both"></div>
    <!-- Agenda ShareStuff Content -->
    <div id="AgendaShareStuffContent">
 <div class="gap"></div>
        <table cellpadding="0" cellspacing="0" border="0" width="100%">
                                    <tr>
                                            <td valign="top" style="width:125px">
                                                <div>
                                                    <?php if($lib_project_obj->is_exemplary): ?>
                                                    <img style="border:1px solid lightgray" src="<?php print $project_image?>" width="95" alt="Featured Project" />
                                                    <?php else: ?>
                                                    <img src="<?php print $project_image?>" width="95" alt="Shared Project" />
                                                    <?php endif; ?>
                                                </div>
                                                <div class="likesCommentsCopies">
                                                <ul>
                                                    <li>
                                                        <div class="img"><img src="<?php print $root.'common/img_copies.png' ?>" /></div>
                                                        <div class="text"><?php echo $count_copies; ?> Copies</div>
                                                    </li>
                                                    <li>
                                                        <div class="img"><img src="<?php print $root.'common/img_like.png' ?>" /></div>
                                                        <div class="text"><?php echo $count_likes; ?> Likes</div>
                                                    </li>
                                                    <li>
                                                        <div class="img"><img src="<?php print $root.'common/img_comment.png' ?>" /></div>
                                                        <div class="text"><?php echo $count_comments; ?> Comments</div>
                                                    </li>
                                                </ul>
                                                </div>
                                            </td>
                                        
                                        <td valign="top">

                                                    <!-- Agenda ShareStuff Heading -->
                                                    <div id="AgendaShareStuffHeading"  style="width:100%; background-color:#D0E6F4;display:table">
                                                        <?php if($lib_project_obj->is_exemplary): ?>
                                                            <div style="float:left;padding:15px 0 0 12px" class="moreExemplary">
                                                                <img src="<?php print $base_path?>themes/Boldr/Images/Icons/star_icon.png" height="15px" />
                                                            </div>
                                                        <?php endif; ?>
                                                        <h2 style="float:left;padding: 0 0 0 10px">
                                                            <?php print $project_title ?>
                                                        </h2>
                                                    </div>
                                                            <DIV class="subData">

                                                                <P><?php print $node->body ?></P>
                                                            </DIV>
                                                            <br clear="all" />
                                                       <DIV>
                                                            <div>
                                                                <div class="SubHeading" style="float:left; padding-right: 10px;">
                                                                    <div style="width:auto;">Course Subject(s): </div>
                                                                </div>
                                                                <div class="SubHeading" style="float:left">
                                                                    <?php print $project_subjects ?>
                                                                </div>
                                                            </div>
                                                            <br clear="all" />
                                                            <div>
                                                                <div class="SubHeading" style="float:left">
                                                                    <div>Keywords:</div>
                                                                </div>
                                                                <div class="SubHeading" style="float:left">
                                                                    <?php print $project_keywords ?>
                                                                </div>
                                                            </div>
                                                           <br clear="all" />
                                                            <div>
                                                                <div class="SubHeading" style="float:left">
                                                                    <div>Submitted By:</div>
                                                                </div>
                                                                <div class="SubHeading" style="float:left">
                                                                    <?php print $submitted_by ?>
                                                                </div>
                                                            </div>

                                                        </DIV>
                                          
                                        </td>

                                    </tr>
                                    <tr>
                                        <td colspan="2">
                                            
                                        </td>
                                    </tr>
                                </table>
                   
                    <div class="gap"></div>
    </div>
</div>