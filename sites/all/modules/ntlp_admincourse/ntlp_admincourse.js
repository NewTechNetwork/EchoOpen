// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$(function(){
    init_dialog();
    $('.enrolledDroppedBtn').click(function(){
        $(this).addClass('gradeSettingLinkActive').removeClass('gradeSettingLinkDisActive');
        $('#'+($(this).text()== 'Enrolled' ? 'Enrolled' : 'Dropped' )+'ShowBtn').addClass('gradeSettingLinkDisActive').removeClass('gradeSettingLinkActive');

        //handle the change status combo box
        var remIndex = $(this).text() == 'Enrolled' ? 2 : 1;           //Remove either Drop or Re-Enroll
        var showIndex = $(this).text() == 'Enrolled' ? 1 : 2;          //Show either Drop or Re-Enroll
        $('#actionsComboBox ul li').eq(remIndex).css('display', 'none').find('a').removeClass('hiLite').parent().parent().parent().find('span').text('').parent().prev().children().eq(0).attr('selected', 'selected')
        $('#actionsComboBox ul li').eq(3).css('display', 'block');     //Show Remove Student
        $('#actionsComboBox ul li').eq(showIndex).css('display', 'block');
    });

    $('#actionsComboBox ul li').eq(2).css('display', 'none');           //Hide Re-Enroll Student

    $('.outcome_item').change(function(e){

        });
});

function init_dialog(){
    $("#dialog-confirm-sbmtFrm").dialog({
        autoOpen: false,
        resizable: false,
        height:200,
        width:450,
        modal: true
    });
    $("#dialog-confirm-outcome").dialog({
        autoOpen: false,
        resizable: true,
        height:140,
        modal: true
    });
    
    $("#dialog-validate-outcome").dialog({
        autoOpen: false,
        resizable: true,
        minHeight: 250,
        minWidth: 400,
        modal: true
    });

}

function selectAllUsersChks(){
    $('input[name="users\\[\\]"]').each(function(){
        this.checked = !this.checked;
    });
}


