=begin
extconf.rb for Ruby/GConf extention library
=end

PACKAGE_NAME = "gconf2"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..') 
SRCDIR = TOPDIR + '/gconf/src'
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

PKGConfig.have_package("gconf-2.0") or exit 1
setup_win32(PACKAGE_NAME)

add_depend_package("glib2", "glib/src", TOPDIR)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GCONF2_COMPILATION")

create_top_makefile
