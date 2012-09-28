<?php
// ---------- DataWarehouse Table Course Grade
function ntlp_views_data_dw_course_grade() {
  $data = array();
  $tablename = 'ntlp_dw_course_grade';
  $data[$tablename]['table']['group']  = t('NTLP');
  $data[$tablename]['table']['base'] = array(
    'field' => 'course_nid',
    'title' => 'NTLP DataWarehouse Course Grades',
    'help' => t("Provides access to pre-calculated grades of a course-term for a student."),
    'weight' => 0,
  );

  $data[$tablename]['table']['join'] = array(
    'node' => array(
      'handler' => 'views_join', // this is actually optional
      'left_table' => 'node', // Because this is a direct link it could be left out.
      'left_field' => 'nid',
      'field' => 'nid',
//      'type' => 'INNER',
    ),
//    'node_revisions' => array(
//      'left_field' => 'nid',
//      'field' => 'nid',
////      'type' => 'INNER',
//    ),
  );
  $data[$tablename]['uid'] = array(
    'title' => t('DW: Student'),
    'help' => t('Student'),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'relationship' => array(
      'title' => t('DW: Student'),
      'help' => t("Bring in information about a student."),
      'base' => 'ntlp_user',
      'base field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('Student'),
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
  );
  $data[$tablename]['course_nid'] = array(
    'title' => t('DW: Course Node ID'),
    'help' => t('NTLP Course Node identification'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => FALSE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  $data[$tablename]['period'] = array(
    'title' => t('DW: Student Course Period'),
    'help' => t('NTLP Course period for the student'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
  );
  $data[$tablename]['school_nid'] = array(
    'title' => t('DW: School Node ID'),
    'help' => t('NTLP Course School Node identification'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => FALSE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'relationship' => array(
      'title' => t('DW: School Node'),
      'help' => t("Bring in information about node for a course's School."),
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('School'),
    ),
  );
  $data[$tablename]['school_term_tid'] = array(
    'title' => t('DW: Course Term Taxonomy ID'),
    'help' => t('NTLP Course School Term Node identification'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => FALSE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'relationship' => array(
      'title' => t('DW: School Term'),
      'help' => t("Bring in information about node for a Course's School Term."),
      'base' => 'ntlp_school_term',
      'field' => 'tid',
      'handler' => 'views_handler_relationship',
      'label' => t('School Term'),
    ),
  );
  $data[$tablename]['is_updated'] = array(
    'title' => t('DW: Is Updated'),
    'help' => t('Is this record updated.'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => FALSE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Is this Updated'),
      'type' => 'yes-no',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data[$tablename]['grade_letter'] = array(
    'title' => t('DW: Overall Grade Scale'),
    'help' => t('Shows Grade Scale/Level for grades students have earned in all the activities in a course-term.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data[$tablename]['grade_percent'] = array(
    'title' => t('DW: Overall Grade Percentage'),
    'help' => t('Shows percentage of grades students have earned in all the activities in a course-term.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data[$tablename]['outcome_tid'] = array(
    'title' => t('DW: Course-Term Outcome'),
    'help' => t('Course-Term wise Outcome.'),
    'field' => array(
      'handler' => 'ntlp_handler_field_dw_cg_outcome',
      'notafield' => TRUE,
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  return $data;
}
