// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

var prev_outcome_scores = new Array();
var submission_grade_tid_InProgress = false;
var gradeall_timer = null;

function save_previous_score(user_id, outcome_tid) {

    prev_outcome_scores[user_id] = new Array();
    prev_outcome_scores[user_id][outcome_tid] = $('input[id="' + user_id + '-' + outcome_tid + '"]').val();
}

function submission_grade_tid(activity_nid, activity_user, max_points, outcome_tid, myinput) {
    var flag = true;
    var div_name = activity_user+'-'+outcome_tid;
    var user_input =  $('#'+div_name).val();
    //        alert('userid '+activity_user+' activity_nid '+activity_nid);
    user_input = user_input.trim();

    if (prev_outcome_scores[activity_user][outcome_tid] == user_input)
        return;

    if(user_input==""){
        //        flag = false;
        user_input="-";
    } else if (user_input.toUpperCase()!="EX") {
        if(isNaN(user_input)) {
            alert('Grade must be a number');
            $('#'+div_name).val(myinput);
            flag = false;
            return false;
        }else{
            if(user_input >= 1000){
                note = 'The maximum point value is 999.99. Please re-enter this score. ';

                note = 'The maximum point value is 999.99. Please re-enter this score. ';

                $("#dialog-show-confirmation").dialog('option', 'buttons',{
                    'Close': function() {
                        $(this).dialog('close');
                    }
                });
                $("#dialog-show-confirmation").find('.msg').css('color', '#000000').text(note);
                $("#dialog-show-confirmation").dialog("open");

                $('#'+div_name).focus();
                $('#'+div_name).val('');
                flag = false;
            }else{
                var index = user_input.indexOf('.');
                if(index > 0){
                    temp_var = user_input.substr(index+1, user_input.length);
                    if(temp_var.length > 2){

                        note = 'Only two decimal places are allowed. Please re-enter.';
                        $("#dialog-show-confirmation").dialog('option', 'buttons',{
                            'Close': function() {
                                $(this).dialog('close');
                            }
                        });
                        $("#dialog-show-confirmation").find('.msg').css('color', '#000000').text(note);
                        $("#dialog-show-confirmation").dialog("open");
                        $('#'+div_name).focus();
                        $('#'+div_name).val('');
                        flag = false;
                    }else{
                        flag = true;
                    }
                }else{
                    flag = true;
                }
            }           
        }
    }
    if(user_input < 0) {
        alert('Grade cannot be in minus');
        $('#'+div_name).val(myinput);
        flag = false;
        return false;
    }
    //    if(user_input>max_points) {
    //        alert('Grade cannot be greater than max points: '+max_points);
    //        $('#'+div_name).val(myinput);
    //        flag = false;
    //        return false;
    //    }

    if(flag){
        init_autosave(activity_nid);

        //Don't allow auto-save if scoring is in progress
        submission_grade_tid_InProgress = true;

        var get_url = "?q=ntlp/activity/gradeall/ajax/"+activity_nid+"/"+activity_user+"/"+outcome_tid+"/"+user_input;

        //$('#anim_'+div_name).show();
        $.ajax({
            type: "GET",
            url: get_url,
            cache: false,
            success: function(data){
                if (data != "OK") {
                    //Show error
                    $('div[name=last_saved_date]').text(data);
                    $('div[name=last_saved_date]').attr("style", "padding: 5px; display: block; background-color: #ddd; border: 1px solid #ddd; float: left; font-size: 12px; color: rgb(0, 0, 0);");
                }
                $('#anim_'+div_name).hide();
                submission_grade_tid_InProgress = false;
            },
            error: function() {
                //                alert(data);
                //                alert('Error saving this score, please re-enter');
                $('#anim_'+div_name).hide();
                submission_grade_tid_InProgress = false;
            }
        });
    }
}

function submission_late_tid(activity_user, activity_nid) {
    //Restart timer
    init_autosave(activity_nid);

    //Get checkbox value
    var checked = $('input[name=outcome_late['+activity_user+']]').is(':checked');

    //Set/Unset late class in textboxes
    if(checked == true){
        $('td[name=user['+activity_user+']]').each(function(){
            $(this).addClass("late");
        });
    }else{
        $('td[name=user['+activity_user+']]').each(function(){
            $(this).removeClass("late");
        });
    }
    var checked_val = (checked)? 1 : 0;
    var get_url = "?q=ntlp/activity/gradeall/ajax_late/"+activity_nid+"/"+activity_user+"/"+checked_val;
    //Don't allow auto-save if scoring is in progress
    submission_grade_tid_InProgress = true;

    $.ajax({
        type: "GET",
        url: get_url,
        cache: false,
        success: function(data){
            if (data != "OK") {
                $('div[name=last_saved_date]').text(data);
                $('div[name=last_saved_date]').attr("style", "padding: 5px; display: block; background-color: #ddd; border: 1px solid #ddd; float: left; font-size: 12px; color: rgb(0, 0, 0);");
            }
            submission_grade_tid_InProgress = false;
        },
        error: function() {
            //                alert(data);
            //                alert('Error saving this score, please re-enter');
            submission_grade_tid_InProgress = false;
        }
    });
}

String.prototype.trim = function() {
    return this.replace(/^\s+|\s+$/g,"");
}


function call_popup(a){
    alert($(a).val());
}



