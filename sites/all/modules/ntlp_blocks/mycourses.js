// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.

function changeMycoursesContentAjax( url, myCallback ) {
    $.ajax({
        type: "GET",
        url: '?q=' + url,
        cache: false,
        success: function(data) {
            $('#mycourses-contents').html(data);
            if( myCallback instanceof Function ) {
                myCallback();
            }
        }
    });
}

function setActiveMyCoursesTab( tab_name ) {
    $('#tab-taking').removeClass('CoursesTabActive');
    $('#tab-teaching').removeClass('CoursesTabActive');
    
    switch( tab_name ) {

        case 'teaching':
            $('#tab-teaching').addClass('CoursesTabActive');
            break;
        case 'taking':
            $('#tab-taking').addClass('CoursesTabActive');
            break;
    }
}




function open_teaching() {
    changeMycoursesContentAjax('ntlp/blocks/get_teaching_content', function(){
      setActiveMyCoursesTab('teaching');
    });
}


function open_child_tab( user_id ) {
    changeMycoursesContentAjax('ntlp/blocks/taking_block_content/'+user_id, function(){
        $('.childrenTab').removeClass('CoursesTabActive');
        $('#child-tab-'+user_id).addClass('CoursesTabActive');
    });
}

function open_taking() {
    changeMycoursesContentAjax('ntlp/blocks/taking_block_content', function(){
      setActiveMyCoursesTab('taking');
    });
}

function getSchoolTerm( school_nid ) {
     $.ajax({
        type: "GET",
        url: "?q=ntlp/blocks/get_school_terms_combo/"+school_nid,
        cache: false,
        success: function(data){
            $("#schoolTermComboDiv").html(data);
        }
    });
}