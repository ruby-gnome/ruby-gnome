=begin
extconf.rb for Ruby/RSVG extention library
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

module_name = "rsvg2"
package_id = "librsvg-2.0"

begin
  require 'mkmf-gnome2'
rescue LoadError
  require 'rubygems'
  gem 'glib2'
  require 'mkmf-gnome2'
end

["glib2", "gdk_pixbuf2"].each do |package|
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

unless required_pkg_config_package([package_id, 2, 16, 1],
                                   :alt_linux => "librsvg-devel",
                                   :debian => "librsvg2-dev",
                                   :redhat => "librsvg2-devel",
                                   :arch_linux => "librsvg",
                                   :homebrew => "librsvg",
                                   :macports => "librsvg")
  exit(false)
end

rsvg_header = "librsvg/rsvg.h"
have_func("rsvg_set_default_dpi", rsvg_header)
have_func("rsvg_set_default_dpi_x_y", rsvg_header)
have_func("rsvg_handle_set_dpi", rsvg_header)
have_func("rsvg_handle_set_dpi_x_y", rsvg_header)
have_func("rsvg_handle_get_metadata", rsvg_header)
have_func("rsvg_handle_free", rsvg_header)
have_func("rsvg_handle_get_pixbuf_sub", rsvg_header)
have_header("librsvg/rsvg-gz.h")
have_type("RsvgDimensionData", "librsvg/rsvg.h")

have_header("librsvg/rsvg-cairo.h")

create_pkg_config_file("Ruby/RSVG", package_id, nil, "ruby-rsvg2.pc")

$defs << " -DRUBY_RSVG2_COMPILATION"
enum_type_prefix = "librsvg-enum-types"
unless have_header("librsvg/#{enum_type_prefix}.h")
  include_paths = PKGConfig.cflags_only_I("librsvg-2.0")
  include_path = include_paths.split.find do |x|
    /librsvg/.match(x)
  end.sub(/^-I/, "")
  headers = Dir.glob(File.join(include_path, "librsvg", "*.h"))
  glib_mkenums(enum_type_prefix, headers, "RSVG_TYPE_", ["librsvg/rsvg.h"])
end
create_makefile(module_name)

pkg_config_dir = with_config("pkg-config-dir")
if pkg_config_dir.is_a?(String)
  File.open("Makefile", "ab") do |makefile|
    makefile.puts
    makefile.puts("pkgconfigdir=#{pkg_config_dir}")
  end
end
