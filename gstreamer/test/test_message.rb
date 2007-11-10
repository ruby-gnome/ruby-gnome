class TestMessage < Test::Unit::TestCase
  def test_create_message
    assert_nothing_raised do
      Gst::Message.new(:unknown, nil, nil)
    end
  end

  def test_create_message_eos
    assert_nothing_raised do
      Gst::MessageEos.new(nil)
    end
  end

  def test_create_message_error
    return "FIXME" if true
    assert_nothing_raised do
      Gst::MessageError.new(nil)
    end
  end

  def test_create_message_warning
    return "FIXME" if true
    assert_nothing_raised do
      Gst::MessageWarning.new(nil)
    end
  end

  def test_create_message_info
    return "FIXME" if true
    assert_nothing_raised do
      Gst::MessageInfo.new(nil)
    end
  end

  def test_create_message_tag
    return "FIXME" if true
    assert_nothing_raised do
      Gst::MessageTag.new(nil)
    end
  end

  def test_create_message_buffering
    assert_nothing_raised do
      Gst::MessageBuffering.new(nil, 50)
    end
  end

  def test_create_message_state_changed
    assert_nothing_raised do
      Gst::MessageStateChanged.new(nil, :paused, :playing, :void_pending)
    end
  end

  def test_create_message_state_dirty
    assert_nothing_raised do
      Gst::MessageStateDirty.new(nil)
    end
  end

  def test_create_message_clock_provide
    assert_nothing_raised do
      Gst::MessageClockProvide.new(nil, Gst::SystemClock.obtain)
    end
  end

  def test_create_message_clock_lost
    assert_nothing_raised do
      Gst::MessageClockLost.new(nil, Gst::SystemClock.obtain)
    end
  end

  def test_create_message_new_clock
    assert_nothing_raised do
      Gst::MessageNewClock.new(nil, Gst::SystemClock.obtain)
    end
  end

  def test_create_message_application
    assert_nothing_raised do
      Gst::MessageApplication.new(nil, {})
    end
  end

  def test_create_message_element
    assert_nothing_raised do
      Gst::MessageElement.new(nil, {})
    end
  end

  def test_create_message_segment_start
    assert_nothing_raised do
      Gst::MessageSegmentStart.new(nil, :default, 100)
    end
  end

  def test_create_message_segment_done
    assert_nothing_raised do
      Gst::MessageSegmentDone.new(nil, :default, 100)
    end
  end

  def test_create_message_duration
    assert_nothing_raised do
      Gst::MessageDuration.new(nil, :default, 10)
    end
  end

  def test_create_message_async_start
    assert_nothing_raised do
      Gst::MessageAsyncStart.new(nil, true)
    end
  end

  def test_create_message_async_done
    assert_nothing_raised do
      Gst::MessageAsyncDone.new(nil)
    end
  end

  def test_create_message_latency
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

    message = Gst::Message.new(:unknown, nil, {})
    assert_equal({}, message.structure)
  end
end
