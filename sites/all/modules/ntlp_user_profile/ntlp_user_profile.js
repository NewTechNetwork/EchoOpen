// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */


//Called by People Finder - single-select students
function on_singleselect_teacher(su_ids, su_names, enroll_in_role_id) {
    var tid = document.getElementById('course_primary_instructor_id');
    var tname = document.getElementById('primary_instructor_view');
    var myteacher = document.getElementById('myteachers');

    tid.value = su_ids;

    tname.innerHTML = su_names.replace("|", "<br />");
    alert('Primary Instructor selected');
    $.ajax({
        type: "GET",
        url: "?q=ntlp/admincourse/enroll_teacher/"+course_nid+"/"+enroll_in_role_id+"/"+su_ids,
        cache: false,
        success: function(data){
            //Set teacher name in predefined div section
            $("#primary_instructor_view").html(data);
            //Set teacher id in hidden variable
            var id = document.getElementById('course_primary_instructor_id');
            id.value = su_ids;
            alert('Instructor Selected Successfully');
        }

    });
}



function get_website_textfield() {
    var main = document.getElementById('ocContainer');
    var count = main.getElementsByTagName('li').length;
    if(count>3) {
        alert('you can add only 3 websites');
        return false;
    }
    count-=1;
    data2 = $('#oc_td2').val();
    data2 = data2.replace("%INDEX%", count);
    data2 = data2.replace("%INDEX%", count);

    document.getElementById("selected_oc_id").value += 'x,';
    document.getElementById("selected_oc_weight").value += '0,';

    var td2 = document.createElement('li');
    td2.setAttribute('class','url');
    td2.id = "outcome_"+(count);
    td2.innerHTML = data2;
    main = document.getElementById('ocLastRow');
    main.parentNode.insertBefore(td2, main);
}


function delete_func(n,index){
    alert(n.value + ' - '+ index);

}
function weight_func(n,index){
    //alert(n.value +' - '+index);
    var oc_id = document.getElementById('selected_oc_id');
    var oc_weight = document.getElementById('selected_oc_weight');
    var mywebsites = document.getElementById('mywebsites');
    var arrmywebsites = mywebsites.value.split("||");
    if(n.value=="")
        arrmywebsites[index]='x';
    else
        arrmywebsites[index]=n.value;
    var myvalues='';
    for( var i =0; i <(arrmywebsites.length-1); i++) {
        myvalues += arrmywebsites[i] +'||';
    }
    mywebsites.value = myvalues;
}


function on_imagechange(fileId, fileName) {
    //    alert('Inside user profile '+fileId + ":" + fileName);
    var uid = $('#user_id').val();
//    alert("user id "+uid + " file id "+ fileId);
    $.ajax({
        type: "GET",
        url: "?q=ntlp/save/user/profile/image/"+fileId+"/"+uid,
        cache: false,
        success:  function(data){
            $("#profile_image").html(data);
            Drupal.behaviors.modalFrameSetup();
        }

    });
    //    alert('image upload successfully');
    Drupal.modalFrame.close();
}

function on_fileupload_success(fileId, fileName){
//    alert('File Uploaded');
}

function web_notify_config(notify_id, user_uid, is_checked) {

    var exclude = (is_checked ? 1 : 0);

    $.ajax({
        type: "GET",
        url: "?q=ntlp/user/profile/webnotify/"+notify_id+"/"+user_uid+"/"+exclude,
        cache: false,
        success:  function(data){

        }
    });

    return true;
}


function email_notify_config(notify_id, user_uid, is_checked) {

    var exclude = (is_checked ? 1 : 0);

    $.ajax({
        type: "GET",
        url: "?q=ntlp/user/profile/emailnotify/"+notify_id+"/"+user_uid+"/"+exclude,
        cache: false,
        success:  function(data){

        }
    });

    return true;
}