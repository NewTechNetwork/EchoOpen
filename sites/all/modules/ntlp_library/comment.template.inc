<?php

$HTMLComment = t(<<<END
<div class="LightGreenBox">
<table cellpadding="0" style="background-color:#d3ffc2" cellspacing="0" border="0" width="100%">
	<tr>
		<td class="LightGreenBoxTopLeft" style="border:none"></td>
		<td style="border:none"></td>
		<td class="LightGreenBoxTopRight" style="border:none"></td>
	</tr>
	<tr>
		<td style="border:none"></td>
		<td style="border:none">
			<table cellpadding="0" cellspacing="0" border="0" width="100%">
			<tr valign="top">
				<td style="border:none" class="LightGreenBoxPerson">@POSTED_BY_IMAGE</td>
                                <td style="border:none" class="LightGreenBoxText">
                                <h6>@HEADING<span>@COMMENTS</span></h6>
                                <p>@DATEDIFF</p>
                                </td>
			</tr>
			</table>
		</td>
		<td style="border:none"></td>
	</tr>
	<tr>
		<td style="border:none" class="LightGreenBoxBottomRight"></td>
		<td style="border:none"></td>
		<td style="border:none" class="LightGreenBoxBottomRight"></td>
	</tr>
</table>
</div>
END
);


$HTMLNewComment = t(<<<END
<div class="clearAll"></div>
<div class="AgendaTextArea">
<textarea rows="3" cols="70" name="comments"></textarea>
</div>
<div align="right" class="CommentBtn">
<a href="#" ></a>
</div>
END
);

$HTMLform_projectDetail_hdr = t(<<<END
<div id="ActivitiesCSS">
    <div class="LightBlueBottomRightCorner">
            <div class="LightBlueBottomLeftCorner">
                    <div class="LightBlueTopRightCorner">
                            <div class="LightBlueTopLeftCorner">
                            <div class="ActivitiesAssignment">

END
);

// End Of Field Set
$HTMLform_projectDetail_ftr = t(<<<END
                                </div>
                            </div>
			</div>
		</div>
	</div>
</div>
END
);

$HTMLRecentCommentWrapper = t(<<<EOD
   <br clear="all" />
   <div style="font-size:10px">&nbsp;</div>
<div style="width: 614px;position:relative" class="network_blue_border_middle_right">
<div class="network_blue_border_middle_left">
<div class="network_blue_border_bottom_center">
<div class="network_blue_border_top_center">
<div class="network_blue_border_top_left">
                    <div class="network_blue_border_top_right">
                    <div class="network_blue_border_bottom_left">
                    <div class="network_blue_border_bottom_right"><div>
                    <div>
                     <div>
                                <h1 style="color:#3570AA;font-size:15px;padding:0px;margin:5px 0 5px 0;">Recent Comments on Projects</h1>

                            </div>
                 @COMMENTS_PLACEHOLDER
                    </div>
              </div></div></div></div></div></div></div></div></div>
EOD
);

$HTMLRecentComment = t(<<<EOD
 
<table cellpadding="0" style="background-color:#d3ffc2" cellspacing="0" border="0" width="99%">
	<tbody><tr>
		<td class="LightGreenBoxTopLeft"></td>
		<td></td>
		<td class="LightGreenBoxTopRight"></td>
	</tr>
	<tr>
		<td></td>
		<td>
   <table cellpadding="0" cellspacing="0" border="0" width="100%">
			<tbody><tr valign="top">
				<td class="LightGreenBoxPerson"><img src="@USER_PICTURE"  width="32px" /></td>
                                <td valign="middle" class="LightGreenBoxText">
                                <h6>@USER_NAME<span></h6>
                                </td><td style="width:450px">
                                <b>@PROJECT_TITLE</b><br /> @COMMENT</span>
                                </td>
			</tr>
			</tbody></table>
                        </td>
		<td></td>
	</tr>
	<tr>
		<td class="LightGreenBoxBottomRight"></td>
		<td></td>
		<td class="LightGreenBoxBottomRight"></td>
	</tr>
</tbody></table>
<div style="font-size:5px">&nbsp;</div>
EOD
);
