<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


module_load_include('inc', 'ntlp_school', 'data_access');

function people_block_contents() {
    drupal_add_js(drupal_get_path('module', 'ntlp_blocks') . '/people.js');
    include "people.template.inc";

    global $user, $base_url, $base_path;
    $user_id = $user->uid;

    $output = $HTMLcontainer;

//    $filter_org = renderOrgFilters();
    $filter_role = renderRoleFilters();

    $school_nid = get_user_school($user->uid);
    $myschool = $school_nid . ",";

//Get all schools
    $rs = get_schools();


    $other_schools = '';
    $show_other_schools_checkbox = false;

    while ($rec = db_fetch_object($rs)) {
        if ($rec->nid != $school_nid) {

            $user_permission_record = user_roles(TRUE, $rec->nid . ' share/view profiles across the network');

//            echo 'school nid ' . $rec->nid . ' <br>';
//            print_r($user_permission_record);
//            echo' <br><br>';
            if (isset($user->roles[NTLP_ROLEID_STUDENT])) {

                if (array_key_exists(NTLP_ROLEID_STUDENT, $user_permission_record)) {
                    $show_other_schools_checkbox = true;
                    $other_schools .= $rec->nid . ",";
                }
            } else if (isset($user->roles[NTLP_ROLEID_TEACHER])) {

                if (array_key_exists(NTLP_ROLEID_TEACHER, $user_permission_record)) {
                    $show_other_schools_checkbox = true;
                    $other_schools .= $rec->nid . ",";
                }
            } else if (isset($user->roles[NTLP_ROLEID_SITEADMIN])) {

                if (array_key_exists(NTLP_ROLEID_TEACHER, $user_permission_record)) {
                    $show_other_schools_checkbox = true;
                    $other_schools .= $rec->nid . ",";
                }
            } else if (isset($user->roles[NTLP_ROLEID_SCHOOLLEADER])) {

                if (array_key_exists(NTLP_ROLEID_TEACHER, $user_permission_record)) {
                    $show_other_schools_checkbox = true;
                    $other_schools .= $rec->nid . ",";
                }
            } else if (isset($user->roles[NTLP_ROLEID_NETWORKSTAFF]) || isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])
                    || isset($user->roles[NTLP_ROLEID_NETWORKGUEST])) {
                $other_schools .= $rec->nid . ",";
            }
        }
    }

    $network_school = get_network_school();
    if ($network_school == false) {
        $network_school = 9;
    }

    $output .= '<style>
       .boldr_table {
        margin-top:63px;

       }

       .views-table td {
       width: 33%;
       }

        #lightbox{
            display:none;
            background:#000000;
            opacity:0.8;
            filter:alpha(opacity=80);
            position:absolute;
            top:0px;
            left:0px;
            min-width:100%;
            min-height:100%;
            z-index:1000;
        }
	#lightbox-panel{
            display:none;
            position:fixed;
            top:183px;
            left:61%;
            margin-left: 150px;
            width:103px;
            background:#FFFFFF;
            padding:0px 0px 0px 0px;
            border:2px solid #CCCCCC;
            z-index:1001;
	}
       </style>
        <div id="lightbox-panel" >
            <p><img src="' . $base_path . 'themes/Boldr/Images/ui-anim_basic_16x16.gif" height="16" width="16" />  Loading Data..</p>
	</div>
<div style="position:absolute;top:109px;margin-left:-18px">
    <div class="BoldrBrandCrumb">
        <ul>
            <li><a href="?q=ntlp/home" class="greenLink">Home</a>&nbsp;>&nbsp;</li>
            <li style="color:#009900 !important">People</li>
        </ul>
    </div>

    <div class="BoldrHeading"  style="clear:both;margin-top:20px">
    	<h1 class="pageHeading">People</h1>
    </div>
