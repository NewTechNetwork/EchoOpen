<?php

$HTMLcontainer = t(<<<END
<div>
    <div class="BoldrBrandCrumb">
    <ul>
        <li>@LINK_HOME >&nbsp;</li>
        <li>@LINK_LIBRARY >&nbsp;</li>
        <li>@LINK_RESOURCE_LIBRARY >&nbsp;</li>
 
    </ul>
    </div>
    <br clear="all" />
    <div style="float:left"><h1 style="padding:0px;margin:0px;font-size:24px;color:#3570AA">@TITLE</h1></div>
    <div class="BoldrHeading" style="float:right">
        <div class="big_Btn" ><table align="right"><tr><td>@ADD_RESOURCE_BTN</td><td>@MANAGE_BTN</td></tr></table></div>
    </div>
</div>
<br clear=all />
END
);

?>
