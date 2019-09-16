=begin
extconf.rb for Ruby/GtkSourceView3 extension library
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

module_name = "gtksourceview3"
package_id = "gtksourceview-3.0"

begin
  require 'mkmf-gnome2'
rescue LoadError
  require 'rubygems'
  gem 'glib2'
  require 'mkmf-gnome2'
end

["glib2", "atk", "pango", "gdk_pixbuf2", "gtk3"].each do |package|
  directory = "#{package}#{version_suffix}"
  build_dir = "#{directory}/tmp/#{RUBY_PLATFORM}/#{package}/#{RUBY_VERSION}"
  add_depend_package(package, "#{directory}/ext/#{package}",
                     top_dir.to_s,
                     :top_build_dir => top_build_dir.to_s,
                     :target_build_dir => build_dir)
end

unless check_cairo(:top_dir => top_dir)
  exit(false)
end

setup_windows(module_name, base_dir)

unless required_pkg_config_package(package_id,
                                   :altlinux => "libgtksourceview3-devel",
                                   :debian => "libgtksourceview-3.0-dev",
                                   :fedora => "gtksourceview3-devel",
                                   :homebrew => "gtksourceview3",
                                   :macports => "gtksourceview3")
  exit(false)
end

make_version_header("GTKSOURCEVIEW3", package_id, ".")

create_pkg_config_file("Ruby/GtkSourceView3", package_id)
$defs << "-DRUBY_GTKSOURCEVIEW3_COMPILATION"
create_makefile(module_name)
pkg_config_dir = with_config("pkg-config-dir")
if pkg_config_dir.is_a?(String)
  File.open("Makefile", "ab") do |makefile|
    makefile.puts
    makefile.puts("pkgconfigdir=#{pkg_config_dir}")
  end
end
