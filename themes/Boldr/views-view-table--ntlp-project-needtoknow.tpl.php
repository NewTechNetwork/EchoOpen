<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


// $Id: views-view-table.tpl.php,v 1.8 2009/01/28 00:43:43 merlinofchaos Exp $
/**
 * @file views-view-table.tpl.php
 * Template to display a view as a table.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $header: An array of header labels keyed by field id.
 * - $fields: An array of CSS IDs to use for each field id.
 * - $class: A class or classes to apply to the table, based on settings.
 * - $row_classes: An array of classes to apply to each row, indexed by row
 *   number. This matches the index in $rows.
 * - $rows: An array of row items. Each row is an array of content.
 *   $rows are keyed by row number, fields within rows are keyed by field ID.
 * @ingroup views_templates
 */
?>
<style type="text/css">
.views-field-title-with-icon {
    width: 70%;
}
.views-field-sort-links {
    width: 15%;
    text-align: center !important;
}
.views-field-availability {
    width: 15%;
    text-align: right;
}
.view-content .views-table tbody tr td {
    text-align:left;
    background-color:transparent !important;
    padding:10px 0px 10px 0;
    border-left:none;
    border-bottom: 1px dotted #B6CDDB;
    border-collapse: collapse;
    color:#000;
}
tr.even, tr.odd {
    background-color:transparent !important;
}
.need_to_know_last_tr {
    border-bottom:none !important;
}

.need_to_know_last_td {
    border-top: 1px solid #bacbdb !important;
    text-align: right !important;
    border-bottom:none !important;
    background-image: none !important;
    padding-bottom: 0px;
}
.need_to_know_content {
    background-image: none;
}
.views-field-fullname {
    color:#090 !important;
    font-weight: bold;
}
.view-content .views-table {
    border:none !important;
}
.views-field-delete-node {
    text-align: right;
}
.views-field-title {
    width:420px;
}
.views-field-fullname  {
    width:100px;
}
.view-content .views-table tbody tr td {
    padding:0px !important;
}
.need_to_know_last_td {
    padding:0px;
}


</style>
<table border="0" cellpadding="0" cellspacing="0" class="<?php print $class; ?>">
  <tbody>
    <?php foreach ($rows as $count => $row): ?>
      <tr class="need_to_know_content_row <?php print implode(' ', $row_classes[$count]); ?>">
        <?php foreach ($row as $field => $content): ?>
          <td class="need_to_know_content views-field-<?php print $fields[$field]; ?>">
           <?php print $content; ?>
          </td>
        <?php endforeach; ?>
      </tr>
      <?php if( $count == $view->display['default']->display_options['items_per_page']-1 ) break; ?>
    <?php endforeach; ?>
      <tr  class="need_to_know_last_tr">
          <td colspan="3" align="right" class="need_to_know_last_td">
               <?php if( count($view->result) > $view->display['default']->display_options['items_per_page'] ): ?>
                    <?php echo dlg( 'View All »', 'ntlp/courses/project/ntk/popup/'.$view->args[0], 500,500 ) ?>
              <?php endif ?>
          </td>
      </tr>
  </tbody>
</table>
