
function setup_date_picker(){
    //alert('dp:'+Drupal.settings.basePath+'themes/Boldr/Images/calendar.gif');
    if($(".ntlp_datepicker").length > 0){
        $(".ntlp_datepicker").datepicker({
            showOn: 'button',
            buttonImage: Drupal.settings.basePath+'themes/Boldr/Images/calendar.gif',
            buttonImageOnly: true
        });
    }
}

$(document).ready(function() {
    setup_date_picker();
});