</div>';

    $NETWORK_NATIONAL = '<div class="ChkBox">
              <!-- <input name="filter-org" type="checkbox" onclick="people_set_org(\'' . $network_school . '\', this.checked)" /> -->
            <input name="filter-org" type="checkbox" onclick="people_set_org(3, this.checked)" />
	Network
          </div>';

    $OTHER_NETWORK_SCHOOL = '<div class="ChkBox">
            <!-- <input name="filter-org" type="checkbox" onclick="people_set_org(\'' . $other_schools . '\', this.checked)" /> -->
            <input name="filter-org" type="checkbox" onclick="people_set_org(2, this.checked)" />
            Other Network Schools
          </div>';

    $network_school = "9,";     
    if (isset($user->roles[NTLP_ROLEID_PARENT])) {
        $output = str_replace('@ROW_ORGANIZATION', "", $output);
        $output = str_replace('@ROLE_HEADING', 'Role', $output);
    }if (isset($user->roles[NTLP_ROLEID_STUDENT]) || isset($user->roles[NTLP_ROLEID_SITEGUEST])) {

        $all_check_box = '<input name="filter-org" type="checkbox" onclick="people_set_org("0, ", this.checked)" /> All';

        $output = str_replace('@ROLE_HEADING', 'Role', $output);
        $html_org = $HTMLorg;
        $html_org = str_replace('@DEFAULT_ORG', 1, $html_org);
        $html_org = str_replace('@ALL_CHECKBOX', '', $html_org);
        $html_org = str_replace('@MYSCHOOL', 1, $html_org);


        $userObj = get_user_role($user->uid);

        if (check_user_permission_for_access($userObj->rid, get_this_school() . ' share/view profiles across the network') && $show_other_schools_checkbox == true) {
            $html_org = str_replace('@OTHER_NETWORK_SCHOOL', $OTHER_NETWORK_SCHOOL, $html_org);
        } else {
            $html_org = str_replace('@OTHER_NETWORK_SCHOOL', '', $html_org);
        }

        $html_org = str_replace('@NETWORK_NATIONAL', '', $html_org);
        $output = str_replace('@ROW_ORGANIZATION', $html_org, $output);
    } else {

        $output = str_replace('@ROLE_HEADING', 'Role', $output);
        $html_org = $HTMLorg;
        $html_org = str_replace('@ALL_CHECKBOX', '', $html_org);
        $html_org = str_replace('@DEFAULT_ORG', 1, $html_org);
        $html_org = str_replace('@MYSCHOOL', 1, $html_org);

        $html_org = str_replace('@OTHER_NETWORK_SCHOOL', $OTHER_NETWORK_SCHOOL, $html_org);

        $html_org = str_replace('@NETWORK_NATIONAL', $NETWORK_NATIONAL, $html_org);
        $output = str_replace('@ROW_ORGANIZATION', $html_org, $output);
    }


    $output = str_replace('@FILTER_ROLES', $filter_role, $output);

    $view_people = get_people_view('1', 'S,0,');
    $output = str_replace('@PEOPLE_VIEW', $view_people, $output);
    return $output;
}

