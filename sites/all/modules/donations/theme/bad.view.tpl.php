<?php
/*Custom function for number formatting... */
function bd_nice_number($n) {
	/*
        // first strip any formatting;
        $n = (0+str_replace(",","",$n));
        
        // is this a number?
        if(!is_numeric($n)) return false;
        
        // now filter it;
        if($n>=1000000000000) return round(($n/1000000000000),2).'T';
        else if($n>=1000000000) return round(($n/1000000000),2).'B';
        else if($n>=1000000) return round(($n/1000000),2).'M';
        */
        return number_format($n);
}

		$current_year = date('Y');
		$overall_total = array();
		$country_membership = array("5" => "Private Donors and Civil Society", "1" => "Member Countries and Observers", "2" => "Regional and  Local Authorities");
		
		$member_types = array(	
								"unpaid" => "Written Pledge", 
								"verbal_pledge" => "Verbal Pledge", 
								"written_pledge" => "Written Pledge", 
								"amount" => "Amount"
								#"single_year_unpaid" => "Single Year Unpaid"
								);
		$red_types = array("unpaid" => "red_unpaid", "verbal_pledge" => "red_verbal", "written_pledge" => "red_written", "amount" => "red_paid");
		$numItems = count($data);
		$counter = 0;
?>
<div id="main-content">
	<?php foreach($data as $member_type_key => $member_type) : 
		$per_section_total = array();
		
	?>
	<h2><?php echo $country_membership[$member_type_key] ?></h2>
	<table border=1 class="donations-table tablesorter" id="donations-table-<?php echo $member_type_key?>">
		<thead>
		<tr>
			<th>&nbsp;</th>
			<th>Donors</th>
			<?php for($i = $min; $i <= $max; $i++) : ?>
					<?php foreach (variable_get($i . "-amounts-list-for-donations", array()) as $key) : ?>
						
							<th class="data"><div><?php echo $i ?><br/><sub><?php echo $member_types[$key] ?></sub></div></th>
					<?php endforeach; ?>
			<?php endfor; ?>
			<th class="data total" data-sortinitialorder="desc" col-name="data-total"><div>Total<br/>Pledges</div></th>
			<th class="data total" data-sortinitialorder="desc" col-name="data-total"><div>Total<br/>Amount</div></th>
		</tr>
		</thead>
		<tbody>
		<?php
			
			foreach ($member_type as $donor_id => $donor) : 

			  if ($donor_total[$donor_id]["pledge"] == 0 
			  		&& $donor_total[$donor_id]["paid"] == 0) continue;

			?>
			<tr>
				<td class="table-sort-ranking"></td>
				<td class="country-data" st-value="<?php echo $donor->name ?>">
					
					<?php if(trim($donor->flag_url) != "") : ?><img width="42" height="25"  src="<?php echo $GLOBALS['base_url'] . "/" . $donor->flag_url; ?>" alt="<?php echo $donor->flag_url ?>" />
					<div class="country-data-name"><?php echo $donor->name ?></div>
					<?php else: ?>
						<?php echo $donor->name ?>
					<?php endif; ?>
				</td>
				
				<?php for($i = $min; $i <= $max; $i++): ?>
					<?php foreach (variable_get($i . "-amounts-list-for-donations", array()) as $key) : ?>
						<?php if (isset($donor->yearly_report[$i])) : ?>
							<td class="data <?php echo $donor->yearly_report[$i]->$red_types[$key] ? "flagged_red" : "" ?>"  col-name="<?php echo $i . "-" . $key ?>" st-value="<?php echo $donor->yearly_report[$i]->$key ?>">
								<div>
									<?php echo $donor->yearly_report[$i]->$key > 0 ? bd_nice_number($donor->yearly_report[$i]->$key) : "&nbsp;"; ?>
							 </div></td>
						<?php else: ?>
							<td class="data"  col-name="<?php echo $i . "-" . $key ?>" st-value="<?php echo 0 ?>"><div>&nbsp;</div></td>
						<?php endif;?>
					<?php endforeach; ?>
				<?php endfor; ?>
				
				<td class="data total pledges" st-value="<?php echo $donor_total[$donor_id]["pledge"] ?>">
					<div><?php echo bd_nice_number( $donor_total[$donor_id]["pledge"] );?></div>
				</td>
				<td class="data total" st-value="<?php echo $donor_total[$donor_id]["paid"] ?>" >
					<div><?php echo bd_nice_number( $donor_total[$donor_id]["paid"] );?></div>
				</td>
			</tr>
			<?php
			 # FOR THE TOTAL PER YEAR
			?>
		<?php endforeach; ?>
		</tbody>
		<tr><td colspan="2" class="data overall-total"><div>Total</div></td>
			<?php 
				for($i = $min; $i <= $max; $i++): ?>
						<?php foreach (variable_get($i . "-amounts-list-for-donations", array()) as $key) : ?>
								<td class="data overall-total"  col-name="<?php echo $i . "-" . $key ?>" >
									<div><?php echo bd_nice_number($overall_total_per_column[$member_type_key][$i]->$key); ?></div>
								</td>
						<?php endforeach; ?>
			<?php endfor; ?>
				
			<td class="data overall-total pledge" col-name="data-total"><div><?php echo bd_nice_number($overall_total_per_column[$member_type_key]["pledge"]); ?></div></td>
			<td class="data overall-total paid" col-name="data-total"><div><?php echo bd_nice_number($overall_total_per_column[$member_type_key]["paid"]); ?></div></td>
		</tr>
		<?php if(++$counter === $numItems) : ?>
		<tr>
			<td colspan="2" class="data mega-overall-total data-text">OVERALL TOTAL</td>
			<?php for($i = $min; $i <= $max; $i++): ?>
				<?php foreach (variable_get($i . "-amounts-list-for-donations", array()) as $key) : ?>
					<td class="data mega-overall-total"  col-name="<?php echo $i . "-" . $key ?>" 	>
						<div><?php echo bd_nice_number($overall_total_per_column["megatotal"]["breakdown"][$i]->$key); ?></div>
					</td>
				<?php endforeach; ?>
			<?php endfor; ?>
			<td class="data mega-overall-total"  col-name="data-total"><div><?php echo bd_nice_number($overall_total_per_column["megatotal"]["pledge"]); ?></div></td>
			<td class="data mega-overall-total"  col-name="data-total"><div><?php echo bd_nice_number($overall_total_per_column["megatotal"]["paid"]); ?></div></td>
		</tr>
		<?php endif; ?>
	</table>
	<?php endforeach;  ?>
</div>
