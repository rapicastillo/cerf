<div id="rwapi-manage-wrapper">
	<div class="rwapi-error">You must fill out required fields.</div>
	<form id="reliefwebapi-insert-item" name="reliefwebapi-insert-item" action="/admin/settings/reliefwebapi/insert" method="POST">
		<? if ($filter['id']) : ?>
			<input type="hidden" name='rwapi-filter-id' value='<?= $filter['id'] ?>' />
		<? endif; ?>
		<h2>Design Options</h2>
		<table>
			<tr>
				<th>Filter Group Name <span class="rwapi-required">*</span></th>
				<td><input type="text" ref="Filter Name" name="rwapi-filter-name" rwapi-required='true' value="<?= $filter['name'] ? $filter['name'] : "" ?>"/></td>
			</tr>
			<tr>
				<th>Number of Items in Filter Summary <span class="rwapi-required">*</span></th>
				<td><input type="number" ref="Number of items in Filter List" name="rwapi-filter-list-cnt" rwapi-required='true' 
					value="<?= isset($filter['post_num_on_list']) ? $filter['post_num_on_list'] : 1 ?>" /></td>
			</tr>
			<tr>
				<th>Number of Items in Filter View <span class="rwapi-required">*</span></th>
				<td><input type="number" ref="Number of items in Filter View" name="rwapi-filter-view-cnt" rwapi-required='true' 
						value="<?= isset($filter['post_num_on_single']) ? $filter['post_num_on_single'] : 10 ?>"/></td>
			</tr>
			<tr>
				<th>Show Thumbnail in Filter Summary</th>
				<td>
					<label><input type="radio" name="rwapi-filter-show-thumb-list" value="0" <?= !$filter['show_thumb_on_list'] ? "checked='checked'" : "" ?> /> NO</label>&nbsp;&nbsp;
					<label><input type="radio" name="rwapi-filter-show-thumb-list" value="1" <?= $filter['show_thumb_on_list'] ? "checked='checked'" : "" ?> /> YES</label>
				</td>
			</tr>
			<tr>
				<th>Show Thumbnail in Filter View</th>
				<td>
					<label><input type="radio" name="rwapi-filter-show-thumb-view" value="0" <?= !$filter['show_thumb_on_single'] ? "checked='checked'" : "" ?> /> NO</label>&nbsp;&nbsp;
					<label><input type="radio" name="rwapi-filter-show-thumb-view" value="1" <?= $filter['show_thumb_on_single'] ? "checked='checked'" : "" ?> /> YES</label>
				</td>
			</tr>
			<tr>
				<th>Show Blurb in Filter Summary</th>
				<td>
					<label><input type="radio" name="rwapi-filter-show-blurb-list" value="0" <?= !$filter['show_blurb_on_list'] ? "checked='checked'" : "" ?> /> NO</label>&nbsp;&nbsp;
					<label><input type="radio" name="rwapi-filter-show-blurb-list" value="1" <?= $filter['show_blurb_on_list'] ? "checked='checked'" : "" ?> /> YES</label>
				</td>
			</tr>
			<tr>
				<th>Show Blurb in Filter View</th>
				<td>
					<label><input type="radio" name="rwapi-filter-show-blurb-view" value="0" <?= !$filter['show_blurb_on_single'] ? "checked='checked'" : "" ?> /> NO</label>&nbsp;&nbsp;
					<label><input type="radio" name="rwapi-filter-show-blurb-view" value="1" <?= $filter['show_blurb_on_single'] ? "checked='checked'" : "" ?> /> YES</label>
				</td>
			</tr>
			<tr>
				<th>List Design</th>
				<td>
					<select name='rwapi-filter-list-design'>
						<option value="LIST" <?= $filter['list_view_design'] == "LIST"? "selected='selected'" : "" ?>>List</option>
						<option value="TILE" <?= $filter['list_view_design'] == "TILE"? "selected='selected'" : "" ?>>Tile</option>
					</select>
				</td>
			</tr>
		</table>
			<?php /*
			<tr>
				<th style="width: 30%">Category <span class="rwapi-required">*</span></th>
				<td>
					<label><input type="radio" name="rwapi-category" value="rwapi_disaster"/> Disaster</label><br/>
					<label><input type="radio" name="rwapi-category" value="rwapi_updates" checked="checked"/> Updates</label><br/>
					<label><input type="radio" name="rwapi-category" value="rwapi_countries"/> Countries</label><br/>
				</td>
			</tr> */ ?>
		<h2>Filtering Options</h2>
		<table>
			<tr rwapi-group="rwapi_updates">
				<th>Filter By Primary Country</th>
				<td>
					<div id="rwapi-curr-primary-country" >
						<?php if ($filter['primary_country']) : ?>
							<div class="rwapi-select-chosen-item">
								<input type="hidden" name="rwapi-chosen-primary-country" value="<?= $filter['primary_country'] ?>">
								<span><?= $filter['primary_country'] ?></span>
								<span class="rwapi-select-chosen-remove">x</span>
							</div>
						<?php endif; ?>
					</div>
					<select name="rwapi-manage-primary-country" <?=  $filter['primary_country'] ? "style='display: none'" : "" ?> >
						<option value="0"><em>Choose on Primary Country</em></option>
						<?php include 'filters/countries.inc' ?>
					</select>
				</td>
			</tr>
			
			<tr rwapi-group="rwapi_updates,rwapi_disaster,rwapi_countries">
				<th>Filter By Country</th>
				<td>
					<?php foreach ($filter['country'] as $country) : ?>
						<?php if(trim($country) == '') { continue; } ?>
						<div class="rwapi-select-chosen-item">
							<input type="hidden" name="rwapi-chosen-country[]" value="<?= $country ?>">
							<span><?= $country ?></span>
							<span class="rwapi-select-chosen-remove">x</span>
						</div>
					<?php endforeach; ?>
					<div id="rwapi-curr-country"></div>
					<select name="rwapi-manage-country">
						<option><em>Choose on Country</em></option>
						<?php include 'filters/countries.inc' ?>
					</select>
				</td>
			</tr>
			
			
			<tr rwapi-group="rwapi_updates">
				<th>Filter By Organisation</th>
				<td>
					<?php foreach ($filter['organisation'] as $item) : ?>
						<?php if(trim($item) == '') { continue; } ?>
						<div class="rwapi-select-chosen-item">
							<input type="hidden" name="rwapi-chosen-organisation[]" value="<?= $item ?>">
							<span><?= $item ?></span>
							<span class="rwapi-select-chosen-remove">x</span>
						</div>
					<?php endforeach; ?>
					<div id="rwapi-curr-organisation"></div>
					<select name="rwapi-manage-organisation">
						<option value="0"><em>Choose on Organisation</em></option>
						<?php include 'filters/organisation.inc' ?>
					</select>
				</td>
			</tr>
			
			<tr rwapi-group="rwapi_updates">
				<th>Filter By Theme</th>
				<td>
					
					<?php foreach ($filter['theme'] as $item) : ?>
						<?php if(trim($item) == '') { continue; } ?>
						<div class="rwapi-select-chosen-item">
							<input type="hidden" name="rwapi-chosen-theme[]" value="<?= $item ?>">
							<span><?= $item ?></span>
							<span class="rwapi-select-chosen-remove">x</span>
						</div>
					<?php endforeach; ?>
					<div id="rwapi-curr-theme"></div>
					<select name="rwapi-manage-theme">
						<option value="0"><em>Choose Theme</em></option>
						<?php include 'filters/theme.inc' ?>
					</select>
				</td>
			</tr>
			
			<tr rwapi-group="rwapi_updates,rwapi_disaster">
				<th>Filter By Disaster Type</th>
				<td>
					
					<?php foreach ($filter['disaster_type'] as $item) : ?>
						<?php if(trim($item) == '') { continue; } ?>
						<div class="rwapi-select-chosen-item">
							<input type="hidden" name="rwapi-chosen-disaster-type[]" value="<?= $item ?>">
							<span><?= $item ?></span>
							<span class="rwapi-select-chosen-remove">x</span>
						</div>
					<?php endforeach; ?>
					<div id="rwapi-curr-disaster-type"></div>
					<select name="rwapi-manage-disaster-type">
						<option value="0"><em>Choose Disaster Type</em></option>
						<?php include 'filters/disaster_type.inc' ?>
					</select>
				</td>
			</tr>
			
			<tr rwapi-group="rwapi_updates">
				<th>Filter By Vulnerable Groups</th>
				<td>
					
					<?php foreach ($filter['vulnerable_groups'] as $item) : ?>
						<?php if(trim($item) == '') { continue; } ?>
						<div class="rwapi-select-chosen-item">
							<input type="hidden" name="rwapi-chosen-vulnerable-groups[]" value="<?= $item ?>">
							<span><?= $item ?></span>
							<span class="rwapi-select-chosen-remove">x</span>
						</div>
					<?php endforeach; ?>
					<div id="rwapi-curr-vulnerable-groups"></div>
					<select name="rwapi-manage-vulnerable-groups">
						<option value="0"><em>Choose Vulnerable Groups</em></option>
						<?php include 'filters/vulnerable_groups.inc' ?>
					</select>
				</td>
			</tr>
			
			
			<tr rwapi-group="rwapi_updates">
				<th>Filter By Content Format</th>
				<td>
					<?php foreach ($filter['content_format'] as $item) : ?>
						<?php if(trim($item) == '') { continue; } ?>
						<input type='hidden' name='rwapi-tmp-content-format' value='<?= $item ?>' />
					<?php endforeach; ?>
					
					<?php include 'filters/content_format.inc' ?>
				</td>
			</tr>
			
			<tr rwapi-group="rwapi_updates">
				<th>Filter By Language</th>
				<td>
					<?php foreach ($filter['language'] as $item) : ?>
						<?php if(trim($item) == '') { continue; } ?>
						<input type='hidden' name='rwapi-tmp-langauge' value='<?= $item ?>' />
					<?php endforeach; ?>
					
					<?php include 'filters/language.inc' ?>
				</td>
			</tr>
			
			<tr rwapi-group="rwapi_updates,rwapi_disaster">
				<th>Filter By Original Published Date</th>
				<td>
					<?php foreach ($filter['published_date'] as $item) : ?>
						<?php if(trim($item) == '') { continue; } ?>
						<input type='hidden' name='rwapi-tmp-published-date' value='<?= $item ?>' />
					<?php endforeach; ?>
					
					<?php include 'filters/published_date.inc' ?>
				</td>
			</tr>
			
		</table>
		<?php if ($filter['id']) : ?>
			<button id="rwapi-manage-submit-button" >Update <?= $filter['name']?></button>
			
			<?= l('Cancel', 'admin/config/content/reliefwebapi') ?>
		<?php else: ?>
			<button id="rwapi-manage-submit-button">Add</button>
		<?php endif; ?>
	</form>
</div>