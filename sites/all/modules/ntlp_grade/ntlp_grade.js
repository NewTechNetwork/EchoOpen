// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


function update_announcement(course_nid, y, m, d) {
    var atext =  $('#edit-announcement-edit').val();
    parent.save_announcement(atext, course_nid, y, m, d);
}

function save_announcement(atext, course_nid, y, m, d) {
    $.ajax({
        type: "GET",
        url: "?q=ntlp/announcement/save/"+course_nid+"/"+y+"/"+m+"/"+d,
        data: "text="+atext,
        cache: false,
        success:  function(data) {
            $("#announcement_view").html(atext);
        }
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

function get_school_outcome_page(student_id, location){

    $.ajax({
        type: "GET",
        url: "?q=ntlp/grades/student_snapshot/outcome/"+student_id+"/"+location,
        //        data: "text="+atext,
        cache: false,
        success:  function(data) {
            $('h3.courses_snapshot_innerpanel_heading'+location+' div').html('Enrolled Courses');
            $('h3.d_courses_snapshot_innerpanel_heading'+location+' div').show();
            $('.courses_grade_link').css('border-bottom','2px solid #090');
            $("#course_"+location).show();
            $("#d_course_"+location).show();
            $("#show_"+location).html(data);
        }
    });
}
