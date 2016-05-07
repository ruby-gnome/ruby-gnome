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

class TestGtkTreeView < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case("instance methods") do
    def setup
      @store = Gtk::TreeStore.new(String)
      @view = Gtk::TreeView.new(@store)
      @column = Gtk::TreeViewColumn.new("Label",
                                        Gtk::CellRendererText.new,
                                        :text => 0)
      @view.append_column(@column)

      @window = Gtk::Window.new
      @window.add(@view)
      @window.signal_connect("destroy") do
        Gtk.main_quit
      end
    end

    sub_test_case "#get_path_at_pos" do
      test "found" do
        iter = @store.append(nil)
        iter[0] = "Hello"
        result = nil
        GLib::Idle.add do
          result = @view.get_path_at_pos(0, 0)
          @window.destroy
          GLib::Source::REMOVE
        end
        @window.show_all
        Gtk.main

        result[0] = result[0].to_s if result[0].is_a?(Gtk::TreePath)
        assert_equal(["0", @column, 0, 0],
                     result)
      end

      test "not found" do
        result = nil
        GLib::Idle.add do
          result = @view.get_path_at_pos(0, 0)
          @window.destroy
          GLib::Source::REMOVE
        end
        @window.show_all
        Gtk.main

        assert_nil(result)
      end
    end

    test "#map_expanded_rows" do
      parent = @store.append(nil)
      parent[0] = "Hello"
      child = @store.append(parent)
      child[0] = "World"

      @view.expand_all
      @view.map_expanded_rows do |view, path|
        iter = @store.get_iter(path)
        iter[0] += " (mapped)"
      end

      normalized_data = @store.collect do |model, path, iter|
        [model, path.to_s, iter[0]]
      end
      assert_equal([
                     [@store, parent.path.to_s, "Hello (mapped)"],
                     [@store, child.path.to_s, "World"],
                   ],
                   normalized_data)
    end

    sub_test_case "#expand_row" do
      setup do
        @parent = @store.append(nil)
        @parent[0] = "parent"
        @child = @store.append(@parent)
        @child[0] = "child"
        @grand_child = @store.append(@child)
        @grand_child[0] = "grand child"
      end

      test "default" do
        @view.expand_row(@parent.path)
        assert do
          @view.row_expanded?(@parent.path)
        end
        assert do
          @view.row_expanded?(@child.path)
        end
      end

      test "true" do
        @view.expand_row(@parent.path, true)
        assert do
          @view.row_expanded?(@parent.path)
        end
        assert do
          @view.row_expanded?(@child.path)
        end
      end

      test "false" do
        @view.expand_row(@parent.path, false)
        assert do
          @view.row_expanded?(@parent.path)
        end
        assert do
          not @view.row_expanded?(@child.path)
        end
      end

      test ":open_all => true" do
        @view.expand_row(@parent.path, :open_all => true)
        assert do
          @view.row_expanded?(@parent.path)
        end
        assert do
          @view.row_expanded?(@child.path)
        end
      end

      test ":open_all => false" do
        @view.expand_row(@parent.path, :open_all => false)
        assert do
          @view.row_expanded?(@parent.path)
        end
        assert do
          not @view.row_expanded?(@child.path)
        end
      end
    end

    test "#row_expanded?" do
      parent = @store.append(nil)
      parent[0] = "Hello"
      child = @store.append(parent)
      child[0] = "World"

      assert do
        not @view.row_expanded?(parent.path)
      end
      @view.expand_all
      assert do
        @view.row_expanded?(parent.path)
      end
    end

    sub_test_case "#insert_column" do
      def setup
        @store = Gtk::TreeStore.new(String)
        @view = Gtk::TreeView.new(@store)
      end

      test "column, position" do
        column = Gtk::TreeViewColumn.new("Label",
                                         Gtk::CellRendererText.new,
                                         :text => 0)
        @view.insert_column(column, 0)
        assert_equal([column], @view.columns)
      end

      test "position, title, cell" do
        @view.insert_column(0, "Label", Gtk::CellRendererText.new) do
        end
        assert_equal(["Label"], @view.columns.collect(&:title))
      end

      test "position, title, cell, attributes" do
        @view.insert_column(0, "Label", Gtk::CellRendererText.new,
                            :text => 0)
        assert_equal(["Label"], @view.columns.collect(&:title))
      end
    end
  end

  sub_test_case "signal" do
    def setup
      @store = Gtk::TreeStore.new(String)
      @view = Gtk::TreeView.new(@store)

      root = @store.append(nil)
      root[0] = "root"
      child = @store.append(root)
      child[0] = "child"
      grand_child = @store.append(child)
      grand_child[0] = "grand child"
    end

    test "row-collapsed" do
      @view.expand_all

      values = []
      @view.signal_connect("row-collapsed") do |_, iter, path|
        values << iter[0]
      end
      @view.collapse_row(Gtk::TreePath.new("0:0"))
      assert_equal(["child"], values)
    end

    test "row-expanded" do
      @view.collapse_all

      values = []
      @view.signal_connect("row-expanded") do |_, iter, path|
        values << iter[0]
      end
      @view.expand_row(Gtk::TreePath.new("0"))
      assert_equal(["root", "child"], values)
    end
  end
end
