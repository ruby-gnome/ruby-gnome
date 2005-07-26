=begin
extconf.rb for Ruby/PanelApplet extention library
=end

PACKAGE_NAME = "panelapplet2"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/panel-applet'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

PKGConfig.have_package('libpanelapplet-2.0', 2, 4) or exit 1
setup_win32(PACKAGE_NAME)

have_header("panel-applet-enums.h")

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("gtk2", "gtk/src", TOPDIR)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_PANELAPPLET_COMPILATION")

