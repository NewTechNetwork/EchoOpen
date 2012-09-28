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


function student_search(keyCode) {
    var name =  $('#finder_name').val();
    var courseid =  $('#finder_course_id').val();
    name += String.fromCharCode(keyCode);

    $.ajax({
        type: "GET",
        url: "?q=ntlp_student_finder/search/"+courseid,
        data: "name="+name,
        success:  function(data){
            $("#searchstudent").html(data);

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

/*function alert2010(){
//    alert($('#doc_link').val());
    var data = $('#doc_link').val();
//    $('#urllist').html(data);

    document.getElementById("urllist").innerHTML += '<div>'+data+' <a href=# >Delete</a></div>';
    document.getElementById("urldata").innerHTML += '<input type="hidden" value="'+data+'" />';

}*/


function selected_students(){
//    alert('selected_students called');
    var x=document.getElementsByName("uid[]");
    var su_ids = document.getElementById('selected_users_ids');
    var su_names = document.getElementById('selected_users_names');
    su_ids.value = '';
    su_names.value = '';

    for(i=0; i < x.length; i++){
        var data = ""+x[i].id;
        data = data.substring(data.indexOf("_"));
//        alert(data);
        if(x[i].checked){
            var id = document.getElementById('uid'+data).value;
            var name = document.getElementById('name'+data).value;
            su_ids.value += id+',';
            su_names.value += name+',';
        }
    }
    on_multiselect_students(su_ids.value, su_names.value);
}

