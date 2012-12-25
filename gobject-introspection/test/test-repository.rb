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

class TestRepository < Test::Unit::TestCase
  def setup
    @repository = GObjectIntrospection::Repository.default
    @repository.require("GObject")
    @repository.require("Gio")
  end

  def test_get_n_infos
    assert_kind_of(Integer, @repository.get_n_infos("GObject"))
  end

  def test_get_info
    assert_kind_of(GObjectIntrospection::BaseInfo,
                   @repository.get_info("GObject", 0))
  end

  def test_loaded_namespaces
    assert_equal(["GObject", "Gio"].sort,
                 @repository.loaded_namespaces.sort)
  end

  def test_enumerable
    namespaces = @repository.collect do |info|
      info.namespace
    end
    assert_equal(["GObject", "Gio"].sort, namespaces.uniq.sort)
  end

  def test_find_by_gtype
    info = @repository.find(GLib::Object.gtype)
    assert_equal("Object", info.name)
  end

  def test_find_by_name
    info = @repository.find("GObject", "Object")
    assert_equal("Object", info.name)
  end
end
