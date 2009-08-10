=begin
extconf.rb for Ruby/GnomeCanvas extention library
=end

PACKAGE_NAME = "gnomecanvas2"
PACKAGE_ID   = "libgnomecanvas-2.0"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gnomecanvas/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

#
# detect GTK+ configurations
#

PKGConfig.have_package(PACKAGE_ID) or exit 1
setup_win32(PACKAGE_NAME)

have_func('gnome_canvas_set_center_scroll_region')
have_func('gnome_canvas_path_def_get_type')

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("gtk2", "gtk/src", TOPDIR)
add_depend_package("libart2", "libart/src", TOPDIR)

make_version_header("GNOMECANVAS", PACKAGE_ID)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GNOMECANVAS2_COMPILATION")
create_pkg_config_file(PACKAGE_NAME, PACKAGE_ID, ruby_gnome2_version)
create_top_makefile
