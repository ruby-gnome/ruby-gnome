class TestEvent < Test::Unit::TestCase
  include GstTestUtils

  def test_type
    assert_operator(Gst, :const_defined?, :EventType)
    assert_nothing_raised do
      Gst::Event::EOS
    end
  end
end
