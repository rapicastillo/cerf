<?php foreach ($debug as $debug_key => $debug_item ) : ?>
	<h2><?php echo $debug_key ?></h2>
	<pre>
		<?php print_r($debug_item) ; ?>		
	</pre>
<?php endforeach; ?>