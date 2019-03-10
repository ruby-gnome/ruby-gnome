#!/usr/bin/env ruby
#
# Copyright (C) 2010-2019  Ruby-GNOME2 Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

require "pathname"

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

module_name = "gtk2"
package_id = "gtk+-2.0"

require "mkmf-gnome2"

have_func("rb_errinfo")

[
  "glib2",
  "atk",
  "pango",
  "gdk_pixbuf2",
  "gobject-introspection",
].each do |package|
  directory = "#{package}#{version_suffix}"
  build_base_path = "#{directory}/tmp/#{RUBY_PLATFORM}"
  package_library_name = package.gsub(/-/, "_")
  build_dir = "#{build_base_path}/#{package_library_name}/#{RUBY_VERSION}"
  add_depend_package(package, "#{directory}/ext/#{package}",
                     top_dir.to_s,
                     :top_build_dir => top_build_dir.to_s,
                     :target_build_dir => build_dir)
end

unless required_pkg_config_package([package_id, 2, 10, 0],
                                   :alt_linux => "libgtk+2-devel",
                                   :debian => "libgtk2.0-dev",
                                   :redhat => "gtk2-devel",
                                   :arch_linux => "gtk2",
                                   :homebrew => "gtk+",
                                   :macports => "gtk2",
                                   :msys2 => "gtk2")
  exit(false)
end

unless check_cairo(:top_dir => top_dir)
  exit(false)
end

PKGConfig.have_package("gobject-introspection-1.0")

have_header("st.h")
have_header("ruby/st.h")

STDOUT.print("checking for target... ")
STDOUT.flush
target = PKGConfig.variable(package_id, "target")
$defs << "-DRUBY_GTK2_TARGET=\\\"#{target}\\\""
STDOUT.print(target, "\n")

gdk_include_path = nil
include_paths =
  $CFLAGS.gsub(/-D\w+/, '').split(/-I/) +
  $INCFLAGS.split(/-I/) +
  ['/usr/include']
include_paths.each do |path|
  path.strip!
  next if path.empty?

  if FileTest.exist?("#{path}/gdk/gdkkeysyms.h")
    gdk_include_path = Pathname("#{path}/gdk")
    break
  end
end
raise "can't find gdkkeysyms.h" if gdk_include_path.nil?

gtk_header = "gtk/gtk.h"
have_func('gtk_plug_get_type', gtk_header)
have_func('gtk_socket_get_type', gtk_header)
have_func('pango_render_part_get_type', gtk_header)
have_header('gtk/gtkfilesystem.h') do |src|
  "#define GTK_FILE_SYSTEM_ENABLE_UNSUPPORTED\n#{src}"
end

xlib_header = "X11/Xlib.h"
if target == "x11" and have_header('X11/Xlib.h') and have_library("X11")
  have_func("XReadBitmapFileData", xlib_header)
  have_func("XGetErrorText", xlib_header)
end

if target != "win32" and PKGConfig.have_package('gtk+-unix-print-2.0')
  $defs.push("-DHAVE_GTK_UNIX_PRINT")
end

create_pkg_config_file("Ruby/GTK2", package_id, ruby_gnome2_version)

rbgdkkeysyms_h_path = Pathname("rbgdkkeysyms.h")
gdkkeysyms_h_paths = []
gdkkeysyms_h_paths << gdk_include_path + "gdkkeysyms.h"
gdkkeysyms_h_paths << gdk_include_path + "gdkkeysyms-compat.h"
rbgdkkeysyms_h_path.open("w") do |rbgdkkeysyms_h|
  gdkkeysyms_h_paths.each do |path|
    next unless path.exist?
    path.each_line do |line|
      if /^#define\s+(GDK_\w+)\s+\d+/ =~ line
        define_line = "rb_define_const(mGdkKeyval, \"#{$1}\", INT2FIX(#{$1}));"
        rbgdkkeysyms_h.puts(define_line)
      end
    end
  end
end

add_distcleanfile("rbgdkkeysyms.h")

ensure_objs

$defs << "-DRUBY_GTK2_COMPILATION"
create_makefile(module_name)

pkg_config_dir = with_config("pkg-config-dir")
if pkg_config_dir.is_a?(String)
  File.open("Makefile", "ab") do |makefile|
    makefile.puts
    makefile.puts("pkgconfigdir=#{pkg_config_dir}")
  end
end
