<?php

$HTMLHeader = t(<<<END
<!--HomePage Feature Panel-->
<div id="AgendaActivitiesAndNotisfication" style="width:215px">
	<div class="TopRightOrangeCorner">
		<div class="TopLeftOrangeCorner">
			<div class="AgendaActivitiesAndNotisfication">
				<div class="AgendaOrangeBoxHeading">
					<h2>@TITLE</h2>
				</div>
			</div>
		</div>
	</div>
	<div class="NoticficationMainContent">
		<table cellpadding="0" cellspacing="0" border="0" width="100%">
			<tr>
				<td class="MiddleLeftOrangeCorner"></td>
				<td class="MiddleCenterOrangeCorner">
					<div>
                    	<div style="clear:both;font-size:5px">&nbsp;</div>
                	      <table cellpadding="0" cellspacing="0" border="0" width="100%">
END
);


$HTMLFooter = t(<<<END
                      </table>
                    </div>
				</td>
				<td class="MiddleRightOrangeCorner"></td>
			</tr>
			<tr>
				<td class="BottomLeftOrangeCorner"></td>
				<td class="BottomCenterOrangeCorner"></td>
				<td class="BottomRightOrangeCorner"></td>
			</tr>
		</table>
	</div>
</div>

END
);


$HTMLContent = t(<<<END
</tr>
  <td>
    <img src="Images/img_camera.png" height="25px" />
  </td>
  <td>
    <a href="#" class="greenLink">@PROJECTNAME</a> <small>@SUBJECT</small>
  </td>
</tr>
END
);
?>