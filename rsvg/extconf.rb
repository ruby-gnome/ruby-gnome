=begin
extconf.rb for Ruby/RSVG extention library
=end

PACKAGE_NAME = "rsvg2"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/rsvg/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

PKGConfig.have_package('librsvg-2.0') or exit 1
setup_win32(PACKAGE_NAME)

have_func("rsvg_set_default_dpi")
have_func("rsvg_set_default_dpi_x_y")
have_func("rsvg_handle_set_dpi")
have_func("rsvg_handle_set_dpi_x_y")
have_func("rsvg_handle_get_metadata")
have_func("rsvg_handle_free")
have_func("rsvg_handle_get_pixbuf_sub")
have_header("librsvg/rsvg-gz.h")
have_type("RsvgDimensionData", "librsvg/rsvg.h")

if PKGConfig.have_package('cairo') and have_header('rb_cairo.h')
  if /mingw|cygwin|mswin32/ =~ RUBY_PLATFORM
    unless ENV["CAIRO_PATH"]
      puts "Error! Set CAIRO_PATH."
      exit 1
    end
    add_depend_package("cairo", "src", ENV["CAIRO_PATH"])
    $defs << "-DRUBY_CAIRO_PLATFORM_WIN32"
  end
  have_header("librsvg/rsvg-cairo.h")
end

add_depend_package("glib2", "glib/src", TOPDIR)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_RSVG2_COMPILATION") do
  enum_type_prefix = "librsvg-enum-types"
  unless have_header("librsvg/#{enum_type_prefix}.h")
    include_paths = PKGConfig.cflags_only_I("librsvg-2.0")
    include_path = include_paths.split.find do |x|
      /librsvg/.match(x)
    end.sub(/^-I/, "")
    headers = Dir.glob(File.join(include_path, "librsvg", "*.h"))
    glib_mkenums(enum_type_prefix, headers, "RSVG_TYPE_", ["librsvg/rsvg.h"])
  end
end

create_top_makefile
