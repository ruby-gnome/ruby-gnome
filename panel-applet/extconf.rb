require 'mkmf'

gnome_config = with_config('gnome-config', 'gnome-config')
rbgtk_dir = with_config("ruby-gtk", File.dirname(__FILE__)+"/../gtk")

unless FileTest.exist?(rbgtk_dir)
  raise "directry #{rbgtk_dir} not found.  Please specify Ruby-GTK source dir."
end

rbgnome_dir = with_config("ruby-gnome", File.dirname(__FILE__)+"/../gnome")
unless FileTest.exist?(rbgnome_dir)
  raise "directry #{rbgnome_dir} not found.  Please specify Ruby-GNOME source dir."
end

$CFLAGS += "-I#{rbgtk_dir}/src -I#{rbgnome_dir}/src " + `gnome-config --cflags applets`.chomp
$LDFLAGS += `gnome-config --libs applets`.chomp

have_library("X11", "XOpenDisplay") &&
have_library("Xi", "XOpenDevice") &&
have_library("Xext", "XextFindDisplay") &&
have_library("Xmu", "XmuInternAtom") &&
have_func("g_print") &&
have_func("gtk_init") &&
have_func("applet_widget_init") &&
create_makefile('panel_applet')
