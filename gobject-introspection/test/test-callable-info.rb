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

class TestCallableInfo < Test::Unit::TestCase
  def setup
    @repository = GObjectIntrospection::Repository.default
    @repository.require("GObject")
    @info = @repository.find("GObject", "signal_name")
  end

  def test_return_type
    assert_kind_of(GObjectIntrospection::TypeInfo,
                   @info.return_type)
  end

  def test_caller_owns
    assert_equal(GObjectIntrospection::Transfer::NOTHING,
                 @info.caller_owns)
  end

  def test_may_return_null?
    if GObjectIntrospection::Version.or_later?(1, 67, 0)
      assert do
        @info.may_return_null?
      end
    else
      assert do
        not @info.may_return_null?
      end
    end
  end

  def test_n_args
    assert_equal(1, @info.n_args)
  end

  def test_get_arg
    assert_equal("signal_id", @info.get_arg(0).name)
  end

  def test_args
    assert_equal(["signal_id"], @info.args.collect(&:name))
  end
end
