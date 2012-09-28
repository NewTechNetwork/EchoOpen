<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_courses_activity', 'data_access');
require_once ('data_access.inc');

function ntlp_most_popular_resource_lib_contents() {

    include 'ntlp_resource_lib_resource.template.inc';

    $output = $HTMLResourcesBock;
    $output = str_replace( '@BLOCK_HEADING', 'Most Popular', $output );
    $tempList = '';

    foreach( get_resource_details( 0, array('WITH_TITLE' => true), 'ntlp_network_resource_library' ) as $resource ) {



        $tempList .= str_replace( '@TITLE', l($resource->title,'ntlp/resource/library/view/'.$resource->nid . '/' . $resource->category_tid), $HTMLResourcesBock_list );
        $tempList = str_replace( '@IMAGE_URL', _get_resource_icon_url($resource->type), $tempList );
    }
    $output = str_replace( '@LIST_PLACEHOLDER', $tempList, $output );

    return $output;
}

function ntlp_recent_additions_resource_lib_contents_by_category(  ) {
    if ($category_id == 0) {
        //Getting Query String From URL
        $resource_nid = 0;
        $category_id = 0;
        $url = $_GET['q'];
        $args = explode('/', $url);
        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i]) && empty($category_id))
                $category_id = $args[$i];
            elseif (is_numeric($args[$i]) && empty($resource_nid))
                $resource_nid = $args[$i];
        }
    }

    return ntlp_recent_additions_resource_lib_contents( $category_id );
}

function ntlp_recent_additions_resource_lib_contents( $category = '' ) {

    include 'ntlp_resource_lib_resource.template.inc';

    $output = $HTMLResourcesBock;
    $output = str_replace( '@BLOCK_HEADING', 'Recent Additions', $output );
    $tempList = '';

    $filters = array('WITH_TITLE' => true, 'ORDER' => array('FIELD'=>'created', 'SORT'=>'DESC') );
    if( $category ) {
        $filters = array_merge($filters, array('BY_CATEGORY' => $category) );
        $school_nid = false;
    }

    foreach( get_resource_details( 0, $filters, 'ntlp_network_resource_library' ) as $resource ) {

        $tempList .= str_replace( '@TITLE', l($resource->title,'ntlp/resource/library/view/'.$resource->nid. '/' . $resource->category_tid ), $HTMLResourcesBock_list );
        $tempList = str_replace( '@IMAGE_URL', _get_resource_icon_url($resource->type), $tempList );
    }
    $output = str_replace( '@LIST_PLACEHOLDER', $tempList, $output );

    return $output;
}

function ntlp_total_resource_lib_contents() {
    include 'ntlp_resource_lib_resource.template.inc';

    $output = $HTMLTotalResourcesBock;
    $output = str_replace( '@BLOCK_HEADING', 'items', $output );

    if( $resource = get_resource_details( 0, array('COUNT' => true), 'ntlp_network_resource_library') ) {
        $output = str_replace( '@TOTAL', $resource->total, $output );
    } else {
        $output = str_replace( '@TOTAL', 0, $output );
    }
    return $output;
}

function ntlp_resource_lib_contents_category_keyword() {
    if ($category_id == 0) {
        //Getting Query String From URL
        $resource_nid = 0;
        $category_id = 0;
        $url = $_GET['q'];
        $args = explode('/', $url);
        for ($i = 0; $i < count($args); $i++) {
            if (is_numeric($args[$i]) && empty($category_id))
                $category_id = $args[$i];
            elseif (is_numeric($args[$i]) && empty($resource_nid))
                $resource_nid = $args[$i];
        }
    }

    $resource_node = node_load($resource_nid);

    //Get Keywords and Subjects
    $node_related_taxo = taxonomy_node_get_terms($resource_node);

    $library_cate_Obj   = _get_lib_vocabulary_vid("NTLP Library Category");
    $ntlp_post_Obj      = _get_lib_vocabulary_vid("NTLP Posts");

    foreach($node_related_taxo as $Obj) {
        if($Obj->vid == $library_cate_Obj->vid) {
            $category_array[] = $Obj;
        }
        if($Obj->vid == $ntlp_post_Obj->vid) {
            $keywords_array[] = $Obj;
        }
    }

    include 'ntlp_resource_lib_resource.template.inc';
    $output = $HTMLResourcesBockCategoryKeyword;
    $output = str_replace( '@BLOCK_HEADING', 'Categories', $output );
    $tempList = '';

    if($category_array != null) {
        foreach($category_array as $subject) {
            $tempList .= $HTMLResourcesBockCategoryKeyword_list;
            $tempList = str_replace('@TITLE', '<a href="'.url('ntlp/resource/library/search/'.$subject->tid).'">'.$subject->name.'</a> ', $tempList );
        }
    }
    $output = str_replace( '@LIST_PLACEHOLDER', $tempList, $output );

    $output  .= $HTMLResourcesBockCategoryKeyword;
    $output = str_replace( '@BLOCK_HEADING', 'Keywords', $output );
    $tempList = '';

    if($keywords_array != null) {
        foreach($keywords_array as $keyword) {
            $tempList .= $HTMLResourcesBockCategoryKeyword_list;
            $tempList = str_replace('@TITLE', '<a href="'.url('taxonomy/term/'.$keyword->tid).'">'.$keyword->name.'</a> ', $tempList );
        }
    }
    $output = str_replace( '@LIST_PLACEHOLDER', $tempList, $output );

    return $output;
}

