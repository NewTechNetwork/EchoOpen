<?php

class ntlp_handler_field_resource_sortlinks extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = array('table' => 'node', 'field' => 'type');
    $this->additional_fields['title'] = array('table' => 'node', 'field' => 'title');
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
            $nid = $values->{$this->aliases['nid']};
            $title = $values->{$this->aliases['title']};
            $type = $values->{$this->aliases['type']};
            $sectiontid = $values->{$this->aliases['sectiontid']};

            global $base_path;
            $image = $base_path . 'themes/Boldr/Images/resourceStuff/ArrowGreen';

            $output = "<a class='jslink' onclick='onchange_resource_order(0, $nid, $sectiontid)'><img src='".$image."Up.png' title='Move Up' /></a>";
            $output .= "&nbsp;";
            $output .= "<a class='jslink' onclick='onchange_resource_order(1, $nid, $sectiontid)'><img src='".$image."Down.png' title='Move Down' /></a>";
        }
    return $output;
  }



}

?>