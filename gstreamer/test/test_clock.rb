class TestClock < Test::Unit::TestCase
  def test_create_clock
    assert_raise(TypeError) do
      Gst::Clock.new
    end
  end
end
