# Copyright (C) 2012-2014  Ruby-GNOME2 Project Team
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

class TestSignalInfo < Test::Unit::TestCase
  include GObjectIntrospectionTestUtils

  def setup
    @repository = GObjectIntrospection::Repository.default
    @repository.require("Gio")
    @object_info = @repository.find("Gio", "Application")
    @info = @object_info.signals.find {|info| info.name == "startup"}
  end

  def test_flags
    require_version(1, 40, 0)
    assert_equal(GLib::SignalFlags::RUN_FIRST,
                 @info.flags)
  end

  def test_class_closure
    assert_nil(@info.class_closure)
  end

  def test_true_stops_emit?
    assert_false(@info.true_stops_emit?)
  end
end
