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
end
