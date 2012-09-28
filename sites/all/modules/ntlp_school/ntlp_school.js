$(document).ready (function () {
    try {
        if ($('#progressbar-users')) {              //Something should be checked before calling this function
            $('#progressbar-users').progressbar({   //it doesn't exist in all pages of School admin
                value: 0
            });
        }
    } catch (e) {

    }
    
   
    init_jquery_dialog();
});

function init_jquery_dialog(){
    
    
    $("#dialog-confirm-massaction").dialog({
        autoOpen: false,
        resizable: false,
        height:100,
        width:500,
        modal: true, 
        zIndex:9999
    });
    
    $("#dialog-confirm-action").dialog({
        autoOpen: false,
        resizable: false,
        height:100,
        minHeight:100,
        width:350,
        modal: true, 
        zIndex:9999
    });    
}

function change_user_status(school_id) {

    var totalUsersSelected = $('input[name="userid\\[\\]"]:checked').length;
    var status  = $('#status').val();
    var msg = "";
    var users = '';
    var inactiveSelected = false;
    var us;

    $('input[name=userid\\[\\]]:checked').each(function(){
        users += $(this).val()+',';
        us = $('input[name=userstatus\\['+$(this).val()+'\\]]').val();
        if (us == "Inactive") {
            inactiveSelected = true;
        //            alert('Inactive user selected');
        }
    });
    $("#dialog-confirm-massaction").dialog('option', 'title', 'Confirm status change');

    if(totalUsersSelected > 0) {
        if (status == 1) {      //Active (Enabled)
            $("#dialog-confirm-massaction").dialog('option', 'buttons',{
                'Yes - Enable accounts': function() {
                    call_change_user_status(school_id, status, users);
                    $(this).dialog('close');
                },
                'Cancel': function() {
                    $(this).dialog('close');
                }
            });

            msg = 'The selected users accounts are about to be enabled so they can login. Are you sure you want to enable these accounts? ';
        } else if (status == 2) {      //Active (Disabed)
            if (inactiveSelected) {
                msg = "One or more of the selected users are Inactive. Only Active (Enabled) user accounts can be disabled. Please re-select users that meet this criteria.";
                $("#dialog-message-err").find('.msg').css('color', '#000000').text(msg);
                $('#dialog-message-err').dialog("open");
                return;
            } else {
                $("#dialog-confirm-massaction").dialog('option', 'buttons', {
                    'Yes - Disable accounts': function() {
                        call_change_user_status(school_id, status, users);
                        $(this).dialog('close');
                    },
                    'Cancel': function() {
                        $(this).dialog('close');
                    }
                });

                msg = 'The selected users will not be able to login once their account is disabled. All of their data, including course activities and gradebooks, will be available to school staff while their account is disabled. Are you sure you want to disable these accounts? ';
            }
        } else if (status == 0) {      //Inactive
            //            alert("?q=ntlp/user/confirmstatuschange/"+status+"/"+school_id+"/"+users);
            $.ajax({
                type: "GET",
                url: "?q=ntlp/user/confirmstatuschange/"+status+"/"+school_id+"/"+users,
                success:  function(data){
                    $("#dialog-confirm-massaction").dialog('option', 'buttons',{
                        'Yes - Inactivate accounts': function() {
                            call_change_user_status(school_id, status, users);
                            $(this).dialog('close');
                        },
                        'Cancel': function() {
                            $(this).dialog('close');
                        }
                    });
                    msg = data;
                    if( data.substr(0,6) == '!DROP!') {
                        msg = data.substr(6);
                    } else if( data.substr(0,4) == '!OK!') {
                        msg = data.substr(4);
                    }
                    $("#dialog-confirm-massaction").find('.msg').css('color', '#000000').text(msg);
                }
            });
        } else if (status == 3) {      //Remove Account
            msg = "Please wait...";
            $("#dialog-confirm-massaction").dialog('option', 'buttons',{
                'Ok':function(){
                    $(this).dialog('close');
                }
            });

            $.ajax({
                type: "GET",
                url: "?q=ntlp/user/candelete/"+school_id+"/"+users,
                success:  function(data){
                    if (typeof(data) == 'string') {
                        jresponse = Drupal.parseJson(data);

                        if (jresponse.canDelete == 1) {
                            msg = jresponse.message;
                            $("#dialog-confirm-massaction").dialog('option', 'buttons',{
                                'Yes - Delete users': function() {
                                    call_delete_users(school_id, users);
                                    $(this).dialog('close');
                                },
                                'Cancel': function() {
                                    $(this).dialog('close');
                                }
                            });
                            $("#dialog-confirm-massaction").find('.msg').css('color', '#000000').text(msg);
                        } else {
                            msg = jresponse.message;
                            $("#dialog-confirm-massaction").find('.msg').css('color', '#000000').text(msg);
                        }
                    } else {
                        msg = 'Invalid response received from server, please try again.';
                        $("#dialog-confirm-massaction").find('.msg').css('color', '#000000').text(msg);
                    }
                }
            });
        }
    } else {
        msg = 'Please select user(s)';
        $("#dialog-confirm-massaction").dialog('option', 'buttons',{
            'Ok':function(){
                $(this).dialog('close');
            }
        });
    }

    $("#dialog-confirm-massaction").find('.msg').css('color', '#000000').text(msg);
    $("#dialog-confirm-massaction").dialog('open');
}


