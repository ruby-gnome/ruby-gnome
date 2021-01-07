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

class TestGLibFileUtils < Test::Unit::TestCase
  include GLibTestUtils

  def normalize_space(string)
    string.gsub("\u{00A0}", " ")
  end

  sub_test_case "#format_size" do
    def test_kb
      assert_equal("1.0 kB",
                   normalize_space(GLib.format_size(1000)))
    end

    def test_mb
      assert_equal("1.0 MB",
                   normalize_space(GLib.format_size(1000 * 1000)))
    end

    def test_gb
      assert_equal("1.0 GB",
                   normalize_space(GLib.format_size(1000 * 1000 * 1000)))
    end

    def test_over_guint32_value
      guint32_max = 2 ** 32 - 1
      assert_equal("4.3 GB",
                   normalize_space(GLib.format_size(guint32_max + 1)))
    end

    sub_test_case "flags" do
      sub_test_case ":iec_units" do
        def format_size(size)
          normalize_space(GLib.format_size(size, :flags => :iec_units))
        end

        def test_kib
          assert_equal("1.0 KiB", format_size(1024))
        end

        def test_mib
          assert_equal("1.0 MiB", format_size(1024 * 1024))
        end

        def test_gib
          assert_equal("1.0 GiB", format_size(1024 * 1024 * 1024))
        end
      end
    end
  end
end
