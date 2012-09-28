/*
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


Copyright (c) 2003-2009, CKSource - Frederico Knabben. All rights reserved.
For licensing, see LICENSE.html or http://ckeditor.com/license
 */
CKEDITOR.plugins.add('uploader',{
    init:function(a){
        var cmd = a.addCommand('uploader', {
            exec:uploader_onclick
        })
        cmd.modes={
            wysiwyg:1,
            source:1
        }
        cmd.canUndo=false
        a.ui.addButton('uploader',{
            label:'Upload an Image',
            command:'uploader',
            icon:this.path+'images/icon_upload_3.png'
        })
    }
})

function uploader_onclick(e)
{
    // run when custom button is clicked
    Drupal.modalFrame.open({
        url: '?q=ntlp/image/uploader/link',
        width: 500,
        height: 300

    });

}

function on_imagechange_link(fileId, fileName){

    $.ajax({
        type: "GET",
        url: "?q=ntlp/get/uploaded/file/detail/"+fileId,
        cache: false,
        success: function(data){
            Drupal.modalFrame.close();
            var textarea_id=$(".ckeditor-processed").attr("id");
            CKEDITOR.instances[textarea_id].insertHtml( data);
            
        }
    });
    
    
}