function renderRoleFilters() {
    global $user;
    $rs = get_user_roles();

    $output = '';

    $all_checkbox = '<div class="ChkBox">
                        <input checked="checked" type="checkbox" name="filter-roles" onclick="people_search(this.id)" id="_0" />
                        ALL</div><div class="ChkBox"><font size="3"> |</font> </div>';

    if (isset($user->roles[NTLP_ROLEID_PARENT])) {
//        if ($rec->rid == NTLP_ROLEID_TEACHER || $rec->rid == NTLP_ROLEID_SITEADMIN || $rec->rid == NTLP_ROLEID_SCHOOLLEADER)

        $output .= '<div class="ChkBox">';
        $output .= '<input type="checkbox" checked="checked" name="filter-roles" onclick="people_search(this.id)" id="_' . NTLP_ROLEID_TEACHER . '_' . NTLP_ROLEID_SCHOOLLEADER . '_' . NTLP_ROLEID_SITEGUEST . '_' . NTLP_ROLEID_SITEADMIN . '" >Staff</input>';
        $output .= '</div>';
        $output .= '<div class="ChkBox">';
        $output .= '<input type="checkbox" name="filter-roles" onclick="people_search(this.id)" id="_' . NTLP_ROLEID_PARENT . '" >Parents</input>';
        $output .= '</div>';
    } else if (isset($user->roles[NTLP_ROLEID_STUDENT])) {

        $output .= '<div class="ChkBox">';
        $output .= '<input type="checkbox" checked="checked" name="filter-roles" onclick="people_search(this.id)" id="_' . NTLP_ROLEID_STUDENT . '" >Students</input>';
        $output .= '</div>';
        $output .= '<div class="ChkBox">';
        $output .= '<input type="checkbox"  name="filter-roles" onclick="people_search(this.id)" id="_' . NTLP_ROLEID_TEACHER . '_' . NTLP_ROLEID_SCHOOLLEADER . '_' . NTLP_ROLEID_SITEGUEST . '_' . NTLP_ROLEID_SITEADMIN . '" >Staff</input>';
        $output .= '</div>';
    } else if (isset($user->roles[NTLP_ROLEID_TEACHER]) || isset($user->roles[NTLP_ROLEID_SITEADMIN])
            || isset($user->roles[NTLP_ROLEID_SCHOOLLEADER]) || isset($user->roles[NTLP_ROLEID_NETWORKMANAGER])
            || isset($user->roles[NTLP_ROLEID_NETWORKSTAFF]) || isset($user->roles[NTLP_ROLEID_NETWORKGUEST])) {

        $output .= $all_checkbox;
        $output .= '<div class="ChkBox">';
        $output .= '<input type="checkbox" name="filter-roles" onclick="people_search(this.id)" id="_' . NTLP_ROLEID_STUDENT . '" >Students</input>';
        $output .= '</div>';
        $output .= '<div class="ChkBox">';
        $output .= '<input type="checkbox"  name="filter-roles" onclick="people_search(this.id)" id="_' . NTLP_ROLEID_PARENT . '" >Parent</input>';
        $output .= '</div>';
        $output .= '<div class="ChkBox">';
        $output .= '<input type="checkbox"  name="filter-roles" onclick="people_search(this.id)" id="_' . NTLP_ROLEID_TEACHER . '_' . NTLP_ROLEID_NETWORKMANAGER . '_' . NTLP_ROLEID_NETWORKSTAFF .
                '_' . NTLP_ROLEID_NETWORKGUEST . '" >Staff</input>';
        $output .= '</div>';

        $output .= '<div class="ChkBox">';
        $output .= '<input type="checkbox"  name="filter-roles" onclick="people_search(this.id)" id="_' . NTLP_ROLEID_SITEGUEST . '" >Partners</input>';
        $output .= '</div>';
        $output .= '<div class="ChkBox">';
        $output .= '<input type="checkbox"  name="filter-roles" onclick="people_search(this.id)" id="_' . NTLP_ROLEID_SCHOOLLEADER . '" >School Leaders</input>';
        $output .= '</div>';
        $output .= '<div class="ChkBox">';
        $output .= '<input type="checkbox"  name="filter-roles" onclick="people_search(this.id)" id="_' . NTLP_ROLEID_SITEADMIN . '" >Tech Managers</input>';
        $output .= '</div>';
    } else if (isset($user->roles[NTLP_ROLEID_SITEGUEST])) {

        $output .= '<div class="ChkBox">';
        $output .= '<input type="checkbox"  checked="checked" name="filter-roles" onclick="people_search(this.id)" id="_' . NTLP_ROLEID_STUDENT . '" >Students</input>';
        $output .= '</div>';

        $output .= '<div class="ChkBox">';
        $output .= '<input type="checkbox"  name="filter-roles" onclick="people_search(this.id)" id="_' . NTLP_ROLEID_TEACHER . '" >Staff</input>';
        $output .= '</div>';

        $output .= '<div class="ChkBox">';
        $output .= '<input type="checkbox"  name="filter-roles" onclick="people_search(this.id)" id="_' . NTLP_ROLEID_SITEGUEST . '" >Partners</input>';
        $output .= '</div>';
        $output .= '<div class="ChkBox">';
        $output .= '<input type="checkbox"  name="filter-roles" onclick="people_search(this.id)" id="_' . NTLP_ROLEID_SCHOOLLEADER . '" >School Leaders</input>';
        $output .= '</div>';
        $output .= '<div class="ChkBox">';
        $output .= '<input type="checkbox"  name="filter-roles" onclick="people_search(this.id)" id="_' . NTLP_ROLEID_SITEADMIN . '" >Tech Managers</input>';
        $output .= '</div>';
    }

    return $output;
}

