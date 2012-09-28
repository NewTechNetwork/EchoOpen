<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


//drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/fileupload_ahah_helper.js', 'footer');
//Files are first placed in this folder
//We should create year-wise sub-folders under it, for better organization and archival.
//File prefixes should be generated each time for each file
//@returns null if unable to create/find directory
function get_path_newuploads() {
//    global $base_path;
//    $dir = $_SERVER["DOCUMENT_ROOT"].$base_path;
    $dir = file_directory_path() . '/new_uploads/' . date('Ymd', time());
//    watchdog('dir', $dir);
    if (file_check_directory($dir, TRUE)) {
        return $dir;
    }
    return NULL;
}

function get_new_filename($filename) {
    $prefix = time();
    return '_' . $prefix . '_' . $filename;
}

//Use this function when you are building your form.
//$form_item = array of parent form items e.g. array('main_fieldset', 'group_fieldset')
function fileupload_get_control(&$form, $form_state, $form_item = FALSE, $valid_extentions = '', $multiple = false) {
    drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/fileupload_ahah_helper.js', 'footer');
//    $form = array('#attributes' => array('enctype' => 'multipart/form-data'));
//    watchdog('upload', 'Operation:' . print_r($form_state, true));
    //Following routine gets the reference to the last form item defined in form hierarchy
    $i = 0;
    $pform = array();
    $pform[$i] = &$form;
    $fs = $form_state['values'];
    $fpath = "";
    if ($form_item) {
        if (is_array($form_item)) {
            foreach ($form_item as $key) {
                $i++;
//                watchdog('path', 'Checking path:'.$key.'<br>'.print_r($pform[$i-1][$key], true));
                if (!isset($pform[$i - 1][$key])) {
                    watchdog('path', 'Fileupload: Invalid form item path');
                    return 'Invalid form item path';
                }
                $pform[$i] = &$pform[$i - 1][$key];
                $fs = $fs[$key];
                $fpath .= $key . "-";
            }
        } else {
            $i++;
            $pform[$i] = &$form[$form_item];
            $fs = $fs[$form_item];
        }
        $form_element_id = 'edit-' . implode('-', $form_item);
        $form_element_id = str_replace('_', '-', $form_element_id);
        $fpath = rtrim($fpath, "-");
    }

    $process_ahah = isset($fs['file_info']['attachment']);
//    watchdog('fileupload', $form_element_id . ": " . $fpath . ": " . ($process_ahah ? 1 : 0));

    $pform[$i]['file_info'] = array(
        '#type' => 'fieldset',
        '#prefix' => '<div id="'.$form_element_id.'-display-files-wrapper">', // This is our wrapper div
        '#suffix' => '</div>',
        '#tree' => TRUE, // don't forget to set #tree!
    );

    if (isset($form['#fileupload_embed'])) {
        if ($form['#fileupload_embed'])
            $embedded = true;
        else
            $embedded = false;
    }

    $fileupload_auto_upload = false;
    if (isset($form['#fileupload_auto_upload'])) {
        if ($form['#fileupload_auto_upload']) {
            $fileupload_auto_upload = true;
            auto_upload_align_attach_control($form_element_id, $multiple);
        }
    }

    $pform[$i]['file_info']['is_embedded'] = array(
        '#type' => 'hidden',
        '#value' => $embedded ? 1 : 0,
        '#id' => 'is_embedded',
    );
    $pform[$i]['file_info']['is_autoupload'] = array(
        '#type' => 'hidden',
        '#value' => $fileupload_auto_upload ? 1 : 0,
        '#id' => 'is_autoupload',
    );

    $fustatus = "";
    if ($process_ahah) {
        //This is a custom property, we used to tell the ahah_helper to exit without doing extensive
        //rendering of the page, which causes the jquery/drupal js routines to crash.
        $form['#fileupload'] = true;
        //Save file
        save_attachment($pform[$i]['file_info'], $fileupload_auto_upload ? 1 : 0, $pform[$i]['#fileupload_path'], $pform[$i]['#fileupload_replace']);
        $fid = $pform[$i]['file_info']['file_info_id']['#value'];
        $fname = addslashes($pform[$i]['file_info']['file_info_name']['#value']);
        $fustatus = $pform[$i]['file_info']['file_info_status']['#value'];

        if (!$embedded) {
            //Action buttons
            $pform[$i]['file_info']['file_actions'] = array(
                '#type' => 'item',
                '#prefix' => '<br clear="all" />
                                  <div>',
                '#suffix' => '</div>',
                '#tree' => TRUE,
            );
            $pform[$i]['file_info']['file_actions']['submit'] = array(
                '#type' => 'markup',
                '#value' => '<div style="float: left !important;">
                                    <input type="button" style="float: left !important;" id="btnSubmit" class="SearchBtn" onclick="file_onsuccess(' . $fid . ', \'' . $fname . '\')" value="Submit">
                                 </div>',
            );
            $pform[$i]['file_info']['file_actions']['cancel'] = array(
                '#type' => 'markup',
                '#value' => '<div style="float: left !important; margin-left: 3px;">
                                    <input type="button" style="float: left !important;" id="btnCancel" class="SearchBtn" onclick="file_cancel()" value="Cancel">
                                 </div>',
            );
        }
    }

//    if (strlen($fustatus) < 6 || substr($fustatus, 0, 6) == "ERROR:" || substr($fustatus, 0, 5) == "WARN:") {
    if ($fileupload_auto_upload) {
        $pform_control = 0;
    } else {
        $pform_control = $i;
    }
    $pform[$pform_control]['file_info']['resource_file'] = array(
        '#name' => 'files[resource_file]',
        '#id' => 'file-info-resource-file',
        '#type' => 'file',
        '#size' => ($fileupload_auto_upload == true) ? '15' : '30',
        '#title' => '',
        '#attributes' => ($fileupload_auto_upload == true) ? array('class' => 'hide') : '',
            //            '#access' => !isset($form_state['values']['attachment']),
    );

    $form_item_ext = array();
    if (is_array($form_item)) {
        $form_item_ext = $form_item;
        $form_item_ext[] = 'file_info';
    } else {
        if (empty($form_item))
            $form_item_ext = array('file_info');
        else
            $form_item_ext = array($form_item, 'file_info');
    }
//        watchdog('formitem', print_r($form_item_ext, true));

    $pform[$i]['file_info']['validextentions'] = array(
        '#value' => $valid_extentions,
        '#type' => 'hidden',
        '#prefix' => '<div id="dialog-fileupload-control-message" title="Error" style="background-color:#fff;display:none;">
            <p><span class="msg"></span></p>
        </div>',
    );
    $pform[$i]['file_info']['attachment'] = array(
        '#value' => t('Upload'),
        '#type' => 'submit',
        '#required' => TRUE,
        '#ahah' => array(
            'event' => 'click',
            'path' => ahah_helper_path($form_item_ext),
            'wrapper' => $form_element_id.'-display-files-wrapper',
            'method' => 'replace',
            'progress' => array('type' => 'throbber'),
        ),
        '#attributes' => ($fileupload_auto_upload == true) ? array('style' => 'display:none') : '',
    );

    if (strlen($fustatus) < 6 || substr($fustatus, 0, 6) == "ERROR:" || substr($fustatus, 0, 5) == "WARN:") {
        if (!$embedded) {
            //Action buttons
            $pform[$i]['file_info']['file_actions'] = array(
                '#type' => 'item',
                '#prefix' => '<br clear="all" /><div>',
                '#suffix' => '</div>',
                '#tree' => TRUE,
            );
            $pform[$i]['file_info']['file_actions']['submit'] = array(
                '#type' => 'markup',
                '#value' => '<div style="float: left !important;">
                                    <input type="button" id="btnSubmit" style="float: left !important;" class="SearchBtn" onclick="file_not_selected()" value="Submit" />
                                 </div>',
            );
            $pform[$i]['file_info']['file_actions']['cancel'] = array(
                '#type' => 'markup',
                '#value' => '<div style="float: left !important; margin-left: 3px;">
                                    <input type="button" id="btnCancel"  style="float: left !important;" class="SearchBtn" onclick="file_cancel()" value="Cancel" />
                                 </div>',
            );
        }
    }
//    watchdog('form', print_r($form, true));
}

