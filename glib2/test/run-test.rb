#!/usr/bin/env ruby
# Copyright (C) 2015  Ruby-GNOME2 Project Team
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


base = File.expand_path(File.join(File.dirname(__FILE__)))
top = File.expand_path(File.join(base, ".."))

$LOAD_PATH.unshift(top)
require 'test/glib-test-init'

if File.exist?("#{top}/Makefile") and system("which make > /dev/null")
  system("cd #{top.dump} && make > /dev/null") or exit(1)
end

$LOAD_PATH.unshift(File.join(top, "ext", "glib2"))
$LOAD_PATH.unshift(File.join(top, "lib"))

$LOAD_PATH.unshift(base)
require 'glib-test-utils'

require 'glib2'

exit Test::Unit::AutoRunner.run(true, base)
