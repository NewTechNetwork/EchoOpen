<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTMLcontainer = t(<<<END

<style>
.views-field-title {
    width:300px;
}
.views-table th {
    width:300px;
}
</style>
<div id="ActivitiesCSS">
	<div class="HeadingPanel" style="float:inherit">
		<div class="Headings" style="float:inherit">
			<div class="Heading" style="float:left"><b>Activities</b></div>
			<div class="project">
                          
                            @LINK_NEW
                        </div>
		</div>
	</div>
</div>
<br clear="all" />
<br clear="all" />
<div id="ActivitiesCSS">
	<div class="BlueBorderBgTopCenter">
		<div class="BlueBorderBgMiddleRight">
			<div class="BlueBorderBgBottomCenter">
				<div class="BlueBorderBgMiddleLeft">
					<div class="BlueBorderBgTopRightCorner">
						<div class="BlueBorderBgBottomRightCorner">
							<div class="BlueBorderBgBottomLeftCorner">
								<div class="BlueBorderBgTopLeftCorner">
									<div class="Filter">
										<div class="yellowTopRightCorner">
											<div class="yellowBottomRightCorner">
												<div class="yellowBottomLeftCorner">
													<div class="yellowTopLeftCorner">
														<table cellpadding="0" cellspacing="0" width="100%" border="0" >
															<tr>
																<td><div>
																		<div class="FilterHeading">
																			<h5>Filter</h5>
																		</div>
																		<div class="CloseLink"> <a href="#">&laquo; Close</a> </div>
																	</div>
																	</td>
															</tr>
														</table>
													</div>
												</div>
											</div>
										</div>
									</div>
									<div class="clearall"></div>
									<br clear="all" />
									<div class="ReportView" style="width:600px>
@ACTIVITIES_VIEW
									</div>
								</div>
								</div>
							</div>
						</div>
					</div>
				</div>
			</div>
		</div>
	</div>
</div>

END
);

$HTML_Upcoming_Activities= t(<<<END
<div id="AgendaActivitiesAndNotisfication">
        	<div class="TopRightOrangeCorner">
        		<div class="TopLeftOrangeCorner">
        			<div class="AgendaActivitiesAndNotisfication">
        				<div class="AgendaOrangeBoxHeading">
        					<h2>Upcoming Activities</h2>
        				</div>
        			</div>
        		</div>
        	</div>
        	<div class="NoticficationMainContent">
        		<table cellpadding="0" cellspacing="0" border="0" width="218px">
        			<tr>
        				<td class="MiddleLeftOrangeCorner">&nbsp;</td>
        				<td class="MiddleCenterOrangeCorner">
        					<ul class="Notisfication">
        						 @LIST_UPCOMING_ACTIVITIES
        					</ul>
        				</td>
        				<td class="MiddleRightOrangeCorner"></td>
        			</tr>
        			<tr>
        				<td class="BottomLeftOrangeCorner">&nbsp;</td>
        				<td class="BottomCenterOrangeCorner">&nbsp;</td>
        				<td class="BottomRightOrangeCorner">&nbsp;</td>
        			</tr>
        		</table>
        	</div>
        </div>
END
);
?>
