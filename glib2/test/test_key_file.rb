# Copyright (C) 2015  Ruby-GNOME2 Project Team
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

require 'tempfile'

class TestGLibKeyFile < Test::Unit::TestCase
  include GLibTestUtils

  def test_load_from_dirs
    only_glib_version(2, 14, 0)

    key_file = GLib::KeyFile.new
    assert_raise(GLib::KeyFileError::NotFound) do
      key_file.load_from_dirs("non-existent")
    end

    temp = Tempfile.new("key-file")
    base_name = File.basename(temp.path)
    search_dirs = [File.dirname(temp.path)]
    assert_raise(GLib::KeyFileError::NotFound) do
      key_file.load_from_dirs("non-existent", search_dirs)
    end
    if GLib.check_version?(2, 31, 2)
      assert_equal(temp.path, key_file.load_from_dirs(base_name, search_dirs))
    elsif GLib.check_version?(2, 30, 0)
      assert_raise(GLib::KeyFileError::NotFound) do
        key_file.load_from_dirs(base_name, search_dirs)
      end
    else
      assert_raise(GLib::KeyFileError::Parse) do
        key_file.load_from_dirs(base_name, search_dirs)
      end
    end
    temp.puts(<<-EOK)
[General]
key = value
EOK
    temp.close
    assert_equal(temp.path, key_file.load_from_dirs(base_name, search_dirs))
  end

  def test_desktop_constants
    only_glib_version(2, 14, 0)
    assert_equal("Desktop Entry", GLib::KeyFile::DESKTOP_GROUP)
    assert_equal("URL", GLib::KeyFile::DESKTOP_KEY_URL)
  end
end
