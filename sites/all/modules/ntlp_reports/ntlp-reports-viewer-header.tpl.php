<?php
    global $base_path;
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en" dir="ltr">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link type="text/css" rel="stylesheet" media="all" href="<?php print $base_path; ?>themes/Boldr/style.css" />
</head>
<body>
    <div style="float:left;margin:10px;"><?php print $logo; ?></div>
    <div style="float:left;color:#3570AA;font-size:24px;font-weight:bold;margin:20px;"><?php print $title; ?></div>
    <?php if ($has_csv == 1) {?>
    <div style="float:right;margin:20px;">
        <a style="font-weight:bold;" class="SubmitAssignmentBtn" href="<?php print $base_path; ?>?q=ntlp/reports/<?php print $report_nid; ?>/CSV">
        Download CSV</a>
    </div>
    <?php }?>
</body>
</html>