function google_docs_search() {

    var name =  $('#finder').val();

    $.ajax({
        type: "GET",
        url: "?q=ntlp/google/docs/search",
        data: "type=checkbox",
        success:  function(data){
            $("#googledocs").html(data);

        }

    });
}

/*delete a attachment from page..*/

function detele_attach(id){

    //Remove (Mark X) in attached files list in the hidden variable
    _delete_hidden(id);
 
    //Remove UI element for the selected file.
    var d = document.getElementById('urllist');
    var olddiv = document.getElementById(id);
    d.removeChild(olddiv);
//    alert("Attached file removed.");
}

function _delete_hidden(filename){

    if(document.getElementById("edit-mysubmission-hAttachedFiles") != null){
        element = document.getElementById("edit-mysubmission-hAttachedFiles");

    }else if(document.getElementById("edit-main-info-hAttachedFiles") != null){
        element = document.getElementById("edit-main-info-hAttachedFiles");

    }else if(document.getElementById("attachted_files_hidden")){
        element = document.getElementById("attachted_files_hidden");
    }

    //    hData = document.getElementById('edit-mysubmission-attachment-url-attachment-hAttachedFiles');
    hData = element;
    var files = hData.value.split(";");
    var obj = new Array();

    for(i = 0 ; i< files.length; i++){
        var attfile = files[i].split(",");
        var recid = (filename.split('_'))[1];
        var rectype = filename.substr(0, 1);

        if(recid == attfile[1]){
            //alert('found:'+rectype+','+recid+":"+attfile[0]+','+attfile[1]);
            hData.value = hData.value.replace(rectype+','+recid, 'X,'+attfile[1]);
            break;
        }
    }
    //    alert(hData.value);
    return obj;
}




function attach_google_docs(){
    var root_path = Drupal.settings.basePath;

    if(parent.document.getElementById("edit-mysubmission-hAttachedFiles") != null){
        element = parent.document.getElementById("edit-mysubmission-hAttachedFiles");

    }else if(parent.document.getElementById("edit-main-info-hAttachedFiles") != null){
        element = parent.document.getElementById("edit-main-info-hAttachedFiles");
    
    }else if(parent.document.getElementById("attachted_files_hidden") != null){
        element = parent.document.getElementById("attachted_files_hidden");
    }


    var x=document.getElementsByName("gdocs[]");
    for(i=0; i< x.length; i++){
        var d = parent.document.getElementById('urllist');
        var count = d.getElementsByTagName('div').length;

        if(x[i].checked){
            var docLink = x[i].value;
            var docName = x[i].id;
            element.value += 'G,'+'-'+(count+1)+','+docName+','+docLink+';';

            d.innerHTML += '<div id="G_'+'-'+(count+1)+'">'+
            '<img style="position:relative;top:2px;margin-right:4px;" onclick="detele_attach(\'G_'+'-'+(count+1)+'\');" width="14" height="14" src="'+root_path+'themes/Boldr/Images/DeleteBtn.png" border="0" />'+
            '<a href="'+docLink+'" target="_blank">'+docName+'</a></div>';
        }
    }
}

function attach_url(){
    
    var root_path = Drupal.settings.basePath;
    var d = document.getElementById('urllist');
    var count = d.getElementsByTagName('div').length;
    var element;
    
    if(document.getElementById("edit-mysubmission-hAttachedFiles") != null){
        element = document.getElementById("edit-mysubmission-hAttachedFiles");

    }else if(document.getElementById("edit-main-info-hAttachedFiles") != null){
        element = document.getElementById("edit-main-info-hAttachedFiles");

    }
  

    //    var data = $('#doc_link').val();
    if( $('input[name="main[info][urls]"]').length > 0 )
        var data = $('input[name="main[info][urls]"]')[0].value;
    else
        var data = $('input[name="mysubmission[urls]"]')[0].value;
    
    //    document.getElementById("edit-mysubmission-attachment-url-attachment-hAttachedFiles").value += 'L,'+'-'+(count+1)+','+data+' ;';
    element.value += 'L,'+'-'+(count+1)+','+data+' ;';
    document.getElementById("urllist").innerHTML += '<div id="L_'+'-'+(count+1)+'"> <a '
    +'href="#" onclick="detele_attach(\'L_'+'-'+(count+1)+'\');" ><img src="'+root_path+'themes/Boldr/Images/common/delete.PNG" '
    +' border="2" /></a>'+data+'</div>';

    //    document.getElementById("urllist").innerHTML += '<div>'+data+' <a href="#" ><img src="/drupal/sites/all/modules/ntlp_course/theme/delete.PNG" border="2" /></a></div>';
    //    document.getElementById("edit-urldata").value += '[L<>'+data+' ],';
    try {
        Drupal.modalFrame.close().closeDialog();

    } catch (e) {

    }
}

