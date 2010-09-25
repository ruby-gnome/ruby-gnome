=begin
extconf.rb for Ruby/Libart2 extension library
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

module_name = "libart2"
package_id = "libart-2.0"

require 'mkmf-gnome2'

setup_win32(module_name, base_dir)

PKGConfig.have_package(package_id) or exit 1

$libs += " -lz "
PKGConfig.have_package('libpng12') or have_header("png.h") or exit 1
have_header("jpeglib.h") or exit 1

have_library("png", "png_create_write_struct")
have_library("jpeg", "jpeg_set_defaults")
have_library("art_lgpl_2", "art_affine_identity") or exit 1

create_pkg_config_file("Ruby/Libart2", package_id)
create_makefile_at_srcdir(module_name, (base_dir + "src").to_s,
                          "-DRUBY_LIBART2_COMPILATION")
create_top_makefile

