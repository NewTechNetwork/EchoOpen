// Echo Open software Copyright © 2012 KnowledgeWorks Foundation
// ECHO OPEN trademark and logo are trademarks of New Technology Network LLC
// The Echo Open software is licensed under the GNU GPLv2.  For licensing information // please contact New Technology Network Licensing at: // webmaster@newtechnetwork.org or 935 Clinton Street, Napa, CA 94559.


var report_format_uploader = "PDF";

function attach_uploaded_file(data, fileId, fileInfo){
    var root_path = Drupal.settings.basePath;
    var element;
    var previous_value;

    if(document.getElementById(report_format_uploader + "_attached_files_hidden")){
        element = document.getElementById(report_format_uploader + "_attached_files_hidden");
//        if (report_format_uploader == 'SUB') {
            previous_value = element.value;
//        } else {
//            previous_value = '';
//        }
        element.value = previous_value + 'F,' + '-' + fileId + ',' + data + ';';
    }

    if (report_format_uploader == 'SUB') {
        previous_value = document.getElementById(report_format_uploader + "_attach_links").innerHTML;
    } else {
        previous_value = '';
    }

    document.getElementById(report_format_uploader + "_attach_links").innerHTML = previous_value + 
        '<div style="display:table;margin:2px;width:100%" id="F_'+'-'+fileId+'">'
    + ' <a style="margin-top:2px;vertical-align:top;" onclick="delete_attach(\'' + report_format_uploader + '\', \'F_'+'-'+(fileId)+'\');"><img width="14" height="14" src="'+root_path+'themes/Boldr/Images/DeleteBtn.png" border="2" /></a>'
    + ' &nbsp;&nbsp;' + data + '</div>'
    + ' <div style="display:inline-block;float:left;margin-top:10px;">' + fileInfo + '</div>';
}

function delete_hidden(elem_id, file_id){
    hData = document.getElementById(elem_id);
    var files = hData.value.split(";");

    for(i = 0 ; i< files.length; i++){
        var attfile = files[i].split(",");
        var recid = (file_id.split('_'))[1];
        var rectype = file_id.substr(0, 1);
        
        if(recid == attfile[1]){
//            alert('found:'+rectype+','+recid+":"+attfile[0]+','+attfile[1]);
            hData.value = hData.value.replace(rectype+','+recid, 'X,'+recid);
            break;
        }
    }
//    hData = document.getElementById(report_format_uploader + "_attached_files_hidden");
//    hData.value = "";
}

function delete_attach(format, id){
    //Remove (Mark X) in attached files list in the hidden variable
    delete_hidden(format + "_attached_files_hidden", id);

    hData = document.getElementById(format + "_params_hidden");
    hData.value = '';
    
    //Remove UI element for the selected file.
    //    var d = document.getElementById('attach_links');
    var olddiv = document.getElementById(id);
    olddiv.parentNode.removeChild(olddiv);
    //    var count = d.getElementsByTagName('div').length;
    //    if(count == 0 ){
    if (report_format_uploader != 'SUB') {
        $('#edit-add-report-' + format + '-layout-file-info-resource-file-wrapper').show();
        $('#' + format + '_attach_links_panel').show();
        $('#' + format + '_attach_links').hide();
        $('#file-info-resource-file').show();
    }
}

function on_fileupload_success(fileId, fileName, filePath) {
    //Compile Report and Get Parameters and other info
    $.ajax({
        type: "GET",
        url: '?q=ntlp/reports/getinfo/'+fileId+'/'+report_format_uploader,
        cache: false,
        success: function(data){
            if (typeof(data) == 'string') {
                jresponse = Drupal.parseJson(data);

                if (jresponse.status == 1) {
                    if (report_format_uploader != 'SUB') {
                        $('#edit-add-report-' + report_format_uploader + '-layout-file-info-resource-file-wrapper').hide();
                        $('#' + report_format_uploader + '_attach_links_panel').hide();
                        $('#' + report_format_uploader + '_attach_links').show();
                    }
                    msg = jresponse.message;
                    attach_uploaded_file(fileName, fileId, msg);

                    hData = document.getElementById(report_format_uploader + "_params_hidden");
                    hData.value = jresponse.data;
                }
                else
                {
                    alert("Error: " + jresponse.message);
                }
            }
        }
    });
    if (report_format_uploader != 'SUB') {
        $('#file-info-resource-file').hide();
    }
}

