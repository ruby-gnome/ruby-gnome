class TestValue < Test::Unit::TestCase
  def test_list
    assert_equal([1234, 4321], audio_convert_caps[0]["endianness"])
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

