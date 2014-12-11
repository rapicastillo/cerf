$(function() {
	$(".donations-checkall").bind("click", function() {
		var $c = $(this);
		
		$c.parents("fieldset").find("input[type=checkbox]").attr("checked", "checked");
		$c.parents("fieldset").find(".donations-clearall").removeAttr("checked");
	});
	
	$(".donations-clearall").bind("click", function() {
		var $c = $(this);
		
		$c.parents("fieldset").find("input[type=checkbox]").removeAttr("checked");
		$c.parents("fieldset").find(".donations-checkall").removeAttr("checked");
	});
	
	$(".form-checkbox").bind("click", function() {
		var $c = $(this);

		var all_checked = true;
		var all_clear = true;
		
		$c.parents("fieldset").find(".donations-clearall").removeAttr("checked");
		$c.parents("fieldset").find(".donations-checkall").removeAttr("checked");
		
		$c.parents("fieldset").find(".form-checkbox").each(function() {
			if ($(this).is(":checked")) {
				all_clear = false;
			} else {
				all_checked = false;
			}
		});
		
		if (all_clear) {
			$c.parents("fieldset").find(".donations-clearall").attr("checked", "checked");
		}
		
		if (all_checked) {
			$c.parents("fieldset").find(".donations-checkall").attr("checked", "checked");
		}
	});
});
