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
  end
end
