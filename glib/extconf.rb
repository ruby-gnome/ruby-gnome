=begin
extconf.rb for Ruby/GLib extention library
=end

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/../glib/src/lib')

PACKAGE_NAME = "glib2"

require 'mkmf-gnome2'

PKGConfig.have_package('gobject-2.0') or exit 1

setup_win32(PACKAGE_NAME)

have_func("g_spawn_close_id")

srcdir =  File.expand_path(File.dirname($0) + "/src")

Dir.mkdir('src') unless File.exist? 'src'
Dir.chdir "src"
begin
  $defs << "-DRUBY_GLIB2_COMPILATION"
  create_makefile(PACKAGE_NAME, srcdir)
ensure
  Dir.chdir('..')
end

create_top_makefile

