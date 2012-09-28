// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$(function(){
    $('#edit-add-resources-stuff-share-ideas').focus(function(){
        if( $(this).hasClass('onClickTxtClr') ) {
            $(this).css('color', '#000000');
            $(this).val('');
            $(this).removeClass('onClickTxtClr')
        }
    });

    //Likes tooltip
    $('.tooltipLikesBtonclick_remove_resourcen').hover(function(){
        $(this).next().fadeIn(function(){
            $(this).css('opacity', 1);
        });
    },
    function(){
        $(this).next().stop().fadeOut();
    });

    $('.tooltipLikes').hover(function(){
        $(this).stop().animate({
            "opacity": 1
        });
    },function(){
        $(this).stop().fadeOut();
    });
});

function show_video(video)
{
    var option=$(video).val();
    if(option=='embd')
    {
        $("span[id=attach_link_suffix]").hide();
        $('#file-info-resource-file-wrapper').hide();
        $("tr[id=uploadlink]").hide();
        $('#video_embd').show();
    }
    else
    {
        $("span[id=attach_link_suffix]").show();
        $('#file-info-resource-file-wrapper').show();
        $("tr[id=uploadlink]").show();
        $('#video_embd').hide();
    }
}

function show_google(google)
{
    var option=$(google).val();
    if(option=='gd')
    {
        if (is_file_attached('edit-add-resources-stuff-google-hAttachedFiles')) {
            $('#google_doc_attached').hide();
            $('#google_docs_row').show();
        } else {
            $('#google_doc_attached').show();
            $('#google_docs_row').hide();
        }

        $('#file-info-resource-file-wrapper').hide();
        $("tr[id=uploadlink]").hide();
    }
    else
    {
        $('#file-info-resource-file-wrapper').show();
        $('#google_doc_attached').hide();
        $('#google_docs_row').hide();
        $("tr[id=uploadlink]").show();
    }
}

function google_docs_search() {

    var name =  $('#finder').val();

    $.ajax({
        type: "GET",
        url: "?q=ntlp/google/docs/search",
        data: "type=radio",
        success:  function(data){
            $("#googledocs").html(data);

        }

    });
}


function page_attach_google_docs(){
    var root_path = Drupal.settings.basePath;
    var element = null;
    var d = parent.document.getElementById('page_attach_links');

    if (d.getElementsByTagName('input') == null || d.getElementsByTagName('input') == 'undefined'){
        var count = d.getElementsByTagName('div').length;
    }else{
        var count = d.getElementsByTagName('input').length;
    }
    element = parent.document.getElementById("page_attached_files_hidden");

    var x = document.getElementsByName("gdocs[]");
    for(i=0; i < x.length; i++) {

        if(x[i].checked){
            var docLink = x[i].value;
            var docName = x[i].id;
            element.value += '-'+(count+1)+',G,'+(count+1)+','+docName+','+docLink+' ;';

            d.innerHTML += '<div id="G_'+'-'+(count+1)+'">'+
            '<img style="position:relative;top:2px;margin-right:4px;" onclick="page_delete_attachment(\'G_'+'-'+(count+1)+'\');" width="14" height="14" src="'+root_path+'themes/Boldr/Images/DeleteBtn.png" border="0" />'+
            '<a href="'+docLink+'" target="_blank">'+docName+'</a></div>';
        }
    }

}

function attach_google_docs(){
    var root_path = Drupal.settings.basePath;
    var element = null;

    if (parent.document.getElementById("google_hAttachedFiles")) {
        element = parent.document.getElementById("google_hAttachedFiles");
        color = '#3570AA';
    } else if (parent.document.getElementById("edit-add-resources-google-hAttachedFiles")) {
        element = parent.document.getElementById("edit-add-resources-google-hAttachedFiles");
        color = '#3570AA';
    } else if(parent.document.getElementById("edit-add-resources-stuff-google-hAttachedFiles")) {
        element = parent.document.getElementById("edit-add-resources-stuff-google-hAttachedFiles");
        color = 'white';
    }else if(parent.document.getElementById("edit-mysubmission-hAttachedFiles")) {
        element = parent.document.getElementById("edit-mysubmission-hAttachedFiles");
        color = '#3570AA';
    }else if(parent.document.getElementById("edit-main-info-hAttachedFiles")) {
        element = parent.document.getElementById("edit-main-info-hAttachedFiles");
        color = '#3570AA';
    }

    if (element) {
        var d = parent.document.getElementById('google_urllist');
        if(d == null || d == 'undefined'){
            d = parent.document.getElementById('urllist');
        }

        if(d.getElementsByTagName('input') == null || d.getElementsByTagName('input') == 'undefined'){
            var count = d.getElementsByTagName('div').length;
        }else{
            var count = d.getElementsByTagName('input').length;
        }

        var x=document.getElementsByName("gdocs[]");

        for(i=0; i< x.length; i++){
            if(x[i].checked){
                var docName = x[i].id;
                var docLink = x[i].value;
                element.value = 'G,'+'-'+(count+1)+','+docName+','+docLink+';';

                //        $divdata .= '<div id="G_' . $resource_Obj->nid . '"> <a onclick="doc_delete_attachment(\'G_' . $resource_Obj->nid . '\');" ><img src="' . $base_path . 'sites/all/modules/ntlp_courses_activity/theme/delete.PNG" border="0" /></a><a href=' . $resource_Obj->path . '>' . $resource_Obj->path . '</a></div>';
                d.innerHTML = '<div id="G_'+'-'+(count+1)+'">'+
                '<img style="position:relative;top:2px;margin-right:4px;" onclick="doc_delete_attachment(\'G_'+'-'+(count+1)+'\');" width="14" height="14" src="'+root_path+'themes/Boldr/Images/DeleteBtn.png" border="0" />'+
                '<a href="'+docLink+'" target="_blank">'+docName+'</a></div>';
                break;
            }
        }
        parent.document.getElementById('google_docs_row').style.display='block';
        if(parent.document.getElementById('google_doc_attached'))
        parent.document.getElementById('google_doc_attached').style.display='none';


    } else {
        alert('Google document container not found.');
    }
}


