// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


// $Id: ahah_helper.js,v 1.1 2008/08/29 18:58:21 wimleers Exp $
var fileupload_control_link = false;
var fileupload_control_id = "file-info-resource-file";

function set_fileupload_control_link_toshow(to_show) {
    fileupload_control_link = to_show;
//    $('a[id=attach_link]').hide();
//    $('#attach_link_prompt').hide();
//    alert(to_show);
//    alert($('#attach_link').length);
}

(function($) {

if (Drupal.jsEnabled) {
$(document).ready(function() {

    //Hide the file upload control, if mouse is not over it.
    $('#'+fileupload_control_id).mouseout(function() {
        $(this).hide();
    });

    $("#dialog-fileupload-control-message").dialog({
        autoOpen: false,
        height:50,
        width:300,
        modal: true, zIndex:9999,
        buttons: {
            Ok: function() {
                $(this).dialog('close');
            }
        }
    });

    if (Drupal.ahah != undefined) {

/**
 * Override of Drupal.ahah.prototype.success. The only difference is that we
 * allow for new Drupal.settings.
 */
Drupal.ahah.prototype.success = function (response, status) {
  var wrapper = $(this.wrapper);
  var form = $(this.element).parents('form');
  // Manually insert HTML into the jQuery object, using $() directly crashes
  // Safari with long string lengths. http://dev.jquery.com/ticket/1152
  var new_content = $('<div></div>').html(response.data);
//  alert('AHAH: '+ new_content.html());

  if ($("#file_info_name", new_content).val()) {
      var strFilename = $("#file_info_name", new_content).val();
      var strFilepath = $("#file_info_path", new_content).val();
      var nFileId = $("#file_info_id", new_content).val();
      var isEmbedded = $("#is_embedded", new_content).val();
      var lSuccess = !(strFilename.substr(0, 6) == "ERROR:");
//      alert(nFileId+":"+strFilename+" / "+strFilepath+" / "+lSuccess+" / "+isEmbedded);
    //  alert('AHAH: '+ $("#file_info_path", new_content).val());
    //  alert('AHAH: '+ $("#file_info_name", new_content).val());
    //  alert('AHAH: '+ $("#file_info_id", new_content).val());
      //var uploaded_image = document.getElementById('file-info-view-image');
      //uploaded_image.src = $("#file_info_path", new_content).val();
  }

  // Restore the previous action and target to the form.
  form.attr('action', this.form_action);
  this.form_target ? form.attr('target', this.form_target) : form.removeAttr('target');
  this.form_encattr ? form.attr('target', this.form_encattr) : form.removeAttr('encattr');

  // Remove the progress element.
  if (this.progress.element) {
    $(this.progress.element).remove();
  }
  if (this.progress.object) {
    this.progress.object.stopMonitoring();
  }
  $(this.element).removeClass('progress-disabled').attr('disabled', false);

  // Add the new content to the page.
  Drupal.freezeHeight();
  if (this.method == 'replace') {
    wrapper.empty().append(new_content);
  }
  else {
    wrapper[this.method](new_content);
  }

  // Immediately hide the new content if we're using any effects.
  if (this.showEffect != 'show') {
    new_content.hide();
  }

  // Determine what effect use and what content will receive the effect, then
  // show the new content. For browser compatibility, Safari is excluded from
  // using effects on table rows.
  if (($.browser.safari && $("tr.ahah-new-content", new_content).size() > 0)) {
    new_content.show();
  }
  else if ($('.ahah-new-content', new_content).size() > 0) {
    $('.ahah-new-content', new_content).hide();
    new_content.show();
    $(".ahah-new-content", new_content)[this.showEffect](this.showSpeed);
  }
  else if (this.showEffect != 'show') {
    new_content[this.showEffect](this.showSpeed);
  }

  // Merge in new and changed settings, if any.
  if (response.settings) {
    $.extend(Drupal.settings, response.settings);
  }

  // Attach all javascript behaviors to the new content, if it was successfully
  // added to the page, this if statement allows #ahah[wrapper] to be optional.
  if (new_content.parents('html').length > 0) {
    Drupal.attachBehaviors(new_content);
  }

  ShowTheUploadLink();

  if (lSuccess && isEmbedded) {
      fileupload_success(nFileId, strFilename, strFilepath);
      //If file uploaded successfully, we update the submit link
//      $("#file_submit", new_content).attr("onclick", "file_onsuccess("+nFileId+", '"+strFilename+"')");
  }

  //Call a custom callback
  if (typeof($("#callon_success", new_content)) == "object") {
      var call = $("#callon_success", new_content).val();
      if (call) {
          ntlp_ahah_success();
      }
  }
  
  Drupal.unfreezeHeight();
};

    }
  });
}

})(jQuery);

