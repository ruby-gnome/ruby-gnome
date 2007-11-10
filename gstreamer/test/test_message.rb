require 'timeout'

class TestMessage < Test::Unit::TestCase
  def test_create_message
    assert_nothing_raised do
      Gst::Message.new(Gst::Message::UNKNOWN, nil, nil)
    end
  end

  def test_type
    message = Gst::Message.new(Gst::Message::UNKNOWN, nil, nil)
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
end
