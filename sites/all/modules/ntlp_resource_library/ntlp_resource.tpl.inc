<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTMLHeader = t(<<<END
    <div class="dialog-confirm-ntk-@RESOURCE_NID" style="background-color:#fff;display:none;" title="Delete Resource">
        <p>You are about to permanently delete this resource and any associated comments. </p>
        <p>Are you sure you want to permanently delete this item?</p>
    </div>
<div style="width:100%" class="clearfix">

     <div class="clearfix" style="width:68%; float:left;">
           <div class="BoldrBrandCrumb" style="float:left; padding:0px; margin-top: 7px;">
                @BACK_LINK
           </div>
    </div>

    <div style="background-color:#fff"> @EDIT_RESOURCE</div>

</div>

    <br clear="all" />
    <!-- Agenda ShareStuff Content -->
    <div id="AgendaShareStuffContent">
    <div class="resourcesDarkBlueTopLeft">
    <div class="resourcesDarkBlueTopRight">
    <div class="resourcesDarkBlueBottomLeft">
    <div class="resourcesDarkBlueBottomRight">
    <div>
    <div class="WhiteBoxMainDiv" style="margin-top: 10px;">
END
);


$HTMLFooter = t(<<<END
</div></div></div></div></div>
			</div>
		</div>
END
);

