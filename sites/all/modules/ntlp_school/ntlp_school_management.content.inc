<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_group', 'filter_functions');
module_load_include('inc', 'ntlp_school', 'data_access');

function manager_users_content() {
    global $base_path;
    include('school_management.template.inc');

     drupal_add_js(drupal_get_path('module', 'ntlp_school') . '/ntlp_school.js');
     drupal_add_css(drupal_get_path('module', 'ntlp_courses_activity') . '/datepicker/jquery.ui.all.css');
    $output = $HTMLform_manage_user;

    $school_nid = get_this_school();
//    $url = $_GET['q'];
//    $args = explode('/', $url);
//    for ($i = 0;
//    $i < count($args);
//    $i++) {
//        if (is_numeric($args[$i])) {
////            if ($last_string_arg == 'node')
//            $school_nid = $args[$i];
//            break;
//        }
//    }

    /* Combo Box for Status START*/
    $status_combo .='<span style="width:125px;float:left" class="comboboxBgLeft"><select name="status" class="my-dropdown" id="status">';
    $status_combo .='<option value="1"> Active (Enabled)</option>';
    $status_combo .='<option value="2"> Active (Disabled)</option>';
    $status_combo .='<option value="0"> Inactive</option>';
//    $status_combo .='<option value="3"> Remove Account</option>';
    $status_combo .='</select></span>';
    /* Combo Box for Status END*/

    /* Combo Box for Roles START*/
    $roles = NTLP_ROLEID_TEACHER .','. NTLP_ROLEID_SITEADMIN .','. NTLP_ROLEID_SITEGUEST .','. NTLP_ROLEID_SCHOOLLEADER .','. NTLP_ROLEID_PARENT .','. NTLP_ROLEID_STUDENT;

    $role_combo .= '<span style="width:100px;" class="comboboxBgLeft"><select name="role" class="my-dropdown" id="role">';
    $role_combo .=   _get_users_role($roles);
    $role_combo .= '</select></span>';

    /* Combo Box for Roles END*/
    $school_nid = get_this_school();
 
    $output = str_replace('@CREATE_USER', l('Create a User','ntlp/school/manage/create/user/'.$school_nid), $output);
    $output = str_replace('@BULK_USER', l('Bulk-create Users','ntlp/school/manage/bulk/user/'.$school_nid), $output);
    $output = str_replace('@DOWNLOAD_USER_LIST', l('Download User List','ntlp/school/management/user/list/'.$school_nid), $output);
    $output = str_replace('@STUDENT_LIST', l('Download User List',''), $output);
    $output = str_replace('@STUDENTS', manage_users($school_nid), $output);
    $output = str_replace('@STATUS', $status_combo , $output);
    $output = str_replace('@BUTTON_STATUS', l('Change Status','',array('attributes' => array( 'class' => 'SubmitAssignmentBtn','onclick' => 'change_user_status('.$school_nid.') ; return false;'))), $output);
    $output = str_replace('@ROLE', $role_combo , $output);
    $output = str_replace('@BUTTON_ROLE', l('Set role','',array('attributes' => array( 'class' => 'SubmitAssignmentBtn','onclick' => 'change_user_role('.$school_nid.') ; return false;'))), $output);
    $output = str_replace('@BASEPATH', $base_path, $output);

    $curDate = date('Y');



//  ADDING FILTER START
    $options = array(
            'Status' => array(
                    '#autoUncheckDefault' => array('value' => false),
                    'Active (Enabled)' => array(
                            'type'=>'checkbox',
                            'group'=>'status',
                            'value' => 'active',
                            'name' => 1,
                            'default' => true,
                    ),
                    'Active (Disabled)' => array(
                            'type'=>'checkbox',
                            'group'=>'status',
                            'value' => 'disabled',
                            'name' => 2,
                    ),
                    'Inactive' => array(
                            'type'=>'checkbox',
                            'group'=>'status',
                            'value' => 'inactive',
                            'name' => 0,
                    ),
            ),
            'Role' => array(
                    'Students' => array(

                            'type'=>'checkbox',
                            'group'=>'role',
                            'value' => 'students',
                            'name' => 0,

                    ),
                    'Parent' => array(

                            'type'=>'checkbox',
                            'group'=>'role',
                            'value' => 'parent',
                            'name' => 1,

                    ),
                    'Staff' => array(

                            'type'=>'checkbox',
                            'group'=>'role',
                            'value' => 'teacher',
                            'name' => 2,

                    ),
                    'Partners' => array(

                            'type'=>'checkbox',
                            'group'=>'role',
                            'value' => 'partner',
                            'name' => 3,

                    ),
                    'School Leader' => array(

                            'type'=>'checkbox',
                            'group'=>'role',
                            'value' => 'school_leader',
                            'name' => 4,

                    ),
                    'Tech Managers' => array(

                            'type'=>'checkbox',
                            'group'=>'role',
                            'value' => 'site_administrator',
                            'name' => 5,

                    ),
            ),

            'Grad Year' => array(

                     $curDate+7 => array(

                            'type'=>'checkbox',
                            'group'=>'gradyear',
                            'value' => $curDate+7,
                            'name' => 0,

                    ),

                    $curDate+6 => array(

                            'type'=>'checkbox',
                            'group'=>'gradyear',
                            'value' => $curDate+6,
                            'name' => 1,

                    ),

                    $curDate+5 => array(

                            'type'=>'checkbox',
                            'group'=>'gradyear',
                            'value' => $curDate+5,
                            'name' => 2,

                    ),

                    $curDate+4 => array(

                            'type'=>'checkbox',
                            'group'=>'gradyear',
                            'value' => $curDate+4,
                            'name' => 3,

                    ),

                    $curDate+3 => array(

                            'type'=>'checkbox',
                            'group'=>'gradyear',
                            'value' => $curDate+3,
                            'name' => 4,

                    ),

                    $curDate+2 => array(

                            'type'=>'checkbox',
                            'group'=>'gradyear',
                            'value' => $curDate+2,
                            'name' => 5,

                    ),

                    $curDate+1 => array(

                            'type'=>'checkbox',
                            'group'=>'gradyear',
                            'value' => $curDate+1,
                            'name' => 6,

                    ),

                    $curDate => array(

                            'type'=>'checkbox',
                            'group'=>'gradyear',
                            'value' => $curDate,
                            'name' => 7,

                    ),

            )

    );

    $opts = render_custom_filter(
            $options,
            array('start' => $HTML_manage_user_school_filter_cellStart, 'end' => $HTML_manage_user_school_filter_cellEnd),
            array( 'template' => $HTML_manage_user_school_filter_heading, 'placeholder' => '@HEADING' ),
            $HTML_manage_user_school_filter_row_end,
            '?q=ntlp/school/management/users/filter',
            'GetUserList'
    );

    $output = str_replace('@FILTER_PLACE_HOLDER',str_replace('@OPTIONS', $opts, $HTML_manage_user_school_filter), $output);

    //  ADDING FILTER END

    return $output . '
        <div id="dialog-message-err" title="Error" style="background-color:#fff;display:none;">
            <p><span class="msg"></span></p>
        </div>
    ';
}

