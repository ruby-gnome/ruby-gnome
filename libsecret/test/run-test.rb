#!/usr/bin/env ruby
#
# Copyright (C) 2019-2021  Ruby-GNOME Project Team
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
           "libsecret",
         ]) do
  begin
    require "libsecret"
  rescue GObjectIntrospection::RepositoryError
    puts("Omit because typelib file doesn't exist: #{$!.message}")
    exit(true)
  end

  require "pkg-config"
  unless PKGConfig.check_version?("libsecret-1", 0, 18, 9)
    puts("Omit because libsecret is old")
    exit(true)
  end

  require_relative "helper"
end