function attach_file(fileId, fileName, filePath){

    var root_path = Drupal.settings.basePath;
    var d = document.getElementById('urllist');
    
    var count = d.getElementsByTagName('div').length;
    var element;
 
    if(document.getElementById("edit-mysubmission-hAttachedFiles") != null){
        element = document.getElementById("edit-mysubmission-hAttachedFiles");

    }else if(document.getElementById("edit-main-info-hAttachedFiles") != null){
        element = document.getElementById("edit-main-info-hAttachedFiles");


    }else if(document.getElementById("attachted_files_hidden")){
        element = document.getElementById("attachted_files_hidden");
    }
    //    var data = $('#doc_link').val();
    //    document.getElementById("edit-mysubmission-attachment-url-attachment-hAttachedFiles").value += 'L,'+'-'+(count+1)+','+data+' ;';
    element.value += 'F,'+'-'+fileId+','+fileName+' ;';
    
    document.getElementById("urllist").innerHTML += '<div id="F_'+'-'+fileId+'">'
    +'<img style="position:relative;top:2px;margin-right:4px;" onclick="detele_attach(\'F_'+'-'+(fileId)+'\');" width="14" height="14" src="'+root_path+'themes/Boldr/Images/DeleteBtn.png" border="0" />'
    +'<a href="'+filePath+'" target="_blank">'+fileName+'</a></div>';
    
//    document.getElementById("urllist").innerHTML += '<div>'+data+' <a href="#" ><img src="/drupal/sites/all/modules/ntlp_course/theme/delete.PNG" border="2" /></a></div>';
//    document.getElementById("edit-urldata").value += '[L<>'+data+' ],';

}





function add_value()
{
    var x=document.getElementsByName("outcome[]");
    var sum=0;
    var totalpoints = 0;

    // This div Hide the Total Points of Graded outcomes By Teacher
    $("#total_points").hide();
 
    for(i=0; i< x.length; i++){
        sum = x[i].value;
        if(sum == 0){

        }else{
            totalpoints = totalpoints + parseInt(sum);
        }
    }
    $('#gradepoints').html(parseInt(totalpoints));

}


function add_values()
{

    var x=document.getElementsByName("out[]");

    var sum=0;
    var totalpoints = 0;

    $("#total_points").hide();

    for(i=0; i< x.length; i++){

        // alert(x[i].value);

        sum = x[i].value;

        if(sum == 0){
            
           
        }else{

            totalpoints = totalpoints + parseInt(sum);
        }
    }
    $('#gradepoints').html(parseInt(totalpoints));

}







/*function GetXmlHttpObject(){

    if (window.XMLHttpRequest)
    {
        // code for IE7+, Firefox, Chrome, Opera, Safari
        return new XMLHttpRequest();
    }
    if (window.ActiveXObject)
    {
        // code for IE6, IE5
        return new ActiveXObject("Microsoft.XMLHTTP");
    }
    return null;
}
 */
/*
function foo(id){

    xmlhttp=GetXmlHttpObject();
    if (xmlhttp==null)
    {
        alert ("Your browser does not support XMLHTTP!");
        return;
    }


    //    alert('values selected id '+id);
    //    document.getElementById('txt_1').value = id;

    var url="availability_calendars/check-id";
    alert(url);


    xmlhttp.open("GET",url,true);
    xmlhttp.send(null);


        xmlhttp.onreadystatechange=function(){
        if(xmlhttp.readyState==0){
            alert('The request is not initialized')
        }else if(xmlhttp.readyState==1){
            alert('The request has been set up')

        }else if(xmlhttp.readyState==2){
            alert('The request has been sent')
        }
        else if(xmlhttp.readyState==3){
            alert('The request is in process')

        }else if(xmlhttp.readyState==4){
            alert('The request is complete')
            alert(xmlhttp.responseText)
             document.getElementById('txt_1').value = xmlhttp.responseText;
        }
    };

//    alert(xmlhttp.responseText);

//    .innerHTML=xmlhttp.responseText;
}
 */

function showcalendar(){

    $('#edit-course-activity-duedate').datepicker();
    $('#edit-course-activity-publishondate').datepicker();
    $('#edit-course-activity-hideondate').datepicker();
        
}

function sections(){
    //alert('here in '+ $('#edit-project').val());
    var projectid = $('#edit-project').val();


    var drupalfrm = function (data){
        //        alert(data);
        var result = Drupal.parseJson(data);
        $('#ntlp_course_w').html(data);
    }

    var formtype = projectid;

    $.get('?q=ntlp_course/load_section', {
        'projectid': formtype
    }, drupalfrm);
    return false;
}

  

function availabilitymode(radioObj){

    var option = $(radioObj).val();


    if(option == 'M'){

        $('#availability').show();
        $('#row_hide').hide();
        $('#row_hides').hide();
        $('#row_hides3').show();


    }else{

        $('#row_hide').show();
        $('#row_hides').show();
        //       $('#availability').hide();
        $('#row_hides3').hide();

    }

}


function project_select(cmbObj){
    var option = cmbObj.options[cmbObj.selectedIndex].text;




    if((option.indexOf('---')>=0) ||  (option.indexOf('None')>=0) || (cmbObj.selectedIndex==0)){
        //alert (cmbObj.options[cmbObj.selectedIndex].text);

        $('#prj_cat').hide();

    }else{

        $('#prj_cat').show();

    }

    if (option.indexOf('----')>=0){
        //	alert (cmbObj.options[cmbObj.selectedIndex].text);

        cmbObj.selectedIndex+=1;
    }
}





//
//
//var a = ('#edit-main-info-coursetype').val();
//
//alert(a);
//
//alert('ssssssssss');
//
//
//
//



function graded_outcomes(optionObj){
    var option = $(optionObj).val();
 
    if(option == 0){

        $(".row_hide1").hide();
        $("#possible").css('display', 'none');
        $("#total").css('display', 'none');
        $("#pos_pnt").css('display', 'none');


    }else{
        $(".row_hide1").show();
        $("#possible").css('display', 'block');
        $("#total").css('display', 'block');
        $("#pos_pnt").css('display', 'block');
    }

}




