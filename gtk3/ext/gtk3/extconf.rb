=begin
extconf.rb for Ruby/GTK extention library
=end

require 'pathname'

source_dir = Pathname(__FILE__).dirname
base_dir = source_dir.parent.parent.expand_path
top_dir = base_dir.parent.expand_path
top_build_dir = Pathname(".").parent.parent.parent.expand_path

mkmf_gnome2_dir = top_dir + "glib2" + 'lib'
version_suffix = ""
unless mkmf_gnome2_dir.exist?
  if /(-\d+\.\d+\.\d+)(?:\.\d+)?\z/ =~ base_dir.basename.to_s
    version_suffix = $1
    mkmf_gnome2_dir = top_dir + "glib2#{version_suffix}" + 'lib'
  end
end

$LOAD_PATH.unshift(mkmf_gnome2_dir.to_s)

module_name = "gtk3"
package_id = "gtk+-3.0"

begin
  require 'mkmf-gnome2'
rescue LoadError
  require 'rubygems'
  gem 'glib2'
  require 'mkmf-gnome2'
end

have_func("rb_errinfo")

["glib2", "cairo-gobject", "atk", "pango", "gdk_pixbuf2"].each do |package|
  depended_module_name = package.gsub(/-/, "_")
  directory = "#{package}#{version_suffix}"
  build_dir = "#{directory}/tmp/#{RUBY_PLATFORM}/#{depended_module_name}/#{RUBY_VERSION}"
  add_depend_package(package, "#{directory}/ext/#{package}",
                     top_dir.to_s,
                     :top_build_dir => top_build_dir.to_s,
                     :target_build_dir => build_dir)
end

rcairo_options = {}
rcairo_source_dir_names = ["rcairo"]
if /mingw|cygwin|mswin/ =~ RUBY_PLATFORM
  rcairo_source_dir_names.unshift("rcairo.win32")
end
rcairo_source_dir_names.each do |rcairo_source_dir_name|
  rcairo_source_dir = top_dir.parent.expand_path + rcairo_source_dir_name
  if rcairo_source_dir.exist?
    rcairo_options[:rcairo_source_dir] = rcairo_source_dir.to_s
    break
  end
end

unless check_cairo(rcairo_options)
  exit(false)
end

setup_win32(module_name, base_dir)

unless required_pkg_config_package(package_id,
                                   :debian => "libgtk-3-dev",
                                   :fedora => "gtk3-devel",
                                   :homebrew => "gtk+3",
                                   :macports => "gtk3")
  exit(false)
end

have_header("st.h")
have_header("ruby/st.h")

STDOUT.print("checking for targets... ")
STDOUT.flush
targets = PKGConfig.variable(package_id, "targets").split(/\s+/)
STDOUT.print(targets.join(" "), "\n")

gtk_header = "gtk/gtk.h"
have_func('gtk_plug_get_type', gtk_header)
have_func('gtk_socket_get_type', gtk_header)
have_func('pango_render_part_get_type', gtk_header)
have_header('gtk/gtkfilesystem.h') do |src|
  "#define GTK_FILE_SYSTEM_ENABLE_UNSUPPORTED\n#{src}"
end

xlib_header = "X11/Xlib.h"
if targets.include?("x11") and have_header('X11/Xlib.h') and have_library("X11")
  have_func("XReadBitmapFileData", xlib_header)
  have_func("XGetErrorText", xlib_header)
end

if !targets.include?("win32") and PKGConfig.have_package('gtk+-unix-print-3.0')
  $defs.push("-DHAVE_GTK_UNIX_PRINT")
end

create_pkg_config_file("Ruby/GTK3", package_id, ruby_gnome2_version)

ensure_objs

$defs << "-DRUBY_GTK3_COMPILATION"
create_makefile(module_name)

pkg_config_dir = with_config("pkg-config-dir")
if pkg_config_dir.is_a?(String)
  File.open("Makefile", "ab") do |makefile|
    makefile.puts
    makefile.puts("pkgconfigdir=#{pkg_config_dir}")
  end
end
