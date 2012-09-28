<?php


$HTML_course_resources_filter_heading = t(<<<EOD
        <div class="Status">
                                    <div><b>@HEADING</b></div>
EOD
);
$HTML_course_resources_filter_row_end = t(<<<EOD
        </div>
EOD
);
$HTML_course_resources_filter_cellStart = t(<<<EOD
        <div>
EOD
);
$HTML_course_resources_filter_cellEnd = t(<<<EOD
        </div>
EOD
);
$HTML_course_resources_filter = t(<<<EOD
    <br clear="all" />
    <div class="Filter">
        <div class="yellowTopRightCorner">
            <div class="yellowBottomRightCorner">
                <div class="yellowBottomLeftCorner">
                    <div class="yellowTopLeftCorner">
                        <table cellpadding="0" cellspacing="0" width="100%" border="0" >
                        <tr>
                            <td>
                                <div>
                                     <div class="FilterHeading">
                                            <h5>Filter</h5>
                                     </div>
                                     <div class="CloseLink">
                                           <a href="#" class="filter_close">&laquo; Close</a>
                                     </div>
                                 </div>
                                <div class="clearall"></div>
                                <div class="filter_hide">@OPTIONS</div>
                            </td>
                         </tr>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
<div style="clear:both"></div>
EOD
);



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
			<div class="Heading" style="float:left"><b>Resources</b></div>
			<div class="Project-Btns">

                           @LINK_NEW
                        </div>
		</div>
	</div>
<br clear="all" />
<br clear="all" />
<div id="ActivitiesCSS">
									@FILTER_PLACEHOLDER
									<br clear="all" />
									<div class="ReportView" style="width:100% !important">
@RESOURCES_VIEW
									</div>
								</div>
</div>

END
);
?>