//
//function availabilitymode(radioObj){
//
//    var option = $(radioObj).val();
//
//    if(option == 'M'){
//
//        $('#availability').css('display', 'block');
//        $('#showhide').css('display', 'none');
//
//    }else{
//
//        $('#showhide').css('display', 'block');
//        $('#availability').css('display', 'none');
//    }
//
//}
//
//function graded_outcomes(optionObj){
//    var option = $(optionObj).val();
//    if(option == 0){
//        $("#points").css('display', 'none');
//        $("#gradingmethod").css('display', 'none');
//    }else{
//        $("#points").css('display', 'block');
//        $("#gradingmethod").css('display', 'block');
//    }
//
//
//
//}




/*if(Drupal.jsEnabled)
{

    $(document).ready(function() {

        // Parent field & respective event
        // Adding change event to Hot Spot Field
        //        alert('hello');
        //        alert($('#edit-course-activity-availabilitymode-M'));
        $('#edit-course-activity-availabilitymode').click(function(event) {
            //            alert($('#edit-course-activity-availabilitymode[value=no]'));
            //            var formid = $('#edit-course-activity-availabilitymode-M').val();
            //            alert('inside fucntion '+formid);
            //alert(date);
            // Call back function for AJAX call
            var frmDrupal = function(data) {
                
                //alert(data);
                // convert the value from Drupal to JSON
                var result = Drupal.parseJson(data);

                // Set the child field(s)' value(s)
                // setting the text for the "test" text field

                // $('#txt_1').text(data);
                //                $('#status').html('selected date '+date);
                $('#element').html(data);
            }

            //AJAX call
            // URL: ActiveField/field-trip     - maps to a Drupal function
            // Parameters: null for now.
            // Call back function:  "frmDrupal"

            //The URL (i.e. the server side resource to be called)
            //needs to be unique to the module

            var formtype = formid;

            $.get('?q=ntlp_course/form_load', {
                'formname': formtype
            }, frmDrupal);
            // preventing entire page from reloading
            return false;
        });


    }); 
    

}*/





function hide_div(){

    $('.hide').slideToggle('slow');
    $('.show').slideToggle('slow');

}


function show_div(){

    $('.hide').slideToggle('slow');
    $('.show').slideToggle('slow');

}


function hide_detail(){

    
    //document.getElementById("hide_more_detail").style.display = "none";


    $('#hide_more_detail').slideToggle(800);


    $('#hide_open').slideToggle('slow');
    $('#hide_close').slideToggle('slow');
    $('.rubric_detail').slideToggle('slow');
   
}


function show_detail(){

    //document.getElementById("hide_more_detail").style.display = "none";
    $('#hide_more_detail').slideToggle(800);
    $('#hide_open').slideToggle('slow');
    $('#hide_close').slideToggle('slow');
    $('.rubric_detail').css('display', '');
}


function hide_rubric_detail(){


    //document.getElementById("hide_more_detail").style.display = "none";

    $('#hide_rubric_more_detail').slideToggle(800);


    $('#hide_rubric_open').slideToggle('slow');
    $('#hide_rubric_close').slideToggle('slow');

}


function show_rubric_detail(){

    //document.getElementById("hide_more_detail").style.display = "none";
    $('#hide_rubric_more_detail').slideToggle(800);
    $('#hide_rubric_open').slideToggle('slow');
    $('#hide_rubric_close').slideToggle('slow');
}

function hide_rubric_view_detail(){


    //document.getElementById("hide_more_detail").style.display = "none";

    $('#hide_rubric_more_detail').slideToggle(800);


    $('#hide_rubric_open').slideToggle('slow');
    $('#hide_rubric_close').slideToggle('slow');

}


function show_rubric_view_detail(){

    //document.getElementById("hide_more_detail").style.display = "none";
    $('#hide_rubric_more_detail').slideToggle(800);
    $('#hide_rubric_open').slideToggle('slow');
    $('#hide_rubric_close').slideToggle('slow');
}


function valueHideData(obj) {
    $('#'+obj.id).val('');
}

function valueShowData(obj, text) {

    if ($('#'+obj.id).val() == "") {
        $('#'+obj.id).val(text);
    }
}


//function update_announcement(course_nid, y, m, d) {
//    var atext =  $('#edit-announcement-edit').val();
//    parent.save_announcement(atext, course_nid, y, m, d);
//}



function update_activity(course_nid) {
    var flag = false;
    var course_nid =  $('#selected_course_nid').val();
    var activity_name =  $('#activity_name').val();
    var activity_nid =  $('#activity_nid').val();    
    var selected_term_tid =  $('#selected_term_tid').val();    
    var note = '';
    
    initDlg();
    
    if(course_nid > 0){
            
        if(selected_term_tid > 0){
            parent.save_activity(course_nid, activity_nid, activity_name, selected_term_tid);
        }else{
            flag = true;
            note = 'Please select the term of the target course.';
        }
        
    }else{
        flag = true;
        note = 'Please select the target course.';
            
    }

    
    if(flag){
        $("#dialog-show-confirmation").dialog('option', 'buttons',{
            'Close': function() {
                $(this).dialog('close');
            }
        });
            
        $("#dialog-show-confirmation").find('.msg').css('color', '#000000').text(note);
        $("#dialog-show-confirmation").dialog("open");
    }
}


