<?php
global $base_path;
$HTMLform_report_hdr = t(<<<END

<div id="ActivitiesCSS">
    <div class="HeadingPanel">
        <div class="Headings">

        </div>
    </div>
</div>

<div style="position:relative">
<table cellpadding="0"  cellspacing="0" >
    <tr>
        <td>
            <div class="Heading"><b></b></div>
        </td>
        <td>
            <div class="Heading">
                <div style ="color:#3570AA; font-size:18px; padding:0px; margin:0 0 10px 0">@REPORT_TITLE</div>
            </div>
        </td>
    <tr>
</table>
<div id="ActivitiesCSS">
    <div class="LightBlueBottomRightCorner">
        <div class="LightBlueBottomLeftCorner">
            <div class="LightBlueTopRightCorner">
                <div class="LightBlueTopLeftCorner">
                  <div class="ActivitiesAssignment">
                   <table cellpadding="8" cellspacing="0" border="0" style="width:820px;" class="table clearfix">

END
);


$HTMLform_report_ftr = t(<<<END
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
</div>
END
);




$HTMLCourse_tabs= t(<<<END

<div class="Agenda">
    <div style="float:right;">
        <!-- CREATE_REPORT -->
    </div>
    <div class="AgendaMenu">
        <ul>
           @RUN_REPORTS
            @MANAGE_REPORTS
        </ul>
        <div class="AgendaRightShade">&nbsp;</div>
    </div>
    <div class="AgendaContents">
        <table border="0" cellpadding="0" cellspacing="0" style="width: 100%">
        <tbody>
            <tr>
                <td class="AgendaMainContentAreaMiddleLeftCorner">&nbsp;</td>
                <td class="AgendaMainContentAreaMiddleCenterCorner">&nbsp;</td>
                <td class="AgendaMainContentAreaMiddleRightCorner">&nbsp;</td>
            </tr>
        </tbody>
        </table>
    </div>
</div>

END
);

$HTMLSelectorsAndHeading = '
<div class="clearfix">
    <div class="BoldrBrandCrumb">
    <ul>
            <li>@LINK_HOME >&nbsp;</li>
            <li>@LINK_REPORTS &nbsp;</li> 
    </ul>
    </div>
    <br clear="all" />
    <div>
        <div style="width:40px; float:left;"><img src="'.$base_path.'themes/Boldr/Images/common/reports-icon.png" width="35" height="35" /></div>
        <div style="width:200px; float:left;"><h1 class="pageHeading" style="float: left;"><!-- heading --></h1></div>
    </div>
</div>';

$HTMLReport_yellow_bar= t(<<<END
       <div id="BoldrProject" style="margin-bottom:10px;">
      <div class="ProjectResources" style="width:848px !important;">
        <div class="ProjectResources-Content" style="padding:0px">
          <div class="ProjectResources-Activities">
        	 <div class="Project-ActivitiesFilter" style="padding:0px;">
              <div class="ProjectResources-YellowBottomRightCorner">
                <div class="ProjectResources-YellowBottomLeftCorner">
                  <div class="ProjectResources-YellowTopRightCorner">
                    <div class="ProjectResources-YellowTopLeftCorner" style="padding: 5px;">
                        <div class="">
                        <table width="100%" border="0">
                          <tr>
                            <td width="50%"><div><strong>School: </strong><!--SCHOOL--></div> </td>
                            <td width="50%"><div><strong>School Year: </strong><!--SCHOOL_YEAR--></div></td>
                          </tr>
                        </table>
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