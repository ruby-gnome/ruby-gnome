require 'mkmf'

imlib_config = with_config("imlib-config", "imlib-config")

while /^--/ =~ ARGV[0]
  ARGV.shift
end

rubygtk_dir = ARGV.shift || "../gtk"
unless FileTest.exist?(rubygtk_dir)
  print "directry '#{rubygtk_dir}' not found.  Please specify Ruby/Gtk source dir.\n"
  exit
end

$CFLAGS += "-g -I#{rubygtk_dir}/src " + `#{imlib_config} --cflags-gdk`.chomp
$LDFLAGS +=  `#{imlib_config} --libs-gdk`.chomp

have_library("X11", "XOpenDisplay") &&
have_library("Xi", "XOpenDevice") &&
have_library("Xext", "XextFindDisplay") &&
have_library("Xmu", "XmuInternAtom") &&
have_func("g_print") &&
have_func("gdk_init") &&
have_func("gdk_imlib_init") &&
create_makefile('gdk_imlib')
