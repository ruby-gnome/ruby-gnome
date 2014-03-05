# Copyright (C) 2012-2014  Ruby-GNOME2 Project Team
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

class TestEnumInfo < Test::Unit::TestCase
  def setup
    @repository = GObjectIntrospection::Repository.default
    @repository.require("Gio")
    @info = @repository.find("Gio", "ResourceError")
  end

  def test_n_values
    assert_equal(2, @info.n_values)
  end

  def test_value
    assert_kind_of(GObjectIntrospection::ValueInfo,
                   @info.get_value(0))
  end

  def test_values
    assert_equal([GObjectIntrospection::ValueInfo] * @info.n_values,
                 @info.values.collect(&:class))
  end

  def test_n_methods
    assert_equal(1, @info.n_methods)
  end

  def test_method
    assert_kind_of(GObjectIntrospection::FunctionInfo,
                   @info.get_method(0))
  end

  def test_methods
    assert_equal([GObjectIntrospection::FunctionInfo] * @info.n_methods,
                 @info.methods.collect(&:class))
  end

  def test_storage_type
    assert_equal(GObjectIntrospection::TypeTag::UINT32,
                 @info.storage_type)
  end

  def test_error_domain
    assert_equal("g-resource-error-quark",
                 @info.error_domain)
  end
end