$HTMLContentVideo = t(<<<END

					<table cellpadding="0" style="background-color:#fff" cellspacing="0" border="0" width="816PX">
						<tr>
							<td class="AgendaTopLeftWhiteCorner"></td>
							<td></td>
							<td class="AgendaTopRightWhiteCorner"></td>
						</tr>
						<tr>
							<td></td>
							<td style="padding:3px">
							<div>
							<div class="AgendaPersonInformation">
								<div class="PersonImage">
									@POSTED_BY_IMAGE
								</div>
								<div class="PersonInformation">
									<div class="PersonName">@POSTED_BY_NAME</div>
									<div class="UploadTime">@DATEDIFF</div>
								</div>
							</div>
							<div class="AgendaDeleteBtn">@DELETE</div>
							

							</div>
							<div class="clearAll"></div>
							<div class="CommentArrow"></div>
							<div>
								<table cellpadding="0"  style="background-color:#cee5f3" cellspacing="0" border="0" width="100%">
									<tr>
										<td class="AgendaLightBlueTopLeftCorner" ></td>
										<td></td>
										<td class="AgendaLightBlueTopRightCorner"></td>
									</tr>
									<tr>
										<td></td>
										<td class="AgendaLightBlueMiddleContent">
											<div>
												<table cellpadding="0" cellspacing="0" border="0" width="100%">
  <tr>
    <td class="Content"><h2>@RESOURCE_TITLE</h4></td>
  </tr>
  <tr>
    <td class="Content"><p>@RESOURCE_DESCRIPTION</p></td>
  </tr>

  <tr>
    <td class="Flash"> @RESOURCE_VIEW </td>

  </tr>
  <tr>
    <td></td>
  </tr>
</table>
											</div>
										</td>
										<td></td>
									</tr>
									<tr>
										<td class="AgendaLightBlueBottomLeftCorner"></td>
										<td></td>
										<td class="AgendaLightBlueBottomRightCorner"></td>
									</tr>
								</table>
							</div>
                                 
							<div  class="AgendaAction">
                                                            <div class="Content">Categories : @RESOURCES_CATEGORIES</div>
                                                            <div class="Content">Keywords : @RESOURCES_KEYWORDS</div>
                                                            <div class="Content">Submitted : @RESOURCES_SUBMITTED</div>
							<div class="clearAll"></div>
							<!-- @ADDLIKING
							@ADDNEWCOMMENT
							<a href="#" class="ntlp_tooltip_n" title="Flag"><img src="@ROOTthemes/Boldr/Images/Flag.jpg" alt="Flag" /></a> -->
							</div>
                                                        							<div class="clearAll"></div>
							@COMMENTS
                                                        @ADDCOMMENTSFORM

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

$HTMLContentPage = t(<<<END

                                    
					<table cellpadding="0" style="background-color:#fff" cellspacing="0" border="0" width="816PX">
						<tr>
							<td class="AgendaTopLeftWhiteCorner"></td>
							<td></td>
							<td class="AgendaTopRightWhiteCorner"></td>
						</tr>
						<tr>
							<td colspan = "2" style="padding:3px;">
							<div>
							<div class="AgendaPersonInformation">
								<div class="PersonImage">
									@POSTED_BY_IMAGE
								</div>
								<div class="PersonInformation">
									<div class="PersonName">@POSTED_BY_NAME</div>
									<div class="UploadTime">@DATEDIFF</div>
								</div>
							</div>
							<div class="AgendaDeleteBtn">@DELETE</div>
						
							<div class="clearAll"></div>
							<div class="CommentArrow"></div>
							<div>
								<table cellpadding="0"  style="background-color:#cee5f3" cellspacing="0" border="0" width="100%">
									<tr>
										<td class="AgendaLightBlueTopLeftCorner" ></td>
										<td></td>
										<td class="AgendaLightBlueTopRightCorner"></td>
									</tr>
									<tr>
										<td></td>
										<td class="AgendaLightBlueMiddleContent">
											<div>
												<table cellpadding="0" cellspacing="0" border="0" width="100%">
													<tr><td class="Content">
                                                            <h4>@RESOURCE_TITLE</h4></td>
                                                        </tr>
                                                        <tr>
                                                            <td class="Image">
                                                                    <div style="overflow-x:auto;width:787px"> @RESOURCE_VIEW </div>
                                                            </td>
                                                        </tr>
                                                        <tr>
                                                            <td class="Content">
                                                                <div><br /><span style="color: #3570AA; font-weight: bold;">Description</span></div>
                                                                <div>@RESOURCE_DESCRIPTION</div>
                                                            </td>
                                                        </tr>
													<tr>
													<td>
													</td>
													</tr>
												</table>
											</div>
										</td>
										<td></td>
									</tr>
									<tr>
										<td class="AgendaLightBlueBottomLeftCorner"></td>
										<td></td>
										<td class="AgendaLightBlueBottomRightCorner"></td>
									</tr>
								</table>
							</div>
							<div class="clearAll"></div>
                            <div class="Content">@RESOURCE_ATTACHMENTS</div>
							<div class="clearAll"></div>
                            <div class="Content">Categories : @RESOURCES_CATEGORIES</div>
                            <div class="Content">Keywords : @RESOURCES_KEYWORDS</div>
                            <div class="Content">Submitted : @RESOURCES_SUBMITTED</div>
						<!--	<div  class="AgendaAction">
							 @ADDLIKING
							@ADDNEWCOMMENT
							<a href="#" class="ntlp_tooltip_n" title="Flag"><img src="@ROOTthemes/Boldr/Images/Flag.jpg" alt="Flag" /></a> 
							</div> -->
							<div class="clearAll"></div>
							@COMMENTS
                                                        @ADDCOMMENTSFORM
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

$HTMLContentImage = t(<<<END

<table cellpadding="0"  style="background-color:#fff" cellspacing="0" border="0" width="816PX">
						<tr>
							<td class="AgendaTopLeftWhiteCorner"></td>
							<td></td>
							<td class="AgendaTopRightWhiteCorner"></td>
						</tr>
						<tr>
							<td></td>
							<td style="padding:3px">
							<div>
							<div class="AgendaPersonInformation">
								<div class="PersonImage">
									@POSTED_BY_IMAGE
								</div>
								<div class="PersonInformation">
									<div class="PersonName">@POSTED_BY_NAME</div>
									<div class="UploadTime">@DATEDIFF</div>
								</div>
							</div>
							<div class="AgendaDeleteBtn">@DELETE</div>
                                                       
							<div class="clearAll"></div>
							<div class="CommentArrow"></div>
							<div>
								<table cellpadding="0" style="background-color:#cee5f3" cellspacing="0" border="0" width="100%">
									<tr>
										<td class="AgendaLightBlueTopLeftCorner" ></td>
										<td></td>
										<td class="AgendaLightBlueTopRightCorner"></td>
									</tr>
									<tr>
										<td></td>
										<td class="AgendaLightBlueMiddleContent">
											<div>
												<table cellpadding="0" cellspacing="0" border="0" width="100%">
													<tr><td class="Content">
                                                            <h4>@RESOURCE_TITLE</h4></td>
                                                        </tr>
                                                        <tr>
                                                            <td class="Content"><p>@RESOURCE_DESCRIPTION</p></td>
                                                        </tr>
                                                        <tr>
														<td class="Image">
															@RESOURCE_VIEW
														</td>
													</tr>
													<tr>
													<td >
													</td>
													</tr>
												</table>
											</div>
										</td>
										<td></td>
									</tr>
									<tr>
										<td class="AgendaLightBlueBottomLeftCorner"></td>
										<td></td>
										<td class="AgendaLightBlueBottomRightCorner"></td>
									</tr>
								</table>
							</div>
												<div class="clearAll"></div>
                                                                                                    <div class="Content">Categories : @RESOURCES_CATEGORIES</div>
                                                                                                    <div class="Content">Keywords : @RESOURCES_KEYWORDS</div>
                                                                                                    <div class="Content">Submitted : @RESOURCES_SUBMITTED</div>
												<!-- <div  class="AgendaAction">
							 @ADDLIKING
							@ADDNEWCOMMENT
							<a href="#" class="ntlp_tooltip_n" title="Flag"><img src="@ROOTthemes/Boldr/Images/Flag.jpg" alt="Flag" /></a>
												</div> --> 
                                                                                               @COMMENTS
                                                                    @ADDCOMMENTSFORM
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

$HTMLContentSharedIdea = t(<<<END

<table cellpadding="0"  style="background-color:#fff" cellspacing="0" border="0" width="816PX">
						<tr>
							<td class="AgendaTopLeftWhiteCorner"></td>
							<td></td>
							<td class="AgendaTopRightWhiteCorner"></td>
						</tr>
						<tr>
							<td></td>
							<td style="padding:3px">
							<div>
							<div class="AgendaPersonInformation">
								<div class="PersonImage">
									@POSTED_BY_IMAGE
								</div>
								<div class="PersonInformation">
									<div class="PersonName">@POSTED_BY_NAME</div>
									<div class="UploadTime">@DATEDIFF</div>
								</div>
							</div>
							<div class="AgendaDeleteBtn">@DELETE</div>
							</div>
							<div class="clearAll"></div>
							<div class="CommentArrow"></div>
							<div>
								<table cellpadding="0" style="background-color:#cee5f3" cellspacing="0" border="0" width="100%">
									<tr>
										<td class="AgendaLightBlueTopLeftCorner" ></td>
										<td></td>
										<td class="AgendaLightBlueTopRightCorner"></td>
									</tr>
									<tr>
										<td></td>
										<td class="AgendaLightBlueMiddleContent">
														<div class="Content">
															<!-- <h4>@RESOURCE_TITLE</h4> -->
															<p>@RESOURCE_DESCRIPTION</p>
														</div>

										</td>
										<td></td>
									</tr>
									<tr>
										<td class="AgendaLightBlueBottomLeftCorner"></td>
										<td></td>
										<td class="AgendaLightBlueBottomRightCorner"></td>
									</tr>
								</table>
							</div>
												<div class="clearAll"></div>
												<!-- <div  class="AgendaAction">
							 @ADDLIKING
							@ADDNEWCOMMENT
							<a href="#" class="ntlp_tooltip_n" title="Flag">
                                                        <img src="@ROOTthemes/Boldr/Images/Flag.jpg" alt="Flag" /></a> 
												</div> -->
@COMMENTS
                                                                    @ADDCOMMENTSFORM
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

$HTMLContentLink = t(<<<END

					<table cellpadding="0"  style="background-color:#fff" cellspacing="0" border="0" width="816PX">
						<tr>
							<td class="AgendaTopLeftWhiteCorner"></td>
							<td></td>
							<td class="AgendaTopRightWhiteCorner"></td>
						</tr>
						<tr>
							<td></td>
							<td style="padding:3px">
							<div>
							<div class="AgendaPersonInformation">
								<div class="PersonImage">
									@POSTED_BY_IMAGE
								</div>
								<div class="PersonInformation">
									<div class="PersonName">@POSTED_BY_NAME</div>
									<div class="UploadTime">@DATEDIFF</div>
								</div>
							</div>
							<div class="AgendaDeleteBtn">@DELETE</div>
                                                     
							</div>
							<div class="clearAll"></div>
							<div class="CommentArrow"></div>
							<div>
								<table cellpadding="0" style="background-color:#cee5f3" cellspacing="0" border="0" width="100%">
									<tr>
										<td class="AgendaLightBlueTopLeftCorner" ></td>
										<td></td>
										<td class="AgendaLightBlueTopRightCorner"></td>
									</tr>
									<tr>
										<td></td>
										<td class="AgendaLightBlueMiddleContent">

                                                    <table cellpadding="0" cellspacing="0" border="0" width="100%">
													<tr><td class="Content">
                                                            <h4>@RESOURCE_TITLE</h4></td>
                                                        </tr>
                                                        <tr>
                                                            <td class="Content"><p>@RESOURCE_DESCRIPTION</p></td>
                                                        </tr>
                                                        <tr>
														<td class="Image">
                                                <br clear="all">
                                                <div class="PDFFiles">
										<div class="PDFFileLink">@RESOURCE_VIEW</div>
											</div>
														</td>

													</tr>
													<tr>
													<td>
													</td>
													</tr>
												</table>



										</td>
										<td></td>
									</tr>
									<tr>
										<td class="AgendaLightBlueBottomLeftCorner"></td>
										<td></td>
										<td class="AgendaLightBlueBottomRightCorner"></td>
									</tr>
								</table>
							</div>
							<div class="clearAll"></div>
                                                            <div class="Content">Categories : @RESOURCES_CATEGORIES</div>
                                                            <div class="Content">Keywords : @RESOURCES_KEYWORDS</div>
                                                            <div class="Content">Submitted : @RESOURCES_SUBMITTED</div>
							<!-- <div  class="AgendaAction">
							 @ADDLIKING
							@ADDNEWCOMMENT
							<a href="#" class="ntlp_tooltip_n" title="Flag"><img src="@ROOTthemes/Boldr/Images/Flag.jpg" alt="Flag" /></a> 
							</div> -->
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
$HTMLContentDocument = t(<<<END
<table cellspacing="0" cellpadding="0" border="0" align="center" width="845px" style="table-layout:fixed;">
<tbody>
    <tr>
        <td> <div>
                <div style="width:30px;float:left;"> <img src="@ROOTthemes/Boldr/Images/@IMG_SRC" vspace="4" align="absbottom" /> </div>&nbsp;
                
                <div class="inner_pages_main_heading" style="width:550px;float:left; font-size:20px"> @RESOURCE_TITLE </div>
                
                <div style="float:right;">
                    <div style="width:20px; float:left;margin-right:10px; margin-top:5px;">
                    @ADDLIKING
                    </div>

                    <div style="float:right; background-color: rgb(239, 239, 222); padding:5px 10px 5px 10px;">
                        <span style="margin-right:60px;float:left;"><b>Likes: &nbsp;</b>@LIKES</span>
                        <span style="float:right;margin-right:20px;"><b>Views: &nbsp;</b>@VIEWS</span>
                    </div>
                </div>
                
             </div>
        </td>
    </tr>
    <tr>
        <td >
        <div>
             @RESOURCE_DESCRIPTION
            <div class="library_project_text"><span style="display:@RESOURCE_DISPLAY_VIEW">@RESOURCE_VIEW &nbsp;</span>
                <div><!-- @RESOURCE_ATTACHMENTS --><div>
            </div>
        </div>
        </td>
     </tr>
     <tr>
        <td>
            <div  class="blue_info_bar">
                <div style="width:325px;float:left;"><b>Author:</b> @AUTHOR</div>
                <div style="width:200px;float:left;"><b>Published:</b> @RESOURCES_SUBMITTED</div>
                <div style="float:right;"><b>Keywords:</b> @RESOURCES_KEYWORDS</div>
            <div class="clearAll"></div>
        </td>
     </tr>
</tbody>
</table>
END
);

$HTMLresource_manage_options = t(<<<END

<div class="anchorBaseCombobox" >
<div id="btnManageproject"></div>
<div id="manageProject" class="hideMODrpDown" style="display:none">
<div class="hideMODrpDownInnerBg">
	<ul>
        <li class="ManageOption">@MANAGE_RESOURCE_EDIT</li>
        <li class="ManageOption">@MANAGE_RESOURCE_DELETE</li>
	</ul>
    </div>
</div>
</div>

END
);


$HTMLcontent = t(<<<END

<table cellpadding="0"  style="background-color:#fff" cellspacing="0" border="0" width="816PX">
						<tr>
							<td class="AgendaTopLeftWhiteCorner"></td>
							<td></td>
							<td class="AgendaTopRightWhiteCorner"></td>
						</tr>
						<tr>
							<td></td>
							<td style="padding:3px">
							<div>
							<div class="AgendaPersonInformation">
								<div class="PersonImage">
									@POSTED_BY_IMAGE
								</div>
								<div class="PersonInformation">
									<div class="PersonName">@POSTED_BY_NAME</div>
									<div class="UploadTime">@DATEDIFF</div>
								</div>
							</div>
							<div class="AgendaDeleteBtn">@DELETE</div>
                                                        
							</div>
							<div class="clearAll"></div>
							<div class="CommentArrow"></div>
							<div>
								<table cellpadding="0" style="background-color:#cee5f3" cellspacing="0" border="0" width="100%">
									<tr>
										<td class="AgendaLightBlueTopLeftCorner" ></td>
										<td></td>
										<td class="AgendaLightBlueTopRightCorner"></td>
									</tr>
									<tr>
										<td></td>
										<td class="AgendaLightBlueMiddleContent">
											<div>
												<table cellpadding="0" cellspacing="0" border="0" width="100%">
													<tr>
														<td class="Image">
															@RESOURCE_VIEW
														</td>
														<td class="Content">
															<h4>@RESOURCE_TITLE</h4>
															<p>@RESOURCE_DESCRIPTION</p>
														</td>
													</tr>
													<tr>
													<td>
													</td>
													</tr>
												</table>
											</div>
										</td>
										<td></td>
									</tr>
									<tr>
										<td class="AgendaLightBlueBottomLeftCorner"></td>
										<td></td>
										<td class="AgendaLightBlueBottomRightCorner"></td>
									</tr>
								</table>
							</div>
							<div class="clearAll"></div>
							<!--  <div  class="AgendaAction">
							@ADDLIKING
							@ADDNEWCOMMENT
							<a href="#" class="ntlp_tooltip_n" title="Flag"><img src="@ROOTthemes/Boldr/Images/Flag.jpg" alt="Flag" /></a>
							</div>  -->
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
					<table cellpadding="0" style="background-color:#fff" cellspacing="0" border="0" width="816PX">
						<tr>
							<td class="AgendaTopLeftWhiteCorner"></td>
							<td></td>
							<td class="AgendaTopRightWhiteCorner"></td>
						</tr>
						<tr>
							<td></td>
							<td>
							<div>
							<div class="AgendaPersonInformation">
								<div class="PersonImage">
									@POSTED_BY_IMAGE
								</div>
								<div class="PersonInformation">
									<div class="PersonName">@POSTED_BY_NAME</div>
									<div class="UploadTime">@DATEDIFF</div>
								</div>
							</div>
							<div class="AgendaDeleteBtn">@DELETE</div>
                                                       
							</div>
							<div class="clearAll"></div>
							<div class="CommentArrow"></div>
							<div>
								<table cellpadding="0" style="background-color:#cee5f3" cellspacing="0" border="0" width="100%">
									<tr>
										<td class="AgendaLightBlueTopLeftCorner" ></td>
										<td></td>
										<td class="AgendaLightBlueTopRightCorner"></td>
									</tr>
									<tr>
										<td></td>
										<td class="AgendaLightBlueMiddleContent">
											<div class="PDFFiles">
											<div class="PDFIcon"><img src="@ROOTthemes/Boldr/Images/Icon/pdfIcon.png" alt="PDF Icon" /></div>
											<div class="PDFFileLink">@RESOURCE_VIEW</div>
											</div>
										</td>
										<td></td>
									</tr>
									<tr>
										<td class="AgendaLightBlueBottomLeftCorner"></td>
										<td></td>
										<td class="AgendaLightBlueBottomRightCorner"></td>
									</tr>
								</table>
							</div>
												<div class="clearAll"></div>
											<!-- 	<div  class="AgendaAction">
							@ADDLIKING
							@ADDNEWCOMMENT
							<a href="#" class="ntlp_tooltip_n" title="Flag"><img src="@ROOTthemes/Boldr/Images/Flag.jpg" alt="Flag" /></a> 
												</div> -->
												<div class="clearAll"></div>
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
																<table cellpadding="0" align="left" cellspacing="0" border="0" width="100%">
																<tr>
																	<td class="LightGreenBoxPerson">@POSTED_BY_IMAGE</td>
																	<td class="LightGreenBoxText">
																	<h6>Heading:<span>asd</span></h6>
																	<p>(2 hours ago)</p>
																	</td>
																	<td valign="middle" class="LightGreenBoxDeletebtn">
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
												<div class="clearAll"></div>
												<div class="AgendaTextArea">
													<textarea rows="3" cols="70">
													</textarea>
												</div>
												<div align="right" class="CommentBtn">
													<a href="#" ></a>
												</div>
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
					<table cellpadding="0"  style="background-color:#fff" cellspacing="0" border="0" width="816PX">
						<tr>
							<td class="AgendaTopLeftWhiteCorner"></td>
							<td></td>
							<td class="AgendaTopRightWhiteCorner"></td>
						</tr>
						<tr>
							<td></td>
							<td>
							<div>
							<div class="AgendaPersonInformation">
								<div class="PersonImage">
									@POSTED_BY_IMAGE
								</div>
								<div class="PersonInformation">
									<div class="PersonName">@POSTED_BY_NAME</div>
									<div class="UploadTime">@DATEDIFF</div>
								</div>
							</div>
							<div class="AgendaDeleteBtn">@DELETE</div>

							</div>
							<div class="clearAll"></div>
							<div class="CommentArrow"></div>
							<div>
								<table cellpadding="0" style="background-color:#cee5f3" cellspacing="0" border="0" width="100%">
									<tr>
										<td class="AgendaLightBlueTopLeftCorner" ></td>
										<td></td>
										<td class="AgendaLightBlueTopRightCorner"></td>
									</tr>
									<tr>
										<td></td>
										<td class="AgendaLightBlueMiddleContent">
											<div>
												<table cellpadding="0" cellspacing="0" border="0" width="100%">
													<tr>
														<td class="Flash">
															<img src="/drupal3/themes/Boldr/Images/flashImg.jpg" alt="Image" height="146" width="195" />
														</td>
														<td class="Content">
															<h4>@RESOURCE_TITLE</h4>
															<p>@RESOURCE_DESCRIPTION</p>
														</td>
													</tr>
													<tr>
													<td>
													</td>
													</tr>
												</table>
											</div>
										</td>
										<td></td>
									</tr>
									<tr>
										<td class="AgendaLightBlueBottomLeftCorner"></td>
										<td></td>
										<td class="AgendaLightBlueBottomRightCorner"></td>
									</tr>
								</table>
							</div>
							<div class="clearAll"></div>
							<!-- <div  class="AgendaAction">
							@ADDLIKING
							@ADDNEWCOMMENT
							<a href="#" class="ntlp_tooltip_n" title="Flag"><img src="@ROOTthemes/Boldr/Images/Flag.jpg" alt="Flag" /></a> 
							</div> -->
							</td>
							<td></td>
						</tr>
						<tr>
							<td class="AgendaBottomLeftWhiteCorner"></td>
							<td></td>
							<td class="AgendaBottomRightWhiteCorner"></td>
						</tr>
					</table>
END
);

?>