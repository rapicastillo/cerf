$(document).ready(function() {
	var ranking = 1;
	$(".donations-table").bind("tablesorter-initialized",function(e, table) {
	      // do something after tablesorter has initialized
  });
	$(".donations-table").each(function(i, item) {
		var colCount = $(item).find("thead").children("tr").find("th").length - 1;
		
		$(this).tablesorter({
			theme: "blue",
			sortList: [[colCount, 1]],
			headers : { 0 : { sorter: false } },
			textExtraction: function(node) { 
	            // extract data from markup and return it  
	            return $(node).attr("st-value"); 
	        }, 
	        initialized: function(e, table) {
	        }
		});
		
		$(this).find(".table-sort-ranking").each(function(i, item) {
    		$(this).text(ranking++);
    	});
	});
	
	
	var max_width = {};
	$("td.data[col-name]").each(function() {
		var name = $(this).attr("col-name");
		var width = $(this).children("div").width();

		if (max_width[name] == null || max_width[name] < width) {
			max_width[name] = width;
		}
	});
	
	
	$("td.data[col-name]").each(function() {
		var name = $(this).attr("col-name");
		
		$(this).children("div").width(max_width[name]);
	});

	/* For the width of the country data name...*/

	var width = $("td.country-data div.country-data-name").width();
	
	$("td.country-data div.country-data-name").width(width - 50);

	$("th.data.header").each(function(i, item) {
		var true_width = $(this).find("div").width();
		$(this).css("backgroundPositionX", true_width-10 + "px");
	});

//	$("td.total.data[st-value=0]").parents("tr").hide();
});
