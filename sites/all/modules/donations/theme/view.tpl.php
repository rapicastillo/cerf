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

		$current_year = $max > (int) date('Y') ? date('Y') : $max;
		$overall_total = array();
		$country_membership = array("2" => "Private Sector and Civil Society", "1" => "Member States and Observers (*)", "5" => "Regional and  Local Authorities");
		
		$member_types = array(
							"amount" => "Contribution",	
								"unpaid" => "Outstanding Contribution", 
								"verbal_pledge" => "Verbal Pledge", 
								"written_pledge" => "Written Pledge", 
								#"single_year_unpaid" => "Single Year Unpaid"
								);
		$red_types = array("unpaid" => "red_unpaid", "verbal_pledge" => "red_verbal", "written_pledge" => "red_written", "amount" => "red_paid");
		$numItems = count($data);
		$counter = 0;
		
		$new_data = array("1" => $data["1"], "5" => $data["5"], "2" => $data["2"]);

function nice_year_list($arr) {
	if (is_array($arr) && count($arr) > 0) {
		if (count($arr) == 2) {
			return "{$arr[0]} & {$arr[1]}";
		} elseif(count($arr) == 1) {
			return $arr[0];
		} else {
			$cnt = count($arr);
			$last_item = $arr[$cnt -1];
			
			unset($arr[$cnt - 1]);
			
			return implode(", ", $arr) . " & " . $last_item;
		}
	} else {
		return $arr;
	}
}
?>
<?php if($min == $max) : ?>
<?php #h3 class="rtecenter"><strong>CERF Pledges and Contributions <?php echo $max; </strong></h3>  ?>
<?php endif;  ?>

