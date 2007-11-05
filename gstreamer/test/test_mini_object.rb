class TestMiniObject < Test::Unit::TestCase
  def test_create_mini_object
    assert_raise(TypeError) do
      Gst::MiniObject.new
    end
  end
end
