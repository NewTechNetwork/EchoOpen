$(document).ready (function () {
    setTimeout('load_gapps_block()', 5000);
});

function load_gapps_block() {

    $.ajax({
        type: "GET",
        url: "?q=ntlp/gapps/gmail/block",
        cache: false,
        success:  function(data){
            $("#gmail_block").html(data);
            
            $.ajax({
                type: "GET",
                url: "?q=ntlp/gapps/gdocs/block",
                cache: false,
                success:  function(data){
                    $("#gdocs_block").html(data);
                
                }
            });

             $.ajax({
                type: "GET",
                url: "?q=ntlp/gapps/gsites/block",
                cache: false,
                success:  function(data){
                    $("#gsites_block").html(data);

                }
            });
        }
    
    });
}

