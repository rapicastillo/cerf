<?php 
	$amount_keys = array('amount', 'unpaid', 'verbal', 'written', 'single_year_unpaid');
	$red_types = array("unpaid" => "red_unpaid", "verbal" => "red_verbal", "written" => "red_written", "amount" => "red_paid");
?>
<?php foreach ($donors as $donor) : ?>
<?php if (!isset($data[$donor['id']])) { $data[$donor['id']] = array(); }?>
<tr class="don-donor-<?php echo $donor['id']; ?> don-donor-list-item" row_country="<?php echo strtoupper($donor['name']) ; ?>">
	<?php foreach (range(2006, date('Y', strtotime('+2 years'))) as $year) : ?>
	<?php $amount_list = isset($data[$donor['id']]->yearly_report[$year]) ? $data[$donor['id']]->yearly_report[$year] : NULL; #foreach ($data[$donor['id']] as $year => ) : ?>
	<?php if ($amount_list) : ?>
		<?php foreach ($amount_keys as $amount_key) : ?>
			<td style="white-space: nowrap;" class="don-type-<?php echo $amount_key ?> don-data-value don-year-<?php echo $year; ?> <?php echo isset($amount_list->$red_types[$amount_key]) && $amount_list->$red_types[$amount_key] ? 'flagged_red' : '' ?>">
				<span class="don-item-amount">
					<?php echo $amount_list->$amount_key ? $amount_list->$amount_key : "-"; ?>
				</span>
				<form class="edit-amount-value">
					<input type="hidden" name="nid" value="<?php echo isset($amount_list->nid) ? $amount_list->nid : ""; ?>" />
					<input type="hidden" name="donor_name" value="<?php echo $donor['name']; ?>" />
					<input type="hidden" name="year" value="<?php echo $year; ?>"/>
					<input type="hidden" name="donor_id" value="<?php echo $donor['id']; ?>"/>
					<input type="hidden" name="amount_key" value="<?php echo $amount_key; ?>"/>
					<input type="hidden" name="original_amount" value="<?php echo $amount_list->$amount_key ? $amount_list->$amount_key : ""; ?>"/>
					<input type="text" value="<?php echo $amount_list->$amount_key ? $amount_list->$amount_key : ""; ?>" name="amount"/>
					<a href='javascript: void(0);' class='don-save-submit'><img src="/cerf/sites/default/files/images/save.jpg" style="height: 12px;"/></a>
					<a href='javascript: void(0);' class='don-cancel-submit'><img src="/cerf/sites/default/files/images/cancel.png" style="height: 12px;"/></a>
					<input type="checkbox" name="red_value" title="Red Value?" value="1" <?php echo isset($amount_list->$red_types[$amount_key]) && $amount_list->$red_types[$amount_key] ? 'checked="checked"' : '' ?>>
				</form>
			</td>
		<?php endforeach; ?>							
	<?php else: ?>
		<?php foreach ($amount_keys as $amount_key) : ?>
			<td style="white-space: nowrap;" class="don-type-<?php echo $amount_key ?> don-year-<?php echo $year; ?>">
				<span>-</span>
				<form class="edit-amount-value">
					<input type="hidden" name="nid" value="" />
					<input type="hidden" name="donor_name" value="<?php echo $donor['name']; ?>" />
					<input type="hidden" name="year" value="<?php echo $year; ?>"/>
					<input type="hidden" name="donor_id" value="<?php echo $donor['id'] ?>"/>
					<input type="hidden" name="amount_key" value="<?php echo $amount_key ?>"/>
					<input type="hidden" name="original_amount" value="<?php echo $amount_list->$amount_key ? $amount_list->$amount_key : ""; ?>"/>
					<input type="text" value="<?php echo $amount_list->$amount_key ? $amount_list->$amount_key : ""; ?>" name="amount"/>

					<a href='javascript: void(0);' class='don-save-submit'><img src="/cerf/sites/default/files/images/save.jpg" style="height: 12px;"/></a>
					<a href='javascript: void(0);' class='don-cancel-submit'><img src="/cerf/sites/default/files/images/cancel.png" style="height: 12px;"/></a>
					<input type="checkbox" name="red_value" value="1" <?php echo isset($amount_list->$red_types[$amount_key]) && $amount_list->$red_types[$amount_key] ? 'checked="checked"' : '' ?>>
				</form>
			</td>						
		<?php endforeach; ?>
	<?php endif; ?>
	<?php endforeach; ?>
</tr>
<?php endforeach; ?>
