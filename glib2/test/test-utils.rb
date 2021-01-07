# Copyright (C) 2015-2021  Ruby-GNOME Project Team
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

class TestGLibUtils < Test::Unit::TestCase
  include GLibTestUtils

  def test_user_cache_dir
    assert_kind_of(String, GLib.user_cache_dir)
  end

  def test_user_data_dir
    assert_kind_of(String, GLib.user_data_dir)
  end

  def test_user_config_dir
    assert_kind_of(String, GLib.user_config_dir)
  end

  def test_system_data_dirs
    assert_kind_of(Array, GLib.system_data_dirs)
  end

  def test_system_config_dirs
    assert_kind_of(Array, GLib.system_config_dirs)
  end

  def test_get_user_special_dir
    assert_nothing_raised do
      GLib.get_user_special_dir(GLib::UserDirectory::DESKTOP)
    end
  end
end
