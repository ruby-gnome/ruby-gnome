=begin
extconf.rb for Ruby/GdkPixbuf2 extention library
=end

require 'pathname'

base_dir = Pathname(__FILE__).dirname.parent.parent.expand_path
top_dir = base_dir.parent
mkmf_gnome2_dir = top_dir + "glib2" + 'lib'

$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s)

module_name = "gdk_pixbuf2"
package_id = "gdk-pixbuf-2.0"

require 'mkmf-gnome2'

setup_win32(module_name, base_dir)

PKGConfig.have_package(package_id) or exit 1

have_func("gdk_pixbuf_set_option", "gdk-pixbuf/gdk-pixbuf.h") do |src|
  "#define GDK_PIXBUF_ENABLE_BACKEND\n#{src}"
end
have_header("gdk-pixbuf/gdk-pixbuf-io.h")

if PKGConfig.have_package('gdk-2.0')
  check_cairo
end

add_depend_package("glib2", "glib/ext/glib2", top_dir.to_s)
create_pkg_config_file("Ruby/GdkPixbuf2", package_id)
create_makefile(module_name)
