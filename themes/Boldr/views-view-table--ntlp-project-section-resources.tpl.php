<?php
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
    float:left;
    height:25px;
    width:600px;

}
.views-field-sort-links {
    text-align: center !important;
    float:left;
    height:20px;
    width: auto;
    padding:5px 5px 0 5px;

}
.views-field-availability {
    text-align: right;
    float:left;
    height:20px;
    width: auto;
    padding:5px 5px 0 5px;

}
.view-content .views-table tbody tr td {
    text-align:left;
    background-color:#fff;
    padding:8px;
    border-left:none;
    border-collapse: collapse;
    color:#000;
}
.views-field-remove-link {
    float:right;
    height:20px;
    width: auto;
    padding:5px 0px 0 5px;
}
</style>
<table class="<?php print $class; ?>">
  <tbody>
    <?php foreach ($rows as $count => $row): ?>
      <tr id="section-resource-row-<?php echo $view->result[$count]->nid ?>" class="<?php print implode(' ', $row_classes[$count]); ?>">
        <td>
            <div style="padding: 0 5px 0 5px;">
      <?php foreach ($row as $field => $content): ?>
          <div class="views-field-<?php print $fields[$field]; ?>">
            <?php print $content; ?>
          </div>
        <?php endforeach; ?>
            </div>
            </td>
      </tr>
    <?php endforeach; ?>
  </tbody>
</table>