function call_change_user_status(school_id, status, users){
    //    alert('This feature is disabled temporarily.');
    //    return;

    $.ajax({
        type: "GET",
        url: "?q=ntlp/user/status/"+status+"/"+school_id+"/"+users,
        success:  function(data){
            //            alert(data);
            //        $('#GetUserList').html(data);
            if( data.substr(0,7) == '!ERROR!') {
                msg = data.substr(7);
                $("#dialog-message-err").find('.msg').css('color', '#000000').text(msg);
                $('#dialog-message-err').dialog("open");
            } else {
                $("input[class^='myFilter']:first").trigger('onclick');
            //                return data;
            }
        }
    });
}

$(function(){
    $('#dialog-message-err').dialog({
        autoOpen: false,
        height:100,
        width:300,
        modal: true, 
        zIndex:9999,
        buttons: {
            Ok: function() {
                $(this).dialog('close');
            }
        }
    });
});

function change_user_role(school_id){

    var role  = $('#role').val();
    var users = '';

    $('input[name=userid\\[\\]]:checked').each(function(){
        users += $(this).val()+',';
    });

    $.ajax({
        type: "GET",
        url: "?q=ntlp/user/role/"+role+"/"+school_id+"/"+users,
        success:  function(data){
            
            if( data.substr(0,6) == 'Error:') {
                $('#dialog-message-err p').html(data.substr(6));
                $('#dialog-message-err').dialog("open");
            }else{
                $('#GetUserList').html(data);
            }
        }

    });

}

function on_imagechange(fileId, fileName) {

    var school_id = $("#school_id").val();

    $.ajax({
        type: "GET",
        url: "?q=ntlp/save/school/image/"+fileId+"/"+school_id,
        cache: false,
        success:  function(data){
            $("#school_image").html(data);
            Drupal.behaviors.modalFrameSetup();
        }
    
    });

    Drupal.modalFrame.close().closeDialog();
}


function on_fileupload_success(fileId, fileName) {

    document.getElementById('file_fid').value = fileId ;
}

function email_validation(){

    var email = $("#edit-main-user-name").val();

    alert(email);


}

function _get_csv_record(){
    $('#hide_button').hide();

    if( _get_csv_record.calls == undefined ) {
        _get_csv_record.calls = 1;
        $('#progressbar-users-row').fadeIn();
    }
    
    var file_id  = document.getElementById('file_info_id').value;
    var file_path  = document.getElementById('file_info_path').value;
    //    var update_record  = $('input[name=main[update_existing_account]]:checked').val();

    var update_record = document.getElementById('user_check').checked;
    var change_password = document.getElementById('change_password').checked;
    //
    if(update_record == true){

        update_record  = 1;
    }else{

        update_record = 0;
    }

    if(change_password == true){

        change_password  = 1;
    }else{

        change_password = 0;
    }

    
    $.get(
        "?q=ntlp/school/management/csv/"+file_id+"/"+update_record+"/"+change_password+'&calls='+_get_csv_record.calls,
        {},
        function(data){
            if(!data.error) {
                var prefixText = 'Creating...';
                if( data.done != data.total ) {
                    _get_csv_record.calls++;
                    _get_csv_record();
                    $('#progressbar-users').progressbar('option', 'value', Math.round(data.done/data.total*100));
                    $('#progressbar-users-processed').html(prefixText+data.processedUser).animate({
                        "opacity":0
                    },1000).animate({
                        "opacity":1
                    },0);
                    $('#progressbar-users-percent').html((Math.round(data.done/data.total*100))+'%');
                } else if(data.done == data.total) {
                    $('#progressbar-users').progressbar('option', 'value', 100);
                    $('#progressbar-users-percent').html('100%');
                    //                    $('#progressbar-users-processed').html(prefixText+data.processedUser).animate({"opacity":1},0).animate({"opacity":0},1000)
                    $('#hide_button').show();
                    window.location.reload();
                }
            } else {
                window.location.reload();
            }
        }
        ,'json'
        );


}

