require 'mkmf'

rbgtk_dir = "../gtk"
rbgtk_dir = ARGV[0] if ARGV[0]
unless FileTest.exist?(rbgtk_dir)
  raise "directry #{rbgtk_dir} not found.  Please specify Ruby-GTK source dir."
end

rbgnome_dir = "../gnome"
rbgnome_dir = ARGV[1] if ARGV[1]
unless FileTest.exist?(rbgnome_dir)
  raise "directry #{rbgnome_dir} not found.  Please specify Ruby-GNOME source dir."
end

$CFLAGS = "-I#{rbgtk_dir}/src -I#{rbgnome_dir}/src " + `gnome-config --cflags applets`.chomp
$LDFLAGS = `gnome-config --libs applets`.chomp

have_library("X11", "XOpenDisplay") &&
have_library("Xi", "XOpenDevice") &&
have_library("Xext", "XextFindDisplay") &&
have_library("Xmu", "XmuInternAtom") &&
have_library("glib", "g_print") &&
have_library("gtk", "gtk_init") &&
have_library("panel_applet", "applet_widget_init") &&

create_makefile('panel_applet')