function ShowTheUploadLink() {
  //empty the file text area, so that when submit button is pressed, it does not reupload the file
  var elem = document.getElementById(fileupload_control_id);
  elem.value = "";
  var elem_new = elem.cloneNode(false);
  elem_new.onchange = elem.onchange;
  elem.parentNode.replaceChild(elem_new, elem);

  //Hide the upload link, if required
  if (fileupload_control_link) {
    $('a[id=attach_link]').show();
    $('#attach_link_prompt').show();
    fileupload_control_link = false;
  }
}

function onchange_image() {
    var x = document.getElementsByName('files[resource_file]');
    for(i=0; i < x.length; i++){
        var txt = x[i];
        txt.click();
//        txt.text = "C:\Users\SHAHBAZ\Pictures\CRaSS\logo.jpg";
        alert(txt.value);
    }
    var btn = document.getElementById('file-info-attachment');
    btn.click();
}

function file_not_selected() {
    alert('Cannot submit, file is not uploaded yet');
}

function file_cancel() {
    try {
        parent.Drupal.modalFrame.close().closeDialog();
    } catch (e) {
    }
}

function file_onsuccess(fileId, fileName) {
//    alert("file_onsuccess:"+fileId + ":" + fileName);
    if(window.location.search.toString().indexOf("ntlp/image/uploader/link") != -1) {
        parent.on_imagechange_link(fileId, fileName);
    } else {
        parent.on_imagechange(fileId, fileName);
    }
}

function fileupload_success(fileId, fileName, filePath) {
//    alert("fileupload_success:"+fileId + ":" + fileName + ":" + filePath);
    if(parent.on_fileupload_success) {
        parent.on_fileupload_success(fileId, fileName, filePath);
    }
}

//------------------
// File Upload control user-interaction events
function place_fileupload_control_mouseout(container){
    var parentNode = "#attach_link";
    if (container != null && typeof container != 'undefined') {
        if (container.parentNode.id.length > 0) {
            parentNode = "#"+container.parentNode.id+" "+parentNode;
        }
    }
    $(parentNode).css('text-decoration', 'none');
    $(parentNode).css('cursor', 'pointer');
//            $(\'#file-info-resource-file\').css(\'display\', \'none\');
}

function call_fileupload_control_submit_upload(form_element_id) {
    var extentions = document.getElementById(form_element_id+'-file-info-validextentions').value;
//    alert(form_element_id+"/"+extentions);
    if (call_fileupload_control_validate_upload(extentions)) {
        document.getElementById(form_element_id+'-file-info-attachment').click();
    } else {
        set_fileupload_control_link_toshow(true);
        ShowTheUploadLink();
    }
}

function call_fileupload_control_validate_upload(extentions) {
    if (extentions.length > 0) {
        $("#dialog-fileupload-control-message").dialog({
            autoOpen: false,
            height:50,
            width:300,
            modal: true, zIndex:9999,
            buttons: {
                Ok: function() {
                    $(this).dialog('close');
                }
            }
        });
        extToSearch = extentions.toLowerCase() + ",";  //so that all extentions end with a comma
        var inpfile = document.getElementById('file-info-resource-file').value;
        var fileext = getFileExtention(inpfile.toLowerCase());
        
        if (extToSearch.indexOf(fileext+',') == -1) {
            msg = "The file '"+inpfile+"' is not allowed in this context. Valid extentions are "+extentions+".";
            $("#dialog-fileupload-control-message").find('.msg').css('color', '#000000').text(msg);
            $('#dialog-fileupload-control-message').dialog("open");
            return false;
        }
    }
    return true;
}

function getFileExtention(filename) {
    return /[^.]+$/.exec(filename);
}
