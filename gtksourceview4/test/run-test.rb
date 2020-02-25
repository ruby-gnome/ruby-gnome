#!/usr/bin/env ruby
#
# Copyright (C) 2008-2020  Ruby-GNOME Project Team
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
atk_base = File.join(ruby_gnome_base, "atk")
cairo_gobject_base = File.join(ruby_gnome_base, "cairo-gobject")
pango_base = File.join(ruby_gnome_base, "pango")
gdk_pixbuf_base = File.join(ruby_gnome_base, "gdk_pixbuf2")
gobject_introspection_base = File.join(ruby_gnome_base, "gobject-introspection")
gio2_base = File.join(ruby_gnome_base, "gio2")
gdk3_base = File.join(ruby_gnome_base, "gdk3")
gtk3_base = File.join(ruby_gnome_base, "gtk3")
gtk_source_view4_base = File.join(ruby_gnome_base, "gtksourceview4")

$LOAD_PATH.unshift(glib_base)
require 'test/glib-test-init'

dependencies = [
  glib_base,
  atk_base,
  cairo_gobject_base,
  pango_base,
  gdk_pixbuf_base,
  gobject_introspection_base,
  gio2_base,
  gdk3_base,
  gtk3_base,
  gtk_source_view4_base,
]
dependencies.each do |target|
  makefile = File.join(target, "Makefile")
  if File.exist?(makefile) and system("which make > /dev/null")
    `make -C #{target.dump} > /dev/null` or exit(false)
  end
  $LOAD_PATH.unshift(File.join(target, "ext", File.basename(target)))
  $LOAD_PATH.unshift(File.join(target, "lib"))
end

$LOAD_PATH.unshift(File.join(gtk3_base, "test"))
require 'gtk-test-utils'

require 'gtksourceview4'

exit Test::Unit::AutoRunner.run(true, File.join(gtk_source_view4_base, "test"))
