class TestMessage < Test::Unit::TestCase
  def test_create_message
    assert_nothing_raised do
      Gst::Message.new(:unknown, nil, nil)
    end
  end

  def test_message_eos
    assert_nothing_raised do
      Gst::MessageEos.new(nil)
    end
  end

  def test_message_error
    return "FIXME" if true
    error = nil
    debug = "ERROR"
    message = Gst::MessageError.new(nil, error, debug)
    assert_equal([error, debug], message.parse)
  end

  def test_message_warning
    return "FIXME" if true
    error = nil
    debug = "WARNING"
    message = Gst::MessageWarning.new(nil, error, debug)
    assert_equal([error, debug], message.parse)
  end

  def test_message_info
    return "FIXME" if true
    error = nil
    debug = "INFO"
    message = Gst::MessageInfo.new(nil, error, debug)
    assert_equal([error, debug], message.parse)
  end

  def test_message_tag
    return "FIXME" if true
    tag_list = []
    message = Gst::MessageTag.new(nil, tag_list)
    assert_equal(tag_list, message.parse)
  end

  def test_message_buffering
    percent = 50
    message = Gst::MessageBuffering.new(nil, percent)
    assert_equal(percent, message.parse)
  end

  def test_message_state_changed
    old = Gst::STATE_PAUSED
    new = Gst::STATE_PLAYING
    pending = Gst::STATE_VOID_PENDING
    message = Gst::MessageStateChanged.new(nil, :paused, :playing, :void_pending)
    assert_equal([old, new, pending], message.parse)
  end

  def test_message_state_dirty
    assert_nothing_raised do
      Gst::MessageStateDirty.new(nil)
    end
  end

  def test_message_clock_provide
    clock = Gst::SystemClock.obtain
    ready = true
    message = Gst::MessageClockProvide.new(nil, clock, ready)
    assert_equal([clock, ready], message.parse)
  end

  def test_message_clock_lost
    clock = Gst::SystemClock.obtain
    message = Gst::MessageClockLost.new(nil, clock)
    assert_equal(clock, message.parse)
  end

  def test_create_message_new_clock
    clock = Gst::SystemClock.obtain
    message = Gst::MessageNewClock.new(nil, clock)
    assert_equal(clock, message.parse)
  end

  def test_message_application
    assert_nothing_raised do
      Gst::MessageApplication.new(nil, nil)
    end
  end

  def test_message_element
    assert_nothing_raised do
      Gst::MessageElement.new(nil, nil)
    end
  end

  def test_message_segment_start
    format = Gst::Format::DEFAULT
    position = 100
    message = Gst::MessageSegmentStart.new(nil, :default, position)
    assert_equal([format, position], message.parse)
  end

  def test_message_segment_done
    format = Gst::Format::DEFAULT
    position = 100
    message = Gst::MessageSegmentDone.new(nil, :default, position)
    assert_equal([format, position], message.parse)
  end

  def test_message_duration
    format = Gst::Format::DEFAULT
    duration = 10
    message = Gst::MessageDuration.new(nil, :default, duration)
    assert_equal([format, duration], message.parse)
  end

  def test_message_async_start
    new_base_time = true
    message = Gst::MessageAsyncStart.new(nil, new_base_time)
    assert_equal(new_base_time, message.parse)
  end

  def test_message_async_done
    assert_nothing_raised do
      Gst::MessageAsyncDone.new(nil)
    end
  end

  def test_message_latency
    assert_nothing_raised do
      Gst::MessageLatency.new(nil)
    end
  end

  def test_type
    message = Gst::Message.new(:unknown, nil, nil)
    assert_equal(Gst::Message::UNKNOWN, message.type)
    message.type = :tag
    assert_equal(Gst::Message::TAG, message.type)
    assert_equal("tag", message.type.name)
  end

  def test_lock
    message = Gst::MessageEos.new(nil)
    assert(!message.have_mutex?)
  end

  def test_cond
    message = Gst::MessageEos.new(nil)
    assert(!message.have_cond?)
  end

  def test_timestamp
    message = Gst::MessageEos.new(nil)
    assert(!message.valid_timestamp?)

    timestamp = Time.now.to_i * Gst::NSECOND
    message.timestamp = timestamp
    assert_equal(timestamp, message.timestamp)
    assert(message.valid_timestamp?)
  end

  def test_source
    assert_nil(Gst::MessageEos.new(nil).source)

    source = Gst::XML.new
    message = Gst::MessageEos.new(source)
    assert_equal(source, message.source)
    message.source = nil
    assert_nil(message.source)
  end

  def test_structure
    assert_nil(Gst::MessageEos.new(nil).structure)

    message = Gst::Message.new(:unknown, nil, nil)
    assert_nil(message.structure)
  end
end
