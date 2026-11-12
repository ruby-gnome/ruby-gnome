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

class TestFlagsInfo < Test::Unit::TestCase
  include GLibTestUtils

  def setup
    only_girepository
    @repository = GLib::Repository.default
    @repository.require("Gio")
    @info = @repository.find("Gio", "FileCopyFlags")
  end

  def test_ancestors
    assert do
      GLib::FlagsInfo < GLib::EnumInfo
    end
  end

  def test_values
    assert_equal(["none", "overwrite", "backup"],
                 @info.values.first(3).collect(&:name))
  end

  def test_value
    assert_equal([0, 1, 2],
                 @info.values.first(3).collect(&:value))
  end
end
