// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


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

function show_event(event_nid) {
    $('#heventid').val(event_nid);
    var aurl = Drupal.settings.basePath+"?q=ntlp/events/show/"+event_nid;
    //    alert('URL: ' + aurl);
    $.ajax({
        type: "GET",
        url: aurl,
        cache: false,
        success: function(data){

            $.ajax({
                type: "GET",
                url: Drupal.settings.basePath+"?q=ntlp/events/editable/"+event_nid,
                cache: false,
                success: function(edit){
                    if (edit)
                        $('#events-edit').css('display', 'block');
                    else
                        $('#events-edit').css('display', 'none');

                    $('#events-save').css('display', 'none');
                    $('#events-add').css('display', 'none');
                    $('#events-body').html(data);
                }
            });


            setup_date_picker();
        }
    });
    return false;
}

function add_events() {

    var gid = $('#hnodeid').val();
    var aurl = Drupal.settings.basePath+"?q=ntlp/events/edit/"+gid+"/0";
    //    alert('URL: ' + aurl);
    $.ajax({
        type: "GET",
        url: aurl,
        cache: false,
        success: function(data){
            $('#heventid').val(0);
            $('#events-edit').css('display', 'none');
            $('#events-save').css('display', 'block');
            $('#events-add').css('display', 'none');

            $('#agenda-edit').css('display', 'none');
            $('#agenda-save').css('display', 'block');

            var wrapper = $("#events-body");
            var new_content = $('<div></div>').html(data);
            wrapper.empty().append(new_content);

            // Attach all javascript behaviors to the new content, if it was successfully
            // added to the page, this if statement allows #ahah[wrapper] to be optional.
            var textarea_id = 'events-text';
            if (typeof(Drupal.settings.ckeditor) == 'undefined') {
                
                jQuery.extend(Drupal.settings, {
                    "ckeditor": {
                        "module_path": Drupal.settings.basePath+"sites/all/modules/ckeditor",
                        "linktocontent_node": true,
                        "linktocontent_menu": true,
                        "theme": [ "boldr", "boldr" ],
                        "settings": {
                            "events-text": {
                                "customConfig": Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor.config.js?1265657154",
                                "defaultLanguage": "en",
                                "toolbar": "DrupalBasic",
                                "enterMode": 1,
                                "shiftEnterMode": 2,
                                "toolbarStartupExpanded": true,
                                "width": "90%",
                                "height": 200,
                                "skin": "office2003",
                                "format_tags": "p;div;pre;address;h1;h2;h3;h4;h5;h6",
                                "stylesCombo_stylesSet": "drupal:/"+Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor.styles.js",
                                "contentsCss": [ Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor.css", Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor/contents.css" ],
                                "uiColor": "default",
                                "filters": [ "filter/0/1" ]
                                }
                            },
                    "autostart": {
                        "events-text": true
                    }
                }
                });
    } else if (typeof(Drupal.settings.ckeditor.settings) == 'undefined') {
        jQuery.extend(Drupal.settings.ckeditor, {
            "settings": {
                "events-text": {
                    "customConfig": Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor.config.js?1265657154",
                    "defaultLanguage": "en",
                    "toolbar": "DrupalBasic",
                    "enterMode": 1,
                    "shiftEnterMode": 2,
                    "toolbarStartupExpanded": true,
                    "width": "90%",
                    "height": 170,
                    "skin": "office2003",
                    "format_tags": "p;div;pre;address;h1;h2;h3;h4;h5;h6",
                    "stylesCombo_stylesSet": "drupal:/"+Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor.styles.js",
                    "contentsCss": [ Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor.css", Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor/contents.css" ],
                    "uiColor": "default",
                    "filters": [ "filter/0/1" ]
                    }
                },
        "autostart": {
            "events-text": true
        }
        });
}
if ((typeof(Drupal.settings.ckeditor.load_timeout) == 'undefined')) {
    jQuery.extend(Drupal.settings.ckeditor, {
        "load_timeout": 30
    });
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

function edit_events() {

    var gid = $('#hnodeid').val();
    var nid = $('#heventid').val();
    var aurl = Drupal.settings.basePath+"?q=ntlp/events/edit/"+gid+'/'+nid;
    //    alert('URL: ' + aurl);
    $.ajax({
        type: "GET",
        url: aurl,
        cache: false,
        success: function(data){
            $('#events-edit').css('display', 'none');
            $('#events-save').css('display', 'block');
            $('#events-add').css('display', 'none');

            var wrapper = $("#events-body");
            var new_content = $('<div></div>').html(data);
            wrapper.empty().append(new_content);

            // Attach all javascript behaviors to the new content, if it was successfully
            // added to the page, this if statement allows #ahah[wrapper] to be optional.
            var textarea_id = 'events-text';
            if (typeof(Drupal.settings.ckeditor) == 'undefined') {
                jQuery.extend(Drupal.settings, {
                    "ckeditor": {
                        "module_path": Drupal.settings.basePath+"sites/all/modules/ckeditor",
                        "linktocontent_node": true,
                        "linktocontent_menu": true,
                        "theme": [ "boldr", "boldr" ],
                        "settings": {
                            "events-text": {
                                "customConfig": Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor.config.js?1265657154",
                                "defaultLanguage": "en",
                                "toolbar": "DrupalBasic",
                                "enterMode": 1,
                                "shiftEnterMode": 2,
                                "toolbarStartupExpanded": true,
                                "width": "90%",
                                "height": 200,
                                "skin": "office2003",
                                "format_tags": "p;div;pre;address;h1;h2;h3;h4;h5;h6",
                                "stylesCombo_stylesSet": "drupal:/"+Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor.styles.js",
                                "contentsCss": [ Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor.css", Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor/contents.css" ],
                                "uiColor": "default",
                                "filters": [ "filter/0/1" ]
                                }
                            },
                    "autostart": {
                        "events-text": true
                    }
                }
                });
    } else if (typeof(Drupal.settings.ckeditor.settings) == 'undefined') {
        jQuery.extend(Drupal.settings.ckeditor, {
            "settings": {
                "events-text": {
                    "customConfig": Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor.config.js?1265657154",
                    "defaultLanguage": "en",
                    "toolbar": "DrupalBasic",
                    "enterMode": 1,
                    "shiftEnterMode": 2,
                    "toolbarStartupExpanded": true,
                    "width": "90%",
                    "height": 170,
                    "skin": "office2003",
                    "format_tags": "p;div;pre;address;h1;h2;h3;h4;h5;h6",
                    "stylesCombo_stylesSet": "drupal:/"+Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor.styles.js",
                    "contentsCss": [ Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor.css", Drupal.settings.basePath+"sites/all/modules/ckeditor/ckeditor/contents.css" ],
                    "uiColor": "default",
                    "filters": [ "filter/0/1" ]
                    }
                },
        "autostart": {
            "events-text": true
        }
        });
}
if ((typeof(Drupal.settings.ckeditor.load_timeout) == 'undefined')) {
    jQuery.extend(Drupal.settings.ckeditor, {
        "load_timeout": 30
    });
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

function save_events() {

    var nid = $('#heventid').val();
    var gid = $('#hnodeid').val();
    var fd = $('#events-fromdate').val();
    var td = $('#events-todate').val();
    var atitle = $('#events-title').val();
    var atext = $('#events-text').val();    //Get value from textarea to be on the safe side
    //Get rich contents from RTE, if its instance is available.
    if (typeof(CKEDITOR.instances["events-text"] != 'undefined')) {
        var e = CKEDITOR.instances["events-text"];
        atext = e.getData();
    }
    var post_to = 'G';
    var x = document.getElementsByName('visibility_to');
    for(i=0; i < x.length; i++){
        if (x[i].checked) {
            post_to = x[i].value;        //This function will not loop, as only one will be selected
        //            alert(post_to);
        }
    }

    var args = fd.split('/');
    var param  = nid+'/'+gid+'/'+args[2]+'/'+args[0]+'/'+args[1];   //yy/mm/dd
    args = td.split('/');
    param += '/'+args[2]+'/'+args[0]+'/'+args[1];   //yy/mm/dd
    param += '/'+post_to;
    var aurl = Drupal.settings.basePath+"?q=ntlp/events/save/"+param;
    //    alert('URL: ' + aurl);
    $.ajax({
        type: "GET",
        url: aurl,
        cache: false,
        data: "body="+escape(atext)+"&title="+escape(atitle),
        success: function(data){
            show_current_event_list();
        },
        error: function(){
            alert('Unknown error, unable to save. Please try again.');
        }
    });
    return false;
}

function onchange_filters() {
    //Get all selected filter checkbox values and call the event list view function
    var event_types = '';
    $('#event-filters :checkbox:checked').each(function() {
        event_types = event_types + "'" + $(this).val() + "',";
    });
    var nid = $('#hnodeid').val();
    var yy = $('#current-year').val();
    var mm = $('#current-month').val();
    var dd = $('#current-day').val();
    var param  = nid+'/'+yy+'/'+mm+'/'+dd;
    var aurl = Drupal.settings.basePath+"?q=ntlp/events/view/"+param;
    $.ajax({
        type: "GET",
        url: aurl,
        cache: false,
        data: "filters="+urlencode(event_types),
        success: function(data){
            $('#events-body').html(data);

            //Reset the checkboxes, as they are overwritten/cleared by the ajax call
            $('#event-filters :checkbox').each(function() {
                if ($(this).val().toString() == 'all') {
                    //alert('Turnoff:'+$(this).val());
                    $(this).removeAttr("checked");
                }
                if (event_types.indexOf("'"+$(this).val().toString()+"'") >= 0) {
                    //                    alert('Resetting:'+$(this).val());
                    $(this).attr("checked", "checked");
                }
            });
        }
    });
    return false;

}
function close_filter() {
    $('.filter_hide').hide('slow');
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
                $('#events-edit').css('display', 'none');
                $('#events-save').css('display', 'none');
                $('#events-add').css('display', 'block');
            }
            $.get(Drupal.settings.basePath+'?q=ntlp/events/get_calendar/'+param, {
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
                $('#events-edit').css('display', 'none');
                $('#events-save').css('display', 'none');
                $('#events-add').css('display', 'block');
            }
            $.get(Drupal.settings.basePath+'?q=ntlp/events/get_calendar/'+param, {
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

                $('#events-body').html(data);
                $('#current-year').val(args[0]);
                $('#current-month').val(args[1]);
                $('#current-day').val(args[2]);
                $('#events-edit').css('display', 'none');
                $('#events-save').css('display', 'none');
                $('#events-add').css('display', 'block');

 
            }
            
            $.get(Drupal.settings.basePath+'?q=ntlp/events/view/'+param, {
                }, frmDrupal);
            //            $.get('?q=availability-calendars/showme/'.nid, frmDrupal);
            return false;
        });

    //        $('#event-filters :checkbox').click(function(event) {
    //        });

    });
}

// PHP-compatible urlencode() for Javascript
function urlencode(s) {
    s = encodeURIComponent(s);
    return s.replace(/~/g,'%7E').replace(/%20/g,'+');
}



function onclick_remove_event(event_id) {

    delete_event(event_id);

    $('.dialog-confirm-ntk').dialog('open');
}

function delete_event(eid){
 
    $(function(){
        $(".dialog-confirm-ntk").dialog({

            autoOpen: false,
            resizable: false,
            height:120,
            width:400,
            modal: true,
            buttons: {
                'Delete': function() {
                    var dlg = this;
                    $.get('?q=ntlp/events/delete/data/'+eid, function(){
                        $(dlg).dialog('close');

                        show_current_event_list();

                    });

                },
                Cancel: function() {
                    $(this).dialog('close');
                }
            }
        });

    });
}

function show_current_event_list() {

    var nid = $('#hnodeid').val();
    var yy = $('#current-year').val();
    var mm = $('#current-month').val();
    var dd = $('#current-day').val();

    var param  = nid+'/'+yy+'/'+mm+'/'+dd;
    var frmDrupal = function(data) {
        var result = Drupal.parseJson(data);

        $('#events-body').html(data);

        $('#events-edit').css('display', 'none');
        $('#events-save').css('display', 'none');
        $('#events-add').css('display', 'block');
    }

    $.get(Drupal.settings.basePath+'?q=ntlp/events/view/'+param, {
        }, frmDrupal);
}