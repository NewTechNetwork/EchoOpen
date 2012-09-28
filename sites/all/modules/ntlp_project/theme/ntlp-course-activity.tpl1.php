<!--// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.
 -->
<div id="course-activity-wrapper">
<table cellspacing="0" cellpadding="2" border="1" width="100%">
    <tr>
        <td><strong><?php print t('Activity Name:');?></strong></td>
        <td class="container-inline" ><?php print $activityname; ?> </td>
    </tr>
    <tr>
        <td><strong><?php print t('Activity Type');?></strong></td>
        <td class="container-inline" ><?php print $activitytype; ?> </td>
    </tr>
    <tr>
        <td><strong><?php print t('Submission Type');?></strong></td>
        <td class="container-inline" ><?php print submissiontype; ?> </td>
    </tr>
    <tr>
        <td><strong><?php print t('Allow Submission reviews');?></strong></td>
        <td class="container-inline" ><?php print $allow_submission_review; ?> </td>
    </tr>
    <tr>
        <td><strong><?php print t('Availability Mode');?></strong></td>
        <td class="container-inline" ><?php print $availabilitymode; ?> </td>
    </tr>
    <tr>
        <td><strong><?php print t('Due Date');?></strong></td>
        <td class="container-inline" >
            <?php print $duedate; ?> &nbsp;
        </td>
    </tr>
    <tr>
        <td><strong><?php print t('Availability');?></strong></td>
        <td class="container-inline" ><?php print $availability; ?> </td>
    </tr>
    <tr>
        <td><strong><?php print t('Description');?></strong></td>
        <td class="container-inline" ><?php print $description; ?> </td>
    </tr>
    <tr>
        <td colspan="2"><hr/></td>
    </tr>
    <tr>
        <th colspan="2"><h2><?php print t('Grading Options');?></h2></th>
    </tr>
    <tr>
        <td><strong><?php print t('Graded?');?></strong></td>
        <td class="container-inline" ><?php print $is_graded; ?> </td>
    </tr>
    <tr>
        <td><strong><?php print t('Grading Method');?></strong></td>
        <td class="container-inline" ><?php print $gradingmethod ?> </td>
    </tr>
    <tr>
        <th><strong><?php print t('Points');?></strong></th>
    </tr>
    <tr>
        <th colspan="2" ><h2><?php print t('Curriculum Mapping');?></h2></th>
    </tr>
    <tr>
        <td><strong><?php print t('Unit');?></strong></td>
        <td class="container-inline" ><?php print $unit; ?> </td>
    </tr>
    <tr>
        <td><strong><?php print t('project');?></strong></td>
        <td class="container-inline" >
            <table width="100%" >
                <tr>
                    <td><?php print $project ?></td>
                    <td align="right" width="100%"><strong><?php print t('Project Section');?></strong> </td>
                    <td align="right" ><?php print $projectsection ?> </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td><strong><?php print t('Academic Standards');?></strong></td>
        <td class="container-inline" ><?php print $academicstandards; ?> </td>
    </tr>
    <tr>
        <th><strong><?php print t('Students');?></strong></th>
    </tr>
    <tr>
        <td><strong><?php print t('Assign To');?></strong></td>
        <td class="container-inline" ><?php print $assignedtostudents ?> </td>
    </tr>

    <tr>
        <td><?php print $submit; ?></td>
    </tr>
</table>
<?php
    //print drupal_render($form);
?>
    <div>Inside DIV</div>
</div>
<div>Yes this is our theme!</div>