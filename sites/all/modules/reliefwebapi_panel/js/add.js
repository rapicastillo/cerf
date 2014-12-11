jQuery(function($) {
	var RWAPI_MANAGE= {
		data_filter : {
				limit : 1000,
				filter: {
					conditions: []
				}
		},
		_category  : ""
	}

	/**
	 *  Initializations...
	 */
	$(".rwapi-error").hide();
	$("input[name=rwapi-tmp-content-format]").each(function(i, item) {
		var _value = $(this).val();
		$("input[name='rwapi-content-type[]'][value='" + _value + "']").attr("checked", "checked");
	});
	
	$("input[name=rwapi-tmp-langauge]").each(function(i, item) {
		var _value = $(this).val();
		$("input[name='rwapi-language[]'][value='" + _value + "']").attr("checked", "checked");
	});
	
	$("input[name=rwapi-tmp-published-date]").each(function(i, item) {
		var _value = $(this).val();
		$("input[name='rwapi-published-date[]'][value='" + _value + "']").attr("checked", "checked");
	});
	
	/* Hide items */
	$("input[name='rwapi-chosen-vulnerable-groups[]']").each(function(i, item) {
		$("select[name=rwapi-manage-vulnerable-groups] option[value='" + $(this).val() + "']").hide();
	});
	
	$("input[name='rwapi-chosen-disaster-type[]']").each(function(i, item) {
		$("select[name=rwapi-manage-disaster-type] option[value='" + $(this).val() + "']").hide();
	});
	
	$("input[name='rwapi-chosen-theme[]']").each(function(i, item) {
		$("select[name=rwapi-manage-theme] option[value='" + $(this).val() + "']").hide();
	});
	
	$("input[name='rwapi-chosen-organisation[]']").each(function(i, item) {
		$("select[name=rwapi-manage-organisation] option[value='" + $(this).val() + "']").hide();
	});
	
	$("input[name='rwapi-chosen-country[]']").each(function(i, item) {
		$("select[name=rwapi-manage-country] option[value='" + $(this).val() + "']").hide();
	});
	
	/**
	 * For choosing the main category
	 */
	$("#rwapi-manage-wrapper #reliefwebapi-insert-item input[name=rwapi-category]").bind("click", function() {
		$("[rwapi-group]").hide();
		$("[rwapi-group*=" + $(this).val() + "]").show();
		
		RWAPI_MANAGE._category = $(this).val();
		
	});
	
	$("select[name=rwapi-manage-primary-country]").live("change", function() {
		var $this = $(this);
		var _optval = $this.val();
		if (_optval != "0") {
			var $item = $("<div />").addClass("rwapi-select-chosen-item")
					.append($("<input />").attr({ type: 'hidden', name: 'rwapi-chosen-primary-country', value: $this.val() }))
					.append($("<span />").text($this.val()))
					.append($("<span />").addClass("rwapi-select-chosen-remove").text("x"));
	
			$this.hide();
			$this.parent().append($item);
		}
	});
	
	$("select[name=rwapi-manage-country]").live("change", function() {
		var $this = $(this);
		var _optval = $this.val();
		if (_optval != "0") {
			var $item = $("<div />").addClass("rwapi-select-chosen-item")
					.append($("<input />").attr({ type: 'hidden', name: 'rwapi-chosen-country[]', value: _optval }))
					.append($("<span />").text(_optval ))
					.append($("<span />").addClass("rwapi-select-chosen-remove").text("x"));
	
			
			$("select[name=rwapi-manage-country] option[value=\"" + _optval + "\"]").hide();
			//$this.hide();
			$this.parent().prepend($item);
		}
	});
	
	$("select[name=rwapi-manage-organisation]").live("change", function() {
		var $this = $(this);
		var _optval = $this.val();
		
		if (_optval != "0") {
			var $item = $("<div />").addClass("rwapi-select-chosen-item")
					.append($("<input />").attr({ type: 'hidden', name: 'rwapi-chosen-organisation[]', value: _optval }))
					.append($("<span />").text(_optval ))
					.append($("<span />").addClass("rwapi-select-chosen-remove").text("x"));
	
			$(target).hide();
			//$this.hide();
			$this.parent().prepend($item);
		}
	});
	
	$("select[name=rwapi-manage-theme]").live("change", function() {
		var $this = $(this);
		var _optval = $this.val();
		
		if (_optval != "0") {
			var $item = $("<div />").addClass("rwapi-select-chosen-item")
					.append($("<input />").attr({ type: 'hidden', name: 'rwapi-chosen-theme[]', value: _optval }))
					.append($("<span />").text(_optval ))
					.append($("<span />").addClass("rwapi-select-chosen-remove").text("x"));
	
			$("select[name=rwapi-manage-theme] option[value=\"" + _optval + "\"]").hide();
			//$this.hide();
			$this.parent().prepend($item);
		}
	});
	
	$("select[name=rwapi-manage-disaster-type]").live("change", function() {
		var $this = $(this);
		var _optval = $this.val();
		if (_optval != "0") {
			var $item = $("<div />").addClass("rwapi-select-chosen-item")
					.append($("<input />").attr({ type: 'hidden', name: 'rwapi-chosen-disaster-type[]', value: _optval }))
					.append($("<span />").text(_optval ))
					.append($("<span />").addClass("rwapi-select-chosen-remove").text("x"));
	
			$("select[name=rwapi-manage-disaster-type] option[value=\"" + _optval + "\"]").hide();
			//$this.hide();
			$this.parent().prepend($item);
		}
	});
	
	$("select[name=rwapi-manage-vulnerable-groups]").live("change", function() {
		var $this = $(this);
		var _optval = $this.val();
		if (_optval != "0") {
			var $item = $("<div />").addClass("rwapi-select-chosen-item")
					.append($("<input />").attr({ type: 'hidden', name: 'rwapi-chosen-vulnerable-groups[]', value: _optval }))
					.append($("<span />").text(_optval ))
					.append($("<span />").addClass("rwapi-select-chosen-remove").text("x"));
	
			$("select[name=rwapi-manage-vulnerable-groups] option[value=\"" + _optval + "\"]").hide();
			//$this.hide();
			$this.parent().prepend($item);
		}
	});
	
	$(".rwapi-select-chosen-remove").live("click", function() {
		var $this = $(this);
		
		var $target = $this.closest(".rwapi-select-chosen-item");
		var $select = $target.siblings("select");
		var _value = $target.find("input[type=hidden]").val();
		
		$target.remove();
		console.log($select, $select.attr("name"));
		if ($select.attr("name") == "rwapi-manage-primary-country") {
			$select.show();
			$select.val(0);
		} else {
			$select.find("option[value=\"" + _value + "\"]").show();
		}
	});
	/**
	 * 
	 * VALIDATION
	 * 
	 */
	$("#reliefwebapi-insert-item").live("submit", function() {
		var _error = false;
		var _errorMessage = "";
		$("input[rwapi-required='true']").each(function(i, item) {
			$(this).css("backgroundColor", "white");
			
			if ( $.trim($(this).val()) == "" ) {
				_error = true;
				$(this).css("backgroundColor", "#FFCCCC");
				_errorMessage = _errorMessage.concat((_errorMessage!="" ?"<br/>":"") + "Please Fillout Required Field: " + $(this).attr("ref"));
			}
			
			if ($(this).attr("type") == "number" && parseInt($(this).val()) < 1 ) {
				_error = true;
				$(this).css("backgroundColor", "#FFCCCC");
				_errorMessage = _errorMessage.concat((_errorMessage!="" ?"<br/>":"") + "Please use a number greater than 0: " + $(this).attr("ref"));
			}
			
			if ($(this).attr("type") == "number" && isNaN($(this).val())) {
				_error = true;
				$(this).css("backgroundColor", "#FFCCCC");
				_errorMessage = _errorMessage.concat((_errorMessage!="" ?"<br/>":"") + "Please use a number greater than 0: " + $(this).attr("ref"));
			}
		});
		
		if (_error) {
			$(".rwapi-error").html(_errorMessage).show();
			$("html, body").animate({scrollTop:0}, "slow");
		}
		return !_error;
	});
	
	/*
	$.ajax({
		url : "/sandbox/sample_country.json", //http://api.rwlabs.org/v0/country/list?limit=1000&sort[]=name:asc 
		dataType: "json", 
		success: function(info) {
			$(info.data.list).each(function(i, item){
				console.log(item);
				$("form#rwapi-manage-item select[name=rwapi-manage-primary-country]")
					.append(
						$("<option />")
							.text(item.fields.name)
							.val(item.id)
					);
			});
			var my_options = $("form#rwapi-manage-item select[name=rwapi-manage-primary-country] option");

			my_options.sort(function(a,b) {
			    if (a.text > b.text) return 1;
			    else if (a.text < b.text) return -1;
			    else return 0
			});
			
			//Append sorted option list
			$("form#rwapi-manage-item select[name=rwapi-manage-primary-country]").empty().append(my_options);
			//
			//?limit=1000&filter[conditions][0][field]=country&filter[conditions][0][value][0]=Algeria&filter[conditions][0][value][]=Philippines
			data_filter.filter.conditions
		}
	}); */
	

	function refresh_filters() {
		switch(RWAPI_MANAGE._category) {
			case "rwapi_updates" :
				break;
			case "rwapi_disaster" :
				break;
			case "rwapi_countries" :
				break;
		}
	}
});