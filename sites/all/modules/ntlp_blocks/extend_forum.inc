<?php

function ntlp_blocks_theme_forum_node_form($form) {
    print_r($form);
    die();
    watchdog('themer', print_r($form));
}

///**
// * Implementation of hook_form_alter().
// */
//function form_alter_forum(&$form, &$form_state, $form_id) {
//
//    if ($form_id == 'forum_node_form') {
//        unset($form['taxonomy']);
//        unset($form['og_nodeapi']['visible']['og_groups']);
//
//    }
//    if ($form_id == 'forum_form_container') {
//        unset($form['make_public']);
//    }
//
//    if ($form_id == 'forum_form_forum') {
//        unset($form['make_public']);
//    }
//} // function og_forum_form_alter()
//
///**
//* This snippet loads a custom page-forum.tpl.php layout file when
//* users click through to the login, request password or register pages
//*/
//
//function _phptemplate_variables($hook, $variables = array()) {
//  switch ($hook) {
//    case 'page':
//   // node type isn't available in template.php so we have to go get it.
//    if (arg(0) == 'node' && is_numeric(arg(1))) {
//      $node = node_load(arg(1));
//      $type = $node->type;
//    }
//    if (($node->type) == 'forum') {
//       // if the node type is forum load the page-forum.tpl.php layout
//        $variables['template_file'] = 'page-forum';
//        }
//      break;
//  }
//
//  return $variables;
//}

?>
