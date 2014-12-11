(function ($) {

Drupal.behaviors.initColorbox = function (context) {
  if (!$.isFunction($.colorbox)) {
    return;
  }
/*  $('a, area, input', context)
    .filter('.colorbox:not(.initColorbox-processed)')
    .addClass('initColorbox-processed')
    .colorbox(Drupal.settings.colorbox); */
  $('a')
    .filter('.colorbox:not(.initColorbox-processed)')
    .addClass('initColorbox-processed')
    .colorbox(Drupal.settings.colorbox);

  $('area')
    .filter('.colorbox:not(.initColorbox-processed)')
    .addClass('initColorbox-processed')
    .colorbox(Drupal.settings.colorbox);

  $('input[type=text]')
    .filter('.colorbox:not(.initColorbox-processed)')
    .addClass('initColorbox-processed')
    .colorbox(Drupal.settings.colorbox);

  $('input[type!=text]')
    .filter('.colorbox:not(.initColorbox-processed)')
    .addClass('initColorbox-processed')
    .colorbox(Drupal.settings.colorbox);


  $(context)
    .filter('.colorbox:not(.initColorbox-processed)')
    .addClass('initColorbox-processed')
    .colorbox(Drupal.settings.colorbox);

};

{
  $(document).bind('cbox_complete', function () {
    Drupal.attachBehaviors('#cboxLoadedContent');
  });
}

})(jQuery);
