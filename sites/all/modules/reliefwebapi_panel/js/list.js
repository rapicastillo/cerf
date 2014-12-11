$jq(function($) {
	
	/* HELPER FUNCTIONS*/

	var rwHelpers = {
			/**
			 * build_pagination : helps with the creation of pagination elements
			 * 
			 */
			build_pagination : function (offset, limit, total) {
				var $temp_container = $("<div />");
				var startingPage = offset > 4 ? offset - 4 : 0;

				if (offset > 0) {
					$temp_container.append(	
						$("<div />").addClass("pagination-item ").append(
							$("<a />").addClass("reliefwebapi-submit-form").attr("href", "javascript: void(0);").text("First").attr("offset", 0)
						)
					);
 		
					$temp_container.append(
						$("<div />").addClass("pagination-item ").append(
							$("<a />").addClass("reliefwebapi-submit-form").attr("href", "javascript: void(0);").text("Previous").attr("offset", offset-1)
						)
					);
				}
				
				while (startingPage * limit < total && startingPage - offset < 5) {
					$temp_container.append(
						$("<div />").addClass("pagination-item  " + (startingPage == offset ? "current-item" : "")).append(
							$("<a />").addClass("reliefwebapi-submit-form").attr("href", "javascript: void(0);").attr("offset", startingPage++).text(startingPage)
						)
					);
				}
				
				if (offset * limit < total) {
					$temp_container.append(
							$("<div />").addClass("pagination-item ").append(
								$("<a />").addClass("reliefwebapi-submit-form").attr("href", "javascript: void(0);").text("Next").attr("offset", offset+1)
							)
						);
						
					$temp_container.append(
						$("<div />").addClass("pagination-item  ").append(
							$("<a />").addClass("reliefwebapi-submit-form").attr("href", "javascript: void(0);").text("Last").attr("offset", Math.floor(total/limit))
						)
					);
				}
				
				return $temp_container.html();
			}, 
			
			/**
			 * format_text: helps with the formatting of blurbs, mainly
			 */
			format_text : function(content) {
				 if (content && $.trim(content) !== "") {
					 content = content.replace(/<.*?>/g, '');
					 content = content.replace(/\n/g,'<br/>')
				 }

				var regExp_b = new RegExp("\\*\\*(.*?)\\*\\*", "g");
				var regExp_i = new RegExp("\\*(.*?)\\*", "g");

				content = content.replace(regExp_b, "<strong>$1</strong>");
				content = content.replace(regExp_i, "<i>$1</i>");
                 
				 return content;
			},
			/**
			 * cut_phrase : helps with the cutting of phrases in relation 
			 * 				to the character limit set by the user
			 */
			cut_phrase : function (phrase, characterLimit) {
				if ( phrase.length > characterLimit ) {
                                        phrase = phrase.substring(0, characterLimit); //cut string
					if (phrase.lastIndexOf("\n") > 0)
					{
						phrase = phrase.substring(0, phrase.lastIndexOf("\n"));
					}
					else
					{
						phrase = phrase.substring(0, phrase.lastIndexOf(" ")).concat("&hellip;"); //go to nearest word
					}
				}
				
				return phrase;
			},
			/**
			 * evaluate_filename : helps with the evaluation of the image filename
			 */
			evaluate_filename : function(fields) {
				if (fields && fields.image) {
					return fields.image['url-small'];
				} else if(fields && fields.file && fields.file.length) {
					return fields.file[0].preview['url-small'];
				} else {
					return "/images/ocha.jpg";
				}
			},
			/**
			 * reliefweb_filter_builder: Helps with the building of the filters..
			 */
			reliefweb_filter_builder : function (filter) {
				var reliefWebFilter = {
					include : ['title', 'url', 'date.created', 'image.url-small', 'file.preview.url-small', 'primary_country'],
					conditions : [],
					query : null
				};
				
				var __include = ['title', 'url', 'date.created', 'image.url-small', 'file.preview.url-small', 'primary_country'];
	 		var __conditions = [];
	 		var __query = null;
	
			if (filter.queryText && filter.queryText !== '') {
				reliefWebFilter.query = { "fields" : ["title", "body"], "value" : filter.queryText };
			}
	 		if (filter.primaryCountry && filter.primaryCountry !== '') { 
	 			reliefWebFilter.include.push('primary_country'); 
	 			reliefWebFilter.conditions.push({ 
	 				field : "primary_country", 
	 				value : filter.primaryCountry.split("|"),
					operator : "OR"
				});
	 		}
	 		
	 		if (filter.country && filter.country !== '') { 
	 			reliefWebFilter.include.push('country'); 
	 			reliefWebFilter.conditions.push({ field : "country", value: filter.country.split("|") });
	 		}
	 		
	 		if (filter.theme && filter.theme !== '') { 
	 			reliefWebFilter.include.push('theme'); 
	 			reliefWebFilter.conditions.push({ field : "theme", value: filter.theme.split("|"), operator: "OR" });
	 		}
	 		
	 		if (filter.organisation && filter.organisation !== '') { 
	 			reliefWebFilter.include.push('source'); 
	 			reliefWebFilter.conditions.push({ field : "source", value: filter.organisation.split("|"), operator: "OR" }); 
	 		}
	 		
	 		if (filter.contentFormat && filter.contentFormat !== '') { 
	 			reliefWebFilter.include.push('format'); 
	 			reliefWebFilter.conditions.push({ field : "format", value: filter.contentFormat.split("|"), operator: "OR" });
	 		}
	 		
	 		if (filter.language && filter.language !== '') { 
	 			reliefWebFilter.include.push('language'); 
	 			reliefWebFilter.conditions.push({ field : "language", value: filter.language.split("|"), operator: "AND" }); 
	 		}
	 		
	 		if (filter.flag.showBlurbInSummary) { 
	 			//This will now be included.
	 			reliefWebFilter.include.push('body');
	 		}
	 		return reliefWebFilter;
		}
		
	};

	$("div.rwapi-item-square").each(function(i, item) {
		$(this).closest(".panel-pane").addClass("reliefweb-api-pane");
	});
	
	//process each square
	$(document).unbind("reliefwebapi.grab-content-from-rw");
	$(document).bind("reliefwebapi.grab-content-from-rw", function(event, option) {
		
		var MONTHS = ["Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"];
		var $target_container = option.target;
		
		$target_container.find("div.rwapi-item-square").each(function(i, item) {
			var $this = $(this);
	 		var $tf = $this.find("form.rwapi-filter-form");
	 		
	 		/**
	 		 * Initialize FILTER ITEM
	 		 * */
	 		var filter = {
	 				id : $tf.find("input[name=rid]").val(),
	 				queryText : $tf.find("input[name=query_text]").val(),
	 				primaryCountry : $tf.find("input[name=primary_country]").val(),
	 				country : $tf.find("input[name=country]").val(),
	 				theme : $tf.find("input[name=theme]").val(),
	 				organisation : $tf.find("input[name=organisation]").val(),
	 				contentFormat : $tf.find("input[name=content_format]").val(),
	 				language : $tf.find("input[name=language]").val(),
	 				flag : {
	 					showThumbnailInSummary : parseInt($tf.find("input[name=summ_show_thumb]").val()),
	 					redirectToReliefWeb : parseInt($tf.find("input[name=redirect-to-reliefweb]").val()),
	 					showBlurbInSummary : parseInt($tf.find("input[name=summ_show_blurb]").val()),

	 					addBorderPerLine : parseInt($tf.find("input[name=add_border_per_line]").val()),
	 					addBulletsPerLine : parseInt($tf.find("input[name=add_bullets_per_line]").val()),
	 					addDatePerLine : parseInt($tf.find("input[name=add_date_per_line]").val())
	 				},
	 				look : {
	 					offset: parseInt(option.offset),
	 					titleTextHeader : $tf.find("input[name=title_text_header]").val(),
	 					titleCharacterLimit : $tf.find("input[name=title_word_limit]").val(),
	 					blurbCharacterLimit : $tf.find("input[name=blurb_word_limit]").val(),
	 					minHeigh :  $tf.find("input[name=min_height]").val(),
	 					itemPadding : $tf.find("input[name=item_padding]").val(),
	 					itemCountInSummary : $tf.find("input[name=summ_show_count]").val(),
	 					baseUrl : $tf.find("input[name=base_url]").val()
	 				}
	 		};
	 		
	 		//Just Add :
	 		if (filter.look.titleTextHeader.length > 0) {
	 			filter.look.titleTextHeader = filter.look.titleTextHeader + ":&nbsp;";
	 		}
	 		
	 		var rwFilterSet = rwHelpers.reliefweb_filter_builder(filter);
	 		var _targetId = null, _targetUrl = null;
			
	 		$.ajax({
	 			//url : 'http://api.rwlabs.org/v0/report/list',
	 			url: '/ie-grab.php',
	 			crossDomain: true, 
	 			dataType: "json",
	 			async: true,
	 			type: "GET",
	 			data : /* _tempQuery */
				{
	 				u: 'http://api.rwlabs.org/v0/report/list',
                                    query_set:rwFilterSet.query ? 
	 			{
	 				offset: filter.look.offset * filter.look.itemCountInSummary,
	 				limit: filter.look.itemCountInSummary, 
	 				fields: { include: rwFilterSet.include },
		 			filter: { conditions: rwFilterSet.conditions },
					query: rwFilterSet.query,
					sort: ["date.original:desc"]
	 			} : {
	 				offset: filter.look.offset * filter.look.itemCountInSummary,
	 				limit: filter.look.itemCountInSummary, 
	 				fields: { include: rwFilterSet.include },
		 			filter: { conditions: rwFilterSet.conditions },
					sort: ["date.created:desc"]
                }}, 
	 			beforeSend: function() {
	 				//$this.find(".rwapi-contents").html("");
	 				//$this.find(".rwapi-filter-pagination").html("");
	 			},
	 			success: function(d) {
					$this.find(".rwapi-contents").html("");
					$this.find(".rwapi-filter-pagination").html("");

	 				var $content_area = $this.find(".rwapi-contents");
	 				var $pagination_area = $this.find(".rwapi-filter-pagination");
	 				
	 				//_targetId= d.data.list[0].id;
	 				d['filter_id'] = filter.id;

					if (d.data.list.length == 0) {
						$content_area.append($("<em />").text("ReliefWeb API did not return any values. Please check the filters or contact your administrator."));
						$this.closest(".reliefwebapi-block-items-filter-list").find(".reliefwebapi-submit-form").hide();
					}
					
	 				$(d.data.list).each(function(i, item) {
	 					var $content_item = $("<li />").addClass("rwapi-content-item");
	// 					$content_area.append($("<h4 />").addClass("rwapi-filter-art-tit").html("<a href='" + item.fields.url + "' target='_blank'>" + item.fields.title + "</a>"));
	 	 				
	 					if (filter.flag.showThumbnailInSummary) {
	 	 					var _previewFilename = rwHelpers.evaluate_filename(item.fields);
	 	 					
	 	 					$content_item.append(
		 	 							$("<a />").attr("href", item.fields.url).attr("target", "_blank")
		 	 								.append($("<img />").addClass("rwapi-item-sq-imag").attr("src", _previewFilename))
	 	 							);
	 	 		 		}
	
						if (filter.flag.addDatePerLine) {
							var date_time = new Date(item.fields.date.created); 
							$content_item.append($("<div />").addClass("rwapi-filter-date").html(MONTHS[date_time.getMonth()] + " " + date_time.getDate() + " " +  date_time.getFullYear()));
						}

						var titleText = item.fields.title;
						var primaryCountry_g = item.fields.primary_country.name;

		                                if (titleText.match(/(.*?):(.*)/g))
        		                        {
							titleText = rwHelpers.cut_phrase(titleText, filter.look.titleCharacterLimit);
                        		                titleText = titleText.replace(/:/g, '</strong>:');
		                                        titleText = "<strong>" + titleText;
                		                }
						else
						{
							titleText = rwHelpers.cut_phrase(titleText, filter.look.titleCharacterLimit-primaryCountry_g.length);
							titleText = "<strong>" + primaryCountry_g + "</strong>: " + titleText;
						}
						
						$content_item.append($("<h4 />").addClass("rwapi-filter-art-tit").html("<strong>" + filter.look.titleTextHeader + "</strong><a href='" + item.fields.url + "' target='_blank'>" + titleText + "</a>"));
	
	 	 				if (filter.flag.showBlurbInSummary && item.fields && item.fields.body) {
	 	 					var actualBlurb = rwHelpers.cut_phrase(item.fields.body, filter.look.blurbCharacterLimit); 
	 	 					actualBlurb = rwHelpers.format_text(actualBlurb);
	 	 					$content_item.append($("<span />").addClass("rwapi-filter-art-spi").html(actualBlurb));
	 	 				}
	 	 				
						$content_item.append($("<div/>").addClass("rwapi-clear-block"));
	 	 				$content_area.append($content_item);
	 				});
	 				
	 				$content_area.append($("<div/>").addClass("rwapi-clear-block"));
	 				//$content_area.append($("<div/>").css("clear", "both"));
	 				if ($this.parent().hasClass("list_type_LIST") & filter.flag.showThumbnailInSummary) {
	 					$content_area.find(".rwapi-filter-art-spi").width($content_area.width() - 90);
	 				}
	 				
					//For the regularization of the height of the content area	
					if ($this.closest(".rounded-corner,.rounded-box-gray-250").length) {
						var $title = $this.closest(".rounded-corner,.rounded-box-gray-250").find(".pane-title");
						var $content_block = $this.closest(".reliefwebapi-block-items-filter-list");
						$content_block.height($this.closest(".rounded-corner,.rounded-box-gray-250").height() - $title.height() - 47);
					}
	 				/* Redirect to RW by getting the matched ID for the RW native filtering*/
	 				$content_area.after($("<div/>").addClass("rwapi-clear-block").css("clear" , "both"));
	 				//Check if pagination is present
	 				
	 				if ($pagination_area.length > 0) {
	 					$pagination_area.html(rwHelpers.build_pagination(filter.look.offset, filter.look.itemCountInSummary, d.data.total));
	 				}
	 				
	 				//Save content received for backup...
					var dcopy = d;
					delete dcopy.data['total']; delete dcopy.data['time']; delete dcopy.data['type'];					
					for(var cnt = dcopy.data.list.length-1; cnt >=0; cnt--) {
						delete dcopy.data.list[cnt]['fields']['primary_country'];
						delete dcopy.data.list[cnt]['fields']['source'];
						delete dcopy.data.list[cnt]['fields']['format'];
						delete dcopy.data.list[cnt]['fields']['date'];
						delete dcopy.data.list[cnt]['fields']['score'];
					} 

					/**
					 * This area is for calling the save_content feature taht 
					 * saves all the retrieved item from the reliefweb to the ocha workspace.
					 * this allows for the backup content in case reliefweb api is down
					 */
	 				$.ajax({
	 					type: "POST", 
	 					url : filter.look.baseUrl + "reliefweb/save_content/", 
	 					data: {
	 						message: dcopy
	 					},
	 					success: function(data) {}
	 				});
	 			}, 
	 			
	 			error: function(a, b, c) {
					/**
					 * In case of an error (which may be caused by reliefweb being donw, 
					 * the RW API will grab content from the back up which was saved by the save_content feature
					 */
	 				var $content_area = $this.find(".rwapi-contents");
	 				//If some error occured get information from db and hide the more button
	 				$.ajax({
	 					url: $tf.find("input[name=base_url]").val() + "reliefweb/list_json", 
	 					data: {
	 						fid: _rid,
	 						limit: $tf.find("input[name=summ_show_count]").val()
	 					}, 
	 					dataType: "json",
	 					success: function(data) {
	 						
	 						$(data.items).each(function(i, item) {
                                var $content_item = $("<li />").addClass("rwapi-content-item");
                                if (filter.flag.showThumbnailInSummary) {
                                        $content_item.append($("<a />").attr("href", item.url).attr("target", "_blank").append($("<img />").addClass("rwapi-item-sq-imag").attr("src", item.image)));
                                }
/*
	                            if (parseInt(_addDatePerLine)) {
	                                    var date_time = new Date(item.fields.date.created);
	                                    $content_item.append($("<div />").addClass("rwapi-filter-date").html(MONTHS[date_time.getMonth()] + " " + date_time.getDate() + " " +  date_time.getFullYear()));
	                            }
*/
                                var titleText = rwHelpers.cut_phrase(item.title);
                                $content_item.append($("<h4 />").addClass("rwapi-filter-art-tit").html("<strong>" + filter.look.titleTextHeader + "</strong><a href='" + item.url + "' target='_blank'>" + titleText + "</a>"));

                                if (filter.flag.showBlurbInSummary) {
                                        var actualBlurb = rwHelpers.format_text(item.body);
                                        actualBlurb = rwHelpers.cut_phrase(actualBlurb);
                                        $content_item.append($("<span />").addClass("rwapi-filter-art-spi").html(actualBlurb));
                                }

                                $content_item.append($("<div/>").addClass("rwapi-clear-block"));
                                $content_area.append($content_item);

	 		 				});

                            if ($this.closest(".rounded-corner").length) {
                                var $title = $this.closest(".rounded-corner").find(".pane-title");
                                var $content_block = $this.closest(".reliefwebapi-block-items-filter-list");
                                $content_block.height($this.closest(".rounded-corner").height() - $title.height() - 47);
                            }

	 						$this.find(".reliefwebapi-submit-form").hide();
	 					}, error: function(a,b,c) {
	 					}
	 				});
	 			}
	 		});
	 			
		});
	});
	
	$(".reliefwebapi-submit-form").die("click");
	$(".reliefwebapi-submit-form").live("click", function() {
		var _offset = $(this).attr("offset");
		$(document).trigger("reliefwebapi.grab-content-from-rw", { offset : _offset, target : $(this).closest(".reliefweb-api-pane") });
	});
	
	$(".reliefweb-api-pane").each(function(i, item) {
		$(document).trigger("reliefwebapi.grab-content-from-rw", { offset : 0, target: $(this) });
	});
});
