<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


//Save a single outcome score of a student for an activity in cache
function save_score_in_cache($teacher_uid, $activity_nid, $activity_user, $outcome_tid, $userinput, $type) {

    $key = "GRADEALL_SCORE:$teacher_uid:$activity_nid:$activity_user";
    $cache_score_obj = cache_get($key, 'ntlp_cache');
    if ($cache_score_obj) {
        $cache_score = $cache_score_obj->data;
    }
    if (!is_object($cache_score)) {
        $cache_score = new stdClass();
    }
    if (is_object($cache_score)) {
        $cache_score->teacher_uid = $teacher_uid;
        $cache_score->activity_nid = $activity_nid;
        $cache_score->activity_user = $activity_user;
        $cache_score->saved = false;
        
        //We don't want to overwrite 'type' with 'late', if a value like 'score' or 'incomplete' already exists
        if (!isset($cache_score->type) && $type == 'late') {
            $cache_score->type = $type;         //late
        } else {
            if ($type != 'late') {
                $cache_score->type = $type;     //score/incomplete
            }
        }
        if ($type == 'late') {
            $cache_score->is_late = $userinput;     //score/incomplete
        }
        else
        {
            if (!is_array($cache_score->value)) {
                $cache_score->value = array();
            }

            if ($type == 'score') {
                $cache_score->value[$outcome_tid] = $userinput;
            } else {
                $cache_score->value[$type] = $userinput;
            }
        }
        cache_set($key, $cache_score, 'ntlp_cache');
    }


}

function clearcache_score_single_student_for_activity($teacher_uid, $activity_nid, $user_uid) {
    //Clear Scores from memcache
    $late_cached = false;
    $key = "GRADEALL_SCORE:$teacher_uid:$activity_nid:$user_uid";
    $cache_score_obj = cache_get($key, 'ntlp_cache');
    if ($cache_score_obj) {
        $cache_score = $cache_score_obj->data;
        if (is_object($cache_score)) {
            if (!$cache_score->saved) {
                if (isset($cache_score->is_late)) {
                    $is_late = intval($cache_score->is_late);
                    $late_cached = true;
                }
                if ($cache_score->type != 'late') {
                    $cache_score->type = 'late';
                    unset($cache_score->value);
                }
            }
        }
    }
    if ($late_cached) {
        //If Late mark found, then preserve cache value, but remove other scores
        cache_set($key, $cache_score, 'ntlp_cache');
    } else {
        //Remove cache completely from memory
        cache_set($key, null, 'ntlp_cache', CACHE_TEMPORARY);
    }
}


function save_grades_from_cache($teacher_uid, $activity_id, $student_uid, $outcome_array, $grading_method) {
    //Check if there is unsaved score in the cache
    $cached = false;
    $late_cached = false;
    $inc_cached = false;
    unset($cache_score);
    $score_in_cache = "";

    $key = "GRADEALL_SCORE:$teacher_uid:$activity_id:$student_uid";
    $cache_score_obj = cache_get($key, 'ntlp_cache');
//    print $key; print_r($cache_score_obj->data);
    if ($cache_score_obj) {
        $cache_score = $cache_score_obj->data;
        if (is_object($cache_score)) {
            if (!$cache_score->saved) {
                if (isset($cache_score->is_late)) {
                    $is_late = intval($cache_score->is_late);
//                            update_activity_late_status($teacher_uid, $subid, $sub_rec->date_submitted, $is_late);
                    $late_cached = true;
                }

                $grade_id = save_activity_user_grade($activity_id, $student_uid, null, $teacher_uid, null, null);

                if (isset($cache_score->value)) {
                    foreach ($cache_score->value as $outcome_tid => $score_in_cache) {
                        if ($cache_score->type == 'score') {
                            $cached = true;
                            $is_incomplete = false;
                            if (ntlp_grade_all_save_activity_scores($grade_id, $outcome_tid, $score_in_cache)) {
                                //Clear the saved outcome score from array
//                                    unset($cache_score->value[$outcome_tid]);
                            }
                        } else {
                            if ($cache_score->type == 'incomplete') {
                                $is_incomplete = $cache_score->value['incomplete'];
                                
                                if ($is_incomplete) {
                                    if ($outcome_array != null) {
                                        
                                        $outcomes = array();
                                        foreach ($outcome_array as $outcome_obj) {
                                            $outcomes[$outcome_obj->tid] = 0;
                                        }
                                        save_activity_user_grade_scores($grade_id, $outcomes, false);
                                    }
                                } else {
                                    if($grading_method == 'C') {
                                        auto_grade_maximum_to_user_submission($grade_id, $outcome_array, false);
                                    }
                                }
                                
                                $inc_cached = true;
                            }
                        }
                    }
                }

                save_activity_user_grade($activity_id, $student_uid, null, $teacher_uid,
                                        ($inc_cached || $cached ? ($is_incomplete ? 0 : 1) : null), 
                                        ($late_cached ? ($is_late ? 1 : 0) : null));

                cache_set($key, null, 'ntlp_cache', CACHE_TEMPORARY);
            }
        }
    }

    return ($cached || $late_cached || $inc_cached);
}