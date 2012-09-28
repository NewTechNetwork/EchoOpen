// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


$(document).ready(function(){
	
    $('.my-dropdown').sSelect();
		
});


function selectOption(){
    $('.my-dropdown').sSelect();
}

function selectOptionInContext(strContext){
    $(strContext + ' .my-dropdown').sSelect();
}


//-----------------------------------
function setupMenuButton(menuDiv, menuBtn) {
    var md = document.getElementById(menuBtn);
    if (md != null) {
        var ahref = $("#"+menuDiv+" li.ManageOption:first a").attr("href");
        var alabel = $("#"+menuDiv+" li.ManageOption:first a").html();
        $("#"+menuDiv+" li.ManageOption:first").hide();     //Hide first option
        $("#"+menuDiv+" li.ManageOption a").attr('onclick', 'dropdownShow("'+menuDiv+'")');
        
        var alink = document.createElement('a');
        var atext = document.createTextNode(alabel);
        $(alink).addClass('ManageOptComboField');
        alink.appendChild(atext);
        alink.setAttribute('href', ahref);
        md.appendChild(alink);
        var abtn = document.createElement('a');
        $(abtn).text('');
        $(abtn).addClass('ManageOptComboBtn');
        $(abtn).click(function(){
            dropdownShow(menuDiv);
        });
        $(abtn).attr('id','comboShowhideBtn_'+menuDiv);

        /*
        abtn.innerText = '';
        abtn.setAttribute('class', 'ManageOptComboBtn');

        abtn.setAttribute('onclick', 'dropdownShow("'+menuDiv+'")');
        abtn.id = 'comboShowhideBtn_'+menuDiv;
        */
       
        //document.getElementById("comboShowhideBtn").onclick = dropdownShow(this);
        //abtn.onclick = 'dropdownShow(this)';
        md.appendChild(abtn);
        // to fix the first link in green selector
//        Drupal.behaviors.modalFrameSetup('.fixGreenCombo .ManageOptComboField');

        $('.fixGreenCombo .ManageOptComboField').addClass('modalframe-setup-child modalframe-setup-size[600,450]');
        Drupal.behaviors.modalFrameSetup();

//        modalframe-setup-child modalframe-setup-size[600,450] modalframe-setup-processed
    }
}
function dropdownShow(menu) {
    if ($('#'+menu).is(":hidden")) {
        $('#'+menu).slideDown("slow");
    } else {
        $('#'+menu).slideUp("fast");
    }
}