=begin
extconf.rb for Ruby/GConf extention library
=end


PACKAGE_NAME = "gnomevfs"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gnomevfs/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

PKGConfig.have_package('gnome-vfs-2.0') or exit 1
PKGConfig.have_package('gnome-vfs-module-2.0') or exit 1
setup_win32(PACKAGE_NAME)

add_depend_package("glib2", "glib/src", TOPDIR)
create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GNOMEVFS_COMPILATION")
create_top_makefile
