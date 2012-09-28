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

<table class="<?php print $class; ?>">
    <?php if (!empty($title)) : ?>
    <caption><?php print $title; ?></caption>
    <?php endif; ?>
    <thead>
        <tr>
            <?php foreach ($header as $field => $label): ?>
                <?php if ($fields[$field] != 'nid') { ?>
            <th class="views-field views-field-<?php print $fields[$field]; ?>">
                        <?php print $label; ?>
            </th>
                    <?php } ?>
            <?php endforeach; ?>
        </tr>
    </thead>
    <tbody>
        <?php foreach ($rows as $count => $row): ?>
            <?php
            global $user;
            module_load_include('inc', 'ntlp_school', 'constants');
            module_load_include('inc', 'ntlp_school', 'permission');
            module_load_include('module', 'ntlp_resources', 'ntlp_resources');
            
            //Load current node/resource
            $node = node_load($row['nid']);

            //Load current user
            if(isset($user->roles[NTLP_ROLEID_PARENT])) {  // if parent come here show their student record
                $user_obj = user_load($_SESSION['PARENT_STUDENT_UID']);
            } else {
                $user_obj = $user;
            }
            $user_role = check_resource_permission($node, $user_obj);
            
            //    print '<tr><td>'.print_r($row, true).'...'.print_r($node, true).'</td></tr>';
            if(isset($user_role) || isset($user_obj->roles[NTLP_ROLEID_TEACHER]) || isset($user_obj->roles[NTLP_ROLEID_NETWORKMANAGER])
                    || isset($user_obj->roles[NTLP_ROLEID_TEACHER]) || isset($user_obj->roles[NTLP_ROLEID_SITEGUEST]) || is_node_owner($row['nid'], $user)) { ?>

        <tr class="<?php print implode(' ', $row_classes[$count]); ?>">
                    <?php foreach ($row as $field => $content): ?>
                        <?php if ($fields[$field] != 'nid') { ?>
            <td class="views-field views-field-<?php print $fields[$field]; ?>">
                                <?php print $content; ?>
            </td>
                            <?php } ?>
                    <?php endforeach; ?>
        </tr>
                <?php } //else { unset($rows[$count]); } ?>
        <?php endforeach; ?>
    </tbody>
</table>
