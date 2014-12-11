<div id="reliefweb-api-vie-fil">
	<div id='rw-menu-area'>
		<?= l('Create New Filter', 'admin/settings/content/reliefwebapi/add') ?>
	</div>
	
	<div id="rw-general-options">
	</div>
	
	<div id="rw-filter-list">
		<table>
			<tr>
				<th>Filter name</th>
				<th>Design</th>
				<th>Primary Country</th>
				<th>Country</th>
				<th>Organisation</th>
				<th>Theme</th>
				<th>Disaster Type</th>
				<th>Vulnerable Groups</th>
				<th>Format</th>
				<th>Language</th>
				<th>Published Date</th>
				<th>Action?</th>
				<th>Get Widget</th>
			</tr>
			<?php foreach($filters as $filter) : ?>
			<tr>
				<td><?= $filter['name']?></td>
				<td><?= $filter['primary_country']?></td>
				<td>Items per Summary: <?= $filter['post_num_on_list'] ?><br/>
					Items per View: <?= $filter['post_num_on_single'] ?><br/>
					Show Blurb on Summary: <?= $filter['show_blurb_on_list'] ? "YES" : "NO" ?><br/>
					Show Blurb on View: <?= $filter['show_blurb_on_single'] ? "YES" : "NO" ?><br/>
					Show Thumb on Summary: <?= $filter['show_thumb_on_list'] ? "YES" : "NO" ?><br/>
					Show Thumb on View: <?= $filter['show_thumb_on_single'] ? "YES" : "NO" ?><br/>
					List Design: <?= ucfirst(strtolower($filter['list_view_design'])) ?><br/>
				</td>
				<td><?= implode("<br/>", $filter['country'] )?></td>
				<td><?= implode("<br/>", $filter['organisation'] )?></td>
				<td><?= implode("<br/>", $filter['theme'])?></td>
				<td><?= implode("<br/>", $filter['disaster_type'])?></td>
				<td><?= implode("<br/>", $filter['vulnerable_groups'])?></td>
				<td><?= implode("<br/>", $filter['content_format'])?></td>
				<td><?= implode("<br/>", $filter['language'])?></td>
				<td><?= implode("<br/>", $filter['published_date'])?></td>
				<td><?= l("Edit", 'admin/settings/content/reliefwebapi/edit/' . $filter['id'], array("class" => "rwapi-action-link-edit")) ?> &bull;
					<?= l("View", 'reliefweb/list/' . $filter['id'], array("class" => "rwapi-action-link-view")) ?> &bull; 
					<?= l("Delete", 'admin/settings/content/reliefwebapi/delete/' . $filter['id'], array("attributes" => array("class" => "rwapi-action-link-delete"))) ?></td>
				<td>{Copy Widget}</td>
			</tr>
			<?php endforeach; ?>
		</table>
	</div>
</div>