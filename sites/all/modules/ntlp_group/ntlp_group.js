// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.

//Called by People Finder - multi-select members
function on_multiselect_users(su_ids, su_names, enroll_in_role_id) {
    //    alert('on_multiselect_members called');
    //    var su_ids =  $('#selected_users_ids').val();

    var group_nid =  $('#group_id').val();

    $.ajax({
        type: "GET",
        url: "?q=ntlp/group/enroll/"+group_nid+"/"+enroll_in_role_id+"/"+su_ids,
        cache: false,
        success: function(data){
            window.location.reload(true);
        //              $("#group_views").html(data);
        //              Drupal.behaviors.fixMyPager('#group_views');
        //              alert('User(s) successfully added to the group');
        }

    });
}


function on_multiselect_teachers(su_ids, su_names) {

    var tid = document.getElementById('group_moderator_ids');
    var tname = document.getElementById('moderator_view');
    tid.value = su_ids;
    tname.innerHTML = su_names.replace("|", "<br />");
}

//Called by People Finder - multi-select members
function drop_member(group_nid, user_uid) {
    //         alert(user_uid);
    $.ajax({
        type: "GET",
        url: "?q=ntlp/group/drop/"+group_nid+"/"+user_uid,
        cache: false,
        success: function(data){
            window.location.reload(true);
        //            $("#group_views").html(data);
        //            Drupal.behaviors.fixMyPager('#group_views');
        //            alert('User Dropped Successfully');
        }
    });
}

//Called by Make Admin/Remove Admin buttons
function onchange_admin(group_nid, user_uid, is_admin) {
    //          alert(user_uid);
    $.ajax({
        type: "GET",
        url: "?q=ntlp/group/changeadmin/"+group_nid+"/"+user_uid+"/"+is_admin,
        cache: false,
        success: function(data){
            if (data.substr(0, 5) != 'ERROR') {
                window.location.reload(true);
            //                $("#group_views").html(data);
            //                Drupal.behaviors.fixMyPager('#group_views');
            //                alert('User status successfully changed');
            }
            else
                alert(data);
        }
    });
}

function onchange_rights(rdoOption) {
    $("input[name='group_dir_options']").attr('disabled', (rdoOption.value == 'O'));
}
 

function on_imagechange(fileId, fileName) {

 
    var group_id = $("#group_id").val();
 
    document.getElementById('file_fid').value = fileId ;
  
    $.ajax({
        type: "GET",
        url: "?q=ntlp/save/group/image/"+fileId+"/"+group_id,
        cache: false,
        success:  function(data){
            $("#group_image").html(data);
            Drupal.behaviors.modalFrameSetup();
        }

    });

    Drupal.modalFrame.close().closeDialog();
}

function on_fileupload_success(fileId, fileName) {
//     alert(fileId + ":" + fileName);
//
//
//    $('#project_image').html(fileName);
  

}



function create_group(school_id) {

    var group_title = $('#txt_group_title').val();
    //  var group_category = $('#edit-add-group-main-table-category').val();
    var school_moderator = $('#combo_school_moderator').val();
    //  var membership_type = $('#edit-add-group-main-table-membership-type').val();
    var group_perm = $('input:radio[name = "add_group[main_table][display_group_in_school_directory]"]:checked').val();

    var project_nid = $('#hidden_project_nid').val();

    
    if (school_moderator==undefined)
        school_moderator = "0";
    
    if(project_nid == undefined || project_nid == null){
        project_nid = 0;
    }

    $.ajax({
        type: "GET",
        url: '?q=ntlp/create/group/'+encodeURIComponent(group_title)+'/'+school_moderator+'/'+group_perm+'/'+project_nid,
        //        cache: false,
        success:  function(data){
            //            $('#groups_table', parent.document).html(data);
            //            Drupal.behaviors.fixMyPager($('#groups_table'));
            parent.Drupal.modalFrame.close();
            parent.window.location.reload();

        //            $("#groups_table").html(data);
        }

    });

}


function delete_group_cancel(){
    try {
        parent.Drupal.modalFrame.close().closeDialog();
    } catch (e) {

    }
}

 

function delete_group(group_id) {
 
    $.ajax({
        type: "GET",
        url: "?q=ntlp/group/delete/"+group_id,
        success:  function(data){

            try {
                parent.Drupal.modalFrame.close().closeDialog();
            } catch (e) {

            }
            parent.window.location = '?q=ntlp/groups/my';
        }

    });

   
    


//    parent.window.location = '?q=ntlp/groups/my';

}