function save_attachment(&$form, $is_autoupload = 0, $pupload_path = '', $preplacement = FALSE) {
//    watchdog('save_attachment', 'example8 form_state '.print_r($form_state, true));
//    watchdog('save_attachment1', 'Uploaded file:'.print_r($_FILES['files']['name'], true));

    $validators = array();
    $upload_field = 'resource_file';
    $upload_filename = 'ERROR';
    $upload_fileid = 'ERROR';
    $upload_success = false;

    //Destination Path
    if (isset($pupload_path) && !empty($pupload_path)) {
        $upload_path = $pupload_path;
    } else {
        $upload_path = get_path_newuploads();
    }

    //File replacement method
    if (isset($preplacement) && $preplacement == TRUE) {
        $replacement = FILE_EXISTS_REPLACE;
    } else {
        $replacement = FILE_EXISTS_RENAME;
    }

//    watchdog('save_attach2', 'Destination path:' . $upload_path . '<br>' . $pupload_path . '<br>' . print_r($replacement, true));
    if ($upload_path != NULL) {
        //Rename file to make its name unique in our upload folder
        $upload_filename = $_FILES['files']['name'][$upload_field];
        //Save original file name
        if (!isset($_FILES['files']['origname'][$upload_field])) {
            $_FILES['files']['origname'][$upload_field] = $upload_filename;
        }
        $status = $upload_filename . ' successfully uploaded.';
//        watchdog('save_attach4', 'Files Object:' . print_r($_FILES, true));

        if (!empty($upload_filename)) {
            if ($preplacement == TRUE) {
//                watchdog('save_attach3', 'Orig File name:' . $_FILES['files']['origname'][$upload_field]);
                $_FILES['files']['name'][$upload_field] = $_FILES['files']['origname'][$upload_field];
            } else {
//                watchdog('save_attach3', 'Change File name:' . $_FILES['files']['origname'][$upload_field]);
                $_FILES['files']['name'][$upload_field] = get_new_filename($upload_filename);
            }
//            watchdog('file', 'New Name:' . $_FILES['files']['name'][$upload_field]);
            //Upload file
            if ($file = file_save_upload($upload_field, $validators, $upload_path, $replacement)) {
                file_set_status($file, 1);
//                watchdog('file', 'New Name:' . $_FILES['files']['name'][$upload_field]);
//                watchdog('file', 'Saved: ' . $file->fid);
                $file->list = 1;
                $file->description = $file->filename;
                $file->weight = 0;
                $file->new = TRUE;
                $upload_fileid = $file->fid;
                $upload_filename = $file->filename;
                $upload_success = true;
            } else {
                $status = 'ERROR: Unable to save the selected file.';
                watchdog('save_attachment3', $status);
                //drupal_set_message('Unable to upload file:'.$upload_path.$upload_filename);
            }
        } else {
            $status = 'WARN: File not selected';
            $upload_filename = $status;
        }
    }
//            $form['file_info'] = array(
//                    '#type' => 'item',
//                    '#value' => '<table><tr><td>File Name : </td><td>'.$file->filename.'</td></tr><tr><td>Path : </td><td>'.file_create_path($file->filepath).'</td></tr></table>'
//            );

    $form['file_info_id'] = array(
        '#type' => 'hidden',
        '#value' => $upload_fileid,
        '#id' => 'file_info_id',
    );

    $form['file_info_path'] = array(
        '#type' => 'hidden',
        '#value' => $upload_path . '/' . $upload_filename,
        '#id' => 'file_info_path',
    );

    if ($upload_success)
        $upload_filename = $_FILES['files']['origname'][$upload_field];
//    watchdog('save_attachment4', $upload_filename);
    $form['file_info_name'] = array(
        '#type' => 'hidden',
        '#value' => $upload_filename,
        '#id' => 'file_info_name',
    );

    if ($is_autoupload == 0) {
        if (substr($status, 0, 5) != "WARN:") {
            $form['file_info_status'] = array(
                '#type' => 'item',
                '#value' => $status,
                '#prefix' => '<table cellspacing="0" cellpadding="0" border="0" align="center" width="99%"><tbody><tr><td class="msgboxleft">&nbsp;</td><td class="msgboxcenter">',
                '#suffix' => '</td><td class="msgboxright">&nbsp;</td></tr></tbody></table>',
            );
        }
    }
}

