=begin
extconf.rb for Ruby/RSVG extention library
=end

require 'pathname'

base_dir = Pathname(__FILE__).dirname.parent.parent.expand_path
top_dir = base_dir.parent
mkmf_gnome2_dir = top_dir + "glib" + 'lib'
top_build_dir = Pathname(".").parent.parent.parent.expand_path

$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s)

module_name = "rsvg2"
package_id = "librsvg-2.0"

require 'mkmf-gnome2'

setup_win32(module_name, base_dir)

PKGConfig.have_package(package_id) or exit 1

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

if have_header("librsvg/rsvg-cairo.h")
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
add_depend_package("glib2", "glib/ext/glib2", top_dir.to_s,
                   :top_build_dir => top_build_dir.to_s,
                   :target_build_dir => build_dir)

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
