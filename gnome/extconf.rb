=begin
extconf.rb for Ruby/GNOME extention library
=end

PACKAGE_NAME = "gnome2"
PACKAGE_ID   = "libgnomeui-2.0"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gnome/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

PKGConfig.have_package(PACKAGE_ID) or exit 1
setup_win32(PACKAGE_NAME)

add_depend_package("glib2", "glib/src", TOPDIR)
add_depend_package("gtk2", "gtk/src", TOPDIR)

make_version_header("GNOME", PACKAGE_ID)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GNOME2_COMPILATION")

create_pkg_config_file('GNOME', 'src/rbgnomeversion.h', 'libgnome-2.0-ruby.pc')
create_top_makefile
