=begin
extconf.rb for Ruby/GLib extention library
=end

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/glib/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

PACKAGE_NAME = "glib2"

require 'mkmf-gnome2'

PKGConfig.have_package('gobject-2.0') or exit 1

setup_win32(PACKAGE_NAME)

have_func("g_spawn_close_id")

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GLIB2_COMPILATION")

create_top_makefile

