/*// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.
*/
/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

var ajax_mutex_project = false;
var tipsy_timer;                    //Timer object, used by tipsy tooltips
var tipsy_timer_interval = 5000;

$(document).ready(function() {

    if ($('#course_project_groups').length) {
        Drupal.behaviors.changeHeaderLinks($('#course_project_groups'), 'ntlp/project/groups/popup/', 'ntlp/courses/project/groups/table/');
        Drupal.behaviors.fixMyPager($('#course_project_groups'));
    }

    if ($('#project_stats_table').length) {
        Drupal.behaviors.changeHeaderLinks($('#project_stats_table'), 'ntlp/courses/project/', 'ntlp/mycourses/myproject/stats/');
        Drupal.behaviors.fixMyPager($('#project_stats_table'));
    }

    if ($('#project_resource_table').length) {
        Drupal.behaviors.changeHeaderLinks($('#project_resource_table'), 'ntlp/project/copy/resource/', 'ntlp/copy/project/resource/');

        // fixMyPager functionality without animate
        var context = $('#project_resource_table');
        $('.pager li a, thead th a:not(.fix_protected)', context).live('click', function(){
            var href = $(this).attr('href');

            $.get(href, function(response){
                $(context).html(response);
            });
            return false;
        });
    }

    if ($('#project_activities_table').length) {
        Drupal.behaviors.changeHeaderLinks($('#project_activities_table'), 'ntlp/project/copy/activity/', 'ntlp/copy/project/activity/');

        // fixMyPager functionality without animate
        var context2 = $('#project_activities_table');
        $('.pager li a, thead th a:not(.fix_protected)', context2).live('click', function(){
            var href = $(this).attr('href');

            $.get(href, function(response){
                $(context2).html(response);
            });
            return false;
        });
    }

});





function close_filter() {
    $('.filter_hide').hide('slow');
}

function showPopup(){

    $('.bluePopup').show(800);
}
function hidePopup() {
    $('.bluePopup').hide(800);
}

function projectDetails(){

    $('#project_detail_edit').show();

    $('#project_detail').hide();
}


function changebuttonname(){
    alert('changebuttonname');
    $('#edit-project-details-proj-edit-button').innerHTML="New Button Text";
}

function resources_search(course_id) {

    var name =  $('#searching_text').val();

    $.ajax({
        type: "GET",
        url: '?q=ntlp/project/existing/resource/search/'+encodeURIComponent(name)+'/'+course_id,
        cache: false,
        success: function(data){
            $("#search_resources").html(data);
        //            parent.Drupal.modalFrame.close().closeDialog();
        }
    });

}


function slide_up(slide_order, tid, project_id) {

    var course_nid = $('#project_course_nid').val();
    
    var get_url ="?q=project/slider/slide_up/"+course_nid+"/"+ slide_order + "/" + tid + "/"+ project_id;

    $.ajax({
        type: "GET",
        url: get_url,
        cache: false,
        success:  function(data){
            $("#project_section_data").html(data);

        }

    });

}


function slide_down(val,tid,project_id) {

    var course_nid = $('#project_course_nid').val();

    var get_url ="?q=project/slider/slide_down/"+course_nid+"/"+ val + "/" + tid + "/"+ project_id;
    $.ajax({
        type: "GET",
        url: get_url,
        cache: false,
        success:  function(data){
            $("#project_section_data").html(data);
        
        }

    });

}


function save_to_library(course_nid, project_id ) {

    //alert($('#keywords').val());
    //    var keywords = $('#keywords').val();
    var project_name =  $('#txt_project_name').val();

    parent.save_project_to_library(project_name, project_id, course_nid);
}


