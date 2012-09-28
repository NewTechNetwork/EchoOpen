// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$(function(){
    $('#dialog-confirm-del-resource').dialog({
        autoOpen: false,
        modal: true,
        buttons: {
            'Yes' : function(){
                var resId = $('#dialog-confirm-del-resource').dialog('option', 'resId');
                $.get('?q=ntlp/document/library/resource/delete/data/'+resId, function(){
                    $(this).dialog('close');
                    window.location = '?q=ntlp/goback';
                });
            //                $(this).dialog('close');
            },
            'No': function(){ 
                $(this).dialog('close');
            }
        }
    });
    
    $("#dialog-show-confirmation").dialog({
        autoOpen: false,
        resizable: false,
        height:90,
        width: 400,
        modal: true,
        zIndex:9999
    });
    
    initConfirmDialog();
});

function initConfirmDialog(){
    $("#dialog-confirm-cat").dialog({
        autoOpen: false,
        resizable: false,
        height:140,
        modal: true
    });
}

function deleteResource( resourceId ) {
    $('#dialog-confirm-del-resource').dialog('option', 'resId', resourceId);
    $('#dialog-confirm-del-resource').dialog('open');
}

function show_data_onload(){

    $('#resourcesname').show();
    $('#rname').show();
    $('#google').hide();
    $('#googledoc').hide();
    $('#url').hide();
    $('#uploadlink').hide();
    $('#upload_link').show();
    $('#file_attachment').hide();
    $('#url_s').hide();
    $('#linkurl').show();
    $('#link_url').show();
    $('#assproject').show();
    $('#ass_project').show();
    $('#resources').hide();
    $('#desc').hide();
    $('#description').show();
    $('#rsrctxt').hide();
    $('#googleattached').hide();
    $('#google_attached').hide();

}

function show_hides(fields)
{

    var option=$(fields).val();

    if(option=='lo_links')
    {
        $('#resourcesname').show();
        $('#google').show();
        $('#url').hide();
        $('#uploadlink').hide();
        $('#file_attachment').hide();
        $('#linkurl').show();
        $('#assproject').show();
        $('#resources').hide();
        $('#description').show();
        $('#google_attached').hide();
        $('#google_doc_attached').hide();
    }
    else
    if(option=='lo_images')
    {
        $('#resourcesname').hide();
        $('#google').hide();
        $('#url').hide();
        $('#uploadlink').show();
        $('#file_attachment').show();
        $('#linkurl').hide();
        $('#assproject').hide();
        $('#resources').show();
        $('#description').show();
        $('#google_attached').hide();
        $('#google_doc_attached').hide();
    }
    else
            
    if(option=='lo_videos')
    {
        $('#resourcesname').hide();
        $('#google').hide();
        $('#url').hide();
        $('#uploadlink').show();
        $('#file_attachment').show();
        $('#linkurl').hide();
        $('#assproject').hide();
        $('#resources').show();
        $('#description').show();
        $('#google_attached').hide();
        $('#google_doc_attached').hide();
    }
    else
    if(option=='lo_documents')
    {
        $('#resourcesname').hide();
        $('#google').hide();
        $('#url').show();
        $('#uploadlink').hide();
        $('#file_attachment').show();
        $('#linkurl').hide();
        $('#assproject').hide();
        $('#resources').show();
        $('#description').show();
        $('#google_attached').hide();
        $('#google_doc_attached').hide();
    }
    else
    if(option=='lo_pages')
    {
        $('#resourcesname').hide();
        $('#google').hide();
        $('#url').hide();
        $('#uploadlink').hide();
        $('#file_attachment').hide();
        $('#linkurl').hide();
        $('#assproject').hide();
        $('#resources').hide();
        $('#description').show();
        $('#rsrctxt').hide();
        $('#google_attached').hide();
        $('#google_attached').hide();
        $('#google_doc_attached').hide();
    }
}
function show_images(images)
{
    var option=$(images).val();
    if(option=='L')
    {
        $('#url').show();
        $('.uploadFiles').css("display","none");
        $('#file_attachment').hide();
        $('#fileattach').hide();
    }
    else
    {
        $('.uploadFiles').show();
        $('#url').hide();
        $('#file_attachment').show();
        $('#fileattach').show();

    }

}

