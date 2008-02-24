class TestBin < Test::Unit::TestCase
  include GstTestUtils

  def test_size
    bin = Gst::Bin.new
    assert_equal(0, bin.size)
    bin << create_element("fakesink")
    assert_equal(1, bin.size)
  end

  def test_children_cookie
    bin = Gst::Bin.new
    cookie = bin.children_cookie
    assert_equal(cookie, bin.children_cookie)
    bin << create_element("fakesink")
    assert_not_equal(cookie, bin.children_cookie)
  end

  def test_child_bus
    bin = Gst::Bin.new
    assert_not_nil(bin.child_bus)
  end

  def test_messages
    bin = Gst::Bin.new
    assert_equal([], bin.messages)
  end

  def test_polling?
    bin = Gst::Bin.new
    assert(!bin.polling?)
  end

  def test_clock_dirty?
    bin = Gst::Bin.new
    assert(!bin.clock_dirty?)
  end

  def test_provided_clock
    bin = Gst::Bin.new
    assert_nil(bin.provided_clock)
    bin.provided_clock = Gst::SystemClock.new
    assert_not_nil(bin.provided_clock)
    bin.provided_clock = nil
    assert_nil(bin.provided_clock)
  end

  def test_clock_provider
    bin = Gst::Bin.new
    assert_nil(bin.clock_provider)
  end

  def test_add
    bin = Gst::Bin.new
    element = create_element("fakesink")
    bin << element
    assert_equal(element, bin.children[0])
    assert_raise(TypeError) do
      bin << "XXX"
    end

    element2 = create_element("fakesink")
    element3 = create_element("fakesink")
    bin.add(element2, element3)
    assert_equal([element3, element2], bin.children[0, 2])
  end

  def test_remove
    bin = Gst::Bin.new
    element1 = create_element("fakesink")
    element2 = create_element("fakesink")
    bin << element1 << element2
    assert_equal(2, bin.size)
    bin.remove(element1, element2)
    assert_equal(0, bin.size)
  end
end
