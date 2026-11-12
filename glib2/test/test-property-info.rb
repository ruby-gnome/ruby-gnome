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

class TestPropertyInfo < Test::Unit::TestCase
  include GLibTestUtils

  def setup
    only_girepository
    @repository = GLib::Repository.default
    @repository.require("Gio")
    @interface_info = @repository.find("Gio", "TlsServerConnection")
    @info = @interface_info.get_property(0)
  end

  def test_name
    assert_equal("authentication-mode", @info.name)
  end

  def test_flags
    assert_equal(GLib::Param::READABLE | GLib::Param::WRITABLE,
                 @info.flags)
  end

  def test_type
    assert_equal(GLib::TypeTag::INTERFACE, @info.type.tag)
  end

  def test_ownership_transfer
    assert_equal(GLib::Transfer::NOTHING,
                 @info.ownership_transfer)
  end

  def test_getter
    info = find_property("validation-flags")
    assert_equal("get_validation_flags", info.getter.name)
  end

  def test_getter_nil
    assert_nil(@info.getter)
  end

  def test_setter
    info = find_property("validation-flags")
    assert_equal("set_validation_flags", info.setter.name)
  end

  def test_setter_nil
    assert_nil(@info.setter)
  end

  private
  def find_property(name)
    interface_info = @repository.find("Gio", "TlsClientConnection")
    interface_info.properties.find do |property_info|
      property_info.name == name
    end
  end
end
