// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.

Drupal.behaviors.fixMyPager = function(context){

//    if(context != document && $(context).find('div.view').length == 0) {
    if(context != document) {
        $('.pager li a, thead th a:not(.fix_protected)', context).live('click', function(){
                var href = $(this).attr('href');
                
                $.get(href, function(response){
                        $(context).html(response);
                        $('html, body').animate({"scrollTop":$(context).offset().top});
                });
                return false;
        });
    }
};

Drupal.behaviors.changeHeaderLinks = function(context, link, replaceWith){
//    if(context != document && $(context).find('div.view').length == 0) {
    if(context != document) {
        $('thead th a', context).each(function () {
            var href = $(this).attr('href');
            href = href.replace(link, replaceWith);
            $(this).attr('href', href);
        });
    }
};

$(document).ready (function () {
    $('.filter_link').click(function () {
        if($(this).html()=='« Close') {
            $(this).html('» Open');
            $('.filter_hide').css('display','none');
//            $('.filter_hide').hide('slow');

            //Call click methods of all default filters
            $('.myFilter-default-opt').click();

        }
        else{
            $(this).html('« Close');
            $('.filter_hide').css('display','block');
//            $('.filter_hide').show('slow');
        }
    });
});