$(document).ready(function() {
    setupMenuButton('manageProject', "btnManageproject");
});

function onclick_report_param(checked, output_format, fileId, paramName, className) {
    if (output_format.length == 0) {
        output_format = report_format_uploader;
    }
    hData = document.getElementById(output_format + "_params_hidden");
    hData.value += fileId + "," + paramName + "," + className + "," + (checked ? 1 : 0) + ";";
}


function on_runreport(report_nid, output_format) {
    $(function(){
        $("#dialog-runningreport").dialog({
            autoOpen: false,
            resizable: false,
            height:80,
            width:350,
            modal: true,
            zIndex:9999
        });
    });
    //Get form values
    var dataString = $("#ntlp-reports-inputform-form").serialize();
    $.ajax({
        type: "POST",
        url: "?q=ntlp/reports/parseinput/"+report_nid+"/"+output_format,
        data: dataString,
        success:  function(data){
            if (typeof(data) == 'string') {
                on_runreport_loading(report_nid, output_format, data);
            }
//            parent.Drupal.modalFrame.close().closeDialog();
        }
    });
}

function on_runreport_loading(report_nid, output_format, data) {
    var jresponse = Drupal.parseJson(data);
    $("#dialog-runningreport").dialog('open');

    if (jresponse.status == 1) {
        var rurl = "?q=ntlp/reports/process/"+report_nid+"/"+output_format+jresponse.params;

        $.ajax({
            type: "GET",
            url: rurl,
            success:  function(data){
                if (typeof(data) == 'string') {
                    jresponse = Drupal.parseJson(data);

                    if (jresponse.status == 1) {
                        $("#dialog-runningreport").find('.green_link').attr('href', '?q=ntlp/reports/'+report_nid+'/'+output_format+'/echo_report.'+output_format.toLowerCase());
                        $("#dialog-runningreport").find('.msg').hide();
                        $("#dialog-runningreport").find('.dialog-title-image').hide();
                        $("#dialog-runningreport").find('.dialog-heading').text('Your report is ready.');
                        $("#dialog-runningreport").find('.green_link').show();
                    } else {
                        $("#dialog-runningreport").find('.msg').text(jresponse.message);
                    }
                } else {
                    $("#dialog-runningreport").find('.msg').text('An unhandled error occurred while running the report.');
                }
            }
        });
    }
    else
    {
        $("#dialog-runningreport").find('.msg').text(jresponse.message);
    }

}

function delete_report(report_nid) {
     $(function(){
        $("#dialog-managereport").dialog({
            autoOpen: false,
            resizable: false,
            height:80,
            width:350,
            modal: true,
            zIndex:9999
        });
    });
   $("#dialog-managereport").dialog('option', 'buttons',{
        'Yes': function() {
            delete_report_confirmed(report_nid);
            $(this).dialog('close');
        },
        'Cancel': function() {
            $(this).dialog('close');
        }
    });

    var msg = 'Are you sure you want to delete this report? ';
    $("#dialog-managereport").find('.msg').css('color', '#000000').text(msg);
    $("#dialog-managereport").dialog('open');
}

function delete_report_confirmed(report_nid) {
    $.ajax({
        type: "GET",
        url: "?q=ntlp/reports/delete/"+report_nid,
        success:  function(data){
            if (typeof(data) == 'string') {
                jresponse = Drupal.parseJson(data);

                if (jresponse.status == 1) {
                    $("#dialog-managereport").dialog('close');
                    window.location.reload();
                } else {
                   $("#dialog-managereport").dialog('option', 'buttons',{
                        'Close': function() {
                            $(this).dialog('close');
                        }
                    });
                    $("#dialog-managereport").find('.msg').css('color', 'red').text(jresponse.message);
                }
            }

        }
    });
}