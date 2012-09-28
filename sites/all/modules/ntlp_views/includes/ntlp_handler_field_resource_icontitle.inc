<?php

class ntlp_handler_field_resource_icontitle extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['type'] = array('table' => 'node', 'field' => 'type');
    $this->additional_fields['title'] = array('table' => 'node', 'field' => 'title');
    $this->additional_fields['nid'] = array('table' => 'node', 'field' => 'nid');
    $this->additional_fields['coursenid'] = array('table' => 'ntlp_course_resource', 'field' => 'course_nid');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

 function render($values) {
    try {
        $nid = $values->{$this->aliases['nid']};
        $title = $values->{$this->aliases['title']};
        $type = $values->{$this->aliases['type']};
        $coursenid = $values->{$this->aliases['coursenid']};

        global $base_path;
        $image = $base_path . 'themes/Boldr/Images/resourceStuff/';
        switch ($type) {
            case 'lo_document':
                $image .= 'img_document.png';
                break;
            case 'lo_image':
                $image .= 'img_picture.png';
                break;
            case 'lo_link':
                $image .= 'img_link.png';
                break;
            case 'lo_page':
                $image .= 'img_page.png';
                break;
            case 'lo_share_idea':
                $image .= 'img_idea.png';
                break;
            case 'lo_video':
                $image .= 'img_video.png';
                break;
        }
        $link = url("ntlp/courses/resource/view/$coursenid/$nid");
        $output = "<a href='$link'><img src='$image' title='$title' />&nbsp;<span>$title</span></a>";
    }
    catch (Exception $e) {
        $output = $e->getMessage();
    }
    return $output;
  }



}

?>