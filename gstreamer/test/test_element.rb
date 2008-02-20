class TestElement < Test::Unit::TestCase
  include GstTestUtils

  def test_clock
    bin = create_element("playbin")
    assert(bin.require_clock?)
    assert(bin.provide_clock?)
    assert_nil(bin.clock)
  end
end