function page_attach_delete_hidden(file_id){
    element = document.getElementById("page_attached_files_hidden");
    hData = element;
    var files = hData.value.split(";");
    var obj = new Array();

    for(i = 0 ; i< files.length; i++){
        var attfile = files[i].split(",");
        var recid = (file_id.split('_'))[1];
        var rectype = file_id.substr(0, 1);

        if(recid == attfile[0]){
            //alert('found:'+rectype+','+recid+":"+attfile[0]+','+attfile[1]);
            hData.value = hData.value.replace(recid+','+rectype, recid+',X');
            break;
        }
    }
    //    alert(hData.value);
    return obj;
}


//Drupal.settings.basePath
/*  Start : Java Script Code For Add Resource Stuff*/

function link_field_hide(){

    var path = Drupal.settings.basePath;

    $('#stuffMainTable').show();
    $('#resourcesname').show();
    $("#linkurl").show();
    $("#resource-stuff-wrapper").show();
    $("#close_button").show();
    $("#post").show();
    $("#title_resource").show();
    $("#link_heading").html('Link');
    $("span[id=attach_link_suffix]").hide();
    
    $("#path").html("<img src=\""+path+"themes/Boldr/Images/resourceStuff/img_link.png \" alt=\"Link Image\" />");
    document.getElementById('type').value = 'lo_link';
    $('#table_hide').fadeIn('slow');
    $("#title").html("Add Link");

    $('.uploadFiles').hide();
    $("tr[id=uploadlink]").hide();
    $('#file-info-resource-file-wrapper').hide();

    //hide video fields
    $('#video_embd').hide();
    $('#video_radio').hide();

    //hide document fields
    $("#google").hide();
    $('#google_doc_attached').hide();
//    $('#google_urllist').hide();
}



function image_field_hide(){

    var path = Drupal.settings.basePath;
    $('#table_hide').fadeIn('slow');
    $("#stuffMainTable").show();
    $("#edit-add-resources-stuff-share-ideas").focus();
    $("#path").html("<img src=\""+path+"themes/Boldr/Images/resourceStuff/img_picture.png \" alt=\"AttachedLink\" />");
    $("#link_heading").html("Images");
    //    $('#google_urllist').hide();
    $("#title_resource").show();
    $("#resource-stuff-wrapper").show();
    $("#title").html("Add Image");
    $('#google_doc').hide()
    $("#close_button").show();
    $("#post").show();
    
    //hide link fields
    $('#resourcesname').hide();
    $("#linkurl").hide();
    $('#file-info-resource-file-wrapper').show();
    $("tr[id=uploadlink]").show();
    $("#attach_link").html("Upload Image");
    $("span[id=attach_link_suffix]").html('<div style="padding-top:10px;">Max file size = 5 MB | File type = JPG, PNG, GIF</div>');
    $('#edit-add-resources-stuff-upload-file-link-file-info-validextentions').val('jpg,jpeg,png,gif');
    $("span[id=attach_link_suffix]").show();

    $('#video_embd').hide();
    $('#video_radio').hide();

    //hide document fields
    $("#google").hide();
    $('#google_doc_attached').hide();
     
    document.getElementById('type').value = 'lo_image';
}

function video_field_hide(){
    var path = Drupal.settings.basePath;
    $('#table_hide').fadeIn('slow');    
    $("#stuffMainTable").show();
    $("#edit-add-resources-stuff-share-ideas").focus();
    $("#path").html("<img src=\""+path+"themes/Boldr/Images/resourceStuff/img_video.png \" alt=\"Video Image\" />");
    $("#link_heading").html("Videos");
    $("#title_resource").show();
    $("#resource-stuff-wrapper").show();
    $("#title").html("Add Media");
    //    $('#google_urllist').hide()
    $("#close_button").show();
    $("#post").show();

    //hide link fields
    $('#resourcesname').hide();
    $("#linkurl").hide();

    //hide upload fields
    $('#file-info-resource-file-wrapper').hide();
    $("tr[id=uploadlink]").hide();
    $("#attach_link").html("Upload Media file");
    $("span[id=attach_link_suffix]").html('<div style="padding-top:10px;">Max file size = 50 MB | File type = FLV only </div>');
    $('#edit-add-resources-stuff-upload-file-link-file-info-validextentions').val('mov,wmv,mp4,flv');
    $("span[id=attach_link_suffix]").hide();

    //show videw fields
    $('#video_radio').show();
    $('#video_embd').show();

    //hide document fields
    $("#google").hide();
    $('#google_doc_attached').hide();

    document.getElementById('type').value = 'lo_video';
    document.getElementById('edit-add-resources-stuff-video-upload-radio-embd').checked = true;
    document.getElementById('edit-add-resources-stuff-video-upload-radio-upld').checked = false;
}



