class TestObject < Test::Unit::TestCase
  include GstTestUtils

  def test_floating
    sink = create_element("fakesink")
    assert(sink.floating?)
    sink.sink
    assert(!sink.floating?)
  end

  def test_name
    sink = create_element("fakesink", "my-fakesink")
    assert_equal("my-fakesink", sink.name)
    sink.name += "0"
    assert_equal("my-fakesink0", sink.name)
  end
end
