=begin
extconf.rb for Ruby/Libglade2 extention library
=end

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/..')
require "mkmf"
require 'mkmf-gnome2'

rubyglib_dir = File.expand_path(File.dirname(__FILE__))+"/../glib"
unless FileTest.exist?(rubyglib_dir)
  raise "Directory #{rubyglib_dir} not found.  Please specify Ruby/GLib2 source dir."
end
$CFLAGS += " -I#{rubyglib_dir}/src "

rubygtk_dir = File.expand_path(File.dirname(__FILE__))+"/../gtk"
unless FileTest.exist?(rubygtk_dir)
  raise "Directory #{rubygtk_dir} not found.  Please specify Ruby/GTK2 source dir."
end
$CFLAGS += " -I#{rubygtk_dir}/src "

PKGConfig.have_package('libglade-2.0') or exit
check_win32

$CFLAGS += " -I#{rubyglib_dir}/src -II#{rubygtk_dir}/src  "

create_makefile("libglade2")  
