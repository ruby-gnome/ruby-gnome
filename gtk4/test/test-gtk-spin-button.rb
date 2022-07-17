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

class TestGtkSpinButton < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case "#initialize" do
    def test_adjustment
      adjustment = Gtk::Adjustment.new(0, 0, 10, 1, 2, 4)
      button = Gtk::SpinButton.new(adjustment)
      assert_equal(adjustment, button.adjustment)
    end

    def test_range
      min  = 0
      max  = 100
      step = 10
      button = Gtk::SpinButton.new(min, max, step)
      adjustment = button.adjustment
      assert_equal([
                     min,
                     max,
                     step,
                   ],
                   [
                     adjustment.lower,
                     adjustment.upper,
                     adjustment.step_increment,
                   ])
    end
  end
end
