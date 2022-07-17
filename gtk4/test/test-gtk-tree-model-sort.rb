# Copyright (C) 2019  Ruby-GNOME Project Team
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

class TestGtkTreeModelSort < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @model = Gtk::TreeStore.new(String)
    @root = @model.append(nil)
    @filter_root = @model.append(@root)
    @filter = Gtk::TreeModelFilter.new(@model, @filter_root.path)
  end

  test(".new") do
    sort = Gtk::TreeModelSort.new(@filter)
    assert_equal(@filter, sort.model)
  end
end
