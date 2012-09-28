<?php

global $user;

$HTMLcontainer = t(<<<END
<div>
    <br clear="all" />
    <div class="BoldrHeading">
    	
    <div style="float:left;"><h3>@MESSAGE</h3><table style="display:none" cellpadding="0" class="bluePopup" cellspacing="0" border="0" width="400px">
<tr>
    <td class="topLeftBluepopup"></td>
    <td class="topCenterBluepopup"></td>
    <td class="topRightBluepopup"></td>
</tr>
<tr>
    <td class="middleLeftBluepopup"></td>
    <td class="middleCenterBluepopup">
    <div style="float:left">
        <b>Select Course Term</b>
        <ul>
          Minimum required data for a project for submission to Project Library is:
<li>Project Title</li>
<li>Project Icon image</li>
<li>Topic/Unit</li>
<li>Summary</li>
<li>At least one Learning Outcome associated with the Project</li>
<li>At least one resource associated with Facilitator Materials Section in the Project Briefcase</li>
<li>At least one other Category Section title with at least one resource associated with that Category in the Project Briefcase</li>
<li>At least one Activity associated with the Project</li>
        </ul>
    </div>
    <div style="float:right">
        <a onclick="javascript:hidePopup();" href="#"><img src="@DRUPAL_BASEthemes/Boldr/Images/DeleteBtn.png" alt="cancel" /></a>
    </div>
    </td>
    <td class="middleRightBluepopup"></td>
</tr>
<tr>
    <td class="bottomLeftBluepopup"></td>
    <td class="bottomCenterBluepopup"></td>
    <td class="bottomRightBluepopup"></td>
</tr>
</table></h1></div>
    </div>
</div>
<br clear=all />
END
);