function _get_resource_lib_icon_url( $type ) {
    global $base_path;

    $icon = '';
    switch( $type ) {
        case 'lo_document':
            $icon = 'img_document.png';
            break;
        case 'lo_image':
            $icon = 'img_picture.png';
            break;
        case 'lo_link';
            $icon = 'img_link.png';
            break;
        case 'lo_page':
            $icon = 'img_page.png';
            break;
        case 'lo_video':
            $icon = 'img_video.png';
            break;
    }

    return $base_path."themes/Boldr/Images/resourceStuff/".$icon;
}


function ntlp_resource_lib_filter_block() {
    include 'ntlp_resource_lib_resource.template.inc';
    drupal_add_js(drupal_get_path('module', 'ntlp_resource_library') . '/ntlp_resource_library.js');

    $cat = (int)(arg(4) ? arg(4) : 0);
    $keys = $_GET['keys'];

    $output = $HTMLResourcesFilter;

    $module_Obj =  _get_module_detail('ntlp_network_resource_library');
    $school_nid = $module_Obj->nid;
    $cats = array();
    $cats["All"] = 0;

//    $selectBoxCategory = '<select onchange="window.location=\'?q=ntlp/resource/library/search&subject=\'+this.value;"><option value="0">Select Category</option>';
    foreach( get_module_categories($module_Obj->nid) as $category ) {
//        $select = @$_GET['cat'] == $category->tid ? ' selected ' : '';
//        $selectBoxCategory .= "<option value=\"{$category->tid}\" $select>{$category->name}</option>";
        $cats["$category->name"] = $category->tid;
    }
//    $selectBoxCategory .= '</select>';

//    $output = str_replace( '@CATEGORY_COMBO', $selectBoxCategory, $output );

    //FILTER START
    $filterOpts = array(
        'Category' => array(
            'Category' => array(
                'type' => 'combobox',
                'options' => $cats,
                'value' => 'category',
                'name' => 0,
                'default' => isset($cat) ? $cat : '0',
                'reload_page' => "?q=ntlp/resource/library/search/@VALUE",
                'style' => 'width:99%',
            ),
        ),
    );
    $fopts = render_custom_filter(
            $filterOpts, array('start' => '', 'end' => ''), array( 'template' => '', 'placeholder' => '' ),
            '', '?q=ntlp/resource/library/filter/'.$cat.'/1&keys='.$keys, 'library_document_table_view'
    );
    $output = str_replace( '@CATEGORY_COMBO', $fopts, $output );

    $filterOpts = array(
        '#autoUncheckDefault' => false,
            '<b style="font-weight:bold;color:#3570AA">Resource Type</b>' => array(
//            '#SelectAll' => true,
            '#autoUncheckDefault' => array('value' => true),
            'All' => array(
                'type'=>'checkbox',
                'group'=>'type',
                'value' => 1,
                'name' => 'all',
                'default' => true,
            ),
                    'Link' => array(
                            'type'=>'checkbox',
                            'group'=>'type',
                            'value' => 'link',
                            'name' => 0,
//                'default' => 1,
                    ),
                    'Images' => array(
                            'type'=>'checkbox',
                            'group'=>'type',
                            'value' => 'images',
                            'name' => 1,
                    ),
                    'Videos' => array(
                            'type'=>'checkbox',
                            'group'=>'type',
                            'value' => 'videos',
                            'name' => 2,
                    ),
                    'Documents' => array(
                            'type'=>'checkbox',
                            'group'=>'type',
                            'value' => 'documents',
                            'name' => 3,
                    ),
                    'Pages' => array(
                            'type'=>'checkbox',
                            'group'=>'type',
                            'value' => 'pages',
                            'name' => 4,
                    ),
//            'Hidden' => array(              // Hidden checkbox is created so that when all checkboxes are
//                'type'=>'checkbox',         // unchecked or all are checked, then no value is passed in POST
//                'group'=>'type',
//                'value' => 'hidden',
//                'name' => 99,
//                'default' => 1,
//                'style' => 'display:none',
//            ),
            ),
        '<br><b style="font-weight:bold;color:#3570AA">Other</b>' => array(
            'Featured Items Only' => array(
                'type'=>'checkbox',
                'group'=>'other',
                'value' => 1,
                'name' => 'featured',
                'default' => false,
            ),
        ),
    );

    $fopts = render_custom_filter(
            $filterOpts,
            array('start' => $HTMLResource_filter_cellStart, 'end' => $HTMLResource_filter_cellEnd),
            array( 'template' => $HTMLResource_filter_heading, 'placeholder' => '@HEADING' ),
            $HTMLResource_filter_row_end,
            '?q=ntlp/resource/library/filter/'.$cat.'/1&keys='.$keys,
            'library_document_table_view'
    );

    $output = str_replace('@FILTER_PLACEHOLDER', $fopts, $output);

    return $output;
 }