<?php $hack_merged = false; ?>
<div id="main-content">
	<?php foreach($new_data as $member_type_key => $member_type) : 
		$per_section_total = array();
		$hack_merged = false	
	?>
	<h2><?php echo $country_membership[$member_type_key] ?></h2>
	<table border=1 class="donations-table tablesorter" id="donations-table-<?php echo $member_type_key?>">
		<thead>
		<tr>
			<th>&nbsp;</th>
			<th>Donors</th>
			<?php foreach (array_keys($member_types) as $payment_type) : 
					$included_years = array();
				?>
				<?php for($i = $min; $i <= $max; $i++) : 
						$key_item = variable_get($i . "-amounts-list-for-donations", array());
						
						#skip adding unpaid items from past eyars
						if ($i < $current_year && $payment_type == "unpaid" && in_array("unpaid", $key_item)) { $included_years[] = $i; continue; }
						elseif($i == $current_year && $payment_type == "unpaid") { $included_years[] = $i; }
					
					?>

					<?php if ($i < 2010 && $min != $max) : continue; ?>
					<?php elseif ($i == 2010 && $min != $max && !$hack_merged) : $hack_merged = true;?>
						<th class="data <?php echo $payment_type=="amount"&&$min==$max?"year-contrib":""?>"><span><?php echo $min . " - " . $i ?></span><br/><sub>Contribution</sub></th>
					<?php elseif (in_array($payment_type, $key_item)) : ?>
							<th class="data <?php echo $payment_type=="amount"&&$min==$max?"year-contrib":""?>"><span><?php echo $payment_type == "unpaid" ? nice_year_list($included_years) : $i; ?><br/><sub><?php echo $member_types[$payment_type] ?></sub></span></th>
						<?php endif; ?>
				<?php endfor; ?>
				
				<?php if ( $payment_type == "amount" && $min!=$max) : ?>
					<th class="data total" data-sortinitialorder="desc" col-name="data-total"><sub>Total<br/>Contributions</sub></th>
				<?php endif; ?>
			<?php endforeach; 


			$hack_merged = false;
			?>
		</tr>
		</thead>
		<tbody>
		<?php

		
			
			foreach ($member_type as $donor_id => $donor) : 

			  $sum_merged = 0;
			  if ($donor_total[$donor_id]["pledge"] == 0 
			  		&& $donor_total[$donor_id]["paid"] == 0 
					&& $donor_total[$donor_id]["unpaid"] == 0) continue;

			?>
			<tr>
				<td class="table-sort-ranking"></td>
				<td class="country-data" st-value="<?php echo $donor->name ?>">
					
					<?php if(trim($donor->flag_url) != "") : ?><img width="42" height="25"  src="<?php echo $GLOBALS['base_url'] . "/" . $donor->flag_url; ?>" alt="<?php echo $donor->flag_url ?>" />
					<span class="country-data-name"><?php echo $donor->name ?></span>
					<?php else: ?>
						<span class="country-data-name country-without-photo"><?php echo $donor->name ?></span>
					<?php endif; ?>
				</td>
				
			<?php 	$hack_merged = false;
				foreach (array_keys($member_types) as $payment_type) : 

				?>
				<?php for($i = $min; $i <= $max; $i++) : 
						$key_item = variable_get($i . "-amounts-list-for-donations", array());
						
						#skip adding unpaid items from past eyars
						if ($i < $current_year && $payment_type == "unpaid") { continue; }
					?>

					<?php if ($i < 2010 && $min != $max) : $sum_merged += $donor->yearly_report[$i]->$payment_type; continue; ?>

                                        <?php elseif ($i == 2010 && $min != $max && !$hack_merged) : $hack_merged = true; 

							$sum_merged += $donor->yearly_report[$i]->$payment_type;
						?>
                                                <td class="data"  col-name="<?php echo $i . "-" . $payment_type ?>" st-value="<?php echo $sum_merged?>"><span><?php echo bd_nice_number($sum_merged); ?></span></td>

					<?php elseif (in_array($payment_type, $key_item)) : ?>

							<?php if (isset($donor->yearly_report[$i])) : ?>
								<td class="data <?php echo $min==$max&&$payment_type=="amount"?"year-contrib":""?> <?php echo $donor->yearly_report[$i]->$red_types[$payment_type] ? "flagged_red" : "" ?>"  col-name="<?php echo $i . "-" . $payment_type ?>" st-value="<?php echo $donor->yearly_report[$i]->$payment_type ?>">
									<span>
										<?php echo $donor->yearly_report[$i]->$payment_type > 0 ? bd_nice_number($donor->yearly_report[$i]->$payment_type) : "&nbsp;"; ?>
								 </span></td>
							<?php else: ?>
								<td class="data"  col-name="<?php echo $i . "-" . $payment_type ?>" st-value="<?php echo 0 ?>"><span>&nbsp;</span></td>
							<?php endif;?>
					<?php endif; ?>
				<?php endfor; ?>
				
				<?php if($payment_type == "amount" && $min != $max) : ?>
					<td class="data total" st-value="<?php echo $donor_total[$donor_id]["paid"] ?>" col-name="data-total" >
						<span><?php echo bd_nice_number( $donor_total[$donor_id]["paid"] );?></span>
					</td>
				<?php endif; ?>
			<?php endforeach; ?>
				<?php /*
				<td class="data total pledges" st-value="<?php echo $donor_total[$donor_id]["pledge"] ?>">
					<div><?php echo bd_nice_number( $donor_total[$donor_id]["pledge"] );?></div>
				</td>
				 * 
				 */?>
				
			</tr>
			<?php
			 # FOR THE TOTAL PER YEAR
			?>
		<?php endforeach; ?>
		</tbody>
		<tr><td colspan="2" class="data overall-total data-text"><span>Subtotal</span></td>
			<?php 
				$sum_merged = 0;
				$hack_merged = false;
				foreach (array_keys($member_types) as $payment_type) : ?>
				<?php for($i = $min; $i <= $max; $i++) : 
						$key_item = variable_get($i . "-amounts-list-for-donations", array());
						
						#skip adding unpaid items from past eyars
						if ($i < $current_year && $payment_type == "unpaid") { continue; }
					?>

					<?php if ($i < 2010 && $min != $max) : $sum_merged += $overall_total_per_column[$member_type_key][$i]->$payment_type; continue; ?>
                                	<?php elseif ($i == 2010 && $min != $max && !$hack_merged) : $hack_merged = true;

                                                        $sum_merged += $overall_total_per_column[$member_type_key][$i]->$payment_type;
                                                ?>
                                        <td class="data overall-total"  col-name="<?php echo $i . "-" . $payment_type ?>" st-value="<?php echo $sum_merged?>"><span><?php echo bd_nice_number($sum_merged); ?></span></td>

					<?php elseif (in_array($payment_type, $key_item)) : ?>
								<td class="data overall-total"  col-name="<?php echo $i . "-" . $payment_type ?>" >
									<span><?php echo bd_nice_number($overall_total_per_column[$member_type_key][$i]->$payment_type); ?></span>
								</td>
					<?php endif; ?>
				<?php endfor; ?>
				

				<?php if ($payment_type == "amount" && $min!=$max) : ?>
					<td class="data overall-total paid" col-name="data-total"><span><?php echo bd_nice_number($overall_total_per_column[$member_type_key]["paid"]); ?></span></td>
				<?php endif; ?>
			<?php endforeach; ?>
				
		</tr>
		<?php if(++$counter === $numItems) : 
			$overall_total_lt2010 = 0;
			$hack_merged = false;
		?>
		<tr>
			<td colspan="2" class="data mega-overall-total data-text">OVERALL TOTAL</td>
			<?php foreach (array_keys($member_types) as $payment_type) : ?>
				<?php for($i = $min; $i <= $max; $i++): 
						$key_item = variable_get($i . "-amounts-list-for-donations", array());
						
						#skip adding unpaid items from past eyars
						if ($i < $current_year && $payment_type == "unpaid") { continue; }
					?>

                                        <?php if ($i < 2010 && $min != $max) : 
							$overall_total_lt2010 += $overall_total_per_column["megatotal"]["breakdown"][$i]->$payment_type; continue; ?>
                                        <?php elseif ($i == 2010 && $min != $max && !$hack_merged) : $hack_merged = true;

                                                        $overall_total_lt2010 += $overall_total_per_column["megatotal"]["breakdown"][$i]->$payment_type;
                                                ?>
                                        	<td class="data"  col-name="<?php echo $i . "-" . $payment_type ?>" st-value="<?php echo $overall_total_lt2010 ?>"><span><?php echo bd_nice_number($overall_total_lt2010); ?></span></td>
					<?php elseif(in_array($payment_type, $key_item)) :#foreach (variable_get($i . "-amounts-list-for-donations", array()) as $key) : ?>
						<td class="data mega-overall-total"  col-name="<?php echo $i . "-" . $payment_type ?>" 	>
							<span><?php echo bd_nice_number($overall_total_per_column["megatotal"]["breakdown"][$i]->$payment_type); ?></span>
						</td>
					<?php endif; ?>
				<?php endfor; ?>
				<?php if ($payment_type == "amount" && $min!=$max) : ?>
					<td class="data mega-overall-total"  col-name="data-total"><span><?php echo bd_nice_number($overall_total_per_column["megatotal"]["paid"]); ?></span></td>
				<?php endif; ?>
			<?php endforeach; ?>
			
			
			
			<?php #	<td class="data mega-overall-total"  col-name="data-total"><div><?php echo bd_nice_number($overall_total_per_column["megatotal"]["pledge"]); </div></td> ?>
		</tr>
		<?php endif; ?>
	</table>
	<?php endforeach;  ?>
</div>
