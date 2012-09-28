<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


// $Id: flag.tpl.php,v 1.1.2.7 2009/03/17 02:10:30 quicksketch Exp $

/**
 * @file
 * Default theme implementation to display a flag link, and a message after the action
 * is carried out.
 *
 * Available variables:
 *
 * - $flag: The flag object itself. You will only need to use it when the
 *   following variables don't suffice.
 * - $flag_name_css: The flag name, with all "_" replaced with "-". For use in 'class'
 *   attributes.
 * - $flag_classes: A space-separated list of CSS classes that should be applied to the link.
 *
 * - $action: The action the link is about to carry out, either "flag" or "unflag".
 * - $last_action: The action, as a passive English verb, either "flagged" or
 *   "unflagged", that led to the current status of the flag.
 *
 * - $link_href: The URL for the flag link.
 * - $link_text: The text to show for the link.
 * - $link_title: The title attribute for the link.
 *
 * - $message_text: The long message to show after a flag action has been carried out.
 * - $after_flagging: This template is called for the link both before and after being
 *   flagged. If displaying to the user immediately after flagging, this value
 *   will be boolean TRUE. This is usually used in conjunction with immedate
 *   JavaScript-based toggling of flags.
 * - $setup: TRUE when this template is parsed for the first time; Use this
 *   flag to carry out procedures that are needed only once; e.g., linking to CSS
 *   and JS files.
 *
 * NOTE: This template spaces out the <span> tags for clarity only. When doing some
 * advanced theming you may have to remove all the whitespace.
 */
  global $base_path;

  if ($setup) {
    drupal_add_css(drupal_get_path('module', 'flag') .'/theme/flag.css');
    drupal_add_js(drupal_get_path('module', 'flag') .'/theme/flag.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_project') . '/js/jquery.tipsy.js');
    drupal_add_js('
     function show_tooltip_like(){
	$(".likeUnlike").tipsy({"opacity" : 1, title : "newTitle"});
     }','inline');
  }
?>
<span class="flag-<?php echo $flag_name_css; ?>">
    <a  href="<?php echo $link_href; ?>" newTitle="<?php echo $link_title; ?>" class="<?php print $flag_classes ?> likeUnlike" rel="nofollow" onmouseover="show_tooltip_like();">
        <img src='<?php echo $base_path . path_to_theme() . "/Images/common/" . ($action == 'flag' ? 'img_like' : 'img_unlike') . ".png" ?>' onmouseout="$('#tips').remove();" /></a>
  <!-- <span class="flag-throbber">&nbsp;</span> -->
  <?php if ($after_flagging): ?>
    <span class="flag-message flag-<?php echo $last_action; ?>-message">
      <?php echo $message_text; ?>
    </span>
  <?php endif; ?>
</span>
