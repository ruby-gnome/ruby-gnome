class TestBin < Test::Unit::TestCase
  include GstTestUtils

  def test_children
    bin = Gst::Bin.new

    element = create_element("filesink")
    no_interface_element = create_element("fakesink")
    bin << element << no_interface_element

    assert_equal([no_interface_element, element], bin.children)
    assert_equal([element], bin.children(Gst::ElementURIHandler))
  end

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

  def test_clear
    bin = Gst::Bin.new
    element1 = create_element("fakesink")
    element2 = create_element("fakesink")
    bin << element1 << element2
    assert_equal(2, bin.size)
    bin.clear
    assert_equal(0, bin.size)
  end

  def test_refer_by_name
    bin = Gst::Bin.new
    sub_bin = Gst::Bin.new
    bin << sub_bin

    element = create_element("fakesink")
    assert_nil(bin.get_child(element.name))
    assert_nil(sub_bin.get_child(element.name))
    assert_nil(sub_bin.get_child(element.name, true))

    bin << element
    assert_equal(element, bin.get_child(element.name))
    assert_equal(element, bin.get_child(element.name, true))
    assert_nil(sub_bin.get_child(element.name))
    assert_equal(element, sub_bin.get_child(element.name, true))
  end

  def test_refer_by_interface
    bin = Gst::Bin.new
    element = create_element("filesink")

    assert_nil(bin[Gst::ElementURIHandler])

    bin << create_element("fakesink")
    assert_nil(bin[Gst::ElementURIHandler])

    bin << element
    assert_equal(element, bin[Gst::ElementURIHandler])
  end

  def test_refer_by_index
    bin = Gst::Bin.new
    element1 = create_element("filesink")
    element2 = create_element("fakesink")

    assert_nil(bin.get_child(0))
    assert_nil(bin.get_child(1))
    assert_nil(bin.get_child(2))

    bin << element1 << element2
    assert_equal(element2, bin.get_child(0))
    assert_equal(element1, bin.get_child(1))
    assert_nil(bin.get_child(2))
  end

  def test_sinks
    bin = Gst::Bin.new
    sink1 = create_element("filesink")
    sink2 = create_element("fakesink")
    src = create_element("fakesrc")

    assert_equal([], bin.sinks)

    bin << src << sink1 << sink2
    assert_equal([sink2, sink1], bin.sinks)
  end

  def test_sources
    bin = Gst::Bin.new
    src1 = create_element("filesrc")
    src2 = create_element("fakesrc")
    sink = create_element("fakesink")

    assert_equal([], bin.sources)

    bin << src1 << src2 << sink
    assert_equal([src2, src1], bin.sources)
  end
end
