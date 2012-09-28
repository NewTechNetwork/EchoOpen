//Returns courses in which the user is enrolled into
//role_in_course = The role of user in a course, defaults to "student"
//JOIN Note: Removing joins with OG tables, to reduce dependency
function get_user_courses($user_uid, $role_in_course = 0) {
    if ($role_in_course == 0)
        $role_in_course = NTLP_ROLEID_STUDENT;
    $rs = db_query("SELECT r.course_nid, n.title, c.course_short_title
            FROM {node} n
            INNER JOIN {ntlp_course} c ON n.nid = c.nid
            INNER JOIN {ntlp_course_user} r ON r.course_nid = c.nid
            WHERE c.is_project = 0 AND r.role_id = %d AND r.user_uid = %d",
            $course_nid, $role_in_course, $user_uid);
    return $rs;
}