
(function ($) {

Drupal.behaviors.saFieldsetSummaries = {
  attach: function (context) {
    if (!$('#edit-sa .form-checkbox:checked').size()) {
      return Drupal.t('Public access');
    }
    else {
      return Drupal.t('Restricted access');
    }
  }
};

})(jQuery);