class TestBin < Test::Unit::TestCase
  include GstTestUtils

  def test_size
    bin = Gst::Bin.new
    assert_equal(0, bin.size)
    bin << create_element("fakesink")
    assert_equal(1, bin.size)
  end
end
