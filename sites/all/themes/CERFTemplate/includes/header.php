<!DOCTYPE html>
<html lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
<head>

  <script type="text/javascript" src="//ajax.googleapis.com/ajax/libs/jquery/1.4.4/jquery.min.js"></script>
  <script type="text/javascript">
    var $jq = jQuery.noConflict();
  </script>

<?php print $head; ?>
<title><?php print $head_title; ?></title>
<?php print $styles; ?>
<?php print $scripts; ?>

<link type="text/css" rel="stylesheet" media="all" href="/cerf/sites/all/themes/CERFTemplate/css/new-styles.css" />

<script type="text/javascript"><?php /* Needed to avoid Flash of Unstyled Content in IE */ ?> </script>
	<script type="text/javascript">
		$(document).ready(function() {
			$("#superfish ul.menu").superfish(); 
		});

		$(function() {
          		$(".donor-amount-0, donations-amount-s").parents("li.views-vTicker-item.views-vTicker-item-Slideshow_jack2").remove();

                	$(".copy-print-link").each(function(i,item) {
                          var link = $(".print-page").attr("href");
                          $(this).attr("href", link);
               		});

        	});
	</script>
<script type="text/javascript">

  var _gaq = _gaq || [];
  _gaq.push(['_setAccount', 'UA-1433955-20']);
  _gaq.push(['_trackPageview']);

  (function() {
    var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
    ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
    var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);
  })();

</script>

</head>
<body class="<?php print $body_classes; ?>" style="background: #5F90C1;">

<div id="page">

<div id="header" role="banner">
  <div id="logo-title">
    <?php if (!empty($logo)): ?>
      <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" id="logo">
        <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
      </a>
    <?php endif; ?>
  </div> <!-- /logo-title -->
<div id="headest">
  <div id="slogan-item"> A sound humanitarian investment</div>
  <?php if (!empty($search_box)): ?>
    <div id="search-box"><?php print $search_box; ?></div>
  <?php endif; ?>
  <?php if (!empty($secondary_links)): ?>
	<?php /*
      <div id="secondary" role="navigation">
		<a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" class="homebutton"><img src="<?php print base_path() . $directory .'/' ?>/images/home_house.png" alt="<?php print t('Home'); ?>" /></a>
        <?php print theme('links', $secondary_links, array('class' => 'links secondary-links')); ?>
      </div>
   	*/ ?>
    <?php endif; ?>
</div>
  <?php if (!empty($header)): ?>
    <div id="header-region">
      <?php print $header; ?>
    </div>
  <?php endif; ?>

  <?php if ($main_menu): ?>
    <p id="skip-link"><em><a href="#navigation">Skip to Navigation</a></em> &darr;</p>
  <?php endif; ?>
</div> <!-- /header -->
<div id="navigation" class="menu <?php if (!empty($primary_links)) { print "withprimary"; } ?> ">
    <?php if ($superfish_menu): ?>
      <div id="superfish" role="navigation">

  		<?php print $superfish_menu; ?>

  	  </div>
<div style="clear: both;"></div>
    <?php endif; ?>
  </div>
