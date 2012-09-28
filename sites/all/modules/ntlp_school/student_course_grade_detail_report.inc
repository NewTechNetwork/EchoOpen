<?php
// Echo Open software Copyright � 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.

  
    module_load_include('inc', 'ntlp_school', 'data_access');
    module_load_include('inc', 'ntlp_grade', 'data_access');
    module_load_include('inc', 'ntlp_courses_activity', 'data_access');
   
    function course_grade_detail_get_course_detail($course_id)
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
      
    function course_grade_detail_get_course_users($course_nid, $student_id = 0)
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

    function get_activities_count($datars)
    {
        $inc_count=0;
        $count=0;
        while ($data = db_fetch_array($datars))
        {
            if ($data['is_complete']==0)
                $inc_count++;

            $count++;
        }

        //mysql_data_seek($datars,0);

        $activities_count['inc_count'] = $inc_count;
        $activities_count['total'] = $count;
        
        return $activities_count;
    }

  function course_grade_detail_report_export($school_nid, $school_year, $selected_term_id, $course_id, $student_id=0)
  {
      $activities_per_page = 7;
      
//      $student_id = arg(7);
      
      $school_students = course_grade_detail_get_course_users($course_id, $student_id);
      
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
        
      $course_detail = course_grade_detail_get_course_detail($course_id);
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
            

      $html = course_grade_detail_getHeaderContent();

      $page_num = 0;      
      while(($student = db_fetch_array($school_students)) /*&& $page_num < 10*/) 
      {
            if ($page_num > 0)
                $html .= '<p style="page-break-after:always";/>';
                
            $page_num++;
               
          $std_id = $student['uid'];
                                                              
            $grade_percent = _get_student_course_percentage($std_id, $course_id, $selected_term_id);
            $grade_letter = _get_course_grade($grade_percent, $course_id, "L");
            $grade_percent = format_course_grade_percent($grade_percent, $course_id);
                                   
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

            $datars = get_student_course_activity_outcomes($course_id, $student_id, $selected_term_id, $outcome_rows);

            $count_activities = get_activities_count($datars);

            //FIXME: Had to call the following function again as resetting datars is not working
            $datars = get_student_course_activity_outcomes($course_id, $student_id, $selected_term_id, $outcome_rows);

            $html .= getSummaryPageContent($school_year, $student, $school, $term_name, $course_detail, $primary_teacher_rec, $primary_teacher_rec2, $grade_letter, $grade_percent, $outcomes, $outcome_rows, $count_activities['inc_count']);
            $html .= get_PageFooterContent(1, $course_detail, $student, ($count_activities['total'] > 0) ? (($count_activities['total']/$activities_per_page)+2) : 1);

            $html .= getActivityDetailsPageContent($datars, $school, $count_activities['total'], $activities_per_page, $outcomes, $outcome_rows, $course_detail, $student);
      }
                
                
      $html .= "<SCRIPT LANGUAGE='JavaScript'>window.print();</script>";
      $html .= course_grade_detail_getFooterContent();

      
    print $html;
    
  }

  function course_grade_detail_getHeaderContent()
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
    {size:landscape;
    margin:0.5in 0.5in 0.5in 0.5in;}
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

<style type="text/css" >
    .printspan
    {
        position: relative; TEXT-ALIGN: center; color:grey;        
    }
</style>
<style type="text/css" media="print">
    /*.printspan
    {
        position: fixed; left:30%; bottom:0%; TEXT-ALIGN: center; color:grey;
    }*/
    @page {
        size:landscape;
    }

</style>

</head>

