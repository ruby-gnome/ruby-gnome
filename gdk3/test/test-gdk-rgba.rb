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

class TestGdkRGBA < Test::Unit::TestCase
  sub_test_case ".try_convert" do
    test("String") do
      assert_equal(Gdk::RGBA.parse("gray"),
                   Gdk::RGBA.try_convert("gray"))
    end

    test("Symbol") do
      assert_equal(Gdk::RGBA.parse("gray"),
                   Gdk::RGBA.try_convert(:gray))
    end
  end

  def test_to_s
    rgba = Gdk::RGBA.new(0.2, 0.4, 0.6, 0.5)
    assert_equal("rgba(51,102,153,0.5)", rgba.to_s)
  end

  def test_to_a
    rgba = Gdk::RGBA.new(0.2, 0.4, 0.6, 0.5)
    assert_equal([0.2, 0.4, 0.6, 0.5], rgba.to_a)
  end

  sub_test_case("new") do
    def test_empty
      rgba = Gdk::RGBA.new
      assert_equal([0.0, 0.0, 0.0, 1.0], rgba.to_a)
    end

    def test_rgb
      rgba = Gdk::RGBA.new(0.2, 0.4, 0.6)
      assert_equal([0.2, 0.4, 0.6, 1.0], rgba.to_a)
    end
  end

  sub_test_case("parse") do
    def test_name
      rgba = Gdk::RGBA.parse("red")
      assert_equal([1.0, 0.0, 0.0, 1.0], rgba.to_a)
    end

    def test_hash_rgb
      rgba = Gdk::RGBA.parse("#f0f")
      assert_equal([1.0, 0.0, 1.0, 1.0], rgba.to_a)
    end

    def test_hash_rrggbb
      rgba = Gdk::RGBA.parse("#ff00ff")
      assert_equal([1.0, 0.0, 1.0, 1.0], rgba.to_a)
    end

    def test_hash_rrrgggbbb
      rgba = Gdk::RGBA.parse("#fff000fff")
      assert_equal([1.0, 0.0, 1.0, 1.0], rgba.to_a)
    end

    def test_hash_rrrrggggbbbb
      rgba = Gdk::RGBA.parse("#ffff0000ffff")
      assert_equal([1.0, 0.0, 1.0, 1.0], rgba.to_a)
    end

    def test_rgb
      rgba = Gdk::RGBA.parse("rgb(255, 0, 255)")
      assert_equal([1.0, 0.0, 1.0, 1.0], rgba.to_a)
    end

    def test_rgba
      rgba = Gdk::RGBA.parse("rgba(255, 0, 255, 0.5)")
      assert_equal([1.0, 0.0, 1.0, 0.5], rgba.to_a)
    end

    def test_invalid
      message = "invalid RGBA format: \"invalid\" "
      message << "(available formats: "
      message << "COLOR_NAME, "
      message << "\#RGB, "
      message << "\#RRGGBB, "
      message << "\#RRRGGGBBB, "
      message << "\#RRRRGGGGBBBB, "
      message << "rgb(R, G, B), "
      message << "rgba(R, G, B, A)"
      message << ")"
      assert_raise(ArgumentError.new(message)) do
        Gdk::RGBA.parse("invalid")
      end
    end
  end
end
