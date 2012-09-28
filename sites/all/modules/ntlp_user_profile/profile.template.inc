<?php
// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$HTMLpersoninfo_hdr = t(<<<END
   <table><tr><td valign="top" width="221px" style="padding-right:10px;">
<div style="width:221px" class="teacherProfile">
  <div class="yellowBacktopleft">
    <div class="yellowBacktopright">
      <div class="yellowBackbottomleft">
        <div class="yellowBackbottomright">
        <div class="teacherHeading">
END
);

$HTMLpersoninfo_ftr = t(<<<END
        </div>
       </div>
      </div>
    </div>
  </div>
</div>
END
);



$HTMLquote_hdr = t(<<<END
<td valign="top"><div style="width:610px" class="teacherProfile">
  <!-- QUOTE
  <div class="greenBacktopleft">
    <div class="greenBacktopright">
      <div class="greenBackbottomleft">
        <div class="greenBackbottomright">
END
);

$HTMLquote_ftr = t(<<<END
         </div>
      </div>
    </div>
  </div>
  QUOTE -->
</div>

END
);






$HTMLaboutme_hdr = t(<<<END
<div style="width:610px" class="teacherProfile">
  <div class="blueBacktopcenter">
      <div class="blueBacktopright">
        <div class="blueBackbottomleft">
          <div class="blueBackbottomright">
            <h2 class="teacherprofileHeading">About Me</h2>
            <div class="profileDetail">
              <div class="whiteBacktopleft">
                <div class="whiteBacktopright">
                  <div class="whiteBackbottomleft">
                    <div class="whiteBackbottomright">
                    <div style="padding:10px;" >
                      <table class="table" cellpadding="8" cellspacing="0" border="0" width="100%">
END
);

$HTMLaboutme_ftr = t(<<<END
         </table></div>
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









$HTMLcourses_hdr = t(<<<END
<div style="width:610px" class="teacherProfile">
  <div class="blueBacktopcenter-Courses">
    <div class="blueBacktopleft-CoursesLogo">
      <div class="blueBacktopright-Courses">
        <div class="blueBackbottomleft">
          <div class="blueBackbottomright">
		  <h2 class="teachercoursesHeading">Courses</h2>
END
);

$HTMLcourses_ftr = t(<<<END
              </div>
        </div>
      </div>
    </div>
  </div>
  </div>
END
);


$HTMLWhiteBoxStart = t(<<<EOD
<div class="profileDetail" style="width:284px;float:left;">
    <div class="whiteBacktopleft">
        <div class="whiteBacktopright">
            <div class="whiteBackbottomleft">
                <div class="whiteBackbottomright" style="min-height:150px;">
                    <div style="padding:10px 10px 10px 10px;">
                        <!-- ADVISOR -->
                        <h2 class="teacherHeading">@WHITEBOX_HEADING</h2>
EOD
);

$HTMLWhiteBoxEnd = t(<<<EOD
                     </div>
                </div>
            </div>
        </div>
    </div>
</div>

EOD
);
$HTMLWhiteBoxStart2 = t(<<<EOD
<div class="profileDetail" style="width:284px;float:left;margin:0px;padding:16px 0 0 0 ">
    <div class="whiteBacktopleft">
        <div class="whiteBacktopright">
            <div class="whiteBackbottomleft">
                <div class="whiteBackbottomright" style="min-height:150px;">
                    <div style="padding:10px 10px 10px 10px;">
                        <!-- ADVISOR -->
                        <h2 class="teacherHeading">@WHITEBOX_HEADING</h2>
EOD
);

$HTMLWhiteBoxEnd2 = t(<<<EOD
                     </div>
                </div>
            </div>
        </div>
    </div>
</div>

EOD
);

$HTMLWhiteBoxCourseLinkStart = '<ul class="coursesLinks">';
$HTMLWhiteBoxCourseLinkEnd = '</ul>';
