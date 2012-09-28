// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.

$(function(){
    if(document.getElementById('agendaBodyWrapper')) {
        var agBody = document.getElementById('agendaBodyWrapper');
        if(agBody.scrollHeight > 100) {

            $('#agendaViewMoreLink').show();
            
            $('#agendaViewMoreLink a').toggle(function(){

                $('#agendaBodyWrapper').animate({"height":agBody.scrollHeight+'px'}, {complete:function() {
                    $('#agendaViewMoreLink a').text('< Show less');
                    $('#agendaBodyWrapper').attr('style', 'overflow: hidden; overflow-x:hidden !important; overflow-y:hidden !important; height:'+agBody.scrollHeight+'px');
                }});
            },
            function(){
                $('#agendaBodyWrapper').animate({"height":100+'px'}, {complete:function() {
                    $('#agendaViewMoreLink a').text('More >');
                    $('#agendaBodyWrapper').attr('style', 'overflow: hidden; overflow-x:hidden !important; overflow-y:hidden !important; height:100px');
                }});
            });

            $('#agendaBodyWrapper').css('overflow-y', 'hidden !important');
        } else {
            $('#agendaViewMoreLink').hide();
        }
    }
});

function getCalendarTitle(mm, yy) {
    var em = "";
    switch (mm) {
        case 1:
            em = "Jan";
            break;
        case 2:
            em = "Feb";
            break;
        case 3:
            em = "Mar";
            break;
        case 4:
            em = "Apr";
            break;
        case 5:
            em = "May";
            break;
        case 6:
            em = "Jun";
            break;
        case 7:
            em = "Jul";
            break;
        case 8:
            em = "Aug";
            break;
        case 9:
            em = "Sep";
            break;
        case 10:
            em = "Oct";
            break;
        case 11:
            em = "Nov";
            break;
        case 12:
            em = "Dec";
            break;
    }
    return em+" "+yy;
}
function daysInMonth(month, year) {
    var dd = new Date(year, month, 0);
    return dd.getDate();
}

function edit_agenda() {

    var node_id = this.name;
    var nid = $('#hnodeid').val();
    var yy = $('#current-year').val();
    var mm = $('#current-month').val();
    var dd = $('#current-day').val();
    var param  = nid+'/'+yy+'/'+mm+'/'+dd;
    var aurl = Drupal.settings.basePath+"?q=ntlp/agenda/edit/"+param;
//    alert('URL: ' + aurl);
    $.ajax({
        type: "GET",
        url: aurl,
        cache: false,
        success: function(data){
            $('#agenda-edit').css('display', 'none');
            $('#agenda-save').css('display', 'block');

            var wrapper = $("#agenda-body");
            var new_content = $('<div></div>').html(data);
            wrapper.empty().append(new_content);

            // Attach all javascript behaviors to the new content, if it was successfully
            // added to the page, this if statement allows #ahah[wrapper] to be optional.
            
            var textarea_id = 'agenda-text';
            if (typeof(Drupal.settings.ckeditor) == 'undefined') {
                jQuery.extend(Drupal.settings, {"ckeditor": {"module_path": Drupal.settings.basePath+"sites/all/modules/ckeditor", "linktocontent_node": true, "linktocontent_menu": true, "theme": [ "boldr", "boldr" ], "settings": {"agenda-text": {"customConfig": Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor.config.js?1265657154", "defaultLanguage": "en", "toolbar": "DrupalBasic", "enterMode": 1, "shiftEnterMode": 2, "toolbarStartupExpanded": true, "width": "90%", "height": 170, "skin": "office2003", "format_tags": "p;div;pre;address;h1;h2;h3;h4;h5;h6", "stylesCombo_stylesSet": "drupal:/"+Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor.styles.js", "contentsCss": [ Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor.css", Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor/contents.css" ], "uiColor": "default", "filters": [ "filter/0/1" ]}}, "autostart": {"agenda-text": true}}});
            } else if (typeof(Drupal.settings.ckeditor.settings) == 'undefined') {
                jQuery.extend(Drupal.settings.ckeditor, {"settings": {"agenda-text": {"customConfig": Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor.config.js?1265657154", "defaultLanguage": "en", "toolbar": "DrupalBasic", "enterMode": 1, "shiftEnterMode": 2, "toolbarStartupExpanded": true, "width": "90%", "height": 170, "skin": "office2003", "format_tags": "p;div;pre;address;h1;h2;h3;h4;h5;h6", "stylesCombo_stylesSet": "drupal:/"+Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor.styles.js", "contentsCss": [ Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor.css", Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor/contents.css" ], "uiColor": "default", "filters": [ "filter/0/1" ]}}, "autostart": {"agenda-text": true}});
            }
            if ((typeof(Drupal.settings.ckeditor.load_timeout) == 'undefined')) {
                jQuery.extend(Drupal.settings.ckeditor, {"load_timeout": 30});
            }
            if (typeof(CKEDITOR.instances[textarea_id] != 'undefined')) {
                delete CKEDITOR.instances[textarea_id];
            }
            Drupal.ckeditorOn(textarea_id);
//            Drupal.attachBehaviors(new_content);

            setup_date_picker();
        }
    });
    return false;
}

