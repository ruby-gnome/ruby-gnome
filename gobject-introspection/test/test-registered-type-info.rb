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

class TestRegisteredTypeInfo < Test::Unit::TestCase
  def setup
    @repository = GObjectIntrospection::Repository.default
    @repository.require("GObject")
    @info = @repository.find("GObject", "TypePlugin")
  end

  def test_type_name
    assert_equal("GTypePlugin", @info.type_name)
  end

  def test_type_init
    assert_equal("g_type_plugin_get_type", @info.type_init)
  end

  def test_gtype
    assert_equal(GLib::Type.new("GTypePlugin"), @info.gtype)
  end

  def test_match
    @repository.require("GLib")
    info = @repository.find("GLib", "Bytes")
    assert do
      info.match?(GLib::Bytes.new("data"))
    end
  end

  def test_match_cast
    @repository.require("GLib")
    info = @repository.find("GLib", "Bytes")
    assert do
      not info.match?("data")
    end
  end

  def test_match_unresolved
    @repository.require("GLib")
    info = @repository.find("GLib", "Cond")
    assert do
      not info.match?(Object.new)
    end
  end
end
