#!/usr/bin/env ruby
#
# Copyright (C) 2019-2021  Ruby-GNOME2 Project Team
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

require_relative "../../glib2/test/run-test"

run_test(__dir__,
         [
           "glib2",
           "gobject-introspection",
           "gio2",
           "gdk_pixbuf2",
           "atk",
           "cairo-gobject",
           "gdk3",
           "gtk3",
           "wnck3",
         ]) do
  begin
    require "wnck3"
  rescue GObjectIntrospection::RepositoryError
    puts("Omit because typelib file doesn't exist: #{$!.message}")
    exit(true)
  end

  require "gtk3"

  Gtk.init

  require_relative "helper"
end
