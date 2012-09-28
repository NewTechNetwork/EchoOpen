<?php
$HTML_ERRORPAGE_container = t(<<<END
   <html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en" class="Gecko ff3 ff35 ff36">
      <head><script type="text/javascript"></script>

    </head>
    <body id="bd" class="">
        <script type="text/javascript">
//setTimeout('window.location="http://www.google.com"',10000)
</script>
<div id="err">
            <div id="head">
<center>
            <img src="@ROOTthemes/Boldr/Images/common/oops.jpg" alt="opps!"/>
            <div class="loginpage" style="display:table;width:auto">
               <div style="" class="greenmsgbox">
        <div class="greenmsgbox_left">
            <div class="greenmsgbox_right">
                <div style="padding:9px 10px 0 10px;">
                    @MESSAGE
                </div>
            </div>
        </div>
    </div>

            </div>
            <div class="BoldrBrandCrumb" style="color:#090;font-weight:bold">« @BACKURL</div>
</center>
</div>

    </body>
</html>
END
);


