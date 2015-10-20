# Copyright (C) 2015  Ruby-GNOME2 Project Team
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

class TestGtkBox < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case ".new" do
    sub_test_case "spacing" do
      def test_nil
        box = Gtk::Box.new(:vertical, nil)
        assert_equal(0, box.spacing)
      end
    end
  end

  test "set_child_packing" do
    box = Gtk::Box.new(:vertical)
    child = Gtk::EventBox.new
    box.add(child)
    box.set_child_packing(child,
                          :expand => false,
                          :fill   => true,
                          :padding => 100,
                          :pack_type => :end)
    assert_equal([false, true, 100, Gtk::PackType::END],
                 box.query_child_packing(child))
  end
end
