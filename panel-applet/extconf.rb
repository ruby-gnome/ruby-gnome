=begin
extconf.rb for Ruby/PanelApplet extention library
=end

$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/../glib/src/lib')

require 'mkmf-gnome2'

gnome_config = with_config('gnome-config', 'gnome-config')
rbgtk_dir = with_config("ruby-gtk", File.dirname(__FILE__)+"/../gtk")

unless FileTest.exist?(rbgtk_dir)
  raise "directry #{rbgtk_dir} not found.  Please specify Ruby-GTK source dir."
end

rbgnome_dir = with_config("ruby-gnome", File.dirname(__FILE__)+"/../gnome")
unless FileTest.exist?(rbgnome_dir)
  raise "directry #{rbgnome_dir} not found.  Please specify Ruby-GNOME source dir."
end

PKGConfig.have_package("libpanelapplet-2.0") or exit
check_win32

$CFLAGS += " -I#{rbgtk_dir}/src -I#{rbgnome_dir}/src "

have_library("X11", "XOpenDisplay") &&
have_library("Xi", "XOpenDevice") &&
have_library("Xext", "XextFindDisplay") &&
have_library("Xmu", "XmuInternAtom") &&
have_func("g_print") &&
have_func("gtk_init") &&
have_func("applet_widget_init") &&
create_makefile('panel_applet')
