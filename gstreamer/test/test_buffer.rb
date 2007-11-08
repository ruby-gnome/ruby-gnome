class TestBuffer < Test::Unit::TestCase
  def test_create_mini_object
    assert_nothing_raised do
      Gst::Buffer.new
    end
  end

  def test_flag
    buffer = Gst::Buffer.new

    assert_nothing_raised do
      buffer.raise_flag(:preroll)
    end
    assert(!buffer.flag_raised?(:discont))
    assert_nothing_raised do
      buffer.lower_flag(:in_caps)
    end

    buffer.flags = [:gap, :delta_unit]
    assert_equal(Gst::Buffer::FLAG_GAP | Gst::Buffer::FLAG_DELTA_UNIT,
                 buffer.flags)
  end

  def test_size
    assert_equal(0, Gst::Buffer.new.size)
    assert_equal(5, Gst::Buffer.new(5).size)
  end
end
