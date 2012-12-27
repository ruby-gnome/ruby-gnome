# Copyright (C) 2012  Ruby-GNOME2 Project Team
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

class TestStructInfo < Test::Unit::TestCase
  def setup
    @repository = GObjectIntrospection::Repository.default
    @repository.require("GObject")
    @info = @repository.find("GObject", "Value")
  end

  def test_n_fields
    assert_equal(2, @info.n_fields)
  end

  def test_get_field
    assert_kind_of(GObjectIntrospection::FieldInfo,
                   @info.get_field(0))
  end

  def test_n_methods
    assert_equal(62, @info.n_methods)
  end

  def test_get_method
    assert_kind_of(GObjectIntrospection::FunctionInfo,
                   @info.get_method(0))
  end
end