function confirmFormSubmit() {
    $("#dialog-confirm-sbmtFrm").dialog('option', 'buttons',{
        'Yes': function() {
            document.getElementById('course-form-controller').submit();
            $(this).dialog('close');
        },
        'Cancel': function() {
            $(this).dialog('close');
        }
    });

    var users = "";
    $('input[name="users\\[\\]"]:checked').each(function(){
        users += $(this).val()+',';
    });
    var course_nid = $('#edit-main-course-node').val();
    var totalUsersSelected = $('input[name="users\\[\\]"]:checked').length;
    var selectedAction =  $.trim($('#actionsComboBox').find('.selectedTxt').text());
    var selectedPeriod =  $.trim($('#periodComboBox').find('.selectedTxt').text());
    if (selectedAction.charCodeAt(0) == 160) selectedAction = '';
    if (selectedPeriod.charCodeAt(0) == 160) selectedPeriod = '';
    //    console.log(selectedAction);
    var msg = 'Are you sure you want to perform this action? ';

    if(totalUsersSelected > 0) {
        if((selectedAction == '' && selectedPeriod != '') || (selectedAction.charCodeAt(0) == 160 && selectedPeriod.charCodeAt(0) != 160)) {
            msg = 'You have selected '+totalUsersSelected+' student(s) to be assigned to a new period.';
        } else if(selectedAction == 'Drop' && selectedPeriod == '') {
            // msg = 'You have selected '+totalUsersSelected+' student(s) to be dropped from this course.  All associated data for activities and grades will be archived, but you will not be able to add these users to any new activities or modify their grades. Are you sure you want to drop these users?';
            msg = 'You are about to drop the selected users from this course. All associated data for activities and grades will be archived and viewable, but you will NOT be able to add these users to any new activities or modify their grades. Also, the user\'s overall grades for ALL current and past terms will no longer print out in course reports.  <br /><br />Are you sure you want to continue?';
        } else if(selectedAction == 'Re-Enroll' && selectedPeriod == '') {
            msg = 'You have selected '+totalUsersSelected+' student(s) to be re-enrolled in this course.  All associated data for activities and grades that existed before the users was dropped will be made active again. Are you sure you want to re-enroll these users?';
        } else if(selectedAction == 'Remove' && selectedPeriod == '') {
            msg = 'Please wait...';
            $("#dialog-confirm-sbmtFrm").dialog('option', 'buttons',{
                'Ok':function(){
                    $(this).dialog('close');
                }
            });

            $.ajax({
                type: "GET",
                url: "?q=ntlp/admincourse/candelete/"+course_nid+"/"+users,
                success:  function(data){
                    if (typeof(data) == 'string') {
                        jresponse = Drupal.parseJson(data);

                        if (jresponse.canDelete == 1) {
                            msg = 'You have selected '+totalUsersSelected+' student(s) to be permanently removed from this course.  ' + jresponse.message;
                            $("#dialog-confirm-sbmtFrm").dialog(
                                'option', 'buttons',{
                                    'Yes - Permanently Remove': function() {
                                        call_delete_courseusers(course_nid, users);
                                        $(this).dialog('close');
                                    },
                                    'Cancel': function() {
                                        $(this).dialog('close');
                                    }

                                });
                            if($("#dialog-confirm-sbmtFrm").dialog( "isOpen" )){
                                $('.ui-dialog-buttonpane').find('button:contains("Yes - Permanently Remove")').attr('id','permenantly_removeBtn');
                            }
                            $("#dialog-confirm-sbmtFrm").find('.msg').css('color', '#000000').text(msg);

                        } else {
                            msg = jresponse.message;
                            $("#dialog-confirm-sbmtFrm").find('.msg').css('color', '#000000').text(msg);
                        }
                    } else {
                        msg = 'Invalid response received from server, please try again.';
                        $("#dialog-confirm-sbmtFrm").find('.msg').css('color', '#000000').text(msg);
                    }
                }
            });

        } else {
            msg = 'Please select either "Assign Period" or "Change Status" but not both at the same time.';
            $("#dialog-confirm-sbmtFrm").dialog('option', 'buttons',{
                'Ok':function(){
                    $(this).dialog('close');
                }
            });
        }
    } else {
        msg = 'Please select student(s).';
        $("#dialog-confirm-sbmtFrm").dialog('option', 'buttons',{
            'Ok':function(){
                $(this).dialog('close');
            }
        });
    }

    $("#dialog-confirm-sbmtFrm").find('.msg').css('color', '#000000');
    $("#dialog-confirm-sbmtFrm").find('.msg').html(msg);
    $("#dialog-confirm-sbmtFrm").dialog('open');
}

//Utility functions

// Array Remove - By John Resig (MIT Licensed)
Array.prototype.remove = function(from, to) {
    var rest = this.slice((to || from) + 1 || this.length);
    this.length = from < 0 ? this.length + from : from;
    return this.push.apply(this, rest);
};

function roundNumber(num, dec) {
    var result = Math.round(num*Math.pow(10,dec))/Math.pow(10,dec);
    return result;
}


function show_hide_grade(obj){

    //    alert($(obj).val());
    
    if($(obj).val()==1){
        document.getElementById('grade_scale_test').style.display='block';
    }else {
        document.getElementById('grade_scale_test').style.display='none';
    }
}

