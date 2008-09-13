=begin
extconf.rb for Ruby/GooCanvas extension library
=end

PACKAGE_NAME = "goocanvas"

TOPDIR = File.expand_path(File.dirname(__FILE__))
SRCDIR = TOPDIR + '/src'

require 'mkmf-gnome2'

PKGConfig.have_package(PACKAGE_NAME, 0, 8) or exit 1
setup_win32(PACKAGE_NAME)

PKGConfig.have_package('cairo') or exit 1
have_header('rb_cairo.h') or exit 1

if /mingw|cygwin|mswin32/ =~ RUBY_PLATFORM
  unless ENV["CAIRO_PATH"]
    puts "Error! Set CAIRO_PATH."
    exit 1
  end
  add_depend_package("cairo", "packages/cairo/ext", ENV["CAIRO_PATH"])
  $defs << "-DRUBY_CAIRO_PLATFORM_WIN32"
end

make_version_header("GOO_CANVAS", PACKAGE_NAME)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_GOO_CANVAS_COMPILATION")

create_top_makefile
