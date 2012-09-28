<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTMLheader = t(<<<END

END
);

$HTMLtabs = t(<<<END

<div class="Agenda">
	<div class="AgendaContents">
		<table border="0" cellpadding="0" class="boldr_table" cellspacing="0" width="100%">
                    <tbody>
                    <tr>
                        <td class="contentTopLeftbg">&nbsp;</td>
                        <td class="contentTopCenterbg">&nbsp;</td>
                        <td class="contentTopRightbg">&nbsp;</td>
                    </tr>
                    </tbody>
		</table>
	</div>
</div>

END
);


$HTMLschools = t(<<<END
<div id="tools-school-selector" style="float:right;margin-left:1em;margin-right:1em; margin-top:5px;">
    <!-- TITLE START --><span style="float: left; line-height: 20px; font-size: 14px; color: #5E8FAD;">School:&nbsp;</span><!-- TITLE END -->
    <!-- WRAPPER START -->
    <span class="comboboxBgLeft" style="width:310px;">
    <!-- WRAPPER END -->
        <select class="my-dropdown" onchange="onchange_schoolselector(this.value)">
            @SCHOOL_OPTIONS
        </select>
    <div style="font-size:5px;height:5px">&nbsp;</div>
    </span>
</div>
END
);


?>
