<?php
// $Id: flag_handler_field_ops.inc,v 1.1.2.5 2008/12/03 14:10:00 mooffie Exp $

/**
 * @file
 * Contains the flag Ops field handler.
 */

/**
 * Views field handler for the Flag operations links (flag/unflag).
 *
 * @ingroup views
 */
class ntlp_handler_field_checkbox extends views_handler_field {
/*  function options(&$options) {
    parent::options($options);
    $options['link_type'] = '';
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['link_type'] = array(
      '#type' => 'radios',
      '#title' => t('Link type'),
      '#options' => array('' => t('Use flag link settings')) + _flag_link_type_options(),
      '#default_value' => $this->options['link_type'],
    );
  }
*/
  function render($values) {
    $output = '';
    $output = "<input type='checkbox' id='chk_".$values->uid."' />";
    return $output;
  }
}
