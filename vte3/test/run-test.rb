#!/usr/bin/env ruby
#
# Copyright (C) 2014-2020  Ruby-GNOME Project Team
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

ruby_gnome_build_base = ENV["RUBY_GNOME_BUILD_DIR"] || ruby_gnome_base

glib_dir = "glib2"
atk_dir = "atk"
pango_dir = "pango"
gdk_pixbuf_dir = "gdk_pixbuf2"
cairo_gobject_dir = "cairo-gobject"
gobject_introspection_dir = "gobject-introspection"
gio2_dir = "gio2"
gdk3_dir = "gdk3"
gtk3_dir = "gtk3"
vte3_dir = "vte3"

modules = [
  [glib_dir, "glib2"],
  [atk_dir, "atk"],
  [pango_dir, "pango"],
  [cairo_gobject_dir, "cairo-gobject"],
  [gdk_pixbuf_dir, "gdk_pixbuf2"],
  [gobject_introspection_dir, "gobject-introspection"],
  [gio2_dir, "gio2"],
  [gdk3_dir, "gdk3"],
  [gtk3_dir, "gtk3"],
  [vte3_dir, "vte3"],
]

modules.each do |dir, module_name|
  source_dir = File.join(ruby_gnome_base, dir)
  build_dir = File.join(ruby_gnome_build_base, dir)
  makefile = File.join(build_dir, "Makefile")
  if File.exist?(makefile) and system("which make > /dev/null")
    `make -C #{build_dir.dump} > /dev/null` or exit(false)
    $LOAD_PATH.unshift(File.join(build_dir, "ext", module_name))
  end
  $LOAD_PATH.unshift(File.join(source_dir, "lib"))
end

$LOAD_PATH.unshift(File.join(ruby_gnome_base, glib_dir, "test"))
require "glib-test-init"

$LOAD_PATH.unshift(File.join(ruby_gnome_base, vte3_dir, "test"))
require "vte3-test-utils"

require "vte3"

test_dir = File.join(ruby_gnome_base, vte3_dir, "test")
exit(Test::Unit::AutoRunner.run(true, test_dir))
