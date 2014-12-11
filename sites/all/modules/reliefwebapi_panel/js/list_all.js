jQuery(function($) {
	//process each square
		var $this = $(this);
 		var $target_form = $this.find("form.rwapi-filter-form");
 		
 		var _primaryCountry = $target_form.find("input[name=primary_country]").val(), 
 			_country = $target_form.find("input[name=country]").val(), 
 			_theme = $target_form.find("input[name=theme]").val(),
 			_vulnerableGroups = $target_form.find("input[name=vulnerable_groups]").val();
 			_organisation = $target_form.find("input[name=organisation]").val(), 
 			_disasterType = $target_form.find("input[name=disaster_type]").val(), 
 			_contentFormat = $target_form.find("input[name=content_format]").val(), 
 			_language = $target_form.find("input[name=language]").val(), 
 			_publishedDate = $target_form.find("input[name=published_date]").val(),
 			
 			_offset = parseInt($target_form.find("input[name=offset]").val()),
 			_limit = parseInt($target_form.find("input[name=limit]").val()),
 			
 			_summShowThumb = $target_form.find("input[name=summ_show_thumb]").val(),
 			_summShowCount =$target_form.find("input[name=summ_show_count]").val()
 			
 			;
 		
 			
 			var blurb_show = $target_form.find("input[name=summ_blurb_show]").val();
 			
 			
 		var __include = ['title', 'url'];
 		var __conditions = [];
 		console.log(_primaryCountry);
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
 		/*
 		
 		if (_publishedDate && _publishedDate !== '') { 
 			__include.push('report_date'); 
 			__conditions.push({ field : "report_date", value: _publishedDate.split("|") });
 		}
 	*/	
 		//console.log(_values);
 		$.ajax({
 			//url : '/sandbox/filter-2.json',
 			url : 'http://api.rwlabs.org/v0/report/list',
 			dataType: "json",
 			data : {
 				offset: _offset * _limit,
 				limit: _limit, 
 				fields: {
 					include: __include 
 				}, 
	 			filter: {
	 				conditions: __conditions
	 			}, 
	 			async: true
 			}, 
 			success: function(d) {
 				var f = [];
 				console.log(d);
 				$(d.data.list).each(function(i, item) {
 					console.log(_summShowThumb);
 					var _targetId = item.id;
 					var $new_item = $("<div />").addClass("rwapi-filter-item")
						.append($("<h4 />")
									.append($("<a />").attr({"href" : item.fields.url, "target" : "_blank" }).text(item.fields.title)))
							.append(parseInt(_summShowThumb) ? $("<img />").attr("src", "http://placehold.it/75x75").addClass("rwapi-item-sq-img") : "")
							.append($("<span />").addClass("rwapi-filter-art-spi"));
 					
 					$("#rwapi-filter-items").append($new_item);
 					
 					if (blurb_show || true) {
 						$new_item.find(".rwapi-filter-art-spi").html(get_body(_targetId));
 						$new_item.find(".rwapi-filter-art-spi").dotdotdot();
 					}
 					
 					if ($("#reliefwebapi-block-items-filter-page").hasClass("reliefweb-layout-tile")) {
 						$new_item.find("h4 a").dotdotdot();
 					}
 					
 				});
 				
 				var _startingPage = _offset > 4 ? _offset - 4 : 0;
				var _total = d.data.total;
				console.log(_total, _offset, _startingPage);
				
				if (_offset > 0) {
					$("#rwapi-filter-pagination").append(
						$("<div />").addClass("pagination-item").append(
							$("<a />").attr("href", "?offset=0").text("First")
						)
					);
 		
			 		$("#rwapi-filter-pagination").append(
						$("<div />").addClass("pagination-item").append(
							$("<a />").attr("href", "?offset=" + (_offset-1)).text("Previous")
						)
					);
				}
				
				while (_startingPage * _limit < _total && _startingPage - _offset < 5) {
					$("#rwapi-filter-pagination").append(
						$("<div />").addClass("pagination-item " + (_startingPage == _offset ? "current-item" : "")).append(
							$("<a />").attr("href", "?offset=" + _startingPage++).text(_startingPage)
						)
					);
				}
				
				if (_offset * _limit < _total) {
					$("#rwapi-filter-pagination").append(
							$("<div />").addClass("pagination-item").append(
								$("<a />").attr("href", "?offset=" + (_offset+1)).text("Next")
							)
						);
						
					$("#rwapi-filter-pagination").append(
							$("<div />").addClass("pagination-item").append(
								$("<a />").attr("href", "?offset=" + Math.floor(_total/_limit)).text("Last")
							)
						);
				} 
 			}, 
 			
 			error: function(a, b, c) {
 				console.log(a, b , c);
 			}
 		});
 		
 		function get_body(target_id) {
 			
 			var _returnBody;
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
 					_returnBody = (resp.data.item.fields.body).replace(/<.*?>/g, '');
 				}, 
 				error: function(a,b,c,d) {
 					console.error(a,b,c,d);
 				},
 				async: false
 			});
 			
 			return _returnBody;
 		}
});
