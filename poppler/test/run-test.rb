#!/usr/bin/env ruby
#
# Copyright (C) 2017  Ruby-GNOME2 Project Team
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


base_dir = File.expand_path(File.join(File.dirname(__FILE__), ".."))

have_make = system("which make > /dev/null")

glib_dir = File.expand_path(File.join(base_dir, "..", "glib2"))
gio_dir = File.expand_path(File.join(base_dir, "..", "gio2"))
cairo_gobject_dir = File.expand_path(File.join(base_dir, "..", "cairo-gobject"))
gdk_pixbuf_dir = File.expand_path(File.join(base_dir, "..", "gdk_pixbuf2"))
pango_dir = File.join(File.join(base_dir, ".." , "pango"))
gobject_introspection_dir =
  File.join(File.join(base_dir, ".." , "gobject-introspection"))

$LOAD_PATH.unshift(File.join(glib_dir, "test"))
require 'glib-test-init'

[
  [glib_dir, "glib2"],
  [gio_dir, "gio2"],
  [cairo_gobject_dir, "cairo-gobject"],
  [gdk_pixbuf_dir, "gdk_pixbuf2"],
  [pango_dir, "pango"],
  [gobject_introspection_dir, "gobject-introspection"],
  [base_dir, "poppler"],
].each do |dir, module_name|
  if File.exist?(File.join(dir, "Makefile"))
    if have_make
      system("cd #{dir.dump} && make > /dev/null") or exit(false)
    end
    $LOAD_PATH.unshift(File.join(dir, "ext", module_name))
  end
  $LOAD_PATH.unshift(File.join(dir, "lib"))
end
require "poppler"

$LOAD_PATH.unshift(File.join(base_dir, "test"))
require 'poppler-test-utils'
class Test::Unit::TestCase
  include PopplerTestUtils
end

exit Test::Unit::AutoRunner.run(true, File.join(base_dir, "test"))
