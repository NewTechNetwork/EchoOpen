// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
$(document).ready(function() {
    setupMenuButton('manageProject', "btnManageproject");
    setupMenuButton('manageResource', "btnManageresource");
});

function projectDetails(){

    $('#project_detail_edit').show();
    
    $('#project_detail').hide();
}


function resource_desc_tooltip(resource_nid, tooltip_text) {

   var selector_name = ".res"+resource_nid;
    $(selector_name).tipsy({
        opacity: 1,
        offset: 3 ,
        html: true,
        gravity: "nw",
        className : "yellow",
        arrowClass : "yellow-arrow",
        fallback: tooltip_text
    });
}
function changebuttonname(){
    
    $('#edit-project-details-proj-edit-button').innerHTML="New Button Text";
}

function resources_search() {

    var name =  $('#searching_text').val();

    $.ajax({
        type: "GET",
        url: "?q=ntlp/project/existing/resource/search",
        data: "name="+name,
        success:  function(data){
            $("#search_resources").html(data);

        }

    });
}

function sayHi(){

    alert('hello ');
}


function slide_up( val,tid,project_id) {

    var slide_order = 0;
    if(val != 0){

        slide_order = val -1;

    }

    
   
    var get_url ="?q=project/slider/slide_up/"+ slide_order + "/" + tid + "/"+ project_id;

    $.ajax({
        type: "GET",
        url: get_url,
        cache: false,
        success:  function(data){
            $("#new-row-wrapper").html(data);
    
        }

    });

}


function slide_down(val,tid,project_id) {
 
    var slide_order = 0;

    if(val != 0){

        slide_order = val +1;

    }
    
    var get_url ="?q=project/slider/slide_down/"+ slide_order + "/" + tid + "/"+ project_id;
    $.ajax({
        type: "GET",
        url: get_url,
        cache: false,
        success:  function(data){
            $("#new-row-wrapper").html(data);
        //
        }

    });

}


function save_to_library(course_nid, project_id ) {

    var project_name =  $('#txt_project_name').val();
    
    parent.save_project_to_library(project_name, project_id, course_nid);
}


function save_project_to_library(project_name, project_id, course_nid) {

    $.ajax({
        type: "GET",
        url: "?q=ntlp/project/save/library/"+course_nid+"/"+project_id+"/"+project_name,
        success:  function(){
            parent.Drupal.modalFrame.close();
            parent.window.location.reload();

        }
    //        data: "text="+atext,
    //        cache: false,
    //        success:  function(data) {
    //          $("#announcement_view").html(atext);
    //        }
    //        complete: function(response) {
    //          alert('completed'+response);
    //        },
    //        error: function(response) {
    //          alert('Error:'+response);
    //        }
    });
}


function copy_project(course_nid, project_id ) {

    var project_name =  $('#txt_project_name').val();
    
    parent.save_project(project_name, project_id, course_nid);
}


function save_project(project_name, project_id, course_nid) {

    $.ajax({
        type: "GET",
        url: "?q=ntlp/project/save/"+course_nid+"/"+project_id+"/"+project_name,
        success:  function(){
            window.location.reload();
         
        }
    //        data: "text="+atext,
    //        cache: false,
    //        success:  function(data) {
    //          $("#announcement_view").html(atext);
    //        }
    //        complete: function(response) {
    //          alert('completed'+response);
    //        },
    //        error: function(response) {
    //          alert('Error:'+response);
    //        }
    });

    try {
        Drupal.modalFrame.close().closeDialog();
    } catch (e) {

    }
}


function delete_project(course_nid, project_nid) {

    parent.delete_project_process(course_nid, project_nid);
}


function delete_project_process(course_nid, project_nid) {

    $.ajax({
        type: "GET",
        url: "?q=ntlp/project/remove/"+course_nid+"/"+project_nid,
        success:  function(){
            //            window.location.reload();
            window.location=""
         
        }

    });

    try {
        Drupal.modalFrame.close().closeDialog();

    } catch (e) {

    }
}


function on_imagechange(fileId, fileName) {
    //    alert('Inside library '+fileId + ":" + fileName);
    var module_id = $('#module_id').val();

    $.ajax({
        type: "GET",
        url: "?q=ntlp/save/module/image/"+fileId+"/"+module_id,
        cache: false,
        success:  function(data){
            $("#module_image").html(data);
            Drupal.behaviors.modalFrameSetup();
        }

    });
    //    alert('image upload successfully');
    Drupal.modalFrame.close();
}

