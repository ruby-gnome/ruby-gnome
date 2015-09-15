# Copyright (C) 2012-2013  Ruby-GNOME2 Project Team
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

require "test-unit"

module ClutterTestUtils
  private
  def only_clutter_version(major, minor, micro=nil)
    micro ||= 0
    unless Clutter.check_version?(major, minor, micro)
      omit("Require Clutter >= #{major}.#{minor}.#{micro}")
    end
  end

  def omit_if_clutter_color_hash_expect_arguments
    unless Clutter::Color.method(:hash).parameters.empty?
      omit("This test can't be run on this environment.")
    end
  end

  def omit_on_travis_ci
    omit("This test can't be run on Travis CI") if ENV["CI"]
  end
end
