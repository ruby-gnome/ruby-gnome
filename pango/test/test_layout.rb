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
end
