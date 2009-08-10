=begin
extconf.rb for Ruby/Libart extention library
=end

PACKAGE_NAME = "libart2"
PACKAGE_ID   = "libart-2.0"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/libart/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

PKGConfig.have_package(PACKAGE_ID) or exit 1

$libs += " -lz "
PKGConfig.have_package('libpng12') or have_header("png.h") or exit 1
have_header("jpeglib.h") or exit 1

have_library("png", "png_create_write_struct")
have_library("jpeg", "jpeg_set_defaults")
have_library("art_lgpl_2", "art_affine_identity") or exit 1

setup_win32(PACKAGE_NAME)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_LIBART2_COMPILATION")
create_pkg_config_file(PACKAGE_NAME, PACKAGE_ID, ruby_gnome2_version)
create_top_makefile

