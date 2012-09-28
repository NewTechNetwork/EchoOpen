<?php

class ntlp_handler_field_resource_removelink extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = array('table' => 'node', 'field' => 'nid');
    $this->additional_fields['sectiontid'] = array('table' => 'ntlp_project_resource', 'field' => 'section_tid');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $output = '';
    if (isset($this->view->resource_edit_mode))
        if ($this->view->resource_edit_mode) {
            $sectiontid = $values->{$this->aliases['sectiontid']};
            $nid        = $values->{$this->aliases['nid']};

            global $base_path;
            $image = $base_path . 'themes/Boldr/Images/DeleteIcon.jpg';

            $output = "<a class='jslink' onclick='onclick_resource_remove($nid, $sectiontid)'><img src='".$image."' title='Remove' /></a>";
        }
    return $output;
  }



}

?>