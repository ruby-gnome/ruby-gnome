require "mkmf"

rubyglib_dir = ARGV.shift || File.dirname(__FILE__)+"/../glib"
unless FileTest.exist?(rubyglib_dir)
  raise "Directory #{rubyglib_dir} not found.  Please specify Ruby/GLib source dir."
end
$CFLAGS += "-I#{rubyglib_dir}/src "

rubygtk_dir = ARGV.shift || File.dirname(__FILE__)+"/../gtk"
unless FileTest.exist?(rubyglib_dir)
  raise "Directory #{rubygtk_dir} not found.  Please specify Ruby/Gtk source dir."
end
$CFLAGS += "-I#{rubygtk_dir}/src "

unless system('pkg-config', '--exists', 'libglade-2.0')
  STDERR.print("libglade-2.0 doesn't exist\n")
  exit
end
$libs += ' ' + `pkg-config libglade-2.0 --libs`.chomp
$CFLAGS  += ' ' + `pkg-config libglade-2.0 --cflags`.chomp
$CFLAGS += ' -g'

create_makefile("libglade2")
