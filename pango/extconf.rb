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

pango_header = "pango/pango.h"
have_func("pango_layout_iter_get_type", pango_header)
have_func("pango_layout_set_ellipsize", pango_header)
have_func("pango_layout_get_font_description", pango_header)
have_func("pango_render_part_get_type", pango_header)
have_func("pango_attr_strikethrough_color_new", pango_header)
have_func("pango_attr_underline_color_new", pango_header)
have_func("pango_glyph_item_free", pango_header)
have_func("pango_glyph_item_get_type", pango_header)
have_func("pango_attr_iterator_get_attrs", pango_header)
have_func("pango_itemize_with_base_dir", pango_header)
have_func("pango_font_family_is_monospace", pango_header)

if PKGConfig.have_package('pangocairo')
  check_cairo
end

add_depend_package("glib2", "glib/src", TOPDIR)

add_distcleanfile("rbpangoinits.c")

make_version_header("PANGO", "pango")

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_PANGO_COMPILATION") {
  SRCDIR_QUOTED = SRCDIR.gsub(' ', '\ ')
  system("#{$ruby} #{SRCDIR_QUOTED}/makeinits.rb #{SRCDIR_QUOTED}/*.c > rbpangoinits.c")
}

create_top_makefile

