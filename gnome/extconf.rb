=begin
extconf.rb for Ruby/GNOME extention library
=end

PACKAGE_NAME = "gnome2"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gnome/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

PKGConfig.have_package('libgnomeui-2.0') or exit 1
setup_win32(PACKAGE_NAME)

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("gtk2", "gtk/src", TOPDIR)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GNOME2_COMPILATION") {
  gnome_version = PKGConfig.modversion('libgnomeui-2.0')

  File.delete("rbgnomeversion.h") if FileTest.exist?("rbgnomeversion.h")
  system("ruby #{SRCDIR}/makeversion.rb #{gnome_version} > rbgnomeversion.h")
}

create_top_makefile
