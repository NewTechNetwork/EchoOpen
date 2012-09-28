<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'constants');

class ntlp_handler_field_date extends views_handler_field {

  function options(&$options) {
    parent::options($options);
    $options['date_format'] = '';
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['date_format'] = array(
      '#type' => 'textfield',
      '#title' => t('Date Format'),

      '#default_value' => isset($this->options['date_format']) ? $this->options['date_format'] : NTLP_DATE_TZ_LONG,
//      '#default_value' => isset($this->options['date_format']) ? $this->options['date_format'] : 'm/d/Y - g:ia',
      '#description' => t('Example: m/d/Y - g:ia'),
      '#required' => TRUE,
    );
  }
  
  function render($values) {
      //print_r("df:".$this->options['date_format']);
        $value = $values->{$this->field_alias};
//        $format = isset($this->options['zdate_format']) ? $this->options['date_format'] : 'm/d/Y - g:ia';
        $format = isset($this->options['date_format']) ? $this->options['date_format'] : NTLP_DATE_TZ_LONG;
        if (isset($value)) {
            $school_nid = get_course_school_nid($values->ntlp_course_activity_course_nid);
            return convert_datetime_tz($school_nid, $value, true, $format);
            //return date($format, strtotime($value));//
        } else {
            return '';
        }

  }
}
