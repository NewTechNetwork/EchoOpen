// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


function update_announcement(course_nid, y, m, d, state) {
    var atext =  $('#edit-announcement-edit').val();
    if(atext.length == 0){
        atext = "";
    }
    parent.save_announcement(atext, course_nid, y, m, d, state);   
}

function save_announcement(atext, course_nid, y, m, d, state) {

    $.ajax({
        type: "GET",
        url: "?q=ntlp/announcement/save/"+course_nid+"/"+y+"/"+m+"/"+d+"/"+state,
        data: "text="+atext,
        cache: false,
        success:  function(data) {
            if (atext.length <= 0)
                atext = "No Announcements";
          $("#announcement_view").html(atext);
        }
    });

    try {
        Drupal.modalFrame.close().closeDialog();
    } catch (e) {

    }
}


