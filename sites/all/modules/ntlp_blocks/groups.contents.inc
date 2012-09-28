<?php
function recent_groups_block_contents() {
    global $user;

//    $groups = ($g = _get_recent_groups_activity( $user->uid, true )) ? $g : _get_recent_groups_activity( $user->uid, false );
    $groups = _get_recent_groups_activity( $user->uid );

    include('misc.template.inc');
    $output = $HTMLGroups;

    $tempRow = '';
    if(!empty($groups)) {
        foreach( $groups as $group ) {
            $tempRow .= $HTMLGroups_row;
            $tempRow = str_replace( '@GROUP_TITLE', l($group->title, 'ntlp/groups/home/'.$group->nid), $tempRow );
            $tempRow = str_replace( '@NEW_COUNT', $group->new_count, $tempRow );
            $tempRow = str_replace( '@NEW_POST', 'New Posts', $tempRow );
            $tempRow = str_replace( '@NO_GROUP_FOUND', '', $tempRow );
        }
    }else {

        $tempRow .= $HTMLGroups_row;
        $tempRow = str_replace( '@GROUP_TITLE', '', $tempRow );
        $tempRow = str_replace( '@NEW_COUNT', '', $tempRow );
        $tempRow = str_replace( '@NEW_POST', '', $tempRow );
        $tempRow = str_replace( '@NO_GROUP_FOUND', 'You are not a member of any groups', $tempRow );

    }
    $output = str_replace('@VIEW_ALL_GROUPS', url('ntlp/groups/all'),  $output);
    $output = str_replace('@GROUPS_ROW_PLACEHOLDER', $tempRow, $output);

    return $output;
}



?>
