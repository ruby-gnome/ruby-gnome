# -*- coding: utf-8 -*-

class TestGLibFileUtils < Test::Unit::TestCase
  include GLibTestUtils

  def test_format_size_for_display
    only_glib_version(2, 16, 0)

    assert_equal("1.0 KB", GLib.format_size_for_display(1024))
    assert_equal("10.0 KB", GLib.format_size_for_display(1024 * 10))
    assert_equal("1.0 MB", GLib.format_size_for_display(1024 * 1024))
    assert_equal("1.5 MB", GLib.format_size_for_display(1024 * 1024 * 1.5))
    assert_equal("1.0 GB", GLib.format_size_for_display(1024 * 1024 * 1024))
  end

  def test_format_size
    only_glib_version(2, 30, 0)

    assert_equal("1.0 kB", GLib.format_size(1000))
    assert_equal("10.0 kB", GLib.format_size(1000 * 10))
    assert_equal("1.0 MB", GLib.format_size(1000 * 1000))
    assert_equal("1.5 MB", GLib.format_size(1000 * 1000 * 1.5))
    assert_equal("1.0 GB", GLib.format_size(1000 * 1000 * 1000))
  end
end
