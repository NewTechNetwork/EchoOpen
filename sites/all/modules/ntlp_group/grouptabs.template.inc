<?php

/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
*/


$get=$_GET['q'];
$br= explode("/",$get);
$cnt=count($br);

# Getting Page Name From URl like event,node...
$target=$br[$cnt-2];

#Getting Query String  From URL
$qs =$br[$cnt-1];

$HTMLgroup_tab =t(<<<END
 <div class="Agenda">
<div class="AgendaMenu">
<ul>

END
);


if($target=="home") {

    $HTMLgroup_tab .=t(<<<END

    <li class="active">
    <a class="active" href="?q=ntlp/groups/home/$qs ">Start</a></li>
END
    );



}  else {


    $HTMLgroup_tab .=t(<<<END

         <li>
    <a href="?q=ntlp/groups/home/$qs">Start</a></li>



END
    );

}

$HTMLgroup_tab .=t(<<<END
  
          <li>


END
);


if($target=="resources") {

    $HTMLgroup_tab .=t(<<<END
</li>
<li class="active">
    <a class="active" href="?q=ntlp/groups/resources/$qs">Resources</a></li>


END
    );

}  else {

    $HTMLgroup_tab .=t(<<<END
 
      <li>
    <a href="?q=ntlp/groups/resources/$qs">Resources</a></li>
<li>
           

END
    );

}
if($target=="discussion") {

    $HTMLgroup_tab .=t(<<<END


           </li>
<li class="active">
    <a class="active" href="?q=ntlp/groups/discussion/$qs">Discussion</a></li>



END
    );
}  else {

    $HTMLgroup_tab .=t(<<<END

           <li>
    <a href="?q=ntlp/groups/discussion/$qs">Discussion</a></li>


END
    );


}
if($target=="member") {

    $HTMLgroup_tab .=t(<<<END



           <li class="active">
    <a class="active" href="?q=ntlp/groups/member/$qs">Members</a></li>

END
    );


}  else {

    $HTMLgroup_tab .=t(<<<END
      
           <li>
    <a href="?q=ntlp/groups/member/$qs">Members</a></li>

END
    );

}

if($target=="events") {

    $HTMLgroup_tab .=t(<<<END



           <li class="active">
    <a class="active" href="?q=ntlp/groups/events/$qs">Events</a></li>

END
    );


}  else {

    $HTMLgroup_tab .=t(<<<END

           <li>
    <a href="?q=ntlp/groups/events/$qs">Events</a></li>

END
    );


}

$HTMLgroup_tab .=t(<<<END
</ul>
<div class="AgendaRightShade">
    &nbsp;</div>
</div>
<div class="AgendaContents">
    <table border="0" cellpadding="0" cellspacing="0" width="100%">
        <tbody>
            <tr>
                <td class="AgendaMainContentAreaMiddleLeftCorner">
                    &nbsp;</td>
                <td class="AgendaMainContentAreaMiddleCenterCorner">
                    &nbsp;</td>
                <td class="AgendaMainContentAreaMiddleRightCorner">
                    &nbsp;</td>
            </tr>
        </tbody>
    </table>
</div>
</div>
 
END
);



if($target=="allprojects") {

    $HTMLproject_library_tab .=t(<<<END

    <li class="active">
    <a class="active" href="?q=ntlp/groups/home/$qs ">All Projects</a></li>
END
    );



}  else {


    $HTMLproject_library_tab .=t(<<<END

         <li>
    <a href="?q=ntlp/groups/home/$qs">All Projects</a></li>



END
    );

}

$HTMLproject_library_tab .=t(<<<END

          <li>


END
);


if($target=="myprojects") {

    $HTMLproject_library_tab .=t(<<<END
</li>
<li class="active">
    <a class="active" href="?q=ntlp/groups/resources/$qs">Resources</a></li>


END
    );

}  else {

    $HTMLproject_library_tab .=t(<<<END

      <li>
    <a href="?q=ntlp/groups/resources/$qs">Resources</a></li>
<li>


END
    );

}


$HTMLproject_library_tab .=t(<<<END
</ul>
<div class="AgendaRightShade">
    &nbsp;</div>
</div>
<div class="AgendaContents">
    <table border="0" cellpadding="0" cellspacing="0" width="100%">
        <tbody>
            <tr>
                <td class="AgendaMainContentAreaMiddleLeftCorner">
                    &nbsp;</td>
                <td class="AgendaMainContentAreaMiddleCenterCorner">
                    &nbsp;</td>
                <td class="AgendaMainContentAreaMiddleRightCorner">
                    &nbsp;</td>
            </tr>
        </tbody>
    </table>
</div>
</div>

END
);




//if($target=="resources") {
$HTMLgroup_resource_tab =t(<<<END
   <div>
      <!-- ADD_RESOURCE_BUTTON -->
      </div>
   <div style="clear: both;"></div>
END
);
//}

$HTMLgroup_start_tab =t(<<<END

<table>
    <tr>
        <td>@TITLE</td>
    </tr>
    <tr>
        <td>Associated Course</td>
        <td>@ASSOCIATED_COURSES (@NAME)</td>
    </tr>
    <tr>
        <td>Associated Project</td>
        <td>@ASSOCIATED_PROJECT</td>
    </tr>
    <tr>
        <td>Summary</td>
        <td>@DESCRIPTION</td>
    </tr>
 </table>
END
);




