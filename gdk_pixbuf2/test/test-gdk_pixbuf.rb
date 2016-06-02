class TestGdkPixbuf < Test::Unit::TestCase
  include GdkPixbufTestUtils

  sub_test_case(".new") do
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
    
    test "from_file" do
      pixbuf = GdkPixbuf::Pixbuf.new(:file => fixture_path("gnome-logo-icon.png"))
      assert_equal(GdkPixbuf::Colorspace::RGB, pixbuf.colorspace)
    end
  end
end
