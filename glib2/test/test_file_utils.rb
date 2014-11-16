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

  sub_test_case "#format_size" do
    def setup
      only_glib_version(2, 30, 0)
    end

    def test_kb
      assert_equal("1.0 kB", GLib.format_size(1000))
    end

    def test_mb
      assert_equal("1.0 MB", GLib.format_size(1000 * 1000))
    end

    def test_gb
      assert_equal("1.0 GB", GLib.format_size(1000 * 1000 * 1000))
    end

    def test_format_size_with_IEC_UNITS
      assert_equal("1.0 KiB",
                   GLib.format_size(1024, :flags => :iec_units))
      assert_equal("10.0 KiB",
                   GLib.format_size(1024 * 10, :flags => :iec_units))
      assert_equal("1.0 MiB",
                   GLib.format_size(1024 * 1024, :flags => :iec_units))
      assert_equal("1.5 MiB",
                   GLib.format_size(1024 * 1024 * 1.5, :flags => :iec_units))
      assert_equal("1.0 GiB",
                   GLib.format_size(1024 * 1024 * 1024, :flags => :iec_units))
    end
  end
end
