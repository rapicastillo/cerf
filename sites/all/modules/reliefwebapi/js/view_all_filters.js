jQuery(function($) {
	$(".rwapi-action-link-delete").click(function(event) {
		event.stopPropagation();
		
		if ( confirm("Are you sure?") ) {
			return true;
		} else {
			return false;
		}
		
		
	});
});