function save_project_to_library(project_name, project_id, course_nid) {
    //+"/"+keywords


    $.ajax({
        type: "GET",
        url: "?q=ntlp/project/SaveToLib/"+course_nid+"/"+project_id+"/"+encodeURIComponent(project_name),
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
        parent.Drupal.modalFrame.close().closeDialog();
    } catch (e) {

    }
}

function cancel_to_library(){
    try {
        parent.Drupal.modalFrame.close().closeDialog();
    } catch (e) {

    }
}

function copy_project(src_course_nid, project_id ) {
    var flag = false;
    var project_name =  $('#txt_project_name').val();
    var copy_to_course_nid = $('#selected_course_nid').val();
    var selected_term_tid = $('#selected_term_tid').val();
    var message = '';
    initDlgNtk();
    if(copy_to_course_nid > 0){
            
        if(selected_term_tid > 0){
            parent.save_project(project_name, project_id, src_course_nid, copy_to_course_nid, selected_term_tid);
        }else{
            flag = true;
            message = 'Please select the term of the target course.';
        }
        
    }else{
        flag = true;
        message = 'Please select the target course.';
            
    }
    
    if(flag){
        $("#dialog-show-confirmation").dialog('option', 'buttons',{
            'Close': function() {
                $(this).dialog('close');
            }
        });
            
        $("#dialog-show-confirmation").find('.msg').css('color', '#000000').text(message);
        $("#dialog-show-confirmation").dialog("open");
    }
}


function save_project(project_name, project_id, src_course_nid, copy_to_course_nid, selected_term_tid) {

    $.ajax({
        type: "GET",
        url: "?q=ntlp/project/save/"+src_course_nid+"/"+project_id+"/"+encodeURIComponent(project_name)+"/"+copy_to_course_nid+"/"+selected_term_tid,
        success:  function(){
            window.location.reload();

        }
    });

    try {
        Drupal.modalFrame.close().closeDialog();
    } catch (e) {

    }
}

function delete_project_cancel(){
    try {
        parent.Drupal.modalFrame.close().closeDialog();
    } catch (e) {

    }
}
function delete_project(course_nid, project_nid, condition) {


    parent.delete_project_process(course_nid, project_nid, condition);
}


function delete_project_process(course_nid, project_nid, condition) {

    $.ajax({
        type: "GET",
        url: "?q=ntlp/project/remove/"+course_nid+"/"+project_nid+"/"+condition,
        success:  function(){
            parent.window.location = '?q=ntlp/courses/project/'+course_nid;
        //            alert('project deleted');
        }

    });

    try {
        Drupal.modalFrame.close();
    //        update_project
    } catch (e) {

    }
//    window.location = '?q=ntlp/goback';
}

function redirect_user_to(course_nid, project_nid){
    try {
        parent.Drupal.modalFrame.close();
    //        update_project
    } catch (e) {

    }
    parent.window.location = '?q=ntlp/courses/project/setting/'+course_nid+'/'+project_nid;
}




function on_imagechange(fileId, fileName) {


    var project_id = $('#project_id').val();
    //    alert('inside project '+fileId + ' '+ fileName +' '+project_id);

    $.ajax({
        type: "GET",
        url: "?q=ntlp/save/project/image/"+fileId+"/"+project_id,
        cache: false,
        success:  function(data){
            $("#project_image").html(data);
            Drupal.behaviors.modalFrameSetup();
        }

    });

    Drupal.modalFrame.close();
}

function attach_uploaded_file(data,fileId){

    var root_path = Drupal.settings.basePath;
    var element;

    if(document.getElementById("attached_files_hidden")){
        element = document.getElementById("attached_files_hidden");
    }

    element.value += 'F,'+'-'+fileId+','+data+' ;';
    document.getElementById("attach_links").innerHTML += '<div id="F_'+'-'+fileId+'"> <a style="vertical-align:middle;" '
    +' onclick="delete_attach(\'F_'+'-'+(fileId)+'\');" ><img width="14" height="14" src="'+root_path+'themes/Boldr/Images/DeleteBtn.png" '
    +' border="2" /></a>&nbsp;&nbsp;'+data+'</div>';

    $('#file-info-resource-file').hide();

}

function delete_hidden(file_id){

    hData = document.getElementById("attached_files_hidden");
    hData.value = "";
//    alert(hData.value);
}

function delete_attach(id){
    //Remove (Mark X) in attached files list in the hidden variable
    delete_hidden(id);

    //Remove UI element for the selected file.
    //    var d = document.getElementById('attach_links');
    var olddiv = document.getElementById(id);
    olddiv.parentNode.removeChild(olddiv);
    //    var count = d.getElementsByTagName('div').length;
    //    if(count == 0 ){
    $('#edit-project-setting-upload-control-file-info-resource-file-wrapper').show();
    $('a[id=attach_link]').show();
    $('#file-info-resource-file').show();
//    }


}

function on_fileupload_success(fileId, fileName, filePath) {

    attach_uploaded_file(fileName, fileId);
    $('#edit-project-setting-upload-control-file-info-resource-file-wrapper').hide();
    $('a[id=attach_link]').hide();
}

$(document).ready(function() {
    setupMenuButton('manageProject', "btnManageproject");
});

function onchange_resource_order(action, resource_id, course_nid, section_tid, resource_type){
    $.ajax({
        type: "GET",
        url: "?q=change/resource/order/"+action+"/"+resource_id+"/"+course_nid+"/"+section_tid+"/"+resource_type,
        cache: false,
        success: function(data){
            $("#project_section_data").html(data);
        }
    });
}

function onchange_resource_availability(resource_id, section_tid, restype){
    if (ajax_mutex_project) {
        alert('An earlier request is still processing');
        return;
    }
    ajax_mutex_project = true;

    var selector_name = 'input[id="resource_'+resource_id+'"]';
    var action = $(selector_name).attr('checked');

    var act = "";
    if(action){ 
        act = 'S';
    }else{
        act = 'H';    
    }

    $.ajax({
        type: "GET",
        url: "?q=ntlp/projectsection/"+restype+"/showhide/"+act+"/"+resource_id+"/"+section_tid,
        cache: false,
        success: function(data){
        //            $("#resource_views_"+section_tid).html(data);
        }
    });
    ajax_mutex_project = false;
}

function onclick_resource_remove(course_nid, project_nid, resource_id, section_tid, restype) {
    initDlgNtk();

    var note = '';
    var title = '';
    if (restype == 'resource') {
        note = 'Are you sure you want to permanently delete this resource?';
        title = "Confirm Deletion";
    }
    else if (restype == 'activity') {
        note = 'This activity will no longer be associated with this project, but it will still exist as a General Purpose activity on the Activities page.\n\
                    <br/><br/>Are you sure you want to remove this activity from the project?';
        title = "Confirm Removal";
    }

    $("#dialog-show-confirmation").dialog('option', 'buttons',{
        'Yes': function() {

            $(this).dialog('close');
            $.ajax({
                type: "GET",
                url: '?q=ntlp/projectsection/'+restype+'/delete/'+course_nid+'/'+project_nid+'/'+resource_id+'/'+section_tid,
                cache: false,
                success: function(data){
                    $("#project_section_data").html(data);
                }
            });
        },
        'No': function() {
            $(this).dialog('close');
        }
    });

    $("#ui-dialog-title-dialog-show-confirmation").html(title);
    $("#dialog-show-confirmation").find('.msg').css('color', '#000000').html(note);
    $("#dialog-show-confirmation").dialog("open");
}


function deleteSection( section_tid ) {

    $.ajax({
        type: "GET",
        url: '?q=ntlp/project/section/delete/'+section_tid,
        cache: false,
        success: function(){
            parent.window.location.reload();
        }
    });

    try {
        parent.Drupal.modalFrame.close().closeDialog();
    } catch (e) {

    }
}


function add_existing_resource(){


    x = document.getElementsByName('rid[]');


    project_nid = document.getElementById('project_id').value;
    section_tid = document.getElementById('section_tid').value;

    for(i = 0; i< x.length ; i++){

        if(x[i].checked == true){
            resource_nid = x[i].value;
        }
    }

    $.ajax({
        type: "GET",
        url: '?q=ntlp/existing/resource/data/'+resource_nid+'/'+section_tid,
        cache: false,
        success: function(){
            parent.window.location.reload();
        }
    });

    try {
        parent.Drupal.modalFrame.close().closeDialog();
        parent.window.location.reload();
    } catch (e) {

    }
}

function initDlgNtk(){

    $("#dialog-confirm-ntk").dialog({
        autoOpen: false,
        resizable: false,
        height:140,
        modal: true
    });

    $("#dialog-show-confirmation").dialog({
        autoOpen: false,
        resizable: false,
        height:100,
        modal: true
    });
}

$(function(){
    initDlgNtk();
});

function onclick_ntk_remove(ntk_id, project_id, page) {

    $.ajax({
        type: "GET",
        url: "?q=ntlp/project/ntk/delete/confirm/"+ntk_id,
        success: function(data){
            $(this).dialog('close');
            if(page == 'popup'){
                try {
                    Drupal.modalFrame.close().closeDialog();
                } catch (e) {

                }
            }
            window.location.reload();
        }
    });
}


//--------------------------------
// Changes the behavior of Accordion JQuery UI to act like a Panel
//--------------------------------
$(document).ready(function() {


    $("#dialog-show-confirmation").dialog({
        autoOpen: false,
        resizable: false,
        height:100,
        modal: true
    });

    var expand_all = $('.notaccordion');
    if (expand_all) {
        //        $("#notaccordion").addClass("ui-accordion ui-accordion-icons ui-widget ui-helper-reset")
        //        .find(".notaccordion")
        //        .addClass("ui-accordion-header ui-helper-reset ui-state-default ui-corner-top ui-corner-bottom")
        $(".notaccordion .ui-icon")
        .hover(function() {
            $(this).toggleClass("ui-state-hover");
        }, function() {
            $(this).toggleClass("ui-state-hover");
        })
        .click(function() {
            $(this)
            .toggleClass("ui-icon-triangle-1-s").toggleClass("ui-icon-triangle-1-e")
            .parent().parent().parent().parent().parent()
            .toggleClass("ui-accordion-header-active ui-state-active ui-state-default ui-corner-bottom")
            .next().toggleClass("ui-accordion-content-active").slideToggle();
            return false;
        });
        $(".notaccordion").next()
        .addClass("ui-accordion-content ui-helper-reset ui-widget-content ui-corner-bottom").show();

        //        alert('expand all exists');
        expand_all_panels(1);
    }
});

function delete_project_section(course_nid, project_nid, section_tid) {

    initDlgNtk();
    
    note = '<ul<li>When you delete this category, all the resources in the category will also be deleted. You will not be able to access them from anywhere else. </li><li>Activities in this category will NOT be deleted, but they will lose their association with this project.</li></ul>Are you sure you want to delete this section and all of its respective data?';

    height = 210;
    var section_resource_count = $('#section_resource_count_'+section_tid).val();

    if(section_resource_count == 0){
        note = 'Are you sure you want to delete this Category?';
        height = 125;
    }
    
    $("#dialog-show-confirmation").dialog('option', 'buttons',{
        'Yes': function() {
            $(this).dialog('close');
            $.ajax({
                type: "GET",
                url: "?q=ntlp/project/section/delete/"+course_nid+"/"+project_nid+"/"+section_tid,
                success: function(data){
                    if (data) {
                        $("#project_section_data").html(data);
                        $(this).dialog('close');
                    } else {
                        $("#dialog-show-confirmation").find('.error-msg').css('color', 'red').text('Unable to delete the section!');
                    }
                },
                error: function() {
                    $(this).find('.error-msg').css('color', 'red').text('Unable to delete the section!');
                }
            });
        },
        'No': function() {
            $(this).dialog('close');
        }
    });

    $( "#dialog-show-confirmation" ).dialog( "option", "height", height );
    
    $("#ui-dialog-title-dialog-show-confirmation").html("Delete Category");
    $("#dialog-show-confirmation").find('.msg').css('color', '#000000').html(note);
    $("#dialog-show-confirmation").dialog("open");
}

function add_project_section_process(project_nid) {
    section_name = document.getElementById('edit-section-add-sectionname').value;
    $.ajax({
        type: "GET",
        url: "?q=ntlp/project/section/add/"+project_nid+"/"+encodeURIComponent(section_name),
        success:  function(data){
            if (data == "OK") {
                parent.Drupal.modalFrame.close();
                parent.window.location.reload();
            } else {
                document.getElementById('edit-section-add-errormsg').innerHTML = 'Unable to add section!';
            }
        },
        error: function() {
            document.getElementById('edit-section-add-errormsg').innerHTML = 'Unable to add section!';
        }
    });
}


function expand_all_panels(to_expand) {
    if(to_expand == 1){
        $(".notaccordion")
        .toggleClass("ui-accordion-header-active ui-state-active ui-state-default ui-corner-bottom")
        .find("> .ui-icon").toggleClass("ui-icon-triangle-1-e ui-icon-triangle-1-s").end()
        .next().toggleClass("ui-accordion-content-active").slideDown();
        $(".notaccordion .ui-icon").attr("class", "ui-icon ui-icon-triangle-1-s");
        $('#expand_all').attr('class', 'GradesTabActive');
        $('#collapse_all').attr('class', '');
    } else {
        $(".notaccordion")
        .toggleClass("ui-accordion-header-active ui-state-active ui-state-default ui-corner-bottom")
        .find("> .ui-icon").toggleClass("ui-icon-triangle-1-s ui-icon-triangle-1-e").end()
        .next().toggleClass("ui-accordion-content-active").slideUp();
        $(".notaccordion .ui-icon").attr("class", "ui-icon ui-icon-triangle-1-e");
        $('#expand_all').attr('class', '');
        $('#collapse_all').attr('class', 'GradesTabActive');
    }
}

function show_addnew_tipsy(tooltip_tid){

    $(".addnew" + tooltip_tid).tipsy({
        className : "grey-middle",
        arrowClass : "grey-arrow",
        delayIn: 0,
        delayOut: 0,
        fade: false,
        fallback: "",
        html: true,
        live: false,
        offset: 0,
        opacity: 1,
        title: function(){
            return $(".section_tooltip_"+ tooltip_tid).html();
        },
        trigger: "manual"
    });
        
    $('.addnew'+tooltip_tid).tipsy('show');
    $('.tipsygreenlinks')
    .click(function() {
        $('.addnew'+tooltip_tid).tipsy("hide");
    })
    .mouseout(function() {
        tipsy_timer = setTimeout('$(".addnew'+tooltip_tid +'").tipsy("hide");' , tipsy_timer_interval);
    })
    .mouseover(function() {
        clearTimeout(tipsy_timer);
    });
    //Call separately, to handle the case when user do not take the mouse over the tip after opening
    tipsy_timer = setTimeout('$(".addnew'+tooltip_tid +'").tipsy("hide");' , tipsy_timer_interval);
    
    return false;
    
}

function show_copy_tipsy(tooltip_tid){
    $(".copyitem"+tooltip_tid).tipsy({
        className : "grey",
        arrowClass : "grey-arrow",
        delayIn: 0,
        delayOut: 3000,
        fade: false,
        fallback: "",
        html: true,
        live: false,
        offset: 0,
        opacity: 1,
        title: function(){
            return $(".resource_item_tooltip_"+tooltip_tid).html();
        },
        trigger: "manual"
    });

    $('.copyitem'+tooltip_tid).tipsy('show');
    $('.tipsygreenlinks').removeClass('modalframe-setup-processed');
    Drupal.behaviors.modalFrameSetup();

    $('.tipsygreenlinks')
    .click(function() {
        $('.copyitem'+tooltip_tid).tipsy("hide");
    })
    .mouseout(function() {
        tipsy_timer = setTimeout('$(".copyitem'+tooltip_tid +'").tipsy("hide");' , tipsy_timer_interval);
    })
    .mouseover(function() {
        clearTimeout(tipsy_timer);
    });
    //Call separately, to handle the case when user do not take the mouse over the tip after opening
    tipsy_timer = setTimeout('$(".copyitem'+tooltip_tid +'").tipsy("hide");' , tipsy_timer_interval);
    return false;
    
}
function update_category_text(section_tid, term_id, newText,old_value){
    if (newText.length>0){
        $.ajax({
            type: "GET",
            url: "?q=ntlp/project/category/rename/"+term_id +"/"+encodeURIComponent(newText) ,
            error: function() {
                $("#toedit"+section_tid).html(real_data);
            }
        });
        $("#toedit"+section_tid).css("display","inline");
        $("#txtedit"+section_tid).css("display","none");
    }
    else{
        /*
        $("#dialog-show-confirmation").dialog('option', 'buttons',{
            'close': function() {
                $(this).dialog('close');  
            }
        });

        $("#dialog-show-confirmation").find('.msg').css('color', '#000000').text("Please Enter Category Title");
        $("#dialog-show-confirmation").dialog("open");
        */
        $("#txtedit"+section_tid).text(old_value);
        $("#toedit"+section_tid).text(old_value);

        $("#toedit"+section_tid).css("display","inline");
        $("#txtedit"+section_tid).css("display","none");

    }
}
function editinplace(section_tid,old_value){

    var real_data = $("#toedit"+section_tid).html();
    var term_id = $('#hidden'+section_tid).val();
   
    if (old_value == undefined ) {
        old_value=$("#toedit"+section_tid).val();
    }

    $("#toedit"+section_tid).tipsy("hide");
    $("#toedit"+section_tid).css("display","none");

    $("#txtedit"+section_tid).css("display","block");
    $("#newText"+section_tid).val(real_data);
    $("#newText"+section_tid).focus();
	
    $("#newText"+section_tid).bind(
        "blur" , function(){
            var newText = trim($("#newText"+section_tid).val());

            if (newText =='')
            {
                return false;
            }

            $("#toedit"+section_tid).html(newText);
            update_category_text(section_tid, term_id, newText,old_value);
		
        }
        );

    $("#eip"+section_tid).bind(
        "change" , function(){
            var newText = trim($("#newText"+section_tid).val());
            if (newText =='')
            {
                return false;
            }
            $("#toedit"+section_tid).html(newText);
            update_category_text(section_tid, term_id, newText);
        }
        );

    $("#eip"+section_tid).bind(
        "keypress" , function(e){
            if(e.which == '13'){
                var newText = trim($("#newText"+section_tid).val());
                
                if (newText=='')
                {
                    return false;
                }

                $("#toedit"+section_tid).html(newText);
                update_category_text(section_tid, term_id, newText);
            }
        }

        );

  
}

function trim(value) {
    value = value.replace(/^\s+/,'');
    value = value.replace(/\s+$/,'');
    return value;
}

function save_new_category(){

    var category_name = $('#txt_category_title').val();
    var other_category_id = $('#other_category_id').val();
    var project_nid = $('#hidden_project_nid').val();
    var visibility = 'H';


    if ($('#chk_category_visibility:checked').val() !== undefined) {
        visibility = 'S';
    }

    if(category_name =="" ){

        $("#dialog-show-confirmation").dialog('option', 'buttons',{
            'Close': function() {
                $(this).dialog('close');
            
            }
        });

        $("#dialog-show-confirmation").find('.msg').css('color', '#000000').text('Category title field cannot be empty.');
        $("#dialog-show-confirmation").dialog("open");
    }else{

        $.ajax({
            type: "GET",
            url: "?q=ntlp/project_cat/add/"+project_nid +"/"+encodeURIComponent(category_name)+"/"+other_category_id+"/"+visibility ,
            success:  function(data){
              
                parent.Drupal.modalFrame.close();
                parent.window.location.reload();
                
            }

        });
    }
    
}


function add_ntk_post(){
    var $flag = true;
    var project_nid = $('#project_nid').val();
    var post_message = $('#post_message').val();
    initDlgNtk();
    if(post_message =="" ){
        $flag = false;
        $("#dialog-show-confirmation").dialog('option', 'buttons',{
            'Close': function() {
                $(this).dialog('close');

            }
        });
        $('#dialog-show-confirmation').dialog('option','height',100);
        $('#dialog-show-confirmation').dialog('option','minHeight',100);

        $("#ui-dialog-title-dialog-show-confirmation").html('Error');
        $("#dialog-show-confirmation").find('.msg').css('color', '#000000').text('Post Message field cannot be empty.');
        $("#dialog-show-confirmation").dialog("open");

        
    }else{
        if(post_message.length > 140){
            $flag = false;
            $("#dialog-show-confirmation").dialog('option', 'buttons',{
                'Close': function() {
                    $(this).dialog('close');

                }
            });
            $('#dialog-show-confirmation').dialog('option','height',100);
            $('#dialog-show-confirmation').dialog('option','minHeight',100);

            $("#ui-dialog-title-dialog-show-confirmation").html('Error');
            $("#dialog-show-confirmation").find('.msg').css('color', '#000000').text('Only 140 Characters are allowed for the post.');
            $("#dialog-show-confirmation").dialog("open");
        }
    }

    if($flag){

        $.ajax({
            type: "GET",
            url: "?q=ntlp/add/needtoknowpost/"+project_nid +"/"+encodeURIComponent(post_message),
            success:  function(data){

                parent.Drupal.modalFrame.close();
                parent.window.location.reload();

            }

        });
    }
   
    
}

function set_section_status(course_nid, project_nid, section_tid){
    initDlgNtk();
    var section_status = '';
    var message = '';
    var section_only = false;
    
    if ($('#chk_'+section_tid+':checked').val() !== undefined) {
        section_status = 'S';
        message = 'This category will now be visible to all users. <br> <br> Would you also like to make all the existing items in this category visible?';
    }else{
        section_status = 'H';
        
        message = 'This category and all the items in it will now be hidden from all students. <br> <br> Are you sure you want to do this?';
    }
    
    $("#dialog-show-confirmation").dialog('option', 'buttons',{
        
        'Yes': function() {

            $(this).dialog('close');
            $.ajax({
                type: "GET",
                url: "?q=ntlp/project/section/showhide/"+section_tid +"/"+section_status+"/"+course_nid+"/"+project_nid+"/"+section_only,
                success:  function(data){
                    $("#project_section_data").html(data);
                }
            });
        },
        'No': function() {

            if(section_status == 'S'){
                section_only = true;

                $.ajax({
                    type: "GET",
                    url: "?q=ntlp/project/section/showhide/"+section_tid +"/"+section_status+"/"+course_nid+"/"+project_nid+"/"+section_only,
                    success:  function(data){
                        $("#project_section_data").html(data);
                    }
                });
            }
           
            
            if(section_status == 'H'){
                $('#chk_'+section_tid).attr('checked', 'checked');
            }else{
                $('#chk_'+section_tid).removeAttr('checked');
            }
            $(this).dialog('close');
        }
    });

    $("#dialog-show-confirmation").find('.msg').css('color', '#000000').html(message);
    $("#ui-dialog-title-dialog-show-confirmation").html("Category Status");
    $("#dialog-show-confirmation").dialog("open");
  
}

function copy_item_into_section(section_tid, mode){
    var resource_ids = '';
    var project_nid = $('#project_nid').val();
    var x=document.getElementsByName("resource[]");
    

    for(i=0; i < x.length; i++){
        if(x[i].checked){
            
            resource_ids += ','+x[i].value;
        }
    }

    if(resource_ids != ''){
        $.ajax({
            type: "GET",
            url: "?q=ntlp/project/copydata/"+resource_ids +"/"+project_nid+"/"+section_tid+"/"+mode,
            success:  function(data){
                parent.Drupal.modalFrame.close();
                parent.window.location.reload();
            }

        });
    }

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

function category_hover_tooltip() {
    $(".cat_hover").tipsy({
        opacity: 1,
        className : "black-toedit" ,
        arrowClass : "black-arrow-toedit",
        gravity: "sw"
    });
}

function copy_from_template_project(src_project_nid, target_course_nid, target_project_nid){
    
    $('#loading_'+src_project_nid).show();
    $.ajax({
        type: "GET",
        url: "?q=ntlp/library/project/template/"+src_project_nid+"/"+target_course_nid+"/"+target_project_nid,
        success:  function(data){
            $('#loading_'+src_project_nid).hide();
            window.location.reload();
        }

    });
}


function on_multiselect_primaryteachers(su_ids, su_names, cache_id){

    var tid = document.getElementById('submitted_by_uid');
    var tname = document.getElementById('submitted_by_view');

    tid.value = su_ids;
    tname.innerHTML = su_names.replace(/,/g, "<br />");
}