//Outcomes sorting
function move_up(rowId) {
    var row = $("#"+rowId);
    var n = row.parent().children().index(row);
    //    alert(row.prev().id+", index:"+n);
    if (n > 1) {
        row.insertBefore(row.prev());
        var oc_custom = document.getElementById('selected_oc_custom');
        var oc_id = document.getElementById('selected_oc_id');
        var oc_weight = document.getElementById('selected_oc_weight');

        find_and_move(oc_custom, n-1, n-2);
        find_and_move(oc_id, n-1, n-2);
        find_and_move(oc_weight, n-1, n-2);
    }
//    elseif (n == 2)
//        row.children(":sortup_link").hide();

}
function move_down(rowId) {
    var row = $("#"+rowId);
    var n = row.parent().children().index(row);
    var c = row.parent().children().length;
    //    alert(n+","+c);
    if ((n+3) < c) {
        row.insertAfter(row.next());
        var oc_custom = document.getElementById('selected_oc_custom');
        var oc_id = document.getElementById('selected_oc_id');
        var oc_weight = document.getElementById('selected_oc_weight');

        find_and_move(oc_custom, n-1, n);
        find_and_move(oc_id, n-1, n);
        find_and_move(oc_weight, n-1, n);
    }
}
function delete_outcome(rowId) {
    var row = $("#"+rowId);
    var n = row.parent().children().index(row);

    if (n > 1) {    //Compensate for Header and first row
        var oc_custom = document.getElementById('selected_oc_custom');
        var oc_id = document.getElementById('selected_oc_id');
        var oc_weight = document.getElementById('selected_oc_weight');
        //        var obj_i = find_value(oc_id, n-1);
        var obj_i = rowId.replace("outcome_", "");          //Get original Index

        var candelete_obj = document.getElementById('txt_candelete_'+obj_i);
        if (candelete_obj.value == "false") {
            $("#dialog-confirm-outcome").dialog('option', 'buttons',{
                'Ok': function() {
                    $(this).dialog('close');
                }
            });
            $("#dialog-confirm-outcome").find('.msg').css('color', '#000000').text("This outcome is being used in one or more activities, you cannot change or remove it.");
            $("#dialog-confirm-outcome").dialog('open');

            return false;
        }

        find_and_delete(oc_custom, n-1);
        find_and_delete(oc_id, n-1);
        find_and_delete(oc_weight, n-1);
        calc_total_wt(oc_weight);
        row.remove();
    }
}

//Toggle Enable/Disable of Outcome Weight text input
function show_hide_learning_outcome(){
//    var disabled = $("input[name='course_weight_grades']:checked").val() != 1;
//    $(".outcome_weight").attr("disabled", disabled);
}

//function onchange_outcome_hidden(obj, index){
//    //    alert($(obj).val());
//    var a = document.getElementById("txt_"+index).value;
////    alert(a);
//    document.getElementById("selected_oc").value += ','+$(obj).val()+','+index;
//
//}
function add_course_outcome(tid, wt, custom_name, can_delete) {
    var oc_id = document.getElementById('selected_oc_id');
    var oc_weight = document.getElementById('selected_oc_weight');
    var oc_custom = document.getElementById('selected_oc_custom');

    var index = get_outcome_combo(true);
    var name_obj = document.getElementById('outcome_cbo_'+index);
    var weight_obj = document.getElementById('txt_'+index);
    var custom_obj = document.getElementById('txt_custom_'+index);
    var candelete_obj = document.getElementById('txt_candelete_'+index);

    //In case custom names is not empty, and tid > 0 then an existing record is being loaded
    //Therefore the combo should show "Custom" along with the custom text box
    if (custom_name.length > 0 && tid < 0) {
        name_obj.options[name_obj.length-1].value = tid;
    }

    name_obj.value = tid;
    weight_obj.value = wt;
    custom_obj.value = custom_name;
    candelete_obj.value = can_delete;

    find_and_replace(oc_id, index, tid);
    find_and_replace(oc_weight, index, wt);
    find_and_replace(oc_custom, index, custom_name);
    calc_total_wt(oc_weight);

    document.getElementById('outcome_div_'+index).style.display = (tid <= 0 ? "block" : "none");
}

