// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


// $Id: ahah_helper.js,v 1.1 2008/08/29 18:58:21 wimleers Exp $

(function($) {
    if (Drupal.jsEnabled && Drupal.ahah != undefined) {
    /**
     * Override of Drupal.ahah.prototype.success.
     */
        $(function(){
            var ahahSuccessProxy = Drupal.ahah.prototype.success;
            Drupal.ahah.prototype.success = function () {
              // @TODO: Remove all the code that was here from other files and use proxy pattern, so that nothing breaks on drupal update.

              // Calling the orignal method i.e. proxy method
              ahahSuccessProxy.apply(this, arguments);

              // Do custom calls below.
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

function fileupload_success(fileId, fileName, filePath) {
    if(parent.on_fileupload_success) {
        parent.on_fileupload_success(fileId, fileName, filePath);
    }
}