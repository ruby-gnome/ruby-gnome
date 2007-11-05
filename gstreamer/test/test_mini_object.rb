class TestMiniObject < Test::Unit::TestCase
  def test_create_mini_object
    assert_raise(TypeError) do
      Gst::MiniObject.new
    end

    assert_operator(Gst::MiniObject, :>, Gst::Message)
    assert_nothing_raised do
      Gst::Message.new
    end
  end
end
