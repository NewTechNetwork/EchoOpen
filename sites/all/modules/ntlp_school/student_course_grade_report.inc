<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.

  
    module_load_include('inc', 'ntlp_school', 'data_access');
    module_load_include('inc', 'ntlp_grade', 'data_access');

   
    function course_grade_get_course_detail($course_id)
    {
        $rs = db_query("SELECT nc.nid, nc.course_short_title, nc.teacher1_uid, nc.teacher2_uid
                    FROM {ntlp_course} nc
                    WHERE nc.is_project <> 1
                     AND nc.nid = %d 
                  ",
                 $course_id);

        $rows = array();
        while( $row = db_fetch_object($rs) ) {
            $rows[] = $row;
        }

        //watchdog('student_snapshot', print_r($rows, true));
        return $rows;

    }
      
function course_grade_get_course_users($course_nid, $student_id = 0)
{
    $filter = '';
    if ($student_id != 0) {
        $filter = ' AND cu.user_uid = '.$student_id;
    }
    $rs = db_query("SELECT u.uid, u.first_name, u.middle_name, u.last_name, u.grad_year, cu.period
            FROM {ntlp_user} u
            INNER JOIN {ntlp_course_user} cu ON cu.user_uid = u.uid
            WHERE cu.course_nid = %d AND cu.role_id = 5
            %s
            ORDER BY u.last_name, u.first_name",
            $course_nid, $filter);
    return $rs;
}
  
function course_grade_report_export($school_nid, $school_year, $selected_term_id, $course_id, $student_id=0)
{
      $school_students = course_grade_get_course_users($course_id, $student_id);

      $school = get_school_record($school_nid);

      $term_name = get_school_term_name($selected_term_id)->name;

      $school_oc = get_course_outcomes($course_id);

      //initialize outcomes
      $outcomes = array();
      $outcome_rows = array();
      while($oc = db_fetch_object($school_oc))
        {
            if (!empty($oc)) {
                $outcomes[$oc->name] = 0;

                $outcome_rows[] = $oc;
            }
        }
        
      $course_detail = course_grade_get_course_detail($course_id);
      $course_detail = $course_detail[0];

        //Find primary teacher info
        $primary_teacher_rec = get_user($course_detail->teacher1_uid);
    if (intval($course_detail->teacher2_uid) > 0 && $course_detail->teacher1_uid != $course_detail->teacher2_uid) {
        $primary_teacher_rec2 = get_user($course_detail->teacher2_uid);
            }
//        //Find teacher info
//        $teachers = get_course_users($course_id, 6);
//
//        $additional_teachers_rec = array();
//        while($teacher = db_fetch_array($teachers))
//        {
//            if ($teacher['uid'] == $primary_teacher_rec->uid)
//            {
//                continue;
//            }
//
//            $additional_teachers_rec[] = get_user($teacher['uid']);
//        }
            

      $html = course_grade_getHeaderContent();

      $page_num = 0;      
      while(($student = db_fetch_array($school_students)) /*&& $page_num < 10*/) 
      {
            if ($page_num > 0)
                $html .= '<p style="page-break-after:always";/>';
                
            $page_num++;
               
          $std_id = $student['uid'];
                                                              
            $grade_percent = _get_student_course_percentage($std_id, $course_id, $selected_term_id);
            $grade_letter = _get_course_grade($grade_percent, $course_id, "L");
            $grade_percent = format_course_grade_percent($grade_percent, $course_id,$grade_letter);
                                 
            $html .= course_grade_getPageHeaderContent($school_year, $student, $school, $term_name, $course_detail, $primary_teacher_rec, $primary_teacher_rec2, $grade_letter, $grade_percent);
                                   
            //Reset school outcome array
            for ($i=0; $i<sizeof($outcome_rows); $i++)
            {
                $outcomes[$outcome_rows[$i]->name] = 0;
            }
              
            $outcomesResult = get_outcome_student($course_id, $school_year, $selected_term_id, $std_id);
                                
            while( $out = db_fetch_object($outcomesResult) )
            {
                //watchdog("student_course_report", "out: ".print_r($out, true));
                if ($out->grade > 100) $gv = 100; else $gv = $out->grade;
                $gv = round($gv, $school->grade_rounding);
                if (isset($outcomes[$out->name]))
                {
                    if (isset($out->grade))
                    {
                        $outcomes[$out->name] += (float)$gv;
                    }
                    else
                    {
                        $outcomes[$out->name] = '*';
                    }
                }
            }
                                     
        $html .= course_grade_getPageFooterContent($school, $course_id, $outcomes, $outcome_rows);
      }
                
                
      $html .= "<SCRIPT LANGUAGE='JavaScript'>window.print();</script>";
      $html .= course_grade_getFooterContent();

      
    print $html;
    return;
    
    require_once("dompdf-0.5.1/dompdf_config.inc.php");

    spl_autoload_register('DOMPDF_autoload');
    ini_set('max_execution_time', '120');
    ini_set("memory_limit", "5000M");
    
    $dompdf = new DOMPDF();
    $dompdf->load_html($html);
    $dompdf->render();
    $dompdf->stream("Student_Course_Grade_Report.pdf", array(1, 0));

    
  }

  function course_grade_getHeaderContent()
  {
      $html = '
            <html>

<head>
<meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
<style>
<!--
 /* Font Definitions */
 @font-face
    {font-family:"Cambria Math";
    panose-1:2 4 5 3 5 4 6 3 2 4;}
@font-face
    {font-family:Calibri;
    panose-1:2 15 5 2 2 2 4 3 2 4;}
@font-face
    {font-family:Tahoma;
    panose-1:2 11 6 4 3 5 4 4 2 4;}
 /* Style Definitions */
 p.MsoNormal, li.MsoNormal, div.MsoNormal
    {margin-top:0in;
    margin-right:0in;
    margin-bottom:10.0pt;
    margin-left:0in;
    line-height:115%;
    font-size:11.0pt;
    font-family:"Calibri","sans-serif";}
@page Section1
    {size:8.5in 11.0in;
    margin:1.0in 1.0in 1.0in 1.0in;}
div.Section1
    {page:Section1;}
 /* List Definitions */
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
font-family:Tahoma;
font-size:10.0pt;
}

