=begin
extconf.rb for Ruby/GtkSourceView2 extension library
=end

require 'pathname'

base_dir = Pathname(__FILE__).dirname.parent.parent.expand_path
top_dir = base_dir.parent
top_build_dir = Pathname(".").parent.parent.parent.expand_path

mkmf_gnome2_dir = top_dir + "glib2" + 'lib'
version_suffix = ""
unless mkmf_gnome2_dir.exist?
  if /(-\d+\.\d+\.\d+)\z/ =~ base_dir.basename.to_s
    version_suffix = $1
    mkmf_gnome2_dir = top_dir + "glib2#{version_suffix}" + 'lib'
  end
end

$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s)

module_name = "gtksourceview2"
package_id = "gtksourceview-2.0"

require 'mkmf-gnome2'

["glib2", "atk", "pango", "gdk_pixbuf2", "gtk2"].each do |package|
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

PKGConfig.have_package(package_id) or exit(false)

# 2.2
have_func('gtk_source_mark_get_type', "gtksourceview/gtksourcemark.h")
have_func('gtk_source_print_compositor_get_type', "gtksourceview/gtksourceprintcompositor.h")
# 2.4
have_func('gtk_source_view_get_mark_category_background', "gtksourceview/gtksourceview.h")
have_func('gtk_source_language_manager_guess_language', "gtksourceview/gtksourcelanguagemanager.h")

make_version_header("GTKSOURCEVIEW2", package_id, ".")

create_pkg_config_file("Ruby/GtkSourceView2", package_id)
$defs << "-DRUBY_GTKSOURCEVIEW2_COMPILATION"
create_makefile(module_name)
pkg_config_dir = with_config("pkg-config-dir")
if pkg_config_dir.is_a?(String)
  File.open("Makefile", "ab") do |makefile|
    makefile.puts
    makefile.puts("pkgconfigdir=#{pkg_config_dir}")
  end
end

