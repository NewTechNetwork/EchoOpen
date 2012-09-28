<?php



//drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/fileupload_ahah_helper.js', 'footer');

//Files are first placed in this folder
//We should create year-wise sub-folders under it, for better organization and archival.
//File prefixes should be generated each time for each file
//@returns null if unable to create/find directory
function get_path_newuploads_resources() {
//    global $base_path;
//    $dir = $_SERVER["DOCUMENT_ROOT"].$base_path;
    $dir = file_directory_path().'/new_uploads/'.date('Y', time());
//    watchdog('dir', $dir);
    if (file_check_directory($dir, TRUE)) {
        return $dir;
    }
    return NULL;
}
function get_new_filename_resources($filename) {
    $prefix = time();
    return '_'.$prefix.'_'.$filename;
}

//Use this function when you are building your form.
//$form_item = array of parent form items e.g. array('main_fieldset', 'group_fieldset')
function fileupload_get_control_resources(&$form, $form_state, $form_item = FALSE) {
    drupal_add_js(drupal_get_path('module', 'example_forms') . '/example_ahah_helper.js', 'footer');

    //Following routine gets the reference to the last form item defined in form hierarchy
    $i = 0;
    $pform = array();
    $pform[$i] = &$form;
    $fs = $form_state['values'];
    if ($form_item) {
        if (is_array($form_item)) {
            foreach ($form_item as $key) {
                if (!isset($form[$key])) {
                    watchdog('path', 'Invalid form item path');
                    return 'Invalid form item path';
                }
                $i++;
                $pform[$i] = &$form[$key];
                $fs = $fs[$key];
            }
        } else {
            $i++;
            $pform[$i] = &$form[$form_item];
            $fs = $fs[$form_item];
        }
    }
//    isset($form_state['values']['example8']['file_info']['attachment']
    $process_ahah = isset($fs['file_info']['attachment']);

    $pform[$i]['file_info'] = array(
            '#type' => 'fieldset',
            '#prefix' => '<div id="display-files-wrapper">', // This is our wrapper div
            '#suffix' => '</div>',
            '#tree' => TRUE, // don't forget to set #tree!
    );

    if($process_ahah) {
        //This is a custom property, we used to tell the ahah_helper to exit without doing extensive
        //rendering of the page, which causes the jquery/drupal js routines to crash.
        $form['#fileupload'] = true;
        //Save file
        save_attachment_resources($pform[$i]['file_info']);
        $fid = $pform[$i]['file_info']['file_info_id']['#value'];
        $fname = $pform[$i]['file_info']['file_info_name']['#value'];

        //Action buttons
        $pform[$i]['file_info']['file_actions'] = array(
                '#type' => 'fieldset',
                '#prefix' => '<br clear="all" /><div>',
                '#suffix' => '</div>',
                '#tree' => TRUE,
        );

        $pform[$i]['file_info']['file_actions']['submit'] = array(
                '#type' => 'item',
                '#value' => '<a id="btnSubmit" class="greenButtonLink" onclick="file_onsuccess('.$fid.', \''.$fname.'\')" href="#">Submit</a>',
        );

        $pform[$i]['file_info']['file_actions']['cancel'] = array(
                '#type' => 'item',
                '#value' => '<a id="btnCancel" class="greenButtonLink" onclick="file_cancel()" href="#">Cancel</a>',
        );

    } else {
        $pform[$i]['file_info']['resource_file'] = array(
                '#name' => 'files[resource_file]',
                '#type' => 'file',
                '#title' => 'Attach file',
                //            '#access' => !isset($form_state['values']['attachment']),
        );
        $form_item_ext = array();
        if (is_array($form_item))
            $form_item_ext[] = 'file_info';
        else {
            if (empty($form_item))
                $form_item_ext = array('file_info');
            else
                $form_item_ext = array($form_item, 'file_info');
        }
        //watchdog('formitem', print_r($form_item_ext, true));
//        $form_item_ext = "<form>";
        $pform[$i]['file_info']['attachment'] = array(
                '#value' => t('Attach'),
                '#type' => 'submit',
                '#required' => TRUE,
                '#ahah' => array(
                        'event' => 'click',
                        'path' => ahah_helper_path($form_item_ext),
                        'wrapper' => 'display-files-wrapper',
                        'method' => 'replace',
                //                'progress' => array('type' => 'bar', 'message' => t('Please wait...'))
                ),
        );

        //Action buttons
        $pform[$i]['file_info']['file_actions'] = array(
                '#type' => 'fieldset',
                '#prefix' => '<br clear="all" /><div>',
                '#suffix' => '</div>',
                '#tree' => TRUE,
        );
        $pform[$i]['file_info']['file_actions']['submit'] = array(
                '#type' => 'item',
                '#value' => '<a id="btnSubmit" class="greenButtonLink" onclick="file_not_selected()" href="#">Submit</a>',
        );
        $pform[$i]['file_info']['file_actions']['cancel'] = array(
                '#type' => 'item',
                '#value' => '<a id="btnCancel" class="greenButtonLink" onclick="file_cancel()" href="#">Cancel</a>',
        );
    }
}

