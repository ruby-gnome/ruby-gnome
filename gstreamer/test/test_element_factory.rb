class TestElementFactory < Test::Unit::TestCase
  def test_long_name
    assert_equal("Player Bin", play_bin.long_name)
  end

  def test_klass
    assert_equal("Generic/Bin/Player", play_bin.klass)
  end

  def test_description
    assert_equal("Autoplug and play media from an uri", play_bin.description)
  end

  def test_author
    assert_equal("Wim Taymans <wim@fluendo.com>", play_bin.author)
  end

  def test_create
    play_bin = Gst::ElementFactory.find("playbin")
    assert_not_nil(play_bin)
    assert_equal("playbin0", play_bin.create.name)
  end

  private
  def play_bin
    play_bin = Gst::ElementFactory.find("playbin")
    assert_not_nil(play_bin)
    play_bin
  end
end
