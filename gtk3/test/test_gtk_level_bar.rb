# Copyright (C) 2014 Ruby-GNOME2 Project Team
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

class TestGtkLevelBar < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    only_gtk_version(3, 6, 0)
    @level_bar = Gtk::LevelBar.new
  end

  def test_for_interval
    @level_bar.for_interval(0.0, 20.0)
    assert_equal(0.0, @level_bar.min_value)
    assert_equal(20.0, @level_bar.max_value)
  end

  def test_min_value_accessors
    @level_bar.set_min_value(1.0)
    assert_equal(1.0, @level_bar.min_value)
  end

  def test_max_value_accessors
    @level_bar.set_max_value(20.0)
    assert_equal(20.0, @level_bar.max_value)
  end

  def test_value_accessors
    @level_bar.set_value(1.0)
    assert_equal(1.0, @level_bar.value)
  end
end
