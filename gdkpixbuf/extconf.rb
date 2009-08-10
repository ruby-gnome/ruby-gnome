=begin
extconf.rb for Ruby/GdkPixbuf extention library
=end

PACKAGE_NAME = "gdk_pixbuf2"
PACKAGE_ID   = "gdk-pixbuf-2.0"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gdkpixbuf'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

PKGConfig.have_package(PACKAGE_ID) or exit 1

setup_win32(PACKAGE_NAME)

have_func("gdk_pixbuf_set_option", "gdk-pixbuf/gdk-pixbuf.h") do |src|
  "#define GDK_PIXBUF_ENABLE_BACKEND\n#{src}"
end
have_header("gdk-pixbuf/gdk-pixbuf-io.h")

if PKGConfig.have_package('gdk-2.0')
  check_cairo
end

add_depend_package("glib2", "glib/src", TOPDIR)
create_pkg_config_file(PACKAGE_NAME, PACKAGE_ID, ruby_gnome2_version)
create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR)

