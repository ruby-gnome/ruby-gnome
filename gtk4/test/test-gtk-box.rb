# Copyright (C) 2015-2022  Ruby-GNOME Project Team
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
    child = Gtk::Box.new(:horizontal)
    box.append(child)
    suppress_warning do
      box.set_child_packing(child,
                            expand: false,
                            fill: true,
                            padding: 100,
                            pack_type: :end)
    end
    assert_equal([
                   false,
                   Gtk::Align::END,
                   100,
                   100,
                   100,
                   100,
                 ],
                 [
                   child.vexpand?,
                   child.valign,
                   child.margin_top,
                   child.margin_bottom,
                   child.margin_start,
                   child.margin_end,
                 ])
  end
end
