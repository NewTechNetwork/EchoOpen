<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


drupal_add_js(drupal_get_path('module', 'ntlp_group').'/filter_functions.js');

/*
 * @author Muhammad Mahad Azad
 *
 * @param $options (Array)
 *      It is a complex array which contains filter option configuration see example below.
 * @param $options['#json'] (Array)
 *      To pass extra JSON parameters
 * @param $options['#ajaxCallback'] (String: javascript function)
 *      This will be called when the ajax has successfully run.
 * @param $options['#autoUncheckDefault'] (Boolean)
 *      This parameter determines whether to uncheck the default
 *      option when a non default option is clicked
 * @param $optionCell (Array)
 *      $optionCell[start] (String), $optionCell[end] (String)
 *      start:  It is rendered before each filter option start.
 *      end:    It is rendered after each filter option start.
 * @param $headingStart (Array)
 *      $headingStart[template] (String), $headingStart[placeholder]
 *      template:   It is rendered before each row is begin. You can place a placeholder here
 *                  whose value will be replaced by the Row heading.
 *      placeholder:    It is an identifier for replacing the Row heading.
 * @param $rowEnd (String)
 *      It is rendered when a row completes.
 * @param $url (String)
 *      Url for ajax call. Where all the options will be POSTed.
 * @param $domId (String)
 *      DOM ID of an element whose inner html will be replaced by the response of the
 *      AJAX call.
 */
