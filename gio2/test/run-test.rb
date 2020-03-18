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

require "fileutils"

build_dir = File.expand_path(".")

ruby_gnome_base = File.join(__dir__, "..", "..")
ruby_gnome_base = File.expand_path(ruby_gnome_base)

glib_base = File.join(ruby_gnome_base, "glib2")
gobject_introspection_base = File.join(ruby_gnome_base, "gobject-introspection")
gio2_base = File.join(ruby_gnome_base, "gio2")

modules = [
  [glib_base, "glib2"],
  [gobject_introspection_base, "gobject-introspection"],
  [gio2_base, "gio2"],
]

modules.each do |target, module_name|
  makefile = File.join(target, "Makefile")
  if File.exist?(makefile) and system("which make > /dev/null")
    `make -C #{target.dump} > /dev/null` or exit(false)
  end
  $LOAD_PATH.unshift(File.join(target, "ext", module_name))
  $LOAD_PATH.unshift(File.join(target, "lib"))
end

source_fixture_dir = File.join(gio2_base, "test", "fixture")
build_fixture_dir = File.join(build_dir, "test", "fixture")
unless source_fixture_dir == build_fixture_dir
  FileUtils.rm_rf(build_fixture_dir)
  FileUtils.mkdir_p(File.dirname(build_fixture_dir))
  FileUtils.cp_r(source_fixture_dir, build_fixture_dir)
end
Dir.chdir(build_fixture_dir) do
  system("rake") or exit(false)
end
ENV["GSETTINGS_SCHEMA_DIR"] = File.join(build_fixture_dir, "schema", "default")

$LOAD_PATH.unshift(File.join(glib_base, "test"))
require "glib-test-init"

$LOAD_PATH.unshift(File.join(gio2_base, "test"))
require "gio2-test-utils"

require "gio2"

exit Test::Unit::AutoRunner.run(true, File.join(gio2_base, "test"))
