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

require_relative "../../glib2/test/run-test"

run_test(__dir__,
         [
           "glib2",
           "gobject-introspection",
           "cairo-gobject",
           "pango",
           "gdk_pixbuf2",
           "clutter",
           "gstreamer",
           "clutter-gstreamer",
         ]) do
  require_relative "../../gobject-introspection/test/gobject-introspection-test-utils"
  require_relative "../../clutter/test/clutter-test-utils"
  require_relative "clutter-gstreamer-test-utils"

  repository = GObjectIntrospection::Repository.default
  begin
    repository.require(ClutterGst::Loader::NAMESPACE)
  rescue GObjectIntrospection::RepositoryError
    puts("Omit because typelib file doesn't exist: #{$!.message}")
    exit(true)
  end

  begin
    Clutter.init
  rescue Clutter::InitError
    puts("Omit because initialization is failed: #{$!.message}")
    exit(true)
  end
end
