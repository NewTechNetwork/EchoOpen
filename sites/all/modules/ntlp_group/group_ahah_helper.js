// $Id: ahah_helper.js,v 1.1 2008/08/29 18:58:21 wimleers Exp $


(function($) {

if (Drupal.jsEnabled) {
  $(document).ready(function() {
//    if (Drupal.ahah != undefined) {

/**
 * Override of Drupal.ahah.prototype.success. The only difference is that we
 * allow for new Drupal.settings.
 */

var proxyAhahSuccess = Drupal.ahah.prototype.success;
Drupal.ahah.prototype.success = function () {
  proxyAhahSuccess.apply(this, arguments);
  //Re-setup combobox styles and datepicker styles
  
   if($.prototype.sSelect) {
       $myDropDowns = $('.my-dropdown');
      if($myDropDowns.length > 0) {
          $myDropDowns.each(function(){
              if(!$(this).next().hasClass('newListSelected')) {
                  $(this).sSelect();
              }
          });
      }
  }
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