<?php if($is_front) : ?>
<h1>ReliefWeb Filters</h1>
<div id="reliefwebapi-block-items-filter-list">
	<?php foreach($items as $item) : ?>
		<div class="rwapi-item-square">
			<form id="rwapi-filters-<?= $item['id'] ?>" class="rwapi-filter-form">
				<input type="hidden" name='primary_country' value='<?= t( isset($item['primary_country'] ) ? $item['primary_country'] : '') ?>'/>
				<input type="hidden" name='country' value='<?= t( isset($item['country'] ) ? $item['country'] : '') ?>'/>
				<input type="hidden" name='organisation' value='<?= t( isset($item['organisation'] ) ? $item['organisation'] : '') ?>'/>
				<input type="hidden" name='theme' value='<?= t( isset($item['theme'] ) ? $item['theme'] : '') ?>'/>
				<input type="hidden" name='disaster_type' value='<?= t( isset($item['disaster_type'] ) ? $item['disaster_type'] : '') ?>'/>
				<input type="hidden" name='vulnerable_groups' value='<?= t( isset($item['vulnerable_groups'] ) ? $item['vulnerable_groups'] : '') ?>'/>
				<input type="hidden" name='content_format' value='<?= t( isset($item['content_format'] ) ? $item['content_format'] : '') ?>'/>
				<input type="hidden" name='language' value='<?= t( isset($item['language'] ) ? $item['language'] : '') ?>'/>
				<input type="hidden" name='published_date' value='<?= t( isset($item['published_date'] ) ? $item['published_date'] : '') ?>'/>
				<input type="hidden" name='offset' value='<?= t( isset($item['published_date'] ) ? $item['published_date'] : '') ?>'/>
				
				<input type="hidden" name='summ_show_blurb' value='<?= t( isset($item['show_blurb_on_list'] ) ? $item['show_blurb_on_list'] : '') ?>' />
				<input type="hidden" name='summ_show_thumb' value='<?= t( isset($item['show_thumb_on_list'] ) ? $item['show_thumb_on_list'] : '') ?>' />
				<input type="hidden" name='summ_show_count' value='<?= t( isset($item['post_num_on_list'] ) ? $item['post_num_on_list'] : '') ?>' />
			</form>
			<h5><?= t( $item['filter_name']) ?></h5>
			
			<h4 class="rwapi-filter-art-tit"></h4>
			<img src="http://placehold.it/150x150" class="rwapi-item-sq-imag" style="display: none;"/>
			<span class="rwapi-filter-art-spi"></span>
			
			<div class="rwapi-more-link"><?= l(t( "more " . $item['filter_name']), 'reliefweb/list/' . $item['id'], array('class' => 'rwapi-more-link-anchor') ) ?></div>
		</div> 
	<?php endforeach; ?>
</div>
<?php endif; ?>

<?php /*
 * 
 * 
 * f[0]=field_primary_country:13
 * &f[1]=field_country:16
 * &f[2]=field_country:20
 * &f[3]=field_source:1482
 * &f[4]=field_content_format:8
 * &f[5]=field_content_format:3
 * &f[6]=field_disaster_type:4611
 * &f[7]=field_disaster_type:4648
 * &f[8]=field_language:267
 */