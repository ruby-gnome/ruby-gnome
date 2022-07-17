# Copyright (C) 2015-2016  Ruby-GNOME2 Project Team
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

    test "with renderer" do
      renderer = Gtk::CellRendererText.new
      column = Gtk::TreeViewColumn.new("title", renderer)
      assert_equal(1, column.cells.length)
      assert_equal(Gtk::CellRendererText, column.cells.first.class)
    end

    sub_test_case("option hash") do
      test "box" do
        box = Gtk::CellAreaBox.new()
        options = {:area => box}
        column = Gtk::TreeViewColumn.new(options)
        renderer = Gtk::CellRendererText.new
        box.pack_start renderer, true, true, true
        renderer = Gtk::CellRendererText.new
        box.pack_start renderer, true, true, true
        assert_equal(2, column.cells.length)
      end

      test "title" do
        options = {:title => 'title'}
        column = Gtk::TreeViewColumn.new(options)
        assert_equal("title", column.title)
      end

      test "renderer" do
        renderer = Gtk::CellRendererText.new
        options = {:renderer => renderer}
        column = Gtk::TreeViewColumn.new(options)
        assert_equal(1, column.cells.length)
        assert_equal(Gtk::CellRendererText, column.cells.first.class)
      end

      test "unknown" do
        options = {:unknown => true}
        message = "unknown option(s): [:unknown]: "
        message << "available options: [:area, :renderer, :title, :attributes]"
        assert_raise(ArgumentError.new(message)) do
          Gtk::TreeViewColumn.new(options)
        end
      end
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

  sub_test_case("instance methods") do
    def setup
      @column = Gtk::TreeViewColumn.new
      @cell = Gtk::CellRendererText.new
      @column.pack_start(@cell, true)
      @column.add_attribute(@cell, :text, 0)
    end

    test "#set_cell_data_func" do
      values = []
      @column.set_cell_data_func(@cell) do |column, cell, model, iter|
        values << iter[0]
      end

      model = Gtk::TreeStore.new(String)
      parent = model.append(nil)
      parent[0] = "Hello"
      child = model.append(parent)
      child[0] = "World"
      @column.cell_set_cell_data(model, parent, true, true)
      assert_equal([parent[0]], values)
    end
  end
end
