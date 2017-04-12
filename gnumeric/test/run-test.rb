#!/usr/bin/env ruby
#
# Copyright (C) 2016-2017  Ruby-GNOME2 Project Team
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

ruby_gnome2_base = File.join(File.dirname(__FILE__), "..", "..")
ruby_gnome2_base = File.expand_path(ruby_gnome2_base)

glib_base = File.join(ruby_gnome2_base, "glib2")
atk_base = File.join(ruby_gnome2_base, "atk")
pango_base = File.join(ruby_gnome2_base, "pango")
gdk_pixbuf_base = File.join(ruby_gnome2_base, "gdk_pixbuf2")
cairo_gobject_base = File.join(ruby_gnome2_base, "cairo-gobject")
gobject_introspection_base = File.join(ruby_gnome2_base, "gobject-introspection")
gio2_base = File.join(ruby_gnome2_base, "gio2")
gdk3_base = File.join(ruby_gnome2_base, "gdk3")
gtk3_base = File.join(ruby_gnome2_base, "gtk3")
gsf_base = File.join(ruby_gnome2_base, "gsf")
goffice_base = File.join(ruby_gnome2_base, "goffice")
gnumeric_base = File.join(ruby_gnome2_base, "gnumeric")

modules = [
  [glib_base, "glib2"],
  [atk_base, "atk"],
  [pango_base, "pango"],
  [cairo_gobject_base, "cairo-gobject"],
  [gdk_pixbuf_base, "gdk_pixbuf2"],
  [gobject_introspection_base, "gobject-introspection"],
  [gio2_base, "gio2"],
  [gdk3_base, "gdk3"],
  [gtk3_base, "gtk3"],
  [gsf_base, "gsf"],
  [goffice_base, "goffice"],
  [gnumeric_base, "gnumeric"]
]

modules.each do |target, module_name|
  makefile = File.join(target, "Makefile")
  if File.exist?(makefile) and system("which make > /dev/null")
    `make -C #{target.dump} > /dev/null` or exit(false)
  end
  $LOAD_PATH.unshift(File.join(target, "ext", module_name))
  $LOAD_PATH.unshift(File.join(target, "lib"))
end

$LOAD_PATH.unshift(File.join(gnumeric_base, "test"))
require "gnumeric-test-utils"

require "gnumeric"
begin
  Gnm.init
rescue GObjectIntrospection::RepositoryError
  puts("Omit because typelib file doesn't exist: #{$!.message}")
  exit(true)
end

exit Test::Unit::AutoRunner.run(true, File.join(gnumeric_base, "test"))
