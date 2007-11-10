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
    buffer = Gst::Buffer.new(5)
    assert_equal(5, buffer.size)
    buffer.size = 3
    assert_equal(3, buffer.size)
  end

  def test_data
    buffer = Gst::Buffer.new
    assert_equal([nil, 0], [buffer.data, buffer.size])
    buffer.data = "XXX"
    assert_equal(["XXX", 3], [buffer.data, buffer.size])
    buffer.data = nil
    assert_equal([nil, 0], [buffer.data, buffer.size])
  end

  def test_timestamp
    buffer = Gst::Buffer.new
    assert_equal(Gst::ClockTime::NONE, buffer.timestamp)
    assert(!buffer.valid_timestamp?)

    timestamp = Time.now.to_i * Gst::NSECOND
    buffer.timestamp = timestamp
    assert_equal(timestamp, buffer.timestamp)
    assert(buffer.valid_timestamp?)
  end

  def test_duration
    buffer = Gst::Buffer.new
    assert_equal(Gst::ClockTime::NONE, buffer.duration)
    assert(!buffer.valid_duration?)

    duration = 10 * 60 * Gst::NSECOND
    buffer.duration = duration
    assert_equal(duration, buffer.duration)
    assert(buffer.valid_duration?)
  end

  def test_caps
    buffer = Gst::Buffer.new
    assert_nil(buffer.caps)

    caps = Gst::Caps.new
    buffer.caps = caps
    assert_equal(caps, buffer.caps)
  end

  def test_offset
    buffer = Gst::Buffer.new
    assert_equal(Gst::Buffer::OFFSET_NONE, buffer.offset)
    assert(!buffer.valid_offset?)

    offset = 10
    buffer.offset = offset
    assert_equal(offset, buffer.offset)
    assert(buffer.valid_offset?)
  end

  def test_offset_end
    buffer = Gst::Buffer.new
    assert_equal(Gst::Buffer::OFFSET_NONE, buffer.offset_end)
    assert(!buffer.valid_offset_end?)

    offset_end = 10
    buffer.offset_end = offset_end
    assert_equal(offset_end, buffer.offset_end)
    assert(buffer.valid_offset_end?)
  end

  def test_discontinuity
    buffer = Gst::Buffer.new
    assert(!buffer.discontinuity?)

    buffer.raise_flag(:discont)
    assert(buffer.discontinuity?)
  end

  def test_copy_flags
    assert_equal(Gst::Buffer::CopyFlags::FLAGS |
                 Gst::Buffer::CopyFlags::TIMESTAMPS |
                 Gst::Buffer::CopyFlags::CAPS,
                 Gst::Buffer::CopyFlags::ALL)

    assert_equal(Gst::Buffer::COPY_FLAGS |
                 Gst::Buffer::COPY_TIMESTAMPS |
                 Gst::Buffer::COPY_CAPS,
                 Gst::Buffer::COPY_ALL)
  end

  def test_copy_metadata
    buffer = Gst::Buffer.new
    buffer.flags = :discont

    copied = buffer.copy_metadata(:all)
    assert(copied.flag_raised?(:discont))
    copied = buffer.copy_metadata(0)
    assert(!copied.flag_raised?(:discont))

    copied = Gst::Buffer.new
    assert(!copied.flag_raised?(:discont))
    buffer.copy_metadata(copied, :flags)
    assert(copied.flag_raised?(:discont))
  end

  def test_copy_metadata_with_readonly
    buffer = Gst::Buffer.new
    buffer.flags = :readonly
    assert(!buffer.writable?)

    copied = buffer.copy_metadata(:all)
    assert(!copied.flag_raised?(:readonly))
    assert(copied.writable?)
  end

  def test_metadata_writable
    buffer = Gst::Buffer.new
    assert(buffer.metadata_writable?)
    buffer.metadata_writable!
    # assert(buffer.metadata_writable?) # FIXME

    buffer = Gst::Buffer.new
    buffer.create_sub(0, 0)
    assert(!buffer.metadata_writable?)
    buffer.metadata_writable!
    # assert(buffer.metadata_writable?) # FIXME

    buffer = Gst::Buffer.new
    sub = buffer.create_sub(0, 0)
    assert(!sub.metadata_writable?)
    sub.metadata_writable!
    # assert(sub.metadata_writable?) # FIXME
  end
end
