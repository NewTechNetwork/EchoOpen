// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


// $Id: modalframe_setup.js,v 1.1.2.7 2009/12/28 02:21:20 markuspetrux Exp $

function change_course_term(course_nid, school_term_tid){
    //
    var get_url = "?q=ntlp/course/term/change/"+course_nid+"/"+school_term_tid;
    //alert(get_url);
    $.ajax({
        type: "GET",
        url: get_url,
        cache: false,
        success: function(data){
            $("#course_term").html(data);
        //            alert('Comment Posted');
        }

    });

    alert("Term Updated Successfully");
    hidePopup();
}

$(document).ready(function() {
//    $('#tools-school-selector .my-dropdown').sSelect();
    setupMenuButton('manageCourse', "btnManageCourse");
    setupMenuButton('studentCourseReport', "btnReports");
});

var schoolselector = {
    current_value : 0,
    subscribers : []
}

var schoolselector_subscribers = {
    objectname : null,
    fn : null
}

function subscribe_schoolselector(objectname, fn) {
    var ns = new schoolselector_subscribers;
    ns.objectname = objectname;
    ns.fn = fn;
    schoolselector.subscribers[schoolselector.subscribers.length] = ns;
}

function onchange_schoolselector(nid) {
    $.ajax({
        type: "GET",
        url: "?q=ntlp/blocks/changeschool/"+nid,
        cache: false,
        success: function(data){
            if (data == "OK")
                window.location.reload();
            else
                alert("You do not have access to this school.");
        }
    });
}
$(document).ready (function () {
  var title =  $('.view-filters input.form-text').attr('title');
  $('.view-filters input.form-text').val(title);

  $('.view-filters input.form-text').focus(function () {
      $('.view-filters input.form-text').val('');
  });
  $('.view-filters input.form-text').blur(function () {

        if ($('.view-filters input.form-text').val()=='') {
        var title =  $('.view-filters input.form-text').attr('title');
        $('.view-filters input.form-text').val(title);
        }
  });

});

function school_year_handler(tab_name, obj) {
    
    $.get('?q=ntlp/courses/school_year/selector/'+tab_name+'/'+obj.value,function(data){
        $('#coursesTabsContentWrapper').html(data)
    });
}