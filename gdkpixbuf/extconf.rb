require 'mkmf'

glib_config = with_config("glib-config", "glib-config")
gdk_pixbuf_config = with_config("gdk-pixbuf-config", "gdk-pixbuf-config")

while /^--/ =~ ARGV[0]
  ARGV.shift
end

rubygtk_dir = ARGV.shift || File.dirname(__FILE__)+"/../gtk"
unless FileTest.exist?(rubygtk_dir)
  raise "Directory #{rubygtk_dir} not found.  Please specify Ruby/Gtk source dir."
end

$CFLAGS += "-I#{rubygtk_dir}/src " + `#{glib_config} --cflags`.chomp + ' ' + `#{gdk_pixbuf_config} --cflags`.chomp
$LDFLAGS += `#{glib_config} --libs`.chomp + ' ' + `#{gdk_pixbuf_config} --libs`.chomp

have_library("X11", "XOpenDisplay") &&
have_library("Xi", "XOpenDevice") &&
have_library("Xext", "XextFindDisplay") &&
have_library("Xmu", "XmuInternAtom") &&
have_func("g_print") &&
have_func("gdk_init") &&
have_func("gdk_pixbuf_new") &&
create_makefile('gdk_pixbuf')
