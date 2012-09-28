<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTMLHeader = t(<<<END

<div id="AgendaShareStuff" style-"text-align:left;">
	<!-- Agenda ShareStuff Content -->
        @BACK_LINK
	<div id="AgendaShareStuffContent" style="padding:0 0 0 7px;">
				<div class="WhiteBoxMainDiv">
END
);


$HTMLFooter = t(<<<END
</div>
			</div>
		</div>

END
);


$HTMLContentLink = t(<<<END
					<table cellpadding="0"  style="background-color:#fff;  table-layout:fixed;" cellspacing="0" border="0" width="588" align="center">
						<tr>
							<td class="AgendaTopLeftWhiteCorner"></td>
							<td></td>
							<td class="AgendaTopRightWhiteCorner"></td>
						</tr>
						<tr>
							<td></td>
							<td style="padding:3px">
							<div class="clearfix" style="width:100%;">
							<div class="AgendaPersonInformation" style="width: 200px; float: left;">
                                                               
								<div class="PersonImage">
									@POSTED_BY_IMAGE
								</div>
								<div class="PersonInformation">
									<div class="PersonName">@POSTED_BY_NAME</div>
									<div class="UploadTime">@DATEDIFF</div>
								</div>
							</div>
                                                        @LIKE_COUNTS
                                                        <div class="AgendaDeleteBtn" style="width: 14px; float: right;">@DELETE</div>
                                                         
							</div>
							<div class="clearAll"></div>
							<div class="CommentArrow"></div>
							<div>
                                                        <!-- comment table starts here -->
                                                        
                                                                <table cellspacing="0" cellpadding="0" border="0" width="100%" style="background-color: rgb(206, 229, 243);  table-layout:fixed;" >
                                                                  <tbody>
                                                                    <tr>
                                                                      <td class="AgendaLightBlueTopLeftCorner"></td>
                                                                      <td></td>
                                                                      <td class="AgendaLightBlueTopRightCorner"></td>
                                                                    </tr>
                                                                    <tr>
                                                                      <td></td>
                                                                      <td class="AgendaLightBlueMiddleContent"><div>

                                                            <div style="width:100%; position:relative; padding-bottom:6px;">
                                                                <div style="width:auto;">
                                                                    <div style="word-wrap:break-word;">
                                                                         <div> @RESOURCE_DESCRIPTION </div>
                                                                    </div>
                                                                </div>
                                                            </div>
                                                                         <table cellspacing="0" cellpadding="0" border="0" width="100%">
                                                                            <tbody>
                                                                              <tr>
                                                                                <td>
                                                                                    @RESOURCE_PREVIEW
                                                                                </td>
                                                                              </tr>
                                                                              @RESOURCE_PADDING
                                                                            </tbody>
                                                                          </table>


                                                                        </div></td>
                                                                      <td></td>
                                                                    </tr>
                                                                    <tr>
                                                                      <td class="AgendaLightBlueBottomLeftCorner"></td>
                                                                      <td></td>
                                                                      <td class="AgendaLightBlueBottomRightCorner"></td>
                                                                    </tr>
                                                                  </tbody>
                                                                </table> <!-- table ends here -->
                                                          
							</div>
							<div class="clearAll"></div>
							<div  class="AgendaAction">
							@ADDLIKING
							@ADDNEWCOMMENT
							<a href="#" class="ntlp_tooltip_n" title="Flag" style="display:none"><img src="@ROOTthemes/Boldr/Images/Flag.jpg" alt="Flag" /></a>
							</div>
							<div class="clearAll"></div>
							<div align="center">@COMMENTS
                                                        @ADDCOMMENTSFORM</div>
							</td>
							<td></td>
						</tr>
						<tr>
							<td class="AgendaBottomLeftWhiteCorner"></td>
							<td></td>
							<td class="AgendaBottomRightWhiteCorner"></td>
						</tr>
					</table>
					<div class="gap"></div>
END
);

?>