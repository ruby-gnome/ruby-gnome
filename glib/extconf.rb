=begin
extconf.rb for Ruby/GLib extention library
=end

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/../glib/src/lib')

require 'mkmf-gnome2'

PKGConfig.have_package('gobject-2.0') or exit 1
check_win32

set_output_lib('libruby-glib2.a')
have_func("g_spawn_close_id")

srcdir = File.dirname($0) == "." ? "." :
  File.expand_path(File.dirname($0) + "/src")

Dir.mkdir('src') unless File.exist? 'src'
Dir.chdir "src"
begin
  $defs << "-DRUBY_GLIB2_COMPILATION"
  create_makefile("glib2", srcdir)
ensure
  Dir.chdir('..')
end

create_top_makefile