function get_outcome_combo(is_first) {
    var count = $("#ocContainer").children.length;
    //    var main = document.getElementById('ocContainer');
    //    var count = main.getElementsByTagName('tr ').length;
    count -= 1;

    //Deletion of current rows may cause duplicate names,
    //following routine perform checks to make sure we have unique ids
    var is_unique = false;
    while (!is_unique) {
        try {
            var id = document.getElementById("outcome_cbo_"+count).id;
            count++;
        } catch (err) {
            is_unique = true;
        }
    }
    //Hidden data - Outcomes
    var data1 = $('#oc_td1').val();
    data1 = data1.replace("@ID", "outcome_cbo_"+count);        //output = outcome_cbo_99
    data1 = data1.replace(/@INDEX/g, ""+count);                   //output = txt_99
    //Hidden data - Wieight
    var data2 = $('#oc_td2').val();
    data2 = data2.replace(/@INDEX/g, ""+count);       //regex does replace-all
    //Hidden data - Wieight
    var data3 = $('#oc_td3').val();
    data3 = data3.replace(/@INDEX/g, ""+count);       //regex does replace-all
    //Hidden data - Stores selected outcomes and their values
    document.getElementById("selected_oc_id").value += 'x,';
    document.getElementById("selected_oc_weight").value += '0,';
    document.getElementById("selected_oc_custom").value += ',';

    var newOC = document.createElement('tr');
    var td1 = document.createElement('td');
    var td2 = document.createElement('td');
    var td3 = document.createElement('td');
    newOC.appendChild(td1);
    newOC.appendChild(td2);
    newOC.appendChild(td3);
    td1.innerHTML = data1;
    td2.innerHTML = data2;
    td3.innerHTML = data3;
    newOC.id = "outcome_"+count;
    //    newOC.innerHTML = data;

    var main = document.getElementById('ocLastRow');
    main.parentNode.insertBefore(newOC, main);
    if (!is_first) {
        $('#'+newOC.id+' .my-dropdown').sSelect();
    }

    //Enable/Disable Outcome weights
    show_hide_learning_outcome();
    return count;
}

function cannot_delete_oc_handler(ctrl, oc_id, index) {
    $("#dialog-confirm-outcome").dialog('option', 'buttons',{
        'Ok': function() {
            $(this).dialog('close');
        }
    });
    $("#dialog-confirm-outcome").find('.msg').css('color', '#000000').text("This outcome is being used in one or more activities, you cannot change or remove it.");
    $("#dialog-confirm-outcome").dialog('open');

    var newLi = $("#"+ctrl.id).parent().parent().find('.newList li a');
    var containerDivText = $("#"+ctrl.id).parent().parent().find('.selectedTxt');
    newLi.eq(ctrl.selectedIndex).removeClass('hiLite');
    ctrl.value = find_value(oc_id, index-1);
    newLi.eq(ctrl.selectedIndex).focus();
    newLi.eq(ctrl.selectedIndex).addClass('hiLite');
    containerDivText.text(newLi.eq(ctrl.selectedIndex).text());
}
function onchange_outcome_item(ctrl){
    var row = $("#"+ctrl.id).parent().parent().parent();    //span/td/tr/table
    var index = row.parent().children().index(row);
    var obj_i = ctrl.id.replace("outcome_cbo_", "");
    
    var oc_id = document.getElementById('selected_oc_id');
    var oc_weight = document.getElementById('selected_oc_weight');

    var candelete_obj = document.getElementById('txt_candelete_'+obj_i);
    if (candelete_obj.value == "false") {
        cannot_delete_oc_handler(ctrl, oc_id, index);
        return false;
    }
    
    var weight_obj = document.getElementById('txt_'+obj_i);
    var weight_value = weight_obj.value;

    if (parseInt(weight_value) <= 0 || !weight_value) {       //Get default value if textbox is empty
        weight_obj.value = get_oc_weight(ctrl.value);
        weight_value = weight_obj.value;
    }
    find_and_replace(oc_id, index-1, ctrl.value);
    find_and_replace(oc_weight, index-1, weight_value);
    calc_total_wt(oc_weight);

    document.getElementById('outcome_div_'+obj_i).style.display = (parseInt(ctrl.value) <= 0 ? "block" : "none");
}

function onchange_ocweight(ctrl){
    var row = $("#"+ctrl.id).parent().parent();
    var index = row.parent().children().index(row);

    var oc_weight = document.getElementById('selected_oc_weight');
    
    find_and_replace(oc_weight, index-1, ctrl.value);
    calc_total_wt(oc_weight);
}

function onchange_custom_name(ctrl){
    var row = $("#"+ctrl.id).parent().parent().parent();    //span/td/tr/table
    var index = row.parent().children().index(row);

    var oc_custom = document.getElementById('selected_oc_custom');

    find_and_replace(oc_custom, index-1, ctrl.value);
}

