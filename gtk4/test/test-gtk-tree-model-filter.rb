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

class TestGtkTreeModelFilter < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @model = Gtk::TreeStore.new(String)
    @root = @model.append(nil)
    @filter_root = @model.append(@root)
    @filter = Gtk::TreeModelFilter.new(@model, @filter_root.path)
  end

  sub_test_case "#set_modify_func" do
    test "no argument" do
      assert_raise(ArgumentError.new("one or more types are required")) do
        @filter.set_modify_func do
        end
      end
    end
  end

  sub_test_case "#convert_iter_to_child_iter" do
    test "converted" do
      iter = @model.append(@filter_root)
      assert_equal(iter,
                   @filter.convert_iter_to_child_iter(@filter.iter_first))
    end
  end

  sub_test_case "#convert_child_iter_to_iter" do
    test "converted" do
      iter = @model.append(@filter_root)
      assert_equal(@filter.iter_first,
                   @filter.convert_child_iter_to_iter(iter))
    end

    test "not converted" do
      assert_nil(@filter.convert_child_iter_to_iter(@root))
    end
  end
end
