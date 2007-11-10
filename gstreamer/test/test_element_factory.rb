class TestElementFactory < Test::Unit::TestCase
  def test_long_name
    play_bin = Gst::ElementFactory.find("playbin")
    assert_not_nil(play_bin)
    assert_equal("Player Bin", play_bin.long_name)
  end
end
