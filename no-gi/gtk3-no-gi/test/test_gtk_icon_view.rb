# -*- coding: utf-8 -*-
#
# Copyright (C) 2014  Ruby-GNOME2 Project Team
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

  class TestGetCellRect < self
    def setup
      only_gtk_version(3, 6, 0)
      model = Gtk::ListStore.new(String, Gdk::Pixbuf)
      iter = model.append
      model.set_values(iter, ["label", nil])
      @path = iter.path
      @icon_view = Gtk::IconView.new(model)
    end

    def test_found
      assert_kind_of(Gdk::Rectangle, @icon_view.get_cell_rect(@path))
    end

    def test_not_found
      not_found_path = Gtk::TreePath.new(@path.indices.first + 1)
      assert_nil(@icon_view.get_cell_rect(not_found_path))
    end
  end
end
