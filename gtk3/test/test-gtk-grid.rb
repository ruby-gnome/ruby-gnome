# Copyright (C) 2016  Ruby-GNOME2 Project Team
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

class TestGtkGrid < Test::Unit::TestCase
  include GtkTestUtils
  
  sub_test_case "attach" do
    child1 = Gtk::Label.new("1")
    child2 = Gtk::Label.new("2")
    grid = Gtk::Grid.new

    test "default options" do
      grid.attach(child1)
      assert_equal(grid.get_child_at(0, 0), child1)
    end

    test "custom options" do
      grid.attach(child2, :left => 1, :top => 1, :width => 1, :height => 1)
      assert_equal(grid.get_child_at(1, 1), child2)
    end
  end
end

