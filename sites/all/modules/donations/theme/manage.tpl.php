<?php 
	$amount_keys = array('amount', 'unpaid', 'verbal', 'written', 'single_year_unpaid');
	
?>
<div id="don-flash-notifier" style="display: none">Saved Item</div>

<div id="don-cont-filter-container">
	
	<?php #FILTER AREA ?>
	<?php echo '<a href="' . $base_url . '/cerf/admin/settings/donations_manager/view_options">Viewing Options</a>'; ?>
	<hr />
	<h4>Filters</h4>
	<form id="don-filters-area">
		<h5>Year</h5>
		<div class="filter-list" id="filter-year">
			<?php foreach(range(2006, date('Y', strtotime('+2 years'))) as $year) : ?> 
				<label for="<?php echo $year?>"><input type="checkbox" name="year" checked="checked" id="<?php echo $year ?>" value=".don-year-<?php echo $year ?>"/><?php echo $year ?></label>
			<?php endforeach; ?>			
		</div>
		
		<h5>Amount Type</h5>
		<div class="filter-list" id="filter-amount">
			<?php foreach($amount_keys as $amount_key) : ?> 
				<label for="<?php echo $amount_key?>"><input type="checkbox" name="amount_key" checked="checked" id="<?php echo $amount_key ?>" value=".don-type-<?php echo $amount_key  ?>" />
							<?php echo str_replace('_', ' ', $amount_key); ?></label>
			<?php endforeach; ?>
		</div>
		
		<h5>Donor</h5>
		<div class="filter-list" id="filter-donor">
			<input type="text" name="country_filter" placeholder="Filter Donor List Here">
		</div>
	</form>	
</div>

<div id="don-cont-editor-container">
	
	<div id='don-hdr-are' style="width: 100%;">
		<div id='don-names-hdr'  style="width:15%; float: left; display: inline-block;">Donor</div>
		<div id='don-fund-hdr' style="width:84%; float: left; overflow-x: hidden;">
			<table  style="width: auto; table-layout:fixed">
				<tr>
					<?php foreach (range(2006, date('Y', strtotime('+2 years'))) as $year) : ?>
						<?php foreach ($amount_keys as $amount_key) : ?>
							<td style="white-space: nowrap;" class="don-year-<?php echo $year; ?> don-type-<?php echo $amount_key ?>"><div><?php echo $year; ?><br/><span style="font-size: 8px"><?php echo str_replace('_', ' ', $amount_key) ?></span></div></td>
						<?php endforeach; ?>
					<?php endforeach; ?>
				</tr>
				<tr></tr>
			</table>
		</div>
		<div style="clear: both;"></div>
	</div>
	
	
	<div id='don-content-are' style="overflow-y: auto; height: 500px;"> 
		<div id='don-names-list' style="width:15%; float: left; display: inline-block;">
			<h2>Member States<br/>and Observers</h2>
			<table style="border-collapse: inherit; border-width: 0px;">
			<?php foreach ($donors as $id => $donor) : ?>
				<tr id="donor-i-<?php echo $id; ?>" class="don-donor-list-label" title="<?php echo strtoupper($donor['name']); ?>">
					<td><div class='don-name-itm'><?php echo $donor['name']; ?></div></td>
				</tr>
			<?php endforeach; ?>
			</table>
		
			<h2>Regional and<br/>Local Authorities</h2>
			<table style="border-collapse: inherit; border-width: 0px;">
			<?php foreach ($regional_donors as $id => $donor) : ?>
				<tr id="donor-i-<?php echo $id; ?>" class="don-donor-list-label" title="<?php echo strtoupper($donor['name']); ?>">
					<td><div class='don-name-itm'><?php echo $donor['name']; ?></div></td>
				</tr>
			<?php endforeach; ?>
			</table>
			
			<h2>Private Donors and<br/>Civil Society</h2>
			<table style="border-collapse: inherit; border-width: 0px;">
			<?php foreach ($private_donors as $id => $donor) : ?>
				<tr id="donor-i-<?php echo $id; ?>" class="don-donor-list-label" title="<?php echo strtoupper($donor['name']); ?>">
					<td><div class='don-name-itm'><?php echo $donor['name']; ?></div></td>
				</tr>
			<?php endforeach; ?>
			</table>
		</div>
		
		<div id='don-fund-cont' style="float: left; display: inline-block; width: 84%; overflow-x: hidden;">
			<div id='don-fund-list' style=" display: inline-block; ">
				<h2>&nbsp;<br/>&nbsp;</h2>
				<table id='don-fund-table-proper' style="border-collapse: inherit; border-width: 0px; width: auto; table-layout:fixed;">
					<?php echo $donor_render["member_states"] ?>
				</table>
	
				<h2>&nbsp;<br/>&nbsp;</h2>
				<table id='don-fund-table-regional-proper' style="border-collapse: inherit; border-width: 0px; width: auto; table-layout:fixed;">
					<?php echo $donor_render["regional"] ?>
				</table>

				<h2>&nbsp;<br/>&nbsp;</h2>
				<table id='don-fund-table-private-proper' style="border-collapse: inherit; border-width: 0px; width: auto; table-layout:fixed;">
					<?php echo $donor_render["private"] ?>
				</table>
			</div>
		</div>
		<div style="clear: both;"></div>
	 
	</div>
	<div style="clear: both;"></div>
	<div id='don-fund-scroller-cont' style="float: left; width: 84%; margin-left: 15%; overflow-x: auto; height: 20px; overflow-y: hidden">
		<div id="don-fund-scroller" style="width: 2000px">&nbsp;</div>
	</div>
	<div style="clear: both;"></div> 
	
	
</div>