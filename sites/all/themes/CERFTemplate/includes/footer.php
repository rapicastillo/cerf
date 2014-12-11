
<div id="footer-wrapper">
  <footer id="footer" role="contentinfo">
    <?php print $footer_message; ?>
    <?php if (!empty($footer)): print $footer; endif; ?>
<div id="footerlft" style="width: 100%; position: relative; text-align: center;">
        <a href="<?php print base_path();?>user" style="position: absolute; top: -20px; left: 0; width: 100px; height: 100px; padding: 0"></a>
        <a href="http://www.unocha.org/" target="_blank" style="padding-left: 0;">OCHA</a>
        <a href="<?php print base_path(); ?>contact-us" class="lftrght">Contact Us</a>
        <a href="http://www.un.org/en/" target="_blank"><img src="/cerf/sites/all/themes/CERFTemplate/images/footer_logo.png" style="width: 50px; vertical-align: middle;"/></a>
        <!--a href="http://www.un.org/en/" target="_blank">UNITED NATIONS</a -->
        <a href="<?php print base_path(); ?>sitemap" class="rghtrght" style="padding-right: 65px">Site Map</a>
        <a href="https://twitter.com/UNCERF" class="rghtrght" style="padding-right: 0px; padding-left: 0px"><img src="/cerf/sites/all/themes/CERFTemplate/images/twitter-bird-light-bgs.png" style="margin-right: -8px; height: 48px; width: auto; vertical-align: middle;">Follow CERF</a>

        <!-- a href="https://twitter.com/UNCERF" style="float:right;" target="_blank"><img src="<?php print base_path(); ?><?php print path_to_theme(); ?>/images/twittericon_sm.png" alt="Follow CERF on Twitter"/></a -->
</div>
<span id="credits"> <a href='http://www.un.org/en/aboutun/copyright/' target="_blank">Copyright</a> United Nations •
<a href="http://www.un.org/en/aboutun/terms/" target="_blank">Terms Of Use</a> •
<a href="http://www.un.org/en/aboutun/privacy/" target="_blank">Privacy Statement</a> </span>
  </footer> <!-- /footer -->
</div> <!-- /footer-wrapper -->

<?php /*
<div id="footer-wrapper">
  <footer id="footer" role="contentinfo">
    <?php print $footer_message; ?>
    <?php if (!empty($footer)): print $footer; endif; ?>
<div id="footerlft">
	<a href="<?php print base_path();?>user"></a>
	<a href="http://www.unocha.org/" target="_blank">OCHA</a>
	<a href="<?php print base_path(); ?>contact-us" class="lftrght">Contact Us</a>
</div>
<div id="footerrght">
	<a href="http://www.un.org/en/" target="_blank">UNITED NATIONS</a>
	<a href="<?php print base_path(); ?>sitemap" class="rghtrght">Site Map</a>
	<a href="https://twitter.com/UNCERF" style="float:right;" target="_blank"><img src="<?php print base_path(); ?><?php print path_to_theme(); ?>/images/twittericon_sm.png" alt="Follow CERF on Twitter"/></a>
</div>
<span id="credits"> Copyright 2012 United Nations • <a href="<?php print base_path();?>terms">Terms Of Use</a> • <a href="<?php print base_path();?>privacy">Privacy Statement</a> </span>
  </footer> <!-- /footer -->
</div> <!-- /footer-wrapper -->
*/ ?>
<?php print $closure; ?>

</div> <!-- /page -->

</body>
</html>
