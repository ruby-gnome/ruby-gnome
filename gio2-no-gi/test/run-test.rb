#!/usr/bin/env ruby
#
# Copyright (C) 2013-2014  Ruby-GNOME2 Project Team
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

modules = [
  "glib2",
  "gio2",
]

modules.each do |name|
  base = File.join(ruby_gnome2_base, name)
  makefile = File.join(base, "Makefile")
  if File.exist?(makefile) and system("which make > /dev/null")
    `make -C #{base.dump} > /dev/null` or exit(false)
  end
  $LOAD_PATH.unshift(File.join(base, "ext", name))
  $LOAD_PATH.unshift(File.join(base, "lib"))
end

$LOAD_PATH.unshift(File.join(ruby_gnome2_base, "glib2", "test"))
require "glib-test-init"
require "gio2"

# TODO: workaround
# exit Test::Unit::AutoRunner.run(true)
exit true