function document_field_hide(){
    var path = Drupal.settings.basePath;
    $("#stuffMainTable").show();
    $('#table_hide').fadeIn('slow');
    $("#path").html("<img src=\""+path+"themes/Boldr/Images/resourceStuff/img_document.png \" alt=\"Document Image\" />");
    $("#title_resource").show();
    $("#edit-add-resources-stuff-share-ideas").focus();

    //    $('#google_urllist').hide()
    $("#link_heading").html("Document");
    $("#title").html("Add Document");
    $("#resource-stuff-wrapper").show();
    $("#google").show();
    $("#close_button").show();
    $("#post").show();

    //hide link fields
    $('#resourcesname').hide();
    $("#linkurl").hide();

    //show upload fields
    $('#file-info-resource-file-wrapper').show();
    $("tr[id=uploadlink]").show();
    $("span[id=attach_link_suffix]").html("");
    $("#attach_link").html("Upload file");
    $('#edit-add-resources-stuff-upload-file-link-file-info-validextentions').val('');

    //hide video fields
    $('#video_embd').hide();
    $('#video_radio').hide();

    //show/hide document fields
    $("#google").show();
    $('#google_doc_attached').hide();
   
    document.getElementById('type').value = 'lo_document';
    document.getElementById('edit-add-resources-stuff-google-radio-gd').checked = false;
    document.getElementById('edit-add-resources-stuff-google-radio-upld').checked = true;

}



function close_button(){

    $('#table_hide').hide();
    $("#stuffMainTable").hide();

    clear_all_contents();

    $("#resource-stuff-wrapper").hide();

    $('#google_urllist').hide()

    $("#linkurl").hide();

    $("#google").hide();
    
    $("#close_button").hide();

    $("#title_resource").hide();

    document.getElementById('type').value = 'lo_share_idea';

    $("#title").html('');
}

/*  End : Java Script Code For Add Resource Stuff*/





function starttime() {
    starttime=new Date()
    starttime=starttime.getTime()
    countdown()

}




function timedRefresh(timeoutPeriod) {
    //    alert('inside timedRefresh');
    setTimeout("location.reload(true);",timeoutPeriod);
}



function countdown(){
    nowtime= new Date()
    nowtime=nowtime.getTime()

    secondssinceloaded=(nowtime-starttime)/1000
    reloadseconds=Math.round(refreshinterval-secondssinceloaded)
    if (refreshinterval>=secondssinceloaded){
        var timer=setTimeout("countdown()",1000)
        if (displaycountdown=="yes"){
            window.status="Page refreshing in "+reloadseconds+ " seconds"
        }
    } else {
        clearTimeout(timer)
        window.location.reload(true);
    }
}


//Called by Post New Comment button; available for each resource on News Feed
function comments_post_new(resource_nid) {
    var comment_text =  encodeURIComponent($('#txt_'+resource_nid).val());

    var get_url = "?q=ntlp/comments/new/"+resource_nid ;
    $.ajax({
        type: "POST",
        url: get_url,
        data: "comment-body="+comment_text,
        success: function(data){
            $("#comment-wrapper-"+resource_nid).html(data);
            $('#txt_'+resource_nid).val('');
            $('#txt_'+resource_nid).hide();
            $('#comment_new_'+resource_nid).hide();
        }

    });
}

function new_post_comments(resource_nid) {
    var comment_text =  $('#txt_'+resource_nid).val();
    var get_url = "?q=ntlp/post/comments/"+resource_nid+"/"+comment_text;
    
    $.ajax({
        type: "GET",
        url: get_url,
        cache: false,
        success: function(data){
            $("#comments-wrapper-"+resource_nid).html(data);
            $('#txt_'+resource_nid).val('');
            $('#txt_'+resource_nid).hide();
            $('#comment_new_'+resource_nid).hide();
        }

    });
}



//Called by Post New Comment button; available for each resource on News Feed
function delete_comments_from_wall(comment_nid, resource_nid) {

    var get_url = "?q=ntlp/comments/delete/wall/"+comment_nid+"/"+resource_nid;
    
    $.ajax({
        type: "GET",
        url: get_url,
        cache: false,
        success: function(data){

            document.getElementById('comment-wrapper-'+resource_nid).innerHTML = data;

        }
    
    });
}


