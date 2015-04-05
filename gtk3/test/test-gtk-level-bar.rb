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
  end

  class TestConstructor < self
    def test_no_arguments
      level_bar = Gtk::LevelBar.new
      assert_equal([
                     0.0,
                     1.0,
                   ],
                   [
                     level_bar.min_value,
                     level_bar.max_value,
                   ])
    end

    def test_min_max_value
      min_value = 1.0
      max_value = 20.0
      level_bar = Gtk::LevelBar.new(min_value, max_value)
      assert_equal([
                     min_value,
                     max_value,
                   ],
                   [
                     level_bar.min_value,
                     level_bar.max_value,
                   ])
    end
  end

  class TestAccessor < self
    def setup
      super
      @level_bar = Gtk::LevelBar.new
    end

    def test_min_value
      @level_bar.min_value = 1.0
      assert_equal(1.0, @level_bar.min_value)
    end

    def test_max_value
      @level_bar.max_value = 20.0
      assert_equal(20.0, @level_bar.max_value)
    end

    def test_value
      @level_bar.value = 1.0
      assert_equal(1.0, @level_bar.value)
    end

    def test_offset_value
      @level_bar.add_offset_value(Gtk::LevelBar::OFFSET_LOW, 0.10)
      assert_equal(0.10, @level_bar.get_offset_value(Gtk::LevelBar::OFFSET_LOW))
    end

    def test_get_nonexistent_offset_value
      assert_nil(@level_bar.get_offset_value("nonexistent"))
    end

    def test_inverted
      only_gtk_version(3, 8, 0)
      @level_bar.inverted = true
      assert_true(@level_bar.inverted?)
    end
  end

  class TestEnum < self
    def test_level_bar_mode
      assert_const_defined(Gtk::LevelBar::Mode, :CONTINUOUS)
    end
  end
end
