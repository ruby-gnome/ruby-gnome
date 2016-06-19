# Copyright (C) 2016  Ruby-GNOME2 Project Team
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

class TestPixbuf < Test::Unit::TestCase
  include GdkPixbufTestUtils

  sub_test_case(".new") do
    def r_xpm
      [
       "10 10 3 1",
       "   c None",
       ".  c #FE0B0B",
       "+  c #FFFFFF",
       "+.......++",
       "+..    ..+",
       "+..    ..+",
       "+..   ...+",
       "+.......++",
       "+.....++++",
       "+..++..+++",
       "+..++...++",
       "+..+++...+",
        "+..++++..+",
      ]
    end

    sub_test_case("legacy form") do
      test "basic" do
        colorspace =  GdkPixbuf::Colorspace::RGB
        width = 100
        height = 100
        has_alpha = true
        bits_per_sample = 8
        pixbuf = GdkPixbuf::Pixbuf.new(colorspace,
                                       has_alpha,
                                       bits_per_sample,
                                       width,
                                       height)
        assert_equal(colorspace, pixbuf.colorspace)
        assert_equal(width, pixbuf.width)
        assert_equal(height, pixbuf.height)
        assert_equal(has_alpha, pixbuf.has_alpha?)
        assert_equal(bits_per_sample, pixbuf.bits_per_sample)
      end

      test "file" do
        pixbuf = GdkPixbuf::Pixbuf.new(fixture_path("gnome-logo-icon.png"))
        assert_equal(GdkPixbuf::Colorspace::RGB, pixbuf.colorspace)
      end

      test "xpm" do
        pixbuf = GdkPixbuf::Pixbuf.new(r_xpm)
        assert_equal(GdkPixbuf::Colorspace::RGB, pixbuf.colorspace)
      end

      test "file: size" do
        pixbuf = GdkPixbuf::Pixbuf.new(fixture_path("gnome-logo-icon.png"),
                                                    32, 48)
        assert_equal(GdkPixbuf::Colorspace::RGB, pixbuf.colorspace)
        assert_equal(32, pixbuf.width)
        assert_equal(32, pixbuf.height)
      end

      test "file: scale" do
        pixbuf = GdkPixbuf::Pixbuf.new(fixture_path("gnome-logo-icon.png"),
                                                    32, 48, false)
        assert_equal(GdkPixbuf::Colorspace::RGB, pixbuf.colorspace)
        assert_equal(32, pixbuf.width)
        assert_equal(48, pixbuf.height)
      end

      test "subpixbuf" do
        src_pixbuf = GdkPixbuf::Pixbuf.new(fixture_path("gnome-logo-icon.png"))
        pixbuf = GdkPixbuf::Pixbuf.new(src_pixbuf, 0, 0, 32, 32)
        assert_equal(GdkPixbuf::Colorspace::RGB, pixbuf.colorspace)
        assert_equal(32, pixbuf.width)
        assert_equal(32, pixbuf.height)
      end

      test "data" do
        src_pixbuf = GdkPixbuf::Pixbuf.new(fixture_path("gnome-logo-icon.png"))
        data = src_pixbuf.pixels.pack("C*")
        pixbuf = GdkPixbuf::Pixbuf.new(data,
                                       src_pixbuf.colorspace,
                                       src_pixbuf.has_alpha?,
                                       src_pixbuf.bits_per_sample,
                                       src_pixbuf.width,
                                       src_pixbuf.height,
                                       src_pixbuf.rowstride,
                                       )
        assert_equal(src_pixbuf.pixels, pixbuf.pixels)
      end
    end

    sub_test_case("Hash form") do
      test "basic" do
        colorspace =  GdkPixbuf::Colorspace::RGB
        width = 100
        height = 100
        has_alpha = true
        bits_per_sample = 8
        pixbuf = GdkPixbuf::Pixbuf.new(:colorspace => colorspace,
                                       :has_alpha => has_alpha,
                                       :width => width,
                                       :height => height,
                                       :bits_per_sample => bits_per_sample)
        assert_equal(colorspace, pixbuf.colorspace)
        assert_equal(width, pixbuf.width)
        assert_equal(height, pixbuf.height)
        assert_equal(has_alpha, pixbuf.has_alpha?)
        assert_equal(bits_per_sample, pixbuf.bits_per_sample)
      end

      test "file" do
        filename = fixture_path("gnome-logo-icon.png")
        pixbuf = GdkPixbuf::Pixbuf.new(:file => filename)
        assert_equal(GdkPixbuf::Colorspace::RGB, pixbuf.colorspace)
      end

      test "xpm" do
        pixbuf = GdkPixbuf::Pixbuf.new(:xpm => r_xpm)
        assert_equal(GdkPixbuf::Colorspace::RGB, pixbuf.colorspace)
      end

      test "file: size" do
        filename = fixture_path("gnome-logo-icon.png")
        pixbuf = GdkPixbuf::Pixbuf.new(:file => filename,
                                       :width => 32,
                                       :height => 48)
        assert_equal(GdkPixbuf::Colorspace::RGB, pixbuf.colorspace)
        assert_equal(32, pixbuf.width)
        assert_equal(32, pixbuf.height)
      end

      test "file: scale" do
        filename = fixture_path("gnome-logo-icon.png")
        pixbuf = GdkPixbuf::Pixbuf.new(:file => filename,
                                       :width => 32,
                                       :height => 48,
                                       :scale => true,
                                       :preserve_aspect_ratio => false)
        assert_equal(GdkPixbuf::Colorspace::RGB, pixbuf.colorspace)
        assert_equal(32, pixbuf.width)
        assert_equal(48, pixbuf.height)
      end

      test "subpixbuf" do
        src_pixbuf = GdkPixbuf::Pixbuf.new(fixture_path("gnome-logo-icon.png"))
        pixbuf = GdkPixbuf::Pixbuf.new(:src_pixbuf => src_pixbuf,
                                       :src_x => 0,
                                       :src_y => 0,
                                       :width => 32,
                                       :height => 32)
        assert_equal(GdkPixbuf::Colorspace::RGB, pixbuf.colorspace)
        assert_equal(32, pixbuf.width)
        assert_equal(32, pixbuf.height)
      end

      test "data" do
        src_pixbuf = GdkPixbuf::Pixbuf.new(fixture_path("gnome-logo-icon.png"))
        data = src_pixbuf.pixels.pack("C*")
        pixbuf = GdkPixbuf::Pixbuf.new(:data => data,
                                       :colorspace => src_pixbuf.colorspace,
                                       :has_alpha => src_pixbuf.has_alpha?,
                                       :bits_per_sample => src_pixbuf.bits_per_sample,
                                       :width => src_pixbuf.width,
                                       :height => src_pixbuf.height,
                                       :row_stride => src_pixbuf.rowstride,
                                       )
        assert_equal(src_pixbuf.pixels, pixbuf.pixels)
      end

      test "bytes" do
        src_pixbuf = GdkPixbuf::Pixbuf.new(fixture_path("gnome-logo-icon.png"))
        bytes = src_pixbuf.pixels.flatten.pack("C*")
        pixbuf = GdkPixbuf::Pixbuf.new(:bytes => bytes,
                                       :colorspace => src_pixbuf.colorspace,
                                       :has_alpha => src_pixbuf.has_alpha?,
                                       :bits_per_sample => src_pixbuf.bits_per_sample,
                                       :width => src_pixbuf.width,
                                       :height => src_pixbuf.height,
                                       :row_stride => src_pixbuf.rowstride,
                                       )
        assert_equal(src_pixbuf.pixels, pixbuf.pixels)
      end
    end

    def test_new_subpixbuf
      src_pixbuf = GdkPixbuf::Pixbuf.new(fixture_path("gnome-logo-icon.png"))
      pixbuf = src_pixbuf.new_subpixbuf(0, 0, 32, 32)
      assert_equal(GdkPixbuf::Colorspace::RGB, pixbuf.colorspace)
      assert_equal(32, pixbuf.width)
      assert_equal(32, pixbuf.height)
    end
  end

  def test_dup
    pixbuf = GdkPixbuf::Pixbuf.new(fixture_path("gnome-logo-icon.png"))
    assert_equal(pixbuf.pixels, pixbuf.dup.pixels)
  end

  def test_fill!
    filename = fixture_path("gnome-logo-icon.png")
    pixbuf = GdkPixbuf::Pixbuf.new(:file => filename)
    pixbuf.fill!(0xffffffff)
    assert_equal([0xff] * (pixbuf.rowstride * pixbuf.height),
                 pixbuf.pixels)
  end
end
