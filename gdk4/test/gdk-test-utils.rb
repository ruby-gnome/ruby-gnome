# Copyright (C) 2013-2022  Ruby-GNOME Project Team
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

require "pkg-config"
require "test-unit"

require "gdk4"

module GdkTestUtils
  private
  def only_gdk_version(major, minor, micro=nil)
    micro ||= 0
    gdk_version_raw = PKGConfig.modversion("gdk-4.0")
    gdk_version = gdk_version_raw.split(".").collect(&:to_i)
    unless (gdk_version <=> [major, minor, micro]) >= 0
      omit("Require GDK >= #{major}.#{minor}.#{micro}")
    end
  end

  def fixture_path(*components)
    File.join(File.dirname(__FILE__), "fixture", *components)
  end
end
