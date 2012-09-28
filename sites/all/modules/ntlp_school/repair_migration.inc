<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'cron_grades');

function repair_gradeportal_courses($school_nid, $todebug, $school_year = 0) {
//    $todebug = intval(arg(3)) > 0 ? true : false;
    //Problem: After migration, we saw that courses migrated from GradePortal are created twice
    //One in one year and one course in another year. The course that is migrated in wrong year has
    //correct outcomes and correct short title. The course that is migrated in correct year has no
    //outcomes and incorrect short title, therefore, this just needs to be deleted.

    //How to detect invalid course/year:
    // --> Its ID is always one larger than the good course.
    // --> It has NO outcomes
    // --> Join with INT_COURSE so that only migrated courses are repaired
    $yrsql = "";
    if (intval($for_year) > 0) {
        $yrsql = " AND ic.SCHOOLYEAR = ".$for_year;
    }
    $crs = db_query("select count(*) cnt, n.*
        FROM dpl_node n, dpl_ntlp_course c, intg.INT_COURSE ic
        WHERE n.nid = c.nid AND c.school_nid = $school_nid
        AND c.nid = ic.ID AND ic.TYPE = 'GP' $yrsql
        GROUP BY n.title HAVING cnt > 1 LIMIT 0, 500");

    echo "######################################################";
    echo "# Repair Duplicate Courses ";
    echo "# DEBUG Mode: " . ($todebug ? "true" : "false") . "<BR>";
    echo "#Starting with $crs->num_rows<BR>";
    while ($crec = db_fetch_object($crs)) {
        $sql = "SELECT c.*
            FROM dpl_node n, dpl_ntlp_course c
            WHERE n.nid = c.nid AND n.nid in (%d, %d, %d) AND n.title = '%s'
            ORDER BY n.nid";
        $ars = db_query($sql, intval($crec->nid)-1, intval($crec->nid), intval($crec->nid)+1, $crec->title);
        $first_rec = "x";
        $last_rec = "x";
        $st_old = "";
        $st_new = "";
        $sy_old = "";
        $sy_new = "";
        while ($arec = db_fetch_object($ars)) {
            if ($first_rec == 'x') {
                $first_rec = $arec->nid;
                $st_old = $arec->school_term_tid;
                $sy_old = $arec->school_year;
            } else {
                $last_rec = $arec->nid;
                $st_new = $arec->school_term_tid;
                $sy_new = $arec->school_year;;
            }
        }
        echo "#Update wrong course '$crec->title' having id=$first_rec with $last_rec<br>";

        if ($first_rec != $last_rec) {
            $tmprs1 = db_query("SELECT count(*) cnt FROM dpl_ntlp_gb_course_outcome
                    WHERE course_nid = %d", $first_rec);
            $tmprec1 = db_fetch_object($tmprs1);
            $tmprs2 = db_query("SELECT count(*) cnt FROM dpl_ntlp_gb_course_outcome
                    WHERE course_nid = %d", $last_rec);
            $tmprec2 = db_fetch_object($tmprs2);

            //The found courses satisifies the original condition
            if ($tmprec1->cnt > 0 && $tmprec2->cnt == 0) {
                $sql1 = t("DELETE FROM dpl_ntlp_course WHERE nid = !NID", array('!NID' => $last_rec));
                $sql1a = t("DELETE FROM dpl_ntlp_course_term WHERE course_nid = !NID", array('!NID' => $last_rec));
                echo $sql1.'<br>'.$sql1a.'<br>';
                if (!$todebug) {
                    db_query($sql1);
                    db_query($sql1a);
                }
                //Delete submissions/grades of all activities of the wrong course
                $casrs = db_query("select a.nid
                    FROM dpl_ntlp_course_activity a
                    WHERE a.course_nid = %d", $last_rec);
                echo '#--------------------------<br>';
                while ($casrec = db_fetch_object($casrs)) {
                    $sql1b = t("DELETE FROM dpl_ntlp_gb_activity_grade WHERE submission_nid IN
                        (SELECT nid FROM dpl_ntlp_activity_submission WHERE activity_nid = !AID)", array('!AID' => $casrec->nid));
                    $sql1c = t("DELETE FROM dpl_ntlp_activity_submission WHERE activity_nid = !AID", array('!AID' => $casrec->nid));
                    echo $sql1b.'<br>'.$sql1c.'<br>';
                    if (!$todebug) {
                        db_query($sql1b);
                        db_query($sql1c);
                    }
                }
                echo '#--------------------------<br>';
                $sql1d = t("DELETE FROM dpl_ntlp_course_activity WHERE course_nid = !NID", array('!NID' => $last_rec));
                $sql1e = t("DELETE FROM dpl_node WHERE nid = !NID", array('!NID' => $last_rec));
                $sql1f = t("DELETE FROM dpl_node_revisions WHERE nid = !NID", array('!NID' => $last_rec));
                echo $sql1d.'<br>'.$sql1e.'<br>'.$sql1f.'<br>';
                if (!$todebug) {
                    db_query($sql1d);
                    db_query($sql1e);
                    db_query($sql1f);
                }

                $sql2 = t("UPDATE dpl_ntlp_course SET school_term_tid = !TERM, school_year = !YEAR
                    WHERE nid = !NID", array('!NID' => $first_rec, '!TERM' => $st_new, '!YEAR' => $sy_new));
                $sql2a = t("UPDATE dpl_ntlp_course_term SET school_term_tid = !TERM
                    WHERE course_nid = !NID", array('!NID' => $first_rec, '!TERM' => $st_new));
                $sql2b = t("UPDATE dpl_ntlp_course_activity SET school_term_tid = !TERM WHERE course_nid = !NID",
                        array('!NID' => $first_rec, '!TERM' => $st_new));
                echo $sql2.'<br>'.$sql2a.'<br>'.$sql2b.'<br>';
                if (!$todebug) {
                    db_query($sql2);
                    db_query($sql2a);
                    db_query($sql2b);
                }

                //TRIGGER_POINT: UPDATE STUDENT RECORD FOR CURRENT COURSE TERMS
    //            dw_set_allterms_course_grade_toupdate($crec->course_nid);
                echo "<BR>";
            } else {
                echo "#NOT COMPLIANT.........skipping<BR>";
            }
        } else {
            echo "#NOT SAFE.........skipping<BR>";
        }
    }
}

function repair_gradeportal_sw_outcomes($school_nid, $todebug) {
//    $todebug = intval(arg(3)) > 0 ? true : false;
    //Problem: After migration, we saw that the School-Wide Outcomes in the courses migrated from GradePortal
    //are wrong. However the count of outcomes defined in the course is correct and the Sort Order and Weight %
    //reflect correct values as well.
    //Solution: Use the Sort Order field and match it with CAT_NUM field in COURSE_CATINFO and OUTCOME to
    //retrieve correct School-wide outcomes

    //How to detect invalid outcome:
    // --> Its VID is NOT 6

//    $school_nid = 1006;

    $crs = db_query("select c.*, ic.courseid, s.gp_id
        FROM dpl_node n, dpl_ntlp_course c, intg.INT_COURSE ic, intg.INT_SCHOOL s
        WHERE n.nid = c.nid AND c.school_nid = %d
        AND ic.id = c.nid AND ic.type = 'GP'
        AND s.id = c.school_nid", $school_nid);

    echo "######################################################";
    echo "# Repair School-wide Outcomes";
    echo "# DEBUG Mode: " . ($todebug ? "true" : "false") . "<BR>";
    echo "#Starting with $crs->num_rows<BR>";
    while ($crec = db_fetch_object($crs)) {
        $sql = "SELECT co.*, gci.outcomeid, go.cat_num
            FROM dpl_ntlp_gb_course_outcome co, dpl_term_data t, gp.COURSE_CATINFO gci, gp.OUTCOME go
            WHERE co.tid = t.tid AND co.course_nid = %d AND t.vid <> 6
            AND gci.outcomeid = go.id AND co.sort_order = gci.cat_num AND gci.courseid = %d
            ORDER BY co.sort_order";
        $ors = db_query($sql, $crec->nid, $crec->courseid);

        while ($orec = db_fetch_object($ors)) {
            $sql = "SELECT o.tid FROM dpl_ntlp_gb_outcome o, dpl_term_data t
                WHERE o.tid = t.tid AND o.school_nid = %d AND o.sort_order = %d AND t.vid = 6";
            $trs = db_query($sql, $school_nid, $orec->cat_num);

            if ($trs->num_rows > 0) {
                if ($trec = db_fetch_object($trs)) {
                    $sql1 = t("UPDATE dpl_ntlp_gb_course_outcome
                        SET tid = !TID
                        WHERE tid = !OLDTID AND course_nid = !NID",
                        array('!TID' => $trec->tid, '!OLDTID' => $orec->tid,
                            '!NID' => $crec->nid));

                    echo $sql1.'<br>';
                    if (!$todebug) {
                        db_query($sql1);
                    }
                }
            } else {
                echo "#Outcome number:'$orec->cat_num' not found<BR>";
            }
        }
    }
}

function repair_gradeportal_dropouts($school_nid, $todebug) {
//    $todebug = intval(arg(3)) > 0 ? true : false;
    //Problem: We noticed that drop-outs are still enrolled in courses after migration.

    //This script will mark them as dropped students
    // --> ROSTER.DROPPEDFLAG tell us that the student is a dropout.
    $crs = db_query("select c.nid, nu.uid
        FROM dpl_node n, dpl_ntlp_course c, dpl_ntlp_user nu, intg.INT_USER iu, intg.INT_COURSE ic, gp.ROSTER r
        WHERE n.nid = c.nid AND c.school_nid = $school_nid
        AND c.nid = ic.ID AND ic.COURSEID = r.COURSEID AND r.DROPPEDFLAG = 1
        AND nu.uid = iu.ID AND iu.GP_ID = r.STUDENTID
        ");

    echo "######################################################";
    echo "# Updating Course Dropouts";
    echo "# DEBUG Mode: " . ($todebug ? "true" : "false") . "<BR>";
    while ($crec = db_fetch_object($crs)) {
        $sql = "UPDATE dpl_ntlp_course_user SET role_id = NULL WHERE course_nid = %d AND user_uid = %d";
        echo '<br>Course:'.$crec->nid.' Student:'.$crec->uid;
        if (!$todebug) {
            db_query($sql, intval($crec->nid), intval($crec->uid));
        }
    }
    echo '<br>';
}
