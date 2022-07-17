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

class TestGtkIconView < Test::Unit::TestCase
  include GtkTestUtils

  sub_test_case(".new") do
    test "no arguments" do
      icon_view = Gtk::IconView.new
      assert_nil(icon_view.model)
    end

    test ":model" do
      model = Gtk::ListStore.new(String)
      icon_view = Gtk::IconView.new(:model => model)
      assert_equal(model, icon_view.model)
    end

    test ":area" do
      area = Gtk::CellAreaBox.new
      icon_view = Gtk::IconView.new(:area => area)
      assert_equal(area, icon_view.area)
    end
  end

  class TestGetCellRect < self
    def setup
      only_gtk_version(3, 6, 0)
      model = Gtk::ListStore.new(String, GdkPixbuf::Pixbuf)
      iter = model.append
      model.set_values(iter, ["label", nil])
      @path = iter.path
      @icon_view = Gtk::IconView.new(:model => model)
    end

    def test_found
      assert_kind_of(Gdk::Rectangle, @icon_view.get_cell_rect(@path))
    end

    def test_not_found
      indices = @path.indices
      not_found_path = Gtk::TreePath.new
      not_found_path.append_index(indices.last + 1)
      assert_nil(@icon_view.get_cell_rect(not_found_path))
    end
  end
end
