=begin
extconf.rb for Ruby/GdkPixbuf2 extention library
=end

require 'pathname'

base_dir = Pathname(__FILE__).dirname.parent.parent.expand_path
top_dir = base_dir.parent
mkmf_gnome2_dir = top_dir + "glib" + 'lib'

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
  options = {}
  rcairo_source_dir_names = ["rcairo"]
  if /mingw|cygwin|mswin32/ =~ RUBY_PLATFORM
    rcairo_source_dir_names.unshift("rcairo.win32")
  end
  rcairo_source_dir_names.each do |rcairo_source_dir_name|
    rcairo_source_dir = top_dir.parent.expand_path + rcairo_source_dir_name
    if rcairo_source_dir.exist?
      options[:rcairo_source_dir] = rcairo_source_dir.to_s
      break
    end
  end
  check_cairo(options)
end

build_dir = "glib/tmp/#{RUBY_PLATFORM}/glib2/#{RUBY_VERSION}"
options = {}
options[:target_build_dir] = build_dir if (top_dir + build_dir).exist?
add_depend_package("glib2", "glib/ext/glib2", top_dir.to_s, options)
create_pkg_config_file("Ruby/GdkPixbuf2", package_id)
create_makefile(module_name)
