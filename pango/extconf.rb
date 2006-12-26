=begin
extconf.rb for Ruby/Pango extention library
=end

PACKAGE_NAME = "pango"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/pango/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR
$LOAD_PATH << "E:\\ruby\\lib\\ruby\\1.8\\i386-mswin32"

require 'mkmf-gnome2'

PKGConfig.have_package('pango') or exit 1
setup_win32(PACKAGE_NAME)

have_func("pango_layout_iter_get_type")
have_func("pango_layout_set_ellipsize")
have_func("pango_layout_get_font_description")
have_func("pango_render_part_get_type")
have_func("pango_attr_strikethrough_color_new")
have_func("pango_attr_underline_color_new")
have_func("pango_glyph_item_free")
have_func("pango_attr_iterator_get_attrs")
have_func("pango_itemize_with_base_dir")
have_func("pango_font_family_is_monospace")

PKGConfig.have_package('pangocairo')
if have_header('rb_cairo.h')
  if /mingw|cygwin|mswin32/ =~ RUBY_PLATFORM
    unless ENV["CAIRO_PATH"]
      puts "Error! Set CAIRO_PATH."
      exit 1
    end
    add_depend_package("cairo", "packages/cairo/ext", ENV["CAIRO_PATH"])
    $defs << "-DRUBY_CAIRO_PLATFORM_WIN32"
  end
end

add_depend_package("glib2", "glib/src", TOPDIR)

add_distcleanfile("rbpangoinits.c")

make_version_header("PANGO", "pango")

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_PANGO_COMPILATION") {
  system("#{$ruby} #{SRCDIR}/makeinits.rb #{SRCDIR}/*.c > rbpangoinits.c")
}

create_top_makefile