<body lang="EN-US">

      ';

    return $html;
  }

    function getSummaryPageContent($school_year, $student, $school, $term_name, $course, $primary_teacher_rec, $primary_teacher_rec2, $grade_letter, $grade_percent, $outcomes, $outcome_rows, $count_incomplete_activities)
  {
      $std_grade = _get_student_grade_by_grad_year($student['grad_year']);
      
      $html = '
       <br><br>
       <div class="Section1">

<p class="MsoNormal" align="center" style="MARGIN-BOTTOM: 0pt; TEXT-ALIGN: center"><b><span class="fontClass_TS"> 
' . $school->title . ' </span></b></p>

<p class="MsoNormal" align="center" style="TEXT-ALIGN: center"><span
 class="fontClass_TS">Detailed Student Report<br>
' . date("F j, Y") . ' </span></p>
  </div> 
<table width="100%" align="center" >
<tr><td style="border-bottom-color: black; border-bottom-width: 1px; border-bottom-style: solid; ">
<p class="MsoNormal" style="BORDER-RIGHT: medium none; PADDING-RIGHT: 0in; BORDER-TOP: medium none; PADDING-LEFT: 0in; BACKGROUND: white; PADDING-BOTTOM: 0.075in; BORDER-LEFT: medium none; PADDING-TOP: 0in; BORDER-BOTTOM: medium none"><b><span
 style="FONT-SIZE: 14pt; LINE-HEIGHT: 115%; FONT-FAMILY:Tahoma,sans-serif"> ' . $student['first_name'] . ' ' . $student['last_name'] .  ' </span></b><span style="FONT-SIZE:
 14pt; LINE-HEIGHT: 115%; FONT-FAMILY:Tahoma,sans-serif">|<b> ' . $std_grade . '<sup>th</sup> Grade</b></span></p>
 </td></tr>
 </table>

<table class="MsoNormalTable" align="center" border=0 cellspacing="0" cellpadding="0" style="BORDER-COLLAPSE: collapse" width="100%">
 <tr>
  <td width="20%" valign="top">
  <p class="MsoNormal"><b><span class="fontClass_TS" style="LINE-HEIGHT: 115%;">Course:</span></b></p>
  </td>
  <td width="30%" valign="top">
  <p class="MsoNormal" style="BACKGROUND: white"><span class="fontClass_TS" style="LINE-HEIGHT:
  115%;">' . $course->course_short_title . ' ( ' . ($student['period'] ? ('Period '. $student['period']) : '*') . ' )</span></p>
  </td>
  <td width="20%" valign="top">
  <p class="MsoNormal"  style="BACKGROUND: white;">
    <b><span class="fontClass_TS">Current Weighted Grade:</span></b></p>
  </td>
  <td width="30%" valign=top style="padding:0in 5.4pt 0in 5.4pt">
  <p class=MsoNormal style="background:white"><span class="fontClass_TS">'. $grade_letter . '   ' . $grade_percent . '</span></p>
  </td>
 </tr>
 <tr>
  <td width="20%" valign="top">
  <p class="MsoNormal"><b><span class="fontClass_TS" style="LINE-HEIGHT: 115%;">Term:</span></b></p>
  </td>
  <td width="35%" valign="top">
  <p class="MsoNormal" style="BACKGROUND: white"><span class="fontClass_TS" style="LINE-HEIGHT:115%;">' . $term_name . ' (' . ($school_year - 1) . ' - ' . $school_year . ')</span></p>
  </td>
  <td width="30%" rowspan="2" valign="top">
  <p class="MsoNormal" style="BACKGROUND: white;">
    <b><span class="fontClass_TS">Incomplete Activities:</span></b></p>
  </td>
  <td width="15%" rowspan="2" valign=top style="padding:0in 5.4pt 0in 5.4pt">
  <p class=MsoNormal style="background:white"><span class="fontClass_TS"">'.$count_incomplete_activities.'</span></p>
  </td>
 </tr>
 <tr>
  <td width="20%" valign="top">
  <p class="MsoNormal"><b><span class="fontClass_TS" style="LINE-HEIGHT:115%;">Instructor(s):</span></b></p>
  </td>
  <td width="35%" valign="top">
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

<br>
<table width="100%" align="center" >
<tr><td>
<p class="MsoNormal"><b><span class="fontClass_TS">
Course Learning Outcomes:</span></b></p>
 </td></tr>
 </table>

<table class="MsoNormalTable" align="center" FRAME=BOX RULES=NONE cellspacing=0 cellpadding=3 style="BORDER-COLLAPSE: collapse; border-top-color: black; border-right-color: black; border-bottom-color: black; border-left-color: black; border-top-width: 1px; border-right-width: 1px; border-bottom-width: 1px; border-left-width: 1px; border-top-style: solid; border-right-style: solid; border-bottom-style: solid; border-left-style: solid;" width="100%">
      ';

        $rows = $outcome_rows;


         for($i=0; $i < sizeof($rows); )
         {
             $html .= '<tr>';

            $table_columns = 0;
            for($oc=$i; /*$oc < sizeof($rows) &&*/ $table_columns++ < 2; $i++, $oc++)
            {
                if ($rows[$oc])
                {
                    $html .=
            '
                      <td width="30%" valign="top">
                      <p class="MsoNormal" style="MARGIN:  3pt 0in 0pt; LINE-HEIGHT: normal">
                        <b><span class="fontClass_TS" style="font-size:10pt;">' . $rows[$oc]->name . '<br>
                      </span></b>
                      <span class="fontClass_TS" style="FONT-SIZE: 10pt;">(Weight '. round($rows[$oc]->weight_percent, $school->grade_rounding) .' %)
                      </span></p></td>
                    <td width="20%" valign="top" align=left>
                      <p class="MsoNormal" style="MARGIN:  6pt 0in 0pt; LINE-HEIGHT: normal">
                        <span class="fontClass_TS">'. $outcomes[$rows[$oc]->name] . (($outcomes[$rows[$oc]->name] >= "0") ? '%' : '')  . '</span></p>
                    </td>  ';
                }
                else
                {
                    $html .=
            '
                      <td width="30%" valign="top">
                      <p class="MsoNormal" style="MARGIN:  6pt 0in 0pt; LINE-HEIGHT: normal">
                        <b><span class="fontClass_TS"><br>
                      </span></b>
                      <span class="fontClass_TS" style="FONT-SIZE: 10pt;">
                      </span></p> </td>
                    <td width="20%" valign="top" align=center>
                      <p class="MsoNormal" style="MARGIN:  6pt 0in 0pt; LINE-HEIGHT: normal">
                        <span class="fontClass_TS"></span></p>
                    </td>  ';
                }
            }
            $html .= ' </tr>';
         }

         $html .= '

        </table>

        <p class="MsoNormal" align="center" style="TEXT-ALIGN: center; color:grey;"><b><span
         class="fontClass_TS" style="FONT-SIZE: 12pt; ">*</span></b><span class="fontClass_TS10"> is for a course learning outcome
        that has not yet been assessed in this course</span></p>

        <p class="MsoNormal"><span class="fontClass_TS"></span>&nbsp;</p>
        
      ';

    return $html;
  }                            

    function course_grade_detail_getFooterContent()
    {
      $html = '
      </body>
      </html>
      ';
      return $html;
    }

    function getActivityDetailsPageContent($datars, $school, $count_activities, $activities_per_page, $outcomes, $outcome_rows, $course, $student)
    {
        $activity_displayed=0;
        $page=1;
        while ($activity_displayed < $count_activities)
        {
            $page++;
            $html .= '

    <p class=MsoNormal><span class="fontClass_TS">&nbsp;</span></p>

    <table width="100%" align=center cellpadding=0 cellspacing=0>
     <tr>
      <td width=30%>
      <b><span class="fontClass_TS">Activity Details</span></b>
      </td>
      <td width=70% style="border:.75pt solid black;
      vertical-align:center;background:#F2F2F2">
      <table cellpadding=0 cellspacing=5 width="100%">
       <tr>
        <td>
        <p align=center style="text-align:center">INC: “Incomplete”
        activities count as a zero     EX: The student is “excused” from this
        requirement   </p>
        </td>
       </tr>
      </table>
      </td>
     </tr>
    </table>
    <br>

    <table class=MsoNormalTable border=1 cellspacing=0 cellpadding=0 width="100%"
     align=center style="border-collapse:collapse;border:none">
     <tr style="height:35.5pt">
      <td valign=bottom style="width:9%;border:solid windowtext 1.0pt;
      background:#D9D9D9;padding:0in 5.4pt 0in 5.4pt;height:35.5pt">
      <p class=MsoNormal><b><span style="font-size:9pt;line-height:115%;
      font-family:"Tahoma","sans-serif"">Due Date</span></b></p>
      </td>
      <td valign=bottom style="width:21%;border:solid windowtext 1.0pt;
      border-left:none;background:#D9D9D9;padding:0in 5.4pt 0in 5.4pt;height:35.5pt">
      <p class=MsoNormal><b><span style="font-size:9pt;line-height:115%;
      font-family:"Tahoma","sans-serif"">Activity Name</span></b></p>
      </td>';

            for($i=0; $i<sizeof($outcome_rows); $i++)
            {
                $html .= '

      <td valign=bottom style="width:'. 70/sizeof($outcome_rows) . '%;border:solid windowtext 1.0pt;
      border-left:none;background:#D9D9D9;padding:0in 5.4pt 0in 5.4pt;height:35.5pt">
      <p class=MsoNormal><b><span style="font-size:9pt;line-height:115%;
      font-family:"Tahoma","sans-serif"">' . $outcome_rows[$i]->name . '</b><br></span>
          <span style="font-size:9pt;font-family:"Tahoma","sans-serif"">(Weight '. round($outcome_rows[$i]->weight_percent, $school->grade_rounding) .'%)</span></p>
      </td>
      ';
            }

            $html .= '</tr>';

            while ($activity_displayed++ < $activities_per_page * ($page-1) && $data = db_fetch_array($datars) ) {

                $html .= '
     <tr style="height:37.5pt">
      <td width="8%" style="border:solid windowtext 1.0pt;border-top:
      none;padding:0in 5.4pt 0in 5.4pt;height:37.5pt">
      <p class=MsoNormal><span style="font-size:9pt;line-height:115%;font-family:
      "Tahoma","sans-serif"">'.($data['due_date'] == 0 ? '*' : date('m/d/Y', strtotime($data['due_date']))).'</span></p>
      </td>
      <td width="21%" style="border-top:none;border-left:none;
      border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
      padding:0in 5.4pt 0in 5.4pt;height:37.5pt">
      <p class=MsoNormal><span class=apple-style-span><span style="font-size:9pt;
      line-height:115%;font-family:"Tahoma","sans-serif";color:black">'.$data['activity_title'].'</span></span></p>
      </td>';

                for ($ocno = 1; $ocno <=sizeof($outcome_rows); $ocno++) {
                    $gv="";
                    if ($data['is_complete'] == 0 && $data['is_graded'] == 1 && !empty($data['date_submitted']))
                        $gv = "INC";
                    elseif (isset($data['outcome_' . $ocno])) {
                        $gv = $data['outcome_' . $ocno];
                    }

                    $html .= '
      <td width="8%" style="border-top:none;border-left:none;
      border-bottom:solid windowtext 1.0pt;border-right:solid windowtext 1.0pt;
      padding:0in 5.4pt 0in 5.4pt;height:37.5pt">
      <p class=MsoNormal><span style="font-size:9pt;line-height:115%;font-family:
      "Tahoma","sans-serif"">'.$gv.'</span></p>
      </td>';
                }

                $html .= '</tr>';
            }

            $html .= '</table><br>';

            $html .= get_PageFooterContent($page, $course, $student, ($count_activities/$activities_per_page)+2);
        }
        return $html;
        
    }

    function get_PageFooterContent($page_num, $course, $student, $total_pages)
    {
        $html = '
        <footer>
        <p class="printspan" >
         <span class="fontClass_TS" style="font-size:11pt;">'. $course->course_short_title . ' | ' .
                    $student['first_name'] . ' ' . $student['last_name'] . ' | '
                .  date("m/d/Y") . ' | '
                . 'Page <b>' . $page_num . '</b> of <b>' . intval($total_pages) . '</b></span></p>
        </footer>' . (($page_num < intval($total_pages)) ? '<p style="page-break-after:always";/>' : '');

        return $html;
    }
?>