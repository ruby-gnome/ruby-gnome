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

build_dir = File.expand_path(".")
ruby_gnome_build_base = File.join(build_dir, "..")
ruby_gnome_build_base = File.expand_path(ruby_gnome_build_base)

ruby_gnome_source_base = File.join(__dir__, "..", "..")
ruby_gnome_source_base = File.expand_path(ruby_gnome_source_base)

glib_source_base = File.join(ruby_gnome_source_base, "glib2")
gstreamer_source_base = File.join(ruby_gnome_source_base, "gstreamer")

modules = [
  "glib2",
  "gobject-introspection",
  "gstreamer",
]
modules.each do |module_name|
  makefile = File.join(ruby_gnome_build_base, module_name, "Makefile")
  if File.exist?(makefile) and system("which make > /dev/null")
    `make -C #{File.dirname(makefile)} > /dev/null` or exit(false)
  end
  $LOAD_PATH.unshift(File.join(ruby_gnome_build_base,
                               module_name,
                               "ext",
                               module_name))
  $LOAD_PATH.unshift(File.join(ruby_gnome_source_base,
                               module_name,
                               "lib"))
end

$LOAD_PATH.unshift(File.join(glib_source_base, "test"))
require "glib-test-init"

$LOAD_PATH.unshift(File.join(gstreamer_source_base, "test"))
require "gstreamer-test-utils"

require "gst"

repository = GObjectIntrospection::Repository.default
begin
  repository.require(Gst::Loader::NAMESPACE)
rescue GObjectIntrospection::RepositoryError
  puts("Omit because typelib file doesn't exist: #{$!.message}")
  exit(true)
end

Gst.init

exit(Test::Unit::AutoRunner.run(true, File.join(gstreamer_source_base, "test")))
