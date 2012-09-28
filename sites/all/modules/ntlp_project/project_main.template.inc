<?php

$HTML_CURRENT_PROJECT_DISPLAY = t(<<<END
   <div align="center" class="section_top_content" style="margin-left:10px; margin-top:10px;margin-bottom:10px;">
       <div style="width: 170px; float: left;">
           <a href="@PROJECT_GOTO_LINK">
            <img hspace="10" vspace="10" style="border: 0px solid rgb(55, 95, 146);" title="" src="@PROJECT_IMAGE_PATH" width="125" height="125">
            </a>
        </div>
        <div class="project_details">
         @VISIBILITY_HTML
        <div style="width:365px;">@PROJECT_NAME</div>
        <div class="pro_date_small">@PROJECT_FROM_AND_TO_DATE</div>

        <div style="float:left; clear:both;">

            <p><strong>Driving Question</strong></p>
            <p>@PROJECT_DESCRIPTION</p>
        </div>
    </div>
</div>
END
);

$HTML_PAST_PROJECT_DISPLAY = t(<<<END
<div class="section_bottom_content">
    <a href="@PROJECT_GOTO_LINK">
    <img height="125" width="125" style="border: 0px solid rgb(55, 95, 146);" title="" src="@PROJECT_IMAGE_PATH">
    </a>
    <div class="pro_green_title"> @PROJECT_NAME </div>
    <div class="pro_strong_date"> @PROJECT_FROM_AND_TO_DATE </div>
    <div class="section_bottom_visibility" style="color:#999">
        @VISIBILITY
        @EDIT_LINK
    </div>
</div>
END
);



$HTML_PROJECT_SETTING_PAGE_HDR = t(<<<END

    <div>
        <div id="main"> <div id="ActivitiesCSS">
                <div class="tabs">
                    <div class="LightBlueBottomRightCorner">
                        <div class="LightBlueBottomLeftCorner">
                            <div class="LightBlueTopRightCorner">
                                <div class="LightBlueTopLeftCorner">
                                    <div class="ActivitiesAssignment">
                                        <div class="formGroupbox">
                                            <table cellspacing="0" cellpadding="8" border="0" width="820px" class="table">
                                            
END
);

$HTML_PROJECT_SETTING_PAGE_FTR = t(<<<END
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