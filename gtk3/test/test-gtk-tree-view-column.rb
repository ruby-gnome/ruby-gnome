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

class TestGtkTreeViewColumn < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case(".new") do
    test "empty" do
      column = Gtk::TreeViewColumn.new
      assert_equal("", column.title)
    end

    test "title only" do
      column = Gtk::TreeViewColumn.new("title")
      assert_equal("title", column.title)
    end

    sub_test_case("attributes") do
      test "string key" do
        column = Gtk::TreeViewColumn.new
        cell = Gtk::CellRendererText.new
        column.pack_start(cell, true)
        column.add_attribute(cell, "text", 0)
      end

      test "symbol key" do
        column = Gtk::TreeViewColumn.new
        cell = Gtk::CellRendererText.new
        column.pack_start(cell, true)
        column.add_attribute(cell, :text, 0)
      end
    end
  end
end