function find_value(element, index){
    var arr = element.value.split(",");
    return arr[index];
}
function find_and_replace(element, index, value){
    var arr = element.value.split(",");
    arr[index] = value;
    
    var new_ids = '';
    for(i=0; i < arr.length-1; i++){
        new_ids += arr[i] + ",";
    }

    element.value = new_ids;
}
function find_and_move(element, index, newIndex){
    var arr = element.value.split(",");
    value = arr[index];
    arr[index] = arr[newIndex];
    arr[newIndex] = value;

    var new_ids = '';
    for(i=0; i < arr.length-1; i++){
        new_ids += arr[i] + ",";
    }

    element.value = new_ids;
}
function find_and_delete(element, index){
    var arr = element.value.split(",");
    arr.remove(index);

    var new_ids = '';
    for(i=0; i < arr.length-1; i++){
        new_ids += arr[i] + ",";
    }

    element.value = new_ids;
}
function calc_total_wt(element) {
    var arr = element.value.split(",");
    totalwt = 0;
    for(i=0; i < arr.length-1; i++){
        if (!isNaN(parseFloat(arr[i])))
            totalwt += parseFloat(arr[i]);
    }
    var oc_weight = document.getElementById('total_oc_weight-wrapper');
    oc_weight.innerHTML = roundNumber(totalwt, 2) + "%";
}

//Called by People Finder - multi-select students
function on_multiselect_users(su_ids, su_names, enroll_in_role_id) {
    //    alert('on_multiselect_students called');
    //    var su_ids =  $('#selected_users_ids').val();
    var course_nid =  $('#edit-main-course-node').val();
    //    alert(su_ids);
    $.ajax({
        type: "POST",
        url: "?q=ntlp/admincourse/enroll/"+course_nid+"/"+enroll_in_role_id+"/use_post/1",
        cache: false,
        data: 'su_ids='+su_ids,
        success: function(data){
            window.location.reload(true);
        }

    });
}
function on_multiselect_primaryteachers(su_ids, su_names, cache_id) {

    var tid, tname;
    tid = document.getElementById('course_primary_instructor_ids');
    tname = document.getElementById('primary_instructor_view');
    var existing_uids = tid.value.split(',');
    
    if (su_names.length > 0 && existing_uids.length < 3) {
        var namesDiv = su_names.split(',');
        var uids = su_ids.split(',');

        for(i=0; i < namesDiv.length; i++){
            namesDiv[i] = '<div id="' + uids[i] + '">\n\
                                <a onclick=remove_primary_user($(this),' + uids[i] + ',"' + cache_id + '");> \n\
                                    <img height="14" width="14" class="down" style="margin-top:4px;" src="themes/Boldr/Images/DeleteBtn.png"/>\n\
                                </a> &nbsp; ' + namesDiv[i] + '\
                            <br/></div>'
        }

        tid.value += ',' + su_ids;
        tname.innerHTML += namesDiv.join(' ');
    }
}

function remove_primary_user(element, uid, cache_id) {
    
    element.parent().fadeOut();
    var owner_id = document.getElementById('course_owner_id').value;
    var tid = document.getElementById('course_primary_instructor_ids');
    var uids = tid.value.split(',');

    var index = uids.indexOf(uid);

    if (index != -1) {
        uids.splice(index, 1);

        document.getElementById('course_primary_instructor_ids').value = uids.join(',');

        var selected_users = '';    //owner_id;

        if (document.getElementById('course_primary_instructor_ids').value.length > 0)
            selected_users += ',' + document.getElementById('course_primary_instructor_ids').value;

        if (cache_id != undefined) {
            $.ajax({
                type: "POST",
                url: '?q=ntlp/cache/set/' + cache_id,
                data: 'cache_data='+selected_users+ '&cache_table=ntlp_cache&expiry=CACHE_TEMPORARY'
            });
        }
    }
}

function on_multiselect_teachers(su_ids, su_names, cache_id) {

    var tid, tname;
    tid = document.getElementById('course_additional_instructor_ids');
    tname = document.getElementById('additional_instructor_view');

    if (su_names.length > 0) {
        var namesDiv = su_names.split(',');
        var uids = su_ids.split(',');

        for(i=0; i < namesDiv.length; i++){
            namesDiv[i] = '<div id="' + uids[i] + '">\n\
                                <a onclick=remove_secondary_user($(this),' + uids[i] + ',"' + cache_id + '");> \n\
                                    <img height="14" width="14" class="down" src="themes/Boldr/Images/DeleteBtn.png"/>\n\
                                </a> &nbsp; ' + namesDiv[i] + '\
                            <br/></div>'
        }

        if (tid.value == '')
            tname.innerHTML = '';//remove the 'None selected as yet' text

        tid.value += ',' + su_ids;
        tname.innerHTML += namesDiv.join(' ');
    }
}


