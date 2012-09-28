<div id="course-activity-wrapper">
<table cellspacing="0" cellpadding="2" border="1" width="100%">
    <tr>
        <td><strong><?php print t('Activity Name');?></strong></td>
        <td class="container-inline" ><?php print drupal_render($form['course_activity']['activityname']); ?> </td>
    </tr>
    <tr>
        <td><strong><?php print t('Activity Type');?></strong></td>
        <td class="container-inline" ><?php print drupal_render($form['course_activity']['coursetype']); ?> </td>
    </tr>
    <tr>
        <td><strong><?php print t('Submission Type');?></strong></td>
        <td class="container-inline" ><?php print drupal_render($form['course_activity']['submissiontype']); ?> </td>
    </tr>
    <tr>
        <td><strong><?php print t('Allow Submission reviews');?></strong></td>
        <td class="container-inline" ><?php print drupal_render($form['course_activity']['allow_submission_review']); ?> </td>
    </tr>
    <tr>
        <td><strong><?php print t('Availability Mode');?></strong></td>
        <td class="container-inline" ><?php print drupal_render($form['course_activity']['availabilitymode']); ?> </td>
    </tr>
    <tr>
        <td><strong><?php print t('Due Date');?></strong></td>
        <td class="container-inline" >
            <?php print drupal_render($form['course_activity']['duedate']); ?> &nbsp;
            <?php print drupal_render($form['course_activity']['hour']); ?> &nbsp;
            <?php print drupal_render($form['course_activity']['minute']); ?> &nbsp;
            <?php print drupal_render($form['course_activity']['time']); ?> &nbsp;
        </td>
    </tr>
    <tr>
        <td><strong><?php print t('Availability');?></strong></td>
        <td class="container-inline" ><?php print drupal_render($form['course_activity']['availability']); ?> </td>
    </tr>
    <tr>
        <td><strong>ffff<?php print t('Description');?></strong></td>
        <td class="container-inline" ><?php print drupal_render($form['course_activity']['description']); ?> </td>
    </tr>
    <tr>
        <td colspan="2"><hr/></td>
    </tr>
    <tr>
        <th colspan="2"><h2><?php print t('Grading Options');?></h2></th>
    </tr>
    <tr>
        <td><strong><?php print t('Graded?');?></strong></td>
        <td class="container-inline" ><?php print drupal_render($form['course_activity']['is_graded']); ?> </td>
    </tr>
    <tr>
        <td><strong><?php print t('Grading Method');?></strong></td>
        <td class="container-inline" ><?php print drupal_render($form['course_activity']['gradingmethod']); ?> </td>
    </tr>
    <tr>
        <th><strong><?php print t('Points');?></strong></th>
    </tr>
    <tr>
        <th colspan="2" ><h2><?php print t('Curriculum Mapping');?></h2></th>
    </tr>
    <tr>
        <td><strong><?php print t('Unit');?></strong></td>
        <td class="container-inline" ><?php print drupal_render($form['course_activity']['unit']); ?> </td>
    </tr>
    <tr>
        <td><strong><?php print t('project');?></strong></td>
        <td class="container-inline" >
            <table width="100%" >
                <tr>
                    <td><?php print drupal_render($form['course_activity']['project']); ?></td>
                    <td align="right" width="100%"><strong><?php print t('Project Section');?></strong> </td>
                    <td align="right" ><?php print drupal_render($form['course_activity']['projectsection']); ?> </td>
                </tr>
            </table>
        </td>
    </tr>
    <tr>
        <td><strong><?php print t('Academic Standards');?></strong></td>
        <td class="container-inline" ><?php print drupal_render($form['course_activity']['academicstandards']); ?> </td>
    </tr>
    <tr>
        <th><strong><?php print t('Students');?></strong></th>
    </tr>
    <tr>
        <td><strong><?php print t('Assign To');?></strong></td>
        <td class="container-inline" ><?php print drupal_render($form['course_activity']['assigntostudents']); ?> </td>
    </tr>
    <tr>
        <td colspan="2" class="container-inline" ><?php
            $view_course_students = views_embed_view('enrolled_course_students');
            print $view_course_students;
            ?>
        </td>
    </tr>
    <tr>
        <td><?php print drupal_render($form['course_activity']['submit']); ?></td>
    </tr>
</table>
<?php
print drupal_render($form);
?>
</div>