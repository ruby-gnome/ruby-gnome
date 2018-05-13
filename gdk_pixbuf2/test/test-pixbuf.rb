# Copyright (C) 2016-2018  Ruby-GNOME2 Project Team
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
      setup do |&block|
        suppress_warning do
          block.call
        end
      end

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
                                       src_pixbuf.row_stride,
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
        filename = fixture_path("gnome-logo-icon.png")
        src_pixbuf = GdkPixbuf::Pixbuf.new(:file => filename)
        pixbuf = suppress_warning do
          GdkPixbuf::Pixbuf.new(:src_pixbuf => src_pixbuf,
                                :src_x => 0,
                                :src_y => 0,
                                :width => 32,
                                :height => 32)
        end
        assert_equal(GdkPixbuf::Colorspace::RGB, pixbuf.colorspace)
        assert_equal(32, pixbuf.width)
        assert_equal(32, pixbuf.height)
      end

      test "data" do
        filename = fixture_path("gnome-logo-icon.png")
        src_pixbuf = GdkPixbuf::Pixbuf.new(:file => filename)
        data = src_pixbuf.pixels.pack("C*")
        pixbuf = GdkPixbuf::Pixbuf.new(:data => data,
                                       :colorspace => src_pixbuf.colorspace,
                                       :has_alpha => src_pixbuf.has_alpha?,
                                       :bits_per_sample => src_pixbuf.bits_per_sample,
                                       :width => src_pixbuf.width,
                                       :height => src_pixbuf.height,
                                       :row_stride => src_pixbuf.row_stride,
                                       )
        assert_equal(src_pixbuf.pixels, pixbuf.pixels)
      end

      test "bytes" do
        only_version(2, 32)
        filename = fixture_path("gnome-logo-icon.png")
        src_pixbuf = GdkPixbuf::Pixbuf.new(:file => filename)
        bytes = src_pixbuf.pixels.flatten.pack("C*")
        pixbuf = GdkPixbuf::Pixbuf.new(:bytes => bytes,
                                       :colorspace => src_pixbuf.colorspace,
                                       :has_alpha => src_pixbuf.has_alpha?,
                                       :bits_per_sample => src_pixbuf.bits_per_sample,
                                       :width => src_pixbuf.width,
                                       :height => src_pixbuf.height,
                                       :row_stride => src_pixbuf.row_stride,
                                       )
        assert_equal(src_pixbuf.pixels, pixbuf.pixels)
      end

      test "resource" do
        filename = fixture_path("gnome-logo-icon.png")
        src_pixbuf = GdkPixbuf::Pixbuf.new(:file => filename)
        resource = Gio::Resource.load(fixture_path("image.gresource"))
        Gio::Resources.register(resource)
        begin
          resource_path = "/org/ruby/gnome/gnome-logo-icon.png"
          pixbuf = GdkPixbuf::Pixbuf.new(:resource => resource_path)
          assert_equal(src_pixbuf.pixels, pixbuf.pixels)
        ensure
          Gio::Resources.unregister(resource)
        end
      end

      test "resource: scale" do
        resource = Gio::Resource.load(fixture_path("image.gresource"))
        Gio::Resources.register(resource)
        begin
          resource_path = "/org/ruby/gnome/gnome-logo-icon.png"
          pixbuf = GdkPixbuf::Pixbuf.new(:resource => resource_path,
                                         :width => 32,
                                         :height => 48,
                                         :scale => true,
                                         :preserve_aspect_ratio => false)
          assert_equal([
                         GdkPixbuf::Colorspace::RGB,
                         32,
                         48,
                       ],
                       [
                         pixbuf.colorspace,
                         pixbuf.width,
                         pixbuf.height,
                       ])
        ensure
          Gio::Resources.unregister(resource)
        end
      end
    end

    def test_subpixbuf
      filename = fixture_path("gnome-logo-icon.png")
      src_pixbuf = GdkPixbuf::Pixbuf.new(:file => filename)
      pixbuf = src_pixbuf.subpixbuf(0, 0, 32, 32)
      assert_equal(GdkPixbuf::Colorspace::RGB, pixbuf.colorspace)
      assert_equal(32, pixbuf.width)
      assert_equal(32, pixbuf.height)
    end
  end

  def test_dup
    filename = fixture_path("gnome-logo-icon.png")
    pixbuf = GdkPixbuf::Pixbuf.new(:file => filename)
    assert_equal(pixbuf.pixels, pixbuf.dup.pixels)
  end

  def test_fill!
    filename = fixture_path("gnome-logo-icon.png")
    pixbuf = GdkPixbuf::Pixbuf.new(:file => filename)
    pixbuf.fill!(0xffffffff)
    assert_equal([0xff] * (pixbuf.row_stride * pixbuf.height),
                 pixbuf.pixels)
  end

  def test_rotate
    filename = fixture_path("gnome-logo-icon.png")
    pixbuf = GdkPixbuf::Pixbuf.new(:file => filename)
    inverted_pixbuf = pixbuf.rotate(:upsidedown)
    assert_not_equal(pixbuf.pixels, inverted_pixbuf.pixels)
    inverted_twice_pixbuf = inverted_pixbuf.rotate(:upsidedown)
    assert_equal(pixbuf.pixels, inverted_twice_pixbuf.pixels)
  end

  sub_test_case("saturate_and_pixelate") do
    # Based on the function gdk_pixbuf_saturate_and_pixelate
    # in gdk-pixbuf-util.c
    def saturate_and_pixelate_pixels(pixbuf, saturation, pixelate)
      alpha = pixbuf.has_alpha?
      width = pixbuf.width
      height = pixbuf.height
      pixels = pixbuf.pixels
      dest_pix = []
      dark_factor = 0.7
      height.times do |i|
        width.times do |k|
          j =((i ) * width + k) * 4
          intens = intensity(pixels[j + 0], pixels[j + 1], pixels[j + 2])
          if pixelate == true && ( (i + k) % 2 == 0)
            dest_pix << (intens / 2 + 127).floor
            dest_pix << (intens / 2 + 127).floor
            dest_pix << (intens / 2 + 127).floor
          elsif pixelate == true
            dest_pix << clamp_uchar(saturate(pixels[j + 0],
                                    saturation, intens) * dark_factor).floor
            dest_pix << clamp_uchar(saturate(pixels[j + 1],
                                    saturation, intens) * dark_factor).floor
            dest_pix << clamp_uchar(saturate(pixels[j + 2],
                                    saturation, intens) * dark_factor).floor
          else
            dest_pix << clamp_uchar(saturate(pixels[j + 0], saturation, intens)).floor
            dest_pix << clamp_uchar(saturate(pixels[j + 1], saturation, intens)).floor
            dest_pix << clamp_uchar(saturate(pixels[j + 2], saturation, intens)).floor
          end
          dest_pix << pixels[j + 3] if alpha
        end
      end
      dest_pix
    end

    def saturate(value, saturation, intensity)
      (1.0 - saturation) * intensity + saturation * (1.0 * value)
    end

    def clamp_uchar(x)
      [0, x, 255].sort[1]
    end

    def intensity(r, g, b)
      (r * 0.30 + g * 0.59 + b * 0.11).floor
    end

    test "no modifications" do
      filename = fixture_path("gnome-logo-icon.png")
      src_pixbuf = GdkPixbuf::Pixbuf.new(:file => filename)
      pixbuf = src_pixbuf.saturate_and_pixelate(1, false)
      assert_equal(src_pixbuf.pixels, pixbuf.pixels)
    end

    test "normal usage" do
      if /\Ai\d86-/ === RUBY_PLATFORM
        omit("floating point calculation result is different on i386")
      end
      filename = fixture_path("gnome-logo-icon.png")
      src_pixbuf = GdkPixbuf::Pixbuf.new(:file => filename)
      pixbuf = src_pixbuf.saturate_and_pixelate(0, true)
      ref = saturate_and_pixelate_pixels(src_pixbuf, 0, true)
      assert_equal(ref, pixbuf.pixels, ref.size)
      pixbuf = src_pixbuf.saturate_and_pixelate(0.5, true)
      ref = saturate_and_pixelate_pixels(src_pixbuf, 0.5, true)
      assert_equal(ref, pixbuf.pixels)
    end
  end

  sub_test_case("#save") do
    test("no options") do
      png_filename = fixture_path("gnome-logo-icon.png")
      pixbuf = GdkPixbuf::Pixbuf.new(:file => png_filename)
      jpeg = pixbuf.save("jpeg")
      assert_equal(["image/jpeg", false],
                   Gio::ContentType.guess(nil, jpeg))
    end

    test("filename") do
      png_filename = fixture_path("gnome-logo-icon.png")
      pixbuf = GdkPixbuf::Pixbuf.new(:file => png_filename)
      output = Tempfile.new(["pixbuf", ".jpg"])
      pixbuf.save(output)
      assert_equal(["image/jpeg", false],
                   Gio::ContentType.guess(nil, output.read))
    end

    test(":filename") do
      png_filename = fixture_path("gnome-logo-icon.png")
      pixbuf = GdkPixbuf::Pixbuf.new(:file => png_filename)
      output = Tempfile.new(["pixbuf", ".jpeg"])
      pixbuf.save(output.path, "jpeg")
      assert_equal(["image/jpeg", false],
                   Gio::ContentType.guess(nil, output.read))
    end
  end

  test("#save_to_buffer") do
    png_filename = fixture_path("gnome-logo-icon.png")
    pixbuf = GdkPixbuf::Pixbuf.new(:file => png_filename)
    jpeg = suppress_warning do
      pixbuf.save_to_buffer("jpeg")
    end
    assert_equal(["image/jpeg", false],
                 Gio::ContentType.guess(nil, jpeg))
  end

  test("#read_pixel_bytes") do
    only_version(2, 32)
    png_filename = fixture_path("gnome-logo-icon.png")
    pixbuf = GdkPixbuf::Pixbuf.new(:file => png_filename)
    assert_equal(pixbuf.pixels.pack("C*"),
                 pixbuf.read_pixel_bytes.to_s)
  end
end