$HTMLgroup_member_filter_heading = t(<<<EOD
<tr>
                            <td class="StatusHeading" style="white-space:nowrap;width:120px">@HEADING</td>
                            <td>
EOD
);
$HTMLgroup_member_filter_row_end = t(<<<EOD
 </td>
                          </tr>
                          <tr>
                            <td colspan="2" class="yellowDottedLine">&nbsp;</td>
                          </tr>
EOD
);
$HTMLgroup_member_filter_cellStart = t(<<<EOD
        <div class="ChkBox">
EOD
);
$HTMLgroup_member_filter_cellEnd = t(<<<EOD
        </div>
EOD
);
$HTMLgroup_member_filter = t(<<<EOD
<div id="BoldrProject" style="width:845px;">
  <div class="ProjectResources" style="width:845px;">
    <div class="ProjectResources-Content" style="padding:0px">
    <div class="ProjectResources-Activities">
        <div class="Project-ActivitiesFilter" style="padding:0px;">
                  <div class="ProjectResources-YellowBottomRightCorner">
                    <div class="ProjectResources-YellowBottomLeftCorner">
                      <div  class="ProjectResources-YellowTopRightCorner">
                        <div  class="ProjectResources-YellowTopLeftCorner">
                          <table cellpadding="0" cellspacing="0" border="0" width="100%">
                            <tr>
                              <td><div class="Project-ActivitiesFilterHeading">
                                  <h4>
                                    Filter
                                  </h4>
                                </div>
                                <div class="Project-ActivitiesFilterCloseLink">
                                  <a href="#">&laquo;Close</a>
                                </div></td>
                            </tr>
                            <tr>
                              <td><table cellpadding="0" class="Project-ActivitiesStatusType"  cellspacing="0" width="100%" border="0">
                                        @OPTIONS
                                </table></td>
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
   </div>
</div>
EOD
);

$HTMLgroup_member_tab = t(<<<EOD
    @FILTER_PLACEHOLDER
    @MEMBERS_LIST_PLACEHOLDER
EOD
);

/*
 $HTMLgroup_member_tab =t(<<<END
 <h1 style="color:#3570aa;padding:0px;margin:0px">
  People
</h1>
<div id="BoldrProject">
  <div class="ProjectResources">
    <div class="ProjectResources-Content" style="padding:0px">
      <div class="ProjectResources-Activities">
        <div>
          <table>
            <tr>
              <td><input id="search-text" value="Enter Person's Name" style="width:200px"  /></td>
              <td><input type="button" value="Search" class="SearchBtn" onclick="people_search()" /></td>
            </tr>
          </table>
        </div>
        <div style="font-size:5px;">
          &nbsp;
        </div>
        <div class="Project-ActivitiesFilter" style="padding:0px;">
          <div class="ProjectResources-YellowBottomRightCorner">
            <div class="ProjectResources-YellowBottomLeftCorner">
              <div  class="ProjectResources-YellowTopRightCorner">
                <div  class="ProjectResources-YellowTopLeftCorner">
                  <table cellpadding="0" cellspacing="0" border="0" width="100%">
                    <tr>
                      <td><div class="Project-ActivitiesFilterHeading">
                          <h4>
                            Filter
                          </h4>
                        </div>
                        <div class="Project-ActivitiesFilterCloseLink">
                          <a href="#">&laquo;Close</a>
                        </div></td>
                    </tr>
                    <tr>
                      <td><table cellpadding="0" class="Project-ActivitiesStatusType" cellspacing="0" width="100%" border="0">
                          <tr>
                            <td class="StatusHeading" style="white-space:nowrap">Organizations</td>
                            <td><div class="ChkBox">
                                <input type="hidden" value="@DEFAULT_ORG" id="ppl-filter-org" />
                                <input name="filter-org" type="checkbox" onclick="people_set_org('0,', this.checked)" />
                                All
                              </div>
                              <div class="ChkBox">
                                <input name="filter-org" type="checkbox" checked="1" onclick="people_set_org('@MYSCHOOL', this.checked)" />
                                My School
                              </div>
                              <div class="ChkBox">
                                <input name="filter-org" type="checkbox" onclick="people_set_org('@OTHERSCHOOLS', this.checked)" />
                                Other Schools
                              </div>
                              <div class="ChkBox">
                                <input name="filter-org" type="checkbox" onclick="people_set_org('@NTNSCHOOL', this.checked)" />
                                New Tech Network
                              </div></td>
                          </tr>
                          <tr>
                            <td colspan="2" class="yellowDottedLine">&nbsp;</td>
                          </tr>
                          <tr>
                            <td class="TypeHeading" style="white-space:nowrap">Type</td>
                            <td><div class="ChkBox">
                                <input checked="checked" type="checkbox" name="filter-roles" onclick="people_search()" id="_0" />
                                All
                              </div>
@FILTER_ROLES
                              </td>
                          </tr>
                        </table></td>
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
<div style="font-size:10px">
  &nbsp;
</div>
<div id="view-people">
@PEOPLE_VIEW
</div>
END
    );
*/
//$HTMLgroup_resource_tab .=t(<<<END
//<table cellpadding  ="0" cellspacing ="0" width="100%">
//    <tr>
//        <td colspan="2"> @TITLE </td>
//    </tr>
//    <tr>
//        <td> Associated Courses </td>
//        <td> @ASSOCIATED_COURSES</td>
//    </tr>
//    <tr>
//        <td> Associated Project</td>
//        <td>@ASSOCIATED_PROJECT</td>
//    </tr>
//    <tr>
//        <td colspan="2">Summary</td>
//    </tr>
//    <tr>
//        <td colspan="2"> @SUMMARY</td>
//     </tr>
//
//</table>
//
//END
//    );


?>
