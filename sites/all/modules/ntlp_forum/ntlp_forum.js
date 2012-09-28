
$(document).ready(function() {

    $("#dialog-show-confirmation").dialog({
        autoOpen: false,
        resizable: false,
        height:90,
        width: 600,
        modal: true,
        open: function() {
            $('.ui-dialog-buttonpane').find('button:contains("No")').attr('id','noBtn');
            $('.ui-dialog-buttonpane').attr('id','disscussion_forum_dialog');
        },
        close: function() {
            $('.ui-dialog-buttonpane').find('button:contains("No")').addClass('CancelBtn');
        }
    });
});

function topic_delete(topic_nid){

    var course_nid = $('#course_nid').val();
    var context = $('#context').val();
    
    var note = 'Are you sure you want to permanently delete this topic and all the associated comments? ';
    $("#dialog-show-confirmation").dialog('option', 'buttons',{
        'Yes': function() {

            //alert(course_nid);
            //alert(context);

            $.ajax({
                type: "GET",
                url: '?q=ntlp/courses/forum/topic/delete/'+topic_nid,
                cache: false,
                success: function(data){
                    $(this).dialog('close');
                    window.location = "?q=ntlp/"+context+"/forum/"+course_nid;
                }
            });
            
        },
        'No': function() {
            $(this).dialog('close');
        }
    });

    $("#ui-dialog-title-dialog-show-confirmation").text('Delete Topic');
    $("#dialog-show-confirmation").find('.msg').css('color', '#000000').text(note);
    $("#dialog-show-confirmation").dialog( "option", "minHeight", 120 );
    $("#dialog-show-confirmation").dialog("open");
}

function delete_forum_comment(comment_cid){

    var course_nid = $('#course_nid').val();
    var context = $('#context').val();

    var note = 'Are you sure you want to permanently delete this comment and all the replies to this comment? ';
    $("#dialog-show-confirmation").dialog('option', 'buttons',{
        'Yes': function() {
            $(this).dialog('close');
            $.ajax({
                type: "GET",
                url: '?q=ntlp/courses/forum/comment/delete/'+comment_cid,
                cache: false,
                success: function(data){
                    
                    window.location.reload();
                }
            });
           
        },
        'No': function() {
            $(this).dialog('close');
        }
    });

    $("#ui-dialog-title-dialog-show-confirmation").text('Delete Comment');
    $("#dialog-show-confirmation").find('.msg').css('color', '#000000').text(note);
    $("#dialog-show-confirmation").dialog("option", "minHeight", 120 );
    $("#dialog-show-confirmation").dialog("open");
}


function attach_upload_file(data,fileId){

    var root_path = Drupal.settings.basePath;
    var d = document.getElementById('attach_links');

    var count = d.getElementsByTagName('div').length;
    var element;

    if(document.getElementById("attachted_files_hidden")){
        element = document.getElementById("attachted_files_hidden");
    }

    element.value += 'F,'+'-'+fileId+','+data+' ;';
    document.getElementById("attach_links").innerHTML += '<div id="F_'+'-'+fileId+'"> <a style="vertical-align:middle;" '
    +' onclick="detele_attach(\'F_'+'-'+(fileId)+'\');" ><img width="14" height="14" src="'+root_path+'themes/Boldr/Images/DeleteBtn.png" '
    +' border="2" /></a>&nbsp;&nbsp;'+data+'</div>';


    
}


function on_fileupload_success(fileId, fileName) {
    
    if (Drupal.modalFrame.isOpen || $('#modalframe-container').size()) {
    }else{
        attach_upload_file(fileName,fileId);
    }

}

function _delete_hidden(file_id){


    element = document.getElementById("attachted_files_hidden");

   
    hData = element;
    var files = hData.value.split(";");
    var obj = new Array();

    for(i = 0 ; i< files.length; i++){
        var attfile = files[i].split(",");
        var recid = (file_id.split('_'))[1];
        var rectype = file_id.substr(0, 1);

        if(recid == attfile[1]){
            //alert('found:'+rectype+','+recid+":"+attfile[0]+','+attfile[1]);
            hData.value = hData.value.replace(rectype+','+recid, 'X,'+attfile[1]);
            break;
        }
    }
    //    alert(hData.value);
    return obj;
}

function detele_attach(id){

    //Remove (Mark X) in attached files list in the hidden variable
    _delete_hidden(id);

    //Remove UI element for the selected file.
    var d = document.getElementById('attach_links');
    var olddiv = document.getElementById(id);
    d.removeChild(olddiv);
    var count = d.getElementsByTagName('div').length;
    
}