function save_attachment_resources(&$form) {
//            watchdog('info', 'example8 form_state '.print_r($form_state, true));
//            watchdog('info', 'Uploaded file:'.print_r($_FILES['files']['name'], true));

    $validators = array();
    $upload_field = 'resource_file';
    $upload_path = get_path_newuploads_resources();
    $upload_filename = 'ERROR';
    $upload_fileid = 'ERROR';
    if ($upload_path != NULL) {
        //Rename file to make its name unique in our upload folder
        $upload_filename = $_FILES['files']['name'][$upload_field];
        $status = $upload_filename . ' successfully uploaded.';

        if (!empty($upload_filename)) {
            $_FILES['files']['name'][$upload_field] = get_new_filename_resources($upload_filename);
            watchdog('file', 'New Name:'.$_FILES['files']['name'][$upload_field]);
            //Upload file
            if ($file = file_save_upload($upload_field, $validators, $upload_path)) {
                watchdog('file', 'Saved: '.$file->fid);
                $file->list = variable_get('upload_list_default', 1);
                $file->description = $file->filename;
                $file->weight = 0;
                $file->new = TRUE;
                $upload_fileid = $file->fid;
                $upload_filename = $file->filename;
            }
            else {
                $status = 'ERROR: Unable to save the selected file.';
                watchdog('file', $status);
                //drupal_set_message('Unable to upload file:'.$upload_path.$upload_filename);
            }
        } else {
            $status = 'ERROR: File not selected';
            $upload_filename = $status;
            watchdog('file', $upload_filename);

//                drupal_set_message('File not selected for upload');
        }
    }
//            $form['file_info'] = array(
//                    '#type' => 'item',
//                    '#value' => '<table><tr><td>File Name : </td><td>'.$file->filename.'</td></tr><tr><td>Path : </td><td>'.file_create_path($file->filepath).'</td></tr></table>'
//            );

    $form['file_info_path'] = array(
            '#type' => 'hidden',
            '#value' => $upload_path.'/'.$upload_filename,
            '#id' => 'file_info_path',
    );
    $form['file_info_name'] = array(
            '#type' => 'hidden',
            '#value' => $upload_filename,
            '#id' => 'file_info_name',
    );
    $form['file_info_id'] = array(
            '#type' => 'hidden',
            '#value' => $upload_fileid,
            '#id' => 'file_info_id',
    );

    $form['file_info_status'] = array(
            '#type' => 'item',
            '#value' => $status,
            '#prefix' => '<table cellspacing="0" cellpadding="0" border="0" align="center" width="99%"><tbody><tr><td class="msgboxleft">&nbsp;</td><td class="msgboxcenter">',
            '#suffix' => '</td><td class="msgboxright">&nbsp;</td></tr></tbody></table>',
    );
}



