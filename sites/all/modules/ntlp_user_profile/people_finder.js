// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


//var school_nid = 0;
var idsClicked = new Array();
var idsClickedTrs = {};
var whichPplTab = 'all';
var filter_name = '';
$(function(){
    if( typeof(alreadySelectedList) != 'undefined' && alreadySelectedList.length > 0 ) {
        for( i = 0; i < alreadySelectedList.length; i++ ) {
            var u = alreadySelectedList[i];
            if( document.getElementById('chk_'+u) ) {
                var elm = document.getElementById('chk_'+u).parentNode.parentNode;
                document.getElementById('chk_'+u).checked = true;
                idsClicked.push(u);
                $(elm).hide();
                idsClickedTrs['data_'+u] = elm;
            }
        }

        document.getElementById('selectedPplBtn').innerHTML = 'Selected ('+idsClicked.length+')';
    }

    $(".pfuidcb").live('click',function(){
        var uid = (this.id).split('_')[1];
        var uidIndex = $.inArray( uid, idsClicked );
        if( this.checked ) { // add uid
            if( uidIndex == -1 ) {
                idsClicked.push(uid);
                idsClickedTrs['data_'+uid] = document.getElementById('chk_'+uid).parentNode.parentNode;
                if( whichPplTab == 'all' ) {
                    $( idsClickedTrs['data_'+uid] ).fadeOut();
                }
            }
        } else {    //remove uid
            if( uidIndex > -1 ) {
                idsClicked.splice(uidIndex, 1);
                if( whichPplTab == 'selected' ) {
                    $(idsClickedTrs['data_'+uid]).fadeOut();
                }
                delete idsClickedTrs['data_'+uid];
            }
        }

        document.getElementById('selectedPplBtn').innerHTML = 'Selected ('+idsClicked.length+')';
    });
});

function onchange_schoolselector( sid ) {
    school_nid = sid;
    show_all_results();
}

function show_all_results() {
    $('#peopleList').height(150);
    $('#pplFilter, #pplSearch').show();
    $('#showselectedBtn').removeClass('active');
    $('#showallresultBtn').addClass('active');

    whichPplTab = 'all';
    $("input[class^='myFilter']:first").trigger('onclick');
}

function show_only_checked() {
    $('#peopleList').height( $('#peopleList').height() + $('#pplFilter').height() + $('#pplSearch').height() );
    
    $('#pplFilter, #pplSearch').hide();
    $('#showselectedBtn').addClass('active');
    $('#showallresultBtn').removeClass('active');

    whichPplTab = 'selected';
    $('#pplf_user_list tr:gt(0)').hide();
    for( var tr in idsClickedTrs ) {
        $( idsClickedTrs[tr] ).show();
        $('#pplf_user_list').append( idsClickedTrs[tr] ).show();
    }
}

function people_search() {

    var name =  $('#finder').val();

    $.ajax({
        type: "GET",
        url: "?q=ntlp_people_finder/search",
        data: "name="+name,
        success:  function(data){
            $("#searchp").html(data);
        }
    });
}


function search_single_record() {

    var name =  $('#finder').val();

    $.ajax({
        type: "GET",
        url: "?q=single_record/search",
        data: "name="+name,
        success:  function(data){
            $("#searchstudent").html(data);
        }
    });
}


function single_student_search(obj){

    y = obj.id;
    x = y.split(',');

    var userid = x[0];
    var name = x[1];

    document.getElementById("records").innerHTML = userid+'&nbsp;&nbsp;&nbsp;';
    document.getElementById("records").innerHTML += name;
    var id =  document.getElementById("userid").value = x[0];

}


function people_search1() {

    var name =  $('#finder').val();

    $.ajax({
        type: "GET",
        url: "?q=ntlp_people_finder/people_record",
        data: "name="+name,
        success:  function(data){
            $("#popups-body").html(data);

        }

    });
}


//Used by menu callbacks:
function user_search(keyCode, school_nid, course_nid, users, role_id, is_multi) {
    filter_name =  $('#finder_name').val();
    $("input[class^='myFilter']:first").trigger('onclick');
}

function selected_users(action, enroll_in_role_id){
    var x=document.getElementsByName("uid[]");
    var su_ids = document.getElementById('selected_users_ids');
    var su_names = document.getElementById('selected_users_names');
    su_ids.value = '';
    su_names.value = '';

    for(i=0; i < x.length; i++){
        var data = ""+x[i].id;
        data = data.substring(data.indexOf("_"));
        if(x[i].checked){
            var id = document.getElementById('uid'+data).value;
            var name = document.getElementById('name'+data).value;
            su_ids.value += ','+id;
            su_names.value += ','+name;
        }
    }

    if (su_ids.value.length > 1)
        su_ids.value = su_ids.value.substring(1);
    if (su_names.value.length > 1)
        su_names.value = su_names.value.substring(1);
    
    if (action == "teacher_single") {
        parent.on_singleselect_teacher(su_ids.value, su_names.value, enroll_in_role_id);
    } else if (action == "teacher_multi") {
        parent.on_multiselect_teachers(su_ids.value, su_names.value, enroll_in_role_id);
    } else if (action == "student_enrollment") {
        parent.on_multiselect_users(su_ids.value, su_names.value, enroll_in_role_id);
    }else if (action == "guest_enrollment") {
        parent.on_multiselect_users(su_ids.value, su_names.value, enroll_in_role_id);
    }else if (action == "enrollments") {
        parent.on_multiselect_users(su_ids.value, su_names.value, enroll_in_role_id);
    }
    try {
        parent.Drupal.modalFrame.close().closeDialog();
    } catch (e) {

    }
}
