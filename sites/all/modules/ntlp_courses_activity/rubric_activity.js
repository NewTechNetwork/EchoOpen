// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$(function(){
    $('input[name="slider_info\\[\\]"]').each(function(){
        //cat_id,min,max,step,value
        var config = this.value.split('|');
        
        $('#slider_'+config[0]).slider({
            value:parseInt(config[4]),
            min: parseInt(config[1]),
            max: parseInt(config[2]),
            step: parseInt(config[3]),
            
            slide: function(event, ui) {

                category_max = $('#maxpoints_'+config[0]).val();
                
                var maxScroll = $('#slider_'+config[0]).width();
                //                $('#slider_'+config[0]).attr({ scrollLeft: ui.value * (maxScroll / 80) });
                
                
                //$('#slider_value_'+config[0]).html(ui.value);
                var level_num = parseInt(ui.value / (100/total_levels));
                
                if (level_num == total_levels) level_num = total_levels-1;
                
                var slider_score = get_score(ui.value, level_num, total_levels, cat_levels[level_num], (level_num +1 < cat_levels.length) ? cat_levels[level_num+1] : 100, category_max);

                $('#slider_value_'+config[0]).html(slider_score);
                $(this).find('.ui-slider-handle').text(slider_score);
                $('#score_'+config[0]).val(slider_score);
            }
        });
        $('#slider_'+config[0]).find('.ui-slider-handle')
        .css('text-align', 'center')
        .text($('#slider_'+config[0]).slider("value"));

        for(var i=0; i<slider_ids.length; i++) {

            if (slider_ids[i] == config[0])
            {
                var scoreFound = false;
                for(var j=0; j<100; j++)
                {
                    $("#slider_" + slider_ids[i]).val(j);
                    //$("#slider_" + slider_ids[i]).slider(options);

                    var hs= $("#slider_" + slider_ids[i]).slider();
                    hs.slider('option', 'value',j);
                    hs.slider("option","slide").call(hs,null,{
                        handle: $('.ui-slider-handle', hs),
                        "value": j
                    });

                    if ($("#score_" + slider_ids[i]).val()== slider_scores[i]) {
                        scoreFound = true;
                        break;
                    }
                }

                if (!scoreFound) {

                    hs= $("#slider_" + slider_ids[i]).slider();
                    hs.slider('option', 'value',0);
                    hs.slider("option","slide").call(hs,null,{
                        handle: $('.ui-slider-handle', hs),
                        "value": 0
                    });

                }
            }
        }
                        
    });

    $("#dialog-confirm-user-delete").dialog({
        autoOpen: false,
        resizable: false,
        height:80,
        width:350,
        modal: true
    });
});

function get_score(slider_val, level_num, total_levels, current_level_percent, next_level_percent, max_score) {

    //alert('level_num: '+ level_num);
    //alert('global slider value: '+ slider_val);
    //convert the slider value from global to within a category
    slider_val = (slider_val - level_num * 100/total_levels) * total_levels;

    //alert('category slider value: '+ slider_val);
    //get the current category length
    var category_length = next_level_percent - current_level_percent;
    //alert('next_level_percent: '+ next_level_percent);
    //alert('current_level_percent: '+ current_level_percent);
    //alert('category_length: '+ category_length);
    //get the category percentage of the category's slider value
    var category_percent = ((slider_val * category_length)/100) + parseInt(current_level_percent);
    //alert('category_percent: '+ category_percent);
    var score = category_percent * max_score/100;

    //alert('score: '+ score);

    return Math.round(score);
}

/* this method is deleted and replaced with get_user_recieved_score_to_grade
 * Jan 28 Saifuddin
 * 
function get_average_score(max_points, feedback_points, outcome){
    var total_percent = 0;

    var selected_outcome =0;
    //    var outcomes = '';

    selected_outcome = $('#selected_outcome').val();
    
    total_percent = parseFloat(feedback_points) * 100 / parseFloat(max_points);
    total_percent = isNaN(total_percent) ? 0 : total_percent;
    
    var  a = total_percent / 100;
    
    var calculate_percent = Math.round(a * parseFloat(selected_outcome));


    $('#outcome_'+outcome).val(calculate_percent);
    $('#outcome_'+outcome).focus();

    
}*/

