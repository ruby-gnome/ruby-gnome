class TestLayout < Test::Unit::TestCase
  include PangoTestUtils

  def setup
    @context = Pango::Context.new
    @layout = Pango::Layout.new(@context)
  end

  def test_set_font_description
    assert_nil(@layout.font_description)

    @layout.font_description = "sans 14"
    assert_equal("sans 14", @layout.font_description.to_s)

    description = Pango::FontDescription.new("monospace")
    description.size = 10 * Pango::SCALE
    @layout.font_description = description
    assert_equal("monospace 10", @layout.font_description.to_s)
  end

  def test_height
    assert_equal(-1, @layout.height)
  end

  def test_set_height
    new_height = 29 * Pango::SCALE
    @layout.height = new_height
    assert_equal(new_height, @layout.height)
  end

  def test_ellipsized?
    assert_false(@layout.ellipsized?)
  end
end
