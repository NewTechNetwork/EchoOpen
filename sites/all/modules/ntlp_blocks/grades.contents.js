// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


var submission_grade_tid_InProgress = false;
var gradeall_timer = null;

$(document).ready(function() {
    //Execute pager jquery again to reset pager
    //    alert($('#school_grade_students').attr('id'));

     $("#dialog-show-confirmation").dialog({
        autoOpen: false,
        resizable: false,
        height:140,
        modal: true
    });
    
    if ($('#school_grade_students').length) {
        //        alert('school_grade_students');
        Drupal.behaviors.changeHeaderLinks($('#school_grade_students'), 'ntlp/courses/grade/', 'ntlp/gradebook/grade/filter/');
        Drupal.behaviors.fixMyPager($('#school_grade_students'));
    }
    if ($('#school_stugrade_activities').length) {
        //        alert('school_stugrade_activities');
        Drupal.behaviors.changeHeaderLinks($('#school_stugrade_activities'), 'ntlp/courses/grade/', 'ntlp/gradebook/stugrade/filter/');
        Drupal.behaviors.fixMyPager($('#school_stugrade_activities'));
    }

});

function on_save_student_grade_all(student_uid, course_nid, term_tid) {
    clear_autosave();

    $.ajax({
        type: "GET",
        url: '?q=ntlp/grade/student/autosave/' + student_uid + "/" + course_nid + "/" + term_tid,
        cache: false,
        success: function(data){

            $('div[name=last_saved_date]').text(data);
            $('div[name=last_saved_date]').attr("style", "padding: 5px; display: block; background-color: #ddd; border: 1px solid #ddd; float: left; font-size: 12px; color: rgb(0, 0, 0);");

        }
    });
}

var prev_outcome_scores = new Array();

function save_previous_score(activity_nid, outcome_tid) {

    prev_outcome_scores[activity_nid] = new Array();
    prev_outcome_scores[activity_nid][outcome_tid] = $('input[name="outcome_scores[' + activity_nid + '][' + outcome_tid + ']"]').val();
}

function submit_grade(activity_nid, activity_user, max_points, outcome_tid, myinput) {
    var flag = true;
    var div_name = $('input[name="outcome_scores[' + activity_nid + '][' + outcome_tid + ']"]');
    var user_input =  div_name.val();
//    alert(div_name+":"+user_input);
    //        alert('userid '+activity_user+' activity_nid '+activity_nid);
    //
    user_input = user_input.trim();

    if (prev_outcome_scores[activity_nid][outcome_tid] == user_input)
        return;
    
    if(user_input==""){
        user_input="-";
    } else if (user_input.toUpperCase()!="EX") {
        if(isNaN(user_input)) {
            alert('Grade must be a number');
            div_name.val(myinput);
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

                div_name.focus();
                div_name.val('');
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
                        div_name.focus();
                        div_name.val('');
                        flag = false;
                    }else{
                        flag = true;
                    }
                }else{
                    flag = true;
                }
            }
        }
    }else{

    }
    if(user_input<0) {
        alert('Grade cannot be in minus');
        div_name.val(myinput);
        flag = false;
        return false;
    }
    if(flag){
        init_autosave();

        var get_url = "?q=ntlp/activity/gradeall/ajax/"+activity_nid+"/"+activity_user+"/"+outcome_tid+"/"+user_input;
        submission_grade_tid_InProgress = true;

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
                submission_grade_tid_InProgress = false;
            },
            error: function() {
                submission_grade_tid_InProgress = false;
            }
        });
    }
}

function submission_late_tid(activity_user, activity_id) {
    init_autosave();

    //Get checkbox value
    var checked = $('input[name=late['+activity_id+']]').is(':checked');

    //Set/Unset late class in textboxes
    if(checked == true){
        $('td[name=activity['+activity_id+']]').each(function(){
            $(this).addClass("late");
        });
    }else{
        $('td[name=activity['+activity_id+']]').each(function(){
            $(this).removeClass("late");
        });
    }
    var checked_val = (checked)? 1 : 0;
    var get_url = "?q=ntlp/activity/gradeall/ajax_late/"+activity_id+"/"+activity_user+"/"+checked_val;
    submission_grade_tid_InProgress = true;
    
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
            submission_grade_tid_InProgress = false;
        },
        error: function() {
            submission_grade_tid_InProgress = false;
        }
    });
}