//Receives comma delimitted organization and roles filters
//and a named "search" parameter
function people_search($org, $roles) {
//    print "$org, $roles";
    print get_people_view($org, $roles);
}

function get_people_view($org = '0,', $roles = '0,') {
    global $user;

    $query = '';
    $search_text = $_GET['search'];

    $roles = trim($roles, ', ');
    $roles_r = explode(',', $roles);
    array_shift($roles_r);
    $roles = implode(',', $roles_r);

    watchdog('filter_org', print_r($org, true));
    watchdog('filter_roles', print_r($roles, true));


    $org_array = explode(',', $org);
    $org_array = array_unique($org_array);
    $org_r = implode(',', $org_array);

    watchdog('filter_org1', print_r($org_r, true));

    if (is_null($org_r) || empty($org_r)) {    //comma is used to pass onto views so that it uses the IN statement
        $org_r = '0';
    }

    watchdog("args_agr", $org_r);

    if (in_array('0', $roles_r)) {
        $arg_roles = 'all';
    } else {
        if (empty($roles)) {
            $arg_roles = '0';
        } else {
            $arg_roles = $roles;
        }
    }

    if (isset($user->roles[NTLP_ROLEID_PARENT])) {
        $school_nid = get_user_school($user->uid);

        $staff_ids = NTLP_ROLEID_TEACHER . ',' . NTLP_ROLEID_SITEADMIN . ',' . NTLP_ROLEID_SCHOOLLEADER;
        if ($arg_roles == 'all') {
            $arg_roles = $staff_ids;
        } else {
            $arg_roles = str_replace(NTLP_ROLEID_TEACHER, $staff_ids, $arg_roles);
        }
    } else if (isset($user->roles[NTLP_ROLEID_STUDENT]) || isset($user->roles[NTLP_ROLEID_SITEGUEST])) {

        $school_nid = get_user_school($user->uid);

        $student_ids = NTLP_ROLEID_STUDENT;
        if ($arg_roles == 'all') {
            $arg_roles = $student_ids;
        } else {
            $arg_roles = str_replace(NTLP_ROLEID_STUDENT, $student_ids, $arg_roles);
        }

        /* criteria for teacher and site admin and school leader */
    } else if (isset($user->roles[NTLP_ROLEID_TEACHER]) || (isset($user->roles[NTLP_ROLEID_SITEADMIN]))
            || (isset($user->roles[NTLP_ROLEID_SCHOOLLEADER]))) {

        $staff_ids = NTLP_ROLEID_TEACHER . ',' . NTLP_ROLEID_SITEADMIN . ',' . NTLP_ROLEID_SITEGUEST . ',' . NTLP_ROLEID_SCHOOLLEADER
                . ',' . NTLP_ROLEID_STUDENT . ',' . NTLP_ROLEID_NETWORKMANAGER . ',' . NTLP_ROLEID_NETWORKSTAFF . ',' . NTLP_ROLEID_NETWORKGUEST;
        if ($arg_roles == 'all') {
            $arg_roles = $staff_ids;
        }
    } else if (isset($user->roles[NTLP_ROLEID_NETWORKGUEST]) || (isset($user->roles[NTLP_ROLEID_NETWORKMANAGER]))
            || (isset($user->roles[NTLP_ROLEID_NETWORKSTAFF]))) {

        $staff_ids = NTLP_ROLEID_TEACHER . ',' . NTLP_ROLEID_SITEADMIN . ',' . NTLP_ROLEID_SITEGUEST . ',' . NTLP_ROLEID_SCHOOLLEADER
                . ',' . NTLP_ROLEID_STUDENT . ',' . NTLP_ROLEID_NETWORKMANAGER . ',' . NTLP_ROLEID_NETWORKSTAFF . ',' . NTLP_ROLEID_NETWORKGUEST;
        if ($arg_roles == 'all') {
            $arg_roles = $staff_ids;
        }
    }
    watchdog('filter_org2', print_r($org_r, true));
    watchdog('filter_roles2', print_r($arg_roles, true));

    if (!isset($user->roles[NTLP_ROLEID_PARENT]) && !isset($user->roles[NTLP_ROLEID_SITEGUEST])) {
        if ($org_r == 0) {
            $my_school_condition = "AND ntlp_user.school_nid = 0";
        } else {
            $my_school_condition = "AND ntlp_user.school_nid = " . get_user_school($user->uid);
        }
    } else {
        $org_r = 1;
        $my_school_condition = "AND ntlp_user.school_nid = " . get_user_school($user->uid);
    }

    if (in_array(array('1', '2'), $org_array)) {
        $other_school_check = "";
    } else {
        $other_school_check = "AND ntlp_user.school_nid <> " . get_user_school($user->uid);
    }

    //Do NOT show test school users
    if (get_user_school($user->uid) != 33) {
        $other_school_check .= ' AND ntlp_user.school_nid <> 33 ';
    }

    $NETWORK_National_nid = get_network_school();
    if ($NETWORK_National_nid == false) {
        $NETWORK_National_nid = 1523;
    }

    if (isset($search_text) && !is_null($search_text) && $search_text != 'WHO ARE YOU LOOKING FOR?') {
        $query = 'SELECT * FROM ( ';
    }

    $query .= "SELECT users.uid AS uid, ntlp_user.school_nid, CONCAT(ntlp_user.first_name,' ', ntlp_user.last_name) AS user_name
         FROM dpl_users users
         LEFT JOIN dpl_ntlp_user ntlp_user ON users.uid = ntlp_user.uid
         INNER JOIN dpl_users_roles users_roles ON users.uid = users_roles.uid
         INNER JOIN dpl_permission p ON p.rid = users_roles.rid
          WHERE (ntlp_user.is_active = 1)
             AND (users_roles.rid IN (" . $arg_roles . "))
             AND ((1 IN (" . $org_r . ") " . $my_school_condition . " )
		OR (2 IN (" . $org_r . ") " . $other_school_check . " AND p.perm LIKE CONCAT('%', CONVERT(ntlp_user.school_nid, CHAR(10)), ' SHARE/VIEW PROFILES across the network%'))
		OR (3 IN (" . $org_r . ") AND users_roles.rid IN (" . NTLP_ROLEID_NETWORKSTAFF . "," . NTLP_ROLEID_NETWORKMANAGER . ") AND ntlp_user.school_nid = " . $NETWORK_National_nid . "))

         ORDER BY ntlp_user.last_name ASC, ntlp_user.first_name ASC";

    if (isset($search_text) && !is_null($search_text) && $search_text != 'WHO ARE YOU LOOKING FOR?') {
        $query .= " ) AS record WHERE record.user_name LIKE '%" . $search_text . "%'";
    }
    watchdog("people_quey", $query);
    $result = db_query($query);

    if ($result->num_rows > 0) {
        $i = 1;
        while ($data = db_fetch_array($result)) {
            if ($result->num_rows > $i) {
                $user_record .= $data['uid'] . '+';
            } else {
                $user_record .= $data['uid'];
            }
            $i++;
        }

        $display_id = 'default';
        return views_embed_view('ntlp_people_search', $display_id, $user_record);
    } else {
        $display_id = 'default';
        return views_embed_view('ntlp_people_search', $display_id, 0);
    }
}