function on_fileupload_success(fileId, fileName) {

//    $('#picture_fid').val() = fileId ;
}


function set_archived(project_id){


    document.getElementById("project_archive").value += project_id+',';
    
}

function copy_project(){

    var project_nid = document.getElementById("project_nid").value;
    var selected_course_nid = document.getElementById("selected_course_nid").value;
    var selected_project_name = document.getElementById("selected_project_name").value;

    //    alert('project nid '+project_nid +' selected course '+selected_course_nid + ' project name '+selected_project_name);

    $.ajax({
        type: "GET",
        url: "?q=ntlp/library/project/copy/"+project_nid+"/"+selected_course_nid+"/"+selected_project_name,
        cache: false,
        success:  function(){
            try {
                parent.Drupal.modalFrame.close().closeDialog();
            }
            catch (e) {

            }
            parent.window.location.reload();
        }

    });
       
}


function enable_reason(obj){


    if(obj.value == 1){
        document.getElementById("txt_reason").disabled=false;
       
    }else{
        document.getElementById("txt_reason").disabled=true;
    }
}

function save_archive_request(){
    var project_nid = document.getElementById('project_nid').value;
    var txt_reason = document.getElementById('txt_reason').value;
    var txt_additional_notes = document.getElementById('txt_additional_notes').value;

    //    alert('projectid '+project_nid +' reason '+txt_reason+ ' additional notes '+txt_additional_notes);


    $.ajax({
        type: "GET",
        url: "?q=ntlp/library/request/archive/"+project_nid+"/"+txt_reason+"/"+txt_additional_notes,
        cache: false,
        success:  function(data){
        //            window.location.reload();
        //                    $("#module_image").html(data);
        }

    }); 
    //    alert('image upload successfully');
    try {
        parent.Drupal.modalFrame.close().closeDialog();
    } catch (e) {

    }
    
}


function save_archived(){
    var project_nid = document.getElementById('project_nid').value;
    

    $.ajax({
        type: "GET",
        url: "?q=ntlp/library/project/archived/"+project_nid,
        cache: false,
        success:  function(data){
            //            $("#module_image").html(data);
            window.location.reload();
        }

    });
    try {
        parent.Drupal.modalFrame.close().closeDialog();
    } catch (e) {

    }
    
}

function save_published(){
    var project_nid = document.getElementById('project_nid').value;
    
    $.ajax({
        type: "GET",
        url: "?q=ntlp/library/project/published/"+project_nid,
        cache: false,
        success:  function(data){
        //            window.location.reload();
        }

    });
    try {
        parent.Drupal.modalFrame.close().closeDialog();
    } catch (e) {

    }
}

function showhide_comments(){
    $('#comment_item').hide();
    $('#comment_box').show();
}

function showhide_reviewing(){
    $('#review_item').hide();
    $('#review_box').show();
}


//Called by Post New Comment button; available for each resource on News Feed
function library_delete_comments(comment_cid, lib_project_nid) {

    var get_url = "?q=ntlp/comments/delete/"+comment_cid+"/"+lib_project_nid;
    //alert(get_url);
    $.ajax({
        type: "GET",
        url: get_url,
        cache: false,
        success: function(data){
        //$("#comment_item"+lib_project_nid).html(data);
        //            alert('Comment Deleted Successfully');
        }
    });
}


