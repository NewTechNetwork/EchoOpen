<?php

function ntlp_reports_getinfo($file_id, $output_format, $ajax = true) {
    $conf = variable_get('ntlp_reports', array());
    $host = $conf['report_server_ip'];
    $port = $conf['report_server_port'];
    
//    define ("JAVA_HOSTS", "{$host}:{$port}");
//    module_load_include('inc', 'ntlp_reports', 'java/Java');

    $filerec = get_file_detail($file_id);

//    //Provide Report XML (jrxml)
//    try {
//        $compileManager = new JavaClass("net.sf.jasperreports.engine.JasperCompileManager");
//        $report = $compileManager->compileReport(realpath('.') . '/' . $filerec->filepath);
//    } catch (Exception $e) {
//        $return_value = json_encode(array(
//            'status' => 0,
//            'message' => 'Report Compilation error on file: ' . $filerec->filename . ', Message:' . $e->getMessage(),
//        ));
//        if ($ajax)
//            echo $return_value;
//        else
//            return $return_value;
//        return;
//    }

    //Load parameters from database into an array
    $params_db = array();
//    $output_format = '';
    if (!$ajax) {
        if ($prs = get_report_parameters_byfile($file_id)) {
            foreach($prs as $pname => $prec) {
                $params_db[$pname] = $prec->source;
            }
//            if (isset($prec->output_format) && strlen($output_format) == 0)
//                $output_format = $prec->output_format;
        }
    }

    $params = "";
    $pdata = "";
    $subreport_count = 0;
    if ($output_format != 'SUB') {
        //Get Report Parameters from jrxml
        try {
            $jrxmldata = xml_parser_xml_to_array(realpath('.') . '/' . $filerec->filepath);
            foreach ($jrxmldata as $row => $value) {
                if (is_array($value)) {
                    if (isset($value['path']) && trim($value['path']) == ':jasperreport:parameter') {
                        if ($value['NAME'] == 'SUBREPORT_DIR') {
                            $subreport_count ++;
                        } else {
                            $is_source_ui = 1;
                            if (isset($params_db[$value['NAME']])) {
                                $is_source_ui = ($params_db[$value['NAME']] == 'U' ? 1 : 0);
                            }
                            $params .= '<input ' . ($is_source_ui == 1 ? 'checked="1"' : '') . '" type="checkbox" class="param-' . $file_id . '" onclick="onclick_report_param(this.checked, \'' . $output_format . '\', ' . $file_id . ', \'' . $value['NAME'] . '\', \'' . $value['CLASS'] . '\')">' . $value['NAME'] . '</input><br>';
                        }
                        $pdata .= $file_id . "," . $value['NAME'] . "," . $value['CLASS'] . ",{$is_source_ui};";
                    }
                }
            }
        } catch (Exception $e) {
            $return_value = json_encode(array(
                'status' => 0,
                'message' => 'Report Compilation error: ' .  $e->getMessage(),
            ));
            if ($ajax)
                echo $return_value;
            else
                return $return_value;
            return;
        }
    }
    $info = '';
    if (strlen($params) > 0) {
        $info = '<span class="pro_box"><span style="text-decoration:underline">Parameters</span><br />' . $params . '</span>';
    }
    $return_value = json_encode(array(
        'status' => 1,
        'message' => $info,
        'data' => $pdata,
        'subreports' => $subreport_count,
    ));
    if ($ajax)
        echo $return_value;
    else
        return $return_value;
}

function ntlp_reports_maintenance() {
    return drupal_get_form('ntlp_reports_maintenance_form');
}

