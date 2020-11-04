/**
 * @file
 * Attaches behaviors for the Simple Access module.
 */
(function ($) {

"use strict";

Backdrop.behaviors.simpleAccessFieldsetSummaries = {
  attach: function (context) {
    $(context).find('fieldset.simple-access-settings').backdropSetSummary(function (element) {
      var $element = $(element);
      console.log($element);
      var checkbox = $element.find('.form-checkbox:checked').size();

      if (!checkbox) {
        return Backdrop.t('Public access');
      }
      else {
        return Backdrop.t('Restricted access');
      }
    });
  }
};

})(jQuery);