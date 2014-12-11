// $Id: README.txt,v 1.4 2010/04/05 03:42:35 winston Exp $

Media: Smugmug

Initial Development by Peter Dowling

This adds support for:
- Smugmug Image provider
- Smugmug Video provider
- Smugmug Gallery provider (acts as video)

Smugmug is a paid image/video hosting service (http://smugmug.com).

See INSTALL.txt for installation instructions.

Usage
-----
1. You will need to apply for and get an api key/secret from smugmug at:
http://www.smugmug.com/hack/apikeys

2. Once you have that you need to go to "Administer", "Content Management",
"Embedded Media Field Configuration".  Expand the appropriate option. 
Enter the api key and secret in the indicated location.  Also, make sure
you have enabled the smugmug provider for the options you want to allow
to be embedded in your site.  The smugmug emfield provider supports:
 - smugmug images
 - smugmug video
 - smugmug gallery (which will show up in the video section)

You may now use in the same way other embedded media fields.  In particular,
note that when using galleries it is enough to select a link to ANY photo
in the gallery.

Notes and Special Options
-------------------------
1. Tastes Good With...
Smugmug can take advantage of other modules if installed.
For example if you install the flowplayer module, then smugmug video will
be displayed with flowplayer rather than the default player provided by 
smugmug oembed.

2. Smugmug Gallery
Smugmug exposes many options for how the gallery may be displayed!
You may change these conveniently on the "Embedded Media Field Configuration"
page.  Just expand the smugmug gallery settings, then expand the "embedded
video player options".
For those who may wish to do custom theming, the meaning of the various options
can be found here:
http://wiki.smugmug.net/display/SmugMug/Flash+Slideshow
