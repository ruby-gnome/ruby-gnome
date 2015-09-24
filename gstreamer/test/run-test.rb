#!/usr/bin/env ruby
#
# Copyright (C) 2014-2015  Ruby-GNOME2 Project Team
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
gstreamer_base = File.join(ruby_gnome2_base, "gstreamer")

modules = [
  [glib_base, "glib2"],
  [gobject_introspection_base, "gobject-introspection"],
  [gstreamer_base, "gstreamer"],
]

modules.each do |target, module_name|
  makefile = File.join(target, "Makefile")
  if File.exist?(makefile) and system("which make > /dev/null")
    `make -C #{target.dump} > /dev/null` or exit(false)
  end
  $LOAD_PATH.unshift(File.join(target, "ext", module_name))
  $LOAD_PATH.unshift(File.join(target, "lib"))
end

$LOAD_PATH.unshift(File.join(glib_base, "test"))
require "glib-test-init"

$LOAD_PATH.unshift(File.join(gstreamer_base, "test"))
require "gstreamer-test-utils"

require "gst"

repository = GObjectIntrospection::Repository.default
begin
  repository.require(Gst::Loader::NAMESPACE)
rescue GLib::Error
  puts("Omit because typelib file doesn't exist: #{$!.message}")
  exit(true)
end

Gst.init

exit Test::Unit::AutoRunner.run(true, File.join(gstreamer_base, "test"))
