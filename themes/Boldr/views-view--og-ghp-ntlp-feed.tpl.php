<?php
// $Id: views-view.tpl.php,v 1.13 2009/06/02 19:30:44 merlinofchaos Exp $
/**
 * @file views-view.tpl.php
 * Main view template
 *
 * Variables available:
 * - $css_name: A css-safe version of the view name.
 * - $header: The view header
 * - $footer: The view footer
 * - $rows: The results of the view query, if any
 * - $empty: The empty text to display if the view is empty
 * - $pager: The pager next/prev links to display, if any
 * - $exposed: Exposed widget form/info to display
 * - $feed_icon: Feed icon to display, if any
 * - $more: A link to view more, if any
 * - $admin_links: A rendered list of administrative links
 * - $admin_links_raw: A list of administrative links suitable for theme('links')
 *
 * @ingroup views_templates
 */

$is_allstuff = (arg(3) == 'allstuff');
?>
<div class="view view-<?php print $css_name; ?> view-id-<?php print $name; ?> view-display-id-<?php print $display_id; ?> view-dom-id-<?php print $dom_id; ?>">
  <?php if ($header): ?>
    <div class="view-header">
      <?php print $header; ?>
    </div>
  <?php endif; ?>

    <div class="dialog-confirm-ntk-resource" style="background-color:#fff; height:123px !important; display:none;" title="Delete Share Stuff Post">
        <p>You are about to permanently delete this post and any associated comments.</p>
        <p>Are you sure you want to permanently delete this item? </p>
    </div>


  <?php if ($rows): ?>
    <div class="view-content">
      <?php print $rows; ?>
    </div>
  <?php elseif ($empty): ?>
    <div class="view-empty">
      <?php print $empty; ?>
    </div>
  <?php endif; ?>

    
        <?php if (!$is_allstuff): ?>
       <div style="width: 100px;margin:0 auto;">
           <a class="stuff-link" href="<?php echo url("ntlp/" . arg(1) . "/resources/allstuff/".arg(3)); ?>"> View all posts</a>
          
       </div>
        <?php endif; ?>
    
    <div class="AgendaShareStuffBottomRightCorner">
        <div class="AgendaShareStuffBottomLeftCorner" style="font-size:0px;">&nbsp;
    </div></div>

  <?php if ($pager && $is_allstuff): ?>
  <div style="background-color:white;display:table;width:100%"><br>
    <?php print $pager; ?>
  </div>
  <?php endif; ?>

  <?php if ($more): ?>
    <?php print $more; ?>
  <?php endif; ?>

  <?php if ($footer): ?>
    <div class="view-footer">
      <?php print $footer; ?>
    </div>
  <?php endif; ?>

</div> <?php /* class view */ ?>