function ntlp_school_management_users_filter() {
    $school_nid = get_this_school();
    $filter = array();
    $roles_arr = array(
        'students' => 5,
        'parent' => 4,
        'teacher' => 6,
        'partner' => 3,
        'school_leader' => 13,
        'site_administrator' => 8,
    );

    $status_arr = array(
        'active' => 1,
        'disabled' => 2,
        'inactive' => 0,
    );

    $roles = array();
    if( isset($_REQUEST['role']) and is_array($_REQUEST['role']) ) {
        foreach( $_REQUEST['role'] as $role ) {
            if( isset( $roles_arr[$role] ) ) {
                $roles[] = $roles_arr[$role];
            }
        }
    }

    $status = array();
    if( isset($_REQUEST['status']) and is_array($_REQUEST['status']) ) {
        foreach( $_REQUEST['status'] as $vstatus ) {
            if( isset( $status_arr[$vstatus] ) ) {
                $status[] = $status_arr[$vstatus];
            }
        }
    }

    $gradYears = array();
    if( isset($_REQUEST['gradyear']) and is_array($_REQUEST['gradyear']) ) {
        $_REQUEST['gradyear'] = array_map('intval', $_REQUEST['gradyear']);
        foreach( $_REQUEST['gradyear'] as $grad ) {
            if( $grad ) {
                $gradYears[] = $grad;
            }
       }
    }


    $filter = array(
        'ROLES' => $roles,
        'STATUS' => $status,
        'GRAD_YEAR' => $gradYears,
    );

    print manage_users($school_nid, $filter);
}


function manage_users($school_nid, $filter = array()) {

    $head = array(
            array('data' => '<a class="fix_protected" onclick="select_all(true); return false;">All</a><span style="color:#000;">&nbsp;|&nbsp;</span><a class="fix_protected" onclick="select_all(false); return false;">None</a>'),
            array('data' => t('Name'), 'field' => 'last_name', 'sort' => 'asc'),
            array('data' => t('User Name'),'field' => 'mail', 'sort' => 'asc'),
            array('data' => t('Role'), 'field' => 'role_name', 'sort' => 'asc'),
            array('data' => t('Grad Year'), 'field' => 'grad_year', 'sort' => 'asc'),
            array('data' => t('Status'), 'field' => 'status', 'sort' => 'asc'),
            array('data' => t('Last Login'), 'field' => 'login', 'sort' => 'asc'),
    );

    $limit = 50;

    $rs = _get_all_school_users($school_nid, $head, $limit, $filter);

    $rows = array();
    while ($record = db_fetch_object($rs)) {

        $last_login = get_last_login_date($record->login);

        if($record->is_active == 1 && $record->status == 1) {
            $status = 'Active';
        } elseif($record->is_active == 1 && $record->status == 0) {
            $status = 'Disabled';
        }else {
            $status = 'Inactive';
        }
        $rows[] = array(
                array('data' => '<input type="checkbox" value = "'.$record->uid.'" name="userid[]" />'.
                                '<input type="hidden" value = "'.$status.'" name="userstatus['.$record->uid.']" />'),
                array('data' => l($record->last_name.', '.$record->first_name,'ntlp/school/manage/user/edit/'.$record->uid,array('attributes' => array('class' => 'greenLink')))),
                array('data' => $record->mail),
                array('data' => $record->role_name),
                array('data' => $record->grad_year),
                array('data' => $status),
                array('data' => $last_login),

        );


    }
    if( empty($rows) ) {
        $rows[] = array(
                array('data' => 'No records found.', 'colspan' => 7, 'style' => 'text-align:center;font-weight:bold'),
        );
    }
    $attributes = array('class' => 'ntlp_table','width' => '100%','cellpadding' => '8');
    $output .= '<div id="GetUserList">';
    $output .= theme('table', $head, $rows,$attributes);
    $output .= theme('pager', NULL, $limit, 0);

    if( $filter ) {
        $output .= "<script type=\"text/javascript\">
                        $('#GetUserList .pager a').click(function(){
                            $.get($(this).attr('href'), function(data){
                                $('#GetUserList').html(data);
                            })
                            return false;
                        });
                    </script>";
    }

    $output .= '</div>';
    return $output;

}


?>
