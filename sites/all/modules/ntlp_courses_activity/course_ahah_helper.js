// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


// $Id: ahah_helper.js,v 1.1 2008/08/29 18:58:21 wimleers Exp $

(function($) {

if (Drupal.jsEnabled) {
  $(document).ready(function() {
//    if (Drupal.ahah != undefined) {

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

  //Re-setup combobox styles and datepicker styles
//  selectOptionInContext('#ActivitiesCSS');
  $(".my-dropdown", new_content).sSelect();
  setup_date_picker();
//  setupMenuButton('manageAttachments', "btnAttachments");
  
  Drupal.unfreezeHeight();
};

//    }
  });
}

})(jQuery);

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
//    alert(fileId + ":" + fileName);
    parent.on_imagechange(fileId, fileName);
}

function fileupload_success(fileId, fileName) {
//    alert(fileId + ":" + fileName);
    parent.on_fileupload_success(fileId, fileName);
}