# Copyright (C) 2015-2021  Ruby-GNOME Project Team
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

class TestGLibTimeout < Test::Unit::TestCase
  include GLibTestUtils

  def setup
    @id = nil
  end

  def teardown
    GLib::Source.remove(@id) if @id
  end

  def test_timeout_add_priority
    priority = GLib::PRIORITY_HIGH
    @id = GLib::Timeout.add(10, priority)
    source = GLib::MainContext.default.find_source(@id)
    assert_equal(priority, source.priority)
  end

  def test_timeout_add_seconds_priority
    priority = GLib::PRIORITY_HIGH
    @id = GLib::Timeout.add_seconds(10, priority)
    source = GLib::MainContext.default.find_source(@id)
    assert_equal(priority, source.priority)
  end
end
