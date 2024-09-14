# Copyright (C) 2024  Ruby-GNOME Project Team
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

class TestListModel < Test::Unit::TestCase
  def setup
    @list_model = Gio::ListStore.new(Gio::SimpleAction)
    @elements = [
      Gio::SimpleAction.new("a"),
      Gio::SimpleAction.new("b"),
    ]
    @elements.each do |element|
      @list_model.append(element)
    end
  end

  def test_aref
    assert_equal(@elements[0], @list_model[0])
  end

  def test_each
    assert_equal(@elements, @list_model.to_a)
  end

  def test_each_without_block
    assert_equal(@elements, @list_model.each.to_a)
  end
end