function select_all(obj){

    var x = document.getElementsByName('userid[]');

    if(x.length > 0){
        for(i = 0; i<= x.length; i++){
            if(obj){
                x[i].checked = true;
            }else{
                x[i].checked = false;
            }
        }
    }
}

function call_delete_users(school_id, users) {
    msg = 'Removing selected users, please wait...';
    $("#dialog-confirm-massaction").dialog('option', 'buttons',{
        'Ok':function(){
            $(this).dialog('close');
        }
    });

    $.ajax({
        type: "GET",
        url: "?q=ntlp/user/confirmdeletion/"+school_id+"/"+users,
        success:  function(data){
            if (typeof(data) == 'string') {
                jresponse = Drupal.parseJson(data);

                if (jresponse.success == 1) {
                    window.location.reload(true);
                } else {
                    msg = jresponse.message;
                    $("#dialog-confirm-massaction").find('.msg').css('color', '#000000').text(msg);
                    $("#dialog-confirm-massaction").dialog('open');
                }
            } else {
                msg = 'Invalid response received from server, please try again.';
                $("#dialog-confirm-massaction").find('.msg').css('color', '#000000').text(msg);
            }
        }
    });
    $("#dialog-confirm-massaction").find('.msg').css('color', '#000000').text(msg);
    $("#dialog-confirm-massaction").dialog('open');
}
//
//function school_general_setting_form_submit(current_school_year){
//    $('input[type="checkbox"]:disabled').attr('disabled', false)
//    
//    new_school_year = $('#edit-main-school-year-current-school-year').val();
//    //    alert(current_school_year +" "+ new_school_year);
//    if(current_school_year != new_school_year){
//        
//        $("#dialog-confirm-massaction").dialog('option', 'buttons',{
//            'Yes':function(){
//                document.getElementById('school-general-settings-form').submit();
//                $(this).dialog('close');
//                   
//            },
//            'No':function(){
//                $(this).dialog('close');
//                return false; 
//            }
//        });
//        $("#dialog-confirm-massaction").find('.msg').css('color', '#000000').text("Are you sure you want to change the school Year? ");
//        $("#dialog-confirm-massaction").dialog('open');
//        
//    }else{
//        document.getElementById('school-general-settings-form').submit();
//    }
//   
//}

function change_school_year_call(obj, current_school_year){
  
    var year = current_school_year;
    var selecte_year_change = parseInt(year)-1 ;
    if(current_school_year != $(obj).val()){
        
//        $("#dialog-confirm-massaction").dialog('option', 'buttons',{
//            'Yes':function(){
//                $(this).dialog('close');
//                   
//            },
//            'No':function(){
//
//                $('#edit-main-school-year-current-school-year-wrapper ul.newList li ').find('.hiLite').removeClass('hiLite');
//                $('#edit-main-school-year-current-school-year-wrapper .selectedTxt').html(selecte_year_change + " - " +year);
//                var new_year =   $('#edit-main-school-year-current-school-year-wrapper .selectedTxt').html(selecte_year_change + " - " +year);
//                $('#edit-main-school-year-current-school-year-wrapper ul.newList li ').find(':contains('+new_year.html()+')').addClass('hiLite');
//                $(this).dialog('close');
//            }
//        });
        $("#dialog-confirm-massaction").find('.msg').css('color', '#000000').html("<p>You are about to change the active school year.  This will cause all courses from the previous year to become inactive. Inactive courses are still accessible to current staff, students, and parents via the Courses section, but they are archived and no new content or grades can be modified.</p> \n\
<br> Note: The active year will not be changed until you click the Save button. ");
        $("#dialog-confirm-massaction").dialog('open');
    }
}

function create_new_year(){
    var new_year = $('#selected_new_year').val();
    var school_nid = $('#school_nid').val();
    var selected_option = '';
    var length = $("input:checked").length;
    var x=document.getElementsByName("content[]");
    for(i=0; i < x.length; i++){
        
        if(x[i].checked){
            if(length == 1){
                selected_option = x[i].value;
            }else{
                if((i +1) == x.length){
                    selected_option += x[i].value;
                }else{
                    selected_option += x[i].value +", ";
                }
                
            }
        }
    }
    
    if(selected_option == ''){
        selected_option = 'none';
    }
    $.ajax({
        type: "GET",
        url: "?q=ntlp/school/createnewyear/data/"+school_nid+"/" +new_year+"/"+selected_option,
        success:  function(data){
          
            //            parent.window.location.reload();

            parent.window.location = '?q=ntlp/school/management/course&foryear='+new_year;           
            try {
                parent.Drupal.modalFrame.close().closeDialog();
            } catch (e) {
            }
        }
    });
    
    
}



