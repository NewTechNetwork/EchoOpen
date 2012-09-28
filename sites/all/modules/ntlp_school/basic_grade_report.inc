<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.

  
    module_load_include('inc', 'ntlp_school', 'data_access');
    module_load_include('inc', 'ntlp_grade', 'data_access');

    function get_dw_student_course_grades($student_uid, $school_term_id)
    {
        $rs = db_query("SELECT c.nid, c.course_short_title, c.teacher1_uid, dw.period, dw.grade_letter, dw.grade_percent 
                        FROM {ntlp_dw_course_grade} dw
                        INNER JOIN {ntlp_course} c ON c.nid=dw.course_nid
                        WHERE dw.school_term_tid = %d AND dw.is_enrolled = 1
                        AND uid = %d", $school_term_id, $student_uid);

        return $rs;
    }
    
    function get_course_info($course_nid)
    {
        $rs = db_query("SELECT n.title, c.nid, c.course_short_title, c.teacher1_uid, c.teacher2_uid
                        FROM {ntlp_course} c, {node} n
                        WHERE c.nid = %d AND n.nid = c.nid ", $course_nid);


        while($course_info = db_fetch_object($rs) ) 
        {
            return $course_info;
        }

        return $course_info;
    }
    

    function get_dw_student_term_outcome($student_uid, $school_term_id)
    {
        $rs = db_query("SELECT dw.*
            FROM dpl_ntlp_dw_course_grade dw
            INNER JOIN dpl_ntlp_course c ON c.nid = dw.course_nid
            WHERE dw.school_term_tid = %d AND dw.is_enrolled = 1
            AND dw.uid = %d" , $school_term_id, $student_uid);

        return $rs;
    }

                    
    function get_outcome_name($outcome_id)
    {
        $sql = 'SELECT distinct outcome.tid, outcome.name
                FROM {term_data} outcome 
                WHERE outcome.tid=%d';

        $rs = db_query($sql, $outcome_id);

        while( $name = db_fetch_object($rs) ) 
        {
            return $name->name;
        }

        return $name;
    }
    

    function get_outcomes($school_nid, $school_year)
    {
        $sql = 'SELECT distinct outcome.name
                FROM {term_data} outcome 
                INNER JOIN {ntlp_gb_outcome} gb_outcome ON gb_outcome.tid = outcome.tid
                WHERE gb_outcome.school_nid = %d AND gb_outcome.school_year = %d
                ORDER BY outcome.name';

        $rs = db_query($sql, $school_nid, $school_year);

        $rows = array();
        while( $row = db_fetch_object($rs) ) 
        {
            $rows[] = $row;
        }

        return $rows;
    }
    
  function get_school_students($school_nid, $student_id = null) {

    $rs = db_query("SELECT u.uid, u.first_name, u.middle_name, u.last_name, u.grad_year, r.name
            FROM {ntlp_user} u
            INNER JOIN {users_roles} ur ON ur.uid = u.uid
            INNER JOIN {role} r ON r.rid = ur.rid
            WHERE r.rid = 5
            AND u.school_nid = %d" . 
            (($student_id <> null) ? " AND u.uid = %d " : "") . 
            " ORDER BY u.last_name, u.first_name", $school_nid, $student_id);

    return $rs;

    }
   
   function get_course_teacher_ids($course_nid) {
    $course_nid = (int)$course_nid;

    $rs = db_query("SELECT u.uid
            FROM {ntlp_course_user} r
            INNER JOIN {ntlp_user} u ON r.user_uid = u.uid
            INNER JOIN {users} nu ON nu.uid = u.uid
            WHERE r.course_nid = %d
            AND u.is_active = 1 AND r.role_id = 6 ",
            $course_nid);
    return $rs;
}

  
function grade_report_export($school_nid, $school_year, $selected_term_id, $print_page, $course_id = null, $student_id = null) {
    $school_students = get_school_students($school_nid, $student_id);
    $school = get_school_record($school_nid);

    $term_name = get_school_term_name($selected_term_id)->name;

//    $school_oc = get_outcomes($school_nid, $school->current_school_year);

    $html = getHeaderContent();
             
    $break = 0;
    while (($student = db_fetch_array($school_students)) /* && $break < 30*/ ) {
        $std_id = $student['uid'];

        $student_term_records = get_dw_student_term_outcome($std_id, $selected_term_id);

        $break++;

        //watchdog("basic_grade_report", "$school_nid, $school_year, $selected_term_id, $course_id, $student_id<br>".print_r($courses, true));

        $page_header_html = getPageHeaderContent($school_year, $student, $school, $term_name);

        //Get school outcome array
        $outcomes = array();
        $sw_outcomes = get_outcome_student_schoolwide($school_nid, $school_year, $selected_term_id, $std_id);
        
        while ($rec = db_fetch_object($sw_outcomes)) {

            if ($rec->grade != "*")
                $gv = FormatNumber($rec->grade, $school->grade_rounding, ',') . '%';
            else
                $gv = '*';
  
            $outcomes[$rec->name] = $gv;
        }

        //courses grades
        while ($record = db_fetch_array($student_term_records)) {            
            //Find primary teacher info
            $course_info = get_course_info($record['course_nid']);

            $primary_teacher_rec = get_user($course_info->teacher1_uid);
            unset($primary_teacher_rec2);
            if (intval($course_info->teacher2_uid) > 0 && $course_info->teacher1_uid != $course_info->teacher2_uid) {
                $primary_teacher_rec2 = get_user($course_info->teacher2_uid);
            }

            $course_html .= getCourseContent($school, $record, $course_info->course_short_title, $primary_teacher_rec, $primary_teacher_rec2);
        }
        
        $page_footer_html .= getPageFooterContent($school, $outcomes);

        if ($course_html <> "" && $page_footer_html <> "")
        {
            $html .= $page_header_html . $course_html . $page_footer_html;
        }

        $course_html = "";
        $page_footer_html = "";
    }

    if ($print_page = "1")
    {
        $html .= "<SCRIPT LANGUAGE='JavaScript'>window.print();</script>";
    }

    $html .= getFooterContent();

    print $html;

    return;
    require_once("dompdf-0.5.1/dompdf_config.inc.php");

    spl_autoload_register('DOMPDF_autoload');

    ini_set('max_execution_time', '600');
    ini_set("memory_limit", "5000M");

    $dompdf = new DOMPDF();
    $dompdf->load_html($html);
    $dompdf->render();
    $dompdf->stream("Student_Grade_Report.pdf");

}

function getHeaderContent()
{
      $html = "
<html>

<head>
<meta http-equiv=Content-Type content='text/html; charset=windows-1252'>
<style>
<!--
@font-face
    {font-family:'Cambria Math';
    panose-1:2 4 5 3 5 4 6 3 2 4;}
@font-face
    {font-family:Calibri;
    panose-1:2 15 5 2 2 2 4 3 2 4;}
@font-face
    {font-family:Tahoma;
    panose-1:2 11 6 4 3 5 4 4 2 4;}
 
p.MsoNormal, li.MsoNormal, div.MsoNormal
    {margin-top:0in;
    margin-right:0in;
    margin-bottom:10.0pt;
    margin-left:0in;
    line-height:115%;
    font-size:11.0pt;
    font-family:Tahoma,Calibri,sans-serif;}
@page Section1
    {size:8.5in 11.0in;
    margin:1.0in 1.0in 1.0in 1.0in;}
div.Section1
    {page:Section1;}
ol
    {margin-bottom:0in;}
ul
    {margin-bottom:0in;}
                  
table.MsoTableGrid { border: 0.75px solid; }
                     
td             { border: none; }

span.fontClass_TS
{
font-family:Tahoma;
}

span.fontClass_TS10
{
font-family:Tahoma,sans-serif;
font-size:9.0pt;
}
-->
</style>

</head>

<body lang=EN-US>

      ";

    return $html;
}

function getPageHeaderContent($school_year, $student, $school, $term_name)
{
    $std_grade = _get_student_grade_by_grad_year($student['grad_year']);
    $html = "
<div class=Section1>
<br><br>
<p class='MsoNormal' align='center' style='margin-bottom:0in;margin-bottom:.0001pt;
text-align:center'>
<b><span class='fontClass_TS'>" . $school->title ."</span></b></p>

<p class='MsoNormal' align='center' style='text-align:center'><span
class='fontClass_TS'>Student Progress Report<br>
" . get_tz($school->nid,"F j, Y",time()) ."</span></p>

<p class='MsoNormal' align='center' style='text-align:center;'><div class='MsoNormal' style='font-size:14.0pt;line-height:115%;' align='center'><br>
<b>" . $student['first_name'] . " " . $student['last_name'] .  " | " . $std_grade . "<sup>th</sup> Grade</b><br>
<b>" . $term_name . " (" . ($school_year - 1) . " - " . $school_year . ")</b></div></p>

<p class='MsoNormal'><span class='fontClass_TS'>&nbsp;</span></p>
</div>

<table class=MsoTableGrid  style='border: 1px solid; border-color:black' FRAME=BOX RULES=NONE cellspacing='0' cellpadding='0' align='center' width='95%' >
<tr style='background:#F2F2F2;'>
<td width='10%' valign=top align=center>
<b><span class=fontClass_TS10>Period</span></b>
</td>
<td width='20%' valign=top >
<b><span class=fontClass_TS10>Course
</span></b>
</td>
<td width='50%' valign=top>
<b><span class=fontClass_TS10>Instructor(s)</span></b>
</td>
<td width='20%' colspan=2 valign=top align=center>
<b><span class=fontClass_TS10>Weighted Grade</span></b>
</td>
</tr>
<tr><td colspan='5'><span class=fontClass_TS10>&nbsp;</span> </td></tr>";

    return $html;
}

function getCourseContent($school, $course, $course_short_title, $primary_teacher_rec, $primary_teacher_rec2)
{
    $html = "
<tr style='height:.4in'>
<td width='10%' valign=top align=center>
<span class=fontClass_TS>". ($course['period'] ? $course['period'] : '*') ."</span>
</td>
<td width='20%' valign=top>
<span class=fontClass_TS>". $course_short_title ."</span>
</td>
<td width='50%' valign=top>
<span class=fontClass_TS>";

    $html .= $primary_teacher_rec->first_name." ".$primary_teacher_rec->last_name;

    if (isset($primary_teacher_rec2)) {
      $html .= ", " . $primary_teacher_rec2->first_name.' '.$primary_teacher_rec2->last_name;
    }

    if (is_not_graded_course($course['course_nid']))
    {
        $gv = '';
        $grade_letter = 'Not Graded';
    } else {
        $gv = round($course['grade_percent'], $school->grade_rounding);
        $grade_letter = $course['grade_letter'] ;
        if ($gv > 100) {
            $gv = 100;
        }
        $gv ='('. $gv .'%)';
    }

    $html .= " 
</span>
</td>
<td width='8%' valign=top align='center'>
<span class=fontClass_TS>". $grade_letter . "</span>
</td>
<td width='12%' valign=top align='center'>
<span class=fontClass_TS>" . $gv . "</span>
</td>
</tr>";
    return $html;
}

function getPageFooterContent($school, $outcomes)
{
    $html = "
</table>

<p class=MsoNormal><b><span class=fontClass_TS>&nbsp;</span></b></p>

<table align='center' border='0' cellspacing=0 cellpadding=0 width='95%'>
<tr><td>
<p class=MsoNormal><b><span class='fontClass_TS' style='margin-bottom:5px'>Schoolwide Learning Outcomes</span></b></p>
</td></tr>
</table>
<table align='center' class=MsoTableGrid style='border: 1px solid; border-color:black; background:#F2F2F2;' cellspacing=0 cellpadding=0 width='95%'>";

    $table_columns = 0;
    $start_row = true;
    while (list($oc_name, $oc_grade) = each($outcomes)) {
        if ($start_row) {
            $html .= "<tr style='height:40px'>" ;
            $start_row = false;
        }
        
        $html .= "
<td valign=top style='background:#F2F2F2;'>
<p class='MsoNormal' align='center' style='text-align:center;margin-top:2px'>
<b><span class='fontClass_TS'>" . $oc_name . "</span></b></p>
<p class='MsoNormal' align='center' style='text-align:center;'>
<span class='fontClass_TS' style='height:20px;margin-bottom:2px'><br>" . $oc_grade . "</span></p>
</td>";

        if ($table_columns++ >= 3) {
            $html .= "</tr>";
            $table_columns = 0;
            $start_row = true;
        }
    }

    if ($table_columns > 0) {
        $html .= "</tr>";
    }
    
    $html .= "
</table>

<p class=MsoNormal><span class='fontClass_TS'>&nbsp;</span></p>

<p class='MsoNormal' align='center' style='text-align:center'>
<b><span style='font-size:12.0pt;line-height:115%;' class=fontClass_TS>*</span></b>
<span class='fontClass_TS'> is for schoolwide learning outcomes that have not yet <br>been assessed across any of this student's courses</span>
</p>
<p style='page-break-after:always'></p>";

    return $html;
}


function getFooterContent()
{
    $html = "
</body>
</html>";
    return $html;
}