function save_agenda() {

    var nid = $('#hnodeid').val();
    var fd = $('#agenda-fromdate').val();
    var td = $('#agenda-todate').val();
    var atext = $('#agenda-text').val();        //Get value from textarea to be on the safe side

    //Get rich contents from RTE, if its instance is available.
    if (typeof(CKEDITOR.instances["agenda-text"] != 'undefined')) {
        var e = CKEDITOR.instances["agenda-text"];
        atext = e.getData();
    }

    var args = fd.split('/');
    var param  = nid+'/'+args[2]+'/'+args[0]+'/'+args[1];   //yy/mm/dd
    args = td.split('/');
    param += '/'+args[2]+'/'+args[0]+'/'+args[1];   //yy/mm/dd

    var aurl = Drupal.settings.basePath+"?q=ntlp/agenda/save/"+param;
//    alert('URL: ' + aurl);
    $.ajax({
        type: "POST",
        url: aurl,
        cache: false,
        data: "agenda="+escape(atext),
        success: function(data){
            $('#agenda-body').html(data);
            $('#agenda-edit').css('display', 'block');
            $('#agenda-save').css('display', 'none');
        },
        error: function(){
            alert('error');
        }
    });


    return false;
}

if(Drupal.jsEnabled){

    $(document).ready(function() {

        $('.previous').click(function(event) {

            var nid = $('#hnodeid').val();
            var yy = $('#current-year').val();
            var mm = $('#current-month').val();
            var dd = $('#current-day').val();
            mm--;
            if (mm < 1) {
                mm = 12;
                yy--;
            }
            ldm = daysInMonth(mm, yy);
            if (dd > ldm)
                dd = ldm;
            var param  = nid+'/'+yy+'/'+mm+'/'+dd;
//            alert(param);

            var frmDrupal = function(data) {
                var result = Drupal.parseJson(data);
                $('#calendar').html(data);
                $('#current-day').val(dd);
                $('#current-month').val(mm);
                $('#current-year').val(yy);
                $('#current-title').html(getCalendarTitle(mm, yy));
                $('#agenda-edit').css('display', 'block');
                $('#agenda-save').css('display', 'none');
            }
            $.get(Drupal.settings.basePath+'?q=ntlp/agenda/get_calendar/'+param, {
            }, frmDrupal);
            return false;
        });


        $('.next').click(function(event) {

            var nid = $('#hnodeid').val();
            var yy = $('#current-year').val();
            var mm = $('#current-month').val();
            var dd = $('#current-day').val();
            mm++;
            if (mm > 12) {
                mm = 1;
                yy++;
            }
            ldm = daysInMonth(mm, yy);
            if (dd > ldm)
                dd = ldm;
            var param  = nid+'/'+yy+'/'+mm+'/'+dd;
//            alert(param);

            var frmDrupal = function(data) {
                var result = Drupal.parseJson(data);
                $('#calendar').html(data);
                $('#current-day').val(dd);
                $('#current-month').val(mm);
                $('#current-year').val(yy);
                $('#current-title').html(getCalendarTitle(mm, yy));
                $('#agenda-edit').css('display', 'block');
                $('#agenda-save').css('display', 'none');
            }
            $.get(Drupal.settings.basePath+'?q=ntlp/agenda/get_calendar/'+param, {
            }, frmDrupal);
            return false;
        });


        $('.click').click(function(event) {

             var node_id = this.name;
            var nid = $('#hnodeid').val();
            var dt = new String(node_id);
            var args = dt.split('-');
            var param  = nid+'/'+args[0]+'/'+args[1]+'/'+args[2];
//            alert('Clicked: ' + param);


        var frmDrupal = function(data) {
                var result = Drupal.parseJson(data);
                $('#agenda-body').html(data);
                $('#current-year').val(args[0]);
                $('#current-month').val(args[1]);
                $('#current-day').val(args[2]);
                $('#agenda-edit').css('display', 'block');
                $('#agenda-save').css('display', 'none');
            }
            $.get(Drupal.settings.basePath+'?q=ntlp/agenda/view/'+param, {
            }, frmDrupal);
            return false;
        });

    });

}