# -*- coding: utf-8 -*-

require 'tempfile'

class TestGLibKeyFile < Test::Unit::TestCase
  include GLibTestUtils

  def test_load_from_dirs
    only_glib_version(2, 14, 0)

    key_file = GLib::KeyFile.new
    assert_raise(GLib::KeyFileError) do
      key_file.load_from_dirs("non-existent")
    end

    temp = Tempfile.new("key-file")
    base_name = File.basename(temp.path)
    search_dirs = [File.dirname(temp.path)]
    assert_raise(GLib::KeyFileError) do
      key_file.load_from_dirs("non-existent", search_dirs)
    end
    if GLib.check_version?(2, 31, 2)
      assert_equal(temp.path, key_file.load_from_dirs(base_name, search_dirs))
    else
      assert_raise(GLib::KeyFileError) do
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
