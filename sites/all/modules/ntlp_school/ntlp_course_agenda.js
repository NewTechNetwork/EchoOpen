// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

$(function(){
    if( $('.showPrintUser').click){
        $('.showPrintUser').click(function(){
            $('.message-close').trigger('click');
        });
    }
});


function GetXmlHttpObject()
{
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



//function func(id){
//
//    alert(id);
//    $('agenda').datepicker();
//
//    //  Starting Date ID  For Course Setting Form
//    $('#edit-sdate').datepicker();
//
//    //  Ending Date ID For Course Setting Form
//    $('#edit-edate').datepicker();
//
//
//}


if(Drupal.jsEnabled)
{

    $(document).ready(function() {

        // Parent field & respective event
        // Adding change event to Hot Spot Field


        $('.agenda_date').click(function(event) {


            var agendaid = this.name;
            //    alert(agendaid);

            //            alert(agendaid);
            //  alert('inside fucntion '+formid);
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
                $('#agendabody').html(data);
            }

            //AJAX call
            // URL: ActiveField/field-trip     - maps to a Drupal function
            // Parameters: null for now.
            // Call back function:  "frmDrupal"

            //The URL (i.e. the server side resource to be called)
            //needs to be unique to the module

            $.get('?q=ntlp_course_agenda/get_agenda_body', {
                'agendid': agendaid
            }, frmDrupal);
            // preventing entire page from reloading
            return false;
        });




    });

}


// Primary Instructor Function For Course Setting
function PrimaryInstructor(){

    window.open("ntlp_course/form_load", "pinstructor", "width=320,height=210,scrollbars=yes");

}


// Additional Instructor Function For Course Setting
function AdditionalInstructor(){

    window.open("", "Ainstructor", "width=320,height=210,scrollbars=yes");

}


// Shows all students in school management new user form
function on_multiselect_student(su_ids, su_names) {
    var tid = document.getElementById('student_ids');
    var tname = document.getElementById('student_names');
    tid.value = su_ids;
    tname.innerHTML = su_names.replace("|", "<br />");
}

function get_courses_csv(is_jasper){
    var course_id = $("#edit-main1-main-courses").val();
    var school_id = $("#school_nid").val();
    var term_id = $("#edit-main1-main-term-name").val();
//    var header = $('input[name=main1[main][column_header]]:checked').val();
    var header = 1;
    if (is_jasper) {
        header = 0;
    }
    var url = "?q=ntlp/school/management/export/"+course_id+"/"+school_id+"/"+header+"/"+term_id;
    alert(url);
    window.location = url;

}

function get_schoolterm_grade_pdf(iReport){

    var school_id = $("#school_nid").val();
    var school_year = $('#edit-main1-main2-school-year').val();
    var term_id = $('#edit-main1-main2-term-id').val();
    if (term_id) {
//        alert(school_id + "/" + term_id + "/" + school_year);
        window.open("?q=ntlp/school/management/export_stgrade/"+school_id+"/"+term_id+"/"+school_year+"/"+iReport);
    } else {
        alert('Please select a valid term first');
    }
}

function get_school_user_list_csv(school_id){
    window.location = "?q=ntlp/school/management/user/list/"+school_id;
}

function get_term_courses(){

    var term_id = $("#edit-main1-main-term-name").val();
    var school_id = $("#school_nid").val();

    $.ajax({
        type: "GET",
        url: "?q=ntlp/school/management/terms/"+term_id+"/"+school_id,
        success:  function(data){
            $('#edit-main1-main-courses').html(data);
        }
    });

}



function _get_csv_record(){

    var file_id  = document.getElementById('file_info_id').value;
    var file_path  = document.getElementById('file_info_path').value;
        var update_record  = $('input[name=main[update_existing_account]]:checked').val();

    var update_record = document.getElementById('user_check').checked;
    var change_password = document.getElementById('change_password').checked;
    
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
        "?q=ntlp/school/management/csv/"+file_id+"/"+update_record+"/"+change_password,
        {},
        function(data){
            var $statusUL = $('<ul></ul>');
            if(data.exists) {
                for(var i = 0; i < data.exists.length; i++) {
                    $statusUL.append($('<li></li>').html(data.exists[i] + ' already exists.'));
                }
            }

            if(data.error) {
                for(var i = 0; i < data.error.length; i++) {
                    $statusUL.append($('<li></li>').html(data.error[i] + ' was created.'));
                }
            }

            var statusColor = '#FC807D'; // error color
            if( !data.error && !data.exists ) {
                $statusUL.append($('<li></li>').html('Users '+(update_record == 1 ? 'updated' : 'created')+' successfuly.'));
                statusColor = '#9AF49A'; // success color
            }

            $('#bulkUserStatus').empty();
            $('#bulkUserStatus').css('background', statusColor).append($statusUL);
            $('#bulkUserStatus').fadeIn();
        },'json'
    );


}

