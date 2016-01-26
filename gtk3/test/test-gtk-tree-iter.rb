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
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA  02110-1301  USA

class TestGtkTreeIter < Test::Unit::TestCase
  include GtkTestUtils

  def setup
    @model = Gtk::TreeStore.new(String)
    @iter = @model.append(nil)
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

  test "#next!" do
    next_iter = @model.append(nil)
    @iter.values = ["first"]
    next_iter.values = ["second"]
    assert_equal("first", @iter[0])
    @iter.next!
    assert_equal("second", @iter[0])
  end

  test "#parent" do
    @iter.values = ["Dad"]
    child_iter = @model.append(@iter)
    child_iter.values = ["First son"]
    assert_equal(@iter, child_iter.parent)
  end

  sub_test_case "#has_child?" do
    test "false" do
      @iter.values = ["Dad"]
      assert do
        not @iter.has_child?
      end
    end

    test "true" do
      @iter.values = ["Dad"]
      child_iter = @model.append(@iter)
      child_iter.values = ["First son"]
      assert(@iter.has_child?)
    end
  end

  def test_n_children
    @iter.values = ["Dad"]
    child_iter = @model.append(@iter)
    child_iter.values = ["First son"]
    child_iter = @model.append(@iter)
    child_iter.values = ["First daughter"]
    assert_equal(2, @iter.n_children)
  end

  def test_nth_children
    @iter.values = ["Dad"]
    first_child_iter = @model.append(@iter)
    first_child_iter.values = ["First son"]
    second_child_iter = @model.append(@iter)
    second_child_iter.values = ["First daughter"]
    assert_equal(first_child_iter, @iter.nth_child(0))
    assert_equal(second_child_iter, @iter.nth_child(1))
  end

  def test_first_child
    @iter.values = ["Dad"]
    first_child_iter = @model.append(@iter)
    first_child_iter.values = ["First son"]
    second_child_iter = @model.append(@iter)
    second_child_iter.values = ["First daughter"]
    assert_equal(first_child_iter, @iter.first_child)
  end

  def test_children
    @iter.values = ["Dad"]
    first_child_iter = @model.append(@iter)
    first_child_iter.values = ["First son"]
    second_child_iter = @model.append(@iter)
    second_child_iter.values = ["First daughter"]
    assert_equal(first_child_iter, @iter.children)
  end
end
