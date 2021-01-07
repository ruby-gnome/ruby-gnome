#!/usr/bin/env ruby
#
# Copyright (C) 2013-2021  Ruby-GNOME Project Team
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

require "fileutils"

build_dir = File.expand_path(".")

ruby_gnome_base = File.join(__dir__, "..", "..")
ruby_gnome_base = File.expand_path(ruby_gnome_base)

ruby_gnome_build_base = ENV["RUBY_GNOME_BUILD_DIR"] || ruby_gnome_base

glib2_dir = "glib2"
gobject_introspection_dir = "gobject-introspection"
atk_dir = "atk"
cairo_gobject_dir = "cairo-gobject"
pango_dir = "pango"
gdk_pixbuf_dir = "gdk_pixbuf2"
gio2_dir = "gio2"
gdk3_dir = "gdk3"
gtk3_dir = "gtk3"

[
  glib2_dir,
  gobject_introspection_dir,
  atk_dir,
  cairo_gobject_dir,
  pango_dir,
  gdk_pixbuf_dir,
  gio2_dir,
  gdk3_dir,
  gtk3_dir,
].each do |module_dir|
  source_dir = File.join(ruby_gnome_base, module_dir)
  build_dir = File.join(ruby_gnome_build_base, module_dir)
  makefile = File.join(build_dir, "Makefile")
  if File.exist?(makefile)
    `make -C #{build_dir.dump} > /dev/null` or exit(false)
    $LOAD_PATH.unshift(File.join(build_dir, "ext", module_dir))
  end
  $LOAD_PATH.unshift(File.join(source_dir, "lib"))
end

source_fixture_dir = File.join(ruby_gnome_base, gtk3_dir, "test", "fixture")
build_fixture_dir = File.join(ruby_gnome_build_base, gtk3_dir, "test", "fixture")
unless source_fixture_dir == build_fixture_dir
  FileUtils.rm_rf(build_fixture_dir)
  FileUtils.mkdir_p(File.dirname(build_fixture_dir))
  FileUtils.cp_r(source_fixture_dir, build_fixture_dir)
end
Dir.chdir(build_fixture_dir) do
  system("rake") or exit(false)
end

$LOAD_PATH.unshift(File.join(ruby_gnome_base, glib2_dir, "test"))
require 'glib-test-init'

$LOAD_PATH.unshift(File.join(ruby_gnome_base, gtk3_dir, "test"))
require 'gtk-test-utils'

require 'gtk3'

Gtk.init

exit Test::Unit::AutoRunner.run(true,
                                File.join(ruby_gnome_base, gtk3_dir, "test"))
