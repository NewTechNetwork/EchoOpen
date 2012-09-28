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
                idsClicked[u] = u;
                $(elm).hide();
                idsClickedTrs['data_'+u] = elm;
            }
        }
        var element_count = 0;for (e in idsClickedTrs) {element_count++;}

        document.getElementById('selectedPplBtn').innerHTML = 'Selected ('+element_count+')';
    }

    update_selectability();

    $(".pfuidcb").live('click',function(){
        var uid = (this.id).split('_')[1];
        if( this.checked ) { // add uid
            
            if( whichPplTab == 'all' ) {
                if (idsClicked[uid] == undefined) {
                   idsClicked[uid] = uid;
                   idsClickedTrs['data_'+uid] = document.getElementById('chk_'+uid).parentNode.parentNode;
                    $(this.parentNode.parentNode).fadeOut();

                    if (document.getElementById('max_selected').value != -1)
                        document.getElementById('max_selected').value--;

                    update_selectability();
                }
            }
            
        } else {    //remove uid
            delete idsClicked[uid];
            if( whichPplTab == 'selected' ) {
                $(this.parentNode.parentNode).fadeOut();
            }
            delete idsClickedTrs['data_'+uid];

            if (document.getElementById('max_selected').value != -1)
                document.getElementById('max_selected').value++;

            update_selectability();
        }

        var element_count = 0;for (e in idsClickedTrs) {element_count++;}

        document.getElementById('selectedPplBtn').innerHTML = 'Selected ('+element_count+')';
    });
});

function update_selectability() {

    var max_selected = document.getElementById('max_selected').value;
    if (max_selected == -1)
        return;

    var disable = (max_selected <= 0);

    var x=document.getElementsByName("uid[]");

    for(i=0; i < x.length; i++){
        x[i].disabled = disable;
    }
}

function enable_all() {

    var x=document.getElementsByName("uid[]");

    for(i=0; i < x.length; i++){
        x[i].disabled = false;
    }
}

function get_selected_ids() {
         
    var selected = new Array();

    for (var uid in idsClicked) {
        
        if (uid != undefined && uid != null && typeof(uid) != 'undefined') {
            selected.push(uid);
        }
    }
    
    selected = selected.join(',');
    
    return selected;
}

function change_AllSelections() {

    var x=document.getElementsByName("uid[]");

    var max_selected = document.getElementById('max_selected').value;

    if( whichPplTab == 'all' ) {

        for(i=0; i < x.length; i++){

            if (!(x[i].checked) && (!$(x[i]).is(":hidden"))){

                if (max_selected != -1 && max_selected <= 0)
                    break;

                if (max_selected != -1) {
                    max_selected--;
                    document.getElementById('max_selected').value = max_selected;
                }


                var data = ""+x[i].id;
                data = data.substring(data.indexOf("_")+1);

                $(x[i].parentNode.parentNode).hide();
                idsClicked[data] = data;
                idsClickedTrs['data_'+data] = document.getElementById('chk_'+data).parentNode.parentNode;
                x[i].checked = true;
            }
        }
    }
    else if( whichPplTab == 'selected' ) {

        if( !(this.checked )) {

            for(i=0; i < x.length; i++){

                if (x[i].checked){

                    if (max_selected != -1) {
                        max_selected++;
                        document.getElementById('max_selected').value = max_selected;
                    }

                    var data = ""+x[i].id;
                    data = data.substring(data.indexOf("_")+1);

                    $(x[i].parentNode.parentNode).hide();
                    delete idsClicked[data];
                    delete idsClickedTrs['data_'+data];
                    x[i].checked = false;
                }
            }
        }
    }

    update_selectability();

    var element_count = 0;for (e in idsClickedTrs) {element_count++;}
    document.getElementById('selectedPplBtn').innerHTML = 'Selected ('+ element_count +')';
}

function onchange_schoolselector( sid ) {
    school_nid = sid;
    triggerFilter('school_nid', school_nid);
    //$("input[class^='myFilter']:first").trigger('onclick');
}

