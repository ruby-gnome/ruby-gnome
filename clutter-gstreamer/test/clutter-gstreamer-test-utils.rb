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

require "test-unit"

module ClutterGStreamerTestUtils
  def later_version?(major, minor, micro=nil)
    micro ||= 0
    clutter_gstreamer_version = [
      ClutterGst::MAJOR_VERSION,
      ClutterGst::MINOR_VERSION,
      ClutterGst::MICRO_VERSION,
    ]
    (clutter_gstreamer_version <=> [major, minor, micro]) >= 0
  end

  def only_clutter_gstreamer_version(major, minor, micro=nil)
    micro ||= 0
    unless later_version?(major, minor, micro)
      omit("Require Clutter-GStreamer >= #{major}.#{minor}.#{micro}")
    end
  end

  def omit_on_travis_ci
    omit("This test can't be run on Travis CI") if ENV["CI"]
  end
end
