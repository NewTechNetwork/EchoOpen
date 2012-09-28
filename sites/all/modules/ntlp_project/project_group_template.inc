<?php


$HTML_Project_Group = t(<<<END
      <div id="BoldrProjectSecondColumn" style="margin:0 0 0 12px">
            <div class="TopRightOrangeCorner">
		<div class="TopLeftOrangeCorner">
			<div class="AgendaActivitiesAndNotisfication">
				<div class="AgendaIconDiv">
						<img src="@ROOTthemes/Boldr/Images/ProjectsImages/ProjectsGroups.jpg" alt="Projects Groups" />
				</div>
				<div class="AgendaOrangeBoxHeading">

					<h2>Project Groups</h2>
				</div>
			</div>
		</div>
            </div>
	<div class="ProjectGroup">
		<table cellpadding="0"  style="height:215px;" cellspacing="0" border="0" width="100%">
			<tr>

				<td class="MiddleLeftOrangeCorner"></td>
				<td valign="top" class="MiddleCenterOrangeCorner" style="height:100%">
                                    @GROUP_POST_LINK
					  

				<div align="right" class="ViewAllLink">
                                    @VIEW_ALL_LINK
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
<style>
.panels-flexible-column {
    width:auto !important;
}
</style>
END
);


$HTML_Group_Post_Box = t(<<<END

<div class="GroupNameAndNewPostLinks">
    <div class="NewPostLinks GroupsName">@GROUP_NAME</div>
    <div class="NewPostLinks">
        @GROUP_POST
    </div>
</div>

END
);



?>