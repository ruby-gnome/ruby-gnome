class TestSeek < Test::Unit::TestCase
  include GstTestUtils

  def test_type
    assert_operator(Gst, :const_defined?, :SeekType)
    assert_nothing_raised do
      Gst::Seek::TYPE_NONE
    end
  end

  def test_flags
    assert_operator(Gst, :const_defined?, :SeekFlags)
    assert_nothing_raised do
      Gst::Seek::FLAG_NONE
    end
  end
end