function remove_secondary_user(element, uid, cache_id) {

    element.parent().fadeOut();
    var tid = document.getElementById('course_additional_instructor_ids');
    var tname = document.getElementById('additional_instructor_view');
    var uids = tid.value.split(',');

    var index = uids.indexOf(uid);

    if (index != -1) {
        uids.splice(index, 1);

        tid.value = uids.join(',');

        if (tid.value == '')
            tname.innerHTML = '<span style="color:gray; font-style:italic;">None selected as yet</span>';

        if (cache_id != undefined) {
            $.ajax({
                type: "POST",
                url: '?q=ntlp/cache/set/' + cache_id,
                data: 'cache_data='+tid.value+ '&cache_table=ntlp_cache&expiry=CACHE_TEMPORARY'
            });
        }
    }
}



//Called by People Finder - multi-select students
function on_singleselect_teacher(su_ids, su_names, enroll_in_role_id) {
    var tid = document.getElementById('course_primary_instructor_id');
    var tname = document.getElementById('primary_instructor_view');
    tid.value = su_ids;
    tname.innerHTML = su_names.replace(",", "<br />");
}


function test() {
    alert('test');
}
//Called by People Finder - multi-select students
function enroll_student(course_nid, user_uid, enroll_in_role_id) {
    var get_url = "?q=ntlp/admincourse/enroll/"+course_nid+"/"+enroll_in_role_id+"/"+user_uid+"/0/1";
    //alert(get_url);
    $.ajax({
        type: "GET",
        url: get_url,
        cache: false,
        success: function(data){
            $("#course_views").html(data);
        //            alert('User Successfully Enrolled');
        }

    });
}

//Called by People Finder - multi-select students
function drop_member(course_nid, user_uid, is_guest) {
    //    alert(su_ids);
    $.ajax({
        type: "GET",
        url: "?q=ntlp/admincourse/drop/"+course_nid+"/"+user_uid+"/"+is_guest,
        cache: false,
        success: function(data){
            $("#course_views").html(data);
            window.location.reload(true);
        //            alert('User Successfully Dropped');
        }
    });
}

function fixPagerAfterAjaxCall(wrapperId) {
    $('#'+wrapperId+' .pager li a, #'+wrapperId+'  table thead th a').live('click', function(){
        $.get($(this).attr('href'), function(response){
            $('#'+wrapperId).html(response);
        });
        return false;
    });
}

function show_enrolled_users(course_nid) {
    $.ajax({
        type: "GET",
        url: "?q=ntlp/admincourse/show_enrolled/"+course_nid,
        cache: false,
        success: function(data){
            $("#course_views").html(data);
            fixPagerAfterAjaxCall('course_views');
        }
    });
    $('#pf_add_students-wrapper').show();
}
function show_dropped_users(course_nid) {
    $.ajax({
        type: "GET",
        url: "?q=ntlp/admincourse/show_dropped/"+course_nid,
        cache: false,
        success: function(data){
            $("#course_views").html(data);
            fixPagerAfterAjaxCall('course_views');
        }
    });
    $('#pf_add_students-wrapper').hide();
}

function copy_course_cancel(){
    try {
        parent.Drupal.modalFrame.close().closeDialog();
    } catch (e) {

    }
}

function delete_course_cancel(){
    try {
        parent.Drupal.modalFrame.close().closeDialog();
    } catch (e) {

    }
}

function delete_resource_cancel(){
    try {
        parent.Drupal.modalFrame.close().closeDialog();
    } catch (e) {

    }
}




function delete_resource_data(resource_nid){

    $.ajax({
        type: "GET",
        url: "?q=ntlp/document/library/resource/delete/data/"+resource_nid,
        cache: false,
        success: function(data){
            //            $("#course_views").html(data);
            window.location.reload(true);
        }
    });

    try {
        parent.Drupal.modalFrame.close().closeDialog();

    } catch (e) {

    }

}

