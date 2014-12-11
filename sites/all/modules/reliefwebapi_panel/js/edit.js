$jq(function($) {
	$(".rw-parent-div input[type=checkbox], .rw-parent-div input[type=radio]").die("click");
	$(".rw-parent-div input[type=text]").die("keyup");
	
	/*Listener*/
	$(".rw-parent-div input[type=checkbox], .rw-parent-div input[type=radio]").live("click", function() {
		var $parent = $(this).closest(".rw-parent-div");
		var $this = $(this);
		
		//For primary countries only
		/* if ($parent.hasClass("primary-country-checkbox")) {
			$parent.find(".rw-chosen-item").first(".rw-chosen-item-item").find(".del-item").trigger("click");
			
		}*/
		if ($this.is(':checked')) {
			$parent.find(".rw-chosen-item").append($("<div />").attr("rwvalue", $this.val()).addClass("rw-chosen-item-item")
														   .append($("<span />").text($this.val()).addClass("rw-chosen-name"))
														   .append($("<div />").text("x").addClass("del-item").bind("click", func___deselectItem)));
		} else {
			var $target = $parent.find('.rw-chosen-item .rw-chosen-item-item[rwvalue="'+ $this.val() +'"] .del-item').trigger("click");
			//$target.find("m").trigger("click");
		}
	});
	
	$(".rw-parent-div input[type=text]").live("keyup", function(event) {
		var $parent = $(this).closest(".rw-parent-div");
		var $this = $(this);
		var _value = $this.val();
		
		$parent.find(".form-item input").closest(".form-item").hide();
		
		if ($.trim(_value) != "") {
			$parent.find(".form-item input:iAttrStart(value," + _value + ")").closest(".form-item").show();
		} else {
			$parent.find(".form-item").show();
		}
		
	});
	
	//Callbacks
	var func___deselectItem = function(event) {
		var $superParent = $(this).closest(".rw-parent-div");
		var $parent = $(this).closest(".rw-chosen-item-item");
		var _value = $parent.find(".rw-chosen-name").text();
		
		$superParent.find('input[value="' + _value + '"]').removeAttr("checked");
		$parent.remove();
	}
	
	$.expr[':'].iAttrStart = function(obj, params, meta, stack) {
	    var opts = meta[3].match(/(.*)\s*,\s*(.*)/);
	    return (opts[1] in obj) && (obj[opts[1]].toLowerCase().indexOf(opts[2].toLowerCase()) === 0);
	};
	
	
	/*Initialize - with workaround*/
   $(document).bind("reliefweb-load-chosen-items", function() {
	$(".rw-chosen-item-item").remove();
	$(".rw-parent-div input:checked").each(function(i,item) {
		var $parent = $(this).closest(".rw-parent-div");
		var $this = $(this);
		
		$parent.find(".rw-chosen-item").append($("<div />").attr("rwvalue", $this.val()).addClass("rw-chosen-item-item")
				   .append($("<span />").text($this.val()).addClass("rw-chosen-name"))
				   .append($("<div />").text("x").addClass("del-item").bind("click", func___deselectItem)));
	});
   });
   
	$("input[name=redirect_to_reliefweb]").die('click');
	$("input[name=redirect_to_reliefweb]").live('click', function(event) {
		if ($(this).val() == "1") {
			$("#edit-redirect-link-wrapper").hide();
		}else {
			$("#edit-redirect-link-wrapper").show();
		}
	});
	
	$("input[name=redirect_to_reliefweb]").trigger("click");
	
});
