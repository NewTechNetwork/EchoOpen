// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


function resource_lib_save_category( ){

    var category_name = document.getElementById('txt_category_name').value;
    var category_desc = document.getElementById('txt_category_description').value;
    var module_nid = document.getElementById('module_id').value;
    var category_id = document.getElementById('category_id').value;
    var message = "";
    var flag = true;
//    var iChars = "!#$%^*+=-[]\\\';/{}|\"";
    
    $("#dialog-show-confirmation").dialog('option', 'buttons',{
        'Close':function(){
            $(this).dialog('close');
        }
    });

    var get_url = "?q=ntlp/resource/savecategory/"+module_nid+"/"+category_name+"/"+category_desc+"/"+category_id;

//    for (var i = 0; i < category_name.length; i++) {
//        alert(iChars.indexOf(category_name.charAt(i)));
//        if (iChars.indexOf(category_name.charAt(i)) != -1) {
//            message ="Special character are not allowed.";
//            flag = false;
//            break;
//        }
//    }
//    category_name = category_name.replace(",", "&#44");
//    alert(category_name);
    
    if(flag){
        $.ajax({
            type: "GET",
            url: get_url,
            cache: false,
            success: function(data){
                parent.Drupal.modalFrame.close();
                parent.window.location.reload();
            }

        });
    }else{
        $("#ui-dialog-title-dialog-show-confirmation").text('Oops!');
        $("#dialog-show-confirmation").find('.msg').css('color', '#000000').text(message);
        $("#dialog-show-confirmation").dialog("option", "minHeight", 100 );
        $("#dialog-show-confirmation").dialog('open');        
    }
}


function resource_lib_cancel_category(){
    try {
        parent.Drupal.modalFrame.close();
    } catch (e) {

    }
}

function focus_func(resource_nid){

    $('#txt_'+resource_nid).show();
    $('#comment_new_'+resource_nid).show();
    $('#txt_'+resource_nid).focus();

}



function remove_resource(resource_id) {

    //    $('#dialog-confirm-ntk').dialog( "option" , 'section_tid' , section_tid );
    $('#dialog-confirm-ntk').dialog( "option" , 'resource_id' , resource_id );

    $('#dialog-confirm-ntk').dialog('open');
}

$(function(){
    $("#dialog-confirm-ntk").dialog({
        autoOpen: false,
        resizable: false,
        height:120,
        width:400,
        modal: true,
        buttons: {
            'Delete': function() {
                var dlg = this;
                var resource_id = $(dlg).dialog('option','resource_id');
            
                $.get('?q=ntlp/node/resource/delete/'+resource_id, function(){
                    $(dlg).dialog('close');
                    window.location = '?q=ntlp/goback';

                })

            },
            Cancel: function() {
                $(this).dialog('close');
            }
        }
    });

});

function fixResourceLibrarySearchResultPager() {
    $("#project_add_test .pager li a").click(function(){
        $.get($(this).attr("href"), function(response){
            $("#project_add_test").html(response);
            $('html,body').animate({
                "scrollTop":0
            },1000);
        });

        return false;
    });
}

function remove_resource_category(category_id) {
    $("#dialog-confirm-cat").dialog({
        autoOpen: false,
        resizable: false,
        height:140,
        modal: true
    });
    $("#dialog-confirm-cat").dialog('option', 'buttons',{
        'Yes': function() {
            $.ajax({
                type: "GET",
                url: "?q=ntlp/resource/library/delcategory/"+category_id,
                success:  function(){
                    window.location.reload();
                }
            });
        },
        'No': function() {
            $(this).dialog('close');
        }
    });

    $("#dialog-confirm-cat").dialog('open');
}
function fixDocumentSearchResultPager() {
    $("#library_document_table_view .pager li a").click(function(){
        $.get($(this).attr("href"), function(response){
            $("#library_document_table_view").html(response);
            $('html,body').animate({
                "scrollTop":0
            },1000);
        });

        return false;
    });
}   

$(document).ready(function() {

    $("#dialog-show-confirmation").dialog({
        autoOpen: false,
        resizable: false,
        height:90,
        width: 400,
        modal: true,
        zIndex:9999
    });
});