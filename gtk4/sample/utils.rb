# Copyright (C) 2018  Ruby-GNOME2 Project Team
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

def require_gtk4
  ruby_gnome_base = File.join(__dir__, "..", "..")
  ruby_gnome_base = File.expand_path(ruby_gnome_base)

  glib_base = File.join(ruby_gnome_base, "glib2")
  gobject_introspection_base = File.join(ruby_gnome_base, "gobject-introspection")
  atk_base = File.join(ruby_gnome_base, "atk")
  cairo_gobject_base = File.join(ruby_gnome_base, "cairo-gobject")
  pango_base = File.join(ruby_gnome_base, "pango")
  gdk_pixbuf_base = File.join(ruby_gnome_base, "gdk_pixbuf2")
  gio2_base = File.join(ruby_gnome_base, "gio2")
  gdk4_base = File.join(ruby_gnome_base, "gdk4")
  gtk4_base = File.join(ruby_gnome_base, "gtk4")

  [
    [glib_base, "glib2"],
    [gobject_introspection_base, "gobject-introspection"],
    [atk_base, "atk"],
    [cairo_gobject_base, "cairo-gobject"],
    [pango_base, "pango"],
    [gdk_pixbuf_base, "gdk_pixbuf2"],
    [gio2_base, "gio2"],
    [gdk4_base, "gdk4"],
    [gtk4_base, "gtk4"]
  ].each do |target, module_name|
    if File.exist?(File.join(target, "Makefile"))
      if system("which make > /dev/null")
        `make -C #{target.dump} > /dev/null` or exit(false)
      end
      $LOAD_PATH.unshift(File.join(target, "ext", module_name))
    end
    $LOAD_PATH.unshift(File.join(target, "lib"))
  end

  require "gtk4"
end
