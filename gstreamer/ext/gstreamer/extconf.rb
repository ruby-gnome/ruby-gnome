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

module_name = "gst"
package_id = "gstreamer-0.10"

require 'mkmf-gnome2'

setup_win32(module_name, base_dir)

PKGConfig.have_package(package_id) or exit 1
PKGConfig.have_package('gstreamer-plugins-base-0.10') or exit 1

have_header("unistd.h")
have_header("io.h")

if have_library("gstinterfaces-0.10") and
    have_header("gst/interfaces/xoverlay.h")
  $CFLAGS += " -DHAVE_GST_OVERLAY"
end

if PKGConfig.have_package("gstreamer-pbutils-0.10")
  $CFLAGS += " -DHAVE_GST_PBUTILS"
end

["glib2"].each do |package|
  directory = "#{package}#{version_suffix}"
  build_dir = "#{directory}/tmp/#{RUBY_PLATFORM}/#{package}/#{RUBY_VERSION}"
  add_depend_package(package, "#{directory}/ext/#{package}",
                     top_dir.to_s,
                     :top_build_dir => top_build_dir.to_s,
                     :target_build_dir => build_dir)
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

