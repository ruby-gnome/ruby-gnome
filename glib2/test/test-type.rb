# Copyright (C) 2019  Ruby-GNOME Project Team
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

class TestGLibType < Test::Unit::TestCase
  sub_test_case(".try_convert") do
    def test_nil
      assert_nil(GLib::Type.try_convert(nil))
    end

    def test_type
      assert_equal(GLib::Type::STRING,
                   GLib::Type.try_convert(GLib::Type::STRING))
    end

    def test_integer
      assert_equal(GLib::Type::STRING,
                   GLib::Type.try_convert(GLib::Type::STRING.to_i))
    end

    def test_string
      assert_equal(GLib::Type::STRING,
                   GLib::Type.try_convert("gchararray"))
    end

    def test_symbol
      assert_equal(GLib::Type::STRING,
                   GLib::Type.try_convert(:gchararray))
    end

    def test_nonexistent
      assert_nil(GLib::Type.try_convert(:nonexistent))
    end

    def test_unconvertable
      assert_nil(GLib::Type.try_convert([]))
    end
  end
end
