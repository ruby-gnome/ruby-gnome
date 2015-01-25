=begin
extconf.rb for Ruby/GStreamer extention library
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

module_name = "gstreamer"
package_id = "gstreamer-1.0"

begin
  require 'mkmf-gnome2'
rescue LoadError
  require 'rubygems'
  gem 'glib2'
  require 'mkmf-gnome2'
end

["glib2", "gobject-introspection"].each do |package|
  directory = "#{package}#{version_suffix}"
  build_base_path = "#{directory}/tmp/#{RUBY_PLATFORM}"
  package_library_name = package.gsub(/-/, "_")
  build_dir = "#{build_base_path}/#{package_library_name}/#{RUBY_VERSION}"
  add_depend_package(package, "#{directory}/ext/#{package}",
                     top_dir.to_s,
                     :top_build_dir => top_build_dir.to_s,
                     :target_build_dir => build_dir)
end

setup_windows(module_name, base_dir)

unless required_pkg_config_package(package_id,
                                   :debian => "libgstreamer1.0-dev",
                                   :redhat => "gstreamer1-devel",
                                   :homebrew => "gstreamer",
                                   :macports => "gstreamer")
  exit(false)
end

unless PKGConfig.have_package("gobject-introspection-1.0")
  exit(false)
end

create_pkg_config_file("Ruby/GStreamer", package_id)
$defs << "-DRUBY_GST_COMPILATION"
create_makefile(module_name)
pkg_config_dir = with_config("pkg-config-dir")
if pkg_config_dir.is_a?(String)
  File.open("Makefile", "ab") do |makefile|
    makefile.puts
    makefile.puts("pkgconfigdir=#{pkg_config_dir}")
  end
end
