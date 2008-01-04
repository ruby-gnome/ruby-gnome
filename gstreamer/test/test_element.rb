class TestElement < Test::Unit::TestCase
  def test_clock
    bin = play_bin
    assert(bin.require_clock?)
    assert(bin.provide_clock?)
    assert_nil(bin.clock)
  end

  private
  def play_bin
    play_bin = Gst::ElementFactory.find("playbin")
    assert_not_nil(play_bin)
    play_bin.create
  end
end

