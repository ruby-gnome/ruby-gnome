=begin
extconf.rb for Ruby/GnomeCanvas extention library
=end

PACKAGE_NAME = "gnomecanvas2"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gnomecanvas/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

#
# detect GTK+ configurations
#

PKGConfig.have_package('libgnomecanvas-2.0') or exit 1
setup_win32(PACKAGE_NAME)

have_func('gnome_canvas_set_center_scroll_region')

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("gtk2", "gtk/src", TOPDIR)
add_depend_package("libart2", "libart/src", TOPDIR)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GNOMECANVAS2_COMPILATION") {
  gnomecanvas_version = PKGConfig.modversion('libgnomecanvas-2.0')
  version_h = "rbgnomecanvasversion.h"
  File.delete(version_h) if FileTest.exist?(version_h)
  system("ruby #{SRCDIR}/makeversion.rb #{gnomecanvas_version} > #{version_h}")
}

create_top_makefile
