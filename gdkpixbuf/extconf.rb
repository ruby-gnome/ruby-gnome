$LOAD_PATH.unshift File.expand_path(File.dirname(__FILE__) + '/..')
require 'mkmf'
require 'mkmf-gnome2'

PKGConfig.have_package('gdk-pixbuf-2.0') or exit
check_win32

rubyglib_dir = File.dirname(__FILE__)+"/../glib"
$CFLAGS += " -I#{rubyglib_dir}/src "

have_func("g_print") &&
have_func("gdk_pixbuf_new") &&
create_makefile('gdk_pixbuf2')
