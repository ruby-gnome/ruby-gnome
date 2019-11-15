# Copyright (C) 2013-2019  Ruby-GNOME Project Team
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

class TestGdkColor < Test::Unit::TestCase
  sub_test_case ".parse" do
    def test_invalid_spec
      spec = "fff"
      message = "Invalid color spec: <#{spec.inspect}>: "
      message << "Color spec must be one of them: "
      message << "\"\#rgb\", "
      message << "\"\#rrggbb\", "
      message << "\"\#rrggbb\", "
      message << "\"\#rrrgggbbb\", "
      message << "\"\#rrrrggggbbbb\""
      assert_raise(ArgumentError.new(message)) do
        Gdk::Color.parse(spec)
      end
    end

    def test_valid
      color = Gdk::Color.parse("#abc")
      assert_equal("#aaaabbbbcccc", color.to_s)
    end
  end

  sub_test_case ".try_convert" do
    test("String") do
      assert_equal(Gdk::Color.parse("gray"),
                   Gdk::Color.try_convert("gray"))
    end

    test("Symbol") do
      assert_equal(Gdk::Color.parse("gray"),
                   Gdk::Color.try_convert(:gray))
    end
  end

  def test_to_s
    color = Gdk::Color.new(0xffff, 0x1234, 0xabcd)
    assert_equal("#ffff1234abcd", color.to_s)
  end
end