//Called by Post New Comment button; available for each resource on News Feed
function delete_comments_from_single(comment_nid, resource_nid) {
    //   alert('here i am 2 '+resource_nid);
    var get_url = "?q=ntlp/comments/delete/"+comment_nid+"/"+resource_nid;
    //alert(get_url);
    $.ajax({
        type: "GET",
        url: get_url,
        cache: false,
        success: function(data){
         
            //            $("#node-"+resource_nid).html(data);
            document.getElementById('node-'+resource_nid).innerHTML = data;
            alert('Comment Deleted Successfully');
        }

    });
}


function focus_func(resource_nid){

    $('#txt_'+resource_nid).show();
    $('#comment_new_'+resource_nid).show();
    $('#txt_'+resource_nid).focus();
    
}

//function nltp_node_delete(comment_nid, resource_nid) {
//
//    var get_url = "?q=ntlp/comments/delete/"+comment_nid+"/"+resource_nid;
//    //alert(get_url);
//    $.ajax({
//        type: "GET",
//        url: get_url,
//        cache: false,
//        success: function(data){
//            $("#node-"+resource_nid).html(data);
////            alert('Comment Posted');
//        }
//
//    });
//}

function delete_resource_from_course(){

    var node_id = document.getElementById('node_id').value;
    var section_tid = document.getElementById('sectin_tid').value;
    section_tid = 0;
    $.ajax({
        type: "GET",
        url: '?q=ntlp/node/delete/data/'+node_id+'/'+section_tid,
        cache: false,
        success: function(){
            parent.Drupal.modalFrame.close().closeDialog();
        }
    });
}

function delete_resource_from_project(){

    var node_id = document.getElementById('node_id').value;
    var sectin_tid = document.getElementById('sectin_tid').value;
    
    $.ajax({
        type: "GET",
        url: '?q=ntlp/node/delete/data/'+node_id+'/'+sectin_tid,
        cache: false,
        success: function(){
            try {
                parent.Drupal.modalFrame.close().closeDialog();
            } catch (e) {
            }

            parent.window.location = '?q=ntlp/goback';
        }
    });
}

function delete_resource_cancel(){
    try {
        parent.Drupal.modalFrame.close().closeDialog();
    } catch (e) {

    }
}

function ntlp_ahah_success() {
    $('#edit-add-resources-project').sSelect();
    $('#edit-add-resources-project-section').sSelect();
}

$(document).ready(function() {
    var rdo = $("input[name='add_resources[selected_resource]']:checked");
    rdo.click();


    setupMenuButton('manageProject', "btnManageproject");

    $(".dialog-confirm-ntk-resource").dialog({
        autoOpen: false,
        resizable: false,
        height:210,
        width:400,
        modal: true
    });

    $("#dialog-show-confirmation").dialog({
        autoOpen: false,
        resizable: false,
        height:150,
        width:400,
        modal: true
    });

    fix_flash(document);
//    update_uploaded_video_image();
});

function update_uploaded_video_image() {
    
    $('a[name=player_link]').each( function() {
        // alert($(this).attr('href'));

        var href = $(this).attr('href');

        var href_arr = href.split('.');
        if (href_arr[href_arr.length-1] != 'flv') {

    //alert(href.substring(0, href.indexOf('.')) + '.flv');
    }
    });
}

