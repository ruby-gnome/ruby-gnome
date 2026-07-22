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

class TestArgInfo < Test::Unit::TestCase
  def setup
    @repository = GObjectIntrospection::Repository.default
    @repository.require("GObject")
    @callable_info = @repository.find("GObject", "signal_name")
    @info = @callable_info.get_arg(0)
  end

  def test_direction
    assert_equal(GObjectIntrospection::Direction::IN,
                 @info.direction)
  end

  def test_caller_allocate?
    assert_false(@info.caller_allocates?)
  end

  def test_return_value?
    assert_false(@info.return_value?)
  end

  def test_optional?
    assert_false(@info.optional?)
  end

  def test_may_be_null?
    assert_false(@info.may_be_null?)
  end

  def test_ownership_transfer
    assert_equal(GObjectIntrospection::Transfer::NOTHING,
                 @info.ownership_transfer)
  end

  def test_scope
    assert_equal(GObjectIntrospection::ScopeType::INVALID,
                 @info.scope)
  end

  def test_closure
    assert_equal(-1, @info.closure)
  end

  def test_destroy
    assert_equal(-1, @info.destroy)
  end

  def test_type
    assert_kind_of(GObjectIntrospection::TypeInfo,
                   @info.type)
  end
end
