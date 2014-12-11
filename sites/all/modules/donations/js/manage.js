$(function() {
	var $active_td = null;
	
	/* Initializations.. */
	/* Set the scroller */
	$("#don-fund-scroller").width($("#don-fund-list table").width());
	
	/* This is to give the feeling of freezing.. */
	$(document).bind("donations.bindEvents", function (event, options) {
		
		$("#don-fund-scroller-cont").unbind('scroll');
		$("#don-fund-list table td span").unbind('click');
		$(".don-save-submit").unbind('click');
		$(".edit-amount-value input[name=amount]").unbind("keypress");
		$(".don-cancel-submit").unbind('click');
		
		$("#don-fund-scroller-cont").bind('scroll', function() {
			$("#don-fund-list table, #don-fund-hdr table").css("marginLeft", -1 * $(this).scrollLeft());
		});
		
		/* This will change a cell into an editable entity */
		$("#don-fund-list table td span").bind('click', function(event) {
			$target = $(this).parents("td");
			
			event.stopPropagation();
			
			$target.children("span").hide();
			$target.children("form").show();
			
			$target.attr("chosen", "1");
			
			$target.find("input[type=text]").select();
			
			$(document).trigger("donations.performSave", { target:$active_td });
			
			$active_td = $target;
		});
		
		$(".don-save-submit").bind('click', function(event) {
			 $target = $(this).parents("td");
			 $(document).trigger("donations.performSave", { target: $target });
		});
		
		$(".edit-amount-value input[name=amount]").bind("keypress", function(event) {
			 var code = event.keyCode || event.which;
			 
			 if (code == 13) {
				 event.preventDefault();
				 
				 $target = $(this).parents("td");
				 $(document).trigger("donations.performSave", { target: $target });
			 }
		});
		
		$(".don-cancel-submit").bind('click', function(event) {
			$target = $(this).parents("td");
			
			$target.find("input[name=amount]").val($target.find("input[name=original_amount]").val());
			
			$target.children("span").show();
			$target.children("form").hide();
			
			$active_td = null;
		});
	});

	/*
	$('html').click(function() {
		//if TD is clicked, it will close the previously active TD. 
		if ($active_td) {
			//This area should submit something to donations.module	
			$form = $active_td.children("form");
			if ($form.find("[name=original_amount]").val() != $form.find("[name=amount]").val()) {
				$active_td.children("form").saveDonation();
				$active_td.children("span").text($form.find("[name=amount]").val());
			}
			
			$active_td.children("span").show();
			$active_td.children("form").hide();
			
			$active_td = null;
		}
	});*/
	
	$.fn.saveDonation = function() {
		$this = $(this);
		
		donor_id = $this.find("[name=donor_id]").val();
		/* console.log(donor_id); */
		
		name = $("#donor-i-" + donor_id).find("div").text();
		$.ajax({
			url : "/cerf/admin/settings/donations_manager/save", 
			data : $this.serialize(),
			//type: "POST", 
			dataType : "json", 
			success: function(data) {
				$(document).trigger("donations.flash", {message: "Item has been saved for " + name + "."});
				
				$this.find("[name=nid]").val(data.nid);
				
				var target_id = ".don-donor-" + data.donor_id;
				/* console.log($this, $this.parents(), target_id, data.donor_id, $this.parents(target_id)); */
				
				var $target_tr = $this.parents(".don-donor-" + data.donor_id);
				var $target_td = $this.parents("td");
				
				
				var red_value = parseInt( data.red_value );
				if (red_value) {
					$target_td.addClass("flagged_red");
				} else {
					$target_td.removeClass("flagged_red");
				}
				
				/* Normalize all the nid of the other items */
				$target_tr.children(".don-year-" + data.year).each(function(i, item) {
					/* console.log("XTER"); */
					$(this).find("form [name=nid]").val(data.nid);
				});
			}
		});
	};
	
	$(document).bind("donations.performSave", function(event, options) {
		var $target  = options.target; //if TD is clicked, it will close the previously active TD. 
			if ($active_td) {
				//This area should submit something to donations.module
				$form = $active_td.children("form");
				
				var value = $form.find("[name=amount]").val();
				if ($.trim(value) == "") { value = "-"; }
				//if ($form.find("[name=original_amount]").val() != $form.find("[name=amount]").val()) {
					$active_td.children("form").saveDonation();
					$active_td.children("span").text(value);
				//}
				
				$active_td.children("span").show();
				$active_td.children("form").hide();
				$active_td = null;
			}
	}) ;
	
	
	var timeoutVar = null;
	$(document).bind("donations.flash", function(event, options) {
		
		/* console.log(options); */
		if(timeoutVar) { clearTimeout(timeoutVar); }
		$("#don-flash-notifier").stop(true, true).hide();
		$("#don-flash-notifier").text(options.message);
		
		$("#don-flash-notifier").fadeIn("fast");
		timeoutVar = setTimeout(function() { $("#don-flash-notifier").fadeOut("fast"); timeoutVar = null; }, 3000);
	});
	
	
	
	/**
	 * 
	 *  
	 *  
	 *  FILTERS AREA 
	 *  
	 *  
	 *  
	 **/
	
	//Filtering the Checkboxed filters
	$("form#don-filters-area input[type=checkbox]").bind("click", function(ev) {
		var $this = $(this);
		var type = $this.val();
		if ( $this.is(':checked') ) { $(type).removeClass($this.attr("name") + "_hidden"); } 
		
		else { $(type).addClass($this.attr("name") + "_hidden"); }
		
		$(document).trigger("donations.scrollerWidthUpdate");
	});
	
	$("form#don-filters-area input[name=country_filter]").bind("keyup", function(ev) {
		var $this = $(this);
		var text = $this.val();
		
		/* console.log("Filtering ... ", text); */
		text = $.trim(text.toUpperCase()) ;
		if (text == "") {
			$(".don-donor-list-item, .don-donor-list-label").show();
		} else {
			
			$(".don-donor-list-item").hide();
			$(".don-donor-list-label").hide();
			
			$(".don-donor-list-item[row_country*='" + text + "']").show();
			$(".don-donor-list-label[title*='" + text + "']").show();
		}
	});
	
	
	$(document).bind("donations.scrollerWidthUpdate", function() {
		var marginLeft = $("#don-fund-table-proper").css("marginLeft");
		
		$("#don-fund-table-proper").css("marginLeft", 0);
		$("#don-fund-scroller").width($("#don-fund-list").width());
		$("#don-fund-table-proper").css("marginLeft", marginLeft);
	});
	
	/***
	 * 
	 * 
	 * Loading the country items...
	 * 
	 * 
	 */
	$(document).bind("donations.loadCountryItems", function(event, options) {
		
		var number_of_items = 25;
		var $target = options.targetTable;
		$.ajax({
			url : "/cerf/admin/settings/donations_manager/list", 
			data : {
				l: number_of_items,
				m: options.memberCountry,
				p: options.page,
				ajax: 1
			},
			//type: "POST", 
			dataType : "html", 
			success: function(data) {
				$target.append($(data).find("tr"));
				
				console.log($(data).find("tr").length );
				if ($(data).find("tr").length >= number_of_items ) {
					$(document).trigger("donations.loadCountryItems", {page : (options.page+1),  memberCountry : options.memberCountry, targetTable: $target});
				} else {
					$(document).trigger("donations.bindEvents");
					$(document).trigger("donations.scrollerWidthUpdate");
				}
				/*
				$(document).trigger("donations.flash", {message: "Item has been saved for " + name + "."});
				
				$this.find("[name=nid]").val(data.nid);
				
				var target_id = ".don-donor-" + data.donor_id;
				
				var $target_td = $this.parents(".don-donor-" + data.donor_id);
				
				$target_td.children(".don-year-" + data.year).each(function(i, item) {
					$(this).find("form [name=nid]").val(data.nid);
				});
				*/
			}
		});
	});
	/*
	$(document).trigger("donations.loadCountryItems", {page : 0, memberCountry : 2, targetTable: $("#don-fund-table-regional-proper") });
	$(document).trigger("donations.loadCountryItems", {page : 0, memberCountry : 1, targetTable: $("#don-fund-table-proper") });
	$(document).trigger("donations.loadCountryItems", {page : 0, memberCountry : 0, targetTable: $("#don-fund-table-private-proper") });
	*/
	$(document).trigger("donations.bindEvents");
	$(document).trigger("donations.scrollerWidthUpdate");
})
