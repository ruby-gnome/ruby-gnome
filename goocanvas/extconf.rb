=begin
extconf.rb for Ruby/GooCanvas extension library
=end

PACKAGE_NAME = "goocanvas"
PACKAGE_ID   = "goocanvas"

base_dir = File.expand_path(File.dirname(__FILE__))
TOPDIR = File.expand_path(base_dir + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = base_dir + '/src'

$LOAD_PATH.unshift(MKMF_GNOME2_DIR)

require 'mkmf-gnome2'

PKGConfig.have_package(PACKAGE_ID, 0, 8) or exit 1
setup_win32(PACKAGE_NAME)

check_cairo or exit 1

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("gtk2", "gtk/src", TOPDIR)

make_version_header("GOO_CANVAS", PACKAGE_NAME)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GOO_CANVAS_COMPILATION")
create_pkg_config_file(PACKAGE_NAME, PACKAGE_ID, ruby_gnome2_version)
create_top_makefile
