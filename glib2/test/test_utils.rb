# -*- coding: utf-8 -*-

class TestGLibUtils < Test::Unit::TestCase
  include GLibTestUtils

  def test_user_cache_dir
    only_glib_version(2, 6, 0)
    assert_kind_of(String, GLib.user_cache_dir)
  end

  def test_user_data_dir
    only_glib_version(2, 6, 0)
    assert_kind_of(String, GLib.user_data_dir)
  end

  def test_user_config_dir
    only_glib_version(2, 6, 0)
    assert_kind_of(String, GLib.user_config_dir)
  end

  def test_system_data_dirs
    only_glib_version(2, 6, 0)
    assert_kind_of(Array, GLib.system_data_dirs)
  end

  def test_system_config_dirs
    only_glib_version(2, 6, 0)
    assert_kind_of(Array, GLib.system_config_dirs)
  end

  def test_get_user_special_dir
    only_glib_version(2, 14, 0)
    assert_nothing_raised do
      GLib.get_user_special_dir(GLib::UserDirectory::DESKTOP)
    end
  end
end