function delete_course_data(course_nid){
    $.ajax({
        type: "GET",
        url: "?q=ntlp/course/delete/data/"+course_nid,
        cache: false,
        success: function(){
            parent.window.location = '?q=ntlp/courses/my';
        }
    //        error: function() {
    //            alert ('An unhandled error occurred while deleting the course!');
    //        }
    });
}

function copy_course_data(course_nid){

    var selected_year = document.getElementById('cbo_course_year').value;
    var selected_term_tid = document.getElementById('cbo_school_term').value;
    var course_name = document.getElementById('txt_course_name').value;
    //    var selected_opt = $('input[name=row1]:checked').val();
    var course_opt = '';
    var selected_opt = 0;
    
    var x=document.getElementsByName("courses_copy[row1]");
    for(i=0; i < x.length; i++){
        if (x[i].checked){
            selected_opt = x[i].value;
        }
    }
    
    if(selected_opt == 1){
        course_opt = 'CS';
    }else if(selected_opt == 2){
        course_opt = 'CSE';
    }else{

        course_opt = 'CE';
    }

    if($.trim(course_name).length>0) {
        
        if(selected_term_tid > 0){
            $('body').prepend(
                $('<div id="plzWait">')
                .css({
                    "position": "absolute",
                    "top": "0px",
                    "left": "0px",
                    "background": "#FFFFFF",
                    "opacity": "0.40",
                    "width": $('body').width(),
                    "height":  $('body').height()
                }).prepend('<h1 style="text-align: center;">Please Wait...</h1>')
                );
                
            $.ajax({
                type: "GET",
                url: "?q=ntlp/admincourse/checkoutcomes/"+course_nid+"/"+selected_year,
                cache: false,
                success: function(data){
                    //                alert(data);
                    jresponse = Drupal.parseJson(data);

                    if (jresponse.warning == 0) {
                    
                        $.ajax({
                            type: "GET",
                            url: "?q=ntlp/admincourse/copy/"+course_nid+"/"+course_name+"/"+course_opt+"/"+selected_year+"/"+selected_term_tid,
                            cache: false,
                            success: function(data){
                                $('#plzWait').hide();
                                parent.Drupal.modalFrame.close();
                                parent.window.location.reload(true);                                      
                                                            
                            }
                        });
                    
                    } else  if (jresponse.warning == 1) {
                        msg = jresponse.message;
                 
                        $("#dialog-validate-outcome").dialog('option', 'buttons',{
                            'Yes': function() {
                                $.ajax({
                                    type: "GET",
                                    url: "?q=ntlp/admincourse/copy/"+course_nid+"/"+course_name+"/"+course_opt+"/"+selected_year+"/"+selected_term_tid,
                                    cache: false,
                                    success: function(data){
                                        $('#plzWait').hide();
                                        $(this).dialog('close');
                                        parent.Drupal.modalFrame.close();
                                        parent.window.location.reload(true);
                                                            
                                                            
                                    }
                                });
                            
                            },
                            'No': function() {
                                $('#plzWait').hide();
                                $(this).dialog('close');
                                parent.Drupal.modalFrame.close();
                    
                            }
                        });
                 
                    
                        $("#dialog-validate-outcome").find('.msg').css('color', '#000000').html(msg);
                        $("#dialog-validate-outcome").dialog('open');
                    } else if (jresponse.warning == 2) {
                        msg = jresponse.message;
                 
                        $("#dialog-validate-outcome").dialog('option', 'buttons',{
                            'Close': function() {
                                $('#plzWait').hide();
                                $(this).dialog('close');
                                parent.Drupal.modalFrame.close();
   
                            }
                        });
                 
                    
                        $("#dialog-validate-outcome").find('.msg').css('color', '#000000').html(msg);
                        $("#dialog-validate-outcome").dialog('open');
                    }
                }
            });
        }else{
            msg = 'Please select the term of the target school year.';
            $("#dialog-validate-outcome").dialog('option', 'buttons',{
                'Close': function() {
                    $(this).dialog('close');
                }
            });
                    
            $("#dialog-validate-outcome").find('.msg').css('color', '#000000').html(msg);
            $("#dialog-validate-outcome").dialog('open');
        }
    
    } else {
        msg = 'Please enter valid course name.';
        $("#dialog-validate-outcome").dialog('option', 'buttons',{
            'Close': function() {
                $(this).dialog('close');
            }
        });
                    
        $("#dialog-validate-outcome").find('.msg').css('color', '#000000').html(msg);
        $("#dialog-validate-outcome").dialog('open');
        
        
    }
    

}


