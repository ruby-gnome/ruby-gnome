#!/usr/bin/env ruby
#
# Copyright (C) 2013-2020  Ruby-GNOME Project Team
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

ruby_gnome_base = File.join(File.dirname(__FILE__), "..", "..")
ruby_gnome_base = File.expand_path(ruby_gnome_base)

glib_base = File.join(ruby_gnome_base, "glib2")
gobject_introspection_base = File.join(ruby_gnome_base, "gobject-introspection")
gio_base = File.join(ruby_gnome_base, "gio2")
atk_base = File.join(ruby_gnome_base, "atk")
cairo_gobject_base = File.join(ruby_gnome_base, "cairo-gobject")
pango_base = File.join(ruby_gnome_base, "pango")
gdk_pixbuf_base = File.join(ruby_gnome_base, "gdk_pixbuf2")
gtk2_base = File.join(ruby_gnome_base, "gtk2")
webkit_gtk2_base = File.join(ruby_gnome_base, "webkit-gtk2")

modules = [
  [glib_base, "glib2"],
  [gobject_introspection_base, "gobject-introspection"],
  [gio_base, "gio2"],
  [atk_base, "atk"],
  [cairo_gobject_base, "cairo-gobject"],
  [pango_base, "pango"],
  [gdk_pixbuf_base, "gdk_pixbuf2"],
  [gtk2_base, "gtk2"],
  [webkit_gtk2_base, "webkit-gtk2"],
]
modules.each do |target, module_name|
  if File.exist?("Makefile") and system("which make > /dev/null")
    `make -C #{target.dump} > /dev/null` or exit(false)
  end
  $LOAD_PATH.unshift(File.join(target, "ext", module_name))
  $LOAD_PATH.unshift(File.join(target, "lib"))
end

$LOAD_PATH.unshift(File.join(glib_base, "test"))
require "glib-test-init"

$LOAD_PATH.unshift(File.join(gobject_introspection_base, "test"))
require "gobject-introspection-test-utils"

$LOAD_PATH.unshift(File.join(webkit_gtk2_base, "test"))
require "webkit-gtk-test-utils"

require "webkit-gtk2"

repository = GObjectIntrospection::Repository.default
begin
  repository.require(WebKitGtk2::Loader::NAMESPACE,
                     WebKitGtk2::Loader::VERSION)
rescue GObjectIntrospection::RepositoryError
  puts("Omit because typelib file doesn't exist: #{$!.message}")
  exit(true)
end

exit Test::Unit::AutoRunner.run(true, File.join(webkit_gtk2_base, "test"))
