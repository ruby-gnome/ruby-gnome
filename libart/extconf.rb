=begin
extconf.rb for Ruby/Libart extention library
=end

PACKAGE_NAME = "libart2"

TOPDIR = File.expand_path(File.dirname(__FILE__) + '/..')
MKMF_GNOME2_DIR = TOPDIR + '/glib/src/lib'
SRCDIR = TOPDIR + '/libart/src'

$LOAD_PATH.unshift MKMF_GNOME2_DIR

require 'mkmf-gnome2'

config_cmd = with_config("libart2-config", "libart2-config")

begin
  version = `#{config_cmd} --version`
  if not version.chomp.empty?
    $libs += ' ' + `#{config_cmd} --libs`.chomp
    $CFLAGS += ' ' + `#{config_cmd} --cflags`.chomp
  end
rescue
  $libs = '-L/usr/local/lib -lart_lgpl_2 -lm'
  $CFLAGS = '-I/usr/local/include/libart-2.0'
end

$libs += " -lz "
unless have_header("png.h")
  puts "libpng is not found."
  exit 1
end
unless have_header("jpeglib.h")
  puts "jpeglib is not found."
  exit 1
end
have_library("png", "png_create_write_struct")
have_library("jpeg", "jpeg_set_defaults")
have_library("art_lgpl_2", "art_affine_identity") or exit 1

setup_win32(PACKAGE_NAME)

create_makefile_at_srcdir(PACKAGE_NAME, SRCDIR, "-DRUBY_LIBART2_COMPILATION")
create_top_makefile

