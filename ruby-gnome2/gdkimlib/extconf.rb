require 'mkmf'

rubygtk_dir = "../gtk"
rubygtk_dir = ARGV[0] if ARGV[0]
unless FileTest.exist?(rubygtk_dir)
  print "directry '#{rubygtk_dir}' not found.  Please specify Ruby/Gtk source dir.\n"
  exit
end
gtklib_dir = []
`imlib-config --libs-gdk`.split(' ').each do |e|
  if e=~ /^-L/ then
    gtklib_dir.push(e)
  end
end

$CFLAGS = "-g -I#{rubygtk_dir}/src " + `imlib-config --cflags-gdk`.chomp
$LDFLAGS = `imlib-config --libs-gdk`.chomp

have_library("X11", "XOpenDisplay") &&
have_library("Xi", "XOpenDevice") &&
have_library("Xext", "XextFindDisplay") &&
have_library("Xmu", "XmuInternAtom") &&
have_library("glib", "g_print") &&
have_library("gdk", "gdk_init") &&
have_library("gdk_imlib", "gdk_imlib_init") &&

create_makefile('gdk_imlib')
