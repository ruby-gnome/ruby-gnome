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

  def test_fourcc
    fourcc = Gst::Fourcc.new("MJPG")
    assert_equal(string_to_fourcc("MJPG"), fourcc.to_i)
    assert_equal("MJPG", fourcc.to_s)

    fourcc.replace!(string_to_fourcc("abcd"))
    assert_equal(string_to_fourcc("abcd"), fourcc.to_i)
    assert_equal("abcd", fourcc.to_s)
  end

  def test_fraction_range_new
    assert_equal("[ 2/1, 3/4 ]",
                 Gst::FractionRange.new(Rational(2, 1), Rational(3, 4)).to_s)
  end

  def test_fraction_range
    assert_equal([Rational(0, 1), Rational(2147483647, 1)],
                 ffmpeg_color_space_caps[0]["framerate"].to_a)
  end

  private
  def audio_convert_caps
    convert = Gst::ElementFactory.find("audioconvert")
    assert_not_nil(convert)
    convert.pad_templates.find do |template|
      template.direction == Gst::Pad::SRC
    end.caps.to_caps
  end

  def ffmpeg_color_space_caps
    convert = Gst::ElementFactory.find("ffmpegcolorspace")
    assert_not_nil(convert)
    convert.pad_templates.find do |template|
      template.direction == Gst::Pad::SRC
    end.caps.to_caps
  end

  def string_to_fourcc(string)
    value = 0
    string.unpack("C4").each_with_index do |v, i|
      value += v << (i * 8)
    end
    value
  end
end

