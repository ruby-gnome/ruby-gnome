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
end
