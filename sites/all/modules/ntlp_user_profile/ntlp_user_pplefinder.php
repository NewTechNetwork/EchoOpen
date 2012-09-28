<script>
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

</script>
<?php
/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */




/////////////////////////////////// people finder starts here - ONLY FOR STUDENTS
//                if($user->uid == $useruid) { //check for own profile edit starts here
//                    if ($view_state == 'Edit Profile') {
//                        if(user_access("edit own profile(Student)")) {
//
//                $form['user_profile']['courses']['course_primary_instructor_heading'] = array(
//                        '#type' => 'item',
//                );
//                $form['user_profile']['courses']['course_primary_instructor_value'] = array(
//                        '#type' => 'item',
//                        '#value' => $profileObj->instructor_uid,
//                        '#prefix' => '<div id="primary_instructor_view">',
//                        '#suffix' => '</div><br>',
//                );
//
//
//                            $form['user_profile']['courses']['course_primary_instructor_id'] = array(
//                                    '#type' => 'hidden',
//                                    '#id' => 'course_primary_instructor_id',
//                                    '#value' => $profileObj->instructor_uid,
//                            );
//                            $school_nid = get_user_school($user->uid);
//                            $course_nid =1;
//                            $rec = get_course($course_nid);
//
//                            if (!empty($course_nid) && isset($rec)) {
//                                if ($rec != false) {
//                                    $course_primary_teacherid = 0;
//                                    $course_primary_teachername = '';
//                                    if ($rs_user = get_user($rec->teacher1_uid)) {
//                                        $course_primary_teacherid = $rec->teacher1_uid;
//                                        $course_primary_teachername = $rs_user->first_name.' '.$rs_user->last_name;
//                                    }
//                                }
//                            }
//
//                            $form['user_profile']['courses']['course_primary_instructor_pplfinder'] = array(
//                                    '#type' => 'item',
//                                    '#value' => dlg('Select Instructor', 'ntlp/people_finder/teacher_single/'.
//                                    $school_nid.'/'.
//                                    $course_nid.'/'.
//                                    $course_primary_teacherid.'/'.
//                                    NTLP_ROLEID_TEACHER,
//                                    350, 600),
//                                    '#prefix' => '<div class="peopleFinderBtn">',
//                                    '#suffix' => '</div>',
//                            );
//                        }
//                    }
//                }
/////////////////////////////////// people finder ends here
?>
