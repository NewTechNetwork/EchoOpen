// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$(document).ready(function() {

    $('.txtbox_focus_text').focus(
        function() {
            $(this).val('');
        }
        );
    $('.txtbox_focus_text').blur(
        function() {
            var title = $(this).attr('title');
            if($(this).val()== '') {
                $(this).val(title);
            }else {
        }

        }
        );
});


/**
 * Comment
 */

function fitlerStudentsByName(str) {
    Drupal.myFilterName = str;
    $('#studentSearchWait').fadeIn();
    $("input[class^='myFilter']:first").trigger('onclick');
//    var obj = event.target || event.srcElement;
//    this.callback = function(str){
//        Drupal.myFilterName = str;
//        $("input[class^='myFilter']:first").trigger('onclick');
//    };
    
//    clearTimeout(this.t);
//    this.t = setTimeout(function(){
//                this.callback(obj.value);
//    }, 1000);
}