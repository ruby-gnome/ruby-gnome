class TestPluginFeature < Test::Unit::TestCase
  def test_load!
    feature = Gst::ElementFactory.find("audiorate")
    assert(!feature.loaded?)
    assert(feature.load!)
    assert(feature.loaded?)
  end

  def test_rank
    feature = Gst::ElementFactory.find("playbin")
    assert_equal(Gst::RANK_NONE, feature.rank)
    feature.rank = :primary
    assert_equal(Gst::RANK_PRIMARY, feature.rank)
  end

  def test_plugin_name
    feature = Gst::ElementFactory.find("playbin")
    assert_equal("playback", feature.plugin_name)
  end
end
