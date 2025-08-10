# Copyright (C) 2025  Ruby-GNOME Project Team
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

require "gsk4"

module GskTestUtils
  private
  def only_version(major, minor, micro=nil)
    micro ||= 0
    gsk_version_raw = PKGConfig.modversion("gsk-4.0")
    gsk_version = gsk_version_raw.split(".").collect(&:to_i)
    unless (gsk_version <=> [major, minor, micro]) >= 0
      omit("Require GSK >= #{major}.#{minor}.#{micro}")
    end
  end
end