-->
</style>

</head>

<body lang="EN-US">

      ';

    return $html;
  }

function course_grade_getPageHeaderContent($school_year, $student, $school, $term_name, $course, $primary_teacher_rec, $primary_teacher_rec2, $grade_letter, $grade_percent) {
      $std_grade = _get_student_grade_by_grad_year($student['grad_year']);

      $html = '
       <br><br>
       <div class="Section1">

<p class="MsoNormal" align="center" style="MARGIN-BOTTOM: 0pt; TEXT-ALIGN: center"><b><span class="fontClass_TS"> 
' . $school->title . ' </span></b></p>

<p class="MsoNormal" align="center" style="TEXT-ALIGN: center"><span
 class="fontClass_TS">Student Progress Report<br>
' . date("F j, Y") . ' </span></p>
  </div> 
<table width="80%" align="center" >
<tr><td style="border-bottom-color: black; border-bottom-width: 1px; border-bottom-style: solid; ">
<p class="MsoNormal" style="BORDER-RIGHT: medium none; PADDING-RIGHT: 0in; BORDER-TOP: medium none; PADDING-LEFT: 0in; BACKGROUND: white; PADDING-BOTTOM: 0in; BORDER-LEFT: medium none; PADDING-TOP: 0in; BORDER-BOTTOM: medium none"><b><span
 style="FONT-SIZE: 14pt; LINE-HEIGHT: 115%; FONT-FAMILY:Tahoma,sans-serif"> ' . $student['first_name'] . ' ' . $student['last_name'] .  ' </span></b><span style="FONT-SIZE:
 14pt; LINE-HEIGHT: 115%; FONT-FAMILY:Tahoma,sans-serif">|<b> ' . $std_grade . '<sup>th</sup> Grade</b></span></p>
 </td></tr>
 </table>

<table class="MsoNormalTable" align="center" border=0 cellspacing="0" cellpadding="0" style="BORDER-COLLAPSE: collapse" width="80%">
 <tr>
  <td width="20%" valign="top">
  <p class="MsoNormal"><b><span class="fontClass_TS" style="LINE-HEIGHT: 115%;">Course:</span></b></p>
  </td>
  <td width="30%" valign="top">
  <p class="MsoNormal" style="BACKGROUND: white"><span class="fontClass_TS" style="LINE-HEIGHT:
  115%;">' . $course->course_short_title . ' ( ' . ($student['period'] ? ('Period '. $student['period']) : '*') . ' )</span></p>
  </td>
  <td width="50%" rowspan="3" valign="top">
  <p class="MsoNormal" align="center" style="BACKGROUND: white; TEXT-ALIGN: center">
    <b><span class="fontClass_TS">Current Weighted Grade</span></b></p>
  <p class="MsoNormal" align="center" style="BACKGROUND: white; TEXT-ALIGN: center">
    <b><span class="fontClass_TS" style="FONT-SIZE: 18pt; LINE-HEIGHT: 115%;">'. $grade_letter . '</span></b></p>
  <p class="MsoNormal" align="center" style="BACKGROUND: white; TEXT-ALIGN: center">
    <b><span class="fontClass_TS" style="FONT-SIZE: 12pt; LINE-HEIGHT: 115%;">' . $grade_percent . '</span></b></p>
  </td>
 </tr>
 <tr>
  <td width="20%" valign="top">
  <p class="MsoNormal"><b><span class="fontClass_TS" style="LINE-HEIGHT: 115%;">Term:</span></b></p>
  </td>
  <td width="30%" valign="top">
  <p class="MsoNormal" style="BACKGROUND: white"><span class="fontClass_TS" style="LINE-HEIGHT:115%;">' . $term_name . ' (' . ($school_year - 1) . ' - ' . $school_year . ')</span></p>
  </td>
 </tr>
 <tr>
  <td width="20%" valign="top">
  <p class="MsoNormal"><b><span class="fontClass_TS" style="LINE-HEIGHT:115%;">Instructor(s):</span></b></p>
  </td>
  <td width="30%" valign="top">
  <p class="MsoNormal"><span class="fontClass_TS" style="LINE-HEIGHT: 115%;">';
  
  
  $html .= $primary_teacher_rec->first_name.' '.$primary_teacher_rec->last_name . '<br>
  ('. $primary_teacher_rec->name . ')<br>';

  if (isset($primary_teacher_rec2)) {
      $html .= $primary_teacher_rec2->first_name.' '.$primary_teacher_rec2->last_name . '<br>
      ('. $primary_teacher_rec2->name . ')<br>';
  }
//  for ($i=0; $i<sizeof($teachers_rec); $i++)
//  {
//      $html .= $teachers_rec[$i]->first_name.' '.$teachers_rec[$i]->last_name . '<br>
//       ('. $teachers_rec[$i]->name . ')<br>';
//  }
  
  $html .= '</span></p>
  </td>
 </tr>
</table>

<br><br>
<table width="80%" align="center" >
<tr><td>
<p class="MsoNormal"><b><span class="fontClass_TS">
Course Learning Outcomes:</span></b></p>
 </td></tr>
 </table>

<table class="MsoNormalTable" align="center" FRAME=BOX RULES=NONE cellspacing=0 cellpadding=5 style="background:#F2F2F2;BORDER-COLLAPSE: collapse; border-top-color: black; border-right-color: black; border-bottom-color: black; border-left-color: black; border-top-width: 1px; border-right-width: 1px; border-bottom-width: 1px; border-left-width: 1px; border-top-style: solid; border-right-style: solid; border-bottom-style: solid; border-left-style: solid;" width="80%">

      ';

    return $html;
  }                            

  function course_grade_getPageFooterContent($school, $course_id, $outcomes, $outcome_rows)
  {
      $rows = $outcome_rows;

                        
         for($i=0; $i < sizeof($rows); ) 
         {
             $html .= '<tr style="HEIGHT: 45.4pt">';
         
            $table_columns = 0;
            for($oc=$i; /*$oc < sizeof($rows) &&*/ $table_columns++ < 2; $i++, $oc++) 
            {
                if ($rows[$oc])
                {
                    $html .= 
            '       
                      <td width="112.5" valign="top">
                      <p class="MsoNormal" style="MARGIN:  6pt 0in 0pt; LINE-HEIGHT: normal">
                        <b><span class="fontClass_TS">' . $rows[$oc]->name . '<br>
                      </span></b>
                      <span class="fontClass_TS" style="FONT-SIZE: 10pt;">(Weight '. round($rows[$oc]->weight_percent, $school->grade_rounding) .' %)
                      </span></p> </td>
                    <td width="112.5" valign="top" align=center>
                      <p class="MsoNormal" style="MARGIN:  6pt 0in 0pt; LINE-HEIGHT: normal">
                        <span class="fontClass_TS">'. $outcomes[$rows[$oc]->name] . (($outcomes[$rows[$oc]->name] >= "0") ? '%' : '')  . '</span></p>
                    </td>  ';
                }
                else
                {
                    $html .= 
            '       
                      <td width="112.5" valign="top">
                      <p class="MsoNormal" style="MARGIN:  6pt 0in 0pt; LINE-HEIGHT: normal">
                        <b><span class="fontClass_TS"><br>
                      </span></b>
                      <span class="fontClass_TS" style="FONT-SIZE: 10pt;">
                      </span></p> </td>
                    <td width="112.5" valign="top" align=center>
                      <p class="MsoNormal" style="MARGIN:  6pt 0in 0pt; LINE-HEIGHT: normal">
                        <span class="fontClass_TS"></span></p>
                    </td>  ';
                }
            }
            $html .= ' </tr>';
         }
                          
         $html .= '

        </table>

        <p class="MsoNormal" align="center" style="TEXT-ALIGN: center"><b><span
         class="fontClass_TS" style="FONT-SIZE: 12pt; LINE-HEIGHT: 115%;">*</span></b><span class="fontClass_TS"> is for a course learning outcome
        that has not yet been assessed in this course</span></p>

        <table class="MsoNormalTable" align="center" cellspacing=0 cellpadding=0 style="BORDER-COLLAPSE: collapse" width="80%">
        <tr><td>
        <p class="MsoNormal"><b><span class="fontClass_TS"><br>
        Instructor Comments</span></b></p>
        </td></tr>       
        </table>
        
        <table class="MsoNormalTable" align="center" FRAME=BOX RULES=NONE cellspacing=0 cellpadding=0 style="BORDER-COLLAPSE: collapse; border-top-color: black; border-right-color: black; border-bottom-color: black; border-left-color: black; border-top-width: 1px; border-right-width: 1px; border-bottom-width: 1px; border-left-width: 1px; border-top-style: solid; border-right-style: solid; border-bottom-style: solid; border-left-style: solid;" width="80%">
        <tr><td>

        <p class="MsoNormal" style="BORDER-RIGHT: medium none; PADDING-RIGHT: 0in; BORDER-TOP: medium none; PADDING-LEFT: 0in; PADDING-BOTTOM: 0in; BORDER-LEFT: medium none; PADDING-TOP: 0in; BORDER-BOTTOM: medium none"><span class="fontClass_TS"></span>&nbsp;</p>

        <p class="MsoNormal" style="BORDER-RIGHT: medium none; PADDING-RIGHT: 0in; BORDER-TOP: medium none; PADDING-LEFT: 0in; PADDING-BOTTOM: 0in; BORDER-LEFT: medium none; PADDING-TOP: 0in; BORDER-BOTTOM: medium none"><span class="fontClass_TS"></span>&nbsp;</p>

        <p class="MsoNormal" style="BORDER-RIGHT: medium none; PADDING-RIGHT: 0in; BORDER-TOP: medium none; PADDING-LEFT: 0in; PADDING-BOTTOM: 0in; BORDER-LEFT: medium none; PADDING-TOP: 0in; BORDER-BOTTOM: medium none"><span class="fontClass_TS"></span>&nbsp;</p>

        </td></tr>       
        </table>

        <p class="MsoNormal"><span class="fontClass_TS"></span>&nbsp;</p>
        <br><br>

        <table class="MsoNormalTable" align="center" cellspacing=0 cellpadding=0 style="BORDER-COLLAPSE: collapse" width="80%">
        <tr><td width="80%" style="border-top-color: black; border-top-width: 1px; border-top-style: solid; ">
        <p class="MsoNormal" style="BORDER-RIGHT: medium none; PADDING-RIGHT: 0in; BORDER-TOP: medium none; PADDING-LEFT: 0in; PADDING-BOTTOM: 0in; BORDER-LEFT: medium none; PADDING-TOP: 0in; BORDER-BOTTOM: medium none">
        <span class="fontClass_TS">Parent Signature</span></p>
        </td>
        <td width="20%" style="border-top-color: black; border-top-width: 1px; border-top-style: solid;">
        <p class="MsoNormal" style="BORDER-RIGHT: medium none; PADDING-RIGHT: 0in; BORDER-TOP: medium none; PADDING-LEFT: 0in; PADDING-BOTTOM: 0in; BORDER-LEFT: medium none; PADDING-TOP: 0in; BORDER-BOTTOM: medium none">
        <span class="fontClass_TS">Date</span></p></td>
        </tr>       
        </table>
      
      ';

    return $html;
  }


    function course_grade_getFooterContent()
    {
      $html = '
      </body>
      </html>
      ';
      return $html;
    }

  
?>