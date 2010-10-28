=begin
extconf.rb for Ruby/GtkHtml2 extention library
=end

require 'pathname'

base_dir = Pathname(__FILE__).dirname.expand_path
top_dir = base_dir.parent.expand_path
top_build_dir = Pathname(".").parent.expand_path

mkmf_gnome2_dir = top_dir + "glib2" + 'lib'
version_suffix = ""
unless mkmf_gnome2_dir.exist?
  if /(-\d+\.\d+\.\d+)\z/ =~ base_dir.basename.to_s
    version_suffix = $1
    mkmf_gnome2_dir = top_dir + "glib2#{version_suffix}" + 'lib'
  end
end

$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s)

module_name = "gtkhtml2"
package_id = "libgtkhtml-2.0"

require 'mkmf-gnome2'

setup_win32(module_name, base_dir)

PKGConfig.have_package(package_id) or exit 1

have_func('html_view_scroll_to_node')
have_func('html_box_block_get_containing_width')
have_func('html_box_embedded_new')
have_func('html_box_table_get_top_offset')
have_func('html_box_table_get_bottom_offset')
#have_func('gnome_canvas_set_center_scroll_region')

["glib2", "gtk2"].each do |package|
  directory = "#{package}#{version_suffix}"
  build_dir = "#{directory}/tmp/#{RUBY_PLATFORM}/#{package}/#{RUBY_VERSION}"
  add_depend_package(package, "#{directory}/ext/#{package}",
                     top_dir.to_s,
                     :top_build_dir => top_build_dir.to_s,
                     :target_build_dir => build_dir)
end

make_version_header("GTKHTML", package_id)

create_pkg_config_file("Ruby/GtkHtml2", package_id)
create_makefile_at_srcdir(module_name, (base_dir + "src").to_s,
                          "-DRUBY_GTKHTML2_COMPILATION")
pkg_config_dir = with_config("pkg-config-dir")
if pkg_config_dir.is_a?(String)
  File.open((base_dir + "src" + "Makefile").to_s, "ab") do |makefile|
    makefile.puts
    makefile.puts("pkgconfigdir=#{pkg_config_dir}")
  end
end
create_top_makefile
