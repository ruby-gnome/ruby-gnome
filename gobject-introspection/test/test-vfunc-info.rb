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

class TestVFuncInfo < Test::Unit::TestCase
  def setup
    @repository = GObjectIntrospection::Repository.default
    @repository.require("Gio")
    @object_info = @repository.find("Gio", "FileOutputStream")
    @info = @object_info.get_vfunc(0)
  end

  def test_flags
    assert_equal(GObjectIntrospection::VFuncInfoFlags.new,
                 @info.flags)
  end

  def test_offset
    assert_equal(GObjectIntrospection::VFuncInfo::UNKNOWN_OFFSET,
                 @info.offset)
  end

  def test_signal
    assert_nil(@info.signal)
  end

  def test_invoker
    assert_nil(@info.invoker)
  end
end
