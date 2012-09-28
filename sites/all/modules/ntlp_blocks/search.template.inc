<?php


$HTMLSearchPanel = t(<<<EOD

<div class="view-filters">
    <div class="search_panel">
	<input type=" text" name="text"  id="edit-keys" value="@DEFAULT_SEARCH_TEXT"
               onclick="if (this.value=='@ORG_DEFAULT_SEARCH_TEXT' || this.value.length<1) {this.value='';} this.style.color='#000';"
               onkeydown="
                if (event.keyCode == 13 && this.value != '') {
                    triggerFilter('keys', this.value);
                }
                "
               onblur="if (this.value.length<1) { this.style.color='#999999'; this.value='@ORG_DEFAULT_SEARCH_TEXT'; }" 
               style="width:460px; font-family:Tahoma, Arial, Helvetica, sans-serif;"

        >
<script type="text/javascript">
<!--

if (document.getElementById('edit-keys').value!='@ORG_DEFAULT_SEARCH_TEXT' ) {
    document.getElementById('edit-keys').style.color='#000000';
    }
//-->
</script>




    </div>
    <div class="search_btn_div">
        <input onclick="
                var searchKey = document.getElementById('edit-keys').value;
                
                if (searchKey != '' && searchKey != '@ORG_DEFAULT_SEARCH_TEXT') {
                    triggerFilter('keys', searchKey);
                }
                
                "
                type="button" class="search_panel_Btn" value="Search">
    </div>
    <div style=" width:170px; text-align:left; margin-top: 10px;" name="search_result_text" class="search_btn_div section-top-visibility"></div>
</div>

EOD
);

?>
