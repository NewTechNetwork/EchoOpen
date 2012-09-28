<!--// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.
 -->
<!-- start node-add-forum.tpl.php -->
<div>
<?php
    if (arg(2) == 'edit') {
        $editnid = arg(1);
        $node = node_load($editnid);
    }
    print $node->title;

    print node_view($node, false, true, false);
?>
</div>
<?php
    $submitform = "<div id='right5050'><h3>Course Discussion Forum</h3>";
//    $submitform .= drupal_set_title('');
//    watchdog('themer', 'Forum form data:'.print_r($form, true));
    unset($form['taxonomy']);
    unset($form['body_field']['format']);
    unset($form['body_field']['teaser_include']);
//    watchdog('discusson_fourm',print_r($form['description'], true));
    $submitform .= drupal_render($form);
    $submitform .= "</div>";
    print $submitform;
?>
<!-- end .tpl.php -->