function show_google(google)
{
    var option=$(google).val();
    if(option=='gd')
    {
        $('#url').hide();
        $('#file_attachment').hide();
        $('#google_attached').show();
        $('#fileattach').show();
        
    }
    else
    {
        $('#url').show();
        $('#file_attachment').show();
        $('#google_attached').hide();


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

function attach_google_docs(){
    var root_path = Drupal.settings.basePath;
    //
    // alert('here i m ');

    //    if(document.getElementById("edit-add-resources-google-hAttachedFiles") != null){
    //        element = document.getElementById("edit-add-resources-google-hAttachedFiles");
    //
    //    }else if(document.getElementById("edit-add-resources-stuff-resoruce-uploadFiles") != null){
    //        element = document.getElementById("edit-add-resources-stuff-resoruce-uploadFiles");
    //    }else{
    //        
    //    }
    var  element = parent.document.getElementById("attached_files_hidden_for_google_docs");
    //    var  element = $("#attached_files_hidden_for_google_docs");
    if (element) {
        var d = parent.document.getElementById('google_urllist');
        if(d == null || d == 'undefined'){
            d = parent.document.getElementById('urllist');
        }
        var x=document.getElementsByName("gdocs[]");

        for(i=0; i< x.length; i++){
            var d = parent.document.getElementById('google_urllist');
            var count = d.getElementsByTagName('div').length;
            
            if(x[i].checked){
                var docLink = x[i].value;
                var docName = x[i].id;
                element.value += '-'+(count+1)+',G,'+docName+','+docLink+' ;';
                
                //                element.value = 'G,'+'-'+(count+1)+','+data+' ;';
                d.innerHTML += '<div id="G_'+'-'+(count+1)+'">'+
                '<img style="position:relative;top:2px;margin-right:4px;" onclick="doc_delete_attachment(\'G_'+'-'+(count+1)+'\');" width="14" height="14" src="'+root_path+'themes/Boldr/Images/DeleteBtn.png" border="0" />'+
                '<a href="'+docLink+'" target="_blank">'+docName+'</a></div>';
            
            }
        }
        
        parent.document.getElementById('google_docs_row').style.display='block';
        if(parent.document.getElementById('google_doc_attached'))
            parent.document.getElementById('google_doc_attached').style.display='none';
    }
}

//Drupal.settings.basePath
/*  Start : Java Script Code For Add Resource Stuff*/

function link_field_hide(){

    var path = Drupal.settings.basePath;

    $('#stuffMainTable').show();

    $("#edit-add-resources-stuff-share-ideas").focus();

    $('#table_hide').fadeIn('slow');

    $('#resourcesname').show();

    $("#uploadlink").hide();

    $("#title").html("Add Link");
   
    $("#path").html("<img src=\""+path+"themes/Boldr/Images/resourceStuff/img_link.png \" alt=\"Link Image\" />");

    $("#google").hide();

    $("#linkurl").show();

    $("#resource-stuff-wrapper").show();

    $("#links").hide();

    $("#close_button").show();

    $("#description").show();

    $(".associated_project").show();

    $("#post").show();

    document.getElementById('type').value = 'lo_link';

    $("#title_resource").show();

    $("#title").html("Add Link");

    $("#google_attached").hide();

    $('#google_urllist').hide();

    $('#file_attachment').hide();
}



function image_field_hide(){

    var path = Drupal.settings.basePath;
    $('#table_hide').fadeIn('slow');

    $("#stuffMainTable").show();

    $("#edit-add-resources-stuff-share-ideas").focus();

    $("#path").html("<img src=\""+path+"themes/Boldr/Images/resourceStuff/img_picture.png \" alt=\"AttachedLink\" />");

    $("#link_heading").html("Images");

    $('#google_urllist').hide()

    $("#title_resource").show();

    $("#resource-stuff-wrapper").show();

    $("#title").html("Add Image");

    $('#resourcesname').show();

    $("#uploadlink").show();

    $("#linkurl").hide();

    $("#link").hide();

    $("#google").hide();

    $('#google_doc').hide()
  
    $("#links").hide();

    $("#close_button").show();

    $("#associate_project").show();

    $("#description").show();

    $(".associated_project").show();

    $("#post").show();

    $("#google_attached").hide();

    $('#file_attachment').show();

    $('#url').hide();

    $('#fileattach').show();
     
    document.getElementById('type').value = 'lo_image';


    document.getElementById('edit-add-resources-stuff-resource-radio-L').checked = false;
    document.getElementById('edit-add-resources-stuff-resource-radio-U').checked = true;

}


function video_field_hide(){
    var path = Drupal.settings.basePath;
    $('#table_hide').fadeIn('slow');
    
    $("#stuffMainTable").show();

    $("#edit-add-resources-stuff-share-ideas").focus();

    $("#path").html("<img src=\""+path+"themes/Boldr/Images/resourceStuff/img_video.png \" alt=\"Video Image\" />");

    $("#link_heading").html("Videos");

    $("#link").hide();

    $("#title_resource").show();

    $("#resource-stuff-wrapper").show();

    $("#title").html("Add Video");
 
    $('#url').hide();

    $('#resourcesname').show();

    $('#google_urllist').hide()

    $("#uploadlink").show();

    $("#linkurl").hide();

    $("#google").hide();

    $("#links").hide();

    $("#close_button").show();

    $("#associate_project").show();

    $("#description").show();

    $('#file_attachment').show();

    $(".associated_project").show();

    $("#post").show();

    $("#google_attached").hide();

    $('#fileattach').show();

    document.getElementById('type').value = 'lo_video';

    document.getElementById('edit-add-resources-stuff-resource-radio-L').checked = false;
    document.getElementById('edit-add-resources-stuff-resource-radio-U').checked = true;

//    if(document.getElementById('edit-add-resources-stuff-resource-radio').checked == true){
//
//     alert('ssssss') ;
//
//    }

   
}



function document_field_hide(){
    var path = Drupal.settings.basePath;
    $("#stuffMainTable").show();

    $('#table_hide').fadeIn('slow');

    
    $("#path").html("<img src=\""+path+"themes/Boldr/Images/resourceStuff/img_document.png \" alt=\"Document Image\" />");

    $("#title_resource").show();

    $("#link").hide();

    $("#edit-add-resources-stuff-share-ideas").focus();

    $("#google_attached").show();

    //    $('#google_urllist').hide()

    $("#link_heading").html("Document");

    $("#title").html("Add Document");

    $("#resource-stuff-wrapper").show();

    $('#resourcesname').show();

    $("#linkurl").hide();

    $("#google").show();

    $("#uploadlink").hide();

    $("#links").hide();

    $("#close_button").show();

    $("#associate_project").show();

    $("#description").show();

    $('#file_attachment').show();
    
    $(".associated_project").show();

    $("#post").show();

    $("#google_attached").hide();

    $('#fileattach').show();
   
    document.getElementById('type').value = 'lo_document';

    document.getElementById('edit-add-resources-stuff-google-radio-gd').checked = false;
    document.getElementById('edit-add-resources-stuff-google-radio-upld').checked = true;

}



function close_button(){

    $("#stuffMainTable").hide();

    $("#links").show();
    
    $("#edit-add-resources-stuff-share-ideas").focus();

    $('#table_hide').fadeOut('slow');

    $('.uploadFiles').show();


    $("#description").show();

    $('#resourcesname').hide();

    $("#resource-stuff-wrapper").hide();

    $('#google_urllist').hide()

    $("#linkurl").hide();

    $("#google").hide();

    $("#uploadlink").hide();

    $("#links").show();
    
    $("#close_button").hide();

    $("#associate_project").hide();

    $("#description").hide();

    $(".associated_project").hide();



    $("#post").show();

    $("#title_resource").hide();

    $("#link").hide();

    $("#google_attached").hide();

    $('#file_attachment').hide();

    $('#url').hide();

    $('#google_attached').hide();

    $('#fileattach').hide();
 
}


function resource_show_google(google)
{
    var option=$(google).val();
    if(option=='gd')
    {
        $('#url').hide();
        $('#file_attachment').hide();
        $('#google_attached').show();
        $('#fileattach').hide();

    }
    else
    {
        $('#url').hide();
        $('#file_attachment').show();
        $('#google_attached').hide();
        $('#google_urllist').hide();
        $('#fileattach').show();

    }
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
    var comment_text =  $('#txt_'+resource_nid).val();
    var get_url = "?q=ntlp/comments/new/"+resource_nid+"/"+comment_text;
    //alert(get_url);
    $.ajax({
        type: "GET",
        url: get_url,
        cache: false,
        success: function(data){
            $("#node-"+resource_nid).html(data);
        //            alert('Comment Posted');
        }

    });
}

//Called by Post New Comment button; available for each resource on News Feed
function delete_comments(comment_nid, resource_nid) {

    var get_url = "?q=ntlp/comments/delete/"+comment_nid+"/"+resource_nid;
    //alert(get_url);
    $.ajax({
        type: "GET",
        url: get_url,
        cache: false,
        success: function(data){
            $("#node-"+resource_nid).html(data);
            alert('Comment Deleted Successfully');
        }

    });
}


function focus_func(resource_nid){

    $('#txt_'+resource_nid).show();
    $('#comment_new_'+resource_nid).show();
    $('#txt_'+resource_nid).focus();
    
}

function show_hide_grade(obj){

    //    alert($(obj).val());

    if($(obj).val()==1){
        document.getElementById('grade_scale_test').style.display='block';
    }else {
        document.getElementById('grade_scale_test').style.display='none';
    }
}

function show_hide_learning_outcome(obj){

    if($(obj).val()==1){
        document.getElementById('add_outcome_test').style.display='block';
    }else {
        document.getElementById('add_outcome_test').style.display='none';
    }
}



function get_outcome_combo(maxCategory) {
    var main = document.getElementById('ocContainer');
    var count = main.getElementsByTagName('tr').length;
    if(count>maxCategory) {
        alert('You can add this resource to maximum of '+maxCategory+' categories');
        return false;
    }
    data1 = $('#oc_td1').val();
    data1 = data1.replace("%ID%", "outcome_cbo_"+count);
    data1 = data1.replace("%INDEX%", count);
    //    document.getElementById("selected_oc_id").value += 'x,';

    var newOC = document.createElement('tr');
    var td1 = document.createElement('td');
    var td2 = document.createElement('td');
    newOC.appendChild(td1);
    newOC.appendChild(td2);
    td1.innerHTML = data1;
    newOC.id = "outcome_"+(count);
    //  newOC.innerHTML = data;
    main = document.getElementById('ocLastRow');
    main.parentNode.insertBefore(newOC, main);
//    $('#'+newOC.id+' .my-dropdown').Select(); 

}


function onchange_outcome_hidden(n, index){
    if(n.value != 0){
        var oc_id = document.getElementById('selected_oc_id');
        find_and_replace(oc_id, index, n.value);
    } else {
        document.getElementById('outcome_div_'+index).style.display = "block";
    }
}

//function onchange_outcome_hidden(obj, index){
//    alert($(obj).val());
//    var a = document.getElementById("txt_"+index).value;
//    alert(a);
//    document.getElementById("selected_oc").value += ','+$(obj).val()+','+index;
//
//}



function find_and_replace(element, index, value){

    var arr = element.value.split(",");
    arr[index] = value;
    var new_ids = new Array();
    for(i=0; i<arr.length; i++){

        arr[index].replace(arr[index], value);
    }

    element.value = arr;

}



function library_save_category( ){

    var category_name = document.getElementById('txt_category_name').value;
    var category_desc = document.getElementById('txt_category_description').value;
    var module_nid = document.getElementById('module_id').value;
    var category_id = document.getElementById('category_id').value;
    var message = "";
    var flag = true;
    
    category_name = urlencode(category_name);

    $("#dialog-show-confirmation").dialog('option', 'buttons',{
        'Close':function(){
            $(this).dialog('close');
        }
    });
    
    var get_url = "?q=ntlp/library/save/category/"+module_nid+"/"+category_id;
    
    if(flag){
        
        $.ajax({
            type: "POST",
            url: get_url,
            cache: false,
            data: "name="+category_name+"&desc="+category_desc,
            success: function(data){
                parent.window.location.reload();
                parent.Drupal.modalFrame.close();
            }
    
        });    
    }else{
        $("#ui-dialog-title-dialog-show-confirmation").text('Oops!');
        $("#dialog-show-confirmation").find('.msg').css('color', '#000000').text(message);
        $("#dialog-show-confirmation").dialog("option", "minHeight", 100 );
        $("#dialog-show-confirmation").dialog('open');        
    }
 
   
}

function library_cancel_category(){
    try {
        parent.Drupal.modalFrame.close().closeDialog();
    } catch (e) {

    }
}



function checkAll(id){
    
    if(document.getElementById('all_'+id).checked == true){
        document.getElementById('teacher_'+id).checked = true;
        document.getElementById('student_'+id).checked = true;
        document.getElementById('parent_'+id).checked = true;
    }else{
        document.getElementById('teacher_'+id).checked = false;
        document.getElementById('student_'+id).checked = false;
        document.getElementById('parent_'+id).checked = false;
    }
}


function set_category_permission(id){

    //    document.getElementById('teacher_'+id).checked = true;
    //    document.getElementById('student_'+id).checked = true;
    //    document.getElementById('parent_'+id).checked = true;
   
    if(document.getElementById('parent_'+id).checked == true ){
        data = $('#parent_'+id).val();
        state = document.getElementById('parent_'+id).checked;

    }

    if(document.getElementById('teacher_'+id).checked == true ){
        data = $('#teacher_'+id).val();
        state = document.getElementById('teacher_'+id).checked;

    }
    
    if(document.getElementById('student_'+id).checked == true ){
        data = $('#student_'+id).val();
        state = document.getElementById('student_'+id).checked;

    }


    category_permission = document.getElementById("categories_perm");
  
    x = category_permission.value.split(',');
        
    if(state == true){
        category_permission.value += data+',';
    }else{
        for(i = 0; i<x.length ; i++){
            if(x[i] == data){
                category_permission.value = category_permission.value.replace(data+',','');
            }
        }
    }
//    alert(data);
//    alert(category_permission.value);
    
}

function focus_func(resource_nid){

    $('#txt_'+resource_nid).show();
    $('#comment_new_'+resource_nid).show();
    $('#txt_'+resource_nid).focus();

}
$(document).ready(
    function() {
        $("#edit-keys").css('display', 'none');
        $('.txtbox_focus_text').focus(
            function() {
                $(this).val('');
            }
            );
        $('.txtbox_focus_text').blur(
            function() {
                var title = $(this).attr('title');
                if($(this).val()== '') {
                    $(this).val(title);
                }else {
            }
            }
            );
    }
    );


function close_filter(){

    $('#table_hide').slideToggle(function(){
        if($('#table_hide').css('display') == 'block' ) {
            $('.filter_link').html(' &laquo; Close');
        }else{
            $('.filter_link').html(' &laquo; Open');
        }
    });
 

}

$(document).ready(function() {
    var rdo = $("input[name='add_resources[selected_type]']:checked");
    resource_type = rdo.val();
    //    alert(rdo.val());
    rdo.click();

    //    if($('#edit-add-resources-selected-type-lo-page')[0] != undefined){
    //        $('#edit-add-resources-selected-type-lo-page')[0].checked = true;
    //    }edit-add-resources-google-radio-gdocs
    if(resource_type  == 'lo_image' || resource_type == 'lo_video'){
        var rdo1 = $("input[name='add_resources[resource_radio]']:checked");
        rdo1.click();
    }
    
    if(resource_type  == 'lo_document'){
        var rdo1 = $("input[name='add_resources[google_radio]']:checked");
        //        alert(rdo1.val());
        rdo1.click(); 
    // I didn't change any thing I just refresh the page again? oh, then problem is not fixed
    //our problem is that the load function should be called after validate, which is not caling
    // may be sir. but I think the problem is in rdo1.click(); this when this click the radio button
    // this load the other fields related to teh resource type where google doc is default.
    // no you don't understand... this clicks the "selected" radio button.\

    // question is why is the gdocs button is selected, problem is in session var, lets go back to php function'

    }

    if(resource_type == 'lo_page'){
        //    $('.upldFile').hide();
        $('#page_data').show();
    //    $('#upload_files_option').css('display','none');
    }
});

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
        $('#google_attached').hide();
        $('#google_doc_attached').hide();

    }else if(selecte_resource_type == 'lo_image'){
        $('#attach_resource_option').show();
        $('#document_attach_option').hide();
        $('#upload_files_option').show();
        $('#attach_link_for_link').hide();
        $('#other_resource_link').hide();
        $("#resource_heading").html("Images");
        $('#page_data').hide();
        $('#page_attachments').hide();

        var rdo1 = $("input[name='add_resources[resource_radio]']:checked");
        if(rdo1.val() == "U"){
            rdo1.click();
            $('.upldFile').show();
            $('#google_attached').hide();
            $('#google_doc_attached').hide();
        }else if(rdo1.val() == "L"){
            $('.upldFile').hide();
            $('#google_attached').hide();
            $('#google_doc_attached').hide();
        }else{
            $('.upldFile').hide();
            $('#google_doc_attached').show();
            $('.other_resource_link').hide();
        }

    }
    else if(selecte_resource_type == 'lo_video'){
        $('#document_attach_option').hide();
        $('#attach_resource_option').show();
        $('#upload_files_option').show();
        $('#attach_link_for_link').hide();
        $('#other_resource_link').hide();
        $("#resource_heading").html("Video");
        $('#page_data').hide();
        $('#page_attachments').hide();

        //        if($("#edit-add-resources-resource-radio-U")[0].checked = true){
        var rdo1 = $("input[name='add_resources[resource_radio]']:checked");
        if(rdo1.val() == "U"){
            $('.upldFile').show();
            $('#google_attached').hide();
            $('#google_doc_attached').hide();
            $('.other_resource_link').hide();

        //        }else if($('#edit-add-resources-resource-radio-L')[0].checked = true){
        }else if(rdo1.val() == "L"){
            $('.upldFile').hide();
            $('#google_attached').hide();
            $('#google_doc_attached').hide();
            $('.other_resource_link').show();
        }else{
            $('.upldFile').hide();
            $('#google_doc_attached').show();
            $('.other_resource_link').hide();
        }

    }else if(selecte_resource_type == 'lo_page'){
        $('#document_attach_option').hide();
        $('#attach_resource_option').hide();
        $('#upload_files_option').hide();
        $('#attach_link_for_link').hide();
        $('#other_resource_link').hide();
        $('#page_data').show();
        $('#page_attachments').show();
        $('#google_attached').hide();
        $('#google_doc_attached').hide();

    }else if(selecte_resource_type == 'lo_document'){ // this call this function where. google is set by default
        
        
        $('#attach_resource_option').hide();
        $('#document_attach_option').show();
        $('#upload_files_option').show();
        $('#attach_link_for_link').hide();
        $('#other_resource_link').hide();
        $('#page_data').hide();
        $('#page_attachments').hide();

        var rdo1 = $("input[name='add_resources[google_radio]']:checked");

        if(rdo1.val() == 'upload'){
           
            $('.upldFile').show();
            $('#upload_files_option').show();
            $('#google_attached').hide();
            $('#google_doc_attached').hide();
            $('.other_resource_link').show();
            $('#google_docs_row').hide();

        }else if(rdo1.val() == 'gdocs'){

            $('#upload_files_option').hide();
            $('#google_doc_attached').show();
            $('.other_resource_link').hide();
            $('#google_docs_row').show();
        }else{
            $('.upldFile').hide();
            $('#google_attached').hide();
            $('#google_doc_attached').hide();
            $('.other_resource_link').show();
            
        }
    }
}

