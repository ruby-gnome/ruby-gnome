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
    @info = @callable_info[0]
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
end