function auto_upload_align_attach_control($form_element_id, $multiple = false) {
    
    $pos_offset = 0;
    if (strpos($_SERVER['HTTP_USER_AGENT'], 'MSIE')) {
        // in IE, single click does not open 'File-Open-Dialog', therefore we need to move Browse button
        // under +Attach File linkfileupload_auto_upload
        $pos_offset = -120;
    }
    drupal_add_js('$(document).ready(function() {
         place_fileupload_control(\'' . $form_element_id . '\', null);


         if (document.getElementById(\'file-info-resource-file\').getAttribute(\'onchange\') == undefined
               || document.getElementById(\'file-info-resource-file\').getAttribute(\'onchange\') == \'null\') {

                var inp = document.getElementById(\'file-info-resource-file\');
                inp.onchange = function() {
                    set_fileupload_control_link_toshow('.($multiple ? 'true' : 'false').');
                    call_fileupload_control_submit_upload(\'' . $form_element_id . '\');
                }
            }
        });

        function place_fileupload_control(form_element_id, container){
            var parentNode = "#attach_link";
            if (container != null && typeof container != \'undefined\') {
                if (container.parentNode.id.length > 0) {
                    parentNode = "#"+container.parentNode.id+" "+parentNode;
                    $(parentNode).css(\'text-decoration\', \'underline\');
                    $(parentNode).css(\'cursor\', \'pointer\');
                }
            }

            var leftposition = $(parentNode).offset().left;
            var topposition = $(parentNode).offset().top;
            leftposition = leftposition + ' . $pos_offset . ';
            $(\'#file-info-resource-file\').css(\'display\', \'block\');
            $(\'#file-info-resource-file\').css(\'left\', leftposition);
            $(\'#file-info-resource-file\').css(\'top\', topposition);
                    }
    ', 'inline', 'footer');
}

?>
