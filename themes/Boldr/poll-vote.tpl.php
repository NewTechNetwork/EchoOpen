<?php
// $Id: poll-vote.tpl.php,v 1.2 2007/08/07 08:39:35 goba Exp $

/**
 * @file poll-vote.tpl.php
 * Voting form for a poll.
 *
 * - $choice: The radio buttons for the choices in the poll.
 * - $title: The title of the poll.
 * - $block: True if this is being displayed as a block.
 * - $vote: The vote button
 * - $rest: Anything else in the form that may have been added via
 *   form_alter hooks.
 *
 * @see template_preprocess_poll_vote()
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
<h5><?php if ($block): ?>
        <?php print $title; ?>:
      <?php endif; ?>
      </h5>
<?php print $choice; ?>
<div class="AgendaPollsChangeLinkAndSubmitBtn">
<div class="AgendaPollsSubmitBtn">
    						 <?php print $vote; ?>
    <?php // This is the 'rest' of the form, in case items have been added. ?>
  <?php print $rest ?>
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