<?php


function ntlp_resources_all_stuff($course_id = 0) {
drupal_add_js ('
  $(function(){
    $("#content_table").css("table-layout", "fixed");
    });
','inline');

    $url = $_GET['q'];
    $args = explode('/', $url);
    if ($course_id == 0) {
        //Getting Query String From URL
        $course_id = 0;
        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i]) && empty($course_id)) {
                $course_id = $args[$i];
                break;
            }
        }
    }

    $output = '<table style="margin-left: 5px;" width="100%"><tr><td colspan="2" style="padding-top:5px;">' . search_block_contents() . '</td></tr>';
    $output .= '<table style="margin-left: 5px;"><tr><td colspan="2">&nbsp</td></tr>';

    $output .= '<tr rowspan=2 /><tr id="container">';
    $output .= '<td id="left_panel" valign=top width=75%>' . ntlp_resources_all_stuff_search(false) . '</td>';
    $output .= '<td id="right_panel" valign=top>' . ntlp_all_stuff_filter($args, $course_id) . '</td>';
    $output .= '</tr>';
    $output .= '</table>';

    return $output;
}

function ntlp_all_stuff_filter($url, $course_id) {
    include 'resources.template.inc';

    $filterOpts = array(
        '#autoUncheckDefault' => true,
        'Type' => array(
            'All' => array(
                'type' => 'checkbox',
                'group' => 'post_type',
                'value' => 1,
                'name' => 'all',
                'default' => 1,
            ),
            'Links' => array(
                'type' => 'checkbox',
                'group' => 'post_type',
                'value' => 'links',
                'name' => 0,
            ),
            'Images' => array(
                'type' => 'checkbox',
                'group' => 'post_type',
                'value' => 'images',
                'name' => 1,
            ),
            'Media' => array(
                'type' => 'checkbox',
                'group' => 'post_type',
                'value' => 'videos',
                'name' => 2,
            ),
            'Documents' => array(
                'type' => 'checkbox',
                'group' => 'post_type',
                'value' => 'documents',
                'name' => 3,
            ),
            ),
        'Sort By' => array(
            'Most Recent' => array(
                'type' => 'radio',
                'group' => 'sort',
                'value' => 'created',
                'name' => 4,
                'default' => 1,
            ),
            'Most Liked' => array(
                'type' => 'radio',
                'group' => 'sort',
                'value' => 'node_liked',
                'name' => 5,
            ),
            'Most Commented' => array(
                'type' => 'radio',
                'group' => 'sort',
                'value' => 'comment_count',
                'name' => 6,
            ),
            ),

    );

    $url = array_splice($url, 0, -1);
    $url = implode("/", $url);

    $fopts = render_custom_filter(
                    $filterOpts,
                    array('start' => '<div>', 'end' => '</div>'),
                    array('template' =>  '<div class="StatusHeading" style="width:auto;padding-top:10px;padding-bottom:10px;"> @HEADING</div>', 'placeholder' => '@HEADING'),
                    '<br>',
                    url($url . '/search/' . $course_id . '/'),
                    'left_panel'
    );

    $fopts = str_replace('@FILTER_OPTIONS', $fopts, $HTMLSearchFilter);
    return $fopts;
}
