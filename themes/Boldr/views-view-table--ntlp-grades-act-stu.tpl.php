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
<table class="<?php print $class; ?>">
  <?php if (!empty($title)) : ?>
    <caption><?php print $title; ?></caption>
  <?php endif; ?>
  <thead>
    <tr>
      <?php foreach ($header as $field => $label): ?>
        <?php if ($fields[$field] != 'date-submitted' && $fields[$field] != 'is-complete' ) { ?>
        <th class="views-field views-field-<?php print $fields[$field]; ?>">
          <?php print $label; ?>
        </th>
        <?php } ?>
      <?php endforeach; ?>
    </tr>
  </thead>
  <tbody>
    <?php foreach ($rows as $count => $row): ?>
      <?php //Check if it is an incomplete submission
      $bgcolor = '#ffffff';
      if (!empty($row['date_submitted']) && $row['is_complete'] == "False") {
          $bgcolor = '#ffcccc';
      }
      $displayed = false;
      ?>
      <tr class="<?php print implode(' ', $row_classes[$count]); ?>">
        <?php foreach ($row as $field => $content): ?>
          <?php if ($fields[$field] != 'date-submitted' && $fields[$field] != 'is-complete' ) {
                if (strpos($fields[$field], 'outcome-tid') === 0) {
                    //Check if it is an incomplete submission
                    if (!empty($row['date_submitted']) && $row['is_complete'] == "False" && !$displayed) {
                        $displayed = true;
          ?>
                    <td class="views-field views-field-<?php print $fields[$field]; ?>"
                        style="text-align:center;background-color:<?php print $bgcolor; ?>"
                        colspan="<?php print sizeof($fields)-6; ?>">
                        <span>Activity Incomplete</span>
                    </td>
                    <?php } ?>
                <?php } ?>
                <?php if (!$displayed) { ?>
                <td class="views-field views-field-<?php print $fields[$field]; ?>" style="background-color:<?php print $bgcolor; ?>">
                  <?php print $content; ?>
                </td>
                <?php } ?>
          <?php } ?>
        <?php endforeach; ?>
      </tr>
    <?php endforeach; ?>
  </tbody>
</table>
