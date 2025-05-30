# Copyright (C) 2015-2025  Ruby-GNOME Project Team
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

class TestGLibWin32 < Test::Unit::TestCase
  include GLibTestUtils

  def test_get_package_install_directory_of_module
    only_windows
    if ENV["GITHUB_ACTIONS"] == "true"
      omit("We can't detect suitable expected on GitHub Actions")
    end

    expected = Pathname(RbConfig.ruby).parent.parent.to_s
    actual = GLib::Win32.get_package_installation_directory_of_module
    assert_equal(expected, normalize_path(actual))
  end
end