//Called by Post New Comment button; available for each resource on News Feed
function library_comments_post_new(lib_project_nid,type) {
    if(type=='posting'){
        var comment_text =  $('#txt_post_'+lib_project_nid).val();
        var comment_cid = $('#comment_cid_'+lib_project_nid).val();
        var get_url = "?q=ntlp/library/project/posting/"+lib_project_nid+"/"+comment_cid+"/0/"+comment_text;
    }
    if(type=='comments'){
        var comment_text =  $('#txt_comment_'+lib_project_nid).val();
        var comment_cid = $('#comment_cid_'+lib_project_nid).val();
        var get_url = "?q=ntlp/library/project/comments/"+lib_project_nid+"/comments/0/"+comment_text;
    }
    if(type=='reviewing'){
        var comment_text =  $('#txt_review_'+lib_project_nid).val();
        var comment_cid = $('#review_cid_'+lib_project_nid).val();
        if ($('#is_exemplary:checked').val() != null)
            is_exemplary=1;
        else
            is_exemplary=0;
        var get_url = "?q=ntlp/library/project/reviewing/"+lib_project_nid+"/"+comment_cid+"/"+is_exemplary+"/"+comment_text;
    }

    if(comment_text=="") {
        alert('Comment cannot be empty');
        return false;
    }
    //    alert(get_url); return false;
    $.ajax({
        type: "GET",
        url: get_url,
        cache: false,
        success: function(data){
            if(type == 'posting'){
                $('#comment_item').show();
                $('#comment_box').hide();
                $("#node-posting").html(data);
            }

            if(type == 'reviewing'){
                $('#review_item').show();
                $('#review_box').hide();
                $("#node-reviewing").html(data);
            }

            if(type == 'comments'){
                $("#node-comments").html(data);
                $('#txt_comment_'+lib_project_nid).text('');
            }
        //            alert('Comment Posted');
        }

    });
}

function set_archived_projects(id){

    var state = document.getElementById('chk_'+id).checked;
    var publish_project = document.getElementById("request_archive_projects");

    x = publish_project.value.split(',');

    if(state == true){
        publish_project.value += id+',';
    }else{
        for(i = 0; i<x.length ; i++){
            if(x[i] == id){
                publish_project.value = publish_project.value.replace(id+',','');
            }
        }
    }

}

function set_published_projects(id){

    var state = document.getElementById('chk_'+id).checked;
    var publish_project = document.getElementById("publish_project_hidden");

    x = publish_project.value.split(',');

    if(state == true){
        document.getElementById("publish_project_hidden").value += id+',';
    }else{
        for(i = 0; i<x.length ; i++){
            if(x[i] == id){
                publish_project.value = publish_project.value.replace(id+',','');
            }
        }
    }
    
}



function fixProjectLibrarySearchResultPager() {
    $("#ntlpLibrarySearchWrapper .pager li a").click(function(){
        $.get($(this).attr("href"), function(response){
            $("#ntlpLibrarySearchWrapper").html(response);
            $('html,body').animate({
                "scrollTop":0
            },1000);
        });

        return false;
    });
}

function get_library_project_content(str){

    project_nid = $('#project_id').val();
    if(str != ''){
        url = '?q=ntlp/library/project/content/'+str+'/'+project_nid;
        
        $.ajax({
            type: "GET",
            url: url,
            cache: false,
            success: function(data){
                $("#project_content").html(data);

                if(str == 'project_detail'){
                    $("#project_detail").attr("class", "GradesTabActive");
                    $("#project_commentary").attr("class", "");
                }else{
                    $("#project_commentary").attr("class", "GradesTabActive");
                    $("#project_detail").attr("class", "");
                }

            //$("#comment_item"+lib_project_nid).html(data);
            //            alert('Comment Deleted Successfully');
            }
        });
    }
}

function set_library_project_comment(str, project_nid){

    //    txt_comment_2970

    if(str != ''){
        url = '?q=ntlp/library/project/content/'+str+'/'+project_nid;

        $.ajax({
            type: "GET",
            url: url,
            cache: false,
            success: function(data){
                $("#project_content").html(data);

                if(str == 'project_detail'){
                    $("#project_detail").attr("class", "GradesTabActive");
                    $("#project_commentary").attr("class", "");
                }else{
                    $("#project_commentary").attr("class", "GradesTabActive");
                    $("#project_detail").attr("class", "");

                    $('#txt_comment_'+project_nid).focus();
                }

            //$("#comment_item"+lib_project_nid).html(data);
            //            alert('Comment Deleted Successfully');
            }
        });
    }
}

function delete_library_resource(type, lib_project_nid, resource_nid){
 
            
    $.ajax({
        type: "GET",
        url: "?q=library/libprojectresource/delete/data/"+type+"/"+lib_project_nid+"/"+resource_nid,
        cache: false,
        success: function(data){
            try {
                parent.Drupal.modalFrame.close();
            } catch (e) {

            }
            parent.window.location = '?q=ntlp/library/project/view/'+lib_project_nid;
        }
    });
}