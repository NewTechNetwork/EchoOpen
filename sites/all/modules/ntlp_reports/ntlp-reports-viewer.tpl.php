<?php
    global $base_path;
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="en" xml:lang="en" dir="ltr">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<link rel="shortcut icon" href="/sites/default/files/boldr_favicon_0.ico" type="image/x-icon" />
<title>Reports Viewer | echo</title>
<script type="text/javascript">

        var _gaq = _gaq || []; // please see this page http://code.google.com/apis/analytics/docs/gaJS/gaJSApi_gaq.html
        _gaq.push(['_setAccount', 'UA-5818281-61']);
        (function() {
            var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
            ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
            var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);
        })();

        _gaq.push(['_setCustomVar',
            1,         // This custom var is set to slot #1.  Required parameter.
            'School',  // The name of the custom variable.  Required parameter.
            'TV-Test_33_', // The value of the custom variable. Required parameter.

            3          // Sets the scope to visitor-level.  Optional parameter.
        ]);
          _gaq.push(['_trackPageview']);

</script>
</head>
<frameset rows="90,98%">
    <frame src="<?php print $base_path; ?>?q=ntlp/reports/previewheader/<?php print $report_nid; ?>/<?php print $has_csv; ?>/<?php print $title; ?>" noresize="0" frameborder="0" scrolling="no">
    <frame src="<?php print $url; ?>" noresize="0" frameborder="0">
</frameset>
</html>