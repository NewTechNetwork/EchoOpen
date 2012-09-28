<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTMLform_Doc_lib_resource = t(<<<END
<table width="100%" cellpadding="5" style="border-top: 1px solid #D6E9F5;">
  <tr>
      @IMAGEPATH
      <td>
          <div style="float:left;font-size: 15px;">
              @RESOURCE_NAME @PROMOTED
          </div>
          <div class="greenLink" style="float:right">
               @EDIT | @DELETE
          </div>

      </td>
  </tr>
  <tr>
      <td>
          <table width="100%">
          <tr>
          <td colspan="2">@PAGE_BODY</td>
          </tr>
              <tr>
                  <td valign="top">
                      <b style="color:#3570aa">Categories:</b>
                  </td>
                  <td class="greenLink">
                  @RESOURCE_SUBJECTS
                  </td>
              </tr>
              <tr>
                  <td valign="top">
                  <b style="color:#3570aa">Keywords:</b>
                  </td>
                  <td class="greenLink">
                   @RESOURCE_KEYWORDS
                  </td>
              </tr>
              <tr>
                  <td valign="top">
                  <b style="color:#3570aa">Submitted:</b>
                  </td>
                  <td>
                   @RESOURCE_DESIGNEDBY
                  </td>
              </tr>
              <tr>
                  <td colspan="2">
                      <div class="GroupsDiscriptionTopRightCorner">
                          <div class="GroupsDiscriptionTopLeftCorner">
                              <div class="GroupsDiscriptionBottomRightCorner">
                                  <div class="GroupsDiscriptionBottomLeftCorner" style="padding:5px">
                                          @RESOURCE_DESCRIPTION
                                  </div>
                              </div>
                          </div>
                      </div>
                    
                  </td>
              </tr>
          </table>
          <div class="clearAll"></div>
	
      </td>
  </tr>
</table>
END
);

//
//	<div  class="AgendaAction">
//			<a href="#" class="ntlp_tooltip_n" title="Like"><img src="@ROOTthemes/Boldr/Images/link.jpg" alt="like" /></a>
//			@ADDNEWCOMMENT
//			<a href="#" class="ntlp_tooltip_n" title="Flag"><img src="@ROOTthemes/Boldr/Images/Flag.jpg" alt="Flag" /></a>
//			</div>
//			<div class="clearAll"></div>
//			@COMMENTS
//                        @ADDCOMMENTSFORM

$HTMLResourcesBock_list = t(<<<EOD
   <tr>
                        	<td style="padding-right:5px;line-height:25px">
                            	<img src="@IMAGE_URL" />
                            </td>
                            <td>
                            	<div class="ntlp_text_style"><div class="ntlp_text_style_inner">@TITLE</div></div>
                            </td>
                        </tr>
 
EOD
);

$HTMLResourcesBock = t(<<<EOD
   <div id="AgendaActivitiesAndNotisfication" style="width:100%">
	<div class="TopRightOrangeCorner">
		<div class="TopLeftOrangeCorner">
			<div class="AgendaActivitiesAndNotisfication">
				<div class="AgendaOrangeBoxHeading">
					<h2>@BLOCK_HEADING</h2>
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
            @LIST_PLACEHOLDER
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
EOD
);


$HTMLResourcesBockCategoryKeyword_list = t(<<<EOD
<tr>
    <td style="line-height:30px;">
        @TITLE
    </td>
</tr>
EOD
);

$HTMLResourcesBockCategoryKeyword = t(<<<EOD
   <div id="AgendaActivitiesAndNotisfication" style="width:100%;">
	<div class="TopRightOrangeCorner">
		<div class="TopLeftOrangeCorner">
			<div class="AgendaActivitiesAndNotisfication">
				<div class="AgendaOrangeBoxHeading">
					<h2>@BLOCK_HEADING</h2>
				</div>
			</div>
		</div>
	</div>
    <div>
      <div class="NoticficationMainContent">
		<table cellpadding="0" cellspacing="0" border="0" width="100%">
			<tr>
				<td class="MiddleLeftOrangeCorner"></td>
				<td class="MiddleCenterOrangeCorner">
					<div>
                    	<div style="clear:both;font-size:5px">&nbsp;</div>
                	<table cellpadding="0" cellspacing="0" border="0" width="100%">
            @LIST_PLACEHOLDER
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
<div style="height: 10px;"></div>
EOD
);



$HTMLTotalResourcesBock = t(<<<EOD
    <div id="AgendaActivitiesAndNotisfication" style="width:100%;">
	<div class="TopRightOrangeCorner">
		<div class="TopLeftOrangeCorner">
			<div class="AgendaActivitiesAndNotisfication">
				<div class="AgendaOrangeBoxHeading">
					<h2>Total Resources</h2>
				</div>
			</div>
		</div>
	</div>
    <div>
      <div class="NoticficationMainContent">
		<table cellpadding="0" cellspacing="0" border="0" width="100%">
			<tr>
				<td class="MiddleLeftOrangeCorner"></td>
				<td class="MiddleCenterOrangeCorner">
					<div>
                    	<div style="clear:both;font-size:5px">&nbsp;</div>
            
<h2 align="center" style="padding:0px;margin:2px">@TOTAL&nbsp;@BLOCK_HEADING</h2>
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

EOD
);

$HTMLResourcesFilter = t(<<<EOD
<div id="AgendaActivitiesAndNotisfication" style="float: right; width: auto;">
	<div class="TopRightOrangeCorner">
		<div class="TopLeftOrangeCorner">
			<div class="AgendaActivitiesAndNotisfication">
				<div class="AgendaOrangeBoxHeading">
					<h2>Filters</h2>
				</div>
			</div>
		</div>
	</div>
    <div>
      <div class="NoticficationMainContent">
		<table cellpadding="0" cellspacing="0" border="0" width="100%">
			<tr>
				<td class="MiddleLeftOrangeCorner"></td>
				<td class="MiddleCenterOrangeCorner">
					<div>
                    	<div style="clear:both;font-size:5px">&nbsp;</div>
           <div><b style="font-weight:bold;color:#3570AA">Category</b></div>
        <div>
            @CATEGORY_COMBO
        </div>
        <div style="height: 10px;"></div>
                        @FILTER_PLACEHOLDER
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
<div style="height: 10px;"></div>
   <div>
        

        
    </div>
EOD
);
//$HTMLResourcesFilter = t(<<<EOD
//    <div>
//        <div>Filters</div>
//        <div>Category</div>
//        <div>
//            @CATEGORY_COMBO
//        </div>
//
//        <div>Resource Type</div>
//        <div>
//            <div><input type="checkbox" /> Link</div>
//            <div><input type="checkbox" /> Images</div>
//            <div><input type="checkbox" /> Videos</div>
//            <div><input type="checkbox" /> Documents</div>
//            <div><input type="checkbox" /> Pages</div>
//        </div>
//
//        <div>Other</div>
//        <div>
//            <div><input type="checkbox" /> Featured Items Only</div>
//        </div>
//    </div>
//EOD
//);

$HTMLResource_filter_cellStart = '<div>';
$HTMLResource_filter_cellEnd = '</div>';
$HTMLResource_filter_heading = '<div>@HEADING';
$HTMLResource_filter_row_end = '</div>';