function load_attributes(obj){

    var selecte_resource_type = $(obj).val();

    if(selecte_resource_type == 'lo_link'){
        
        $('#attach_resource_option').hide();
        $('#document_attach_option').hide();
        $('#upload_files_option').hide();
        $('#attach_link_for_link').show();
        $('#other_resource_link').hide();
        $('#page_data').hide();
        $('#page_attachments').hide();
        $('#google_doc_attached').hide();
        $('#google_docs_row').hide();
        $('#radio_doc_options').hide();
        $('#file-info-resource-file-wrapper').hide();


    }else if(selecte_resource_type == 'lo_image'){
        $('#attach_resource_option').show();
        $('#document_attach_option').hide();
        $('#upload_files_option').show();
        $('#attach_link_for_link').hide();
        $('#other_resource_link').hide();
        $("#resource_heading").html("Image*");
        $("#attach_link").html("Upload Image");
        $("span[id=attach_link_suffix]").html('<div style="padding-top:10px;">Max file size = 5 MB</div>');
        $("span[id=attach_link_tip]").html("Accepted file formats: JPG, PNG, GIF");
        $('#edit-add-resources-pageattachments-upload-control-file-info-validextentions').val('jpg,jpeg,png,gif');
        $('#page_data').hide();
        $('#page_attachments').hide();
        $('#google_doc_attached').hide();
        $('#google_docs_row').hide();
        $('#radio_attach_options').hide();
        $('#radio_doc_options').hide();

        $('#attach_links_panel').show();
        $('#video_embd').hide();
        $('#file-info-resource-file-wrapper').show();
    
    }else if(selecte_resource_type == 'lo_video'){
        $('#document_attach_option').hide();
        $('#attach_resource_option').show();
        $('#upload_files_option').show();
        $('#attach_link_for_link').hide();
        $('#other_resource_link').hide();
        $("#resource_heading").html("Media*");
        $("#attach_link").html("Upload Media file");
        $("span[id=attach_link_suffix]").html('<div style="padding-top:10px;">Max file size = 50 MB</div>');
        $("span[id=attach_link_tip]").html("File type = FLV only");
        $('#edit-add-resources-pageattachments-upload-control-file-info-validextentions').val('mov,wmv,mp4,mpeg,flv');
        $('#page_data').hide();
        $('#page_attachments').hide();
        $('#google_doc_attached').hide();
        $('#google_docs_row').hide();
        $('#radio_attach_options').show();
        $('#radio_doc_options').hide();

        if($('#edit-add-resources-resource-radio-U')[0].checked == true){
            $("span[id=attach_link_tip]").show();
            $('#upload_files_option').show();
            $('#video_embd').hide();
            $('#attach_links_panel').show();
            $('#file-info-resource-file-wrapper').show();
        }else{
            $("span[id=attach_link_tip]").hide();
            $('#upload_files_option').hide();
            $('#video_embd').show();
            $('#attach_links_panel').hide();
            $('#file-info-resource-file-wrapper').hide();
        }

    }else if(selecte_resource_type == 'lo_page'){
        $('#document_attach_option').hide();
        $('#attach_resource_option').hide();
        $('#upload_files_option').hide();
        $('#attach_link_for_link').hide();
        $('#other_resource_link').hide();
        $('#upload_files_option').hide();
        $('#page_data').show();
        $('#page_attachments').show();
        $('#google_doc_attached').hide();
        $('#google_docs_row').hide();
        $('#radio_doc_options').hide();
        $('#file-info-resource-file-wrapper').show();
        $('#edit-add-resources-pageattachments-upload-control-file-info-validextentions').val('');
        $("#attach_link").html("Attach file");

    }else if(selecte_resource_type == 'lo_document'){
        $('#attach_resource_option').show();
        $('#document_attach_option').show();
        $('#upload_files_option').show();
        $('#attach_link_for_link').hide();
        $('#other_resource_link').hide();
        $('#page_data').hide();
        $('#page_attachments').hide();
        //        $('#google_docs_row').hide();
        $('#radio_doc_options').show();
        $('#radio_attach_options').hide();

        $("#resource_heading").html("Document*");
        $("#attach_link").html("Attach a file");
        $("span[id=attach_link_tip]").html("One document only. Use \"Page\" to attach multiple files.");

        $('#attach_links_panel').show();
        $('#video_embd').hide();
        $('#edit-add-resources-pageattachments-upload-control-file-info-validextentions').val('');

        if($('#edit-add-resources-google-radio-upload')[0].checked == true){

            $('#google_doc_attached').hide();
            $('#upload_files_option').show();
            $('#attach_links_panel').show();
            $('#file-info-resource-file-wrapper').show();
            $("span[id=attach_link_suffix]").html('');

        }else{
            $("span[id=attach_link_suffix]").html('');
            
            if (!$('#google_docs_row').is(":visible")) {
                $('#google_doc_attached').show();
            }

            $('#upload_files_option').hide();
            $('#attach_links_panel').hide();
            $('#file-info-resource-file-wrapper').hide();
        }
    }
}

function show_google_option(google)
{
    var option=$(google).val();
    if(option=='gdocs'){
        $('#upload_files_option').hide();
        $('#attach_links_panel').hide();
        if (!$('#google_docs_row').is(":visible")) {
            $('#google_doc_attached').show();
        }
        $('#file-info-resource-file-wrapper').hide();

    } else {
        $('#google_doc_attached').hide();
        $('#upload_files_option').show();
        $('#google_docs_row').hide();
        $('#attach_links_panel').show();
        $('#file-info-resource-file-wrapper').show();
    }
}

function show_attachement_options(images)
{
    var option=$(images).val();
    if(option=='E'){

        $('#upload_files_option').hide();
        $('#attach_links_panel').hide();
        $('#video_embd').show();
        $('#file-info-resource-file-wrapper').hide();
    }
    else{
        $('#upload_files_option').show();
        $('#video_embd').hide();
        $('#attach_links_panel').show();
        $('#file-info-resource-file-wrapper').show();
    }
}


function onclick_remove_resource(resource_id, afterdelete_goback) {

    $('.dialog-confirm-ntk-resource').dialog( "option" , 'resource_id' , resource_id );
    $('.dialog-confirm-ntk-resource').dialog( "option" , 'afterdelete_goback' , afterdelete_goback );
 
    $(".dialog-confirm-ntk-resource").dialog('option', 'buttons',{
        'Delete': function() {
            var dlg = this;
            var resource_id = $(dlg).dialog('option','resource_id');
            var afterdelete_goback = $(dlg).dialog('option','afterdelete_goback');

            $.get('?q=ntlp/node/delete/data/'+resource_id+"/"+0, function(){
                $(dlg).dialog('close');

                if (afterdelete_goback){
                    window.location = '?q=ntlp/goback';
                }else{
                    window.location.reload();
                }
            })

        },
        Cancel: function() {
            $(this).dialog('close');
        }
    }
    );

    $('.dialog-confirm-ntk-resource').dialog('open');
}