function save_activity(course_nid, activity_nid, activity_name, selected_term_tid) {
    $.ajax({
        type: "GET",
        url: "?q=ntlp/activity/save/"+course_nid+"/"+activity_nid+"/"+encodeURIComponent(activity_name)+"/"+selected_term_tid,
        success:  function(){
            window.location.reload();
         
        }
    });

    try {
        Drupal.modalFrame.close().closeDialog();
	
    } catch (e) {

    }
}

function delete_activity_cancel(){
    try {
        parent.Drupal.modalFrame.close().closeDialog();
    } catch (e) {

    }
}

function delete_activity(course_nid) {


    var activity_nid =  $('#edit-delete-activity-activity-nid').val();
    var course_nid =  $('#edit-delete-activity-course-nid').val();


    parent.delete_activity_process(course_nid, activity_nid);
}


function delete_activity_process(course_nid, activity_nid) {

    $.ajax({
        type: "GET",
        url: "?q=ntlp/courses/activity/remove/"+course_nid+"/"+activity_nid,
        success:  function(data) {
            if (data == "ERROR") {
                window.location.reload();
            } else {
                window.location = "?q=ntlp/courses/activity/"+course_nid;
            }
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
        Drupal.modalFrame.close();
    } catch (e) {

    }
//    window.location = '?q=ntlp/goback';
}

//    var a = document.getElementById('file_fid');
$(document).ready(function() {
    setupMenuButton('manageAttachments', "btnAttachments");
    setupMenuButton('manageActivities', "btnManageActivities");
});


function on_imagechange(fileId, fileName) {

    attach_file(fileId, fileName, fileName);

    //    var group_id = $("#group_id").val();
    try {
        Drupal.modalFrame.close().closeDialog();
    } catch (e) {

    }
    $('#google_docs_row').show();
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


function on_fileupload_success(fileId, fileName, filePath) {
    
    var auto_upload =  document.getElementById('attach_link');
    if(auto_upload){
        attach_file(fileId, fileName, filePath);
    }
    var element = document.getElementById('google_docs_row');
    if(element != null && element != 'undefined'){
        $('#google_docs_row').show();
    }
    
}

function on_multiselect_teachers(su_ids, su_names) {

    var activity_nid =  $('#activity_nid').val();
    var course_nid =  $('#course_nid').val();
 
    $.ajax({
        type: "GET",
        url: "?q=ntlp/add/activity/user/"+course_nid+"/"+activity_nid+"/"+su_ids,
        cache: false,
        success: function(data){

            window.location.reload();

        }

    });
}


function get_rubric_users(su_ids, su_names, enroll_in_role_id){
    var activity_nid =  $('#activity_nid').val();
    var feedback_by =  $('#user_id').val();

    $.ajax({
        type: "GET",
        url: "?q=ntlp/add/activity/user/"+activity_nid+"/"+feedback_by+"/"+su_ids,
        cache: false,
        success: function(data){
            window.location.reload();

        }

    });

}

function self_evaluation_set(obj){
    $('input[name="main[detail][grades][auto_calc]"]').attr('disabled', obj.value == 1 ? false : true );
}

function atgoogledoc() {
    if($(parent.document).find('input[name=main\\[info\\]\\[urls\\]]').length > 0){
        $(parent.document).find('input[name=main\\[info\\]\\[urls\\]]')[0].value=$('#doc_link').val();
    }else{
        $(parent.document).find('input[name=mysubmission\\[urls\\]]')[0].value=$('#doc_link').val();
    }

    parent.attach_url();
}



function select_all(obj){

    if(obj){
        $('input[name="students_uid[]"]').each(function(){
            $(this).attr('checked', 'checked');
        });
    }else{
        $('input[name="students_uid[]"]').each(function(){
            $(this).removeAttr("checked");
        });
    }
    return;
}


function onclick_grade_all(activityID) {
 
    $('.dialog-confirm-ntk').dialog( "option" , 'activity_id' , activityID);

    $('.dialog-confirm-ntk').dialog('open');
}


$(function(){
    
    $(".dialog-confirm-ntk").dialog({

        autoOpen: false,
        resizable: false,
        height:120,
        width:400,
        modal: true,
        buttons: {
            Yes: function() {
                var dlg = this;
                var activityID = $(dlg).dialog('option','activity_id');

                $.ajax({
                    type: "GET",
                    url: '?q=ntlp/courses/activity/grade100all/'+activityID,
                    cache: false,
                    success: function(data){
                        window.location.reload();
                    }
                });
            },
            No: function() {
                $(this).dialog('close');
            }
        }
    });

});

function submission_inc_tid(user_id, activity_nid){
    //Restart timer
    init_autosave(activity_nid);

    //Get checkbox value
    var grading_method = $('#activity_grading_method').val();
    
    var a = $('input[name=outcome['+user_id+']]').is(':checked');

    //Set/Unset disabled attribute in textboxes
    if(a == true){
        
        $('input[name=disabled_outcome['+user_id+']]').each(function(){
            $(this).val(0);
            $(this).attr("disabled", "disabled");
        });
        $('td[name=user['+user_id+']]').each(function(){
            $(this).addClass("incomplete");
        });
    }else{

        if(grading_method == 1){
            
            $('input[name=disabled_outcome['+user_id+']]').each(function(){
                $(this).removeAttr("disabled");
            });

            $('input[name=auto_graded_outcome['+user_id+']]').each(function(){

                var index ='';
                var max_points = $(this).val();
                var index = $(this)[0].id;

                $('input[name=disabled_outcome['+user_id+']]').each(function(){

                    var txtid = $(this)[0].id;
                    //                    submission_grade_tid(activity_nid, activity_user, max_points, outcome_tid, myinput)
                    if(index == txtid){
                        $(this).val(max_points);
                    }
                });

            });

        }else{
            
            $('input[name=disabled_outcome['+user_id+']]').each(function(){
                $(this).removeAttr("disabled");
            });
        }
       
        $('td[name=user['+user_id+']]').each(function(){
            $(this).removeClass("incomplete");
        });
    }

    var checked_val = (a)? 1 : 0;
    var get_url = "?q=ntlp/activity/gradeall/ajax_inc/"+activity_nid+"/"+user_id+"/"+checked_val;
    //Don't allow auto-save if scoring is in progress
    submission_grade_tid_InProgress = true;

    $.ajax({
        type: "GET",
        url: get_url,
        cache: false,
        success: function(data){
            if (data != "OK") {
                $('div[name=last_saved_date]').text(data);
                $('div[name=last_saved_date]').attr("style", "padding: 5px; display: block; background-color: #ddd; border: 1px solid #ddd; float: left; font-size: 12px; color: rgb(0, 0, 0);");
            }
            submission_grade_tid_InProgress = false;
        },
        error: function() {
            submission_grade_tid_InProgress = true;
            //                alert(data);
            //                alert('Error saving this score, please re-enter');
            submission_grade_tid_InProgress = false;
        }
    });
}

function disabled_outcomes_foruser(){
    //Get checkbox value
    var a = $('input[name=chk_incomplete]').is(':checked');
    var grading_method = $('#activity_grading_method').val();

    //Set/Unset disabled attribute in textboxes
    if(a == true){
        $('input[name=disabled_outcome[]]').each(function(){
            $(this).attr("disabled", "disabled");
            $(this).val(0);
        });
    }else{
        /* if activity is auto_graded acitivity then place a maxim points when
         * teacher uncheck incomplete */
        if(grading_method == 1){
          
            $('input[name=auto_graded_outcome[]]').each(function(){
                var key = '';
                key = $(this)[0].id;
                key = key.substr(2, key.length);
                var maximum_points = $(this).val();
                $('input[name=disabled_outcome[]]').each(function(){
                    var outcome_id= '';
                    outcome_id = $(this)[0].id;

                    if(key == outcome_id){
                        $(this).val(maximum_points);
                    }
                });
                
            });
            $('input[name=disabled_outcome[]]').each(function(){
                $(this).removeAttr("disabled");

            });
        }else{
            $('input[name=disabled_outcome[]]').each(function(){
                $(this).removeAttr("disabled");
                
            });
        }


    }
}

function publish_this_activity(activity_nid, status){
    $.ajax({
        type: "GET",
        url: '?q=ntlp/courses/activity/publish/' + activity_nid + '/' +status,
        cache: false,
        success: function(data){
            
            $('#publishlink').html(data)
        //                == "Publish") {
        //                $('#publishlink').html("Unpublish Grades");
        //            } else {
        //                $('#publishlink').html("Publish Grades");
        //            }
        }
    });
}


//JR new Req. Clear Rubric


function clr_rubric(){

    for (var i=1; i<=4;i++){
        elem_name = 'level'+ i + '_descriptors[]';
        elements = document.getElementsByName(elem_name );
        for (var j=0; j<5;j++)
            elements[j].value ='';
    }
}

function clear_rubric_evaluation_fields(){
    
    $("textarea").each(function() {

        $(this).val('');
        var element_id = this.id;
        var id_value = element_id.split(/[\s-]+/);
        var id = id_value[id_value.length-1];

        var hs= $("#slider_" + id).slider();
        hs.slider('option', 'value',0);
        hs.slider("option","slide").call(hs,null,{
            handle: $('.ui-slider-handle', hs),
            "value": 0
        });
        
    });

}

function initDlg(){
    
    $("#dialog-show-confirmation").dialog({
        autoOpen: false,
        resizable: false,
        height:100,
        modal: true
    });
}

//--------------------------------
// Changes the behavior of Accordion JQuery UI to act like a Panel
//--------------------------------
$(document).ready(function() {

    initDlg();
    
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
            .toggleClass("ui-icon-triangle-1-e").toggleClass("ui-icon-triangle-1-s")
            .parent().parent().parent().parent().parent()
            .toggleClass("ui-accordion-header-active ui-state-active ui-state-default ui-corner-bottom")
            .next().toggleClass("ui-accordion-content-active").slideToggle();
            return false;
        });
        $(".notaccordion").next()
        .addClass("ui-accordion-content ui-helper-reset ui-widget-content ui-corner-bottom")
        .show();

        //        alert('expand all exists');
        expand_all_panels(1);
    }

    expand_default_panel();
});