function delete_school_outcome(outcome_tid, school_nid, school_year){
    
    msg = 'Checking the outcome usage, please wait...';
    $("#dialog-confirm-massaction").dialog('option', 'buttons',{
        'Close':function(){
            $(this).dialog('close');
        }
    });
    
    $.ajax({
        type: "GET",
        url: "?q=ntlp/school/delete/outcome/"+outcome_tid+"/"+school_nid+"/"+school_year,
        success:  function(data){
            if (typeof(data) == 'string') {
                jresponse = Drupal.parseJson(data);

                if (jresponse.success == 1) {
                    window.location.reload(true);
                } else {
                    msg = jresponse.message;
                    $("#dialog-confirm-massaction").find('.msg').css('color', '#000000').text(msg);
                    $("#dialog-confirm-massaction").dialog('open');
                }
            } else {
                msg = 'Invalid response received from server, please try again.';
                $("#dialog-confirm-massaction").find('.msg').css('color', '#000000').text(msg);
            }
        }
    });
                
    
    $("#dialog-confirm-massaction").find('.msg').css('color', '#000000').text(msg);
    $("#dialog-confirm-massaction").dialog('open');
} 

function save_outcome(school_nid, school_year){

    init_jquery_dialog();
    
    var description = encodeURIComponent($('#outcome_desc').val());
    var outcome_short_title = encodeURIComponent($('#outcome_short_title').val());
    var school_oc_tid = $('#school_oc_tid').val();
    var new_outcome_title = encodeURIComponent($('#new_outcome_title').val());
    var msg = '';
    var validation_status = true;
    
    if(school_oc_tid == 'new'){
        if(new_outcome_title != undefined || new_outcome_title != null){
            
            if(new_outcome_title == ''){
                msg = 'You must provide the outcome Name.';
                validation_status = false;
            }

        }
    }else if(school_oc_tid == 0){
        msg = 'You must provide the outcome Name or select from list.';
        validation_status = false;
    }
   
    if(validation_status){
        
        if(school_oc_tid == 'new'){
            $.ajax({
                type: "GET",
                url: "?q=ntlp/school/check/outcomename/"+school_nid+"/"+new_outcome_title+"/"+0,
                success:  function(data){
                    jresponse = Drupal.parseJson(data);
                    /* success equal to 0 if no match found */
                    if (jresponse.success == 0) {
            
                        $.ajax({
                            type: "GET",
                            url: "?q=save_school_outcome/"+school_nid+"/"+school_year+"/"+school_oc_tid+"/"+outcome_short_title+"/"+description+"/"+new_outcome_title,
                            success:  function(data){
                                parent.window.location.reload();
                
                                try {
                                    parent.Drupal.modalFrame.close().closeDialog();
                                } catch (e) {
                                }
                            }
                        });
                    
                    }else{
                        msg = jresponse.message;
                        $("#dialog-confirm-action").dialog('option', 'buttons',{
                            'Close':function(){
                                $(this).dialog('close');
                            }
                        });
                        $("#dialog-confirm-action").find('.msg').css('color', '#000000').text(msg);
                        $("#dialog-confirm-action").dialog('open');
                    }
                }
            });
        }else{
            $.ajax({
                type: "GET",
                url: "?q=save_school_outcome/"+school_nid+"/"+school_year+"/"+school_oc_tid+"/"+outcome_short_title+"/"+description+"/"+new_outcome_title,
                success:  function(data){
                    parent.window.location.reload();
                
                    try {
                        parent.Drupal.modalFrame.close().closeDialog();
                    } catch (e) {
                    }
                }
            });
        }
        
    }else{
        $("#dialog-confirm-action").dialog('option', 'buttons',{
            'Close':function(){
                $(this).dialog('close');
            }
        });
        $("#dialog-confirm-action").find('.msg').css('color', '#000000').text(msg);
        $("#dialog-confirm-action").dialog('open');
    }
    
}

