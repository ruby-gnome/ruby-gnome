#!/usr/bin/env ruby
#
# Copyright (C) 2013-2017  Ruby-GNOME2 Project Team
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
gobject_introspection_base = File.join(ruby_gnome2_base, "gobject-introspection")
atk_base = File.join(ruby_gnome2_base, "atk")
cairo_gobject_base = File.join(ruby_gnome2_base, "cairo-gobject")
pango_base = File.join(ruby_gnome2_base, "pango")
gdk_pixbuf_base = File.join(ruby_gnome2_base, "gdk_pixbuf2")
gio2_base = File.join(ruby_gnome2_base, "gio2")
gdk3_base = File.join(ruby_gnome2_base, "gdk3")
gtk3_base = File.join(ruby_gnome2_base, "gtk3")

[
  [glib_base, "glib2"],
  [gobject_introspection_base, "gobject-introspection"],
  [atk_base, "atk"],
  [cairo_gobject_base, "cairo-gobject"],
  [pango_base, "pango"],
  [gdk_pixbuf_base, "gdk_pixbuf2"],
  [gio2_base, "gio2"],
  [gdk3_base, "gdk3"],
  [gtk3_base, "gtk3"]
].each do |target, module_name|
  if File.exist?(File.join(target, "Makefile"))
    if system("which make > /dev/null")
      `make -C #{target.dump} > /dev/null` or exit(false)
    end
    $LOAD_PATH.unshift(File.join(target, "ext", module_name))
  end
  $LOAD_PATH.unshift(File.join(target, "lib"))
end

Dir.chdir(File.join(gtk3_base, "test", "fixture")) do
  system("rake") or exit(false)
end

$LOAD_PATH.unshift(File.join(glib_base, "test"))
require 'glib-test-init'

$LOAD_PATH.unshift(File.join(gtk3_base, "test"))
require 'gtk-test-utils'

require 'gtk3'

Gtk.init

exit Test::Unit::AutoRunner.run(true, File.join(gtk3_base, "test"))
