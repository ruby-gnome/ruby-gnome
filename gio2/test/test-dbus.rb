# Copyright (C) 2014  Ruby-GNOME2 Project Team
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

class TestDBus < Test::Unit::TestCase
  def test_dbus_guid
    guid = Gio::DBus.generate_guid
    assert_true(Gio::DBus.guid?(guid))
  end

  def test_dbus_name
    name = "org.freedesktop.DBus"
    assert_true(Gio::DBus.name?(name))
  end

  def test_dbus_unique_name
    name = ":org.freedesktop.DBus"
    assert_true(Gio::DBus.unique_name?(name))
  end

  def test_dbus_interface_name
    name = "org.freedesktop.DBus"
    assert_true(Gio::DBus.interface_name?(name))
  end

  def test_dbus_member_name
    member = "StartServiceByName"
    assert_true(Gio::DBus.member_name?(member))
  end
end