//Page Attachment
function page_attach_upload_file(fileId, fileName, filePath){

    var root_path = Drupal.settings.basePath;
    var element;
    var d = document.getElementById('page_attach_links');
    var count = d.getElementsByTagName('div').length;

    if(document.getElementById("page_attached_files_hidden")){
        element = document.getElementById("page_attached_files_hidden");
        element.value += '-'+(count+1)+',F,'+fileId+','+fileName+','+filePath+' ;';

        d.innerHTML += '<div id="F_'+'-'+(count+1)+'">'+
        '<img style="position:relative;top:2px;margin-right:4px;" onclick="page_delete_attachment(\'F_'+'-'+(count+1)+'\');"  width="14" height="14" src="'+root_path+'themes/Boldr/Images/DeleteBtn.png" border="0" />'+
        '<a href="'+root_path+filePath+'" target="_blank">'+fileName+'</a></div>';
    }
}
function other_attach_upload_file(fileId, fileName, filePath){

    var root_path = Drupal.settings.basePath;
    var element;

    if(document.getElementById("attached_files_hidden")){
        element = document.getElementById("attached_files_hidden");
        element.value += 'F,'+'-'+fileId+','+fileName+','+filePath+';';
        $('#attach_link_prompt').hide();
        
        document.getElementById("attach_links").innerHTML += '<div id="F_'+'-'+fileId+'">'+
        '<img style="position:relative;top:2px;margin-right:4px;" onclick="other_delete_attachment(\'F_'+'-'+(fileId)+'\');"  width="14" height="14" src="'+root_path+'themes/Boldr/Images/DeleteBtn.png" border="0" />'+
        '<a href="'+root_path+filePath+'" target="_blank">'+fileName+'</a></div>';
    }

    $('span[id=attach_link_suffix]').hide();
}

function on_fileupload_success(fileId, fileName, filePath) {
    var rdo = $("input[name='add_resources[selected_resource]']:checked");
    var selected_resource_type = $(rdo).val();
    //        alert(selected_resource_type);
    if(selected_resource_type == 'lo_page'){
        //If user is using multiple file attachment control for page resource.
        if (Drupal.modalFrame.isOpen || $('#modalframe-container').size()) {
        }else{
            page_attach_upload_file(fileId, fileName, filePath);
        }
    } else {
        other_attach_upload_file(fileId, fileName, filePath);
    }
}