function expand_default_panel() {

    $('html, body').animate({
        "scrollTop":$(".def_mode_exp").offset().top
    });

    //    //collapse all panels first
    //    $(".def_mode_col")
    //        .toggleClass("ui-accordion-header-active ui-state-active ui-state-default ui-corner-bottom")
    //        .find("> .ui-icon").toggleClass("ui-icon-triangle-1-s ui-icon-triangle-1-e").end()
    //        .next().toggleClass("ui-accordion-content-active").slideUp();
    //        $(".notaccordion .ui-icon").attr("class", "ui-icon ui-icon-triangle-1-s");
    //        $('#expand_all').attr('class', '');
    //        $('#collapse_all').attr('class', 'GradesTabActive');


    //expand default panel
    $(".def_mode_exp")
    .toggleClass("ui-accordion-header-active ui-state-active ui-state-default ui-corner-bottom")
    .find("> .ui-icon").toggleClass("ui-icon-triangle-1-e ui-icon-triangle-1-s").end()
    .next().toggleClass("ui-accordion-content-active").slideDown();

    $(".def_mode_exp .ui-icon").attr("class", "ui-icon ui-icon-triangle-1-s");

    $('html, body').animate({
        "scrollTop":$(".def_mode_exp").offset().top
    });
  
}

function expand_all_panels(to_expand) {
    if(to_expand == 1){
        $(".notaccordion")
        .toggleClass("ui-accordion-header-active ui-state-active ui-state-default ui-corner-bottom")
        .find("> .ui-icon").toggleClass("ui-icon-triangle-1-e ui-icon-triangle-1-s").end()  //Done
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

function disable_lateOption(optiondis){
    
    var option = $(optiondis).val();
    //alert(optiondis);
    
    if(option=='O'){
        $('#allow_late_chk').attr("disabled","disabled");
        
        $('input[name="main[info][allow_late_submission]"]').each(function(){
            $(this).attr("disabled", "disabled");
            $('#edit-main-info-allow-late-submission-0').attr('checked','checked');
        });

        $('input[name="main[info][allow_student_resubmit]"]').each(function(){
            $(this).attr("disabled", "disabled");
            $('#edit-main-info-allow-student-resubmit-0').attr('checked','checked');
        });

        $('#edit-main-detail-grades-gradingmethod-M').attr('disabled','disabled');
        $('#edit-main-detail-grades-gradingmethod-C').attr('disabled','disabled');
        $('#edit-main-detail-grades-gradingmethod-M').attr('checked','checked');
        
    }else {

        $('input[name="main[info][allow_student_resubmit]"]').each(function(){
            $(this).removeAttr("disabled");
        });

        $('input[name="main[info][allow_late_submission]"]').each(function(){
            $(this).removeAttr("disabled");
        });
        
        $('input[name=auto_grading_option]:checked').removeAttr("disabled","disabled");
        $('#edit-main-detail-grades-gradingmethod-M').removeAttr('disabled','disabled');
        $('#edit-main-detail-grades-gradingmethod-C').removeAttr('disabled','disabled');
    }
}

function on_save_grade_all(activity_nid) {
    clear_autosave();

    $.ajax({
        type: "GET",
        url: '?q=ntlp/grade/activity/autosave/' + activity_nid,
        cache: false,
        success: function(data){

            $('div[name=last_saved_date]').text(data);
            $('div[name=last_saved_date]').attr("style", "padding: 5px; display: block; background-color: #ddd; border: 1px solid #ddd; float: left; font-size: 12px; color: rgb(0, 0, 0);");

        }
    });
//   
//    $('div[name=last_saved_date]').animate({
//        "opacity": "toggle"
//    }, "slow");
//    $('div[name=last_saved_date]').animate({
//        "opacity": "toggle"
//    }, 2000);
//    $('div[name=last_saved_date]').text(data);
//    $('div[name=last_saved_date]').attr("style", "padding: 5px; display: block; background-color: #ddd; border: 1px solid #ddd; float: left; font-size: 12px; color: rgb(0, 0, 0);");

//
//    var d = new Date();
//    var curr_date = leadingZeros(d.getDate(), 2);
//    var curr_month = leadingZeros(d.getMonth()+1, 2);
//    var curr_year = d.getFullYear();
//    var hh = d.getHours();
//    if (hh < 12) {
//        m = 'am';
//        hh++;
//    } else {
//        m = 'pm';
//        hh -= 11;
//    }
//    hh = leadingZeros(hh, 2);
//    var mm = leadingZeros(d.getMinutes(), 2);
//    var lsd = 'Last saved ' + curr_month + "/" + curr_date + "/" + curr_year + " @ " + hh + ":" + mm + " " +m;
}

function leadingZeros(num, totalChars, padWith) {
    num = num + "";
    padWith = (padWith) ? padWith : "0";
    if (num.length < totalChars) {
        while (num.length < totalChars) {
            num = padWith + num;
        }
    } else {}

    if (num.length > totalChars) { //if padWith was a multiple character string and num was overpadded
        num = num.substring((num.length - totalChars), totalChars);
    } else {}

    return num;
}


function validate_input(x){
    var flag = false;
    var value = $(x).val();
   
    if (!isNaN(value)) {
        if(value >= 1000){
            note = 'The maximum point value is 999.99. Please re-enter this score. ';

            $("#dialog-show-confirmation").dialog('option', 'buttons',{
                'Close': function() {
                    $(this).dialog('close');
                }
            });
            
            $("#dialog-show-confirmation").find('.msg').css('color', '#000000').text(note);
            $("#dialog-show-confirmation").dialog("open");
            
            x.focus();
            x.value = '';
            flag = false;
        }else{
            var index = value.indexOf('.');
            if(index > 0){
                temp_var = value.substr(index+1, value.length);                
                if(temp_var.length > 2){

                    note = 'Only two decimal places are allowed. Please re-enter.';
                    $("#dialog-show-confirmation").dialog('option', 'buttons',{
                        'Close': function() {
                            $(this).dialog('close');
                        }
                    });
                    $("#dialog-show-confirmation").find('.msg').css('color', '#000000').text(note);
                    $("#dialog-show-confirmation").dialog("open");
                    
                    x.focus();
                    x.value = '';
                    flag = false;
                }else{
                    flag = true;
                }
            }
        }
    }
    return flag;
    
}



function show_note(note){
    
    $("#dialog-show-confirmation").dialog('option', 'buttons',{
        'Close': function() {
            $(this).dialog('close');
        }
    });
    $("#dialog-show-confirmation").find('.msg').css('color', '#000000').text(note);
    $("#dialog-show-confirmation").dialog("open");
}

function show_addnew_tipsy(){

    $('.addnew').tipsy('show');
    setTimeout('$(".addnew").tipsy("hide");' , 5000);
    return false;

}

function init_autosave(activity_nid) {
    if (gradeall_timer == null) {
        gradeall_timer = setTimeout("autosave_activity_grades("+activity_nid+")", 30*1*1000);
    }
}
function clear_autosave() {
    clearTimeout(gradeall_timer);
    gradeall_timer = null;
}

function autosave_activity_grades(activity_nid) {
    if (submission_grade_tid_InProgress)
        return;

    $('div[name=last_saved_date]').text("Auto Saving...");
    $('.SaveBtn').hide();
    $('.SaveBtnDisabled').show();

    //Call the Save function
    on_save_grade_all(activity_nid);

    $('.SaveBtn').show();
    $('.SaveBtnDisabled').hide();
}

$(document).ready(function() {

    $("#dialog-activity-delete").dialog({
        autoOpen: false,
        resizable: false,
        height:90,
        width: 600,
        modal: true
    });
});



function ntlp_activity_delete(course_nid, activity_nid){
    $.ajax({
        type: "GET",
        url: "?q=ntlp/courses/activity/is_graded/"+course_nid+"/"+activity_nid,
        success:  function(data) {
            if(data == 1){
                $( "#dialog-activity-delete" ).dialog( "option", "width", 600 );
                $("#dialog-activity-delete").dialog('option', 'buttons',{
                    'Ok': function() {
                        $(this).dialog('close');
                    }
                });
                msg = "You cannot delete this activity because its graded. In order to delete, you must un-assign all students fom this activity and then proceed.";
               $(".ui-dialog-title").addClass('tahomaDialog');
               $("#dialog-activity-delete").find('.msg').css({
                    'color': '#000000',
                    'font': '12px Tahoma,Helvetica,Arial,sans-serif',
                    'font-weight': 'bold'
                }).text(msg);  
                $("#dialog-activity-delete").dialog("open");
            }
            else {
                $(".ui-dialog-title").addClass('tahomaDialog');
                $('.ui-widget').attr('id','delete_activity_dialog');
                $( "#dialog-activity-delete" ).dialog( "option", "width", 600 );
                $( "#dialog-activity-delete" ).dialog( "option",  "height", 175 );
                $("#dialog-activity-delete").dialog(
                    'option', 'buttons',{
                        'Yes': function() {
                            $('.delete_activity_dialog').find('button:contains("Yes")').attr('disabled','disabled');
                            $('.delete_activity_dialog').find('button:contains("No")').attr('disabled','disabled');
                            $.ajax({
                                type: "GET",
                                url: "?q=ntlp/courses/activity/remove/"+course_nid+"/"+activity_nid,
                                success:  function(data) {
                                    if (data == "ERROR") {
                                        window.location.reload();
                                    } else {
                                        window.location = "?q=ntlp/courses/activity/"+course_nid;
                                    }
                                }

                            });
                        },
                        'No': function() {
                            $(this).dialog('close');
                        }
                    });
                
                              
                msg = "<div><div class='ntlp_people_finder'><div class='content'><div class='top_right'><div class='top_left'><div class='bot_left'><div class='bot_right_2' style='height:100px !important;'><div style='padding:0px 5px 5px 7px;'>You are about to delete this activity.<br /><br />All student work and grades associated with this activity will be permanently removed. Are you sure you want to delete this activity?</div></div></div></div></div></div></div></div>";
                $(".ui-dialog").find('#ui-dialog-title-dialog-show-confirmation').html('Confirm Delete Activity');  
                $("#dialog-activity-delete").find('.msg').css({
                    'color':'#3570AA',
                    'font': '12px Tahoma,Helvetica,Arial,sans-serif',
                    'font-weight': 'bold'
                }).html(msg);  
                $("#dialog-activity-delete").dialog("open");
                
                if( $('#delete_activity_dialog').find('.ui-dialog-buttonpane')){
                    $('.ui-dialog-buttonpane').addClass('delete_activity_dialog');
                    $('.delete_activity_dialog').find('button:contains("No")').attr('id','NewSaveBtn');
                    $('.delete_activity_dialog').find('button:contains("Yes")').attr('id','NewSaveBtn');
                }
               
            }
        }

    });

    
}

