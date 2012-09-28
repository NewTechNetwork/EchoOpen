// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.

$(document).ready (function () {
    $('.persons_name_search').focus(function() {
        $(this).val('');
    });

    $('.persons_name_search').blur(function() {
        var title = $(this).attr('title');
        if($(this).val()== '') {
            $(this).val(title);
        }
    });

    $('.filter_close').click(function () {
        if($(this).html()=='« Close') {
            $(this).html('« Open');
            $('.filter_hide').css('display','none');
        }
        else{
            $(this).html('« Close');
            $('.filter_hide').css('display','block');
        }
    });


});

function people_set_org(org_id, selected) {
    var o = document.getElementById("ppl-filter-org");


    var n = $("input[name='filter-org']:checked").length;
    if(n == 0){
        o.value = '';
        org_id = '';
    }else{
        if (o) {
            var org_ids = o.value;

           
            if (selected) {
                if(n == 1){
                    org_ids += org_id;

                }else{
                    org_ids += ","+org_id;
                }
                
            } else {
                org_ids = org_ids.replace(org_id+",", '');
            }
            
            o.value = org_ids;
            people_search(-1);
        }
    }

 
}

function people_search($check_box_id) {
    var filterOrgs = '';      //S is just a starting point, so that this value
    var filterRoles = 'S';     //does no come blank in the url

    //Get text to search

    var all_element = document.getElementById("_0");

    
    
    var searchText = document.getElementById("search-text").value;

    //Get Organization filter checkboxes
    var o = document.getElementById("ppl-filter-org");
    if (o) {
        filterOrgs += (document.getElementById("ppl-filter-org").value);
    } else {
        filterOrgs = '';
    }


    //Get Roles filter checkboxes
    var x = document.getElementsByName('filter-roles');

    var k = $("input[name='filter-roles']:checked");

    var checked = false;
    for(i=0; i < x.length; i++){
        if(all_element != null && all_element != 'undefined' ){
            if(x[i].id == $check_box_id){
                checked = x[i].checked;
            }
        }
    }
    if($check_box_id == "_0"){//all checkbox clicked
        if (checked) {
            //uncheck all others
            for(i=0; i < x.length; i++){
                if (x[i].checked) {
                    if(all_element != null && all_element != 'undefined' ){
                        if(x[i].id != '_0'){
                            x[i].checked = false;
                        }
                    }
                }
            }
        }
    }
    else { //some other checkbox clicked
        if (checked) {
            for(i=0; i < x.length; i++){
                if (x[i].checked) {
                    if(all_element != null && all_element != 'undefined' ){
                        if(x[i].id == '_0'){
                            all_element.checked = false;
                        }
                    }
                }
            }
        }
    }

    

    for(i=0; i < x.length; i++){
        if (x[i].checked) {
            filterRoles += x[i].id.split("_");
                   
        }
    }
    searchText = searchText.toUpperCase();


    $("input[type=checkbox][checked]").each(function(){
        $("#lightbox, #lightbox-panel").fadeIn(300);
    });

    $.ajax({
        type: "GET",
        url: "?q=ntlp/people/search/"+filterOrgs+"/"+filterRoles,
        data: "search="+searchText,
        cache: false,
        success: function(data){
            $("#view-people").html(data);
            $("#lightbox, #lightbox-panel").fadeOut(300);
            $('#view-people .pager li a, #view-people thead th a').live('click',function(){
                $.get( $(this).attr('href'), function(pagerData){
                    $("#view-people").html(pagerData);
                    Drupal.behaviors.fixMyPager('#view-people');
                });
                
                return false;
            });
        }
    });
}
