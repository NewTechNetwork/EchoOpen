<?php

$HTMLcontainer = t(<<<END
<div>
    <div class="BoldrBrandCrumb">
    <ul>
        <li>@LINK_HOME >&nbsp;</li>
        <li>@LINK_LIBRARY >&nbsp;</li>
        <li>@LINK_DOCUMENT_LIBRARY >&nbsp;</li>
 
    </ul>
    </div>
    <br clear="all" />
    <h1 style="color:#3570AA;padding:0px;margin:0px;float:left;font-weight:bold;font-size:21px">@TITLE</h1>
    <div class="BoldrHeading" style="float:right;width:500px">
    	@DOCUMENT_LIBRARY_TOOLS
      
        <div class="big_Btn" style="float:inherit" >@ADD_RESOURCE_BTN @MANAGE_BTN</div>
    </div>
</div>
<br clear=all />
END
);

?>
