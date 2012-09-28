<?php

/**
 * Implementation of hook_form_alter().
 *
 * Detect menutrails for non-node-view pages.
 * Custom support for NTLP menu paths
 * This hook is used to allow the called menus to render their output
 * If we use this function in hook_init than the parent node gets rendered.
 * @todo Replace TRUE with settings check.
 */
function form_alter_menu_trails(&$form, &$form_state, $form_id) {
    $item = menu_get_item();
    if ($item['page_callback'] != 'node_page_view') {
        $mid         = get_ntlp_menuid($item['path']);
        $ntlp_trails = variable_get('menutrails_ntlp', array());
        $href        = $ntlp_trails[$mid] ? $ntlp_trails[$mid] : FALSE;
        if ($href) {
            //Set default main menu for this form
            $mainmenu = menu_get_item($href);
            menu_set_item(NULL, $mainmenu);
        }
    }
}

/**
 * Implementation of hook_menutrails_settings().
 *
 * Allows other modules to define their own menutrail behavior.
 *
 * Please define your input as a fieldset and do not assign a weight. This will
 * keep the groups of menutrails settings in order.
 *
 * @param $options
 *   Options array to be used by other modules to define their own menutrails.
 *
 * @return
 *   A form element (or array) for the menutrails system settings form.
 */
function ntlp_blocks_menutrails_settings($options) {
  // NTLP support.
  $ntlp_trails = variable_get('menutrails_ntlp', array());
//  watchdog('ntlp', print_r($ntlp_trails, true));
  $form['menutrails_ntlp'] = array(
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('NTLP'),
      '#description' => t('Settings for NTLP menu items.'),
  );
  $rs_mi = get_ntlp_menus();
  while ($rec_mi = db_fetch_array($rs_mi)) {
      $form['menutrails_ntlp'][$rec_mi['mlid']] = array('#type' => 'select',
          '#title' => t('Parent item for @title (@path)', array(
              '@title' => $rec_mi['link_title'],
              '@path' => $rec_mi['link_path']
              )),
          '#default_value' => isset($ntlp_trails[$rec_mi['mlid']]) ? $ntlp_trails[$rec_mi['mlid']] : NULL,
          '#options' => $options,
      );

  }

//  // NTLP School-Selector support.
//  $ntlp_trails = variable_get('menutrails_ntlp_schoolselector', array());
////  watchdog('ntlp', print_r($ntlp_trails, true));
//  $form['menutrails_ntlp_schoolselector'] = array(
//      '#tree' => TRUE,
//      '#type' => 'fieldset',
//      '#collapsible' => TRUE,
//      '#collapsed' => TRUE,
//      '#title' => t('NTLP School Selector'),
//      '#description' => t('Availability of NTLP School Selector.'),
//  );
//  $rs_mi = get_ntlp_menus();
//  while ($rec_mi = db_fetch_array($rs_mi)) {
//      $form['menutrails_ntlp_schoolselector'][$rec_mi['mlid']] = array(
//          '#type' => 'checkbox',
//          '#title' => t('Parent item for @title (@path)', array(
//              '@title' => $rec_mi['link_title'],
//              '@path' => $rec_mi['link_path']
//              )),
//          '#value' => isset($ntlp_trails[$rec_mi['mlid']]) ? $ntlp_trails[$rec_mi['mlid']] : NULL,
//      );
//
//  }
  return $form;
}


/**
 * Get NTLP menu trails
 */
function get_ntlp_menus() {
  $menu = db_query("SELECT * FROM {menu_links} WHERE link_path LIKE 'ntlp/%' or link_path LIKE 'nav/%' order by link_path");
  if (isset($menu))
    return $menu;
  else
    return false;
}
function get_ntlp_menuid($path) {
  return db_result(db_query("SELECT mlid FROM {menu_links} WHERE link_path LIKE '%s'", $path));
}


?>