function render_custom_filter( $options, $optionCell, $headingStart, $rowEnd, $url, $domId ) {
    static $filterNo = 1;
    $filterNo++;
    
    if (!isset($options['#autoUncheckDefault'])) {
        $defaultOptions = array(
            '#autoUncheckDefault' => true,
        );
        $options = array_merge($defaultOptions, $options);
    }
    /*

    Example:
    --------

    $options = array(
        'Row Heading One' => array(
            'All' => array(
                'type'=>'checkbox',
                'group'=>'rowOne',
                'value' => 1,
                'name' => 'all',
            ),
            'Value Two' => array(
                'type'=>'checkbox',
                'group'=>'rowOne',
                'value' => 'hello',
                'name' => 'value_two',
            ),
        ),
        'Row Heading Two' => array(
            'Hello World' => array(
                'type'=>'checkbox',
                'group'=>'rowTwo',
                'value' => true,
                'name' => 'hello',
            ),
            'Value Two' => array(
                'type'=>'checkbox',
                'group'=>'rowTwo',
                'value' => 'name as integer zero',
                'name' => 0,
            ),
            'Value Three' => array(
                'type'=>'checkbox',
                'group'=>'rowTwo',
                'value' => 'name as integer one',
                'name' => 1,
            ),
        ),

        VALUES THAT WILL BE POSTED:
            Array(
                rowOne => Array(
                    all => 1,
                    value_two => hello
                ),
                rowTwo => Array(
                    hello => true,
                    0     => name as integer zero,
                    1     => name as integer one
                ),
            )
    );
    */

    $extraJson = '';
    if( isset($options['#json']) ) {
        foreach( $options['#json'] as $prop => $value ) {
            $extraJson .= "\t\t jsonData['$prop'] = $value;\n";
        }
        unset($options['#json']);
    }
    $autoUncheckDefault = $options['#autoUncheckDefault'];
    unset($options['#autoUncheckDefault']);

    $ajaxCallback = '';
    if(isset($options['#ajaxCallback'])) {
        $ajaxCallback = "({$options['#ajaxCallback']})($)";
        unset($options['#ajaxCallback']);
    }

    drupal_add_js("
        var jsonData = {};
        var url = '$url';
        var queryString = '';
        var filterAjaxCallbacks = {};
        function getReloadURL(value, url) {
            var data = value.split(':'); //[0] => type[opt], [1] => value
            var opt = data[0];
            var optValue = data[1];
            url = url.replace('@VALUE', optValue);
            return url;
        }
        function triggerFilter(keys, value) {
                queryString = '&' + keys + '=' + value;
                filterCalled{$filterNo}(null);
            }
        function filterCalled{$filterNo}(event) {
             $('.myFilter{$filterNo}').each(function(){
                var data = ($(this).val()).split(':'); //[0] => type[opt], [1] => value
                var opt = data[0];
                var optValue = data[1];
                delete jsonData[opt];

                if ( ( !($(this).is(':checkbox')) && !($(this).is(':radio')) ) || this.checked ) {
                    jsonData[opt] = {};
                    jsonData[opt] = optValue;
                }

             });
             $extraJson

             jsonData['cache'] = false;
             
             if (!$.browser.msie) {
                 if (typeof filterAjaxCallbacks.abort == 'function'){
                    filterAjaxCallbacks.abort();
                    filterAjaxCallbacks = {};
                 }
             }

             filterAjaxCallbacks = $.post(url + queryString, jsonData, function(responseData){
                $('#{$domId}').html(responseData);
                $ajaxCallback;

                //Execute pager jquery again to reset pager
                Drupal.behaviors.fixMyPager('#{$domId}');
                //Execute modalFrame jquery again to reset it of Join buttons
                Drupal.behaviors.modalFrameSetup();
             });
        }

        function uncheckDefaultOpt{$filterNo}( defaultOptClass ) {
            $('.'+defaultOptClass).attr('checked', false);
        }

        function uncheckNonDefaultOpts{$filterNo}( defaultOptClass, row ) {
            $('input.myFilter{$filterNo}.myFilterGroup'+row+'.isDefault_0').each(function(){
                $(this).attr('checked', false);
            });
        }

    ",'inline');

    $output = '';


  $row = 1;
  foreach( $options as $groupTitle => $opts ) {
        $selectAll = false;
        if (isset($opts['#SelectAll'])) {
            $selectAll = $opts['#SelectAll'];
//            unset($groupTitle['#SelectAll']);
        }

        if( $groupTitle == '#json' ) {continue;}
        $group_uncheck_default = $autoUncheckDefault;

        $output .= $headingStart['template'];
        $output  = str_replace($headingStart['placeholder'], $groupTitle, $output);
        foreach( $opts as $opt => $config ) {
            if ($opt == '#autoUncheckDefault') {
                $group_uncheck_default = $config['value'];
                continue;
            }

            $isDefault = false;
            if( isset($config['default']) and $config['default'] == true ) {
                $isDefault = true;
            }
//            print($opt.' uncheck val:'.($group_uncheck_default ? 1 : 0).' / default: '.($isDefault ? 1 : 0). '/'. ((!$isDefault && $group_uncheck_default) ? 1 : 0 ).'<br>');
            $output .= $optionCell['start'];

            //Fix this logic to uncheck all other non-default options
//                                onclick=\"". ((!$isDefault && $group_uncheck_default) ? "uncheckDefaultOpt{$filterNo}('flDflt{$filterNo}{$row}');" : ($isDefault ? '' : "uncheckNonDefaultOpts{$filterNo}('flDflt{$filterNo}{$row}', {$row});")) . "filterCalled{$filterNo}(event)\"

            if ($config['type'] == 'combobox') {

                $output .= $config['prefix'];

                $output .= "<label class=" . $config['class'] . "' style='" . $config['style'] . "'>
                                <select
                                    @HANDLER
                                    class=\"myFilter{$filterNo} myFilterGroup{$row}\"
                                    value=\"" . $config['value'] . ":" . $config['default'] . "\"
                                    style=\"" . $config['style'] . "\"
                                />";

                foreach( $config['options'] as $option_name => $option_value ) {

                    $option_selected = "";
                    if ($config['default'] == $option_value)
                        $option_selected = ' selected ';

                    $output .= "<option value=\"".$config['value'].":$option_value\" $option_selected >$option_name</option>";
                }

                $output .= '</select></label>';
                $output .= $config['suffix'];

                if (isset($config['reload_page'])) {
                    $output = str_replace('@HANDLER', "onchange=\"window.location=getReloadURL(this.value, '".$config['reload_page']."');\"", $output);
                } else {
                    $output = str_replace('@HANDLER', "onchange=\"filterCalled{$filterNo}(event)\"", $output);
                }
            }
            elseif ($config['type'] == 'checkbox') {
                $output .= "<label class='" . $config['class'] . "' style='" . $config['style'] . "'>
                            <input
                                    @HANDLER
                                class=\"myFilter{$filterNo} myFilterGroup{$row} isDefault_".($isDefault ? 1 : 0)." group_uncheck_default:". ($group_uncheck_default ? 1 : 0) . ' ' .($isDefault ? " flDflt{$filterNo}{$row} myFilter-default-opt" : '') ."\"
                                value=\"{$config['group']}[{$config['name']}]:{$config['value']}\"
                                    type=\"{$config['type']}\" " . (($isDefault || $selectAll) ? 'checked' : '') .
                            "/> $opt
                        </label>";

                $output = str_replace('@HANDLER', "onclick=\"". ((!$isDefault && $group_uncheck_default) ? "uncheckDefaultOpt{$filterNo}('flDflt{$filterNo}{$row}');" : ($isDefault ? "uncheckNonDefaultOpts{$filterNo}('flDflt{$filterNo}{$row}', {$row});" : '')) . "filterCalled{$filterNo}(event)\"", $output);
            }
            elseif ($config['type'] == 'radio') {
                $output .= "<label class='" . $config['class'] . "' style='" . $config['style'] . "'>
                            <input
                                    @HANDLER
                                name=" . $config['group'] . "
                                class=\"myFilter{$filterNo} myFilterGroup{$row} isDefault_".($isDefault ? 1 : 0).($isDefault ? " flDflt{$filterNo}{$row} myFilter-default-opt" : '') ."\"
                                value=\"{$config['group']}[{$config['name']}]:{$config['value']}\"
                                    type=\"{$config['type']}\" " . ($isDefault ? 'checked' : '') .
                            "/> $opt
                        </label>";

                $output = str_replace('@HANDLER', "onchange=\"filterCalled{$filterNo}(event)\"", $output);

            }

            $output .= $optionCell['end'];
        }
        $output .= $rowEnd;
        $row++;
    }

    $filterNo++;
    return $output;
}