function ntlp_edit_outcome(school_nid, school_year, outcome_tid){
    
    init_jquery_dialog();
    var desc = encodeURIComponent($('#outcome_desc').val());
    var short_name = encodeURIComponent($('#outcome_short_title').val());
    var outcome_title = encodeURIComponent($('#new_outcome_title').val());
    var msg = '';
    var validation_status = true;
    
    if(outcome_title == ''){
        msg = 'You must provide the outcome Name.';
        validation_status = false;
    }
    if(validation_status){
     
        $.ajax({
            type: "GET",
            url: "?q=ntlp/school/check/outcomename/"+school_nid+"/"+outcome_title+"/"+outcome_tid,
            success:  function(data){
                jresponse = Drupal.parseJson(data);
                /* success equal to 0 if no match found */
                if (jresponse.success == 0) {
            
                    $.ajax({
                        type: "GET",
                        url: "?q=edit_school_outcome/"+school_nid+"/"+school_year+"/"+outcome_tid+"/"+short_name+"/"+desc+"/"+outcome_title,
                        success:  function(data){
                            parent.window.location.reload();
            
                            try {
                                parent.Drupal.modalFrame.close().closeDialog();
                            } catch (e) {
                            }
                        }
                    });
                    
                }else{
                    msg = jresponse.message;
                    $("#dialog-confirm-action").dialog('option', 'buttons',{
                        'Close':function(){
                            $(this).dialog('close');
                        }
                    });
                    $("#dialog-confirm-action").find('.msg').css('color', '#000000').text(msg);
                    $("#dialog-confirm-action").dialog('open');
                }
            }
        });
    
    }else{
        $("#dialog-confirm-action").dialog('option', 'buttons',{
            'Close':function(){
                $(this).dialog('close');
            }
        });
        $("#dialog-confirm-action").find('.msg').css('color', '#000000').text(msg);
        $("#dialog-confirm-action").dialog('open');
    }
}

function change_outcome_order(action, school_nid, school_year, outcome_tid, sort_order){

    $.ajax({
        type: "GET",
        url: "?q=change_school_outcome/order/"+action+"/"+school_nid+"/"+school_year+"/"+outcome_tid+"/"+sort_order,
        success:  function(data){
            $("#school-outcomes-wrapper").html(data);
            $('.new_green_link').removeClass('modalframe-setup-processed');
            Drupal.behaviors.modalFrameSetup();
        }
    });
}

function delete_school_term(term_tid, school_nid, school_year){
    
   
    
    $.ajax({
        type: "GET",
        url: "?q=ntlp/school/delete/term/"+term_tid+"/"+school_nid+"/"+school_year,
        success:  function(data){
            $("#dialog-confirm-massaction").dialog('option', 'buttons',{
                'Close':function(){
                    $(this).dialog('close');
                }
            });
            
            if (typeof(data) == 'string') {
                jresponse = Drupal.parseJson(data);

                if (jresponse.success == 1) {
                    window.location.reload(true);
                } else {
                    msg = jresponse.message;
                    
                    $("#dialog-confirm-massaction").find('.msg').css('color', '#000000').text(msg);
                    $("#dialog-confirm-massaction").dialog('open');
                }
            } else {
                msg = 'Invalid response received from server, please try again.';
                $("#dialog-confirm-massaction").find('.msg').css('color', '#000000').text(msg);
            }
        }
    });
                
    
}

function check_term_changes(school_nid, school_year, save_data){
    
    $('#loading').show();
    var dataString = $("#school-course-settings-form").serialize();
    $("#dialog-confirm-massaction").dialog({
        autoOpen: false,
        resizable: false,
        height:100,
        width:500,
        modal: true, 
        zIndex:9999
    });
    
    $.ajax({
        type: "POST",
        url: "?q=ntlp/school/check/term_changes/"+school_nid+"/"+school_year+"/"+save_data,
        data: dataString,
        success:  function(data){
            
            if(save_data == 0){
                if(data == "ERROR"){
                    $('#loading').hide();
                    $("#dialog-confirm-massaction").dialog('option', 'buttons',{
                        'Yes':function(){
                            check_term_changes(school_nid, school_year, 1);
                            $(this).dialog('close');
                        },
                        'No':function(){
                            window.location.reload(true);
                            $(this).dialog('close');
                        }
                    });
                    
                    $("#dialog-confirm-massaction").find('.msg').css('color', '#000000').text('You are making a change to a term that is being used in one or more courses.  Are you sure you want to continue?');
                    $("#dialog-confirm-massaction").dialog('open');
                
                }else if(data == 'SUCCESS'){
                    $('#loading').hide();
                    window.location.reload(true);
                }
            }else if(save_data == 1){
                $('#loading').hide();
                window.location.reload(true);
            }
        }
    });
}


// PHP-compatible urlencode() for Javascript
function urlencode(s) {
    s = encodeURIComponent(s);
    return s.replace(/~/g,'%7E').replace(/%20/g,'+');
}
