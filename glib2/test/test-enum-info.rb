# Copyright (C) 2012-2026  Ruby-GNOME Project Team
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
  include GLibTestUtils

  def setup
    only_girepository
    @repository = GLib::Repository.default
    @repository.require("Gio")
    @info = @repository.find("Gio", "ResourceError")
  end

  def test_n_values
    assert_equal(2, @info.n_values)
  end

  def test_get_value
    assert_equal("not_found", @info.get_value(0).name)
  end

  def test_values
    assert_equal(["not_found", "internal"],
                 @info.values.collect(&:name))
  end

  def test_n_methods
    assert_equal(1, @info.n_methods)
  end

  def test_get_method
    assert_equal("quark", @info.get_method(0).name)
  end

  def test_methods
    assert_equal(["quark"],
                 @info.methods.collect(&:name))
  end

  def test_storage_type
    assert_equal(GLib::TypeTag::UINT32,
                 @info.storage_type)
  end

  def test_error_domain
    assert_equal("g-resource-error-quark",
                 @info.error_domain)
  end

  def test_error_domain_nil
    info = @repository.find("Gio", "FileAttributeType")
    assert_nil(info.error_domain)
  end
end
