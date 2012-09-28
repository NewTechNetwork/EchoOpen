// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


function delete_comment_single(cid) {
    $.get( "?q=comment/delete/single/"+cid, function() {
        $('#comment-'+cid).fadeOut(function(){
            $(this).remove();
            console.log($('#comments-wrapper').children().length);
            if( $('#comments-wrapper').children().length == 0 ) {
                $('#no-comments').fadeIn();
            }
        });
    });

}