function show_attachement_options(images)
{
    var option=$(images).val();
    if(option=='L'){

        $('#attach_link_for_link').hide();
        $('#other_resource_link').show();
        $('#upload_files_option').hide();
        $('#google_attached').hide();
        $('#google_doc_attached').hide();
        $('.upldFile').hide();
    }
    else{
        $('.upldFile').show();
        $('#upload_files_option').show();
        $('#attach_link_for_link').hide();
        $('#google_attached').hide();
        $('#google_doc_attached').hide();
        $('#other_resource_link').hide();
    }
}

function show_google_option(google)
{
    var option=$(google).val();
    if(option=='gdocs'){
        if($('#attached_files_hidden_for_google_docs').val() != ''){
            $('#google_doc_attached').hide();    
        }else{
            $('#google_doc_attached').show();    
        }
        
        $('#upload_files_option').hide();
    } else {
        $('#upload_files_option').show();
        $('#google_doc_attached').hide();
    }

//    $('#edit-add-resources-google-hAttachedFiles').val('');
//    $('#google_urllist').html('');
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


///////////////////

//Page Attachment
function page_attach_upload_file(data,fileId){

    var root_path = Drupal.settings.basePath;
    var element;

    if(document.getElementById("page_attached_files_hidden")){
        element = document.getElementById("page_attached_files_hidden");
        element.value += 'F,'+'-'+fileId+','+data+' ;';

        document.getElementById("page_attach_links").innerHTML += '<div id="F_'+'-'+fileId+'"> <a style="vertical-align:middle;" '
        +' onclick="page_delete_attachment(\'F_'+'-'+(fileId)+'\');" ><img width="14" height="14" src="'+root_path+'themes/Boldr/Images/DeleteBtn.png" '
        +' border="2" /></a>&nbsp;&nbsp;'+data+'</div>';
    }
}
function other_attach_upload_file(data,fileId){

    var root_path = Drupal.settings.basePath;
    var element;

    if(document.getElementById("attached_files_hidden")){
        element = document.getElementById("attached_files_hidden");
        element.value += 'F,'+'-'+fileId+','+data+' ;';
        $('#attach_link').hide();

        document.getElementById("attach_links").innerHTML += '<div id="F_'+'-'+fileId+'"> <a style="vertical-align:middle;" '
        +' onclick="other_delete_attachment(\'F_'+'-'+(fileId)+'\');" ><img width="14" height="14" src="'+root_path+'themes/Boldr/Images/DeleteBtn.png" '
        +' border="2" /></a>&nbsp;&nbsp;'+data+'</div>';
    }
}

function on_fileupload_success(fileId, fileName) {

    var rdo = $("input[name='add_resources[selected_type]']:checked");
    var selected_resource_type = $(rdo).val();
    //    alert(selected_resource_type);
    if(selected_resource_type == 'lo_page'){
        //If user is using multiple file attachment control for page resource.
        if (Drupal.modalFrame.isOpen || $('#modalframe-container').size()) {
        }else{
            page_attach_upload_file(fileName,fileId);
        }
    } else {
        other_attach_upload_file(fileName,fileId);
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
    attach_delete_hidden(id, "page_attached_files_hidden");

    //Remove UI element for the selected file.
    var d = document.getElementById('page_attach_links');
    var olddiv = document.getElementById(id);
    d.removeChild(olddiv);
}


function other_delete_attachment(id){
    $('#attach_link').show();

    //Remove (Mark X) in attached files list in the hidden variable
    attach_delete_hidden(id, "attached_files_hidden");

    //Remove UI element for the selected file.
    var d = document.getElementById('attach_links');
    var olddiv = document.getElementById(id);
    d.removeChild(olddiv);
}


function delete_resource_from_library(resource_nid){
        
    $.ajax({
        type: "GET",
        url: '?q=ntlp/document/library/resource/delete/data/'+resource_nid,
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

function doc_delete_attachment(id){

    attach_delete_hidden(id, "attached_files_hidden_for_google_docs");
    //Remove UI element for the selected file.
    var d = document.getElementById('google_urllist');
    var olddiv = document.getElementById(id);
    d.removeChild(olddiv);

    $('#google_doc_attached').show();
    $('#google_docs_row').hide();
}


function remove_library_category(category_id) {
    
    var message = "<p>Are you sure you want to delete this category?</p>";

    $("#dialog-confirm-cat").dialog('option', 'buttons',{
        'Yes': function() {
            $.ajax({
                type: "GET",
                url: "?q=ntlp/document/library/categoryresourcecheck/"+category_id,
                success:  function(data){
                    $(this).dialog('close');
                    parent.Drupal.modalFrame.close();
                    jresponse = Drupal.parseJson(data);
                    if (jresponse.warning == 1) {
                        initConfirmDialog();
                        
                        message = "<p>This category contains one or more resources. If you delete this category, all resources within it will be deleted.</p><p>Are you sure you want to proceed?</p>";
                        $("#dialog-confirm-cat").dialog('option', 'buttons',{
                            'Yes': function() {
                                $.ajax({
                                    type: "GET",
                                    url: "?q=ntlp/document/library/delcategory/"+category_id,
                                    success:  function(){
                                        window.location.reload();
                                    }
                                });
                            },
                            'No': function() {
                                $(this).dialog('close');
                            }
                        });
                        
                        $("#ui-dialog-title-dialog-confirm-cat").html("WARNING:");
                        $("#dialog-confirm-cat").find('.msg').css('color', '#000000').html(message);
                        $("#dialog-confirm-cat").dialog('open');
                    }else{
                
                        $.ajax({
                            type: "GET",
                            url: "?q=ntlp/document/library/delcategory/"+category_id,
                            success:  function(){
                                window.location.reload();
                            }
                        });
                    }
                }
            });
        },
        'No': function() {
            $(this).dialog('close');
        }
    });
        
    $("#dialog-confirm-cat").find('.msg').css('color', '#000000').html(message);
    $("#dialog-confirm-cat").dialog('open');
}