function ntlp_reports_maintenance_form($form_state) {
    global $user, $base_path;
    include('report_main.template.inc');
   
    drupal_add_css(drupal_get_path('theme', 'Boldr') . '/StylishSelect/stylish-select.css', 'module', 'all', FALSE);
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/externalFile.js');
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/jquery.stylish-select.js');

    drupal_add_js(drupal_get_path('module', 'ntlp_reports') . '/ntlp_reports.js');

    $report_nid = 0;
    if (arg(2) != 'edit') {
        $report_title = 'Add Report';
    } else {
        $report_title = 'Edit Report';
        $report_nid = arg(3);

        if ($rnode = node_load($report_nid)) {
            $last_saved = ' Last Saved ' . date(DATE_FORMAT_LAST_SAVED, $rnode->changed) . '&nbsp;&nbsp;';
            $last_saved = '<span class="published_grade_box" style="padding: 4px !important;float:right !important;">' . $last_saved . '</span>';
            $rtitle = $rnode->title;
            $rvisibility = $rnode->status;
            $rcategory = $rnode->type;
            $notes = $rnode->body;
            if ($report_rec = get_report_basic($report_nid)) {
                $app_location = $report_rec->app_location;
                $rtype = $report_rec->type;
            }
        }
    }

    $form['add_report'] = array(
        '#tree' => TRUE, // Don't forget to set #tree!
        '#prefix' => str_replace('@REPORT_TITLE', $report_title, $HTMLform_report_hdr),
        '#suffix' => $HTMLform_report_ftr,
    );
    $form['add_report']['report_nid'] = array(
        '#type' => 'hidden',
        '#value' => $report_nid,
    );

    $form['add_report']['cancel_button'] = array(
        '#type' => 'submit',
        '#value' => t('Cancel'),
        '#prefix' => '<div style="position:absolute;top:0px;right:0px;width:390px;"><div style="float:right;width:80px;" >',
        '#suffix' => '</div>',
        '#attributes' => array('class' => 'CancelBtn'),
    );

    $form['add_report']['save_button'] = array(
        '#type' => 'submit',
//        '#submit' => array('ntlp_reports_maintenance_form_submit'),
        '#value' => t('Save'),
//        '#validate' => array('ntlp_reports_maintenance_form_validate'),
        '#prefix' => '<div style="float:right;width:80px;" >',
        '#suffix' => '</div>' . $last_saved . '</div>',
        '#attributes' => array('class' => 'PostBtn'),
    );

    $form['add_report']['title'] = array(
        '#type' => 'textfield',
        '#default_value' => $rtitle,
        '#maxlength' => 50,
        '#prefix' => '<tr><th style="width: 139px;">Title*</th><td style="width:680px"><div style="float:left;">',
        '#suffix' => '</div><div style="float: left; margin-top: 7px;">&nbsp;Max characters = 50</div></td></tr>',
    );

    $form['add_report']['visibility'] = array(
        '#type' => 'checkbox',
        '#default_value' => $rvisibility,
        '#prefix' => '<tr><th style="width: 139px;">Visibility</th><td><span class="pro_box">A report must be set to “show” for it to be accessed by users.</span>',
        '#suffix' => '</td></tr>',
        '#title' => 'Show',
    );

    $option = array("ntlp_report_network" => "Network", "ntlp_report_school" => "School");

    $form['add_report']['category'] = array(
        '#type' => 'select',
        '#options' => $option,
        '#prefix' => '<tr><th style="width: 139px;">Type*</th><td><div><span style="width: 346px;" class="pro_box">Network reports show data across all schools. School reports show data on one selected school.</span>',
        '#suffix' => '</div></td></tr>',
        '#attributes' => array('style' => 'width:200px;'),
        '#default_value' => $rcategory,
    );

//    $option = array("M" => "Main Report", "S" => "Sub-Report");
//
//    $form['add_report']['type'] = array(
//        '#type' => 'select',
//        '#options' => $option,
//        '#prefix' => '<tr><th style="width: 139px;">Type*</th><td><div><span style="width: 346px;" class="pro_box">Main reports can run from the application. Sub-Reports will only be used with Main reports.</span>',
//        '#suffix' => '</div></td></tr>',
//        '#attributes' => array('style' => 'width:200px;'),
//        '#default_value' => $rtype,
//    );

    $form['add_report']['app_location'] = array(
        '#type' => 'select',
        '#options' => get_app_locations(),
        '#default_value' => $app_location,
        '#prefix' => '<tr><th style="width: 139px;">App Location*</th><td><div><span class="pro_box">Where users will access and run the report.</span>',
        '#suffix' => '</div></td></tr>',
        '#attributes' => array('style' => 'width:260px;'),
    );

   
    $form['#fileupload_embed'] = true;
    $form['#fileupload_auto_upload'] = true;

    $hiddendata = '';
    $attached_files_data = '';
    if (isset($rnode) && $rnode != null) {
        $file_path = '';
        if ($rep = get_report_info($report_nid, 'PDF')) {
            $file_path = $rep->filepath;
            $hiddendata .= 'F,' . $rep->fid . ',' . $rep->filename . ';';
            $file_params_json = json_decode(ntlp_reports_getinfo($rep->fid, 'PDF', false));

            if ($file_params_json->status == 1) {
                $params_hiddendata = $file_params_json->data;
                $file_params = $file_params_json->message;
                $attached_files_data = '<div style="display:table;margin:2px;width:100%" id="F_'.$rep->fid.'">'
                . ' <a style="margin-top:2px;vertical-align:top;" onclick="delete_attach(\'PDF\', \'F_'.$rep->fid.'\');"><img width="14" height="14" src="'.$base_path.'themes/Boldr/Images/DeleteBtn.png" border="2" /></a>'
                . ' &nbsp;&nbsp;<a href="' . $rep->filepath . '">' . $rep->filename . '</a></div>'
                . ' <div style="display:inline-block;float:left;margin-top:10px;">' . $file_params . '</div>';
            }
        }
    }

    $attach_fileupload_link = '
     <div id="PDF_attach_links_panel" style="cursor: pointer !important; clear:both;' . (empty($attached_files_data) ? '' : 'display:none;') . '">
     <div onmouseover="report_format_uploader = \'PDF\';place_fileupload_control(\'edit-add-report-PDF-layout\', this);" onmouseout="place_fileupload_control_mouseout(this);" >
       <div class="clearfix">
          <span id="attach_link_prompt"><span style="color:#000">+&nbsp;</span>
          <a id="attach_link" class="project_resource_Green_text">Upload layout file</a>
          <div id="attach_link_suffix" style="padding-left:60px;float:right">Format = jrxml</div>
          </span>
       </div>
       <span id="PDF_attach_link_tip" class="pro_box">Upload the WEB/PDF layout.</span>
     </div></div>
     <div id="PDF_attach_links" style="width:auto;">' . $attached_files_data . '</div></td></tr>';

    $form['add_report']['PDF_layout'] = array(
        '#fileupload_path' => get_path_reportsuploads(),
        '#fileupload_replace' => TRUE,
        '#prefix' => '<tr>
            <th style="width:139px; vertical-align:top; padding:10px 10px 10px 0;" >' . t('Primary Layout*') . '</th>
            <td style="background-color:#cfe5f3;cursor:default !important">' . $attach_fileupload_link,
        '#suffix' => '</td></tr>',
    );
    $form['add_report']['PDF_attached_files_hidden'] = array(
        '#type' => 'hidden',
        '#value' => $hiddendata,
        '#id' => 'PDF_attached_files_hidden',
    );
    $form['add_report']['PDF_params_hidden'] = array(
        '#type' => 'hidden',
        '#value' => $params_hiddendata,
        '#id' => 'PDF_params_hidden',
    );

    $hiddendata = '';
    $attached_files_data = '';
    if (isset($rnode) && $rnode != null) {
        $file_path = '';
        if ($rep = get_report_info($report_nid, 'CSV')) {
            $file_path = $rep->filepath;
            $hiddendata .= 'F,' . $rep->fid . ',' . $rep->filename . ';';
            $file_params_json = json_decode(ntlp_reports_getinfo($rep->fid, 'CSV', false));
            
            if ($file_params_json->status == 1) {
                $params_hiddendata = $file_params_json->data;
                $file_params = $file_params_json->message;
                $attached_files_data = '<div style="display:table;margin:2px;width:100%" id="F_'.$rep->fid.'">'
                . ' <a style="margin-top:2px;vertical-align:top;" onclick="delete_attach(\'CSV\', \'F_'.$rep->fid.'\');"><img width="14" height="14" src="'.$base_path.'themes/Boldr/Images/DeleteBtn.png" border="2" /></a>'
                . ' &nbsp;&nbsp;<a href="' . $rep->filepath . '">' . $rep->filename . '</a></div>'
                . ' <div style="display:inline-block;float:left;margin-top:10px;">' . $file_params . '</div>';
            }
        }
    }

    $attach_fileupload_link = '
     <div id="CSV_attach_links_panel" style="cursor: pointer !important; clear:both;' . (empty($attached_files_data) ? '' : 'display:none;') . '">
     <div onmouseover="report_format_uploader = \'CSV\';place_fileupload_control(\'edit-add-report-CSV-layout\', this);" onmouseout="place_fileupload_control_mouseout(this);" >
       <div class="clearfix">
          <span id="attach_link_prompt"><span style="color:#000">+&nbsp;</span>
          <a id="attach_link" class="project_resource_Green_text">Upload layout file</a>
          <div id="attach_link_suffix" style="padding-left:60px;float:right">Format = jrxml</div>
          </span>
       </div>
       <span id="CSV_attach_link_tip" class="pro_box">Upload CSV layout associated with this report.</span>
     </div></div>
     <div id="CSV_attach_links" style="width:auto;">' . $attached_files_data . '</div></td></tr>';
    
    $form['add_report']['CSV_layout'] = array(
        '#fileupload_path' => get_path_reportsuploads(),
        '#fileupload_replace' => TRUE,
        '#prefix' => '<tr>
            <th style="width:139px; vertical-align:top; padding:10px 10px 10px 0;" >' . t('CSV Layout') . '</th>
            <td style="background-color:#cfe5f3;cursor:default !important">' . $attach_fileupload_link,
        '#suffix' => '</td></tr>',
    );
    $form['add_report']['CSV_attached_files_hidden'] = array(
        '#type' => 'hidden',
        '#value' => $hiddendata,
        '#id' => 'CSV_attached_files_hidden',
    );
    $form['add_report']['CSV_params_hidden'] = array(
        '#type' => 'hidden',
        '#value' => $params_hiddendata,
        '#id' => 'CSV_params_hidden',
    );

    $hiddendata = '';
    $attached_files_data = '';
    if (isset($rnode) && $rnode != null) {
        $file_path = '';
        if ($rs = get_subreports($report_nid)) {
            while ($rep = db_fetch_object($rs)) {
                $file_path = $rep->filepath;
                $hiddendata .= 'F,' . $rep->fid . ',' . $rep->filename . ';';
                $file_params_json = json_decode(ntlp_reports_getinfo($rep->fid, 'SUB', false));

                if ($file_params_json->status == 1) {
                    $params_hiddendata = $file_params_json->data;
                    $file_params = $file_params_json->message;
                    $attached_files_data .= '<div style="display:table;margin:2px;width:100%" id="F_'.$rep->fid.'">'
                    . ' <a style="margin-top:2px;vertical-align:top;" onclick="delete_attach(\'SUB\', \'F_'.$rep->fid.'\');"><img width="14" height="14" src="'.$base_path.'themes/Boldr/Images/DeleteBtn.png" border="2" /></a>'
                    . ' &nbsp;&nbsp;<a href="' . $rep->filepath . '">' . $rep->filename . '</a></div>'
                    . ' <div style="display:inline-block;float:left;margin-top:10px;">' . $file_params . '</div>';
                }
            }
        }
    }

    $attach_fileupload_link = '
     <div id="SUB_attach_links_panel" style="cursor: pointer !important; clear:both;">
     <div onmouseover="report_format_uploader = \'SUB\';place_fileupload_control(\'edit-add-report-SUB-layout\', this);" onmouseout="place_fileupload_control_mouseout(this);" >
       <div class="clearfix">
          <span id="attach_link_prompt"><span style="color:#000">+&nbsp;</span>
          <a id="attach_link" class="project_resource_Green_text">Upload layout file</a>
          <div id="attach_link_suffix" style="padding-left:60px;float:right">Format = jrxml</div>
          </span>
       </div>
       <span id="SUB_attach_link_tip" class="pro_box">Upload Sub-Reports required by the Primary and CSV report layouts.</span>
     </div></div>
     <div id="SUB_attach_links" style="width:auto;">' . $attached_files_data . '</div></td></tr>';

    $form['add_report']['SUB_layout'] = array(
        '#fileupload_path' => get_path_reportsuploads(),
        '#fileupload_replace' => TRUE,
        '#prefix' => '<tr>
            <th style="width:139px; vertical-align:top; padding:10px 10px 10px 0;" >' . t('Sub-Reports') . '</th>
            <td style="background-color:#cfe5f3;cursor:default !important">' . $attach_fileupload_link,
        '#suffix' => '</td></tr>',
    );
    $form['add_report']['SUB_attached_files_hidden'] = array(
        '#type' => 'hidden',
        '#value' => $hiddendata,
        '#id' => 'SUB_attached_files_hidden',
    );
    $form['add_report']['SUB_params_hidden'] = array(
        '#type' => 'hidden',
        '#value' => $params_hiddendata,
        '#id' => 'SUB_params_hidden',
    );

    fileupload_get_control($form, $form_state, array('add_report', 'PDF_layout'));
    fileupload_get_control($form, $form_state, array('add_report', 'CSV_layout'));
    fileupload_get_control($form, $form_state, array('add_report', 'SUB_layout'));

    $form['add_report']['notes'] = array(
        '#type' => 'textarea',
        '#wysiwyg' => False,
        '#maxlength' => '200',
        '#prefix' => '<tr><th style="width: 139px;">Notes</th><td>',
        '#suffix' => '</td></tr>',
        '#default_value' => $notes,
    );

    return $form;
}

