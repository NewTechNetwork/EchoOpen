<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


/*
  Available variables:

 * $title: the (sanitized) title of the node.
 * $content: Node body or teaser depending on $teaser flag.
 * $picture: The authors picture of the node output from theme_user_picture().
 * $date: Formatted creation date (use $created to reformat with format_date()).
 * $links: Themed links like "Read more", "Add new comment", etc. output from theme_links().
 * $name: Themed username of node author output from theme_username().
 * $node_url: Direct url of the current node.
 * $terms: the themed list of taxonomy term links output from theme_links().
 * $submitted: themed submission information output from theme_node_submitted().

  Other variables:

 * $node: Full node object. Contains data that may not be safe.
 * $type: Node type, i.e. story, page, blog, etc.
 * $comment_count: Number of comments attached to the node.
 * $uid: User ID of the node author.
 * $created: Time the node was published formatted in Unix timestamp.
 * $zebra: Outputs either "even" or "odd". Useful for zebra striping in teaser listings.
 * $id: Position of the node. Increments each time it's output.

  Node status variables:

 * $teaser: Flag for the teaser state.
 * $page: Flag for the full page state.
 * $promote: Flag for front page promotion state.
 * $sticky: Flags for sticky post setting.
 * $status: Flag for published status.
 * $comment: State of comment settings for the node.
 * $readmore: Flags true if the teaser content of the node cannot hold the main body content.
 * $is_front: Flags true when presented in the front page.
 * $logged_in: Flags true when the current user is a logged-in member.
 * $is_admin: Flags true when the current user is an administrator.
 */

drupal_add_js(drupal_get_path('module', 'ntlp_forum') . '/ntlp_forum.js');
$nodeUser = get_user_record($uid);
$root_url = url('', array('absolute' => TRUE));
set_item_url_handler('Topic View');
?>
<div>
    <?php
    //echo l('Ã‚Â« Back to Topics', 'ntlp/' . (arg(1) == 'courses' ? 'courses' : 'groups') . '/forum/' . arg(4), array('attributes' => array('class' => 'BoldrBrandCrumb')));
    if (arg(1) == 'groups') {

        $isGroups = 'groups';
    } else {
        $isGroups = 'courses';
    }

    $new_topic_url = url('ntlp/' . $isGroups . '/forum/comment/' . arg(4) . '/' . arg(5) . '/0');
    ?>
    <input type="hidden" id="course_nid" value="<?php print arg(4) ?>" >
    <input type="hidden" id="context" value="<?php print $isGroups ?>" >

</div>
<div id="dialog-show-confirmation" style="display: none;" title="Warning">
    <span class="msg">Are you sure you want to submit the form?</span>
</div>
<div style="float: right;">
    <div><a  style='color:#00b050;' href='<?php print $new_topic_url ?>' > + post new comment </a></div>
</div>

<div>
    <div style="width:700px;"><h1 style="color: #3570AA;"><?php echo $title; ?></h1></div>
    <?php
    if (arg(1) == 'groups') {

        $isGroups = 'groups';
    } else {
        $isGroups = 'courses';
    }

    if (user_access('delete comment')) {
        $delete_topic = "<a style='color:#00b050;'  onclick='topic_delete(" . arg(5) . ")' >delete</a>";
        $edit_topic = "<a style='color:#00b050;' href='" . url('ntlp/' . $isGroups . '/forum/topic/' . arg(4) . '/' . arg(5)) . "'> edit </a>";
        $topic_manage_link = $edit_topic . ' | ' . $delete_topic;
    } else if (is_node_owner(arg(5), $user)) {
        
        $delete_topic = "<a  onclick='topic_delete(" . arg(5) . ")' >Delete </a>";
        $edit_topic = "<a href='" . url('ntlp/' . $isGroups . '/forum/topic/' . arg(4) . '/' . arg(5)) . "'> Edit </a>";
        $topic_manage_link = $edit_topic . ' | ' . $delete_topic;
    }
    ?>

    <div class="discuss_view">
        <div class="bl">
            <div class="br">
                <div class="tl">
                    <div class="tr">
                        <div class="discuss_info">
                            <div class="discuss_image"><img src="<?php echo is_user_image_exist($nodeUser->picture) ?>" width ="32"  height ="32"/></div>
                            <div class="discuss_name">Posted By: <?php echo l($nodeUser->first_name . ' ' . $nodeUser->last_name, 'ntlp/user/profile/' . $nodeUser->uid, array('attributes' => array('style' => 'color: #00b050;'))) . "&nbsp;" ?></div>
                            <div class="discuss_date"><?php echo " on " . get_tz_course(arg(4), 'd/m/Y - h:i a', $node->created) ?></div>
                        </div>
                        <div class="discuss_action">
                            <div style="float: right;"> <?php print $topic_manage_link ?></div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
        <?php
        if($content == ''){
            $new_style='style="border:none !important;"';
        }
            ?>
        <div class="discuss_view_content" <?php echo $new_style; ?> >
            <?php echo $node->body ;?>
        </div>
    </div>
</div>
<h1>Comments</h1>
<?php $result = db_query('SELECT c.cid as cid, c.pid, c.nid, c.subject, c.comment, c.format, c.timestamp,
    c.name, c.mail, c.homepage, u.uid, nu.first_name, nu.last_name, u.name AS registered_name, u.signature,
    u.signature_format, u.picture, u.data, c.thread, c.status
    FROM {comments} c
    LEFT JOIN {users} u ON c.uid = u.uid
    LEFT JOIN {ntlp_user} nu ON nu.uid = u.uid
    WHERE c.nid = %d
    ORDER BY SUBSTRING(c.thread, 1, (LENGTH(c.thread) - 1))',
    $node->nid); ?>
            <div id="no-comments" <?php echo $result->num_rows ? 'style="display: none;"' : '' ?> >No Comments</div>

                                <?php
    $divs = 0;
    $comments = '';
    while ($commentObj = db_fetch_object($result)) {
        $comment = drupal_unpack($commentObj);
        $comment->name = $commentObj->uid ? $commentObj->registered_name : $commentObj->name;
        $comment->depth = count(explode('.', $commentObj->thread)) - 1;

        if ($comment->depth > $divs) {
            $divs++;
            $comments .= '<div class="indented">';
                                } else {
            while ($comment->depth < $divs) {
                $divs--;
                $comments .= '</div>';
                                }
                    }
//        echo 'CID:'. $commentObj->cid . ', depth ' . $comment->depth . ' divs  ' . $divs . '<br>';
        $comments .= ntlp_comment_render($commentObj, $node_nid, $isGroups);
    }
    while ($divs-- > 0) {
        $comments .= '</div>';
    }
    print $comments;
?>

