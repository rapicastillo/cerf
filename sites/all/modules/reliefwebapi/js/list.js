jQuery(function($) {
	
	//process each square
	$("#reliefwebapi-block-items-filter-list div.rwapi-item-square").each(function(i, item) {
		var $this = $(this);
 		var $target_form = $this.find("form.rwapi-filter-form");
 		
 		
 		/**
 		 * Initialize
 		 * */
 		
 		var _primaryCountry = $target_form.find("input[name=primary_country]").val(), 
 			_country = $target_form.find("input[name=country]").val(), 
 			_theme = $target_form.find("input[name=theme]").val(),
 			_vulnerableGroups = $target_form.find("input[name=vulnerable_groups]").val();
 			_organisation = $target_form.find("input[name=organisation]").val(), 
 			_disasterType = $target_form.find("input[name=disaster_type]").val(), 
 			_contentFormat = $target_form.find("input[name=content_format]").val(), 
 			_language = $target_form.find("input[name=language]").val(), 
 			_publishedDate = $target_form.find("input[name=published_date]").val(),
 			
 			//Design options
 			_summShowThumb = $target_form.find("input[name=summ_show_thumb]").val(),
 			_summShowCount =$target_form.find("input[name=summ_show_count]").val()
 			;
 		
 		var blurb_show = $target_form.find("input[name=summ_show_blurb]").val();
 		var __include = ['title', 'url'];
 		var __conditions = [];
 		
 		if (_primaryCountry && _primaryCountry !== '') { 
 			console.log(_primaryCountry.split("|"));
 			__include.push('primary_country'); 
 			__conditions.push({ 
 				field : "primary_country", 
 				value : _primaryCountry.split("|")
 			}); 
 		}
 		if (_country && _country !== '') { 
 			__include.push('country'); 
 			__conditions.push({ field : "country", value: _country.split("|") });
 		}
 		
 		if (_theme && _theme !== '') { 
 			__include.push('theme'); 
 			__conditions.push({ field : "theme", value: _theme.split("|") });
 		}
 		
 		if (_organisation && _organisation !== '') { 
 			__include.push('source'); 
 			__conditions.push({ field : "source", value: _organisation.split("|") }); 
 		}
 		
 		if (_vulnerableGroups && _vulnerableGroups !== '') {
 			__include.push('vulnerable_groups'); 
 			__conditions.push({ field : "vulnerable_groups", value: _vulnerableGroups.split("|") });
 		}
 		if (_disasterType && _disasterType !== '') { 
 			__include.push('disaster_type'); 
 			__conditions.push({ field : "disaster_type", value: _disasterType.split("|") }); 
 		}
 		
 		if (_contentFormat && _contentFormat !== '') { 
 			__include.push('format'); 
 			__conditions.push({ field : "format", value: _contentFormat.split("|") });
 		}
 		
 		if (_language && _language !== '') { 
 			__include.push('language'); 
 			__conditions.push({ field : "language", value: _language.split("|") }); 
 		}
 		
 		__include.push('primary_country');
 		/*
 		
 		if (_publishedDate && _publishedDate !== '') { 
 			__include.push('report_date'); 
 			__conditions.push({ field : "report_date", value: _publishedDate.split("|") });
 		}
 	*/	
 		//console.log(_values);
 		var _targetId = null, _targetUrl = null;
 		$.ajax({
 			url : '/sandbox/filter-1.json',
 			//url : 'http://api.rwlabs.org/v0/report/list',
 			dataType: "json",
 			async: false,
 			data : {
 				offset: 0,
 				limit: 1, 
 				fields: {
 					include: __include 
 				}, 
	 			filter: {
	 				conditions: __conditions
	 			}
 			}, 
 			success: function(d) {
 				var f = [];
 				_targetId= d.data.list[0].id;
 				_targetUrl= d.data.list[0].fields.url;
 				
 				console.log(d.data.list[0].fields, _targetId);
 				
				var title_text = d.data.list[0].fields.title;
				alert("XXXX");
				if (title_text.match(/(.*?):(.*)/g))
				{
					title_text.replace(/:/g, '</strong>:');
					title_text = "<strong>" + title_text;
				}
 				$this.find(".rwapi-filter-art-tit").html("<a href='" + _targetUrl + "'>" + d.data.list[0].fields.title + "</a>");
 				
 				if (parseInt(_summShowThumb)) {
 					console.log("THUMB", $this.find(".rwapi-item-sq-imag"));
 		 			$this.find(".rwapi-item-sq-imag").show();
 		 			//$this.find(".rwapi-item-sq-imag").attr("src","");
 		 		}
 		 		
 				return false;
 			}, 
 			
 			error: function(a, b, c) {
 				console.log(a, b , c);
 			}
 		});
 		
 		if (parseInt(blurb_show)) {
 			 $.ajax({
 				//url : "http://api.rwlabs.org/v0/report/" + _targetId,
 				url : "/sandbox/report-item.json",
 				data : {
 					fields : {
 						include : ["body"]
 					}
 				}, 
 				crossDomain : true,
 				dataType: "json",
 				success: function(resp) {
 					console.log(resp);
 					var _actualBlurb = (resp.data.item.fields.body).replace(/<.*?>/g, '');
 					console.log(_actualBlurb);
 					$this.find(".rwapi-filter-art-spi").text(_actualBlurb);
 					$this.find(".rwapi-filter-art-spi").dotdotdot();
 				}, 
 				error: function(a,b,c,d) {
 					console.error(a,b,c,d);
 				}
 			});
		}
 			
	});
	
	function getBlurb(targetURL, targetContainer) {
		console.log("Showing blurb", targetURL);
		targetContainer.load(targetURL);
		
		/* $.ajax({
			url : _targetUrl,
			crossDomain : true,
			dataType: "html",
			success: function(resp) {
				console.log(resp);
				$this.find(".rwapi-filter-art-spi").html($(resp).find(".field.body"));
			}, 
			error: function(a,b,c) {
				console.error(a,b,c);
			}
		}); */
	}
});
