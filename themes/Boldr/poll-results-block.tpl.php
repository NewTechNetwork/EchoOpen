<?php
// $Id: poll-results-block.tpl.php,v 1.2 2007/08/02 20:08:53 dries Exp $
/**
 * @file poll-results-block.tpl.php
 * Display the poll results in a block.
 *
 * Variables available:
 * - $title: The title of the poll.
 * - $results: The results of the poll.
 * - $votes: The total results in the poll.
 * - $links: Links in the poll.
 * - $nid: The nid of the poll
 * - $cancel_form: A form to cancel the user's vote, if allowed.
 * - $raw_links: The raw array of links. Should be run through theme('links')
 *   if used.
 * - $vote: The choice number of the current user's vote.
 *
 * @see template_preprocess_poll_results()
 */
?>
<div class="HomePagePolls">
<div id="AgendaPolls">
<div class="TopRightOrangeCorner">
<div class="TopLeftOrangeCorner">
<div class="AgendaActivitiesAndNotisfication">
<div class="AgendaOrangeBoxHeading">
<h2>Polls</h2>
</div>
</div>
</div>
</div>
<div class="ActivitiesMainContent">
<table cellspacing="0" cellpadding="0" border="0" width="100%">
<tbody><tr>
<td class="MiddleLeftOrangeCorner"></td>
<td class="MiddleCenterOrangeCorner">
<h5><?php print $title ?>
      </h5>
<div class="poll"><?php print $results ?></div>
<div class="AgendaPollsChangeLinkAndSubmitBtn">
<div class="AgendaPollsSubmitBtn" style="padding:4px 0 3px 0">
    <b style="height:20px;font-size:15px; "><?php print t('Total votes: @votes', array('@votes' => $votes)); ?></b>
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
</tbody></table>
</div>
</div>
</div>

