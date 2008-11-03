=begin
extconf.rb for Ruby/Poppler extention library
=end

PACKAGE_NAME = "poppler"
PACKAGE_ID = "poppler-glib"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/poppler/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

PKGConfig.have_package(PACKAGE_ID, 0, 8, 0) or exit 1
setup_win32(PACKAGE_NAME)

check_cairo

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("gtk2", "gtk/src", TOPDIR)
add_depend_package("gdk_pixbuf2", "gdkpixbuf", TOPDIR)

make_version_header("POPPLER", PACKAGE_ID)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_POPPLER_COMPILATION")
create_top_makefile