//function select_semester(){
//
//    var ids = '';
//    var semester_id = $('input[name=semester[]]:checked').each(function(){
//
//        ids += $(this).val()+',';
//    });
//
//
//    $.ajax({
//        type: "GET",
//        url: "?q=ntlp/course/select/semester/"+ids,
//        success: function(data){
////                    alert(data);
//         $('#edit-course-term').html(data);
//
//         $('.newList').html(data);
//        }
//    });
//
//}

function call_delete_courseusers(course_nid, users) {
    msg = 'Removing selected students from this course, please wait...';
    $("#dialog-confirm-sbmtFrm").dialog('option', 'buttons',{
        'Ok':function(){
            $(this).dialog('close');
        }
    });

    $.ajax({
        type: "GET",
        url: "?q=ntlp/admincourse/confirmdeletion/"+course_nid+"/"+users,
        success:  function(data){
            if (typeof(data) == 'string') {
                jresponse = Drupal.parseJson(data);

                if (jresponse.success == 1) {
                    window.location.reload(true);
                } else {
                    msg = jresponse.message;
                    $("#dialog-confirm-sbmtFrm").find('.msg').css('color', '#000000').text(msg);
                    $("#dialog-confirm-sbmtFrm").dialog('open');
                }
            } else {
                msg = 'Invalid response received from server, please try again.';
                $("#dialog-confirm-sbmtFrm").find('.msg').css('color', '#000000').text(msg);
            }
        }
    });
    $("#dialog-confirm-sbmtFrm").find('.msg').css('color', '#000000').text(msg);
    $("#dialog-confirm-sbmtFrm").dialog('open');
}

function allow_self_enroll (course_id){
    
    var checkbox_state = $('#student_self_enroll').is(':checked');
    //alert (checkbox_state);
    var url = "?q=ntlp/admincourse/allow_self_enroll/"+course_id+"/"+(checkbox_state ? 1 : 0);
    //alert (url);
    
    $.ajax({
        type: "GET",
        url: url,
        success:  function(data){
            return true;
        },
        error: function() {
            $('#student_self_enroll').attr('checked') = !(checkbox_state);
            
            alert('Could not save Self Enrollment setting.  Please try again.');
        }
    });
   

}
//Called by People Finder - multi-select students
function self_enroll_student(course_name, course_nid, user_uid, enroll_in_role_id) {
  
    msg = 'You are about to sign up for course '+course_name+'. <br> Are you sure you want to do this? ';
    var get_url = "?q=ntlp/admincourse/self_enroll/"+course_nid+"/"+user_uid+"/"+enroll_in_role_id;
    
   
    $("#dialog-confirm-outcome").dialog('option', 'buttons',{
        'Continue':function(){
            
            $.ajax({
                type: "GET",
                url: get_url,
                cache: false,
                success: function(data){
                    
                    if (typeof(data) == 'string') {
                        jresponse = Drupal.parseJson(data);
                        $(this).dialog('close');
                        
                        msg = jresponse.message;
                        
                        init_dialog();
                        $("#dialog-confirm-outcome").dialog('option', 'buttons',{
                            'Close':function(){
                                $(this).dialog('close');
                                window.location.reload(true);
                            }
                        });
                        $("#dialog-confirm-outcome").find('.msg').css('color', '#000000').html(msg);
                        $("#dialog-confirm-outcome").dialog('open');
                    }
                }

            });
            
            
        },
        'Cancel':function(){
            $(this).dialog('close');
        }
    });
    $("#dialog-confirm-outcome").find('.msg').css('color', '#000000').html(msg);
    $("#dialog-confirm-outcome").dialog('open');
    
    
    
}
