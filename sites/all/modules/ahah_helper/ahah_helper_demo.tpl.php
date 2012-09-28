<table cellspacing="0" cellpadding="2" width="100%">

    <tr>
        <td align="left"><strong><?php print t('Usage');?></strong></td>
        <td class="container-inline" >
            <?php print drupal_render($form['billing_info']['usage']); ?>
        </td>
    </tr>
    <tr>
        <td align="left"><strong><?php print t('Company Name ');?></strong></td>

        <td class="container-inline" >
            <?php print drupal_render($form['billing_info']['company_name']); ?>
        </td>
    </tr>
    <tr>
        <td align="left"><strong><?php print t('VAT No');?></strong></td>
        <td class="container-inline" >
            <?php print drupal_render($form['billing_info']['vat']); ?>
        </td>
    </tr>
    <tr>
        <td align="left"><strong><?php print t('First Name ');?></strong></td>
        <td class="container-inline" >
            <?php print drupal_render($form['billing_info']['first_name']); ?>
        </td>
    </tr>

    <tr>
        <td align="left"><strong><?php print t('Last Name ');?></strong></td>
        <td class="container-inline" >
            <?php print drupal_render($form['billing_info']['last_name']); ?>
        </td>
    </tr>
    <tr>
        <td align="left"><strong><?php print t('Address ');?></strong></td>
        <td class="container-inline" >
            <?php print drupal_render($form['billing_info']['address']); ?>
        </td>
    </tr>
    <tr>
        <td align="left"><strong><?php print t('Country ');?></strong></td>
        <td class="container-inline" >
            <?php print drupal_render($form['billing_info']['country']); ?>
        </td>
    </tr>

</table>
<?php

//print_r($form);
//print drupal_render($form);
?>