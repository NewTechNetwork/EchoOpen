//
//function save_image_path(obj) {
//
//var a = document.getElementById('edit-image-upload').value;
//
//  document.getElementById('edit-image').value = a;
//
//}
//
//

function on_imagechange(fileId, fileName) {
    alert("inside image_uploader "+fileId + ":" + fileName);
    
}

function ImageUploader_showLink() {
    if ($('.imgUploadLink').css("display","none")){
        $('.imgUploadLink').css("display","block");
    }
    else {
    }
}
function ImageUploader_hideLink() {
    if ($('.imgUploadLink').css("display","block"))
    {
        $('.imgUploadLink').css("display","none");
    }
    else {
    }
}

$(function() {
         $('#changeimage-wrapper').mouseover
             (function(){
                 
             $('#showlink-wrapper').show();
         });
         $('#changeimage-wrapper').mouseout
             (function(){
              $('#showlink-wrapper').hide();
         });

 });
