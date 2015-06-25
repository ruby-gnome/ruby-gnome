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
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA  02110-1301  USA

class TestGtkTreeIter < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @model = Gtk::ListStore.new(String)
    @iter = @model.append
  end

  def test_path
    assert_equal("0", @iter.path.to_s)
  end

  sub_test_case("#set_values") do
    test "Array" do
      @iter.set_values(["string"])
      assert_equal("string", @iter[0])
    end

    test "Hash" do
      @iter.set_values(0 => "string")
      assert_equal("string", @iter[0])
    end
  end

  sub_test_case("#values=") do
    test "Array" do
      @iter.values = ["string"]
      assert_equal("string", @iter[0])
    end

    test "Hash" do
      @iter.values = {0 => "string"}
      assert_equal("string", @iter[0])
    end
  end
end
