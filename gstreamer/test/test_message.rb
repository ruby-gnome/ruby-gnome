class TestMessage < Test::Unit::TestCase
  def test_create_message
    assert_nothing_raised do
      Gst::Message.new(:unknown, nil, nil)
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
