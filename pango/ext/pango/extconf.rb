=begin
extconf.rb for Ruby/Pango extention library
=end

require 'pathname'

base_dir = Pathname(__FILE__).dirname.parent.parent.expand_path
top_dir = base_dir.parent
top_build_dir = Pathname(".").parent.parent.parent.expand_path

mkmf_gnome2_dir = top_dir + "glib2" + 'lib'
version_suffix = ""
unless mkmf_gnome2_dir.exist?
  if /(-\d+\.\d+\.\d+)(?:\.\d+)?\z/ =~ base_dir.basename.to_s
    version_suffix = $1
    mkmf_gnome2_dir = top_dir + "glib2#{version_suffix}" + 'lib'
  end
end

$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s)

module_name = "pango"
package_id = "pango"

begin
  require 'mkmf-gnome2'
rescue LoadError
  require 'rubygems'
  gem 'glib2'
  require 'mkmf-gnome2'
end

ruby_header = "ruby.h"
have_func("rb_errinfo", ruby_header)

["glib2"].each do |package|
  directory = "#{package}#{version_suffix}"
  build_dir = "#{directory}/tmp/#{RUBY_PLATFORM}/#{package}/#{RUBY_VERSION}"
  add_depend_package(package, "#{directory}/ext/#{package}",
                     top_dir.to_s,
                     :top_build_dir => top_build_dir.to_s,
                     :target_build_dir => build_dir)
end

rcairo_options = {}
rcairo_source_dir_names = ["rcairo"]
if /mingw|cygwin|mswin32/ =~ RUBY_PLATFORM
  rcairo_source_dir_names.unshift("rcairo.win32")
end
rcairo_source_dir_names.each do |rcairo_source_dir_name|
  rcairo_source_dir = top_dir.parent.expand_path + rcairo_source_dir_name
  if rcairo_source_dir.exist?
    rcairo_options[:rcairo_source_dir] = rcairo_source_dir.to_s
    break
  end
end
check_cairo(rcairo_options)

setup_win32(module_name, base_dir)

unless required_pkg_config_package(package_id,
                                   :debian => "libpango1.0-dev",
                                   :redhat => "pango-devel",
                                   :homebrew => "pango",
                                   :macports => "pango")
  exit(false)
end
PKGConfig.have_package('pangocairo')

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

make_version_header("PANGO", "pango", ".")

create_pkg_config_file("Ruby/Pango", package_id)
$defs << " -DRUBY_PANGO_COMPILATION"
create_makefile(module_name)

pkg_config_dir = with_config("pkg-config-dir")
if pkg_config_dir.is_a?(String)
  File.open("Makefile", "ab") do |makefile|
    makefile.puts
    makefile.puts("pkgconfigdir=#{pkg_config_dir}")
  end
end
