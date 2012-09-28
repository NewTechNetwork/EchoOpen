<?php
/**
 * Outcome field in Data Warehouse Course Grade (ntlp_dw_course_grade)
 */
class ntlp_handler_field_dw_cg_outcome extends views_handler_field {

  function options(&$options) {
    parent::options($options);
    $options['outcome_tid'] = '';
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['outcome_num'] = array(
      '#type' => 'textfield',
      '#title' => t('Outcome field number'),
      '#default_value' => 1,
      '#description' => t('The field number in DW Course Grade which you want to display for this Outcome (Taxonomy term).'),
      '#required' => TRUE,
    );

    $form['outcome_tid'] = array(
      '#type' => 'textfield',
      '#title' => t('Outcome (Taxonomy term) ID'),
      '#default_value' => isset($this->options['outcome_tid']) ? $this->options['outcome_tid'] : '',
      '#description' => t('The Outcome (Taxonomy term) ID of the data you want in this field. Blank this field to get aggregate of all outcomes.'),
      '#required' => TRUE,
    );

    $form['outcome_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Outcome Name'),
      '#default_value' => isset($this->options['outcome_name']) ? $this->options['outcome_name'] : '',
      '#description' => t('Leave empty for default.'),
      '#required' => FALSE,
    );
  }

  function query() {
    $ocno = isset($this->options['outcome_num']) ? $this->options['outcome_num'] : 0;
    $otid = isset($this->options['outcome_tid']) ? $this->options['outcome_tid'] : 0;
    $otname = isset($this->options['outcome_name']) ? $this->options['outcome_name'] : "Outcome $otid";
    $this->options['label'] = $otname;

    $this->query->add_field('', 'outcome_value_'.$ocno, 'outcome_'.$otid);
    $this->field_alias = 'outcome_'.$otid;
  }

  function render($values) {
//    $ocno = isset($this->options['outcome_num']) ? $this->options['outcome_num'] : 0;
//    if (!empty($ocno)) {
        $value = $values->{$this->field_alias};
        return number_format($value, 0, '.', '').'%';
//    } else {
//        return '';
//    }
  }
}