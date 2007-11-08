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
    mini_object.flags = :readonly
    assert(!mini_object.writable?)
    writable = mini_object.make_writable
    assert(writable.writable?)
  end

  def test_flag
    mini_object = Gst::Buffer.new

    assert_equal(0, mini_object.flags)
    assert(!mini_object.flag_raised?(:readonly))

    mini_object.raise_flag(:readonly)
    assert(mini_object.flag_raised?(:readonly))
    mini_object.lower_flag(:readonly)
    assert(!mini_object.flag_raised?(:readonly))

    mini_object.raise_flag(:readonly)
    assert(mini_object.flag_raised?(:readonly))
    mini_object.flags = 0
    assert(!mini_object.flag_raised?(:readonly))
  end
end
