<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


global $base_path;
$HTMLHeader = t(<<<END
    <div class="dialog-confirm-ntk-@RESOURCE_NID" style="background-color:#fff;display:none;" title="Delete Resource">
        <p>You are about to permanently delete this resource and any associated comments. This will also remove the resource from all project briefcases that currently link to it.</p>
        <p>If you want to keep the resource, but simply remove it from a project briefcase, cancel this action and modify the project briefcase instead.</p>
        <p>Are you sure you want to permanently delete this item?</p>
    </div>
    <div id="dialog-show-confirmation" style="display: none;" title="Message:">
    <span class="msg">Are you sure you want to submit the form?</span>
   </div>
<div style="width:100%" class="clearfix">

     <div class="clearfix" style="width:68%; float:left;">
           <div class="BoldrBrandCrumb" style="float:left; padding:0px; margin-top: 7px;">
                <ul><li>@BACK_LINK  &gt;&nbsp;</li><li style="color:#000; font-weight:bold;">Project Resource</li></ul>
           </div>
    </div>

    <div style="color:grey; width: 100px; float: left;margin-top: 3px;">@VISIBILITY</div>
    <div style="padding:0 0px 0 0;background-color:#fff">@EDIT</div>

   
                    
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


$HTMLContentDocument = t(<<<END
<table cellspacing="0" cellpadding="0" border="0" align="center" width="845px" style="table-layout:fixed;">
<tbody>
    <tr>
        <td> <div><span> <img src="@ROOTthemes/Boldr/Images/@IMG_SRC" vspace="4" align="absbottom" /> </span>&nbsp;
             <span class="inner_pages_main_heading" style="font-size:20px"> @RESOURCE_TITLE </span></div>
        </td>
    </tr>
    <tr>
        <td >
          <div>
              <!-- project Description -->
                     @RESOURCE_DESCRIPTION
              <!-- project Description -->
              <div class="library_project_text">
              <span style="display:@RESOURCE_DISPLAY_VIEW">@RESOURCE_VIEW &nbsp;</span>
              @RESOURCES_ATTACHMENT
              <div><!-- @RESOURCE_ATTACHMENTS --></div>
              </div>
          </div>
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
        <li class="ManageOption">@MANAGE_RESOURCE_COPY</li>
        <li class="ManageOption">@MANAGE_RESOURCE_DELETE</li>
	</ul>
    </div>
</div>
</div>

END
);
?>
