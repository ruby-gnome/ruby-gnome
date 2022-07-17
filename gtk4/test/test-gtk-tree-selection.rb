# -*- coding: utf-8 -*-
#
# Copyright (C) 2014-2015  Ruby-GNOME2 Project Team
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

class TestGtkTreeSelection < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case("accessors") do
    def setup
      @tree = Gtk::TreeView.new
      @selection =  @tree.selection
    end

    test "mode" do
      @selection.mode = :multiple
      assert_equal(Gtk::SelectionMode::MULTIPLE, @selection.mode)
    end
  end

  sub_test_case("selected") do
    def setup
      @model = Gtk::TreeStore.new(String)
      @view = Gtk::TreeView.new(@model)
      @column = Gtk::TreeViewColumn.new("Label",
                                        Gtk::CellRendererText.new,
                                        :text => 0)
      @iter = @model.append(nil)
      @iter[0] = "Hello"

      @selection = @view.selection
      @selection.select_path(@iter.path)
    end

    test "#selected_rows" do
      assert_equal([[@iter.path], @model],
                   @selection.selected_rows)
    end

    test "#each" do
      normalized_selected = @selection.collect do |model, path, iter|
        [model, path.to_s, iter.class]
      end
      assert_equal([[@model, @iter.path.to_s, @iter.class]],
                   normalized_selected)
    end
  end
end
