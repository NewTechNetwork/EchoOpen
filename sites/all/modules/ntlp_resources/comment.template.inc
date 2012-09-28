<?php

$HTMLComment = t(<<<END
<div class="LightGreenBox">
<table cellpadding="0" style="background-color:#d3ffc2" cellspacing="0" border="0" width="100%">
	<tr>
		<td class="LightGreenBoxTopLeft"></td>
		<td></td>
		<td class="LightGreenBoxTopRight"></td>
	</tr>
	<tr>
		<td></td>
		<td>
			<table cellpadding="0" cellspacing="0" border="0" width="100%">
			<tr valign="top">
				<td class="LightGreenBoxPerson">@POSTED_BY_IMAGE</td>
                                <td class="LightGreenBoxText">
                                <h6>@HEADING<span>@COMMENTS</span></h6>
                                <p>@DATEDIFF</p>
                                </td>
				<td class="LightGreenBoxDeletebtn">
				@DELETE
				</td>
			</tr>
			</table>
		</td>
		<td></td>
	</tr>
	<tr>
		<td class="LightGreenBoxBottomRight"></td>
		<td></td>
		<td class="LightGreenBoxBottomRight"></td>
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

?>