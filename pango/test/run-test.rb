#!/usr/bin/env ruby
#
# Copyright (C) 2017-2021  Ruby-GNOME Project Team
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

$VERBOSE = true

ruby_gnome_base = File.join(File.dirname(__FILE__), "..", "..")
ruby_gnome_base = File.expand_path(ruby_gnome_base)

ruby_gnome_build_base = ENV["RUBY_GNOME_BUILD_DIR"] || ruby_gnome_base

glib_dir = "glib2"
gobject_introspection_dir = "gobject-introspection"
cairo_gobject_dir = "cairo-gobject"
pango_dir = "pango"

$LOAD_PATH.unshift(File.join(ruby_gnome_base, glib_dir, "test"))

modules = [
  glib_dir,
  gobject_introspection_dir,
  cairo_gobject_dir,
  pango_dir,
]
modules.each do |module_dir|
  source_dir = File.join(ruby_gnome_base, module_dir)
  build_dir = File.join(ruby_gnome_build_base, module_dir)
  makefile = File.join(build_dir, "Makefile")
  if File.exist?(makefile) and system("which make > /dev/null")
    `make -C #{build_dir.dump} > /dev/null` or exit(false)
     $LOAD_PATH.unshift(File.join(build_dir, "ext", module_dir))
  end
  $LOAD_PATH.unshift(File.join(source_dir, "lib"))
end

$LOAD_PATH.unshift(File.join(ruby_gnome_base, glib_dir, "test"))
require "glib-test-init"

$LOAD_PATH.unshift(File.join(ruby_gnome_base, pango_dir, "test"))
require "pango-test-utils"

require "pango"

test_dir = File.join(ruby_gnome_base, pango_dir, "test")
exit(Test::Unit::AutoRunner.run(true, test_dir))
