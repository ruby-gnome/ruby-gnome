=begin
extconf.rb for Ruby/Poppler extention library
=end

require 'pathname'

base_dir = Pathname(__FILE__).dirname.parent.parent.expand_path
top_dir = base_dir.parent
mkmf_gnome2_dir = top_dir + "glib" + 'lib'

$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s)

module_name = "poppler"
package_id = "poppler-glib"

require 'mkmf-gnome2'

setup_win32(module_name, base_dir)

PKGConfig.have_package(package_id) or exit 1

if PKGConfig.have_package('poppler-cairo')
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

[["glib", "glib2"],
 ["gtk", "gtk2"],
 ["gdkpixbuf", "gdk_pixbuf2"]].each do |directory, library_name|
  build_dir = "#{directory}/tmp/#{RUBY_PLATFORM}/#{library_name}/#{RUBY_VERSION}"
  options = {}
  options[:target_build_dir] = build_dir if (top_dir + build_dir).exist?
  add_depend_package(library_name, "#{directory}/ext/#{library_name}",
                     top_dir.to_s, options)
end

unless have_macro("POPPLER_MAJOR_VERSION", ["poppler.h"])
  make_version_header("POPPLER", package_id, ".")
end

create_pkg_config_file("Ruby/Poppler", package_id)
$defs << " -DRUBY_POPPLER_COMPILATION"
create_makefile(module_name)
