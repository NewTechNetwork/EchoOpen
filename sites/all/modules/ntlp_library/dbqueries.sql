CREATE TABLE `devdpl`.`dpl_ntlp_module` (
`nid` INT( 10 ) NOT NULL ,
`icon_fid` INT( 10 ) NOT NULL
) ENGINE = MYISAM ;








CREATE TABLE `devdpl`.`dpl_ntlp_library_project` (
`nid` int(10) NOT NULL,
  `module_nid` int(10) NOT NULL,
  `submitted_on` datetime NOT NULL,
  `submitted_by` int(10) NOT NULL,
  `published_on` datetime NOT NULL,
  `published_by` int(10) NOT NULL,
  `archived_on` datetime NOT NULL,
  `archived_by` int(10) NOT NULL,
  `is_exemplary` int(10) NOT NULL,
  `src_project_nid` int(10) NOT NULL,
  `outcomes` varchar(255) NOT NULL,
  `teacher_comment_cid` int(10) NOT NULL,
  `reviewer_comment_cid` int(10) NOT NULL
) ENGINE=MyISAM DEFAULT CHARSET=latin1;



CREATE TABLE `devdpl`.`dpl_ntlp_library_project_usage` (
`src_project` INT( 10 ) NOT NULL ,
`action` VARCHAR( 1 ) NOT NULL ,
`action_by` INT( 10 ) NOT NULL ,
`action_on` DATETIME NOT NULL ,
`target_project` INT NOT NULL
) ENGINE = MYISAM ;







ALTER TABLE  `dpl_ntlp_library_project` ADD  `is_deleted` INT( 1 ) NOT NULL AFTER  `archive_notes` ,
ADD  `deleted_by` INT( 10 ) NOT NULL AFTER  `is_deleted` ,
ADD  `deleted_on` DATETIME NOT NULL AFTER  `deleted_by`