function ntlp_category_resource_lib_filter() {
    include 'document.template.inc';
    return $HTMLCategoryResourceFilter;
}


function ntlp_resource_library_search_block_content() {
    $output = '';
    $subjects = _get_resource_library_keywords();
//'ntlp/library/project
    if(!empty($subjects) && !empty($subjects[0])) {

        $example_subject = '<span style="color:#3570AA;font-weight:bold">Example:</span>';

        for($i=0; $i< 3; $i++ ) {
            $example_subject .= '<a class="greenLink" href="'.url('ntlp/resource/library/search/'.$subjects[$i]->tid).'">'.$subjects[$i]->name.'</a>, ';
        }
        $example_subject = rtrim(trim($example_subject), ',');
    }else {
        $example_subject ='';
    }

    $category_id = arg(4);
    $cat = "";
    if ($category_id > 0) {
        $cat = "/$category_id";
    }

    $output = '<div class="network_blue_border_middle_right" style="width:614px">
<div class="network_blue_border_middle_left">
<div class="network_blue_border_bottom_center">
<div class="network_blue_border_top_center">
<div class="network_blue_border_top_left">
                    <div class="network_blue_border_top_right">
                    <div class="network_blue_border_bottom_left">
                    <div class="network_blue_border_bottom_right"><div>
                    <div>
                      <table>
                        <tr>
                            <td class="view-filters">
                                <input type="text" class="form-text" id="search_field" style="width:500px" title="What are you looking for?" 
                                onkeydown="if (event.keyCode == 13) {
                                    if(!document.getElementById(\'search_field\').value == \'\'){
                                        window.location =\'?q=ntlp/resource/library/search'.$cat.'&keys=\'+document.getElementById(\'search_field\').value;
                                       }
                                    }"
                                 />
                            </td>
                            <td>
                                <input value="Search"  type="button" class="SearchBtn" 
                                    onclick="
                                    if(document.getElementById(\'search_field\').value == \'\'  || document.getElementById(\'search_field\').value == \'What are you looking for?\'){
                                            //do nothing..
                                        }else{
                                            window.location = \'?q=ntlp/resource/library/search'.$cat.'&keys=\'+document.getElementById(\'search_field\').value; 
                                          }" />
                            </td>
                        </tr>
                    </table>
                        <div style="clear: both;"></div>
                        <div style="font-size:2px">&nbsp;</div>
                        <div>
                          <!-- '.$example_subject.' -->
                        </div>
                    </div>
                    <div style="font-size:10px">&nbsp;</div>
              </div></div></div></div></div></div></div></div></div></div><div style="font-size:10px">';

    return $output;
}
