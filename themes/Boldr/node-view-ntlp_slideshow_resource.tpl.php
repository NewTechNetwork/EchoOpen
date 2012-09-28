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
module_load_include('inc', 'ntlp_course', 'data_access');
module_load_include('inc', 'ntlp_school', 'data_access');

global $base_path;
global $user;
$root = $base_path;

$resource = get_resource_info($node);
$content = '';
if($resource->course_nid  != null) {
    switch ($resource->type) {
        case "lo_document":
            if($resource->dtype == 'U') {
                $resrouce_obj = get_attahced_file_detail($resource->fid);
                $content = '<a href="'.$root.$resrouce_obj->filepath.'" target="_blank" >'.$resource->path.'</a>';
            }else {
                if(!empty($resource->path) && $resource->path != '') {
                    $content = '<a href="'.$resource->path.'" target="_blank" >Google Document</a>';
                }else {
                    $content = 'FILE NOT FOUND';
                }
            }
            break;
        case "lo_link":
            $content = l('Click to Open Link', $resource->path);
            break;
        case "lo_image":
            if($resource->dtype == 'U') {
                $resrouce_obj = get_attahced_file_detail($resource->fid);
                $content = '<img width="100%" src="'.$root.$resrouce_obj->filepath.'" />';
            }else {
                $content = '<img width="98px" src="'.$resource->path.'" />';
            }
            break;
        case "lo_page":
            $content = '<div>'.$resource->path.'</div>';
            break;
        case "lo_video":
            if($resource->dtype == 'U') {
                $resrouce_obj = get_attahced_file_detail($resource->fid);

                $content = '<script type="text/javascript" src="'.$root.'themes/Boldr/flowplayer/flowplayer-3.1.4.min.js"></script>
                    <script type="text/javascript">
                    flowplayer("player", "'.$root.'themes/Boldr/flowplayer/flowplayer-3.1.5.swf");
                    </script>

                    <table cellpadding="0" cellspacing="0" border="0" width="100%"><tr><td class="Flash">
                    <div id="page"><a href="'.$root.$resrouce_obj->filepath.'" class="Boldrplayer"
                    id="player"><img src="'.$root.'themes/Boldr/Images/flashImg.jpg" alt="Image" height="146" width="230" /></a></div>
                    </td></tr></table>';

            }else {
                $content = '<script type="text/javascript" src="'.$root.'themes/Boldr/flowplayer/flowplayer-3.1.4.min.js"></script>
                    <script type="text/javascript">
                    flowplayer("player", "'.$root.'themes/Boldr/flowplayer/flowplayer-3.1.5.swf");
                    </script>

                    <table cellpadding="0" cellspacing="0" border="0" width="100%"><tr><td class="Flash">
                    <div id="page"><a href="'.$resource->path.'" class="Boldrplayer"
                    id="player"><img src="'.$root.'themes/Boldr/Images/flashImg.jpg" alt="Image" height="146" width="230" /></a></div>
                    </td></tr></table>';
            }
            break;
        case "lo_share_idea":
            $content = "";
            break;
        default:
            $content = "Unknown content";
            break;
    }
}
?>
<div class="HomePageFeatures" style="margin:7px 0 0 0">
<div id="AgendaActivitiesAndNotisfication">
	<div class="TopRightOrangeCorner">
		<div class="TopLeftOrangeCorner">
			<div class="AgendaActivitiesAndNotisfication">
				<div class="AgendaIconDiv"><img src="<?php print $root ?>themes/Boldr/Images/orange_box_feature_Icon.jpg" alt="Features" /> </div>
				<div class="AgendaOrangeBoxHeading">
					<h2>Features</h2>
				</div>
			</div>
		</div>
	</div>
	<div class="NoticficationMainContent">
		<table cellpadding="0" cellspacing="0" border="0" width="100%">
			<tr>
                            <td class="MiddleLeftOrangeCorner"></td>
                            <td class="MiddleCenterOrangeCorner">
                            <div class="Feature">
                                <div class="FeatureFlashPanel" style="height:163px" align="center">
                                   <?php print $content ?>
                                </div>
                                <div class="clearAll"></div>
                                <div class="FeatureFlashPanelContent">
                                    <div class="FeatureFlashPanelHeading"><h6>
                                        <a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a>
                                        </h6></div>
                                </div>
                            </div>
                            </td>
                            <td class="MiddleRightOrangeCorner"></td>
			</tr>
			<tr>
                            <td class="BottomLeftOrangeCorner"></td>
                            <td class="BottomCenterOrangeCorner"></td>
                            <td class="BottomRightOrangeCorner"></td>
			</tr>
		</table>
	</div>
</div>
</div>
<div style="clear:both"> </div>

