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

class TestGtkTreeSortable < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @model = Gtk::ListStore.new(String)
  end

  test "#set_sort_func" do
    @model.append[0] = "abc"
    @model.append[0] = "xyz"
    @model.append[0] = "efg"
    @model.set_sort_column_id(0, :ascending)
    @model.set_sort_func(0) do |_model, iter1, iter2|
      iter2[0] <=> iter1[0]
    end
    assert_equal(["xyz", "efg", "abc"],
                 @model.collect {|_model, _path, iter| iter[0]})
  end
end
