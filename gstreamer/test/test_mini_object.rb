class TestMiniObject < Test::Unit::TestCase
  def test_create_mini_object
    assert_raise(TypeError) do
      Gst::MiniObject.new
    end

    assert_operator(Gst::MiniObject, :>, Gst::Buffer)
    assert_nothing_raised do
      Gst::Buffer.new
    end
  end

  def test_writable
    mini_object = Gst::Buffer.new
    assert(mini_object.writable?)
    mini_object.flag = :readonly
    assert(!mini_object.writable?)
    writable = mini_object.make_writable
    assert(writable.writable?)
  end
end