function submission_inc_tid(user_id, activity_id){
    init_autosave();

    //Get checkbox value
    var grading_method = $('input[name=activites_nid['+activity_id+']]').val();

    var a = $('input[name=inc['+activity_id+']]').is(':checked');

    //Set/Unset disabled attribute in textboxes
    if(a == true){

        $('input[id=score_outcome_'+activity_id+']').each(function(){
            $(this).val(0);
            $(this).attr("disabled", "disabled");
        });
        $('td[name=activity['+activity_id+']]').each(function(){
            $(this).addClass("incomplete");
        });
    }else{

        $('input[id=score_outcome_'+activity_id+']').each(function(){
            $(this).removeAttr("disabled");
        });

        $('td[name=activity['+activity_id+']]').each(function(){
            $(this).removeClass("incomplete");
        });

        if(grading_method == 1){

            $('input[name="activity_outcome['+activity_id+']"]').each(function(){
                var outcome_tid = $(this)[0].id;
                var max_points = $(this).val();
                $('input[id="score_outcome_'+activity_id+'"]').each(function(){
                    var str = $(this)[0].name;
                    str = str.substring(str.indexOf("]")+1, str.length-1);
                    ot_tid = str.substring(str.indexOf("[")+1, str.length);
                    if(outcome_tid == ot_tid){
                        $(this).val(max_points);
                    }
                });

            });
        }
    }

    var checked_val = (a)? 1 : 0;
    var get_url = "?q=ntlp/activity/gradeall/ajax_inc/"+activity_id+"/"+user_id+"/"+checked_val;
    submission_grade_tid_InProgress = true;
    
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
            submission_grade_tid_InProgress = false;
        },
        error: function() {
            submission_grade_tid_InProgress = false;
        }
    });
}

function get_course_students_view(course_id, term_id){
    $.ajax({
        type: "GET",
        url: "?q=ntlp/gradebook/student_view/"+course_id+"/"+term_id+"/0/0",
        cache: false,
        data:"block=none",
        success: function(data){
            $("#gradebook_tab").html(data);
            $("#grades_tab_students").attr("class", "GradesTabActive");
            $("#grades_tab_activities").attr("class", "")
            Drupal.behaviors.fixMyPager($('#school_grade_students'));
        },
        error: function(data) {
            alert('error:'+data);
        }
        
    });

}

function get_course_activities_view(course_id, term_id){
    //gradebook_tab

    $.ajax({
        type: "GET",
        url: "?q=ntlp/gradebook/activities_view/"+course_id+"/"+term_id,
        cache: false,
        success: function(data){
            $("#gradebook_tab").html(data);
            $("#grades_tab_students").attr("class", "");
            $("#grades_tab_activities").attr("class", "GradesTabActive");
        }
    });
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
            }else{
                flag = true;
            }
        }
    }
    return flag;

}

String.prototype.trim = function() {
    return this.replace(/^\s+|\s+$/g,"");
}

function init_autosave() {
    if (gradeall_timer == null) {
        var student_uid = $("#edit-student-id").val();
        var course_nid = $("#edit-course-id").val();
        var term_tid = $("#edit-school-term-id").val();
        gradeall_timer = setTimeout("autosave_student_grades("+student_uid + "," + course_nid + "," + term_tid+")", 30*1*1000);
    }
}
function clear_autosave() {
    clearTimeout(gradeall_timer);
    gradeall_timer = null;
}

function autosave_student_grades(student_uid, course_nid, term_tid) {
    if (submission_grade_tid_InProgress)
        return;

    $('div[name=last_saved_date]').text("Auto Saving...");
    $('.SaveBtn').hide();
    $('.SaveBtnDisabled').show();

    //Call the Save function
    on_save_student_grade_all(student_uid, course_nid, term_tid);

    $('.SaveBtn').show();
    $('.SaveBtnDisabled').hide();
}