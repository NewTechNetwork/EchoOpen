<?php

class ntlp_handler_field_ntk_removelink extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = array('table' => 'node', 'field' => 'nid');
    $this->additional_fields['group_nid'] = array('table' => 'og_ancestry', 'field' => 'group_nid');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $output = '';
    $nid    = $values->{$this->aliases['nid']};
    $group_nid    = $values->{$this->aliases['group_nid']};

    global $base_path;
    $image = $base_path . 'themes/Boldr/Images/DeleteIcon.jpg';

    $output = "<a class='jslink' onclick='onclick_ntk_remove($nid, $group_nid)'><img src='".$image."' title='Remove' /></a>";
    return $output;
  }
}

?>