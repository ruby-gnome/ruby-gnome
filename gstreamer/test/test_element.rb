class TestElement < Test::Unit::TestCase
  include GstTestUtils

  def test_clock
    bin = create_element("playbin")
    assert(bin.require_clock?)
    assert(bin.provide_clock?)
    assert_nil(bin.clock)
  end

  def test_each_pad
    pads = []
    create_element("filesrc").each_pad do |pad|
      pads << pad.name
    end
    assert_equal(["src"], pads)
  end
end

