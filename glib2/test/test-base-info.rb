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

class TestBaseInfo < Test::Unit::TestCase
  include GLibTestUtils

  def setup
    unless GLib.const_defined?(:Repository)
      omit("Need RUBY_GNOME_GLIB2_GIREPOSITORY_ENABLE=yes on build")
    end
    @repository = GLib::Repository.default
    @repository.require("GObject")
    @info = @repository.find("GObject", "Object")
  end

  def test_name
    assert_equal("Object", @info.name)
  end

  def test_namespace
    assert_equal("GObject", @info.namespace)
  end

  def test_container
    assert_nil(@info.container)
  end

  def test_enumerable
    assert_equal([], @info.to_a)
  end
end
