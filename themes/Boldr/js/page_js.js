/* //Echo Open licensed under GPLv2 (http://www.gnu.org/licenses/gpl-2.0.html)
   //Modified Date: June 30, 2011.
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

var first_run = true;
var tipsy_popup_opening = false;
var tipsy_timer;  
var tipsy_timer_interval = 5000;                

$(function (){
    $(document).click(function(){
        if(!tipsy_popup_opening){
            $('#gapps-tipsy').tipsy('hide');
            $('#gapps').attr('original-title', 'Google Apps');
            $('#gapps-tipsy').removeClass('gapps-on_png');
            $('#gapps-tipsy').addClass('gapps-default_png');
            $('#settings-tipsy').tipsy('hide');
            $('#settings').attr('original-title', 'Options');
            $('#settings-tipsy').removeClass('settings-on_png');
            $('#settings-tipsy').addClass('settings-default_png');
        }
        tipsy_popup_opening = false;
    });
});
                
                 
$(function (){
    $('.gapps-title').tipsy();
    $('.settings-title').tipsy();
});


function gapps_tipsy_call(){
        $('#gapps-tipsy').tipsy({
        className : 'grey',
        arrowClass : 'grey-arrow',
        delayIn: 0,  
        delayOut: 0,   
        fade: false, 
        fallback: '', 
        html: true,  
        live: false,   
        offset: 0,    
        opacity: 1,  
        title: function(){
            return $('#gapps-data').html();
        }, 
        trigger: 'manual' 
    });
    
    $('#gapps').attr('original-title', '');
    $('.gapps-title').tipsy('hide');
                                                                
    $('#gapps-tipsy').tipsy('show');
    $('#gapps-tipsy').removeClass('gapps-default_png');
    $('#gapps-tipsy').addClass('gapps-on_png');

    $('#settings-tipsy').tipsy('hide');
    $('#settings-tipsy').addClass('settings-default_png');
    $('#settings-tipsy').removeClass('settings-on_png');

    setTimeout('$(\'#gapps-tipsy\').tipsy(\'hide\');\n\
        $(\'#gapps\').attr(\'original-title\', \'Google Apps\');\n\
        $(\'#gapps-tipsy\').addClass(\'gapps-default_png\');\n\
        $(\'#gapps-tipsy\').removeClass(\'gapps-on_png\');',5000);

    tipsy_popup_opening = true;
                                                                
    return false;
}

function settings_tipsy_call(){
        $('#settings-tipsy').tipsy({
        className : 'grey',
        arrowClass : 'grey-arrow',
        delayIn: 0,     
        delayOut: 0,  
        fade: false,   
        fallback: '',  
        html: true,   
        live: false,   
        offset: 0,    
        opacity: 1,  
        title: function(){
            return $('#settings-data').html();
        },  
        trigger: 'manual' 
    });
    
    $('#settings').attr('original-title', '');
    $('.settings-title').tipsy('hide');
    $('#settings-tipsy').tipsy('show');
    $('#settings-tipsy').removeClass('settings-default_png');
    $('#settings-tipsy').addClass('settings-on_png');
                                                                
    $('#gapps-tipsy').tipsy('hide');
    $('#gapps-tipsy').addClass('gapps-default_png');
    $('#gapps-tipsy').removeClass('gapps-on_png');

    setTimeout('$(\'#settings-tipsy\').tipsy(\'hide\');\n\
        $(\'#settings\').attr(\'original-title\', \'Options\');\n\
        $(\'#settings-tipsy\').addClass(\'settings-default_png\');\n\
        $(\'#settings-tipsy\').removeClass(\'settings-on_png\');',5000); 
                                                                
    tipsy_popup_opening = true;
                                                                
    return false;
}
