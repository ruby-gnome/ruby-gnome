class TestAttribute < Test::Unit::TestCase
  include PangoTestUtils

  def setup
    @attribute = Pango::AttrLanguage.new(Pango::Language.default)
  end

  def test_start_index
    assert_equal(0, @attribute.start_index)
    @attribute.start_index = 5
    assert_equal(5, @attribute.start_index)
  end

  def test_end_index
    assert_equal(GLib::MAXUINT, @attribute.end_index)
    @attribute.end_index = 5
    assert_equal(5, @attribute.end_index)
  end
end
