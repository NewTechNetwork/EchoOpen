<?php

class ntlp_handler_field_resource_hideshow extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = array('table' => 'node', 'field' => 'nid');
    $this->additional_fields['sectiontid'] = array('table' => 'ntlp_project_resource', 'field' => 'section_tid');
    $this->additional_fields['availability'] = array('table' => 'ntlp_project_resource', 'field' => 'availability');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $nid = $values->{$this->aliases['nid']};
    $sectiontid = $values->{$this->aliases['sectiontid']};
    $availability = $values->{$this->aliases['availability']};

    $hideChecked = "";
    $showChecked = "";
    $output = '';
    $showradio =  isset($this->view->resource_edit_mode) && isset($this->view->resource_teacher_section) ?
        $this->view->resource_edit_mode && $this->view->resource_teacher_section != $sectiontid : false;

    if ($showradio) {
        watchdog('view', "Edit:".$this->view->resource_edit_mode."., Section:".$this->view->resource_teacher_section);
        if ($availability == 'H')
            $hideChecked = "checked='CHECKED'";
        else
            $showChecked = "checked='CHECKED'";
        $output = "<input type='radio' name='rdoHideShow_$nid' onclick='onchange_resource_availability(0, $nid, $sectiontid)' $hideChecked>Hide</input>";
        $output .= "&nbsp;&nbsp;";
        $output .= "<input type='radio' name='rdoHideShow_$nid' onclick='onchange_resource_availability(1, $nid, $sectiontid)' $showChecked>Show</input>";
    }
    else
    {
        watchdog('view', "Edit:No, Section:".$sectiontid);
        if ($availability == 'H')
            $output = "Hidden";
        else
            $output = "Show";
    }
    return $output;
  }



}

?>