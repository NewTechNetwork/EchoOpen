/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */


(function($) {
    if (Drupal.jsEnabled && Drupal.ahah != undefined) {
    /**
     * Override of Drupal.ahah.prototype.success.
     */
        $(function(){
            var ahahSuccessProxy = Drupal.ahah.prototype.success;
            Drupal.ahah.prototype.success = function () {
              // @TODO: Remove all the code that was here from other files and use proxy pattern, so that nothing breaks on drupal update.

              // Calling the orignal method i.e. proxy method
              ahahSuccessProxy.apply(this, arguments);

              // Do custom calls below.
              if($.prototype.sSelect) {
                  $myDropDowns = $('.my-dropdown');
                  if($myDropDowns.length > 0) {
                      $myDropDowns.each(function(){
                          if(!$(this).next().hasClass('newListSelected')) {
                              $(this).sSelect();
                          }
                      });
                  }
              }
            };
        });
    }
})(jQuery);