function show_all_results() {
    $('#peopleList').height(150);
    $('#pplFilter, #pplSearch').show();
    $('#showselectedBtn').removeClass('active');
    $('#showallresultBtn').addClass('active');

    whichPplTab = 'all';

    $('#pplf_user_list tr:gt(0)').show();

    for (var uid in idsClicked) {
        if (document.getElementById('chk_'+uid))
            $(document.getElementById('chk_'+uid).parentNode.parentNode).hide();

        $(idsClickedTrs['data_'+uid]).hide();
    }

    update_selectability();
    if (document.getElementById('chk_unchk_all') != null)
        document.getElementById('chk_unchk_all').checked = false;
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

    enable_all();
    if (document.getElementById('chk_unchk_all') != null)
        document.getElementById('chk_unchk_all').checked = true;
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
//   ntlp/people_finder/enrollments
//   ntlp/people_finder/guests
function user_search() {
    filter_name =  $('#finder_name').val();

    $('#pplf_user_list tr:gt(0)').show();

    for (var uid in idsClicked) {
        if (document.getElementById('chk_'+uid))
            $(document.getElementById('chk_'+uid).parentNode.parentNode).hide();

        $(idsClickedTrs['data_'+uid]).hide();
    }

    var x=document.getElementsByName("ppluser_name");

    for(i=0; i < x.length; i++){

       if (x[i].innerHTML.toLowerCase().indexOf(filter_name.toLowerCase(), 0) < 0) {
           $(x[i].parentNode).hide();
       }
    }
}

//action = This parameter can be used to add an identifier for the function you want to call
function selected_users(action, enroll_in_role_id, cache_id, disclude, discludeList){
    //    alert('selected_students called');
    //    alert(parent.test());
    var x=document.getElementsByName("uid[]");
    var su_ids = document.getElementById('selected_users_ids');
    var su_names = document.getElementById('selected_users_names');
    su_ids.value = '';
    su_names.value = '';

    var uids = new Array();
    var unames = new Array();

    for( var tr in idsClickedTrs ) {
        $('#pplf_user_list').append( idsClickedTrs[tr] );

        var data = ""+tr;
        data = data.substring(data.indexOf("_"));

        var id = document.getElementById('uid'+data).value;
        var name = document.getElementById('name'+data).value;
        uids.push(id);
        unames.push(name);
    }

    su_ids.value = uids.join(',');
    su_names.value = unames.join(',');

    if (disclude && su_ids.value.length > 0)
        discludeList = discludeList + ',' + su_ids.value;
    else
        discludeList = su_ids.value;

    if (cache_id != undefined) {
        $.ajax({
            type: "POST",
            url: '?q=ntlp/cache/set/' + cache_id,
            data: 'cache_data='+discludeList+ '&cache_table=ntlp_cache&expiry=CACHE_TEMPORARY'
        });
    }

    if (action == "teacher_single") {
        parent.on_singleselect_teacher(su_ids.value, su_names.value, enroll_in_role_id);
    } else if (action == "primaryteacher_multi") {
        parent.on_multiselect_primaryteachers(su_ids.value, su_names.value, cache_id);
    } else if (action == "teacher_multi") {
        parent.on_multiselect_teachers(su_ids.value, su_names.value, cache_id);
    } else if (action == "student_enrollment") {
        parent.on_multiselect_users(su_ids.value, su_names.value, enroll_in_role_id);
    }else if (action == "guest_enrollment") {
        parent.on_multiselect_users(su_ids.value, su_names.value, enroll_in_role_id);
    }else if (action == "multi_users") {
        parent.on_multiselect_users(su_ids.value, su_names.value);
    }else if (action == "enrollments") {
        parent.on_multiselect_users(su_ids.value, su_names.value, enroll_in_role_id);
    }else if (action == "get_rubric_users") {
        parent.get_rubric_users(su_ids.value, su_names.value, enroll_in_role_id);
    }else if (action == "single_student") {
        parent.on_singleselect_student(su_ids.value, su_names.value);
    }else if (action == "student_multi") {
        parent.on_multiselect_student(su_ids.value, su_names.value);
    }
    
    parent.Drupal.modalFrame.close();
}
