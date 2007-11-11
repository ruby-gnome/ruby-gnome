class TestElementFactory < Test::Unit::TestCase
  def test_long_name
    play_bin = Gst::ElementFactory.find("playbin")
    assert_not_nil(play_bin)
    assert_equal("Player Bin", play_bin.long_name)
  end

  def test_create
    play_bin = Gst::ElementFactory.find("playbin")
    assert_not_nil(play_bin)
    assert_equal("playbin0", play_bin.create.name)
  end
end
