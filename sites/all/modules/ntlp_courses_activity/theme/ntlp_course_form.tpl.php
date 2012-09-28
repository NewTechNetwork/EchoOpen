<table cellspacing="0" cellpadding="2" width="100%">
    <tr>
        <td align="left"><strong><?php print t('Activity Name ');?></strong></td>

        <td  colspan="2" class="container-inline" >
            <?php print drupal_render($form['course_wrapper']['activityname']); ?>
        </td>

    </tr>
    <tr>
        <td align="left"><strong><?php print $form['course_wrapper']['coursetype']['#name'];?></strong></td>
        <td colspan="2" class="container-inline" >
            <?php print drupal_render($form['course_wrapper']['coursetype']); ?>
        </td>   
    </tr>
    <?php if($form['course_wrapper']['coursetype']['#value'] == 'Agenda') {?>
    <tr>
        <td><strong><?php print t('Submission Type');?></strong></td>

    </tr>

        <?php } ?>
    <tr>
        <td><strong><?php print t('Submission Type');?></strong></td>
        <td class="container-inline" ><?php print drupal_render($form['course_wrapper']['submissiontype']); ?> </td>
    </tr>
    <tr>
        <td colspan="3" >
            <?php print drupal_render($form['course_wrapper']['div']); ?>
        </td>
    </tr>
    <tr>
        <td colspan="2" class="container-inline" ><?php
            $view_course_students = views_embed_view('ntlp_course_students');
            print $view_course_students;
            ?>
        </td>
    </tr>

</table>
<?php

//print_r($form);
//print drupal_render($form);
?>