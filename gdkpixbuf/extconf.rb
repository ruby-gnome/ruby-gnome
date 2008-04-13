=begin
extconf.rb for Ruby/GdkPixbuf extention library
=end

PACKAGE_NAME = "gdk_pixbuf2"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/gdkpixbuf'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

PKGConfig.have_package('gdk-pixbuf-2.0') or exit 1

setup_win32(PACKAGE_NAME)

have_func("gdk_pixbuf_set_option")
have_header("gdk-pixbuf/gdk-pixbuf-io.h")

if PKGConfig.have_package('cairo') and
    PKGConfig.have_package('gdk-2.0') and
    have_header('rb_cairo.h')
  if /mingw|cygwin|mswin32/ =~ RUBY_PLATFORM
    unless ENV["CAIRO_PATH"]
      puts "Error! Set CAIRO_PATH."
      exit 1
    end
    add_depend_package("cairo", "src", ENV["CAIRO_PATH"])
    $defs << "-DRUBY_CAIRO_PLATFORM_WIN32"
  end
end

add_depend_package("glib2", "glib/src", TOPDIR)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR)
