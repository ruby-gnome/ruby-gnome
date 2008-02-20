class TestElementFactory < Test::Unit::TestCase
  include GstTestUtils

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
    assert_match(/\AWim Taymans <.*?>\z/, play_bin.author)
  end

  def test_create
    assert_match(/\Aplaybin\d+\z/, play_bin.create.name)
  end

  private
  def play_bin
    find_element_factory("playbin")
  end
end
