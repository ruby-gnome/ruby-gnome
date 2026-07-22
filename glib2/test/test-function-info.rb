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

class TestFunctionInfo < Test::Unit::TestCase
  def setup
    @repository = GObjectIntrospection::Repository.default
    @repository.require("GObject")
    @info = @repository.find("GObject", "signal_name")
  end

  def test_symbol
    assert_equal("g_signal_name", @info.symbol)
  end

  def test_flags
    assert_equal(GObjectIntrospection::FunctionInfoFlags.new(0),
                 @info.flags)
  end

  def test_invoke
    # TODO: "#invoke" expects Array. We should confirm specification.
    #assert_equal("notify", @info.invoke(1))
    assert_equal("notify", @info.invoke([1]))
  end
end