function ntlp_reports_maintenance_form_submit($form_state) {
    global $user;

//    print_r($_POST['add_report']);
//    print_r($form_state);
//    die();
    cache_set("ntlp_report:" . session_id(), null, 'cache', CACHE_TEMPORARY);

    $rtitle = $_POST['add_report']['title'];
    $rvisibility = $_POST['add_report']['visibility'];
    $rcategory = $_POST['add_report']['category'];
    $rtype = $_POST['add_report']['type'];
    $applocation = $_POST['add_report']['app_location'];
    $notes = $_POST['add_report']['notes'];

    if (isset($_POST['add_report']['report_nid'])) {
        $report_nid = $_POST['add_report']['report_nid'];
    }

    if (isset($report_nid) && $report_nid > 0) {
        $report_node = node_load($report_nid);
    } else {
        $report_node = new stdClass();
        $report_node->created = time();
        $report_node->sticky = 0;
        $report_node->format = 1;       // Filtered HTML
        $report_node->comment = 0;
    }
    $report_node->title = $rtitle;
    $report_node->type = $rcategory;   // Your specified content type
    $report_node->body = $notes;
    $report_node->status = $rvisibility;
    $report_node->uid = $user->uid;

    node_save($report_node);
//    drupal_set_message('Saved Node:' . print_r($report_node, true) . ', Report NID from param:' . $report_nid);

//    drupal_set_message('report node saved with nid='.$report_nid.' type='.$rcategory);
    if (isset($report_nid) && $report_nid > 0) {
//        echo 'Old:'.print_r($report_nid, true);
//        die();
        $query = db_query("UPDATE {ntlp_report} SET type = '%s', app_location = '%s' WHERE nid = %d",
                    $rtype, $applocation, $report_node->nid);
    } else {
//        echo 'New:'.print_r($report_nid, true);
//        die();
        $query = db_query("INSERT INTO {ntlp_report} (nid, type, app_location)
            VALUES(%d, '%s', '%s')", $report_node->nid, $rtype, $applocation);
    }

    $attachmentdata = $_POST['add_report']['PDF_attached_files_hidden'];
    save_report_attachments($report_node->nid, 'PDF', $attachmentdata);
//    echo '<br>Params:'.print_r($_POST['add_report']['PDF_params_hidden'], true);
//    die();
    $paramdata = $_POST['add_report']['PDF_params_hidden'];
    save_report_parameters($report_node->nid, 'PDF', $paramdata);

    $attachmentdata = $_POST['add_report']['CSV_attached_files_hidden'];
    save_report_attachments($report_node->nid, 'CSV', $attachmentdata);
    $paramdata = $_POST['add_report']['CSV_params_hidden'];
    save_report_parameters($report_node->nid, 'CSV', $paramdata);

    $attachmentdata = $_POST['add_report']['SUB_attached_files_hidden'];
    save_report_attachments($report_node->nid, 'SUB', $attachmentdata);

    drupal_goto('ntlp/reports/manage');
}

function save_report_attachments($report_nid, $output_type, $attachmentdata) {
    if (isset($attachmentdata) && !empty($attachmentdata)) {
        $linksarray = get_attached_files($attachmentdata);
        //watchdog("attach_data", print_r($linksarray, true));

        foreach ($linksarray as $l) {
            list($ftype, $file_id, $file_name) = $l;
            //The attachment of Type==X will be overwritten by a valid resource.
            //To delete an attachment, resource should be deleted.
            if (!empty($ftype)) {
                if ($ftype == 'X') {
//                    echo '<br>Delete:'.$file_id;
                    $query = db_query("DELETE FROM {ntlp_report_attachment} WHERE nid = %d AND fid = %d AND output_format = '%s'",
                                $report_nid, abs($file_id), $output_type);
                } else {
                    if ($file_id < 0) {
//                        echo '<br>Insert:'.$file_id;
                        $query = db_query("INSERT INTO {ntlp_report_attachment} (nid, output_format, fid)
                            VALUES(%d, '%s', %d)", $report_nid, $output_type, abs($file_id));
                        }
                }
            }
        }
    }
}

function save_report_parameters($report_nid, $output_type, $paramdata) {
    if (isset($paramdata) && !empty($paramdata)) {
        $linksarray = get_attached_files($paramdata);
        //watchdog("attach_data", print_r($linksarray, true));

        //Delete all params first
        $query = db_query("DELETE FROM {ntlp_report_param} WHERE report_nid = %d AND output_format = '%s'",
            $report_nid, $output_type);

        //Insert them now in an array (to remove any duplicates)
        $params = array();
        foreach ($linksarray as $l) {
            list($file_id, $pname, $pclass, $insert) = $l;
            //The attachment of Type==X will be overwritten by a valid resource.
            //To delete an attachment, resource should be deleted.
            if (!empty($file_id)) {
                if (intval($insert) == 1) {
                    $source = 'U';      //UI
                } else {
                    $source = 'E';      //Environment
                }
                $params[$pname] = array('datatype' => $pclass, 'source' => $source);
            }
        }
        //Insert them in database now from array
        foreach ($params as $p => $values) {
            $query = db_query("INSERT INTO {ntlp_report_param} (report_nid, output_format, name, datatype, source, default_value)
                VALUES(%d, '%s', '%s', '%s', '%s', '%s')", $report_nid, $output_type, $p, $values['datatype'], $values['source'], '');
        }
    }
}

function get_attached_files($hiddendata) {
    $array = split(';', $hiddendata);

    foreach ($array as $element) {
        if (!empty($element)) {
            $link[] = explode(',', trim($element));
        }
    }
    return $link;
}

function ntlp_reports_viewer_header_form($report_nid, $has_csv, $report_title) {
    print drupal_get_form('ntlp_reports_viewer_header', $report_nid, $has_csv, $report_title);
}
function ntlp_reports_viewer_header($form_state, $report_nid, $has_csv, $report_title) {
    global $base_path;

    $form['logo'] = array(
        '#type' => 'item',
        '#value' => '<img src="' . $base_path . 'themes/Boldr/Images/echo_logo_appheader_new.jpg" alt="logo">',
    );
    $form['title'] = array(
        '#type' => 'item',
        '#value' => $report_title,
    );
    $form['report_nid'] = array(
        '#type' => 'hidden',
        '#value' => $report_nid,
    );
    $form['has_csv'] = array(
        '#type' => 'hidden',
        '#value' => $has_csv,
    );
    return $form;
}
function template_preprocess_ntlp_reports_viewer_header(&$variables) {
    $form = $variables['form'];
    $variables['logo'] = drupal_render($form['logo']);
    $variables['report_nid'] = intval($form['report_nid']['#value']);
    $variables['has_csv'] = intval($form['has_csv']['#value']);
    $variables['title'] = $form['title']['#value'];
}

function ntlp_reports_viewer_form($report_nid, $format) {
    print drupal_get_form('ntlp_reports_viewer', $report_nid, $format);
}

function ntlp_reports_viewer($form_state, $report_nid, $format) {
    global $base_path;

    $form['header'] = array(
        '#type' => 'fieldset',
    );
    if ($rnode = node_load($report_nid)) {
        if ($rnode->status == 1) {

            $report_title = $rnode->title;

            $form['content'] = array(
                '#type' => 'fieldset',
            );
            $form['content']['report'] = array(
                '#type' => 'item',
            );
            if (get_report_info($report_nid, 'CSV')) {
                $form['header']['has_csv'] = array('#type' => 'item', '#value' => 1);
            } else {
                $form['header']['has_csv'] = array('#type' => 'item', '#value' => 0);
            }
        } else {
            $report_title = "Report is no longer available.";
        }
    } else {
        $report_title = "Report not found";
    }
    $form['header']['title'] = array(
        '#type' => 'hidden',
        '#value' => $report_title,
    );
    $form['report_nid'] = array(
        '#type' => 'hidden',
        '#value' => $report_nid,
    );
    $form['output_format'] = array(
        '#type' => 'hidden',
        '#value' => $format,
    );
    return $form;
}

function template_preprocess_ntlp_reports_viewer(&$variables) {
    global $base_root;
    $form = $variables['form'];
    $variables['report_nid'] = intval($form['report_nid']['#value']);
    $variables['has_csv'] = intval($form['header']['has_csv']['#value']);
    $variables['title'] = $form['header']['title']['#value'];
//    $variables['url'] = str_replace('ntlp/reports/', 'ntlp/reports/show/', $base_root . request_uri());

    if (!isset($_SESSION['report_path']) || empty($_SESSION['report_path'])) {
        ntlp_reports_process($form['report_nid']['#value'], $form['output_format']['#value'], false);
    }
    if (isset($_SESSION['report_path']) && !empty($_SESSION['report_path'])) {
        $variables['url'] = $_SESSION['report_path'];
    } else {
        $variables['url'] = "#";
    }
    unset($_SESSION['report_path']);
}

// Input Report parameters
function ntlp_reports_inputform($report_nid, $output_format) {
    modalframe_child_js();
    return drupal_get_form('ntlp_reports_inputform_form', $report_nid, $output_format);
}

function ntlp_reports_inputform_form($form_state, $report_nid, $output_format) {
    global $user, $base_path;
    
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/externalFile.js');
    drupal_add_js(drupal_get_path('theme', 'Boldr') . '/StylishSelect/jquery.stylish-select.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_reports') . '/ntlp_reports.js');
    drupal_add_js(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/date_picker.js');
    
    drupal_add_css(drupal_get_path('theme', 'Boldr') . '/StylishSelect/stylish-select.css', 'module', 'all', FALSE);
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/jquery.ui.datepicker.css', 'module', 'all', FALSE);
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/demos.css', 'module', 'all', FALSE);
    drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/jquery.ui.all.css', 'module', 'all', FALSE);
    jquery_ui_add(array('ui.dialog', 'ui.draggable', 'ui.datepicker'));

    $form = array();
    ahah_helper_register($form, $form_state);

    if ($rnode = node_load($report_nid)) {
        $rtitle = $rnode->title;
        $rvisibility = $rnode->status;
        $rcategory = $rnode->type;
        $notes = $rnode->body;
        if ($report_rec = get_report_basic($report_nid)) {
            $app_location = $report_rec->app_location;
            $rtype = $report_rec->type;
        }
    }

    //Get parameters from environment
    $school_nid = get_this_school();
    if ($school_nid == 0) {
        $school_name = "All Schools";
    } else {
        $school_name = get_school_name($school_nid);
    }
    if (!$user_rec = get_user($uid)) {
        $user_rec = new stdClass();     //To suppress errors when using this object below
    }

    //Init Report parameters array
    $report_params = array();
    $report_params['school_nid'] = $school_nid;
    $report_params['school_name'] = $school_name;
    $report_params['user_uid'] = $user->uid;
    $report_params['user_fullname'] = $user_rec->first_name . ' ' . $user_rec->last_name;

    $form['inputform'] = array(
        '#type' => 'fieldset',
        '#tree' => TRUE, // Don't forget to set #tree!
    );
    $form['inputform']['report_nid'] = array(
        '#type' => 'hidden',
        '#value' => $report_nid,
    );
    $form['inputform']['output_format'] = array(
        '#type' => 'hidden',
        '#value' => $output_format,
    );

    $form['inputform']['title'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div class="div-popups-table">',
        '#suffix' => '</div>',
    );
    $form['inputform']['title']['title'] = array(
        '#type' => 'item',
        '#value' => $rtitle,
        '#prefix' => '<div class="div-popups-row"><div class="div-popups-first-cell">Report Title</div><div class="div-popups-second-cell">',
        '#suffix' => '</div></div>',
    );
    $form['inputform']['title']['notes'] = array(
        '#type' => 'item',
        '#value' => $notes,
        '#prefix' => '<div class="div-popups-row"><div class="div-popups-first-cell">Report Notes</div><div class="div-popups-second-cell">',
        '#suffix' => '</div></div>',
    );
    $form['inputform']['title']['school_name'] = array(
        '#type' => 'item',
        '#value' => $school_name,
        '#prefix' => '<div class="div-popups-row"><div class="div-popups-first-cell">School Name</div><div class="div-popups-second-cell">',
        '#suffix' => '</div></div>',
    );
    $form['inputform']['wrapper'] = array(
        '#type' => 'item',
        '#prefix' => '<div id="params-wrapper">',
        '#suffix' => '</div>',
    );
    $form['inputform']['wrapper']['params'] = array(
        '#type' => 'item',
        '#prefix' => '<div class="div-popups-table">',
        '#suffix' => '</div>',
    );

    //Define form UI elements for each paramater
    if ($prs = get_report_parameters($report_nid, $output_format)) {
        $pname = 'school_nid';
        if (array_key_exists($pname, $prs)) {
            //Get default value
            if (isset($form_state['storage']['inputform']['wrapper']['params'][$pname])) {
                $default_value = $form_state['storage']['inputform']['wrapper']['params'][$pname];
            } else {
                $default_value = $report_params[$pname];
            }
            //Set var for use in other dependent elements
            $report_params[$pname] = $default_value;
            
            //Display user control if UI source is 'U'
            if ($prs[$pname]->source == 'U') {
                //Get Options
                $options = array();
                $rs = get_schools();
                while ($rec = db_fetch_object($rs)) {
                    $options[$rec->nid] = (strlen($rec->school_name) >= 40 ? substr($rec->school_name, 0, 40) . '...' : $rec->school_name);
                }
                //Define form element
                $form['inputform']['wrapper']['params'][$pname] = array(
                    '#type' => 'select',
                    '#options' => $options,
                    '#maxlength' => '200',
                    '#prefix' => '<div class="div-popups-row"><div class="div-popups-first-cell">School</div><div class="div-popups-second-cell">',
                    '#suffix' => '</div></div>',
                    '#default_value' => $default_value,
                    '#ahah' => array(
                        'path' => ahah_helper_path(array('inputform', 'wrapper', 'params')),
                        'progress' => array('type' => 'none'),
                        'wrapper' => 'params-wrapper',
                    ),
                );
            } else {
                $form['inputform']['wrapper']['params'][$pname] = array(
                    '#type' => 'hidden',
                    '#value' => $default_value,
                );
            }
            //Remove item from array
            unset($prs[$pname]);
        }

        $pname = 'school_name';
        if (array_key_exists($pname, $prs)) {
            //Get default value
            if (isset($form_state['storage']['inputform']['wrapper']['params'][$pname])) {
                $default_value = $form_state['storage']['inputform']['wrapper']['params'][$pname];
            } else {
                $default_value = get_school_name($report_params['school_nid']);
            }
            //Set var for use in other dependent elements
            $report_params[$pname] = $default_value;

            //Display user control if UI source is 'U'
            if ($prs[$pname]->source == 'U') {
                //Define form element
                $form['inputform']['wrapper']['params'][$pname] = array(
                    '#type' => 'item',
                    '#prefix' => '<div class="div-popups-row"><div class="div-popups-first-cell">School Name</div><div class="div-popups-second-cell">',
                    '#suffix' => '</div></div>',
                    '#value' => $default_value,
                );
            } else {
                $form['inputform']['wrapper']['params'][$pname] = array(
                    '#type' => 'hidden',
                    '#value' => $default_value,
                );
            }            //Remove item from array
            unset($prs[$pname]);
        }

        $pname = 'school_year';
        if (array_key_exists($pname, $prs)) {
            //Get default value
            if (isset($form_state['storage']['inputform']['wrapper']['params'][$pname])) {
                $default_value = $form_state['storage']['inputform']['wrapper']['params'][$pname];
            } else {
                $default_value = get_this_school_year();        //get_school_year($report_params['school_nid']);
            }
            //Set var for use in other dependent elements
            $report_params[$pname] = $default_value;

            //Display user control if UI source is 'U'
            if ($prs[$pname]->source == 'U') {
                //Get Options
                $options = array();
                $options = _get_all_school_years($report_params['school_nid']);
                //Define form element
                $form['inputform']['wrapper']['params'][$pname] = array(
                    '#type' => 'select',
                    '#options' => $options,
                    '#maxlength' => '200',
                    '#prefix' => '<div class="div-popups-row"><div class="div-popups-first-cell">School Year</div><div class="div-popups-second-cell">',
                    '#suffix' => '</div></div>',
                    '#default_value' => $default_value,
                    '#ahah' => array(
                        'path' => ahah_helper_path(array('inputform', 'wrapper', 'params')),
                        'progress' => array('type' => 'none'),
                        'wrapper' => 'params-wrapper',
                    ),
                );
            } else {
                $form['inputform']['wrapper']['params'][$pname] = array(
                    '#type' => 'hidden',
                    '#value' => $default_value,
                );
            }
            //Remove item from array
            unset($prs[$pname]);
        }

        $pname = 'school_term_tid';
        if (array_key_exists($pname, $prs)) {
            //Get default value
            if (isset($form_state['storage']['inputform']['wrapper']['params'][$pname])) {
                $default_value = $form_state['storage']['inputform']['wrapper']['params'][$pname];
            } else {
                $rs = get_current_school_term($report_params['school_nid'], $report_params['school_year']);
                if ($rec = db_fetch_object($rs))
                    $default_value = $rec->school_term_tid;
                else
                    $default_value = 0;
            }
            //Set var for use in other dependent elements
            $report_params[$pname] = $default_value;
            watchdog('rp', 'school_term_tid='.$default_value);

            //Display user control if UI source is 'U'
            if ($prs[$pname]->source == 'U') {
                //Get Options
                $options = array();
                $rs = get_school_terms($report_params['school_nid'], $report_params['school_year']);
                while ($rec = db_fetch_object($rs)) {
                    $options[$rec->tid] = (strlen($rec->name) >= 40 ? substr($rec->name, 0, 40) . '...' : $rec->name);
                }
                //Define form element
                $form['inputform']['wrapper']['params'][$pname] = array(
                    '#type' => 'select',
                    '#options' => $options,
                    '#maxlength' => '200',
                    '#prefix' => '<div class="div-popups-row"><div class="div-popups-first-cell">Term</div><div class="div-popups-second-cell">',
                    '#suffix' => '</div></div>',
                    '#default_value' => $default_value,
                    '#ahah' => array(
                        'path' => ahah_helper_path(array('inputform', 'wrapper', 'params')),
                        'progress' => array('type' => 'none'),
                        'wrapper' => 'params-wrapper',
                    ),
                );
            } else {
                $form['inputform']['wrapper']['params'][$pname] = array(
                    '#type' => 'hidden',
                    '#value' => $default_value,
                );
            }
            //Remove item from array
            unset($prs[$pname]);
        }

        $pname = 'school_term_name';
        if (array_key_exists($pname, $prs)) {
            //Get default value - Get Name type parameters cannot use form-state - storage array
            $rec = get_school_term_name($report_params['school_term_tid']);
            $default_value = $rec->name;
            
            //Set var for use in other dependent elements
            $report_params[$pname] = $default_value;

            //Display user control if UI source is 'U'
            if ($prs[$pname]->source == 'U') {
                //Define form element
                $form['inputform']['wrapper']['params'][$pname] = array(
                    '#type' => 'item',
                    '#prefix' => '<div class="div-popups-row"><div class="div-popups-first-cell">Term Name</div><div class="div-popups-second-cell">',
                    '#suffix' => '</div></div>',
                    '#value' => $default_value,
                );
            } else {
                $form['inputform']['wrapper']['params'][$pname] = array(
                    '#type' => 'hidden',
                    '#value' => $default_value,
                );
            }
            //Remove item from array
            unset($prs[$pname]);
        }

        $pname = 'school_curriculum_unit_tid';
        if (array_key_exists($pname, $prs)) {
            //Get default value
            if (isset($form_state['storage']['inputform']['wrapper']['params'][$pname])) {
                $default_value = $form_state['storage']['inputform']['wrapper']['params'][$pname];
            } else {
                $default_value = '';
            }

            //Set var for use in other dependent elements
            $report_params[$pname] = get_topic_unit_byname($default_value);
            
            //Display user control if UI source is 'U'
            if ($prs[$pname]->source == 'U') {
                $form['inputform']['wrapper']['params'][$pname] = array(
                    '#type' => 'textfield',
                    '#default_value' => $default_value,
                    '#size' => '30',
                    '#prefix' => '<div class="div-popups-row"><div class="div-popups-first-cell">Curriculum Unit</div><div class="div-popups-second-cell">',
                    '#suffix' => '</div></div>',
                    '#autocomplete_path' => 'ntlp/unit/autocomplete/' . $report_params['school_nid'],    //MODIFY THE autocomplete FUNCTION TO GET NODE TYPE AND RETURN TERMS FOR SCHOOL OR COURSE
                );
            } else {
                $form['inputform']['wrapper']['params'][$pname] = array(
                    '#type' => 'hidden',
                    '#value' => $default_value,
                );
            }
            //Remove item from array
            unset($prs[$pname]);
        }

        $parr = array('todays_date', 'from_date', 'to_date');
        foreach ($parr as $pname) {
            if (array_key_exists($pname, $prs)) {
                //Get default value
                if (isset($form_state['storage']['inputform']['wrapper']['params'][$pname])) {
                    $default_value = $form_state['storage']['inputform']['wrapper']['params'][$pname];
                } else {
                    $default_value = date('m/d/Y', time());
                }

                //Set var for use in other dependent elements
                $report_params[$pname] = $default_value;

                //Display user control if UI source is 'U'
                if ($prs[$pname]->source == 'U') {
                    switch ($pname) {
                        case 'from_date': $prompt = 'From Date'; break;
                        case 'to_date': $prompt = 'To Date'; break;
                        default: $prompt = 'For Date';
                    }
                    $form['inputform']['wrapper']['params'][$pname] = array(
                        '#type' => 'textfield',
                        '#default_value' => $default_value,
                        '#size' => '10',
                        '#prefix' => '<div class="div-popups-row"><div class="div-popups-first-cell">' . $prompt . '</div><div class="div-popups-second-cell">',
                        '#suffix' => '</div></div>',
                        '#attributes' => array('class' => 'ntlp_datepicker', 'style' => 'padding-left:2px;padding-right:0px;'),
                    );
                } else {
                    $form['inputform']['wrapper']['params'][$pname] = array(
                        '#type' => 'hidden',
                        '#value' => $default_value,
                    );
                }
                //Remove item from array
                unset($prs[$pname]);
            }
        }

        //Render other custom parameters
        foreach($prs as $pname => $prec) {
            if ($pname != "SUBREPORT_DIR") {
                if (isset($form_state['storage']['inputform']['wrapper']['params'][$pname])) {
                    $default_value = $form_state['storage']['inputform']['wrapper']['params'][$pname];
                }
                //Define form element
                $form['inputform']['wrapper']['params'][$pname] = array(
                    '#type' => 'textfield',
                    '#maxlength' => '50',
                    '#prefix' => '<div class="div-popups-row"><div class="div-popups-first-cell">' . $pname . '</div><div class="div-popups-second-cell">',
                    '#suffix' => '</div></div>',
                    '#default_value' => $default_value,
                );
            }
        }
//        $form['inputform']['wrapper']['params']['debugahah'] = array(
//            '#type' => 'item',
//            '#value' => print_r($form_state, true),
//            '#prefix' => '<div class="div-popups-row"><div class="div-popups-first-cell">Report Notes</div><div class="div-popups-second-cell">',
//            '#suffix' => '</div></div>',
//        );
        $imagepath = drupal_get_path('module', 'modalframe') . '/images/loading.gif';
        $form['inputform']['runbutton'] = array(
            '#type' => 'item',
            '#value' => '<a href="#" class="SubmitBtnLong" onclick="on_runreport(' . $report_nid . ', \'' . $output_format . '\')">Run Report</a>',
            '#prefix' => '<div class="div-popups-row">',
            '#suffix' => '</div><div id="dialog-runningreport" style="background-color:#fff;display:none;">
                <p><div style="display:table-row"><div class="dialog-heading" style="padding-top:10px;display:table-cell;vertical-align:top;color:#3570AA;font-size:14px;font-weight:bold;">Running the report now...</div><div class="dialog-title-image" style="display:table-cell;"><img style="margin-left:20px;" src="' . $imagepath . '" /></div></div></p>
                <p><span class="msg" style="color:#000000;">Sit tight. This could take up to 1 minute to complete.</span></p>
                <p><a target="_blank" class="green_link" style="display:none;" onclick="parent.Drupal.modalFrame.close().closeDialog();">&gt;&nbsp;View it in a new browser tab</a></p></div>',
        );
    }

    return $form ;
}

function ntlp_reports_inputform_parseparams($report_nid, $output_format) {
    global $user, $base_path;
    watchdog('params', print_r($_POST, true));
//    watchdog('formstate', print_r($form_state, true));

//    $report_nid = $_POST['report_nid'];
//    $output_format = $_POST['output_format'];

    if ($rnode = node_load($report_nid)) {
        $rtitle = $rnode->title;
        $rvisibility = $rnode->status;
        $rcategory = $rnode->type;
        $notes = $rnode->body;
        if ($report_rec = get_report_basic($report_nid)) {
            $app_location = $report_rec->app_location;
            $rtype = $report_rec->type;
        }
    }

    //Get parameters from environment
    $school_nid = get_this_school();
    if ($school_nid == 0) {
        $school_name = "All Schools";
    } else {
        $school_name = get_school_name($school_nid);
    }
    if (!$user_rec = get_user($uid)) {
        $user_rec = new stdClass();     //To suppress errors when using this object below
    }

    //Init Report parameters array
    $report_params = "";
    $report_params .= "&school_nid=$school_nid";
    $report_params .= "&school_name=$school_name";
    $report_params .= "&user_uid=$user->uid";
    $report_params .= "&user_fullname=" . trim($user_rec->first_name . ' ' . $user_rec->last_name);

    //Define form UI elements for each paramater
    if (isset($_POST['inputform']['wrapper']['params'])) {
        foreach ($_POST['inputform']['wrapper']['params'] as $pname => $pvalue) {
            $report_params .= "&" . $pname . "=" . $pvalue;
        }
    }

    drupal_json(array(
        'status' => 1,
        'params' => $report_params,
    ));
//    return drupal_goto("ntlp/reports/{$report_nid}/{$output_format}", $report_params);
}