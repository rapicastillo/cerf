<?php
	error_reporting(E_ALL);
	ini_set('display_errors', TRUE);
	ini_set('display_startup_errors', TRUE);
//	date_default_timezone_set('Europe/London');
	
	define('EOL',(PHP_SAPI == 'cli') ? PHP_EOL : '<br />');

	require_once './Classes/PHPExcel.php';
	
	$objPHPExcel = new PHPExcel();
	
	// echo date('H:i:s') , " Set document properties" , EOL;
	$objPHPExcel->getProperties()->setCreator("United Nations CERF")
							 ->setLastModifiedBy("UN CERF")
							 ->setTitle("UN CERF Donations Listing")
							 ->setSubject("UN CERF Donations Listing")
							 ->setDescription("List of all donations given by member states and private donors")
							 ->setKeywords("UN CERF Donations")
							 ->setCategory("UN CERF Donations");
							 	
	
	#connect database
	$db_link = new PDO('mysql:host=localhost;dbname=cerf', 'root','');
	$stmt_members = $db_link->prepare("
				SELECT DISTINCT field_member_value FROM content_type_donors ;
			");
	$stmt_members->execute();
	$member_vals = array();
	while ($o = $stmt_members->fetch())
	{
		$member_vals[] = $o["field_member_value"];
	}
	
	$title_col = 0;
	$row = 2;
	
	$member_keys = array("Private Donors", "Member States", "Regional Members", "");
	foreach ($member_vals as $member_value)
	{
		//Set group name
		$objPHPExcel->getActiveSheet()->setCellValueByColumnAndRow($title_col, $row++, $member_keys[$member_value]);
			
		$stmt = $db_link->prepare("
				SELECT  
						n2.nid AS `donor_id`,
						n2.title AS `donor`, 
						IFNULL(donations.field_amount_value, 0) AS `Contribution`, 
						IFNULL(donations.field_verbal_amount_value, 0) AS `Verbal Pledge`,
						IFNULL(donations.field_written_amount_value, 0) AS `Written Pledge`,
						IFNULL(donations.field_unpaid_amount_value, 0) AS `Unpaid`,
						field_year_value AS year
				FROM node n
				JOIN content_type_donations donations ON n.nid = donations.nid
				JOIN content_type_donors donors ON donations.field_donor_nid_nid = donors.nid
				JOIN node n2 ON donors.nid = n2.nid   
				WHERE n.type LIKE 'donations'
				AND donors.field_member_value=" . $member_value . "
				ORDER BY donor, year
				");
		$stmt->execute();
		
		// $objPHPExcel->setActiveSheetIndex(0)
	            // ->setCellValue('A1', 'Hello')
	            // ->setCellValue('B2', 'world!')
	            // ->setCellValue('C1', 'Hello')
	            // ->setCellValue('D2', 'world!');
		
		//$row = 2; //1-based index
		$current_donor = 0;
		//$keys = array("Contribution" => 0, "Verbal Pledge" => 1, "Written Pledge" => 2, "Unpaid" => 3);
		$keys = array("Contribution" => 0);
		
		//Hardcoded
		$min = (int) date("Y");
		$start_of_vals = 1;
		
		//End of hardcoded
		
		$years = array();
		while($o = $stmt->fetch())
		{
			$test_year = (int) $o["year"];
			if ($test_year < $min)
			{
				$min = $test_year;
			}
			
			if (!in_array($o, $years))
			{
				$years[] = $o["year"];
			}
			
			if ($current_donor != $o['donor_id'])
			{
				$current_donor = $o['donor_id'];
				$row++;
				
				//Set title
				$objPHPExcel->getActiveSheet()->setCellValueByColumnAndRow($title_col, $row, $o['donor']);
			}
			
			$year = $o['year'];
			foreach($keys as $column_name => $column_order)
			{
				$target_column = $start_of_vals + (($year-$min) * count($keys)) + $column_order;
				$objPHPExcel->getActiveSheet()->setCellValueByColumnAndRow($target_column, $row, $o[$column_name]);
			}
		}
		
		//Set Years
		foreach($years as $year)
		{
			
			$year = (int) $year;
			$target_column = $start_of_vals + (($year-$min) * count($keys));
			
			$objPHPExcel->getActiveSheet()->setCellValueByColumnAndRow($target_column, 1, $year);
			$start = PHPExcel_Cell::stringFromColumnIndex($target_column);
			$end = PHPExcel_Cell::stringFromColumnIndex($target_column + count($keys) - 1);
			
			foreach($keys as $column_name => $column_order)
			{
				$objPHPExcel->getActiveSheet()->setCellValueByColumnAndRow($target_column+$column_order, 2, $column_name);
			}
			
			$merge=$start."1:$end" . "1";
			$objPHPExcel->getActiveSheet()->mergeCells($merge);
		}
		
		$row+=2;
	}
	
	$date =  date("yMdHms") ;
	$objWriter = PHPExcel_IOFactory::createWriter($objPHPExcel, 'Excel2007');
	$filename = "/tmp/" . $date. "_CERF_Data.xlsx";
	$objWriter->save($filename);
	
	
	
	if (file_exists($filename)) {
        header($_SERVER["SERVER_PROTOCOL"] . " 200 OK");
        header("Cache-Control: public"); // needed for i.e.
        header("Content-Type: application/vnd.ms-excel");
        header("Content-Length:".filesize($filename));
        header("Content-Disposition: attachment; filename='" . $date. "_CERF_Data.xlsx'");
        readfile($filename);
        die();        
    } else {
        die("Error: File not found.");
    } 
	
