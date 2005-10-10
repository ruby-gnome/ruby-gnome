=begin
extconf.rb for Ruby/PanelApplet extention library
=end

PACKAGE_NAME = "panelapplet2"
PACKAGE_ID   = "libpanelapplet-2.0"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/panel-applet'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

PKGConfig.have_package(PACKAGE_ID, 2, 6) or exit 1
setup_win32(PACKAGE_NAME)

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("gtk2", "gtk/src", TOPDIR)

make_version_header("PANELAPPLET", PACKAGE_ID, ".")

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_PANELAPPLET_COMPILATION")

