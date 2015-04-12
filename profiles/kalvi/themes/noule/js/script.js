(function ($) {
    Drupal.behaviors.noule = {
	attach: function (context, settings) {
            if($.fn.iCheck) {
	      $('input').iCheck({
	        checkboxClass: 'icheckbox_flat-red',
	        radioClass: 'iradio_flat-red'
	      });
	    }
	    $('#kalvi-menu-inject').mouseover(
              function() {
                $('#kalvi-menu').animate({'left': 0});
            });
            $('#kalvi-menu').mouseleave(
              function() {
                $(this).animate({'left': -90});
            });

            if(($('div.view-admin-views-node').length !== 0) || ($('div.view-admin-views-user').length !== 0)) {
	      $('td.views-field-edit-node a:nth-child(1)').each(function() {
	        $(this).html('<i class="fa fa-pencil"></i>');
	      });
	      $('td.views-field-edit-node a:nth-child(2)').each(function() {
	        $(this).html('<i class="fa fa-times-circle"></i>');
	      });
            }
            else {
              $('td.views-field-edit-node a').each(function() {
	        $(this).html('<i class="fa fa-pencil"></i>');
	      });
	      $('td.views-field-delete-node a').each(function() {
	        $(this).html('<i class="fa fa-times-circle"></i>');
	      });
	    }
            $('tr.ok span.icon').each(function() {
              $(this).html('<i class="fa fa-check"></i>');
            });
            $('tr.warning span.icon').each(function() {
              $(this).html('<i class="fa fa-warning"></i>');
            });
            $('tr.error span.icon').each(function() {
              $(this).html('<i class="fa fa-bug"></i>');
            });
            $('tr.unknown span.icon').each(function() {
              $(this).html('<i class="fa fa-question"></i>');
            });

            $('table.system-status-report tr.ok td.status-icon').each(function() {
              $(this).html('<i class="fa fa-check"></i>');
            });
            $('table.system-status-report tr.warning td.status-icon').each(function() {
              $(this).html('<i class="fa fa-warning"></i>');
            });
            $('table.system-status-report tr.error td.status-icon').each(function() {
              $(this).html('<i class="fa fa-bug"></i>');
            });
            $('table.system-status-report tr.info td.status-icon').each(function() {
              $(this).html('<i class="fa fa-info"></i>');
            });

            $('a.tabledrag-handle .handle').each(function() {
              $(this).html('<i class="fa fa-arrows"></i>');
            });
	}
    };
})(jQuery);
