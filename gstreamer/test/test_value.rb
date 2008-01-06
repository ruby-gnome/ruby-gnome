class TestValue < Test::Unit::TestCase
  def test_list
    assert_equal([1234, 4321], audio_convert_caps[0]["endianness"])
  end

  def test_int_range
    range = Gst::IntRange.new(10, 100)
    assert_equal(10, range.min)
    assert_equal(100, range.max)
    assert_equal("[10,100]", range.to_s)

    range.min = 1
    range.max = 500
    assert_equal(1, range.min)
    assert_equal(500, range.max)
    assert_equal("[1,500]", range.to_s)

    range.set(100, 9999)
    assert_equal(100, range.min)
    assert_equal(9999, range.max)
    assert_equal("[100,9999]", range.to_s)
  end

  private
  def audio_convert_caps
    convert = Gst::ElementFactory.find("audioconvert")
    assert_not_nil(convert)
    convert.pad_templates.find do |template|
      template.direction == Gst::Pad::SRC
    end.caps.to_caps
  end
end

