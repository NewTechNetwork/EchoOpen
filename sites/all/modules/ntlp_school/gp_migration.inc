<?php
function gp_to_echo($log, &$varNId, $dtsMain, $varCourseId, $school_nid, $varSchoolYearM) {

    $dtsST = db_query("select school_term_tid from dpl_ntlp_school_term
        where school_nid=%d and school_year=%d", $school_nid, $varSchoolYearM);
    if ($dtsST->num_rows == 0) {
        $msg = "Term not found for school: " . $school_nid . " and school_year=" . $varSchoolYearM;
        $log->lwrite("#ERROR:".$msg);
        print ($msg);
        return;
    }
    $arec = db_fetch_object($dtsST);
    $varSTId = $arec->school_term_tid;

    while ($arec = db_fetch_object($dtsMain)) {
        echo '.';
        $varCreatedOn = $arec->CREATEDON;
        $varLastModifiedOn = $arec->LASTMODIFIEDON;
        $varDueDate = $arec->DUEDATE;
        if (!empty($arec->MAKEUPDATE)) {
            $varMakeupDate = $arec->MAKEUPDATE;
        } else {
            $varMakeupDate = $arec->DUEDATE;
        }

        $arec->COMMENTS = str_replace("'", "''", $arec->COMMENTS);
        $arec->NAME = str_replace("'", "''", $arec->NAME);

        $varCreatedBy = SelectUId($arec->CREATEDBY, "GP", "STA");
        $varModifiedBy = SelectUId($arec->LASTMODIFIEDBY, "GP", "STA");

        $varNId++;
        InsNode($log, $varNId, "lo_task", $arec->NAME,
            $varCreatedBy,
            $arec->CREATEDON,
            $arec->LASTMODIFIEDON, "1",
            $arec->COMMENTS, "0");

        $msg = "INSERT INTO dpl_ntlp_course_activity (curriculum_standards, curriculum_unit_tid, nid, course_nid, project_nid, activity_order, allow_resubmission, available_from, available_to, due_date, is_graded, grading_method, timecreated, timemodified, created_by, modified_by, submission_type, availability_mode, school_term_tid, is_published)
                VALUES ('', 0, " . $varNId . ", " . $varCourseId . ", 0, 0, 0, '" . $varCreatedOn . "', '" . $varMakeupDate . "', '" . $varDueDate . "', 1, 'M', '" . $varCreatedOn . "', '" . $varLastModifiedOn . "', " . $varCreatedBy . ", " . $varModifiedBy . ", 'D', 'M', " . $varSTId . ", " . $arec->PUBLISHED . ");";
        $log->lwrite($msg);

        $varStartNId = $varNId;
        $dtsLog = db_query("SELECT s.ID, s.STUDENTID, s.CREATEDON, s.LASTMODIFIEDON, su.ID as SU_ID, cbu.ID as CBU_ID, s.EXCUSESTATUS
            FROM gp.SCORE s
                INNER JOIN intg.INT_USER su ON s.STUDENTID = su.GP_ID AND su.TYPE = 'STU'
                LEFT JOIN intg.INT_USER cbu ON s.CREATEDBY = cbu.GP_ID AND cbu.TYPE = 'STA'
            WHERE s.ASSIGNMENTID=%d", $arec->ID);

        while ($srec = db_fetch_object($dtsLog)) {
            //Create submission record for student
                $varNIdAc = ++$varStartNId;
            InsNode($log, $varNIdAc, "lo_task", "Submission_" . $srec->ID,
                    $srec->SU_ID, $srec->CREATEDON,
                    $srec->LASTMODIFIEDON, "1", "", "0");

            $msg = "INSERT INTO dpl_ntlp_activity_submission (nid, activity_nid, user_uid, date_opened, date_submitted, date_graded, is_graded, is_complete, graded_by_uid, resubmit_count) values(" . $varNIdAc . ", " . $varNId . ", " . $srec->SU_ID . ", '" . $varCreatedOn . "', '" . $srec->CREATEDON . "', '" . $srec->CREATEDON . "', 1, 1, " . intval($srec->CBU_ID) . ", 1);";
                $log->lwrite($msg);

            //Get outcome wise scores for the submission
                $dtsOcPnt = db_query("select P.*, O.CAT_NUM, O.COURSEID, co.tid
                    FROM gp.SCORE_POINT P, gp.COURSE_CATINFO O, dpl_ntlp_gb_course_outcome co
                    WHERE P.COURSEID=O.COURSEID AND P.CAT_NUM=O.CAT_NUM AND P.ASSIGNMENTID=%d
                    AND P.SCHOOLID=%d AND O.COURSEID=%d AND P.STUDENTID=%d AND P.SCOREID=%d
                    AND O.CAT_NUM = co.sort_order AND co.course_nid = %d",
                $arec->ID, $arec->SCHOOLID, $arec->COURSEID, $srec->STUDENTID, $srec->ID, $varCourseId);

                while ($ocrec = db_fetch_object($dtsOcPnt)) {
                    $varTID = $ocrec->tid;
                    if (strtolower($ocrec->ENTERED) == "x") {
                        //Add score as Excused outcome
                        $msg = "INSERT INTO dpl_ntlp_gb_activity_excuse(tid, submission_nid) VALUES(" . $varTID . ", " . $varNIdAc . "); ";
                        $log->lwrite($msg);
                    } else {
                        //Don't save BLANK scores
//                        if ($ocrec->ENTERED != "") {
                            $varScore = $ocrec->EARNED;
                            $msg = "INSERT INTO dpl_ntlp_gb_activity_grade(tid, submission_nid, points) VALUES(" . $varTID . ", " . $varNIdAc . ", " . $varScore . ");";
                            $log->lwrite($msg);
//                        }
                    }
                }
            }

        //Inserting The user who submitted this assignment
        $dtsEnrollment = db_query("SELECT DISTINCT u.ID
            FROM gp.SCORE s, intg.INT_USER u
            WHERE s.STUDENTID = u.GP_ID AND u.TYPE = 'STU' AND s.ASSIGNMENTID=%d", $arec->ID);
        while ($erec = db_fetch_object($dtsEnrollment)) {
            $varUser = $erec->ID;
            $msg = "INSERT INTO dpl_ntlp_activity_user(activity_nid, user_uid) values(" . $varNId . ", " . $varUser . ");";
            $log->lwrite($msg);
        }


        //SAVE MAX POSSIBLE POINTS OF ALL OUTCOMES FOR THE CURRENT ACTIVITY
        //CHECK2
        $dtsActOC = db_query("select P.ASSIGNMENTID, O.CAT_NUM, O.COURSEID, P.POINTSPOSS, co.tid
            FROM gp.ASSIGNMENT_POINT P, gp.COURSE_CATINFO O, dpl_ntlp_gb_course_outcome co
            WHERE P.COURSEID=O.COURSEID AND P.CAT_NUM=O.CAT_NUM AND P.ASSIGNMENTID = %d
            AND co.sort_order = O.CAT_NUM AND co.course_nid = %d",
            $arec->ID, $varCourseId);
        while ($aorec = db_fetch_object($dtsActOC)) {
            $varTID = $aorec->tid;
            $msg = "INSERT INTO dpl_ntlp_gb_activity_outcome(tid, activity_nid, max_points) VALUES(" . $varTID . ", " . $varNId . ", " . $aorec->POINTSPOSS . ");";
            $log->lwrite($msg);
        }
        $varNId = $varStartNId + 1;   //To compensate the id we used in submission
    }
}


function InsNode($log, $varNId, $type, $title, $varUser, $created, $changed, $status, $body, $promote) {
//    $varUser = SelectUId($createdby,  $db,  $role);
    $msg = "INSERT INTO dpl_node(nid, vid, type, title, uid, created, changed, comment, status, promote) values (" . $varNId . ", " . $varNId . ", '" . $type . "', '" . $title . "', " . $varUser . ",  unix_timestamp('".$created."') , unix_timestamp('".$changed."') , 0, " . $status . ", " . $promote . ");";
    $log->lwrite($msg);
    $msg = "INSERT INTO dpl_node_revisions(nid, vid, title, uid, body, teaser, log, timestamp) values(" . $varNId . ", " . $varNId . ", '" . $title . "', " . $varUser . ", '" . $body . "', '', '', unix_timestamp('".$created."') );";
    $log->lwrite($msg);
}

function SelectUId($varSId, $varFrom, $varType)
{
    $varUId = 0;
    $dtsUser = db_query("SELECT ID from intg.INT_USER WHERE " . $varFrom . "_ID=" . $varSId . " AND TYPE='" . $varType . "'");
    if ($urec = db_fetch_object($dtsUser)) {
        $varUId = intval($urec->ID);
    }
    return $varUId;
}

function gp_migration_start($school_nid) {
    $start_nid = array(
        1006 => array('gp_id' => 46, 'start_nid' => 13000000, 'end_nid' => 13117660),
        1021 => array('gp_id' => 33, 'start_nid' => 13117661, 'end_nid' => 13355149),
        1027 => array('gp_id' => 5, 'start_nid' => 13355150, 'end_nid' => 13865099),
        1001 => array('gp_id' => 16, 'start_nid' => 13865100, 'end_nid' => 14063303),
        1002 => array('gp_id' => 37, 'start_nid' => 14063304, 'end_nid' => 14322078),
        1003 => array('gp_id' => 24, 'start_nid' => 14322079, 'end_nid' => 14322085),
        1004 => array('gp_id' => 25, 'start_nid' => 14322086, 'end_nid' => 14420669),
        1029 => array('gp_id' => 40, 'start_nid' => 14420670, 'end_nid' => 14543892),
        1045 => array('gp_id' => 64, 'start_nid' => 14543893, 'end_nid' => 14584446),
        1009 => array('gp_id' => 58, 'start_nid' => 14584447, 'end_nid' => 14653853),
        1007 => array('gp_id' => 62, 'start_nid' => 14653854, 'end_nid' => 14689902),
        1008 => array('gp_id' => 63, 'start_nid' => 14689903, 'end_nid' => 14703445),
        1025 => array('gp_id' => 41, 'start_nid' => 14703446, 'end_nid' => 14796047),
        1030 => array('gp_id' => 42, 'start_nid' => 14796048, 'end_nid' => 14812889),
        1032 => array('gp_id' => 56, 'start_nid' => 14812890, 'end_nid' => 14876393),
        1012 => array('gp_id' => 53, 'start_nid' => 14876394, 'end_nid' => 14979875),
        1022 => array('gp_id' => 38, 'start_nid' => 14979876, 'end_nid' => 15071002),
        1015 => array('gp_id' => 59, 'start_nid' => 15071003, 'end_nid' => 15159390),
        1010 => array('gp_id' => 4, 'start_nid' => 15159391, 'end_nid' => 15238449),
        1016 => array('gp_id' => 51, 'start_nid' => 15238450, 'end_nid' => 15345362),
        1028 => array('gp_id' => 15, 'start_nid' => 15345363, 'end_nid' => 15562806),
        1013 => array('gp_id' => 65, 'start_nid' => 15562807, 'end_nid' => 15583142),
        1005 => array('gp_id' => 2, 'start_nid' => 15583143, 'end_nid' => 16050289),
        1019 => array('gp_id' => 21, 'start_nid' => 16050290, 'end_nid' => 16187862),
        1042 => array('gp_id' => 45, 'start_nid' => 16187863, 'end_nid' => 16516585),
        1034 => array('gp_id' => 27, 'start_nid' => 16516586, 'end_nid' => 16696813),
        1024 => array('gp_id' => 44, 'start_nid' => 16696814, 'end_nid' => 16909884),
        1043 => array('gp_id' => 22, 'start_nid' => 16909885, 'end_nid' => 17454741),
        1011 => array('gp_id' => 18, 'start_nid' => 17454742, 'end_nid' => 17866397),
        1041 => array('gp_id' => 32, 'start_nid' => 18020000, 'end_nid' => 18465150),
        1020 => array('gp_id' => 20, 'start_nid' => 18465151, 'end_nid' => 18904344),
        1037 => array('gp_id' => 29, 'start_nid' => 18904345, 'end_nid' => 19142781),
        1026 => array('gp_id' => 6, 'start_nid' => 19142781, 'end_nid' => 19539502),
        1040 => array('gp_id' => 28, 'start_nid' => 19539503, 'end_nid' => 19738389),
        1018 => array('gp_id' => 9, 'start_nid' => 19738390, 'end_nid' => 20035729),
        1033 => array('gp_id' => 39, 'start_nid' => 20035730, 'end_nid' => 20156702),
        1014 => array('gp_id' => 52, 'start_nid' => 20156703, 'end_nid' => 20239921),
        1035 => array('gp_id' => 55, 'start_nid' => 20239922, 'end_nid' => 20292973),
        1023 => array('gp_id' => 13, 'start_nid' => 20292974, 'end_nid' => 20629223),
        50840 => array('gp_id' => 31, 'start_nid' => 20629224, 'end_nid' => 0),         //Old school=33
    );
    $todebug = intval(arg(4)) > 0 ? true : false;
    $repair_only = intval(arg(5)) > 0 ? true : false;
    $yrsql = "";
    $for_year = $_GET['year'];

//    delete from dpl_node where nid >= 14322079 and nid <= 17000000;
//    delete from dpl_node_revisions where nid >= 14322079 and nid <= 17000000;
//    delete from dpl_ntlp_course_activity where nid >= 14322079 and nid <= 17000000;
//    delete from dpl_ntlp_activity_submission where nid >= 14322079 and nid <= 17000000;
//    delete from dpl_ntlp_gb_activity_grade where submission_nid >= 14322079 and submission_nid <= 17000000;
//    delete from dpl_ntlp_gb_activity_excuse where submission_nid >= 14322079 and submission_nid <= 17000000;
//    delete from dpl_ntlp_gb_activity_outcome where activity_nid >= 14322079 and activity_nid <= 17000000;
//    delete from dpl_ntlp_activity_user where activity_nid >= 14322079 and activity_nid <= 17000000;

    // Call Repair functions
    if ($repair_only) {
        repair_gradeportal_courses($school_nid, $todebug, intval($for_year));
    repair_gradeportal_sw_outcomes($school_nid, $todebug);
    repair_gradeportal_dropouts($school_nid, $todebug);
        echo '<p>Course Repaired</p>';
        return;
    } else {
        echo '<p>SKIPPING: Course Repair</p>';
    }
    // Setup Log file
    if (intval($for_year) > 0) {
        $log = new logfile($_SERVER['DOCUMENT_ROOT']."/sites/default/files/migration/school_".$school_nid."_".$for_year.".sql");
        $yrsql = " AND ic.SCHOOLYEAR = ".$for_year;
        echo "Year: $for_year<br>";
    } else {
    $log = new logfile($_SERVER['DOCUMENT_ROOT']."/sites/default/files/migration/school_".$school_nid.".sql");
    }
    $today = date('Y-m-d');
    $log->lwrite("/* Created On: ".$today."*/");

    //Last node id will be given manually when resuming for new year
    if (intval($_GET['nid']) > 0) {
        $varNId = intval($_GET['nid']);
    } else {
    $varNId = $start_nid[$school_nid]['start_nid'];
    }
    echo "Starting Node: $varNId<br>";
    $log->lwrite("/* Starting with node: $varNId */");

    $dtsCourse = db_query("SELECT ic.ID, ic.COURSEID, ic.SCHOOLYEAR
        FROM intg.INT_COURSE ic, dpl_ntlp_course c
        WHERE ic.TYPE='GP' AND ic.ID = c.nid AND c.school_nid = %d
        /*AND c.nid = 110258*/ %s
        ORDER BY ic.ID", $school_nid, $yrsql);

    while ($crec = db_fetch_object($dtsCourse)) {
        echo "<br>Processing Course: $crec->ID";
        if (!$todebug) {
        $dtsMain = db_query("SELECT SCHOOLID, ID, COURSEID, TYPE, NAME, DUEDATE, MAKEUPDATE, CREATEDBY, CREATEDON, LASTMODIFIEDBY, LASTMODIFIEDON, COMMENTS, PUBLISHED
            FROM gp.ASSIGNMENT WHERE COURSEID=" . $crec->COURSEID);

        gp_to_echo($log, $varNId, $dtsMain, $crec->ID, $school_nid, $crec->SCHOOLYEAR);
    }
    }

    $dtsCourse = db_query("DELETE FROM dpl_ntlp_dw_course_grade WHERE school_nid = %d", $school_nid);

    $log->lwrite("/* Ending node: $varNId */");
    echo "/* Ending node: $varNId */";
}
