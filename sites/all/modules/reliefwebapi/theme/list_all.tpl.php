<?php

?>
<div id="reliefwebapi-block-items-filter-page" class="<?php echo isset($filter['list_view_design']) ? "reliefweb-layout-" . strtolower($filter['list_view_design']) : "" ?>">
	<h3><?php echo $filter['name'] ?></h3>
	<form id="rwapi-filters-<?php echo $filter['id'] ?>" class="rwapi-filter-form">
		<input type="hidden" name='primary_country' value='<?php echo t( isset($filter['primary_country'] ) ? $filter['primary_country'] : '') ?>'/>
		<input type="hidden" name='country' value='<?php echo t( isset($filter['country'] ) && !empty($filter['country']) ? implode("|", $filter['country']) : '') ?>'/>
		<input type="hidden" name='organisation' value='<?php echo t( isset($filter['organisation'] ) ? implode("|", $filter['organisation']) : '') ?>'/>
		<input type="hidden" name='theme' value='<?php echo t( isset($filter['theme'] ) ? implode("|",$filter['theme']) : '' ) ?>'/>
		<input type="hidden" name='disaster_type' value='<?php echo t( isset($filter['disaster_type'] ) ? implode("|",$filter['disaster_type']) : '') ?>'/>
		<input type="hidden" name='vulnerable_groups' value='<?php echo t( isset($filter['vulnerable_groups'] ) ?implode("|",$filter['vulnerable_groups']) : '') ?>'/>
		<input type="hidden" name='content_format' value='<?php echo t( isset($filter['content_format'] ) ? implode("|",$filter['content_format']) : '') ?>'/>
		<input type="hidden" name='language' value='<?php echo t( isset($filter['language'] ) ? implode("|",$filter['language']) : '') ?>'/>
		<input type="hidden" name='published_date' value='<?php echo t( isset($filter['published_date'] ) ? implode("|",$filter['published_date']) : '') ?>'/>
		
		<input type="hidden" name='offset' value='<?php echo isset($offset) ? $offset : '0' ?>'/>
		<input type="hidden" name='limit' value='<?php echo $limit ?>'/>
		
		<input type="hidden" name='sing_show_blurb' value='<?php echo t( isset($filter['show_blurb_on_single'] ) ? $filter['show_blurb_on_single'] : '') ?>' />
		<input type="hidden" name='sing_show_thumb' value='<?php echo t( isset($filter['post_num_on_single'] ) ? $filter['show_thumb_on_single'] : '') ?>' />
		<input type="hidden" name='sing_show_count' value='<?php echo t( isset($filter['post_num_on_single'] ) ? $filter['post_num_on_single'] : '') ?>' />

	</form>
	
	<div id="rwapi-filter-items">
	</div>
	
	<div id="rwapi-filter-pagination">
	</div>
</div>