function ntlp_resource_attachment() {

    $cached_form_state = array();
    $files = array();

    // Load the form from the Form API cache.
    if (!($cached_form = form_get_cache($_POST['form_build_id'], $cached_form_state))) {
        form_set_error('form_token', t('Validation error, please try again. If this error persists, please contact the site administrator.'));
        $output = theme('status_messages');
        print drupal_to_js(array('status' => TRUE, 'data' => $output));
        exit();
    }
    watchdog('info', 'ntlp_resource_attachment '.print_r($_POST,true));
    $form_state = array('values' => $_POST);

    // Handle new uploads, and merge tmp files into node-files.
    attachment_form_submit($cached_form, $form_state);

    if(!empty($form_state['values']['files'])) {
        foreach ($form_state['values']['files'] as $fid => $file) {
            if (isset($cached_form['#updfiles']->files[$fid])) {
                $files[$fid] = $cached_form['#updfiles']->files[$fid];
            }
        }
    }

       $node = $cached_form['#updfiles'];

    foreach($files as $key => $file) {

        watchdog('attachment', $file->filename. ' '.file_create_path($file->filepath) . ' '. format_size($file->filesize));
        $file = (object)$file;

        $form['files'][$key]['file'] = array(
                '#type' => 'item',
                '#value' => '<table><tr><td>File Name : </td><td>'.$file->filename.'</td><td></tr><tr><td>Path : </td><td>'.file_create_path($file->filepath).'</td></tr></table>'
        );

        $form['files'][$key]['filename'] = array(
                '#type' => 'value',
                '#value' => $file->filename
        );

        $form['files'][$key]['filepath'] = array(
                '#type' => 'value',
                '#value' => file_create_url($file->filepath),
        );

        $form['files'][$key]['filesize'] = array(
                '#type' => 'value',
                '#value' => format_size($file->filesize)
        );

        $form['files'][$key]['fid'] = array(
                '#type' => 'value',
                '#value' => $file->fid
        );
    }




    // rebuild form object and output new form elements
    $output .= attachment_ahah_render($form, 'newfield');



//    print drupal_to_js(array('status' => TRUE, 'data' =>"Hello Drupal World"));
    print drupal_to_js(array('status' => TRUE, 'data' => $output));
//    exit;
}

function resrouce_attachment_submit(&$form, &$form_state) {

    $validators = array();

    if ($file = file_save_upload('resource_attach', $validators, file_directory_path())) {
        $file->list = variable_get('upload_list_default', 1);
        $file->description = $file->filename;
        $file->weight = 0;
        $file->new = TRUE;
        $form['#updfiles']->files[$file->fid] = $file;
        $form_state['values']['files'][$file->fid] = (array)$file;
    }
    else {
        drupal_set_message('Unable to upload file...');
    }
}


function attachment_form_submit(&$form, &$form_state) {

    $validators = array();

    if ($file = file_save_upload('add_resources_stuff', $validators, file_directory_path())) {
        $file->list = variable_get('upload_list_default', 1);
        $file->description = $file->filename;
        $file->weight = 0;
        $file->new = TRUE;
        $form['#updfiles']->files[$file->fid] = $file;
        $form_state['values']['files'][$file->fid] = (array)$file;
    }
    else {
        drupal_set_message('Unable to upload file...');
    }
}

function attachment_ahah_render($fields, $name) {

    $form_state = array('submitted' => FALSE);
    $form_build_id = $_POST['form_build_id'];
    // Add the new element to the stored form. Without adding the element to the
    // form, Drupal is not aware of this new elements existence and will not
    // process it. We retreive the cached form, add the element, and resave.
    $form = form_get_cache($form_build_id, $form_state);
    $form[$name] = $fields;
    form_set_cache($form_build_id, $form, $form_state);
    $form += array(
            '#post' => $_POST,
            '#programmed' => FALSE,
    );
    // Rebuild the form.
    $form = form_builder($_POST['form_id'], $form, $form_state);

    // Render the new output.
    $new_form = $form[$name];
    return drupal_render($new_form);
}

?>
