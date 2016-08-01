# Copyright (C) 2010-2016  Ruby-GNOME2 Project Team
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

class TestGtkTreePath < Test::Unit::TestCase
  include GtkTestUtils

  def test_compare
    assert_equal(-1, tree_path("0") <=> tree_path("1"))
    assert_equal(0, tree_path("0") <=> tree_path("0"))
    assert_equal(1, tree_path("1") <=> tree_path("0"))
  end

  def test_equal
    assert_equal(tree_path("0"), tree_path("0"))
    assert_not_equal(tree_path("0"), tree_path("1"))
    assert_not_equal(tree_path("0"), "0")
  end

  def test_indices
    assert_equal([2], tree_path("2").indices)
  end

  def test_down!
    path = tree_path("10:4:5")
    path.down!
    assert_equal("10:4:5:0", path.to_s)
  end

  def test_up!
    path = tree_path("10:4:5")
    path.up!
    assert_equal("10:4", path.to_s)
  end

  def test_next!
    path = tree_path("10:4:5")
    path.next!
    assert_equal("10:4:6", path.to_s)
  end

  def test_prev!
    path = tree_path("10:4:5")
    path.prev!
    assert_equal("10:4:4", path.to_s)
  end

  private

  def tree_path(path)
    Gtk::TreePath.new(path)
  end
end
