# -*- coding: utf-8 -*-

class TestGLibWin32 < Test::Unit::TestCase
  include GLibTestUtils

  def test_get_package_install_directory_of_module
    only_win32
    only_glib_version(2, 16, 0)

    assert_equal("FIXME",
                 GLib::Win32.get_package_installation_directory_of_module)
  end
end
