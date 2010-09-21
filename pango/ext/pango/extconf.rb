=begin
extconf.rb for Ruby/Pango extention library
=end

require 'pathname'

base_dir = Pathname(__FILE__).dirname.parent.parent.expand_path
top_dir = base_dir.parent
mkmf_gnome2_dir = top_dir + "glib" + 'lib'

$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s)

module_name = "pango"
package_id = "pango"

require 'mkmf-gnome2'

setup_win32(module_name, base_dir)

PKGConfig.have_package(package_id) or exit 1

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
  options = {}
  rcairo_source_dir_names = ["rcairo"]
  if /mingw|cygwin|mswin32/ =~ RUBY_PLATFORM
    rcairo_source_dir_names.unshift("rcairo.win32")
  end
  rcairo_source_dir_names.each do |rcairo_source_dir_name|
    rcairo_source_dir = top_dir.parent.expand_path + rcairo_source_dir_name
    if rcairo_source_dir.exist?
      options[:rcairo_source_dir] = rcairo_source_dir.to_s
      break
    end
  end
  check_cairo(options)
end

build_dir = "glib/tmp/#{RUBY_PLATFORM}/glib2/#{RUBY_VERSION}"
options = {}
options[:target_build_dir] = build_dir if (top_dir + build_dir).exist?
add_depend_package("glib2", "glib/ext/glib2", top_dir.to_s, options)

add_distcleanfile("rbpangoinits.c")

make_version_header("PANGO", "pango")

create_pkg_config_file("Ruby/Pango", package_id)
$defs << " -DRUBY_PANGO_COMPILATION"
create_makefile(module_name)