function auto_populate_all_grades(activity_nid){
    var total_percent = 0;

    var max_points = outcome_max_points;
    var calculated_points = new Array();

    for(var i=0; i<user_ids.length; i++) {

        //do not populate grades for inc marked activities
        if ($('input[name=outcome['+user_ids[i]+']]').is(':checked'))
            continue;

        var points = feedback_points[i];

        total_percent = parseFloat(points) * 100 / parseFloat(max_rubric_score);
        total_percent = isNaN(total_percent) ? 0 : total_percent;

        var  a = total_percent / 100;

        var calculate_percent = Math.round(a * parseFloat(max_points));

        $('input[id='+user_ids[i]+'-'+primary_outcome_id+']').val(calculate_percent);

        calculated_points.push(calculate_percent);
    }

    var users = "[" + user_ids.join(",") + "]";
    var scores = "[" + calculated_points.join(",") + "]";


    var get_url = "?q=ntlp/activity/autogradeall/ajax/"+activity_nid+"/"+users+"/"+scores+"/"+primary_outcome_id;

    $.ajax({
        type: "GET",
        url: get_url,
        cache: false,
        success: function(data){
            $('div[name=last_saved_date]').text(data);
            $('div[name=last_saved_date]').attr("style", "padding: 5px; display: block; background-color: #ddd; border: 1px solid #ddd; float: left; font-size: 12px; color: rgb(0, 0, 0);");
        },
        error: function() {
        }
    });
}

function clear_all_grades(activity_nid) {

    for(var i=0; i<user_ids.length; i++) {

        //do not clear grades for inc marked activities
        if ($('input[name=outcome['+user_ids[i]+']]').is(':checked'))
            continue;

        $('input[name=disabled_outcome['+user_ids[i]+']]').val('');
    }

    var users = "[" + user_ids.join(",") + "]";


    var get_url = "?q=ntlp/activity/clearall/ajax/"+activity_nid+"/"+users;

    $.ajax({
        type: "GET",
        url: get_url,
        cache: false,
        success: function(data){
            $('div[name=last_saved_date]').text(data);
            $('div[name=last_saved_date]').attr("style", "padding: 5px; display: block; background-color: #ddd; border: 1px solid #ddd; float: left; font-size: 12px; color: rgb(0, 0, 0);");
        },
        error: function() {
        }
    });
}

function delete_rubric_user(course_nid, activity_nid, evaluated_user, given_by_user){
    //
    //url('ntlp/delete/rubric/user/' . $activityObj->course_nid . '/' . $activityObj->nid . '/' . $course_student->user_uid . '/' . $user_uid)
    var candelete_obj = document.getElementById('txt_'+evaluated_user);

    if (candelete_obj.value == 1) {
        
        $("#dialog-confirm-user-delete").dialog('option', 'buttons',{
            'Yes': function() {

                $.ajax({
                    type: "GET",
                    url: "?q=ntlp/delete/rubric/user/"+course_nid+"/"+activity_nid+"/"+evaluated_user+"/"+given_by_user,
                    
                    success:  function(){
                        window.location.reload();

                    }

                });
                
                $(this).dialog('close');
            },
            'Cancel': function() {
                $(this).dialog('close');
            }

        });
        $("#dialog-confirm-user-delete").find('.msg').css('color', '#000000').text("You already submitted feedback scores for this student. Removing the student will also remove all your scores and comments.  Are you sure you want to remove this student?");
        $("#dialog-confirm-user-delete").dialog('open');

        

    }else if(candelete_obj.value == 0){
        $.ajax({
            type: "GET",
            url: "?q=ntlp/delete/rubric/user/"+course_nid+"/"+activity_nid+"/"+evaluated_user+"/"+given_by_user,
            success:  function(){
                window.location.reload();
            }
        });

    }

}

function get_user_recieved_score_to_grade(outcome){


    var total_percent = 0;

    var max_points = $('#max_category_scores').val();
    var selected_outcome = $('#selected_outcome').val();

    var recieved_scored = 0;
    var submission_count = 0;

    var recieved_evaluation_count = $('input[name=user_received_sub_nid[]]').length;

    if(recieved_evaluation_count > 0){
        $('input[name=user_received_sub_nid[]]').each(function(){
            submission_nid = $(this)[0].id;

            flag = $('input[name=exclude_entry['+submission_nid+']]').is(':checked');
            if(!flag){
                submission_count += 1;
                recieved_scored += parseInt($(this).val());
            }
        });

        var feedback_points = Math.round((recieved_scored / submission_count));

        total_percent = parseFloat(feedback_points) * 100 / parseFloat(max_points);
        total_percent = isNaN(total_percent) ? 0 : total_percent;

        var  a = total_percent / 100;

        var calculate_percent = Math.round(a * parseFloat(selected_outcome));
    }else{
        var note ='A grade cannot be calculated, because the student has not received any evaluations yet.';


        $("#dialog-show-confirmation").dialog('option', 'buttons',{
            'Close': function() {
                $(this).dialog('close');
            }
        });

        $("#dialog-show-confirmation").find('.msg').css('color', '#000000').text(note);
        $("#dialog-show-confirmation").dialog("open");
        calculate_percent = '';
    }
    $('#outcome_'+outcome).val(calculate_percent);
    $('#outcome_'+outcome).focus();
}