function attach_delete_hidden(file_id, element_id){
    element = document.getElementById(element_id);
    if (element) {
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
    return false;
}

function page_delete_attachment(id){

    //Remove (Mark X) in attached files list in the hidden variable
    page_attach_delete_hidden(id);

    //Remove UI element for the selected file.
    var d = document.getElementById('page_attach_links');
    var olddiv = document.getElementById(id);
    d.removeChild(olddiv);
}

function doc_delete_attachment(id){

    attach_delete_hidden(id, "google_hAttachedFiles");
    attach_delete_hidden(id, "edit-add-resources-stuff-google-hAttachedFiles");
    attach_delete_hidden(id, "edit-mysubmission-hAttachedFiles");
    attach_delete_hidden(id, "edit-main-info-hAttachedFiles");

    //Remove UI element for the selected file.
    var d = document.getElementById('google_urllist');
    var olddiv = document.getElementById(id);
    d.removeChild(olddiv);

    $('#google_doc_attached').show();
    $('#google_docs_row').hide();
}

function other_delete_attachment(id){
    $('#attach_link_prompt').show();
    $('a[id=attach_link]').show();

    //Remove (Mark X) in attached files list in the hidden variable
    attach_delete_hidden(id, "attached_files_hidden");

    //Remove UI element for the selected file.
    var d = document.getElementById('attach_links');
    var olddiv = document.getElementById(id);
    d.removeChild(olddiv);

    $('span[id=attach_link_suffix]').show();
}


function clear_all_contents() {

    $('#edit-add-resources-stuff-share-ideas').val('');

    $('#edit-add-resources-stuff-website-title').val('');
    $('#edit-add-resources-stuff-link-url').val('');

    remove_all_attachments();
    remove_all_gdocs();

    $('#edit-add-resources-stuff-video-embedded-code').val('Copy and paste <embed> or <iframe> code from other websites');
}

function remove_all_attachments() {
    $('#attach_link_prompt').show();
    $('a[id=attach_link]').show();

    //Remove UI element for the selected file.
    var d = document.getElementById('attach_links');

    $('div[id^=F_]').each(function() {
        attach_delete_hidden($(this).attr('id'), "attached_files_hidden");
        d.removeChild(this);
    });

    $('span[id=attach_link_suffix]').show();
}

function remove_all_gdocs() {

    var d = document.getElementById('google_urllist');

    $('div[id^=G_]').each(function() {
        attach_delete_hidden($(this).attr('id'), "attached_files_hidden");
        d.removeChild(this);
    });

    $('#google_doc_attached').show();
    $('#google_docs_row').hide();
}

function post_shared_resource() {

    var retVal = true;
    var strLinkHeading = $("#title").html();
    var pos = -1;
    var missing_resource_msg = 'You have not included RES_TYPE. Please add your resource before saving';

    if(strLinkHeading == "Add Image") {

        missing_resource_msg = missing_resource_msg.replace("RES_TYPE", 'an Image');

        if (!is_file_attached('attached_files_hidden')) {

            return form_set_error(null, missing_resource_msg);
        }

    }else if(strLinkHeading == 'Add Media'){

        missing_resource_msg = missing_resource_msg.replace("RES_TYPE", 'a Media');

        if ($('#edit-add-resources-stuff-video-upload-radio-embd').attr('checked')) {
            if ($('#edit-add-resources-stuff-video-embedded-code').val() == 'Copy and paste <embed> or <iframe> code from other websites' ||
                $('#edit-add-resources-stuff-video-embedded-code').val().length <= 0)
                retVal = form_set_error($('#edit-add-resources-stuff-video-embedded-code'), '');
            else {
                pos = $('#edit-add-resources-stuff-video-embedded-code').val().indexOf("</script>");
                if (pos >= 0) {
                    retVal = form_set_error($('#edit-add-resources-stuff-video-embedded-code'), '');
                }
            }
        }
        else {//upload case
            if (!is_file_attached('attached_files_hidden')) {
                return form_set_error(null, missing_resource_msg);
            }
        }

    }else if(strLinkHeading == 'Add Document'){

        missing_resource_msg = missing_resource_msg.replace("RES_TYPE", 'a Document');

        if ($('#edit-add-resources-stuff-google-radio-gd').attr('checked')) {
        if (!is_file_attached('edit-add-resources-stuff-google-hAttachedFiles')) {
            return form_set_error(null, missing_resource_msg);
        }
        }
        else if($('#edit-add-resources-stuff-google-radio-upld').attr('checked')) {
            if (!is_file_attached('attached_files_hidden')) {
                return form_set_error(null, missing_resource_msg);
            }
        }

    }else if(strLinkHeading == "Add Link"){

        missing_resource_msg = missing_resource_msg.replace("RES_TYPE", 'a link');

        if ($('#edit-add-resources-stuff-website-title').val().length <= 0)
            retVal = form_set_error($('#edit-add-resources-stuff-website-title'), '');
        else {
            pos = $('#edit-add-resources-stuff-website-title').val().indexOf("</script>");
            if (pos >= 0) {
                retVal = form_set_error($('#edit-add-resources-stuff-website-title'), '');
            }
        }

        if ($('#edit-add-resources-stuff-link-url').val().length <= 0)
            retVal = form_set_error($('#edit-add-resources-stuff-link-url'), '');
        else {
            pos = $('#edit-add-resources-stuff-link-url').val().indexOf("</script>");
            if (pos >= 0) {
                retVal = form_set_error($('#edit-add-resources-stuff-link-url'), '');
            }
        }
    }
    else { //share idea
        if ($('#edit-add-resources-stuff-share-ideas').val() ==  'Share your thoughts with the class' ||
            $('#edit-add-resources-stuff-share-ideas').val().length <= 0)
            retVal = form_set_error($('#edit-add-resources-stuff-share-ideas'), '');
        else {
            //Make sure that text message does not contain a script tag
            pos = $('#edit-add-resources-stuff-share-ideas').val().indexOf("</script>");
            if (pos >= 0) {
                retVal = form_set_error($('#edit-add-resources-stuff-share-ideas'), '');
            }
        }
    }
    
    return retVal;
}

function form_set_error(selector, msg) {

    if (selector)
        selector.addClass('error');

    if (msg.length > 0) {

        $("#dialog-show-confirmation").dialog('option', 'buttons',{
            'Close': function() {
                $(this).dialog('close');
            }
        });

        $("#ui-dialog-title-dialog-show-confirmation").html('OOPS!');
        $("#dialog-show-confirmation").find('.msg').css('color', '#000000').html(msg);
        $("#dialog-show-confirmation").dialog("open");
    }

    return false;
}

function is_file_attached(element_id){

    hData = document.getElementById(element_id);
    
    if (hData.value.length <= 0) return false;

    var files = hData.value.split(";");

    for(i = 0 ; i< files.length; i++){
        var attfile = files[i].split(",");

        if (attfile == "")
            continue;
        
        if(attfile[0] == 'X')
            continue;
        else
            return true;
    }

    return false;
}



function copy_resource(){
    var flag = true;
    resource_nid = $('#resource_nid').val();
    project_nid = $('#selec_project_nid').val();
    course_nid = $('#selec_course_nid').val();
    categeory_tid = $('#selec_categoery_tid').val();
    txt_resource_name = $('#txt_resource_name').val();

    if(txt_resource_name == ''){
        flag = false;
        msg = 'Please enter resource name before copy';
    }else if(course_nid == 0){
        flag = false;
        msg = 'Please Select Course before copy';
    }else if(project_nid == 0){
        flag = false;
        msg = 'Please Select project before copy';
    }else if(categeory_tid == 0){
        flag = false;
        msg = 'Please Select category before copy';
    }

    if(flag){
        $.ajax({
            type: "GET",
            url: '?q=ntlp/copy/resource/'+course_nid+'/'+project_nid+'/'+ categeory_tid+'/'+resource_nid+'/'+encodeURIComponent(txt_resource_name),
            cache: false,
            success: function(data){
                try {
                    parent.Drupal.modalFrame.close().closeDialog();
                } catch (e) {
                }
                parent.load_confirmation_dialog(course_nid, project_nid);
            }
        });
    }else{
        $("#dialog-show-confirmation").dialog('option', 'buttons',{
            'close': function() {
                $(this).dialog('close');
            }
        });
        $('#dialog-show-confirmation').dialog('option','height',100);
        $('#dialog-show-confirmation').dialog('option','minHeight',100);
        
        $("#ui-dialog-title-dialog-show-confirmation").html('Error');
        $("#dialog-show-confirmation").find('.msg').css('color', '#000000').html(msg);
        $("#dialog-show-confirmation").dialog("open");
    }
    
}

function load_confirmation_dialog(course_nid, project_nid){

    message =  'Resource was copied successfully! <br><br> <a class="green_link" href="?q=ntlp/courses/projects/'+course_nid+'/'+project_nid+'">&gt; View the new resource in its project briefcase </a>';
    $("#dialog-show-confirmation").dialog('option', 'buttons',{
        'close': function() {
            $(this).dialog('close');
        }
    });
    
    $("#ui-dialog-title-dialog-show-confirmation").html('Status');
    $("#dialog-show-confirmation").find('.msg').css('color', '#000000').html(message);
    $("#dialog-show-confirmation").dialog("open");
    
}

function fix_flash(container) {
    // loop through every iframe tag on the site and modify the src url
    var iframes = document.getElementsByTagName('iframe');
    //    alert('iframe:'+iframes.length);
    for (i = 0; i < iframes.length; i++) {
        iframe = iframes[i];

        if (iframe.src.search("http:\/\/www.youtube.com\/embed\/") == 0) {
            if (iframe.src.search(/wmode=transparent/i) <= 0) {
                iframe.src += "?wmode=transparent";
            }
        }
    //        alert(iframe.src);
    }

    // loop through every embed tag on the site
    var embeds = container.getElementsByTagName('embed');
    //    alert('embeds:'+embeds.length);
    for (i = 0; i < embeds.length; i++) {
        embed = embeds[i];
        var new_embed;
        // everything but Firefox & Konqueror
        if (embed.outerHTML) {
            var html = embed.outerHTML;
            // replace an existing wmode parameter
            if (html.match(/wmode\s*=\s*('|")[a-zA-Z]+('|")/i))
                new_embed = html.replace(/wmode\s*=\s*('|")window('|")/i, "wmode='transparent'");
            // add a new wmode parameter
            else
                new_embed = html.replace(/<embed\s/i, "<embed wmode='transparent' ");
            // replace the old embed object with the fixed version
            embed.insertAdjacentHTML('beforeBegin', new_embed);
            embed.parentNode.removeChild(embed);
        } else {
            // cloneNode is buggy in some versions of Safari & Opera, but works fine in FF
            new_embed = embed.cloneNode(true);
            if (!new_embed.getAttribute('wmode') || new_embed.getAttribute('wmode').toLowerCase() == 'window')
                new_embed.setAttribute('wmode', 'transparent');
            embed.parentNode.replaceChild(new_embed, embed);
        }
    }
    // loop through every object tag on the site
    var objects = container.getElementsByTagName('object');
    //    alert('objects:'+objects.length);
    for (i = 0; i < objects.length; i++) {
        object = objects[i];
        var new_object;
        // object is an IE specific tag so we can use outerHTML here
        if (object.outerHTML) {
            var html = object.outerHTML;
            // replace an existing wmode parameter
            if (html.match(/<param\s+name\s*=\s*('|")wmode('|")\s+value\s*=\s*('|")[a-zA-Z]+('|")\s*\/?\>/i))
                new_object = html.replace(/<param\s+name\s*=\s*('|")wmode('|")\s+value\s*=\s*('|")window('|")\s*\/?\>/i, "<param name='wmode' value='transparent' />");
            // add a new wmode parameter
            else
                new_object = html.replace(/<\/object\>/i, "<param name='wmode' value='transparent' />\n</object>");
            // loop through each of the param tags
            var children = object.childNodes;
            for (j = 0; j < children.length; j++) {
                try {
                    if (children[j] != null) {
                        var theName = children[j].getAttribute('name');
                        if (theName != null && theName.match(/flashvars/i)) {
                            new_object = new_object.replace(/<param\s+name\s*=\s*('|")flashvars('|")\s+value\s*=\s*('|")[^'"]*('|")\s*\/?\>/i, "<param name='flashvars' value='" + children[j].getAttribute('value') + "' />");
                        }
                    }
                }
                catch (err) {
                }
            }
            // replace the old embed object with the fixed versiony
            object.insertAdjacentHTML('beforeBegin', new_object);
            object.parentNode.removeChild(object);
        }
    }
}
